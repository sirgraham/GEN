//------------------------------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDX11.CPP
//
//  LINUX input device keyboard X11 class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 11/02/2003
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <unistd.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>

#include "XFactory.h"
#include "XFileTXT.h"

#include "GRPLINUXScreenX11.h"

#include "INPButton.h"

#include "INPLINUXDeviceKeyboardX11.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------



//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDX11X11::INPLINUXDEVICEKEYBOARDX11X11
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/03/2012 12:14:58
//
//  @return
//  @param        grpscreen :

*/
/*-----------------------------------------------------------------*/
INPLINUXDEVICEKEYBOARDX11::INPLINUXDEVICEKEYBOARDX11() : INPDEVICE()
{
  Clean();
  /*
  if(grpscreen)
    {
      grpscreenx11 = (GRPLINUXSCREENX11*)grpscreen;
      XSelectInput(grpscreenx11->GetDisplay(), (*grpscreenx11->GetWindow()), KeyPressMask | KeyReleaseMask);
    }
    */

  created = true;
  SetType(INPDEVICE_TYPE_KEYBOARD);
  SetEnabled(CreateAllButtons());
}



//-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDX11::~INPLINUXDEVICEKEYBOARDX11
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2003 12:55:30
//
//  @return       void :
//  */
//-------------------------------------------------------------------
INPLINUXDEVICEKEYBOARDX11::~INPLINUXDEVICEKEYBOARDX11()
{
  DeleteAllButtons();

  SetEnabled(false);
  created = false;

  Clean();
}


/*-------------------------------------------------------------------
//  INPLINUXDXDEVICEKEYBOARD::GetButtons
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/12/2011 10:43:42
//
//  @return       XVECTOR<INPBUTTON*>* :
//  */
/*-----------------------------------------------------------------*/
XVECTOR<INPBUTTON*>* INPLINUXDEVICEKEYBOARDX11::GetButtons()
{
  return &buttons;
}


//-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDX11::Release
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/08/2004 11:57:47
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool INPLINUXDEVICEKEYBOARDX11::Release()
{
  for(XDWORD c=0;c<buttons.GetSize();c++)
    {
      INPBUTTON* button = (INPBUTTON*)buttons.Get(c);

      if(button)
        {
          button->SetPressed(false);
          button->SetMarkToRelase(true);
        }
    }

  return true;
}



//-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDX11::Update
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/02/2003 11:48:00
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool INPLINUXDEVICEKEYBOARDX11::Update()
{
  if((!created)||(!enabled)) return false;

  if(!grpscreenx11) return false;

  if(!grpscreenx11->HasFocus())  return false;

  XEvent event;

  /*
  while(XCheckWindowEvent(grpscreenx11->GetDisplay(),(*grpscreenx11->GetWindow()), KeyPress | KeyRelease ,&event))
    {
      XKeyEvent* keyevent = (XKeyEvent *)&event;
      long       keysym   = XLookupKeysym (keyevent, 0);
      INPBUTTON* button   = GetButtonByCode(keysym);
      if(button)
        {
          switch(event.type)
            {
              case KeyPress   : button->SetPressed(true);    break;
              case KeyRelease : button->SetPressed(false);   break;
            }
        }
    }
    */

  for (int e=0;e<this->buttons.GetSize();e++)
  {
    INPBUTTON* button=this->buttons.FastGet(e);

    if (button->state==INPBUTTON_STATE_RELEASED)
    {
        button->state=INPBUTTON_STATE_UP;
        button->updated=false;
    }

    if (button->state==INPBUTTON_STATE_PRESSED)
        button->state=INPBUTTON_STATE_HOLD;

  }

  while(XCheckWindowEvent(grpscreenx11->GetDisplay(),(*grpscreenx11->GetWindow()),  KeyPressMask | KeyReleaseMask ,&event))
    {
      XKeyEvent* keyevent = (XKeyEvent *)&event;

      //if(button)
        {
          switch(event.type)
            {
              case KeyPress   :
                {
                long       keysym   = XLookupKeysym (keyevent, 0);
                INPBUTTON* button   = GetButtonByCode(keysym);

                if (!button)
                {
                   KeySym low = 0;
                   KeySym upper = 0;
                  XConvertCase(keysym,&low,&upper);
                  button    = GetButtonByCode(upper);
                }

                if (!button)
                break;

                button->updated=true;

                if (button->state==INPBUTTON_STATE_UP)
                  button->state=INPBUTTON_STATE_PRESSED;

                  button->SetPressed(true);
                }
                break;

              case KeyRelease :
                {

                long       keysym   = XLookupKeysym (keyevent, 0);
                INPBUTTON* button   = GetButtonByCode(keysym);

                if (!button)
                {
                   KeySym low = 0;
                   KeySym upper = 0;
                  XConvertCase(keysym,&low,&upper);
                  button    = GetButtonByCode(upper);
                }

                if (!button)
                break;

                button->updated=true;

                button->state=INPBUTTON_STATE_RELEASED;
                button->SetPressed(false);
                }
                break;
            }
        }
    }
  return true;
}


/*-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDX11::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2012 13:45:52
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void INPLINUXDEVICEKEYBOARDX11::Clean()
{
  grpscreenx11 = NULL;
}


//-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDX11::CreateAllButtons
/**
//
//  @author        Abraham J. Velez
//  @version       18/2/2003   15:33:47
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool INPLINUXDEVICEKEYBOARDX11::CreateAllButtons()
{
  INPBUTTON::CreateButton( &buttons, XK_BackSpace     , INPBUTTON_ID_BACK_SPACE        , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Tab           , INPBUTTON_ID_TAB               , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Return        , INPBUTTON_ID_RETURN            , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Escape          , INPBUTTON_ID_ESCAPE            , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Caps_Lock     , INPBUTTON_ID_CAPS_LOCK         , __C('\x0'));

  INPBUTTON::CreateButton( &buttons, XK_Shift_R         , INPBUTTON_ID_SHIFT_RIGHT       , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Shift_L       , INPBUTTON_ID_SHIFT_LEFT        , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Control_R       , INPBUTTON_ID_CONTROL_RIGHT     , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Control_L     , INPBUTTON_ID_CONTROL_LEFT      , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Alt_R         , INPBUTTON_ID_ALT_RIGHT         , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Alt_L           , INPBUTTON_ID_ALT_LEFT          , __C('\x0'));

  INPBUTTON::CreateButton( &buttons, XK_parenleft     , INPBUTTON_ID_OPEN_BRANCH       , __C('('  ));
  INPBUTTON::CreateButton( &buttons, XK_parenright      , INPBUTTON_ID_CLOSE_BRANCH      , __C(')'  ));
  INPBUTTON::CreateButton( &buttons, XK_comma         , INPBUTTON_ID_COMMA             , __C(','   ));
  INPBUTTON::CreateButton( &buttons, XK_minus         , INPBUTTON_ID_MINUS             , __C('-'  ));
  INPBUTTON::CreateButton( &buttons, XK_periodcentered  , INPBUTTON_ID_POINT             , __C('.'   ));
  INPBUTTON::CreateButton( &buttons,  XK_slash          , INPBUTTON_ID_SLASH             , __C('/'   ));

//INPBUTTON::CreateButton( &buttons, OPEN_BRACKET     , INPBUTTON_ID_OPEN_BRACKET      , __C('['   ));
//INPBUTTON::CreateButton( &buttons, CLOSE_BRACKET      , INPBUTTON_ID_CLOSE_BRACKET     , __C(']'  ));

  INPBUTTON::CreateButton( &buttons, XK_space         , INPBUTTON_ID_SPACE             , __C(' '  ));
  INPBUTTON::CreateButton( &buttons, XK_Page_Up       , INPBUTTON_ID_PAGE_UP           , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Page_Down     , INPBUTTON_ID_PAGE_DOWN         , __C('\x0'));


  INPBUTTON::CreateButton( &buttons, XK_End           , INPBUTTON_ID_END               , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Home            , INPBUTTON_ID_HOME              , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Left            , INPBUTTON_ID_LEFT              , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Up              , INPBUTTON_ID_UP                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Right         , INPBUTTON_ID_RIGHT             , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Down            , INPBUTTON_ID_DOWN              , __C('\x0'));

  INPBUTTON::CreateButton( &buttons, XK_Insert          , INPBUTTON_ID_INSERT            , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Delete          , INPBUTTON_ID_DELETE            , __C('\x0'));

  INPBUTTON::CreateButton( &buttons, XK_0             , INPBUTTON_ID_0                 , __C('0'  ));
  INPBUTTON::CreateButton( &buttons, XK_1             , INPBUTTON_ID_1                 , __C('1'  ));
  INPBUTTON::CreateButton( &buttons, XK_2             , INPBUTTON_ID_2                 , __C('2'  ));
  INPBUTTON::CreateButton( &buttons, XK_3             , INPBUTTON_ID_3                 , __C('3'  ));
  INPBUTTON::CreateButton( &buttons, XK_4             , INPBUTTON_ID_4                 , __C('4'  ));
  INPBUTTON::CreateButton( &buttons, XK_5             , INPBUTTON_ID_5                 , __C('5'  ));
  INPBUTTON::CreateButton( &buttons, XK_6             , INPBUTTON_ID_6                 , __C('6'  ));
  INPBUTTON::CreateButton( &buttons, XK_7             , INPBUTTON_ID_7                 , __C('7'  ));
  INPBUTTON::CreateButton( &buttons, XK_8             , INPBUTTON_ID_8                 , __C('8'  ));
  INPBUTTON::CreateButton( &buttons, XK_9             , INPBUTTON_ID_9                 , __C('9'  ));

  INPBUTTON::CreateButton( &buttons, XK_A             , INPBUTTON_ID_A                 , __C('A'  ));
  INPBUTTON::CreateButton( &buttons, XK_B             , INPBUTTON_ID_B                 , __C('B'  ));
  INPBUTTON::CreateButton( &buttons, XK_C             , INPBUTTON_ID_C                 , __C('C'  ));
  INPBUTTON::CreateButton( &buttons, XK_D             , INPBUTTON_ID_D                 , __C('D'  ));
  INPBUTTON::CreateButton( &buttons, XK_E             , INPBUTTON_ID_E                 , __C('E'  ));
  INPBUTTON::CreateButton( &buttons, XK_F             , INPBUTTON_ID_F                 , __C('F'  ));
  INPBUTTON::CreateButton( &buttons, XK_G             , INPBUTTON_ID_G                 , __C('G'  ));
  INPBUTTON::CreateButton( &buttons, XK_H             , INPBUTTON_ID_H                 , __C('H'  ));
  INPBUTTON::CreateButton( &buttons, XK_I             , INPBUTTON_ID_I                 , __C('I'  ));
  INPBUTTON::CreateButton( &buttons, XK_J             , INPBUTTON_ID_J                 , __C('J'  ));
  INPBUTTON::CreateButton( &buttons, XK_K             , INPBUTTON_ID_K                 , __C('K'  ));
  INPBUTTON::CreateButton( &buttons, XK_L             , INPBUTTON_ID_L                 , __C('L'  ));
  INPBUTTON::CreateButton( &buttons, XK_M             , INPBUTTON_ID_M                 , __C('M'  ));
  INPBUTTON::CreateButton( &buttons, XK_N             , INPBUTTON_ID_N                 , __C('N'  ));
  INPBUTTON::CreateButton( &buttons, XK_O             , INPBUTTON_ID_O                 , __C('O'  ));
  INPBUTTON::CreateButton( &buttons, XK_P             , INPBUTTON_ID_P                 , __C('P'  ));
  INPBUTTON::CreateButton( &buttons, XK_Q             , INPBUTTON_ID_Q                 , __C('Q'  ));
  INPBUTTON::CreateButton( &buttons, XK_R             , INPBUTTON_ID_R                 , __C('R'  ));
  INPBUTTON::CreateButton( &buttons, XK_S             , INPBUTTON_ID_S                 , __C('S'  ));
  INPBUTTON::CreateButton( &buttons, XK_T             , INPBUTTON_ID_T                 , __C('T'  ));
  INPBUTTON::CreateButton( &buttons, XK_U             , INPBUTTON_ID_U                 , __C('U'  ));
  INPBUTTON::CreateButton( &buttons, XK_V             , INPBUTTON_ID_V                 , __C('V'  ));
  INPBUTTON::CreateButton( &buttons, XK_W             , INPBUTTON_ID_W                 , __C('W'  ));
  INPBUTTON::CreateButton( &buttons, XK_X             , INPBUTTON_ID_X                 , __C('X'  ));
  INPBUTTON::CreateButton( &buttons, XK_Y             , INPBUTTON_ID_Y                 , __C('Y'  ));
  INPBUTTON::CreateButton( &buttons, XK_Z             , INPBUTTON_ID_Z                 , __C('Z'  ));

  INPBUTTON::CreateButton( &buttons, XK_Num_Lock        , INPBUTTON_ID_NUMLOCK           , __C('\x0'));

  INPBUTTON::CreateButton( &buttons, XK_F1              , INPBUTTON_ID_F1                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_F2              , INPBUTTON_ID_F2                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_F3              , INPBUTTON_ID_F3                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_F4              , INPBUTTON_ID_F4                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_F5              , INPBUTTON_ID_F5                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_F6              , INPBUTTON_ID_F6                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_F7              , INPBUTTON_ID_F7                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_F8              , INPBUTTON_ID_F8                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_F9              , INPBUTTON_ID_F9                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_F10           , INPBUTTON_ID_F10               , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_F11           , INPBUTTON_ID_F11               , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_F12           , INPBUTTON_ID_F12               , __C('\x0'));

  INPBUTTON::CreateButton( &buttons, XK_Print         , INPBUTTON_ID_PRINTSCREEN       , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Scroll_Lock     , INPBUTTON_ID_SCROLL_LOCK       , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, XK_Pause         , INPBUTTON_ID_PAUSE             , __C('\x0'));

  return true;
}



//-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDX11::DeleteAllButtons
/**
//
//  @author        Abraham J. Velez
//  @version       18/2/2003   15:34:3
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool INPLINUXDEVICEKEYBOARDX11::DeleteAllButtons()
{
  if(buttons.IsEmpty())  return false;

  buttons.DeleteContents();
  buttons.DeleteAll();

  return true;
}

bool INPLINUXDEVICEKEYBOARDX11::SetScreen(void* screenpointer)
{
    this->grpscreenx11 = (GRPLINUXSCREENX11*)screenpointer;

    if (grpscreenx11)
    {
        XSelectInput(grpscreenx11->GetDisplay(), (*grpscreenx11->GetWindow()), KeyPressMask | KeyReleaseMask);
    }

    return true;
}




