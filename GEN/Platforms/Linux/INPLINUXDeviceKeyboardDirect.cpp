
/*------------------------------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDDIRECT.CPP
//
//  Input LINUX device keyboard direct class
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 10/04/2014 10:37:49
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <unistd.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <cerrno>

#include "XFactory.h"
#include "XBase.h"
#include "XFileTXT.h"
#include "XDebugTrace.h"

#include "INPButton.h"

#include "INPLINUXDeviceKeyboardDirect.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDDIRECT::INPLINUXDEVICEKEYBOARDDIRECT
*/
/**
//
//  Class Constructor INPLINUXDEVICEKEYBOARDDIRECT
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/04/2014 10:48:59
//

*/
/*-----------------------------------------------------------------*/
INPLINUXDEVICEKEYBOARDDIRECT::INPLINUXDEVICEKEYBOARDDIRECT() : INPDEVICE()
{
  Clean();

  created = true;

  SetType(INPDEVICE_TYPE_KEYBOARD);

  SetEnabled(CreateAllButtons());

  eventnumber = INPLINUXDEVICEID_INVALID;

  GetHandleEvent();
}




/*-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDDIRECT::~INPLINUXDEVICEKEYBOARDDIRECT
*/
/**
//
//   Class Destructor INPLINUXDEVICEKEYBOARDDIRECT
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/04/2014 10:49:10
//
*/
/*-----------------------------------------------------------------*/
INPLINUXDEVICEKEYBOARDDIRECT::~INPLINUXDEVICEKEYBOARDDIRECT()
{
  keyboards.DeleteContents();
  keyboards.DeleteAll();

  DeleteAllButtons();

  SetEnabled(false);

  created = false;

  Clean();
}

bool INPLINUXDEVICEKEYBOARDDIRECT::SetScreen(void* screenpointer)
{
    return true;
}





/*-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDDIRECT::Release
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/04/2014 10:50:19
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDDIRECT::Release()
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





/*-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDDIRECT::Update
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/04/2014 10:50:32
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDDIRECT::Update()
{
  if((!created)||(!enabled)) return false;

  int                   eventsize = sizeof(struct input_event);
  int                   bytesread = 0;
  struct  input_event   event;

  if(keyboards.GetSize() == 0)
    {
      GetHandleEvent();
    }

  for(XDWORD e=0; e<keyboards.GetSize(); e++)
    {

    errno = 0;
      do{ bytesread = read(keyboards.Get(e)->GetFileDescriptor(), &event, eventsize);
          if(bytesread == -1 && errno == ENODEV)
            {
              // we lost the handle to the event file, need to reclaim it
              keyboards.DeleteContents();
              GetHandleEvent();
            }

          if(bytesread == eventsize)
            {
              if(EV_KEY == event.type)
                {
                  INPBUTTON* button   = GetButtonByCode(event.code); //emitimos el boton

                  if(button)
                    button->SetPressed(event.value!=0);
                  else
                    continue;

                  switch (event.value)
                  {
                  case 0:
                          {
                          button->state=INPBUTTON_STATE_RELEASED;
                          //XDEBUGTRACE_PRINTCOLOR(0,__L("INPBUTTON_STATE_RELEASED %d"),event.code);
                          }
                          break;
                  case 1:
                          if (button->state==INPBUTTON_STATE_PRESSED)
                          {
                            button->state=INPBUTTON_STATE_HOLD;
                            //XDEBUGTRACE_PRINTCOLOR(0,__L("INPBUTTON_STATE_PRESSED %d"),event.code);
                          }
                          else
                          {
                            button->state=INPBUTTON_STATE_PRESSED;
                            //XDEBUGTRACE_PRINTCOLOR(0,__L("INPBUTTON_STATE_PRESSED %d"),event.code);
                          }
                          break;
                  case 2:
                          button->state=INPBUTTON_STATE_HOLD;
                          //XDEBUGTRACE_PRINTCOLOR(0,__L("INPBUTTON_STATE_HOLD %d"),event.code);
                          break;
                  }
                  button->updated=true;
                }
            }
        } while(bytesread == eventsize);

      for (int i=0;i<this->buttons.GetSize();i++)
      {
        INPBUTTON* button=buttons.Get(i);
        if (!button) continue;

        if (!button->updated)
        {
          if (button->state==INPBUTTON_STATE_PRESSED)
          {
            button->state=INPBUTTON_STATE_HOLD;
          }
          else
          if (button->state==INPBUTTON_STATE_RELEASED)
          {
            button->state=INPBUTTON_STATE_UP;
            //XDEBUGTRACE_PRINTCOLOR(0,__L("INPBUTTON_STATE_UP %d"),event.code);
          }
        }
        else
          button->updated=false;
      }
    }

  return true;
}



/*-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDDIRECT::Clean
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/04/2014 11:45:04
//
*/
/*-----------------------------------------------------------------*/
void INPLINUXDEVICEKEYBOARDDIRECT::Clean()
{

}



/*-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDDIRECT::CreateAllButtons
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/04/2014 11:45:29
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDDIRECT::CreateAllButtons()
{
  INPBUTTON::CreateButton( &buttons, KEY_BACKSPACE        , INPBUTTON_ID_BACK_SPACE         , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_TAB              , INPBUTTON_ID_TAB                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_ENTER            , INPBUTTON_ID_RETURN             , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_ESC              , INPBUTTON_ID_ESCAPE             , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_CAPSLOCK         , INPBUTTON_ID_CAPS_LOCK          , __C('\x0'));

  INPBUTTON::CreateButton( &buttons, KEY_RIGHTSHIFT       , INPBUTTON_ID_SHIFT_RIGHT        , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_LEFTSHIFT        , INPBUTTON_ID_SHIFT_LEFT         , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_RIGHTCTRL        , INPBUTTON_ID_CONTROL_RIGHT      , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_LEFTCTRL         , INPBUTTON_ID_CONTROL_LEFT       , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_RIGHTALT         , INPBUTTON_ID_ALT_RIGHT          , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_LEFTALT          , INPBUTTON_ID_ALT_LEFT           , __C('\x0'));

  INPBUTTON::CreateButton( &buttons, KEY_KPLEFTPAREN      , INPBUTTON_ID_OPEN_BRANCH        , __C('(' ));
  INPBUTTON::CreateButton( &buttons, KEY_KPRIGHTPAREN     , INPBUTTON_ID_CLOSE_BRANCH       , __C(')' ));
  INPBUTTON::CreateButton( &buttons, KEY_COMMA            , INPBUTTON_ID_COMMA              , __C(',' ));
  INPBUTTON::CreateButton( &buttons, KEY_MINUS            , INPBUTTON_ID_MINUS              , __C('-' ));
  INPBUTTON::CreateButton( &buttons, KEY_DOT              , INPBUTTON_ID_POINT              , __C('.' ));
  INPBUTTON::CreateButton( &buttons, KEY_SLASH           , INPBUTTON_ID_SLASH              , __C('/' ));

  INPBUTTON::CreateButton( &buttons, KEY_LEFTBRACE        , INPBUTTON_ID_OPEN_BRACKET       , __C('[' ));
  INPBUTTON::CreateButton( &buttons, KEY_RIGHTBRACE       , INPBUTTON_ID_CLOSE_BRACKET      , __C(']'  ));

  INPBUTTON::CreateButton( &buttons, KEY_SPACE            , INPBUTTON_ID_SPACE              , __C(' '  ));
  INPBUTTON::CreateButton( &buttons, KEY_PAGEUP           , INPBUTTON_ID_PAGE_UP            , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_PAGEDOWN         , INPBUTTON_ID_PAGE_DOWN          , __C('\x0'));


  INPBUTTON::CreateButton( &buttons, KEY_END              , INPBUTTON_ID_END                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_HOME             , INPBUTTON_ID_HOME               , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_LEFT             , INPBUTTON_ID_LEFT               , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_UP               , INPBUTTON_ID_UP                 , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_RIGHT            , INPBUTTON_ID_RIGHT              , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_DOWN             , INPBUTTON_ID_DOWN               , __C('\x0'));

  INPBUTTON::CreateButton( &buttons, KEY_INSERT           , INPBUTTON_ID_INSERT             , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_DELETE           , INPBUTTON_ID_DELETE             , __C('\x0'));

  INPBUTTON::CreateButton( &buttons, KEY_A                , INPBUTTON_ID_A                  , __C('A' ));
  INPBUTTON::CreateButton( &buttons, KEY_B                , INPBUTTON_ID_B                  , __C('B' ));
  INPBUTTON::CreateButton( &buttons, KEY_C                , INPBUTTON_ID_C                  , __C('C' ));
  INPBUTTON::CreateButton( &buttons, KEY_D                , INPBUTTON_ID_D                  , __C('D' ));
  INPBUTTON::CreateButton( &buttons, KEY_E                , INPBUTTON_ID_E                  , __C('E' ));
  INPBUTTON::CreateButton( &buttons, KEY_F                , INPBUTTON_ID_F                  , __C('F' ));
  INPBUTTON::CreateButton( &buttons, KEY_G                , INPBUTTON_ID_G                  , __C('G' ));
  INPBUTTON::CreateButton( &buttons, KEY_H                , INPBUTTON_ID_H                  , __C('H' ));
  INPBUTTON::CreateButton( &buttons, KEY_I                , INPBUTTON_ID_I                  , __C('I' ));
  INPBUTTON::CreateButton( &buttons, KEY_J                , INPBUTTON_ID_J                  , __C('J' ));
  INPBUTTON::CreateButton( &buttons, KEY_K                , INPBUTTON_ID_K                  , __C('K' ));
  INPBUTTON::CreateButton( &buttons, KEY_L                , INPBUTTON_ID_L                  , __C('L' ));
  INPBUTTON::CreateButton( &buttons, KEY_M                , INPBUTTON_ID_M                  , __C('M' ));
  INPBUTTON::CreateButton( &buttons, KEY_N                , INPBUTTON_ID_N                  , __C('N' ));
  INPBUTTON::CreateButton( &buttons, KEY_O                , INPBUTTON_ID_O                  , __C('O' ));
  INPBUTTON::CreateButton( &buttons, KEY_P                , INPBUTTON_ID_P                  , __C('P' ));
  INPBUTTON::CreateButton( &buttons, KEY_Q                , INPBUTTON_ID_Q                  , __C('Q' ));
  INPBUTTON::CreateButton( &buttons, KEY_R                , INPBUTTON_ID_R                  , __C('R' ));
  INPBUTTON::CreateButton( &buttons, KEY_S                , INPBUTTON_ID_S                  , __C('S' ));
  INPBUTTON::CreateButton( &buttons, KEY_T                , INPBUTTON_ID_T                  , __C('T' ));
  INPBUTTON::CreateButton( &buttons, KEY_U                , INPBUTTON_ID_U                  , __C('U' ));
  INPBUTTON::CreateButton( &buttons, KEY_V                , INPBUTTON_ID_V                  , __C('V' ));
  INPBUTTON::CreateButton( &buttons, KEY_W                , INPBUTTON_ID_W                  , __C('W' ));
  INPBUTTON::CreateButton( &buttons, KEY_X                , INPBUTTON_ID_X                  , __C('X' ));
  INPBUTTON::CreateButton( &buttons, KEY_Y                , INPBUTTON_ID_Y                  , __C('Y' ));
  INPBUTTON::CreateButton( &buttons, KEY_Z                , INPBUTTON_ID_Z                  , __C('Z' ));


  INPBUTTON::CreateButton( &buttons, KEY_1                , INPBUTTON_ID_1                  , __C('1' ));
  INPBUTTON::CreateButton( &buttons, KEY_2                , INPBUTTON_ID_2                  , __C('2' ));
  INPBUTTON::CreateButton( &buttons, KEY_3                , INPBUTTON_ID_3                  , __C('3' ));
  INPBUTTON::CreateButton( &buttons, KEY_4                , INPBUTTON_ID_4                  , __C('4' ));
  INPBUTTON::CreateButton( &buttons, KEY_5                , INPBUTTON_ID_5                  , __C('5' ));
  INPBUTTON::CreateButton( &buttons, KEY_6                , INPBUTTON_ID_6                  , __C('6' ));
  INPBUTTON::CreateButton( &buttons, KEY_7                , INPBUTTON_ID_7                  , __C('7' ));
  INPBUTTON::CreateButton( &buttons, KEY_8                , INPBUTTON_ID_8                  , __C('8' ));
  INPBUTTON::CreateButton( &buttons, KEY_9                , INPBUTTON_ID_9                  , __C('9' ));
  INPBUTTON::CreateButton( &buttons, KEY_0                , INPBUTTON_ID_0                  , __C('0' ));

  INPBUTTON::CreateButton( &buttons, KEY_NUMLOCK          , INPBUTTON_ID_NUMLOCK           , __C('\x0'));

  INPBUTTON::CreateButton( &buttons, KEY_F1               , INPBUTTON_ID_F1                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_F2               , INPBUTTON_ID_F2                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_F3               , INPBUTTON_ID_F3                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_F4               , INPBUTTON_ID_F4                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_F5               , INPBUTTON_ID_F5                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_F6               , INPBUTTON_ID_F6                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_F7               , INPBUTTON_ID_F7                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_F8               , INPBUTTON_ID_F8                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_F9               , INPBUTTON_ID_F9                , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_F10              , INPBUTTON_ID_F10               , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_F11              , INPBUTTON_ID_F11               , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_F12              , INPBUTTON_ID_F12               , __C('\x0'));

  #ifdef KEY_SCREEN
  INPBUTTON::CreateButton( &buttons, KEY_SCREEN           , INPBUTTON_ID_PRINTSCREEN       , __C('\x0'));
  #endif
  INPBUTTON::CreateButton( &buttons, KEY_SCROLLLOCK       , INPBUTTON_ID_SCROLL_LOCK       , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, KEY_PAUSE            , INPBUTTON_ID_PAUSE             , __C('\x0'));

  return true;
}




/*-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDDIRECT::DeleteAllButtons
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/04/2014 11:47:29
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDDIRECT::DeleteAllButtons()
{
  if(buttons.IsEmpty())  return false;

  buttons.DeleteContents();
  buttons.DeleteAll();

  return true;
}




/*-------------------------------------------------------------------
//  INPLINUXDEVICEKEYBOARDDIRECT::GetHandleEvent
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/04/2014 11:47:40
//
//  @return       int :
//
*/
/*-----------------------------------------------------------------*/
bool INPLINUXDEVICEKEYBOARDDIRECT::GetHandleEvent()
{
  XFILETXT* xfiletxtdevices =  new XFILETXT();
  XPATH     xpathdevices;

  if(!xfiletxtdevices) return false;

  xpathdevices = INPLINUXDEVICEID_HANDLEFILE;

  //-------------------------------------------------------------------------------------
  // We open the device list so we can get additional data on our devices attached to kbd & mousedev
  if(!xfiletxtdevices->Open(xpathdevices)) return false;

  xfiletxtdevices->ReadAllFile();

  for(int e=0;e<xfiletxtdevices->GetNLines();e++)
    {
      // check if kbd handler
      XSTRING* string         = xfiletxtdevices->GetLine(e);
      XSTRING  toscan;
      int      pos            = string->Find(__L("kbd "),0);
      XSTRING  namehardware;
      int      filedescriptor = INPLINUXDEVICEID_INVALID;
      int      eventnumber    = INPLINUXDEVICEID_INVALID;
      char     keyboardOEMname[_MAXSTR];

      if(pos!=NOTFOUND) // ID of keyboard
        {
          pos=string->Find(__L(" event"),0);

          if(string->Find(__L("sysrq"),0)!=NOTFOUND)
            {
              if(pos != NOTFOUND) // Valid ID
                {
                  XSTRING event;

                  toscan= &(string->Get()[pos]);
                  toscan.UnFormat(__L(" event%d"), &eventnumber);

                  event.Format(__L("/dev/input/event%d"), eventnumber);
                  XSTRING_CREATEOEM(event,keyboard_char_oem);
                  filedescriptor = open(keyboard_char_oem, O_RDONLY | O_NONBLOCK);
                  XSTRING_DELETEOEM(keyboard_char_oem);
                  if(filedescriptor == -1) return false;

                  // we get the keyboard name

                  memset(keyboardOEMname, 0, _MAXSTR);

                  ioctl(filedescriptor, EVIOCGNAME(_MAXSTR), keyboardOEMname);
                  namehardware.Set(keyboardOEMname);

                  //XDEBUGTRACE_PRINTCOLOR(1,__L("Keyboard : (%s) on Event[%d] %s"), namehardware.Get(), eventnumber, event.Get());

                  INPLINUXDEVICEID* deviceID = new INPLINUXDEVICEID();
                  if(!deviceID) return false;

                  deviceID->SetFileDescriptor(filedescriptor);
                  deviceID->GetName()->Set(namehardware);

                  keyboards.Add(deviceID);
                }
            }
        }
    }

  xfiletxtdevices->Close();

  delete xfiletxtdevices;

  return true;
}

