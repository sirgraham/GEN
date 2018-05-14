/*------------------------------------------------------------------------------------------
//  GRPBUFFERCOLORGRAY.H
*/
/**
// \class
//
//  Graphics Color Gray Class
//
//  @author  Diego Martinez Ruiz de Gaona
//  @version 01/08/2013 10:13:24
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPBUFFERCOLORGRAY_H_
#define _GRPBUFFERCOLORGRAY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPBuffer.h"
#include "GRPBufferColor.h"
#include "GRPBufferColorRGB.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/



class GRPBUFFERCOLORGRAY8 : public GRPBUFFERCOLOR
{
  public:

    enum BASESCALE
    {
      BASESCALE_SHIFT = 8                     ,
      BASESCALE_SCALE = 1 << BASESCALE_SHIFT  ,
      BASESCALE_MASK  = BASESCALE_SCALE - 1
    };

    typedef XBYTE               VALUETYPE;
    typedef XDWORD              CALCTYPE;
    typedef int                 LONGTYPE;
    typedef GRPBUFFERCOLORGRAY8 SELFTYPE;


                                GRPBUFFERCOLORGRAY8                   ()
                                {
                                  Clean();
                                }


                                GRPBUFFERCOLORGRAY8                   (XDWORD v, XDWORD a = BASESCALE_MASK)
                                {
                                  this->v = XBYTE(v);
                                  this->a = XBYTE(a);
                                }


                                GRPBUFFERCOLORGRAY8                   (const SELFTYPE& c, XDWORD a)
                                {
                                  this->v = c.v;
                                  this->a = VALUETYPE(a);
                                }


                                GRPBUFFERCOLORGRAY8                   (const GRPBUFFERCOLORRGBA& c, XDWORD a)
                                {
                                  Clean();

                                  this->v = (VALUETYPE)uRound((0.299*c.r + 0.587*c.g + 0.114*c.b) * double(BASESCALE_MASK));
                                  this->a = (VALUETYPE)uRound(c.a * double(BASESCALE_MASK));
                                }


                                GRPBUFFERCOLORGRAY8                   (const GRPBUFFERCOLORRGBA& c)
                                {
                                  this->v = (VALUETYPE)uRound((0.299*c.r + 0.587*c.g + 0.114*c.b) * double(BASESCALE_MASK));
                                  this->a = (VALUETYPE)uRound(c.a * double(BASESCALE_MASK));
                                }


                                GRPBUFFERCOLORGRAY8                   (const GRPBUFFERCOLORRGBA& c, double a)
                                {
                                  this->v = (VALUETYPE)uRound((0.299*c.r + 0.587*c.g + 0.114*c.b) * double(BASESCALE_MASK));
                                  this->a = (VALUETYPE)uRound(a * double(BASESCALE_MASK));
                                }


                                GRPBUFFERCOLORGRAY8                   (const GRPBUFFERCOLORRGBA8& c)
                                {
                                  this->v = ((c.r*77 + c.g*150 + c.b*29) >> 8);
                                  this->a = (c.a);
                                }


                                GRPBUFFERCOLORGRAY8                   (const GRPBUFFERCOLORRGBA8& c, XDWORD a)
                                {
                                  this->v = ((c.r*77 + c.g*150 + c.b*29) >> 8);
                                  this->a = a;
                                }


    virtual                    ~GRPBUFFERCOLORGRAY8                   ()
                                {
                                  Clean();
                                }


    void                        Clear                                 ()
                                {
                                  v = 0;
                                  a = 0;
                                }


    const SELFTYPE&             Transparent                           ()
                                {
                                  a = 0;
                                  return *this;
                                }


    void                        Opacity                               (double a)
                                {
                                  if(a < 0.0) a = 0.0;
                                  if(a > 1.0) a = 1.0;

                                  this->a = (VALUETYPE)uRound(a * double(BASESCALE_MASK));
                                }


    double                      Opacity                               () const
                                {
                                  return double(a) / double(BASESCALE_MASK);
                                }



    const SELFTYPE&             PreMultiply                           ()
                                {
                                  if(a == BASESCALE_MASK) return *this;
                                  if(a == 0)
                                    {
                                      v = 0;
                                      return *this;
                                    }

                                  v = VALUETYPE((CALCTYPE(v) * a) >> BASESCALE_SHIFT);

                                  return *this;
                                }


    const SELFTYPE&             PreMultiply                           (XDWORD a_)
                                {
                                  if(this->a == BASESCALE_MASK && a >= BASESCALE_MASK) return *this;
                                  if(this->a == 0 || a == 0)
                                    {
                                      v = this->a = 0;
                                      return *this;
                                    }

                                  CALCTYPE _v = (CALCTYPE(v) * a) / this->a;

                                  this->v = VALUETYPE((_v > a) ? a : _v);
                                  this->a = VALUETYPE(a);

                                  return *this;
                                }


    const SELFTYPE&             DeMultiply                            ()
                                {
                                  if(a == BASESCALE_MASK) return *this;
                                  if(a == 0)
                                    {
                                      v = 0;
                                      return *this;
                                    }

                                  CALCTYPE _v = (CALCTYPE(v) * BASESCALE_MASK) / a;
                                  v = VALUETYPE((_v > BASESCALE_MASK) ? (VALUETYPE)BASESCALE_MASK : _v);

                                  return *this;
                                }


    SELFTYPE                    Gradient                              (SELFTYPE c, double k) const
                                {
                                  SELFTYPE ret;
                                  CALCTYPE ik = uRound(k * BASESCALE_SCALE);

                                  ret.v = VALUETYPE(CALCTYPE(v) + (((CALCTYPE(c.v) - v) * ik) >> BASESCALE_SHIFT));
                                  ret.a = VALUETYPE(CALCTYPE(a) + (((CALCTYPE(c.a) - a) * ik) >> BASESCALE_SHIFT));

                                  return ret;
                                }


    void                        Add(const SELFTYPE& c, XDWORD cover)
                                {
                                  CALCTYPE cv, ca;
                                  if(cover == COVERTYPE_MASK)
                                    {
                                      if(c.a == BASESCALE_MASK)
                                        {
                                          *this = c;
                                        }
                                       else
                                        {
                                          cv = v + c.v; v = (cv > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : cv;
                                          ca = a + c.a; a = (ca > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : ca;
                                        }
                                    }
                                   else
                                    {
                                      cv = v + ((c.v * cover + COVERTYPE_MASK/2) >> COVERTYPE_SHIFT);
                                      ca = a + ((c.a * cover + COVERTYPE_MASK/2) >> COVERTYPE_SHIFT);
                                      v  = (cv > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : cv;
                                      a  = (ca > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : ca;
                                    }
                                }


    static SELFTYPE             GetNoColor()
                                {
                                  return SELFTYPE(0,0);
                                }


    VALUETYPE                   v;
    VALUETYPE                   a;

  private:

    void                        Clean                                 ()
                                {
                                  v = 0;
                                  a = 0;
                                }

};


/*
    //-------------------------------------------------------------gray8_pre
    inline gray8 gray8_pre(XDWORD v, XDWORD a = gray8::BASESCALE_MASK)
    {
        return gray8(v,a).premultiply();
    }
    inline gray8 gray8_pre(const gray8& c, XDWORD a)
    {
        return gray8(c,a).premultiply();
    }
    inline gray8 gray8_pre(const rgba& c)
    {
        return gray8(c).premultiply();
    }
    inline gray8 gray8_pre(const rgba& c, double a)
    {
        return gray8(c,a).premultiply();
    }
    inline gray8 gray8_pre(const rgba8& c)
    {
        return gray8(c).premultiply();
    }
    inline gray8 gray8_pre(const rgba8& c, XDWORD a)
    {
        return gray8(c,a).premultiply();
    }
*/


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

