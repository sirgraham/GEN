/*------------------------------------------------------------------------------------------
//  GRPOBSERVER.H
*/
/**
// \class
//
//  Graphic observer
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 03/11/2015 12:47:57
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPOBSERVER_H_
#define _GRPOBSERVER_H_

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


enum GRPMSGTYPE
{
  GRPMSG_UNKNOWN=0,
  GRPMSG_CREATED,       // the object was created by notifier
  GRPMSG_DESTROYED,     // the object was destroyed by notifier

  GRPMSG_ATTACHED,      // the object was attached to notifier
  GRPMSG_DETTACHED,     // the object was dettached from notifier

  GRPMSG_INSTANCED,     // the object wants to be an istance of notifier
  GRPMSG_DEREFERENCE,   //  when disembodied

  GRPMSG_ADDED,         // the object was added to some list
  GRPMSG_REMOVED,       // the object was removed from some list
  GRPMSG_ORDER,         // the object requests new order in list

  GRPMSG_ENTER,         // the object entered an animation milestone
  GRPMSG_CONTINUE,      // the object continues on an animation milestone
  GRPMSG_EXIT,          // the object exited an animation milestone
  GRPMSG_RESTART,       // the object is restarting
  GRPMSG_PAUSE,         // the object is paused

  GRPMSG_ENABLE,        // the object requests enabling
  GRPMSG_DISABLE,       // the object requests disabling

  GRPMSG_STEP,          // the object is heart beating
  GRPMSG_NOTIFY,

  GRPMSG_INIT,          // the object is initialized
  GRPMSG_END,           // the object is ended

  GRPMSG_CONNECTION,    // the object connected
  GRPMSG_DISCONNECTION, // the object disconnected

  GRPMSG_CONTEXTINIT,   // graphics context is initialized
  GRPMSG_CONTEXTLOSS,   // we lost the graphics context

  GRPMSG_FRAMESTARTED,  // renderer started
  GRPMSG_FRAMEENDED,    // renderer finished

  GRPMSG_PASSSTARTED,   // render pass started
  GRPMSG_PASSENDED,     // render pass ended

  GRPMSG_VISIBILITY_CHANGED,

  GRPMSG_PROPERTY_CHANGED,

  SNDPLAY,
  SNDSTOP,
  SNDLOOP,

  UI_WIDGET_RELOAD,
  UI_WIDGET_ACTIVE,
  UI_WIDGET_HOVER,
  UI_WIDGET_DRAG,
  UI_WIDGET_RELEASE,
  UI_WIDGET_EXIT,

  UI_MOUSE_MOVE,
  UI_MOUSE_CLICK,
  UI_MOUSE_DOUBLE_CLICK,
  UI_MOUSE_PINCH_IN,
  UI_MOUSE_PINCH_OUT,

  UI_SCROLL_UP,
  UI_SCROLL_DOWN,

  GRPMSG_UP,
  GRPMSG_DOWN,
  GRPMSG_TOP,
  GRPMSG_BOTTOM,

  GRPMSG_CUSTOM,        // custom action
  GRPMSG_MAX
};

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPSUBJECT;

class GRPOBSERVER
{
public:
                              ~GRPOBSERVER  ();
        XVECTOR<GRPSUBJECT*>* GetSubjects   ();
        virtual bool          Notify        (GRPMSGTYPE     msg,    GRPSUBJECT*     notifier)=0;
        bool                  Subscribe     (GRPSUBJECT*    subject);
        void                  Unsubscribe   ();
        void                  Unsubscribe   (GRPSUBJECT* subject);

protected:

        XVECTOR<GRPSUBJECT*>  subjects;

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

