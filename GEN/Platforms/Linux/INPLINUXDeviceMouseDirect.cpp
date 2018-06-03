/*------------------------------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEDIRECT.CPP
//
//  Input LINUX device mouse direct class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 10/04/2014 12:41:32
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

#include "XTimer.h"
#include "XFileTXT.h"
#include "XDebugTrace.h"

#include "INPLinuxDeviceMouseDirect.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEDIRECT::INPLINUXDEVICEMOUSEDIRECT
*/
/**
//
//  Class Constructor INPLINUXDEVICEMOUSEDIRECT
//
//  @author       Abraham J. Velez
//  @version      10/04/2014 13:14:17
//

//  @param        grpscreen :
*/
/*-----------------------------------------------------------------*/
INPLINUXDEVICEMOUSEDIRECT::INPLINUXDEVICEMOUSEDIRECT( GRPSCREEN* grpscreen) : INPDEVICE()
{
  Clean();

  SetType(INPDEVICE_TYPE_MOUSE);

  created = true;

  if(CreateAllButtons())
    {
      if(CreateAllCursors())
        {
          SetEnabled(true);
        }
    }

  SetScreen(grpscreen);

  UpdateMouseData();

  OpenAllDevicesID();
}




/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEDIRECT::~INPLINUXDEVICEMOUSEDIRECT
*/
/**
//
//   Class Destructor INPLINUXDEVICEMOUSEDIRECT
//
//  @author       Abraham J. Velez
//  @version      10/04/2014 13:14:28
//
*/
/*-----------------------------------------------------------------*/
INPLINUXDEVICEMOUSEDIRECT::~INPLINUXDEVICEMOUSEDIRECT()
{
  CloseAllDevicesID();

  mice.DeleteContents();
  mice.DeleteAll();

  DeleteAllButtons();
  DeleteAllCursors();

  SetEnabled(false);
  created = false;

  Clean();
}


/*-------------------------------------------------------------------
//   INPLINUXDEVICEMOUSEDIRECT::SetScreen
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
bool INPLINUXDEVICEMOUSEDIRECT::SetScreen(void* screenpointer)
{
    this->grpscreen = (GRPSCREEN*)screenpointer;
    if (grpscreen)
    {
        mousex = grpscreen->GetWidth() / 2;
        mousey = grpscreen->GetHeight() / 2;
        mousez = 0;
    }
    return true;
}



/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEDIRECT::Update
*/
/**
//
//
//
//  @author       Imanol Celaya / Diego Martinez
//  @version      16/06/2017 13:29:42
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEDIRECT::Update()
{
  if((!created)||(!enabled)) return false;

  int                 eventsize = sizeof(struct input_event);
  int                 bytesread = 0;

  struct input_event  event;

  static int          delta     = 0;
  static int          hdelta    = 0;

  //--------------- clean up old states
  for(XDWORD c=0;c<buttons.GetSize();c++)
  {
    INPBUTTON* button=buttons.Get(c);
    if (button)
    {
      if (button->state==INPBUTTON_STATE_RELEASED)
        button->state=INPBUTTON_STATE_UP;
      else
      if (button->state==INPBUTTON_STATE_PRESSED)
        button->state=INPBUTTON_STATE_HOLD;
    }
  }

  //--------------- process

  if(mice.GetSize() == 0)
    {
      OpenAllDevicesID();
    }

  for(XDWORD e=0; e<mice.GetSize(); e++)
    {
      while(true)
        {
          bytesread=0;

          memset(&event, 0, sizeof(event));

          errno = 0;
          bytesread = read(mice.Get(e)->GetFileDescriptor(), &event, sizeof(event)/*eventsize*/);

          // if device is diconnected we receive a ENODEV message (happens for example every time a touchscreen is poweredoff)

          if(bytesread == -1 && errno == ENODEV)
            {
              // we lost the handle to the event file, need to reclaim it
              mice.DeleteContents();
              OpenAllDevicesID();
            }

          if(bytesread==-1 && errno!=EAGAIN) break;

          if((bytesread < sizeof(struct input_event)) || (bytesread == -1))
            break;

          if(bytesread == sizeof(event)/*eventsize*/)
            {
                switch(event.type)
                  {
                    case EV_ABS : switch(event.code)
                                    {
                                        case ABS_X : {
                                          mousex  = event.value;
                                          float x = mousex / (float)maxx;

                                          mousex = x * grpscreen->GetWidth();
                                          break;
                                                     }
                                      case ABS_Y      : {
                                        mousey  = event.value;
                                        float y = mousey / (float)maxy;

                                        mousey = y * grpscreen->GetHeight();
                                        mousey = (grpscreen->GetHeight()-mousey);
                                        break;
                                                        }
                                      case ABS_Z      : mousez  = event.value;  break;
                                      case REL_HWHEEL : hdelta  = event.value;  break;
                                      case REL_WHEEL  : delta   = event.value;  break;
                                    }
                                  break;

                    case EV_REL : switch(event.code)
                                    {
                                      case REL_X      : mousex  += event.value; break;
                                      case REL_Y      : mousey  -= event.value; break;
                                      case REL_Z      : mousez  += event.value; break;
                                      case REL_HWHEEL : hdelta  += event.value; break;
                                      case REL_WHEEL  : delta   += event.value; break;
                                    }
                                  break;


                    case EV_KEY : { INPBUTTON* button = GetButtonByCode(event.code);
                                    if(button)
                                    {

                                        switch(button->state)
                                        {
                                          case INPBUTTON_STATE_UP:      (event.value==0) ? button->state=INPBUTTON_STATE_UP       : button->state=INPBUTTON_STATE_PRESSED;  break;
                                          case INPBUTTON_STATE_PRESSED: (event.value==0) ? button->state=INPBUTTON_STATE_RELEASED : button->state=INPBUTTON_STATE_HOLD;     break;
                                          case INPBUTTON_STATE_RELEASED:
                                          case INPBUTTON_STATE_HOLD:    (event.value==0) ? button->state=INPBUTTON_STATE_RELEASED : button->state=INPBUTTON_STATE_PRESSED;  break;
                                        }
                                        button->SetPressed((event.value==1)?true:false);

                                    }

                                    // emulate mouse button
                                    if(event.code == BTN_TOUCH)
                                      {
                                        button = GetButtonByCode(BTN_LEFT);
                                        if (button)
                                        {
                                          switch(button->state)
                                          {
                                            case INPBUTTON_STATE_UP:      (event.value==0) ? button->state=INPBUTTON_STATE_UP       : button->state=INPBUTTON_STATE_PRESSED;  break;
                                            case INPBUTTON_STATE_PRESSED: (event.value==0) ? button->state=INPBUTTON_STATE_RELEASED : button->state=INPBUTTON_STATE_HOLD;     break;
                                            case INPBUTTON_STATE_RELEASED:
                                            case INPBUTTON_STATE_HOLD:    (event.value==0) ? button->state=INPBUTTON_STATE_RELEASED : button->state=INPBUTTON_STATE_PRESSED;  break;
                                          }
                                          button->SetPressed((event.value==1)?true:false);
                                        }

                                      }
                                  }
                                  break;
                    #ifdef EV_SYN
                    case EV_SYN :  //  UpdateMouseData();
                                  break;
                    #endif
                        default : continue;
                  }
            } else break;
        }
    }
  UpdateMouseData();
  return true;
}





/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEDIRECT::CreateAllButtons
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/04/2014 13:22:07
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEDIRECT::CreateAllButtons()
{
  INPBUTTON::CreateButton( &buttons, BTN_RIGHT    , INPBUTTON_ID_MOUSE_RIGHT        , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, BTN_LEFT     , INPBUTTON_ID_MOUSE_LEFT         , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, BTN_MIDDLE   , INPBUTTON_ID_MOUSE_MIDDLE       , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, BTN_TOUCH    , INPBUTTON_ID_TOUCH              , __C('\x0'));

  return true;
}




/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEDIRECT::DeleteAllButtons
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/04/2014 13:29:52
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEDIRECT::DeleteAllButtons()
{
  if(buttons.IsEmpty())  return false;

  buttons.DeleteContents();
  buttons.DeleteAll();

  return true;
}





/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEDIRECT::CreateAllCursors
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/04/2014 13:30:00
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEDIRECT::CreateAllCursors()
{
  for (XDWORD e=0;e<this->buttons.GetSize();e++)
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



/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEDIRECT::DeleteAllCursors
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/04/2014 13:30:06
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEDIRECT::DeleteAllCursors()
{
  if(cursors.IsEmpty())  return false;

  cursors.DeleteContents();
  cursors.DeleteAll();

  return true;
}



/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEDIRECT::OpenAllDevicesID
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/04/2014 16:50:37
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEDIRECT::OpenAllDevicesID()
{
  XFILETXT* xfiletxtdevices =  new XFILETXT();
  XPATH     xpathdevices;

  if(!xfiletxtdevices) return false;

  xpathdevices = INPLINUXDEVICEID_HANDLEFILE;

  //-------------------------------------------------------------------------------------
  // We open the device list so we can get additional data on our devices attached to kbd & mousedev
  if(!xfiletxtdevices->Open(xpathdevices)) return false;

  xfiletxtdevices->ReadAllFile();

  for(int e=0; e<xfiletxtdevices->GetNLines(); e++)
    {
      // check if kbd handler
      XSTRING* string         = xfiletxtdevices->GetLine(e);
      XSTRING  toscan;
      int      pos            = string->Find(__L("mouse"),0);
      XSTRING  namehardware;
      int      filedescriptor = INPLINUXDEVICEID_INVALID;
      int      eventnumber    = INPLINUXDEVICEID_INVALID;
      char     mouseOEMname[_MAXSTR];

      if(pos!=NOTFOUND) // ID of Mouse
        {
          pos = string->Find(__L(" event"),0);

          if(pos!=NOTFOUND)  // Valid ID
            {
              XSTRING event;

              toscan=&(string->Get()[pos]);
              toscan.UnFormat(__L(" event%d"), &eventnumber);

              event.Format(__L("/dev/input/event%d"), eventnumber);

              XSTRING_CREATEOEM(event,mouse_char_oem);
              filedescriptor = open(mouse_char_oem, O_RDONLY | O_NONBLOCK);
              XSTRING_DELETEOEM(mouse_char_oem);
              if(filedescriptor == -1) return false;

              //we get the keyboard name
              ioctl(filedescriptor, EVIOCGNAME(_MAXSTR), mouseOEMname);
              namehardware.Set(mouseOEMname);

              // get max/min values on each axis
              int abs[6] = {0};
              ioctl(filedescriptor, EVIOCGABS(ABS_X), abs);
              minx = abs[1];
              maxx = abs[2];

              ioctl(filedescriptor, EVIOCGABS(ABS_Y), abs);
              miny = abs[1];
              maxy = abs[2];

              ioctl(filedescriptor, EVIOCGABS(ABS_Z), abs);
              minz = abs[1];
              maxz = abs[2];

             XDEBUGTRACE_PRINTCOLOR(0,__L("Device %s x(%d,%d) y(%d,%d) z(%d,%d)"),namehardware.Get(),minx,maxx,miny,maxy,minz,maxz);

              INPLINUXDEVICEID* deviceID = new INPLINUXDEVICEID();
              if(!deviceID) return false;

              deviceID->SetFileDescriptor(filedescriptor);
              deviceID->GetName()->Set(namehardware);

              mice.Add(deviceID);
            }
        }
    }

  xfiletxtdevices->Close();

  delete xfiletxtdevices;

  return true;
}





/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEDIRECT::CloseAllDevicesID
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/04/2014 16:49:34
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEDIRECT::CloseAllDevicesID()
{
  if(mice.IsEmpty()) return false;

  for(XDWORD c=0; c<mice.GetSize(); c++)
    {
      INPLINUXDEVICEID* deviceID = mice.Get(c);
      if(deviceID)
        {
          close(deviceID->GetFileDescriptor());
        }
    }

  return true;
}




/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEDIRECT::UpdateMouseData
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/04/2014 10:29:02
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEDIRECT::UpdateMouseData()
{
  if(!grpscreen) return false;

  for (int e=0;e<GetCursors()->GetSize();e++)
  {
  INPCURSOR* cursor = GetCursors()->Get(e);
  if(!cursor) continue;

  cursor->SetIsChanged(false);

        if((cursor->GetX() != mousex) || (cursor->GetY() != mousey))
        {
          cursor->Set(mousex, mousey);
          cursor->SetIsChanged(true);
        }
  }

  INPBUTTON* button = GetButton(INPBUTTON_ID_MOUSE_LEFT);
  if(button)
  if (GetCursors()->Get(0))
    GetCursors()->Get(0)->AddPointToMotion(button->IsPressed());

  //if((mousex>=0) && (mousey>=0))// && (mousex<grpscreen->GetWidth()) && (mousey<grpscreen->GetHeight()))
  //  {

  /*
      cursor->SetIsChanged(false);

      if((cursor->GetX() != mousex) || (cursor->GetY() != mousey))
        {
          cursor->Set(mousex, mousey);
          cursor->SetIsChanged(true);

         XDEBUGTRACE_PRINTCOLOR(0,__L("Update %f %f %d %d"),cursor->GetX(),cursor->GetY(),mousex, mousey);
        }

    else
        {
          if(mousex<0) mousex = 0;
          if(mousey<0) mousey = 0;

          if(mousex>=grpscreen->GetWidth())  mousex = grpscreen->GetWidth()-1;
          if(mousey>=grpscreen->GetHeight()) mousey = grpscreen->GetHeight()-1;
        }
*/


    //} else return false;

  return true;
}