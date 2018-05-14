/*------------------------------------------------------------------------------------------
//  INPWINDOWSDEVICEMOUSE.CPP
//
//  WINDOWS INPUT Device Mouse class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 21/12/2011 16:54:00
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <windows.h>

#include "GRPWINDOWSScreen.h"

#include "INPButton.h"
#include "INPCursor.h"

#include "INPWINDOWSDeviceMouse.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  INPWINDOWSDEVICEMOUSE::INPWINDOWSDEVICEMOUSE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/03/2012 12:20:23
//
//  @return

//  @param        grpscreen :
*/
/*-----------------------------------------------------------------*/
INPWINDOWSDEVICEMOUSE::INPWINDOWSDEVICEMOUSE( GRPWINDOWSSCREEN* grpscreen) : INPDEVICE()
{
  Clean();

  SetType(INPDEVICE_TYPE_MOUSE);

  SetScreen(grpscreen);
  created = true;

  if(CreateAllButtons())
    {
      if(CreateAllCursors())
        {
          SetEnabled(true);
        }
    }
}



/*-------------------------------------------------------------------
//  INPWINDOWSDEVICEMOUSE::~INPWINDOWSDEVICEMOUSE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/12/2011 17:26:28
//
//  @return
//  */
/*-----------------------------------------------------------------*/
INPWINDOWSDEVICEMOUSE::~INPWINDOWSDEVICEMOUSE()
{
  DeleteAllButtons();
  DeleteAllCursors();

  SetEnabled(false);
  created = false;

  Clean();
}

/*-------------------------------------------------------------------
//   INPWINDOWSDEVICEMOUSE::SetScreen
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return   bool :
//
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
bool INPWINDOWSDEVICEMOUSE::SetScreen(void* screenpointer)
{

    if (screenpointer) this->grpscreen = (GRPWINDOWSSCREEN*)screenpointer;
    return true;
}



/*-------------------------------------------------------------------
//  INPWINDOWSDEVICEMOUSE::GetButtons
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/12/2011 11:54:19
//
//  @return       XVECTOR* :
//  */
/*-----------------------------------------------------------------*/
XVECTOR<INPBUTTON*>* INPWINDOWSDEVICEMOUSE::GetButtons()
{
  return &buttons;
}



/*-------------------------------------------------------------------
//  INPWINDOWSDEVICEMOUSE::GetCursors
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/12/2011 11:54:25
//
//  @return       XVECTOR* :
//  */
/*-----------------------------------------------------------------*/
XVECTOR<INPCURSOR*>* INPWINDOWSDEVICEMOUSE::GetCursors()
{
  return &cursors;
}




/*-------------------------------------------------------------------
//  INPWINDOWSDEVICEMOUSE::Update
*/
/**
//
//
//  @author       Diego Martinez
//  @version      16/06/2017
//
//  @return       bool :
//  @param        void :
*/
/*-----------------------------------------------------------------*/
bool INPWINDOWSDEVICEMOUSE::Update()
{
  if(!IsEnabled()) return false;

  if(grpscreen)
    {
      if(grpscreen->GetHandle()!=GetForegroundWindow())
        {
          return false;
        }
    }


  // ----------------------------------------------------------------------------------
  // Update Cursor

  //INPCURSOR* cursor = GetCursor(INPCURSOR_ID_MOUSE);
  //if(cursor)

      float x=0.0f;
      float y=0.0f;

      POINT point;
      if(GetCursorPos(&point))
        {
          int screenx;
          int screeny;
          int screenw;
          int screenh;

          if(GetWindowPosition(screenx,screeny,screenw,screenh))
            {
               x = (float)(point.x - screenx);
               y = (float)(screeny - point.y);

              //cursor->SetIsChanged(false);

              if((x<0)||(y<0)||(x>screenw)||(y>screenh))
                {
                  x = -1;
                  y = -1;
                }
            }
        }

      //button = GetButton(INPBUTTON_ID_MOUSE_LEFT);
      //if(button) cursor->AddPointToMotion(button->IsPressed());





  // ----------------------------------------------------------------------------------
  int n=buttons.GetSize();
  for(int c=0;c<n;c++)
  {
    INPBUTTON* button = buttons.Get(c);
    if(button)
    {
      INPCURSOR* cursor=cursors.Get(c);
      if (cursor)
      {
        if (x!=cursor->GetX() || y!=cursor->GetY())
        {
        cursor->SetIsChanged(true);
        cursor->SetX(x);
        cursor->SetY(y);
        }
        else
          cursor->SetIsChanged(false);
      }


      SHORT state=GetAsyncKeyState(button->GetKeyCode());
      if (state & 0x8000) //key down
      {
        if (state & 0x01) //changed
        {
          button->SetPressed(true);

          if (button->state!=INPBUTTON_STATE_HOLD)
            button->state=INPBUTTON_STATE_PRESSED;

          if (cursor)
            cursor->AddPointToMotion(true);
        }
        else
        {
          button->SetPressed(true);
          button->state=INPBUTTON_STATE_HOLD;

          if (cursor)
            cursor->AddPointToMotion(true);

        }
      }
      else
      {
              if(button->IsRead())
                  button->SetPressed(false);
               else
                  button->SetMarkToRelase(true);


        if (button->state==INPBUTTON_STATE_HOLD || button->state==INPBUTTON_STATE_PRESSED) //changed
          button->state=INPBUTTON_STATE_RELEASED;
        else
            button->state=INPBUTTON_STATE_UP;

        if (cursor)
          cursor->AddPointToMotion(false);
      }
    }
  }

  /*
    // ----------------------------------------------------------------------------------
  // Update Cursor
  /*
  for(int c=0;c<(int)buttons.GetSize();c++)
    {
      INPBUTTON* button = (INPBUTTON*)buttons.Get(c);

      if(button)
        {
          if(GetKeyState(button->GetKeyCode()) & 0x80)
            {
              button->SetPressed(true);
            }
           else
            {
              if(button->IsRead())
                {
                  button->SetPressed(false);
                }
               else
                {
                  button->SetMarkToRelase(true);
                }
            }
        }
    }

  INPBUTTON* button;

  button = GetButton(INPBUTTON_ID_MOUSE_MIDDLE);
  if(button) button->SetPressed(GetAsyncKeyState(VK_MBUTTON) != 0);

  button = GetButton(INPBUTTON_ID_MOUSE_RIGHT);
  if(button) button->SetPressed(GetAsyncKeyState(VK_RBUTTON) != 0);

  button = GetButton(INPBUTTON_ID_MOUSE_LEFT);
  if(button) button->SetPressed(GetAsyncKeyState(VK_LBUTTON) != 0);
  */

  // ----------------------------------------------------------------------------------




  return true;
}



/*-------------------------------------------------------------------
//  INPWINDOWSDEVICEMOUSE::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/12/2011 18:27:18
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void INPWINDOWSDEVICEMOUSE::Clean()
{
  grpscreen =  NULL;
}





/*-------------------------------------------------------------------
//  INPWINDOWSDEVICEMOUSE::GetWindowPosition
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/03/2012 13:37:48
//
//  @return       bool :
//  @param        screenx :
//  @param        screeny :
//  @param        screenw :
//  @param        screenh :
*/
/*-----------------------------------------------------------------*/
bool INPWINDOWSDEVICEMOUSE::GetWindowPosition(int& screenx,int& screeny,int& screenw,int& screenh)
{
  if(!grpscreen) return false;

  WINDOWINFO info;

  GetWindowInfo((HWND)grpscreen->GetHandle(),&info);

  screenx = info.rcClient.left;
  screeny = info.rcClient.bottom;

  screenw = abs(info.rcClient.right  - info.rcClient.left);
  screenh = abs(info.rcClient.bottom - info.rcClient.top);

  return true;
}




//-------------------------------------------------------------------
//  INPWINDOWSDEVICEMOUSE::CreateAllButtons
/**
//
//  @author        Abraham J. Velez
//  @version       18/2/2003   15:33:47
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool INPWINDOWSDEVICEMOUSE::CreateAllButtons()
{
  INPBUTTON::CreateButton( &buttons, VK_RBUTTON , INPBUTTON_ID_MOUSE_RIGHT     , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, VK_LBUTTON , INPBUTTON_ID_MOUSE_LEFT      , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, VK_MBUTTON , INPBUTTON_ID_MOUSE_MIDDLE    , __C('\x0'));

  return true;
}



//-------------------------------------------------------------------
//  INPWINDOWSDEVICEMOUSE::DeleteAllButtons
/**
//
//  @author        Abraham J. Velez
//  @version       18/2/2003   15:34:3
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool INPWINDOWSDEVICEMOUSE::DeleteAllButtons()
{
  if(buttons.IsEmpty())  return false;

  buttons.DeleteContents();
  buttons.DeleteAll();

  return true;
}




/*-------------------------------------------------------------------
//  INPWINDOWSDEVICEMOUSE::CreateAllCursors
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/12/2011 10:55:24
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool INPWINDOWSDEVICEMOUSE::CreateAllCursors()
{
  for (XDWORD e=0;e<buttons.GetSize();e++)
  {
    INPCURSOR* cursor;

    cursor = new INPCURSOR();
    if(!cursor) return false;
    cursor->SetID(INPCURSOR_ID_MOUSE);
    cursor->SetHavePreSelect(true);

    cursors.Add(cursor);
  }

  return true;
}




//-------------------------------------------------------------------
//  INPWINDOWSDEVICEMOUSE::DeleteAllButtons
/**
//
//  @author        Abraham J. Velez
//  @version       18/2/2003   15:34:3
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool INPWINDOWSDEVICEMOUSE::DeleteAllCursors()
{
  if(cursors.IsEmpty())  return false;

  cursors.DeleteContentsInstanced();
  cursors.DeleteAll();

  return true;
}


