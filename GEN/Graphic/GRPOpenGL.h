/*------------------------------------------------------------------------------------------
//  GRPOPENGL.H
*/
/**
// \class
//
//  Graphics Open GL headers (Multiplatform)
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 14/04/2014 11:48:55
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPOPENGL_H_
#define _GRPOPENGL_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"
#include "XString.h"
#include "XDebugTrace.h"
#include "GRPRect.h"



#ifdef HW_PC
  #define GRPOPENGL
  #define GRPSHADERVERSION 120
#endif

#if defined(HW_RASPBERRYPI) || defined(HW_ODROIDC1)
#define GRPOPENGLES
#define GRPSHADERVERSION 100
#endif

#if defined(ANDROID)
#define GRPOPENGLES
#define GRPSHADERVERSION 100
#endif

//--------------------------------------
// Windows
#ifdef WINDOWS
  #ifdef GRPOPENGL
    #define GL_GLEXT_PROTOTYPES
    #include "GL/glew.h"
    #include "GL/wglew.h"
    #include "GL/glext.h"
  #endif
#endif


//---------------------------------------
// Linux
#ifdef LINUX

  #ifdef HW_RASPBERRYPI
    #include <bcm_host.h>
  #endif


  #ifdef GRPOPENGL
    #define GL_GLEXT_PROTOTYPES
    #include "GL/glew.h"
    #include "GL/glext.h"
    #include <X11/X.h>
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/Xatom.h>
    #include <X11/extensions/xf86vmode.h>
    #include <GL/glx.h>
  #endif


#ifdef GRPOPENGLES
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    //#include <GLES/gl.h>
    //#include <GLES/glext.h>
    #include <EGL/egl.h>
    #include <EGL/eglext.h>
  #endif

  #if defined(GRPOPENGLES) && defined(HW_ODROIDC1)
    #include <X11/X.h>
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/Xatom.h>
    #include <X11/extensions/xf86vmode.h>
  #endif

#endif



//---------------------------------------
// Android
#ifdef ANDROID
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
  #include <GLES/gl.h>
  #include <GLES/glext.h>
  #include <EGL/egl.h>
  #include <EGL/eglext.h>
  #include <EGL/eglplatform.h>
#endif


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/



//--------------------------------------
// Windows
#ifdef WINDOWS
  #define GRPGLORTHO                      glOrtho
  #define GRPGLFRUSTUM                    glFrustum
  #define GRPGLCLEARDEPTH(x)              glClearDepth((float)x);
  #define LOAD_OPENGL_EXTENSIONS
#endif



//---------------------------------------
// Linux
#ifdef LINUX

  #ifdef GRPOPENGL
    #define GRPGLORTHO                    glOrtho
    #define GRPGLFRUSTUM                  glFrustum
    #define GRPGLCLEARDEPTH(x)            glClearDepth((int)(65535*((float)x)));
    #define GL_GLEXT_PROTOTYPES

    #define LOAD_OPENGL_EXTENSIONS
  #endif

  #ifdef GRPOPENGLES
    #define GL_VERTEX_PROGRAM_POINT_SIZE  0x8642
    #define GL_POINT_SPRITE               0x8861
    #define GRPGLORTHO                    glOrthof
    #define GRPGLFRUSTUM                  glFrustumf
    #define GRPGLCLEARDEPTH(x)            glClearDepthf((float)x);
  #endif

#endif



//---------------------------------------
// Android
#ifdef ANDROID

  #define GRPGLORTHO                      glOrthof
  #define GRPGLFRUSTUM                    glFrustumf
  #define GRPGLCLEARDEPTH(x)              glClearDepthf((float)x);

// need to take these out, ES 2 doesn't actually support them
  #define GL_VERTEX_PROGRAM_POINT_SIZE    0x8642
  #define GL_POINT_SPRITE                 0x8861
#endif


#define GRPTEXTUREHANDLE  GLuint

typedef GLuint            GLUINT;
typedef GLfloat           GLFLOAT;
typedef GLboolean         GLBOOLEAN;
typedef GLint             GLINT;

#ifdef WINDOWS
#ifdef XDEBUG
#define CHECK_OPENGL_ERRORS
#endif
#endif

#ifdef CHECK_OPENGL_ERRORS
#define OPENGLCHECKERROR(x) GRPOPENGLCTRL::CheckError(x,__FILE__,__FUNCTION__,__LINE__);
#endif


#ifndef OPENGLCHECKERROR
#define OPENGLCHECKERROR(x) //
#endif

#ifdef LOAD_OPENGL_EXTENSIONS
#ifndef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
    #define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS          GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
#endif
#endif

// because GLES 2 doesn't have anything other than depth16
// https://www.khronos.org/registry/gles/extensions/OES/OES_depth24.txt
// https://www.khronos.org/registry/gles/extensions/OES/OES_depth32.txt

#ifdef GRPOPENGLES
    #define TEXTURE_BORDER_COLOR_EXT                          0x1004
    #define GL_TEXTURE_BORDER_COLOR                           TEXTURE_BORDER_COLOR_EXT

    #define CLAMP_TO_BORDER_EXT                               0x812D
    #define GL_CLAMP_TO_BORDER                                CLAMP_TO_BORDER_EXT
#endif

#ifndef glDiscardFramebufferEXT
    #define glDiscardFramebufferEXT(x, y, z)
#endif

#ifndef GL_DEPTH_COMPONENT24_OES
    #define GL_DEPTH_COMPONENT24_OES 0x81A6
#endif

#ifndef GL_DEPTH_COMPONENT32_OES
    #define GL_DEPTH_COMPONENT32_OES 0x81A7
#endif

#ifndef GL_DEPTH_COMPONENT24
    #define GL_DEPTH_COMPONENT24 GL_DEPTH_COMPONENT24_OES
#endif

#ifndef GL_DEPTH_COMPONENT32
    #define GL_DEPTH_COMPONENT32 GL_DEPTH_COMPONENT32_OES
#endif

/*---- CLASS -----------------------------------------------------------------------------*/

enum GRPBLENDINGEQUATION
{
  GRPBLENDINGEQUATION_NONE=0,
  GRPBLENDINGEQUATION_ADD =GL_FUNC_ADD,
  GRPBLENDINGEQUATION_SUB =GL_FUNC_SUBTRACT
};


enum GRPBLENDINGMODE
{
  GRPBLENDINGMODE_UNKNOWN=0,
  GRPBLENDINGMODE_NONE,
  GRPBLENDINGMODE_ADDITIVE,
  GRPBLENDINGMODE_MULTIPLICATIVE,
  GRPBLENDINGMODE_SUBSTRACTIVE,
  GRPBLENDINGMODE_ALPHABLEND
};

enum GRPBLENDINGFUNCTION
{
  GRPBLENDINGFUNCTION_UNKNOWN                   =0,
  GRPBLENDINGFUNCTION_ZERO                      =GL_ZERO,
  GRPBLENDINGFUNCTION_ONE                       =GL_ONE,
  GRPBLENDINGFUNCTION_SRC_COLOR                 =GL_SRC_COLOR,
  GRPBLENDINGFUNCTION_SRC_ONE_MINUS_SRC_COLOR   =GL_ONE_MINUS_SRC_COLOR,
  GRPBLENDINGFUNCTION_DST_COLOR                 =GL_DST_COLOR,
  GRPBLENDINGFUNCTION_ONE_MINUS_DST_COLOR       =GL_ONE_MINUS_DST_COLOR,
  GRPBLENDINGFUNCTION_SRC_ALPHA                 =GL_SRC_ALPHA,
  GRPBLENDINGFUNCTION_ONE_MINUS_SRC_ALPHA       =GL_ONE_MINUS_SRC_ALPHA,
  GRPBLENDINGFUNCTION_DST_ALPHA                 =GL_DST_ALPHA,
  GRPBLENDINGFUNCTION_ONE_MINUS_DST_ALPHA       =GL_ONE_MINUS_DST_ALPHA,
  GRPBLENDINGFUNCTION_CONSTANT_COLOR            =GL_CONSTANT_COLOR,
  GRPBLENDINGFUNCTION_ONE_MINUS_CONSTANT_COLOR  =GL_ONE_MINUS_CONSTANT_COLOR,
  GRPBLENDINGFUNCTION_CONSTANT_ALPHA            =GL_CONSTANT_ALPHA,
  GRPBLENDINGFUNCTION_ONE_MINUS_CONSTANT_ALPHA  =GL_ONE_MINUS_CONSTANT_ALPHA,
  GRPBLENDINGFUNCTION_SRC_ALPHA_SATURATE        =GL_SRC_ALPHA_SATURATE
};

enum GRPCULLMODE
{
  GRPCULLMODE_NONE=0,
  GRPCULLMODE_BACK,
  GRPCULLMODE_FRONT
};

enum GRPTEXTUREMODE
{
  GRPTEXTUREMODE_2D             = GL_TEXTURE_2D,
  GRPTEXTUREMODE_CUBE           = GL_TEXTURE_CUBE_MAP
};

enum GRPDEPTHFUNC
{
  GRPDEPTHFUNC_NEVER            = GL_NEVER,
  GRPDEPTHFUNC_LESS             = GL_LESS,
  GRPDEPTHFUNC_EQUAL            = GL_EQUAL,
  GRPDEPTHFUNC_LESS_OR_EQUAL    = GL_LEQUAL,
  GRPDEPTHFUNC_GREATER          = GL_GREATER,
  GRPDEPTHFUNC_NOT_EQUAL        = GL_NOTEQUAL,
  GRPDEPTHFUNC_GREATER_OR_EQUAL = GL_GEQUAL,
  GRPDEPTHFUNC_ALWAYS           = GL_ALWAYS
};

enum GRPPASS_CLEAR_FLAG
{
  GRPPASS_CLEAR_FLAG_NONE=0,
  GRPPASS_CLEAR_FLAG_COLOR                  = GL_COLOR_BUFFER_BIT,
  GRPPASS_CLEAR_FLAG_DEPTH                  = GL_DEPTH_BUFFER_BIT,
  GRPPASS_CLEAR_FLAG_STENCIL                = GL_STENCIL_BUFFER_BIT,
//GRPPASS_CLEAR_FLAG_ACCUMULATOR            = GL_ACCUM_BUFFER_BIT,
  GRPPASS_CLEAR_FLAG_BOTH                   = GRPPASS_CLEAR_FLAG_COLOR | GRPPASS_CLEAR_FLAG_DEPTH,
  GRPPASS_CLEAR_FLAG_ALL                    = GRPPASS_CLEAR_FLAG_COLOR | GRPPASS_CLEAR_FLAG_DEPTH | GRPPASS_CLEAR_FLAG_STENCIL
};

enum GRPSTENCILOPERATION
{
  GRPSTENCILOPERATION_KEEP                  = GL_KEEP,        //Keeps the current value.
  GRPSTENCILOPERATION_ZERO                  = GL_ZERO,        //Sets the stencil buffer value to 0.
  GRPSTENCILOPERATION_REPLACE               = GL_REPLACE,   //Sets the stencil buffer value to ref, as specified by glStencilFunc.
  GRPSTENCILOPERATION_INCREASE              = GL_INCR,        //Increments the current stencil buffer value. Clamps to the maximum representable unsigned value.
  GRPSTENCILOPERATION_INCREASE_WITH_WRAP    = GL_INCR_WRAP, //Increments the current stencil buffer value. Wraps stencil buffer value to zero when incrementing the maximum representable unsigned value.
  GRPSTENCILOPERATION_DECREASE              = GL_DECR,        //Decrements the current stencil buffer value. Clamps to 0.
  GRPSTENCILOPERATION_DECREASE_WITH_WRAP    = GL_DECR_WRAP, //Decrements the current stencil buffer value. Wraps stencil buffer value to the maximum representable unsigned value when decrementing a stencil buffer value of zero.
  GRPSTENCILOPERATION_INVERT                = GL_INVERT     //Bitwise inverts the current stencil buffer value
};

enum GRPSTENCILFUNCTION
{
  GRPSTENCILFUNCTION_NEVER                  = GL_NEVER,     //Always fails.
  GRPSTENCILFUNCTION_LESS                   = GL_LESS,      //Passes if ( ref & mask ) < ( stencil & mask ).
  GRPSTENCILFUNCTION_LESS_OR_EQUAL          = GL_LEQUAL,    //Passes if ( ref & mask ) <= ( stencil & mask ).
  GRPSTENCILFUNCTION_GREATER                = GL_GREATER,   //Passes if ( ref & mask ) > ( stencil & mask ).
  GRPSTENCILFUNCTION_GREATER_OR_EQUAL       = GL_GEQUAL,    //Passes if ( ref & mask ) >= ( stencil & mask ).
  GRPSTENCILFUNCTION_EQUAL                  = GL_EQUAL,     //Passes if ( ref & mask ) = ( stencil & mask ).
  GRPSTENCILFUNCTION_NOT_EQUAL              = GL_NOTEQUAL,  //Passes if ( ref & mask ) != ( stencil & mask ).
  GRPSTENCILFUNCTION_ALWAYS                 = GL_ALWAYS     //Always passes.
};


enum GRPSTENCILBUFFER
{
  GRPSTENCILBUFFER_NONE                     =GL_NONE,
  GRPSTENCILBUFFER_BOTH                     =GL_FRONT_AND_BACK,
  GRPSTENCILBUFFER_FRONT                    =GL_FRONT,
  GRPSTENCILBUFFER_BACK                     =GL_BACK
};

enum GRPSHADERPROGRAMTYPE
  {
  GRPSHADERPROGRAMTYPE_NONE                   =0,
  GRPSHADERPROGRAMTYPE_VERTEX                 =GL_VERTEX_SHADER,
  GRPSHADERPROGRAMTYPE_FRAGMENT               =GL_FRAGMENT_SHADER,

#ifdef GL_COMPUTE_SHADER
  GRPSHADERPROGRAMTYPE_COMPUTE                =GL_COMPUTE_SHADER,
  GRPSHADERPROGRAMTYPE_TESSELATION_CONTROL    =GL_TESS_CONTROL_SHADER,
  GRPSHADERPROGRAMTYPE_TESSELATION_EVALUATION =GL_TESS_EVALUATION_SHADER,
  GRPSHADERPROGRAMTYPE_GEOMETRY               =GL_GEOMETRY_SHADER,
#endif

  GRPSHADERPROGRAMTYPE_MAX
  };

enum GRPSHADERPROGRAMSLOT
  {
  GRPSHADERPROGRAMSLOT_VERTEX=0,
  GRPSHADERPROGRAMSLOT_FRAGMENT,
  GRPSHADERPROGRAMSLOT_COMPUTE,
  GRPSHADERPROGRAMSLOT_TESSELATION_CONTROL,
  GRPSHADERPROGRAMSLOT_TESSELATION_EVALUATION,
  GRPSHADERPROGRAMSLOT_GEOMETRY
  };


class GRPOPENGLCTRL
{
  public:

            void                  Ini                   ();

    static  bool                  ClearErrors           ();
    static  bool                  Log                   (char*  text);
    static  bool                  CheckError            (XCHAR* text,   const char* file,   const char* function,   int line);

    //-------------------------------- color ----------------------------
    class COLOR
    {
    protected:

      static  bool                  ColorWritingIsActive;

      static  bool                  RedMask;
      static  bool                  BlueMask;
      static  bool                  GreenMask;
      static  bool                  AlphaMask;

      static  GLFLOAT               ClearColorRed;
      static  GLFLOAT               ClearColorBlue;
      static  GLFLOAT               ClearColorGreen;
      static  GLFLOAT               ClearColorAlpha;

    public:

      static  void                  Clear                 (XDWORD   hasToClear);
      static  void                  SetClearColor         (GLFLOAT  r,GLFLOAT g,GLFLOAT b,GLFLOAT a);
      static  void                  SetColorWriting       (bool     b, bool RedMask=true, bool GreenMask=true, bool BlueMask=true, bool AlphaMask=true);
    };

    //-------------------------------- dither ----------------------------
    class DITHER
    {
    protected:

    static  bool                    DitherIsActive;

    public:

    static  void                    SetDither             (bool       b);
    };

    //-------------------------------- texture ----------------------------
    class TEXTURE
    {
    public:

        struct UNIT
        {
            GLUINT handle;
            GLUINT active;
            GLUINT type;
        };

      static TEXTURE::UNIT units[32];

      //static  GLUINT                CurrentActiveTexture;
      static  void                  SetTextureMode        (bool b, GRPTEXTUREMODE m = GRPTEXTUREMODE_2D);
      static  bool                  TextureIsActive;

      static  void*                 CurrentTexture;
      static  GLUINT                CurrentActiveTexture;

      static  void                  SetCurrentTexture     (GLUINT);
      static  void                  BindTexture           (GLUINT target, GLUINT mode, GLUINT handle);

    };

    //-------------------------------- depth ----------------------------
    class DEPTH
    {
    protected:

      static  bool                  TestIsActive;
      static  bool                  WritingIsActive;
      static  GRPDEPTHFUNC          Function;
      static  float                 ClearDepth;

    public:

      static  void                  SetDepthTest          (bool b);
      static  void                  SetDepthWriting       (bool b);
      static  void                  SetDepthFunc          (GRPDEPTHFUNC         type);
      static  void                  SetClearDepth         (float                dep);
    };

    //-------------------------------- viewport ----------------------------
    class VIEWPORT
    {
    public:

      static  GLFLOAT             x;
      static  GLFLOAT             y;
      static  GLFLOAT             width;
      static  GLFLOAT             height;
      static  GLFLOAT             pixelwidth;
      static  GLFLOAT             pixelheight;
    };

    //-------------------------------- culling ----------------------------
    class CULLING
    {
    protected:

      static  bool                IsActive;
      static  GRPCULLMODE         Mode;

    public:

      static  void                SetBackFaceCulling    (bool                 b,      GRPCULLMODE mode=GRPCULLMODE_BACK);
    };

    //-------------------------------- scissor ----------------------------
    class SCISSOR
    {
    public:

      static  bool                IsActive;
      static  GRPRECTINT          Rect;

      static  void                SetScissor            (bool                 onoff,  int xo=0, int yo=0, int xd=0, int yd=0);
    };

    //-------------------------------- blending ----------------------------
    class BLENDING
    {
    protected:

      static  bool                IsActive;
      static  GRPBLENDINGFUNCTION FunctionSrc;
      static  GRPBLENDINGFUNCTION FunctionDst;
      static  GRPBLENDINGEQUATION Equation;

      static  GRPBLENDINGMODE     Mode;

    public:

      static  void                SetAlphaBlending      (bool                 alpha,  GRPBLENDINGMODE mode=GRPBLENDINGMODE_ALPHABLEND);
      static  void                SetBlendingMode       (GRPBLENDINGMODE      src);
      static  void                SetBlendingFunction   (GRPBLENDINGFUNCTION  src,    GRPBLENDINGFUNCTION dst);
      static  void                SetBlendingEquation   (GRPBLENDINGEQUATION  eq);
    };

    //-------------------------------- stencil ----------------------------
    class STENCIL
    {
    protected:

      static bool                 IsActive;
      static GRPSTENCILOPERATION  StencilFailOperation;
      static GRPSTENCILOPERATION  DepthFailOperation;
      static GRPSTENCILOPERATION  BothPassOperation;
      static GRPSTENCILFUNCTION   Function;
      static XDWORD               Mask;
      static XDWORD               Value;
      static GRPSTENCILBUFFER     Buffers;

    public:

      static  void                SetStencil            (bool                 onoff);
      static  void                SetStencilOperation   (GRPSTENCILBUFFER     buffer,     GRPSTENCILOPERATION  stfail,      GRPSTENCILOPERATION depthfail, GRPSTENCILOPERATION allpass);
      static  void                SetStencilFunction    (GRPSTENCILFUNCTION   function,   XDWORD value=0);
      static  void                SetStencilMask        (GLUINT               mask);
    };

    //-------------------------------- VBO ----------------------------
    class VBO
    {
    public:
      static  bool                channel_active[32];

      static  GLINT               MaxAttributes;
      static  GLUINT              CurrentVBO;
      static  GLUINT              CurrentVBOIndex;

      static void                 EnableVertexAttribArray (GLUINT             channel);
      static void                 DisableVertexAttribArray(GLUINT             channel);
      static void                 VertexAttributePointer  (GLUINT             channel,  GLUINT channelsize, GLUINT datatype, GLUINT isnormalized, GLUINT stride, void* offset);
    };
};




/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif




