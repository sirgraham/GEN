//------------------------------------------------------------------------------------------
//  XSTM32FXXXTHREAD.H
//
/**
// \class
//
//  STM32Fxxx thread class
//
//  @author  Abraham J. Velez
//  @version 06/03/2006 11:41:39
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XSTM32FXXXTHREAD_H_
#define _XSTM32FXXXTHREAD_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XThread.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------


class XSTM32FXXXMUTEX : public XMUTEX
{
  public:
                          XSTM32FXXXMUTEX           ()              { Clean();                  }
    virtual              ~XSTM32FXXXMUTEX           ()              { Clean();                  }

    bool                  Lock                      ()
                          {
                            counterlock++;
                            if(counterlock) islock = true;

                            return true;
                          }

    bool                  UnLock                    ()
                          {
                            if(!counterlock) return false;

                            counterlock--;
                            if(!counterlock)  islock = false;

                            return true;
                          }
  protected:


     void                 Clean                     ()
                          {
                            counterlock  = 0;
                          }

     XDWORD               counterlock;
};



class XSTM32FXXXTHREAD  : public XTHREAD
{
  public:
                          XSTM32FXXXTHREAD          (XTHREADGROUPID groupID, XCHAR* ID,XTHREADFUNCTION function = NULL,void* data = NULL);
     virtual             ~XSTM32FXXXTHREAD          ();

     bool                 Ini                       (bool run = true);
     bool                 Wait                      (int miliseconds = XTHREAD_DEFAULTWAITYIELD);
     bool                 End                       ();


  private:

     void                 Clean                     ()
                          {

                          }

};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

