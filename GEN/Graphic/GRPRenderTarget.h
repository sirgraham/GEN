/*------------------------------------------------------------------------------------------
//  GRPRENDERTARGET.H
*/
/**
// \class
//
//  Creates a Render Target Buffer (Render to texture and offscreen rendering)
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 25/06/2014 17:52:52
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPRENDERTARGET_H_
#define _GRPRENDERTARGET_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPOpenGL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPVIEWPORT;

/*---- CLASS -----------------------------------------------------------------------------*/
class GRPTEXTURE;
class GRPSHADER;


enum GRPRENDERTARGETCOMPONENT
{
  GRPRENDERTARGETCOMPONENT_COLOR = GL_COLOR_ATTACHMENT0,
  GRPRENDERTARGETCOMPONENT_DEPTH = GL_DEPTH_ATTACHMENT
};

class GRPRENDERTARGET
{
public:
                    GRPRENDERTARGET       (int width, int height);
 virtual            ~GRPRENDERTARGET      ();

    void            CaptureFrameData      ();
    void            InitFrameBuffers      (GRPTEXTURE* colorbuffer,GRPRENDERTARGETCOMPONENT component=GRPRENDERTARGETCOMPONENT_COLOR);
    void            SelectAsTarget        (XDWORD face=0);
    void            Deselect              ();
    void            SaveAsRaw             (XCHAR* Name);
    bool            CheckFramebuffer      ();


    int             GetCaptureWidth       ()                        { return CaptureWidth;                }
    int             GetCaptureHeight      ()                        { return CaptureHeight;               }
    int             GetCaptureSize        ()                        { return Size;                        }
    int             GetCaptureChannels    ()                        { return CaptureChannels;             }
    XBYTE*          GetFrameBufferData    ()                        { return FrameBufferData;             }
    void            SetFrameBufferData    (XBYTE*       p)          { this->FrameBufferData   =p;         }
    void            SetViewPort           (GRPVIEWPORT* viewport)   { this->viewport          =viewport;  }
    void            SetShader             (GRPSHADER*   shader)     { this->shader            =shader;    }

protected:

    int             CaptureWidth;
    int             CaptureHeight;
    int             CaptureChannels;
    int             Size;

    GLUINT          frameBufferTargetID;
    GLUINT          DepthBufferTargetID;

    GRPTEXTURE*     colorbuffer;
    XBYTE*          FrameBufferData;
    GRPVIEWPORT*    viewport;
    GRPSHADER*      shader;

    GRPRENDERTARGETCOMPONENT component;

private:

  void Clean()
  {
    colorbuffer     =NULL;
    FrameBufferData =NULL;
    viewport        =NULL;
    shader          =NULL;

    CaptureChannels =4;
    CaptureWidth    =0;
    CaptureHeight   =0;
    Size            =0;
  }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

