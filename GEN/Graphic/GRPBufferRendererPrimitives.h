/*------------------------------------------------------------------------------------------
//  GRPRENDERPRIMITIVES.H
*/
/**
// \class
//
//  Graphics Renderer Base Class
//
//  @author  Diego Martinez Ruiz de Gaona
//  @version 04/08/2013 12:25:26
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPRENDERPRIMITIVES_H_
#define _GRPRENDERPRIMITIVES_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"

#include "GRPRect.h"
#include "GRPBufferRendererBase.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

template<class RENDERERBASE>
class GRPRENDERPRIMITIVES
{
  public:

    class RENDERERBASETYPE;

    //typedef RENDERERBASE                                RENDERERBASETYPE;
    //typedef typename RENDERERBASETYPE::COLORTYPE        COLORTYPE;
    //
    //
    //                                                    GRPRENDERPRIMITIVES               (RENDERERBASETYPE& renderer)
    //                                                    {
    //                                                      Clean();
    //
    //                                                      this->renderer = &renderer;
    //                                                    }
    //
    //
    //virtual                                            ~GRPRENDERPRIMITIVES               ()
    //                                                    {
    //                                                      Clean();
    //                                                    }
    //
    //
    //const RENDERERBASETYPE&                             GetRenderer                         () const
    //                                                    {
    //                                                      return (*renderer);
    //                                                    }
    //
    //RENDERERBASETYPE&                                   GetRenderer                         ()
    //                                                    {
    //                                                      return (*renderer);
    //                                                    }
    //
    //
    //const GRPBUFFER&                                    GetPixelFormatBuffer                () const
    //                                                    {
    //                                                      return renderer.GetPixelFormatBuffer();
    //                                                    }
    //
    //
    //GRPBUFFER&                                          GetPixelFormatBuffer                ()
    //                                                    {
    //                                                      return renderer.GetPixelFormatBuffer();
    //                                                    }
    //
    //
    //const COLORTYPE&                                    GetFillColor() const
    //                                                    {
    //                                                      return fillcolor;
    //                                                    }
    //
    //
    //const COLORTYPE&                                    GetLineColor() const
    //                                                    {
    //                                                      return linecolor;
    //                                                    }
    //
    //
    //void                                                SetFillColor                        (const COLORTYPE& c)
    //                                                    {
    //                                                      fillcolor = c;
    //                                                    }
    //
    //
    //void                                                SetLineColor                        (const COLORTYPE& c)
    //                                                    {
    //                                                      linecolor = c;
    //                                                    }
    //
    //void                                                Rectangle                           (int x1, int y1, int x2, int y2)
    //                                                    {
    //                                                      renderer->PutBlendHLine(x1,   y1,   x2-1  , linecolor , COVERTYPE_FULL);
    //                                                      renderer->PutBlendVLine(x2,   y1,   y2-1  , linecolor , COVERTYPE_FULL);
    //                                                      renderer->PutBlendHLine(x1+1, y2,   x2    , linecolor , COVERTYPE_FULL);
    //                                                      renderer->PutBlendVLine(x1,   y1+1, y2    , linecolor , COVERTYPE_FULL);
    //                                                    }
    //
    //void                                                RectangleSolid                      (int x1, int y1, int x2, int y2)
    //                                                    {
    //                                                      renderer->PutBlendBar(x1, y1, x2, y2, fillcolor, COVERTYPE_FULL);
    //                                                    }
    //
    //
    //void                                                RectangleOutlined                   (int x1, int y1, int x2, int y2)
    //                                                    {
    //                                                      Rectangle(x1, y1, x2, y2);
    //                                                      renderer->PutBlendBar(x1+1, y1+1, x2-1, y2-1, fillcolor, COVERTYPE_FULL);
    //                                                    }
    //

  private:


    //void                                                Clean                               ()
    //                                                    {
    //                                                      renderer = NULL;
    //                                                      currentx = 0;
    //                                                      currenty = 0;
    //                                                    }
    //
    //
    //RENDERERBASETYPE*                                   renderer;
    //COLORTYPE                                           linecolor;
    //COLORTYPE                                           fillcolor;
    //int                                                 currentx;
    //int                                                 currenty;

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

