/*------------------------------------------------------------------------------------------
//  GRPANIMATION.H
*/
/**
// \class
//
//  Holds animation layers for geometric animation
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 10/02/2015 13:29:44
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPANIMATION_H_
#define _GRPANIMATION_H_

/*---- INCLUDES --------------------------------------------------------------------------*/

#include <limits.h>

#include "GRPNamed.h"
#include "GRPAnimationStatus.h"
#include "GRPSubject.h"

#include "XMap.h"
#include "XMemory.h"

#define GRPANIMATIONSTACKMILESTONE_STOPMARGIN 0.003f


class XTIMER;
class XFACTORY;

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*
eTangentAuto                      eTangentTCB             Auto key (spline cardinal).
eTangentUser                      Spline TCB (Tension, Continuity, Bias)
eTangentGenericBreak              Next slope at the left equal to slope at the right.
eTangentBreak                     Independent left and right slopes.
eTangentAutoBreak                 Independent left and right slopes, with next slope at the left equal to slope at the right.
eTangentGenericClamp              Independent left and right slopes, with auto key.
eTangentGenericTimeIndependent    Clamp: key should be flat if next or previous key has the same value (overrides tangent mode).
eTangentGenericClampProgressive   Time independent tangent (overrides tangent mode).
*/

enum GRPANIMATIONCURVEFLAG_ORDER
{
  GRPANIMATIONCURVEFLAG_ORDER_RIGHTSLOPE          = 0 ,
  GRPANIMATIONCURVEFLAG_ORDER_NEXTLEFTSLOPE       = 1 ,
  GRPANIMATIONCURVEFLAG_ORDER_WEIGHT              = 2 ,
  GRPANIMATIONCURVEFLAG_ORDER_RIGHTWEIGHT         = 2 ,
  GRPANIMATIONCURVEFLAG_ORDER_NEXTLEFTWEIGHT      = 3 ,
  GRPANIMATIONCURVEFLAG_ORDER_VELOCITY            = 5 ,
  GRPANIMATIONCURVEFLAG_ORDER_RIGHTVELOCITY       = 4 ,
  GRPANIMATIONCURVEFLAG_ORDER_NEXTLEFTVELOCITY    = 5 ,
  GRPANIMATIONCURVEFLAG_ORDER_TCBTENSION          = 0 ,
  GRPANIMATIONCURVEFLAG_ORDER_TCBCONTINUITY       = 1 ,
  GRPANIMATIONCURVEFLAG_ORDER_TCBBIAS             = 2
 };

enum GRPANIMATIONCURVETANGENTMODE
{
  GRPANIMATIONCURVETANGENTMODE_CUBIC              = 0x00000008 ,
  GRPANIMATIONCURVETANGENTMODE_LINEAR             = 0x00000004 ,
  GRPANIMATIONCURVETANGENTMODE_CONSTANT           = 0x00000002 ,

  GRPANIMATIONCURVETANGENTMODE_AUTO               = 0x00000100 ,      // Autokey spline cardinal
  GRPANIMATIONCURVETANGENTMODE_TCB                = 0x00000200 ,      // Spline TCB
  GRPANIMATIONCURVETANGENTMODE_USER               = 0x00000400 ,      // Next slope at the left equal to slope at the right
  GRPANIMATIONCURVETANGENTMODE_GENERICBREAK       = 0x00000800 ,      // Independent left and right slopes

  GRPANIMATIONCURVETANGENTMODE_BREAK              = GRPANIMATIONCURVETANGENTMODE_GENERICBREAK |GRPANIMATIONCURVETANGENTMODE_USER              ,
  GRPANIMATIONCURVETANGENTMODE_AUTOBREAK          = GRPANIMATIONCURVETANGENTMODE_BREAK        |GRPANIMATIONCURVETANGENTMODE_AUTO              ,

  GRPANIMATIONCURVETANGENTMODE_GENERICCLAMP       = 0x00001000 ,      // Independent left and right slopes with autokey
  GRPANIMATIONCURVETANGENTMODE_TIMEINDEPENDENT    = 0x00002000 ,      // Key showuld be flat if next or previous has the same value
  GRPANIMATIONCURVETANGENTMODE_CLAMPPROGRESSIVE   = 0x00004000                                | GRPANIMATIONCURVETANGENTMODE_TIMEINDEPENDENT  ,

  GRPANIMATIONCURVETANGENTMODE_WEIGHTEDNONE       = 0x00000000 ,
  GRPANIMATIONCURVETANGENTMODE_WEIGHTEDRIGHT      = 0x01000000 ,
  GRPANIMATIONCURVETANGENTMODE_WEIGHTEDNEXTLEFT   = 0x02000000 ,

  GRPANIMATIONCURVETANGENTMODE_WEIGHTEDALL        = GRPANIMATIONCURVETANGENTMODE_WEIGHTEDRIGHT| GRPANIMATIONCURVETANGENTMODE_WEIGHTEDNEXTLEFT
};

enum GRPANIMATIONCURVETYPE
{
  GRPANIMATIONCURVETYPE_UNKNOWN                   = 0,
  GRPANIMATIONCURVETYPE_TRANSLATION,
  GRPANIMATIONCURVETYPE_ROTATION,
  GRPANIMATIONCURVETYPE_SCALE,
  GRPANIMATIONCURVETYPE_VISIBILITY,
  GRPANIMATIONCURVETYPE_DIFFUSECOLOR,
  GRPANIMATIONCURVETYPE_DIFFUSEFACTOR,
  GRPANIMATIONCURVETYPE_SPECULARCOLOR,
  GRPANIMATIONCURVETYPE_EMISSIVECOLOR,
  GRPANIMATIONCURVETYPE_AMBIENTCOLOR,
  GRPANIMATIONCURVETYPE_SHININESSEXPONENT,
  GRPANIMATIONCURVETYPE_SPECULARFACTOR,
  GRPANIMATIONCURVETYPE_TRANSPARENCYFACTOR,
  GRPANIMATIONCURVETYPE_FOV,
  GRPANIMATIONCURVETYPE_FIELDOFVIEWX,
  GRPANIMATIONCURVETYPE_FIELDOFVIEWY,
  GRPANIMATIONCURVETYPE_INTENSITY,
  GRPANIMATIONCURVETYPE_ROLL,
  GRPANIMATIONCURVETYPE_PITCH,
  GRPANIMATIONCURVETYPE_YAW,
  GRPANIMATIONCURVETYPE_DEFORMPERCENT,
  GRPANIMATIONCURVETYPE_USERVALUE,
  GRPANIMATIONCURVETYPE_MAX
};

enum GRPANIMATIONCURVEAXIS
{
  GRPANIMATIONCURVEAXIS_VISIBILITY,

  GRPANIMATIONCURVEAXIS_X,
  GRPANIMATIONCURVEAXIS_Y,
  GRPANIMATIONCURVEAXIS_Z,

  GRPANIMATIONCURVEAXIS_YAW,
  GRPANIMATIONCURVEAXIS_PITCH,
  GRPANIMATIONCURVEAXIS_ROLL,

  GRPANIMATIONCURVECOLOR_R,
  GRPANIMATIONCURVECOLOR_G,
  GRPANIMATIONCURVECOLOR_B,
  GRPANIMATIONCURVECOLOR_A,

  GRPANIMATIONCURVECOLOR_DIFFUSEFACTOR,
  GRPANIMATIONCURVECOLOR_SPECULARFACTOR,
  GRPANIMATIONCURVECOLOR_TRANSPARENCYFACTOR,
  GRPANIMATIONCURVECOLOR_INTENSITY,

  GRPANIMATIONCURVEFOV_FIELDOFVIEW,
  GRPANIMATIONCURVEFOV_FIELDOFVIEWX,
  GRPANIMATIONCURVEFOV_FIELDOFVIEWY,

  GRPANIMATIONCURVETYPE_SHININESSFACTOR,

  GRPANIMATIONCURVEAXIS_DEFORMPERCENT,

  GRPANIMATIONCURVEAXIS_UNKNOWN,
  GRPANIMATIONCURVEAXIS_USERVALUE,
  GRPANIMATIONCURVEAXIS_MAX
};


enum GRPANIMATIONLOOPMODE
{
  GRPANIMATIONLOOPMODE_NONE            =  0,
  GRPANIMATIONLOOPMODE_REPEAT,
  GRPANIMATIONLOOPMODE_PINGPONG
};

enum GRPANIMATIONPLAYDIRECTION
{
  GRPANIMATIONPLAYDIRECTION_FORWARD      =  0,
  GRPANIMATIONPLAYDIRECTION_BACKWARD
};


/*
eDEFAULT_MODE
eFRAMES120        120 frames/s
eFRAMES100        100 frames/s
eFRAMES60         60 frames/s
eFRAMES50         50 frames/s
eFRAMES48         48 frame/s
eFRAMES30         30 frames/s BLACK & WHITE NTSC
eFRAMES30_DROP    30 frames/s use when display in frame is selected(equivalent to NTSC_DROP)
eNTSC_DROP_FRAME  29.97002617 frames/s drop COLOR NTSC
eNTSC_FULL_FRAME  29.97002617 frames/s COLOR NTSC
ePAL              25 frames/s PAL/SECAM
eCINEMA           24 frames/s
eFRAMES1000       1000 milli/s (use for date time)
eCINEMA_ND        23.976 frames/s
eCUSTOM           Custom Framerate value
*/

enum GRPANIMATIONCONTROLTIMEFPS
{
  GRPANIMATIONCONTROLTIMEFPS_DEFAULT_MODE         =  0 ,
  GRPANIMATIONCONTROLTIMEFPS_FRAMES120            =  1 ,
  GRPANIMATIONCONTROLTIMEFPS_FRAMES100            =  2 ,
  GRPANIMATIONCONTROLTIMEFPS_FRAMES60             =  3 ,
  GRPANIMATIONCONTROLTIMEFPS_FRAMES50             =  4 ,
  GRPANIMATIONCONTROLTIMEFPS_FRAMES48             =  5 ,
  GRPANIMATIONCONTROLTIMEFPS_FRAMES30             =  6 ,
  GRPANIMATIONCONTROLTIMEFPS_FRAMES30_DROP        =  7 ,
  GRPANIMATIONCONTROLTIMEFPS_NTSC_DROP_FRAME      =  8 ,
  GRPANIMATIONCONTROLTIMEFPS_NTSC_FULL_FRAME      =  9 ,
  GRPANIMATIONCONTROLTIMEFPS_PAL                  = 10 ,
  GRPANIMATIONCONTROLTIMEFPS_CINEMA               = 11 ,
  GRPANIMATIONCONTROLTIMEFPS_FRAMES1000           = 12 ,
  GRPANIMATIONCONTROLTIMEFPS_CINEMA_ND            = 13 ,
  GRPANIMATIONCONTROLTIMEFPS_CUSTOM               = 14 ,
  GRPANIMATIONCONTROLTIMEFPS_TIME_MODE_COUNT      = 15
};

enum GRPANIMATIONCONTROLTIMEPROTOCOLTYPE
{
  GRPANIMATIONCONTROLTIMEPROTOCOLTYPE_SMPTE                   ,
  GRPANIMATIONCONTROLTIMEPROTOCOLTYPE_FRAME                   ,
  GRPANIMATIONCONTROLTIMEPROTOCOLTYPE_DEFAULT_PROTOCOL        ,
  GRPANIMATIONCONTROLTIMEPROTOCOLTYPE_TIME_PROTOCOL_COUNT
};


#define GRPANIMATION_FBXSECOND                    46186158000
#define GRPANIMATION_FBXMILISECOND                46186158
#define GRPANIMATION_FBXMICROSECOND               46186.158f


/*---- CLASS -----------------------------------------------------------------------------*/

class GRPBEZIERSPLINE;
class GRPANIMATIONCURVE;
class GRPANIMATIONNODE;
class GRPANIMATIONLAYER;
class GRPANIMATIONSTACK;
class GRPANIMATIONCONTROL;
class GRPINTERFACEANIMABLE;


//------------------- CLASS GRPANIMATIONCURVE ---------------------------------------------------------------------------------------------------------------------

class GRPANIMATIONCURVE : public virtual GRPNAMED
{
  public:

                                        GRPANIMATIONCURVE               ()
                                        {
                                          Clean();
                                          GRPNAMED::classtype = GRPNAMED_TYPE_GRPANIMATIONCURVE;
                                        }

    virtual                            ~GRPANIMATIONCURVE               ();

    void                                Init                            ();

    XQWORD                              GetAttribute                    (XQWORD index,XQWORD* offset);

    bool                                Update                          (XQWORDSIG t);
    void                                UpdateAsConstant                (XQWORDSIG t,XDWORD key)    {     target = (float)(keyvalues.FastGet(key));   }
    void                                UpdateAsLinear                  (XQWORDSIG t,XDWORD key);
    void                                UpdateAsCubic                   (XQWORDSIG t,XDWORD key);

    bool                                Active;


    float                               target;

    XVECTOR<XQWORDSIG>                  keytime;
    XVECTOR<double>                     keyvalues;
    XVECTOR<double>                     keyslopes;

    XVECTOR<XQWORD>                     keyattributes;
    XVECTOR<long long>                  keyattrdatafloat;
    XVECTOR<XDWORD>                     keyattrrefcount;

    XVECTOR<int>                        UnpackedAttributes;
    XVECTOR<float>                      fkeytime;


    int                                 currentkey;

    XVECTOR<GRPBEZIERSPLINE*>           bcs;

    GRPANIMATIONNODE*                   ParentNode;

    XQWORDSIG                           firsttime;
    XQWORDSIG                           lasttime;

    float                               firstvalue;
    float                               lastvalue;
    XDWORD                              nkeys;
    XDWORD                              bcsSize;
    int                                 nkeyattrrefcount;

    double                              initime;
    double                              endtime;

    XQWORDSIG                           mintime;
    XQWORDSIG                           maxtime;

  private:

      void                              Clean()
                                        {
                                          initime                     =0.0;
                                          endtime                     =0.0;
                                          target                      =0;
                                          ParentNode                  =NULL;
                                          Active                      =true;
                                          currentkey                  =0;
                                          nkeyattrrefcount            =0;
                                          firsttime                   =LLONG_MAX;
                                          lasttime                    =LLONG_MIN;
                                          keytime.SetIsMulti          (true);
                                          keyvalues.SetIsMulti        (true);
                                          keyattributes.SetIsMulti    (true);
                                          keyattrdatafloat.SetIsMulti (true);
                                          keyattrrefcount.SetIsMulti  (true);
      }
};

//------------------- CLASS GRPANIMATIONNODE ---------------------------------------------------------------------------------------------------------------------


class GRPANIMATIONNODE : public virtual GRPNAMED
{
  public:
                                        GRPANIMATIONNODE                ();
    virtual                            ~GRPANIMATIONNODE                ();

    GRPANIMATIONCURVETYPE               GetType                         ();
    void                                SetType                         (GRPANIMATIONCURVETYPE  type);
    XVECTOR<GRPANIMATIONCURVE*>*        GetCurves                       ();

    GRPINTERFACEANIMABLE*               GetTarget                       ();
    void                                SetTarget                       (GRPINTERFACEANIMABLE*  target);

    XQWORD                              GetTargetTag                    ();
    void                                SetTargetTag                    (XQWORD tag);

    XSTRING*                            GetTargetName                   ();
    void                                SetTargetName                   (XCHAR* name);


    XMAP<XDWORD,float>*                 GetDefaultValues                ();
    void                                SetLayer                        (GRPANIMATIONLAYER*     layer);
    GRPANIMATIONLAYER*                  GetLayer                        ();
    XMAP<XDWORD,GRPANIMATIONCURVE*>*    GetCurveConnections             ();
    XMAP<XDWORD,float>*                 GetOffsets                      ();

    void                                Update                          (XQWORDSIG t);

    bool                                AddConnection                   (GRPANIMATIONCURVEAXIS  axis, GRPANIMATIONCURVE*    curve);
    bool                                AttachTo                        (XCHAR*                 name, GRPINTERFACEANIMABLE* animable);


    XQWORDSIG                           GetMinTime                      ();
    XQWORDSIG                           GetMaxTime                      ();

    void                                SetMinTime                      (XQWORDSIG);
    void                                SetMaxTime                      (XQWORDSIG);

    XVECTOR<XDWORD>*                    GetCurveAxis                    ()                                                                          { return &curvesaxis; }

    bool                                removeme;


protected:

    GRPINTERFACEANIMABLE*               target;
    XQWORD                              targetTag;
    XSTRING                             targetName;

    GRPANIMATIONLAYER*                  layer;

    GRPANIMATIONCURVETYPE               type;

    XVECTOR<GRPANIMATIONCURVE*>         curves;
    XVECTOR<XDWORD>                     curvesaxis;

    XMAP  <XDWORD,GRPANIMATIONCURVE*>   CurveConnections;
    XMAP  <XDWORD,float>                DefaultNodeValues;
    XMAP  <XDWORD,float>                OffsetValues;

    XQWORDSIG                           mintime;
    XQWORDSIG                           maxtime;

    XVECTOR<GRPINTERFACEANIMABLE*>      AdditionalTargets;

private:

    void                                Clean                           ()
                                        {
                                          type          = GRPANIMATIONCURVETYPE_UNKNOWN;
                                          target        = NULL;
                                          layer         = NULL;
                                          curves.SetIsMulti(true);
                                          curvesaxis.SetIsMulti(true);
                                          removeme      = false;
                                          targetTag     = 0;
                                          targetName    = __L("NoTargetName");
                                          mintime       = 0;
                                          maxtime       = 0;
                                          mintime         =LLONG_MAX;
                                          maxtime         =LLONG_MIN;
                                        }
};


//------------------- CLASS GRPANIMATIONLAYER ---------------------------------------------------------------------------------------------------------------------

class GRPANIMATIONLAYER : public virtual GRPNAMED
{
  public:

                                        GRPANIMATIONLAYER               ();
    virtual                            ~GRPANIMATIONLAYER               ();

    XDWORD                              GetStatus                       ();
    bool                                GetIsFinished                   ();

    void                                SetMaxTime                      (XQWORDSIG maxframe);
    void                                SetMinTime                      (XQWORDSIG minframe);

    bool                                IsSync                          ();
    void                                SetSync                         (bool synced);

    bool                                IsActive                        ();
    void                                SetActive                       (bool active);

    bool                                IsFinished                      ();
    void                                SetIsFinished                   (bool finished);

    float                               GetSpeedMultiplier              ();
    void                                SetSpeedMultiplier              (float speedmultipler);

    void                                SetLoopMode                     (GRPANIMATIONLOOPMODE loopmode);
    GRPANIMATIONLOOPMODE                GetLoopMode                     ();

    bool                                thereareremovables;

    void                                Stop                            ();
    void                                Play                            ();
    void                                Pause                           ();
    void                                Rewind                          ();
    void                                Forward                         ();
    void                                Reverse                         ();


    void                                Update                          (XQWORDSIG t);

    void                                AddAnimationNode                (GRPANIMATIONNODE* node);

    void                                AddCurve                        (GRPANIMATIONCURVE* curve);

    XVECTOR<GRPANIMATIONCURVE*>*        GetCurves                       ();

    XVECTOR<GRPANIMATIONNODE*>*         GetAnimationNodes               ();
    void                                SetAnimationNodes               (XVECTOR<GRPANIMATIONNODE*> animationnodes);

    void                                SetFrame                        (XQWORDSIG frame);
    XQWORDSIG                           GetFrame                        ();

    bool                                SetStack                        (GRPANIMATIONSTACK* stack);
    GRPANIMATIONSTACK*                  GetStack                        ();

    void                                SetStatus                       (GRPANIMATION_STATUS status);

    XQWORDSIG                           GetMinTime                      ();
    XQWORDSIG                           GetMaxTime                      ();

    void                                SetLocalTime                    (XQWORDSIG localtime);
    void                                SetLastStackTime                (XQWORDSIG laststacktime);

    XQWORDSIG                           GetLocalTime                    ();
    XQWORDSIG                           GetLastStackTime                ();

  protected:

    XVECTOR<GRPANIMATIONNODE*>          animationnodes;
    XVECTOR<GRPANIMATIONCURVE*>         curves;

    GRPANIMATIONLOOPMODE                loopmode;
    float                               speedmultiplier;

    XQWORDSIG                           mintime;
    XQWORDSIG                           maxtime;
    XQWORDSIG                           localtime;
    XQWORDSIG                           laststacktime;

    bool                                synced;
    bool                                active;
    bool                                finished;

    GRPANIMATION_STATUS                 status;

    GRPANIMATIONSTACK*                  stack;
    GRPANIMATIONPLAYDIRECTION           direction;

private:

    void                                Clean                           ()
                                        {
                                          loopmode            = GRPANIMATIONLOOPMODE_NONE;
                                          speedmultiplier     = 0.0f;

                                          maxtime             = 0;
                                          mintime             = 0;

                                          localtime           = 0;
                                          laststacktime       = 0;

                                          synced              = true;
                                          active              = false;
                                          finished            = false;

                                          status              = GRPANIMATION_STATUS_STOPPED;
                                          thereareremovables  = false;
                                          direction           = GRPANIMATIONPLAYDIRECTION_FORWARD;
                                          curves.SetIsMulti(false);

                                        }
};


//------------------- CLASS GRPANIMATIONSTACKMILESTONE ---------------------------------------------------------------------------------------------



class GRPANIMATIONSTACKMILESTONE : public GRPSUBJECT, virtual public GRPNAMED
{
public:

  enum TYPE
  {
    NONE        =0,
    HARD_STOP     ,
    FINISH        ,
    HARD_FINISH   ,
    SPRING        ,
    DECCELERATE   ,
    BOUNCE        ,
    NOTIFY
  };

      virtual XDWORD                    GetSubjectType              ()    { return GRPNAMED_TYPE_GRPMILESTONE;  }

                                        GRPANIMATIONSTACKMILESTONE()                                        { Clean(); }

      void                              SetPasses                   (int p);
      void                              SetCurrentPass              (int p);
      void                              SetStartFrame               (float f);
      void                              SetEndFrame                 (float f);
      void                              SetStartFrameWithOffset     (float f);
      void                              SetEndFrameWithOffset       (float f);
      void                              SetTension                  (float t);
      virtual void                      SetType                     (XDWORD t);
      void                              SetStack                    (GRPANIMATIONSTACK* s);
      void                              SetStartTime                ();
      void                              SetEndTime                  ();


      int                               GetPasses                   ();
      int                               GetCurrentPass              ();
      float                             GetStartFrame               ();
      float                             GetEndFrame                 ();
      XQWORD                            GetStartTime                ();
      XQWORD                            GetEndTime                  ();
      float                             GetTension                  ();
      virtual XDWORD                    GetType                     ();
      GRPANIMATIONSTACK*                GetStack                    ();

      float                             GetDurationFrames           ();
      XQWORD                            GetDurationTime             ();

      void                              Enable                      (bool isenabled);
      bool                              GetIsActive                 ();
      bool                              GetIsDisabled               ();

      XQWORD                            GetCurrentLocalTime   ();
      XQWORD                            GetLastLocalTime      ();
      void                              SetMilestoneLocalTime (XQWORD nowtime);
      void                              SetMilestoneLastTime  (XQWORD lasttime);

      //----------- actions

      bool                              DoEnter                     (XQWORD t);
      bool                              DoContinue                  (XQWORD t);
      bool                              DoExit                      (XQWORD t);
      bool                              DoDisable                   (bool   disable);


      virtual void                      OnEnter                     (XQWORD t);
      virtual void                      OnContinue                  (XQWORD t);
      virtual void                      OnExit                      (XQWORD t);
      virtual void                      OnDisable                   (bool   disable) {}

      bool                              disabled;       //disable it to reuse on other layers

protected:

      int                               passes;         //number of times the animation can traspass the milestone without activation
      int                               currentpass;    //number of times the animation has traspassed this milestone
      float                             start_frame;    //target frame
      float                             end_frame;      //end frame
      float                             durationframes; //duration on frames
      XQWORD                            start_time;     //target frame time
      XQWORD                            end_time;       //target frame time
      XQWORD                            durationtime;   //duration in time units
      float                             enter_speed;
      float                             tension;        //spring tension
      bool                              isactive;       //is under effect?

      XQWORD                            milestonelocaltime;
      XQWORD                            milestonelasttime;

      GRPANIMATIONSTACKMILESTONE::TYPE  type;           //type of milestone
      GRPANIMATIONSTACK*                stack;          //stack holder

private:

      void Clean()
      {
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
      isactive        =false;   //is under effect?

      disabled        =true;    //disable it to reuse on other layers
      tag             =0;
      type            =NONE;    //type of milestone
      stack           =0;       //stack holder
      }
};

//------------------- CLASS GRPANIMATIONSTACK ---------------------------------------------------------------------------------------------------------------------

class GRPANIMATIONSTACK : public virtual GRPNAMED
{
  public:
                                        GRPANIMATIONSTACK               ();
                                       ~GRPANIMATIONSTACK               ();

  XVECTOR<GRPANIMATIONLAYER*>*          GetLayers                       ();
  void                                  SetLayers                       (XVECTOR<GRPANIMATIONLAYER*> layers);

  bool                                  AddLayer                        (GRPANIMATIONLAYER* layer);

  GRPANIMATIONLAYER*                    GetLayer                        ();
  GRPANIMATIONLAYER*                    GetLayer                        (int layer);
  void                                  SetCurrentLayer                 (int currentlayer);
  XQWORD                                GetCurrentLayer                 ();

  XQWORDSIG                             GetMaxTime                      ();
  void                                  SetMaxTime                      (XQWORDSIG maxtime);
  XQWORDSIG                             GetMinTime                      ();
  void                                  SetMinTime                      (XQWORDSIG mintime);

  int                                   GetFrames                       ();

  XQWORDSIG                             GetMaxFrame                     ();
  void                                  SetMaxFrame                     (XQWORDSIG minframes);

  XQWORDSIG                             GetMinFrame                     ();
  void                                  SetMinFrame                     (XQWORDSIG minframes);


  void                                  SetFrame                        (XQWORDSIG frame);
  XQWORDSIG                             GetFrame                        ();
  void                                  HaltFrame                       (XQWORDSIG frame);

  void                                  Update                          (XQWORDSIG t);
  bool                                  IsActive                        ();
  bool                                  GetInitialActive                ()                                                              { return initialactive;     }
  void                                  SetInitialActive                (bool init)                                                     { this->initialactive=init; }
  void                                  SetActive                       (bool active);
  GRPANIMATION_STATUS                   GetStatus                       ();
  void                                  SetStatus                       (GRPANIMATION_STATUS status)                                    { this->status = status;    }

  bool                                  IsFinished                      ();
  void                                  SetIsFinished                   (bool finished);

  bool                                  SetAnimationControl             (GRPANIMATIONCONTROL* control);
  GRPANIMATIONCONTROL*                  GetAnimationControl             ();

  XQWORDSIG                             GetStackFrame                   ();
  XQWORDSIG                             GetLastStackFrame               ();
  void                                  SetStackFrame                   (XQWORDSIG q);
  void                                  SetLastStackFrame               (XQWORDSIG q);

  void                                  SetLoopMode                     (GRPANIMATIONLOOPMODE mode);
  GRPANIMATIONLOOPMODE                  GetLoopMode                     ();


  bool                                  Trim                            (XQWORDSIG start_frame, XQWORDSIG end_frame);

  bool                                  Play                            ();
  bool                                  Pause                           ();
  bool                                  Stop                            ();
  bool                                  Rewind                          ();
  bool                                  Forward                         ();
  bool                                  Reverse                         ();

  bool                                  IsSynced                        ();
  void                                  SetIsSynced                     (bool b);
  void                                  SetFrameSpeed                   (float speed);
  float                                 GetFrameSpeed                   ();
  void                                  SetFrameAcceleration            (float acc);
  float                                 GetFrameAcceleration            ();
  void                                  SetFriction                     (float fric);
  float                                 GetFriction                     ();

  bool                                  AddMilestone                    (GRPANIMATIONSTACKMILESTONE*);

  XVECTOR<GRPANIMATIONSTACKMILESTONE*>* GetMilestones                   ();
  GRPANIMATIONSTACKMILESTONE*           GetMilestone                    (int n=0);
  void                                  CheckMilestones                 (XQWORDSIG increment);
  void                                  DisableMilestones               ();

  int                                   GetFrameOfTime                  (XQWORDSIG time);
  XQWORDSIG                             GetTimeOfFrame                  (int frame);

  void                                  SetDirection                    (GRPANIMATIONPLAYDIRECTION direction);

  GRPANIMATIONNODE*                     FindAnimationNode               (XCHAR* name, GRPANIMATIONCURVETYPE type, GRPANIMATIONCURVEAXIS axis);

  void                                  SetID                           (XDWORD id) { this->id=id;      }
  XDWORD                                GetID                           ()          { return this->id;  }

protected:

  XDWORD                                id;

  XVECTOR<GRPANIMATIONLAYER*>           layers;
  XQWORDSIG                             maxframes;
  XQWORDSIG                             minframes;

  XQWORDSIG                             maxtime;
  XQWORDSIG                             mintime;


  int                                   currentlayer;
  GRPANIMATION_STATUS                   status;
  GRPANIMATION_STATUS                   initial_status;
  bool                                  active;
  bool                                  initialactive;

  XQWORDSIG                             stackFrame;
  XQWORDSIG                             laststacktime;
  bool                                  synced;
  float                                 speed;
  float                                 speed_acceleration;

  float                                 friction;
  bool                                  finished;



  XVECTOR<GRPANIMATIONSTACKMILESTONE*>  milestones;

  GRPANIMATIONLOOPMODE                  loopmode;

  GRPANIMATIONCONTROL*                  animationcontrol;

private:

  void                                  Clean                           ()
                                        {
                                          currentlayer        =0;
                                          //maxframes           =0;
                                          //minframes           =0;
                                          maxtime             =0;
                                          mintime             =0;
                                          active              =true;
                                          status              =GRPANIMATION_STATUS_PLAYING;
                                          loopmode            =GRPANIMATIONLOOPMODE_NONE;
                                          animationcontrol    =NULL;
                                          synced              =false;
                                          finished            =false;
                                          speed               =1.0f;
                                          speed_acceleration  =0.0f;
                                          friction            =0.0f;
                                          stackFrame          =0;
                                          laststacktime       =0;
                                          id                  =0;
                                        }
};


//------------------- CLASS GRPANIMATIONCONTROL ---------------------------------------------------------------------------------------------------------------------
class GRPOBJECT;
class GRPANIMATIONCONTROL
{
  public:
      enum
      {
          DIRECT,
          REVERSED
      };

                                                    GRPANIMATIONCONTROL                         ();
                                                   ~GRPANIMATIONCONTROL                         ();

    GRPANIMATIONCONTROLTIMEFPS                      GetControlTimeFPS                           ();
    void                                            SetControlTimeFPS                           (GRPANIMATIONCONTROLTIMEFPS controltimeFPS);

    GRPANIMATIONCONTROLTIMEPROTOCOLTYPE             GetControlTimeProtocolType                  ();
    void                                            SetControlTimeProtocolType                  (GRPANIMATIONCONTROLTIMEPROTOCOLTYPE controltimeprotocoltype);
    double                                          GetFramesPerSecondByMode                    ();
    GRPANIMATION_STATUS                             GetStatusType                               ();
    void                                            SetStatusType                               (GRPANIMATION_STATUS statustype);

    float                                           GetFrameSpeed                               ();
    void                                            SetFrameSpeed                               (float framespeed);

    XQWORD                                          GetCurrentFrame                             ();
    void                                            SetCurrentFrame                             (XQWORD currentframe);
    void                                            SetCurrentFrame                             (float currentframe);

    void                                            SetFrame                                    (XQWORD frame);

    XQWORD                                          GetTotalFrames                              ();


    GRPANIMATIONLOOPMODE                            GetLoopMode                                 ();
    void                                            SetLoopMode                                 (GRPANIMATIONLOOPMODE loopmode);

    bool                                            AddStack                                    (GRPANIMATIONSTACK* stack, XDWORD id=0);

    XMAP<XDWORD,GRPANIMATIONSTACK*>*                GetStacks                                   ();
    GRPANIMATIONSTACK*                              GetStack                                    ();
    GRPANIMATIONSTACK*                              GetStack                                    (XDWORD i);

    bool                                            GetStateStack                               (int i);
    GRPANIMATIONSTACK*                              SetStateStack                               (int i, bool status);
    GRPANIMATIONSTACK*                              SetStateStack                               (XCHAR*,bool status);
    void                                            DisableStacks                               ();

    bool                                            GetIsStopped                                ();
    void                                            Play                                        ();
    void                                            Stop                                        ();
    void                                            Begin                                       ();
    void                                            Pause                                       ();
    void                                            Pause                                       (XDWORD frame);
    void                                            Update                                      ();
    void                                            End                                         ();
    void                                            Reset                                       ();

    XTIMER*                                         GetTimer                                    ();
    void                                            SetupFrames                                 ();

    void                                            ShowAllData                                 ();
    void                                            ShowAllNodesAndCurvesAffectingTargetByName  (XCHAR* targetname);
    void                                            ShowAllNodesAndCurvesAffectingTargetByName  (XCHAR* targetname,GRPANIMATIONCURVETYPE type, GRPANIMATIONCURVEAXIS axis);

    bool                                            DettachAnimation                            (GRPINTERFACEANIMABLE* element);
    bool                                            AttachAnimation                             (GRPINTERFACEANIMABLE* element,GRPANIMATIONNODE*);

    void                                            TrimStacks                                  ();
    void                                            SetOrder                                    (XDWORD type);

    GRPOBJECT*                                      GetOwner                                    ();
    void                                            SetOwner                                    (GRPOBJECT* obj);


  protected:

    GRPOBJECT*                                      owner;

    GRPANIMATION_STATUS                             statustype;
    float                                           framespeed;

    XQWORDSIG                                       currentframe;

    XQWORDSIG                                       minTime;
    XQWORDSIG                                       maxTime;

    GRPANIMATIONLOOPMODE                            loopmode;
    GRPANIMATIONCONTROLTIMEFPS                      controltimeFPS;
    GRPANIMATIONCONTROLTIMEPROTOCOLTYPE             controltimeprotocoltype;


    XTIMER*                                         xtimer;
    XMAP<GRPINTERFACEANIMABLE*,GRPANIMATIONNODE*>   attachees;

    XQWORD                                          milis;
    //XVECTOR<GRPANIMATIONSTACK*>                     stacks;
    XMAP<XDWORD,GRPANIMATIONSTACK*>                 stacks;
    XDWORD                                          currentstack;

    int                                             order;

private:

    void  Clean()
    {
      owner = NULL;
      statustype              = GRPANIMATION_STATUS_STOPPED;
      framespeed              = 1.0f;
      currentframe            = 0;

      loopmode                = GRPANIMATIONLOOPMODE_NONE;
      currentstack            = 0;
      minTime                 = 0;
      maxTime                 = 0;

      controltimeFPS          = GRPANIMATIONCONTROLTIMEFPS_FRAMES30;
      controltimeprotocoltype = GRPANIMATIONCONTROLTIMEPROTOCOLTYPE_DEFAULT_PROTOCOL;

      xtimer                  = NULL;
      milis                   = 0;

      order                   = REVERSED;
      attachees.SetIsMulti(true);
    }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif




