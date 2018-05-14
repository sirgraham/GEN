/*------------------------------------------------------------------------------------------
//  SNDLINUXFACTORY.H
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

#ifndef _SNDLINUXFACTORY_H_
#define _SNDLINUXFACTORY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "SNDFactory.h"
#include "SNDOpenAL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class SNDOPENAL;
class SNDELEMENT;
class SNDSTREAMELEMENT;
class SNDINSTANCE;
class SNDSOURCE;
class SNDFILE;
class XFACTORY;
class XPATHS;
class XPUBLISHER;

class SNDLINUXFACTORY : public SNDFACTORY
{
  public:

                                              SNDLINUXFACTORY           ();
    virtual                                  ~SNDLINUXFACTORY           ();

    virtual SNDELEMENT*                       AddFile                   (XPATH& xpath, XSTRING* namefile, bool stream = false);
    virtual SNDELEMENT*                       AddFile                   (XPATH& xpath, XCHAR* namefile, bool stream = false);
    virtual SNDELEMENT*                       GetFile                   (XSTRING* namefile, bool stream = false);
    virtual SNDELEMENT*                       GetFile                   (XCHAR* namefile, bool stream = false);


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
                                                isinit = false;
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

