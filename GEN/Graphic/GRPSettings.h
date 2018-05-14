/*------------------------------------------------------------------------------------------
//  GRPSETTINGS.H
*/
/**
// \class
//
//  Manages global graphical settings
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 09/09/2016 11:20:32
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPSETTINGS_H_
#define _GRPSETTINGS_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"
#include "GRPSubject.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/


class GRPSETTINGS
{
public:

  enum ANTIALIASING
  {
    DISABLED=0,
    Ax2 =2,
    Ax4 =4,
    Ax8 =8
  };

  enum QUALITY
  {
    NONE=0,
    LOWEST,
    LOW,
    MEDIUM,
    HIGH,
    HIGHEST
  };


static bool LoadDefault         ()
      {

#ifdef HW_PC

        ModelDetail           = HIGHEST;
        TextureDetail         = HIGHEST;
        RenderDetail          = HIGHEST;
        LightningDetail       = HIGHEST;
        ShadowDetail          = HIGHEST;
        FogDetail             = HIGHEST;
        AntialiasingLevel     = Ax4;
        PhysicLevel           = MEDIUM;
        MotionBlur            = LOWEST;
        AmbientOcclusion      = LOWEST;
        WindowWidth           = 1920;
        WindowHeight          = 1080;
        Windowed              = false;
        RenderingDistance     = 10000.0f;

#endif

#ifdef HW_RASPBERRYPI

        ModelDetail           = LOWEST;
        TextureDetail         = LOWEST;
        RenderDetail          = LOWEST;
        LightningDetail       = LOWEST;
        ShadowDetail          = LOWEST;
        FogDetail             = LOWEST;
        AntialiasingLevel     = Ax4;
        PhysicLevel           = LOWEST;
        MotionBlur            = LOWEST;
        AmbientOcclusion      = LOWEST;
        WindowWidth           = 1920;
        WindowHeight          = 1080;
        Windowed              = false;
        RenderingDistance     = 10000.0f;

#endif
        IsLightingEnabled     = true;
        return true;
      }

static      QUALITY                   ModelDetail;
static      QUALITY                   TextureDetail;
static      QUALITY                   RenderDetail;
static      QUALITY                   LightningDetail;
static      QUALITY                   ShadowDetail;
static      QUALITY                   FogDetail;
static      ANTIALIASING              AntialiasingLevel;
static      QUALITY                   PhysicLevel;
static      QUALITY                   MotionBlur;
static      QUALITY                   AmbientOcclusion;

static      XDWORD                    WindowWidth;
static      XDWORD                    WindowHeight;
static      bool                      Windowed;
static      float                     RenderingDistance;

//----------- realtime adjustmments

static      XDWORD                    currentVertexBudget;
static      bool                      IsLightingEnabled;

private:

  void Clean()
  {
    LoadDefault       ();
  }

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

