/*------------------------------------------------------------------------------------------
//  GRPPASS.CPP
//
//  Pass control
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 12/07/2016 13:00:20
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPPass.h"
#include "GRPScene.h"
#include "GRPCamera.h"
#include "GRPViewport.h"
#include "GRPProfiler.h"
#include "GRPCamera.h"
#include "GRPScreen.h"
#include "GRPRenderTarget.h"
#include "GRPFactory.h"
#include "GRPMaterialLibrary.h"

#include "XFactory.h"
#include "XThread.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  GRPPASS::GRPPASS
*/
/**
//
//  Class Constructor GRPPASS
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/07/2016 14:03:58
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPPASS::GRPPASS(XCHAR* name) : GRPNAMED(name)
{
  Clean();
  mutex=xfactory->Create_Mutex();
  SetRenderComparator<GRPELEMENT::COMPARATOR>();  //default render order
  SetName(name);
  //XDEBUG_PRINTCOLOR(5,__L("------------------------------------ Created Pass : %s %x"),name,this);
}

/*-------------------------------------------------------------------
//  GRPPASS::~GRPPASS
*/
/**
//
//   Class Destructor GRPPASS
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/07/2016 14:04:03
//
*/
/*-----------------------------------------------------------------*/
GRPPASS::~GRPPASS()
{
  DestroyRenderOrder();
  Clean();
  acceptElements.DeleteContents();
  acceptElements.DeleteAll();
  rejectElements.DeleteContents();
  rejectElements.DeleteAll();
  delete mutex;
}

/*-------------------------------------------------------------------
//  GRPPASS::COMPARATOR::Compare
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 13:09:05
//
//  @return       int :
//
//  @param        test1 :
//  @param        test2 :
*/
/*-----------------------------------------------------------------*/
int GRPPASS::COMPARATOR::Compare(GRPPASS* test1,GRPPASS* test2)
{
  if (test1->priority<test2->priority)
    return 1;
  return -1;
}


/*-------------------------------------------------------------------
//  GRPPASS::GetRenderOrder
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/07/2016 17:21:41
//
//  @return       XLIST<GRPELEMENT*>* :
//
*/
/*-----------------------------------------------------------------*/
GRPRENDERLIST GRPPASS::GetRenderOrder()
{
  return RenderOrder;
}

void  GRPPASS::ClearAllObjects()
{
    RenderOrder->DeleteAll();
}

/*-------------------------------------------------------------------
//  GRPPASS::SetRenderOrder
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/07/2016 17:21:34
//
//  @param        queue :
*/
/*-----------------------------------------------------------------*/
void  GRPPASS::SetRenderOrder (GRPRENDERLIST queue)
{
  this->mutex->Lock();

  DestroyRenderOrder();
  this->RenderOrder=queue;
  if (queue)
  this->OrderClone=true;

  this->mutex->UnLock();
}

/*-------------------------------------------------------------------
//  GRPPASS::DestroyRenderOrder
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/07/2016 17:21:27
//
*/
/*-----------------------------------------------------------------*/
void  GRPPASS::DestroyRenderOrder ()
{
  this->mutex->Lock();

  if (!this->OrderClone)
    if (this->RenderOrder)
      delete this->RenderOrder;

  this->OrderClone=false;
  RenderOrder=NULL;

    this->mutex->UnLock();
}

/*-------------------------------------------------------------------
//  GRPPASS::Begin
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/07/2016 13:01:59
//
*/
/*-----------------------------------------------------------------*/
void  GRPPASS::Begin()
{
    ApplyFlags    ();

    if (hasToClear!=GRPPASS_CLEAR_FLAG_NONE)
    Clear         ();

    this->Publish (GRPMSG_PASSSTARTED);
}

/*-------------------------------------------------------------------
//  GRPPASS::ApplyFlags
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/07/2016 10:19:06
//
*/
/*-----------------------------------------------------------------*/
void  GRPPASS::ApplyFlags()
{

  if (type & GRPPASS_TYPE_TEXTUREMODE)
  {
    GRPOPENGLCTRL::TEXTURE::SetTextureMode      (TextureIsActive);                                                          OPENGLCHECKERROR(__L("Pass Flags: GRPPASS_TYPE_TEXTUREMODE"));
  }

  if (type & GRPPASS_TYPE_DEPTHTEST)
  {
    GRPOPENGLCTRL::DEPTH::SetDepthTest          (DepthTestIsActive);                                                        OPENGLCHECKERROR(__L("Pass Flags: GRPPASS_TYPE_DEPTHTEST"));
  }

  if (type & GRPPASS_TYPE_DEPTHWRITE)
  {
    GRPOPENGLCTRL::DEPTH::SetDepthWriting       (DepthWritingIsActive);                                                     OPENGLCHECKERROR(__L("Pass Flags: GRPPASS_TYPE_DEPTHWRITE"));
  }

  if (type & GRPPASS_TYPE_DEPTHFUNC)
  {
    GRPOPENGLCTRL::DEPTH::SetDepthFunc          (DepthFunction);                                                            OPENGLCHECKERROR(__L("Pass Flags: GRPPASS_TYPE_DEPTHFUNC"));
  }

  if (type & GRPPASS_TYPE_CLEARDEPTH)
  {
    GRPOPENGLCTRL::DEPTH::SetClearDepth         (ClearDepth);                                                               OPENGLCHECKERROR(__L("Pass Flags: GRPPASS_TYPE_CLEARDEPTH"));
  }

  if (type & GRPPASS_TYPE_COLORWRITE)
  {
    GRPOPENGLCTRL::COLOR::SetColorWriting       (ColorWritingIsActive);                                                     OPENGLCHECKERROR(__L("Pass Flags: GRPPASS_TYPE_COLORWRITE"));
  }

  if (type & GRPPASS_TYPE_CULLFACE)
  {
    GRPOPENGLCTRL::CULLING::SetBackFaceCulling  (BackFaceCullingIsActive, BackFaceCullingMode);                             OPENGLCHECKERROR(__L("Pass Flags: GRPPASS_TYPE_COLORWRITE"));
  }

  if (type & GRPPASS_TYPE_ALPHABLEND)
  {
    GRPOPENGLCTRL::BLENDING::SetAlphaBlending   (AlphaBlendingIsActive,   AlphaBlendingMode);                               OPENGLCHECKERROR(__L("Pass Flags: GRPPASS_TYPE_ALPHABLEND"));
  }

  if (type & GRPPASS_TYPE_SCISSOR)
  {
    GRPOPENGLCTRL::SCISSOR::SetScissor          (ScissorTestIsActive,     Scissor.x1, Scissor.y1, Scissor.x2, Scissor.y2);  OPENGLCHECKERROR(__L("Pass Flags: GRPPASS_TYPE_SCISSOR"));
  }



  if (type & GRPPASS_TYPE_STENCIL)
  {
    GRPOPENGLCTRL::STENCIL::SetStencil          (StencilTestIsActive);                                                      OPENGLCHECKERROR(__L("Pass Flags: SetStencil"));

    if (type & GRPPASS_TYPE_STENCIL_OPERATION)
    GRPOPENGLCTRL::STENCIL::SetStencilOperation (GRPSTENCILBUFFER_FRONT, StencilFailOperation,  StencilDepthFailOperation,  StencilPassOperation);  OPENGLCHECKERROR(__L("Pass Flags: SetStencilOp"));

    if (type & GRPPASS_TYPE_STENCIL_MASK)
    GRPOPENGLCTRL::STENCIL::SetStencilMask      (StencilMask);                                                              OPENGLCHECKERROR(__L("Pass Flags: SetStencilMask"));

    if (type & GRPPASS_TYPE_STENCIL_FUNCTION)
    GRPOPENGLCTRL::STENCIL::SetStencilFunction  (StencilFunction,StencilValue);                                             OPENGLCHECKERROR(__L("Pass Flags: SetStencilFunc"));

  }


}


/*-------------------------------------------------------------------
//  GRPPASS::SetScissor
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/07/2016 11:48:46
//
//  @param        onoff :
//  @param        xo :
//  @param        yo :
//  @param        xf :
//  @param        yf :
*/
/*-----------------------------------------------------------------*/
void  GRPPASS::SetScissor (bool onoff, int xo, int yo, int xf, int yf)
{
  this->ScissorTestIsActive =onoff;
  this->Scissor             =GRPRECTINT(xo,yo,xf,yf);
}

/*-------------------------------------------------------------------
//  GRPPASS::Clear
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/07/2016 10:19:37
//
*/
/*-----------------------------------------------------------------*/
void  GRPPASS::Clear()
{
  GRPOPENGLCTRL::COLOR::SetClearColor (this->clearcolor.red,this->clearcolor.blue,this->clearcolor.green,this->clearcolor.alpha);
  GRPOPENGLCTRL::COLOR::Clear         (this->hasToClear);
}

/*-------------------------------------------------------------------
//  GRPPASS::End
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/07/2016 13:02:06
//
*/
/*-----------------------------------------------------------------*/
void  GRPPASS::End()
{
  ///this->Publish(GRPMSG_PASSENDED);
}

/*-------------------------------------------------------------------
//  GRPPASS::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/07/2016 14:06:17
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool  GRPPASS::Add    (GRPELEMENT* element)
{
  this->mutex->Lock();
  RenderOrder->Add(element);
  this->mutex->UnLock();
  return true;
}

/*-------------------------------------------------------------------
//  GRPPASS::Remove
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/07/2016 14:06:28
//
//  @return       bool :
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
bool  GRPPASS::Remove (GRPELEMENT* element)
{
  if (!this->mutex->IsLock())
    this->mutex->Lock();

  int n=NOTFOUND;

  n=this->RenderOrder->Find(element);
  if (n!=NOTFOUND)
    {
      RenderOrder->DeleteIndex(n);
    }

  if (!this->mutex->UnLock())
    this->mutex->UnLock();

  return n!=NOTFOUND;
}

/*-------------------------------------------------------------------
//  GRPPASS::Notify
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/07/2016 10:41:56
//
//  @return       bool :
//
//  @param        type :
//  @param        subject :
*/
/*-----------------------------------------------------------------*/
bool  GRPPASS::Notify (GRPMSGTYPE type, GRPSUBJECT* subject)
{
  switch(type)
  {
  case GRPMSG_ADDED:
      isloaded = false;
    break;
  case GRPMSG_ORDER:
    {
      GRPELEMENT* element=dynamic_cast<GRPELEMENT*>(subject);
      if (element!=NULL)
      {
      if (Remove  (element))
          Add     (element);
      }
    }
    break;


  case GRPMSG_DESTROYED:
      {
      GRPELEMENT* element=dynamic_cast<GRPELEMENT*>(subject);
      if (element!=NULL)
      {
      Remove  (element);
      }
    }
    break;
  }

  return true;
}

/*-------------------------------------------------------------------
//  GRPPASS::GetProgram
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/08/2016 10:12:56
//
//  @return       GRPSHADERPROGRAM* :
//
*/
/*-----------------------------------------------------------------*/
GRPSHADERPROGRAM* GRPPASS::GetProgram()
{
  return this->shader;
}

/*-------------------------------------------------------------------
//  GRPPASS::SetProgram
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/08/2016 10:17:43
//
//  @param        program :
*/
/*-----------------------------------------------------------------*/
void  GRPPASS::SetProgram(GRPSHADERPROGRAM* program)
{
  this->shader=program;
}

/*-------------------------------------------------------------------
//  GRPPASS::Draw
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 13:40:45
//
//  @return       bool :
//
//  @param        GRPPASS* :
*/
/*-----------------------------------------------------------------*/
bool    GRPPASS::Draw               (GRPPASS*)
{
  return this->Draw();
}


/*-------------------------------------------------------------------
//  GRPPASS::Draw
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 9:28:28
//
//  @return       bool :
//
//  @param        view :
//  @param        viewprojection :
//  @param        projection :
*/
/*-----------------------------------------------------------------*/
bool  GRPPASS::Draw ()
{
    if (!isloaded)
    {
        isloaded=OnAdded();
        if (!isloaded)
        {
            isloaded = true;
            return false;
        }
    }

    if (!this->active || !this->GetRenderOrder())     return true;


    this->mutex->Lock();

    for (int e=0;e<iterations;e++)
    {
      currentiteration=e;
      OPENGLCHECKERROR(__L("GRPPASS::iterations"));

    //--------------------- Call event
        Start();
        OPENGLCHECKERROR(__L("GRPPASS::Start"));

    //--------------------- Calcule Matrices
        CalculateMatrices();

    //---------------------- call observers for a new frame step
        this->Publish(GRPMSG_STEP);
        OPENGLCHECKERROR(__L("GRPPASS::GRPMSG_STEP"));

    //---------------------- Actual render
        this->Begin();

          if (this->globalshaderoverride)
              DrawRenderOrderWithOverride();
          else
              DrawRenderOrder();
        this->End();
    }
    this->mutex->UnLock();

    this->Finish();
    OPENGLCHECKERROR(__L("GRPPASS::Draw finish"));

    GRPPROFILER::nElements  += (int)this->GetRenderOrder()->GetSize();
    return true;
}


/*-------------------------------------------------------------------
//  GRPPASS::DrawRenderOrderWithOverride
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2017 12:27:44
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPPASS::DrawRenderOrderWithOverride()
{
  GRPRENDERLIST renderorder=this->GetRenderOrder();

  if (this->GetRenderOrder())
  {
    XLISTDATA<GRPELEMENT*>* current=NULL;
    XDWORD n=renderorder->GetSize();

      current=renderorder->GetFirstData();

      GRPSHADER::UseShader(this->shader);
      for (int e=n;e>0;e--)
      {
        this->OnDraw(current->data);
        current->data->Draw(this);
        current=current->next;
      }
  }



  return true;
}


/*-------------------------------------------------------------------
//  GRPPASS::DrawRenderOrder
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2017 12:31:46
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPPASS::DrawRenderOrder()
{
  GRPRENDERLIST renderorder=this->GetRenderOrder();


            if (renderorder)
            if (renderorder->GetSize())
            {
              XLISTDATA<GRPELEMENT*>* current=NULL;
              XDWORD n=renderorder->GetSize();
              current=renderorder->GetFirstData();

              while(current)
              {
                current->data->ComputeMatrices    (viewmatrix,&viewprojection,projectionmatrix);                          OPENGLCHECKERROR(__L("ComputeMatrices"));;
                current=current->next;
              }

              current=renderorder->GetFirstData();
              if (current)
              if (current->data->GetProgram())
              {
                  if (current->data->GetProgram()->ViewMatrixLoc != -1)
                  {
                      glUniformMatrix4fv(current->data->GetProgram()->ViewMatrixLoc, (GLsizei)1, GL_FALSE, &(viewmatrix->matrix[0][0]));                OPENGLCHECKERROR(__L("ViewMatrixLoc"));
                  }
                  if (current->data->GetProgram()->ProjectionMatrixLoc != -1)
                  {
                      glUniformMatrix4fv(current->data->GetProgram()->ProjectionMatrixLoc, (GLsizei)1, GL_FALSE, &(projectionmatrix->matrix[0][0]));          OPENGLCHECKERROR(__L("ProjectionMatrixLoc"));
                  }
              }

              while (current)
              {
                  bool isinside = true;
                  //if (camera)
                  //   isinside=(GRPFRUSTUM_OUTSIDE!=camera->GetFrustum()->boxInFrustum(*current->data->GetBoundingBox()));

                if (isinside)
                {
                      GRPSHADER::UseShader(current->data->GetProgram());
                      this->OnDraw(current->data);
                      current->data->Draw(viewmatrix, &viewprojection, projectionmatrix);
                }
                else
                    GRPPROFILER::nCulledElements++;

                current=current->next;
              }
            }

  return true;
}


/*-------------------------------------------------------------------
//  GRPPASS::CalculateMatrices
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2017 12:23:18
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  GRPPASS::CalculateMatrices()
{


      //---------------------- Get Viewport
        GRPVIEWPORT*    viewport  = GetViewport();    OPENGLCHECKERROR(__L("GRPPASS::CalculateMatrices()::GetViewport"));

    //---------------------- Get Camera
        GRPCAMERA*      camera    = this->GetCamera(); OPENGLCHECKERROR(__L("GRPPASS::CalculateMatrices()::GetCamera"));

    //---------------------- Get view Matrix from camera
        this->viewmatrix        = &camera->viewmatrix;          if (flipmatrixIsActive) this->viewmatrix->GetMultiplicationMatrix(&camera->viewmatrix,&flipmatrix);

    //---------------------- Get Projection Matrix from viewPort
        this->projectionmatrix  = *viewport;

    //---------------------- Get View*Projection
        this->viewprojection.GetMultiplicationMatrix(this->viewmatrix,this->projectionmatrix);

    //----------------------- lightning
        if (lightingIsEnabled)  holderscene->PrepareLightning(); OPENGLCHECKERROR(__L("GRPPASS::CalculateMatrices()::PrepareLightning"));

  return true;
}

/*-------------------------------------------------------------------
//  GRPPASS::CreateCamera
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2017 11:29:24
//
//  @param        type :
*/
/*-----------------------------------------------------------------*/
void GRPPASS::CreateCamera(GRPCAMERA_TYPE type)
{
  this->camera=holderscene->CreateCamera();
  this->camera->SetType(type);
  //this->camera->SetPosition(0.0,0.0,+100.0);
  this->camera->BuildLookAt();

  this->ownscamera=true;
}



/*-------------------------------------------------------------------
//  GRPPASS::CreateViewport
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2017 11:29:28
//
//  @param        isOrtho :
*/
/*-----------------------------------------------------------------*/
void GRPPASS::CreateViewport(GRPPROJECTIONTYPE projectiontype)
{
  this->viewport=holderscene->CreateViewport();

  this->viewport->SetSize(1.0f,1.0f);
  this->viewport->SetWindowSize((GLFLOAT)holderscene->GetScreen()->GetWidth(),(GLFLOAT)holderscene->GetScreen()->GetHeight());
  this->viewport->SetProjection(projectiontype);
  this->ownsviewport=true;
}

/*-------------------------------------------------------------------
//   GRPPASS::SetRect
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  16/10/2017 15:48:10
//  @return   void :
//
//  @param    GRPRECTFLOAT* :
//
*//*-----------------------------------------------------------------*/
void GRPPASS::SetRect(GRPRECTFLOAT* rect)
{
    this->rect.x1 = rect->x1;
    this->rect.x2 = rect->x2;
    this->rect.y1 = rect->y1;
    this->rect.y2 = rect->y2;
}

/*-------------------------------------------------------------------
//   GRPPASS::SetRect
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return   void :
//
//  @param    float :
//  @param    float :
//  @param    float :
//  @param    float :
//
*//*-----------------------------------------------------------------*/
void GRPPASS::SetRect(float x, float y, float xf, float yf)
{
    rect.x1 = x;
    rect.x2 = xf;
    rect.y1 = y;
    rect.y2 = yf;
}

/*-------------------------------------------------------------------
//   GRPPASS::GetRect
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  16/10/2017 15:49:05
//  @return   GRPRECTFLOAT* :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
GRPRECTFLOAT* GRPPASS::GetRect()
{
    return &rect;
}

/*-------------------------------------------------------------------
//   GRPPASS::SetCamera
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return   void :
//
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
void GRPPASS::SetCamera(XCHAR*  camera_name)
{
    this->camera_name = camera_name;

    if (this->camera_name.GetSize())
    if (holderscene)
    {
        this->SetCamera(holderscene->FindCamera(camera_name));
    }
}

/*-------------------------------------------------------------------
//   GRPPASS::SetViewport
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  24/10/2017 16:42:55
//  @return   void :
//
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
void GRPPASS::SetViewport(XCHAR*  viewport_name)
{
    this->viewport_name = viewport_name;
    if (this->viewport_name.GetSize())
    if (holderscene)
    {
        this->SetViewport(holderscene->FindViewport(viewport_name));
    }
}

/*-------------------------------------------------------------------
//   GRPPASS::SetLight
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  23/11/2017 16:52:46
//  @return   void :
//
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
void GRPPASS::SetLight(XCHAR* light_name)
{
    this->light_name = light_name;
    if (this->light_name.GetSize())
        if (holderscene)
        {
            this->SetLight(holderscene->FindLight(light_name));
        }
}

/*-------------------------------------------------------------------
//   GRPPASS::SetClone
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  16/10/2017 13:32:53
//  @return   void :
//
//  @param    bool :
//
*//*-----------------------------------------------------------------*/
void GRPPASS::SetClone(bool b)
{
    mustClone = b;
}

/*-------------------------------------------------------------------
//   GRPPASS::SetStencilWriting
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  16/10/2017 10:37:04
//  @return   void :
//
//  @param    XDWORD :
//
*//*-----------------------------------------------------------------*/
void GRPPASS::SetStencilWriting(XDWORD tag)
{
    this->SetStencilTestIsActive(true);

    this->type |= GRPPASS_TYPE_STENCIL;
    this->type |= GRPPASS_TYPE_STENCIL_FUNCTION;
    this->type |= GRPPASS_TYPE_STENCIL_OPERATION;
    this->type |= GRPPASS_TYPE_STENCIL_MASK;

    this->SetStencilFailOperation       (GRPSTENCILOPERATION_KEEP);
    this->SetStencilDepthFailOperation  (GRPSTENCILOPERATION_KEEP);
    this->SetStencilPassOperation       (GRPSTENCILOPERATION_REPLACE);
    this->SetStencilFunction            (GRPSTENCILFUNCTION_ALWAYS, tag);
    this->SetStencilMask                (0xFF);

    this->lightingIsEnabled             = false;

    this->SetDepthTestIsActive          (false);
    this->SetDepthWritingIsActive       (false);
    this->SetColorWritingIsActive       (false);
}

/*-------------------------------------------------------------------
//   GRPPASS::SetStencilTesting
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  16/10/2017 10:56:10
//  @return   void :
//
//  @param    XDWORD :
//
*//*-----------------------------------------------------------------*/
void GRPPASS::SetStencilTesting(XDWORD tag)
{
    this->type |= GRPPASS_TYPE_STENCIL;
    this->type |= GRPPASS_TYPE_STENCIL_OPERATION;
    this->type |= GRPPASS_TYPE_STENCIL_MASK;

    this->SetStencilTestIsActive        (true);
    this->SetStencilFunction            (GRPSTENCILFUNCTION_EQUAL, tag);
    this->SetStencilMask                (0x00);
    this->SetStencilFailOperation       (GRPSTENCILOPERATION_KEEP);
    this->SetStencilDepthFailOperation  (GRPSTENCILOPERATION_KEEP);
    this->SetStencilPassOperation       (GRPSTENCILOPERATION_REPLACE);

}

/*-------------------------------------------------------------------
//   GRPPASS::DisableStencil
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  16/10/2017 10:56:15
//  @return   void :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
void GRPPASS::DisableStencil()
{
    this->type |= GRPPASS_TYPE_STENCIL;

    this->SetStencilTestIsActive(false);
}

/*-------------------------------------------------------------------
//  GRPPASS::GetViewport
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2017 12:03:13
//
//  @return       GRPVIEWPORT* :
//
*/
/*-----------------------------------------------------------------*/
GRPVIEWPORT* GRPPASS::GetViewport()
{
  GRPVIEWPORT*  v=viewport;
  if (!v && holderscene)
                v  = holderscene->GetViewport();

                v->SetViewport();
                v->Select     ();  //TODO : optimizable

  return v;
}

/*-------------------------------------------------------------------
//  GRPPASS::GetCamera
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2017 12:13:49
//
//  @return       GRPCAMERA* :
//
*/
/*-----------------------------------------------------------------*/
GRPCAMERA* GRPPASS::GetCamera()
{
GRPCAMERA* c=camera;

          if (!c && holderscene)
                    c=holderscene->GetCamera();

          c->BuildLookAtMatrix();
return c;
}

/*-------------------------------------------------------------------
//   GRPPASS::CreateRenderTarget
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  23/11/2017 16:52:27
//  @return   void :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
void GRPPASS::CreateRenderTarget()
{
    //------------- create a texture render target
    GLFLOAT w = 512;
    GLFLOAT h = 512;

    if (this->viewport)
        this->viewport->GetRealWindowSize(w, h);
    else
        this->holderscene->GetViewport()->GetRealWindowSize(w, h);

    target = this->holderscene->CreateRenderTarget((XDWORD)w, (XDWORD)h);
    target->InitFrameBuffers(this->GetTexture());
    target->Deselect();
}