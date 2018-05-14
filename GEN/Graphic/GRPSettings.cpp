/*------------------------------------------------------------------------------------------
// GRPSETTINGS.CPP
*/
/**
// \class
//
// default settings
//
//  @author: Diego Martinez Ruiz de Gaona
//
//  Date of Creation : 06/03/2018 9:59:23
//  Last Modification :
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPSettings.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

#ifdef HW_PC
GRPSETTINGS::QUALITY GRPSETTINGS::ModelDetail               = GRPSETTINGS::HIGHEST;
GRPSETTINGS::QUALITY GRPSETTINGS::TextureDetail             = GRPSETTINGS::HIGHEST;
GRPSETTINGS::QUALITY GRPSETTINGS::RenderDetail              = GRPSETTINGS::HIGHEST;
GRPSETTINGS::QUALITY GRPSETTINGS::LightningDetail           = GRPSETTINGS::HIGHEST;
GRPSETTINGS::QUALITY GRPSETTINGS::ShadowDetail              = GRPSETTINGS::HIGHEST;
GRPSETTINGS::QUALITY GRPSETTINGS::FogDetail                 = GRPSETTINGS::HIGHEST;
GRPSETTINGS::ANTIALIASING GRPSETTINGS::AntialiasingLevel    = GRPSETTINGS::Ax4;
GRPSETTINGS::QUALITY GRPSETTINGS::PhysicLevel               = GRPSETTINGS::MEDIUM;
GRPSETTINGS::QUALITY GRPSETTINGS::MotionBlur                = LOWEST;
GRPSETTINGS::QUALITY GRPSETTINGS::AmbientOcclusion          = LOWEST;
XDWORD GRPSETTINGS::WindowWidth                             = 1920;
XDWORD GRPSETTINGS::WindowHeight                            = 1080;
bool GRPSETTINGS::Windowed                                  = false;
float GRPSETTINGS::RenderingDistance                        = 10000.0f;
bool GRPSETTINGS::IsLightingEnabled                         = true;
#endif

#ifdef HW_RASPBERRYPI
GRPSETTINGS::QUALITY GRPSETTINGS::ModelDetail = GRPSETTINGS::LOWEST;
GRPSETTINGS::QUALITY GRPSETTINGS::TextureDetail = GRPSETTINGS::LOWEST;
GRPSETTINGS::QUALITY GRPSETTINGS::RenderDetail = GRPSETTINGS::LOWEST;
GRPSETTINGS::QUALITY GRPSETTINGS::LightningDetail = GRPSETTINGS::LOWEST;
GRPSETTINGS::QUALITY GRPSETTINGS::ShadowDetail = GRPSETTINGS::LOWEST;
GRPSETTINGS::QUALITY GRPSETTINGS::FogDetail = GRPSETTINGS::LOWEST;
GRPSETTINGS::ANTIALIASING GRPSETTINGS::AntialiasingLevel = GRPSETTINGS::Ax4;
GRPSETTINGS::QUALITY GRPSETTINGS::PhysicLevel = GRPSETTINGS::LOWEST;
GRPSETTINGS::QUALITY GRPSETTINGS::MotionBlur = LOWEST;
GRPSETTINGS::QUALITY GRPSETTINGS::AmbientOcclusion = LOWEST;
XDWORD GRPSETTINGS::WindowWidth = 1920;
XDWORD GRPSETTINGS::WindowHeight = 1080;
bool GRPSETTINGS::Windowed = false;
float GRPSETTINGS::RenderingDistance = 10000.0f;
bool GRPSETTINGS::IsLightingEnabled = true;
#endif


#ifdef ANDROID
GRPSETTINGS::QUALITY GRPSETTINGS::ModelDetail = GRPSETTINGS::HIGHEST;
GRPSETTINGS::QUALITY GRPSETTINGS::TextureDetail = GRPSETTINGS::HIGHEST;
GRPSETTINGS::QUALITY GRPSETTINGS::RenderDetail = GRPSETTINGS::HIGHEST;
GRPSETTINGS::QUALITY GRPSETTINGS::LightningDetail = GRPSETTINGS::HIGHEST;
GRPSETTINGS::QUALITY GRPSETTINGS::ShadowDetail = GRPSETTINGS::HIGHEST;
GRPSETTINGS::QUALITY GRPSETTINGS::FogDetail = GRPSETTINGS::HIGHEST;
GRPSETTINGS::ANTIALIASING GRPSETTINGS::AntialiasingLevel = GRPSETTINGS::Ax4;
GRPSETTINGS::QUALITY GRPSETTINGS::PhysicLevel = GRPSETTINGS::MEDIUM;
GRPSETTINGS::QUALITY GRPSETTINGS::MotionBlur = LOWEST;
GRPSETTINGS::QUALITY GRPSETTINGS::AmbientOcclusion = LOWEST;
XDWORD GRPSETTINGS::WindowWidth = 1920;
XDWORD GRPSETTINGS::WindowHeight = 1080;
bool GRPSETTINGS::Windowed = false;
float GRPSETTINGS::RenderingDistance = 10000.0f;
bool GRPSETTINGS::IsLightingEnabled = true;
#endif