//------------------------------------------------------------------------------------------
//  XOBSERVER.H
//
/**
// \class
//
//  Basic eXtended Observed class
//
//  @author  Abraham J. Velez
//  @version 09/01/2004 13:24:41
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XOBSERVER_H_
#define _XOBSERVER_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XPublisher.h"
#include "XEvent.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

//---- CLASS -------------------------------------------------------------------------------

class XSUBJECT;
class XPUBLISHER;

class XOBSERVER
{
  public:

                                    XOBSERVER                     ()                                { Clean();                                                                    }
    virtual                        ~XOBSERVER                     ()                                { Clean();                                                                    }

    bool                            SubscribeEvent                (XDWORD type, XSUBJECT* subject)
                                    {
                                      return XPUBLISHER::GetInstance().SubscribeEvent(type, subject, this);
                                    }

    bool                            UnSubscribeEvent              (XDWORD type, XSUBJECT* subject)
                                    {
                                      return XPUBLISHER::GetInstance().UnSubscribeEvent(type, subject, this);
                                    }

    virtual void                    HandleEvent                   (XEVENT* event) = 0;

  private:

    void                            Clean                         ()
                                    {

                                    }
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
