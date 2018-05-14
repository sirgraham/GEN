
/*------------------------------------------------------------------------------------------
//  GRPELEMENT.CPP
//
//  Elements
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 30/05/2014 13:09:20
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XDebug.h"
#include "GRPFace.h"
#include "GRPObjectBuilder.h"
#include "GRPShader.h"
#include "GRPElement.h"
#include "GRPSkinning.h"
#include "GRPRenderer.h"
#include "GRPDeformer.h"
#include "GRPProfiler.h"
#include "GRPPass.h"
#include "GRPShaderLibrary.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  GRPELEMENT::COMPARATOR::Compare
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/10/2015 11:55:28
//
//  @return       int :
//
//  @param        test1 :
//  @param        test2 :
*/
/*-----------------------------------------------------------------*/
int GRPELEMENT::COMPARATOR::Compare(GRPELEMENT* test1,GRPELEMENT* test2)
{

  if (test1->renderpriority < test2->renderpriority)  return 1;
  if (test1->renderpriority > test2->renderpriority)  return -1;

  if (test1->GetIsLoaded() && test2->GetIsLoaded())
  {
    if (test1->GetMesh()->vertexVBOID > test2->GetMesh()->vertexVBOID)          return +1;
    if (test1->GetMesh()->vertexVBOID < test2->GetMesh()->vertexVBOID)          return -1;
  }

  if (test1->GetShader() && test2->GetShader())
  {
    if (test1->GetShader()->GetIndex()<test2->GetShader()->GetIndex()) return +1;
    if (test1->GetShader()->GetIndex()>test2->GetShader()->GetIndex()) return -1;
  }

  if (test2->GetNode() && test1->GetNode())
  {
    if (test1->GetNode()->GetMaterial()==NULL && test2->GetNode()->GetMaterial()!=NULL) return -1;
    if (test1->GetNode()->GetMaterial()!=NULL && test2->GetNode()->GetMaterial()==NULL) return 1;

    if (test1->GetNode()->GetMaterial() && test2->GetNode()->GetMaterial())
    {
      if (!test1->GetNode()->GetMaterial()->GetLayer(0) && test2->GetNode()->GetMaterial()->GetLayer(0))  return -1;
      if (test1->GetNode()->GetMaterial()->GetLayer(0) && !test2->GetNode()->GetMaterial()->GetLayer(0))  return 1;

      if (test1->GetNode()->GetMaterial()->GetLayer(0) && test2->GetNode()->GetMaterial()->GetLayer(0))
      {
        if (test1->GetNode()->GetMaterial()->GetLayer(0)->isloaded && test2->GetNode()->GetMaterial()->GetLayer(0)->isloaded)
        {
        if  (test1->GetNode()->GetMaterial()->GetLayer(0)->GetHandle() > test2->GetNode()->GetMaterial()->GetLayer(0)->GetHandle()) return -1;
        if (test1->GetNode()->GetMaterial()->GetLayer(0)->GetHandle() < test2->GetNode()->GetMaterial()->GetLayer(0)->GetHandle())  return +1;
        }
      }
    }
  }



  return -1;
}

/*-------------------------------------------------------------------
//  GRPELEMENT::GRPELEMENT
*/
/**
//
//  Class Constructor GRPELEMENT
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/05/2014 13:18:59
//
*/
/*-----------------------------------------------------------------*/
GRPELEMENT::GRPELEMENT(GLUINT tag)
{
  Clean();
  GRPNAMED::classtype = GRPNAMED_TYPE_GRPELEMENT;
}



/*-------------------------------------------------------------------
//  GRPELEMENT::~GRPELEMENT
*/
/**
//
//   Class Destructor GRPELEMENT
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/05/2014 13:19:05
//
*/
/*-----------------------------------------------------------------*/
GRPELEMENT::~GRPELEMENT()
{
  this->Publish(GRPMSG_DESTROYED);

  if(!isinstance)
    {
      morphs.DeleteContents();
      morphs.DeleteAll();
      if (mesh)
      delete mesh;
    }

    mesh=NULL;

    Unsubscribe();

      if (node!=NULL)
        this->Dettach();

    Clean();
}

/*-------------------------------------------------------------------
//  GRPELEMENT::SetMesh
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2016 14:42:48
//
//  @return       bool :
//
//  @param        mesh :
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::SetMesh(GRPMESH* mesh)
{
  if (!this->IsInstance())
    if (this->mesh)
      delete(this->mesh);

  this->mesh=mesh;
  return true;
}

/*-------------------------------------------------------------------
//  GRPELEMENT::GetMesh
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2016 14:27:04
//
//  @return       GRPMESH* :
//
//  @param        index_lod :
*/
/*-----------------------------------------------------------------*/
GRPMESH* GRPELEMENT::GetMesh(int index_lod)
{
  return this->mesh;
}

/*-------------------------------------------------------------------
//   GRPELEMENT::ChooseShader
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return   GRPSHADERPROGRAM* :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
GRPSHADERPROGRAM* GRPELEMENT::ChooseShader()
{
    if (this->node == NULL) return NULL;

    GRPSHADERPROGRAM*   program = NULL;
    GRPMATERIAL*        material = this->node->GetMaterial();

    if (!this->haslightning || !GRPSETTINGS::IsLightingEnabled)
        program=GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Diffuse"));
    else
    {
        switch (GRPSETTINGS::LightningDetail)
        {
        case GRPSETTINGS::LOWEST:
        case GRPSETTINGS::LOW:     program = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("DOTDiff"));           break;
        case GRPSETTINGS::MEDIUM:
        case GRPSETTINGS::HIGH:
        case GRPSETTINGS::HIGHEST: program = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("DOTDiffPerFragment")); break;
        }
    }

    if (material != NULL)
    {
        if (material->GetLayer(GRPTEXTURELAYERTYPE_DIFFUSE))
        {
            if (this->haslightning && GRPSETTINGS::IsLightingEnabled)
            {
                switch (GRPSETTINGS::LightningDetail)
                {
                case GRPSETTINGS::LOWEST:
                case GRPSETTINGS::LOW:     program = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("DOT3"));            break;
                case GRPSETTINGS::MEDIUM:
                case GRPSETTINGS::HIGH:
                case GRPSETTINGS::HIGHEST: program = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("DOT3PerFragment")); break;
                }
            }
            else
                program = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Texture"));

            if (material->GetLayer(GRPTEXTURELAYERTYPE_ENVMAP))
                program = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("ReflectiveTexture"));
        }
        else
        {
            if (material->GetLayer(GRPTEXTURELAYERTYPE_ENVMAP))
                program = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("ReflectiveDiffuse"));
        }
    }

    if (this->morphs.GetSize())
    {
        if (this->skin)
            program = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Skinning"));
        else
            program = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Morph"));
    }
    return program;
}


/*-------------------------------------------------------------------
//  GRPELEMENT::Draw
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 10:14:00
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::Draw()
{
  return true;
}


/*-------------------------------------------------------------------
//  GRPELEMENT::Draw
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 13:35:30
//
//  @return       bool :
//
//  @param        pass :
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::Draw( GRPPASS* pass)
{
    if  (!IsVisible()) return true;
    if (isCulled)     return true;

  //------------------------------------- matrix
    ComputeMatrices   (pass->viewmatrix,&pass->viewprojection,pass->projectionmatrix);                          OPENGLCHECKERROR(__L("ComputeMatrices"));

  if (!pass->globalshaderoverride)
  {
    GRPSHADER::UseShader  (this->GetShader());
    LoadMatricesToGPU     (pass->viewmatrix,&pass->viewprojection,pass->projectionmatrix,this->GetShader());
    LoadMaterialToGPU     ();                                                                                     OPENGLCHECKERROR(__L("material"));
  }
  else
  {
    LoadMatricesToGPU (pass->viewmatrix,&pass->viewprojection,pass->projectionmatrix,pass->GetShader());
    LoadMaterialToGPU(pass->GetShader());                                                                       OPENGLCHECKERROR(__L("material"));
  }

                                                                                                                OPENGLCHECKERROR(__L("LoadMatricesToGPU"));
  //-------------------------------------- material

#ifdef GRPDEFORMERS_ACTIVE
      ProcessMorphs();
#endif

  //---------------------------------------------- Draw

      //GRPOPENGLCTRL::BLENDING::SetAlphaBlending(this->blending,this->blendingmode);

      DrawVBO(this->rendermode);

  return true;
}

/*-------------------------------------------------------------------
//  GRPELEMENT::Draw
*/
/**
//
//  Draws the element
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/05/2014 13:20:05
//
//  @return       bool :
//
//  @param        rendermode :
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::Draw(const GRPMATRIX* viewmatrix,const GRPMATRIX* viewprojection,const GRPMATRIX* projection)
{
    if (!IsVisible()) return true;
    if (isCulled)     return true;

  //---------------------------------------------- matrix
    LoadMatricesToGPU (viewmatrix,viewprojection,projection,this->GetShader());       OPENGLCHECKERROR(__L("LoadMatricesToGPU"));

  //---------------------------------------------- material
    LoadMaterialToGPU();                                                              OPENGLCHECKERROR(__L("material"));

  //---------------------------------------------- Deformers
#ifdef GRPDEFORMERS_ACTIVE
    ProcessMorphs();
#endif

  //---------------------------------------------- Draw
    DrawVBO(this->rendermode);


  return true;
}

/*-------------------------------------------------------------------
//  GRPELEMENT::LoadMatricesToGPU
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:49:46
//
//  @param        view :
//  @param        viewprojection :
//  @param        projection :
*/
/*-----------------------------------------------------------------*/
void GRPELEMENT::LoadMatricesToGPU(const GRPMATRIX*  viewmatrix,const GRPMATRIX*  viewprojection, const GRPMATRIX*  projection,const GRPSHADERPROGRAM*  link)
{
  if (link)
  {
  if (link->ModelMatrixLoc              !=-1)         { glUniformMatrix4fv(link->ModelMatrixLoc,                (GLsizei)1, GL_FALSE, &(modelmatrix.matrix[0][0]));               OPENGLCHECKERROR(__L("ModelMatrixLoc")); }
  if (link->ModelViewMatrixLoc          !=-1)         { glUniformMatrix4fv(link->ModelViewMatrixLoc,            (GLsizei)1, GL_FALSE, &(modelviewmatrix.matrix[0][0]));           OPENGLCHECKERROR(__L("ModelViewMatrixLoc")); }
  if (link->ModelViewProjectionMatrixLoc!=-1)         { glUniformMatrix4fv(link->ModelViewProjectionMatrixLoc,  (GLsizei)1, GL_FALSE, &(modelviewprojectionmatrix.matrix[0][0])); OPENGLCHECKERROR(__L("ModelViewProjectionMatrixLoc")); }

  if (link->invViewMatrixLoc            !=-1)
  {
    GRPMATRIX invView;
    invView=*viewmatrix;
    invView.matrix[3][0]=0.0f;
    invView.matrix[3][1]=0.0f;
    invView.matrix[3][2]=0.0f;
    invView.Inverse();
    glUniformMatrix4fv(link->invViewMatrixLoc,                  (GLsizei)1, GL_FALSE, &(invView.matrix[0][0]));               OPENGLCHECKERROR(__L("invViewMatrixLoc"));
  }
  if (link->invProjectionMatrixLoc      !=-1)
  {
    GRPMATRIX invProj;
    invProj=*projection;
    invProj.Inverse();
    glUniformMatrix4fv(link->invProjectionMatrixLoc,            (GLsizei)1, GL_FALSE, &(invProj.matrix[0][0]));               OPENGLCHECKERROR(__L("invViewMatrixLoc"));
  }

  if (link->TextureMatrixLoc != -1)
  {
    glUniformMatrix4fv(link->TextureMatrixLoc, (GLsizei)1, GL_FALSE, &(this->texturematrix.matrix[0][0]));                OPENGLCHECKERROR(__L("TextureMatrixLoc"));
  }


  }
}


/*-------------------------------------------------------------------
//  GRPELEMENT::ComputeMatrices
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 13:30:58
//
//  @param        viewmatrix :
//  @param        viewprojection :
//  @param        projection :
*/
/*-----------------------------------------------------------------*/
void GRPELEMENT::ComputeMatrices(const GRPMATRIX*  viewmatrix,  const GRPMATRIX*  viewprojection, const GRPMATRIX*  projection)
{
      if (node->UsesGeometric)
      {
          modelmatrix.GetMultiplicationMatrix               (node->GetWorldMatrix(),(*node->GetGeometricMatrix()));

          modelviewmatrix.GetMultiplicationMatrix           (viewmatrix,  &modelmatrix);
          modelviewprojectionmatrix.GetMultiplicationMatrix (projection,  &modelviewmatrix);
      }
      else
      {
          modelmatrix.Copy(node->GetWorldMatrix());

          modelviewmatrix.GetMultiplicationMatrix           (viewmatrix,  &modelmatrix);
          modelviewprojectionmatrix.GetMultiplicationMatrix (projection,  &modelviewmatrix);
      }

      if (node->UsesTextureMatrix)
      {
          GRPMATRIX texturepositionmatrix;
                    texturepositionmatrix.BuildTranslationMatrix(node->textureposition.vector[0], node->textureposition.vector[1], node->textureposition.vector[2]);
          GRPMATRIX texturerotationmatrixRotationX;
                    texturerotationmatrixRotationX.BuildRotationMatrixX(node->texturerotation.vector[0]);
          GRPMATRIX texturerotationmatrixRotationY;
                    texturerotationmatrixRotationY.BuildRotationMatrixY(node->texturerotation.vector[1]);
          GRPMATRIX texturerotationmatrixRotationZ;
                    texturerotationmatrixRotationZ.BuildRotationMatrixZ(node->texturerotation.vector[2]);
          GRPMATRIX texturescalematrix;
                    texturescalematrix.BuildScaleMatrix(node->texturescale.vector[0], node->texturescale.vector[1], node->texturescale.vector[2]);

          GRPMATRIX texturerotationmatrix = texturerotationmatrixRotationX;
                    texturerotationmatrix.GetMultiplicationMatrix(texturerotationmatrixRotationX,   texturerotationmatrixRotationY);
                    texturerotationmatrix.GetMultiplicationMatrix(texturerotationmatrix,            texturerotationmatrixRotationZ);


                    texturematrix.GetMultiplicationMatrix(texturepositionmatrix, texturerotationmatrix);
                    texturematrix.GetMultiplicationMatrix(texturematrix, texturescalematrix);
      }
}




/*-------------------------------------------------------------------
//  GRPELEMENT::LoadMaterialToGPU
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:53:24
//
*/
/*-----------------------------------------------------------------*/
void GRPELEMENT::LoadMaterialToGPU(GRPSHADERPROGRAM* outprogram)
{
      if (outprogram==NULL && this->program==NULL)  return;

      GLUINT                    alias       =-1;
      GRPMATERIAL*              material    =node->GetMaterial();
      GRPMATERIALATTRIBUTES*    attributes  =NULL;

      if (outprogram==NULL)
        alias=this->program->iMaterialMatrix;
      else
        alias=outprogram->iMaterialMatrix;

    if(material)
    {
      if (material->GetLayers()->GetSize()>0)
      {
        GRPTEXTURE* t=material->GetLayer(GRPTEXTURELAYERTYPE_DIFFUSE);
        if (t) t->SetFrame((float)this->textureframe);
        material->Select();                                                     OPENGLCHECKERROR(__L("Texture"));
      }
      attributes=material->GetAttributes();
      if (alias!=-1)
      {
        attributes->UseMaterialData(node->BaseMaterial.GetIntensity(),alias);   OPENGLCHECKERROR(__L("MatMatrix"));
      }
    }
    else
    {
      attributes=this->GetNode()->GetBaseMaterial();
      attributes->SetDiffuse(this->defaultcolor);
      attributes->isupdated = true;
      attributes->UseMaterialData(node->BaseMaterial.GetIntensity(),&this->defaultcolor,alias);
    }
}



/*-------------------------------------------------------------------
//  GRPELEMENT::ProcessMorphs
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:54:19
//
*/
/*-----------------------------------------------------------------*/
void GRPELEMENT::ProcessMorphs()
{
    int nmorphs=this->morphs.GetSize();

    for (int e=0; e<nmorphs;  e++)
      morphs.FastGet(e)->Update();
}


/*-------------------------------------------------------------------
//  GRPELEMENT::DrawVBO
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/08/2014 17:20:04
//
*/
/*-----------------------------------------------------------------*/
void GRPELEMENT::DrawVBO(GRPRENDERMODE mode)
{
  GRPSHADERPROGRAM* link=GRPSHADER::CurrentProgram;

  if (!GetIsLoaded())
  {
    OPENGLCHECKERROR(__L("DrawVBO , not loaded"));
    if (mesh!=NULL)
        this->LoadToGPU();

    if (!GetIsLoaded())
        return;
  }

  GRPMESH* mesh = GetMesh();
  if (mesh->nvertexdata == 0)
      return;

  OPENGLCHECKERROR(__L("DrawVBO"));
  if (GRPOPENGLCTRL::VBO::CurrentVBO!= mesh->vertexVBOID)
  {
      GRPOPENGLCTRL::VBO::CurrentVBO= mesh->vertexVBOID;

      glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexVBOID);
      GRPPROFILER::nBufferChanges++;

      OPENGLCHECKERROR(__L("glBindBuffer"));


      int channel = GRPVERTEX_CHANNEL_POSITION;

        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_POSITION,      GRPVERTEX_CHANNEL_POSITION_SIZE,            GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_POSITION));
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_NORMAL,        GRPVERTEX_CHANNEL_NORMAL_SIZE,              GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_NORMAL));
#ifdef GRPVERTEX_EXTENDED
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_BINORMAL,      GRPVERTEX_CHANNEL_BINORMAL_SIZE,            GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_BINORMAL));
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_TANGENT,       GRPVERTEX_CHANNEL_TANGENT_SIZE,             GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_TANGENT));
#endif
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_TEXTURE,       GRPVERTEX_CHANNEL_TEXTURE_SIZE,             GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_TEXTURE));

#ifdef GRPVERTEX_SKINNING
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_BONE_INDEX,    GRPVERTEX_CHANNEL_BONE_INDEX_SIZE,          GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_BONE_INDEX));
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_BONE_WEIGHTS,  GRPVERTEX_CHANNEL_BONE_WEIGHTS_SIZE,        GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_BONE_WEIGHTS));
#endif

#ifdef GRPDEFORMERS_ACTIVE
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_MORPH_POSITION1,  GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE,   GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION1));  OPENGLCHECKERROR(__L("GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION1"));
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_MORPH_POSITION2,  GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE,   GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION2));  OPENGLCHECKERROR(__L("GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION2"));
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_MORPH_POSITION3,  GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE,   GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION3));  OPENGLCHECKERROR(__L("GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION3"));
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_MORPH_POSITION4,  GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE,   GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION4));  OPENGLCHECKERROR(__L("GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION4"));
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_MORPH_POSITION5,  GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE,   GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION5));  OPENGLCHECKERROR(__L("GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION5"));
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_MORPH_POSITION6,  GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE,   GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION6));  OPENGLCHECKERROR(__L("GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION6"));
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_MORPH_POSITION7,  GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE,   GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION7));  OPENGLCHECKERROR(__L("GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION7"));
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_MORPH_POSITION8,  GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE,   GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION8));  OPENGLCHECKERROR(__L("GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION8"));
        /*
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_MORPH_POSITION9,  GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE,   GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION9));
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_MORPH_POSITION10, GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE,   GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION10));
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_MORPH_POSITION11, GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE,   GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION11));
        GRPOPENGLCTRL::VBO::VertexAttributePointer(GRPVERTEX_CHANNEL_MORPH_POSITION12, GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE,   GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION12));
        */
#endif


        GRPOPENGLCTRL::VBO::CurrentVBOIndex= mesh->indexVBOID;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexVBOID);
        glDrawElements(mode, mesh->nindexdata, GL_UNSIGNED_SHORT, (void*)(0));   //The starting point of the IBO

        OPENGLCHECKERROR(__L("GL_ELEMENT_ARRAY_BUFFER"));
  }
  else
    glDrawElements(mode, mesh->nindexdata, GL_UNSIGNED_SHORT, (void*)(0));   //The starting point of the IBO

  GRPPROFILER::nDrawCalls++;
  GRPPROFILER::nFacesRendered+=mesh->nindexdata/3;
  OPENGLCHECKERROR(__L("DrawVBO"));
}

/*-------------------------------------------------------------------
//  GRPELEMENT::Reference
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/08/2014 17:39:27
//
//  @return       bool :
//
//  @param        reference :
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::Reference(GRPELEMENT* reference)
{
  if (!this->IsInstance())
    delete(this->mesh);

  this->isinstance  = true;
  this->mesh        = reference->GetMesh();
  this->AddObserver(reference);

  return true;
}

/*-------------------------------------------------------------------
//  GRPELEMENT::Reference
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      22/09/2016 18:04:00
//
//  @return       bool :
//
//  @param        reference :
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::Reference(GRPMESH* reference)
{
  if (!this->IsInstance())
    delete(this->mesh);

  this->isinstance  = true;
  this->mesh        = reference;

  return true;
}


/*-------------------------------------------------------------------
//  GRPELEMENT::LoadToGPU
*/
/**
//
//  Loads the buffers to gpu for VBO drawing
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/05/2014 13:20:19
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::LoadToGPU()
{
    if (!GetMesh())
        return false;

    if (!GetMesh()->nvertexdata)
        return false;

    if (!GetMesh()->nindexdata)
        return false;

    if (!GetMesh()->vertexdata)
        return false;

    if (!GetMesh()->indexdata)
        return false;

    OPENGLCHECKERROR(__L(" GRPELEMENT::LoadToGPU::initLoadToGPU"));
  //--------- generate vbo  buffers -----------------------------------------------

//if(!glIsBuffer(GetMesh()->vertexVBOID))

  if (!GetMesh()->vertexVBOID)
  glGenBuffers(1, &(GetMesh()->vertexVBOID));                                                                                               OPENGLCHECKERROR(__L("GPU: GL_ARRAY_BUFFER glGenBuffers"));

  if (GetMesh()->vertexVBOID==0) //something went wrong!
    return false;

  glBindBuffer(GL_ARRAY_BUFFER,         GetMesh()->vertexVBOID);                                                                          OPENGLCHECKERROR(__L("GPU: GL_ARRAY_BUFFER glGenBuffers.glBindBuffer"));
  glBufferData(GL_ARRAY_BUFFER,         sizeof(GRPVERTEX)*GetMesh()->nvertexdata,     GetMesh()->vertexdata, GL_STATIC_DRAW);             OPENGLCHECKERROR(__L("GPU: GL_ARRAY_BUFFER glGenBuffers.glBufferData"));

//if(!glIsBuffer(GetMesh()->indexVBOID))
  glGenBuffers(1, &(GetMesh()->indexVBOID));                                                                                                OPENGLCHECKERROR(__L("GPU: GL_ELEMENT_ARRAY_BUFFER glGenBuffers"));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetMesh()->indexVBOID);                                                                           OPENGLCHECKERROR(__L("GPU: GL_ELEMENT_ARRAY_BUFFER glGenBuffers.glBindBuffer"));
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short)*GetMesh()->nindexdata, GetMesh()->indexdata, GL_STATIC_DRAW);              OPENGLCHECKERROR(__L("GPU: GL_ELEMENT_ARRAY_BUFFER glGenBuffers.glBufferData"));

    //--------- model wireframe -----------------------------------------------------------
  int a=0;

    glVertexAttribPointer     (GRPVERTEX_CHANNEL_POSITION,  GRPVERTEX_CHANNEL_POSITION_SIZE,  GL_FLOAT, GL_FALSE,   sizeof(GRPVERTEX),    (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_POSITION));    OPENGLCHECKERROR(__L("GPU: glVertexAttribPointer"));
    glEnableVertexAttribArray (GRPVERTEX_CHANNEL_POSITION);                                                                               OPENGLCHECKERROR(__L("GPU: glEnableVertexAttribArray"));
    GRPOPENGLCTRL::VBO::channel_active[GRPVERTEX_CHANNEL_POSITION] = true;

    glVertexAttribPointer     (GRPVERTEX_CHANNEL_NORMAL,    GRPVERTEX_CHANNEL_NORMAL_SIZE,    GL_FLOAT, GL_FALSE,   sizeof(GRPVERTEX),    (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_NORMAL));    OPENGLCHECKERROR(__L("GPU: glVertexAttribPointer"));
    glEnableVertexAttribArray (GRPVERTEX_CHANNEL_NORMAL);                                                                                 OPENGLCHECKERROR(__L("GPU: glEnableVertexAttribArray"));
    GRPOPENGLCTRL::VBO::channel_active[GRPVERTEX_CHANNEL_NORMAL] = true;

#ifdef GRPVERTEX_EXTENDED

    glVertexAttribPointer     (GRPVERTEX_CHANNEL_BINORMAL,  GRPVERTEX_CHANNEL_BINORMAL_SIZE, GL_FLOAT, GL_FALSE,    sizeof(GRPVERTEX),      (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_BINORMAL));    OPENGLCHECKERROR(__L("GPU: glVertexAttribPointer"));
    glEnableVertexAttribArray (GRPVERTEX_CHANNEL_BINORMAL);             OPENGLCHECKERROR(__L("GPU: glEnableVertexAttribArray"));
    GRPOPENGLCTRL::VBO::channel_active[GRPVERTEX_CHANNEL_BINORMAL] = true;

    glVertexAttribPointer     (GRPVERTEX_CHANNEL_TANGENT,   GRPVERTEX_CHANNEL_TANGENT_SIZE, GL_FLOAT, GL_FALSE,   sizeof(GRPVERTEX),      (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_TANGENT));   OPENGLCHECKERROR(__L("GPU: glVertexAttribPointer"));
    glEnableVertexAttribArray (GRPVERTEX_CHANNEL_TANGENT);                                                                                OPENGLCHECKERROR(__L("GPU: glEnableVertexAttribArray"));
    GRPOPENGLCTRL::VBO::channel_active[GRPVERTEX_CHANNEL_TANGENT] = true;

#endif
      if (this->node)
      if (this->node->GetMaterial())
      {
      glVertexAttribPointer     (GRPVERTEX_CHANNEL_TEXTURE, GRPVERTEX_CHANNEL_TEXTURE_SIZE, GL_FLOAT, GL_FALSE, sizeof(GRPVERTEX),    (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_TEXTURE));   OPENGLCHECKERROR(__L("GPU: glVertexAttribPointer"));
      glEnableVertexAttribArray (GRPVERTEX_CHANNEL_TEXTURE);                                                                              OPENGLCHECKERROR(__L("GPU: glEnableVertexAttribArray"));
      GRPOPENGLCTRL::VBO::channel_active[GRPVERTEX_CHANNEL_TEXTURE] = true;
      }

//--------------- skinning ---------------------------------------------------------------
#ifdef GRPVERTEX_SKINNING
    if (this->skin)
    {
      glVertexAttribPointer     (GRPVERTEX_CHANNEL_BONE_INDEX,    GRPVERTEX_CHANNEL_BONE_INDEX_SIZE, GL_FLOAT, GL_FALSE,  sizeof(GRPVERTEX),  (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_BONE_INDEX));      OPENGLCHECKERROR(__L("GPU: glVertexAttribPointer"));
      glEnableVertexAttribArray (GRPVERTEX_CHANNEL_BONE_INDEX);
      GRPOPENGLCTRL::VBO::channel_active[GRPVERTEX_CHANNEL_BONE_INDEX] = true; OPENGLCHECKERROR(__L("GPU: glEnableVertexAttribArray"));

      glVertexAttribPointer     (GRPVERTEX_CHANNEL_BONE_WEIGHTS,  GRPVERTEX_CHANNEL_BONE_WEIGHTS_SIZE, GL_FLOAT, GL_FALSE,  sizeof(GRPVERTEX),  (void*)(sizeof(GLFLOAT)*GRPVERTEX_CHANNEL_OFFSET_BONE_WEIGHTS));    OPENGLCHECKERROR(__L("GPU: glVertexAttribPointer"));
      glEnableVertexAttribArray (GRPVERTEX_CHANNEL_BONE_WEIGHTS);                                                                                                                   OPENGLCHECKERROR(__L("GPU: glEnableVertexAttribArray"));
      GRPOPENGLCTRL::VBO::channel_active[GRPVERTEX_CHANNEL_BONE_WEIGHTS] = true;
    }
#endif

//--------------- morphing ---------------------------------------------------------------
#ifdef GRPDEFORMERS_ACTIVE
    if (this->deformer)
    {
        int a=GRPVERTEX_CHANNEL_MORPH_POSITION1;
        int n=0;

        for (int e=0,n=0;e<12;a++,n+=3,e++)
        {
          if (a<GRPOPENGLCTRL::VBO::MaxAttributes)
          {
              glVertexAttribPointer     (a, 3, GL_FLOAT, GL_FALSE,  sizeof(GRPVERTEX), (void*)(sizeof(GLFLOAT)*(GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION1+n)));
                OPENGLCHECKERROR(__L("attributePositionMorph"));
              glEnableVertexAttribArray (a);
              GRPOPENGLCTRL::VBO::channel_active[a] = true;
          }
          /*
          else
          {
              XDEBUG_PRINTCOLOR(0, __L("Max Attributtes surpassed on morph"));
          }
          */
        }
    }

#endif

GRPPROFILER::vertexmemory+=(GetMesh()->nvertexdata*sizeof(GRPVERTEX)*GetMesh()->nvertexdata)+(sizeof(unsigned short)*GetMesh()->nindexdata);

  GetMesh()->isloaded=true;
  GRPOPENGLCTRL::VBO::CurrentVBO = -1;
  return true;
}




/*-------------------------------------------------------------------
//  GRPMESH::UpdateBoundingBox
*/
/**
//
//  Updates max AABB
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/08/2014 11:03:07
//
*/
/*-----------------------------------------------------------------*/
void  GRPELEMENT::UpdateBoundingBox()
{
  if (boundingbox.blocked)
    return;

  if (!this->GetMesh())
    return;

  if(this->GetMesh()->nvertexdata<=0) return;

  GRPVERTEX* vertex=&this->GetMesh()->vertexdata[0];
  boundingbox.maxpoint.x  = vertex->position[0];
  boundingbox.minpoint.x  = vertex->position[0];
  boundingbox.maxpoint.y  = vertex->position[1];
  boundingbox.minpoint.y  = vertex->position[1];
  boundingbox.maxpoint.z  = vertex->position[2];
  boundingbox.minpoint.z  = vertex->position[2];

  for(int e=(this->GetMesh()->nvertexdata-1); e>0; e--)
    {
      GRPVERTEX* vertex=&this->GetMesh()->vertexdata[e];

      if(vertex->position[0] > boundingbox.maxpoint.x)  boundingbox.maxpoint.x = vertex->position[0];
      else
      if(vertex->position[0] < boundingbox.minpoint.x)  boundingbox.minpoint.x = vertex->position[0];

      if(vertex->position[1] > boundingbox.maxpoint.y)  boundingbox.maxpoint.y = vertex->position[1];
      else
      if(vertex->position[1] < boundingbox.minpoint.y)  boundingbox.minpoint.y = vertex->position[1];

      if(vertex->position[2] > boundingbox.maxpoint.z)  boundingbox.maxpoint.z = vertex->position[2];
      else
      if(vertex->position[2] < boundingbox.minpoint.z)  boundingbox.minpoint.z = vertex->position[2];
    }

  boundingbox.width =boundingbox.maxpoint.x-boundingbox.minpoint.x;
  boundingbox.height=boundingbox.maxpoint.y-boundingbox.minpoint.y;
  boundingbox.depth =boundingbox.maxpoint.z-boundingbox.minpoint.z;
}

/*-------------------------------------------------------------------
//  GRPELEMENT::MergeElement
*/
/**
//
//  Fuses two elements in one
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/06/2014 10:36:41
//
//  @param        Mergeable :
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::MergeElement(GRPELEMENT* merging)
{
  GRPELEMENT* mergeable=NULL;
  if (!merging->IsInstance())
  {
    mergeable=merging;
  }
  else
  {
    mergeable=new GRPELEMENT();
    if (mergeable)
    {
    mergeable->SetNode            (merging->GetNode());
    mergeable->Reference          (merging->GetMesh());
    mergeable->CollapseTransforms ();
    }
    else
      return false;
  }

  if (this->IsInstance())
  {
    this->CollapseTransforms  ();
  }

  XDWORD totalVertex=this->GetMesh()->nvertexdata+mergeable->GetMesh()->nvertexdata;
  XDWORD totalIndex =this->GetMesh()->nindexdata+mergeable->GetMesh()->nindexdata;

  GRPVERTEX*  mergued_vertex=new GRPVERTEX  [totalVertex];
  if (mergued_vertex==NULL)
  {
    if (merging->IsInstance())
    delete(mergeable);
    return false;
  }
  XWORD*      mergued_index =new XWORD      [totalIndex ];
  if (mergued_index==NULL)
  {
    if (merging->IsInstance())
    delete mergeable;
    delete[] mergued_vertex;
    return false;
  }

  XDWORD e=0;

  if (GetMesh()->nindexdata>0)
    memcpy(mergued_index,GetMesh()->indexdata,GetMesh()->nindexdata*sizeof(XWORD));

  for (XDWORD i=0;i<mergeable->GetMesh()->nindexdata;i++)
    mergued_index[GetMesh()->nindexdata+i] = mergeable->GetMesh()->indexdata[i] + GetMesh()->nindexdata;

  if (GetMesh()->nvertexdata>0)
    memcpy(mergued_vertex,GetMesh()->vertexdata,GetMesh()->nvertexdata*sizeof(GRPVERTEX));

  if (mergeable->GetMesh()->nvertexdata>0)
    memcpy(&mergued_vertex[GetMesh()->nvertexdata],mergeable->GetMesh()->vertexdata,mergeable->GetMesh()->nvertexdata*sizeof(GRPVERTEX));

  if (GetMesh()->vertexdata)
    delete[] GetMesh()->vertexdata;

  if (GetMesh()->indexdata)
    delete[] GetMesh()->indexdata;



  GetMesh()->vertexdata =mergued_vertex;
  GetMesh()->indexdata    =mergued_index;

  GetMesh()->nindexdata =totalIndex;
  GetMesh()->nvertexdata  =totalVertex;

  GetMesh()->maxindex+=totalIndex;


  if (merging->IsInstance())
    delete mergeable;

  this->isinstance=false;

  GetMesh()->isloaded=false;
  return true;
}





/*-------------------------------------------------------------------
//  GRPELEMENT::CollapseTransforms
*/
/**
//  Clears the transforms and alters the mesh data accordingly
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      22/10/2015 17:12:38
//
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::Collapse(bool Translationonly)
{
  if (Translationonly)
  return CollapseTranslations();

return CollapseTransforms();
}

/*-------------------------------------------------------------------
//  GRPELEMENT::CollapseTranslations
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/09/2016 16:25:41
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::CollapseTranslations()
{
  if (this->IsInstance())
     if (!Disembody())
       return false;

  const GRPVECTOR* position=this->GetNode()->GetPosition();
  XDWORD n=this->GetMesh()->nvertexdata;
  for (XDWORD e=0;e<n;e++)
  {
    GRPVERTEX* vertex=&this->mesh->vertexdata[e];

    vertex->position[0]+=position->vector[0];
    vertex->position[1]+=position->vector[1];
    vertex->position[2]+=position->vector[2];
  }

  this->node->SetPosition (0.0,0.0,0.0);
  this->node->SetScale    (1.0,1.0,1.0);
  GetMesh()->isloaded=false;
  return true;

}


/*-------------------------------------------------------------------
//  GRPELEMENT::CollapseTransforms
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/09/2016 16:25:54
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::CollapseTransforms ()
{
  if (this->IsInstance())
     Disembody();

  if (this->node==NULL)
    return false;

  if (this->GetMesh()==NULL)
    return false;

  this->node->Update();

  GRPMATRIX* model;
  model=this->node->GetModelMatrix();

  model->GetMultiplicationMatrix(this->node->GetModelMatrix(),node->GetGeometricMatrix());

  GRPMATRIX rot;
  rot=this->node->rotationmatrix;

  float scy=this->node->GetScale()->vector[1];

  for (XDWORD e=0;e<this->GetMesh()->nvertexdata;e++)
  {
    GRPVERTEX* vertex=&this->GetMesh()->vertexdata[e];
    GRPVECTOR vector;
    vector=vertex->position;
    GRPVECTOR result;
    result=model->TransformPoint(vector);

    vertex->position[0]=result.vector[0];
    vertex->position[1]=result.vector[1];
    vertex->position[2]=result.vector[2];

    GRPVECTOR normal;
              vector=vertex->normal;
              normal=rot.TransformVector(vector);


    vertex->normal[0]=normal.vector[0];
    vertex->normal[1]=normal.vector[1];
    vertex->normal[2]=normal.vector[2];


  }
  node->GetGeometricMatrix()->LoadIdentity();

  this->node->SetPosition (0.0,0.0,0.0);
  this->node->SetRotation (0.0,0.0,0.0);
  this->node->SetScale    (1.0,1.0,1.0);
  this->node->positionChanged=true;
  this->node->scaleChanged=true;
  this->node->rotationChanged=true;
  this->node->UsesGeometric=false;
  GetMesh()->isloaded=false;

  this->UpdateBoundingBox();
  this->GetBoundingBox()->ascend*=scy;
  this->GetBoundingBox()->descend*=scy;
  this->GetBoundingBox()->linegap*=scy;
  this->GetBoundingBox()->lineheight *= scy;

  return true;
}


/*-------------------------------------------------------------------
//  GRPELEMENT::Disembody
*/
/**
//
//  Converts instance to concrete model with vertex data
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/10/2015 10:28:13
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::Disembody()
{
  if (!this->isinstance)
    return false;

  if (this->mesh==NULL)
    return false;

  GRPMESH* instance=this->mesh;

  GRPMESH* mesh=new GRPMESH();
  this->SetMesh(mesh);
  this->isinstance=false;

  if (this->GetMesh()->indexdata!=NULL)     delete[] this->GetMesh()->indexdata;
  if (this->GetMesh()->vertexdata!=NULL)    delete[] this->GetMesh()->vertexdata;

  if (instance->nindexdata==0 || instance->nvertexdata==0)
    return true;

  this->GetMesh()->indexdata=new XWORD[instance->nindexdata];
  if (!this->GetMesh()->indexdata)
    return false;

  this->GetMesh()->vertexdata=new GRPVERTEX[instance->nvertexdata];
  if (!this->GetMesh()->vertexdata)
  {
      delete(this->GetMesh()->indexdata);
      this->GetMesh()->indexdata=NULL;
      return false;
  }

  this->GetMesh()->nvertexdata  =instance->nvertexdata;
  this->GetMesh()->nindexdata   =instance->nindexdata;

  memcpy(this->GetMesh()->vertexdata, instance->vertexdata, sizeof(GRPVERTEX)*this->GetMesh()->nvertexdata);
  memcpy(this->GetMesh()->indexdata,  instance->indexdata,  sizeof(XWORD)*this->GetMesh()->nindexdata);

  GetMesh()->faces.DeleteContents();
  GetMesh()->faces.DeleteAll();

  isinstance  =false;
  instance    =NULL;
  GetMesh()->isloaded   =false;



return true;
}


/*-------------------------------------------------------------------
//  GRPELEMENT::SetProgram
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/11/2014 11:00:42
//
//  @return        :
//
//  @param        programID :
*/
/*---------------------------re--------------------------------------*/
void GRPELEMENT::SetProgram (GRPSHADERPROGRAM* programID)
{
  if (programID!=this->program)
  {
    this->program=programID;
    this->Publish(GRPMSG_ORDER);
  }
}

/*-------------------------------------------------------------------
//  GRPELEMENT::GetProgram
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/08/2016 10:15:12
//
//  @return       PSHADERPROGRAM* :
//
*/
/*-----------------------------------------------------------------*/
GRPSHADERPROGRAM* GRPELEMENT::GetProgram()
{
    return this->program;
}




/*-------------------------------------------------------------------
//  GRPELEMENT::SetSkin
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:02:10
//
//  @param        pskin :
*/
/*-----------------------------------------------------------------*/
void GRPELEMENT::SetSkin(GRPSKIN* pskin)
{
    this->skin=pskin;
    this->skin->target=this;
    this->morphs.Add(skin);
}

/*-------------------------------------------------------------------
//  GRPELEMENT::SetVisible
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:10:19
//
//  @param        isvisible :
*/
/*-----------------------------------------------------------------*/
void GRPELEMENT::SetVisible (bool isvisible)
{
  this->visibility=(isvisible ? 1.0f : 0.0f);
}

/*-------------------------------------------------------------------
//  GRPELEMENT::IsVisible
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:10:44
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::IsVisible()
{
  /*
  if (node->inherited_visibility<0.5f)
    return false;
  if (this->visibility<0.5f)
    return false;
  if (node->visibility<0.5f)
    return false;
    */

  if (this->GetNode()->GetIsVisible() && this->visibility>0.5f)
    return true;




  return false;
}

void GRPELEMENT::SetIsCulled(bool b)
{
    this->isCulled = b;
}


/*-------------------------------------------------------------------
//  GRPELEMENT::GetIsReferible
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:15:24
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::GetIsReferible ()
{
  return isreferible;
}


/*-------------------------------------------------------------------
//  GRPELEMENT::GetRenderMode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:15:51
//
//  @return       GRPRENDERMODE :
//
*/
/*-----------------------------------------------------------------*/
GRPRENDERMODE GRPELEMENT::GetRenderMode()
{
  return rendermode;
}


/*-------------------------------------------------------------------
//  GRPELEMENT::SetRenderMode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:16:13
//
//  @param        rendermode :
*/
/*-----------------------------------------------------------------*/
void GRPELEMENT::SetRenderMode(GRPRENDERMODE rendermode)
{
  this->rendermode = rendermode;
}




/*-------------------------------------------------------------------
//  GRPELEMENT::IsInstance
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:18:02
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::IsInstance()
{
  return isinstance;
}

/*-------------------------------------------------------------------
//  GRPMESH::GetIsLoaded
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:19:00
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPELEMENT::GetIsLoaded()
{
  if (GetMesh())
  return GetMesh()->isloaded;
  return false;
}


/*-------------------------------------------------------------------
//  GRPELEMENT::SetTextureFrame
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:18:33
//
//  @param        frame :
*/
/*-----------------------------------------------------------------*/
void GRPELEMENT::SetTextureFrame(float frame)
{
  this->textureframe=frame;
}


/*-------------------------------------------------------------------
//  GRPELEMENT::Finish
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:18:54
//
*/
/*-----------------------------------------------------------------*/
void GRPELEMENT::Finish()
{
  GetMesh()->SetFaceBuffers();
}




/*-------------------------------------------------------------------
//  GRPELEMENT::GetNode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:20:00
//
//  @return       GRPNODE* :
//
*/
/*-----------------------------------------------------------------*/
GRPNODE* GRPELEMENT::GetNode()
{
  return node;
}

/*-------------------------------------------------------------------
//  GRPELEMENT::SetNode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:20:19
//
//  @param        n :
*/
/*-----------------------------------------------------------------*/
void GRPELEMENT::SetNode(GRPNODE* n)
{
  if (this->node!=NULL)
  {
    this->Dettach();
    this->node=NULL;
  }
  else
  {
    node=n;
    if (n)
    node->modified=true;
  }
}

/*-------------------------------------------------------------------
//  GRPELEMENT::GetDeformer
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:20:36
//
//  @return       GRPDEFORMER* :
//
*/
/*-----------------------------------------------------------------*/
GRPDEFORMER* GRPELEMENT::GetDeformer()
{
  return this->deformer;
}


/*-------------------------------------------------------------------
//  GRPELEMENT::SetDeformer
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:21:20
//
//  @param        pdeformer :
*/
/*-----------------------------------------------------------------*/
void GRPELEMENT::SetDeformer(GRPDEFORMER* pdeformer)
{
  this->deformer=pdeformer;
  this->morphs.Add(pdeformer);
}

/*-------------------------------------------------------------------
//  GRPELEMENT::GetSkin
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:21:26
//
//  @return       GRPSKIN* :
//
*/
/*-----------------------------------------------------------------*/
GRPSKIN* GRPELEMENT::GetSkin()
{
  return this->skin;
}

/*-------------------------------------------------------------------
//  GRPELEMENT::SetDefaultColor
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:24:44
//
//  @param        color :
*/
/*-----------------------------------------------------------------*/
void GRPELEMENT::SetDefaultColor(GRPCOLOR color)
{
  this->defaultcolor=color;
}

/*-------------------------------------------------------------------
//  GRPELEMENT::SetRenderFlags
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:44:31
//
//  @param        renderflags :
*/
/*-----------------------------------------------------------------*/
void GRPELEMENT::SetRenderFlags     (XDWORD renderflags)
{
  this->renderflags=renderflags;
}


/*-------------------------------------------------------------------
//  GRPELEMENT::GetRenderFlags
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2015 11:44:49
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPELEMENT::GetRenderFlags   ()
{
  return this->renderflags;
}




/*-------------------------------------------------------------------
//  GRPELEMENT::GetRenderedFaces
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/11/2014 12:15:38
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPELEMENT::GetRenderedFaces()
{
  //if (this->isinstance)   return this->instance->GetRenderedFaces();
  return this->GetMesh()->faces.GetSize();
}


/*-------------------------------------------------------------------
//  GRPELEMENT::GetShader
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/09/2015 10:40:16
//
//  @return       GRPSHADERPROGRAM* :
//
*/
/*-----------------------------------------------------------------*/
GRPSHADERPROGRAM* GRPELEMENT::GetShader ()
{
  return this->program;
}




/*-------------------------------------------------------------------
//  GRPELEMENT::SetRenderPriority
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/11/2015 10:37:26
//
//  @param        lowerpriorityfirst :
*/
/*-----------------------------------------------------------------*/
void  GRPELEMENT::SetRenderPriority (int lowerpriorityfirst)
{
  this->renderpriority=lowerpriorityfirst;
  this->Publish(GRPMSG_ORDER);
}

/*-------------------------------------------------------------------
//  GRPELEMENT::GetRenderPriority
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/11/2015 10:37:29
//
//  @return       int :
//
*/
/*-----------------------------------------------------------------*/
int GRPELEMENT::GetRenderPriority   ()
{
return this->renderpriority;
}


/*-------------------------------------------------------------------
//  GRPELEMENT::Notify
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/02/2016 9:50:53
//
//  @return       bool :
//
//  @param        msg :
//  @param        notifier :
*/
/*-----------------------------------------------------------------*/
bool  GRPELEMENT::Notify(GRPMSGTYPE msg,GRPSUBJECT* notifier)
{
  switch(msg)
  {
    case GRPMSG_DESTROYED:
      {
        GRPELEMENT* element=dynamic_cast<GRPELEMENT*>(notifier);
        if (element)
        {
            this->Disembody();
        }
      }
      break;
    case GRPMSG_DEREFERENCE:
      {
        GRPELEMENT* element=dynamic_cast<GRPELEMENT*>(notifier);
        if (element)
          this->Observers.Delete(element);
        return true;
      }
      break;
  }
return true;
}

/*-------------------------------------------------------------------
//  GRPELEMENT::GetSubjectType
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/02/2016 13:37:21
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD  GRPELEMENT::GetSubjectType()
{
  return GRPNAMED_TYPE_GRPELEMENT;
}

/*-------------------------------------------------------------------
//  GRPELEMENT::SetBlending
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/06/2016 12:09:15
//
//  @param        b :
*/
/*-----------------------------------------------------------------*/
void  GRPELEMENT::SetBlending(bool b)
{
  this->blending=b;
  //(b ?  this->renderpriority-- : this->renderpriority++);
  //this->Publish(GRPMSG_ORDER);
}

/*-------------------------------------------------------------------
//  GRPELEMENT::SetBlendingMode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/06/2016 17:07:04
//
//  @param        src :
//  @param        dest :
*/
/*-----------------------------------------------------------------*/
void  GRPELEMENT::SetBlendingMode(GRPBLENDINGMODE mode)
{
  this->blendingmode=mode;
  if (mode==GRPBLENDINGMODE_NONE)
    this->blending=false;
}

/*-------------------------------------------------------------------
//  GRPELEMENT::IsBlending
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/11/2016 14:09:15
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  GRPELEMENT::IsBlending()
{
  if (blending)
    return true;
  if (this->node)
    if (this->node->GetMaterial())
      if (this->node->GetMaterial()->GetAttributes()->GetTransparency()<1.0f)
        return true;
  return false;
}
