//------------------------------------------------------------------------------------------
//  XPUBLISHER.H
//
/**
// \class
//
//  Basic eXtender Publisher
//
//  @author  Abraham J. Velez
//  @version 09/01/2004 15:21:34
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XPUBLISHER_H_
#define _XPUBLISHER_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XVector.h"
#include "XMap.h"
#include "XThread.h"
#include "XEvent.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

//---- CLASS -------------------------------------------------------------------------------

class  XOBSERVER;
class  XSUBJECT;
class  XPUBLISHER;

class XPUBLISHERENTRY
{
  public:
                                        XPUBLISHERENTRY             ()          { Clean();                          }
    virtual                            ~XPUBLISHERENTRY             ()          { Clean();                          }

    XDWORD                              type;
    XSUBJECT*                           subject;
    XOBSERVER*                          observer;

  private:

    void                                Clean                       ()
                                        {
                                          type      = 0;
                                          subject   = NULL;
                                          observer  = NULL;
                                        }
};




class XPUBLISHER
{
  public:
      static XPUBLISHER&                GetInstance                 ()
                                        {
                                          if(!instance) instance = new XPUBLISHER();

                                          return (*instance);
                                        }

    static bool                         DelInstance                 ()
                                        {
                                          if(instance)
                                            {
                                              delete instance;
                                              instance = NULL;

                                              return true;
                                            }

                                          return false;
                                        }


    bool                                RegisterEvent               (XDWORD type, XSUBJECT* subject);
    bool                                DeRegisterEvent             (XDWORD type, XSUBJECT* subject);
    bool                                DeRegisterAllEvents         ();

    bool                                SubscribeEvent              (XDWORD type, XSUBJECT* subject, XOBSERVER* observer);
    bool                                UnSubscribeEvent            (XDWORD type, XSUBJECT* subject, XOBSERVER* observer);


    void                                Active                      (bool on)                                                       { this->on  = on;                   }
    bool                                IsInEvent                   ()                                                              { return inevent;                   }

    bool                                PostEvent                   (XEVENT* event, XSUBJECT* subject);


  private:
                                        XPUBLISHER                  ();
                                        XPUBLISHER                  (XPUBLISHER const&);        // Don't implement
    virtual                            ~XPUBLISHER                  ();

    void                                operator =                  (XPUBLISHER const&);        // Don't implement

    void                                Clean                       ()
                                        {
                                          on           = true;
                                          inevent      = false;
                                          xmutexevents = NULL;
                                        }

    static XPUBLISHER*                  instance;

    bool                                on;
    bool                                inevent;
    XMUTEX*                             xmutexevents;
    XMAP<XDWORD, XSUBJECT*>             subjectsmap;
    XVECTOR<XPUBLISHERENTRY*>           eventsvector;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif
