//------------------------------------------------------------------------------------------
//  DIOANDROIDSTREAMICMP.H
//
/**
// \class
//
//  ANDROID Data IO Stream UDP class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOANDROIDSTREAMICMP_H_
#define _DIOANDROIDSTREAMICMP_H_

#if defined(DIO_ACTIVE) && defined(DIOICMP_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XFSMachine.h"

#include "DIOStreamICMP.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOANDROIDICMPFSMEVENTS
{
  DIOANDROIDICMPFSMEVENT_NONE               = 0 ,

  DIOANDROIDICMPFSMEVENT_GETTINGCONNEXION       ,
  DIOANDROIDICMPFSMEVENT_CONNECTED              ,
  DIOANDROIDICMPFSMEVENT_WAITINGTOREAD          ,
  DIOANDROIDICMPFSMEVENT_SENDINGDATA            ,
  DIOANDROIDICMPFSMEVENT_DISCONNECTING          ,

  DIOANDROIDICMP_LASTEVENT

};


enum DIOANDROIDICMPFSMSTATES
{
  DIOANDROIDICMPFSMSTATE_NONE               = 0 ,

  DIOANDROIDICMPFSMSTATE_GETTINGCONNEXION       ,
  DIOANDROIDICMPFSMSTATE_CONNECTED              ,
  DIOANDROIDICMPFSMSTATE_WAITINGTOREAD          ,
  DIOANDROIDICMPFSMSTATE_SENDINGDATA            ,
  DIOANDROIDICMPFSMSTATE_DISCONNECTING          ,

  DIOANDROIDICMP_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class XTHREAD;

class DIOANDROIDSTREAMICMP : public DIOSTREAMICMP , public XFSMACHINE
{
  public:
                              DIOANDROIDSTREAMICMP                    ( );
    virtual                  ~DIOANDROIDSTREAMICMP                    ();

    bool                      Open                                  ();
    bool                      Disconnect                            ();
    bool                      Close                                 ();

  protected:

    int                       IsReadyConnect                        (int sock);

  private:

    void                      Clean                                 ();
    static void               ThreadRunFunction                     (void* thread);

    XTHREAD*                  threadconnexion;
    int                       handle;
    XSTRING                   remoteaddress;
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

#endif

