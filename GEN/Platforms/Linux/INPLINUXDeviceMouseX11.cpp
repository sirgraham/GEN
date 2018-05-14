/*------------------------------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEX11.CPP
//
//  LINUX input device mouse X11 class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 21/12/2011 16:54:00
//  Last Mofificacion :
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

#include "GRPLINUXScreenX11.h"

#include "INPButton.h"
#include "INPCursor.h"

#include "INPLINUXDeviceMouseX11.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEX11::INPLINUXDEVICEMOUSEX11
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
INPLINUXDEVICEMOUSEX11::INPLINUXDEVICEMOUSEX11() : INPDEVICE()
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
}



/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEX11::~INPLINUXDEVICEMOUSEX11
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
INPLINUXDEVICEMOUSEX11::~INPLINUXDEVICEMOUSEX11()
{
  DeleteAllButtons();
  DeleteAllCursors();

  SetEnabled(false);
  created = false;

  Clean();
}



/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEX11::GetButtons
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
XVECTOR<INPBUTTON*>* INPLINUXDEVICEMOUSEX11::GetButtons()
{
  return &buttons;
}



/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEX11::GetCursors
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
XVECTOR<INPCURSOR*>* INPLINUXDEVICEMOUSEX11::GetCursors()
{
  return &cursors;
}




/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEX11::Update
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/12/2011 17:05:21
//
//  @return       bool :
//  @param        void :
*/
/*-----------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEX11::Update()
{
  if((!created)||(!enabled)) return false;

  if(!grpscreenx11)               return false;
  if(!grpscreenx11->HasFocus())   return false;

  XEvent event;


  for (int e = 0; e<this->buttons.GetSize(); e++)
  {
      INPBUTTON* button = this->buttons.FastGet(e);

      if (button->state == INPBUTTON_STATE_RELEASED)
      {
          button->state = INPBUTTON_STATE_UP;
          button->updated = false;
      }

      if (button->state == INPBUTTON_STATE_PRESSED)
          button->state = INPBUTTON_STATE_HOLD;

  }

  if(XCheckWindowEvent(grpscreenx11->GetDisplay(),(*grpscreenx11->GetWindow()), ButtonPressMask | ButtonReleaseMask,&event))
    {
      switch(event.type)
        {
          case ButtonPress   : { INPBUTTON* button = GetButtonByCode(event.xbutton.button);
                                  if (button)
                                  {
                                      switch (button->GetState())
                                      {
                                      case INPBUTTON_STATE_UP:       button->state = INPBUTTON_STATE_PRESSED; break;
                                      case INPBUTTON_STATE_HOLD:     button->state = INPBUTTON_STATE_HOLD; break;
                                      case INPBUTTON_STATE_PRESSED:  button->state = INPBUTTON_STATE_HOLD; break;
                                      case INPBUTTON_STATE_RELEASED: button->state = INPBUTTON_STATE_PRESSED; break;
                                      }
                                      button->SetPressed(true);
                                  }


                               }
                               break;

          case ButtonRelease : { INPBUTTON* button = GetButtonByCode(event.xbutton.button);
                                  if (button)
                                  {
                                      switch (button->GetState())
                                      {
                                      case INPBUTTON_STATE_UP:       button->state = INPBUTTON_STATE_UP; break;
                                      case INPBUTTON_STATE_HOLD:     button->state = INPBUTTON_STATE_RELEASED; break;
                                      case INPBUTTON_STATE_PRESSED:  button->state = INPBUTTON_STATE_RELEASED; break;
                                      case INPBUTTON_STATE_RELEASED: button->state = INPBUTTON_STATE_RELEASED; break;
                                      }
                                      button->SetPressed(false);
                                  }

                               }
                               break;
        }
    }

  Window       root;
  Window       child;
  int          rootx;
  int          rooty;
  int          mousex = -1;
  int          mousey = -1;
  unsigned int mask;

  XQueryPointer(grpscreenx11->GetDisplay()  , (*grpscreenx11->GetWindow())
                                            , &root,&child,&rootx,&rooty,&mousex,&mousey,&mask);

  INPCURSOR* cursor = GetCursor(INPCURSOR_ID_MOUSE);
  if(cursor)
    {
      if((mousex>=0)&&(mousey>=0)&&(mousex < grpscreenx11->GetWidth()) && (mousey < grpscreenx11->GetHeight()))
        {
          cursor->SetIsChanged(false);

          if((cursor->GetX() != mousex) || (cursor->GetY() != mousey))
            {
              mousey = (grpscreenx11->GetHeight()-mousey);
              cursor->Set(mousex,mousey);
              cursor->SetIsChanged(true);
            }
        }
       else
        {
          mousex = -1;
          mousey = -1;

          cursor->Set(mousex,mousey);
          cursor->SetIsChanged(true);
        }

      INPBUTTON* button = GetButton(INPBUTTON_ID_MOUSE_LEFT);
      if(button) cursor->AddPointToMotion(button->IsPressed());
    }

  return true;
}



/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEX11::Clean
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
void INPLINUXDEVICEMOUSEX11::Clean()
{
  grpscreenx11  =  NULL;
}



//-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEX11::CreateAllButtons
/**
//
//  @author        Abraham J. Velez
//  @version       18/2/2003   15:33:47
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool INPLINUXDEVICEMOUSEX11::CreateAllButtons()
{
  INPBUTTON::CreateButton( &buttons, Button3   , INPBUTTON_ID_MOUSE_RIGHT      , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, Button1   , INPBUTTON_ID_MOUSE_LEFT       , __C('\x0'));
  INPBUTTON::CreateButton( &buttons, Button2   , INPBUTTON_ID_MOUSE_MIDDLE     , __C('\x0'));

  return true;
}



//-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEX11::DeleteAllButtons
/**
//
//  @author        Abraham J. Velez
//  @version       18/2/2003   15:34:3
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool INPLINUXDEVICEMOUSEX11::DeleteAllButtons()
{
  if(buttons.IsEmpty())  return false;

  buttons.DeleteContents();
  buttons.DeleteAll();

  return true;
}




/*-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEX11::CreateAllCursors
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
bool INPLINUXDEVICEMOUSEX11::CreateAllCursors()
{
  INPCURSOR* cursor;

  cursor = new INPCURSOR();
  if(!cursor) return false;

  cursor->SetID(INPCURSOR_ID_MOUSE);

  cursor->SetHavePreSelect(true);

  cursors.Add(cursor);

  return true;
}



//-------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEX11::DeleteAllButtons
/**
//
//  @author        Abraham J. Velez
//  @version       18/2/2003   15:34:3
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool INPLINUXDEVICEMOUSEX11::DeleteAllCursors()
{
  if(cursors.IsEmpty())  return false;

  cursors.DeleteContents();
  cursors.DeleteAll();

  return true;
}

/*-------------------------------------------------------------------
//   INPLINUXDEVICEMOUSEX11::SetScreen
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  30/10/2017 16:01:57
//  @return   bool :
//
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
bool INPLINUXDEVICEMOUSEX11::SetScreen(void* screenpointer)
{
    this->grpscreenx11 = (GRPLINUXSCREENX11*)screenpointer;
    if (grpscreenx11)
    {
        XSelectInput(grpscreenx11->GetDisplay(), (*grpscreenx11->GetWindow()), ButtonPressMask | ButtonReleaseMask | KeyPressMask | KeyReleaseMask);
    }
    return true;
}


