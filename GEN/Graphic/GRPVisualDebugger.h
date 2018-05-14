/*------------------------------------------------------------------------------------------
//  GRPVISUALDEBUGGER.H
*/
/**
// \class
//
//  Debugs normals, bounding boxes, and AABBs
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 08/06/2015 10:17:19
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPVISUALDEBUGGER_H_
#define _GRPVISUALDEBUGGER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPOpenGL.h"
#include "XVector.h"
#include "GRPPass.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPOBJECT;
class GRPNODE;
class GRPSKINLIMB;
class GRPLIGHT;
class GRPCAMERA;
class GRPOBJECTBUILDER;
class GRPSCENE;
class GRPELEMENT;
class GRPPOINT;
class GRPANIMATIONCONTROL;
class GRPANIMATIONCURVE;

enum GRPDEBUG_FLAG
{
  GRPDEBUG_FLAG_NORMAL=1,
  GRPDEBUG_FLAG_DEBUG =2,
  GRPDEBUG_FLAG_BOTH = GRPDEBUG_FLAG_NORMAL | GRPDEBUG_FLAG_DEBUG
};

enum GRPVISUALDEBUGGER_FLAG
{
  GRPVISUALDEBUGGER_FLAG_NONE                     =0,
  GRPVISUALDEBUGGER_FLAG_BBS                      =1<<1,
  GRPVISUALDEBUGGER_FLAG_CAMERAS                  =1<<2,
  GRPVISUALDEBUGGER_FLAG_LIGHTS                   =1<<3,
  GRPVISUALDEBUGGER_FLAG_DEFORMERS                =1<<4,
  GRPVISUALDEBUGGER_FLAG_NORMALS                  =1<<5,
  GRPVISUALDEBUGGER_FLAG_AABBS                    =1<<6,
  GRPVISUALDEBUGGER_FLAG_LOCATORS                 =1<<7,
  GRPVISUALDEBUGGER_FLAG_ANIMATION_TRANSLATION    =1<<8,
  GRPVISUALDEBUGGER_FLAG_ANIMATION_ROTATION       =1<<9,
  GRPVISUALDEBUGGER_FLAG_ANIMATION_SCALE          =1<<10,
  GRPVISUALDEBUGGER_FLAG_ANIMATION_VISIBILITY     =1<<11,
  GRPVISUALDEBUGGER_FLAG_ANIMATION_OTHERS         =1<<12,
  GRPVISUALDEBUGGER_FLAG_ANIMATION                =1<<13,
  GRPVISUALDEBUGGER_FLAG_ANIMATION_ALL            = GRPVISUALDEBUGGER_FLAG_ANIMATION_TRANSLATION | GRPVISUALDEBUGGER_FLAG_ANIMATION_ROTATION | GRPVISUALDEBUGGER_FLAG_ANIMATION_SCALE | GRPVISUALDEBUGGER_FLAG_ANIMATION_VISIBILITY | GRPVISUALDEBUGGER_FLAG_ANIMATION_OTHERS | GRPVISUALDEBUGGER_FLAG_ANIMATION,
  GRPVISUALDEBUGGER_FLAG_ALL                      = GRPVISUALDEBUGGER_FLAG_BBS | GRPVISUALDEBUGGER_FLAG_CAMERAS | GRPVISUALDEBUGGER_FLAG_LIGHTS | GRPVISUALDEBUGGER_FLAG_DEFORMERS | GRPVISUALDEBUGGER_FLAG_NORMALS | GRPVISUALDEBUGGER_FLAG_ANIMATION_ALL | GRPVISUALDEBUGGER_FLAG_AABBS | GRPVISUALDEBUGGER_FLAG_LOCATORS
};

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPVISUALDEBUGGER
{
public:
                      GRPVISUALDEBUGGER         (GRPSCENE*  scene);
  virtual             ~GRPVISUALDEBUGGER        () {}


  void                CreateScene               (XDWORD flags=GRPVISUALDEBUGGER_FLAG_ALL);

  GRPOBJECT*          CreateBoundingBox         (GRPOBJECT*                 obj);
  GRPOBJECT*          CreateNormal              (GRPOBJECT*                 obj);
  GRPOBJECT*          CreateLocator             (GRPNODE*                   node);
  GRPOBJECT*          CreateLimb                (GRPSKINLIMB*               ode);
  GRPOBJECT*          CreateLight               (GRPLIGHT*                  light);
  GRPOBJECT*          CreateCamera              (GRPCAMERA*                 camera);

  GRPOBJECT*          CreateSkinDebug           (GRPELEMENT*                obj);

  GRPOBJECT*          CreateAnimation           (GRPANIMATIONCONTROL*       animationnode,  XDWORD      flags);
  GRPELEMENT*         ConstructDummy            (XVECTOR<GRPPOINT*>*        Points,         GLFLOAT   Scale=1.0f);
  GRPELEMENT*         ConstructSpline           (XVECTOR<GRPPOINT*>*        Points);
  //GRPELEMENT*         ConstructShape            (GRPSHAPE*);

  double              DebugSize;

protected:

  GRPELEMENT*         CreateBoxLocator          ();
  GRPELEMENT*         CreatePointLocator        ();
  GRPELEMENT*         CreateLightLocator        ();

  GRPELEMENT*         CreateNormalViewerElement (GRPELEMENT*            element);
  GRPELEMENT*         CreateLimbLocator         (GRPSKINLIMB*           node);
  GRPELEMENT*         CreateBoundingBoxElement  (GRPELEMENT*            element);
  GRPELEMENT*         CreateLine                (GRPPOINT               node,           GRPPOINT  node2);

  GRPELEMENT*         CreateAnimationCurve      (GRPANIMATIONCURVE*     curve,          GRPANIMATIONCONTROL* control);
//  GRPELEMENT*         CreateAnimationCurveNodes (GRPANIMATIONCURVE*     curve,          GRPANIMATIONCONTROL* control);


  XVECTOR<GRPOBJECT*> DebugObjects;
  GRPSCENE*           scene;



  double              AnimationDebugSizeMax;
  double              AnimationDebugSizeMin;

private:

  void Clean()
  {
    scene     = NULL;
    DebugSize = 1.0f;

    AnimationDebugSizeMin=100;
    AnimationDebugSizeMax=-100;
  }

};

class GRPDEBUGGERPASS : public GRPPASS
{
public:

    enum {
        DEBUG_FLAGS,
        DEBUG_SIZE
    };
                      GRPDEBUGGERPASS   ();
                      ~GRPDEBUGGERPASS  ();

    void              Start             ();
    void              Finish            ();
    bool              OnAdded           ();

    bool              CreateNormals     (GRPELEMENT* element,           float DebugSize = 1.0f);
    bool              CreateBoundingBox (GRPELEMENT* element,           float DebugSize = 1.0f);
    bool              CreateLightLocator(GRPLIGHT* light,               float DebugSize = 1.0f);
    bool              CreateLocator     (GRPNODE* element,              float DebugSize = 1.0f);
    GRPELEMENT*       ConstructDummy    (XVECTOR<GRPPOINT*>* Points,    float DebugSize = 1.0f);

    bool              SetValue          (XDWORD type, double value);
    bool              SetValue          (XDWORD type, XDWORD value);

protected:

    XDWORD            DebugFlags;
    float             DebugSize;

private:

    void Clean()
    {
        DebugFlags = GRPVISUALDEBUGGER_FLAG_BBS | GRPVISUALDEBUGGER_FLAG_CAMERAS | GRPVISUALDEBUGGER_FLAG_LIGHTS | GRPVISUALDEBUGGER_FLAG_LOCATORS | GRPVISUALDEBUGGER_FLAG_NORMALS;
        DebugSize = 1.0f;
    }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif


