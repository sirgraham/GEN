/*------------------------------------------------------------------------------------------
//  DIOXDEBUGINTERSTREAMS.H
*/
/**
// \class
//
//  DIO XDebug UART to Net
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 16/05/2017 17:05:17
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOXDEBUGINTERSTREAMS_H_
#define _DIOXDEBUGINTERSTREAMS_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XDebug.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


#define DIOXDEBUGINTERSTREAMS_LOGSECTIONID              __L("Debug")

/*---- CLASS -----------------------------------------------------------------------------*/

class DIOXDEBUGINTERSTREAMS
{
  public:

                          DIOXDEBUGINTERSTREAMS   (DIOSTREAM* streamIO, bool iswithlog)
                          {
                            Clean();

                            this->streamIO  = streamIO;
                            this->iswithlog = iswithlog;
                          }

    virtual              ~DIOXDEBUGINTERSTREAMS   ()
                          {
                            Clean();
                          }


    bool                  Process                 ();

  private:

    void                  Clean                   ()
                          {
                            streamIO  = NULL;
                            iswithlog = false;
                          }

    DIOSTREAM*            streamIO;
    bool                  iswithlog;

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

