
/*------------------------------------------------------------------------------------------
//        GRPSKINNING.CPP
//
//        Updates skinning data
//
//        Author                                                :
//        Date Of Creation        : 29/06/2015 20:57:57
//        Last Modification        :
//
//        (C) Copyright All right reserved.
//----------------------------------------------------------------------------------------*/



/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPSkinning.h"
#include "GRPShader.h"
#include "GRPElement.h"
#include "GRPShaderLibrary.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


const char GRPSHADERLIBRARY::SkinVertex               []=


        "attribute vec3 a_position;\n"
        "attribute vec3 a_normal;\n"
        "attribute vec2 a_texCoord;\n"
        "attribute vec4 a_boneIndices;\n"
        "attribute vec4 a_boneWeights;\n"

        "uniform mat4 boneRootMatrix;\n"
        "uniform mat4 boneMatrices[32];\n"
        "uniform mat4 bonePoseMatrices[32];\n"

        "uniform mat4 modelMat;\n"
        "uniform mat4 modelviewMat;\n"
        "uniform mat4 modelviewprojectionMat;\n"

        "uniform mat4 lightDataMatrix[1];\n"
        "uniform mat4 lightColorMatrix[1];\n"
        "uniform mat4 materialDataMat;\n"

        //------------- varying

        "vec3 normalInterp;\n"
        "vec3 vertPos;\n"

        "varying vec4 v_position;\n"
        "varying vec3 v_normal;\n"
        "varying vec2 v_texCoord;\n"

        "float attenuattion;\n"
        "float lambertian;\n"
        "float specular;\n"
        "float lightInfluence;\n"

        "uniform sampler2D s_texture;        \n"
        "varying vec4 diffuseColor,specularColor,lambertianColor; \n"



        "void main(){\n"
          "   vec3  L;                \n"
          "   float fspecular=0.0;    \n"
          "   float flambertian=0.0;  \n"
          "   float fattenuattion=1.0;\n"

          "   float Tlambertian=0.0;\n"
          "   float Tspecular=0.0;\n"
          /*
            skinnedPosition = (boneWeight0 * (Bones[boneIndex0] * vec4(iPosition, 1.0)) +
                       boneWeight1 * (Bones[boneIndex1] * vec4(iPosition, 1.0))).xyz;

              // Compute the final position
              gl_Position = ModelViewProjection * vec4(skinnedPosition, 1.0);
              */
        //----------------- skinning

        "mat4                        matPose  =bonePoseMatrices[int(a_boneIndices.x)]* a_boneWeights.x; \n"
        "                            matPose +=bonePoseMatrices[int(a_boneIndices.y)]* a_boneWeights.y;\n"
        "                            matPose +=bonePoseMatrices[int(a_boneIndices.z)]* a_boneWeights.z;\n"
        "                            matPose +=bonePoseMatrices[int(a_boneIndices.w)]* a_boneWeights.w;\n"

        "mat4                        matTransform  =boneMatrices[int(a_boneIndices.x)]*a_boneWeights.x; \n"
        "                            matTransform +=boneMatrices[int(a_boneIndices.y)]*a_boneWeights.y; \n"
        "                            matTransform +=boneMatrices[int(a_boneIndices.z)]*a_boneWeights.z; \n"
        "                            matTransform +=boneMatrices[int(a_boneIndices.w)]*a_boneWeights.w; \n"
/*
        "vec4                        vt  =boneMatrices[int(a_boneIndices.x)]*bonePoseMatrices[int(a_boneIndices.x)]*boneRootMatrix* vec4(a_position, 1.0)*a_boneWeights.x; \n"
        "                            vt +=boneMatrices[int(a_boneIndices.y)]*bonePoseMatrices[int(a_boneIndices.y)]*boneRootMatrix* vec4(a_position, 1.0)*a_boneWeights.y; \n"
        "                            vt +=boneMatrices[int(a_boneIndices.z)]*bonePoseMatrices[int(a_boneIndices.z)]*boneRootMatrix* vec4(a_position, 1.0)*a_boneWeights.z; \n"
        "                            vt +=boneMatrices[int(a_boneIndices.w)]*bonePoseMatrices[int(a_boneIndices.w)]*boneRootMatrix* vec4(a_position, 1.0)*a_boneWeights.w; \n"
        */
        "vec4                        vt  =boneMatrices[int(a_boneIndices.x)]*bonePoseMatrices[int(a_boneIndices.x)]* vec4(a_position, 1.0)*a_boneWeights.x; \n"
        "                            vt +=boneMatrices[int(a_boneIndices.y)]*bonePoseMatrices[int(a_boneIndices.y)]* vec4(a_position, 1.0)*a_boneWeights.y; \n"
        "                            vt +=boneMatrices[int(a_boneIndices.z)]*bonePoseMatrices[int(a_boneIndices.z)]* vec4(a_position, 1.0)*a_boneWeights.z; \n"
        "                            vt +=boneMatrices[int(a_boneIndices.w)]*bonePoseMatrices[int(a_boneIndices.w)]* vec4(a_position, 1.0)*a_boneWeights.w; \n"


        "vec3                        nt  =mat3(boneMatrices[int(a_boneIndices.x)]*bonePoseMatrices[int(a_boneIndices.x)]*boneRootMatrix)* a_normal *a_boneWeights.x; \n"
        "                            nt +=mat3(boneMatrices[int(a_boneIndices.y)]*bonePoseMatrices[int(a_boneIndices.y)]*boneRootMatrix)* a_normal *a_boneWeights.y; \n"
        "                            nt +=mat3(boneMatrices[int(a_boneIndices.z)]*bonePoseMatrices[int(a_boneIndices.z)]*boneRootMatrix)* a_normal *a_boneWeights.z; \n"
        "                            nt +=mat3(boneMatrices[int(a_boneIndices.w)]*bonePoseMatrices[int(a_boneIndices.w)]*boneRootMatrix)* a_normal *a_boneWeights.w; \n"
        //*/

        " gl_Position = modelviewprojectionMat *boneRootMatrix * vt;\n"

        "    v_texCoord   = a_texCoord;\n"
        "    v_position = modelviewMat * vt;\n"

        //----------------- light0 //las normales no estan bien calculadas
  "     L                 = (v_position.xyz-(lightDataMatrix[0])[0].xyz).xyz;                               \n" //precalculated lightposition*viewMat
  "     float distance    = length (L);                                                                   \n"
  "     fattenuattion     = (materialDataMat[3][1]*(lightDataMatrix[0][1][3])) / ((((lightDataMatrix[0][1][2]*distance)+lightDataMatrix[0][1][1])*distance)+lightDataMatrix[0][1][0]);\n"

  //"     normalInterp      = (modelviewMat * vec4(a_normal, 0.0)).xyz;                                           \n"
  "     vec3 normal       = normalize(a_normal);                                                                  \n"
  "          v_normal     = (modelviewMat*vec4(normal,0.0)).xyz;\n"
  "     vec3 lightDir     = normalize(-L);                                                                \n"
  "     lambertianColor   = max(dot(lightDir,normal), 0.0)*lightColorMatrix[0][1]*fattenuattion;          \n"


  "     vec4 Emissive     = materialDataMat[2];                                                         \n"
  "     vec3 reflectDir   = reflect(-lightDir, normal);                                                   \n"
  "     float specAngle   = max(dot(reflectDir, normalize(-v_position.xyz)), 0.0);                        \n"
  "       specularColor   = (materialDataMat[1]*lightColorMatrix[0][2]*fattenuattion*pow(specAngle, materialDataMat[3][0]))+Emissive;   \n"
  "       diffuseColor    = (lambertianColor*materialDataMat[0]) + specularColor; \n"

  //*/
        "}";

#ifdef GRPSKINNING_ACTIVE
/*-------------------------------------------------------------------
//  GRPSKIN::~GRPSKIN
*/
/**
//
//   Class Destructor GRPSKIN
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/05/2017 17:15:53
//
*/
/*-----------------------------------------------------------------*/
GRPSKIN::~GRPSKIN()
{
  if (BoneMatrices!=NULL)
    delete BoneMatrices;

  if (PoseMatrices!=NULL)
    delete PoseMatrices;

  delete this->BoneWeightsArray;
  delete this->BoneIndexArray;

  this->Clusters.DeleteContents();
  this->Clusters.DeleteAll();
  this->PoseNodes.DeleteContents();
  this->PoseNodes.DeleteAll();
}

/*-------------------------------------------------------------------
//        GRPSKIN::Init
*/
/**
//
//
//
//        @author
//        @version                        29/06/2015 21:00:08
//
//        @return                         bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPSKIN::Init()
{

  #ifdef GRPDEFORMERS_ACTIVE

                XDWORD nv=target->GetMesh()->GetNVertex();

                if (nv==0)  return true;

                BoneMatrices=new GLFLOAT[16*this->Clusters.GetSize()];

                if (BoneMatrices==NULL)
                                return false;

                  PoseMatrices=new GLFLOAT[4*4*this->Clusters.GetSize()];

                if (PoseMatrices==NULL)
                                return false;

                BoneWeightsArray                =new GLFLOAT[4*nv];
                BoneIndexArray                  =new GLUINT[4*nv];

                if (BoneWeightsArray==NULL || BoneIndexArray==NULL)
                                return false;

                for (XDWORD e=0;e<(nv*4);e++)
                {
                  BoneIndexArray[e]=0;
                  BoneWeightsArray[e]=0.0f;
                }

      if (target->GetMesh()->vertexlayers.Find(GRPVERTEX_CHANNEL_BONE_INDEX) == NOTFOUND)
      {
        XVECTOR<XVECTOR<XDWORD>* > Indexes;
        XVECTOR<XVECTOR<double>* > Weights;
        XVECTOR<int> faceindexes;

        Indexes.SetAddInLimit(nv);
        Weights.SetAddInLimit(nv);
        Indexes.SetIsMulti(true);
        Weights.SetIsMulti(true);
        faceindexes.SetIsMulti(true);

        XDWORD n = this->target->GetMesh()->GetFaces()->GetSize();
        for (XDWORD e=0;e<n;e++)
        {
          GRPFACE* face=this->target->GetMesh()->GetFaces()->FastGet(e);
                faceindexes.Add(face->a);
                faceindexes.Add(face->b);
                faceindexes.Add(face->c);
        }


        XDWORD faceindexsize = faceindexes.GetSize();
        XDWORD clustersizes = Clusters.GetSize();
        for (XDWORD e=0;e<faceindexsize;e++)
        {
                Indexes.Add(new XVECTOR<XDWORD>());
                Weights.Add(new XVECTOR<double>());

                int i=faceindexes.FastGet(e);
                for (XDWORD c=0;c<clustersizes;c++)
                {
                        GRPSKINCLUSTER* cluster = Clusters.FastGet(c);
                        int index= cluster->Indexes.Find(i);
                        if (index!=NOTFOUND)
                        {
                                double w= cluster->Weights.FastGet(index);
                                Indexes.FastGet(e)->Add(c);
                                Weights.FastGet(e)->Add(w);
                        }
                }
        }

        int i=0;


          XDWORD nIndexdata = target->GetMesh()->GetNindexData();
          for (XDWORD e = 0; e < nIndexdata; e++)
          {
            GRPVERTEX* vertex = &this->target->GetMesh()->GetVertexData()[e];
            XWORD* index = &this->target->GetMesh()->GetIndexData()[e];

            vertex->boneIndices[0] = (GLFLOAT)Indexes.Get(e)->Get(0);
            vertex->boneIndices[1] = (GLFLOAT)Indexes.Get(e)->Get(1);
            vertex->boneIndices[2] = (GLFLOAT)Indexes.Get(e)->Get(2);
            vertex->boneIndices[3] = (GLFLOAT)Indexes.Get(e)->Get(3);

            double wa = Weights.Get(e)->Get(0);
            double wb = Weights.Get(e)->Get(1);
            double wc = Weights.Get(e)->Get(2);
            double wd = Weights.Get(e)->Get(3);

            vertex->boneWeights[0] = (GLFLOAT)Weights.Get(e)->Get(0);
            vertex->boneWeights[1] = (GLFLOAT)Weights.Get(e)->Get(1);
            vertex->boneWeights[2] = (GLFLOAT)Weights.Get(e)->Get(2);
            vertex->boneWeights[3] = (GLFLOAT)Weights.Get(e)->Get(3);

            if (wa == 0.0 && wb == 0.0 && wc == 0.0 && wd == 0.0)
              i++;
          }
          target->GetMesh()->vertexlayers.Add(GRPVERTEX_CHANNEL_BONE_INDEX);
          target->GetMesh()->vertexlayers.Add(GRPVERTEX_CHANNEL_BONE_WEIGHTS);

          Indexes.DeleteContents();
          Indexes.DeleteAll();
          Weights.DeleteContents();
          Weights.DeleteAll();
        }



            this->target->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SKD"));

            if (this->target->GetNode())
              if (this->target->GetNode()->GetMaterial())
                if (this->target->GetNode()->GetMaterial()->GetLayers())
                  this->target->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SKN"));


        target->GetNode()->SetScale(1.0f,1.0f,1.0f);
        target->GetNode()->UsesGeometricScale=false;
        target->GetNode()->Update();
        target->GetNode()->UpdateWorldMatrix();

        Root=target->GetNode()->GetWorldMatrix();
        Root.Inverse();
      //  Root.LoadIdentity();

#endif

        return true;
}

/*-------------------------------------------------------------------
//        GRPSKIN::Update
*/
/**
//
//
//
//        @author
//        @version                        29/06/2015 21:00:14
//
//        @return                         bool :
//
*/
/*-----------------------------------------------------------------*/

/*
To retrieve the node of bone, I also use FbxNode* pBoneNode = pCluster->GetLink(), and call pBoneNode->EvaluateLocalTransform( CurrentTime ) to get the local transformation
matrix of the bone at current time. But there is a special thing I did: remember that I converted the mesh by the global transformation matrix of root node right? So for the
root node of skeleton, I need to get its global transformation matrix by calling FbxAMatrix BoneInitPoseGlobalTransform = pBoneRootNode->EvaluateGlobalTransform(), then
inverse it. And the local transform matrix of my root bone will be: BoneLocalTransform = InvBoneInitPoseGlobalTransform * BoneLocalTransform;
We only need to do it for the root node of bone.
*/

bool GRPSKIN::Update()
{
    #ifdef GRPDEFORMERS_ACTIVE
        if (PoseNodes.GetSize()==0)
                return true;

        //this->target->GetNode()->GetWorldMatrix().Inverse();

        GRPMATRIX m;

        m.LoadIdentity();
        for (XDWORD c=0;c<this->Clusters.GetSize();c++)
        {
                m=this->Clusters.Get(c)->TransformLink;
                memcpy(&PoseMatrices[4*4*c],  m.GetMatrixPointer(),4*4*sizeof(GLFLOAT));
        }

        for (XDWORD c=0;c<this->Clusters.GetSize();c++)
        {
            if (this->Clusters.Get(c)->GetLimbs()->GetSize())
            {
              m=this->Clusters.Get(c)->GetLimbs()->Get(0)->GetWorldMatrix();
              memcpy(&BoneMatrices[4*4*c],  m.GetMatrixPointer(),4*4*sizeof(GLFLOAT));
            }
        }


        glUniformMatrix4fv(this->target->GetShader()->iBoneRootMatrix, 1, GL_FALSE, (const GLFLOAT*)&(this->Root.GetMatrixPointer()[0]));

        if (this->target->GetShader()->iBonePoseMatrices!=-1)
        {
                glUniformMatrix4fv(this->target->GetShader()->iBonePoseMatrices,  (GLsizei)Clusters.GetSize(), GL_FALSE,  (const GLFLOAT*)&(PoseMatrices[0]));
                OPENGLCHECKERROR(__L("BoneMatrix"));
        }

        if (this->target->GetShader()->iBoneMatrices!=-1)
        {
                glUniformMatrix4fv(this->target->GetShader()->iBoneMatrices,      (GLsizei)Clusters.GetSize(), GL_FALSE,  (const GLFLOAT*)&(BoneMatrices[0]));
                OPENGLCHECKERROR(__L("BoneMatrix"));
        }

#endif
                return true;
}

#else

#include "GRPSkinning.h"

GRPSKIN::~GRPSKIN() {   }
bool GRPSKIN::Init  () { return true;  }
bool GRPSKIN::Update() { return true;  }

#endif

/*-------------------------------------------------------------------
//  GRPSKIN::SetValue
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/05/2017 17:17:27
//
//  @return       bool :
//
//  @param        type :
//  @param        subtype :
//  @param        value :
//  @param        base :
//  @param        offbound :
*/
/*-----------------------------------------------------------------*/
bool GRPSKIN::SetValue(GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS subtype, double value,double base,unsigned int offbound)
{
  return false;
};
