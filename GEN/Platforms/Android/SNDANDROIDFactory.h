/*------------------------------------------------------------------------------------------
//  SNDANDROIDFACTORY.H
*/
/**
// \file
//
//  sound system for linux
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 11/11/2015 10:37:51
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _SNDANDROIDFACTORY_H_
#define _SNDANDROIDFACTORY_H_

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

class SNDANDROIDFACTORY : public SNDFACTORY
{
  public:

                                              SNDANDROIDFACTORY         ();
    virtual                                  ~SNDANDROIDFACTORY         ();

    virtual SNDELEMENT*                       AddFile                   (XPATH&     xpath,    XSTRING*    namefile,       bool stream = false);
    virtual SNDELEMENT*                       AddFile                   (XPATH&     xpath,    XCHAR*      namefile,       bool stream = false);
    virtual SNDELEMENT*                       GetFile                   (XSTRING*   namefile, bool        stream = false);
    virtual SNDELEMENT*                       GetFile                   (XCHAR*     namefile, bool        stream = false);

    virtual bool                              RemoveFile                (SNDELEMENT* element);

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
                                                isinit = true;
                                              }

    void                                      EndEvents                 ()
                                              {
                                                sndopenal->EndEvents();
                                                isinit = true;
                                              }

    SNDRESAMPLER*                             GetResampler              ();

    virtual SNDFILE*                          CreateSNDFile             ();
    virtual void                              DeleteSNDFile             (SNDFILE* file);

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

