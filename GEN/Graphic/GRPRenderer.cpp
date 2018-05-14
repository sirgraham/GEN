
/*------------------------------------------------------------------------------------------
//  GRPRENDERER.CPP
//
//  collection of scenes and general rendering
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 16/10/2015 16:28:45
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPRenderer.h"
#include "GRPScene.h"
#include "GRPViewport.h"
#include "GRPObjectBuilder.h"
#include "GRPProfiler.h"
#include "GRPScreen.h"
#include "GRPFactory.h"
#include "GRPBitmap.h"
#include "GRPTexture.h"
#include "GRPMaterialLibrary.h"

#include "GRPContext.h"

#include "GRPObject.h"

#include "GRPRenderTarget.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  GRPRENDERER::GRPRENDERER
*/
/**
//
//  Class Constructor GRPRENDERER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      26/10/2015 14:02:50
//

*/
/*-----------------------------------------------------------------*/
GRPRENDERER::GRPRENDERER( GRPSCREEN* screen, GRPCONTEXT* context, bool postprocess)
{

  GRPOBJECTBUILDER::Get()->Ini();

  mainscreen    = screen;
  this->context = context;

  this->postprocess = postprocess;



  GRPSHADERLIBRARY::Get();
}



/*-------------------------------------------------------------------
//  GRPRENDERER::~GRPRENDERER
*/
/**
//
//   Class Destructor GRPRENDERER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      26/10/2015 14:02:57
//
*/
/*-----------------------------------------------------------------*/
GRPRENDERER::~GRPRENDERER()
{
  for(XDWORD i = 0; i < scenes.GetSize(); i++)
  {
    delete scenes.Get(i);
  }

  scenes.DeleteAll();

  GRPMATERIALLIBRARY::Destroy ();
  GRPSHADERLIBRARY::Destroy   ();
  GRPPASSMANAGER::Destroy     ();
  GRPOBJECTBUILDER::Destroy   ();
}



/*-------------------------------------------------------------------
//  GRPRENDERER::Ini
*/
/**
//
//  initialize scenes
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      26/10/2015 14:03:12
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPRENDERER::Ini()
{
  XDEBUG_PRINTCOLOR(2,__L("GRPRENDERER::Ini()"));                                       OPENGLCHECKERROR(__L("GRPREnderer Ini"));

   //------------------------------------------------------ GENERATES TEXTURE TYPES MAP
  grpfactory->GenerateTextureMap();

  //------------------------------------------------------ LOAD SHADERS
  GRPSHADERLIBRARY::Get()->LoadShaders();                                               OPENGLCHECKERROR(__L("GRPSCENE::LoadShaders"));

  //------------------------------------------------------ Initialize scenes
  for(XDWORD i = 0; i < scenes.GetSize(); i++)
    if(!scenes.Get(i)->Ini())
      return false;

  //-------------------------------------------------------Manage Postprocessing
  if(postprocess)
      if (!SetupPostProcessing())
        return false;


  this->SetClearColor(GRPCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
  this->SetClearFlags(GRPSCENE_CLEAR_ALL);
  this->Clear();

   return true;
}



/*-------------------------------------------------------------------
//  GRPRENDERER::End
*/
/**
//
//  ends scenes
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      26/10/2015 14:03:23
//
*/
/*-----------------------------------------------------------------*/
void GRPRENDERER::End()
{

  if(postprocess)
    {
      if(rendertarget0)
        {
          delete rendertarget0;
        }

      if(rendertarget1)
        {
          delete rendertarget1;
        }

      if(this->PostProcessCanvas)
        {
          delete this->PostProcessCanvas;
        }
    }

  for(XDWORD i = 0; i < scenes.GetSize(); i++)
    {
      scenes.Get(i)->End();
    }
}


/*-------------------------------------------------------------------
//  GRPRENDERER::Render
*/
/**
//
//  renders all scenes
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      26/10/2015 14:03:45
//
*/
/*-----------------------------------------------------------------*/
void GRPRENDERER::Render()
{
GRPPROFILER::Clear();

  if(!GetContext()->IsLost())
    {

    // to render to texture
    if(postprocess)
    {
      if(!pingpong)
        {
          rendertarget0->SelectAsTarget();
        }
      else
        {
          rendertarget1->SelectAsTarget();
        }

      // Qualcomm says this is better
      this->Clear();
    }

    // Actual rendering
    this->ClearFlags=GRPPASS_CLEAR_FLAG_ALL;
    this->Clear();


    scenes.FastGet(0)->Render();

    XDWORD nscenes=scenes.GetSize();
    for(XDWORD i = 1; i < nscenes; i++)
    {
      scenes.FastGet(i)->Render();
    }




    // actual postprocessing passes
    if(postprocess)
      {
        const GLenum discards[]  = {GL_DEPTH_ATTACHMENT};
        glDiscardFramebufferEXT(GL_FRAMEBUFFER, 1, discards);

        if(!pingpong)
          {
            rendertarget0->Deselect();
          }
        else
          {
            rendertarget1->Deselect();
          }
        this->Clear();

        // texture usage needs to go the other way around of the fbo bindings
        if(!pingpong)
          {
            GRPMATERIAL * postmaterial = GRPMATERIALLIBRARY::Get()->Get(__L("Pong"));

            this->PostProcessCanvas->SetMaterial(postmaterial);
            this->PostProcessCanvas->GetNode()->SetMaterial(postmaterial);
          }
        else
          {
            GRPMATERIAL * postmaterial = GRPMATERIALLIBRARY::Get()->Get(__L("Ping"));

            this->PostProcessCanvas->SetMaterial(postmaterial);
            this->PostProcessCanvas->GetNode()->SetMaterial(postmaterial);
          }

        //// render the canvas
        GRPSHADER::UseShader(this->PostProcessCanvas->GetElements()->Get(0)->GetProgram());
        glUniform2i(this->PostProcessCanvas->GetElements()->Get(0)->GetProgram()->iResolutionHandle, mainscreen->GetWidth(), mainscreen->GetHeight());

        //
        GRPOPENGLCTRL::DEPTH::SetDepthTest(false);
        this->PostProcessCanvas->GetElements()->FastGet(0)->Draw(&GRPMATRIX::Identity, &GRPMATRIX::Identity, &GRPMATRIX::ortho);
        GRPOPENGLCTRL::DEPTH::SetDepthTest(true);

        pingpong = !pingpong;
      }
    }
}

/*-------------------------------------------------------------------
//  GRPRENDERER::PrepareScenes
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/10/2015 11:49:25
//
*/
/*-----------------------------------------------------------------*/
void   GRPRENDERER::PrepareScenes ()
{
  for(XDWORD i = 0; i < scenes.GetSize(); i++)
  {
    scenes.Get(i)->PrepareScene();
  }
}


/*-------------------------------------------------------------------
//  GRPRENDERER::SetupPostProcessing
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/11/2016 12:45:17
//
*/
/*-----------------------------------------------------------------*/
bool   GRPRENDERER::SetupPostProcessing()
{
      pingpong = false;

      rendertarget0 = new GRPRENDERTARGET(mainscreen->GetWidth(), mainscreen->GetHeight());
      if(!rendertarget0) { return false;}

      rendertarget1 = new GRPRENDERTARGET(mainscreen->GetWidth(), mainscreen->GetHeight());
      if(!rendertarget1)  {return false;}

      // create test for custom texture
      //GRPBITMAP* bitmap = grpfactory->CreateBitmap(mainscreen->GetWidth(), mainscreen->GetHeight(), GRPPROPERTYMODE_32_RGBA_8888);
      GRPBITMAP* bitmap0 = grpfactory->CreateBitmap(mainscreen->GetWidth(), mainscreen->GetHeight(), GRPPROPERTYMODE_32_RGBA_8888);
      GRPTEXTURE* texture0 = new GRPTEXTURE();
      texture0->Create(bitmap0);
      texture0->SetName(__L("Ping"));

      GRPMATERIALLIBRARY::Get()->Create(texture0);

      rendertarget0->InitFrameBuffers(texture0);
      rendertarget0->Deselect();

      GRPBITMAP* bitmap1 = grpfactory->CreateBitmap(mainscreen->GetWidth(), mainscreen->GetHeight(), GRPPROPERTYMODE_32_RGBA_8888);
      GRPTEXTURE* texture1 = new GRPTEXTURE();
      texture1->Create(bitmap1);
      texture1->SetName(__L("Pong"));

      GRPMATERIALLIBRARY::Get()->Create(texture1);

      rendertarget1->InitFrameBuffers(texture1);
      rendertarget1->Deselect();

      //GRPMATRIX::ortho = GRPMATRIX((GRPOPENGLCTRL::pixelwidth/2.0f), 0.0f,  0.0f,  0.0f,
      //                             0.0f, (GRPOPENGLCTRL::pixelheight/2.0f),  0.0f,  0.0f,
      //                             0.0f, 0.0f, -1.0f, -3.0f,
      //                             0.0f, 0.0f,  0.0f,  1.0f);

      this->PostProcessCanvas = new GRPOBJECT();

      this->PostProcessCanvas->SetNode(scenes.FastGet(0)->CreateNode());

      GRPMATERIAL * postmaterial = GRPMATERIALLIBRARY::Get()->Get(__L("Ping"));

      this->PostProcessCanvas->SetMaterial(postmaterial);
      this->PostProcessCanvas->GetNode()->SetMaterial(postmaterial);

      this->PostProcessCanvas->GetNode()->Init();

      GRPELEMENT * plane = GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_SCREEN);
      //GRPELEMENT * plane = this->GetObjectBuilder()->CreatePlane(GRPOBJECTBUILDER::AXIS_NEGX);
      plane->SetNode(this->PostProcessCanvas->GetNode());
      plane->GetNode()->SetPosition(0.0f, 0.0f, -10.0f);
      this->PostProcessCanvas->AddElement(plane);
      //this->PostProcessCanvas->SetProgram(scenes.FastGet(0)->GetShaderLibrary()->GetProgramByKey("STX"));
      this->PostProcessCanvas->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"PST"));

      return true;
}

/*-------------------------------------------------------------------
//  GRPRENDERER::Notify
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/03/2017 12:28:26
//
//  @return       bool :
//
//  @param        msg :
//  @param        subject :
*/
/*-----------------------------------------------------------------*/
bool  GRPRENDERER::Notify(GRPMSGTYPE msg,GRPSUBJECT* subject)
{
  switch(msg)
  {
  case GRPMSG_DESTROYED:
    {
      if (subject->GetSubjectType()==GRPNAMED_TYPE_GRPSCENE)
      {
        int index=this->scenes.Find((GRPSCENE*)subject);
        if (index!=NOTFOUND)
        {
          subject->RemoveObserver(this);
          this->scenes.DeleteIndex(index);
        }
      }
    }
    break;
  }
  return true;
}

/*-------------------------------------------------------------------
//  GRPRENDERER::SetClearColor
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/03/2017 12:27:55
//
//  @param        c :
*/
/*-----------------------------------------------------------------*/
void  GRPRENDERER::SetClearColor(GRPCOLOR c)
{
  this->ClearColor=c;
  GRPOPENGLCTRL::COLOR::SetClearColor(c.GetRed(),c.GetGreen(),c.GetBlue(),c.GetAlpha());
}


/*-------------------------------------------------------------------
//  GRPRENDERER::GetScene
*/
/**
//
//  gets an scene by index
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      26/10/2015 14:03:58
//
//  @return       GRPSCENE* :
//
//  @param        index :
*/
/*-----------------------------------------------------------------*/
GRPSCENE* GRPRENDERER::GetScene(int index)
{
  return scenes.Get(index);
}



/*-------------------------------------------------------------------
//  GRPRENDERER::AddScene
*/
/**
//
//  adds a scee to the renderer
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      26/10/2015 14:04:09
//
//  @param        scene :
*/
/*-----------------------------------------------------------------*/
void  GRPRENDERER::AddScene(GRPSCENE* scene)
{
  scenes.Add(scene);
  //this->Subscribe(scene);
  scene->renderer = this;
}

