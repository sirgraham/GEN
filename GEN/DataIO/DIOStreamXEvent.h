//------------------------------------------------------------------------------------------
//  DIOSTREAMXEVENT.H
//
/**
// \class
//
//  Data IO Stream Event class
//
//  @author  Abraham J. Velez
//  @version 23/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTREAMXEVENT_H_
#define _DIOSTREAMXEVENT_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XEvent.h"

#include "DIOStreamDevice.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOSTREAMXEVENTTYPE
{
  DIOSTREAMXEVENTTYPE_UNKNOWN             = XEVENTTYPE_DIOSTREAM  ,
  DIOSTREAMXEVENTTYPE_GETTINGCONNEXION                            ,
  DIOSTREAMXEVENTTYPE_CONNECTED                                   ,
  DIOSTREAMXEVENTTYPE_DISCONNECTED                                ,
  DIOSTREAMXEVENTTYPE_EXCEPTIONDEVICE                             ,
  DIOSTREAMXEVENTTYPE_COOPERATIONDEVICE                           ,
};


//---- CLASS -------------------------------------------------------------------------------


class DIOSTREAM;


class DIOSTREAMXEVENT : public XEVENT
{
  public:
                                DIOSTREAMXEVENT                 (XSUBJECT* subject, XDWORD type = DIOSTREAMXEVENTTYPE_UNKNOWN, XDWORD family = XEVENTTYPE_DIOSTREAM) : XEVENT(subject, type, family)
                                {
                                  Clean();
                                }

    virtual                    ~DIOSTREAMXEVENT                 ()                                                                                          { Clean();                          }

    DIOSTREAM*                  GetDIOStream                    ()                                                                                          { return diostream;                 }
    void                        SetDIOStream                    (DIOSTREAM* diostream)                                                                      { this->diostream = diostream;      }

    DIOSTREAMDEVICE*            GetDevice                       ()                                                                                          { return &device;                   }
    void                        SetDevice                       (DIOSTREAMDEVICE& device)                                                                   {                                   }

  private:

    void                        Clean                           ()
                                {
                                  diostream = NULL;
                                }

    DIOSTREAM*                  diostream;
    DIOSTREAMDEVICE             device;
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif
