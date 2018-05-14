/*------------------------------------------------------------------------------------------
//  APPLICATIONUPDATEXEVENT.H
*/
/**
// \class
//
//  Update Application XEvent
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 17/11/2015 13:55:39
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _XAPPLICATIONUPDATEXEVENT_H_
#define _XAPPLICATIONUPDATEXEVENT_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XEvent.h"
#include "XString.h"

#include "InnomaticXEvents.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum XAPPLICATIONUPDATEXEVENTTYPE
{
  XAPPLICATIONUPDATEXEVENTTYPE_UNKNOWN                    = XEVENTTYPE_APPLICATIONUPDATE  ,
  XAPPLICATIONUPDATEXEVENTTYPE_DOUPDATE                                                 ,
};

/*---- CLASS -----------------------------------------------------------------------------*/

class APPLICATIONUPDATEXEVENT : public XEVENT
{
  public:
                                          APPLICATIONUPDATEXEVENT                 (XSUBJECT* subject, XDWORD type = APPLICATIONUPDATEXEVENTTYPE_UNKNOWN, XDWORD family = XEVENTTYPE_PRINTERTICKET) : XEVENT(subject, type, family)
                                          {
                                            Clean();
                                          }

    virtual                              ~APPLICATIONUPDATEXEVENT                 ()
                                          {
                                            Clean();
                                          }

    XSTRING*                              GetDateTime                             ()                  { return &datetime;                                 }

    XDWORD                                GetDelay                                ()                  { return delay;                                     }
    void                                  SetDelay                                (XDWORD delay)      { this->delay = delay;                              }


  private:

    void                                  Clean                                   ()
                                          {
                                            delay = 0;
                                          }

    XSTRING                               datetime;
    XDWORD                                delay;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif




