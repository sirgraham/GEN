/*------------------------------------------------------------------------------------------
//  SNDSOURCE.H
*/
/**
// \file
//
//  base class for sources
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 11/11/2015 10:02:29
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _SNDSOURCE_H_
#define _SNDSOURCE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class SNDELEMENT;
class SNDINSTANCE;

class SNDSOURCE
{
  public:

                                    SNDSOURCE                           ()                              { Clean();                          }
    virtual                        ~SNDSOURCE                           ()                              { Clean();                          }

    virtual void                    Stop                                ()                              {                                   }
    virtual void                    Pause                               ()                              {                                   }
    virtual void                    UnPause                             ()                              {                                   }

    virtual void                    SetLoop                             (bool loop)                     {                                   }

    virtual bool                    IsPLaying                           ()                              { return false;                     }
    virtual bool                    IsStopped                           ()                              { return false;                     }
    virtual bool                    IsPaused                            ()                              { return false;                     }

    virtual void                    SetVolume                           (float volume)                  {                                   }
    virtual float                   GetVolume                           ()                              { return 0.0f;                      }

    virtual void                    SetPitch                            (float pitch)                   {                                   }
    virtual float                   GetPitch                            ()                              { return 0.0f;                      }

    virtual void                    SetElement                          (SNDELEMENT* element)           { this->element = element;          }
    virtual SNDELEMENT*             GetElement                          ()                              { return this->element;             }

    virtual void                    SetSecondsOffset                    (float seconds)                 {                                   }
    virtual void                    SetSamplesOffset                    (int samples)                   {                                   }


    void                            SetInstance                         (SNDINSTANCE* sndinstance)      { this->sndinstance = sndinstance;  }
    SNDINSTANCE*                    GetInstance                         ()                              { return this->sndinstance;         }
    bool                            IsInstancePlaying                   ();

  protected:

    SNDELEMENT*                     element;
    SNDINSTANCE*                    sndinstance;

  private:

    void                            Clean                               ()
                                    {
                                      element     = NULL;
                                      sndinstance = NULL;
                                    }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

