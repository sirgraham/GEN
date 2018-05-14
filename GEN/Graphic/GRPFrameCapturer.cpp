
/*------------------------------------------------------------------------------------------
//  GRPFRAMECAPTURER.CPP
//
//  captures a frame
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 17/05/2017 13:18:40
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPFrameCapturer.h"
#include "GRPRenderer.h"
#include "GRPScreen.h"
#include "GRPBitmap.h"
#include "GRPFactory.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  GRPFRAMECAPTURER::GRPFRAMECAPTURER
*/
/**
//
//  Class Constructor GRPFRAMECAPTURER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      17/05/2017 13:27:10
//
//  @param        renderer :
*/
/*-----------------------------------------------------------------*/
GRPFRAMECAPTURER::GRPFRAMECAPTURER(GRPRENDERER* renderer)
{
  Clean();

  this->renderer = renderer;

  datasize = renderer->GetMainScreen()->GetWidth()*renderer->GetMainScreen()->GetHeight()*4;
}




/*-------------------------------------------------------------------
//  GRPFRAMECAPTURER::~GRPFRAMECAPTURER
*/
/**
//
//   Class Destructor GRPFRAMECAPTURER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      17/05/2017 13:27:16
//
*/
/*-----------------------------------------------------------------*/
GRPFRAMECAPTURER::~GRPFRAMECAPTURER()
{
  Clean();
}



/*-------------------------------------------------------------------
//  GRPFRAMECAPTURER::Capture
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      17/05/2017 13:27:39
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPFRAMECAPTURER::Capture()
{
  if(framedata == NULL)
    {
      framedata = new XBYTE[datasize];
    }

  if(framedata)
    {
      glReadPixels(0, 0, renderer->GetMainScreen()->GetWidth(), renderer->GetMainScreen()->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE,  framedata);
    }

  return true;
}




/*-------------------------------------------------------------------
//  GRPFRAMECAPTURER::GetFrameData
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      17/05/2017 13:27:48
//
//  @return       XBYTE* :
//
*/
/*-----------------------------------------------------------------*/
XBYTE* GRPFRAMECAPTURER::GetFrameData()
{
  return framedata;
}




/*-------------------------------------------------------------------
//  GRPFRAMECAPTURER::CreateBitmap
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      17/05/2017 13:27:56
//
//  @return       GRPBITMAP* :
//
*/
/*-----------------------------------------------------------------*/
GRPBITMAP* GRPFRAMECAPTURER::CreateBitmap()
{
  if(framedata)
    {
      GRPBITMAP* ret = grpfactory->CreateBitmap(renderer->GetMainScreen()->GetWidth(), renderer->GetMainScreen()->GetHeight(), GRPPROPERTYMODE_32_RGBA_8888);
      if(ret == NULL)
        {
          return NULL;
        }

      memcpy(ret->GetBuffer(), framedata, datasize);
      return ret;
    }

  return NULL;
}
