//------------------------------------------------------------------------------------------
//  INPBUTTON.CPP
//
//  Input Button class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 11/02/2003
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <string.h>

#include "XTimer.h"
#include "XFactory.h"

#include "INPButton.h"

#include "XDebugTrace.h"
#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------



//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  INPBUTTON::INPBUTTON
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
INPBUTTON::INPBUTTON()
{
  Clean();

  xtimer=xfactory->CreateTimer();
}




//-------------------------------------------------------------------
//  INPBUTTON::~INPBUTTON
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2003 17:35:03
//
//  @return
//  */
//-------------------------------------------------------------------
INPBUTTON::~INPBUTTON()
{
  if(xtimer) xfactory->DeleteTimer(xtimer);

  Clean();
}



//-------------------------------------------------------------------
//  INPBUTTON::GetID
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2003 17:35:27
//
//  @return       char* :
//  */
//-------------------------------------------------------------------
INPBUTTON_ID INPBUTTON::GetID()
{
  return ID;
}


//-------------------------------------------------------------------
//  INPBUTTON::SetID
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2003 17:40:21
//
//  @return       void :
//  @param        ID :
*/
//-------------------------------------------------------------------
void INPBUTTON::SetID(INPBUTTON_ID ID)
{
  this->ID = ID;
}




/*-------------------------------------------------------------------
//  INPBUTTON::GetKeyCode
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/11/2009 12:51:55 p.m.
//
//  @return       XWORD :
//  */
/*-----------------------------------------------------------------*/
XWORD INPBUTTON::GetKeyCode()
{
  return keycode;
}


/*-------------------------------------------------------------------
//  INPBUTTON::SetCode
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/11/2009 12:52:16 p.m.
//
//  @return       void :
//  @param        keycode :
*/
/*-----------------------------------------------------------------*/
void INPBUTTON::SetKeyCode(XWORD keycode)
{
  this->keycode = keycode;
}


/*-------------------------------------------------------------------
//  INPBUTTON::GetSymbol
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/11/2009 12:53:15 p.m.
//
//  @return       XCHAR :
//  */
/*-----------------------------------------------------------------*/
XCHAR INPBUTTON::GetSymbol()
{
  return symbol;
}



/*-------------------------------------------------------------------
//  INPBUTTON::SetSymbol
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/11/2009 12:53:07 p.m.
//
//  @return       void :
//  @param        symbol :
*/
/*-----------------------------------------------------------------*/
void INPBUTTON::SetSymbol(XCHAR symbol)
{
  this->symbol = symbol;
}

float  INPBUTTON::GetPressure           ()
{
return pressure;
}

void   INPBUTTON::SetPressure           (float p)
{
  this->pressure=p;
}


//-------------------------------------------------------------------
//  INPBUTTON::IsPressed
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2003 17:36:12
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool INPBUTTON::IsPressed()
{
  isread    = true;

  bool pbtn = pressed;

  if(marktorelase)
    {
      SetPressed(false);
      marktorelase = false;
    }

  return pbtn;
}


//-------------------------------------------------------------------
//  INPBUTTON::IsPressedWithRelease()
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2003 17:36:12
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool INPBUTTON::IsPressedWithRelease()
{
  if(!IsPressed()) return false;

  SetPressed(false);

  return true;
}


//-------------------------------------------------------------------
//  INPBUTTON::IsRead
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/07/2004 12:42:48
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool INPBUTTON::IsRead()
{
  return isread;
}


//-------------------------------------------------------------------
//  INPBUTTON::SetPressed
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2003 17:36:36
//
//  @return       void :
//  @param        on :
*/
//-------------------------------------------------------------------
void INPBUTTON::SetPressed(bool on)
{
  /*
  if (on)
  {
    switch(state)
    {
    case INPBUTTON_STATE_UNKNOWN:         state=INPBUTTON_STATE_PRESSED;          XDEBUGTRACE_PRINTCOLOR(0,__L("INPBUTTON_STATE_PRESSED %d"),this->GetKeyCode());break;
    case INPBUTTON_STATE_RELEASED:        state=INPBUTTON_STATE_PRESSED;          XDEBUGTRACE_PRINTCOLOR(0,__L("INPBUTTON_STATE_PRESSED %d"),this->GetKeyCode());break;
    case INPBUTTON_STATE_UP:              state=INPBUTTON_STATE_PRESSED;          XDEBUGTRACE_PRINTCOLOR(0,__L("INPBUTTON_STATE_PRESSED %d"),this->GetKeyCode()); break;
    case INPBUTTON_STATE_PRESSED:         state=INPBUTTON_STATE_HOLD;             break;
    case INPBUTTON_STATE_HOLD:            state=INPBUTTON_STATE_HOLD;             break;
    }
  }
  else
  {
    switch(state)
    {
    case INPBUTTON_STATE_UNKNOWN:         state=INPBUTTON_STATE_RELEASED;         XDEBUGTRACE_PRINTCOLOR(0,__L("INPBUTTON_STATE_RELEASED %d"),this->GetKeyCode());;break;
    case INPBUTTON_STATE_RELEASED:        state=INPBUTTON_STATE_UP;               break;
    case INPBUTTON_STATE_UP:              state=INPBUTTON_STATE_UP;               break;
    case INPBUTTON_STATE_PRESSED:         state=INPBUTTON_STATE_RELEASED;         XDEBUGTRACE_PRINTCOLOR(0,__L("INPBUTTON_STATE_RELEASED %d"),this->GetKeyCode());;break;
    case INPBUTTON_STATE_HOLD:            state=INPBUTTON_STATE_RELEASED;         XDEBUGTRACE_PRINTCOLOR(0,__L("INPBUTTON_STATE_RELEASED %d"),this->GetKeyCode());;break;
    }
  }
*/

  if(pressed != on)
    {
      if(on)
        {
          if(xtimer) ResetTimePressed();
        }
    }

  pressed = on;
  if(on) isread = false;
}


//-------------------------------------------------------------------
//  INPBUTTON::IsMarkToRelase
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/07/2004 13:27:53
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool INPBUTTON::IsMarkToRelase()
{
  return marktorelase;
}


//-------------------------------------------------------------------
//  INPBUTTON::SetMarkToRelase
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/07/2004 13:28:07
//
//  @return       bool :
//  @param        on :
*/
//-------------------------------------------------------------------
bool INPBUTTON::SetMarkToRelase(bool on)
{
  marktorelase = on;

  return true;
}



//-------------------------------------------------------------------
//  INPBUTTON::GetTimePressed
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 17:10:41
//
//  @return       double :
//  */
//-------------------------------------------------------------------
XQWORD INPBUTTON::GetTimePressed()
{
  if(!pressed) return 0;
  if(!xtimer)  return 0;

  return xtimer->GetMeasureMilliSeconds();
}


//-------------------------------------------------------------------
//  INPBUTTON::SetTimePressed
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/07/2004 13:12:41
//
//  @return       bool :
//  @param        time :
*/
//-------------------------------------------------------------------
bool INPBUTTON::SetTimePressed(XDWORD time)
{
  if(!xtimer) return false;

  xtimer->Reset();
  xtimer->AddMilliSeconds(time);

  return true;
}


//-------------------------------------------------------------------
//  INPBUTTON::ResetTimePressed
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/05/2004 21:12:27
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool INPBUTTON::ResetTimePressed()
{
  if(!xtimer) return false;

  xtimer->Reset();

  return true;
}



/*-------------------------------------------------------------------
//  INPBUTTON::CreateButton
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/01/2012 11:39:04
//
//  @return       bool :
//  @param        buttons :
//  @param        keycode :
//  @param        ID :
//  @param        symbol :
*/
/*-----------------------------------------------------------------*/
bool INPBUTTON::CreateButton( XVECTOR<INPBUTTON*>* buttons, int keycode, INPBUTTON_ID ID, XCHAR symbol)
{
  if(!buttons) return false;

  INPBUTTON* button;

  button = new INPBUTTON();
  if(!button) return false;

  button->SetKeyCode(keycode);
  button->SetID(ID);
  button->SetSymbol(symbol);

  buttons->Add(button);

  return true;
}




//-------------------------------------------------------------------
//  INPBUTTON::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2003 17:37:08
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void INPBUTTON::Clean()
{
  keycode       = 0;
  symbol        = 0x00;

  pressed       = false;
  xtimer        = NULL;
  isread        = true;
  marktorelase  = false;

  pressure      = 0.0f;

  state         = INPBUTTON_STATE_UP;
};


