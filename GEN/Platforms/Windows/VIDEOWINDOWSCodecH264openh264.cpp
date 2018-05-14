
/*------------------------------------------------------------------------------------------
//  VIDEOWINDOWSCODECH264OPENH264.CPP
//
//  windows h264 software codec
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 19/04/2016 11:12:33
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "VIDEOWINDOWSCODECH264openh264.h"
#include "VIDEOH264FrameReader.h"

#include "GRPRenderer.h"
#include "GRPScene.h"
#include "GRPNode.h"
#include "GRPCamera.h"
#include "GRPViewport.h"
#include "GRPObject.h"
#include "GRPElement.h"
#include "GRPShaderLibrary.h"
#include "GRPMaterialLibrary.h"
#include "GRPObjectBuilder.h"
#include "GRPTextureEXT.h"

#include "XTimer.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  VIDEOWINDOWSCODECH264OPENH264::VIDEOWINDOWSCODECH264OPENH264
*/
/**
//
//  Class Constructor VIDEOWINDOWSCODECH264OPENH264
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      19/04/2016 11:17:17
//
//  @param        videoplayer :
*/
/*-----------------------------------------------------------------*/
VIDEOWINDOWSCODECH264OPENH264::VIDEOWINDOWSCODECH264OPENH264(VIDEOPLAYER* videoplayer) : VIDEOCODECH264(videoplayer)
{
  Clean();

  framereader = new VIDEOH264FRAMEREADER();
}






/*-------------------------------------------------------------------
//  VIDEOWINDOWSCODECH264OPENH264::~VIDEOWINDOWSCODECH264OPENH264
*/
/**
//
//   Class Destructor VIDEOWINDOWSCODECH264OPENH264
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      19/04/2016 11:17:26
//
*/
/*-----------------------------------------------------------------*/
VIDEOWINDOWSCODECH264OPENH264::~VIDEOWINDOWSCODECH264OPENH264()
{
  delete framereader;
  framereader = NULL;

  Clean();
}






/*-------------------------------------------------------------------
//  VIDEOWINDOWSCODECH264OPENH264::Ini
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      19/04/2016 11:17:35
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool VIDEOWINDOWSCODECH264OPENH264::Ini()
{
  framereader->ResetStream();

  xtimer = xfactory->CreateTimer();
  if(!xtimer) return false;

  if(0 != WelsCreateDecoder(&decoder)) return false;

  SDecodingParam sDecParam = {0};
  sDecParam.sVideoProperty.eVideoBsType = VIDEO_BITSTREAM_AVC;
  sDecParam.bParseOnly = false;

  if(0 != decoder->Initialize(&sDecParam))
    {
      WelsDestroyDecoder(decoder);
      return false;
    }

  xmutex = xfactory->Create_Mutex();
  if(!xmutex)
    {
      decoder->Uninitialize();
      WelsDestroyDecoder(decoder);
      return false;
    }

  textureY = new GRPTEXTUREEXT();
  textureU = new GRPTEXTUREEXT();
  textureV = new GRPTEXTUREEXT();
  if(!textureY || !textureU || !textureV)
    {
      decoder->Uninitialize();
      WelsDestroyDecoder(decoder);
      xfactory->Delete_Mutex(xmutex);
      return false;
    }

  //textureY->Create(NULL, width,   height,   GRPTEXTURE_FORMAT_R);
  //textureU->Create(NULL, width/2, height/2, GRPTEXTURE_FORMAT_R);
  //textureV->Create(NULL, width/2, height/2, GRPTEXTURE_FORMAT_R);

  textureY->Create(NULL, width,   height,   GRPTEXTURE_FORMAT_R);
  textureU->Create(NULL, width/2, height/2, GRPTEXTURE_FORMAT_R);
  textureV->Create(NULL, width/2, height/2, GRPTEXTURE_FORMAT_R);

  // now create the material and quad to render to the scene
  GRPMATERIALLIBRARY* matlibrary = videoplayer->GetRenderer()->GetScene(1)->GetMaterialLibrary();
  if(!matlibrary) return false;
  GRPMATERIAL* mat = new GRPMATERIAL();

  mat->SetName(__L("VideroRender"));
  mat->AddLayer(GRPMATERIAL_LAYER_DIFFUSECOLOR, textureY);
  mat->AddLayer(GRPMATERIAL_LAYER_BUMPMAP, textureU);
  mat->AddLayer(GRPMATERIAL_LAYER_NORMAL, textureV);

  matlibrary->Add(mat);

  GRPOBJECT* quad = new GRPOBJECT();
  quad->SetNode(this->videoplayer->GetRenderer()->GetScene(1)->CreateNode());

  surface = videoplayer->GetRenderer()->GetObjectBuilder()->CreatePlane(GRPOBJECTBUILDER::AXIS_X);
  surface->SetNode(quad->GetNode());

  surface->GetNode()->Init();
  surface->GetNode()->SetMaterial(mat);
  surface->SetProgram(videoplayer->GetRenderer()->GetScene(1)->GetShaderLibrary()->GetProgramByKey((char*)"VID"));

  viewport = videoplayer->GetRenderer()->GetScene(1)->GetViewport();

  //// video aspect ratio
  //float vaspect = ((float)height)/((float)width);
  //float paspect = 1.0f/viewport->GetAspectRatio();
  //
  //float xs = 1.0f;
  //float ys = 1.0f;
  //
  //if(vaspect <= paspect)
  //{
  //  // perfect for vertical bars or just fit
  //  ys = vaspect;
  //}
  //else
  //{
  //  // video is convined and need horizontal bars(ugh ugh ugh)
  //  // need to touch up xs
  //  ys = paspect;
  //  xs = ys/vaspect; // ??
  //}
  //
  ////surface->GetNode()->SetScale(1.0f, (vaspect/viewport->GetAspectRatio()), 1.0f);
  //surface->GetNode()->SetScale(xs, ys, 1.0f);
  ////surface->GetNode()->SetRotation(0.0f, 0.0f, 25.0f);

  videoplayer->GetRenderer()->GetScene(1)->Add(quad);
  videoplayer->GetRenderer()->GetScene(1)->Add(surface);


  viewport->SetSize(1.0f, 1.0f);
  viewport->SetPosition(0.0f, 0.0f);
  viewport->SetProjection(GRPPROJECTIONTYPE_ORTHO);
  viewport->SelectProjectionOrtho();
  viewport->Select();

  // Setup the Viewport

  videoplayer->GetRenderer()->GetScene(1)->GetCamera()->SetPosition(0.0f, 0.0f, 140.0f);
  videoplayer->GetRenderer()->GetScene(1)->GetCamera()->GetTarget()->SetPosition(0.0f, 0.0f, 0.0f);

  isplaying = true;

  fps = 25.0f;

  return true;
}





/*-------------------------------------------------------------------
//  VIDEOWINDOWSCODECH264OPENH264::End
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      19/04/2016 11:17:42
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool VIDEOWINDOWSCODECH264OPENH264::End()
{
  if(decoder)
    {
      decoder->Uninitialize();
      WelsDestroyDecoder(decoder);
    }

  if(xtimer)
  {
    xfactory->DeleteTimer(xtimer);
  }

  if(xmutex)
    {
      xfactory->Delete_Mutex(xmutex);
    }

  if(textureY)
    {
      delete textureY;
    }

  return true;
}





/*-------------------------------------------------------------------
//  VIDEOWINDOWSCODECH264OPENH264::SendFrame
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      19/04/2016 11:17:49
//
//  @return       bool :
//
//  @param        frame :
*/
/*-----------------------------------------------------------------*/
bool VIDEOWINDOWSCODECH264OPENH264::SendFrame(XVECTOR<XBYTE> & frame)
{
  if(!decoder) return false;
  //if(ready) return false;

  DECODING_STATE state = dsErrorFree;

  if((state = decoder->DecodeFrame2(frame.GetPointer(0), frame.GetSize()*sizeof(XBYTE), yuvdata, &sbufferinfo)) != 0)
    {
      return false;
    }
  //if(0 != decoder->DecodeFrameNoDelay(frame.GetPointer(0), frame.GetSize()*sizeof(XBYTE), yuvdata, &sbufferinfo)) return false;

  // Lock Mutex
  xmutex->Lock();
  // copy the data to the actual xbuffer to upload
  // only luminance for now
  if(sbufferinfo.iBufferStatus == 1)
    {
      Y.Resize(sbufferinfo.UsrData.sSystemBuffer.iWidth*sbufferinfo.UsrData.sSystemBuffer.iHeight);
      // try to copy the frame with stride
      for(XDWORD i = 0; i < (XDWORD)sbufferinfo.UsrData.sSystemBuffer.iHeight; i++)
        {
          memcpy(Y.Get() + sbufferinfo.UsrData.sSystemBuffer.iWidth*i, yuvdata[0] + sbufferinfo.UsrData.sSystemBuffer.iStride[0]*i, sbufferinfo.UsrData.sSystemBuffer.iWidth);
        }

      textureY->Resize(sbufferinfo.UsrData.sSystemBuffer.iWidth, sbufferinfo.UsrData.sSystemBuffer.iHeight);

      U.Resize((sbufferinfo.UsrData.sSystemBuffer.iWidth/2)*(sbufferinfo.UsrData.sSystemBuffer.iHeight/2));
      V.Resize((sbufferinfo.UsrData.sSystemBuffer.iWidth/2)*(sbufferinfo.UsrData.sSystemBuffer.iHeight/2));
      for(XDWORD i = 0; i < (XDWORD)(sbufferinfo.UsrData.sSystemBuffer.iHeight/2); i++)
        {
          memcpy(U.Get() + (sbufferinfo.UsrData.sSystemBuffer.iWidth/2)*i, yuvdata[1] + sbufferinfo.UsrData.sSystemBuffer.iStride[1]*i, (sbufferinfo.UsrData.sSystemBuffer.iWidth/2));
          memcpy(V.Get() + (sbufferinfo.UsrData.sSystemBuffer.iWidth/2)*i, yuvdata[2] + sbufferinfo.UsrData.sSystemBuffer.iStride[1]*i, (sbufferinfo.UsrData.sSystemBuffer.iWidth/2));
        }

      textureU->Resize(sbufferinfo.UsrData.sSystemBuffer.iWidth/2, sbufferinfo.UsrData.sSystemBuffer.iHeight/2);
      textureV->Resize(sbufferinfo.UsrData.sSystemBuffer.iWidth/2, sbufferinfo.UsrData.sSystemBuffer.iHeight/2);

      ready = true;

      height = sbufferinfo.UsrData.sSystemBuffer.iHeight;
      width = sbufferinfo.UsrData.sSystemBuffer.iWidth;
    }
  else
    {
      // let's hope so?
      if(framereader->IsEndOfStream())
        {
          isplaying = false;
        }
    }

  // UnLock Mutex
  xmutex->UnLock();

  return true;
}






/*-------------------------------------------------------------------
//  VIDEOWINDOWSCODECH264OPENH264::Decode
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      19/04/2016 11:17:56
//
*/
/*-----------------------------------------------------------------*/
void VIDEOWINDOWSCODECH264OPENH264::Decode()
{
  if(!initialized)
  {
    initialized = true;
  }

  xmutex->Lock();
  bool localready = ready;
  xmutex->UnLock();


  // localready is not really useful as then decoding happens as fast as possible :(
  // but this works correctly if using mp4 as container(something seems different somehwre)
  if((timestamp + xtimer->GetMeasureMicroSeconds() > (((1.0/fps)*1000*1000) + timestamp)) || !localready)
    {
      // Read Frame
      XVECTOR<XBYTE> frame;
      framereader->ReadFrame(frame);

      // Send Frame for decoding
      SendFrame(frame);
      timestamp += xtimer->GetMeasureMicroSeconds();
      xtimer->Reset();
    }
}







/*-------------------------------------------------------------------
//  VIDEOWINDOWSCODECH264OPENH264::Update
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      22/04/2016 9:22:44
//
*/
/*-----------------------------------------------------------------*/
void VIDEOWINDOWSCODECH264OPENH264::Update()
{
  if(!textureY || !textureU || !textureV) return;

  xmutex->Lock();
  if(ready)
    {
      textureY->LoadToGPU(&Y);
      textureU->LoadToGPU(&U);
      textureV->LoadToGPU(&V);
      GRPOPENGLCTRL::CurrentTexture = NULL;
      ready = false;

      // video aspect ratio
      if(width != 0)
        {
          float vaspect = ((float)height)/((float)width);
          float paspect = 1.0f/viewport->GetAspectRatio();

          float xs = 1.0f;
          float ys = 1.0f;

          if(vaspect <= paspect)
            {
              // perfect for vertical bars or just fit
              ys = vaspect;
            }
          else
            {
              // video is convined and need horizontal bars(ugh ugh ugh)
              // need to touch up xs
              ys = paspect;
              xs = ys/vaspect; // ??
            }

          //surface->GetNode()->SetScale(1.0f, (vaspect/viewport->GetAspectRatio()), 1.0f);
          surface->GetNode()->SetScale(xs, ys, 1.0f);
          //surface->GetNode()->SetRotation(0.0f, 0.0f, 25.0f);
        }
    }

  xmutex->UnLock();
}
