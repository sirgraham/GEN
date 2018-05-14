/*------------------------------------------------------------------------------------------
//  GRPCOLOR.H
*/
/**
// \class
//
//  Graphics Color class
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 07/05/2014 13:26:35
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPCOLOR_H_
#define _GRPCOLOR_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XMap.h"
#include "XString.h"
#include "XDictionary.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/



class GRPCOLOR
{
public:

  static GRPCOLOR WHITE;
  static GRPCOLOR BLACK;
  static GRPCOLOR GRAY;

  static GRPCOLOR RED;
  static GRPCOLOR GREEN;
  static GRPCOLOR BLUE;

  static GRPCOLOR YELLOW;
  static GRPCOLOR PURPLE;
  static GRPCOLOR PINK;
  static GRPCOLOR ORANGE;

                        GRPCOLOR              (float* c);
                        GRPCOLOR              (double r,  double g, double  b,  double a=1.0);
                        GRPCOLOR              (float  r,  float g, float  b,  float a=1.0);
                        GRPCOLOR              (int    r,  int   g, int    b,  int   a=255);

                        GRPCOLOR              (XCHAR*);

                        GRPCOLOR              ();
    virtual            ~GRPCOLOR              ();


    float               GetRed                ();
    void                SetRed                (float red);

    float               GetGreen              ();
    void                SetGreen              (float green);

    float               GetBlue               ();
    void                SetBlue               (float blue);

    float               GetAlpha              ();
    void                SetAlpha              (float alpha);

    void                Set                   (float red,float green,float blue,float alpha=1.0f);
    bool                Set                   (XCHAR* s);
    void                Get                   (float& r,float& g,float& b,float& a);
    XSTRING*            GetString             () { return &string;}

    XSTRING             string;


    operator            XSTRING*              ()
    {
      return &this->string;
    }

    GRPCOLOR&           operator=             (GRPCOLOR rhs)
                        {
                          red   = rhs.red;
                          green = rhs.green;
                          blue  = rhs.blue;
                          alpha = rhs.alpha;
                          this->string = rhs.string;
                          return *this;
                        }

    bool                operator==            (const GRPCOLOR & rhs)
                        {
                          return (red   == rhs.red)   &&
                                 (green == rhs.green) &&
                                 (blue  == rhs.blue)  &&
                                 (alpha == rhs.alpha);
                        }

                        float       red;
                        float       green;
                        float       blue;
                        float       alpha;


    void                Clean                 ()
                        {
                          red         = 1.0f;
                          green       = 1.0f;
                          blue        = 1.0f;
                          alpha       = 1.0f;
                        }
};


class GRPCOLORPALETTE
{
public:
    GRPCOLORPALETTE();
    ~GRPCOLORPALETTE();

    XDICTIONARY<GRPCOLOR> palette;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

