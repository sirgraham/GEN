/*------------------------------------------------------------------------------------------
//  SNDANIMATIONLISTENER.H
*/
/**
// \class
//
//  Manages sound from animation events
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 13/12/2016 16:16:55
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _SNDANIMATIONLISTENER_H_
#define _SNDANIMATIONLISTENER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "SNDInstance.h"
#include "GRPAnimation.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

class SNDMANAGER;

class SNDANIMATIONLISTENER : public GRPANIMATIONSTACKMILESTONE
{
public:

  enum AUDIOTYPE
  {
    NONE,
    PLAY,
    PLAYANDSTOP,
    ONLYSTOP,
    PLAYONDISABLE,
    STOPONDISABLE,
    PLAYONPASS,
    FADEIN,
    PLAYLIST,
    FADEOUT
  };

                            SNDANIMATIONLISTENER  ();
  virtual                   ~SNDANIMATIONLISTENER ();

  bool                      Init                  ();

  virtual XDWORD            GetSubjectType        ();

  void                      SetAudioType          (XDWORD       type);
  XDWORD                    GetAudioType          ();

  void                      SetLoop               (bool         loop);
  bool                      GetLoop               ();

  void                      SetBaseVolume         (float        basevolume);
  float                     GetBaseVolume         ();

  void                      SetInstance           (SNDINSTANCE* instance);
  SNDINSTANCE*              GetInstance           ();

  void                      SetSampleName         (XCHAR*       name);
  XCHAR*                    GetSampleName         ();

  void                      SetPlaylistName       (XCHAR*       name);
  XCHAR*                    GetPlaylistName       ();

  void                      SetIsRemote           (bool         b);
  void                      SetIsLocal            (bool         b);

  bool                      HandleDisable         (GRPMSGTYPE   msg);
  bool                      Notify                (GRPMSGTYPE   msg,    GRPSUBJECT*   subject);

  bool                      AddSampleToPlaylist   (XDWORD,      XSTRING* );
  bool                      SetNextPlaylistItem   (XDWORD       next);

  void                      Play                  ();
  void                      Stop                  ();

  void                      OnEnter               (XQWORD       t);
  void                      OnExit                (XQWORD       t);
  void                      OnDisable             (bool         disable);

  bool                      GetIsRemote           ();
  bool                      GetIsLocal            ();
  XMAP<XDWORD,SNDELEMENT*>* GetSoundGroup         ();
  XDWORD                    GetNextPlaylistItem   ();

protected:

  XSTRING                   samplename;
  XSTRING                   playlist;

  AUDIOTYPE                 audiotype;
  bool                      loop;
  bool                      isremote;
  bool                      islocal;

  int                       playlistItem;
  float                     basevolume;

  XMAP<XDWORD,SNDELEMENT*>  soundgroup;

  SNDINSTANCE*              sndinstance;
  SNDFACTORY*               sndfactory;

private:

  void Clean()
  {
    type            = GRPANIMATIONSTACKMILESTONE::NONE;   //prevent affecting the animation

    passes          =0;       //number of times the animation can traspass the milestone without activation
    currentpass     =0;       //number of times the animation has traspassed this milestone
    start_frame     =0;       //target frame
    end_frame       =0;       //end frame
    durationframes  =0;       //duration on frames
    start_time      =0;       //target frame time
    end_time        =0;       //target frame time
    durationtime    =0;       //duration in time units
    enter_speed     =0;
    tension         =0;       //spring tension
    tag             =0;
    playlistItem    =0;

    isactive        =false;   //is under effect?
    disabled        =false;   //disable it to reuse on other layers

    stack           =NULL;    //stack holder
    audiotype       =PLAY;

    loop            =false;
    basevolume      =1.0f;

    sndinstance     =NULL;
    sndfactory      =NULL;
  }
};




/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

