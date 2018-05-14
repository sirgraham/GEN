/*------------------------------------------------------------------------------------------
//  GRPBUFFERPIXELFORMATGRAY.H
*/
/**
// \class
//
//  Graphics Pixel Format Buffer Gray Class
//
//  @author  Diego Martinez Ruiz de Gaona
//  @version 01/08/2013 16:02:44
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPBUFFERPIXELFORMATGRAY_H_
#define _GRPBUFFERPIXELFORMATGRAY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPBase.h"
#include "GRPRect.h"
#include "GRPColorGray.h"
#include "GRPBuffer.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/



template<class COLORT>
class BLENDERGRAY
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT
    };



    static void                                       BlendPixel            (VALUETYPE* p, XDWORD cv, XDWORD alpha, XDWORD cover = 0)
                                                      {
                                                        *p = (VALUETYPE)((((cv - CALCTYPE(*p)) * alpha) + (CALCTYPE(*p) << BASESCALE_SHIFT)) >> BASESCALE_SHIFT);
                                                      }
};



template<class COLORT>
class BLENDERGRAYPRE
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT
    };


    static void                                       BlendPixel                      (VALUETYPE* p, XDWORD cv, XDWORD alpha, XDWORD cover = 0)
                                                      {
                                                        alpha = COLORTYPE::BASESCALE_MASK - alpha;
                                                        cover = (cover + 1) << (BASESCALE_SHIFT - 8);
                                                        *p = (VALUETYPE)((*p * alpha + cv * cover) >> BASESCALE_SHIFT);
                                                      }

    static void                                       BlendPixel                      (VALUETYPE* p, XDWORD cv, XDWORD alpha)
                                                      {
                                                        *p = (VALUETYPE)(((*p * (COLORTYPE::BASESCALE_MASK - alpha)) >> BASESCALE_SHIFT) + cv);
                                                      }
};



template<class COLORT, class GAMMALUT>
class APPLYGAMMADIRGRAY
{
  public:

    typedef typename COLORT::VALUETYPE                VALUETYPE;

                                                      APPLYGAMMADIRGRAY               (const GAMMALUT& gamma)
                                                      {
                                                        this->gamma = gamma;
                                                      }

    void                                              operator()                      (VALUETYPE* p)
                                                      {
                                                        *p = gamma.Dir(*p);
                                                      }

  private:

    const GAMMALUT&                                   gamma;
};



template<class COLORT, class GAMMALUT>
class APPLYGAMMAINVGRAY
{
  public:

    typedef typename COLORT::VALUETYPE                VALUETYPE;

                                                      APPLYGAMMAINVGRAY               (const GAMMALUT& gamma)
                                                      {
                                                        this->gamma = gamma;
                                                      }

    void                                              operator()                      (VALUETYPE* p)
                                                      {
                                                        *p = gamma.Inv(*p);
                                                      }

  private:

    const GAMMALUT&                                   gamma;
};



template<class BLENDER, class RENDERERBUFFER, XDWORD STEP = 1, XDWORD OFFSET = 0>
class GRPBUFFERPIXELFORMATGRAY
{
  public:

    typedef RENDERERBUFFER                            RENDERERBUFFERTYPE;
    typedef typename RENDERERBUFFERTYPE::ROWDATA      ROWDATA;
    typedef BLENDER                                   BLENDERTYPE;
    typedef typename BLENDERTYPE::COLORTYPE           COLORTYPE;
    typedef int                                       ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_SCALE = COLORTYPE::BASESCALE_SCALE  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK   ,

      PIXELWIDTH      = sizeof(VALUETYPE)           ,
      PIXELSTEP       = STEP                        ,
      PIXELOFFSET     = OFFSET
    };


  public:
                                                      GRPBUFFERPIXELFORMATGRAY        (RENDERERBUFFERTYPE& rendererbuffer)
                                                      {
                                                        this->rendererbuffer = &rendererbuffer;
                                                      }


    void                                              Set                             (RENDERERBUFFERTYPE& rendererbuffer)
                                                      {
                                                        this->rendererbuffer = &rendererbuffer;
                                                      }


    template<class BUFFERPIXELFORMAT>
    bool                                              Set                             (BUFFERPIXELFORMAT& pixelformatbuffer, int x1, int y1, int x2, int y2)
                                                      {
                                                        GRPRECTINT r(x1, y1, x2, y2);

                                                        if(r.clip(GRPRECTINT(0, 0, pixelformatbuffer.GetWidth()-1, pixelformatbuffer.GetHeight()-1)))
                                                          {
                                                            int stride = pixelformatbuffer.GetStride();

                                                            rendererbuffer->Set(pixelformatbuffer.GetPtr(r.x1, stride < 0 ? r.y2 : r.y1), (r.x2 - r.x1) + 1, (r.y2 - r.y1) + 1, stride);

                                                            return true;
                                                          }

                                                        return false;
                                                      }


    XDWORD                                            GetWidth                        () const                            { return rendererbuffer->GetWdth();         }
    XDWORD                                            GetHeight                       () const                            { return rendererbuffer->Height();          }
    int                                               GetStride                       () const                            { return rendererbuffer->GetStride();       }


    XBYTE*                                            Row                             (int y)                             { return rendererbuffer->Row(y);            }
    const XBYTE*                                      Row                             (int y) const                       { return rendererbuffer->Row(y);            }

    ROWDATA                                           RowData                         (int y) const                       { return rendererbuffer->Row(y);            }

    const XBYTE*                                      GetPtr                          (int x, int y) const                { return rendererbuffer->Row(y) + x * STEP + OFFSET;    }
    XBYTE*                                            GetPtr                          (int x, int y)                      { return rendererbuffer->Row(y) + x * STEP + OFFSET;    }


    COLORTYPE                                         GetPixel                        (int x, int y) const
                                                      {
                                                        VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(y) + x * STEP + OFFSET;
                                                        return COLORTYPE(*p);
                                                      }


     static void                                      PutPixel                        (XBYTE* p, const COLORTYPE& c)
                                                      {
                                                        *(VALUETYPE*)p = c.v;
                                                      }


     void                                             PutPixel                        (int x, int y, const COLORTYPE& c)
                                                      {
                                                        *((VALUETYPE*)rendererbuffer->Row(x, y, 1) + x * STEP + OFFSET) = c.v;
                                                      }


     void                                             PutBlendPixel                   (int x, int y, const COLORTYPE& c, XBYTE cover)
                                                      {
                                                        PutOrBlendPixel((VALUETYPE*) rendererbuffer->Row(x, y, 1) + x * STEP + OFFSET, c, cover);
                                                      }


     void                                             PutHLine                        (int x, int y, XDWORD len, const COLORTYPE& c)
                                                      {
                                                        VALUETYPE* p = (VALUETYPE*) rendererbuffer->Row(x, y, len) + x * STEP + OFFSET;

                                                        do{ *p  = c.v;
                                                             p += STEP;

                                                          } while(--len);
                                                      }


    void                                              PutHLine                        (int x, int y, XDWORD len, const COLORTYPE& c)
                                                      {
                                                        do{ VALUETYPE* p = (VALUETYPE*) rendererbuffer->Row(x, y++, 1) + x * STEP + OFFSET;

                                                            *p = c.v;

                                                          } while(--len);
                                                      }


    void                                              PutBlendHLine                   (int x, int y, XDWORD len, const COLORTYPE& c, XBYTE cover)
                                                      {
                                                        if(c.a)
                                                          {
                                                            VALUETYPE* p = (VALUETYPE*) rendererbuffer->Row(x, y, len) + x * STEP + OFFSET;

                                                              CALCTYPE alpha = (CALCTYPE(c.a) * (cover + 1)) >> 8;
                                                              if(alpha == BASESCALE_MASK)
                                                                {
                                                                  do{ *p  = c.v;
                                                                       p += STEP;

                                                                    } while(--len);
                                                                }
                                                               else
                                                                {
                                                                  do{ BLENDER::blend_pix(p, c.v, alpha, cover);
                                                                      p += STEP;

                                                                    } while(--len);
                                                                }
                                                          }
                                                      }


    void                                              PutBlendVLine                   (int x, int y,  XDWORD len, const COLORTYPE& c, XBYTE cover)
                                                      {
                                                        if(c.a)
                                                          {
                                                            VALUETYPE* p;
                                                            CALCTYPE   alpha = (CALCTYPE(c.a) * (cover + 1)) >> 8;

                                                            if(alpha == BASESCALE_MASK)
                                                              {
                                                                do{ p = (VALUETYPE*)
                                                                    rendererbuffer->Row(x, y++, 1) + x * STEP + OFFSET;

                                                                    *p = c.v;

                                                                  } while(--len);
                                                              }
                                                             else
                                                              {
                                                                do{ p = (VALUETYPE*)
                                                                    rendererbuffer->Row(x, y++, 1) + x * STEP + OFFSET;

                                                                    BLENDER::PutBlendPixel(p, c.v, alpha, cover);

                                                                  } while(--len);
                                                              }
                                                          }
                                                      }


    void                                              PutBlendSolidHSpan              (int x, int y, XDWORD len, const COLORTYPE& c, const XBYTE* covers)
                                                      {
                                                        if(c.a)
                                                          {
                                                            VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, len) + x * STEP + OFFSET;

                                                            do{ CALCTYPE alpha = (CALCTYPE(c.a) * (CALCTYPE(*covers) + 1)) >> 8;

                                                                if(alpha == BASESCALE_MASK)
                                                                  {
                                                                    *p = c.v;
                                                                  }
                                                                 else
                                                                  {
                                                                    BLENDER::blend_pix(p, c.v, alpha, *covers);
                                                                  }

                                                                p += STEP;
                                                                ++covers;

                                                              } while(--len);
                                                          }
                                                      }


    void                                              PutBlendSolidVSpan              (int x, int y, XDWORD len, const COLORTYPE& c, const XBYTE* covers)
                                                      {
                                                        if(c.a)
                                                          {
                                                            do{ CALCTYPE alpha = (CALCTYPE(c.a) * (CALCTYPE(*covers) + 1)) >> 8;

                                                                VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + x * STEP + OFFSET;

                                                                if(alpha == BASESCALE_MASK)
                                                                  {
                                                                    *p = c.v;
                                                                  }
                                                                 else
                                                                  {
                                                                    BLENDER::blend_pix(p, c.v, alpha, *covers);
                                                                  }

                                                                ++covers;

                                                              } while(--len);
                                                          }
                                                      }


    void                                              PutColorHSpan                   (int x, int y, XDWORD len, const COLORTYPE* colors)
                                                      {
                                                        VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, len) + x * STEP + OFFSET;

                                                        do{ *p  = colors->v;
                                                             p += STEP;
                                                             ++colors;

                                                          } while(--len);
                                                      }


    void                                              PutColorVSpan                   (int x, int y, XDWORD len, const COLORTYPE* colors)
                                                      {
                                                        do{ VALUETYPE* p = (VALUETYPE*)
                                                            rendererbuffer->Row(x, y++, 1) + x * STEP + OFFSET;
                                                            *p = colors->v;
                                                            ++colors;

                                                          } while(--len);
                                                      }


    void                                              PutBlendColorHSpan              (int x, int y, XDWORD len, const COLORTYPE* colors, const XBYTE* covers, XBYTE cover)
                                                      {
                                                        VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, len) + x * STEP + OFFSET;
                                                        if(covers)
                                                          {
                                                            do{ PutOrBlendPixel(p, *colors++, *covers++);
                                                                p += STEP;

                                                              } while(--len);
                                                          }
                                                         else
                                                          {
                                                            if(cover == 255)
                                                              {
                                                                do{ if(colors->a == BASESCALE_MASK)
                                                                      {
                                                                        *p = colors->v;
                                                                      }
                                                                      else
                                                                      {
                                                                        PutOrBlendPixel(p, *colors);
                                                                      }
                                                                      p += STEP;
                                                                      ++colors;

                                                                  } while(--len);
                                                              }
                                                             else
                                                              {
                                                                do{ PutOrBlendPixel(p, *colors++, cover);
                                                                    p += STEP;

                                                                  } while(--len);
                                                              }
                                                          }
                                                      }


    void                                              PutBlendColorSpan               (int x, int y,  XDWORD len, const COLORTYPE* colors, const XBYTE* covers, XBYTE cover)
                                                      {
                                                        VALUETYPE* p;
                                                        if(covers)
                                                          {
                                                            do{ p = (VALUETYPE*)
                                                                rendererbuffer->Row(x, y++, 1) + x * STEP + OFFSET;

                                                                PutOrBlendPixel(p, *colors++, *covers++);

                                                              } while(--len);
                                                          }
                                                         else
                                                          {
                                                            if(cover == 255)
                                                              {
                                                                do{ p = (VALUETYPE*)
                                                                    rendererbuffer->Row(x, y++, 1) + x * STEP + OFFSET;

                                                                    if(colors->a == BASESCALE_MASK)
                                                                      {
                                                                        *p = colors->v;
                                                                      }
                                                                     else
                                                                      {
                                                                        PutOrBlendPixel(p, *colors);
                                                                      }

                                                                      ++colors;

                                                                  } while(--len);
                                                              }
                                                             else
                                                              {
                                                                do{ p = (VALUETYPE*)
                                                                    rendererbuffer->Row(x, y++, 1) + x * STEP + OFFSET;

                                                                    PutOrBlendPixel(p, *colors++, cover);

                                                                  } while(--len);
                                                              }
                                                          }
                                                      }


    template<class FUNCTION>
    void                                              ForEachPpixel                   (FUNCTION f)
                                                      {
                                                        XDWORD y;
                                                        for(y = 0; y < GetHeight(); ++y)
                                                          {
                                                            ROWDATA r = rendererbuffer->row(y);
                                                            if(r.ptr)
                                                              {
                                                                XDWORD len = r.x2 - r.x1 + 1;

                                                                VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(r.x1, y, len) + r.x1 * STEP + OFFSET;
                                                                do{
                                                                    f(p);
                                                                    p += STEP;

                                                                  } while(--len);
                                                              }
                                                          }
                                                      }


    template<class GAMMALUT>
    void                                              ApplyGammaDir                   (const GAMMALUT& g)
                                                      {
                                                        ForEachPixel(APPLYGAMMADIRGRAY<COLORTYPE, GAMMALUT>(g));
                                                      }

    template<class GAMMALUT>
    void                                              ApplyGammaInv                   (const GAMMALUT& g)
                                                      {
                                                        ForEachPixel(APPLYGAMMAINVGRAY<COLORTYPE, GAMMALUT>(g));
                                                      }


    template<class RENDERERBUFFER2>
    void                                              CopyFrom                        (const RENDERERBUFFER2& from, int xdst, int ydst,int xsrc, int ysrc, XDWORD size)
                                                      {
                                                        const XBYTE* p = from.Row(ysrc);
                                                        if(p) memmove(rendererbuffer->Row(xdst, ydst, size) + xdst * PIXELWIDTH, p + xsrc * PIXELWIDTH, size * PIXELWIDTH);

                                                      }


    template<class SOURCEBUFFERPIXELFORMAT>
    void                                              PutBlendFromColor               (const SOURCEPIXELFORMATRENDERER& from, const COLORTYPE& color, int xdst, int ydst, int xsrc, int ysrc, XDWORD size, XBYTE cover)
                                                      {
                                                        typedef typename SOURCEPIXELFORMATRENDERER::VALUETYPE srcvaluetype;

                                                        const srcvaluetype* psrc = (srcvaluetype*)from.Row(ysrc);
                                                        if(psrc)
                                                          {
                                                            VALUETYPE* pdst =  (VALUETYPE*)rendererbuffer->Row(xdst, ydst, size) + xdst;

                                                            do{ PutOrBlendPixel(pdst, color, (*psrc * cover + BASESCALE_MASK) >> BASESCALE_SHIFT);

                                                                ++psrc;
                                                                ++pdst;

                                                              } while(--size);
                                                          }
                                                      }


    template<class SOURCEBUFFERPIXELFORMAT>
    void                                              PutBlendFromLut                 (const SOURCEPIXELFORMATRENDERER& from, const COLORTYPE* colorlut, int xdst, int ydst, int xsrc, int ysrc, XDWORD size, XBYTE cover)
                                                      {
                                                        typedef typename SOURCEPIXELFORMATRENDERER::VALUETYPE srcvaluetype;

                                                        const srcvaluetype* psrc = (srcvaluetype*)from.Row(ysrc);

                                                        if(psrc)
                                                          {
                                                            VALUETYPE* pdst = (VALUETYPE*)rendererbuffer->Row(xdst, ydst, len) + xdst;

                                                            do{ PutOrBlendPixel(pdst, colorlut[*psrc], cover);

                                                                ++psrc;
                                                                ++pdst;

                                                              } while(--size);
                                                          }
                                                      }


  private:

    static void                                       PutOrBlendPixel                 (VALUETYPE* p, const COLORTYPE& c, XDWORD cover)
                                                      {
                                                        if(c.a)
                                                          {
                                                            CALCTYPE alpha = (CALCTYPE(c.a) * (cover + 1)) >> 8;
                                                            if(alpha == BASESCALE_MASK)
                                                              {
                                                                *p = c.v;
                                                              }
                                                             else
                                                              {
                                                                BLENDER::blend_pix(p, c.v, alpha, cover);
                                                              }
                                                          }
                                                      }


    static void                                       PutOrBlendPixel                 (VALUETYPE* p, const COLORTYPE& c)
                                                      {
                                                        if(c.a)
                                                          {
                                                            if(c.a == BASESCALE_MASK)
                                                              {
                                                                *p = c.v;
                                                              }
                                                             else
                                                              {
                                                                BLENDER::blend_pix(p, c.v, c.a);
                                                              }
                                                          }
                                                      }


    RENDERERBUFFERTYPE*                               rendererbuffer;
};








/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/



#endif



