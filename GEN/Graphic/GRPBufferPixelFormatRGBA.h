/*------------------------------------------------------------------------------------------
//  GRPBUFFERPIXELFORMATRGBA.H
*/
/**
// \class
//
//  Graphics Pixel Format Buffer RGBA Class
//
//  @author  Diego Martinez Ruiz de Gaona
//  @version 01/08/2013 16:02:44
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPBUFFERPIXELFORMATRGBA_H_
#define _GRPBUFFERPIXELFORMATRGBA_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XDebugTrace.h"

#include "GRPBufferColorRGB.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


enum COMPOP
{
  COMPOP_CLEAR      ,
  COMPOP_SOURCE     ,
  COMPOP_TARGET     ,
  COMPOP_SOURCEOVER ,
  COMPOP_TARGETOVER ,
  COMPOP_SOURCEIN   ,
  COMPOP_TARGETIN   ,
  COMPOP_SOURCEOUT  ,
  COMPOP_TARGETOUT  ,
  COMPOP_SOURCEATOP ,
  COMPOP_TARGETATOP ,
  COMPOP_XOR        ,
  COMPOP_PLUS       ,
  COMPOP_MINUS      ,
  COMPOP_MULTIPLY   ,
  COMPOP_SCREEN     ,
  COMPOP_OVERLAY    ,
  COMPOP_DARKEN     ,
  COMPOP_LIGHTEN    ,
  COMPOP_COLORDODGE ,
  COMPOP_COLORBURN  ,
  COMPOP_HARDLIGHT  ,
  COMPOP_SOFTLIGHT  ,
  COMPOP_DIFFERENCE ,
  COMPOP_EXCLUSION  ,
  COMPOP_CONTRAST   ,
  COMPOP_INVERT     ,
  COMPOP_INVERTRGB  ,

  ENDOFCOMPOP
};


template<class T> inline T SD_MIN(T a, T b) { return (a < b) ? a : b; }
template<class T> inline T SD_MAX(T a, T b) { return (a > b) ? a : b; }


/*---- CLASS -----------------------------------------------------------------------------*/


template<class COLORT, class ORDER>
class MULTIPLIERRGBA
{
  public:

    typedef typename COLORT::VALUETYPE                VALUETYPE;
    typedef typename COLORT::CALCTYPE                 CALCTYPE;


    static void                                       Premultiply                         (VALUETYPE* p)
                                                      {
                                                        CALCTYPE a = p[ORDER::A];
                                                        if(a < COLORT::BASESCALE_MASK)
                                                          {
                                                            if(a == 0)
                                                              {
                                                                p[ORDER::R] = p[ORDER::G] = p[ORDER::B] = 0;
                                                                return;
                                                              }

                                                            p[ORDER::R] = VALUETYPE((p[ORDER::R] * a + COLORT::BASESCALE_MASK) >> COLORT::BASESCALE_SHIFT);
                                                            p[ORDER::G] = VALUETYPE((p[ORDER::G] * a + COLORT::BASESCALE_MASK) >> COLORT::BASESCALE_SHIFT);
                                                            p[ORDER::B] = VALUETYPE((p[ORDER::B] * a + COLORT::BASESCALE_MASK) >> COLORT::BASESCALE_SHIFT);
                                                          }
                                                      }



    static void                                       Demultiply                          (VALUETYPE* p)
                                                      {
                                                        CALCTYPE a = p[ORDER::A];
                                                        if(a < COLORT::BASESCALE_MASK)
                                                          {
                                                            if(a == 0)
                                                              {
                                                                p[ORDER::R] = p[ORDER::G] = p[ORDER::B] = 0;
                                                                return;
                                                              }

                                                            CALCTYPE r = (CALCTYPE(p[ORDER::R]) * COLORT::BASESCALE_MASK) / a;
                                                            CALCTYPE g = (CALCTYPE(p[ORDER::G]) * COLORT::BASESCALE_MASK) / a;
                                                            CALCTYPE b = (CALCTYPE(p[ORDER::B]) * COLORT::BASESCALE_MASK) / a;

                                                            p[ORDER::R] = VALUETYPE((r > COLORT::BASESCALE_MASK) ? COLORT::BASESCALE_MASK : r);
                                                            p[ORDER::G] = VALUETYPE((g > COLORT::BASESCALE_MASK) ? COLORT::BASESCALE_MASK : g);
                                                            p[ORDER::B] = VALUETYPE((b > COLORT::BASESCALE_MASK) ? COLORT::BASESCALE_MASK : b);
                                                          }
                                                      }
};




template<class COLORT, class ORDER, class GAMMALUT>
class APPLYGAMMADIRRGBA
{
  public:

    typedef typename COLORT::VALUETYPE                VALUETYPE;

                                                      APPLYGAMMADIRRGBA                   (const GAMMALUT& gamma)
                                                      {
                                                        this->gamma = gamma;
                                                      }

    void                                              operator()                          (VALUETYPE* p)
                                                      {
                                                        p[ORDER::R] = gamma.Dir(p[ORDER::R]);
                                                        p[ORDER::G] = gamma.Dir(p[ORDER::G]);
                                                        p[ORDER::B] = gamma.Dir(p[ORDER::B]);
                                                      }

  private:

    const GAMMALUT&                                   gamma;
};




template<class COLORT, class ORDER, class GAMMALUT>
class APPLYGAMMAINVRGBA
{
  public:

    typedef typename COLORT::VALUETYPE                VALUETYPE;

                                                      APPLYGAMMAINVRGBA                   (const GAMMALUT& gamma)
                                                      {
                                                        this->gamma = gamma;
                                                      }

    void                                              operator()                          (VALUETYPE* p)
                                                      {
                                                        p[ORDER::R] = gamma.Inv(p[ORDER::R]);
                                                        p[ORDER::G] = gamma.Inv(p[ORDER::G]);
                                                        p[ORDER::B] = gamma.Inv(p[ORDER::B]);
                                                      }

  private:

    const GAMMALUT&                                   gamma;
};




template<class COLORT, class ORDER>
class BLENDERRGBA
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };


    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD cover = 0)
                                                      {
                                                        CALCTYPE r = p[ORDER::R];
                                                        CALCTYPE g = p[ORDER::G];
                                                        CALCTYPE b = p[ORDER::B];
                                                        CALCTYPE a = p[ORDER::A];

                                                        p[ORDER::R] = (VALUETYPE)(((cr - r) * alpha + (r << BASESCALE_SHIFT)) >> BASESCALE_SHIFT);
                                                        p[ORDER::G] = (VALUETYPE)(((cg - g) * alpha + (g << BASESCALE_SHIFT)) >> BASESCALE_SHIFT);
                                                        p[ORDER::B] = (VALUETYPE)(((cb - b) * alpha + (b << BASESCALE_SHIFT)) >> BASESCALE_SHIFT);
                                                        p[ORDER::A] = (VALUETYPE)((alpha + a) - ((alpha * a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                      }
};



template<class COLORT, class ORDER>
class BLENDERRGBAPRE
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };


    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD cover)
                                                      {
                                                        alpha = COLORTYPE::BASESCALE_MASK - alpha;
                                                        cover = (cover + 1) << (BASESCALE_SHIFT - 8);

                                                        p[ORDER::R] = (VALUETYPE)((p[ORDER::R] * alpha + cr * cover) >> BASESCALE_SHIFT);
                                                        p[ORDER::G] = (VALUETYPE)((p[ORDER::G] * alpha + cg * cover) >> BASESCALE_SHIFT);
                                                        p[ORDER::B] = (VALUETYPE)((p[ORDER::B] * alpha + cb * cover) >> BASESCALE_SHIFT);
                                                        p[ORDER::A] = (VALUETYPE)(BASESCALE_MASK - ((alpha * (BASESCALE_MASK - p[ORDER::A])) >> BASESCALE_SHIFT));
                                                      }


    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha)
                                                      {
                                                        alpha = COLORTYPE::BASESCALE_MASK - alpha;

                                                        p[ORDER::R] = (VALUETYPE)(((p[ORDER::R] * alpha) >> BASESCALE_SHIFT) + cr);
                                                        p[ORDER::G] = (VALUETYPE)(((p[ORDER::G] * alpha) >> BASESCALE_SHIFT) + cg);
                                                        p[ORDER::B] = (VALUETYPE)(((p[ORDER::B] * alpha) >> BASESCALE_SHIFT) + cb);
                                                        p[ORDER::A] = (VALUETYPE)(BASESCALE_MASK - ((alpha * (BASESCALE_MASK - p[ORDER::A])) >> BASESCALE_SHIFT));
                                                      }
};



template<class COLORT, class ORDER>
class BLENDERRGBAPLAIN
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT
    };


    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD cover = 0)
                                                      {
                                                        if(alpha == 0) return;

                                                        CALCTYPE a = p[ORDER::A];
                                                        CALCTYPE r = p[ORDER::R] * a;
                                                        CALCTYPE g = p[ORDER::G] * a;
                                                        CALCTYPE b = p[ORDER::B] * a;

                                                        a = ((alpha + a) << BASESCALE_SHIFT) - alpha * a;

                                                        p[ORDER::A] = (VALUETYPE)(a >> BASESCALE_SHIFT);
                                                        p[ORDER::R] = (VALUETYPE)((((cr << BASESCALE_SHIFT) - r) * alpha + (r << BASESCALE_SHIFT)) / a);
                                                        p[ORDER::G] = (VALUETYPE)((((cg << BASESCALE_SHIFT) - g) * alpha + (g << BASESCALE_SHIFT)) / a);
                                                        p[ORDER::B] = (VALUETYPE)((((cb << BASESCALE_SHIFT) - b) * alpha + (b << BASESCALE_SHIFT)) / a);
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_CLEAR
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD, XDWORD, XDWORD, XDWORD, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            cover = 255 - cover;

                                                            p[ORDER::R] = (VALUETYPE)((p[ORDER::R] * cover + 255) >> 8);
                                                            p[ORDER::G] = (VALUETYPE)((p[ORDER::G] * cover + 255) >> 8);
                                                            p[ORDER::B] = (VALUETYPE)((p[ORDER::B] * cover + 255) >> 8);
                                                            p[ORDER::A] = (VALUETYPE)((p[ORDER::A] * cover + 255) >> 8);
                                                          }
                                                         else
                                                          {
                                                            p[0] = p[1] = p[2] = p[3] = 0;
                                                          }
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_SOURCE
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;

    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            XDWORD alpha = 255 - cover;

                                                            p[ORDER::R] = (VALUETYPE)(((p[ORDER::R] * alpha + 255) >> 8) + ((sr * cover + 255) >> 8));
                                                            p[ORDER::G] = (VALUETYPE)(((p[ORDER::G] * alpha + 255) >> 8) + ((sg * cover + 255) >> 8));
                                                            p[ORDER::B] = (VALUETYPE)(((p[ORDER::B] * alpha + 255) >> 8) + ((sb * cover + 255) >> 8));
                                                            p[ORDER::A] = (VALUETYPE)(((p[ORDER::A] * alpha + 255) >> 8) + ((sa * cover + 255) >> 8));
                                                          }
                                                         else
                                                          {
                                                            p[ORDER::R] = sr;
                                                            p[ORDER::G] = sg;
                                                            p[ORDER::B] = sb;
                                                            p[ORDER::A] = sa;
                                                          }
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_TARGET
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;

    static void                                       PutBlendPixel                       (VALUETYPE*, XDWORD, XDWORD, XDWORD, XDWORD, XDWORD)
                                                      {
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_SOURCEOVER
{
  public:

    typedef COLORT COLORTYPE;
    typedef ORDER ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = Sca + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        CALCTYPE s1a = BASESCALE_MASK - sa;

                                                        p[ORDER::R] = (VALUETYPE)(sr + ((p[ORDER::R] * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                        p[ORDER::G] = (VALUETYPE)(sg + ((p[ORDER::G] * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                        p[ORDER::B] = (VALUETYPE)(sb + ((p[ORDER::B] * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                        p[ORDER::A] = (VALUETYPE)(sa + p[ORDER::A] - ((sa * p[ORDER::A] + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_TARGETOVER
{
  public:

    typedef COLORT                        COLORTYPE;
    typedef ORDER                         ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE  CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = Dca + Sca.(1 - Da)
    // Da'  = Sa + Da - Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        CALCTYPE d1a = BASESCALE_MASK - p[ORDER::A];

                                                        p[ORDER::R] = (VALUETYPE)(p[ORDER::R] + ((sr * d1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                        p[ORDER::G] = (VALUETYPE)(p[ORDER::G] + ((sg * d1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                        p[ORDER::B] = (VALUETYPE)(p[ORDER::B] + ((sb * d1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                        p[ORDER::A] = (VALUETYPE)(sa + p[ORDER::A] - ((sa * p[ORDER::A] + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_SOURCEIN
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = Sca.Da
    // Da'  = Sa.Da
    static void                                       PutBlendPixel                         (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        CALCTYPE da = p[ORDER::A];
                                                        if(cover < 255)
                                                          {
                                                            XDWORD alpha = 255 - cover;

                                                            p[ORDER::R] = (VALUETYPE)(((p[ORDER::R] * alpha + 255) >> 8) + ((((sr * da + BASESCALE_MASK) >> BASESCALE_SHIFT) * cover + 255) >> 8));
                                                            p[ORDER::G] = (VALUETYPE)(((p[ORDER::G] * alpha + 255) >> 8) + ((((sg * da + BASESCALE_MASK) >> BASESCALE_SHIFT) * cover + 255) >> 8));
                                                            p[ORDER::B] = (VALUETYPE)(((p[ORDER::B] * alpha + 255) >> 8) + ((((sb * da + BASESCALE_MASK) >> BASESCALE_SHIFT) * cover + 255) >> 8));
                                                            p[ORDER::A] = (VALUETYPE)(((p[ORDER::A] * alpha + 255) >> 8) + ((((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT) * cover + 255) >> 8));
                                                          }
                                                         else
                                                          {
                                                            p[ORDER::R] = (VALUETYPE)((sr * da + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::G] = (VALUETYPE)((sg * da + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::B] = (VALUETYPE)((sb * da + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::A] = (VALUETYPE)((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                          }
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_TARGETIN
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = Dca.Sa
    // Da'  = Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD, XDWORD, XDWORD, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sa = BASESCALE_MASK - ((cover * (BASESCALE_MASK - sa) + 255) >> 8);
                                                          }

                                                        p[ORDER::R] = (VALUETYPE)((p[ORDER::R] * sa + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                        p[ORDER::G] = (VALUETYPE)((p[ORDER::G] * sa + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                        p[ORDER::B] = (VALUETYPE)((p[ORDER::B] * sa + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                        p[ORDER::A] = (VALUETYPE)((p[ORDER::A] * sa + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_SOURCEOUT
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = Sca.(1 - Da)
    // Da'  = Sa.(1 - Da)
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        CALCTYPE da = BASESCALE_MASK - p[ORDER::A];
                                                        if(cover < 255)
                                                          {
                                                            XDWORD alpha = 255 - cover;

                                                            p[ORDER::R] = (VALUETYPE)(((p[ORDER::R] * alpha + 255) >> 8) + ((((sr * da + BASESCALE_MASK) >> BASESCALE_SHIFT) * cover + 255) >> 8));
                                                            p[ORDER::G] = (VALUETYPE)(((p[ORDER::G] * alpha + 255) >> 8) + ((((sg * da + BASESCALE_MASK) >> BASESCALE_SHIFT) * cover + 255) >> 8));
                                                            p[ORDER::B] = (VALUETYPE)(((p[ORDER::B] * alpha + 255) >> 8) + ((((sb * da + BASESCALE_MASK) >> BASESCALE_SHIFT) * cover + 255) >> 8));
                                                            p[ORDER::A] = (VALUETYPE)(((p[ORDER::A] * alpha + 255) >> 8) + ((((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT) * cover + 255) >> 8));
                                                          }
                                                         else
                                                          {
                                                            p[ORDER::R] = (VALUETYPE)((sr * da + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::G] = (VALUETYPE)((sg * da + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::B] = (VALUETYPE)((sb * da + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::A] = (VALUETYPE)((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                          }
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_TARGETOUT
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = Dca.(1 - Sa)
    // Da'  = Da.(1 - Sa)
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD, XDWORD, XDWORD, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        sa = BASESCALE_MASK - sa;

                                                        p[ORDER::R] = (VALUETYPE)((p[ORDER::R] * sa + BASESCALE_SHIFT) >> BASESCALE_SHIFT);
                                                        p[ORDER::G] = (VALUETYPE)((p[ORDER::G] * sa + BASESCALE_SHIFT) >> BASESCALE_SHIFT);
                                                        p[ORDER::B] = (VALUETYPE)((p[ORDER::B] * sa + BASESCALE_SHIFT) >> BASESCALE_SHIFT);
                                                        p[ORDER::A] = (VALUETYPE)((p[ORDER::A] * sa + BASESCALE_SHIFT) >> BASESCALE_SHIFT);
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_SOURCEATOP
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = Sca.Da + Dca.(1 - Sa)
    // Da'  = Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        CALCTYPE da = p[ORDER::A];

                                                        sa = BASESCALE_MASK - sa;

                                                        p[ORDER::R] = (VALUETYPE)((sr * da + p[ORDER::R] * sa + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                        p[ORDER::G] = (VALUETYPE)((sg * da + p[ORDER::G] * sa + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                        p[ORDER::B] = (VALUETYPE)((sb * da + p[ORDER::B] * sa + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_TARGETATOP
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = Dca.Sa + Sca.(1 - Da)
    // Da'  = Sa
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        CALCTYPE da = BASESCALE_MASK - p[ORDER::A];

                                                        if(cover < 255)
                                                          {
                                                            XDWORD alpha = 255 - cover;

                                                            sr = (p[ORDER::R] * sa + sr * da + BASESCALE_MASK) >> BASESCALE_SHIFT;
                                                            sg = (p[ORDER::G] * sa + sg * da + BASESCALE_MASK) >> BASESCALE_SHIFT;
                                                            sb = (p[ORDER::B] * sa + sb * da + BASESCALE_MASK) >> BASESCALE_SHIFT;

                                                            p[ORDER::R] = (VALUETYPE)(((p[ORDER::R] * alpha + 255) >> 8) + ((sr * cover + 255) >> 8));
                                                            p[ORDER::G] = (VALUETYPE)(((p[ORDER::G] * alpha + 255) >> 8) + ((sg * cover + 255) >> 8));
                                                            p[ORDER::B] = (VALUETYPE)(((p[ORDER::B] * alpha + 255) >> 8) + ((sb * cover + 255) >> 8));
                                                            p[ORDER::A] = (VALUETYPE)(((p[ORDER::A] * alpha + 255) >> 8) + ((sa * cover + 255) >> 8));

                                                          }
                                                         else
                                                          {
                                                            p[ORDER::R] = (VALUETYPE)((p[ORDER::R] * sa + sr * da + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::G] = (VALUETYPE)((p[ORDER::G] * sa + sg * da + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::B] = (VALUETYPE)((p[ORDER::B] * sa + sb * da + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::A] = (VALUETYPE)sa;
                                                          }
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_XOR
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - 2.Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                         if(sa)
                                                          {
                                                            CALCTYPE s1a = BASESCALE_MASK - sa;
                                                            CALCTYPE d1a = BASESCALE_MASK - p[ORDER::A];

                                                            p[ORDER::R] = (VALUETYPE)((p[ORDER::R] * s1a + sr * d1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::G] = (VALUETYPE)((p[ORDER::G] * s1a + sg * d1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::B] = (VALUETYPE)((p[ORDER::B] * s1a + sb * d1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::A] = (VALUETYPE)(sa + p[ORDER::A] - ((sa * p[ORDER::A] + BASESCALE_MASK/2) >> (BASESCALE_SHIFT - 1)));
                                                          }
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_PLUS
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = Sca + Dca
    // Da'  = Sa + Da
    static void                                       PutBlenbPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        if(sa)
                                                          {
                                                            CALCTYPE dr = p[ORDER::R] + sr;
                                                            CALCTYPE dg = p[ORDER::G] + sg;
                                                            CALCTYPE db = p[ORDER::B] + sb;
                                                            CALCTYPE da = p[ORDER::A] + sa;

                                                            p[ORDER::R] = (dr > BASESCALE_MASK) ? (VALUETYPE)BASESCALE_MASK : dr;
                                                            p[ORDER::G] = (dg > BASESCALE_MASK) ? (VALUETYPE)BASESCALE_MASK : dg;
                                                            p[ORDER::B] = (db > BASESCALE_MASK) ? (VALUETYPE)BASESCALE_MASK : db;
                                                            p[ORDER::A] = (da > BASESCALE_MASK) ? (VALUETYPE)BASESCALE_MASK : da;
                                                          }
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_MINUS
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = Dca - Sca
    // Da' = 1 - (1 - Sa).(1 - Da)
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        if(sa)
                                                          {
                                                            CALCTYPE dr = p[ORDER::R] - sr;
                                                            CALCTYPE dg = p[ORDER::G] - sg;
                                                            CALCTYPE db = p[ORDER::B] - sb;

                                                            p[ORDER::R] = (dr > BASESCALE_MASK) ? 0 : dr;
                                                            p[ORDER::G] = (dg > BASESCALE_MASK) ? 0 : dg;
                                                            p[ORDER::B] = (db > BASESCALE_MASK) ? 0 : db;
                                                            p[ORDER::A] = (VALUETYPE)(sa + p[ORDER::A] - ((sa * p[ORDER::A] + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            //p[ORDER::A] = (VALUETYPE)(BASESCALE_MASK - (((BASESCALE_MASK - sa) * (BASESCALE_MASK - p[ORDER::A]) + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                          }
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_MULTIPLY
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        if(sa)
                                                          {
                                                            CALCTYPE s1a = BASESCALE_MASK - sa;
                                                            CALCTYPE d1a = BASESCALE_MASK - p[ORDER::A];
                                                            CALCTYPE dr = p[ORDER::R];
                                                            CALCTYPE dg = p[ORDER::G];
                                                            CALCTYPE db = p[ORDER::B];

                                                            p[ORDER::R] = (VALUETYPE)((sr * dr + sr * d1a + dr * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::G] = (VALUETYPE)((sg * dg + sg * d1a + dg * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::B] = (VALUETYPE)((sb * db + sb * d1a + db * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::A] = (VALUETYPE)(sa + p[ORDER::A] - ((sa * p[ORDER::A] + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                          }
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_SCREEN
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = Sca + Dca - Sca.Dca
    // Da'  = Sa + Da - Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        if(sa)
                                                          {
                                                            CALCTYPE dr = p[ORDER::R];
                                                            CALCTYPE dg = p[ORDER::G];
                                                            CALCTYPE db = p[ORDER::B];
                                                            CALCTYPE da = p[ORDER::A];

                                                            p[ORDER::R] = (VALUETYPE)(sr + dr - ((sr * dr + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::G] = (VALUETYPE)(sg + dg - ((sg * dg + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::B] = (VALUETYPE)(sb + db - ((sb * db + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::A] = (VALUETYPE)(sa + da - ((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                          }
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_OVERLAY
{
 public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // if 2.Dca < Da
    //   Dca' = 2.Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
    // otherwise
    //   Dca' = Sa.Da - 2.(Da - Dca).(Sa - Sca) + Sca.(1 - Da) + Dca.(1 - Sa)
    //
    // Da' = Sa + Da - Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        if(sa)
                                                          {
                                                            CALCTYPE d1a  = BASESCALE_MASK - p[ORDER::A];
                                                            CALCTYPE s1a  = BASESCALE_MASK - sa;
                                                            CALCTYPE dr   = p[ORDER::R];
                                                            CALCTYPE dg   = p[ORDER::G];
                                                            CALCTYPE db   = p[ORDER::B];
                                                            CALCTYPE da   = p[ORDER::A];
                                                            CALCTYPE sada = sa * p[ORDER::A];

                                                            p[ORDER::R] = (VALUETYPE)(((2*dr < da) ? 2*sr*dr + sr*d1a + dr*s1a : sada - 2*(da - dr)*(sa - sr) + sr*d1a + dr*s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::G] = (VALUETYPE)(((2*dg < da) ? 2*sg*dg + sg*d1a + dg*s1a : sada - 2*(da - dg)*(sa - sg) + sg*d1a + dg*s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::B] = (VALUETYPE)(((2*db < da) ? 2*sb*db + sb*d1a + db*s1a : sada - 2*(da - db)*(sa - sb) + sb*d1a + db*s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::A] = (VALUETYPE)(sa + da - ((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                          }
                                                      }
};






template<class COLORT, class ORDER>
class COMPOPRGBA_DARKEN
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = min(Sca.Da, Dca.Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        if(sa)
                                                          {
                                                            CALCTYPE d1a = BASESCALE_MASK - p[ORDER::A];
                                                            CALCTYPE s1a = BASESCALE_MASK - sa;
                                                            CALCTYPE dr  = p[ORDER::R];
                                                            CALCTYPE dg  = p[ORDER::G];
                                                            CALCTYPE db  = p[ORDER::B];
                                                            CALCTYPE da  = p[ORDER::A];

                                                            p[ORDER::R] = (VALUETYPE)((SD_MIN(sr * da, dr * sa) + sr * d1a + dr * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::G] = (VALUETYPE)((SD_MIN(sg * da, dg * sa) + sg * d1a + dg * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::B] = (VALUETYPE)((SD_MIN(sb * da, db * sa) + sb * d1a + db * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::A] = (VALUETYPE)(sa + da - ((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                          }
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_LIGHTEN
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = max(Sca.Da, Dca.Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        if(sa)
                                                          {
                                                            CALCTYPE d1a = BASESCALE_MASK - p[ORDER::A];
                                                            CALCTYPE s1a = BASESCALE_MASK - sa;
                                                            CALCTYPE dr  = p[ORDER::R];
                                                            CALCTYPE dg  = p[ORDER::G];
                                                            CALCTYPE db  = p[ORDER::B];
                                                            CALCTYPE da  = p[ORDER::A];

                                                            p[ORDER::R] = (VALUETYPE)((SD_MAX(sr * da, dr * sa) + sr * d1a + dr * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::G] = (VALUETYPE)((SD_MAX(sg * da, dg * sa) + sg * d1a + dg * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::B] = (VALUETYPE)((SD_MAX(sb * da, db * sa) + sb * d1a + db * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::A] = (VALUETYPE)(sa + da - ((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                          }
                                                      }
};





template<class COLORT, class ORDER>
class COMPOPRGBA_COLORDODGE
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;
    typedef typename COLORTYPE::LONGTYPE              LONGTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT  ,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // if Sca.Da + Dca.Sa >= Sa.Da
    //   Dca' = Sa.Da + Sca.(1 - Da) + Dca.(1 - Sa)
    // otherwise
    //   Dca' = Dca.Sa/(1-Sca/Sa) + Sca.(1 - Da) + Dca.(1 - Sa)
    //
    // Da'  = Sa + Da - Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        if(sa)
                                                          {
                                                            CALCTYPE d1a  = BASESCALE_MASK - p[ORDER::A];
                                                            CALCTYPE s1a  = BASESCALE_MASK - sa;
                                                            CALCTYPE dr   = p[ORDER::R];
                                                            CALCTYPE dg   = p[ORDER::G];
                                                            CALCTYPE db   = p[ORDER::B];
                                                            CALCTYPE da   = p[ORDER::A];

                                                            LONGTYPE drsa = dr * sa;
                                                            LONGTYPE dgsa = dg * sa;
                                                            LONGTYPE dbsa = db * sa;
                                                            LONGTYPE srda = sr * da;
                                                            LONGTYPE sgda = sg * da;
                                                            LONGTYPE sbda = sb * da;
                                                            LONGTYPE sada = sa * da;

                                                            p[ORDER::R] = (VALUETYPE)((srda + drsa >= sada) ? (sada + sr * d1a + dr * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT : drsa / (BASESCALE_MASK - (sr << BASESCALE_SHIFT) / sa) + ((sr * d1a + dr * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::G] = (VALUETYPE)((sgda + dgsa >= sada) ? (sada + sg * d1a + dg * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT : dgsa / (BASESCALE_MASK - (sg << BASESCALE_SHIFT) / sa) + ((sg * d1a + dg * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::B] = (VALUETYPE)((sbda + dbsa >= sada) ? (sada + sb * d1a + db * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT : dbsa / (BASESCALE_MASK - (sb << BASESCALE_SHIFT) / sa) + ((sb * d1a + db * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::A] = (VALUETYPE)(sa + da - ((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                          }
                                                      }
};





template<class COLORT, class ORDER>
class COMPOPRGBA_COLORBURN
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;
    typedef typename COLORTYPE::LONGTYPE              LONGTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // if Sca.Da + Dca.Sa <= Sa.Da
    //   Dca' = Sca.(1 - Da) + Dca.(1 - Sa)
    // otherwise
    //   Dca' = Sa.(Sca.Da + Dca.Sa - Sa.Da)/Sca + Sca.(1 - Da) + Dca.(1 - Sa)
    //
    // Da'  = Sa + Da - Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        if(sa)
                                                          {
                                                            CALCTYPE d1a  = BASESCALE_MASK - p[ORDER::A];
                                                            CALCTYPE s1a  = BASESCALE_MASK - sa;

                                                            CALCTYPE dr   = p[ORDER::R];
                                                            CALCTYPE dg   = p[ORDER::G];
                                                            CALCTYPE db   = p[ORDER::B];
                                                            CALCTYPE da   = p[ORDER::A];

                                                            LONGTYPE drsa = dr * sa;
                                                            LONGTYPE dgsa = dg * sa;
                                                            LONGTYPE dbsa = db * sa;
                                                            LONGTYPE srda = sr * da;
                                                            LONGTYPE sgda = sg * da;
                                                            LONGTYPE sbda = sb * da;
                                                            LONGTYPE sada = sa * da;

                                                            p[ORDER::R] = (VALUETYPE)(((srda + drsa <= sada) ? sr * d1a + dr * s1a : sa * (srda + drsa - sada) / sr + sr * d1a + dr * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::G] = (VALUETYPE)(((sgda + dgsa <= sada) ? sg * d1a + dg * s1a : sa * (sgda + dgsa - sada) / sg + sg * d1a + dg * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::B] = (VALUETYPE)(((sbda + dbsa <= sada) ? sb * d1a + db * s1a : sa * (sbda + dbsa - sada) / sb + sb * d1a + db * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::A] = (VALUETYPE)(sa + da - ((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                          }
                                                      }
};





template<class COLORT, class ORDER>
class COMPOPRGBA_HARDLIGHT
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;
    typedef typename COLORTYPE::LONGTYPE              LONGTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // if 2.Sca < Sa
    //    Dca' = 2.Sca.Dca + Sca.(1 - Da) + Dca.(1 - Sa)
    // otherwise
    //    Dca' = Sa.Da - 2.(Da - Dca).(Sa - Sca) + Sca.(1 - Da) + Dca.(1 - Sa)
    //
    // Da'  = Sa + Da - Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        if(sa)
                                                          {
                                                            CALCTYPE d1a  = BASESCALE_MASK - p[ORDER::A];
                                                            CALCTYPE s1a  = BASESCALE_MASK - sa;
                                                            CALCTYPE dr   = p[ORDER::R];
                                                            CALCTYPE dg   = p[ORDER::G];
                                                            CALCTYPE db   = p[ORDER::B];
                                                            CALCTYPE da   = p[ORDER::A];
                                                            CALCTYPE sada = sa * da;

                                                            p[ORDER::R] = (VALUETYPE)(((2*sr < sa) ? 2*sr*dr + sr*d1a + dr*s1a : sada - 2*(da - dr)*(sa - sr) + sr*d1a + dr*s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::G] = (VALUETYPE)(((2*sg < sa) ? 2*sg*dg + sg*d1a + dg*s1a : sada - 2*(da - dg)*(sa - sg) + sg*d1a + dg*s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::B] = (VALUETYPE)(((2*sb < sa) ? 2*sb*db + sb*d1a + db*s1a : sada - 2*(da - db)*(sa - sb) + sb*d1a + db*s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::A] = (VALUETYPE)(sa + da - ((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                          }
                                                      }
};





template<class COLORT, class ORDER>
class COMPOPRGBA_SOFTLIGHT
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;
    typedef typename COLORTYPE::LONGTYPE              LONGTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // if 2.Sca < Sa
    //   Dca' = Dca.(Sa + (1 - Dca/Da).(2.Sca - Sa)) + Sca.(1 - Da) + Dca.(1 - Sa)
    // otherwise if 8.Dca <= Da
    //   Dca' = Dca.(Sa + (1 - Dca/Da).(2.Sca - Sa).(3 - 8.Dca/Da)) + Sca.(1 - Da) + Dca.(1 - Sa)
    // otherwise
    //   Dca' = (Dca.Sa + ((Dca/Da)^(0.5).Da - Dca).(2.Sca - Sa)) + Sca.(1 - Da) + Dca.(1 - Sa)
    //
    // Da'  = Sa + Da - Sa.Da

    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD r, XDWORD g, XDWORD b, XDWORD a, XDWORD cover)
                                                      {
                                                        double sr = double(r * cover) / (BASESCALE_MASK * 255);
                                                        double sg = double(g * cover) / (BASESCALE_MASK * 255);
                                                        double sb = double(b * cover) / (BASESCALE_MASK * 255);
                                                        double sa = double(a * cover) / (BASESCALE_MASK * 255);

                                                        if(sa > 0)
                                                          {
                                                            double dr = double(p[ORDER::R]) / BASESCALE_MASK;
                                                            double dg = double(p[ORDER::G]) / BASESCALE_MASK;
                                                            double db = double(p[ORDER::B]) / BASESCALE_MASK;
                                                            double da = double(p[ORDER::A] ? p[ORDER::A] : 1) / BASESCALE_MASK;

                                                            if(cover < 255)
                                                              {
                                                                a = (a * cover + 255) >> 8;
                                                              }

                                                            if(2*sr < sa) dr = dr*(sa + (1 - dr/da)*(2*sr - sa)) + sr*(1 - da) + dr*(1 - sa);
                                                              else if(8*dr <= da) dr = dr*(sa + (1 - dr/da)*(2*sr - sa)*(3 - 8*dr/da)) + sr*(1 - da) + dr*(1 - sa);
                                                                     else dr = (dr*sa + (sqrt(dr/da)*da - dr)*(2*sr - sa)) + sr*(1 - da) + dr*(1 - sa);

                                                            if(2*sg < sa) dg = dg*(sa + (1 - dg/da)*(2*sg - sa)) + sg*(1 - da) + dg*(1 - sa);
                                                              else if(8*dg <= da) dg = dg*(sa + (1 - dg/da)*(2*sg - sa)*(3 - 8*dg/da)) + sg*(1 - da) + dg*(1 - sa);
                                                                     else dg = (dg*sa + (sqrt(dg/da)*da - dg)*(2*sg - sa)) + sg*(1 - da) + dg*(1 - sa);

                                                              if(2*sb < sa) db = db*(sa + (1 - db/da)*(2*sb - sa)) + sb*(1 - da) + db*(1 - sa);
                                                                else if(8*db <= da) db = db*(sa + (1 - db/da)*(2*sb - sa)*(3 - 8*db/da)) + sb*(1 - da) + db*(1 - sa);
                                                                       else db = (db*sa + (sqrt(db/da)*da - db)*(2*sb - sa)) + sb*(1 - da) + db*(1 - sa);

                                                            p[ORDER::R] = (VALUETYPE)uround(dr * BASESCALE_MASK);
                                                            p[ORDER::G] = (VALUETYPE)uround(dg * BASESCALE_MASK);
                                                            p[ORDER::B] = (VALUETYPE)uround(db * BASESCALE_MASK);
                                                            p[ORDER::A] = (VALUETYPE)(a + p[ORDER::A] - ((a * p[ORDER::A] + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                          }
                                                      }
 };




template<class COLORT, class ORDER>
class COMPOPRGBA_DIFFERENCE
{
  public:

    typedef COLORT COLORTYPE;
    typedef ORDER ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE CALCTYPE;
    typedef typename COLORTYPE::LONGTYPE LONGTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_SCALE = COLORTYPE::BASESCALE_SCALE,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };


    // Dca' = Sca + Dca - 2.min(Sca.Da, Dca.Sa)
    // Da'  = Sa + Da - Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        if(sa)
                                                          {
                                                            CALCTYPE dr = p[ORDER::R];
                                                            CALCTYPE dg = p[ORDER::G];
                                                            CALCTYPE db = p[ORDER::B];
                                                            CALCTYPE da = p[ORDER::A];

                                                            p[ORDER::R] = (VALUETYPE)(sr + dr - ((2 * SD_MIN(sr*da, dr*sa) + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::G] = (VALUETYPE)(sg + dg - ((2 * SD_MIN(sg*da, dg*sa) + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::B] = (VALUETYPE)(sb + db - ((2 * SD_MIN(sb*da, db*sa) + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::A] = (VALUETYPE)(sa + da - ((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                          }
                                                      }
};




template<class COLORT, class ORDER>
class COMPOPRGBA_EXCLUSION
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;
    typedef typename COLORTYPE::LONGTYPE              LONGTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = (Sca.Da + Dca.Sa - 2.Sca.Dca) + Sca.(1 - Da) + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        if(sa)
                                                          {
                                                            CALCTYPE d1a = BASESCALE_MASK - p[ORDER::A];
                                                            CALCTYPE s1a = BASESCALE_MASK - sa;
                                                            CALCTYPE dr = p[ORDER::R];
                                                            CALCTYPE dg = p[ORDER::G];
                                                            CALCTYPE db = p[ORDER::B];
                                                            CALCTYPE da = p[ORDER::A];

                                                            p[ORDER::R] = (VALUETYPE)((sr*da + dr*sa - 2*sr*dr + sr*d1a + dr*s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::G] = (VALUETYPE)((sg*da + dg*sa - 2*sg*dg + sg*d1a + dg*s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::B] = (VALUETYPE)((sb*da + db*sa - 2*sb*db + sb*d1a + db*s1a + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                            p[ORDER::A] = (VALUETYPE)(sa + da - ((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                          }
                                                      }
};





template<class COLORT, class ORDER>
class COMPOPRGBA_CONTRAST
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;
    typedef typename COLORTYPE::LONGTYPE              LONGTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };


    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        LONGTYPE dr  = p[ORDER::R];
                                                        LONGTYPE dg  = p[ORDER::G];
                                                        LONGTYPE db  = p[ORDER::B];
                                                        int      da  = p[ORDER::A];
                                                        LONGTYPE d2a = da >> 1;
                                                        XDWORD   s2a = sa >> 1;

                                                        int r = (int)((((dr - d2a) * int((sr - s2a)*2 + BASESCALE_MASK)) >> BASESCALE_SHIFT) + d2a);
                                                        int g = (int)((((dg - d2a) * int((sg - s2a)*2 + BASESCALE_MASK)) >> BASESCALE_SHIFT) + d2a);
                                                        int b = (int)((((db - d2a) * int((sb - s2a)*2 + BASESCALE_MASK)) >> BASESCALE_SHIFT) + d2a);

                                                        r = (r < 0) ? 0 : r;
                                                        g = (g < 0) ? 0 : g;
                                                        b = (b < 0) ? 0 : b;

                                                        p[ORDER::R] = (VALUETYPE)((r > da) ? da : r);
                                                        p[ORDER::G] = (VALUETYPE)((g > da) ? da : g);
                                                        p[ORDER::B] = (VALUETYPE)((b > da) ? da : b);
                                                      }
};





template<class COLORT, class ORDER>
class COMPOPRGBA_INVERT
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;
    typedef typename COLORTYPE::LONGTYPE              LONGTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = (Da - Dca) * Sa + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        sa = (sa * cover + 255) >> 8;
                                                        if(sa)
                                                          {
                                                            CALCTYPE da = p[ORDER::A];
                                                            CALCTYPE dr = ((da - p[ORDER::R]) * sa + BASESCALE_MASK) >> BASESCALE_SHIFT;
                                                            CALCTYPE dg = ((da - p[ORDER::G]) * sa + BASESCALE_MASK) >> BASESCALE_SHIFT;
                                                            CALCTYPE db = ((da - p[ORDER::B]) * sa + BASESCALE_MASK) >> BASESCALE_SHIFT;
                                                            CALCTYPE s1a = BASESCALE_MASK - sa;

                                                            p[ORDER::R] = (VALUETYPE)(dr + ((p[ORDER::R] * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::G] = (VALUETYPE)(dg + ((p[ORDER::G] * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::B] = (VALUETYPE)(db + ((p[ORDER::B] * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::A] = (VALUETYPE)(sa + da - ((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                          }
                                                      }
};




template<class COLORT, class ORDER>
class COMPOP_RGBAINVERTRGB
{
  public:

    typedef COLORT                                    COLORTYPE;
    typedef ORDER                                     ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;
    typedef typename COLORTYPE::LONGTYPE              LONGTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    // Dca' = (Da - Dca) * Sca + Dca.(1 - Sa)
    // Da'  = Sa + Da - Sa.Da
    static void                                       PutBlendPixel                       (VALUETYPE* p, XDWORD sr, XDWORD sg, XDWORD sb, XDWORD sa, XDWORD cover)
                                                      {
                                                        if(cover < 255)
                                                          {
                                                            sr = (sr * cover + 255) >> 8;
                                                            sg = (sg * cover + 255) >> 8;
                                                            sb = (sb * cover + 255) >> 8;
                                                            sa = (sa * cover + 255) >> 8;
                                                          }

                                                        if(sa)
                                                          {
                                                            CALCTYPE da = p[ORDER::A];
                                                            CALCTYPE dr = ((da - p[ORDER::R]) * sr + BASESCALE_MASK) >> BASESCALE_SHIFT;
                                                            CALCTYPE dg = ((da - p[ORDER::G]) * sg + BASESCALE_MASK) >> BASESCALE_SHIFT;
                                                            CALCTYPE db = ((da - p[ORDER::B]) * sb + BASESCALE_MASK) >> BASESCALE_SHIFT;
                                                            CALCTYPE s1a = BASESCALE_MASK - sa;

                                                            p[ORDER::R] = (VALUETYPE)(dr + ((p[ORDER::R] * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::G] = (VALUETYPE)(dg + ((p[ORDER::G] * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::B] = (VALUETYPE)(db + ((p[ORDER::B] * s1a + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                            p[ORDER::A] = (VALUETYPE)(sa + da - ((sa * da + BASESCALE_MASK) >> BASESCALE_SHIFT));
                                                          }
                                                      }
};




template<class COLORT, class ORDER>
class COMPOP_TABLERGBA
{
  public:

    typedef typename COLORT::VALUETYPE                VALUETYPE;
    typedef void (*COMPOPFUNCTIONTYPE)                (VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD ca, XDWORD cover);

    static COMPOPFUNCTIONTYPE                         compopfunc[];
};




template<class COLORT, class ORDER>
typename COMPOP_TABLERGBA<COLORT, ORDER>::COMPOPFUNCTIONTYPE  COMPOP_TABLERGBA<COLORT, ORDER>::compopfunc[] = { COMPOPRGBA_CLEAR<COLORT,ORDER>::PutBlendPixel       ,
                                                                                                                COMPOPRGBA_SOURCE<COLORT,ORDER>::PutBlendPixel      ,
                                                                                                                COMPOPRGBA_TARGET<COLORT,ORDER>::PutBlendPixel      ,
                                                                                                                /*
                                                                                                                COMPOPRGBA_SOURCEOVER<COLORT,ORDER>::PutBlendPixel  ,
                                                                                                                COMPOPRGBA_TARGETOVER<COLORT,ORDER>::PutBlendPixel  ,
                                                                                                                COMPOPRGBA_SOURCEIN<COLORT,ORDER>::PutBlendPixel    ,
                                                                                                                COMPOPRGBA_TARGETIN<COLORT,ORDER>::PutBlendPixel    ,
                                                                                                                COMPOPRGBA_SOURCEOUT<COLORT,ORDER>::PutBlendPixel   ,
                                                                                                                COMPOPRGBA_TARGETOUT<COLORT,ORDER>::PutBlendPixel   ,
                                                                                                                COMPOPRGBA_SOURCEATOP<COLORT,ORDER>::PutBlendPixel  ,
                                                                                                                COMPOPRGBA_TARGETATOP<COLORT,ORDER>::PutBlendPixel  ,
                                                                                                                COMPOPRGBA_XOR<COLORT,ORDER>::PutBlendPixel         ,
                                                                                                                COMPOPRGBA_PLUS<COLORT,ORDER>::PutBlendPixel        ,
                                                                                                                COMPOPRGBA_MINUS<COLORT,ORDER>::PutBlendPixel       ,
                                                                                                                COMPOPRGBA_MULTIPLY<COLORT,ORDER>::PutBlendPixel    ,
                                                                                                                COMPOPRGBA_SCREEN<COLORT,ORDER>::PutBlendPixel      ,
                                                                                                                COMPOPRGBA_OVERLAY<COLORT,ORDER>::PutBlendPixel     ,
                                                                                                                COMPOPRGBA_DARKEN<COLORT,ORDER>::PutBlendPixel      ,
                                                                                                                COMPOPRGBA_LIGHTEN<COLORT,ORDER>::PutBlendPixel     ,
                                                                                                                COMPOPRGBA_COLORDODGE<COLORT,ORDER>::PutBlendPixel  ,
                                                                                                                COMPOPRGBA_COLORBURN<COLORT,ORDER>::PutBlendPixel   ,
                                                                                                                COMPOPRGBA_HARDLIGHT<COLORT,ORDER>::PutBlendPixel   ,
                                                                                                                COMPOPRGBA_SOFTLIGHT<COLORT,ORDER>::PutBlendPixel   ,
                                                                                                                COMPOPRGBA_DIFFERENCE<COLORT,ORDER>::PutBlendPixel  ,
                                                                                                                COMPOPRGBA_EXCLUSION<COLORT,ORDER>::PutBlendPixel   ,
                                                                                                                COMPOPRGBA_CONTRAST<COLORT,ORDER>::PutBlendPixel    ,
                                                                                                                COMPOPRGBA_INVERT<COLORT,ORDER>::PutBlendPixel      ,
                                                                                                                COMPOPRGBA_INVERTRGB<COLORT,ORDER>::PutBlendPixel   ,
                                                                                                                */
                                                                                                                NULL
                                                                                                            };



template<class COLORT, class ORDER>
class COMPOP_ADAPTORRGBA
{
  public:

    typedef ORDER                                     ORDERTYPE;
    typedef COLORT                                    COLORTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    static void                                       PutBlendPixel                       (XDWORD op, VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD ca, XDWORD cover)
                                                      {
                                                        COMPOP_TABLERGBA<COLORT, ORDER>::compopfunc[op](p, (cr * ca + BASESCALE_MASK) >> BASESCALE_SHIFT, (cg * ca + BASESCALE_MASK) >> BASESCALE_SHIFT, (cb * ca + BASESCALE_MASK) >> BASESCALE_SHIFT, ca, cover);
                                                      }
};




template<class COLORT, class ORDER>
class COMPOP_ADAPTORCLIPTOTARGETRGBA
{
  public:

    typedef ORDER                                     ORDERTYPE;
    typedef COLORT                                    COLORTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    static void                                       PutBlendPixel                       (XDWORD op, VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD ca, XDWORD cover)
                                                      {
                                                        cr = (cr * ca + BASESCALE_MASK) >> BASESCALE_SHIFT;
                                                        cg = (cg * ca + BASESCALE_MASK) >> BASESCALE_SHIFT;
                                                        cb = (cb * ca + BASESCALE_MASK) >> BASESCALE_SHIFT;

                                                        XDWORD da = p[ORDER::A];

                                                        COMPOP_TABLERGBA<COLORT, ORDER>::compopfunc[op](p, (cr * da + BASESCALE_MASK) >> BASESCALE_SHIFT, (cg * da + BASESCALE_MASK) >> BASESCALE_SHIFT, (cb * da + BASESCALE_MASK) >> BASESCALE_SHIFT, (ca * da + BASESCALE_MASK) >> BASESCALE_SHIFT, cover);
                                                      }
    };




template<class COLORT, class ORDER>
class COMPOP_ADAPTORRGBAPRE
{
  public:

    typedef ORDER                                     ORDERTYPE;
    typedef COLORT                                    COLORTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    static void                                       PutBlendPixel                       (XDWORD op, VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD ca, XDWORD cover)
                                                      {
                                                        COMPOP_TABLERGBA<COLORT, ORDER>::compopfunc[op](p, cr, cg, cb, ca, cover);
                                                      }
};




template<class COLORT, class ORDER>
class COMPOP_ADAPTORCLIPTOTARGETRGBAPRE
{
  public:

    typedef ORDER                                     ORDERTYPE;
    typedef COLORT                                    COLORTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    static void                                       PutBlendPixel                       (XDWORD op, VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD ca, XDWORD cover)
                                                      {
                                                        XDWORD da = p[ORDER::A];
                                                        COMPOP_TABLERGBA<COLORT, ORDER>::compopfunc[op](p, (cr * da + BASESCALE_MASK) >> BASESCALE_SHIFT, (cg * da + BASESCALE_MASK) >> BASESCALE_SHIFT, (cb * da + BASESCALE_MASK) >> BASESCALE_SHIFT, (ca * da + BASESCALE_MASK) >> BASESCALE_SHIFT, cover);
                                                      }
};





template<class BLENDERPRE>
class COMP_ADAPTORRGBA
{
  public:

    typedef typename BLENDERPRE::ORDERTYPE            ORDERTYPE;
    typedef typename BLENDERPRE::COLORTYPE            COLORTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    static void                                       PutBlendPixel                       (XDWORD op, VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD ca, XDWORD cover)
                                                      {
                                                        BLENDERPRE::PutBlendPixel(p, (cr * ca + BASESCALE_MASK) >> BASESCALE_SHIFT, (cg * ca + BASESCALE_MASK) >> BASESCALE_SHIFT, (cb * ca + BASESCALE_MASK) >> BASESCALE_SHIFT, ca, cover);
                                                      }
};




template<class BLENDERPRE>
class COMP_ADAPTORCLIPTOTARGETRGBA
{
  public:

    typedef typename BLENDERPRE::ORDERTYPE            ORDERTYPE;
    typedef typename BLENDERPRE::COLORTYPE            COLORTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    static void                                       PutBlendPixel                       (XDWORD op, VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD ca, XDWORD cover)
                                                      {
                                                        cr = (cr * ca + BASESCALE_MASK) >> BASESCALE_SHIFT;
                                                        cg = (cg * ca + BASESCALE_MASK) >> BASESCALE_SHIFT;
                                                        cb = (cb * ca + BASESCALE_MASK) >> BASESCALE_SHIFT;

                                                        XDWORD da = p[ORDERTYPE::A];

                                                        BLENDERPRE::PutBlendPixel(p, (cr * da + BASESCALE_MASK) >> BASESCALE_SHIFT, (cg * da + BASESCALE_MASK) >> BASESCALE_SHIFT, (cb * da + BASESCALE_MASK) >> BASESCALE_SHIFT, (ca * da + BASESCALE_MASK) >> BASESCALE_SHIFT, cover);
                                                      }
};




template<class BLENDERPRE>
class COMP_ADAPTORCLIPTOTARGETRGBAPRE
{
  public:

    typedef typename BLENDERPRE::ORDERTYPE            ORDERTYPE;
    typedef typename BLENDERPRE::COLORTYPE            COLORTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    static void                                       PutBlendPixel                       (XDWORD op, VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD ca, XDWORD cover)
                                                      {
                                                        XDWORD da = p[ORDERTYPE::A];
                                                        BLENDERPRE::PutBlendPixel(p, (cr * da + BASESCALE_MASK) >> BASESCALE_SHIFT, (cg * da + BASESCALE_MASK) >> BASESCALE_SHIFT, (cb * da + BASESCALE_MASK) >> BASESCALE_SHIFT, (ca * da + BASESCALE_MASK) >> BASESCALE_SHIFT, cover);
                                                      }
};




template<class BLENDER>
class PUTORBLENDRGBAWRAPPER
{
  public:

    typedef typename BLENDER::COLORTYPE               COLORTYPE;
    typedef typename BLENDER::ORDERTYPE               ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_SCALE = COLORTYPE::BASESCALE_SCALE,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK
    };

    static void                                       PutOrBlendPixel                     (VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha)
                                                      {
                                                        if(alpha)
                                                          {
                                                            if(alpha == BASESCALE_MASK)
                                                              {
                                                                p[ORDERTYPE::R] = cr;
                                                                p[ORDERTYPE::G] = cg;
                                                                p[ORDERTYPE::B] = cb;
                                                                p[ORDERTYPE::A] = BASESCALE_MASK;
                                                              }
                                                             else
                                                              {
                                                                BLENDER::PutBlendPixel(p, cr, cg, cb, alpha);
                                                              }
                                                          }
                                                      }


    static void                                       PutOrBlendPixel                     (VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD cover)
                                                      {
                                                        if(cover == 255)
                                                          {
                                                            PutOrBlendPixel(p, cr, cg, cb, alpha);
                                                          }
                                                         else
                                                          {
                                                            if(alpha)
                                                              {
                                                                alpha = (alpha * (cover + 1)) >> 8;

                                                                if(alpha == BASESCALE_MASK)
                                                                  {
                                                                    p[ORDERTYPE::R] = cr;
                                                                    p[ORDERTYPE::G] = cg;
                                                                    p[ORDERTYPE::B] = cb;
                                                                    p[ORDERTYPE::A] = BASESCALE_MASK;
                                                                  }
                                                                 else
                                                                  {
                                                                    BLENDER::PutBlendPixel(p, cr, cg, cb, alpha, cover);
                                                                  }
                                                              }
                                                          }
                                                      }
};





template<class BLENDER, class RENDERERBUFFER, class PIXELT = XDWORD>
class GRPBUFFERPIXELFORMATALPHABLENDRGBA
{
  public:

    typedef RENDERERBUFFER                            RENDERERBUFFERTYPE;
    typedef typename RENDERERBUFFERTYPE::ROWDATA      ROWDATA;
    typedef PIXELT                                    PIXELTYPE;
    typedef BLENDER                                   BLENDERTYPE;
    typedef typename BLENDERTYPE::COLORTYPE           COLORTYPE;
    typedef typename BLENDERTYPE::ORDERTYPE           ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;
    typedef PUTORBLENDRGBAWRAPPER<BLENDERTYPE>        PUTORBLENDTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_SCALE = COLORTYPE::BASESCALE_SCALE,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK,
      PIXELWIDTH      = sizeof(PIXELTYPE)
    };

                                                      GRPBUFFERPIXELFORMATALPHABLENDRGBA  ()
                                                      {
                                                        rendererbuffer = NULL;
                                                      }


                                                      GRPBUFFERPIXELFORMATALPHABLENDRGBA  (RENDERERBUFFERTYPE& rb)
                                                      {
                                                        rendererbuffer = &rb;
                                                      }


    void                                              Set                                 (RENDERERBUFFERTYPE& rb)
                                                      {
                                                        rendererbuffer = &rb;
                                                      }


    template<class BUFFERPIXELFORMAT>
    bool                                              Set                                 (BUFFERPIXELFORMAT& pixelformatbuffer, int x1, int y1, int x2, int y2)
                                                      {
                                                        GRPRECTINT r(x1, y1, x2, y2);

                                                        if(r.Clip(GRPRECTINT(0, 0, pixelformatbuffer.GetWidth()-1, pixelformatbuffer.GetHeight()-1)))
                                                          {
                                                            int stride = pixelformatbuffer.GetStride();

                                                            rendererbuffer->Set(pixelformatbuffer.pix_ptr(r.x1, stride < 0 ? r.y2 : r.y1), (r.x2 - r.x1) + 1, (r.y2 - r.y1) + 1, stride);

                                                            return true;
                                                          }

                                                        return false;
                                                      }


    XDWORD                                            GetWidth                            () const                    { return rendererbuffer->GetWidth();                }
    XDWORD                                            GetHeight                           () const                    { return rendererbuffer->GetHeight();               }
    int                                               GetStride                           () const                    { return rendererbuffer->GetStride();               }

    XBYTE*                                            Row                                 (int y)                     { return rendererbuffer->Row(y);                    }
    const XBYTE*                                      Row                                 (int y) const               { return rendererbuffer->Row(y);                    }
    ROWDATA                                           RowData                             (int y) const               { return rendererbuffer->RowData(y);                }

    XBYTE*                                            GetPtr                              (int x, int y)              { return rendererbuffer->Row(y) + x * PIXELWIDTH;   }
    const XBYTE*                                      GetPtr                              (int x, int y) const        { return rendererbuffer->Row(y) + x * PIXELWIDTH;   }

    COLORTYPE                                         GetPixel                            (int x, int y) const
                                                      {
                                                        const VALUETYPE* p = (const VALUETYPE*)rendererbuffer->Row(y);
                                                        if(p)
                                                          {
                                                            p += x << 2;
                                                            return COLORTYPE(p[ORDERTYPE::R], p[ORDERTYPE::G], p[ORDERTYPE::B], p[ORDERTYPE::A]);
                                                          }

                                                        return COLORTYPE::GetNoColor();
                                                      }


    static void                                       PutPixel                            (XBYTE* p, const COLORTYPE& c)
                                                      {
                                                        ((VALUETYPE*)p)[ORDERTYPE::R] = c.r;
                                                        ((VALUETYPE*)p)[ORDERTYPE::G] = c.g;
                                                        ((VALUETYPE*)p)[ORDERTYPE::B] = c.b;
                                                        ((VALUETYPE*)p)[ORDERTYPE::A] = c.a;
                                                      }


    void                                              PutPixel                            (int x, int y, const COLORTYPE& c)
                                                      {
                                                        VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, 1) + (x << 2);

                                                        //XDEBUG_PRINTCOLOR(4, __L("R:%d G:%d B:%d"), c.r, c.g, c.b);

                                                        p[ORDERTYPE::R] = c.r;
                                                        p[ORDERTYPE::G] = c.g;
                                                        p[ORDERTYPE::B] = c.b;
                                                        p[ORDERTYPE::A] = c.a;
                                                      }


    void                                              PutBlendPixel                       (int x, int y, const COLORTYPE& c, XBYTE cover)
                                                      {
                                                        PUTORBLENDTYPE::PutOrBlendPixel((VALUETYPE*)rendererbuffer->Row(x, y, 1) + (x << 2), c.r, c.g, c.b, c.a, cover);
                                                      }


    void                                              PutHLine(int x, int y, XDWORD size, const COLORTYPE& c)
                                                      {
                                                        VALUETYPE*  p = (VALUETYPE*)rendererbuffer->Row(x, y, size) + (x << 2);
                                                        PIXELTYPE   v;

                                                        ((VALUETYPE*)&v)[ORDERTYPE::R] = c.r;
                                                        ((VALUETYPE*)&v)[ORDERTYPE::G] = c.g;
                                                        ((VALUETYPE*)&v)[ORDERTYPE::B] = c.b;
                                                        ((VALUETYPE*)&v)[ORDERTYPE::A] = c.a;

                                                        do{ *(PIXELTYPE*)p = v;
                                                            p += 4;

                                                          } while(--size);
                                                      }



    void                                              PutVLine                            (int x, int y, XDWORD size, const COLORTYPE& c)
                                                      {
                                                        PIXELTYPE v;

                                                        ((VALUETYPE*)&v)[ORDERTYPE::R] = c.r;
                                                        ((VALUETYPE*)&v)[ORDERTYPE::G] = c.g;
                                                        ((VALUETYPE*)&v)[ORDERTYPE::B] = c.b;
                                                        ((VALUETYPE*)&v)[ORDERTYPE::A] = c.a;

                                                        do{ VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + (x << 2);
                                                            *(PIXELTYPE*)p = v;

                                                          } while(--size);
                                                      }


    void                                              PutBlendHLine                       (int x, int y, XDWORD size, const COLORTYPE& c, XBYTE cover)
                                                      {
                                                        if(c.a)
                                                          {
                                                            VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, size) + (x << 2);
                                                            CALCTYPE alpha = (CALCTYPE(c.a) * (cover + 1)) >> 8;

                                                            if(alpha == BASESCALE_MASK)
                                                              {
                                                                PIXELTYPE v;

                                                                ((VALUETYPE*)&v)[ORDERTYPE::R] = c.r;
                                                                ((VALUETYPE*)&v)[ORDERTYPE::G] = c.g;
                                                                ((VALUETYPE*)&v)[ORDERTYPE::B] = c.b;
                                                                ((VALUETYPE*)&v)[ORDERTYPE::A] = c.a;

                                                                do{ *(PIXELTYPE*)p = v;
                                                                    p += 4;

                                                                  } while(--size);
                                                              }
                                                             else
                                                              {
                                                                if(cover == 255)
                                                                  {
                                                                    do{ BLENDERTYPE::PutBlendPixel(p, c.r, c.g, c.b, alpha);
                                                                        p += 4;

                                                                      } while(--size);
                                                                  }
                                                                 else
                                                                  {
                                                                    do{ BLENDERTYPE::PutBlendPixel(p, c.r, c.g, c.b, alpha, cover);
                                                                        p += 4;

                                                                      } while(--size);
                                                                  }
                                                              }
                                                          }
                                                      }


    void                                              PutBlendVLine                       (int x, int y, XDWORD size, const COLORTYPE& c, XBYTE cover)
                                                      {
                                                        if(c.a)
                                                          {
                                                            VALUETYPE* p;
                                                            CALCTYPE alpha = (CALCTYPE(c.a) * (cover + 1)) >> 8;

                                                            if(alpha == BASESCALE_MASK)
                                                              {
                                                                PIXELTYPE v;

                                                                ((VALUETYPE*)&v)[ORDERTYPE::R] = c.r;
                                                                ((VALUETYPE*)&v)[ORDERTYPE::G] = c.g;
                                                                ((VALUETYPE*)&v)[ORDERTYPE::B] = c.b;
                                                                ((VALUETYPE*)&v)[ORDERTYPE::A] = c.a;

                                                                do{ p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + (x << 2);
                                                                    *(PIXELTYPE*)p = v;

                                                                  } while(--size);
                                                              }
                                                             else
                                                              {
                                                                if(cover == 255)
                                                                  {
                                                                    do{ p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + (x << 2);
                                                                        BLENDERTYPE::PutBlendPixel(p, c.r, c.g, c.b, alpha);

                                                                      } while(--size);
                                                                  }
                                                                 else
                                                                  {
                                                                    do{ p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + (x << 2);
                                                                        BLENDERTYPE::PutBlendPixel(p, c.r, c.g, c.b, alpha, cover);

                                                                      } while(--size);
                                                                  }
                                                              }
                                                          }
                                                      }


    void                                              PutBlendSolidHSpan                  (int x, int y, XDWORD size, const COLORTYPE& c, const XBYTE* covers)
                                                      {
                                                        if(c.a)
                                                          {
                                                            VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, size) + (x << 2);

                                                            do{ CALCTYPE alpha = (CALCTYPE(c.a) * (CALCTYPE(*covers) + 1)) >> 8;
                                                                if(alpha == BASESCALE_MASK)
                                                                  {
                                                                    p[ORDERTYPE::R] = c.r;
                                                                    p[ORDERTYPE::G] = c.g;
                                                                    p[ORDERTYPE::B] = c.b;
                                                                    p[ORDERTYPE::A] = BASESCALE_MASK;
                                                                  }
                                                                 else
                                                                  {
                                                                    BLENDERTYPE::PutBlendPixel(p, c.r, c.g, c.b, alpha, *covers);
                                                                  }

                                                                p += 4;
                                                                ++covers;

                                                              } while(--size);
                                                          }
                                                      }


    void                                              PutBlendSolidVSpan                  (int x, int y,  XDWORD size, const COLORTYPE& c, const XBYTE* covers)
                                                      {
                                                        if(c.a)
                                                          {
                                                            do{ VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + (x << 2);
                                                                CALCTYPE alpha = (CALCTYPE(c.a) * (CALCTYPE(*covers) + 1)) >> 8;

                                                                if(alpha == BASESCALE_MASK)
                                                                  {
                                                                    p[ORDERTYPE::R] = c.r;
                                                                    p[ORDERTYPE::G] = c.g;
                                                                    p[ORDERTYPE::B] = c.b;
                                                                    p[ORDERTYPE::A] = BASESCALE_MASK;
                                                                  }
                                                                 else
                                                                  {
                                                                    BLENDERTYPE::PutBlendPixel(p, c.r, c.g, c.b, alpha, *covers);
                                                                  }

                                                                ++covers;

                                                              } while(--size);
                                                          }
                                                      }


    void                                              PutColorHSpan                       (int x, int y, XDWORD size, const COLORTYPE* colors)
                                                      {
                                                        VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, size) + (x << 2);
                                                        do{
                                                            p[ORDERTYPE::R] = colors->r;
                                                            p[ORDERTYPE::G] = colors->g;
                                                            p[ORDERTYPE::B] = colors->b;
                                                            p[ORDERTYPE::A] = colors->a;

                                                            ++colors;
                                                            p += 4;

                                                          } while(--size);
                                                      }


    void                                              PutColorVSpan                       (int x, int y, XDWORD size, const COLORTYPE* colors)
                                                      {
                                                        do{ VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + (x << 2);

                                                            p[ORDERTYPE::R] = colors->r;
                                                            p[ORDERTYPE::G] = colors->g;
                                                            p[ORDERTYPE::B] = colors->b;
                                                            p[ORDERTYPE::A] = colors->a;

                                                            ++colors;
                                                          } while(--size);
                                                      }


    void                                              PutBlendColorHSpan                  (int x, int y, XDWORD size, const COLORTYPE* colors, const XBYTE* covers, XBYTE cover)
                                                      {
                                                        VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, size) + (x << 2);

                                                        if(covers)
                                                          {
                                                            do{ PUTORBLENDTYPE::copy_or_PutBlendPixel(p, colors->r, colors->g, colors->b, colors->a, *covers++);
                                                                p += 4;
                                                                ++colors;

                                                              } while(--size);
                                                          }
                                                         else
                                                          {
                                                            if(cover == 255)
                                                              {
                                                                do{ PUTORBLENDTYPE::copy_or_PutBlendPixel(p, colors->r, colors->g, colors->b, colors->a);
                                                                    p += 4;
                                                                    ++colors;

                                                                  } while(--size);
                                                              }
                                                             else
                                                              {
                                                                do{ PUTORBLENDTYPE::copy_or_PutBlendPixel(p, colors->r, colors->g, colors->b, colors->a, cover);
                                                                    p += 4;
                                                                    ++colors;

                                                                  } while(--size);
                                                              }
                                                          }
                                                      }


    void                                              PutBlendColorVSpan                  (int x, int y,  XDWORD size, const COLORTYPE* colors, const XBYTE* covers,  XBYTE cover)
                                                      {
                                                        VALUETYPE* p;
                                                        if(covers)
                                                          {
                                                            do{ p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + (x << 2);
                                                                PUTORBLENDTYPE::copy_or_PutBlendPixel(p, colors->r, colors->g, colors->b, colors->a, *covers++);
                                                                ++colors;

                                                              } while(--size);
                                                          }
                                                         else
                                                          {
                                                            if(cover == 255)
                                                              {
                                                                do{ p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + (x << 2);
                                                                    PUTORBLENDTYPE::copy_or_PutBlendPixel(p, colors->r, colors->g, colors->b, colors->a);
                                                                    ++colors;

                                                                  } while(--size);
                                                              }
                                                             else
                                                              {
                                                                do{ p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + (x << 2);
                                                                    PUTORBLENDTYPE::copy_or_PutBlendPixel(p, colors->r, colors->g, colors->b, colors->a, cover);
                                                                    ++colors;

                                                                  } while(--size);
                                                              }
                                                          }
                                                      }


    template<class FUNCTION>
    void                                              ForEachPixel                        (FUNCTION f)
                                                      {
                                                        XDWORD y;
                                                        for(y = 0; y < GetHeight(); ++y)
                                                          {
                                                            ROWDATA r = rendererbuffer->Row(y);
                                                            if(r.ptr)
                                                              {
                                                                XDWORD size = r.x2 - r.x1 + 1;
                                                                VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(r.x1, y, size) + (r.x1 << 2);
                                                                do{ f(p);
                                                                    p += 4;
                                                                  } while(--size);
                                                              }
                                                          }
                                                      }


    void                                              Premultiply                         ()
                                                      {
                                                        ForEachPixel(MULTIPLIERRGBA<COLORTYPE, ORDERTYPE>::Premultiply);
                                                      }



    void                                              Demultiply                          ()
                                                      {
                                                        ForEachPixel(MULTIPLIERRGBA<COLORTYPE, ORDERTYPE>::Demultiply);
                                                      }


    template<class GAMMALUT>
    void                                              ApplyGammaDir                       (const GAMMALUT& g)
                                                      {
                                                        ForEachPixel(APPLYGAMMADIRRGBA<COLORTYPE, ORDERTYPE, GAMMALUT>(g));
                                                      }


    template<class GAMMALUT>
    void                                              ApplyGammaInv                       (const GAMMALUT& g)
                                                      {
                                                        ForEachPixel(APPLYGAMMAINVRGBA<COLORTYPE, ORDERTYPE, GAMMALUT>(g));
                                                      }


    template<class RENDERERBUFFER2>
    void                                              CopyFrom                            (const RENDERERBUFFER2& from, int xdst, int ydst, int xsrc, int ysrc, XDWORD size)
                                                      {
                                                        const XBYTE* p = from.Row(ysrc);
                                                        if(p) memmove(rendererbuffer->Row(xdst, ydst, size) + xdst * PIXELWIDTH, p + xsrc * PIXELWIDTH, size * PIXELWIDTH);
                                                      }


    template<class SOURCEBUFFERPIXELFORMAT>
    void                                              PutBlendFrom                        (const SOURCEBUFFERPIXELFORMAT& from, int xdst, int ydst, int xsrc, int ysrc, XDWORD size, XBYTE cover)
                                                      {
                                                        typedef typename SOURCEBUFFERPIXELFORMAT::ORDERTYPE SOURCEORDER;

                                                        const VALUETYPE* psrc = (VALUETYPE*)from.Row(ysrc);
                                                        if(psrc)
                                                          {
                                                            psrc += xsrc << 2;
                                                            VALUETYPE* pdst = (VALUETYPE*)rendererbuffer->Row(xdst, ydst, size) + (xdst << 2);
                                                            int incp = 4;

                                                            if(xdst > xsrc)
                                                              {
                                                                psrc += (size-1) << 2;
                                                                pdst += (size-1) << 2;
                                                                incp = -4;
                                                              }

                                                            if(cover == 255)
                                                              {
                                                                do{ PUTORBLENDTYPE::copy_or_PutBlendPixel(pdst, psrc[SOURCEORDER::R], psrc[SOURCEORDER::G], psrc[SOURCEORDER::B], psrc[SOURCEORDER::A]);
                                                                    psrc += incp;
                                                                    pdst += incp;

                                                                  } while(--size);
                                                              }
                                                             else
                                                              {
                                                                do{ PUTORBLENDTYPE::copy_or_PutBlendPixel(pdst, psrc[SOURCEORDER::R], psrc[SOURCEORDER::G], psrc[SOURCEORDER::B], psrc[SOURCEORDER::A], cover);
                                                                    psrc += incp;
                                                                    pdst += incp;

                                                                  } while(--size);
                                                              }
                                                          }
                                                      }


    template<class SOURCEBUFFERPIXELFORMAT>
    void                                              PutBlendFromCcolor                  (const SOURCEBUFFERPIXELFORMAT& from, const COLORTYPE& color, int xdst, int ydst, int xsrc, int ysrc, XDWORD size, XBYTE cover)
                                                      {
                                                        typedef typename SOURCEBUFFERPIXELFORMAT::VALUETYPE SOURCEVALUETYPE;

                                                        const SOURCEVALUETYPE* psrc = (SOURCEVALUETYPE*)from.Row(ysrc);
                                                        if(psrc)
                                                          {
                                                            VALUETYPE* pdst = (VALUETYPE*)rendererbuffer->Row(xdst, ydst, size) + (xdst << 2);

                                                            do{ PUTORBLENDTYPE::copy_or_PutBlendPixel(pdst, color.r, color.g, color.b, color.a, (*psrc * cover + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                                ++psrc;
                                                                pdst += 4;

                                                              } while(--size);
                                                          }
                                                      }


    template<class SOURCEBUFFERPIXELFORMAT>
    void                                              PutBlendFromLut                     (const SOURCEBUFFERPIXELFORMAT& from, const COLORTYPE* colorlut,  int xdst, int ydst, int xsrc, int ysrc, XDWORD size, XBYTE cover)
                                                      {
                                                        typedef typename SOURCEBUFFERPIXELFORMAT::VALUETYPE SOURCEVALUETYPE;

                                                        const SOURCEVALUETYPE* psrc = (SOURCEVALUETYPE*)from.Row(ysrc);
                                                        if(psrc)
                                                          {
                                                            VALUETYPE* pdst = (VALUETYPE*)rendererbuffer->Row(xdst, ydst, size) + (xdst << 2);

                                                            if(cover == 255)
                                                              {
                                                                do{ const COLORTYPE& color = colorlut[*psrc];
                                                                    PUTORBLENDTYPE::copy_or_PutBlendPixel(pdst, color.r, color.g, color.b, color.a);
                                                                    ++psrc;
                                                                    pdst += 4;

                                                                  } while(--size);
                                                              }
                                                             else
                                                              {
                                                                do{ const COLORTYPE& color = colorlut[*psrc];
                                                                    PUTORBLENDTYPE::copy_or_PutBlendPixel(pdst, color.r, color.g, color.b, color.a, cover);
                                                                    ++psrc;
                                                                    pdst += 4;

                                                                  } while(--size);
                                                              }
                                                          }
                                                      }

  private:

    RENDERERBUFFERTYPE*                               rendererbuffer;

};





template<class BLENDER, class RENDERERBUFFER>
class GRPBUFFERPIXELFORMATCUSTOMBLENDRGBA
{
  public:

    typedef RENDERERBUFFER                            RENDERERBUFFERTYPE;
    typedef typename RENDERERBUFFERTYPE::ROWDATA      ROWDATA;
    typedef BLENDER                                   BLENDERTYPE;
    typedef typename BLENDERTYPE::COLORTYPE           COLORTYPE;
    typedef typename BLENDERTYPE::ORDERTYPE           ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE             VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE              CALCTYPE;

    enum BASESCALE
    {
      BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT,
      BASESCALE_SCALE = COLORTYPE::BASESCALE_SCALE,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK,
      PIXELWIDTH      = sizeof(VALUETYPE) * 4
    };

                                                      GRPBUFFERPIXELFORMATCUSTOMBLENDRGBA()
                                                      {
                                                        rendererbuffer = NULL;
                                                        comp_op        = 3;
                                                      }


                                                      GRPBUFFERPIXELFORMATCUSTOMBLENDRGBA(RENDERERBUFFERTYPE& rb, XDWORD comp_op=3)
                                                      {
                                                        rendererbuffer = &rb;
                                                        this->comp_op  = comp_op;
                                                      }

    void                                              Set                                 (RENDERERBUFFERTYPE& rb)
                                                      {
                                                        rendererbuffer = &rb;
                                                      }


    template<class BUFFERPIXELFORMAT>
    bool                                              Set                                 (BUFFERPIXELFORMAT& pixelformatbuffer, int x1, int y1, int x2, int y2)
                                                      {
                                                        GRPRECTINT r(x1, y1, x2, y2);

                                                        if(r.Clip(GRPRECTINT(0, 0, pixelformatbuffer.width()-1, pixelformatbuffer.height()-1)))
                                                          {
                                                            int stride = pixelformatbuffer.GetStride();

                                                            rendererbuffer->Set(pixelformatbuffer.pix_ptr(r.x1, stride < 0 ? r.y2 : r.y1), (r.x2 - r.x1) + 1, (r.y2 - r.y1) + 1, stride);
                                                            return true;
                                                          }

                                                        return false;
                                                      }


    XDWORD                                            GetWidth                            () const                      { return rendererbuffer->GetWidth();                }
    XDWORD                                            GetHeight                           () const                      { return rendererbuffer->GetHeight();               }
    int                                               GetStride                           () const                      { return rendererbuffer->GetStride();               }

    XBYTE*                                            Row                                 (int y)                       { return rendererbuffer->Row(y);                    }
    const XBYTE*                                      Row                                 (int y) const                 { return rendererbuffer->Row(y);                    }
    ROWDATA                                           RowData                             (int y) const                 { return rendererbuffer->RowData(y);                }

    XBYTE*                                            GetPtr                              (int x, int y)                { return rendererbuffer->Row(y) + x * PIXELWIDTH;   }
    const XBYTE*                                      GetPtr                              (int x, int y) const          { return rendererbuffer->Row(y) + x * PIXELWIDTH;   }

    XDWORD                                            GetComp_Op                          () const                      { return comp_op;   }
    void                                              SetComp_Op                          (XDWORD op)                   { comp_op = op;     }


    COLORTYPE                                         GetPixel                            (int x, int y) const
                                                      {
                                                        const VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(y) + (x << 2);
                                                        return COLORTYPE(p[ORDERTYPE::R], p[ORDERTYPE::G], p[ORDERTYPE::B], p[ORDERTYPE::A]);
                                                      }

    static void                                       PutPixel                            (XBYTE* p, const COLORTYPE& c)
                                                      {
                                                        ((VALUETYPE*)p)[ORDERTYPE::R] = c.r;
                                                        ((VALUETYPE*)p)[ORDERTYPE::G] = c.g;
                                                        ((VALUETYPE*)p)[ORDERTYPE::B] = c.b;
                                                        ((VALUETYPE*)p)[ORDERTYPE::A] = c.a;
                                                      }

    void                                              PutPixel                            (int x, int y, const COLORTYPE& c)
                                                      {
                                                        BLENDERTYPE::PutBlendPixel(comp_op, (VALUETYPE*)rendererbuffer->Row(x, y, 1) + (x << 2), c.r, c.g, c.b, c.a, 255);
                                                      }


    void                                              PutBlendPixel                       (int x, int y, const COLORTYPE& c, XBYTE cover)
                                                      {
                                                        BLENDERTYPE::PutBlendPixel(comp_op, (VALUETYPE*)rendererbuffer->Row(x, y, 1) + (x << 2), c.r, c.g, c.b, c.a, cover);
                                                      }


    void                                              PutHLine                            (int x, int y, XDWORD size, const COLORTYPE& c)
                                                      {
                                                        VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, size) + (x << 2);

                                                        do{ BLENDERTYPE::PutBlendPixel(comp_op, p, c.r, c.g, c.b, c.a, 255);
                                                            p += 4;

                                                          } while(--size);
                                                      }


    void                                              PutVLine                            (int x, int y, XDWORD size, const COLORTYPE& c)
                                                      {
                                                        do{ BLENDERTYPE::PutBlendPixel(comp_op, (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + (x << 2), c.r, c.g, c.b, c.a, 255);

                                                          } while(--size);
                                                      }

    void                                              PutBlendHLine                       (int x, int y, XDWORD size, const COLORTYPE& c, XBYTE cover)
                                                      {
                                                        VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, size) + (x << 2);

                                                        do{ BLENDERTYPE::PutBlendPixel(comp_op, p, c.r, c.g, c.b, c.a, cover);
                                                            p += 4;

                                                          } while(--size);
                                                      }


    void                                              PutBlendVLine                       (int x, int y, XDWORD size, const COLORTYPE& c, XBYTE cover)
                                                      {
                                                        do{ BLENDERTYPE::PutBlendPixel(comp_op, (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + (x << 2), c.r, c.g, c.b, c.a, cover);

                                                          } while(--size);
                                                      }


    void                                              PutBlendSolidHSpan                  (int x, int y, XDWORD size, const COLORTYPE& c, const XBYTE* covers)
                                                      {
                                                        VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, size) + (x << 2);

                                                        do{ BLENDERTYPE::PutBlendPixel(comp_op, p, c.r, c.g, c.b, c.a, *covers++);
                                                            p += 4;

                                                          } while(--size);
                                                      }


    void                                              PutBlendSolidVSpan                  (int x, int y, XDWORD size, const COLORTYPE& c, const XBYTE* covers)
                                                      {
                                                        do{ BLENDERTYPE::PutBlendPixel(comp_op, (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + (x << 2), c.r, c.g, c.b, c.a, *covers++);

                                                          } while(--size);
                                                      }


    void                                              PutColorHSpan                       (int x, int y, XDWORD size, const COLORTYPE* colors)
                                                      {
                                                        VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, size) + (x << 2);

                                                        do{ p[ORDERTYPE::R] = colors->r;
                                                            p[ORDERTYPE::G] = colors->g;
                                                            p[ORDERTYPE::B] = colors->b;
                                                            p[ORDERTYPE::A] = colors->a;
                                                            ++colors;
                                                            p += 4;

                                                          } while(--size);
                                                      }

    void                                              PutColorVSpan                       (int x, int y, XDWORD size, const COLORTYPE* colors)
                                                      {
                                                        do{ VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + (x << 2);

                                                            p[ORDERTYPE::R] = colors->r;
                                                            p[ORDERTYPE::G] = colors->g;
                                                            p[ORDERTYPE::B] = colors->b;
                                                            p[ORDERTYPE::A] = colors->a;
                                                            ++colors;

                                                          } while(--size);
                                                      }


    void                                              PutBlendColorHSpan                  (int x, int y, XDWORD size, const COLORTYPE* colors, const XBYTE* covers, XBYTE cover)
                                                      {
                                                        VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, size) + (x << 2);

                                                        do{ BLENDERTYPE::PutBlendPixel(comp_op, p, colors->r, colors->g, colors->b, colors->a, covers ? *covers++ : cover);
                                                            p += 4;
                                                            ++colors;

                                                          } while(--size);
                                                      }


    void                                              PutBlendColorVSpan                  (int x, int y, XDWORD size, const COLORTYPE* colors, const XBYTE* covers, XBYTE cover)
                                                      {
                                                        do{ BLENDERTYPE::PutBlendPixel( comp_op, (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + (x << 2), colors->r, colors->g, colors->b, colors->a, covers ? *covers++ : cover);
                                                            ++colors;

                                                          } while(--size);
                                                      }


    template<class FUNCTION>
    void                                              ForEachPixel                        (FUNCTION f)
                                                      {
                                                        XDWORD y;
                                                        for(y = 0; y < GetHeight(); ++y)
                                                          {
                                                            ROWDATA r = rendererbuffer->row(y);
                                                            if(r.ptr)
                                                              {
                                                                XDWORD size = r.x2 - r.x1 + 1;
                                                                VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(r.x1, y, size) + (r.x1 << 2);

                                                                do{ f(p);
                                                                    p += 4;

                                                                  } while(--size);
                                                              }
                                                          }
                                                      }


    void                                              Premultiply                         ()
                                                      {
                                                        ForEachPixel(MULTIPLIERRGBA<COLORTYPE, ORDERTYPE>::Premultiply);
                                                      }


    void                                              Demultiply                          ()
                                                      {
                                                        ForEachPixel(MULTIPLIERRGBA<COLORTYPE, ORDERTYPE>::Demultiply);
                                                      }


    template<class GAMMALUT>
    void                                              ApplyGammaDir                       (const GAMMALUT& g)
                                                      {
                                                        ForEachPixel(APPLYGAMMADIRRGBA<COLORTYPE, ORDERTYPE, GAMMALUT>(g));
                                                      }


    template<class GAMMALUT>
    void                                              ApplyGammaInv                       (const GAMMALUT& g)
                                                      {
                                                        ForEachPixel(APPLYGAMMAINVRGBA<COLORTYPE, ORDERTYPE, GAMMALUT>(g));
                                                      }


    template<class RENDERERBUFFER2>
    void                                              CopyFrom                            (const RENDERERBUFFER2& from, int xdst, int ydst, int xsrc, int ysrc, XDWORD size)
                                                      {
                                                        const XBYTE* p = from.Row(ysrc);
                                                        if(p) memmove(rendererbuffer->Row(xdst, ydst, size) + xdst * PIXELWIDTH, p + xsrc * PIXELWIDTH, size * PIXELWIDTH);
                                                      }


    template<class SOURCEBUFFERPIXELFORMAT>
    void                                              PutBlendFrom                        (const SOURCEBUFFERPIXELFORMAT& from, int xdst, int ydst, int xsrc, int ysrc, XDWORD size, XBYTE cover)
                                                      {
                                                        typedef typename SOURCEBUFFERPIXELFORMAT::ORDERTYPE SOURCEORDER;

                                                        const VALUETYPE* psrc = (const VALUETYPE*)from.Row(ysrc);

                                                        if(psrc)
                                                          {
                                                            psrc += xsrc << 2;
                                                            VALUETYPE* pdst = (VALUETYPE*)rendererbuffer->Row(xdst, ydst, size) + (xdst << 2);

                                                            int incp = 4;
                                                            if(xdst > xsrc)
                                                              {
                                                                psrc += (size-1) << 2;
                                                                pdst += (size-1) << 2;
                                                                incp = -4;
                                                              }

                                                            do{ BLENDERTYPE::PutBlendPixel(comp_op, pdst, psrc[SOURCEORDER::R], psrc[SOURCEORDER::G], psrc[SOURCEORDER::B], psrc[SOURCEORDER::A], cover);
                                                                psrc += incp;
                                                                pdst += incp;

                                                              } while(--size);
                                                          }
                                                      }

    template<class SOURCEBUFFERPIXELFORMAT>
    void                                              PutBlendFromColor                   (const SOURCEBUFFERPIXELFORMAT& from, const COLORTYPE& color, int xdst, int ydst, int xsrc, int ysrc, XDWORD size, XBYTE cover)
                                                      {
                                                        typedef typename SOURCEBUFFERPIXELFORMAT::VALUETYPE SOURCEVALUETYPE;
                                                        const SOURCEVALUETYPE* psrc = (SOURCEVALUETYPE*)from.Row(ysrc);

                                                        if(psrc)
                                                          {
                                                            VALUETYPE* pdst = (VALUETYPE*)rendererbuffer->Row(xdst, ydst, size) + (xdst << 2);
                                                            do{ BLENDERTYPE::PutBlendPixel(comp_op, pdst, color.r, color.g, color.b, color.a, (*psrc * cover + BASESCALE_MASK) >> BASESCALE_SHIFT);
                                                                ++psrc;
                                                                pdst += 4;

                                                              } while(--size);
                                                          }
                                                      }

    template<class SOURCEBUFFERPIXELFORMAT>
    void                                              PutBlendFromLut                     (const SOURCEBUFFERPIXELFORMAT& from, const COLORTYPE* colorlut, int xdst, int ydst, int xsrc, int ysrc, XDWORD size, XBYTE cover)
                                                      {
                                                        typedef typename SOURCEBUFFERPIXELFORMAT::VALUETYPE SOURCEVALUETYPE;
                                                        const SOURCEVALUETYPE* psrc = (SOURCEVALUETYPE*)from.Row(ysrc);

                                                        if(psrc)
                                                          {
                                                            VALUETYPE* pdst = (VALUETYPE*)rendererbuffer->Row(xdst, ydst, size) + (xdst << 2);
                                                            do{ const COLORTYPE& color = colorlut[*psrc];
                                                                BLENDERTYPE::PutBlendPixel(comp_op, pdst, color.r, color.g, color.b, color.a, cover);
                                                                ++psrc;
                                                                pdst += 4;

                                                              } while(--size);
                                                          }
                                                      }

  private:

    RENDERERBUFFERTYPE*                               rendererbuffer;
    XDWORD                                            comp_op;
};



typedef GRPBUFFERPIXELFORMATALPHABLENDRGBA<BLENDERRGBA<GRPBUFFERCOLORRGBA8, GRPBUFFERCOLORORDER_RGBA>, GRPBUFFER, XDWORD> GRPBUFFERPIXELFORMAT_RGBA32;
typedef GRPBUFFERPIXELFORMATALPHABLENDRGBA<BLENDERRGBA<GRPBUFFERCOLORRGBA8, GRPBUFFERCOLORORDER_ARGB>, GRPBUFFER, XDWORD> GRPBUFFERPIXELFORMAT_ARGB32;
typedef GRPBUFFERPIXELFORMATALPHABLENDRGBA<BLENDERRGBA<GRPBUFFERCOLORRGBA8, GRPBUFFERCOLORORDER_ABGR>, GRPBUFFER, XDWORD> GRPBUFFERPIXELFORMAT_ABGR32;
typedef GRPBUFFERPIXELFORMATALPHABLENDRGBA<BLENDERRGBA<GRPBUFFERCOLORRGBA8, GRPBUFFERCOLORORDER_BGRA>, GRPBUFFER, XDWORD> GRPBUFFERPIXELFORMAT_BGRA32;



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/


#endif



