/*------------------------------------------------------------------------------------------
//  UIEVENT.H
*/
/**
// \file
//
//  Event class for communication between controls, the manager and the world
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 05/10/2015 12:30:56
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIEVENT_H_
#define _UIEVENT_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XEvent.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
// add all the events needed once the complete list of widgets is in stone
enum UIXEVENTTYPE
{
  // window events
  UIXEVENTTYPE_UNKNOWN              = XEVENTTYPE_UI , // not sure if it should init to this or 0, as these events are separate
  UIXEVENTTYPE_MOVE                                 ,
  UIXEVENTTYPE_RESIZE                               ,

  UIXEVENTTYPE_VALUE_CHANGE                         ,

  UIXEVENTTYPE_TRIGGER                              ,
  UIXEVENTTYPE_CLICK                                ,
  UIXEVENTTYPE_PRESS                                ,
  UIXEVENTTYPE_LONGPRESS                            ,
  UIXEVENTTYPE_RELEASE                              ,
  UIXEVENTTYPE_HOVER                                ,

  UIXEVENTTYPE_CHAGESTATE                           ,

  UIXEVENTTYPE_DIALOG_OK                            ,
  UIXEVENTTYPE_DIALOG_CANCEL                        ,

  UIXEVENTTYPE_MOTION                               ,

  // this one always at the end
  // it's just a guard for looping
  UIXEVENTTYPE_END
};



/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGET;
class UIMOTION;

class UIEVENT : public XEVENT
{
  public:

                                      UIEVENT                           (XSUBJECT *subject, XDWORD type) : XEVENT(subject, type)
                                      {
                                        Clean();
                                        SetEventFamily(XEVENTTYPE_UI);
                                      }

                                      UIEVENT                           (UIEVENT* uievent) : XEVENT(uievent->subject, uievent->type)
                                      {
                                        Clean();
                                        sender      = uievent->sender;
                                        uieventtype = uievent->uieventtype;
                                        SetEventFamily(XEVENTTYPE_UI);
                                      }

    virtual                          ~UIEVENT                           ()
                                      {
                                        Clean();
                                      }

                      void            SetSubject                        (XSUBJECT* sub)                                                 { subject = sub;                    }

                      void            SetSender                         (UIWIDGET* s)                                                   {sender = s;                        }
                      UIWIDGET*       GetSender                         ()                                                              {return sender;                     }

                      void            SetMotion                         (UIMOTION* motion)                                              { this->motion = motion;            }
                      UIMOTION*       GetMotion                         ()                                                              { return motion;                    }

                      UIXEVENTTYPE    GetUIEventType                    ()                                                              { return uieventtype;               }
                      void            SetUIEventType                    (UIXEVENTTYPE type)                                             { uieventtype = type;               }

                      XDWORD          GetWidgetState                    ()                                                              { return this->widgetstate;         }
                      void            SetWidgetState                    (XDWORD widgetstate)                                            { this->widgetstate = widgetstate;  }
  protected:
                      UIXEVENTTYPE    uieventtype;
                      UIWIDGET*       sender;
                      UIMOTION*       motion;
                      XDWORD          widgetstate;
  private:
                      void            Clean                             ()
                                      {
                                        sender        = NULL;
                                        motion        = NULL;
                                        widgetstate   = 0;
                                      }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

