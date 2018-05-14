//------------------------------------------------------------------------------------------
//  GRPBITMAPSEQUENCE.CPP
//
//  Graphics Bitmap Sequence class
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 28/11/2003 16:16:44
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XTimer.h"

#include "GRPRect.h"
#include "GRPBitmap.h"

#include "GRPBitmapSequence.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::GRPBITMAPSEQUENCE
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/02/2004 11:22:42
//
//  @return       void :

*/
//-------------------------------------------------------------------
GRPBITMAPSEQUENCE::GRPBITMAPSEQUENCE()
{
  Clean();

  timer = xfactory->CreateTimer();
  AdjustTime(GRPBITMAPSEQUENCE_FRAMESSECOND);
}



//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::~GRPBITMAPSEQUENCE
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2003 16:24:51
//
//  @return
//  */
//-------------------------------------------------------------------
GRPBITMAPSEQUENCE::~GRPBITMAPSEQUENCE()
{
  if(timer) xfactory->DeleteTimer(timer);

  DelAllSequence();

  Clean();
}



//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::AddFrame
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2003 16:32:35
//
//  @return       bool :
//  @param        bitmap :
//  @param        adjustx :
//  @param        adjusty :
*/
//-------------------------------------------------------------------
bool GRPBITMAPSEQUENCE::AddFrame(GRPBITMAP* bitmap,int adjustx,int adjusty)
{
  GRPBITMAPFRAME* frame = new GRPBITMAPFRAME();
  if(!frame) return false;

  frame->SetBitmap(bitmap);
  frame->SetAdjustX(adjustx);
  frame->SetAdjustY(adjusty);

  frames.Add(frame);

  return true;
}



//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::DelAllSequence
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/01/2004 16:21:34
//
//  @return       bool :
//  @param        full :
*/
//-------------------------------------------------------------------
bool GRPBITMAPSEQUENCE::DelAllSequence(bool full)
{
  if(frames.IsEmpty())  return false;

  if(full) frames.DeleteContents();

  frames.DeleteAll();

  return true;
}



//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::GetActualFrame
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2003 16:31:18
//
//  @return       GRPBITMAPFRAME* :
//  */
//-------------------------------------------------------------------
GRPBITMAPFRAME* GRPBITMAPSEQUENCE::GetActualFrame()
{
  return (GRPBITMAPFRAME*)frames.Get(frameindex);
}


//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::GetActualFrameIndex
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2004 17:39:17
//
//  @return       int :
//  */
//-------------------------------------------------------------------
int GRPBITMAPSEQUENCE::GetActualFrameIndex()
{
  return frameindex;
}


//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::GetFrame
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/01/2004 11:03:14
//
//  @return       GRPBITMAPFRAME* :
//  @param        nframe :
*/
//-------------------------------------------------------------------
GRPBITMAPFRAME* GRPBITMAPSEQUENCE::GetFrame(XWORD nframe)
{
  if(nframe>=(XWORD)frames.GetSize()) return NULL;
  return (GRPBITMAPFRAME*)frames.Get(nframe);
}


//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::GetNFrames
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/01/2004 12:55:29
//
//  @return       int :
//  */
//-------------------------------------------------------------------
int GRPBITMAPSEQUENCE::GetNFrames()
{
  return frames.GetSize();
}



/*-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::GetNLoops
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/01/2012 18:27:26
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int GRPBITMAPSEQUENCE::GetNLoops()
{
  return nloops;
}



/*-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::GetNLoopsMade
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/01/2012 18:27:01
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int GRPBITMAPSEQUENCE::GetNLoopsMade()
{
  return nloopsmade;
}



/*-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::SetNLoops
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/01/2012 18:26:22
//
//  @return       bool :
//  @param        nloops :
*/
/*-----------------------------------------------------------------*/
bool GRPBITMAPSEQUENCE::SetNLoops(int nloops)
{
  this->nloops = nloops;

  return true;
}



/*-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::SetNLoopsMade
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/01/2012 19:45:01
//
//  @return       bool :
//  @param        nloopsmade :
*/
/*-----------------------------------------------------------------*/
bool GRPBITMAPSEQUENCE::SetNLoopsMade(int nloopsmade)
{
  this->nloopsmade = nloopsmade;

  return true;
}



//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::AdjustAllSequence
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/02/2004 18:10:39
//
//  @return       int :
//  @param        adjustx :
//  @param        adjusty :
*/
//-------------------------------------------------------------------
bool GRPBITMAPSEQUENCE::AdjustAllSequence(int adjustx,int adjusty)
{
  if(frames.IsEmpty())  return false;

  for(XDWORD c=0;c<frames.GetSize();c++)
    {
      GRPBITMAPFRAME* frame = (GRPBITMAPFRAME*)frames.Get(c);

      if(frame)
        {
          frame->SetAdjustX(adjustx);
          frame->SetAdjustY(adjusty);
        }
    }


  return true;
}



//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::Play
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/10/2004 17:21:37
//
//  @return       GRPBITMAPFRAME* :
//  @param        nloops :
//  @param        adjusttimeauto :
//  @param        reverse :
*/
//-------------------------------------------------------------------
GRPBITMAPFRAME* GRPBITMAPSEQUENCE::Play(int nloops, bool adjusttimeauto, bool reverse)
{
  if(status==GRPBITMAPSEQUENCESTATUS_PLAY) return NULL;

  this->nloops = nloops;

  status = GRPBITMAPSEQUENCESTATUS_PLAY;

  if(adjusttimeauto) AdjustTime(frames.GetSize());

  this->reverse = reverse;

  if(!reverse)
        frameindex = 0;
   else frameindex = frames.GetSize()-1;

  nloopsmade = 0;

  return (GRPBITMAPFRAME*)frames.Get(frameindex);
}




//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::Stop
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2003 16:26:55
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool GRPBITMAPSEQUENCE::Stop()
{
  if(frames.IsEmpty())                       return false;
  if(status==GRPBITMAPSEQUENCESTATUS_NONE) return false;

  status = GRPBITMAPSEQUENCESTATUS_STOP;

  if(!reverse)
        frameindex = 0;
   else frameindex = (frames.GetSize()-1);

  nloopsmade   = 0;

  return true;
}


//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::Pause
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2003 16:27:04
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool GRPBITMAPSEQUENCE::Pause()
{
  if(frames.IsEmpty())                     return false;
  if(status!=GRPBITMAPSEQUENCESTATUS_PLAY) return false;

  status = GRPBITMAPSEQUENCESTATUS_PAUSE;

  return true;
}


//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::Finished
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/02/2004 11:19:23
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool GRPBITMAPSEQUENCE::Finished()
{
  if(frames.IsEmpty())                     return false;
  if(status!=GRPBITMAPSEQUENCESTATUS_PLAY) return false;

  status = GRPBITMAPSEQUENCESTATUS_END;

  return true;
}



//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::IsPlaying
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/01/2004 10:24:12
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool GRPBITMAPSEQUENCE::IsPlaying()
{
  if(status==GRPBITMAPSEQUENCESTATUS_PLAY) return true;

  return false;
}


//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::IsEndSequence
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/01/2004 17:06:15
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool GRPBITMAPSEQUENCE::IsEndSequence()
{
  if(status==GRPBITMAPSEQUENCESTATUS_END) return true;

  return false;
}



//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::Update
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/01/2004 16:39:48
//
//  @return       GRPBITMAPFRAME* :
//  */
//-------------------------------------------------------------------
GRPBITMAPFRAME* GRPBITMAPSEQUENCE::Update()
{
  if(!timer)                               return NULL;
  if(frames.IsEmpty())                     return NULL;
  if(status!=GRPBITMAPSEQUENCESTATUS_PLAY) return NULL;

  if(timer->GetMeasureMilliSeconds()<timebyframe)  return NULL;

  timer->Reset();

  if(!reverse)
    {
      if(frames.GetSize()!=1) frameindex++;

      if(frameindex == frames.GetSize())
        {
          frameindex = 0;
          nloopsmade++;
          if((nloops)&&(nloops==nloopsmade)) status=GRPBITMAPSEQUENCESTATUS_END;
        }
    }
   else
    {
      if(frames.GetSize()!=1) frameindex--;

      if(frameindex<0)
        {
          frameindex = (frames.GetSize()-1);

          nloopsmade++;
          if((nloops)&&(nloops==nloopsmade)) status=GRPBITMAPSEQUENCESTATUS_END;
        }
    }

  return (GRPBITMAPFRAME*)frames.Get(frameindex);
}



//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::AdjustTime
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/02/2004 11:57:27
//
//  @return       bool :
//  @param        int framebysec :
*/
//-------------------------------------------------------------------
bool GRPBITMAPSEQUENCE::AdjustTime(int  framebysec)
{
  if(!timer) return false;

  this->framebysec  = framebysec;
  timebyframe       = ((float)1000/(float)framebysec);

  return true;
}



//-------------------------------------------------------------------
//  GRPBITMAPSEQUENCE::Clean
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2003 16:27:18
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void GRPBITMAPSEQUENCE::Clean()
{
  framebysec    = 0;
  timebyframe   = 0;
  timer         = NULL;

  repeat        = false;
  status        = GRPBITMAPSEQUENCESTATUS_NONE;
  frameindex    = 0;

  nloops        = 0;
  nloopsmade    = 0;

  reverse       = false;
}

