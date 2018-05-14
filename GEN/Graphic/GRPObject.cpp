/*------------------------------------------------------------------------------------------
//  GRPOBJECT.CPP
//
//  Graphics Base for Object class
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 19/05/2014 11:31:15
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XTimer.h"

#include "GRPObject.h"
#include "GRPTexture.h"
#include "GRPViewport.h"
#include "GRPScene.h"
#include "GRPPass.h"
#include "GRPCamera.h"
#include "GRPLight.h"
#include "GRPTextBox.h"
#include "GRPObjectBuilder.h"
#include "GRPFont.h"
#include "GRPFontManager.h"

#include "GRPMaterialLibrary.h"

#include "XTimer.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  GRPOBJECT::GRPOBJECT
*/
/**
//
//  Class Constructor GRPOBJECT
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/05/2014 11:33:06
//
*/
/*-----------------------------------------------------------------*/
GRPOBJECT::GRPOBJECT()
{
  Clean();
  if (elements.GetSize()>0)
    elements.DeleteContents();

  AnimationControl=new GRPANIMATIONCONTROL();
}




/*-------------------------------------------------------------------
//  GRPOBJECT::~GRPOBJECT
*/
/**
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/05/2014 11:33:52
*/
/*-----------------------------------------------------------------*/
GRPOBJECT::~GRPOBJECT()
{
  Publish(GRPMSG_DESTROYED);

  elements.DeleteContentsInstanced();
  elements.DeleteAll();

  cameras.DeleteContentsInstanced();
  cameras.DeleteAll();

  lights.DeleteContentsInstanced();
  lights.DeleteAll();

  nodes.DeleteContentsInstanced();
  nodes.DeleteAll();

  delete(AnimationControl);

  if (logic)
    delete(logic);

  passes.DeleteAll();
  Clean();
}



/*-------------------------------------------------------------------
//  GRPOBJECT::SetTag
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/11/2016 16:58:22
//
//  @param        tag :
*/
/*-----------------------------------------------------------------*/
void GRPOBJECT::SetTag(XQWORD tag)
{
  for(XDWORD e=0; e<elements.GetSize(); e++)
    elements.Get(e)->SetTag(tag);
}


/*-------------------------------------------------------------------
//  GRPOBJECT::Draw
*/
/**
//
//  Draw it
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/05/2014 11:36:45
//
*/
/*-----------------------------------------------------------------*/
bool GRPOBJECT::Draw(GRPMATRIX& viewmatrix,GRPSCENE* scene)
{
  for(XDWORD e=0; e<elements.GetSize(); e++)
    {
      if(GRPSHADER::CurrentShader==elements.Get(e)->GetShader()->GetID())
      {
        if(!elements.Get(e)->Draw(&viewmatrix,scene->GetPass()->GetViewProjection(),scene->GetPass()->GetProjectionMatrix()))
        return false;
      }
    }
  return true;
}


/*-------------------------------------------------------------------
//  GRPOBJECT::GetMaterial
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/09/2016 9:57:30
//
//  @return       GRPMATERIAL* :
//
*/
/*-----------------------------------------------------------------*/
GRPMATERIAL* GRPOBJECT::GetMaterial()
{
  return this->material;
}

/*-------------------------------------------------------------------
//  GRPOBJECT::SetMaterial
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/10/2015 10:11:55
//
//  @param        material :
*/
/*-----------------------------------------------------------------*/
void GRPOBJECT::SetMaterial(GRPMATERIAL* material)
{
  this->material=material;

  if (this->node!=NULL)
    node->SetMaterial(material);

  for(XDWORD e=0; e<elements.GetSize(); e++)
    {
      elements.Get(e)->GetNode()->SetMaterial(material);
    }
}


/*-------------------------------------------------------------------
//  GRPOBJECT::SetProgram
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/10/2015 10:12:01
//
//  @param        p :
*/
/*-----------------------------------------------------------------*/
void GRPOBJECT::SetProgram          (GRPSHADERPROGRAM* p)
{
  this->program=p;

  if (program==NULL)
    return;

  for(XDWORD e=0; e<elements.GetSize(); e++)
    {
      elements.Get(e)->SetProgram(p);
    }
}

/*-------------------------------------------------------------------
//  GRPOBJECT::SetRenderMode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/10/2015 10:12:06
//
//  @param        mode :
*/
/*-----------------------------------------------------------------*/
void GRPOBJECT::SetRenderMode           (GRPRENDERMODE mode)
{
  for(XDWORD e=0; e<elements.GetSize(); e++)
    {
      elements.Get(e)->SetRenderMode(mode);
    }
}

/*-------------------------------------------------------------------
//  GRPOBJECT::MergeElements
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/10/2015 10:11:00
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPOBJECT::MergeElements()
{
  //FIXME : bool para elegir si perder la rotación

  //------------------------------- prepare elements
  int n=this->elements.GetSize();

    //if (n<=1)     return true;
  GRPMATERIAL*            material    =NULL;
  GRPMATERIALATTRIBUTES   basematerial;

  if (elements.GetSize()>0)
  if (elements.FastGet(0)->GetNode())
  {
    material    =elements.FastGet(0)->GetNode()->GetMaterial();
    basematerial=*elements.FastGet(0)->GetNode()->GetBaseMaterial();
  }

  float rightmargin = elements.GetLast()->GetBoundingBox()->advance - elements.GetLast()->GetBoundingBox()->width;

  for (int e=n-1;e>=0;e--)
      if (!elements.FastGet(e)->Collapse(true))
        return false;



  GRPSHADERPROGRAM* Program=NULL;
  if (elements.GetSize()>0)
    Program=elements.FastGet(0)->GetProgram();

  XVECTOR<GRPOBSERVER*> observers;

  for (int e=0;e<n;e++)
  {
      GRPELEMENT* element=elements.FastGet(e);
                  //element->Publish(GRPMSG_DESTROYED);

      XVECTOR<GRPOBSERVER*>* v=element->GetObservers();

      //------------------ destroy references between elements
      int no=v->GetSize();
      for (int o=0;o<no;o++)
      {
        GRPOBSERVER* obj=v->FastGet(o);
        if (obj)
        {
            bool found=false;
            for (int i=0;i<n;i++)
              if (obj==elements.FastGet(i))
              {
                found=true;
                break;
              }
            if (!found)
              observers.Add(obj);
        }
      }
  }

    //----------------------------- prepare new buffers
  XDWORD totalVertex=0;
  XDWORD totalIndex =0;

  for (int e=0;e<n;e++)
  {
    GRPELEMENT* element  =  elements.FastGet        (e);
    totalVertex         +=  element->GetMesh()->GetNvertexData  ();
    totalIndex          +=  element->GetMesh()->GetNindexData ();
  }

  if (!totalVertex || !totalIndex)  return false;

  GRPVERTEX*  mergued_vertex=new GRPVERTEX  [totalVertex];
  if (mergued_vertex==NULL)
  {
    return false;
  }

  XWORD*      mergued_index =new XWORD      [totalIndex ];
  if (mergued_index==NULL)
  {
    delete[] mergued_vertex;
    return false;
  }


  //----------------------------- sum up the data
  int e=0,index=0,vertex=0;

  for (;e<n;e++)
  {
  GRPELEMENT* element=elements.FastGet(e);

  int nindexdata=element->GetMesh()->GetNindexData();
  int nvertexdata=element->GetMesh()->GetNvertexData();

  XWORD* idata=element->GetMesh()->GetIndexData();

  for (int i=0;i<nindexdata;i++)
    mergued_index [index+i] =idata[i]+index;

  index+=nindexdata;

  if (nvertexdata>0)
    memcpy(&mergued_vertex[vertex],element->GetMesh()->GetVertexData(),nvertexdata*sizeof(GRPVERTEX));

  vertex+=nvertexdata;
  }

  //----------------------------- destroy current elements
  GRPNODE* node=NULL;

  node=elements.FastGet(0)->GetNode();   //we save first node to give it to the merged result
  for (int e=0;e<n;e++)
  {
    elements.FastGet(e)->GetNode()->Dettach();
    if (e!=0)
      {
        // remove node and elements from rendering before continuing
        // mutex to be safe?
        delete(elements.FastGet(e)->GetNode());
      }

    delete(elements.FastGet(e));
  }
  elements.DeleteAll();

  //-----------------------------Create Final element
  GRPELEMENT* element=new GRPELEMENT();

              if (element==NULL)
                return false;

              GRPMESH* mesh=new GRPMESH();
              element->SetMesh(mesh);

              if (!node)
                return false;

              element->SetNode                    (node);
                          node->LinkTo            (this->node);

              element->GetMesh()->SetIndexData    (mergued_index);
              element->GetMesh()->SetNindexData   (index);
              element->GetMesh()->SetNvertexData  (vertex);
              element->GetMesh()->SetVertexData   (mergued_vertex);

              element->GetMesh()->SetIsLoaded     (false);
              element->SetProgram                 (Program);

              this->AddElement                    (element);
              this->SetMaterial                   (material);
              this->GetNode()->SetBaseMaterial    (&basematerial);

              //element->UpdateBoundingBox();
              XDWORD nobservers=observers.GetSize ();
              for (XDWORD o=0;o<nobservers;o++)
              {
                if(observers.Get(o))
                      element->GetObservers()->Add(observers.Get(o));
              }
              element->GetBoundingBox()->rightmargin = rightmargin;
              element->Publish                    (GRPMSG_ORDER);
  return true;
}


/*-------------------------------------------------------------------
//  GRPOBJECT::MergeAnimations
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/09/2016 11:48:24
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPOBJECT::MergeAnimations()
{
  bool verbose  =false;
  int nLights   =this->GetLights()->GetSize();
  int nMaterials=GRPMATERIALLIBRARY::Get()->GetLibrary()->GetSize();
  int nNodes    =this->nodes.GetSize();
  int nCameras  =this->GetCameras()->GetSize();

  if (this->AnimationControl!=NULL)
  {
    int nStacks=this->AnimationControl->GetStacks()->GetSize();
    for (int s=0;s<nStacks;s++)
    {
      GRPANIMATIONSTACK* stack=this->AnimationControl->GetStacks()->GetElement(s);
      XDEBUG_PRINTCOLOR(3,__L("Linking [%d] stack ---- %d %s"),stack->GetID(),s,stack->GetName()->Get());

      int nLayers=stack->GetLayers()->GetSize();

      for (int l=0;l<nLayers;l++)
      {
        GRPANIMATIONLAYER* layer=stack->GetLayers()->FastGet(l);
        int nAnimationNodes=layer->GetAnimationNodes()->GetSize();

        for (int n=0;n<nAnimationNodes;n++)
        {
          GRPANIMATIONNODE* animationnode=layer->GetAnimationNodes()->FastGet(n);
          if (animationnode->GetTarget()==NULL)
          {
            if(animationnode->GetTargetName()!=NULL)
            {
              ///---------------- check if root animation, discard
              if (animationnode->GetTargetTag()==0)
              {
                animationnode->removeme=true;
                continue;
              }
              ///---------------- check if isolated node, discard
              if (animationnode->GetCurveConnections()->GetSize()<1)
              {
                animationnode->removeme=true;
                continue;
              }

              XSTRING * animation_node_name =animationnode->GetTargetName();

              //----------------- check if animation node
              bool found=false;
              for (int e=0;e<nNodes;e++)
              {
                XSTRING * node_name           =nodes.FastGet(e)->GetName();

                if (node_name->Compare(animation_node_name->Get())==0)
                {
                  animationnode->SetTarget(nodes.FastGet(e));

                  if (verbose)
                  { XDEBUG_PRINTCOLOR(3,__L("Linking animation %lld %s %s"),animationnode->tag,animationnode->GetName()->Get(),node_name->Get());}

                  if (animationnode->GetType()==GRPANIMATIONCURVETYPE_VISIBILITY)
                    nodes.FastGet(e)->visibility_isdefined=true;

                  found=true;
                  break;
                }
              }

              //----------------- check if animation material
              if (!found)
                for (int e=0;e<nMaterials;e++)
              {
                GRPMATERIAL* material=GRPMATERIALLIBRARY::Get()->GetLibrary()->GetElement(e);
                XSTRING * material_name           =material->GetName();

                if (material_name->Compare(animation_node_name->Get())==0)
                {
                  animationnode->SetTarget(GRPMATERIALLIBRARY::Get()->GetLibrary()->GetElement(e));
                  if (verbose)
                  { XDEBUG_PRINTCOLOR(3,__L("Linking animation %lld %s %s"),animationnode->tag,animationnode->GetName()->Get(),material_name->Get());}

                  found=true;
                  break;
                }
              }

                //----------------- check if animation light
              if (!found)
                for (int e=0;e<nLights;e++)
              {
                XSTRING * light_name=this->GetLights()->FastGet(e)->GetName();
                if (light_name->Compare(animation_node_name->Get())==0)
                {
                  animationnode->SetTarget(this->GetLights()->FastGet(e));
                  if (verbose)
                  { XDEBUG_PRINTCOLOR(3,__L("Linking animation %lld %s %s"),animationnode->tag,animationnode->GetName()->Get(),light_name->Get());}
                  found=true;
                  break;
                }
              }

                //----------------- check if animation camera
              if (!found)
                for (int e=0;e<nCameras;e++)
              {
                XSTRING * camera_name             =this->GetCameras()->FastGet(e)->GetName();
                if (camera_name->Compare(animation_node_name->Get())==0)
                {
                  animationnode->SetTarget(this->GetCameras()->FastGet(e));
                  if (verbose)
                  { XDEBUG_PRINTCOLOR(3,__L("Linking animation %lld %s %s"),animationnode->tag,animationnode->GetName()->Get(),camera_name->Get());}
                  found=true;
                  break;
                }
              }

                  if (!found ||!animationnode->GetTarget())
                  { XDEBUG_PRINTCOLOR(4,__L("Animation target not found!  %s"),animation_node_name->Get());}

            }
            else
                  { XDEBUG_PRINTCOLOR(4,__L("Animation target NULL!  %s"),animationnode->GetTargetName()->Get());}
          }
        }
      }
    }
  }



  return true;
}


/*-------------------------------------------------------------------
//  GRPOBJECT::SetVisible
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/06/2016 13:30:48
//
//  @param        visible :
*/
/*-----------------------------------------------------------------*/
void GRPOBJECT::SetVisible(bool visible)
{
  XDWORD size = elements.GetSize();

  for(XDWORD i = 0; i < size; i++)
  {
    elements.FastGet(i)->SetVisible(visible);
  }
}


/*-------------------------------------------------------------------
//  GRPOBJECT::GetAnimationControl
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/09/2016 11:43:04
//
//  @return       GRPANIMATIONCONTROL* :
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONCONTROL*  GRPOBJECT::GetAnimationControl()
{
  return this->AnimationControl;
}

/*-------------------------------------------------------------------
//  GRPOBJECT::SetAnimationControl
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/05/2017 14:03:04
//
//  @param        ac :
*/
/*-----------------------------------------------------------------*/
void GRPOBJECT::SetAnimationControl(GRPANIMATIONCONTROL* ac)
{
  if (this->AnimationControl!=NULL)
    delete(this->AnimationControl);
  ac->SetOwner(this);
  this->AnimationControl=ac;
}


/*-------------------------------------------------------------------
//  GRPOBJECT::FindByName
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/10/2015 10:13:23
//
//  @return       GRPELEMENT* :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*  GRPOBJECT::FindByName(XCHAR* name)
{
  int n=this->elements.GetSize();
  for (int e=0;e<n;e++)
  {
    if (elements.FastGet(e)->GetName()->Compare(name)==0)
      return elements.FastGet(e);
  }
return NULL;
}

/*-------------------------------------------------------------------
//  GRPOBJECT::AddElement
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/10/2015 10:13:46
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool   GRPOBJECT::AddElement  (GRPELEMENT* element)
{
  element->AddObserver(this);
  this->GetElements()->Add(element);
  return true;
}


/*-------------------------------------------------------------------
//  GRPOBJECT::AddTextBox
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      26/09/2016 10:34:26
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool   GRPOBJECT::AddText   (GRPTEXTELEMENT* element)
{
  this->GetTexts()->Add(element);
  return true;
}




/*-------------------------------------------------------------------
//  GRPOBJECT::GetElements
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/10/2015 10:15:57
//
//  @return       XVECTOR<GRPELEMENT*>* :
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPELEMENT*>* GRPOBJECT::GetElements()
{
  return &elements;
}

/*-------------------------------------------------------------------
//  GRPOBJECT::GetProgram
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/09/2016 9:58:19
//
//  @return       GRPSHADERPROGRAM* :
//
*/
/*-----------------------------------------------------------------*/
GRPSHADERPROGRAM* GRPOBJECT::GetProgram()
{
  return this->program;
}

/*-------------------------------------------------------------------
//  GRPOBJECT::GetNode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/10/2015 10:16:29
//
//  @return       GRPNODE* :
//
*/
/*-----------------------------------------------------------------*/
GRPNODE*  GRPOBJECT::GetNode            ()
{
  return node;
}

/*-------------------------------------------------------------------
//  GRPOBJECT::SetNode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/10/2015 10:16:38
//
//  @param        n :
*/
/*-----------------------------------------------------------------*/
void  GRPOBJECT::SetNode                (GRPNODE* n)
{
  node=n;
}




/*-------------------------------------------------------------------
//  GRPSCENE::PlayAll
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/06/2015 15:41:10
//
*/
/*-----------------------------------------------------------------*/
void GRPOBJECT::PlayAll()
{

  if (this->GetAnimationControl())
  {
    for (XDWORD s=0;s<this->GetAnimationControl()->GetStacks()->GetSize();s++)
    {

      for (XDWORD l=0;l<this->GetAnimationControl()->GetStacks()->GetElement(s)->GetLayers()->GetSize();l++)
      {
        this->GetAnimationControl()->GetStacks()->GetElement(s)->GetLayers()->Get(l)->SetActive(true);
        this->GetAnimationControl()->GetStacks()->GetElement(s)->GetLayers()->Get(l)->SetLoopMode(GRPANIMATIONLOOPMODE_REPEAT);
      }
      this->GetAnimationControl()->GetStacks()->GetElement(s)->Rewind();
      this->GetAnimationControl()->GetStacks()->GetElement(s)->Play();
    }

  this->GetAnimationControl()->SetLoopMode  (GRPANIMATIONLOOPMODE_REPEAT);
  this->GetAnimationControl()->Play         ();
  }

  this->GetAnimationControl()->Reset();
}

bool  GRPOBJECT::Add(GRPPASS*    pass)
{
    return this->GetPasses()->Add(pass);
}

bool  GRPOBJECT::Add(GRPNODE* node)
{
    return nodes.Add(node);
}

bool  GRPOBJECT::Add(GRPCAMERA* camera)
{
    if (camera)
    {
        if (camera->upnode)
            Subscribe(camera->upnode);

        Subscribe(camera);
        return cameras.Add(camera);
    }
    return false;
}

bool  GRPOBJECT::Add(GRPLIGHT* light)
{
    if (light)
    {
        if (light->upnode)
            Subscribe(light->upnode);

        if (light->target)
            Subscribe(light->target);

        Subscribe(light);
        return lights.Add(light);
    }
    return false;
}

bool  GRPOBJECT::Add(GRPELEMENT* element)
{
    return AddElement(element);
}

bool  GRPOBJECT::Add(GRPTEXTELEMENT* textbox)
{
    return AddText(textbox);
}


/*-------------------------------------------------------------------
//  GRPOBJECT::CreateNode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/09/2016 12:41:23
//
//  @return       GRPNODE* :
//
*/
/*-----------------------------------------------------------------*/
GRPNODE* GRPOBJECT::CreateNode()
{
  GRPNODE* node = new GRPNODE();
  if(!node)
    return NULL;

  if(!(nodes.Add(node)))
    return NULL;

  node->managed=true;
  node->Init();

  return node;
}


/*-------------------------------------------------------------------
//   GRPOBJECT::CreateCamera
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  20/12/2017 10:17:34
//  @return   GRPCAMERA* :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
GRPCAMERA* GRPOBJECT::CreateCamera()
{
    GRPCAMERA* camera = new GRPCAMERA();
    if (camera)
    {
        this->Subscribe(camera);
        this->cameras.Add(camera);
    }
    return camera;
}

/*-------------------------------------------------------------------
//   GRPOBJECT::CreateLight
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  20/12/2017 10:17:40
//  @return   GRPLIGHT* :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
GRPLIGHT* GRPOBJECT::CreateLight()
{
  GRPLIGHT* light=new GRPLIGHT();
  if (light)
  {
    this->Subscribe(light);
    this->lights.Add(light);
  }
  return light;
}


/*-------------------------------------------------------------------
//  GRPOBJECT::CreateTextLocators
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/05/2017 17:58:56
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPOBJECT::CreateTextLocators()
{
      //------------------------------------- create Text Locators
      for (XDWORD e=0;e<this->GetNodes()->GetSize();e++)
      {
        GRPNODE* node=this->GetNodes()->Get(e);
        if (node->GetUserData()->Get())
        if (node->GetUserData()->GetSize()>0)
        {
            GRPTEXTCONTAINER* container=FindTextBox(node->GetName()->Get());
            if (container==NULL)
            {
                if (CreateTextFromLocator(node)==NULL)
                  return false;
            }
        }
      }
    return true;
}

/*-------------------------------------------------------------------
//  GRPOBJECT::CreateTextFromLocator
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/09/2016 10:01:38
//
//  @return       GRPTEXTELEMENT* :
//
//  @param        node :
*/
/*-----------------------------------------------------------------*/
GRPTEXTELEMENT* GRPOBJECT::CreateTextFromLocator(GRPNODE* pnode)
{
  //XDEBUG_PRINTCOLOR(2,__L("--------------------------------- Creating text locator %s"),pnode->GetName()->Get());

  GRPNODE* node=pnode;
  if (node==NULL)
    node=CreateNode();

    int textalign=GRPTEXTBOX_ALIGNRIGHT;
    XSTRING fontname=GRPOBJECTBUILDER::Get()->GetFontManager()->GetDefaultFont()->Get();
    XCHAR* textvalue=NULL;
    XCHAR* textmask=NULL;

    //------------------------------------------- obtain data
    XSTRING* userfontname=node->GetUserData()->GetUserDataByKey(__L("Font"));
    if (userfontname!=NULL)
    {
      fontname=userfontname->Get();
      fontname.Add(__L(".ttf"));
    }

    XSTRING* uservalue=node->GetUserData()->GetUserDataByKey(__L("UserValue"));
    if (uservalue!=NULL)
    {
      textvalue=uservalue->Get();
    }

    XSTRING* usermask=node->GetUserData()->GetUserDataByKey(__L("UserMask"));
    if (usermask!=NULL)
    {
      textmask=usermask->Get();
      if (usermask->GetSize()==0)
        return NULL;
    }

    XSTRING* align=node->GetUserData()->GetUserDataByKey(__L("Align"));
    if (align!=NULL)
      textalign=align->ConvertToInt();

    XSTRING* materialname =node->GetUserData()->GetUserDataByKey(__L("Material"));
    XSTRING* color        =node->GetUserData()->GetUserDataByKey(__L("Color"));

    GRPFONT* font=GRPOBJECTBUILDER::Get()->GetFontManager()->GetFont(fontname.Get());
    if (font==NULL)
    {
        XDEBUG_PRINTCOLOR(4,__L("GRPOBJECT::CreateTextFromLocator: Unknown Font Name : %s"),fontname.Get());
        return NULL;
    }

    GRPMATERIAL* material=NULL;
    if (materialname!=NULL)
      material=GRPMATERIALLIBRARY::Get()->FindByName(materialname->Get());


  GRPTEXTELEMENT* text=new GRPTEXTELEMENT();
                  text->SetName               (node->GetName()->Get());
                  text->SetNode               (node);
                  text->SetFont               (font);
                  text->SetAlign              (textalign);
                  text->SetContent            (textvalue);
                  text->SetContentMask        (textmask);

                  if (material!=NULL)
                      text->GetNode()->SetMaterial(material);

                  node->animables.Add(text);

                  //---------- attach user animation


      /*
                  for (XDWORD e=0;e<this->GetAnimationControl()->GetStacks()->GetSize();e++)
                  if(GetAnimationControl()->GetStacks()->GetElement(e))
                  {
                  XVECTOR<GRPANIMATIONNODE*>* nodes=GetAnimationControl()->GetStacks()->GetElement(e)->GetLayer()->GetAnimationNodes();
                  for (XDWORD e=0;e<nodes->GetSize();e++)
                    {
                      if (nodes->Get(e)->GetTarget())
                      {
                          if (nodes->Get(e)->GetTarget()->GetName()->Compare(node->GetName()->Get())==0)
                          {
                            nodes->Get(e)->AttachTo(__L("value"),         text);
                            nodes->Get(e)->AttachTo(__L("DiffuseFactor"), text);
                          }
                      }
                    }
                  }

                  */
                  //--------------

                  this->AddText               (text);
                  this->AddElement            (text);
    return text;
}


/*-------------------------------------------------------------------
//  GRPOBJECT::Notify
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/01/2017 14:14:11
//
//  @return       bool :
//
//  @param        msg :
//  @param        notifier :
*/
/*-----------------------------------------------------------------*/
bool GRPOBJECT::Notify(GRPMSGTYPE msg,GRPSUBJECT* notifier)
{
  switch(msg)
  {
  case GRPMSG_DESTROYED:
      switch (notifier->GetSubjectType())
      {
          case GRPNAMED_TYPE_GRPCAMERA:
          case GRPNAMED_TYPE_GRPLIGHT   :
              this->lights.Delete(dynamic_cast<GRPLIGHT*>(notifier));
              this->cameras.Delete(dynamic_cast<GRPCAMERA*>(notifier));
              this->nodes.Delete(dynamic_cast<GRPNODE*>(notifier)); break;

          case GRPNAMED_TYPE_GRPNODE    :
              this->nodes.Delete(dynamic_cast<GRPNODE*>(notifier)); break;
      }
      break;

  case GRPMSG_DETTACHED:
    switch(notifier->GetSubjectType())
    {
    case GRPNAMED_TYPE_GRPELEMENT:
      {
      int n=this->elements.Find(static_cast<GRPELEMENT*>(notifier));
      if (n!=NOTFOUND)
        {
        this->elements.DeleteIndex(n);
        notifier->GetObservers()->Delete(this);
        }
      }
          break;
    }
  }
  return true;
}

/*-------------------------------------------------------------------
//  GRPOBJECT::UpdateBoundingBox
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      02/03/2017 13:08:00
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
void GRPOBJECT::UpdateBoundingBox()
{
  if (boundingbox.blocked)
    return;
  if (elements.GetSize()==0)
    return;
  bool initialized=false;

  for (XDWORD e=0;e<this->elements.GetSize();e++)
  {
    GRPMESH* mesh=elements.Get(e)->GetMesh();
    if (mesh)
    {
      elements.Get(e)->UpdateBoundingBox();
      GRPBB* bb=elements.Get(e)->GetBoundingBox();
      if (!initialized)
      {
        this->boundingbox.minpoint.x=(bb->minpoint.x*elements.Get(e)->GetNode()->GetScale()->vector[0])+elements.Get(e)->GetNode()->GetPosition()->vector[0];
        this->boundingbox.minpoint.y=(bb->minpoint.y*elements.Get(e)->GetNode()->GetScale()->vector[1])+elements.Get(e)->GetNode()->GetPosition()->vector[1];
        this->boundingbox.maxpoint.x=(bb->maxpoint.x*elements.Get(e)->GetNode()->GetScale()->vector[0])+elements.Get(e)->GetNode()->GetPosition()->vector[0];
        this->boundingbox.maxpoint.y=(bb->maxpoint.y*elements.Get(e)->GetNode()->GetScale()->vector[1])+elements.Get(e)->GetNode()->GetPosition()->vector[1];
        initialized=true;
      }
      else
      {
        if (this->boundingbox.minpoint.x < (bb->minpoint.x*elements.Get(e)->GetNode()->GetScale()->vector[0])+elements.Get(e)->GetNode()->GetPosition()->vector[0])
            this->boundingbox.minpoint.x = (bb->minpoint.x*elements.Get(e)->GetNode()->GetScale()->vector[0])+elements.Get(e)->GetNode()->GetPosition()->vector[0];

        if (this->boundingbox.minpoint.y < (bb->minpoint.y*elements.Get(e)->GetNode()->GetScale()->vector[1])+elements.Get(e)->GetNode()->GetPosition()->vector[1])
            this->boundingbox.minpoint.y = (bb->minpoint.y*elements.Get(e)->GetNode()->GetScale()->vector[1])+elements.Get(e)->GetNode()->GetPosition()->vector[1];

        if (this->boundingbox.maxpoint.x > (bb->maxpoint.x*elements.Get(e)->GetNode()->GetScale()->vector[0])+elements.Get(e)->GetNode()->GetPosition()->vector[0])
            this->boundingbox.maxpoint.x = (bb->maxpoint.x*elements.Get(e)->GetNode()->GetScale()->vector[0])+elements.Get(e)->GetNode()->GetPosition()->vector[0];

        if (this->boundingbox.maxpoint.y > (bb->maxpoint.y*elements.Get(e)->GetNode()->GetScale()->vector[1])+elements.Get(e)->GetNode()->GetPosition()->vector[1])
            this->boundingbox.maxpoint.y = (bb->maxpoint.y*elements.Get(e)->GetNode()->GetScale()->vector[1])+elements.Get(e)->GetNode()->GetPosition()->vector[1];
      }
    }
  }

  if (initialized)
  {
    boundingbox.width =boundingbox.maxpoint.x-boundingbox.minpoint.x;
    boundingbox.height=boundingbox.maxpoint.y-boundingbox.minpoint.y;
    boundingbox.depth =boundingbox.maxpoint.z-boundingbox.minpoint.z;
  }
  else
  {
    boundingbox.width =0;
    boundingbox.height=0;
    boundingbox.depth =0;
  }

}

/*-------------------------------------------------------------------
//  GRPSCENE::FindNode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/05/2015 11:27:52
//
//  @return       GRPNODE* :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPNODE*  GRPOBJECT::FindNode                       (XCHAR* name)
{
  int n=(int)this->nodes.GetSize();
  for (int e=n-1;e>=0;e--)
  {
    XSTRING* nodename=this->nodes.FastGet(e)->GetName();
    if (nodename->Compare(name)==0)
      return this->nodes.FastGet(e);
  }
  return NULL;
}
/*-------------------------------------------------------------------
//  GRPSCENE::FindElementByNodeName
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/10/2015 16:35:06
//
//  @return       GRPELEMENT* :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* GRPOBJECT::FindElementByNodeName            (XCHAR* name)
{
    int nElements=GetElements()->GetSize();
    for (int e=nElements-1;e>=0;e--)
    {
      GRPELEMENT* element=GetElements()->FastGet(e);
      if (element->GetNode()!=NULL)
      {
      XSTRING* nodename=element->GetNode()->GetName();
      if (nodename->Compare(name)==0)
        return element;
      }
    }
  return NULL;
}


/*-------------------------------------------------------------------
//  GRPOBJECT::FindElement
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/11/2015 12:26:51
//
//  @return       GRPELEMENT* :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*   GRPOBJECT::FindElement                      (XCHAR* name)
{
      int nElements=this->GetElements()->GetSize();
      for (int i=nElements-1;i>=0;i--)
      {
        GRPELEMENT* element=this->GetElements()->FastGet(i);
        if (element->GetName()->Compare(name)==0)
          return element;
      }
  return NULL;
}
/*-------------------------------------------------------------------
//  GRPOBJECT::FindCamera
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/05/2015 11:28:04
//
//  @return        :
//
//  @param        name :
//-------------------------------------------------------------------
*/
GRPCAMERA*  GRPOBJECT::FindCamera                   (XCHAR* name)
{
  int nCameras=this->cameras.GetSize();
  for (int e=nCameras-1;e>=0;e--)
    if (this->cameras.FastGet(e)->GetName()->Compare(name)==0)
      return this->cameras.FastGet(e);

  return NULL;
}


/*-------------------------------------------------------------------
//  GRPSCENE::FindLight
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/05/2015 11:28:35
//
//  @return       GRPLIGHT* :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPLIGHT* GRPOBJECT::FindLight  (XCHAR* name)
{
  int nLights=this->lights.GetSize();
  for (int e=nLights-1;e>=0;e--)
    if (this->lights.FastGet(e)->GetName()->Compare(name)==0)
      return this->lights.FastGet(e);
  return NULL;
}


/*-------------------------------------------------------------------
//  GRPOBJECT::FindTextBox
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/09/2016 10:14:28
//
//  @return       GRPTEXTELEMENT* :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPTEXTELEMENT* GRPOBJECT::FindTextBox (XCHAR* name)
{
  int nText=this->textboxes.GetSize();
  for (int e=nText-1;e>=0;e--)
    if (this->textboxes.FastGet(e)->GetName()->Compare(name)==0)
      return this->textboxes.FastGet(e);
  return NULL;
}