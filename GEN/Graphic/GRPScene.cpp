
/*------------------------------------------------------------------------------------------
//  GRPSCENE.CPP
//
//  Scene wrapper
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 11/07/2014 13:18:14
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XTimer.h"

#include "GRPLight.h"
#include "GRPCamera.h"
#include "GRPObject.h"
#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "GRPViewport.h"
#include "GRPShaderLibrary.h"
//#include "GRPSprite.h"
#include "GRPScreen.h"
#include "GRPPath.h"
#include "GRPBufferOpenGLRender.h"
#include "GRPVisualDebugger.h"
#include "GRPTextBox.h"
#include "GRPMaterial.h"
#include "GRPProfiler.h"
//#include "GRPParticleSystem.h"
#include "GRPRenderer.h"
#include "GRPRenderTarget.h"
#include "GRPScene.h"
#include "GRPPassManager.h"


#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

GRPSCENE::GRPSCENE()
{
  Clean();
  visualdebugger=new GRPVISUALDEBUGGER(this);

  RegisterEvent(GRPXEVENTTYPE_SCENE_INI);
  RegisterEvent(GRPXEVENTTYPE_SCENE_END);
  RegisterEvent(GRPXEVENTTYPE_SCENE_FRAMEINI);
  RegisterEvent(GRPXEVENTTYPE_SCENE_FRAMEEND);
  RegisterEvent(GRPXEVENTTYPE_SCENE_ENTITYINI);
  RegisterEvent(GRPXEVENTTYPE_SCENE_ENTITYEND);
  RegisterEvent(GRPXEVENTTYPE_SCENE_INPUT);

  this->nodes.SetIsMulti(false);


  lock=xfactory->Create_Mutex();

}

/*-------------------------------------------------------------------
//  GRPSCENE::GRPSCENE
*/
/**
//
//  Class Constructor GRPSCENE
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/07/2014 13:25:32
//

//  @param        mainscreen :
*/
/*-----------------------------------------------------------------*/
GRPSCENE::GRPSCENE(GRPSCREEN* screen)
{
  Clean();
  visualdebugger=new GRPVISUALDEBUGGER(this);

  this->screen      = screen;

  RegisterEvent(GRPXEVENTTYPE_SCENE_INI);
  RegisterEvent(GRPXEVENTTYPE_SCENE_END);
  RegisterEvent(GRPXEVENTTYPE_SCENE_FRAMEINI);
  RegisterEvent(GRPXEVENTTYPE_SCENE_FRAMEEND);
  RegisterEvent(GRPXEVENTTYPE_SCENE_ENTITYINI);
  RegisterEvent(GRPXEVENTTYPE_SCENE_ENTITYEND);
  RegisterEvent(GRPXEVENTTYPE_SCENE_INPUT);

  this->nodes.SetIsMulti(false);


  lock=xfactory->Create_Mutex();

}




/*-------------------------------------------------------------------
//  GRPSCENE::~GRPSCENE
*/
/**
//
//   Class Destructor GRPSCENE
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/07/2014 13:26:22
//
*/
/*-----------------------------------------------------------------*/
GRPSCENE::~GRPSCENE()
{
  DeRegisterEvent(GRPXEVENTTYPE_SCENE_FRAMEINI);
  DeRegisterEvent(GRPXEVENTTYPE_SCENE_FRAMEEND);
  DeRegisterEvent(GRPXEVENTTYPE_SCENE_ENTITYINI);
  DeRegisterEvent(GRPXEVENTTYPE_SCENE_ENTITYEND);
  DeRegisterEvent(GRPXEVENTTYPE_SCENE_INI);
  DeRegisterEvent(GRPXEVENTTYPE_SCENE_END);
  DeRegisterEvent(GRPXEVENTTYPE_SCENE_INPUT);

  End();

  if (LightMatrices)          delete [] LightMatrices;        LightMatrices     =NULL;
  if (LightColorMatrices)     delete [] LightColorMatrices;   LightColorMatrices=NULL;
  if (visualdebugger)         delete visualdebugger;          visualdebugger    =NULL;

  if (lock)                   xfactory->Delete_Mutex(lock);   lock=NULL;

  Clean();
}


/*-------------------------------------------------------------------
//  GRPSCENE::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/10/2014 10:46:34
//
//  @return       bool :
//
//  @param        obj :
*/
/*-----------------------------------------------------------------*/
bool GRPSCENE::Add(GRPOBJECT* obj, bool checkshaders)
{
  if (obj == NULL) return false;

  lock->Lock();

  if (obj->Notify(GRPMSG_ADDED,this))
  {
    int nelements=this->GetPass()->GetRenderOrder()->GetSize();
    if (this->objects.Find(obj)==NOTFOUND)
      {
        GetObjects()->Add(obj);

        for (XDWORD e=0;e<obj->GetCameras()->GetSize();e++)
          this->Add(obj->GetCameras()->Get(e));

        for (XDWORD e=0;e<obj->GetLights()->GetSize();e++)
          this->Add(obj->GetLights()->Get(e));

        for (XDWORD e=0;e<obj->GetNodes()->GetSize();e++)
          this->Add(obj->GetNodes()->Get(e));
      }

       nelements=this->GetPass()->GetRenderOrder()->GetSize();
       nelements=obj->GetElements()->GetSize();

       if (!this->lights.GetSize())
           GRPSETTINGS::IsLightingEnabled = false;
       else
           GRPSETTINGS::IsLightingEnabled = true;

    for (int e=0;e<nelements;e++)
      {
        GRPELEMENT* element=obj->GetElements()->FastGet(e);

        // need to find if the element is already in the renderobject before doing anything
        if(pass->GetRenderOrder()->Find(element) == NOTFOUND)
          {
            element->AddObserver(this);
            //if (checkshaders)
            //if (element->GetProgram()==NULL)
            //element->SetProgram(element->ChooseShader());
            pass->Add(element);
          }
        else
        {
          continue; //Strange..
        }
      }
    if (obj->GetCurrentCamera() != -1)
    {
        this->SelectCamera(obj->GetCamera());
        this->GetCamera()->Update();
    }
    //this->InitializeLightning();
  }

  lock->UnLock();
  return true;
}



/*-------------------------------------------------------------------
//  GRPSCENE::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/10/2015 10:21:46
//
//  @return       bool :
//
//  @param        GRPMATERIAL*mat :
*/
/*-----------------------------------------------------------------*/
bool  GRPSCENE::Add (GRPMATERIAL*mat)
{
  return GRPMATERIALLIBRARY::Get()->Add(mat);
}



/*-------------------------------------------------------------------
//  GRPSCENE::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/10/2015 10:21:46
//
//  @return       bool :
//
//  @param        GRPMATERIAL*mat :
*/
/*-----------------------------------------------------------------*/
bool  GRPSCENE::Add (GRPELEMENT* element)
{
  this->mainpass->Add(element);
  return true;
}

/*-------------------------------------------------------------------
//  GRPSCENE::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/06/2016 23:46:43
//
//  @return       bool :
//
//  @param        n :
*/
/*-----------------------------------------------------------------*/
bool  GRPSCENE::Add(GRPLIGHT* n)
{
    bool success=GetLights()->Add(n);
    if (success)
    {
      if (n->GetType()==GRPLIGHTTYPE_SPOTLIGHT)
        this->Add((GRPCAMERA*)n);
    }
    return success;
}


/*-------------------------------------------------------------------
//  GRPSCENE::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/08/2016 11:48:05
//
//  @return       bool :
//
//  @param        pass :
//  @param        copyRenderOrder :
*/
/*-----------------------------------------------------------------*/
bool  GRPSCENE::Add(GRPPASS* pass, bool copyRenderOrder)
{
    this->Lock();
  bool status=GetPasses()->Add(pass);

  if (status)
  {
                            pass->SetScene      (this);
      if (copyRenderOrder)  pass->SetClone      (copyRenderOrder);
      if (pass->GetClone()) pass->SetRenderOrder(this->GetPass()->GetRenderOrder());
                            pass->Notify        (GRPMSG_ADDED,  this);
  }

  this->UnLock();
  return status;
}


/*-------------------------------------------------------------------
//  GRPSCENE::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/09/2016 13:23:48
//
//  @return       bool :
//
//  @param        cam :
*/
/*-----------------------------------------------------------------*/
bool  GRPSCENE::Add(GRPCAMERA* cam)
{
  if (GetCameras()->Add(cam))
  {
    cam->AddObserver(this);
    return true;
  }
  return false;
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
GRPNODE*  GRPSCENE::FindNode                        (XCHAR* name)
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
GRPELEMENT* GRPSCENE::FindElementByNodeName           (XCHAR* name)
{
  int nObjects=this->objects.GetSize();

  for (int o=nObjects-1;o>=0;o--)
  {
    GRPOBJECT* obj=this->objects.FastGet(o);
    int nElements=obj->GetElements()->GetSize();
    for (int e=nElements-1;e>=0;e--)
    {
      GRPELEMENT* element=obj->GetElements()->FastGet(e);
      if (element->GetNode()!=NULL)
      {
      XSTRING* nodename=element->GetNode()->GetName();
      if (nodename->Compare(name)==0)
        return element;
      }
    }
  }
  return NULL;
}
/*-------------------------------------------------------------------
//  GRPSCENE::FindObject
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/05/2015 11:27:57
//
//  @return       GRPOBJECT* :
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPOBJECT*  GRPSCENE::FindObject                    (XCHAR* name)
{
  int nObjects=this->objects.GetSize();
  for (int e=nObjects-1;e>=0;e--)
    if (this->objects.FastGet(e)->GetNode()->GetName()->Compare(name)==0)
      return this->objects.FastGet(e);
  return NULL;
}

/*-------------------------------------------------------------------
//  GRPSCENE::FindElement
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
GRPELEMENT*   GRPSCENE::FindElement                     (XCHAR* name)
{
    int nObjects=this->objects.GetSize();
    for (int e=nObjects-1;e>=0;e--)
    {
      GRPOBJECT* obj=this->objects.FastGet(e);
      int nElements=obj->GetElements()->GetSize();
      for (int i=nElements-1;i>=0;i--)
      {
        GRPELEMENT* element=obj->GetElements()->FastGet(i);
        if (element->GetName()->Compare(name)==0)
          return element;
      }
    }
  return NULL;
}
/*-------------------------------------------------------------------
//  GRPSCENE::FindCamera
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
GRPCAMERA*  GRPSCENE::FindCamera                    (XCHAR* name)
{
  int nCameras=this->cameras.GetSize();
  for (int e=nCameras-1;e>=0;e--)
    if (this->cameras.FastGet(e)->GetName()->Compare(name)==0)
      return this->cameras.FastGet(e);
  return NULL;
}

/*-------------------------------------------------------------------
//   GRPSCENE::FindViewport
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  24/10/2017 16:43:21
//  @return   GRPVIEWPORT* :
//
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
GRPVIEWPORT*  GRPSCENE::FindViewport                (XCHAR* name)
{
    int nViewports = this->viewports.GetSize();
    for (int e = nViewports - 1; e >= 0; e--)
        if (this->viewports.FastGet(e)->GetName()->Compare(name) == 0)
            return this->viewports.FastGet(e);
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
GRPLIGHT* GRPSCENE::FindLight                   (XCHAR* name)
{
  int nLights=this->lights.GetSize();
  for (int e=nLights-1;e>=0;e--)
    if (this->lights.FastGet(e)->GetName()->Compare(name)==0)
      return this->lights.FastGet(e);
  return NULL;
}

/*-------------------------------------------------------------------
//  GRPSCENE::OrderScene
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/04/2015 11:06:30
//
*/
/*-----------------------------------------------------------------*/
void GRPSCENE::PrepareScene()
{
  GRPELEMENT* BackGround=NULL;

    PrepareShaders();
    //ReorderScene  ();

    for (int j=this->GetNodes()->GetSize()-1;j>=0;j--)
      this->GetNodes()->Get(j)->modified=true;

    InitializeLightning   ();

    for (XDWORD e=0;e<cameras.GetSize();e++)
    {
        GRPCAMERA*  cam=cameras.Get(e);

                    cam->positionChanged=true;
                    cam->scaleChanged   =true;
                    cam->rotationChanged=true;
    }

    this->Publish(GRPMSG_INIT);

}


/*-------------------------------------------------------------------
//  GRPSCENE::PrepareShaders
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      31/08/2016 10:28:30
//
*/
/*-----------------------------------------------------------------*/
void  GRPSCENE::PrepareShaders()
{
    GRPSHADERPROGRAM* textureprogram=GRPSHADERLIBRARY::Instance->GetProgramByKey((char*)"DOT");
    GRPSHADERPROGRAM* diffuseprogram=GRPSHADERLIBRARY::Instance->GetProgramByKey((char*)"DDF");

    SelectPass(mainpass);
    for (XDWORD e=0;e<this->mainpass->GetRenderOrder()->GetSize();e++)
    {
      if (mainpass->GetRenderOrder()->Get(e)->GetProgram()==NULL)
      {
        GRPDRAWABLE* drawable=mainpass->GetRenderOrder()->Get(e);

        GRPELEMENT* element=dynamic_cast<GRPELEMENT*>(drawable);

        drawable->SetProgram(textureprogram);

        if (element)
          if (element->GetNode())
            if (!element->GetNode()->GetMaterial() || element->GetNode()->GetMaterial()->GetLayer(GRPTEXTURELAYERTYPE_DIFFUSE)==NULL)
              drawable->SetProgram(diffuseprogram);
      }
    }


}


/*-------------------------------------------------------------------
//  GRPSCENE::ReorderScene
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      31/08/2016 10:29:25
//
*/
/*-----------------------------------------------------------------*/
void  GRPSCENE::ReorderScene()
{
    this->Publish(GRPMSG_ORDER);

    this->pass->GetRenderOrder()->DeleteAll();

    for (XDWORD e=0;e<this->GetObjects()->GetSize();e++)
    {
      for (XDWORD i=0;i<this->GetObjects()->Get(e)->GetElements()->GetSize();i++)
      {
        GRPELEMENT* element=this->GetObjects()->Get(e)->GetElements()->Get(i);
        this->pass->Add(element);
        /*
          for (XDWORD e=0;e<this->Passes.GetSize();e++)
          {
            if    (this->Passes.Get(e)->Remove(element))
                  this->Passes.Get(e)->Add(element);
          }
          **/
      }
    }
}




/*-------------------------------------------------------------------
//  GRPSCENE::RenderFrame
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/07/2014 13:20:25
//
*/
/*-----------------------------------------------------------------*/
void  GRPSCENE::RenderFrame()
{
  OPENGLCHECKERROR(__L("RenderFrame"));

  if (!mainpass->GetRenderOrder())  return;

  int n=(int)mainpass->GetRenderOrder()->GetSize();
  if (n==0) return;

  GRPPROFILER::nElements  += n;

  XLISTDATA<GRPELEMENT*>* current=mainpass->GetRenderOrder()->GetFirstData();//pass->GetRenderOrder()->GetLastData();

  for (int e=n;e>0;e--)
  {
    current->data->ComputeMatrices(mainpass->viewmatrix,&mainpass->viewprojection,mainpass->projectionmatrix);
    current=current->next;
  }

   current=mainpass->GetRenderOrder()->GetFirstData();//pass->GetRenderOrder()->GetLastData();

  for (int e=n;e>0;e--)
  {
    current->data->Draw(mainpass->viewmatrix,&mainpass->viewprojection,mainpass->projectionmatrix);
    current=current->next;
  }

}

/*-------------------------------------------------------------------
//  GRPSCENE::SetCurrentCamera
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/02/2015 11:45:17
//
//  @return       GRPCAMERA* :
//
//  @param        i :
*/
/*-----------------------------------------------------------------*/
GRPCAMERA*  GRPSCENE::SetCurrentCamera(XDWORD i)
{
  if (i<this->GetCameras()->GetSize())
  CurrentCamera=i;
  else
      CurrentCamera=0;


  GRPCAMERA*  camera=this->GetCameras()->Get(CurrentCamera);

  if (camera)
  {
      camera->UpdateFov();
      GRPVIEWPORT* viewport=this->viewports.Get(CurrentViewport);

      if (viewport)
      {
        viewport->SetFieldOfView(camera->fov);
        viewport->AttachCam     (i);
      }

      pass->SetViewport (viewport);
      pass->SetCamera   (camera);
      camera->modified = true;
      camera->rotationChanged = true;
      camera->positionChanged = true;
      camera->scaleChanged = true;
      camera->Update();
      camera->BuildLookAtMatrix();
  }


  return camera;
}

/*-------------------------------------------------------------------
//  GRPSCENE::SelectCamera
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/09/2016 17:01:11
//
//  @return       GRPCAMERA* :
//
//  @param        i :
*/
/*-----------------------------------------------------------------*/
GRPCAMERA*  GRPSCENE::SelectCamera(GRPCAMERA* i)
{
  XDWORD index=-1;
  if ((index=this->cameras.Find(i))!=NOTFOUND)
  {
    this->SetCurrentCamera(index);
  }
  return this->GetCamera();
}

/*-------------------------------------------------------------------
//  GRPSCENE::SetCurrentViewport
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/06/2016 10:38:22
//
//  @return       GRPVIEWPORT* :
//
//  @param        i :
*/
/*-----------------------------------------------------------------*/
GRPVIEWPORT*  GRPSCENE::SetCurrentViewport(XDWORD i)
{
  if (i<this->GetViewports()->GetSize())
  CurrentViewport=i;
  else
      CurrentViewport=0;

  return this->GetViewports()->Get(i);
}

/*-------------------------------------------------------------------
//  GRPSCENE::CreateNode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/05/2015 17:20:05
//
//  @return       GRPNODE* :
//
*/
/*-----------------------------------------------------------------*/
GRPNODE* GRPSCENE::CreateNode                     ()
{
  lock->Lock();
  GRPNODE* node = new GRPNODE();
  if (node)
  if (nodes.Add(node))
  {
    node->AddObserver(this);
    node->managed = true;
    node->Init();
  }
  lock->UnLock();
  return node;
}

/*-------------------------------------------------------------------
//  GRPSCENE::CreateNode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/05/2015 17:20:14
//
//  @return       GRPNODE* :
//
//  @param        p :
*/
/*-----------------------------------------------------------------*/
GRPNODE* GRPSCENE::CreateNode                     (GRPVECTOR* p)
{
  return CreateNode(p->vector[0],p->vector[1],p->vector[2]);
}

/*-------------------------------------------------------------------
//  GRPSCENE::CreateNode
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/05/2015 17:20:24
//
//  @return       GRPNODE* :
//
//  @param        x :
//  @param        y :
//  @param        z :
*/
/*-----------------------------------------------------------------*/
GRPNODE* GRPSCENE::CreateNode                     (GLFLOAT x,GLFLOAT y, GLFLOAT z)
{
  GRPNODE* node = CreateNode();

  node->SetPosition(x,y,z);

  if(!node)
    return NULL;

    node->Init();

  return node;
}


/*-------------------------------------------------------------------
//  GRPSCENE::CreateCamera
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/07/2014 13:19:47
//
//  @return        :
//
*/
/*-----------------------------------------------------------------*/
GRPCAMERA* GRPSCENE::CreateCamera()
{
  GRPCAMERA* camera = new GRPCAMERA();
  if(!camera) return NULL;

  camera->viewmatrix.LoadIdentity();
  camera->BuildLookAtMatrix();

  if(!this->Add(camera))  return NULL;

  return camera;
}


/*-------------------------------------------------------------------
//  GRPSCENE::CreateLight
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/07/2014 13:19:47
//
//  @return        :
//
*/
/*-----------------------------------------------------------------*/
GRPLIGHT* GRPSCENE::CreateLight()
{
  GRPLIGHT* light = new GRPLIGHT();
  if(!light) return NULL;

  light->BuildData();

  if(!lights.Add(light))  return NULL;
  if(!nodes.Add(light))   return NULL;

  return light;
}

/*-------------------------------------------------------------------
//  GRPSCENE::CreateRenderTarget
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/06/2016 12:20:59
//
//  @return       GRPRENDERTARGET* :
//
*/
/*-----------------------------------------------------------------*/
GRPRENDERTARGET* GRPSCENE::CreateRenderTarget(XDWORD w,XDWORD h)
{
  GRPRENDERTARGET* target=new GRPRENDERTARGET(w,h);
  if (target)
        this->Add(target);

  return target;
}


/*-------------------------------------------------------------------
//  GRPSCENE::CreateViewport
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      02/06/2016 13:29:21
//
//  @return       GRPVIEWPORT* :
//
*/
/*-----------------------------------------------------------------*/
GRPVIEWPORT* GRPSCENE::CreateViewport()
{
  GRPVIEWPORT* viewport=new GRPVIEWPORT();
  if (viewport)
  {
    this->viewports.Add(viewport);
  }
  return viewport;
}


/*-------------------------------------------------------------------
//  GRPSCENE::CreatePass
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/07/2016 14:43:48
//
//  @return       GRPPASS* :
//
*/
/*-----------------------------------------------------------------*/
GRPPASS* GRPSCENE::CreatePass(XCHAR* name,int priority, bool addToPasses)
{
  GRPPASS* pass=GRPPASSMANAGER::Get()->Create(name);
  if (pass)
  {
    pass->priority=priority;
    if (addToPasses)
      this->Passes.Add(pass);
    pass->SetScene(this);
  }
  return pass;
}



/*-------------------------------------------------------------------
//  GRPSCENE::InitializeLightning
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/05/2015 12:14:03
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  GRPSCENE::InitializeLightning             ()
{

  if (this->GetLights()->GetSize()==0) return true;

  if (LightMatrices!=NULL)
    delete [] LightMatrices;

    LightMatrices=(GLFLOAT*)new GLFLOAT[GRPSHADER::MaxLights*16];
    if (LightMatrices==NULL)      return false;

    memset(LightMatrices,0,GRPSHADER::MaxLights*16*sizeof(GLFLOAT));

    if (LightColorMatrices!=NULL)
    delete [] LightColorMatrices;

    LightColorMatrices=(GLFLOAT*)new GLFLOAT[GRPSHADER::MaxLights*16];
    if (LightColorMatrices==NULL)     return false;

    memset(LightColorMatrices,0,GRPSHADER::MaxLights*16*sizeof(GLFLOAT));

    for (XDWORD e=0;e<this->GetLights()->GetSize();e++)
      GetLights()->Get(e)->BuildData();

  return true;
}

/*-------------------------------------------------------------------
//  GRPSCENE::PrepareLightning
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/05/2015 11:08:07
//
*/
/*-----------------------------------------------------------------*/
bool  GRPSCENE::PrepareLightning                ()
{
    OPENGLCHECKERROR(__L("PrepareLightning"));

  XDWORD nLights=this->GetLights()->GetSize();
  if (nLights==0) return true;
  if (nLights>GRPSHADER::MaxLights)
    nLights=GRPSHADER::MaxLights;

  if (LightMatrices==NULL)
    InitializeLightning();

  memset(LightMatrices,0,sizeof(GLFLOAT)*16*GRPSHADER::MaxLights);

  XDWORD e=0;
  for (;e<nLights;e++)
  {
    GRPLIGHT*   l=lights.Get(e);
                l->Update(pass->viewmatrix);

    GRPMATRIX*  lm=l->GetLightDataMatrix();
                lm->CopyTo(&(LightMatrices[e*16]));
  }


  memset(LightColorMatrices,0,sizeof(GLFLOAT)*16*GRPSHADER::MaxLights);

  for (e=0;e<nLights;e++)
  {
    GRPMATRIX* lm=lights.Get(e)->GetLightColorMatrix();
    lm->CopyTo(&(LightColorMatrices[e*16]));
  }


  XDWORD nProgs=GRPSHADERLIBRARY::Get()->GetCompiledPrograms()->GetSize();

  for (int e=nProgs-1;e>=0;e--)
  {
    GRPSHADERPROGRAM* shader=GRPSHADERLIBRARY::Get()->GetCompiledPrograms()->Get(e);
    if (shader)
    {
      if (shader->iLightDataMatrix!=-1)
      {
        XDWORD id=shader->GetID();
        GRPSHADER::UseShader(id);
        glUniformMatrix4fv(shader->iLightDataMatrix,  (GLsizei)nLights,   GL_FALSE, (const GLFLOAT*)&(LightMatrices[0]));

        if (shader->iLightColorMatrix!=-1)
          glUniformMatrix4fv(shader->iLightColorMatrix, (GLsizei)nLights, GL_FALSE, (const GLFLOAT*)&(LightColorMatrices[0]));

          OPENGLCHECKERROR(__L("LightMatrix"));
      }

      if (shader->iLightSpaceMatrix!=-1 && lights.Get(0)->depthtexture)
      {
          XDWORD id=shader->GetID();
          GRPSHADER::UseShader(id);
          OPENGLCHECKERROR(__L("shader->iLightSpaceMatrix"));
          glUniformMatrix4fv(shader->iLightSpaceMatrix, (GLsizei)1, GL_FALSE, (const GLFLOAT*)&(lights.Get(0)->lightSpaceMatrix.matrix[0][0]));

          if (lights.Get(0))
              if (lights.Get(0)->depthtexture)
              {
                  GRPOPENGLCTRL::TEXTURE::SetCurrentTexture(GRPTEXTURELAYERTYPE_DEPTH);
                  lights.Get(0)->depthtexture->Bind(GRPTEXTURELAYERTYPE_DEPTH);

                  if (shader->itextureSize5)
                  glUniform2f(shader->itextureSize5, (GLFLOAT)lights.Get(0)->depthtexture->GetWidth(), (GLFLOAT)lights.Get(0)->depthtexture->GetHeight());
              }
        /// glBindTexture(lights.Get(0)->depthtexture->GetIsCubeMap() ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D,lights.Get(0)->texturehandle);

          OPENGLCHECKERROR(__L("Light glBindTexture"));

          GLUINT pos=glGetUniformLocation(id,"bias");
          if (pos!=-1)
          glUniform1f(pos,lights.Get(0)->bias);


        OPENGLCHECKERROR(__L("LightMatrix"));
      }
    }
  }

  return true;
}

/*-------------------------------------------------------------------
//  GRPSCENE::Ini
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/11/2014 10:03:49
//
*/
/*-----------------------------------------------------------------*/
bool  GRPSCENE::Ini (XDWORD flags)
{
  OPENGLCHECKERROR(__L("GRPSCENE::Ini"));
  XDEBUG_PRINTCOLOR(1,__L("GRPSCENE::CreateViewport"));

  //visualdebugger  = new GRPVISUALDEBUGGER(this);          if (!visualdebugger)  return false;

  //------------------------------------------------------ Clear previous state
  this->End();
  OPENGLCHECKERROR(__L("GRPSCENE::End"));

  if (flags & GRPSCENE_INIT_FLAGS_CREATE_PASS)
  {
            mainpass=CreatePass(__L("main"));
            //mainpass->SetClear(GRPPASS_CLEAR_FLAG_BOTH);

            SelectPass(mainpass);
  }
  else
    if (this->Passes.GetSize())
      this->SelectPass(this->Passes.Get(0));

  OPENGLCHECKERROR(__L("GRPSCENE::Pass"));

    //------------------------------------------------------ Init Default Viewport
  if (flags & GRPSCENE_INIT_FLAGS_CREATE_VIEWPORT)
  {
  GRPVIEWPORT* viewport = new GRPVIEWPORT();
  if(!viewport) return false;

          viewport->SetProjection (GRPPROJECTIONTYPE_FRUSTUM);
          viewport->SetPosition   (0.0f, 0.0f);

          screen->AddViewport     (viewport);
          viewport->Select        ();

  this->GetViewports()->Add         (viewport);
  this->mainpass->SetViewport(viewport);
  }
  else
    if (this->viewports.GetSize()>0)
      this->mainpass->SetViewport(this->viewports.Get(0));

  OPENGLCHECKERROR(__L("GRPSCENE::Viewports"));

  //------------------------------------------------------ Shader loading
  if (!GRPSHADERLIBRARY::Get()) return false;

  //------------------------------------------------------ Ajustamos la cámara para mirar hacia el suelo
  if (flags & GRPSCENE_INIT_FLAGS_CREATE_CAMERA)
  if (this->GetCameras()->GetSize()==0)
  {
  GRPCAMERA*  defaultCamera=this->CreateCamera();
              defaultCamera->SetTarget                (this->CreateNode(+0.01f,   +0.01f,   +0.01f));
              defaultCamera->GetTarget()->SetName     ( __L("Default Camera Target"));
              defaultCamera->SetPosition              (-153.093f,   +125.0f,    -153.093f);
              defaultCamera->SetName                  ( __L("Default Camera Node"));
              defaultCamera->BuildLookAtMatrix  ();
              mainpass->SetCamera(defaultCamera);

  this->SetCurrentCamera(0);
  XDEBUG_PRINTCOLOR(0,__L("GRPSCENE::Camera init"));
  }

  OPENGLCHECKERROR(__L("SCREENRENDER::Ini"));

  //------------------------------------------------------DepthPassShader

  return true;

}




/*-------------------------------------------------------------------
//  GRPSCENE::Render
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/07/2014 13:19:55
//
//  @return        :
//
*/
/*-----------------------------------------------------------------*/
void GRPSCENE::Render()
{
  lock->Lock();

  GRPPROFILER::Scenes++;
  OPENGLCHECKERROR(__L("GRPSCENE::Render"));

  Publish(GRPMSG_STEP);

  GRPOPENGLCTRL::DEPTH::SetDepthTest          (false);              OPENGLCHECKERROR(__L("GRPSCENE::Render"));
  GRPOPENGLCTRL::DEPTH::SetDepthWriting       (false);              OPENGLCHECKERROR(__L("GRPSCENE::Render"));
  GRPOPENGLCTRL::DEPTH::SetDepthFunc          (GRPDEPTHFUNC_LESS);  OPENGLCHECKERROR(__L("GRPSCENE::Render"));
  GRPOPENGLCTRL::CULLING::SetBackFaceCulling  (true);               OPENGLCHECKERROR(__L("GRPSCENE::Render"));
  GRPOPENGLCTRL::COLOR::SetColorWriting       (false);              OPENGLCHECKERROR(__L("GRPSCENE::Render"));
  GRPSHADER::UseShader                        ((GLUINT)0);          OPENGLCHECKERROR(__L("GRPSCENE::Render"));

  //---------------------- update profiler
  int nnodes=this->GetNodes()->GetSize();
  GRPPROFILER::nNodes     += nnodes;

  //--------------------- update animation

  XDWORD nObjects = this->objects.GetSize();
  for (XDWORD e=0;e<nObjects;e++)
    this->objects.FastGet(e)->GetAnimationControl()->Update();

  //-------------------- update nodes

  for (XDWORD j=nnodes;j>0;j--)
    this->nodes.FastGet(j-1)->Update();

  for (XDWORD j=nnodes;j>0;j--)
    this->nodes.FastGet(j-1)->UpdateWorldMatrix();

  //-------------------- draw passes
  XLISTDATA<GRPPASS*>* passitem = this->Passes.GetFirstData();
  while (passitem)
  {
    pass      = passitem->data;
    passitem = passitem->next;

    if (!pass->Draw())
    {
        this->Passes.Delete(pass);
        break;
    }
    GRPPROFILER::Passes++;
  }

  pass=mainpass;
  //*/
  lock->UnLock();
}

/*-------------------------------------------------------------------
//  GRPSCENE::SetDebug
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/03/2015 17:53:28
//
*/
/*-----------------------------------------------------------------*/
void GRPSCENE::SetDebug()
{
  ShowDebug(GRPDEBUG_FLAG_NORMAL);
}


/*-------------------------------------------------------------------
//  GRPSCENE::ShowDebug
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/03/2015 11:00:36
//
//  @param        b :
*/
/*-----------------------------------------------------------------*/
void  GRPSCENE::ShowDebug(GRPDEBUG_FLAG b)
{

  for (XDWORD e=0;e<objects.GetSize();e++)
    for (XDWORD i=0;i<objects.Get(e)->GetElements()->GetSize();i++)
    {
      if (b & GRPDEBUG_FLAG_NORMAL)
      objects.Get(e)->GetElements()->Get(i)->SetVisible(true);
      else
      objects.Get(e)->GetElements()->Get(i)->SetVisible(false);
    }

  for (XDWORD e=0;e<debugObjects.GetSize();e++)
    for (XDWORD i=0;i<debugObjects.Get(e)->GetElements()->GetSize();i++)
    {
      if (b & GRPDEBUG_FLAG_DEBUG)
      debugObjects.Get(e)->GetElements()->Get(i)->SetVisible(true);
      else
      debugObjects.Get(e)->GetElements()->Get(i)->SetVisible(false);
    }

}

/*-------------------------------------------------------------------
//  GRPSCENE::GetNodeHierarchy
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/03/2015 13:44:00
//
//  @return       XDWORD :
//
//  @param        v :
//  @param        master :
*/
/*-----------------------------------------------------------------*/
XDWORD  GRPSCENE::GetNodeHierarchy          (XVECTOR<GRPNODE*>* v,GRPNODE* master)
{
  if (master==NULL)
    return 0;

  v->SetIsMulti(false);

  for (XDWORD e=0;e<this->nodes.GetSize();e++)
    if (this->nodes.Get(e)->GetParent()==master)
    {
      v->Add            (this->nodes.Get(e));
      GetNodeHierarchy  (v,this->nodes.Get(e));
    }

    return v->GetSize();
}


/*-------------------------------------------------------------------
//  GRPSCENE::End
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/05/2015 12:45:24
//
*/
/*-----------------------------------------------------------------*/
void GRPSCENE::End()
{
  XDEBUG_PRINTCOLOR(0, __L("End Scene"));
  XTIMER* timer=xfactory->CreateTimer();

  this->Publish(GRPMSG_DESTROYED);

  GRPOPENGLCTRL::VBO::CurrentVBO    =-1;
  GRPSHADER::CurrentShader    =-1;

  objects.DeleteContents();
  objects.DeleteAll();

    OPENGLCHECKERROR(__L("GRPSCENE::End::objects"));

    XDEBUG_PRINTCOLOR(0,__L("Destroying Scene Objects: %lld"),timer->GetMeasureMilliSeconds());
    timer->Reset();

  nodes.DeleteContents();
  nodes.DeleteAll();

    OPENGLCHECKERROR(__L("GRPSCENE::End::nodes"));
    XDEBUG_PRINTCOLOR(0,__L("Destroying nodes Objects: %lld"),timer->GetMeasureMilliSeconds());
    timer->Reset();

  cameras.DeleteContents();
  cameras.DeleteAll();

    OPENGLCHECKERROR(__L("GRPSCENE::End::cameras"));
    XDEBUG_PRINTCOLOR(0,__L("Destroying cameras Objects: %lld"),timer->GetMeasureMilliSeconds());
    timer->Reset();

  debugObjects.DeleteAll();

    OPENGLCHECKERROR(__L("GRPSCENE::End::Debuggers"));
    XDEBUG_PRINTCOLOR(0,__L("Destroying Debuggers Objects: %lld"),timer->GetMeasureMilliSeconds());
    timer->Reset();

  lights.DeleteAll();

    OPENGLCHECKERROR(__L("GRPSCENE::End::lights"));
    XDEBUG_PRINTCOLOR(0,__L("Destroying lights Objects: %lld"),timer->GetMeasureMilliSeconds());
    timer->Reset();

  paths.DeleteContents();
  paths.DeleteAll();

    OPENGLCHECKERROR(__L("GRPSCENE::End::paths"));
    XDEBUG_PRINTCOLOR(0,__L("Destroying paths Objects: %lld"),timer->GetMeasureMilliSeconds());
    timer->Reset();

  viewports.DeleteContents();
  viewports.DeleteAll();

    OPENGLCHECKERROR(__L("GRPSCENE::End::Viewports"));
    XDEBUG_PRINTCOLOR(0,__L("Destroying Viewports Objects: %lld"),timer->GetMeasureMilliSeconds());
    timer->Reset();

  delete(timer);

  this->Unsubscribe();
  this->Hide();
  XDEBUG_PRINTCOLOR(0, __L("End Scene"));
}

/*-------------------------------------------------------------------
//  GRPSCENE::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/05/2015 12:59:03
//
//  @return       bool :
//
//  @param        n :
*/
/*-----------------------------------------------------------------*/
bool GRPSCENE::Add(GRPNODE* n)
{
  lock->Lock();
  if (GetNodes()->Find(n)==NOTFOUND)
  {
    n->modified=true;
    n->AddObserver(this);
    bool r=GetNodes()->Add(n);
    lock->UnLock();
    return r;
  }
  lock->UnLock();
  return false;
}

/*-------------------------------------------------------------------
//  GRPSCENE::Notify
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/11/2015 13:30:58
//
//  @return       bool :
//
//  @param        msg :
//  @param        notifier :
*/
/*-----------------------------------------------------------------*/
bool GRPSCENE::Notify (GRPMSGTYPE msg,GRPSUBJECT* notifier)
{
  switch(msg)
  {
  case GRPMSG_ORDER:
    {
        for (XDWORD e=0;e<this->Passes.GetSize();e++)
        {
          this->Passes.Get(e)->Notify(msg,notifier);
        }
    }
    break;

  case GRPMSG_DESTROYED:
    {
        GRPOBJECT* object = dynamic_cast<GRPOBJECT*>(notifier);
        if (object != NULL)
        {
          this->objects.Delete(object);
          return true;
        }

        GRPELEMENT* element=dynamic_cast<GRPELEMENT*>(notifier);
        if (element!=NULL)
        {
          for (XDWORD e=0;e<this->Passes.GetSize();e++)
          {
            this->Passes.Get(e)->Notify(msg,notifier);
          }
          return true;
        }

        GRPLIGHT* light=dynamic_cast<GRPLIGHT*>(notifier);
        if (light!=NULL)
        {
            while (this->lights.Delete(light));
        }

        GRPCAMERA* camera=dynamic_cast<GRPCAMERA*>(notifier);
        if (camera!=NULL)
        {
            while (this->cameras.Delete(camera));
        }

        GRPNODE* node=dynamic_cast<GRPNODE*>(notifier);
        if (node!=NULL)
        {
          RemoveFromNodes(node);
          return true;
        }
        break;
    }
  default: break;

  }
return true;
}


/*-------------------------------------------------------------------
//  GRPSCENE::AddToRenderOrder
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/07/2016 15:40:51
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
void GRPSCENE::AddToRenderOrder(GRPELEMENT* element)
{
  pass->GetRenderOrder()->Add(element);
}

/*-------------------------------------------------------------------
//   GRPSCENE::RemoveFromRenderOrder
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  02/01/2018 16:32:43
//  @return   void :
//
//  @param    GRPELEMENT* :
//
*//*-----------------------------------------------------------------*/
void GRPSCENE::RemoveFromRenderOrder(GRPELEMENT* element)
{
  lock->Lock();
  bool status=false;
  for (XDWORD e=0;e<this->Passes.GetSize();e++)
  {
    if (this->Passes.Get(e)->Remove(element)==true)
      status=true;
  }
  lock->UnLock();
}




/*-------------------------------------------------------------------
//   GRPSCENE::RemoveFromNodes
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  02/01/2018 16:31:37
//  @return   void :
//
//  @param    GRPNODE* :
//
*//*-----------------------------------------------------------------*/
void GRPSCENE::RemoveFromNodes(GRPNODE* node)
{
  lock->Lock();
  int n=this->nodes.Find(node);
  if (n!=NOTFOUND)
    {
      nodes.DeleteIndex(n);
      lock->UnLock();
      RemoveFromNodes(node);
    }
  lock->UnLock();
}
