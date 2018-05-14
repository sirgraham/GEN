/*------------------------------------------------------------------------------------------
//  INPCURSOR.CPP
//
//  Input Cursor Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 21/12/2011 12:03:46
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


//---- INCLUDES ----------------------------------------------------------------------------

#include <string.h>


#include "XFactory.h"
#include "XTimer.h"

#include "INPCursor.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------



//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  INPCURSORMOTION::INPCURSORMOTION
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/12/2011 17:27:28
//
//  @return
//  */
/*-----------------------------------------------------------------*/
INPCURSORMOTION::INPCURSORMOTION() : INPPATH()
{
  Clean();
}


/*-------------------------------------------------------------------
//  INPCURSORMOTION::~INPCURSORMOTION
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/12/2011 17:27:19
//
//  @return
//  */
/*-----------------------------------------------------------------*/
INPCURSORMOTION::~INPCURSORMOTION()
{
  DeleteAllPoints();

  Clean();
}



/*-------------------------------------------------------------------
//  INPCURSORMOTION::IsInCurse
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/12/2011 17:27:15
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool INPCURSORMOTION::IsInCurse()
{
  return isincurse;
}



/*-------------------------------------------------------------------
//  INPCURSORMOTION::SetActive
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/12/2011 17:27:02
//
//  @return       bool :
//  @param        isincurse :
*/
/*-----------------------------------------------------------------*/
bool INPCURSORMOTION::SetIsInCurse(bool isincurse)
{
  this->isincurse = isincurse;

  return true;
}


/*-------------------------------------------------------------------
//  INPCURSORMOTION::IsReadyToTest
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/04/2012 16:22:39
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool INPCURSORMOTION::IsReadyToTest()
{
  if((!isincurse) && (!points.IsEmpty())) return true;

  return false;
}


/*-------------------------------------------------------------------
//  INPCURSORMOTION::GetDirection
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/12/2011 17:26:39
//
//  @return       INPCURSOR_MOTIONDIR :
//  */
/*-----------------------------------------------------------------*/
INPCURSOR_MOTIONDIR INPCURSORMOTION::GetDirection(bool indetail)
{
  if(!Get())             return INPCURSOR_MOTIONDIR_UNKNOWN;
  if(Get()->GetSize()<2) return INPCURSOR_MOTIONDIR_UNKNOWN;

  float angle = GetAngle();

  if(indetail)
    {
      if((angle > 337.5f) || (angle <=  22.5f))  return INPCURSOR_MOTIONDIR_RIGHT;
      if((angle >  22.5f) && (angle <=  67.5f))  return INPCURSOR_MOTIONDIR_RIGHTUP;
      if((angle >  67.5f) && (angle <= 112.5f))  return INPCURSOR_MOTIONDIR_UP;
      if((angle > 112.5f) && (angle <= 157.5f))  return INPCURSOR_MOTIONDIR_LEFTUP;
      if((angle > 157.5f) && (angle <= 202.5f))  return INPCURSOR_MOTIONDIR_LEFT;
      if((angle > 202.5f) && (angle <= 247.5f))  return INPCURSOR_MOTIONDIR_LEFTDOWN;
      if((angle > 247.5f) && (angle <= 292.5f))  return INPCURSOR_MOTIONDIR_DOWN;
      if((angle > 292.5f) && (angle <= 337.5f))  return INPCURSOR_MOTIONDIR_RIGHTDOWN;
    }
   else
    {
      if((angle > 270) || (angle <=  45))        return INPCURSOR_MOTIONDIR_RIGHT;
      if((angle >  45) && (angle <= 135))        return INPCURSOR_MOTIONDIR_UP;
      if((angle > 135) && (angle <= 180))        return INPCURSOR_MOTIONDIR_LEFT;
      if((angle > 180) && (angle <= 270))        return INPCURSOR_MOTIONDIR_DOWN;
    }

  return INPCURSOR_MOTIONDIR_UNKNOWN;
}



/*-------------------------------------------------------------------
//  INPCURSORMOTION::GetTimeElapsed
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/12/2011 19:05:34
//
//  @return       double :
//  */
/*-----------------------------------------------------------------*/
XQWORD INPCURSORMOTION::GetTimeElapsed()
{
  return timeelapsed;
}



/*-------------------------------------------------------------------
//  INPCURSORMOTION::SetTimeElapsed
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/12/2011 19:08:40
//
//  @return       bool :
//  @param        timeelapsed :
*/
/*-----------------------------------------------------------------*/
bool INPCURSORMOTION::SetTimeElapsed(XQWORD timeelapsed)
{
  this->timeelapsed = timeelapsed;

  return true;
}




/*-------------------------------------------------------------------
//  INPCURSORMOTION::Reset
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/12/2011 20:10:16
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool INPCURSORMOTION::Reset()
{
  isincurse   = false;
  timeelapsed = 0;

  DeleteAllPoints();

  return true;
}




/*-------------------------------------------------------------------
//  INPCURSORMOTION::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/12/2011 17:26:34
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void INPCURSORMOTION::Clean()
{
  isincurse   = false;
  timeelapsed = 0;
}





//-------------------------------------------------------------------
//  INPCURSOR::INPCURSOR
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2003 17:34:42
//
//  @return
//
*/
//-------------------------------------------------------------------
INPCURSOR::INPCURSOR()
{
  Clean();

  timer=xfactory->CreateTimer();
}




//-------------------------------------------------------------------
//  INPCURSOR::~INPCURSOR
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2003 17:35:03
//
//  @return
//  */
//-------------------------------------------------------------------
INPCURSOR::~INPCURSOR()
{
  if(timer)
    {
      xfactory->DeleteTimer(timer);
    }

  Clean();
}



/*-------------------------------------------------------------------
//  INPCURSOR::GetType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/04/2012 11:10:52
//
//  @return       INPCURSOR_ID :
//  */
/*-----------------------------------------------------------------*/
INPCURSOR_ID INPCURSOR::GetID()
{
  return ID;
}



//-------------------------------------------------------------------
//  INPCURSOR::SetID
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2003 17:40:21
//
//  @return       void :
//  @param        type :
*/
//-------------------------------------------------------------------
void INPCURSOR::SetID(INPCURSOR_ID ID)
{
  this->ID = ID;
}



/*-------------------------------------------------------------------
//  INPCURSOR::GetTimer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/12/2011 12:06:36
//
//  @return       double :
//  */
/*-----------------------------------------------------------------*/
XTIMER* INPCURSOR::GetTimer()
{
  return timer;
}



/*-------------------------------------------------------------------
//  INPCURSOR::HavePreSelect
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/01/2012 9:55:07
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool INPCURSOR::HavePreSelect()
{
  return havepreselect;
}



/*-------------------------------------------------------------------
//  INPCURSOR::SetHavePreSelect
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/01/2012 9:55:38
//
//  @return       bool :
//  @param        havepreselect :
*/
/*-----------------------------------------------------------------*/
bool INPCURSOR::SetHavePreSelect(bool havepreselect)
{
  this->havepreselect = havepreselect;

  return true;
}



/*-------------------------------------------------------------------
//  INPCURSOR::IsChanged
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/12/2011 19:17:17
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool INPCURSOR::IsChanged()
{
  return ischanged;
}



/*-------------------------------------------------------------------
//  INPCURSOR::SetIsChanged
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/12/2011 19:17:21
//
//  @return       bool :
//  @param        ischanged :
*/
/*-----------------------------------------------------------------*/
bool INPCURSOR::SetIsChanged(bool ischanged)
{
  this->ischanged = ischanged;

  return true;
}


/*-------------------------------------------------------------------
//  INPCURSOR::IsPositionInRect
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/01/2012 9:58:51
//
//  @return       bool :
//  @param        x :
//  @param        y :
//  @param        width :
//  @param        height :
*/
/*-----------------------------------------------------------------*/
bool INPCURSOR::IsPositionInRect(int x, int y, int width,int height)
{
  if((GetX() >= x) && (GetX() <= (x+width)) &&
     (GetY() >= y) && (GetY() <= (y+height))) return true;

  return false;
}



/*-------------------------------------------------------------------
//  INPCURSOR::GetMotion
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/12/2011 17:33:27
//
//  @return       INPCURSORMOTION* :
//  */
/*-----------------------------------------------------------------*/
INPCURSORMOTION* INPCURSOR::GetMotion()
{
  return &motion;
}



/*-------------------------------------------------------------------
//  INPDEVICE::AddToCursorMotion
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/04/2012 16:08:33
//
//  @return       bool :
//  @param        ispressed :
//  @param        cursor :
*/
/*-----------------------------------------------------------------*/
bool INPCURSOR::AddPointToMotion(bool ispressed)
{
  if(ispressed)
    {
      if(!motion.IsInCurse())
        {
          motion.Reset();
          motion.SetIsInCurse(true);
        }

      motion.AddPoint(GetX(),GetY(),GetZ(),true);
    }
   else
    {
      if(motion.IsInCurse())
        {
          motion.AddPoint(GetX(),GetY(),GetZ(),true);

          motion.SetTimeElapsed(GetTimer()->GetMeasureMilliSeconds());
          motion.SetIsInCurse(false);
        }
    }

  return true;
}



void INPCURSOR::Set(float x , float y, float z)
{
  float xx=(x-this->source.originx)/(this->source.endx-this->source.originx);
        xx=xx*(this->destination.endx-this->destination.originx)+this->destination.originx;
  float yy=(y-this->source.originy)/(this->source.endy-this->source.originy);
        yy=yy*(this->destination.endy-this->destination.originy)+this->destination.originy;

  this->x=xx;
  this->y=yy;
  this->z=z;
}

void INPCURSOR::SetX(float x)
{
  float xx=(x-this->source.originx)/(this->source.endx-this->source.originx);
        xx=xx*(this->destination.endx-this->destination.originx)+this->destination.originx;
  this->x=xx;
}

void INPCURSOR::SetY(float y)
{
  float yy=(y-this->source.originy)/(this->source.endy-this->source.originy);
        yy=yy*(this->destination.endy-this->destination.originy)+this->destination.originy;
  this->y=y;
}

void INPCURSOR::SetZ(float z)
{
  this->z=z;
}

//-------------------------------------------------------------------
//  INPCURSOR::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2003 17:37:08
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void INPCURSOR::Clean()
{
  timer         = NULL;

  ID            = INPCURSOR_ID_NONE;

  ischanged     = false;
  havepreselect = false;
};
