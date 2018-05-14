/*------------------------------------------------------------------------------------------
//  GRPSHADERLIBRARY.H
*/
/**
// \class
//
//  Library for Vertex/Fragment Shaders
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 01/10/2014 9:54:49
//  Last Modification :
*/
//* GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPSHADERLIBRARY_H_
#define _GRPSHADERLIBRARY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"
#include "GRPOpenGL.h"
#include "GRPShader.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum
  {
    GRPSHADERLIBRARY_ACRONYM=0,
    GRPSHADERLIBRARY_NAME,
    GRPSHADERLIBRARY_VERTEXSHADER,
    GRPSHADERLIBRARY_FRAGMENTSHADER,
    GRPSHADERLIBRARY_DESCRIPTION,
    GRPSHADERLIBRARY_MAX
  };

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPSHADERLIBRARY
{
public:

                                      ~GRPSHADERLIBRARY   ();
  static  GRPSHADERLIBRARY*           Get                 ();
  static  void                        Destroy             ();


          bool                        LoadShaders         ();
          bool                        LoadShader          (int index, char name[], char   acronym[],  char vertexshader[],  char fragmentshader[],  char description[]);
          XVECTOR<GRPSHADERPROGRAM*>* GetCompiledPrograms ();
          GRPSHADERPROGRAM*           GetProgramByName    (XCHAR*);
          GRPSHADERPROGRAM*           GetProgramByKey     (char   key[3]);
          GRPSHADERPROGRAM*           GetProgram          (XDWORD i);

  static  GRPSHADERLIBRARY*           Instance;

                                      GRPSHADERLIBRARY    ();
  static  const char*                 ProgramList         [][GRPSHADERLIBRARY_MAX];


   // ------------------------ MEMBERS ----------------------

  XVECTOR<GRPSHADERPROGRAM*> CompiledPrograms;
  static const char VertexSimple                        [];
  static const char Vertex                              [];
  static const char VertexShaderDOT                     [];
  static const char VertexShaderDOTDiff                 [];

  static const char VertexTextureOperation              [];

  static const char FragmentShaderDOTTexture            [];
  static const char FragmentShaderDOTTextureSpecular    [];
  static const char FragmentShaderDOTTextureInterpolated[];
  static const char LoadingRing                         [];
  static const char LoadingBar                          [];
  static const char FragmentShaderDOTDiffuse            [];
  static const char TextureShader                       [];
  static const char DiffuseShader                       [];

  static const char VertexShaderDOTPerFragment          [];
  static const char FragmentShaderDOTPerFragment        [];

  static const char VertexShaderNormalMapping           [];
  static const char FragmentShaderNormalMapping         [];

  static const char DiffuseFragmentShaderDOTPerFragment [];
  static const char LightSpaceVertex                    [];
  static const char LightSpaceFragment                  [];
  static const char ShadowCastingVertex                 [];
  static const char ShadowCastingFragment               [];
  static const char ShadowCastingFragmentDiffuse        [];
  static const char CubeMapVertex                       [];
  static const char CubeMapFragment                     [];
  static const char Invert                              [];
  static const char ReflectiveVertex                    [];
  static const char ReflectiveFragmentDiffuse           [];
  static const char ReflectiveFragmentTexture           [];
  static const char SkinVertex                          [];
  static const char Morph                               [];
  static const char Deformer                            [];

  static const char NullShader                          [];
  static const char Basic                               [];
  static const char BasicBlack                          [];
  static const char BasicRed                            [];
  static const char BasicBlue                           [];
  static const char BasicGreen                          [];
  static const char BasicYellow                         [];
  static const char BasicPink                           [];
  static const char BasicGray                           [];

  static const char Opacity                             [];
  static const char InverseMask                         [];
  static const char Mask                                [];
  static const char SkyboxVertex                        [];
  static const char SkyboxFragment                      [];
  static const char SimpleTexture                       [];

  static const char VideoRender                         [];
  static const char PostProcessing                      [];
  static const char Normal                              [];
  static const char Blur                                [];
  static const char Fire                                [];
  static const char Hdr                                 [];
  static const char Gradient                            [];


  void Clean()
  {

  }

};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif



