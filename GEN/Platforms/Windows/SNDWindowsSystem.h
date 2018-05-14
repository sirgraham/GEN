/*------------------------------------------------------------------------------------------
//  SNDWINDOWSFACTORY.H
*/
/**
// \file
//
//  sound system for windows
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 11/11/2015 10:37:51
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _SNDWINDOWSFACTORY_H_
#define _SNDWINDOWSFACTORY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "SNDFactory.h"
#include "SNDOpenAL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class SNDOPENAL;
class SNDELEMENT;
class SNDSTREAMELEMENT;
class SNDSOURCE;
class SNDINSTANCE;
class XFACTORY;
class XPATHS;
class XPUBLISHER;

class SNDWINDOWSFACTORY : public SNDFACTORY
{
  public:

                                              SNDWINDOWSFACTORY         ();
    virtual                                  ~SNDWINDOWSFACTORY         ();

    virtual SNDELEMENT*                       AddFile                   (XSTRING* file);
    virtual SNDELEMENT*                       AddFile                   (XCHAR* file);
    virtual SNDELEMENT*                       GetFile                   (XSTRING* file);
    virtual SNDELEMENT*                       GetFile                   (XCHAR* file);

    virtual SNDSTREAMELEMENT*                 GetStreamer               ();

    virtual SNDINSTANCE*                      PlaySound                 (SNDELEMENT* element);
    virtual void                              StopSound                 (SNDELEMENT* element);
    virtual SNDINSTANCE*                      PauseSound                (SNDELEMENT* element);


    virtual bool                              IsAnyPlaying              ();

    virtual void                              StopAll                   ();

    virtual void                              SetMasterVolume           (float mastervolume);
    virtual float                             GetMasterVolume           ();

    void                                      IniEvents                 ()
                                              {
                                                sndopenal->IniEvents();
                                                isinit=true;
                                              }

    void                                      EndEvents                 ()
                                              {
                                                sndopenal->EndEvents();
                                                isinit=false;
                                              }

  protected:
    virtual void                              ImpUpdate                 ();
  private:

    void                                      Clean                     ()
                                              {
                                                sndopenal = NULL;
                                              }

    SNDOPENAL*                                sndopenal;

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

