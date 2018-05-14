/*------------------------------------------------------------------------------------------
//  GRPBUFFERRENDERERBASE.H
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

#ifndef _GRPBUFFERRENDERERBASE_H_
#define _GRPBUFFERRENDERERBASE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPRect.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

template<class BUFFERPIXELFORMAT>
class GRPBUFFERRENDERERBASE
{
  public:

    typedef BUFFERPIXELFORMAT                           BUFFERPIXELFORMATTYPE;
    typedef typename BUFFERPIXELFORMATTYPE::COLORTYPE   COLORTYPE;
    typedef typename BUFFERPIXELFORMATTYPE::ROWDATA     ROWDATA;

                                                        GRPBUFFERRENDERERBASE               ()
                                                        {
                                                          Clean();

                                                          this->pixelformatbuffer = NULL;
                                                          clipbox.Set(1, 1, 0, 0);
                                                        }

                                                        GRPBUFFERRENDERERBASE               (BUFFERPIXELFORMATTYPE& pixelformatbuffer)
                                                        {
                                                          Clean();

                                                          this->pixelformatbuffer = &pixelformatbuffer;
                                                          clipbox.Set(0, 0, pixelformatbuffer.GetWidth() - 1, pixelformatbuffer.GetHeight() - 1);
                                                        }


    virtual                                            ~GRPBUFFERRENDERERBASE               ()
                                                        {
                                                          Clean();
                                                        }


     const BUFFERPIXELFORMATTYPE&                       GetPixelFormatBuffer                () const
                                                        {
                                                          return *pixelformatbuffer;
                                                        }

     BUFFERPIXELFORMATTYPE&                             GetPixelFormatBuffer()
                                                        {
                                                          return *pixelformatbuffer;
                                                        }


    const XDWORD                                        GetWidth                            ()
                                                        {
                                                          return pixelformatbuffer->GetWidth();
                                                        }


    const XDWORD                                        GetHeight                           ()
                                                        {
                                                          return pixelformatbuffer->GetHeight();
                                                        }


    bool                                                SetClipBox                          (int x1, int y1, int x2, int y2)
                                                        {
                                                          GRPRECTINT cb(x1, y1, x2, y2);
                                                          cb.Normalize();

                                                          if(cb.Clip(GRPRECT(0, 0, GetWidth() - 1, GetHeight() - 1)))
                                                            {
                                                                clipbox = cb;
                                                                return true;
                                                            }

                                                          clipbox.x1 = 1;
                                                          clipbox.y1 = 1;
                                                          clipbox.x2 = 0;
                                                          clipbox.y2 = 0;

                                                          return false;
                                                        }

    void                                                ResetClipping                       (bool visibility)
                                                        {
                                                          if(visibility)
                                                            {
                                                              clipbox.x1 = 0;
                                                              clipbox.y1 = 0;
                                                              clipbox.x2 = GetWidth() - 1;
                                                              clipbox.y2 = GetHeight() - 1;
                                                            }
                                                           else
                                                            {
                                                              clipbox.x1 = 1;
                                                              clipbox.y1 = 1;
                                                              clipbox.x2 = 0;
                                                              clipbox.y2 = 0;
                                                            }
                                                        }

    void                                                ClipBoxNaked                        (int x1, int y1, int x2, int y2)
                                                        {
                                                            clipbox.x1 = x1;
                                                            clipbox.y1 = y1;
                                                            clipbox.x2 = x2;
                                                            clipbox.y2 = y2;
                                                        }


    bool                                                IsInBox                             (int x, int y) const
                                                        {
                                                            return x >= clipbox.x1 && y >= clipbox.y1 &&
                                                                   x <= clipbox.x2 && y <= clipbox.y2;
                                                        }


    const GRPRECTINT&                                   GetClipBox                          () const
                                                        {
                                                          return clipbox;
                                                        }

    GRPRECTINT                                          ClipRectArea                        (GRPRECTINT& dst, GRPRECTINT& src, int wsrc, int hsrc) const
                                                        {
                                                          GRPRECTINT rc(0,0,0,0);
                                                          GRPRECTINT cb = GetClipBox();

                                                          ++cb.x2;
                                                          ++cb.y2;

                                                          if(src.x1 < 0)
                                                            {
                                                              dst.x1 -= src.x1;
                                                              src.x1 = 0;
                                                            }

                                                          if(src.y1 < 0)
                                                            {
                                                              dst.y1 -= src.y1;
                                                              src.y1 = 0;
                                                            }

                                                          if(src.x2 > wsrc) src.x2 = wsrc;
                                                          if(src.y2 > hsrc) src.y2 = hsrc;

                                                          if(dst.x1 < cb.x1)
                                                            {
                                                              src.x1 += cb.x1 - dst.x1;
                                                              dst.x1 = cb.x1;
                                                            }

                                                          if(dst.y1 < cb.y1)
                                                            {
                                                              src.y1 += cb.y1 - dst.y1;
                                                              dst.y1 = cb.y1;
                                                            }

                                                          if(dst.x2 > cb.x2) dst.x2 = cb.x2;
                                                          if(dst.y2 > cb.y2) dst.y2 = cb.y2;

                                                          rc.x2 = dst.x2 - dst.x1;
                                                          rc.y2 = dst.y2 - dst.y1;

                                                          if(rc.x2 > src.x2 - src.x1) rc.x2 = src.x2 - src.x1;
                                                          if(rc.y2 > src.y2 - src.y1) rc.y2 = src.y2 - src.y1;

                                                          return rc;
                                                        }


    int                                                 GetX1                               ()     const { return clipbox.x1; }
    int                                                 GetY1                               ()     const { return clipbox.y1; }
    int                                                 GetX2                               ()     const { return clipbox.x2; }
    int                                                 GetY2                               ()     const { return clipbox.y2; }


    void                                                Clear                               (const COLORTYPE& c)
                                                        {
                                                          XDWORD y;
                                                          if(GetWidth())
                                                            {
                                                              for(y=0; y<GetHeight(); y++)
                                                                {
                                                                  pixelformatbuffer->PutHLine(0, y, GetWidth(), c);
                                                                }
                                                            }
                                                        }


    COLORTYPE                                           GetPixel                            (int x, int y) const
                                                        {
                                                          return IsInBox(x, y) ? pixelformatbuffer->GetPixel(x, y) : COLORTYPE::GetNoColor();
                                                        }


    void                                                PutPixel                            (int x, int y, const COLORTYPE& c)
                                                        {
                                                          if(IsInBox(x, y))
                                                            {
                                                              pixelformatbuffer->PutPixel(x, y, c);
                                                            }
                                                        }


    void                                                PutBlendPixel                       (int x, int y, const COLORTYPE& c, COVERTYPE cover)
                                                        {
                                                          if(IsInBox(x, y))
                                                            {
                                                              pixelformatbuffer->PutBlendPixel(x, y, c, cover);
                                                            }
                                                        }


    void                                                PutHLine                            (int x1, int y, int x2, const COLORTYPE& c)
                                                        {
                                                          if(x1 > x2) { int t = x2; x2 = x1; x1 = t; }

                                                          if(y  > GetY2()) return;
                                                          if(y  < GetY1()) return;
                                                          if(x1 > GetX2()) return;
                                                          if(x2 < GetX1()) return;

                                                          if(x1 < GetX1()) x1 = GetX1();
                                                          if(x2 > GetX2()) x2 = GetX2();

                                                          pixelformatbuffer->PutHLine(x1, y, x2 - x1 + 1, c);
                                                        }


    void                                                PutVLine                            (int x, int y1, int y2, const COLORTYPE& c)
                                                        {
                                                          if(y1 > y2) { int t = y2; y2 = y1; y1 = t; }

                                                          if(x  > GetX2()) return;
                                                          if(x  < GetX1()) return;
                                                          if(y1 > GetY2()) return;
                                                          if(y2 < GetY1()) return;

                                                          if(y1 < GetY1()) y1 = GetY1();
                                                          if(y2 > GetY2()) y2 = GetY2();

                                                          pixelformatbuffer->PutVLine(x, y1, y2 - y1 + 1, c);
                                                        }

     void                                               PutBlendHLine                       (int x1, int y, int x2, const COLORTYPE& c, COVERTYPE cover)
                                                        {
                                                          if(x1 > x2) { int t = x2; x2 = x1; x1 = t; }

                                                          if(y  > GetY2()) return;
                                                          if(y  < GetY1()) return;
                                                          if(x1 > GetX2()) return;
                                                          if(x2 < GetX1()) return;

                                                          if(x1 < GetX1()) x1 = GetX1();
                                                          if(x2 > GetX2()) x2 = GetX2();

                                                          pixelformatbuffer->PutBlendHLine(x1, y, x2 - x1 + 1, c, cover);
                                                        }

    void                                                PutBlendVLine                       (int x, int y1, int y2, const COLORTYPE& c, COVERTYPE cover)
                                                        {
                                                          if(y1 > y2) { int t = y2; y2 = y1; y1 = t; }

                                                          if(x  > GetX2()) return;
                                                          if(x  < GetX1()) return;
                                                          if(y1 > GetY2()) return;
                                                          if(y2 < GetY1()) return;

                                                          if(y1 < GetY1()) y1 = GetY1();
                                                          if(y2 > GetY2()) y2 = GetY2();

                                                          pixelformatbuffer->PutBlendVLine(x, y1, y2 - y1 + 1, c, cover);
                                                        }


     void                                               PutBlendHLineSolidSpan              (int x, int y, int size, const COLORTYPE& c, const COVERTYPE* covers)
                                                        {
                                                          if(y > GetY2()) return;
                                                          if(y < GetY1()) return;

                                                          if(x < GetX1())
                                                            {
                                                              size -= GetX1() - x;
                                                              if(size <= 0) return;
                                                              covers += GetX1() - x;
                                                              x = GetX1();
                                                            }

                                                          if(x + size > GetX2())
                                                            {
                                                              size = GetX2() - x + 1;
                                                              if(size <= 0) return;
                                                            }

                                                          pixelformatbuffer->PutBlendHLineSolidSpan(x, y, size, c, covers);
                                                        }


     void                                               PutBlendVLineSolidSpan              (int x, int y, int size, const COLORTYPE& c, const COVERTYPE* covers)
                                                        {
                                                          if(x > GetX2()) return;
                                                          if(x < GetX1()) return;

                                                          if(y < GetY1())
                                                            {
                                                              size -= GetY1() - y;
                                                              if(size <= 0) return;
                                                              covers += GetY1() - y;
                                                              y = GetY1();
                                                            }

                                                          if(y + size > GetY2())
                                                            {
                                                              size = GetY2() - y + 1;
                                                              if(size <= 0) return;
                                                            }

                                                          pixelformatbuffer->PutBlendVLineSolidSpan(x, y, size, c, covers);
                                                        }


      void                                              PutBlendHLineColorSpan              (int x, int y, int size, const COLORTYPE* colors, const COVERTYPE* covers)
                                                        {
                                                          if(y > GetY2()) return;
                                                          if(y < GetY1()) return;

                                                          if(x < GetX1())
                                                            {
                                                              int d = GetX1() - x;
                                                              size -= d;
                                                              if(size <= 0) return;
                                                              if(covers) covers += d;
                                                              colors += d;
                                                              x = GetX1();
                                                            }

                                                          if(x + size > GetX2())
                                                            {
                                                              size = GetX2() - x + 1;
                                                              if(size <= 0) return;
                                                            }

                                                          pixelformatbuffer->PutBlendHLineColorSpan(x, y, size, colors, covers);
                                                        }


    void                                                PutBlendVLineColorSpan              (int x, int y, int size, const COLORTYPE* colors, const COVERTYPE* covers)
                                                        {
                                                          if(x > GetX2()) return;
                                                          if(x < GetX1()) return;

                                                          if(y < GetY1())
                                                            {
                                                              int d = GetY1() - y;
                                                              size -= d;
                                                              if(size <= 0) return;
                                                              if(covers) covers += d;
                                                              colors += d;
                                                              y = GetY1();
                                                            }

                                                          if(y + size > GetY2())
                                                            {
                                                              size = GetY2() - y + 1;
                                                              if(size <= 0) return;
                                                            }

                                                          pixelformatbuffer->PutBlendVLineColorSpan(x, y, size, colors, covers);
                                                        }


    void                                                PutBlendHLineColorSpanNoClip        (int x, int y, int size, const COLORTYPE* colors, const COVERTYPE* covers)
                                                        {
                                                          pixelformatbuffer->PutBlendHLineColorSpan(x, y, size, colors, covers);
                                                        }


    void                                                PutBlendVLineColorSpanNoClip        (int x, int y, int size, const COLORTYPE* colors, const COVERTYPE* covers)
                                                        {
                                                          pixelformatbuffer->PutBlendVLineColorSpan(x, y, size, colors, covers);
                                                        }



    void                                                PutBar                              (int x1, int y1, int x2, int y2, const COLORTYPE& c)
                                                        {
                                                          GRPRECTINT rc(x1, y1, x2, y2);

                                                          rc.Normalize();

                                                          if(rc.Clip(GetClipBox()))
                                                            {
                                                              int y;
                                                              for(y = rc.y1; y <= rc.y2; y++)
                                                                {
                                                                  pixelformatbuffer->PutHLine(rc.x1, y, unsigned(rc.x2 - rc.x1 + 1), c);
                                                                }
                                                            }
                                                        }


    void                                                PutBlendBar                         (int x1, int y1, int x2, int y2, const COLORTYPE& c, COVERTYPE cover)
                                                        {
                                                          GRPRECTINT rc(x1, y1, x2, y2);
                                                          rc.Normalize();
                                                          if(rc.Clip(GetClipBox()))
                                                            {
                                                              int y;

                                                              for(y = rc.y1; y <= rc.y2; y++)
                                                                {
                                                                  pixelformatbuffer->PutBlendHLine(rc.x1, y, unsigned(rc.x2 - rc.x1 + 1), c, cover);
                                                                }
                                                            }
                                                        }


    void                                                CopyFrom                            (GRPBUFFER* from, const GRPRECTINT* rc = 0, int xto = 0, int yto = 0)
                                                        {
                                                          GRPRECTINT tmprect(0, 0, from->GetWidth(), from->GetHeight());
                                                          if(rc == 0)
                                                            {
                                                              rc = &tmprect;
                                                            }

                                                          GRPRECTINT rc2(*rc);

                                                          rc2.Normalize();

                                                          if(rc2.Clip(GRPRECTINT(0, 0, from->GetWidth() - 1, from->GetHeight() - 1)))
                                                            {
                                                              GRPRECTINT rc3( xto + rc2.x1 - rc->x1,
                                                                              yto + rc2.y1 - rc->y1,
                                                                              xto + rc2.x2 - rc->x1,
                                                                              yto + rc2.y2 - rc->y1);

                                                              rc3.Normalize();

                                                              if(rc3.Clip(GetClipBox()))
                                                                {
                                                                  while(rc3.y1 <= rc3.y2)
                                                                    {
                                                                      pixelformatbuffer->CopyFrom((*from),
                                                                                                  rc3.x1, rc3.y1,
                                                                                                  rc2.x1, rc2.y1,
                                                                                                  rc3.x2 - rc3.x1 + 1);
                                                                      ++rc2.y1;
                                                                      ++rc3.y1;
                                                                    }
                                                                }
                                                            }

                                                        }

    template<class SOURCEBUFFERPIXELFORMAT>
    void                                                PutBlendFromLut                     (const SOURCEBUFFERPIXELFORMAT& src, const COLORTYPE* colorlut, const GRPRECTINT* rectsourceptr = 0, int dx = 0, int dy = 0, COVERTYPE cover = COVERTYPE_FULL)
                                                        {
                                                          GRPRECTINT rsrc(0, 0, src.GetWidth(), src.GetHeight());
                                                          if(rectsourceptr)
                                                            {
                                                              rsrc.x1 = rectsourceptr->x1;
                                                              rsrc.y1 = rectsourceptr->y1;
                                                              rsrc.x2 = rectsourceptr->x2 + 1;
                                                              rsrc.y2 = rectsourceptr->y2 + 1;
                                                            }

                                                          // Version with xdst, ydst (absolute positioning)
                                                          //GRPRECTINT rdst(xdst, ydst, xdst + rsrc.x2 - rsrc.x1, ydst + rsrc.y2 - rsrc.y1);

                                                          // Version with dx, dy (relative positioning)
                                                          GRPRECTINT rdst(rsrc.x1 + dx, rsrc.y1 + dy, rsrc.x2 + dx, rsrc.y2 + dy);
                                                          GRPRECTINT rc = ClipRectArea(rdst, rsrc, src.GetWidth(), src.GetHeight());

                                                          if(rc.x2 > 0)
                                                            {
                                                              int incy = 1;
                                                              if(rdst.y1 > rsrc.y1)
                                                                {
                                                                  rsrc.y1 += rc.y2 - 1;
                                                                  rdst.y1 += rc.y2 - 1;
                                                                  incy = -1;
                                                                }

                                                              while(rc.y2 > 0)
                                                                {
                                                                  typename SOURCEBUFFERPIXELFORMAT::ROWDATA rw = src.Row(rsrc.y1);

                                                                  if(rw.ptr)
                                                                    {
                                                                      int x1src = rsrc.x1;
                                                                      int x1dst = rdst.x1;
                                                                      int size  = rc.x2;

                                                                      if(rw.x1 > x1src)
                                                                        {
                                                                          x1dst += rw.x1 - x1src;
                                                                          size  -= rw.x1 - x1src;
                                                                          x1src  = rw.x1;
                                                                        }

                                                                      if(size > 0)
                                                                        {
                                                                          if(x1src + size-1 > rw.x2) size -= x1src + size - rw.x2 - 1;

                                                                          if(size > 0)  pixelformatbuffer->PutBlendFromLut(src, colorlut, x1dst, rdst.y1, x1src, rsrc.y1, size, cover);
                                                                        }
                                                                    }

                                                                  rdst.y1 += incy;
                                                                  rsrc.y1 += incy;
                                                                  --rc.y2;
                                                                }
                                                            }
                                                        }


  private:


    void                                                Clean                               ()
                                                        {
                                                          pixelformatbuffer = NULL;
                                                        }

     BUFFERPIXELFORMATTYPE*                             pixelformatbuffer;
     GRPRECTINT                                         clipbox;

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

