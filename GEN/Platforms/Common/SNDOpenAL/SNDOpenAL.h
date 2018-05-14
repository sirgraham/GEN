/*------------------------------------------------------------------------------------------
//  SNDOPENALSYSTEM.H
*/
/**
// \file
//
//  OpenAL backend device
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 03/11/2015 14:08:04
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _SNDOPENALSYSTEM_H_
#define _SNDOPENALSYSTEM_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "AL/al.h"
#include "AL/alc.h"

#include "XString.h"
#include "XVector.h"
#include "XBase.h"
#include "XSubject.h"

#include "SNDFactory.h"
#include "SNDEvent.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class SNDELEMENT;
class SNDSOURCE;
class SNDFACTORY;
class SNDOPENALSOURCE;
class SNDOPENALELEMENT;
class SNDOPENALSTREAMELEMENT;
class SNDINSTANCE;

class XFACTORY;
class XPATHS;

class SNDOPENAL : public XSUBJECT
{
  public:

                                              SNDOPENAL                 (SNDFACTORY* sndystem);
    virtual                                  ~SNDOPENAL                 ();

    virtual SNDELEMENT*                       AddFile                   (XPATH& xpath, XSTRING* namefile, bool stream);
    virtual SNDELEMENT*                       AddFile                   (XPATH& xpath, XCHAR* namefile, bool stream);
    virtual SNDELEMENT*                       GetFile                   (XSTRING* namefile, bool stream);
    virtual SNDELEMENT*                       GetFile                   (XCHAR* namefile, bool stream);

    virtual bool                              RemoveFile                (SNDELEMENT* element);

    virtual SNDSTREAMELEMENT*                 GetStreamer               ();

    virtual SNDINSTANCE*                      PlaySound                 (SNDELEMENT* element);
    virtual void                              StopSound                 (SNDELEMENT* element);
    virtual SNDINSTANCE*                      PauseSound                (SNDELEMENT* element);

    void                                      Update                    ();
    bool                                      IsAnyPlaying              ();

    void                                      StopAll                   ();

    void                                      SetMasterVolume           (float mastervolume);
    float                                     GetMasterVolume           ()                                                      { return mastervolume;                }

    void                                      IniEvents                 ()
                                              {
                                                this->RegisterEvent(XEVENTTYPE_SOUND);
                                                sndsystem->SetXSUBJECT(this);

                                                isinit = true;
                                              }

    void                                      EndEvents                 ()
                                              {
                                                StopAll();
                                                DeRegisterEvent(XEVENTTYPE_SOUND);
                                                sndsystem->SetXSUBJECT(NULL);

                                              }

    static void                               StreamingThread           (void* param);

  protected:

    ALCdevice*                                device;
    ALCcontext*                               context;
    float                                     mastervolume;

  private:

    bool                                      isinit;

    SNDFACTORY*                               sndsystem;

    XVECTOR<SNDOPENALSOURCE*>                 sources;
    XVECTOR<SNDOPENALELEMENT*>                playqueue;

    XVECTOR<SNDOPENALELEMENT*>                loadedfiles;
    XVECTOR<SNDOPENALSTREAMELEMENT*>          streamelements;

    XVECTOR<SNDELEMENT*>                      deletequeue;

    XDWORD                                    maxchannels;

    XTHREAD*                                  streamthread;
    XMUTEX*                                   streammutex;

    void                                      Clean                     ()
                                              {
                                                sndsystem     = NULL;
                                                device        = NULL;
                                                context       = NULL;
                                                maxchannels   = 0;
                                                mastervolume  = 0.0f;
                                                streamthread  = NULL;
                                                streammutex   = NULL;
                                                isinit        = false;
                                              }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

