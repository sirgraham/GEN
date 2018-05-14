
/*------------------------------------------------------------------------------------------
//  GRPFILE3DGGF.CPP
//
//  Compress scene into GGF
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 04/04/2017 12:57:09
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPFile3DGGF.h"
#include "GRPScene.h"
#include "GRPObject.h"
#include "GRPLight.h"
#include "GRPLoaderInterface.h"
#include "XDebug.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"

#include "GRPAnimation.h"
#include "AILogic.h"
#include "AIAnimation.h"

#include "GRPDeformer.h"
#include "GRPSkinning.h"

#include "GRPFontManager.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  GRPFILE3DGGF::GRPFILE3DGGF
*/
/**
//
//  Class Constructor GRPFILE3DGGF
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 13:02:23
//
*/
/*-----------------------------------------------------------------*/
GRPFILE3DGGF::GRPFILE3DGGF()
{
  Clean();
  timer=xfactory->CreateTimer();
}

/*-------------------------------------------------------------------
//  GRPFILE3DGGF::~GRPFILE3DGGF
*/
/**
//
//   Class Destructor GRPFILE3DGGF
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 13:02:31
//
*/
/*-----------------------------------------------------------------*/
GRPFILE3DGGF::~GRPFILE3DGGF()
{
  Clear();
  if (textholder)
  delete[]textholder;
  delete(timer);
}

/*-------------------------------------------------------------------
//  GRPFILE3DGGF::Clear
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/05/2017 11:15:56
//
*/
/*-----------------------------------------------------------------*/
void GRPFILE3DGGF::Clear()
{
  OPENGLCHECKERROR(__L("GRPFILE3DGGF::Clear()"));

  Builders.DeleteElementContents();
  Builders.DeleteAll();

  LoadableSections.DeleteAll();

  this->animationcontrols.DeleteAll();
  this->animationlayers.DeleteAll();
  this->animationnodes.DeleteAll();
  this->animationcurves.DeleteAll();
  this->animationstacks.DeleteAll();

  this->nodes.DeleteAll();
  this->lights.DeleteAll();
  this->cameras.DeleteAll();
  this->elements.DeleteAll();
  this->meshes.DeleteAll();
  this->materials.DeleteAll();
  this->textures.DeleteAll();
  this->shaders.DeleteContents();
  this->shaders.DeleteAll();
  this->Properties.DeleteContents();
  this->Properties.DeleteAll();

  this->datacontainers.DeleteContents();
  this->datacontainers.DeleteAll();

  this->ai_animations.DeleteAll();

  if (root)
  {
    delete(root);
    root=NULL;
  }
}

/*-------------------------------------------------------------------
//  GRPFILE3DGGF::Load
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 14:28:08
//
//  @return       GRPOBJECT* :
//
//  @param        file :
*/
/*-----------------------------------------------------------------*/
GRPOBJECT* GRPFILE3DGGF::Load (XCHAR* filename)
{

   Clear();
   Init ();

  XTIMER* timer=xfactory->CreateTimer();
  if (!timer) return NULL;

  timer->Reset();

  //------------------------------------ open file

      XFILE*  file=xfactory->Create_File();
      if (!file->Open(filename))
      {
        GGFDEBUG(GGF_ERROR,__L("Unable to open file for reading : %s"),filename);
        delete file;
        return NULL;
      }

      XBYTE *     result  = NULL;
      XQWORD      size=0;

      //------------------------------------ Load binary data
      size=file->GetSize();

      XBYTE* buffer=new XBYTE[(long)size];
      if (!buffer)
      {
        GGFDEBUG(GGF_ERROR,__L("Unable to locate memory for file : %s : %lld"),filename,size);
        delete(file);
        return NULL;
      }

      if (!file->Read(buffer,(int)size))
      {
        GGFDEBUG(GGF_ERROR,__L("Unable to read file : %s : %lld"),filename,size);
        delete(file);
        return NULL;
      }

      //-------------------------------------- read root block (wich contains the others blocks)
      this->size=size;

      XQWORD position=0;

      GGFSECTIONBUILDER* rootBuilder=Builders.Get(GGF_SECTION_ROOT);
                         rootBuilder->ReadBlock(this,buffer,&position);

      //------------------------------------ Apply Hierarchy, Animations, Logic and Media
      if (!this->Setup())
      {
        GGFDEBUG(GGF_ERROR,__L("Unable to read file : %s : %lld"),filename,size);
        delete(file);
        return NULL;
      }

      file->Close();


  delete(buffer);
  delete(result);
  delete(file);

  XQWORD total=timer->GetMeasureMilliSeconds();

  this->Output(3,__L(" texture time: %lld"),    texture_timer_total);
  this->Output(3,__L(" font time: %lld"),       ttfs_timer_total);
  this->Output(3,__L(" animation time: %lld"),  animation_total);
  this->Output(3,__L(" reading time: %lld"),    total-(animation_total+ttfs_timer_total+texture_timer_total));
  this->Output(3,__L("--------------------------------   GGF::Load : %lld"),total);
  timer->Reset();

  delete(timer);
  return object;
}

/*-------------------------------------------------------------------
//  GRPFILE3DGGF::Setup
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/04/2017 15:58:30
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DGGF::Setup()
{

  //---------------- Apply properties
  XDWORD nP=Properties.GetSize();
  for (XDWORD e=0;e<nP;e++)
  {
    GGFPROPERTY* prop=Properties.FastGet(e);

    if (prop->ownertag==332302016 || prop->referencetag==332302016)
    {
      static int k=0;
      k++;
    }
    switch(prop->ownertype)
    {
    case GGF_SECTION_LOCATOR          : //----------------------------locator
                                      {
                                        GRPNODE* node=NULL;
                                        XDWORD nnodes=this->nodes.GetSize();

                                        if (prop->ownertag!=0)
                                        for (XDWORD e=0;e<nnodes;e++)
                                          if (this->nodes.FastGet(e)->GetTag()==prop->ownertag)
                                          {
                                            node=nodes.FastGet(e);
                                            break;
                                          }


                                        if (!node && prop->ownertag!=0)
                                          this->Output(4,__L("Error lcoalizando node %lld"),prop->ownertag);

                                        if (prop->ownertag==0)
                                          node=object->GetNode();

                                        if (node)
                                        switch(prop->referencetype)
                                        {
                                          case GGF_SECTION_MATERIAL: //------------------ APPLY MATERIAL
                                                                    {
                                                                      GRPMATERIAL* material=NULL;
                                                                      for (XDWORD e=0;e<materials.GetSize();e++)
                                                                        if (materials.FastGet(e)->tag==prop->referencetag)
                                                                        {
                                                                          material=materials.FastGet(e);
                                                                          break;
                                                                        }

                                                                      if (material)
                                                                      {
                                                                        if (node)
                                                                            node->SetMaterial(material);
                                                                      }
                                                                      else
                                                                      {
                                                                        this->Output(4,__L("Error aplicando material"));
                                                                      }
                                                                    }
                                                                    break;

                                          case GGF_SECTION_ELEMENT: //----------------- attach element
                                                                    {
                                                                          GRPELEMENT* element=NULL;
                                                                          for (XDWORD e=0;e<elements.GetSize();e++)
                                                                          if (elements.Get(e)->tag==prop->referencetag)
                                                                          {
                                                                            element=elements.Get(e);
                                                                            element->SetNode(node);
                                                                            break;
                                                                          }

                                                                          if (!element)
                                                                          this->Output(4,__L("Error attacheando el localizador al elemento"));

                                                                    }
                                                                    break;

                                          case GGF_SECTION_DATA:
                                                                    {
                                                                          for (XDWORD e=0;e<datacontainers.GetSize();e++)
                                                                          if (datacontainers.Get(e)->tag==prop->referencetag)
                                                                          {
                                                                            node->GetUserData()->SetUserData(datacontainers.Get(e));
                                                                            break;
                                                                          }
                                                                    }
                                                                    break;

                                          case GGF_SECTION_LIGHT:
                                                                      for (XDWORD e=0;e<lights.GetSize();e++)
                                                                        {
                                                                          if (lights.FastGet(e)->tag==prop->referencetag)
                                                                          {
                                                                            node->LinkTo(lights.FastGet(e));
                                                                            break;
                                                                          }
                                                                        }
                                                                    break;

                                          case GGF_SECTION_CAMERA:

                                                                        for (XDWORD e=0;e<cameras.GetSize();e++)
                                                                        {
                                                                          if (cameras.FastGet(e)->tag==prop->referencetag)
                                                                          {
                                                                            node->LinkTo(cameras.FastGet(e));
                                                                            break;
                                                                          }
                                                                        }

                                                                    break;


                                        case GGF_SECTION_LOCATOR:   //-------------- attach locator
                                                                    {
                                                                      if (prop->referencetag==0)
                                                                      {
                                                                        node->LinkTo(object->GetNode());
                                                                        break;
                                                                      }
                                                                      else
                                                                      {
                                                                        GRPNODE* target=NULL;
                                                                        XDWORD nnodes=nodes.GetSize();
                                                                        for (XDWORD e=0;e<nnodes;e++)
                                                                        {
                                                                          if (nodes.FastGet(e)->tag==prop->referencetag)
                                                                          {
                                                                            target=nodes.FastGet(e);
                                                                            node->LinkTo(target);
                                                                            break;
                                                                          }
                                                                        }

                                                                        if (!target)
                                                                          this->Output(4,__L("Error attacheando locator"));

                                                                      }
                                                                    }
                                                                    break;
                                        }
                                      }
                                      break;


    case GGF_SECTION_ANIMATION        : //---------------------------- animation control
                                        switch(prop->referencetype)
                                        {
                                        case GGF_SECTION_ANIMATION_STACK:
                                                                        {
                                                                            XDWORD nS=this->animationstacks.GetSize();
                                                                            for (XDWORD e=0;e<nS;e++)
                                                                            {
                                                                              GRPANIMATIONSTACK* stack=this->animationstacks.FastGet(e);
                                                                              if (stack->GetID()==prop->PropertyType)
                                                                              {
                                                                                this->animationcontrols.FastGet(0)->AddStack(stack,stack->GetID());
                                                                              }
                                                                            }
                                                                        }
                                                                            break;
                                        }
                                        break;


    case GGF_SECTION_ANIMATION_STACK  : //---------------------------- animation stack
                                      {
                                        GRPANIMATIONSTACK* stack=NULL;
                                        XDWORD nS=this->animationstacks.GetSize();
                                        for (XDWORD e=0;e<nS;e++)
                                        {
                                          stack=this->animationstacks.FastGet(e);
                                          if (stack->tag==prop->ownertag)
                                              break;
                                        }

                                        if (stack)
                                        switch(prop->referencetype)
                                        {
                                        case GGF_SECTION_ANIMATION_LAYER: {
                                                                            XDWORD nl=this->animationlayers.GetSize();
                                                                            for (XDWORD l=0;l<nl;l++)
                                                                            {
                                                                              GRPANIMATIONLAYER* layer=animationlayers.FastGet(l);
                                                                              if (layer->GetTag()==prop->referencetag)
                                                                              {
                                                                                stack->AddLayer(layer);
                                                                                break;
                                                                              }
                                                                            }
                                                                          }
                                                                            break;
                                        case GGF_SECTION_ANIMATION_EVENT:
                                                                          {
                                                                            XDWORD nS=this->animationevents.GetSize();
                                                                            for (XDWORD e=0;e<nS;e++)
                                                                              {
                                                                                GRPANIMATIONSTACKMILESTONE* milestone=this->animationevents.FastGet(e);
                                                                                if (milestone->tag==prop->referencetag)
                                                                                {
                                                                                  stack->AddMilestone(milestone);
                                                                                  break;
                                                                                }
                                                                              }
                                                                          }
                                                                          break;
                                          }
                                          break;
                                        }
                                      break;
    case GGF_SECTION_ANIMATION_NODE   :  //---------------------------- animation node
      {
                                        GRPANIMATIONNODE* node=NULL;

                                        XDWORD nnodes=this->animationnodes.GetSize();

                                        for (XDWORD n=0;n<nnodes;n++)
                                        {
                                          if (this->animationnodes.FastGet(n)->tag==prop->ownertag)
                                          {
                                            node=this->animationnodes.FastGet(n);
                                            this->animationnodes.DeleteIndex(n);
                                            break;
                                          }
                                        }


                                        if (node!=NULL)
                                        switch(prop->referencetype)
                                        {
                                        case GGF_SECTION_CAMERA:  //------------------ camera
                                            for (XDWORD m=0;m<this->cameras.GetSize();m++)
                                            if (this->cameras.Get(m)->tag==prop->referencetag)
                                            {
                                                GRPCAMERA*   camera=this->cameras.Get(m);
                                                             node->SetTarget(camera);
                                                             break;
                                            }
                                            break;

                                        case GGF_SECTION_LIGHT: //------------------ light
                                            for (XDWORD m=0;m<this->lights.GetSize();m++)
                                            if (this->lights.Get(m)->tag==prop->referencetag)
                                            {
                                                GRPLIGHT* light=this->lights.Get(m);
                                                             node->SetTarget(light);
                                                             break;
                                            }
                                            break;

                                        case GGF_SECTION_MATERIAL:  //------------------ material
                                            for (XDWORD m=0;m<this->materials.GetSize();m++)
                                            if (this->materials.FastGet(m)->tag==prop->referencetag)
                                            {
                                                GRPMATERIAL* material=this->materials.Get(m);
                                                             node->SetTarget(material);
                                                             break;
                                            }
                                            break;

                                        case GGF_SECTION_SUBDEFORMER:   //------------------ animation
                                          {
                                            XDWORD nsubs=this->subdeformers.GetSize();
                                            for (XDWORD n=0;n<nsubs;n++)
                                            {
                                              GRPSUBDEFORMER* sub=this->subdeformers.FastGet(n);
                                              if (sub->tag==prop->referencetag)
                                              {
                                                 node->SetTarget(sub);
                                                 break;
                                              }
                                            }
                                          }
                                            break;
                                        case GGF_SECTION_LOCATOR:   //------------------ animation
                                          {
                                            XDWORD nlocators=this->nodes.GetSize();
                                            for (XDWORD n=0;n<nlocators;n++)
                                            {
                                              GRPNODE* locator=this->nodes.FastGet(n);
                                              if (locator->tag==prop->referencetag)
                                              {
                                                 node->SetTarget(locator);
                                                 break;
                                              }
                                            }
                                          }
                                            break;
                                        }
      }
                                        break;


    case GGF_SECTION_MATERIAL         : //----------------------------- material
                                      {

                                        GRPMATERIAL* material=NULL;
                                        for (XDWORD e=0;e<materials.GetSize();e++)
                                          if (materials.FastGet(e)->tag==prop->ownertag)
                                            material=materials.FastGet(e);

                                        if (material)
                                        {
                                          GRPTEXTURE* texture=NULL;
                                          for (XDWORD e=0;e<textures.GetSize();e++)
                                            if (textures.FastGet(e)->tag==prop->referencetag)
                                              texture=textures.FastGet(e);

                                          //GRPTEXTURE* texture=GRPMATERIALLIBRARY::Get()->GetTexture(prop->reference.Get());
                                                  if (texture)
                                                              material->AddLayer(prop->PropertyType,texture);
                                                  else
                                                  {
                                                    XDEBUG_PRINTCOLOR(4,__L("texture not found"));
                                                  }
                                        }
                                      }
                                      break;
    case GGF_SECTION_ELEMENT          : //------------------------------- element
                                      {
                                        GRPELEMENT* element=NULL;//object->FindElement(prop->owner.Get());
                                        for (XDWORD e=0;e<elements.GetSize();e++)
                                          if (elements.FastGet(e)->tag==prop->ownertag)
                                            element=elements.FastGet(e);

                                        if (element)
                                        switch(prop->referencetype)
                                        {

                                          //--------------------------------------------------------- APPLY SHADER
                                          case GGF_SECTION_SHADER:
                                                                    {
                                                                      for (XDWORD e=0;e<shaders.GetSize();e++)
                                                                        if (shaders.FastGet(e)->tag==prop->referencetag)
                                                                        {
                                                                          GRPSHADERPROGRAM* shader=GRPSHADERLIBRARY::Get()->GetProgramByName(shaders.FastGet(e)->GetName()->Get());
                                                                          if (shader)
                                                                          {
                                                                            element->SetProgram(shader);
                                                                          }
                                                                        }
                                                                    }
                                                                    break;
                                          case GGF_SECTION_MESH:
                                                                    {
                                                                      for (XDWORD c=0;c<this->meshes.GetSize();c++)
                                                                      {
                                                                        GRPMESH* mesh=meshes.Get(c);
                                                                        if (mesh->tag==prop->referencetag)
                                                                        {
                                                                          element->SetMesh(mesh);
                                                                          break;
                                                                        }
                                                                      }
                                                                    }
                                                                    break;
                                        }
                                      }
                                      break;

    case GGF_SECTION_SKINCLUSTER      :
                                      {
                                          GRPSKINCLUSTER* cluster=NULL;
                                          for (XDWORD e=0;e<this->skincluster.GetSize();e++)
                                          {
                                            if (skincluster.FastGet(e)->tag==prop->ownertag)
                                            {
                                              cluster=skincluster.FastGet(e);
                                              break;
                                            }
                                          }
                                          if (cluster)
                                          {
                                            if (prop->referencetag!=0)
                                            switch(prop->referencetype)
                                            {
                                            case GGF_SECTION_LOCATOR:   //------------------ animation
                                                {
                                                  XDWORD nlocators=this->nodes.GetSize();
                                                  for (XDWORD n=0;n<nlocators;n++)
                                                  {
                                                    GRPNODE* locator=this->nodes.FastGet(n);
                                                    if (locator->tag==prop->referencetag)
                                                    {
                                                       cluster->GetLimbs()->Add((GRPSKINLIMB*)locator);
                                                       break;
                                                    }
                                                  }
                                                }
                                                break;
                                            }
                                          }
                                      }
                                      break;
    case GGF_SECTION_SKINPOSE         :
                                      {
                                        GRPSKINPOSE* pose=NULL;

                                          for (XDWORD e=0;e<this->skinpose.GetSize();e++)
                                          {
                                            if (skinpose.FastGet(e)->tag==prop->ownertag)
                                            {
                                              pose=skinpose.FastGet(e);
                                              break;
                                            }
                                          }

                                        if (pose)
                                        {
                                          if (prop->referencetag!=0)
                                          switch(prop->referencetype)
                                          {
                                          case GGF_SECTION_LOCATOR:   //------------------ animation
                                              {
                                                XDWORD nlocators=this->nodes.GetSize();
                                                for (XDWORD n=0;n<nlocators;n++)
                                                {
                                                  GRPNODE* locator=this->nodes.FastGet(n);
                                                  if (locator->tag==prop->referencetag)
                                                  {
                                                    pose->Node=(GRPSKINLIMB*)locator;
                                                    break;
                                                  }
                                                }
                                              }
                                              break;
                                          }
                                        }
                                      }
                                      break;
    case GGF_SECTION_LOGIC_ANIMATION:
                                      {
                                        XDWORD n=ai_animations.GetSize();
                                        AI::ANIMATION* ani=NULL;
                                        for (XDWORD e=0;e<n;e++)
                                        {
                                          AI::ANIMATION* test=ai_animations.FastGet(e);
                                          if (test->id==prop->ownertag)
                                          {
                                            ani=test;
                                            break;
                                          }
                                        }

                                        if (ani)
                                        {
                                          AI::ANIMATION* attach=new AI::ANIMATION(prop->referencetag,0,0);
                                          ani->Attach(attach);
                                        }

                                      break;
                                      }
    default:                          break;
    }
  }



  object->MergeAnimations();
  object->CreateTextLocators();


  //---------------------------- default shaders

  for (XDWORD e=0;e<object->GetElements()->GetSize();e++)
  {
      if (object->GetElements()->Get(e)->GetProgram() == NULL)
      {
          object->GetElements()->Get(e)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("DOT3")));

          if (object->GetElements()->Get(e)->GetNode()->GetMaterial() == NULL)
              object->GetElements()->Get(e)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("DOTDiff")));
          else
          if (object->GetElements()->Get(e)->GetNode()->GetMaterial()->GetLayers()->GetSize() == 0)
              object->GetElements()->Get(e)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("DOTDiff")));
      }
  }



  //---------------------------- init Morphers
  for (XDWORD e=0;e<object->GetElements()->GetSize();e++)
   if (object->GetElements()->FastGet(e)->GetDeformer()!=NULL)
       object->GetElements()->FastGet(e)->GetDeformer()->Init();

  //---------------------------- init Skinning
  for (XDWORD e=0;e<object->GetElements()->GetSize();e++)
   if (object->GetElements()->FastGet(e)->GetSkin()!=NULL)
       object->GetElements()->FastGet(e)->GetSkin()->Init();

  if (object->GetAnimationControl())
    object->GetAnimationControl()->Begin();

  if (object->GetLogic())
    object->GetLogic()->GetControllers()->Get(0)->ChangeToDefaultState();

  object->GetAnimationControl()->Play();

  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DGGF::Save
*/
/**
//
//  Saves the content of the selected scene to binary file
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 13:02:37
//
//  @return       bool : result , false on failure (GetLastError contains cause)
//
//  @param        file : XCHAR* name of the file to be generated
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DGGF::Save(XCHAR* filename)
{
      //------------------------------------ open file

      XFILE*  file=xfactory->Create_File();
      if (!file->Create(filename))
      {
        GGFDEBUG(GGF_ERROR,__L("Unable to create file for writing : %s"),filename);
        delete(file);
        return false;
      }

      XBYTE * result=NULL;
      XQWORD size=0,total_size=0;

      //------------------------------------ export data
      GGFSECTIONBUILDER* rootBuilder=Builders.Get(GGF_SECTION_ROOT);
      root=rootBuilder->CreateBlock(this);
      if (!root)
      {
        GGFDEBUG(GGF_ERROR,__L("Failed to fill data"),filename);
        delete(file);
        return false;
      }

      //------------------------------------ calculate total file size

      total_size=root->GetSize();
      result = new XBYTE[(int)total_size];
      if (!result)
      {
        GGFDEBUG(GGF_ERROR,__L("Failed to locate memory: %d"),total_size);
        delete(file);
        return false;
      }



      //------------------------------------ create a single block
      size=0;
      if (!root->MergeBlocks(&result,&size))
      {
        GGFDEBUG(GGF_ERROR,__L("Failed to create dictionary"),filename);
        delete(file);
        return false;
      }

      //------------------------------------ write to disk

      if (!file->Write(result,(int)size))
      {
        GGFDEBUG(GGF_ERROR,__L("Unable to create file for writing : %s"),filename);
        delete(file);
        return false;
      }

      //-------------------------------------end

      file->Close();

  delete(result);
  delete(file);
  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DGGF::SetScene
*/
/**
//  Select the scene to export
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 13:02:53
//
//  @param        scene : GRPSCENE* : scene to export
*/
/*-----------------------------------------------------------------*/
void GRPFILE3DGGF::SetScene(GRPSCENE* scene)
{
  this->scene=scene;
}

/*-------------------------------------------------------------------
//  GRPFILE3DGGF::GetScene
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 16:49:03
//
//  @return       GRPSCENE* :
//
*/
/*-----------------------------------------------------------------*/
GRPSCENE*  GRPFILE3DGGF::GetScene()
{
  return this->scene;
}

/*-------------------------------------------------------------------
//  GRPFILE3DGGF::Init
*/
/**
//  Inits the builder with the preferred version
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 14:33:34
//
//  @param        mayorversion : mayor version
//  @param        minorversion : minor version
*/
/*-----------------------------------------------------------------*/
void GRPFILE3DGGF::Init()
{
  this->majorversion=MAYOR_VERSION;
  this->minorversion=MINOR_VERSION;


  materials.DeleteContents();
  materials.DeleteAll();
  textures.DeleteContents();
  textures.DeleteAll();

  animationcontrols.DeleteContents();
  animationcontrols.DeleteAll();

  animationstacks.DeleteContents();
  animationstacks.DeleteAll();

  animationlayers.DeleteContents();
  animationlayers.DeleteAll();

  animationnodes.DeleteContents();
  animationnodes.DeleteAll();

  animationcurves.DeleteContents();
  animationcurves.DeleteAll();

  animationevents.DeleteContents();
  animationevents.DeleteAll();


  Builders.DeleteElementContents();
  Builders.DeleteAll();

  this->InitSectionBuilders();

  isinitialized=true;

  nobjects=0;


  return;
}



/*-------------------------------------------------------------------
//  GRPFILE3DGGF::InitSectionBuilders
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 15:43:56
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool   GRPFILE3DGGF::InitSectionBuilders()
{
  LoadableSections.DeleteAll();

  LoadableSections.Add(GGF_SECTION_DATA);
  LoadableSections.Add(GGF_SECTION_ROOT);
  LoadableSections.Add(GGF_SECTION_DICTIONARY);
  LoadableSections.Add(GGF_SECTION_PROPERTY);
  LoadableSections.Add(GGF_SECTION_HEADER);
  LoadableSections.Add(GGF_SECTION_FONT);

  LoadableSections.Add(GGF_SECTION_OBJECT);
  LoadableSections.Add(GGF_SECTION_ELEMENT);
  LoadableSections.Add(GGF_SECTION_CAMERA);
  LoadableSections.Add(GGF_SECTION_LIGHT);
  LoadableSections.Add(GGF_SECTION_LOCATOR);
  LoadableSections.Add(GGF_SECTION_MESH);
  LoadableSections.Add(GGF_SECTION_MESH_LAYER);

  LoadableSections.Add(GGF_SECTION_MATERIAL);
  LoadableSections.Add(GGF_SECTION_TEXTURE);
  LoadableSections.Add(GGF_SECTION_SHADER);

  LoadableSections.Add(GGF_SECTION_ANIMATION);
  LoadableSections.Add(GGF_SECTION_ANIMATION_STACK);
  LoadableSections.Add(GGF_SECTION_ANIMATION_LAYER);
  LoadableSections.Add(GGF_SECTION_ANIMATION_NODE);
  LoadableSections.Add(GGF_SECTION_ANIMATION_CURVE);
  LoadableSections.Add(GGF_SECTION_ANIMATION_EVENT);

  LoadableSections.Add(GGF_SECTION_LOGIC);
  LoadableSections.Add(GGF_SECTION_LOGIC_CONTROLLER);
  LoadableSections.Add(GGF_SECTION_LOGIC_STATE);
  LoadableSections.Add(GGF_SECTION_LOGIC_ANIMATION);
  LoadableSections.Add(GGF_SECTION_LOGIC_OVERLAY);

  LoadableSections.Add(GGF_SECTION_DEFORMER);
  LoadableSections.Add(GGF_SECTION_SUBDEFORMER);
  LoadableSections.Add(GGF_SECTION_SKIN);
  LoadableSections.Add(GGF_SECTION_SKINPOSE);
  LoadableSections.Add(GGF_SECTION_SKINCLUSTER);


  for (XDWORD e=0;e<LoadableSections.GetSize();e++)
  {
    GGF_SECTION_TYPE    section         = LoadableSections.Get  (e);
    GGFSECTIONBUILDER*  sectionbuilder  = CreateSectionBuilder  ( section,
                                                                    majorversion,
                                                                    minorversion);
                          Builders.Add  (section,sectionbuilder);
  }
  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DGGF::MergeBlocks
*/
/**
//
//  Merges all the blocks and generates dictionary
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 16:21:39
//
//  @return       bool :
//
//  @param        result :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool   GGFSECTIONBLOCK::MergeBlocks(XBYTE** result, XQWORD* size)
{
  for (XDWORD e=0;e<this->Blocks.GetSize();e++)
  {
    memcpy(
            &(*result)[*size],
            this->Blocks.Get(e)->Get(),
            (size_t)this->Blocks.Get(e)->GetHeaderSize()
          );
    *size+=this->Blocks.Get(e)->GetHeaderSize();
    this->Blocks.Get(e)->MergeBlocks(result,size);
  }
  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DGGF::CreateSectionBuilder
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 15:44:03
//
//  @return       GGFSECTIONBUILDER* :
//
//  @param        GGFSECTION :
//  @param        majorversion :
//  @param        minorversion :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBUILDER*   GRPFILE3DGGF::CreateSectionBuilder(GGF_SECTION_TYPE section,XDWORD majorversion,XDWORD minorversion)
{
  GGFSECTIONBUILDER* builder=NULL;
  switch(section)
  {
  case GGF_SECTION_DATA             : builder=new GGFDATA_BUILDER             (); break;
  case GGF_SECTION_ROOT             : builder=new GGFROOT_BUILDER             (); break;
  case GGF_SECTION_DICTIONARY       : builder=new GGFDICTIONARY_BUILDER       (); break;
  case GGF_SECTION_PROPERTY         : builder=new GGFPROPERTY_BUILDER         (); break;
  case GGF_SECTION_HEADER           : builder=new GGFHEADER_BUILDER           (); break;
  case GGF_SECTION_FONT             : builder=new GGFFONT_BUILDER             (); break;
  case GGF_SECTION_OBJECT           : builder=new GGFOBJECT_BUILDER           (); break;
  case GGF_SECTION_ELEMENT          : builder=new GGFELEMENT_BUILDER          (); break;
  case GGF_SECTION_LOCATOR          : builder=new GGFLOCATOR_BUILDER          (); break;
  case GGF_SECTION_CAMERA           : builder=new GGFCAMERA_BUILDER           (); break;
  case GGF_SECTION_LIGHT            : builder=new GGFLIGHT_BUILDER            (); break;
  case GGF_SECTION_MESH             : builder=new GGFMESH_BUILDER             (); break;
  case GGF_SECTION_MESH_LAYER       : builder=new GGFMESH_LAYER_BUILDER       (); break;
  case GGF_SECTION_MATERIAL         : builder=new GGFMATERIAL_BUILDER         (); break;
  case GGF_SECTION_TEXTURE          : builder=new GGFTEXTURE_BUILDER          (); break;
  case GGF_SECTION_ANIMATION        : builder=new GGFANIMATION_BUILDER        (); break;
  case GGF_SECTION_ANIMATION_STACK  : builder=new GGFANIMATION_STACK_BUILDER  (); break;
  case GGF_SECTION_ANIMATION_LAYER  : builder=new GGFANIMATION_LAYER_BUILDER  (); break;
  case GGF_SECTION_ANIMATION_NODE   : builder=new GGFANIMATION_NODE_BUILDER   (); break;
  case GGF_SECTION_ANIMATION_CURVE  : builder=new GGFANIMATION_CURVE_BUILDER  (); break;
  case GGF_SECTION_ANIMATION_EVENT  : builder=new GGFANIMATION_EVENT_BUILDER  (); break;
  case GGF_SECTION_LOGIC            : builder=new GGFLOGIC_BUILDER            (); break;
  case GGF_SECTION_LOGIC_CONTROLLER : builder=new GGFLOGIC_CONTROLLER_BUILDER (); break;
  case GGF_SECTION_LOGIC_STATE      : builder=new GGFLOGIC_STATE_BUILDER      (); break;
  case GGF_SECTION_LOGIC_ANIMATION  : builder=new GGFLOGIC_ANIMATION_BUILDER  (); break;
  case GGF_SECTION_LOGIC_OVERLAY    : builder=new GGFLOGIC_OVERLAY_BUILDER    (); break;
  case GGF_SECTION_DEFORMER         : builder=new GGFDEFORMER_BUILDER         (); break;
  case GGF_SECTION_SUBDEFORMER      : builder=new GGFSUBDEFORMER_BUILDER      (); break;
  case GGF_SECTION_SKIN             : builder=new GGFSKIN_BUILDER             (); break;
  case GGF_SECTION_SKINCLUSTER      : builder=new GGFSKINCLUSTER_BUILDER      (); break;
  case GGF_SECTION_SKINPOSE         : builder=new GGFSKINPOSE_BUILDER         (); break;
  case GGF_SECTION_SHADER           : builder=new GGFSHADER_BUILDER           (); break;

  default:
    GGFDEBUG(GGF_ERROR,__L("Unknown section type. Requested %d %d:%d Current %d:%d"),section,majorversion,minorversion,this->majorversion,this->minorversion);
  }

  return builder;
}


/*-------------------------------------------------------------------
//  GRPFILE3DGGF::Output
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/03/2017 11:55:40
//
//  @param        priority :
//  @param        mask :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
void GRPFILE3DGGF::Output(XDWORD priority,XCHAR* mask,...)
{
    XSTRING s;
    va_list argptr;
    va_start(argptr, mask);
    s.FormatArg(mask,&argptr);

    /*
      if (task)
        this->task->Print(priority,s.Get());
      else
      {
        XDEBUG_PRINTCOLOR(priority,s.Get());
      }
      */
    XDEBUG_PRINTCOLOR(priority, s.Get());
    va_end(argptr);

}

/*-------------------------------------------------------------------
//  GRPFILE3DGGF::WriteString
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/06/2017 17:06:16
//
//  @return       bool :
//
//  @param        string :
//  @param        pos :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DGGF::WriteString(XSTRING* string,GGFSECTIONBLOCK* block,XQWORD* pos)
{
  XWORD* text=NULL;
  string->CreateNormalize(text);

  XWORD size=string->GetSize(); memcpy(&(block->Get()[*pos]), &size,  sizeof(XWORD));   *pos=*pos+sizeof(XWORD);
  if (size)
  {
      memcpy(&(block->Get()[*pos]), text, size*sizeof(XWORD));                          *pos=*pos+(size*sizeof(XWORD));
  }

  delete(text);
  return true;
}

/*-------------------------------------------------------------------
//  GRPFILE3DGGF::ReadString
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/06/2017 17:06:27
//
//  @return       bool :
//
//  @param        string :
//  @param        position :
//  @param        offset :
*/
/*-----------------------------------------------------------------*/
bool GRPFILE3DGGF::ReadString(XSTRING* string,XBYTE* buffer, XQWORD* position, XQWORD* offset)
{
  XWORD size=0; memcpy(&size, &(buffer[*offset]),   sizeof(XWORD));   *offset=*offset+sizeof(XWORD);


  if (size>textholdersize)
  {
  textholdersize=size;
  delete[] (textholder);
  if (size==0) return false;
  textholder=new XWORD[textholdersize+1];
  }

  memcpy(textholder, &(buffer[*offset]),    size*sizeof(XWORD));    *offset=*offset+(size*sizeof(XWORD));
  textholder[size]=__C('\0');
  string->Set((XWORD*)textholder);

  return true;
}


/*-------------------------------------------------------------------
//  GGFDICTIONARY_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/05/2017 16:53:54
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        v :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFDICTIONARY_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* v)
{

  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK*  block=new GGFSECTIONBLOCK();
                    block->SetType(GGF_SECTION_DICTIONARY);

  XDWORD calc_size =1*sizeof(XBYTE)+ //TYPE OBJECT
                    1*sizeof(XDWORD)+ //SIZE
                    1*sizeof(XDWORD)+ //number of entries to load
                    f3d->GetDictionary()->GetSize()*(sizeof(XQWORDSIG)+sizeof(XDWORD)); //entries

  block->Set(calc_size);
  XQWORD pos=0;

  XBYTE   type= GGF_SECTION_DICTIONARY;               memcpy(&(block->Get()[pos]),  &type,      sizeof(XBYTE));     pos+=sizeof(XBYTE);
                                                      memcpy(&(block->Get()[pos]),  &calc_size, sizeof(XDWORD));    pos+=sizeof(XDWORD);
  XDWORD  nentries=f3d->GetDictionary()->GetSize();   memcpy(&(block->Get()[pos]),  &nentries,  sizeof(XDWORD));    pos+=sizeof(XDWORD);

  for (XDWORD e=0;e<nentries;e++)
  {
    XQWORDSIG tag =f3d->GetDictionary()->GetKey(e);
    XDWORD    type=f3d->GetDictionary()->GetElement(e);

    memcpy(&(block->Get()[pos]),  &tag,   sizeof(XQWORDSIG));   pos+=sizeof(XQWORDSIG);
    memcpy(&(block->Get()[pos]),  &type,  sizeof(XDWORD));      pos+=sizeof(XDWORD);
  }

  return block;
}

/*-------------------------------------------------------------------
//  GGFDICTIONARY_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/05/2017 18:09:08
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool  GGFDICTIONARY_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void* parent)
{
  XQWORD pos=*position;

  XBYTE   type        =0;
  XDWORD  size        =0;
  XDWORD  nentries    =0;

  memcpy(&type,   &(buffer[pos]),     sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_DICTIONARY)
  {
    f3d->Output(4,__L("GGF_SECTION_DICTIONARY::ReadBlock Unexpected Section! : %d"),type);
    return false;
  }
  memcpy(&size,     &(buffer[pos]),     sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&nentries, &(buffer[pos]),     sizeof(XDWORD));              pos+=sizeof(XDWORD);

  f3d->GetDictionary()->SetAddInLimit(nentries);
  for (XDWORD e=0;e<nentries;e++)
  {
    XQWORDSIG  tag =0;
    XDWORD     type=0;

    memcpy(&tag,  &(buffer[pos]),     sizeof(XQWORDSIG));             pos+=sizeof(XQWORDSIG);
    memcpy(&type, &(buffer[pos]),     sizeof(XDWORD));                pos+=sizeof(XDWORD);

    f3d->GetDictionary()->Add(tag,(GGF_SECTION_TYPE)type);
  }

  *position=pos;

  return true;
}


/*-------------------------------------------------------------------
//  GGFPROPERTY_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/05/2017 12:07:22
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        root :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFPROPERTY_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* root)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK*  block=new GGFSECTIONBLOCK();
                    block->SetType(GGF_SECTION_PROPERTY);

  int calc_size=1*sizeof(XDWORD); //number of properties to load


  for (XDWORD e=0;e<f3d->GetProperties()->GetSize();e++)
  {
    GGFPROPERTY* prop=f3d->GetProperties()->Get(e);

    calc_size+=sizeof(XDWORD);                          //property type

    calc_size+=sizeof(XDWORD);                          //prop->owner->type
    calc_size+=sizeof(XDWORD);                          //prop->owner->tag

    calc_size+=sizeof(XDWORD);                          //prop->reference->type
    calc_size+=sizeof(XDWORD);                          //prop->reference->tag

  }

  block->Set(calc_size);
  XQWORD pos=0;

  //-----
  XDWORD nprops=f3d->GetProperties()->GetSize();
  memcpy(&(block->Get()[pos]),&nprops,                        sizeof(XDWORD));      pos+=1*sizeof(XDWORD);

  //------
  for (XDWORD e=0;e<f3d->GetProperties()->GetSize();e++)
  {
    GGFPROPERTY* prop=f3d->GetProperties()->Get(e);

    //-----
    memcpy(&(block->Get()[pos]),&prop->PropertyType,          sizeof(XDWORD));      pos+=1*sizeof(XDWORD);

    //-----
    memcpy(&(block->Get()[pos]),&prop->ownertype,             sizeof(XDWORD));      pos+=1*sizeof(XDWORD);
    memcpy(&(block->Get()[pos]),&prop->ownertag,              sizeof(XDWORD));      pos+=1*sizeof(XDWORD);

    //-----
    memcpy(&(block->Get()[pos]),&prop->referencetype,         sizeof(XDWORD));      pos+=1*sizeof(XDWORD);
    memcpy(&(block->Get()[pos]),&prop->referencetag,          sizeof(XDWORD));      pos+=1*sizeof(XDWORD);
  }


  return block;
}


/*-------------------------------------------------------------------
//  GGFPROPERTY_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/05/2017 12:32:47
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
*/
/*-----------------------------------------------------------------*/
bool GGFPROPERTY_BUILDER::ReadBlock   (GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{

  GRPOBJECT* object=f3d->object;

  XQWORD pos=*position;

  XBYTE   type        =0;
  XDWORD  size        =0;
  XDWORD  name_length =0;

  XDWORD number=0;

  memcpy(&number,         &(buffer[pos]),     sizeof(XDWORD));                pos+=sizeof(XDWORD);

  f3d->GetProperties()->DeleteContents();
  f3d->GetProperties()->DeleteAll();
  f3d->GetProperties()->SetAddInLimit(number);

  XDWORD e=0;
  for (e=0;e<number;e++)
  {
    GGFPROPERTY* property=new GGFPROPERTY();
    if (property==NULL) return false;

    //-----
    memcpy(&property->PropertyType,   &(buffer[pos]),     sizeof(XDWORD));      pos+=1*sizeof(XDWORD);

    //------
    memcpy(&property->ownertype,      &(buffer[pos]),     sizeof(XDWORD));      pos+=1*sizeof(XDWORD);
    memcpy(&property->ownertag,       &(buffer[pos]),     sizeof(XDWORD));      pos+=1*sizeof(XDWORD);

    //------
    memcpy(&property->referencetype,  &(buffer[pos]),     sizeof(XDWORD));      pos+=1*sizeof(XDWORD);
    memcpy(&property->referencetag,   &(buffer[pos]),     sizeof(XDWORD));      pos+=1*sizeof(XDWORD);
    f3d->GetProperties()->Add(property);
  }

  f3d->Output(0,__L("Loaded %d/%d Properties"),e,number);

  f3d->Output(0,__L("Position %lld %lld"),*position,pos);
  *position=pos;
  return true;
}


/*-------------------------------------------------------------------
//  GGFMEDIAFILE_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/05/2017 11:55:52
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        media :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFMEDIAFILE_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* media)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK*  block=new GGFSECTIONBLOCK();
                    block->SetType(GGF_SECTION_MEDIA);

  return block;
}

bool GGFMEDIAFILE_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  return true;
}

/*-------------------------------------------------------------------
//  GGF::ROOT_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 16:47:59
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFROOT_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* root)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK*  block=new GGFSECTIONBLOCK();
                    block->SetType(GGF_SECTION_ROOT);

  //---------------------- save header
    block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_HEADER)->CreateBlock(f3d));

  //------------- ponemos los shaders antes de los objetos
  for (XDWORD e=0;e<GRPSHADERLIBRARY::Get()->GetCompiledPrograms()->GetSize();e++)
  {
  GRPSHADERPROGRAM* shader=GRPSHADERLIBRARY::Get()->GetProgram(e);
  block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_SHADER)->CreateBlock(f3d,shader));
  f3d->GetDictionary()->Add((XQWORDSIG)shader,GGF_SECTION_SHADER);
  }

  //---------------------- save fonts
  for (XDWORD e=0;e<GRPOBJECTBUILDER::Get()->GetFontManager()->GetFonts()->GetSize();e++)
  {
    GRPFONT* font=GRPOBJECTBUILDER::Get()->GetFontManager()->GetFonts()->GetElement(e);
    block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_FONT)->CreateBlock(f3d,font));
    f3d->GetDictionary()->Add((XQWORDSIG)font,GGF_SECTION_FONT);
  }

  //---------------------- save objects
  for (XDWORD e=0;e<scene->GetObjects()->GetSize();e++)
  {
    GRPOBJECT* object=scene->GetObjects()->Get(e);
    block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_OBJECT)->CreateBlock(f3d,object));
    f3d->GetDictionary()->Add((XQWORDSIG)object,GGF_SECTION_OBJECT);
  }

  //---------------------- save materials
  for (XDWORD e=0;e<GRPMATERIALLIBRARY::Get()->GetLibrary()->GetSize();e++)
  {
    GRPMATERIAL* material=GRPMATERIALLIBRARY::Get()->GetLibrary()->GetElement(e);
    block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_MATERIAL)->CreateBlock(f3d,material));
    f3d->GetDictionary()->Add((XQWORDSIG)material,GGF_SECTION_MATERIAL);
  }

  //---------------------- save texture
  for (XDWORD e=0;e<GRPMATERIALLIBRARY::Get()->GetTextures()->GetSize();e++)
  {
    GRPTEXTURE* texture=GRPMATERIALLIBRARY::Get()->GetTextures()->GetElement(e);
    block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_TEXTURE)->CreateBlock(f3d,texture));
    f3d->GetDictionary()->Add((XQWORDSIG)texture,GGF_SECTION_TEXTURE);
  }

  //---------------------- save animations
  for (XDWORD e=0;e<scene->GetObjects()->GetSize();e++)
  {
    f3d->object=scene->GetObjects()->Get(e);
    GRPANIMATIONCONTROL* animationcontrol=scene->GetObjects()->Get(e)->GetAnimationControl();
    block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_ANIMATION)->CreateBlock(f3d,animationcontrol));
    f3d->GetDictionary()->Add((XQWORDSIG)animationcontrol,GGF_SECTION_ANIMATION);
  }

  int order=1;

  //------------- primero el diccionario
  GGFSECTIONBLOCK* dictionary =f3d->GetBuilders()->Get(GGF_SECTION_DICTIONARY)->CreateBlock(f3d);
  block->Blocks.Insert(order++,dictionary);

  //------------- ahora que ya conocmemos las propiedades la ponemos lo siguiente
  GGFSECTIONBLOCK* properties =f3d->GetBuilders()->Get(GGF_SECTION_PROPERTY)->CreateBlock(f3d);
  block->Blocks.Insert(order++,properties);



  //--------- finish
  block->Set(0);
  return block;
}


/*-------------------------------------------------------------------
//  GGF::ROOT_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 17:02:30
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        GRPFILE3DGGF* :
*/
/*-----------------------------------------------------------------*/
bool GGFROOT_BUILDER::ReadBlock   (GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  //on the future, the order will be independent, now is sequential
  f3d->percent =0.0f;

  if (!f3d->GetBuilders()->Get(GGF_SECTION_HEADER     )->ReadBlock(f3d,buffer,position))    return false;
  if (!f3d->GetBuilders()->Get(GGF_SECTION_DICTIONARY )->ReadBlock(f3d,buffer,position))    return false;
  if (!f3d->GetBuilders()->Get(GGF_SECTION_PROPERTY   )->ReadBlock(f3d,buffer,position))    return false;

  for (XDWORD e=0;e<f3d->nshaders;e++)
  if (!f3d->GetBuilders()->Get(GGF_SECTION_SHADER     )->ReadBlock(f3d,buffer,position))    return false;

  f3d->percent += 16.0f;
  f3d->Publish(GRPMSG_STEP);

  for (XDWORD e=0;e<f3d->nfonts;e++)
  if (!f3d->GetBuilders()->Get(GGF_SECTION_FONT       )->ReadBlock(f3d,buffer,position,(void*)GRPOBJECTBUILDER::Get()->GetFontManager()))   return false;

  f3d->percent += 16.0f;
  f3d->Publish(GRPMSG_STEP);

  for (XDWORD e=0;e<f3d->nobjects;e++)
  if (!f3d->GetBuilders()->Get(GGF_SECTION_OBJECT     )->ReadBlock(f3d,buffer,position))    return false;

  f3d->percent += 16.0f;
  f3d->Publish(GRPMSG_STEP);

  for (XDWORD e=0;e<f3d->nmaterials;e++)
  if (!f3d->GetBuilders()->Get(GGF_SECTION_MATERIAL   )->ReadBlock(f3d,buffer,position))    return false;

  f3d->percent += 16.0f;
  f3d->Publish(GRPMSG_STEP);

  for (XDWORD e=0;e<f3d->ntextures;e++)
  if (!f3d->GetBuilders()->Get(GGF_SECTION_TEXTURE    )->ReadBlock(f3d,buffer,position))    return false;

  f3d->percent += 16.0f;
  f3d->Publish(GRPMSG_STEP);

  for (XDWORD e=0;e<f3d->nobjects;e++)
  if (!f3d->GetBuilders()->Get(GGF_SECTION_ANIMATION  )->ReadBlock(f3d,buffer,position))    return false;

  f3d->percent += 16.0f;
  f3d->Publish(GRPMSG_STEP);

//if (!f3d->GetBuilders()->Get(GGF_SECTION_PHYSICS    )->ReadBlock(f3d,buffer,position))    return false;
//if (!f3d->GetBuilders()->Get(GGF_SECTION_SOUNDS     )->ReadBlock(f3d,buffer,position))    return false;

  return true;
}



/*-------------------------------------------------------------------
//  GGF::HEADER_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 16:47:59
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFHEADER_BUILDER::CreateBlock(GRPFILE3DGGF* f3d,  void* header)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK*  block=new GGFSECTIONBLOCK();
                    block->SetType(GGF_SECTION_HEADER);

  int calc_size=
                3*sizeof(XBYTE)+ //GGF mark
                2*sizeof(XBYTE)+ //majorversion,minorversion
                1*sizeof(XBYTE)+ //nsections

                1*sizeof(XBYTE)+ //nfonts
                1*sizeof(XBYTE)+ //nobjects
                1*sizeof(XBYTE)+ //nmaterials
                1*sizeof(XBYTE)+ //ntextures
                1*sizeof(XBYTE); //nshaders


  block->Set(calc_size);

  if (block->Get())
  {
  XQWORD pos=0;
    const char mark[]=("GGF");
    memcpy(&(block->Get()[pos]),mark,sizeof(mark));           pos+=3*sizeof(XBYTE);
    memcpy(&(block->Get()[pos]),&majorversion,sizeof(XBYTE)); pos+=sizeof(XBYTE);
    memcpy(&(block->Get()[pos]),&minorversion,sizeof(XBYTE)); pos+=sizeof(XBYTE);

    XBYTE nsections=5;
    memcpy(&(block->Get()[pos]),&nsections,   sizeof(XBYTE)); pos+=sizeof(XBYTE);

    //nfonts
    XBYTE nfonts=GRPOBJECTBUILDER::Get()->GetFontManager()->GetFonts()->GetSize();
    memcpy(&(block->Get()[pos]),&nfonts,    sizeof(XBYTE)); pos+=sizeof(XBYTE);

    //nobjects
    XBYTE nobjects=scene->GetObjects()->GetSize();
    memcpy(&(block->Get()[pos]),&nobjects,    sizeof(XBYTE)); pos+=sizeof(XBYTE);

    //nmaterials
    XBYTE nmaterials=GRPMATERIALLIBRARY::Get()->GetLibrary()->GetSize();
    memcpy(&(block->Get()[pos]),&nmaterials,  sizeof(XBYTE)); pos+=sizeof(XBYTE);

    //ntextures
    XBYTE ntextures=GRPMATERIALLIBRARY::Get()->GetTextures()->GetSize();
    memcpy(&(block->Get()[pos]),&ntextures,   sizeof(XBYTE)); pos+=sizeof(XBYTE);

    //nshaders
    XBYTE nshaders=GRPSHADERLIBRARY::Get()->GetCompiledPrograms()->GetSize();
    memcpy(&(block->Get()[pos]),&nshaders,    sizeof(XBYTE)); pos+=sizeof(XBYTE);
  }

  return block;
}

/*-------------------------------------------------------------------
//  GGF::HEADER_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 17:02:30
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        GRPFILE3DGGF* :
*/
/*-----------------------------------------------------------------*/
bool  GGFHEADER_BUILDER::ReadBlock    (GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  XBYTE mark[3];

  XQWORD pos=*position;

    memcpy(mark,          &(buffer[pos]), sizeof(mark));            pos+=3*sizeof(XBYTE);
    memcpy(&majorversion, &(buffer[pos]), sizeof(XBYTE));           pos+=sizeof(XBYTE);
    memcpy(&minorversion, &(buffer[pos]), sizeof(XBYTE));           pos+=sizeof(XBYTE);

    if (mark[0]!=__C('G') || mark[1]!=__C('G') || mark[2]!=__C('F'))
    {
      f3d->Output(4,__L("File mark failed reading file: %s "),f3d->filename.Get());
      return false;
    }

    XBYTE nsections=0;
    memcpy(&nsections,  &(buffer[pos]), sizeof(XBYTE));           pos+=sizeof(XBYTE);

    XBYTE n;
    memcpy(&n,          &(buffer[pos]), sizeof(XBYTE));           pos+=sizeof(XBYTE);  f3d->nfonts    =n;
    memcpy(&n,          &(buffer[pos]), sizeof(XBYTE));           pos+=sizeof(XBYTE);  f3d->nobjects  =n;
    memcpy(&n,          &(buffer[pos]), sizeof(XBYTE));           pos+=sizeof(XBYTE);  f3d->nmaterials=n;
    memcpy(&n,          &(buffer[pos]), sizeof(XBYTE));           pos+=sizeof(XBYTE);  f3d->ntextures =n;
    memcpy(&n,          &(buffer[pos]), sizeof(XBYTE));           pos+=sizeof(XBYTE);  f3d->nshaders  =n;


    f3d->Output(0,__L("Header:   Fonts %d  Objects %d Materials %d Textures %d Shaders %d"),f3d->nfonts,f3d->nobjects,f3d->nmaterials,f3d->ntextures,f3d->nshaders);

  *position=pos;
  return true;
}


/*-------------------------------------------------------------------
//  GGF::OBJECT_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 12:00:35
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFFONT_BUILDER::CreateBlock(GRPFILE3DGGF* f3d,  void* obj)
{
    GRPSCENE* scene=f3d->GetScene();

    GRPFONT* font=static_cast<GRPFONT*>(obj);

    GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
    block->SetType(GGF_SECTION_OBJECT);
    int calc_size=
                1*sizeof(XBYTE)+ //TYPE OBJECT
                1*sizeof(XDWORD)+ //SIZE
                1*sizeof(XDWORD)+ //tag OBJECT
                (font->GetName()->GetSize()+1)*sizeof(XWORD)+ //name
                (font->GetFile()->GetSize()+1)*sizeof(XWORD)+ //file
                1*sizeof(XBYTE)+ //quality
                1*sizeof(float)+ //borderextrusion
                1*sizeof(float)+ //innerborder
                1*sizeof(float)+ //outerborder
                1*sizeof(XBYTE)+ //jointtype
                1*sizeof(XBYTE)+ //pentype
                1*sizeof(XBYTE)+ //hasFront
                1*sizeof(XBYTE)+ //hasBack
                1*sizeof(float)+ //extrusion
                1*sizeof(float)+ //bevel
                1*sizeof(XBYTE)+ //force monospace
                1*sizeof(XBYTE)+ //align
                1*sizeof(XBYTE)+ //is Reversed
                1*sizeof(XBYTE)+ //unifynormals
                1*sizeof(XBYTE); //reversenormals

  block->Set(calc_size);
      XQWORD pos=0;
  ///----------------------------------------- HEADER
  if (block->Get())
  {

    XBYTE type              =GGF_SECTION_FONT;                        memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));     pos+=sizeof(XBYTE);
                                                                      memcpy(&(block->Get()[pos]),&calc_size,                           sizeof(XDWORD));    pos+=sizeof(XDWORD);
    XDWORD tag              =(XDWORD)(void*)font;                     memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));    pos+=sizeof(XDWORD);

    f3d->WriteString(font->GetName(),block,&pos);
    f3d->WriteString(font->GetFile(),block,&pos);

    XBYTE   quality         =font->GetAttributes()->quality;          memcpy(&(block->Get()[pos]),&quality,                             sizeof(XBYTE));     pos+=sizeof(XBYTE);
    float   borderextrusion =font->GetAttributes()->borderextrusion;  memcpy(&(block->Get()[pos]),&borderextrusion,                     sizeof(float));     pos+=sizeof(float);
    float   innerborder     =font->GetAttributes()->innerborder;      memcpy(&(block->Get()[pos]),&innerborder,                         sizeof(float));     pos+=sizeof(float);
    float   outerborder     =font->GetAttributes()->outerborder;      memcpy(&(block->Get()[pos]),&outerborder,                         sizeof(float));     pos+=sizeof(float);
    XBYTE   jointtype       =font->GetAttributes()->jointtype;        memcpy(&(block->Get()[pos]),&jointtype,                           sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE   pentype         =font->GetAttributes()->pentype;          memcpy(&(block->Get()[pos]),&pentype,                             sizeof(XBYTE));     pos+=sizeof(XBYTE);

    XBYTE   hasFront        =font->GetAttributes()->front;            memcpy(&(block->Get()[pos]),&hasFront,                            sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE   hasBack         =font->GetAttributes()->back;             memcpy(&(block->Get()[pos]),&hasBack,                             sizeof(XBYTE));     pos+=sizeof(XBYTE);
    float   extrusion       =font->GetAttributes()->extrude;          memcpy(&(block->Get()[pos]),&extrusion,                           sizeof(float));     pos+=sizeof(float);

    float   bevel           =font->GetAttributes()->bevel;            memcpy(&(block->Get()[pos]),&bevel,                               sizeof(float));     pos+=sizeof(float);
    XBYTE   monospace       =font->GetAttributes()->monospace;        memcpy(&(block->Get()[pos]),&monospace,                           sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE   align           =font->GetAttributes()->align;            memcpy(&(block->Get()[pos]),&align,                               sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE   isReversed      =font->GetAttributes()->isReversed;       memcpy(&(block->Get()[pos]),&isReversed,                          sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE   unifynormals    =font->GetAttributes()->unifynormals;     memcpy(&(block->Get()[pos]),&unifynormals,                        sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE   reversenormals  =font->GetAttributes()->reversenormals;   memcpy(&(block->Get()[pos]),&reversenormals,                      sizeof(XBYTE));     pos+=sizeof(XBYTE);
  }


  return block;
}

/*-------------------------------------------------------------------
//  GGF::OBJECT_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 17:02:30
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        GRPFILE3DGGF* :
*/
/*-----------------------------------------------------------------*/
bool GGFFONT_BUILDER::ReadBlock   (GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  GRPFONTMANAGER* fontmanager=static_cast<GRPFONTMANAGER*>(parent);
  if (!fontmanager) return false;


  XQWORD pos=*position;

  XBYTE   type        =0;
  XDWORD  size        =0;
  XDWORD  name_length =0,file_length=0;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (type!=GGF_SECTION_FONT)
  {
      f3d->Output(4,__L("GGFFONT_BUILDER::ReadBlock Unexpected Section"));
      return false;
  }

  XDWORD tag=0;

  memcpy(&size,         &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&tag,          &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  XSTRING fontname;
  XSTRING fontfilename;

  f3d->ReadString(&fontname,    buffer,position,&pos);
  f3d->ReadString(&fontfilename,buffer,position,&pos);

  GRPFONTATTRIBUTES attributes;

  memcpy(&attributes.quality,         &(buffer[pos]), sizeof(XBYTE));     pos+=sizeof(XBYTE);
  memcpy(&attributes.borderextrusion, &(buffer[pos]), sizeof(float));     pos+=sizeof(float);
  memcpy(&attributes.innerborder,     &(buffer[pos]), sizeof(float));     pos+=sizeof(float);
  memcpy(&attributes.outerborder,     &(buffer[pos]), sizeof(float));     pos+=sizeof(float);
  memcpy(&attributes.jointtype,       &(buffer[pos]), sizeof(XBYTE));     pos+=sizeof(XBYTE);
  memcpy(&attributes.pentype,         &(buffer[pos]), sizeof(XBYTE));     pos+=sizeof(XBYTE);
  memcpy(&attributes.front,           &(buffer[pos]), sizeof(XBYTE));     pos+=sizeof(XBYTE);
  memcpy(&attributes.back,            &(buffer[pos]), sizeof(XBYTE));     pos+=sizeof(XBYTE);
  memcpy(&attributes.extrude,         &(buffer[pos]), sizeof(float));     pos+=sizeof(float);

  memcpy(&attributes.bevel,           &(buffer[pos]), sizeof(float));     pos+=sizeof(float);
  memcpy(&attributes.monospace,       &(buffer[pos]), sizeof(XBYTE));     pos+=sizeof(XBYTE);
  memcpy(&attributes.align,           &(buffer[pos]), sizeof(XBYTE));     pos+=sizeof(XBYTE);
  memcpy(&attributes.isReversed,      &(buffer[pos]), sizeof(XBYTE));     pos+=sizeof(XBYTE);
  memcpy(&attributes.unifynormals,    &(buffer[pos]), sizeof(XBYTE));     pos+=sizeof(XBYTE);
  memcpy(&attributes.reversenormals,  &(buffer[pos]), sizeof(XBYTE));     pos+=sizeof(XBYTE);

  *position=(*position)+size;

  XPATH xpath,fileext;
  xpath.Set(fontfilename);
  xpath.GetNamefileExt(fileext);

  xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1,fileext.Get());

  f3d->timer->Reset();

  if (!fontmanager->Load(xpath.Get(), fontname.Get(), &attributes))
  {
    f3d->Output(4,__L("Error loading Font: %s"),fontfilename.Get());
    return false;
  }
  f3d->ttfs_timer_total+=f3d->timer->GetMeasureMilliSeconds();


  return true;
}

/*-------------------------------------------------------------------
//  GGF::OBJECT_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 12:00:35
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFOBJECT_BUILDER::CreateBlock(GRPFILE3DGGF* f3d,  void* obj)
{
    GRPSCENE* scene=f3d->GetScene();

    GRPOBJECT* object=static_cast<GRPOBJECT*>(obj);

    GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
    block->SetType(GGF_SECTION_OBJECT);
    int calc_size=
                1*sizeof(XBYTE)+ //TYPE OBJECT
                1*sizeof(XDWORD)+ //SIZE
                1*sizeof(XDWORD)+ //tag OBJECT
                //1*sizeof(XDWORD)+ //name string size
                (object->GetName()->GetSize()+1)*sizeof(XWORD)+ //name
                1*sizeof(XWORD)+  //number of elements
                1*sizeof(XWORD)+  //number of locators
                1*sizeof(XWORD)+  //number of cameras
                1*sizeof(XWORD)+  //number of lights
                1*sizeof(XWORD);  //number of logic



  block->Set(calc_size);
      XQWORD pos=0;
  ///----------------------------------------- HEADER
  if (block->Get())
  {

    XBYTE type      =GGF_SECTION_OBJECT;                memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));     pos+=sizeof(XBYTE);
                                                        memcpy(&(block->Get()[pos]),&calc_size,                           sizeof(XDWORD));    pos+=sizeof(XDWORD);
    XDWORD tag      =(XDWORD)(void*)object;             memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));    pos+=sizeof(XDWORD);

    f3d->WriteString(object->GetName(),block,&pos);
    XWORD nelements =0;
    for (XDWORD e=0;e<object->GetElements()->GetSize();e++)
    {
      GRPELEMENT* element=object->GetElements()->Get(e);
      if (element->GetMesh()!=NULL && !element->GetMesh()->isprocedural)
        nelements++;
    }
                                                            memcpy(&(block->Get()[pos]),&nelements,                           sizeof(XWORD));     pos+=sizeof(XWORD);
    XWORD nlocators = object->GetNodes()->GetSize();        memcpy(&(block->Get()[pos]),&nlocators,                           sizeof(XWORD));     pos+=sizeof(XWORD);
    XWORD ncameras  = object->GetCameras()->GetSize();      memcpy(&(block->Get()[pos]),&ncameras,                            sizeof(XWORD));     pos+=sizeof(XWORD);
    XWORD nlights   = object->GetLights()->GetSize();       memcpy(&(block->Get()[pos]),&nlights,                             sizeof(XWORD));     pos+=sizeof(XWORD);
    XWORD nlogic    =(object->GetLogic()!=NULL ? 1 : 0);    memcpy(&(block->Get()[pos]),&nlogic,                              sizeof(XWORD));     pos+=sizeof(XWORD);

  block->Blocks.SetAddInLimit         (nlocators+ncameras+nlights+nelements);
  f3d->GetDictionary()->SetAddInLimit (nlocators+ncameras+nlights+nelements);

  ///----------------------------------------- LOCATORS
  for (XDWORD e=0;e<object->GetNodes()->GetSize();e++)
  {
    GRPNODE* node=object->GetNodes()->Get(e);
    block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_LOCATOR)->CreateBlock(f3d,node));
    f3d->GetDictionary()->Add((XQWORDSIG)node,GGF_SECTION_LOCATOR);
  }

  ///----------------------------------------- ELEMENTS
  for (XDWORD e=0;e<object->GetElements()->GetSize();e++)
  {
    GRPELEMENT* element=object->GetElements()->Get(e);
    if (element->GetMesh()!=NULL && !element->GetMesh()->isprocedural)
    {
    block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_ELEMENT)->CreateBlock(f3d,element));
    f3d->GetDictionary()->Add((XQWORDSIG)element,GGF_SECTION_ELEMENT);
    }
  }


  ///----------------------------------------- CAMERAS
  for (XDWORD e=0;e<object->GetCameras()->GetSize();e++)
  {
    GRPCAMERA* camera=object->GetCameras()->Get(e);
    block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_CAMERA)->CreateBlock(f3d,camera));
    f3d->GetDictionary()->Add((XQWORDSIG)camera,GGF_SECTION_CAMERA);
  }

  ///----------------------------------------- LIGHTS
  for (XDWORD e=0;e<object->GetLights()->GetSize();e++)
  {
    GRPLIGHT* light=object->GetLights()->Get(e);
    block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_LIGHT)->CreateBlock(f3d,light));
    f3d->GetDictionary()->Add((XQWORDSIG)light,GGF_SECTION_LIGHT);
  }

  if (nlogic)
    block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_LOGIC)->CreateBlock(f3d,object->GetLogic()));
  }


  return block;
}

/*-------------------------------------------------------------------
//  GGF::OBJECT_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 17:02:30
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        GRPFILE3DGGF* :
*/
/*-----------------------------------------------------------------*/
bool GGFOBJECT_BUILDER::ReadBlock   (GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  if (f3d->object==NULL)
  {
    f3d->object=new GRPOBJECT();
    if (!f3d->object)
      return false;
  }

  if (f3d->object->GetNode()==NULL)
  {
    GRPNODE* node=f3d->object->CreateNode();
    if (!node)
      return false;
    f3d->object->SetNode(node);
  }

  GRPOBJECT* object=f3d->object;


  XQWORD pos=*position;

  XBYTE   type        =0;
  XDWORD  size        =0;
  XDWORD  name_length =0;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&size,         &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&object->tag,  &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  f3d->ReadString(object->GetName(),buffer,position,&pos);

    XWORD nelements =0; memcpy(&nelements,  &(buffer[pos]), sizeof(XWORD));     pos+=sizeof(XWORD);
    XWORD nlocators =0; memcpy(&nlocators,  &(buffer[pos]), sizeof(XWORD));     pos+=sizeof(XWORD);
    XWORD ncameras  =0; memcpy(&ncameras,   &(buffer[pos]), sizeof(XWORD));     pos+=sizeof(XWORD);
    XWORD nlights   =0; memcpy(&nlights,    &(buffer[pos]), sizeof(XWORD));     pos+=sizeof(XWORD);
    XWORD nlogic    =0; memcpy(&nlogic,     &(buffer[pos]), sizeof(XWORD));     pos+=sizeof(XWORD);

    *position=*position+size;

    XDWORD e=0;
      for (e=0;e<nlocators;e++)
      {
        if (!f3d->GetBuilders()->Get(GGF_SECTION_LOCATOR)->ReadBlock(f3d,buffer,position))    break;
      }

      f3d->Output(0,__L("Loaded %d/%d Locators"),e,nlocators);

      for (e=0;e<nelements;e++)
      {
        if (!f3d->GetBuilders()->Get(GGF_SECTION_ELEMENT)->ReadBlock(f3d,buffer,position))    break;
      }

      f3d->Output(0,__L("Loaded %d/%d nelements"),e,nelements);

      for (e=0;e<ncameras;e++)
      {
        if (!f3d->GetBuilders()->Get(GGF_SECTION_CAMERA)->ReadBlock(f3d,buffer,position))     break;
      }

      f3d->Output(0,__L("Loaded %d/%d cameras"),e,ncameras);


      for (e=0;e<nlights;e++)
      {
        if (!f3d->GetBuilders()->Get(GGF_SECTION_LIGHT)->ReadBlock(f3d,buffer,position))      break;
      }

      f3d->Output(0,__L("Loaded %d/%d lights"),e,nlights);

      for (e=0;e<nlogic;e++)
      {
        if (!f3d->GetBuilders()->Get(GGF_SECTION_LOGIC)->ReadBlock(f3d,buffer,position,object))     break;
      }

      f3d->Output(0,__L("Loaded %d/%d logic"),e,nlogic);

  return true;
}




/*-------------------------------------------------------------------
//  GGF::ELEMENT_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 16:47:59
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFELEMENT_BUILDER::CreateBlock(GRPFILE3DGGF* f3d,void * pelement)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
  if (!block)
  {
    f3d->Output(4,__L("Out of memory!"));
    return NULL;
  }
  block->SetType(GGF_SECTION_ELEMENT);

  GRPELEMENT* element=static_cast<GRPELEMENT*>(pelement);

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XDWORD)+ //size
                1*sizeof(XDWORD)+ //tag

                (element->GetName()->GetSize()+1)*sizeof(XWORD)+ //name
                1*sizeof(XBYTE)+  //is_blending
                1*sizeof(XBYTE)+  //blendingmode
                4*sizeof(GLFLOAT)+ //base color
                1*sizeof(XBYTE)+  //rendermode
                1*sizeof(XBYTE)+  //renderflags
                1*sizeof(XBYTE)+  //castshadows
                1*sizeof(XBYTE)+  //receivesshadows
                1*sizeof(XBYTE)+  //castreflections
                1*sizeof(XBYTE)+  //receivesreflections
                1*sizeof(XBYTE)+  //renderpriority
                1*sizeof(XBYTE)+  //is instance
                1*sizeof(XBYTE)+  //number of meshes
                1*sizeof(XBYTE)+  //has morph
                1*sizeof(XBYTE);  //has skin


  block->Set(calc_size);

  ///----------------------------------------- HEADER
  if (block->Get())
  {
    XQWORD pos=0;
    XBYTE type            = GGF_SECTION_ELEMENT;                      memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));     pos+=sizeof(XBYTE);
                                                                      memcpy(&(block->Get()[pos]),&calc_size,                           sizeof(XDWORD));    pos+=sizeof(XDWORD);
    XDWORD tag            =(XDWORD)(void*)element;                    memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));    pos+=sizeof(XDWORD);

    f3d->WriteString(element->GetName(),block,&pos);

    XBYTE blending        = element->IsBlending             ();       memcpy(&(block->Get()[pos]),&blending,                            sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE blendmode       = element->GetBlendingMode        ();       memcpy(&(block->Get()[pos]),&blendmode,                           sizeof(XBYTE));     pos+=sizeof(XBYTE);

    GLFLOAT color[4];
            color[0]=element->GetDefaultColor()->GetRed();
            color[1]=element->GetDefaultColor()->GetGreen();
            color[2]=element->GetDefaultColor()->GetBlue();
            color[3]=element->GetDefaultColor()->GetAlpha();            memcpy(&(block->Get()[pos]),&color,                             4*sizeof(GLFLOAT));   pos+=4*sizeof(GLFLOAT);

    XBYTE rendermode      = element->GetRenderMode          ();       memcpy(&(block->Get()[pos]),&rendermode,                          sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE renderflag      = element->GetRenderFlags         ();       memcpy(&(block->Get()[pos]),&renderflag,                          sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE castshadows     = element->GetCastsShadows        ();       memcpy(&(block->Get()[pos]),&castshadows,                         sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE receivesshadows = element->GetReceivesShadows     ();       memcpy(&(block->Get()[pos]),&receivesshadows,                     sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE castsreflect    = element->GetCastsReflections    ();       memcpy(&(block->Get()[pos]),&castsreflect,                        sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE receivesreflect = element->GetReceivesReflections ();       memcpy(&(block->Get()[pos]),&receivesreflect,                     sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE renderpriority  = element->GetRenderPriority      ();       memcpy(&(block->Get()[pos]),&renderpriority,                      sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE is_instance     = element->IsInstance             ();       memcpy(&(block->Get()[pos]),&is_instance,                         sizeof(XBYTE));     pos+=sizeof(XBYTE);

    XBYTE numbermeshes    = 0;
    if (element->GetMesh() && !element->GetMesh()->isprocedural)
        numbermeshes    = 1;

    memcpy(&(block->Get()[pos]),&numbermeshes,                        sizeof(XBYTE));     pos+=sizeof(XBYTE);

    XBYTE hasMorph        = (element->GetDeformer()!=NULL ? 1 : 0);
    memcpy(&(block->Get()[pos]),&hasMorph,                            sizeof(XBYTE));     pos+=sizeof(XBYTE);

    XBYTE hasSkin         = (element->GetSkin()!=NULL ? 1 : 0);
    memcpy(&(block->Get()[pos]),&hasSkin,                             sizeof(XBYTE));     pos+=sizeof(XBYTE);

  ///------------------------------------------- MESH
  if (!element->IsInstance())
  {
    GRPMESH* mesh=element->GetMesh();
    if (mesh)
    {
      GGFSECTIONBUILDER*  builder=f3d->GetBuilders()->Get(GGF_SECTION_MESH);
      GGFSECTIONBLOCK*    mesh_block  =builder->CreateBlock(f3d,mesh);
                            block->Blocks.Add(mesh_block);
    }
  }

  //-------------------------------------------- DEFORMER
  if (element->GetDeformer())
  {
    GGFSECTIONBUILDER*  builder=f3d->GetBuilders()->Get(GGF_SECTION_DEFORMER);
    GGFSECTIONBLOCK*    deformer_block  =builder->CreateBlock(f3d,element->GetDeformer());
                        block->Blocks.Add(deformer_block);
  }

  //-------------------------------------------- SKIN
  if (element->GetSkin())
  {
    GGFSECTIONBUILDER*  builder=f3d->GetBuilders()->Get(GGF_SECTION_SKIN);
    GGFSECTIONBLOCK*    skin_block  =builder->CreateBlock(f3d,element->GetSkin());
                        block->Blocks.Add(skin_block);
  }

  //-------------------------------------------assign locator
  if (element->GetNode())
  {
      GGFPROPERTY* property=new GGFPROPERTY();
      if (property)
      {
          property->PropertyType=GGF_SECTION_LOCATOR;

          property->ownertype=GGF_SECTION_LOCATOR;
          property->ownertag=(XDWORD)(void*)element->GetNode();

          property->referencetype=GGF_SECTION_ELEMENT;
          property->referencetag=(XDWORD)(void*)element;

          f3d->GetProperties()->Add(property);
        }
  }


    //------------------------------------------- assign mesh
    if (element->GetMesh())
    if (!element->GetMesh()->isprocedural)
    {
        GGFPROPERTY* property=new GGFPROPERTY();
        if (property)
        {
          property->PropertyType=GGF_SECTION_ELEMENT;

          property->ownertype=GGF_SECTION_ELEMENT;
          property->ownertag=(XDWORD)(void*)element;

          property->referencetype=GGF_SECTION_MESH;
          property->referencetag=(XDWORD)(void*)element->GetMesh();

          f3d->GetProperties()->Add(property);
        }
    }



    //----------------------------------------- assign program
    if (element->GetProgram())
    {
      GGFPROPERTY* property=new GGFPROPERTY();
      if (property)
      {
        property->PropertyType=GGF_SECTION_ELEMENT;

        property->ownertype=GGF_SECTION_ELEMENT;
        property->ownertag=(XDWORD)(void*)element;

        property->referencetype=GGF_SECTION_SHADER;
        property->referencetag=(XDWORD)(void*)element->GetProgram();

        f3d->GetProperties()->Add(property);
      }
    }
  }
  return block;
}

/*-------------------------------------------------------------------
//  GGF::ELEMENT_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 17:02:30
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        GRPFILE3DGGF* :
*/
/*-----------------------------------------------------------------*/
bool GGFELEMENT_BUILDER::ReadBlock    (GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{

  GRPELEMENT* element     =new GRPELEMENT();
  if (!element) return false;

  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (type!=GGF_SECTION_ELEMENT)
  {
    f3d->Output(4,__L("GGFELEMENT_BUILDER::ReadBlock Unexpected Section"));
    return false;
  }

  memcpy(&size,         &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&element->tag, &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  f3d->ReadString(element->GetName(),buffer,position,&pos);


  XBYTE isblending          =0,
        blendingmode        =0,
        rendermode          =0,
        renderflags         =0,
        castshadows         =0,
        receiveshadows      =0,
        castreflections     =0,
        receivesreflections =0,
        renderpriority      =0,
        is_instance         =0,
        nmeshes             =0,
        hasMorph            =0,
        hasSkin             =0;

  GLFLOAT color[4];

  memcpy(&isblending,           &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&blendingmode,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  memcpy(&color,                &(buffer[pos]), 4*sizeof(GLFLOAT));           pos+=4*sizeof(GLFLOAT);

  memcpy(&rendermode,           &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&renderflags,          &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&castshadows,          &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&receiveshadows,       &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&castreflections,      &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&receivesreflections,  &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&renderpriority,       &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&is_instance,          &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&nmeshes,              &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&hasMorph,             &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&hasSkin,              &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  element->SetBlending            (isblending!=0);
  element->SetBlendingMode        ((GRPBLENDINGMODE)blendingmode);
  element->SetDefaultColor        (GRPCOLOR(color[0],color[1],color[2],color[3]));
  element->SetRenderMode          ((GRPRENDERMODE)  rendermode);
  element->SetRenderFlags         (renderflags);
  element->SetCastsShadows        (castshadows!=0);
  element->SetReceivesShadows     (receiveshadows!=0);
  element->SetCastsReflections    (castreflections!=0);
  element->SetReceivesReflections (receivesreflections!=0);
  element->SetRenderPriority      (renderpriority);
  element->SetIsInstance          (is_instance!=0);

  *position=(*position)+size;

  f3d->element=element;
  if (!is_instance)
  for (int e=0;e<nmeshes;e++)
    if (!f3d->GetBuilders()->Get(GGF_SECTION_MESH)->ReadBlock(f3d,buffer,position,element))       return false;


  if (hasMorph)
    if (!f3d->GetBuilders()->Get(GGF_SECTION_DEFORMER)->ReadBlock(f3d,buffer,position,element))   return false;

  if (hasSkin)
    if (!f3d->GetBuilders()->Get(GGF_SECTION_SKIN)->ReadBlock(f3d,buffer,position,element))       return false;

  f3d->object->Add(element);
  f3d->elements.Add(element);

  return true;
}

/*-------------------------------------------------------------------
//  GGF::MESH_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 16:47:59
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFMESH_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
  block->SetType(GGF_SECTION_MESH);

  GRPMESH* mesh=static_cast<GRPMESH*>(pnode);

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XDWORD)+ //tag

                (mesh->GetName()->GetSize()+1)*sizeof(XWORD)+ //name

                1*sizeof(XBYTE)+ //is procedural
                1*sizeof(XBYTE)+ //number of vertex layers
                (mesh->isprocedural ? 0 : mesh->vertexlayers.GetSize())*sizeof(XBYTE)+ ///layer types

                1*sizeof(XDWORD)+ //number of faces
                1*sizeof(XBYTE)+ //vertex per faces
                3*sizeof(XWORD)*mesh->GetFaces()->GetSize();


  if (!mesh->isprocedural)
  {
  calc_size+=   1*sizeof(XWORD)+                  //n index
                mesh->GetNIndex()*sizeof(XWORD)+  // indices
                0;
  }

  block->Set(calc_size);

  XQWORD pos=0;

  ///----------------------------------------- HEADER
  if (block->Get())
  {
    XBYTE type            = GGF_SECTION_MESH;                                         memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XDWORD tag            = (XDWORD)(void*)mesh;                                      memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));      pos+=sizeof(XDWORD);
    /*
    XBYTE length          = mesh->GetName()->GetSize();                               memcpy(&(block->Get()[pos]),&length,                              sizeof(XBYTE));       pos+=sizeof(XBYTE);
                            if (length)                                               memcpy(&(block->Get()[pos]),mesh->GetName()->Get(),        length*sizeof(XCHAR));       pos+= length*sizeof(XCHAR);
                            */
    f3d->WriteString(mesh->GetName(),block,&pos);

    XBYTE isprocedural    = mesh->isprocedural;                                       memcpy(&(block->Get()[pos]),&isprocedural,                        sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XBYTE numberoflayers  = (mesh->isprocedural ? 0 : mesh->vertexlayers.GetSize());  memcpy(&(block->Get()[pos]),&numberoflayers,                      sizeof(XBYTE));       pos+=sizeof(XBYTE);

    block->Blocks.SetAddInLimit(block->Blocks.GetAddInLimit()+numberoflayers);

    for (XDWORD e=0;e<numberoflayers;e++)
    {
      XBYTE layertype = mesh->vertexlayers.Get(e);
      memcpy(&(block->Get()[pos]),  &layertype,   sizeof(XBYTE));
      pos+=sizeof(XBYTE);
    }

    XDWORD nFaces         = (mesh->isprocedural ? 0 : mesh->GetNFaces());             memcpy(&(block->Get()[pos]),&nFaces,                              sizeof(XDWORD));      pos+=sizeof(XDWORD);
    XBYTE nVertexPerFace  = (mesh->isprocedural ? 0 : mesh->GetNVertexPerFace());     memcpy(&(block->Get()[pos]),&nVertexPerFace,                      sizeof(XBYTE));       pos+=sizeof(XBYTE);

    //------------ index
    if (!mesh->isprocedural)
    {
      XWORD  nindex         = mesh->GetNIndex     ();                           memcpy(&(block->Get()[pos]),&nindex,                              sizeof(XWORD));       pos+=sizeof(XWORD);
      XWORD* indexes        = mesh->GetIndexData  ();                           memcpy(&(block->Get()[pos]),indexes,            mesh->GetNIndex()*sizeof(XWORD));       pos+=mesh->GetNIndex()*sizeof(XWORD);

      GGFSECTIONBUILDER*  builder         =NULL;
      GGFSECTIONBLOCK*    mesh_layer_block=NULL;

    //------------ faces
    for (XDWORD f=0;f<nFaces;f++)
    {
      GRPFACE* face=mesh->GetFaces()->Get(f);
      XWORD a=face->a;
      XWORD b=face->b;
      XWORD c=face->c;

      memcpy(&(block->Get()[pos]),&a,sizeof(XWORD));  pos+=sizeof(XWORD);
      memcpy(&(block->Get()[pos]),&b,sizeof(XWORD));  pos+=sizeof(XWORD);
      memcpy(&(block->Get()[pos]),&c,sizeof(XWORD));  pos+=sizeof(XWORD);
    }

    block->Blocks.SetAddInLimit(block->Blocks.GetAddInLimit()+GRPVERTEX_CHANNEL_MORPH_MAX);

    //------------ position
      if (mesh->vertexlayers.Find(GRPVERTEX_CHANNEL_POSITION)!=NOTFOUND)
      {
                          builder=f3d->GetBuilders()->Get(GGF_SECTION_MESH_LAYER);
                          static_cast<GGFMESH_LAYER_BUILDER*>(builder)->layertype=GRPVERTEX_CHANNEL_POSITION;
                          static_cast<GGFMESH_LAYER_BUILDER*>(builder)->mesh=mesh;
                          mesh_layer_block  =builder->CreateBlock(f3d,mesh);
                          block->Blocks.Add(mesh_layer_block);
      }

    //------------ normal
      if (mesh->vertexlayers.Find(GRPVERTEX_CHANNEL_NORMAL)!=NOTFOUND)
      {
                          builder=f3d->GetBuilders()->Get(GGF_SECTION_MESH_LAYER);
                          static_cast<GGFMESH_LAYER_BUILDER*>(builder)->layertype=GRPVERTEX_CHANNEL_NORMAL;
                          static_cast<GGFMESH_LAYER_BUILDER*>(builder)->mesh=mesh;
                          mesh_layer_block  =builder->CreateBlock(f3d,mesh);
                          block->Blocks.Add(mesh_layer_block);
      }

    //------------ binormal
#ifdef GRPVERTEX_EXTENDED
      if (mesh->vertexlayers.Find(GRPVERTEX_CHANNEL_BINORMAL)!=NOTFOUND)
      {
                          builder=f3d->GetBuilders()->Get(GGF_SECTION_MESH_LAYER);
                          static_cast<GGFMESH_LAYER_BUILDER*>(builder)->layertype=GRPVERTEX_CHANNEL_BINORMAL;
                          mesh_layer_block  =builder->CreateBlock(f3d,mesh);
                          block->Blocks.Add(mesh_layer_block);
      }


    //------------ tangent
      if (mesh->vertexlayers.Find(GRPVERTEX_CHANNEL_TANGENT)!=NOTFOUND)
      {
                          builder=f3d->GetBuilders()->Get(GGF_SECTION_MESH_LAYER);
                          static_cast<GGFMESH_LAYER_BUILDER*>(builder)->layertype=GRPVERTEX_CHANNEL_TANGENT;
                          mesh_layer_block  =builder->CreateBlock(f3d,mesh);
                          block->Blocks.Add(mesh_layer_block);
      }
#endif
    //------------ texture
      if (mesh->vertexlayers.Find(GRPVERTEX_CHANNEL_TEXTURE)!=NOTFOUND)
      {
                          builder=f3d->GetBuilders()->Get(GGF_SECTION_MESH_LAYER);
                          static_cast<GGFMESH_LAYER_BUILDER*>(builder)->layertype=GRPVERTEX_CHANNEL_TEXTURE;
                          static_cast<GGFMESH_LAYER_BUILDER*>(builder)->mesh=mesh;
                          mesh_layer_block  =builder->CreateBlock(f3d,mesh);
                          block->Blocks.Add(mesh_layer_block);
      }

    //------------ boneindex
      if (mesh->vertexlayers.Find(GRPVERTEX_CHANNEL_BONE_INDEX)!=NOTFOUND)
      {
                          builder=f3d->GetBuilders()->Get(GGF_SECTION_MESH_LAYER);
                          static_cast<GGFMESH_LAYER_BUILDER*>(builder)->layertype=GRPVERTEX_CHANNEL_BONE_INDEX;
                          mesh_layer_block  =builder->CreateBlock(f3d,mesh);
                          block->Blocks.Add(mesh_layer_block);
      }

    //------------ boneweights
      if (mesh->vertexlayers.Find(GRPVERTEX_CHANNEL_BONE_WEIGHTS)!=NOTFOUND)
      {
                          builder=f3d->GetBuilders()->Get(GGF_SECTION_MESH_LAYER);
                          static_cast<GGFMESH_LAYER_BUILDER*>(builder)->layertype=GRPVERTEX_CHANNEL_BONE_WEIGHTS;
                          mesh_layer_block  =builder->CreateBlock(f3d,mesh);
                          block->Blocks.Add(mesh_layer_block);
      }

    //------------ morph
      if (mesh->vertexlayers.Find(GRPVERTEX_CHANNEL_MORPH_POSITION1)!=NOTFOUND)
      {
        for (int e=GRPVERTEX_CHANNEL_MORPH_POSITION1;e<GRPVERTEX_CHANNEL_MORPH_MAX;e++)
        if (mesh->vertexlayers.Find((GRPVERTEX_CHANNEL)e)!=NOTFOUND)
        {
        GGFSECTIONBUILDER*  builder=f3d->GetBuilders()->Get(GGF_SECTION_MESH_LAYER);
                            static_cast<GGFMESH_LAYER_BUILDER*>(builder)->layertype=(GRPVERTEX_CHANNEL)e;
        GGFSECTIONBLOCK*    mesh_layer_block  =builder->CreateBlock(f3d,mesh);
                            block->Blocks.Add(mesh_layer_block);
        }
      }
    }
  }

  mesh->isloaded=false;

  return block;
}

/*-------------------------------------------------------------------
//  GGF::MESH_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 17:02:30
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        GRPFILE3DGGF* :
*/
/*-----------------------------------------------------------------*/
bool GGFMESH_BUILDER::ReadBlock   (GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{

  GRPMESH*    mesh        =new GRPMESH();
  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;

  //------------------------- header
  mesh->isloaded = false;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_MESH)
  {
    f3d->Output(4,__L("GGFMESH_BUILDER Unexpected Section! : %d"),type);
    return false;
  }
  memcpy(&mesh->tag,    &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  f3d->ReadString(mesh->GetName(),buffer,position,&pos);

  //--------------------------- mesh data
  XDWORD nindex=0;

  memcpy(&mesh->isprocedural, &(buffer[pos]), sizeof(XBYTE));                               pos+=sizeof(XBYTE);
  if (!mesh->isprocedural)
  {
    XBYTE numberoflayers=0;
    memcpy(&numberoflayers, &(buffer[pos]), sizeof(XBYTE));                           pos+=sizeof(XBYTE);

    mesh->vertexlayers.DeleteAll();
    mesh->vertexlayers.SetAddInLimit(numberoflayers);
    for (XDWORD e=0;e<numberoflayers;e++)
    {
      XBYTE layertype = 0;
      memcpy(&layertype, &(buffer[pos]), sizeof(XBYTE));
      mesh->vertexlayers.Add((GRPVERTEX_CHANNEL)layertype);
      pos+=sizeof(XBYTE);
    }

    XDWORD nFaces=0;
    memcpy(&nFaces,               &(buffer[pos]), sizeof(XDWORD));                          pos+=sizeof(XDWORD);
    XBYTE nVertexPerFace  =0;
    memcpy(&nVertexPerFace,       &(buffer[pos]), sizeof(XBYTE));                           pos+=sizeof(XBYTE);


    //---------- indexes
    memcpy(&mesh->nindexdata,   &(buffer[pos]), sizeof(XWORD));                           pos+=sizeof(XWORD);
    if (mesh->nindexdata)
    {
    mesh->indexdata=new XWORD[mesh->nindexdata];  if (!mesh->indexdata)                     return false;
    memcpy(mesh->indexdata,   &(buffer[pos]),   mesh->nindexdata*sizeof(XWORD));            pos+=mesh->nindexdata*sizeof(XWORD);
    }

    //---------- faces
    mesh->GetFaces()->SetAddInLimit(nFaces);
    for (XDWORD f=0;f<nFaces;f++)
    {
      GRPFACE* face=new GRPFACE();
      XWORD a=0;
      XWORD b=0;
      XWORD c=0;

      memcpy(&a,&(buffer[pos]),sizeof(XWORD));  pos+=sizeof(XWORD);
      memcpy(&b,&(buffer[pos]),sizeof(XWORD));  pos+=sizeof(XWORD);
      memcpy(&c,&(buffer[pos]),sizeof(XWORD));  pos+=sizeof(XWORD);

      face->a=a;
      face->b=b;
      face->c=c;

      mesh->GetFaces()->Add(face);
    }

    //----------- vertex layers
    *position=pos;
    XDWORD n=numberoflayers;
    for (XDWORD e=0;e<n;e++)
    {
      GGFMESH_LAYER_BUILDER* builder=static_cast<GGFMESH_LAYER_BUILDER*>(f3d->GetBuilders()->Get(GGF_SECTION_MESH_LAYER));
      if (builder)
      {
        builder->mesh=mesh;
        if (!builder->ReadBlock(f3d,buffer,position))   break;
      }
      else
        break;
    }
  }
  else
  {
      static int a = 0;
      a++;
  }
  mesh->isloaded=false;
  f3d->meshes.Add(mesh);

  return true;
}


/*-------------------------------------------------------------------
//  GGFMESH_LAYER_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/04/2017 14:41:39
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        pnode :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFMESH_LAYER_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{

  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
                   block->SetType(GGF_SECTION_MESH_LAYER);

  GRPMESH* mesh=static_cast<GRPMESH*>(pnode);


  // ------------------------------------------------ select layer

    XDWORD offset=0;
    XBYTE  datasize=0;
    XDWORD stride=sizeof(GRPVERTEX);

    XSTRING name;
    switch(this->layertype)
    {
    case GRPVERTEX_CHANNEL_POSITION         : name.Set(__L("position"));      offset=GRPVERTEX_CHANNEL_OFFSET_POSITION;         datasize=GRPVERTEX_CHANNEL_POSITION_SIZE;       break;
    case GRPVERTEX_CHANNEL_NORMAL           : name.Set(__L("normal"));        offset=GRPVERTEX_CHANNEL_OFFSET_NORMAL;           datasize=GRPVERTEX_CHANNEL_NORMAL_SIZE;         break;
    case GRPVERTEX_CHANNEL_TEXTURE          : name.Set(__L("texturecoords")); offset=GRPVERTEX_CHANNEL_OFFSET_TEXTURE;          datasize=GRPVERTEX_CHANNEL_TEXTURE_SIZE;        break;

#ifdef GRPVERTEX_EXTENDED
    case GRPVERTEX_CHANNEL_TANGENT          : name.Set(__L("tangent"));       offset=GRPVERTEX_CHANNEL_OFFSET_TANGENT;          datasize=GRPVERTEX_CHANNEL_TANGENT_SIZE;        break;
    case GRPVERTEX_CHANNEL_BINORMAL         : name.Set(__L("binormal"));      offset=GRPVERTEX_CHANNEL_OFFSET_BINORMAL;         datasize=GRPVERTEX_CHANNEL_BINORMAL_SIZE;       break;
#endif

#ifdef GRPVERTEX_SKINNING
    case GRPVERTEX_CHANNEL_BONE_INDEX       : name.Set(__L("boneindex"));     offset=GRPVERTEX_CHANNEL_OFFSET_BONE_INDEX;       datasize=GRPVERTEX_CHANNEL_BONE_INDEX_SIZE;     break;
    case GRPVERTEX_CHANNEL_BONE_WEIGHTS     : name.Set(__L("boneweights"));   offset=GRPVERTEX_CHANNEL_OFFSET_BONE_WEIGHTS;     datasize=GRPVERTEX_CHANNEL_BONE_WEIGHTS_SIZE;   break;
#endif

#ifdef GRPDEFORMERS_ACTIVE
    case GRPVERTEX_CHANNEL_MORPH_POSITION1  : name.Set(__L("morph1"));        offset=GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION1;  datasize=GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE; break;
    case GRPVERTEX_CHANNEL_MORPH_POSITION2  : name.Set(__L("morph2"));        offset=GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION2;  datasize=GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE; break;
    case GRPVERTEX_CHANNEL_MORPH_POSITION3  : name.Set(__L("morph3"));        offset=GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION3;  datasize=GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE; break;
    case GRPVERTEX_CHANNEL_MORPH_POSITION4  : name.Set(__L("morph4"));        offset=GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION4;  datasize=GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE; break;
    case GRPVERTEX_CHANNEL_MORPH_POSITION5  : name.Set(__L("morph5"));        offset=GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION5;  datasize=GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE; break;
    case GRPVERTEX_CHANNEL_MORPH_POSITION6  : name.Set(__L("morph6"));        offset=GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION6;  datasize=GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE; break;
    case GRPVERTEX_CHANNEL_MORPH_POSITION7  : name.Set(__L("morph7"));        offset=GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION7;  datasize=GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE; break;
    case GRPVERTEX_CHANNEL_MORPH_POSITION8  : name.Set(__L("morph8"));        offset=GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION8;  datasize=GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE; break;
    case GRPVERTEX_CHANNEL_MORPH_POSITION9  : name.Set(__L("morph9"));        offset=GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION9;  datasize=GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE; break;
    case GRPVERTEX_CHANNEL_MORPH_POSITION10 : name.Set(__L("morph10"));       offset=GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION10; datasize=GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE; break;
    case GRPVERTEX_CHANNEL_MORPH_POSITION11 : name.Set(__L("morph11"));       offset=GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION11; datasize=GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE; break;
    case GRPVERTEX_CHANNEL_MORPH_POSITION12 : name.Set(__L("morph12"));       offset=GRPVERTEX_CHANNEL_OFFSET_MORPH_POSITION12; datasize=GRPVERTEX_CHANNEL_MORPH_POSITION_SIZE; break;
#endif
    }

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XDWORD)+ //size
                1*sizeof(XDWORD)+ //tag
                (name.GetSize()+1)*sizeof(XWORD)+ //name
                1*sizeof(XWORD)+  //nvertex
                1*sizeof(XBYTE)+  //size of vertex
                datasize*mesh->GetNVertex()*sizeof(GLFLOAT);

  block->Set(calc_size);

  XDWORD tag=0;
  XQWORD pos=0;

  ///----------------------------------------- HEADER
  if (block->Get())
  {
    // ------------------------------------------------ block header

    XBYTE type            = GGF_SECTION_MESH_LAYER;         memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XDWORD size           = calc_size;                      memcpy(&(block->Get()[pos]),&size,                                sizeof(XDWORD));      pos+=sizeof(XDWORD);
                                                            memcpy(&(block->Get()[pos]),&this->layertype,                     sizeof(XDWORD));      pos+=sizeof(XDWORD);

    // ------------------------------------------------ save layer name and (numvertex*size of data)

    f3d->WriteString(&name,block,&pos);
    XWORD nindex          = mesh->GetNVertex();             memcpy(&(block->Get()[pos]),&nindex,                              sizeof(XWORD));       pos+=sizeof(XWORD);
                                                            memcpy(&(block->Get()[pos]),&datasize,                            sizeof(XBYTE));       pos+=sizeof(XBYTE);

    // ------------------------------------------------ actual vertex layer saving

    GRPVERTEX* vertex=mesh->GetVertexData();
    for (XDWORD e=0;e<mesh->GetNVertex();e++)
    {
      GLFLOAT data[4];

      switch(this->layertype)
      {
        case GRPVERTEX_CHANNEL_POSITION         : memcpy(data,vertex[e].position,           datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_NORMAL           : memcpy(data,vertex[e].normal,             datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_TEXTURE          : memcpy(data,vertex[e].texturecoordinates, datasize*sizeof(GLFLOAT)); break;

#ifdef GRPVERTEX_EXTENDED
        case GRPVERTEX_CHANNEL_BINORMAL         : memcpy(data,vertex[e].binormal,           datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_TANGENT          : memcpy(data,vertex[e].tangent,            datasize*sizeof(GLFLOAT)); break;
#endif

#ifdef GRPVERTEX_SKINNING
        case GRPVERTEX_CHANNEL_BONE_INDEX       : memcpy(data,vertex[e].boneIndices,        datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_BONE_WEIGHTS     : memcpy(data,vertex[e].boneWeights,        datasize*sizeof(GLFLOAT)); break;
#endif

#ifdef GRPDEFORMERS_ACTIVE
        case GRPVERTEX_CHANNEL_MORPH_POSITION1  : memcpy(data,vertex[e].positionMorph1,     datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION2  : memcpy(data,vertex[e].positionMorph2,     datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION3  : memcpy(data,vertex[e].positionMorph3,     datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION4  : memcpy(data,vertex[e].positionMorph4,     datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION5  : memcpy(data,vertex[e].positionMorph5,     datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION6  : memcpy(data,vertex[e].positionMorph6,     datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION7  : memcpy(data,vertex[e].positionMorph7,     datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION8  : memcpy(data,vertex[e].positionMorph8,     datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION9  : memcpy(data,vertex[e].positionMorph9,     datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION10 : memcpy(data,vertex[e].positionMorph10,    datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION11 : memcpy(data,vertex[e].positionMorph11,    datasize*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION12 : memcpy(data,vertex[e].positionMorph12,    datasize*sizeof(GLFLOAT)); break;
#endif
      }
        memcpy(&(block->Get()[pos]),&data,sizeof(GLFLOAT)*datasize);
        pos+=sizeof(GLFLOAT)*datasize;
    }
  }

  mesh->isloaded=false;
  return block;
}



/*-------------------------------------------------------------------
//  GGFMESH_LAYER_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/04/2017 14:41:39
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        pnode :
*/
/*-----------------------------------------------------------------*/
bool GGFMESH_LAYER_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;

  GRPMESH* mesh=this->mesh;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_MESH_LAYER)
  {
    f3d->Output(4,__L("GGFMESH_LAYER_BUILDER Unexpected Section! : %d"),type);
    return false;
  }

  memcpy(&size,             &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&this->layertype,  &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  XSTRING s;
  f3d->ReadString(&s,buffer,position,&pos);

  XWORD nvertex=0;          memcpy(&nvertex,          &(buffer[pos]), sizeof(XWORD));             pos+=sizeof(XWORD);
  XWORD sizevertex=0;       memcpy(&sizevertex,       &(buffer[pos]), sizeof(XBYTE));             pos+=sizeof(XBYTE);

  if (nvertex>0)
  {
    if (mesh->vertexdata==NULL)
      mesh->vertexdata=new GRPVERTEX[nvertex];

    if (mesh->vertexdata==NULL)
      return false;

    mesh->nvertexdata=nvertex;

    GRPVERTEX* vertex=mesh->GetVertexData();
    for (XDWORD e=0;e<nvertex;e++)
    {
      GLFLOAT data[4];
      memcpy(&data,&(buffer[pos]),sizeof(GLFLOAT)*sizevertex);

      switch(this->layertype)
      {
        case GRPVERTEX_CHANNEL_POSITION         : memcpy(vertex[e].position,            data, sizevertex*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_NORMAL           : memcpy(vertex[e].normal,              data, sizevertex*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_TEXTURE          : memcpy(vertex[e].texturecoordinates,  data, sizevertex*sizeof(GLFLOAT)); break;

#ifdef GRPVERTEX_EXTENDED
        case GRPVERTEX_CHANNEL_BINORMAL         : memcpy(vertex[e].binormal,            data, sizevertex*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_TANGENT          : memcpy(vertex[e].tangent,             data, sizevertex*sizeof(GLFLOAT)); break;
#endif
#ifdef GRPVERTEX_SKINNING
        case GRPVERTEX_CHANNEL_BONE_INDEX       : memcpy(vertex[e].boneIndices,         data, sizevertex * sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_BONE_WEIGHTS     : memcpy(vertex[e].boneWeights,         data, sizevertex * sizeof(GLFLOAT)); break;
#endif

#ifdef GRPDEFORMERS_ACTIVE
        case GRPVERTEX_CHANNEL_MORPH_POSITION1  : memcpy(vertex[e].positionMorph1,      data, sizevertex*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION2  : memcpy(vertex[e].positionMorph2,      data, sizevertex*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION3  : memcpy(vertex[e].positionMorph3,      data, sizevertex*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION4  : memcpy(vertex[e].positionMorph4,      data, sizevertex*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION5  : memcpy(vertex[e].positionMorph5,      data, sizevertex*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION6  : memcpy(vertex[e].positionMorph6,      data, sizevertex*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION7  : memcpy(vertex[e].positionMorph7,      data, sizevertex*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION8  : memcpy(vertex[e].positionMorph8,      data, sizevertex*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION9  : memcpy(vertex[e].positionMorph9,      data, sizevertex*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION10 : memcpy(vertex[e].positionMorph10,     data, sizevertex*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION11 : memcpy(vertex[e].positionMorph11,     data, sizevertex*sizeof(GLFLOAT)); break;
        case GRPVERTEX_CHANNEL_MORPH_POSITION12 : memcpy(vertex[e].positionMorph12,     data, sizevertex*sizeof(GLFLOAT)); break;
#endif
        default: break;
      }
        pos+=sizeof(GLFLOAT)*sizevertex;
    }

    if (mesh->vertexlayers.Find((GRPVERTEX_CHANNEL)this->layertype)==NOTFOUND)
        mesh->vertexlayers.Add((GRPVERTEX_CHANNEL)this->layertype);
  }

  *position=(*position)+size;
  return true;
}


/*-------------------------------------------------------------------
//  GGF::LOCATOR_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 16:47:59
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFLOCATOR_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK*  block=new GGFSECTIONBLOCK();
                    block->SetType(GGF_SECTION_LOCATOR);

  GRPNODE* node=static_cast<GRPNODE*>(pnode);

  int calc_size=1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XDWORD)+ //size
                1*sizeof(XDWORD)+ //tag

                (node->GetName()->GetSize()+1)*sizeof(XWORD)+ //name

                1*sizeof(XBYTE)+ //node mode
                1*sizeof(double)+ //Value
                1*sizeof(XBYTE)+ //UsesGeometric
                1*sizeof(XBYTE)+ //UsesGeometricTranslation
                1*sizeof(XBYTE)+ //UsesGeometricScale
                1*sizeof(XBYTE)+ //UsesGeometricRotation

                1*sizeof(XBYTE)+ //UsesRotationOffset
                1*sizeof(XBYTE)+ //UsesRotationPivot
                1*sizeof(XBYTE)+ //UsesScalingOffset
                1*sizeof(XBYTE)+ //UsesScalingPivot
                1*sizeof(XBYTE)+ //UsesPreRotation
                1*sizeof(XBYTE)+ //UsesPostRotation

                1*sizeof(double)+ //visibility
                1*sizeof(XBYTE)+ //visibility_inheritance
                1*sizeof(XBYTE)+ //rendereable

                4*sizeof(float)+ //RotationOffset
                4*sizeof(float)+ //RotationPivot
                4*sizeof(float)+ //ScalingOffset
                4*sizeof(float)+ //ScalingPivot

                4*sizeof(float)+ //prerotation
                4*sizeof(float)+ //postrotation

                1*sizeof(XBYTE)+ //InheritType

                4*sizeof(float)+ //position
                4*sizeof(float)+ //rotation
                4*sizeof(float)+ //scale

                4*sizeof(float)+ //geometric position
                4*sizeof(float)+ //geometric rotation
                4*sizeof(float); //geometric scale


  block->Set(calc_size);
      XQWORD pos=0;
    ///----------------------------------------- HEADER
  if (block->Get())
  {

    XBYTE type                        = GGF_SECTION_LOCATOR;                      memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));     pos+=sizeof(XBYTE);
                                                                                  memcpy(&(block->Get()[pos]),&calc_size,                           sizeof(XDWORD));    pos+=sizeof(XDWORD);
    XDWORD tag                        =(XDWORD)(void*)node;                       memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));    pos+=sizeof(XDWORD);

    f3d->WriteString(node->GetName(),block,&pos);

    XBYTE nodemode                    = node->NodeMode                  ;         memcpy(&(block->Get()[pos]),&nodemode,                            sizeof(XBYTE));     pos+=sizeof(XBYTE);
    double value                      = node->Value                     ;         memcpy(&(block->Get()[pos]),&value,                               sizeof(double));    pos+=sizeof(double);

    XBYTE UsesGeometric               = node->UsesGeometric             ;         memcpy(&(block->Get()[pos]),&UsesGeometric,                       sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE UsesGeometricTranslation    = node->UsesGeometricTranslation  ;         memcpy(&(block->Get()[pos]),&UsesGeometricTranslation,            sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE UsesGeometricScale          = node->UsesGeometricScale        ;         memcpy(&(block->Get()[pos]),&UsesGeometricScale,                  sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE UsesGeometricRotation       = node->UsesGeometricRotation     ;         memcpy(&(block->Get()[pos]),&UsesGeometricRotation,               sizeof(XBYTE));     pos+=sizeof(XBYTE);

    XBYTE UsesRotationOffset          = node->UsesRotationOffset        ;         memcpy(&(block->Get()[pos]),&UsesRotationOffset,                  sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE UsesRotationPivot           = node->UsesRotationPivot         ;         memcpy(&(block->Get()[pos]),&UsesRotationPivot,                   sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE UsesScalingOffset           = node->UsesScalingOffset         ;         memcpy(&(block->Get()[pos]),&UsesScalingOffset,                   sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE UsesScalingPivot            = node->UsesScalingPivot          ;         memcpy(&(block->Get()[pos]),&UsesScalingPivot,                    sizeof(XBYTE));     pos+=sizeof(XBYTE);

    XBYTE UsesPreRotation             = node->UsesPreRotation           ;         memcpy(&(block->Get()[pos]),&UsesPreRotation,                     sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE UsesPostRotation            = node->UsesPostRotation          ;         memcpy(&(block->Get()[pos]),&UsesPostRotation,                    sizeof(XBYTE));     pos+=sizeof(XBYTE);

    double visibility                 = node->visibility                ;         memcpy(&(block->Get()[pos]),&visibility,                          sizeof(double));    pos+=sizeof(double);
    XBYTE visibility_inheritance      = node->visibility_inheritance    ;         memcpy(&(block->Get()[pos]),&visibility_inheritance,              sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE rendereable                 = node->rendereable               ;         memcpy(&(block->Get()[pos]),&visibility_inheritance,              sizeof(XBYTE));     pos+=sizeof(XBYTE);

    float* RotationOffset             = node->RotationOffset.vector     ;         memcpy(&(block->Get()[pos]),RotationOffset,                     4*sizeof(float));     pos+=4*sizeof(float);
    float* RotationPivot              = node->RotationPivot.vector      ;         memcpy(&(block->Get()[pos]),RotationPivot,                      4*sizeof(float));     pos+=4*sizeof(float);
    float* ScalingOffset              = node->ScalingOffset.vector      ;         memcpy(&(block->Get()[pos]),ScalingOffset,                      4*sizeof(float));     pos+=4*sizeof(float);
    float* ScalingPivot               = node->ScalingPivot.vector       ;         memcpy(&(block->Get()[pos]),ScalingPivot,                       4*sizeof(float));     pos+=4*sizeof(float);

    float* prerotation                = node->prerotation.vector        ;         memcpy(&(block->Get()[pos]),prerotation,                        4*sizeof(float));     pos+=4*sizeof(float);
    float* postrotation               = node->postrotation.vector       ;         memcpy(&(block->Get()[pos]),postrotation,                       4*sizeof(float));     pos+=4*sizeof(float);

    XBYTE InheritType                 = node->InheritType               ;         memcpy(&(block->Get()[pos]),&InheritType,                         sizeof(XBYTE));     pos+=sizeof(XBYTE);

const float* position                 = node->GetPosition()->vector     ;         memcpy(&(block->Get()[pos]),position,                           4*sizeof(float));     pos+=4*sizeof(float);
const float* rotation                 = node->GetRotation()->vector     ;         memcpy(&(block->Get()[pos]),rotation,                           4*sizeof(float));     pos+=4*sizeof(float);
const float* scale                    = node->GetScale()->vector        ;         memcpy(&(block->Get()[pos]),scale,                              4*sizeof(float));     pos+=4*sizeof(float);

const float* geometricposition        = node->GetGeometricPosition()->vector;     memcpy(&(block->Get()[pos]),geometricposition,                  4*sizeof(float));     pos+=4*sizeof(float);
const float* geometricrotation        = node->GetGeometricRotation()->vector;     memcpy(&(block->Get()[pos]),geometricrotation,                  4*sizeof(float));     pos+=4*sizeof(float);
const float* geometricscale           = node->GetGeometricScale()->vector;        memcpy(&(block->Get()[pos]),geometricscale,                     4*sizeof(float));     pos+=4*sizeof(float);
  }

  GGFPROPERTY*  property=new GGFPROPERTY();

                property->PropertyType  =GGF_SECTION_LOCATOR;
                property->ownertag      =(XDWORD)(void*)node;
                property->ownertype     =GGF_SECTION_LOCATOR;

                if (node->GetParent()!=NULL)
                {
                  switch(node->GetParent()->GetType())
                   {
                   case GRPNAMED_TYPE_GRPNODE:        property->referencetype=GGF_SECTION_LOCATOR;      break;
                   case GRPNAMED_TYPE_GRPMATERIAL:    property->referencetype=GGF_SECTION_MATERIAL;     break;
                   case GRPNAMED_TYPE_GRPTEXTURE:     property->referencetype=GGF_SECTION_TEXTURE;      break;
                   case GRPNAMED_TYPE_GRPCAMERA:      property->referencetype=GGF_SECTION_CAMERA;       break;
                   case GRPNAMED_TYPE_GRPLIGHT:       property->referencetype=GGF_SECTION_LIGHT;        break;
                   case GRPNAMED_TYPE_GRPDEFORMER:    property->referencetype=GGF_SECTION_DEFORMER;     break;
                   case GRPNAMED_TYPE_GRPSUBDEFORMER: property->referencetype=GGF_SECTION_SUBDEFORMER;  break;
                   }
                property->referencetag=(XDWORD)(void*)(node->GetParent());
                }
                else
                {
                property->referencetag=0;
                property->referencetype=GGF_SECTION_LOCATOR;
                }




  f3d->Properties.Add(property);

  //----------------------------------------- assign material
    if (node->GetMaterial())
    {
      GGFPROPERTY* property=new GGFPROPERTY();
      if (property)
      {
        property->PropertyType=GGF_SECTION_ELEMENT;

        property->ownertype=GGF_SECTION_LOCATOR;
        property->ownertag=(XDWORD)(void*)node;

        property->referencetype=GGF_SECTION_MATERIAL;
        property->referencetag=(XDWORD)(void*)node->GetMaterial();

        f3d->GetProperties()->Add(property);
      }
    }

  //------------------------------------------------------ USER DATA
  {
      GGFSECTIONBUILDER*  builder=f3d->GetBuilders()->Get(GGF_SECTION_DATA);
      GGFSECTIONBLOCK*    data_block  =builder->CreateBlock(f3d,node->GetUserData());
                          block->Blocks.Add(data_block);

      if (node->GetUserData()->GetSize())
      {
      GGFPROPERTY*  property=new GGFPROPERTY();
                    property->PropertyType=GGF_SECTION_LOCATOR;

                    property->ownertag=(XDWORD)(void*)node;
                    property->ownertype=GGF_SECTION_LOCATOR;

                    property->referencetag=(XDWORD)(void*)node->GetUserData();
                    property->referencetype=GGF_SECTION_DATA;

      f3d->Properties.Add(property);
      }

  }

  return block;
}


/*-------------------------------------------------------------------
//  GGF::LOCATOR_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 17:02:30
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        GRPFILE3DGGF* :
*/
/*-----------------------------------------------------------------*/
bool GGFLOCATOR_BUILDER::ReadBlock    (GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  GRPNODE* node=f3d->object->CreateNode();

  ///----------------------------------------- HEADER

  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;


  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_LOCATOR)
  {
    f3d->Output(4,__L("GGFLOCATOR_BUILDER::ReadBlock Unexpected section!"));
    return false;
  }
  XDWORD tag;

  memcpy(&size,         &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&tag,          &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  f3d->ReadString(node->GetName(),buffer,position,&pos);

  memcpy(&node->NodeMode,                   &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&node->Value,                      &(buffer[pos]), sizeof(double));              pos+=sizeof(double);

  memcpy(&node->UsesGeometric,              &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&node->UsesGeometricTranslation,   &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&node->UsesGeometricScale,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&node->UsesGeometricRotation,      &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  memcpy(&node->UsesRotationOffset,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&node->UsesRotationPivot,          &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&node->UsesScalingOffset,          &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&node->UsesScalingPivot,           &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  memcpy(&node->UsesPreRotation,            &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&node->UsesPostRotation,           &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  memcpy(&node->visibility,                 &(buffer[pos]), sizeof(double));              pos+=sizeof(double);
  memcpy(&node->visibility_inheritance,     &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  memcpy(&node->rendereable,                &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  memcpy(&node->RotationOffset,             &(buffer[pos]), 4*sizeof(float));             pos+=4*sizeof(float);
  memcpy(&node->RotationPivot,              &(buffer[pos]), 4*sizeof(float));             pos+=4*sizeof(float);
  memcpy(&node->ScalingOffset,              &(buffer[pos]), 4*sizeof(float));             pos+=4*sizeof(float);
  memcpy(&node->ScalingPivot,               &(buffer[pos]), 4*sizeof(float));             pos+=4*sizeof(float);

  memcpy(&node->prerotation,                &(buffer[pos]), 4*sizeof(float));             pos+=4*sizeof(float);
  memcpy(&node->postrotation,               &(buffer[pos]), 4*sizeof(float));             pos+=4*sizeof(float);

  memcpy(&node->InheritType,                &(buffer[pos]),   sizeof(XBYTE));             pos+=sizeof(XBYTE);   float vector[4];

  memcpy(vector,                            &(buffer[pos]), 4*sizeof(float));             pos+=4*sizeof(float); node->SetPosition           (vector[0],vector[1],vector[2]);
  memcpy(vector,                            &(buffer[pos]), 4*sizeof(float));             pos+=4*sizeof(float); node->SetRotation           (vector[0],vector[1],vector[2]);
  memcpy(vector,                            &(buffer[pos]), 4*sizeof(float));             pos+=4*sizeof(float); node->SetScale              (vector[0],vector[1],vector[2]);

  memcpy(vector,                            &(buffer[pos]), 4*sizeof(float));             pos+=4*sizeof(float); node->SetGeometricPosition  (vector[0],vector[1],vector[2]);
  memcpy(vector,                            &(buffer[pos]), 4*sizeof(float));             pos+=4*sizeof(float); node->SetGeometricRotation  (vector[0],vector[1],vector[2]);
  memcpy(vector,                            &(buffer[pos]), 4*sizeof(float));             pos+=4*sizeof(float); node->SetGeometricScale     (vector[0],vector[1],vector[2]);

  node->modified        =true;
  node->positionChanged =true;
  node->rotationChanged =true;
  node->scaleChanged    =true;


  node->Init      ();
  node->Update    ();
  node->SetTag    (tag);

  f3d->nodes.Add  (node);
  *position=(*position)+size;

  if (!f3d->GetBuilders()->Get(GGF_SECTION_DATA)->ReadBlock(f3d,buffer,position))   return false;

  return true;
}


/*-------------------------------------------------------------------
//  GGF::CAMERA_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 16:47:59
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFCAMERA_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
  block->SetType(GGF_SECTION_CAMERA);

  GRPCAMERA* camera=static_cast<GRPCAMERA*>(pnode);

int calc_size=  1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XDWORD)+ //size
                1*sizeof(XDWORD)+ //tag

                (camera->GetName()->GetSize()+1)*sizeof(XWORD)+//name

                1*sizeof(XBYTE)+ //type
                4*sizeof(GLFLOAT)+ //EyePosition
                4*sizeof(GLFLOAT)+ //UpVector
                4*sizeof(GLFLOAT)+ //InterestPosition

                1*sizeof(GLFLOAT)+ //AspectWidth
                1*sizeof(GLFLOAT)+ //AspectHeight

                1*sizeof(GLFLOAT)+ //FilmWidth
                1*sizeof(GLFLOAT)+ //FilmHeight

                1*sizeof(GLFLOAT)+ //NearPlane
                1*sizeof(GLFLOAT)+ //FarPlane

                1*sizeof(GLFLOAT)+ //fov
                1*sizeof(GLFLOAT)+ //lens

                1*sizeof(GLFLOAT)+ //FocalLength
                1*sizeof(GLFLOAT)+ //BackPlaneDistance
                1*sizeof(GLFLOAT)+ //BackPlaneScaleX
                1*sizeof(GLFLOAT)+ //BackPlaneScaleY
                1*sizeof(XBYTE)+ //UseFocalLength
                1*sizeof(XBYTE)+  //name string size
                ((camera->GetTarget()) ?  camera->GetTarget()->GetName()->GetSize()*sizeof(XCHAR) : 0);//name



  block->Set(calc_size);
      XQWORD pos=0;
    ///----------------------------------------- HEADER
  if (block->Get())
  {

    XBYTE type                        = GGF_SECTION_CAMERA;                       memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));     pos+=sizeof(XBYTE);
                                                                                  memcpy(&(block->Get()[pos]),&calc_size,                           sizeof(XDWORD));    pos+=sizeof(XDWORD);
    XDWORD tag                        =(XDWORD)(void*)camera;                     memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));    pos+=sizeof(XDWORD);

    f3d->WriteString(camera->GetName(),block,&pos);

    GRPCAMERA_TYPE Type               = camera->GetCameraType           ();       memcpy(&(block->Get()[pos]),&Type,                                sizeof(XBYTE));     pos+=sizeof(XBYTE);

    GRPVECTOR* ep=camera->GetEyePosition      ();
    GRPVECTOR* up=camera->GetUpVector         ();
    GRPVECTOR* ip=camera->Getinterestposition ();

    GLFLOAT EyePosition [4];
            EyePosition [0]=ep->vector  [0];
            EyePosition [1]=ep->vector  [1];
            EyePosition [2]=ep->vector  [2];                                      memcpy(&(block->Get()[pos]),&EyePosition,                       4*sizeof(GLFLOAT));     pos+=4*sizeof(GLFLOAT);

    GLFLOAT UpVector    [4];
            UpVector    [0]=up->vector  [0];
            UpVector    [1]=up->vector  [1];
            UpVector    [2]=up->vector  [2];                                      memcpy(&(block->Get()[pos]),&UpVector,                          4*sizeof(GLFLOAT));     pos+=4*sizeof(GLFLOAT);

    GLFLOAT Interest    [4];
            Interest    [0]=ip->vector  [0];
            Interest    [1]=ip->vector  [1];
            Interest    [2]=ip->vector  [2];                                      memcpy(&(block->Get()[pos]),&Interest,                          4*sizeof(GLFLOAT));     pos+=4*sizeof(GLFLOAT);

    GLFLOAT AspectWidth               = camera->AspectWidth;                      memcpy(&(block->Get()[pos]),&AspectWidth,                       sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
    GLFLOAT AspectHeight              = camera->AspectHeight;                     memcpy(&(block->Get()[pos]),&AspectHeight,                      sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);

    GLFLOAT FilmWidth                 = camera->FilmWidth;                        memcpy(&(block->Get()[pos]),&FilmWidth,                         sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
    GLFLOAT FilmHeight                = camera->FilmHeight;                       memcpy(&(block->Get()[pos]),&FilmHeight,                        sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);

    GLFLOAT NearPlane                 = camera->NearPlane;                        memcpy(&(block->Get()[pos]),&NearPlane,                         sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
    GLFLOAT FarPlane                  = camera->FarPlane;                         memcpy(&(block->Get()[pos]),&FarPlane,                          sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);

    GLFLOAT fov                       = camera->fov;                              memcpy(&(block->Get()[pos]),&fov,                               sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
    GLFLOAT lens                      = camera->lens;                             memcpy(&(block->Get()[pos]),&lens,                              sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);

    GLFLOAT FocalLength               = camera->FocalLength;                      memcpy(&(block->Get()[pos]),&FocalLength,                       sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
    GLFLOAT BackPlaneDistance         = camera->BackPlaneDistance;                memcpy(&(block->Get()[pos]),&BackPlaneDistance,                 sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
    GLFLOAT BackPlaneScaleX           = camera->BackPlaneScaleX;                  memcpy(&(block->Get()[pos]),&BackPlaneScaleX,                   sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
    GLFLOAT BackPlaneScaleY           = camera->BackPlaneScaleY;                  memcpy(&(block->Get()[pos]),&BackPlaneScaleY,                   sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);

    XBYTE   UseFocalLength            = camera->UseFocalLength;                   memcpy(&(block->Get()[pos]),&UseFocalLength,                    sizeof(XBYTE));       pos+=sizeof(XBYTE);

    if ( camera->GetCameraType()==GRPCAMERA_TYPE_TARGET)
    {
    XDWORD  length                    = camera->GetTarget()->GetName()->GetSize();memcpy(&(block->Get()[pos]),&length,                              sizeof(XBYTE));     pos+=sizeof(XBYTE);
                                                                    if (length)   memcpy(&(block->Get()[pos]),camera->GetTarget()->GetName()->Get(),length*sizeof(XCHAR));    pos+=length*sizeof(XCHAR);
    }
    else
    {
      XDWORD  length                    = 0;                                      memcpy(&(block->Get()[pos]),&length,                              sizeof(XBYTE));     pos+=sizeof(XBYTE);
    }

  }


  return block;
}


/*-------------------------------------------------------------------
//  GGF::CAMERA_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 17:02:30
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        GRPFILE3DGGF* f3d: ggf file loader
//  @param        XBYTE*        buffer: datachunk
//  @param        XQWORD*       position: current buffer position holder
*/
/*-----------------------------------------------------------------*/
bool GGFCAMERA_BUILDER::ReadBlock   (GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{

  //GRPCAMERA* camera=new GRPCAMERA();
    GRPCAMERA* camera = f3d->object->CreateCamera();


  XDWORD type=0;
  XQWORD pos=*position;
  XDWORD size=0;
  XDWORD name_length=0;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (type!=GGF_SECTION_CAMERA)
  {
    f3d->Output(4,__L("GGFCAMERA_BUILDER::ReadBlock Unexpected Section! : %d"),type);
    return false;
  }

  memcpy(&size,         &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&camera->tag,  &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  f3d->ReadString(camera->GetName(),buffer,position,&pos);

    GRPCAMERA_TYPE  Type    = (GRPCAMERA_TYPE)0;  memcpy(&Type,               &(buffer[pos]),   sizeof(XBYTE));     pos+=   sizeof(XBYTE);     camera->SetType              (Type);
    GLFLOAT         ep[4];                        memcpy(ep,                  &(buffer[pos]), 4*sizeof(GLFLOAT));   pos+= 4*sizeof(GLFLOAT);   camera->SetEyePosition       (ep[0],ep[1],ep[2]);
    GLFLOAT         up[4];                        memcpy(up,                  &(buffer[pos]), 4*sizeof(GLFLOAT));   pos+= 4*sizeof(GLFLOAT);   camera->SetUpVector          (up[0],up[1],up[2]);
    GLFLOAT         ip[4];                        memcpy(ip,                  &(buffer[pos]), 4*sizeof(GLFLOAT));   pos+= 4*sizeof(GLFLOAT);   camera->Setinterestposition  (ip[0],ip[1],ip[2]);
    GLFLOAT         AspectWidth;                  memcpy(&AspectWidth,        &(buffer[pos]),   sizeof(GLFLOAT));   pos+=   sizeof(GLFLOAT);
    GLFLOAT         AspectHeight;                 memcpy(&AspectHeight,       &(buffer[pos]),   sizeof(GLFLOAT));   pos+=   sizeof(GLFLOAT);   camera->SetAspect            (AspectWidth, AspectHeight);
    GLFLOAT         FilmWidth;                    memcpy(&FilmWidth,          &(buffer[pos]),   sizeof(GLFLOAT));   pos+=   sizeof(GLFLOAT);
    GLFLOAT         FilmHeight;                   memcpy(&FilmHeight,         &(buffer[pos]),   sizeof(GLFLOAT));   pos+=   sizeof(GLFLOAT);   camera->SetFilm              (FilmWidth,   FilmHeight);

    GLFLOAT         NearPlane;                    memcpy(&NearPlane,          &(buffer[pos]),   sizeof(GLFLOAT));   pos+=   sizeof(GLFLOAT);   camera->NearPlane=NearPlane;
    GLFLOAT         FarPlane;                     memcpy(&FarPlane,           &(buffer[pos]),   sizeof(GLFLOAT));   pos+=   sizeof(GLFLOAT);   camera->FarPlane=FarPlane;

    GLFLOAT         fov;                          memcpy(&fov,                &(buffer[pos]),   sizeof(GLFLOAT));   pos+=   sizeof(GLFLOAT);   camera->fov=fov;
    GLFLOAT         lens;                         memcpy(&lens,               &(buffer[pos]),   sizeof(GLFLOAT));   pos+=   sizeof(GLFLOAT);   camera->lens=lens;

    GLFLOAT         FocalLength;                  memcpy(&FocalLength,        &(buffer[pos]),   sizeof(GLFLOAT));   pos+=   sizeof(GLFLOAT);   camera->FocalLength=FocalLength;
    GLFLOAT         BackPlaneDistance;            memcpy(&BackPlaneDistance,  &(buffer[pos]),   sizeof(GLFLOAT));   pos+=   sizeof(GLFLOAT);   camera->BackPlaneDistance=BackPlaneDistance;
    GLFLOAT         BackPlaneScaleX;              memcpy(&BackPlaneScaleX,    &(buffer[pos]),   sizeof(GLFLOAT));   pos+=   sizeof(GLFLOAT);   camera->BackPlaneScaleX=BackPlaneScaleX;
    GLFLOAT         BackPlaneScaleY;              memcpy(&BackPlaneScaleY,    &(buffer[pos]),   sizeof(GLFLOAT));   pos+=   sizeof(GLFLOAT);   camera->BackPlaneScaleY=BackPlaneScaleY;

    XBYTE           UseFocalLength;               memcpy(&UseFocalLength,     &(buffer[pos]),   sizeof(XBYTE));     pos+=   sizeof(XBYTE);      camera->UseFocalLength=(UseFocalLength!=0);
    XBYTE           target_name_length;           memcpy(&target_name_length, &(buffer[pos]),   sizeof(XBYTE));     pos+=   sizeof(XBYTE);
    XSTRING target;

                if (target_name_length)
                {
                    XCHAR* target_name=new XCHAR[target_name_length+1];
                    memcpy(target_name,         &(buffer[pos]), sizeof(XCHAR)*target_name_length);    pos+=sizeof(XCHAR)*target_name_length;
                    target_name[target_name_length]=__C('\0');
                    target.Set(target_name);
                    delete(target_name);
                }

  *position=pos;

    GRPNODE* node=f3d->object->FindNode(camera->GetName()->Get());
            if (node)
            {
              camera->Copy(node);
              int i=f3d->object->GetNodes()->Find(node);
              f3d->object->GetNodes()->DeleteIndex(i);
              f3d->object->GetNodes()->Insert(i,camera);
              f3d->nodes.Delete(node);
              delete(node);
            }

    if (camera->GetCameraType()==GRPCAMERA_TYPE_TARGET)
    {
            node=f3d->object->FindNode(target.Get());
            if (node)
            {
              camera->SetTarget(node);
            }
    }


    camera->modified=true;
    camera->Update();
    camera->BuildLookAtMatrix();

    f3d->cameras.Add(camera);
    f3d->nodes.Add  (camera);
    f3d->object->Add(camera);


  return true;
}

/*-------------------------------------------------------------------
//  GGF::LIGHT_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/04/2017 16:47:59
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFLIGHT_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK*  block=new GGFSECTIONBLOCK();
                    block->SetType(GGF_SECTION_LIGHT);

  GRPLIGHT* light=static_cast<GRPLIGHT*>(pnode);

  int calc_size=1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XDWORD)+ //size
                1*sizeof(XDWORD)+ //tag

                (light->GetName()->GetSize()+1)*sizeof(XWORD)+ //name

                1*sizeof(XBYTE)+ //Decay Type
                1*sizeof(XBYTE)+ //Light Type
                1*sizeof(GLFLOAT)+ //Strength
                1*sizeof(GLFLOAT)+ //Multiplier
                1*sizeof(GLFLOAT)+ //ConstantAttenuation
                1*sizeof(GLFLOAT)+ //LinearAttenuation
                1*sizeof(GLFLOAT)+ //QuadraticAttenuation
                1*sizeof(GLFLOAT)+ //CubicAttenuation

                4*sizeof(GLFLOAT)+ //DiffuseColor
                4*sizeof(GLFLOAT)+ //Ambient
                4*sizeof(GLFLOAT)+ //Specular

                1*sizeof(GLFLOAT)+ //SpotInner
                1*sizeof(GLFLOAT)+ //SpotOuter

                1*sizeof(GLFLOAT)+ //LightScaleFactor
                1*sizeof(GLFLOAT)+ //shadowquality
                1*sizeof(GLFLOAT); //bias

  block->Set(calc_size);
      XQWORD pos=0;
    ///----------------------------------------- HEADER
  if (block->Get())
  {

    XBYTE type                        = GGF_SECTION_LIGHT;                        memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));     pos+=sizeof(XBYTE);
                                                                                  memcpy(&(block->Get()[pos]),&calc_size,                           sizeof(XDWORD));    pos+=sizeof(XDWORD);
    XDWORD tag                        =(XDWORD)(void*)light;                      memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));    pos+=sizeof(XDWORD);

    f3d->WriteString(light->GetName(),block,&pos);

    XBYTE decay                       = light->GetDecayType           ();         memcpy(&(block->Get()[pos]),&decay,                               sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE lighttype                   = light->GetType                ();         memcpy(&(block->Get()[pos]),&lighttype,                           sizeof(XBYTE));     pos+=sizeof(XBYTE);
    GLFLOAT strength                  = light->GetStrength            ();         memcpy(&(block->Get()[pos]),&strength,                            sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
    GLFLOAT Multiplier                = light->GetMultiplier          ();         memcpy(&(block->Get()[pos]),&Multiplier,                          sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
    GLFLOAT ConstantAttenuation       = light->GetConstantAttenuation ();         memcpy(&(block->Get()[pos]),&ConstantAttenuation,                 sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
    GLFLOAT LinearAttenuation         = light->GetLinearAttenuation   ();         memcpy(&(block->Get()[pos]),&LinearAttenuation,                   sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
    GLFLOAT QuadraticAttenuation      = light->GetQuadraticAttenuation();         memcpy(&(block->Get()[pos]),&QuadraticAttenuation,                sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
    GLFLOAT CubicAttenuation          = light->GetCubicAttenuation    ();         memcpy(&(block->Get()[pos]),&CubicAttenuation,                    sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);

    GLFLOAT DiffuseColor[4];
    DiffuseColor[0]                   = light->GetDiffuse().red;
    DiffuseColor[1]                   = light->GetDiffuse().green;
    DiffuseColor[2]                   = light->GetDiffuse().blue;
    DiffuseColor[3]                   = light->GetDiffuse().alpha;                memcpy(&(block->Get()[pos]),&DiffuseColor,                      4*sizeof(GLFLOAT));   pos+=4*sizeof(GLFLOAT);

    GLFLOAT AmbientColor[4];
    AmbientColor[0]                   = light->GetAmbient().red;
    AmbientColor[1]                   = light->GetAmbient().green;
    AmbientColor[2]                   = light->GetAmbient().blue;
    AmbientColor[3]                   = light->GetAmbient().alpha;                memcpy(&(block->Get()[pos]),&AmbientColor,                      4*sizeof(GLFLOAT));   pos+=4*sizeof(GLFLOAT);

    GLFLOAT SpecularColor[4];
    SpecularColor[0]                    = light->GetSpecular().red;
    SpecularColor[1]                    = light->GetSpecular().green;
    SpecularColor[2]                    = light->GetSpecular().blue;
    SpecularColor[3]                    = light->GetSpecular().alpha;             memcpy(&(block->Get()[pos]),&SpecularColor,                     4*sizeof(GLFLOAT));   pos+=4*sizeof(GLFLOAT);

    GLFLOAT SpotInner                   = light->GetInnerSpot     ();             memcpy(&(block->Get()[pos]),&SpotInner,                           sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
    GLFLOAT SpotOuter                   = light->GetOuterSpot     ();             memcpy(&(block->Get()[pos]),&SpotOuter,                           sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
    GLFLOAT LightScaleFactor            = light->GetScaleFactor   ();             memcpy(&(block->Get()[pos]),&LightScaleFactor,                    sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
    GLFLOAT shadowquality               = light->GetShadowQuality ();             memcpy(&(block->Get()[pos]),&shadowquality,                       sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
    GLFLOAT bias                        = light->GetBias          ();             memcpy(&(block->Get()[pos]),&bias,                                sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
  }

  return block;
}



/*-------------------------------------------------------------------
//  GGF::LIGHT_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 17:02:30
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        GRPFILE3DGGF* :
*/
/*-----------------------------------------------------------------*/
bool GGFLIGHT_BUILDER::ReadBlock    (GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  //GRPLIGHT* light=new GRPLIGHT();
    GRPLIGHT* light = f3d->object->CreateLight();


  ///----------------------------------------- HEADER

  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;


  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (type!=GGF_SECTION_LIGHT)
  {
    f3d->Output(4,__L("GGFLIGHT_BUILDER::ReadBlock  Unexpected Section! : %d"),type);
    return false;
  }

  memcpy(&size,         &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&light->tag,   &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  f3d->ReadString(light->GetName(),buffer,position,&pos);

    XBYTE   decay               = 0;      memcpy(&decay,                &(buffer[pos]), sizeof(XBYTE));     pos+=sizeof(XBYTE);     light->SetDecayType           ((GRPLIGHTDECAYTYPE)decay);
    XBYTE   lighttype           = 0;      memcpy(&lighttype,            &(buffer[pos]), sizeof(XBYTE));     pos+=sizeof(XBYTE);     light->SetType                (GRPLIGHTTYPE(lighttype));
    GLFLOAT strength            = 0.0f;   memcpy(&strength,             &(buffer[pos]), sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);   light->SetStrength            (strength);
    GLFLOAT Multiplier          = 0.0f;   memcpy(&Multiplier,           &(buffer[pos]), sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);   light->SetMultiplier          (Multiplier);
    GLFLOAT ConstantAttenuation = 0.0f;   memcpy(&ConstantAttenuation,  &(buffer[pos]), sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);   light->SetConstantAttenuation (ConstantAttenuation);
    GLFLOAT LinearAttenuation   = 0.0f;   memcpy(&LinearAttenuation,    &(buffer[pos]), sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);   light->SetLinearAttenuation   (LinearAttenuation);
    GLFLOAT QuadraticAttenuation= 0.0f;   memcpy(&QuadraticAttenuation, &(buffer[pos]), sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);   light->SetQuadraticAttenuation(QuadraticAttenuation);
    GLFLOAT CubicAttenuation    = 0.0f;   memcpy(&CubicAttenuation,     &(buffer[pos]), sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);   light->SetCubicAttenuation    (CubicAttenuation);

    GLFLOAT DiffuseColor  [4];            memcpy(&DiffuseColor,         &(buffer[pos]), 4*sizeof(GLFLOAT)); pos+=4*sizeof(GLFLOAT); light->SetDiffuse             (GRPCOLOR(DiffuseColor  [0],DiffuseColor  [1],  DiffuseColor  [2],  DiffuseColor  [3]));
    GLFLOAT AmbientColor  [4];            memcpy(&AmbientColor,         &(buffer[pos]), 4*sizeof(GLFLOAT)); pos+=4*sizeof(GLFLOAT); light->SetAmbient             (GRPCOLOR(AmbientColor  [0],AmbientColor  [1],  AmbientColor  [2],  AmbientColor  [3]));
    GLFLOAT SpecularColor [4];            memcpy(&SpecularColor,        &(buffer[pos]), 4*sizeof(GLFLOAT)); pos+=4*sizeof(GLFLOAT); light->SetSpecular            (GRPCOLOR(SpecularColor [0],SpecularColor [1],  SpecularColor [2],  SpecularColor [3]));

    GLFLOAT SpotInner           = 0.0f;   memcpy(&SpotInner,            &(buffer[pos]), sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);   light->SetInnerSpot           (SpotInner);
    GLFLOAT SpotOuter           = 0.0f;   memcpy(&SpotOuter,            &(buffer[pos]), sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);   light->SetOuterSpot           (SpotOuter);
    GLFLOAT LightScaleFactor    = 0.0f;   memcpy(&LightScaleFactor,     &(buffer[pos]), sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);   light->SetScaleFactor         (LightScaleFactor);
    GLFLOAT shadowquality       = 0.0f;   memcpy(&shadowquality,        &(buffer[pos]), sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);   light->SetShadowQuality       (shadowquality);
    GLFLOAT bias                = 0.0f;   memcpy(&bias,                 &(buffer[pos]), sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);   light->SetBias                (bias);

  *position=pos;


  GRPNODE* node=f3d->object->FindNode(light->GetName()->Get());
          if (node)
          {
            light->Copy(node);
          }
  f3d->object->Add(light);
  f3d->lights.Add (light);
  f3d->nodes.Add  (light);
  light->BuildData();
  return true;
}



/*-------------------------------------------------------------------
//  ANIMATION_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/05/2017 17:42:32
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d   : GRPFILE3DGGF*
//  @param        panimationcontrol : void*  -> GRPANIMATIONCONTROL*
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFANIMATION_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* object)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK*  block=new GGFSECTIONBLOCK();
                    block->SetType(GGF_SECTION_ANIMATION);

  GRPANIMATIONCONTROL* animationcontrol=static_cast<GRPANIMATIONCONTROL*>(object);

  int calc_size=  1*sizeof(XBYTE)     + //TYPE ELEMENT
                  1*sizeof(XDWORD)    + //size
                  1*sizeof(XDWORD)    + //tag
                  1*sizeof(GLFLOAT)   + //framespeed
                  1*sizeof(XBYTE)     + //loopmode
                  1*sizeof(XBYTE)     + //controltimeFPS
                  1*sizeof(XBYTE)     + //controltimeprotocoltype
                  1*sizeof(XWORD);      //nStacks

  XQWORD tag=(XQWORD)animationcontrol;

  block->Set(calc_size);

  XQWORD pos=0;
  if (block->Get())
  {
    XBYTE     type                    = GGF_SECTION_ANIMATION;                              memcpy(&(block->Get()[pos]),&type,                    sizeof(XBYTE));     pos+=sizeof(XBYTE);
                                                                                            memcpy(&(block->Get()[pos]),&calc_size,               sizeof(XDWORD));    pos+=sizeof(XDWORD);
    XDWORD    tag                     =(XDWORD)(void*)animationcontrol;                     memcpy(&(block->Get()[pos]),&tag,                     sizeof(XDWORD));    pos+=sizeof(XDWORD);
    GLFLOAT   framespeed              =animationcontrol->GetFrameSpeed              ();     memcpy(&(block->Get()[pos]),&framespeed,              sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
    XBYTE     loopmode                =animationcontrol->GetLoopMode                ();     memcpy(&(block->Get()[pos]),&loopmode,                sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE     controltimeFPS          =animationcontrol->GetControlTimeFPS          ();     memcpy(&(block->Get()[pos]),&controltimeFPS,          sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE     controltimeprotocoltype =animationcontrol->GetControlTimeProtocolType ();     memcpy(&(block->Get()[pos]),&controltimeprotocoltype, sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XWORD     nStacks                 =animationcontrol->GetStacks()->GetSize       ();     memcpy(&(block->Get()[pos]),&nStacks,                 sizeof(XWORD));     pos+=sizeof(XWORD);


  //----------------------------------------

  GGFPROPERTY*    property  =new GGFPROPERTY();
                  property->PropertyType  =GGF_SECTION_ANIMATION;

                //  property->owner.Set(f3d->object->GetName()->Get());
                  property->ownertype     =GGF_SECTION_OBJECT;
                  property->ownertag      =(XDWORD)(void*)f3d->object;

                //  property->reference.Set(f3d->object->GetName()->Get());
                  property->referencetype =GGF_SECTION_ANIMATION;
                  property->referencetag  =(XDWORD)(void*)animationcontrol;

    f3d->GetProperties()->Add(property);

  //----------------------------------------
    block->Blocks.SetAddInLimit(nStacks);
    for (XDWORD e=0;e<animationcontrol->GetStacks()->GetSize();e++)
    {
      GRPANIMATIONSTACK* stack=animationcontrol->GetStacks()->GetElement(e);
      block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_ANIMATION_STACK)->CreateBlock(f3d,stack));
    }
  }
  return block;
}




/*-------------------------------------------------------------------
//  ANIMATION_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/05/2017 9:27:50
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
*/
/*-----------------------------------------------------------------*/
bool GGFANIMATION_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  GRPANIMATIONCONTROL* animationcontrol=f3d->object->GetAnimationControl();
  f3d->Output(0,__L("loading animation control %s"),f3d->object->GetName()->Get());



  ///----------------------------------------- HEADER

  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;


  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (type!=GGF_SECTION_ANIMATION)
  {
    f3d->Output(4,__L("GGFANIMATION_BUILDER::ReadBlock  Unexpected Section! : %d"),type);
    return false;
  }
  memcpy(&size,         &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  XQWORD tag=0;
  memcpy(&tag,          &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  {
    GLFLOAT   framespeed              =0;     memcpy(&framespeed,               &(buffer[pos]), sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);   animationcontrol->SetFrameSpeed               (framespeed);
    XBYTE     loopmode                =0;     memcpy(&loopmode,                 &(buffer[pos]), sizeof(XBYTE));       pos+=sizeof(XBYTE);     animationcontrol->SetLoopMode                 ((GRPANIMATIONLOOPMODE)loopmode);
    XBYTE     controltimeFPS          =0;     memcpy(&controltimeFPS,           &(buffer[pos]), sizeof(XBYTE));       pos+=sizeof(XBYTE);     animationcontrol->SetControlTimeFPS           ((GRPANIMATIONCONTROLTIMEFPS) controltimeFPS);
    XBYTE     controltimeprotocoltype =0;     memcpy(&controltimeprotocoltype,  &(buffer[pos]), sizeof(XBYTE));       pos+=sizeof(XBYTE);     animationcontrol->SetControlTimeProtocolType  ((GRPANIMATIONCONTROLTIMEPROTOCOLTYPE)  controltimeprotocoltype);
    XWORD     nStacks                 =0;     memcpy(&nStacks,                  &(buffer[pos]), sizeof(XWORD));       pos+=sizeof(XWORD);

    f3d->animationcontrols.Add(animationcontrol);

    *position=pos;
    GGFSECTIONBUILDER* stackbuilder= f3d->GetBuilders()->Get(GGF_SECTION_ANIMATION_STACK);
    animationcontrol->GetStacks()->SetAddInLimit(nStacks);
    for (int e=0;e<nStacks;e++)
    {
        if (!stackbuilder->ReadBlock(f3d,buffer,position,(void*)animationcontrol))    return false;
    }


  }


  return true;
}


/*-------------------------------------------------------------------
//  GGFANIMATION_STACK_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/05/2017 11:12:40
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        object :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFANIMATION_STACK_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* object)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK*  block=new GGFSECTIONBLOCK();
                    block->SetType(GGF_SECTION_ANIMATION);

  GRPANIMATIONSTACK* stack=static_cast<GRPANIMATIONSTACK*>(object);


  int calc_size=1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XDWORD)+ //size
                1*sizeof(XDWORD)+ //tag

                (stack->GetName()->GetSize()+1)*sizeof(XWORD)+
                1*sizeof(XDWORD)    + //id

                1*sizeof(XQWORDSIG) + //maxframes
                1*sizeof(XQWORDSIG) + //minframes

                1*sizeof(XQWORDSIG) + //maxtime
                1*sizeof(XQWORDSIG) + //mintime

                1*sizeof(XBYTE)     + //status
                1*sizeof(GLFLOAT)   + //framespeed
                1*sizeof(GLFLOAT)   + //framespeed_acceleration
                1*sizeof(GLFLOAT)   + //framespeed_friction
                1*sizeof(XBYTE)     + //loopmode
                1*sizeof(XWORD)     + //nmilestones
                1*sizeof(XWORD);      //nlayers

  XQWORD tag=(XQWORD)stack;

  block->Set(calc_size);

  XQWORD pos=0;
  if (block->Get())
  {
    XBYTE     type                    = GGF_SECTION_ANIMATION_STACK;                memcpy(&(block->Get()[pos]),&type,                              sizeof(XBYTE));     pos+=sizeof(XBYTE);
                                                                                    memcpy(&(block->Get()[pos]),&calc_size,                         sizeof(XDWORD));    pos+=sizeof(XDWORD);
    XDWORD    tag                     =(XDWORD)(void*)stack;                        memcpy(&(block->Get()[pos]),&tag,                               sizeof(XDWORD));    pos+=sizeof(XDWORD);
    f3d->WriteString(stack->GetName(),block,&pos);

    XDWORD    id                      = stack->GetID();                             memcpy(&(block->Get()[pos]),&id,                                sizeof(XDWORD));    pos+=sizeof(XDWORD);

    XQWORDSIG maxframes               = stack->GetMaxFrame                  ();     memcpy(&(block->Get()[pos]),&maxframes,                         sizeof(XQWORDSIG)); pos+=sizeof(XQWORDSIG);
    XQWORDSIG minframes               = stack->GetMinFrame                  ();     memcpy(&(block->Get()[pos]),&minframes,                         sizeof(XQWORDSIG)); pos+=sizeof(XQWORDSIG);

    XQWORDSIG maxtime                 = stack->GetMaxTime                   ();     memcpy(&(block->Get()[pos]),&maxtime,                           sizeof(XQWORDSIG)); pos+=sizeof(XQWORDSIG);
    XQWORDSIG mintime                 = stack->GetMinTime                   ();     memcpy(&(block->Get()[pos]),&mintime,                           sizeof(XQWORDSIG)); pos+=sizeof(XQWORDSIG);

    XBYTE     active                  = stack->GetInitialActive             ();     memcpy(&(block->Get()[pos]),&active,                            sizeof(XBYTE));     pos+=sizeof(XBYTE);

    GLFLOAT   framespeed              = stack->GetFrameSpeed                ();     memcpy(&(block->Get()[pos]),&framespeed,                        sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
    GLFLOAT   framespeed_acc          = stack->GetFrameAcceleration         ();     memcpy(&(block->Get()[pos]),&framespeed_acc,                    sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
    GLFLOAT   framespeed_friction     = stack->GetFriction                  ();     memcpy(&(block->Get()[pos]),&framespeed_friction,               sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);

    XBYTE     loopmode                = stack->GetLoopMode                  ();     memcpy(&(block->Get()[pos]),&loopmode,                          sizeof(XBYTE));     pos+=sizeof(XBYTE);

    XWORD     nmilestones             =0;
    XDWORD nm=stack->GetMilestones()->GetSize();
    for (XDWORD e=0;e<nm;e++)
    {
      if (stack->GetMilestones()->FastGet(e)->GetSubjectType()==GRPNAMED_TYPE_GRPMILESTONE)
        nmilestones++;
    }
                                                                                    memcpy(&(block->Get()[pos]),&nmilestones,                       sizeof(XWORD));     pos+=sizeof(XWORD);
    XWORD     nlayers                 = stack->GetLayers()->GetSize         ();     memcpy(&(block->Get()[pos]),&nlayers,                           sizeof(XWORD));     pos+=sizeof(XWORD);


    block->Blocks.SetAddInLimit(nlayers+nmilestones);

  for (XDWORD e=0;e<stack->GetLayers()->GetSize();e++)
  {
    GRPANIMATIONLAYER* layer=stack->GetLayers()->Get(e);
    block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_ANIMATION_LAYER)->CreateBlock(f3d,layer));
  }

  for (XDWORD e=0;e<stack->GetMilestones()->GetSize();e++)
  {
    GRPANIMATIONSTACKMILESTONE* milestone=stack->GetMilestones()->Get(e);
    if (milestone->GetSubjectType()==GRPNAMED_TYPE_GRPMILESTONE)
        block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_ANIMATION_EVENT)->CreateBlock(f3d,milestone));
  }
}

  return block;
}

/*-------------------------------------------------------------------
//  GGFANIMATION_STACK_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/05/2017 12:11:05
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
*/
/*-----------------------------------------------------------------*/
bool GGFANIMATION_STACK_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  GRPANIMATIONSTACK* stack=new GRPANIMATIONSTACK();

///----------------------------------------- HEADER

  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;


  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (type!=GGF_SECTION_ANIMATION_STACK)
  {
    f3d->Output(4,__L("GGFANIMATION_STACK_BUILDER::ReadBlock  Unexpected Section! : %d"),type);
    return false;
  }
  memcpy(&size,         &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);


  memcpy(&stack->tag,   &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  f3d->ReadString(stack->GetName(),buffer,position,&pos);

  {
    XDWORD    id                      =0;     memcpy(&id,                       &(buffer[pos]), sizeof(XDWORD));        pos+=sizeof(XDWORD);    stack->SetID(id);

    XQWORDSIG maxframes               =0;     memcpy(&maxframes,                &(buffer[pos]), sizeof(XQWORDSIG));     pos+=sizeof(XQWORDSIG); stack->SetMaxFrame(maxframes);
    XQWORDSIG minframes               =0;     memcpy(&minframes,                &(buffer[pos]), sizeof(XQWORDSIG));     pos+=sizeof(XQWORDSIG); stack->SetMinFrame(minframes);

    XQWORDSIG maxtime                 =0;     memcpy(&maxtime,                  &(buffer[pos]), sizeof(XQWORDSIG));     pos+=sizeof(XQWORDSIG); stack->SetMaxTime(maxtime);
    XQWORDSIG mintime                 =0;     memcpy(&mintime,                  &(buffer[pos]), sizeof(XQWORDSIG));     pos+=sizeof(XQWORDSIG); stack->SetMinTime(mintime);

    XBYTE     status                  =0;     memcpy(&status,                   &(buffer[pos]), sizeof(XBYTE));         pos+=sizeof(XBYTE);
    GLFLOAT   framespeed              =0;     memcpy(&framespeed,               &(buffer[pos]), sizeof(GLFLOAT));       pos+=sizeof(GLFLOAT);   stack->SetFrameSpeed        (framespeed);
    GLFLOAT   framespeed_Acc          =0;     memcpy(&framespeed_Acc,           &(buffer[pos]), sizeof(GLFLOAT));       pos+=sizeof(GLFLOAT);   stack->SetFrameAcceleration (framespeed_Acc);
    GLFLOAT   framespeed_Friction     =0;     memcpy(&framespeed_Friction,      &(buffer[pos]), sizeof(GLFLOAT));       pos+=sizeof(GLFLOAT);   stack->SetFriction          (framespeed_Friction);

    XBYTE     loopmode                =0;     memcpy(&loopmode,                 &(buffer[pos]), sizeof(XBYTE));         pos+=sizeof(XBYTE);   stack->SetLoopMode((GRPANIMATIONLOOPMODE)loopmode);

    XWORD     nmilestones             =0;     memcpy(&nmilestones,              &(buffer[pos]), sizeof(XWORD));         pos+=sizeof(XWORD);
    XWORD     nlayers                 =0;     memcpy(&nlayers,                  &(buffer[pos]), sizeof(XWORD));         pos+=sizeof(XWORD);



    *position=(*position)+size;
    f3d->animationstacks.Add(stack);

    if (f3d->object)
      if (f3d->object->GetAnimationControl())
        f3d->object->GetAnimationControl()->AddStack(stack,stack->GetID());


    stack->GetLayers()->SetAddInLimit(nlayers);
    for (int e=0;e<nlayers;e++)
    {
        if (!f3d->GetBuilders()->Get(GGF_SECTION_ANIMATION_LAYER)->ReadBlock(f3d,buffer,position,(void*)stack))   return false;
    }

    stack->GetMilestones()->SetAddInLimit(nmilestones);
    for (int e=0;e<nmilestones;e++)
    {
        if (!f3d->GetBuilders()->Get(GGF_SECTION_ANIMATION_EVENT)->ReadBlock(f3d,buffer,position,(void*)stack))   return false;
    }

    stack->SetInitialActive(status!=0);
    stack->SetActive(status!=0);





  }




return true;
}

/*-------------------------------------------------------------------
//  GGFANIMATION_LAYER_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/05/2017 11:09:52
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        object :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFANIMATION_LAYER_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* object)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK*  block=new GGFSECTIONBLOCK();
                    block->SetType(GGF_SECTION_ANIMATION);

  GRPANIMATIONLAYER* layer=static_cast<GRPANIMATIONLAYER*>(object);
  //f3d->Output(0,__L("saving animation layer %s"),layer->GetName()->Get());


  int calc_size=1*sizeof(XBYTE)     +                       //TYPE ELEMENT
                1*sizeof(XDWORD)    +                       //size
                1*sizeof(XDWORD)    +                       //tag
                (layer->GetName()->GetSize()+1)*sizeof(XWORD)+
                1*sizeof(XQWORDSIG) +                       //maxtime
                1*sizeof(XQWORDSIG) +                       //mintime

                1*sizeof(XBYTE)     +                       //status
                1*sizeof(XBYTE)     +                       //loopmode

                1*sizeof(XWORD)     +                       //nnodes
                1*sizeof(XWORD);                            //ncurves

  block->Set(calc_size);

  XQWORD pos=0;
  if (block->Get())
  {
    XBYTE     type                    = GGF_SECTION_ANIMATION_LAYER;                memcpy(&(block->Get()[pos]),&type,                              sizeof(XBYTE));     pos+=sizeof(XBYTE);
                                                                                    memcpy(&(block->Get()[pos]),&calc_size,                         sizeof(XDWORD));    pos+=sizeof(XDWORD);
    XDWORD    tag                     =(XDWORD)(void*)layer;                        memcpy(&(block->Get()[pos]),&tag,                               sizeof(XDWORD));    pos+=sizeof(XDWORD);
    /*
    XBYTE     namelength              = layer->GetName()->GetSize();                memcpy(&(block->Get()[pos]),&namelength,                        sizeof(XBYTE));     pos+=sizeof(XBYTE);
                                                                  if (namelength)   memcpy(&(block->Get()[pos]),layer->GetName()->Get(), namelength*sizeof(XCHAR));     pos+=namelength*sizeof(XCHAR);
    */
    f3d->WriteString(layer->GetName(),block,&pos);

    XQWORDSIG maxtime                 = layer->GetMaxTime                   ();     memcpy(&(block->Get()[pos]),&maxtime,                           sizeof(XQWORDSIG)); pos+=sizeof(XQWORDSIG);
    XQWORDSIG mintime                 = layer->GetMinTime                   ();     memcpy(&(block->Get()[pos]),&mintime,                           sizeof(XQWORDSIG)); pos+=sizeof(XQWORDSIG);

    XBYTE     active                  = layer->GetStatus                    ();     memcpy(&(block->Get()[pos]),&active,                            sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE     loopmode                = layer->GetLoopMode                  ();     memcpy(&(block->Get()[pos]),&loopmode,                          sizeof(XBYTE));     pos+=sizeof(XBYTE);

    XWORD     nnodes                  = layer->GetAnimationNodes()->GetSize ();     memcpy(&(block->Get()[pos]),&nnodes,                            sizeof(XWORD));     pos+=sizeof(XWORD);
    XWORD     ncurves                 = layer->GetCurves()->GetSize ();             memcpy(&(block->Get()[pos]),&ncurves,                           sizeof(XWORD));     pos+=sizeof(XWORD);


    block->Blocks.SetAddInLimit         (layer->GetAnimationNodes()->GetSize()+layer->GetCurves()->GetSize());

    for (XDWORD e=0;e<layer->GetCurves()->GetSize();e++)
    {
      GRPANIMATIONCURVE* curve=layer->GetCurves()->Get(e);
      block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_ANIMATION_CURVE)->CreateBlock(f3d,curve));
    }

    for (XDWORD e=0;e<layer->GetAnimationNodes()->GetSize();e++)
    {
      GRPANIMATIONNODE* node=layer->GetAnimationNodes()->Get(e);
      block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_ANIMATION_NODE)->CreateBlock(f3d,node));
    }
  }

  return block;
}

/*-------------------------------------------------------------------
//  GGFANIMATION_LAYER_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/05/2017 12:50:44
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFANIMATION_LAYER_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{

  GRPANIMATIONSTACK* stack=static_cast<GRPANIMATIONSTACK*>(parent);

  GRPANIMATIONLAYER* layer=new GRPANIMATIONLAYER();

  if (!layer)
    return false;

  ///----------------------------------------- HEADER

  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;


  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (type!=GGF_SECTION_ANIMATION_LAYER)
  {
    f3d->Output(4,__L("GGFANIMATION_LAYER_BUILDER::ReadBlock  Unexpected Section! : %d"),type);
    return false;
  }
  memcpy(&size,         &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&layer->tag,   &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  f3d->ReadString(layer->GetName(),buffer,position,&pos);

  //------------------------------------

  XQWORDSIG maxtime =0 ; memcpy(&maxtime,         &(buffer[pos]), sizeof(XQWORDSIG));             pos+=sizeof(XQWORDSIG);   layer->SetMaxTime (maxtime);
  XQWORDSIG mintime =0 ; memcpy(&mintime,         &(buffer[pos]), sizeof(XQWORDSIG));             pos+=sizeof(XQWORDSIG);   layer->SetMinTime (mintime);

  XBYTE     status  =0 ; memcpy(&status,          &(buffer[pos]), sizeof(XBYTE));                 pos+=sizeof(XBYTE);       //layer->SetStatus  ((GRPANIMATION_STATUS)status);
  XBYTE     loopmode=0 ; memcpy(&loopmode,        &(buffer[pos]), sizeof(XBYTE));                 pos+=sizeof(XBYTE);       layer->SetLoopMode((GRPANIMATIONLOOPMODE)loopmode);

  XWORD     nnodes  =0 ; memcpy(&nnodes,          &(buffer[pos]), sizeof(XWORD));                 pos+=sizeof(XWORD);
  XWORD     ncurves =0 ; memcpy(&ncurves,         &(buffer[pos]), sizeof(XWORD));                 pos+=sizeof(XWORD);

  f3d->animationlayers.Add(layer);
  layer->SetActive(false);
  stack->AddLayer(layer);

  layer->GetCurves()->SetAddInLimit(ncurves);

    GGFSECTIONBUILDER* curvebuilder=f3d->GetBuilders()->Get(GGF_SECTION_ANIMATION_CURVE);
    for (int e=0;e<ncurves;e++)
    {
        if (!curvebuilder->ReadBlock(f3d,buffer,&pos,(void*)layer))   return false;
    }

  layer->GetAnimationNodes()->SetAddInLimit(nnodes);

    GGFSECTIONBUILDER* nodebuilder=f3d->GetBuilders()->Get(GGF_SECTION_ANIMATION_NODE);
    for (int e=0;e<nnodes;e++)
    {
        if (!nodebuilder->ReadBlock(f3d,buffer,&pos,(void*)layer))    return false;
    }
  //------------------------------------
  *position=pos;

  return true;
}

/*-------------------------------------------------------------------
//  GGFANIMATION_EVENT_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/05/2017 16:39:20
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        object :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFANIMATION_EVENT_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* object)
{
  GGFSECTIONBLOCK*  block=new GGFSECTIONBLOCK();
                    block->SetType(GGF_SECTION_ANIMATION_EVENT);

  GRPANIMATIONSTACKMILESTONE* milestone=static_cast<GRPANIMATIONSTACKMILESTONE*>(object);


  int calc_size=1*sizeof(XBYTE)     +                           //TYPE ELEMENT
                1*sizeof(XDWORD)    +                           //size
                1*sizeof(XDWORD)    +                           //tag
                /*
                1*sizeof(XBYTE)     +                           //name string size
                milestone->GetName()->GetSize()*sizeof(XCHAR) + //name
                */
              //  (milestone->GetName()->GetSize()+1)*sizeof(XWORD) + //name
                1*sizeof(XBYTE)     +                           //subject type
                1*sizeof(XBYTE)     +                           //milestone type
                1*sizeof(XBYTE)     +                           //disabled
                1*sizeof(XDWORD)    +                           //passes
                1*sizeof(float)     +                           //start_frame
                1*sizeof(float)     +                           //end_frame
                1*sizeof(float)     +                           //tension
                0;

  block->Set(calc_size);

  XQWORD pos=0;
  if (block->Get())
  {
    XBYTE     type          = GGF_SECTION_ANIMATION_EVENT;      memcpy(&(block->Get()[pos]),&type,          sizeof(XBYTE));     pos+=sizeof(XBYTE);
                                                                memcpy(&(block->Get()[pos]),&calc_size,     sizeof(XDWORD));    pos+=sizeof(XDWORD);
    XDWORD    tag           =(XDWORD)(void*)milestone;          memcpy(&(block->Get()[pos]),&tag,           sizeof(XDWORD));    pos+=sizeof(XDWORD);
    /*
    XBYTE     namelength    = milestone->GetName()->GetSize();  memcpy(&(block->Get()[pos]),&namelength,    sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XCHAR*    name          = milestone->GetName()->Get();
                                              if (namelength)   memcpy(&(block->Get()[pos]),name, namelength*sizeof(XCHAR));    pos+=namelength*sizeof(XCHAR);
    */
    //f3d->WriteString(milestone->GetName(),block,&pos);

    XBYTE     subjecttype   = milestone->GetSubjectType();      memcpy(&(block->Get()[pos]),&subjecttype,    sizeof(XBYTE));    pos+=sizeof(XBYTE);

    XBYTE     milestonetype = milestone->GetType      ();       memcpy(&(block->Get()[pos]),&milestonetype, sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XBYTE     disabled      = milestone->GetIsDisabled();       memcpy(&(block->Get()[pos]),&disabled,      sizeof(XBYTE));     pos+=sizeof(XBYTE);
    XDWORD    passes        = milestone->GetPasses    ();       memcpy(&(block->Get()[pos]),&passes,        sizeof(XDWORD));    pos+=sizeof(XDWORD);
    GLFLOAT   start_frame   = milestone->GetStartFrame();       memcpy(&(block->Get()[pos]),&start_frame,   sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
    GLFLOAT   end_frame     = milestone->GetEndFrame  ();       memcpy(&(block->Get()[pos]),&end_frame,     sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
    GLFLOAT   tension       = milestone->GetTension   ();       memcpy(&(block->Get()[pos]),&tension,       sizeof(GLFLOAT));   pos+=sizeof(GLFLOAT);
  }

  return block;
}


/*-------------------------------------------------------------------
//  GGFANIMATION_EVENT_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/05/2017 16:57:39
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFANIMATION_EVENT_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  GRPANIMATIONSTACK*          stack=static_cast<GRPANIMATIONSTACK*>(parent);
  GRPANIMATIONSTACKMILESTONE* milestone=new GRPANIMATIONSTACKMILESTONE();

  if (!milestone)
  {
    f3d->Output(4,__L("Not enought memory!"));
    return false;
  }

  ///----------------------------------------- HEADER

  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;
  XDWORD      target_length =0;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));                 pos+=sizeof(XBYTE);

  if (type!=GGF_SECTION_ANIMATION_EVENT)
  {
    f3d->Output(4,__L("GGF_SECTION_ANIMATION_EVENT::ReadBlock  Unexpected Section! : %d"),type);
    return false;
  }
  memcpy          (&size,           &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy          (&milestone->tag, &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
//  f3d->ReadString (milestone->GetName(),buffer,position,&pos);

  /*
  memcpy(&name_length,    &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (name_length)
  {
    XCHAR* name=new XCHAR[name_length+1];
    memcpy(name,          &(buffer[pos]), sizeof(XCHAR)*name_length);   pos+=sizeof(XCHAR)*name_length;
    name[name_length]=__C('\0');
    milestone->GetName()->Set(name);
    delete(name);
  }
  */
  /*
                1*sizeof(XBYTE)     +                           //milestone type
                1*sizeof(XBYTE)     +                           //disabled
                1*sizeof(XDWORD)    +                           //passes
                1*sizeof(float)     +                           //start_frame
                1*sizeof(float)     +                           //end_frame
                1*sizeof(float)     +                           //enter_speed
                1*sizeof(float)     +                           //tension
                1*sizeof(XBYTE)     +                           //isactive
                1*sizeof(XQWORD)    +                           //milestonelocaltime
                1*sizeof(XQWORD)    +                           //milestonelasttime
                0;
  */

  XBYTE   subjecttype   =0; memcpy(&subjecttype,    &(buffer[pos]), sizeof(XBYTE));       pos+=sizeof(XBYTE);
  XBYTE   milestonetype =0; memcpy(&milestonetype,  &(buffer[pos]), sizeof(XBYTE));       pos+=sizeof(XBYTE);
  XBYTE   status        =0; memcpy(&status,         &(buffer[pos]), sizeof(XBYTE));       pos+=sizeof(XBYTE);
  XDWORD  passes        =0; memcpy(&passes,         &(buffer[pos]), sizeof(XDWORD));      pos+=sizeof(XDWORD);
  GLFLOAT start_frame   =0; memcpy(&start_frame,    &(buffer[pos]), sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
  GLFLOAT end_frame     =0; memcpy(&end_frame,      &(buffer[pos]), sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
  GLFLOAT tension       =0; memcpy(&tension,        &(buffer[pos]), sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);

  milestone->SetType        (milestonetype);
  milestone->Enable         (status!=0);
  milestone->SetPasses      (passes);
  milestone->SetStartFrame  (start_frame);
  milestone->SetTension     (tension);
  milestone->SetEndFrame    (end_frame);
  milestone->Enable         (false);

  f3d->animationevents.Add  (milestone);
  *position=(*position)+size;

  stack->AddMilestone(milestone);

  return true;
}
/*-------------------------------------------------------------------
//  GGFANIMATION_NODE_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/05/2017 12:09:30
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        object :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFANIMATION_NODE_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* object)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK*  block=new GGFSECTIONBLOCK();
                    block->SetType(GGF_SECTION_ANIMATION_NODE);

  GRPANIMATIONNODE* node=static_cast<GRPANIMATIONNODE*>(object);

  int calc_size=1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XDWORD)+ //size
                1*sizeof(XDWORD)+ //tag
                /*
                1*sizeof(XBYTE)+  //name string size
                node->GetName()->GetSize()*sizeof(XCHAR)+ //name

                1*sizeof(XBYTE)+  //name string size
                node->GetTargetName()->GetSize()*sizeof(XCHAR)+ //name
                */
                (node->GetName()->GetSize()+1)*sizeof(XWORD)+
                (node->GetTargetName()->GetSize()+1)*sizeof(XWORD)+

                1*sizeof(XBYTE)+  //node_type
                1*sizeof(XWORD)+  //node_connections
                node->GetCurveConnections()->GetSize()*(sizeof(XDWORD)+sizeof(XWORD)); //connection data

  block->Set(calc_size);

  XQWORD pos=0;
  if (block->Get())
  {
    XBYTE     type                    = GGF_SECTION_ANIMATION_NODE;                 memcpy(&(block->Get()[pos]),&type,                                       sizeof(XBYTE));      pos+=sizeof(XBYTE);
                                                                                    memcpy(&(block->Get()[pos]),&calc_size,                                  sizeof(XDWORD));     pos+=sizeof(XDWORD);
    XDWORD    tag                     = (XDWORD)(void*)node;                        memcpy(&(block->Get()[pos]),&tag,                                        sizeof(XDWORD));     pos+=sizeof(XDWORD);
    /*
    XBYTE     namelength              = node->GetName()->GetSize();                 memcpy(&(block->Get()[pos]),&namelength,                                 sizeof(XBYTE));      pos+=sizeof(XBYTE);
                                                                  if (namelength)   memcpy(&(block->Get()[pos]),node->GetName()->Get(),           namelength*sizeof(XCHAR));      pos+=namelength*sizeof(XCHAR);
    XBYTE     targetlength            = node->GetTargetName()->GetSize();           memcpy(&(block->Get()[pos]),&targetlength,                               sizeof(XBYTE));      pos+=sizeof(XBYTE);
                                                                  if (targetlength) memcpy(&(block->Get()[pos]),node->GetTargetName()->Get(),   targetlength*sizeof(XCHAR));      pos+=targetlength*sizeof(XCHAR);
                                                                  */
    f3d->WriteString(node->GetName(),block,&pos);
    f3d->WriteString(node->GetTargetName(),block,&pos);

    XBYTE     node_type               = node->GetType();                            memcpy(&(block->Get()[pos]),&node_type,                                  sizeof(XBYTE));      pos+=sizeof(XBYTE);
    XWORD     nconnections            = node->GetCurveConnections()->GetSize();     memcpy(&(block->Get()[pos]),&nconnections,                               sizeof(XWORD));      pos+=sizeof(XWORD);

    for (XDWORD e=0;e<node->GetCurveConnections()->GetSize();e++)
    {
    XDWORD key              =node->GetCurveConnections()->GetKey    (e);
    GRPANIMATIONCURVE* curve=node->GetCurveConnections()->GetElement(e);

    memcpy(&(block->Get()[pos]),&key,     sizeof(XDWORD));    pos+=sizeof(XDWORD);
    XWORD curveID=node->GetLayer()->GetCurves()->Find(curve);
    memcpy(&(block->Get()[pos]),&curveID, sizeof(XWORD));     pos+=sizeof(XWORD);
    }

/*
    for (XDWORD e=0;e<node->GetCurveConnections()->GetSize();e++)
    {
    XDWORD key              =node->GetCurveConnections()->GetKey    (e);
    GRPANIMATIONCURVE* curve=node->GetCurveConnections()->GetElement(e);

    GGFPROPERTY* property=new GGFPROPERTY();
                 property->PropertyType=key;

                 property->owner.Set(node->name.Get());
                 property->ownertype=GGF_SECTION_ANIMATION_NODE;
                 property->ownertag=(XDWORD)(void*)node;

                 property->reference.Set(curve->name.Get());
                 property->referencetype=GGF_SECTION_ANIMATION_CURVE;
                 property->referencetag=(XDWORD)(void*)curve;

    f3d->GetProperties()->Add(property);
    }*/
  }

    GGFPROPERTY* property=new GGFPROPERTY();
                 property->PropertyType=GGF_SECTION_ANIMATION_NODE;

                 //property->owner.Set(node->name.Get());
                 property->ownertype=GGF_SECTION_ANIMATION_NODE;
                 property->ownertag=(XDWORD)(void*)node;

                // property->reference.Set(node->GetTargetName()->Get());
                 if (node->GetTarget())
                 {
                   switch(node->GetTarget()->GetType())
                   {
                   case GRPNAMED_TYPE_GRPNODE:        property->referencetype=GGF_SECTION_LOCATOR;      break;
                   case GRPNAMED_TYPE_GRPMATERIAL:    property->referencetype=GGF_SECTION_MATERIAL;     break;
                   case GRPNAMED_TYPE_GRPTEXTURE:     property->referencetype=GGF_SECTION_TEXTURE;      break;
                   case GRPNAMED_TYPE_GRPCAMERA:      property->referencetype=GGF_SECTION_CAMERA;       break;
                   case GRPNAMED_TYPE_GRPLIGHT:       property->referencetype=GGF_SECTION_LIGHT;        break;
                   case GRPNAMED_TYPE_GRPDEFORMER:    property->referencetype=GGF_SECTION_DEFORMER;     break;
                   case GRPNAMED_TYPE_GRPSUBDEFORMER: property->referencetype=GGF_SECTION_SUBDEFORMER;  break;
                   }
                 }
                 else
                   property->referencetype=GGF_SECTION_LOCATOR;

                 property->referencetag=(XDWORD)(void*)node->GetTarget();

    f3d->GetProperties()->Add(property);

//  f3d->Output(0,__L("saving animation node %s"),node->GetName()->Get());

  return block;
}


/*-------------------------------------------------------------------
//  GGFANIMATION_NODE_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/05/2017 14:59:52
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFANIMATION_NODE_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  GRPANIMATIONLAYER* layer=static_cast<GRPANIMATIONLAYER*>(parent);
  GRPANIMATIONNODE* node=new GRPANIMATIONNODE();

  if (!node)
  {
    f3d->Output(4,__L("Not enought memory!"));
    return false;
  }

  ///----------------------------------------- HEADER

  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;
  XDWORD      target_length =0;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (type!=GGF_SECTION_ANIMATION_NODE)
  {
    f3d->Output(4,__L("GGF_SECTION_ANIMATION_NODE::ReadBlock  Unexpected Section! : %d"),type);
    return false;
  }
  memcpy(&size,         &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&node->tag,    &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  f3d->ReadString(node->GetName(),      buffer,position,&pos);
  f3d->ReadString(node->GetTargetName(),buffer,position,&pos);

  //------------------------------------

    XBYTE node_type     = 0; memcpy(&node_type,&(buffer[pos]),    sizeof(XBYTE)); pos+=sizeof(XBYTE); node->SetType((GRPANIMATIONCURVETYPE)node_type);
    XWORD nconnections  = 0; memcpy(&nconnections,&(buffer[pos]), sizeof(XWORD)); pos+=sizeof(XWORD);

    layer->AddAnimationNode(node);
    f3d->animationnodes.Add(node);

    node->GetCurveConnections()->SetAddInLimit(nconnections);
    for (XDWORD e=0;e<nconnections;e++)
    {
    XDWORD key              =0;
    GRPANIMATIONCURVE* curve=0;

    memcpy(&key,    &(buffer[pos]), sizeof(XDWORD));  pos+=sizeof(XDWORD);
    XWORD curveID=0;
    memcpy(&curveID,&(buffer[pos]), sizeof(XWORD)); pos+=sizeof(XWORD);

    curve=layer->GetCurves()->Get(curveID);
    if (curve)
      node->AddConnection((GRPANIMATIONCURVEAXIS)key,curve);
    }




  //------------------------------------
  *position=*position+size;

  return true;
}

/*-------------------------------------------------------------------
//  GGFANIMATION_CURVE_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/05/2017 17:45:19
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        object :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFANIMATION_CURVE_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* object)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK*  block=new GGFSECTIONBLOCK();
                    block->SetType(GGF_SECTION_ANIMATION_CURVE);

  GRPANIMATIONCURVE* curve=static_cast<GRPANIMATIONCURVE*>(object);

  int calc_size=1*sizeof(XBYTE)+                                      //TYPE ELEMENT
                1*sizeof(XDWORD)+                                     //size
                1*sizeof(XDWORD)+                                     //tag
                /*
                1*sizeof(XBYTE)+                                      //name string size
                curve->GetName()->GetSize()*sizeof(XCHAR)+            //name
                */
                (curve->GetName()->GetSize()+1)*sizeof(XWORD)+        //name
                1*sizeof(XBYTE)+ //Active
                1*sizeof(XWORD)+ //n keytime
                curve->keytime.GetSize()*sizeof(XQWORDSIG)+           //keytimes
                1*sizeof(XWORD)+ //n keyvalues
                curve->keyvalues.GetSize()*sizeof(double)+            //keyvalues
                1*sizeof(XWORD)+ //n keyattributes
                curve->keyattributes.GetSize()*sizeof(XQWORD)+        //keyattributes
                1*sizeof(XWORD)+ //n attributes data float
                curve->keyattrdatafloat.GetSize()*sizeof(XQWORDSIG)+  //keyattrdatafloat
                1*sizeof(XWORD)+ //n attributes reference count data float
                curve->keyattrrefcount.GetSize()*sizeof(XDWORD);      //keyattrrefcount




  block->Set(calc_size);

  XQWORD pos=0;
  if (block->Get())
  {
    XBYTE     type                    = GGF_SECTION_ANIMATION_CURVE;                memcpy(&(block->Get()[pos]),&type,                                       sizeof(XBYTE));      pos+=sizeof(XBYTE);
                                                                                    memcpy(&(block->Get()[pos]),&calc_size,                                  sizeof(XDWORD));     pos+=sizeof(XDWORD);
    XDWORD    tag                     = (XDWORD)(void*)curve;                       memcpy(&(block->Get()[pos]),&tag,                                        sizeof(XDWORD));     pos+=sizeof(XDWORD);
    /*
    XBYTE     namelength              = curve->GetName()->GetSize();                memcpy(&(block->Get()[pos]),&namelength,                                 sizeof(XBYTE));      pos+=sizeof(XBYTE);
                                                                  if (namelength)   memcpy(&(block->Get()[pos]),curve->GetName()->Get(),          namelength*sizeof(XCHAR));      pos+=namelength*sizeof(XCHAR);
    */
    f3d->WriteString(curve->GetName(),block,&pos);

    XBYTE     active                  = curve->Active;                              memcpy(&(block->Get()[pos]),&active,                                     sizeof(XBYTE));      pos+=sizeof(XBYTE);
    XWORD     ntime                   = curve->keytime.GetSize();                   memcpy(&(block->Get()[pos]),&ntime,                                      sizeof(XWORD));      pos+=sizeof(XWORD);
    for (XDWORD e=0;e<curve->keytime.GetSize();e++)
    {
      XQWORDSIG keytime=curve->keytime.Get(e);
      memcpy(&(block->Get()[pos]),&keytime, sizeof(XQWORDSIG));
      pos+=sizeof(XQWORDSIG);
    }

    XWORD     nvalues                   = curve->keyvalues.GetSize();               memcpy(&(block->Get()[pos]),&nvalues,                                      sizeof(XWORD));      pos+=sizeof(XWORD);
    for (XDWORD e=0;e<curve->keyvalues.GetSize();e++)
    {
      double keyvalue=curve->keyvalues.Get(e);
      memcpy(&(block->Get()[pos]),&keyvalue, sizeof(double));
      pos+=sizeof(double);
    }

    XWORD     nattributes             = curve->keyattributes.GetSize();               memcpy(&(block->Get()[pos]),&nattributes,                                   sizeof(XWORD));     pos+=sizeof(XWORD);
    for (XDWORD e=0;e<curve->keyattributes.GetSize();e++)
    {
      XQWORD keyvalue=curve->keyattributes.Get(e);
      memcpy(&(block->Get()[pos]),&keyvalue, sizeof(XQWORD));
      pos+=sizeof(XQWORD);
    }

    XWORD     nattributesdatafloat    = curve->keyattrdatafloat.GetSize();            memcpy(&(block->Get()[pos]),&nattributesdatafloat,                      sizeof(XWORD));     pos+=sizeof(XWORD);
    for (XDWORD e=0;e<curve->keyattrdatafloat.GetSize();e++)
    {
      XQWORDSIG keyvalue=curve->keyattrdatafloat.Get(e);
      memcpy(&(block->Get()[pos]),&keyvalue, sizeof(XQWORDSIG));
      pos+=sizeof(XQWORDSIG);
    }

    XWORD     nattrrefcount   = curve->keyattrrefcount.GetSize();                     memcpy(&(block->Get()[pos]),&nattrrefcount,                     sizeof(XWORD));     pos+=sizeof(XWORD);
    for (XDWORD e=0;e<curve->keyattrrefcount.GetSize();e++)
    {
      XDWORD keyvalue=curve->keyattrrefcount.Get(e);
      memcpy(&(block->Get()[pos]),&keyvalue, sizeof(XDWORD));
      pos+=sizeof(XDWORD);
    }
  }

  return block;
}

/*-------------------------------------------------------------------
//  GGFANIMATION_CURVE_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/05/2017 17:58:52
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFANIMATION_CURVE_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  GRPANIMATIONLAYER* layer=static_cast<GRPANIMATIONLAYER*>(parent);
  GRPANIMATIONCURVE* curve=new GRPANIMATIONCURVE();

  if (!curve)
  {
    f3d->Output(4,__L("Not enought memory!"));
    return false;
  }

  ///----------------------------------------- HEADER

  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;
  XDWORD      target_length =0;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (type!=GGF_SECTION_ANIMATION_CURVE)
  {
    f3d->Output(4,__L("GGF_SECTION_ANIMATION_CURVE::ReadBlock  Unexpected Section! : %d"),type);
    return false;
  }
  memcpy(&size,         &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);


  memcpy(&curve->tag,   &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  /*
  memcpy(&name_length,  &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (name_length)
  {
    XCHAR* name=new XCHAR[name_length+1];
    memcpy(name,          &(buffer[pos]), sizeof(XCHAR)*name_length);   pos+=sizeof(XCHAR)*name_length;
    name[name_length]=__C('\0');
    curve->GetName()->Set(name);
    delete(name);
  }
  */
  f3d->ReadString(curve->GetName(),buffer,position,&pos);


  XBYTE active=false; memcpy(&active, &(buffer[pos]),sizeof(XBYTE)); pos+=sizeof(XBYTE); curve->Active=(active!=0);

  XWORD ntime=0;      memcpy(&ntime, &(buffer[pos]),sizeof(XWORD)); pos+=sizeof(XWORD);
  curve->keytime.SetAddInLimit(ntime);
  for (XDWORD e=0;e<ntime;e++)
  {
    XQWORDSIG keytime=0;
    memcpy(&keytime, &(buffer[pos]),sizeof(XQWORDSIG)); pos+=sizeof(XQWORDSIG);
    curve->keytime.Add(keytime);
  }

  XWORD nvalues=0;      memcpy(&nvalues, &(buffer[pos]),sizeof(XWORD)); pos+=sizeof(XWORD);
  curve->keyvalues.SetAddInLimit(nvalues);
  for (XDWORD e=0;e<nvalues;e++)
  {
    double keyvalue=0;
    memcpy(&keyvalue, &(buffer[pos]),sizeof(double)); pos+=sizeof(double);
    curve->keyvalues.Add(keyvalue);
  }

  XWORD nattributes=0;  memcpy(&nattributes, &(buffer[pos]),sizeof(XWORD)); pos+=sizeof(XWORD);
  curve->keyattributes.SetAddInLimit(nattributes);
  for (XWORD e=0;e<nattributes;e++)
  {
    XQWORD keyvalue=0;
    memcpy(&keyvalue,&(buffer[pos]),sizeof(XQWORD)); pos+=sizeof(XQWORD);
    curve->keyattributes.Add(keyvalue);
  }

  XWORD nattributesdatafloat=0; memcpy(&nattributesdatafloat, &(buffer[pos]),sizeof(XWORD)); pos+=sizeof(XWORD);
  curve->keyattrdatafloat.SetAddInLimit(nattributesdatafloat);
  for (XWORD e=0;e<nattributesdatafloat;e++)
  {
    XQWORDSIG keyvalue=0;
    memcpy(&keyvalue,&(buffer[pos]),sizeof(XQWORDSIG)); pos+=sizeof(XQWORDSIG);
    curve->keyattrdatafloat.Add(keyvalue);
  }

  XWORD nattrrefcount=0;        memcpy(&nattrrefcount, &(buffer[pos]),sizeof(XWORD)); pos+=sizeof(XWORD);
  curve->keyattrrefcount.SetAddInLimit(nattrrefcount);
  for (XWORD e=0;e<nattrrefcount;e++)
  {
    XDWORD keyvalue=0;
    memcpy(&keyvalue,&(buffer[pos]),sizeof(XDWORD)); pos+=sizeof(XDWORD);
    curve->keyattrrefcount.Add(keyvalue);
  }

  f3d->animationcurves.Add(curve);

  curve->Init();

  layer->AddCurve(curve);

  *position=*position+size;
  return true;
}


/*-------------------------------------------------------------------
//  GGFLOGIC_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/05/2017 17:12:01
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        pnode :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFLOGIC_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  AI::LOGIC* logic=static_cast<AI::LOGIC*>(pnode);

  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
  block->SetType(GGF_SECTION_LOGIC);

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XDWORD)+ //size
                1*sizeof(XDWORD)+ //tag
                /*
                1*sizeof(XBYTE)+  //name string size
                logic->GetName()->GetSize()*sizeof(XCHAR)+ //name
                */
                (logic->GetName()->GetSize()+1)*sizeof(XWORD)+ //name
                1*sizeof(XBYTE); //n controllers

  block->Set(calc_size);

  XQWORD pos=0;

    ///----------------------------------------- HEADER
    if (block->Get())
    {
      XBYTE type            = GGF_SECTION_LOGIC;                                    memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));       pos+=sizeof(XBYTE);
      XDWORD size           = calc_size;                                            memcpy(&(block->Get()[pos]),&size,                                sizeof(XDWORD));      pos+=sizeof(XDWORD);
      XDWORD tag            = (XDWORD)(void*)logic;                                 memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));      pos+=sizeof(XDWORD);
      /*
      XBYTE length          = logic->GetName()->GetSize();                          memcpy(&(block->Get()[pos]),&length,                              sizeof(XBYTE));       pos+=sizeof(XBYTE);
                              if (length)                                           memcpy(&(block->Get()[pos]),logic->GetName()->Get(),       length*sizeof(XCHAR));     pos+= length*sizeof(XCHAR);
      */
      f3d->WriteString(logic->GetName(),block,&pos);

      XBYTE nControllers    = logic->GetControllers()->GetSize();                   memcpy(&(block->Get()[pos]),&nControllers,                        sizeof(XBYTE));       pos+=sizeof(XBYTE);

      for (XDWORD e=0;e<nControllers;e++)
      {
        AI::CONTROLLER* controller=logic->GetControllers()->Get(e);
        block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_LOGIC_CONTROLLER)->CreateBlock(f3d,controller));
      }
    }



return block;
}

/*-------------------------------------------------------------------
//  GGFLOGIC_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      22/05/2017 18:02:16
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFLOGIC_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{

  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      tag         =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;

  GRPOBJECT* object=static_cast<GRPOBJECT*>(parent);
  if (!object) return false;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_LOGIC)
  {
    f3d->Output(4,__L("GGF_SECTION_LOGIC Unexpected Section! : %d"),type);
    return false;
  }

  memcpy(&size,             &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&tag,              &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);


  /*
  memcpy(&name_length,      &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (name_length)
  {
    XCHAR* name=new XCHAR[name_length+1];
    memcpy(name,            &(buffer[pos]), sizeof(XCHAR)*name_length);   pos+=sizeof(XCHAR)*name_length;
    name[name_length]=__C('\0');
    s.Set(name);
    delete(name);
  }
  */
  if (object->GetLogic()!=NULL)
  {
    delete(object->GetLogic());
    object->SetLogic(NULL);
  }

  AI::LOGIC* logic=new AI::LOGIC();
  object->SetLogic(logic);

  f3d->ReadString(logic->GetName(),buffer,position,&pos);

  XBYTE ncontrollers=0;
  memcpy(&ncontrollers,     &(buffer[pos]), sizeof(XBYTE));             pos+=sizeof(XBYTE);

  *position=(*position)+size;

    for (XDWORD e=0;e<ncontrollers;e++)
      {
        if (!f3d->GetBuilders()->Get(GGF_SECTION_LOGIC_CONTROLLER)->ReadBlock(f3d,buffer,position,(void*)logic))    return false;
      }

return false;
}

/*-------------------------------------------------------------------
//  GGFLOGIC_CONTROLLER_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/05/2017 14:12:22
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        GRPFILE3DGGF* :
//  @param        v :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFLOGIC_CONTROLLER_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  AI::CONTROLLER* controller=static_cast<AI::CONTROLLER*>(pnode);

  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
  block->SetType(GGF_SECTION_LOGIC_CONTROLLER);

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XDWORD)+ //size
                1*sizeof(XDWORD)+ //tag
                /*
                1*sizeof(XBYTE)+  //name string size
                controller->GetName()->GetSize()*sizeof(XCHAR)+ //name
                */
                (controller->GetName()->GetSize()+1)*sizeof(XWORD)+ //name
                1*sizeof(XDWORD)+ //defaultstate
                1*sizeof(XDWORD)+ //nstates
                1*sizeof(XDWORD)+ //noverlays
                1*sizeof(XDWORD)+ //ncommands
                controller->GetCommands()->GetSize()*sizeof(XDWORD)*4; //commands

                for (XDWORD e=0;e<controller->GetCommands()->GetSize();e++)
                {
                  calc_size+=controller->GetCommands()->GetElement(e)->properties.GetSize()*sizeof(XDWORD)*2;
                }

  block->Set(calc_size);


  XQWORD pos=0;

    ///----------------------------------------- HEADER
    if (block->Get())
    {
      XBYTE   type            = GGF_SECTION_LOGIC_CONTROLLER;               memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));       pos+=sizeof(XBYTE);
      XDWORD  size            = calc_size;                                  memcpy(&(block->Get()[pos]),&size,                                sizeof(XDWORD));      pos+=sizeof(XDWORD);
      XDWORD  tag             = (XDWORD)(void*)controller;                  memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));      pos+=sizeof(XDWORD);
      /*
      XBYTE   length          = controller->GetName()->GetSize();           memcpy(&(block->Get()[pos]),&length,                              sizeof(XBYTE));       pos+=sizeof(XBYTE);

                                if (length)                                 memcpy(&(block->Get()[pos]),controller->GetName()->Get(),  length*sizeof(XCHAR));       pos+= length*sizeof(XCHAR);
      */
      f3d->WriteString(controller->GetName(),block,&pos);

      XDWORD    defaultstate    = controller->GetDefaultState();            memcpy(&(block->Get()[pos]),&defaultstate,                        sizeof(XDWORD));      pos+=sizeof(XDWORD);
      XDWORD    nstates         = controller->GetStates()->GetSize();       memcpy(&(block->Get()[pos]),&nstates,                             sizeof(XDWORD));      pos+=sizeof(XDWORD);
      XDWORD    noverlays       = controller->GetOverlays()->GetSize();     memcpy(&(block->Get()[pos]),&noverlays,                           sizeof(XDWORD));      pos+=sizeof(XDWORD);
      XDWORD    ncommands       = controller->GetCommands()->GetSize();     memcpy(&(block->Get()[pos]),&ncommands,                           sizeof(XDWORD));      pos+=sizeof(XDWORD);

      for (XDWORD e=0;e<ncommands;e++)
      {
        AI::COMMAND* command=controller->GetCommands()->GetElement(e);

        XDWORD id   =command->id;                     memcpy(&(block->Get()[pos]),&id,    sizeof(XDWORD));      pos+=sizeof(XDWORD);
        XDWORD type =command->type;                   memcpy(&(block->Get()[pos]),&type,  sizeof(XDWORD));      pos+=sizeof(XDWORD);
        XDWORD value=command->value;                  memcpy(&(block->Get()[pos]),&value, sizeof(XDWORD));      pos+=sizeof(XDWORD);

        XDWORD np   =command->properties.GetSize();   memcpy(&(block->Get()[pos]),&np,    sizeof(XDWORD));      pos+=sizeof(XDWORD);

        for (XDWORD p=0;p<np;p++)
        {
          XDWORD key  =command->properties.GetKey(p);     memcpy(&(block->Get()[pos]),&key,   sizeof(XDWORD));  pos+=sizeof(XDWORD);
          XDWORD value=command->properties.GetElement(p); memcpy(&(block->Get()[pos]),&value, sizeof(XDWORD));  pos+=sizeof(XDWORD);
        }
      }

      for (XDWORD e=0;e<nstates;e++)
      {
        AI::STATE* state=controller->GetStates()->GetElement(e);
        block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_LOGIC_STATE)->CreateBlock(f3d,state));
      }

      for (XDWORD e=0;e<noverlays;e++)
      {
        AI::OVERLAY* overlay=controller->GetOverlays()->GetElement(e);
        block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_LOGIC_OVERLAY)->CreateBlock(f3d,overlay));
      }

    }

return block;
}

/*-------------------------------------------------------------------
//  GGFLOGIC_CONTROLLER_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/05/2017 14:26:26
//
//  @return       bool :
//
//  @param        GRPFILE3DGGF* :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFLOGIC_CONTROLLER_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void* parent)
{
  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      tag         =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;

  AI::LOGIC* logic=static_cast<AI::LOGIC*>(parent);
  if (!logic) return false;

  AI::CONTROLLER* controller=new AI::CONTROLLER();
  if (!controller) return false;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_LOGIC_CONTROLLER)
  {
    f3d->Output(4,__L("GGF_SECTION_LOGIC_CONTROLLER Unexpected Section! : %d"),type);
    return false;
  }

  memcpy(&size,             &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&tag,              &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  /*
  memcpy(&name_length,      &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  XSTRING s;

  if (name_length)
  {
    XCHAR* name=new XCHAR[name_length+1];
    memcpy(name,            &(buffer[pos]), sizeof(XCHAR)*name_length);   pos+=sizeof(XCHAR)*name_length;
    name[name_length]=__C('\0');
    s.Set(name);
    delete(name);
  }
  */
  f3d->ReadString(controller->GetName(),buffer,position,&pos);

  XDWORD defaultstate=0;  memcpy(&defaultstate, &(buffer[pos]), sizeof(XDWORD));  pos+=sizeof(XDWORD);
  XDWORD nstates=0;       memcpy(&nstates,      &(buffer[pos]), sizeof(XDWORD));  pos+=sizeof(XDWORD);
  XDWORD noverlays=0;     memcpy(&noverlays,    &(buffer[pos]), sizeof(XDWORD));  pos+=sizeof(XDWORD);
  XDWORD ncommands=0;     memcpy(&ncommands,    &(buffer[pos]), sizeof(XDWORD));  pos+=sizeof(XDWORD);

  for (XDWORD e=0;e<ncommands;e++)
      {
        XDWORD id=0;        memcpy(&id,     &(buffer[pos]), sizeof(XDWORD));  pos+=sizeof(XDWORD);
        XDWORD type=0;      memcpy(&type,   &(buffer[pos]), sizeof(XDWORD));  pos+=sizeof(XDWORD);
        XDWORD value=0;     memcpy(&value,  &(buffer[pos]), sizeof(XDWORD));  pos+=sizeof(XDWORD);

        AI::COMMAND* command=new AI::COMMAND();
                     command->id=id;
                     command->type=type;
                     command->value=value;

        XDWORD np=0;        memcpy(&np,     &(buffer[pos]), sizeof(XDWORD));  pos+=sizeof(XDWORD);
        for (XDWORD p=0;p<np;p++)
        {
          XDWORD key=0;     memcpy(&key,    &(buffer[pos]), sizeof(XDWORD));  pos+=sizeof(XDWORD);
          XDWORD value=0;   memcpy(&value,  &(buffer[pos]), sizeof(XDWORD));  pos+=sizeof(XDWORD);
          command->properties.Add(key,value);
        }

        controller->GetCommands()->Add(command->id,command);
      }

  *position=(*position)+size;

      for (XDWORD e=0;e<nstates;e++)
        if (!f3d->GetBuilders()->Get(GGF_SECTION_LOGIC_STATE)->ReadBlock(f3d,buffer,position,(void*)controller))      return false;


      for (XDWORD e=0;e<noverlays;e++)
        if (!f3d->GetBuilders()->Get(GGF_SECTION_LOGIC_OVERLAY)->ReadBlock(f3d,buffer,position,(void*)controller))    return false;



  logic->Add(controller);
  controller->SetOwner(f3d->object);
  controller->SetDefaultState(defaultstate);

  return false;
}

/*-------------------------------------------------------------------
//  GGFLOGIC_STATE_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/05/2017 12:51:35
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        pnode :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFLOGIC_STATE_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  AI::STATE* state=static_cast<AI::STATE*>(pnode);

  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
  block->SetType(GGF_SECTION_LOGIC_STATE);

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XDWORD)+ //size
                1*sizeof(XDWORD)+ //tag
                (state->GetName()->GetSize()+1)*sizeof(XWORD)+ //name
                1*sizeof(XDWORD)+ //id
                1*sizeof(XDWORD)+ //nAnimations
                1*sizeof(XDWORD)+ //nTransitions
                state->GetTransitions()->GetSize()*sizeof(XDWORD); //transitions

  block->Set(calc_size);

  XQWORD pos=0;

    ///----------------------------------------- HEADER
    if (block->Get())
    {
      XBYTE type            = GGF_SECTION_LOGIC_STATE;                              memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));   pos+=sizeof(XBYTE);
      XDWORD size           = calc_size;                                            memcpy(&(block->Get()[pos]),&size,                                sizeof(XDWORD));  pos+=sizeof(XDWORD);
      XDWORD tag            = (XDWORD)(void*)state;                                 memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));  pos+=sizeof(XDWORD);

      f3d->WriteString(state->GetName(),block,&pos);

      XDWORD id             = state->GetIndex();                                    memcpy(&(block->Get()[pos]),&id,                                  sizeof(XDWORD));  pos+=sizeof(XDWORD);
      XDWORD nAnimations    = state->GetTracks()->Get(0)->animation.GetSize();      memcpy(&(block->Get()[pos]),&nAnimations,                         sizeof(XDWORD));  pos+=sizeof(XDWORD);
      XDWORD nTransitions   = state->GetTransitions()->GetSize();                   memcpy(&(block->Get()[pos]),&nTransitions,                        sizeof(XDWORD));  pos+=sizeof(XDWORD);

      //-----------
      for (XDWORD e=0;e<state->GetTransitions()->GetSize();e++)
      {
        XDWORD transition=state->GetTransitions()->Get(e);
        memcpy(&(block->Get()[pos]),&transition,sizeof(XDWORD));  pos+=sizeof(XDWORD);
      }

      //-----------
      for (XDWORD e=0;e<state->GetTracks()->Get(0)->animation.GetSize();e++)
      {
        AI::ANIMATION* ani=state->GetTracks()->Get(0)->animation.Get(e);
        block->Blocks.Add(f3d->GetBuilders()->Get(GGF_SECTION_LOGIC_ANIMATION)->CreateBlock(f3d,ani));
      }

    }

return block;
}

/*-------------------------------------------------------------------
//  GGFLOGIC_STATE_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/05/2017 12:50:26
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFLOGIC_STATE_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      tag         =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;



  AI::CONTROLLER* controller=static_cast<AI::CONTROLLER*>(parent);

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_LOGIC_STATE)
  {
    f3d->Output(4,__L("GGF_SECTION_LOGIC_STATE Unexpected Section! : %d"),type);
    return false;
  }

  memcpy(&size,             &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&tag,              &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  XSTRING name;
  f3d->ReadString(&name,buffer,position,&pos);

  XDWORD id=0;
  memcpy(&id,                       &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  XDWORD nAnimations=0;
  memcpy(&nAnimations,              &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  XDWORD nTransitions=0;
  memcpy(&nTransitions,             &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  AI::STATE* state=new AI::STATE(id,name.Get());
  if (!state) return false;

      for (XDWORD e=0;e<nTransitions;e++)
      {
        XDWORD transition=0;
        memcpy(&transition, &(buffer[pos]), sizeof(XDWORD));
        pos+=sizeof(XDWORD);

        state->GetTransitions()->Add(transition);
      }

  *position=(*position)+size;

    controller->Add(id,state);
    for (XDWORD e=0;e<nAnimations;e++)
      {
        if (!f3d->GetBuilders()->Get(GGF_SECTION_LOGIC_ANIMATION)->ReadBlock(f3d,buffer,position,(void*)state))   return false;
      }


    return true;
}

/*-------------------------------------------------------------------
//  GGFLOGIC_ANIMATION_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/05/2017 13:18:49
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        pnode :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFLOGIC_ANIMATION_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  AI::ANIMATION* ani=static_cast<AI::ANIMATION*>(pnode);

  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
  block->SetType(GGF_SECTION_LOGIC_ANIMATION);

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XDWORD)+ //size
                1*sizeof(XDWORD)+ //tag
                (ani->name.GetSize()+1)*sizeof(XCHAR)+
                1*sizeof(XDWORD)+ //id
                1*sizeof(XDWORD)+ //loops
                1*sizeof(XDWORD)+ //transition
                1*sizeof(XDWORD)+ //next_start
                1*sizeof(XDWORD)+ //onStartEvent
                1*sizeof(XDWORD)+ //onEndEvent
                1*sizeof(XDWORD); //acceptor

  block->Set(calc_size);

  XQWORD pos=0;

    ///----------------------------------------- HEADER
    if (block->Get())
    {
      XBYTE type            = GGF_SECTION_LOGIC_ANIMATION;                          memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));   pos+=sizeof(XBYTE);
      XDWORD size           = calc_size;                                            memcpy(&(block->Get()[pos]),&size,                                sizeof(XDWORD));  pos+=sizeof(XDWORD);
      XDWORD tag            = (XDWORD)(void*)ani;                                   memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));  pos+=sizeof(XDWORD);

      f3d->WriteString(&ani->name,block,&pos);

      XDWORD id             = ani->id;                                              memcpy(&(block->Get()[pos]),&id,                                  sizeof(XDWORD));  pos+=sizeof(XDWORD);
      XDWORD loops          = ani->loops;                                           memcpy(&(block->Get()[pos]),&loops,                               sizeof(XDWORD));  pos+=sizeof(XDWORD);
      int    transition     = ani->transition;                                      memcpy(&(block->Get()[pos]),&transition,                          sizeof(int));     pos+=sizeof(int);

      XDWORD next_start     = ani->next_start;                                      memcpy(&(block->Get()[pos]),&next_start,                          sizeof(XDWORD));  pos+=sizeof(XDWORD);
      XDWORD onStartEvent   = ani->onStartEvent;                                    memcpy(&(block->Get()[pos]),&onStartEvent,                        sizeof(XDWORD));  pos+=sizeof(XDWORD);
      XDWORD onEndEvent     = ani->onEndEvent;                                      memcpy(&(block->Get()[pos]),&onEndEvent,                          sizeof(XDWORD));  pos+=sizeof(XDWORD);
      XDWORD acceptor       = ani->acceptor;                                        memcpy(&(block->Get()[pos]),&acceptor,                            sizeof(XDWORD));  pos+=sizeof(XDWORD);


      for (XDWORD e=0;e<ani->Attachees.GetSize();e++)
      {
        GGFPROPERTY*  property=new GGFPROPERTY();
                      property->PropertyType=GGF_SECTION_LOGIC_ANIMATION;

                      property->ownertag=(XDWORD)ani->id;
                      property->ownertype=GGF_SECTION_LOGIC_ANIMATION;

                      property->referencetag=(XDWORD)(ani->Attachees.Get(e)->id);
                      property->referencetype=GGF_SECTION_LOGIC_ANIMATION;

        f3d->GetProperties()->Add(property);
      }
    }

return block;
}

/*-------------------------------------------------------------------
//  GGFLOGIC_ANIMATION_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/05/2017 13:18:55
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFLOGIC_ANIMATION_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      tag         =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;


  AI::STATE* state=static_cast<AI::STATE*>(parent);

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_LOGIC_ANIMATION)
  {
    f3d->Output(4,__L("GGF_SECTION_LOGIC_ANIMATION Unexpected Section! : %d"),type);
    return false;
  }



  memcpy(&size,             &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&tag,              &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  XSTRING name;
  f3d->ReadString(&name,buffer,position,&pos);



  XDWORD id=0;            memcpy(&id,           &(buffer[pos]), sizeof(XDWORD));    pos+=sizeof(XDWORD);
  XDWORD loops=0;         memcpy(&loops,        &(buffer[pos]), sizeof(XDWORD));    pos+=sizeof(XDWORD);
  int    transition=0;    memcpy(&transition,   &(buffer[pos]), sizeof(int));       pos+=sizeof(int);
  XDWORD next_start=0;    memcpy(&next_start,   &(buffer[pos]), sizeof(XDWORD));    pos+=sizeof(XDWORD);
  XDWORD onStartEvent=0;  memcpy(&onStartEvent, &(buffer[pos]), sizeof(XDWORD));    pos+=sizeof(XDWORD);
  XDWORD onEndEvent=0;    memcpy(&onEndEvent,   &(buffer[pos]), sizeof(XDWORD));    pos+=sizeof(XDWORD);
  XDWORD acceptor=0;      memcpy(&acceptor,     &(buffer[pos]), sizeof(XDWORD));    pos+=sizeof(XDWORD);

  *position=(*position)+size;

  AI::ANIMATION*  animation= new AI::ANIMATION(id,loops,transition);
  if (!animation) return false;

                  animation->name.Set(name);
                  animation->loops        =loops;
                  animation->transition   =transition;
                  animation->next_start   =next_start;
                  animation->onStartEvent =onStartEvent;
                  animation->onEndEvent   =onEndEvent;
                  animation->acceptor     =(AI::TRANSITION::ACCEPTTYPE)acceptor;

  state->Add(animation);

  f3d->ai_animations.Add(animation);
  return true;
}

/*-------------------------------------------------------------------
//  GGFLOGIC_OVERLAY_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/05/2017 11:55:55
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        pnode :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFLOGIC_OVERLAY_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  AI::OVERLAY* ov=static_cast<AI::OVERLAY*>(pnode);

  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
  block->SetType(GGF_SECTION_LOGIC_ANIMATION);

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XDWORD)+ //size
                1*sizeof(XDWORD)+ //tag
                /*
                1*sizeof(XBYTE)+  //name string size
                ov->name.GetSize()*sizeof(XCHAR)+ //name
                */
                (ov->name.GetSize()+1)*sizeof(XWORD)+
                1*sizeof(XDWORD)+ //id
                1*sizeof(XBYTE)+  //status
                1*sizeof(XDWORD)+ //nOverlayables
                1*sizeof(XDWORD)+ //nTransitions
                1*sizeof(XDWORD)+ //nRejections
                1*sizeof(XDWORD)+ //nAttachees
                1*sizeof(XDWORD)*ov->GetOverlayables()->GetSize()+
                1*sizeof(XDWORD)*ov->GetTransitions()->GetSize()+
                1*sizeof(XDWORD)*ov->GetRejections()->GetSize()+
                2*sizeof(XDWORD)*ov->GetAttachees()->GetSize();

  block->Set(calc_size);

  XQWORD pos=0;

    ///----------------------------------------- HEADER
    if (block->Get())
    {
      XBYTE type            = GGF_SECTION_LOGIC_OVERLAY;                            memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));   pos+=sizeof(XBYTE);
      XDWORD size           = calc_size;                                            memcpy(&(block->Get()[pos]),&size,                                sizeof(XDWORD));  pos+=sizeof(XDWORD);
      XDWORD tag            = (XDWORD)(void*)ov;                                    memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));  pos+=sizeof(XDWORD);

      f3d->WriteString(&ov->name,block,&pos);

      XDWORD id             = ov->GetID();                                          memcpy(&(block->Get()[pos]),&id,                                  sizeof(XDWORD));  pos+=sizeof(XDWORD);
      XBYTE  status         = ov->GetInitialStatus();                               memcpy(&(block->Get()[pos]),&status,                              sizeof(XBYTE));   pos+=sizeof(XBYTE);
      XDWORD nOverlayables  = ov->GetOverlayables()->GetSize();                     memcpy(&(block->Get()[pos]),&nOverlayables,                       sizeof(XDWORD));  pos+=sizeof(XDWORD);
      XDWORD nTransitions   = ov->GetTransitions()->GetSize();                      memcpy(&(block->Get()[pos]),&nTransitions,                        sizeof(XDWORD));  pos+=sizeof(XDWORD);
      XDWORD nRejections    = ov->GetRejections()->GetSize();                       memcpy(&(block->Get()[pos]),&nRejections,                         sizeof(XDWORD));  pos+=sizeof(XDWORD);
      XDWORD nAttachees     = ov->GetAttachees()->GetSize();                        memcpy(&(block->Get()[pos]),&nAttachees,                          sizeof(XDWORD));  pos+=sizeof(XDWORD);

      for (XDWORD e=0;e<nOverlayables;e++)
      {
        XDWORD id=  ov->GetOverlayables()->Get(e);
        memcpy(&(block->Get()[pos]),&id,sizeof(XDWORD));
        pos+=sizeof(XDWORD);
      }
      for (XDWORD e=0;e<nTransitions;e++)
      {
        XDWORD id=  ov->GetTransitions()->Get(e);
        memcpy(&(block->Get()[pos]),&id,sizeof(XDWORD));
        pos+=sizeof(XDWORD);
      }
      for (XDWORD e=0;e<nRejections;e++)
      {
        XDWORD id=  ov->GetRejections()->Get(e);
        memcpy(&(block->Get()[pos]),&id,sizeof(XDWORD));
        pos+=sizeof(XDWORD);
      }

      for (XDWORD e=0;e<nAttachees;e++)
      {
        XDWORD id   = ov->GetAttachees()->GetKey(e);
        memcpy(&(block->Get()[pos]),&id,sizeof(XDWORD));
        pos+=sizeof(XDWORD);
        XDWORD value= ov->GetAttachees()->GetElement(e);
        memcpy(&(block->Get()[pos]),&value,sizeof(XDWORD));
        pos+=sizeof(XDWORD);
      }
    }

return block;
}


/*-------------------------------------------------------------------
//  GGFLOGIC_OVERLAY_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/05/2017 11:56:46
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFLOGIC_OVERLAY_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void* parent)
{
  XQWORD      pos         =*position;
  XBYTE       type        =0;
  XDWORD      tag         =0;
  XDWORD      size        =0;
  XDWORD      name_length =0;


  AI::CONTROLLER* controller  =static_cast<AI::CONTROLLER*>(parent);

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_LOGIC_OVERLAY)
  {
    f3d->Output(4,__L("GGF_SECTION_LOGIC_OVERLAY Unexpected Section! : %d"),type);
    return false;
  }

  AI::OVERLAY* overlay=new AI::OVERLAY();

  memcpy(&size,             &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&tag,              &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  f3d->ReadString(overlay->GetName(),buffer,position,&pos);

  overlay->SetTag(tag);

  XDWORD id=0;
  memcpy(&id,               &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);    overlay->SetID(id);
  XBYTE status=0;
  memcpy(&status,           &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);     overlay->SetStatus(status);
  XDWORD nOverlayables=0;
  memcpy(&nOverlayables,    &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  XDWORD nTransitions=0;
  memcpy(&nTransitions,     &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  XDWORD nRejections=0;
  memcpy(&nRejections,      &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  XDWORD nAttachees=0;
  memcpy(&nAttachees,       &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);



//--
      for (XDWORD e=0;e<nOverlayables;e++)
      {
        XDWORD id=0;
        memcpy(&id,               &(buffer[pos]), sizeof(XDWORD));
        pos+=sizeof(XDWORD);
        overlay->GetOverlayables()->Add(id);
      }
//--
      for (XDWORD e=0;e<nTransitions;e++)
      {
        XDWORD id=0;
        memcpy(&id,               &(buffer[pos]), sizeof(XDWORD));
        pos+=sizeof(XDWORD);
        overlay->GetTransitions()->Add(id);
      }
//--
      for (XDWORD e=0;e<nRejections;e++)
      {
        XDWORD id=0;
        memcpy(&id,               &(buffer[pos]), sizeof(XDWORD));
        pos+=sizeof(XDWORD);
        overlay->GetRejections()->Add(id);
      }
//--
      for (XDWORD e=0;e<nAttachees;e++)
      {
        XDWORD id=0;
        memcpy(&id,               &(buffer[pos]), sizeof(XDWORD));
        pos+=sizeof(XDWORD);
        XDWORD value=0;
        memcpy(&value,            &(buffer[pos]), sizeof(XDWORD));
        pos+=sizeof(XDWORD);
        overlay->GetAttachees()->Add(id,value);
      }
//---
      controller->Add(overlay->GetID(),overlay);

  *position=(*position)+size;
  return true;
}


/*-------------------------------------------------------------------
//  GGFDEFORMER_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/05/2017 9:54:59
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        GRPFILE3DGGF* :
//  @param        v :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFDEFORMER_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
  block->SetType(GGF_SECTION_DEFORMER);

  GRPDEFORMER* morpher=static_cast<GRPDEFORMER*>(pnode);

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XBYTE)+ //size
                1*sizeof(XDWORD)+ //tag
                /*
                1*sizeof(XBYTE)+  //name string size
                morpher->GetName()->GetSize()*sizeof(XCHAR)+ //name
                */
                (morpher->GetName()->GetSize()+1)*sizeof(XWORD)+
                1*sizeof(XBYTE); //number of subdeformers, max 12


  block->Set(calc_size);

  XQWORD pos=0;

  ///----------------------------------------- HEADER
  if (block->Get())
  {
    XBYTE type            = GGF_SECTION_DEFORMER;                               memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XBYTE size            = calc_size;                                          memcpy(&(block->Get()[pos]),&size,                                sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XDWORD tag            = (XDWORD)(void*)morpher;                             memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));      pos+=sizeof(XDWORD);

    f3d->WriteString(morpher->GetName(),block,&pos);

    XBYTE nsubdeformers   = morpher->GetSubdeformers()->GetSize();              memcpy(&(block->Get()[pos]),&nsubdeformers,                       sizeof(XBYTE));       pos+=sizeof(XBYTE);

    for (XDWORD e=0;e<nsubdeformers;e++)
    {
    GGFSECTIONBUILDER*  builder=f3d->GetBuilders()->Get(GGF_SECTION_SUBDEFORMER);
    GGFSECTIONBLOCK*    deformer_block  =builder->CreateBlock(f3d,morpher->GetSubdeformers()->GetElement(e));
                        block->Blocks.Add(deformer_block);
    }
  }

  return block;
}

/*-------------------------------------------------------------------
//  GGFDEFORMER_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/05/2017 9:55:07
//
//  @return       bool :
//
//  @param        GRPFILE3DGGF* :
//  @param        buffer :
//  @param        position :
*/
/*-----------------------------------------------------------------*/
bool GGFDEFORMER_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void* parent)
{
  GRPDEFORMER*    morpher     = new GRPDEFORMER();
  XQWORD          pos         =*position;
  XBYTE           type        =0;
  XDWORD          size        =0;
  XDWORD          name_length =0;

  GRPELEMENT*     element     = static_cast<GRPELEMENT*>(parent);


  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_DEFORMER)
  {
    f3d->Output(4,__L("GGFDEFORMER_BUILDER Unexpected Section! : %d"),type);
    return false;
  }

  XBYTE nsubdeformers=0;

  memcpy(&size,             &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&morpher->tag,     &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  f3d->ReadString(morpher->GetName(),buffer,position,&pos);

  memcpy(&nsubdeformers,    &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  *position=(*position)+size;

  f3d->deformers.Add(morpher);

  for (XDWORD e=0;e<nsubdeformers;e++)
  {
    if (!f3d->GetBuilders()->Get(GGF_SECTION_SUBDEFORMER)->ReadBlock(f3d,buffer,position,(void*)morpher))   return false;
  }
  morpher->SetTarget(element);
  element->SetDeformer(morpher);


  return true;
}

/*-------------------------------------------------------------------
//  GGFSUBDEFORMER_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/05/2017 14:36:18
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        GRPFILE3DGGF* :
//  @param        pnode :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFSUBDEFORMER_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
  block->SetType(GGF_SECTION_SUBDEFORMER);

  GRPSUBDEFORMER* morpher=static_cast<GRPSUBDEFORMER*>(pnode);

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE ELEMENT
                1*sizeof(XBYTE)+ //size
                1*sizeof(XDWORD)+ //tag
                (morpher->GetName()->GetSize()+1)*sizeof(XWORD); //name


  block->Set(calc_size);

  XQWORD pos=0;

  ///----------------------------------------- HEADER
  if (block->Get())
  {
    XBYTE type            = GGF_SECTION_SUBDEFORMER;                            memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XBYTE size            = calc_size;                                          memcpy(&(block->Get()[pos]),&size,                                sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XDWORD tag            = (XDWORD)(void*)morpher;                             memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));      pos+=sizeof(XDWORD);

    f3d->WriteString(morpher->GetName(),block,&pos);
  }

return block;
}



  /*-------------------------------------------------------------------
  //  GGFSUBDEFORMER_BUILDER::ReadBlock
  */
  /**
  //
  //
  //
  //  @author       Diego Martinez Ruiz de Gaona
  //  @version      18/05/2017 14:36:28
  //
  //  @return       bool :
  //
  //  @param        f3d :
  //  @param        buffer :
  //  @param        position :
  //  @param        parent :
  */
  /*-----------------------------------------------------------------*/
bool GGFSUBDEFORMER_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void* parent)
{
  GRPSUBDEFORMER* subdef      = new GRPSUBDEFORMER();
  XQWORD          pos         =*position;
  XBYTE           type        =0;
  XDWORD          size        =0;
  XDWORD          name_length =0;

  GRPDEFORMER* deformer=static_cast<GRPDEFORMER*>(parent);


  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_SUBDEFORMER)
  {
    f3d->Output(4,__L("GGFSUBDEFORMER_BUILDER Unexpected Section! : %d"),type);
    return false;
  }

  XBYTE nsubdeformers=0;

  memcpy(&size,             &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&subdef->tag,      &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);

  f3d->ReadString(subdef->GetName(),buffer,position,&pos);

  deformer->Add(subdef);

  f3d->subdeformers.Add(subdef);

  *position=(*position)+size;

  return true;
}


/*-------------------------------------------------------------------
//  GGFSKIN_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/05/2017 12:21:35
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        pnode :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFSKIN_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void * pnode)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
                   block->SetType(GGF_SECTION_SKIN);

  GRPSKIN* skin=static_cast<GRPSKIN*>(pnode);

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE MATERIAL
                1*sizeof(XBYTE)+  //size
                1*sizeof(XDWORD)+ //tag

                (skin->GetName()->GetSize()+1)*sizeof(XWORD)+
                1*sizeof(XWORD)+  //number of SKinPose
                1*sizeof(XWORD);  //number of skincluster

  block->Set(calc_size);

  XQWORD pos=0;
  if (block->Get())
  {
    XBYTE type              = GGF_SECTION_SKIN;                                 memcpy(&(block->Get()[pos]),&type,                                  sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XBYTE size              = calc_size;                                        memcpy(&(block->Get()[pos]),&size,                                  sizeof(XBYTE));       pos+=sizeof(XBYTE);

    XDWORD tag              = (XDWORD)(void*)skin;                              memcpy(&(block->Get()[pos]),&tag,                                   sizeof(XDWORD));      pos+=sizeof(XDWORD);

    f3d->WriteString(skin->GetName(),block,&pos);

    XBYTE nSkinPose         = skin->GetPoses()->GetSize();                      memcpy(&(block->Get()[pos]),&nSkinPose,                             sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XBYTE nSkinCluster      = skin->GetClusters()->GetSize();                   memcpy(&(block->Get()[pos]),&nSkinCluster,                          sizeof(XBYTE));       pos+=sizeof(XBYTE);

    for (XDWORD e=0;e<nSkinPose;e++)
    {
          GGFSECTIONBUILDER*  builder=f3d->GetBuilders()->Get(GGF_SECTION_SKINPOSE);
          GGFSECTIONBLOCK*    skinpose_block  =builder->CreateBlock(f3d,skin->GetPoses()->Get(e));
                              block->Blocks.Add(skinpose_block);
    }

    for (XDWORD e=0;e<nSkinCluster;e++)
    {
          GGFSECTIONBUILDER*  builder=f3d->GetBuilders()->Get(GGF_SECTION_SKINCLUSTER);
          GGFSECTIONBLOCK*    skinpose_block  =builder->CreateBlock(f3d,skin->GetClusters()->Get(e));
                              block->Blocks.Add(skinpose_block);

    }
  }

  return block;
}

/*-------------------------------------------------------------------
//  GGFSKIN_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/05/2017 12:37:01
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFSKIN_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  GRPSKIN*      skin        =new GRPSKIN();
  XQWORD        pos         =*position;
  XBYTE         type        =0;
  XDWORD        size        =0;
  XDWORD        name_length =0;

  GRPELEMENT* element=static_cast<GRPELEMENT*>(parent);


  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_SKIN)
  {
    f3d->Output(4,__L("GGFSKIN_BUILDER Unexpected Section! : %d"),type);
    return false;
  }

  memcpy(&size,             &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&skin->tag,        &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  /*
  memcpy(&name_length,      &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (name_length)
  {
    XCHAR* name=new XCHAR[name_length+1];
    memcpy(name,        &(buffer[pos]), sizeof(XCHAR)*name_length);       pos+=sizeof(XCHAR)*name_length;
    name[name_length]=__C('\0');
    skin->GetName()->Set(name);
    delete(name);
  }
  */
  f3d->ReadString(skin->GetName(),buffer,position,&pos);

  XBYTE nSkinPose     = 0;    memcpy(&nSkinPose,    &(buffer[pos]),   sizeof(XBYTE)); pos+=sizeof(XBYTE);
  XBYTE nSkinClusters = 0;    memcpy(&nSkinClusters,&(buffer[pos]),   sizeof(XBYTE)); pos+=sizeof(XBYTE);

  *position=(*position)+size;

  for (XDWORD e=0;e<nSkinPose;e++)
    if (!f3d->GetBuilders()->Get(GGF_SECTION_SKINPOSE)->ReadBlock(f3d,buffer,position,skin))    return false;

  for (XDWORD e=0;e<nSkinClusters;e++)
    if (!f3d->GetBuilders()->Get(GGF_SECTION_SKINCLUSTER)->ReadBlock(f3d,buffer,position,skin)) return false;

  element->SetSkin(skin);

  return true;
}


/*-------------------------------------------------------------------
//  GGFSKINPOSE_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/05/2017 12:37:29
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        pnode :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFSKINPOSE_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void * pnode)
{
  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
                   block->SetType(GGF_SECTION_SKINPOSE);

  GRPSKINPOSE* skinpose=static_cast<GRPSKINPOSE*>(pnode);



  int calc_size=
                1*sizeof(XBYTE)+  //TYPE MATERIAL
                1*sizeof(XBYTE)+  //size
                1*sizeof(XDWORD)+ //tag
                /*
                1*sizeof(XBYTE)+  //name string size
                skinpose->GetName()->GetSize()*sizeof(XCHAR)+ //name
                */
                (skinpose->GetName()->GetSize()+1)*sizeof(XWORD)+ //name

                1*sizeof(XDWORD)+  //NodeID
                16*sizeof(GLFLOAT)+
                1*sizeof(XDWORD); //Nodelimb tag

  /*
  XDWORD        NodeId;
  GRPMATRIX     Matrix;
  GRPSKINLIMB*  Node;
  */


  block->Set(calc_size);

  XQWORD pos=0;
  if (block->Get())
  {
    XBYTE type              = GGF_SECTION_SKINPOSE;                             memcpy(&(block->Get()[pos]),&type,                                  sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XBYTE size              = calc_size;                                        memcpy(&(block->Get()[pos]),&size,                                  sizeof(XBYTE));       pos+=sizeof(XBYTE);

    XDWORD tag              = (XDWORD)(void*)skinpose;                          memcpy(&(block->Get()[pos]),&tag,                                   sizeof(XDWORD));      pos+=sizeof(XDWORD);
    /*
    XBYTE length            = skinpose->GetName()->GetSize();                   memcpy(&(block->Get()[pos]),&length,                                sizeof(XBYTE));       pos+=sizeof(XBYTE);
                              if (length)                                       memcpy(&(block->Get()[pos]),skinpose->GetName()->Get(),          length*sizeof(XCHAR));       pos+=length*sizeof(XCHAR);
    */
    f3d->WriteString(skinpose->GetName(),block,&pos);

    XDWORD nodeid           =skinpose->NodeId;                                  memcpy(&(block->Get()[pos]),&nodeid,                                sizeof(XDWORD));      pos+=sizeof(XDWORD);
    GLFLOAT matrix[16];     skinpose->Matrix.CopyTo(matrix);                    memcpy(&(block->Get()[pos]),&matrix,                             16*sizeof(GLFLOAT));     pos+=16*sizeof(GLFLOAT);
    XDWORD nodeTag          =(XDWORD)(void*)skinpose->Node;                     memcpy(&(block->Get()[pos]),&nodeTag,                               sizeof(XDWORD));      pos+=sizeof(XDWORD);

          GGFPROPERTY* property=new GGFPROPERTY();
          if (property)
                {
                    property->PropertyType=GGF_SECTION_SKINPOSE;
                    //property->owner.Set(skinpose->GetName()->Get());
                    property->ownertype=GGF_SECTION_SKINPOSE;
                    property->ownertag=(XDWORD)(void*)skinpose;

                    //property->reference.Set(skinpose->Node->GetName()->Get());
                    property->referencetype=GGF_SECTION_LOCATOR;
                    property->referencetag=(XDWORD)(void*)skinpose->Node;

                    f3d->GetProperties()->Add(property);
                  }


  }

  return block;
}


/*-------------------------------------------------------------------
//  GGFSKIN_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/05/2017 12:37:01
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFSKINPOSE_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  GRPSKINPOSE*  skinpose    =new GRPSKINPOSE();
  XQWORD        pos         =*position;
  XBYTE         type        =0;
  XDWORD        size        =0;
  XDWORD        name_length =0;

  GRPSKIN* skin=static_cast<GRPSKIN*>(parent);

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_SKINPOSE)
  {
    f3d->Output(4,__L("GGFSKINPOSE_BUILDER Unexpected Section! : %d"),type);
    return false;
  }

  memcpy(&size,             &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&skinpose->tag,    &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  /*
  memcpy(&name_length,      &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (name_length)
  {
    XCHAR* name=new XCHAR[name_length+1];
    memcpy(name,        &(buffer[pos]), sizeof(XCHAR)*name_length);       pos+=sizeof(XCHAR)*name_length;
    name[name_length]=__C('\0');
    skinpose->GetName()->Set(name);
    delete(name);
  }
  */
  f3d->ReadString(skinpose->GetName(),buffer,position,&pos);

    XDWORD NodeID;//,NodeTAG;
    GLFLOAT matrix[16];

    memcpy(&NodeID,               &(buffer[pos]), sizeof(XDWORD));      pos+=sizeof(XDWORD);      skinpose->NodeId=NodeID;
    memcpy(&matrix,               &(buffer[pos]), 16*sizeof(GLFLOAT));  pos+=16*sizeof(GLFLOAT);  skinpose->Matrix.BuildMatrixFromFloatArray(matrix);
    memcpy(&skinpose->skinlimbtag,&(buffer[pos]), sizeof(XDWORD));      pos+=sizeof(XDWORD);

  *position=(*position)+size;

  f3d->skinpose.Add(skinpose);
  skin->AddPose(skinpose);
  return true;
}


/*-------------------------------------------------------------------
//  GGFSKINCLUSTER_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/05/2017 12:43:05
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        pnode :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFSKINCLUSTER_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void * pnode)
{
  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
                   block->SetType(GGF_SECTION_SKINPOSE);

  GRPSKINCLUSTER* skincluster=static_cast<GRPSKINCLUSTER*>(pnode);

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE MATERIAL
                1*sizeof(XDWORD)+  //size
                1*sizeof(XDWORD)+ //tag
                /*
                1*sizeof(XBYTE)+  //name string size
                skincluster->GetName()->GetSize()*sizeof(XCHAR)+ //name
                */
                (skincluster->GetName()->GetSize()+1)*sizeof(XWORD)+ //name
                1*sizeof(XDWORD)+  //nIndexes
                skincluster->Indexes.GetSize()*sizeof(XDWORD)+   //Indexes
                1*sizeof(XDWORD)+   //nWeights
                skincluster->Weights.GetSize()*sizeof(double)+    //Weights
                16*sizeof(GLFLOAT)+ //Transform
                16*sizeof(GLFLOAT); //TransformLink

  /*
  XVECTOR<XDWORD>           Indexes;
  XVECTOR<double>           Weights;

  GRPMATRIX                 Transform;
  GRPMATRIX                 TransformLink;
  */


  block->Set(calc_size);

  XQWORD pos=0;
  if (block->Get())
  {
    XBYTE type              = GGF_SECTION_SKINCLUSTER;                          memcpy(&(block->Get()[pos]),&type,                                  sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XDWORD size             = calc_size;                                        memcpy(&(block->Get()[pos]),&size,                                  sizeof(XDWORD));      pos+=sizeof(XDWORD);

    XDWORD tag              = (XDWORD)(void*)skincluster;                       memcpy(&(block->Get()[pos]),&tag,                                   sizeof(XDWORD));      pos+=sizeof(XDWORD);
    /*
    XBYTE length            = skincluster->GetName()->GetSize();                memcpy(&(block->Get()[pos]),&length,                                sizeof(XBYTE));       pos+=sizeof(XBYTE);
                              if (length)                                       memcpy(&(block->Get()[pos]),skincluster->GetName()->Get(),           length*sizeof(XCHAR));       pos+=length*sizeof(XCHAR);
    */
    f3d->WriteString(skincluster->GetName(),block,&pos);

    XDWORD nIndexes         = skincluster->Indexes.GetSize();                   memcpy(&(block->Get()[pos]),&nIndexes,                              sizeof(XDWORD));      pos+=sizeof(XDWORD);
    for (XDWORD e=0;e<nIndexes;e++)
    {
      XDWORD i= skincluster->Indexes.Get(e);
      memcpy(&(block->Get()[pos]),&i,sizeof(XDWORD));
      pos+=sizeof(XDWORD);
    }
    XDWORD nWeights         = skincluster->Weights.GetSize();                   memcpy(&(block->Get()[pos]),&nWeights,                              sizeof(XDWORD));      pos+=sizeof(XDWORD);
    for (XDWORD e=0;e<nWeights;e++)
    {
      double w= skincluster->Weights.Get(e);
      memcpy(&(block->Get()[pos]),&w,sizeof(double));
      pos+=sizeof(double);
    }
    GLFLOAT matrix[16];   skincluster->Transform.CopyTo(matrix);                memcpy(&(block->Get()[pos]),matrix,16*sizeof(GLFLOAT));     pos+=16*sizeof(GLFLOAT);
                          skincluster->TransformLink.CopyTo(matrix);            memcpy(&(block->Get()[pos]),matrix,16*sizeof(GLFLOAT));     pos+=16*sizeof(GLFLOAT);

    for (XDWORD e=0;e<skincluster->GetLimbs()->GetSize();e++)
    {
          GGFPROPERTY* property=new GGFPROPERTY();
          if (property)
                {
                    property->PropertyType=GGF_SECTION_SKINCLUSTER;
                    //property->owner.Set(skincluster->GetName()->Get());
                    property->ownertype=GGF_SECTION_SKINCLUSTER;
                    property->ownertag=(XDWORD)(void*)skincluster;

                    //property->reference.Set(skincluster->GetLimbs()->Get(e)->GetName()->Get());
                    property->referencetype=GGF_SECTION_LOCATOR;
                    property->referencetag=(XDWORD)(void*)skincluster->GetLimbs()->Get(e);

                    f3d->GetProperties()->Add(property);
                  }
    }

  }



  return block;
}


/*-------------------------------------------------------------------
//  GGFSKINCLUSTER_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/05/2017 12:43:12
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFSKINCLUSTER_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  GRPSKINCLUSTER* skincluster =new GRPSKINCLUSTER();
  XQWORD          pos         =*position;
  XBYTE           type        =0;
  XDWORD          size        =0;
  XDWORD          name_length =0;

  GRPSKIN* skin = static_cast<GRPSKIN*>(parent);

  //-----

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_SKINCLUSTER)
  {
    f3d->Output(4,__L("GGFSKINCLUSTER_BUILDER Unexpected Section! : %d"),type);
    return false;
  }

  memcpy(&size,             &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  memcpy(&skincluster->tag, &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  /*
  memcpy(&name_length,      &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (name_length)
  {
    XCHAR* name=new XCHAR[name_length+1];
    memcpy(name,        &(buffer[pos]), sizeof(XCHAR)*name_length);       pos+=sizeof(XCHAR)*name_length;
    name[name_length]=__C('\0');
    skincluster->GetName()->Set(name);
    delete(name);
  }
  */
    f3d->ReadString(skincluster->GetName(),buffer,position,&pos);

    XDWORD nIndexes=0;    memcpy(&nIndexes, &(buffer[pos]), sizeof(XDWORD));  pos+=sizeof(XDWORD);
    for (XDWORD e=0;e<nIndexes;e++)
    {
      XDWORD i= 0;
      memcpy(&i,&(buffer[pos]), sizeof(XDWORD));
      pos+=sizeof(XDWORD);
      skincluster->Indexes.Add(i);
    }

    XDWORD nWeights=0;    memcpy(&nWeights, &(buffer[pos]), sizeof(XDWORD));  pos+=sizeof(XDWORD);
    for (XDWORD e=0;e<nWeights;e++)
    {
      double w= 0;
      memcpy(&w,&(buffer[pos]), sizeof(double));
      pos+=sizeof(double);
      skincluster->Weights.Add(w);
    }

    GLFLOAT matrix[16];
    memcpy(matrix,  &(buffer[pos]),16*sizeof(GLFLOAT));     pos+=16*sizeof(GLFLOAT);  skincluster->Transform.BuildMatrixFromFloatArray(matrix);
    memcpy(matrix,  &(buffer[pos]),16*sizeof(GLFLOAT));     pos+=16*sizeof(GLFLOAT);  skincluster->TransformLink.BuildMatrixFromFloatArray(matrix);

  *position=(*position)+size;

  //-----

  skin->AddCluster(skincluster);
  f3d->skincluster.Add(skincluster);
  return true;
}

/*-------------------------------------------------------------------
//  MATERIAL_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/04/2017 14:42:01
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        pnode :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFMATERIAL_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
                   block->SetType(GGF_SECTION_MATERIAL);

  GRPMATERIAL* material=static_cast<GRPMATERIAL*>(pnode);
//  f3d->Output(0,__L("Saving material: %s"),material->GetName()->Get());

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE MATERIAL
                1*sizeof(XBYTE)+  //size
                1*sizeof(XDWORD)+ //tag
                /*
                1*sizeof(XBYTE)+  //name string size
                material->GetName()->GetSize()*sizeof(XCHAR)+ //name
                */
                (material->GetName()->GetSize()+1)*sizeof(XWORD)+ //name

                1*sizeof(GLFLOAT)+ //glosiness
                1*sizeof(GLFLOAT)+ //shininess
                1*sizeof(GLFLOAT)+ //shininessFactor
                1*sizeof(GLFLOAT)+ //dissolve
                1*sizeof(GLFLOAT)+ //specularFactor

                4*sizeof(GLFLOAT)+ //ambientcolor
                4*sizeof(GLFLOAT)+ //diffusecolor
                4*sizeof(GLFLOAT)+ //specularColor
                4*sizeof(GLFLOAT)+ //emissioncolor

                1*sizeof(GLFLOAT)+ //intensity
                1*sizeof(GLFLOAT)+ //transparency
                1*sizeof(XBYTE); //number of layers

  block->Set(calc_size);


  XQWORD pos=0;
  if (block->Get())
  {
    XBYTE type              = GGF_SECTION_MATERIAL;                             memcpy(&(block->Get()[pos]),&type,                                sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XBYTE size              = calc_size;                                        memcpy(&(block->Get()[pos]),&size,                                sizeof(XBYTE));       pos+=sizeof(XBYTE);

    XDWORD tag              = (XDWORD)(void*)material;                          memcpy(&(block->Get()[pos]),&tag,                                 sizeof(XDWORD));      pos+=sizeof(XDWORD);
    /*
    XBYTE length            = material->GetName()->GetSize();                   memcpy(&(block->Get()[pos]),&length,                              sizeof(XBYTE));       pos+=sizeof(XBYTE);
                              if (length)                                       memcpy(&(block->Get()[pos]),material->GetName()->Get(),    length*sizeof(XCHAR));       pos+= length*sizeof(XCHAR);
                              */
    f3d->WriteString(material->GetName(),block,&pos);

    GLFLOAT glosiness       = material->GetAttributes()->GetGlossiness      (); memcpy(&(block->Get()[pos]),&glosiness,                           sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
    GLFLOAT shininess       = material->GetAttributes()->GetShininess       (); memcpy(&(block->Get()[pos]),&shininess,                           sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
    GLFLOAT shininessFactor = material->GetAttributes()->GetShininessFactor (); memcpy(&(block->Get()[pos]),&shininessFactor,                     sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
    GLFLOAT dissolve        = material->GetAttributes()->GetDissolve        (); memcpy(&(block->Get()[pos]),&dissolve,                            sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
    GLFLOAT specularFactor  = material->GetAttributes()->GetSpecularFactor  (); memcpy(&(block->Get()[pos]),&specularFactor,                      sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);

    GLFLOAT ambientcolor[4];
            ambientcolor[0]=  material->GetAttributes()->GetAmbient()->red;
            ambientcolor[1]=  material->GetAttributes()->GetAmbient()->green;
            ambientcolor[2]=  material->GetAttributes()->GetAmbient()->blue;
            ambientcolor[3]=  material->GetAttributes()->GetAmbient()->alpha;   memcpy(&(block->Get()[pos]),ambientcolor,                   4*sizeof(GLFLOAT));     pos+=4*sizeof(GLFLOAT);

    GLFLOAT diffuseColor[4];
            diffuseColor[0]=  material->GetAttributes()->GetDiffuse()->red;
            diffuseColor[1]=  material->GetAttributes()->GetDiffuse()->green;
            diffuseColor[2]=  material->GetAttributes()->GetDiffuse()->blue;
            diffuseColor[3]=  material->GetAttributes()->GetDiffuse()->alpha;   memcpy(&(block->Get()[pos]),diffuseColor,                   4*sizeof(GLFLOAT));     pos+=4*sizeof(GLFLOAT);

    GLFLOAT specularColor[4];
            specularColor[0]= material->GetAttributes()->GetSpecular()->red;
            specularColor[1]= material->GetAttributes()->GetSpecular()->green;
            specularColor[2]= material->GetAttributes()->GetSpecular()->blue;
            specularColor[3]= material->GetAttributes()->GetSpecular()->alpha;  memcpy(&(block->Get()[pos]),specularColor,                    4*sizeof(GLFLOAT));     pos+=4*sizeof(GLFLOAT);

    GLFLOAT emissionColor[4];
            emissionColor[0]= material->GetAttributes()->GetEmission()->red;
            emissionColor[1]= material->GetAttributes()->GetEmission()->green;
            emissionColor[2]= material->GetAttributes()->GetEmission()->blue;
            emissionColor[3]= material->GetAttributes()->GetEmission()->alpha;  memcpy(&(block->Get()[pos]),emissionColor,                    4*sizeof(GLFLOAT));     pos+=4*sizeof(GLFLOAT);

    GLFLOAT intensity       = material->GetAttributes()->GetIntensity();        memcpy(&(block->Get()[pos]),&intensity,                         sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
    GLFLOAT transparency    = material->GetAttributes()->GetTransparency();     memcpy(&(block->Get()[pos]),&transparency,                      sizeof(GLFLOAT));     pos+=sizeof(GLFLOAT);
    XBYTE   nlayers         = material->GetLayers()->GetSize();                 memcpy(&(block->Get()[pos]),&nlayers,                           sizeof(XBYTE));       pos+=sizeof(XBYTE);
  }

  //----------------------------------------------------------------- add layers as properties
  for (XDWORD e=0;e<material->GetLayers()->GetSize();e++)
  {
    XDWORD        layertype =material->GetLayers()->GetKey(e);
    GRPTEXTURE*   texture   =material->GetLayers()->GetElement(e);

    GGFPROPERTY*  property  =new GGFPROPERTY();
                  property->PropertyType  =layertype;

                  property->ownertype     =GGF_SECTION_MATERIAL;
                  property->ownertag      =(XDWORD)(void*)material;

                  property->referencetype =GGF_SECTION_TEXTURE;
                  property->referencetag  =(XDWORD)(void*)texture;

    f3d->GetProperties()->Add(property);
  }

  return block;
}

/*-------------------------------------------------------------------
//  GGFMATERIAL_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/04/2017 10:27:40
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
*/
/*-----------------------------------------------------------------*/
bool GGFMATERIAL_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  GRPMATERIAL*  material    =new GRPMATERIAL();
  XQWORD        pos         =*position;
  XBYTE         type        =0;
  XDWORD        size        =0;
  XDWORD        name_length =0;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_MATERIAL)
  {
    f3d->Output(4,__L("GGFMATERIAL_BUILDER Unexpected Section! : %d"),type);
    return false;
  }

  memcpy(&size,             &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  memcpy(&material->tag,    &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  /*
  memcpy(&name_length,      &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  if (name_length)
  {
    XCHAR* name=new XCHAR[name_length+1];
    memcpy(name,        &(buffer[pos]), sizeof(XCHAR)*name_length);       pos+=sizeof(XCHAR)*name_length;
    name[name_length]=__C('\0');
    material->GetName()->Set(name);
    delete(name);
  }
  */
  f3d->ReadString(material->GetName(),buffer,position,&pos);

  GLFLOAT glosiness       = 0.0f;   memcpy(&glosiness,        &(buffer[pos]),   sizeof(GLFLOAT)); pos+=sizeof(GLFLOAT);   material->GetAttributes()->SetGlossiness      (glosiness);
  GLFLOAT shininess       = 0.0f;   memcpy(&shininess,        &(buffer[pos]),   sizeof(GLFLOAT)); pos+=sizeof(GLFLOAT);   material->GetAttributes()->SetShininess       (shininess);
  GLFLOAT shininessFactor = 0.0f;   memcpy(&shininessFactor,  &(buffer[pos]),   sizeof(GLFLOAT)); pos+=sizeof(GLFLOAT);   material->GetAttributes()->SetShininessFactor (shininessFactor);
  GLFLOAT dissolve        = 0.0f;   memcpy(&dissolve,         &(buffer[pos]),   sizeof(GLFLOAT)); pos+=sizeof(GLFLOAT);   material->GetAttributes()->SetDissolve        (dissolve);
  GLFLOAT specularfactor  = 0.0f;   memcpy(&specularfactor,   &(buffer[pos]),   sizeof(GLFLOAT)); pos+=sizeof(GLFLOAT);   material->GetAttributes()->SetSpecularFactor  (specularfactor);

  GLFLOAT ambientColor  [4];        memcpy(ambientColor,      &(buffer[pos]), 4*sizeof(GLFLOAT)); pos+=4*sizeof(GLFLOAT); material->GetAttributes()->SetAmbient       (GRPCOLOR(ambientColor  [0],  ambientColor  [1],  ambientColor  [2],  ambientColor  [3]));
  GLFLOAT diffuseColor  [4];        memcpy(diffuseColor,      &(buffer[pos]), 4*sizeof(GLFLOAT)); pos+=4*sizeof(GLFLOAT); material->GetAttributes()->SetDiffuse       (GRPCOLOR(diffuseColor  [0],  diffuseColor  [1],  diffuseColor  [2],  diffuseColor  [3]));
  GLFLOAT specularColor [4];        memcpy(specularColor,     &(buffer[pos]), 4*sizeof(GLFLOAT)); pos+=4*sizeof(GLFLOAT); material->GetAttributes()->SetSpecular      (GRPCOLOR(specularColor [0],  specularColor [1],  specularColor [2],  specularColor [3]));
  GLFLOAT emissionColor [4];        memcpy(emissionColor,     &(buffer[pos]), 4*sizeof(GLFLOAT)); pos+=4*sizeof(GLFLOAT); material->GetAttributes()->SetEmission      (GRPCOLOR(emissionColor [0],  emissionColor [1],  emissionColor [2],  emissionColor [3]));

  GLFLOAT intensity       = 0.0f;   memcpy(&intensity,        &(buffer[pos]),   sizeof(GLFLOAT)); pos+=sizeof(GLFLOAT);   material->GetAttributes()->SetIntensity     (intensity);
  GLFLOAT transparency    = 0.0f;   memcpy(&transparency,     &(buffer[pos]),   sizeof(GLFLOAT)); pos+=sizeof(GLFLOAT);   material->GetAttributes()->SetTransparency  (transparency);

  XBYTE   nlayers         = 0;      memcpy(&nlayers,          &(buffer[pos]),   sizeof(XBYTE));   pos+=sizeof(XBYTE);

  material->GetAttributes()->isupdated=true;

  GRPMATERIALLIBRARY::Get()->Add(material);

  f3d->materials.Add(material);

  *position=(*position)+size;
  return true;
}

/*-------------------------------------------------------------------
//  GGFTEXTURE_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/04/2017 11:47:55
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        pnode :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFTEXTURE_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
  block->SetType(GGF_SECTION_TEXTURE);

  GRPTEXTURE* texture=static_cast<GRPTEXTURE*>(pnode);
  //f3d->Output(0,__L("Saving texture: %s"),texture->GetName()->Get());

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE TEXTURE
                1*sizeof(XWORD)+  //size
                1*sizeof(XDWORD)+ //tag
                /*
                1*sizeof(XBYTE)+  //name string size
                texture->GetName()->GetSize()*sizeof(XCHAR)+
                */
                (texture->GetName()->GetSize()+1)*sizeof(XWORD)+
                1*sizeof(XBYTE)+  //texture layer type
                1*sizeof(XBYTE)+  //is cube map
                /*
                1*sizeof(XBYTE)+  //texture filename size
                texture->GetFileName()->GetSize()*sizeof(XCHAR)+
                */
                (texture->GetFileName()->GetSize()+1)*sizeof(XWORD)+
                1*sizeof(XDWORD)+ //width
                1*sizeof(XDWORD); //height

  block->Set(calc_size);

  XQWORD pos=0;
  if (block->Get())
  {
    XBYTE type              = GGF_SECTION_TEXTURE;                memcpy(&(block->Get()[pos]),&type,                                      sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XWORD size              = calc_size;                          memcpy(&(block->Get()[pos]),&size,                                      sizeof(XWORD));       pos+=sizeof(XWORD);

    XDWORD tag              =(XDWORD)(void*)texture;              memcpy(&(block->Get()[pos]),&tag,                                       sizeof(XDWORD));      pos+=sizeof(XDWORD);
    /*
    XBYTE length            = texture->GetName()->GetSize();      memcpy(&(block->Get()[pos]),&length,                                    sizeof(XBYTE));       pos+=sizeof(XBYTE);
                              if (length)                         memcpy(&(block->Get()[pos]),texture->GetName()->Get(),           length*sizeof(XCHAR));       pos+= length*sizeof(XCHAR);
                              */
    f3d->WriteString(texture->GetName(),block,&pos);
    XBYTE layertype         = texture->GetLayertype ();           memcpy(&(block->Get()[pos]),&layertype,                                     sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XBYTE is_cube_map       = texture->GetIsCubeMap ();           memcpy(&(block->Get()[pos]),&is_cube_map,                               sizeof(XBYTE));       pos+=sizeof(XBYTE);

    /*
    XBYTE filelength        = texture->GetFileName()->GetSize();  memcpy(&(block->Get()[pos]),&filelength,                                sizeof(XBYTE));       pos+=sizeof(XBYTE);
                              if (filelength)                     memcpy(&(block->Get()[pos]),texture->GetFileName()->Get(),   filelength*sizeof(XCHAR));       pos+= filelength*sizeof(XCHAR);
    */
    f3d->WriteString(texture->GetFileName(),block,&pos);
    XDWORD width            = (XDWORD)texture->GetWidth     ();   memcpy(&(block->Get()[pos]),&width,                                     sizeof(XDWORD));      pos+=sizeof(XDWORD);
    XDWORD height           = (XDWORD)texture->GetHeight    ();   memcpy(&(block->Get()[pos]),&height,                                    sizeof(XDWORD));      pos+=sizeof(XDWORD);
  }

  return block;
}

/*-------------------------------------------------------------------
//  GGFTEXTURE_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/04/2017 13:01:07
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
*/
/*-----------------------------------------------------------------*/
bool GGFTEXTURE_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  GRPTEXTURE*   texture     =NULL;
  XQWORD        pos         =*position;
  XBYTE         type        =0;
  XDWORD        size        =0;
  XDWORD        name_length =0;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_TEXTURE)
  {
    f3d->Output(4,__L("GGFTEXTURE_BUILDER Unexpected Section! : %d"),type);
    return false;
  }

  XDWORD tag=0;

  memcpy(&size,             &(buffer[pos]), sizeof(XWORD));               pos+=sizeof(XWORD);
  memcpy(&tag,              &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  /*
  memcpy(&name_length,      &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);

  XSTRING texture_name,file_name;

  if (name_length)
  {
    XCHAR* name=new XCHAR[name_length+1];
    memcpy(name,        &(buffer[pos]), sizeof(XCHAR)*name_length);       pos+=sizeof(XCHAR)*name_length;
    name[name_length]=__C('\0');
    texture_name.Set(name);
    delete(name);
  }*/
  XSTRING name,filename;
  f3d->ReadString(&name,buffer,position,&pos);

    XBYTE layertype;            memcpy(&layertype,                    &(buffer[pos]),                                   sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XBYTE is_cube_map;          memcpy(&is_cube_map,                  &(buffer[pos]),                                   sizeof(XBYTE));       pos+=sizeof(XBYTE);
    /*
    XBYTE filelength;           memcpy(&filelength,                   &(buffer[pos]),                                   sizeof(XBYTE));       pos+=sizeof(XBYTE);

    if (filelength)
    {
      XCHAR* name=new XCHAR[filelength+1];
      memcpy(name,        &(buffer[pos]), sizeof(XCHAR)*filelength);        pos+=sizeof(XCHAR)*filelength;
      name[filelength]=__C('\0');
      file_name.Set(name);
      delete(name);
    }
    */
    f3d->ReadString(&filename,buffer,position,&pos);

    XDWORD width;               memcpy(&width,                        &(buffer[pos]),                                   sizeof(XDWORD));      pos+=sizeof(XDWORD);
    XDWORD height;              memcpy(&height,                       &(buffer[pos]),                                   sizeof(XDWORD));      pos+=sizeof(XDWORD);


    XPATH xpath,fileext;

    xpath.Set(filename);
    xpath.GetNamefileExt(fileext);

    xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1,fileext.Get());

    f3d->timer->Reset();

    if (!GRPMATERIALLIBRARY::Get()->Create(xpath.Get(), name.Get()))
    {
      f3d->Output(4,__L("Failed reading texture %s"),name.Get());
    }
    else
    {
    texture=GRPMATERIALLIBRARY::Get()->GetTexture(name.Get());

    texture->SetLayertype((GRPTEXTURELAYERTYPE)layertype);
    texture->SetIsCubeMap(is_cube_map!=0);
    texture->SetTag(tag);

    f3d->textures.Add(texture);
    }
    f3d->texture_timer_total+=f3d->timer->GetMeasureMilliSeconds();


  *position=(*position)+size;
  return true;
}

/*-------------------------------------------------------------------
//  GGFSHADER_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/06/2017 13:46:39
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        pnode :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFSHADER_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK* block=new GGFSECTIONBLOCK();
  block->SetType(GGF_SECTION_SHADER);

  GRPSHADERPROGRAM* program=static_cast<GRPSHADERPROGRAM*>(pnode);
  int calc_size=
                1*sizeof(XBYTE)+  //TYPE GGF_SECTION_SHADER
                1*sizeof(XWORD)+  //size
                1*sizeof(XDWORD)+ //tag
                //1*sizeof(XBYTE)+  //name string size
                (program->GetName()->GetSize()+1)*sizeof(XWORD);
                //program->GetName()->GetSize()*sizeof(XCHAR);


  block->Set(calc_size);

  XQWORD pos=0;
  if (block->Get())
  {
    XBYTE type              = GGF_SECTION_SHADER;                 memcpy(&(block->Get()[pos]),&type,                                      sizeof(XBYTE));       pos+=sizeof(XBYTE);
    XWORD size              = calc_size;                          memcpy(&(block->Get()[pos]),&size,                                      sizeof(XWORD));       pos+=sizeof(XWORD);

    XDWORD tag              = (XDWORD)(void*)program;             memcpy(&(block->Get()[pos]),&tag,                                       sizeof(XDWORD));      pos+=sizeof(XDWORD);
    f3d->WriteString(program->GetName(),block,&pos);
  }
  return block;
}

/*-------------------------------------------------------------------
//  GGFSHADER_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/06/2017 13:54:17
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFSHADER_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{
  XQWORD        pos         =*position;
  XBYTE         type        =0;
  XDWORD        size        =0;
  XDWORD        name_length =0;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_SHADER)
  {
    f3d->Output(4,__L("GGF_SECTION_SHADER Unexpected Section! : %d"),type);
    return false;
  }

  XDWORD tag=0;

  memcpy(&size,             &(buffer[pos]), sizeof(XWORD));               pos+=sizeof(XWORD);
  memcpy(&tag,              &(buffer[pos]), sizeof(XDWORD));              pos+=sizeof(XDWORD);
  XSTRING shadername;
  /*
  memcpy(&name_length,      &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (name_length)
  {
    XCHAR* name=new XCHAR[name_length+1];
    memcpy(name,        &(buffer[pos]), sizeof(XCHAR)*name_length);       pos+=sizeof(XCHAR)*name_length;
    name[name_length]=__C('\0');
    shadername.Set(name);
    delete(name);
  }
  */
  f3d->ReadString(&shadername,buffer,position,&pos);

  GRPSHADERPROGRAM* program=new GRPSHADERPROGRAM();
                    program->SetName(shadername.Get());
                    program->tag=tag;

  *position=(*position)+size;

  f3d->shaders.Add(program);
  return true;
}

/*-------------------------------------------------------------------
//  GGFDATA_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/05/2017 15:52:35
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        pnode :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFDATA_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* pnode)
{
  GRPSCENE* scene=f3d->GetScene();

  GGFSECTIONBLOCK*  block=new GGFSECTIONBLOCK();
                    block->SetType(GGF_SECTION_DATA);

  GRPUSERDATACONTAINER* data=static_cast<GRPUSERDATACONTAINER*>(pnode);
  if (!data)
    return NULL;

  int calc_size=
                1*sizeof(XBYTE)+  //TYPE DATA
                1*sizeof(XWORD)+  //size
                1*sizeof(XDWORD)+ //tag
                1*sizeof(XWORD);  //n datas

  for (XDWORD e=0;  e<data->Get()->GetSize(); e++)
  {
    XSTRING* key  =data->Get()->GetKey(e);
    XSTRING* value=data->Get()->GetElement(e);

    calc_size+=(key->GetSize()+1)*sizeof(XWORD);
    calc_size+=(value->GetSize()+1)*sizeof(XWORD);
  }

  block->Set(calc_size);

  XQWORD pos=0;
  if (block->Get())
  {
  XBYTE   type  = GGF_SECTION_DATA;                   memcpy(&(block->Get()[pos]),&type,  sizeof(XBYTE));       pos+=sizeof(XBYTE);
  XWORD   size  = calc_size;                          memcpy(&(block->Get()[pos]),&size,  sizeof(XWORD));       pos+=sizeof(XWORD);
  XDWORD  tag   = (XDWORD)(void*)data;                memcpy(&(block->Get()[pos]),&tag,   sizeof(XDWORD));      pos+=sizeof(XDWORD);

  XWORD   n     = data->Get()->GetSize();             memcpy(&(block->Get()[pos]),&n,     sizeof(XWORD));       pos+=sizeof(XWORD);

    for (XDWORD e=0;  e<data->Get()->GetSize(); e++)
    {
      XSTRING* key  =data->Get()->GetKey(e);
      XSTRING* value=data->Get()->GetElement(e);

      f3d->WriteString(key,   block,&pos);
      f3d->WriteString(value, block,&pos);
    }
  }
  return block;
}

/*-------------------------------------------------------------------
//  GGFDATA_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/05/2017 15:52:49
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFDATA_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void * parent)
{

  GRPUSERDATACONTAINER* datacontainer=new GRPUSERDATACONTAINER();

  XQWORD        pos         =*position;
  XBYTE         type        =0;
  XDWORD        size        =0;
  XDWORD        name_length =0;

  memcpy(&type,         &(buffer[pos]), sizeof(XBYTE));               pos+=sizeof(XBYTE);
  if (type!=GGF_SECTION_DATA)
  {
    f3d->Output(4,__L("GGF_SECTION_DATA Unexpected Section! : %d"),type);
    return false;
  }

  XDWORD tag=0;
              memcpy(&size,   &(buffer[pos]), sizeof(XWORD));   pos+=sizeof(XWORD);
              memcpy(&tag,    &(buffer[pos]), sizeof(XDWORD));  pos+=sizeof(XDWORD);
  XWORD n=0;  memcpy(&n,      &(buffer[pos]), sizeof(XWORD));   pos+=sizeof(XWORD);


  for (XWORD e=0;e<n;e++)
  {
    XSTRING key;
    XSTRING value;

    f3d->ReadString(&key,buffer,position,&pos);
    f3d->ReadString(&value,buffer,position,&pos);

    datacontainer->Add(key.Get(),value.Get());
  }

  if (n)
  {
  f3d->datacontainers.Add(datacontainer);
  datacontainer->SetTag(tag);
  }
  else
    delete(datacontainer);

  *position=(*position)+size;

  return true;
}

/*-------------------------------------------------------------------
//  SOUNDS_BUILDER::CreateBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/06/2017 13:20:51
//
//  @return       GGFSECTIONBLOCK* :
//
//  @param        f3d :
//  @param        node :
*/
/*-----------------------------------------------------------------*/
GGFSECTIONBLOCK* GGFSOUNDS_BUILDER::CreateBlock(GRPFILE3DGGF* f3d, void* node)
{

  return NULL;
}


/*-------------------------------------------------------------------
//  SOUNDS_BUILDER::ReadBlock
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/06/2017 13:20:59
//
//  @return       bool :
//
//  @param        f3d :
//  @param        buffer :
//  @param        position :
//  @param        parent :
*/
/*-----------------------------------------------------------------*/
bool GGFSOUNDS_BUILDER::ReadBlock(GRPFILE3DGGF* f3d,XBYTE* buffer, XQWORD* position, void* parent)
{
  return true;
}



/*-------------------------------------------------------------------
//  GGFSECTIONBLOCK::Set
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 10:50:24
//
//  @return       XBYTE* :
//
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XBYTE* GGFSECTIONBLOCK::Set(XQWORD size)
{
  if (size)
  {
    if (this->size)
    {
      delete[] this->header;
    }
    this->header=new XBYTE[(int)size];
    this->size=size;
  }
  else
  {
    delete(this->header);
    this->header=NULL;
    size=0;
  }
  return this->header;
}

/*-------------------------------------------------------------------
//  GGFSECTIONBLOCK::GetSize
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 10:50:33
//
//  @return       XQWORD :
//
*/
/*-----------------------------------------------------------------*/
XQWORD GGFSECTIONBLOCK::GetSize()
{
  XQWORD total_size=this->size;

  for (XDWORD e=0;e<this->Blocks.GetSize();e++)
  {
    total_size+=this->Blocks.Get(e)->GetSize();
  }

  return total_size;
}

/*-------------------------------------------------------------------
//  GGFSECTIONBLOCK::GetHeaderSize
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 13:22:38
//
//  @return       XQWORD :
//
*/
/*-----------------------------------------------------------------*/
XQWORD GGFSECTIONBLOCK::GetHeaderSize()
{
  return this->size;
}

/*-------------------------------------------------------------------
//  GGFSECTIONBLOCK::SetType
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/04/2017 11:45:47
//
//  @param        s :
*/
/*-----------------------------------------------------------------*/
void GGFSECTIONBLOCK::SetType(GGF_SECTION_TYPE s)
{
  this->sectiontype=s;
}

/*-------------------------------------------------------------------
//  GGFSECTIONBLOCK::Get
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/04/2017 10:50:42
//
//  @return       XBYTE* :
//
*/
/*-----------------------------------------------------------------*/
XBYTE* GGFSECTIONBLOCK::Get()
{
  return this->header;
}



