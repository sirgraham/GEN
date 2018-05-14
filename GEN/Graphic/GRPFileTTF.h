/*------------------------------------------------------------------------------------------
//  GRPFILETTF.H
*/
/**
// \class
//
//  Graphic File TTF class (Creates 3d text from vectorial data in TTF)
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 19/05/2014 9:35:38
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPFILETTF_H_
#define _GRPFILETTF_H_

/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>

#ifndef __STB_INCLUDE_STB_TRUETYPE_H__
#include "stb_truetype.h"
#endif

#include "XMap.h"
#include "XString.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


enum GRPFILETTFANCHORS
{
  GRPFILETTFANCHOR_VMOVE                        =   1,
  GRPFILETTFANCHOR_VLINE                             ,
  GRPFILETTFANCHOR_VCURVE,
  GRPFILETTFANCHOR_VCUBIC,
};


enum  GRPFILETTFPLATFORMID
{
  GRPFILETTFPLATFORMID_UNICODE                  =   0,
  GRPFILETTFPLATFORMID_MAC                      =   1,
  GRPFILETTFPLATFORMID_ISO                      =   2,
  GRPFILETTFPLATFORMID_MICROSOFT                =   3
};


enum GRPFILETTFUNICODEENCODINGID
{
  GRPFILETTFUNICODEENCODINGID_UNICODE_1_0       =   0,
  GRPFILETTFUNICODEENCODINGID_UNICODE_1_1       =   1,
  GRPFILETTFUNICODEENCODINGID_ISO_10646         =   2,
  GRPFILETTFUNICODEENCODINGID_UNICODE_2_0_BMP   =   3,
  GRPFILETTFUNICODEENCODINGID_UNICODE_2_0_FULL  =   4
};


enum GRPFILETTFMICROSOFTENCODINGID
{
  GRPFILETTFMICROSOFTENCODINGID_SYMBOL          =   0,
  GRPFILETTFMICROSOFTENCODINGID_UNICODE_BMP     =   1,
  GRPFILETTFMICROSOFTENCODINGID_SHIFTJIS        =   2,
  GRPFILETTFMICROSOFTENCODINGID_UNICODE_FULL    =  10
};


enum GRPFILETTFAPPLEENCODINGID
{
  GRPFILETTFAPPLEENCODINGID_ROMAN               =   0,
  GRPFILETTFAPPLEENCODINGID_ARABIC              =   4,
  GRPFILETTFAPPLEENCODINGID_JAPANESE            =   1,
  GRPFILETTFAPPLEENCODINGID_HEBREW              =   5,
  GRPFILETTFAPPLEENCODINGID_CHINESE_TRAD        =   2,
  GRPFILETTFAPPLEENCODINGID_GREEK               =   6,
  GRPFILETTFAPPLEENCODINGID_KOREAN              =   3,
  GRPFILETTFAPPLEENCODINGID_RUSSIAN             =   7
};


enum GRPFILETTFMICROSOFTLANGUAGEID
{
  GRPFILETTFMICROSOFTLANGUAGEID_ENGLISH         = 0x0409,
  GRPFILETTFMICROSOFTLANGUAGEID_ITALIAN         = 0x0410,
  GRPFILETTFMICROSOFTLANGUAGEID_CHINESE         = 0x0804,
  GRPFILETTFMICROSOFTLANGUAGEID_JAPANESE        = 0x0411,
  GRPFILETTFMICROSOFTLANGUAGEID_DUTCH           = 0x0413,
  GRPFILETTFMICROSOFTLANGUAGEID_KOREAN          = 0x0412,
  GRPFILETTFMICROSOFTLANGUAGEID_FRENCH          = 0x040C,
  GRPFILETTFMICROSOFTLANGUAGEID_RUSSIAN         = 0x0419,
  GRPFILETTFMICROSOFTLANGUAGEID_GERMAN          = 0x0407,
  GRPFILETTFMICROSOFTLANGUAGEID_SPANISH         = 0x0409,
  GRPFILETTFMICROSOFTLANGUAGEID_HEBREW          = 0x040D,
  GRPFILETTFMICROSOFTLANGUAGEID_SWEDISH         = 0x041D
};

enum GRPFILETTFAPPLELANGUAGEID
{
  GRPFILETTFAPPLELANGUAGEID_ENGLISH             =   0,
  GRPFILETTFAPPLELANGUAGEID_JAPANESE            =  11,
  GRPFILETTFAPPLELANGUAGEID_ARABIC              =  12,
  GRPFILETTFAPPLELANGUAGEID_KOREAN              =  23,
  GRPFILETTFAPPLELANGUAGEID_DUTCH               =   4,
  GRPFILETTFAPPLELANGUAGEID_RUSSIAN             =  32,
  GRPFILETTFAPPLELANGUAGEID_FRENCH              =   1,
  GRPFILETTFAPPLELANGUAGEID_SPANISH             =   6,
  GRPFILETTFAPPLELANGUAGEID_GERMAN              =   2,
  GRPFILETTFAPPLELANGUAGEID_SWEDISH             =   5,
  GRPFILETTFAPPLELANGUAGEID_HEBREW              =  10,
  GRPFILETTFAPPLELANGUAGEID_CHINESE_SIMPLIFIED  =  33,
  GRPFILETTFAPPLELANGUAGEID_ITALIAN             =   3,
  GRPFILETTFAPPLELANGUAGEID_CHINESE_TRAD        =  19
};

enum GRPFILETTF_STRINGID
{
    GRPFILETTF_STRINGID_COPYRIGHT =0,
    GRPFILETTF_STRINGID_FAMILY,
    GRPFILETTF_STRINGID_STYLE,
    GRPFILETTF_STRINGID_ID,
    GRPFILETTF_STRINGID_NAME,
    GRPFILETTF_STRINGID_VERSION,
    GRPFILETTF_STRINGID_POSTSCRIPT_NAME,
    GRPFILETTF_STRINGID_TRADEMARK,
    GRPFILETTF_STRINGID_MANUFACTURER,
    GRPFILETTF_STRINGID_DESIGNER,
    GRPFILETTF_STRINGID_DESCRIPTION,
    GRPFILETTF_STRINGID_URL_VENDOR,
    GRPFILETTF_STRINGID_URL_DESIGNER,
    GRPFILETTF_STRINGID_LICENSE_DESCRIPTION,
    GRPFILETTF_STRINGID_LICENSE_INFO,
    GRPFILETTF_STRINGID_LICENSE_URL,
    GRPFILETTF_STRINGID_RESERVED,
    GRPFILETTF_STRINGID_TYPOGRAPHIC_FAMILY,
    GRPFILETTF_STRINGID_TYPOGRAPHIC_SUBFAMILY,
    GRPFILETTF_STRINGID_COMPATIBLE_FULL,
    GRPFILETTF_STRINGID_SAMPLE_TEXT,
    GRPFILETTF_STRINGID_POSTSCRIPT_CID,
    GRPFILETTF_STRINGID_WWS_FAMILY_NAME,
    GRPFILETTF_STRINGID_WWS_SUBFAMILY_NAME,
    GRPFILETTF_STRINGID_LIGHT_BACKGROUND_PALETTE,
    GRPFILETTF_STRINGID_DARK_BACKGROUND_PALETTE,
    GRPFILETTF_STRINGID_VARIOUS_POSTSCRIPT_NAME_PREFIX,
    GRPFILETTF_STRINGID_MAX
};


typedef struct
{
  int     width;
  int     height;
  int     stride;
  XBYTE*  pixels;

} GRPFILETTFBITMAP;


#define GRPFILETTF_TAG4(tag, c0, c1, c2, c3)    (((tag)[0] == c0) && ((tag)[1] == c1) && ((tag)[2] == c2) && ((tag)[3] == c3))
#define GRPFILETTF_TAG(tag, str)                GRPFILETTF_TAG4(tag, str[0], str[1], str[2], str[3])


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XPATH;
class GRPOBJECT;
class GRPSHAPE;
class GRPMATERIAL;


class GRPFILETTFGLYPHVERTEX
{
  public:
                                      GRPFILETTFGLYPHVERTEX             (GRPFILETTFGLYPHVERTEX* v)
                                      {
                                        this->Set(v->type,(int)v->x,(int)v->y,(int)v->cx,(int)v->cy,(int)v->cx1,(int)v->cy1);
                                      }

                                      GRPFILETTFGLYPHVERTEX             (stbtt_vertex* v)
                                      {
                                      type=v->type;
                                      x=v->x;
                                      y=v->y;
                                      cx=v->cx;
                                      cy=v->cy;
                                      cx1=v->cx1;
                                      cy1=v->cy1;
                                      padding=v->padding;
                                      }


                                      GRPFILETTFGLYPHVERTEX             ()
                                      {
                                        Clean();
                                      }

                                     ~GRPFILETTFGLYPHVERTEX             ()
                                      {
                                        Clean();
                                      }

    void                              Set                               (XBYTE type, int x, int y, int cx, int cy,int cx1, int cy1)
                                      {
                                        this->type  = type;
                                        this->x     = (float) x;
                                        this->y     = (float) y;
                                        this->cx    = (float) cx;
                                        this->cy    = (float) cy;
                                        this->cx    = (float) cx1;
                                        this->cy    = (float) cy1;
                                      }

    XBYTE                             type;
    float                             x;
    float                             y;
    float                             cx;
    float                             cy;
    float                             cx1;
    float                             cy1;
    XBYTE                             padding;

  private:

    void                              Clean                             ()
                                      {
                                        type    = 0;
                                        x       = 0.0f;
                                        y       = 0.0f;
                                        cx      = 0.0f;
                                        cy      = 0.0f;
                                        padding = 0;
                                      }
};





class GRPFILETTFGLYPH
{
  public:
                                      GRPFILETTFGLYPH                   (int ID)
                                      {
                                        Clean();

                                        this->ID = ID;
                                      }

                                     ~GRPFILETTFGLYPH                   ()
                                      {
                                        vertexlist.DeleteContents();
                                        vertexlist.DeleteAll();

                                        Clean();
                                      }


    int                               ID;
    float                             scale_x;
    float                             scale_y;
    float                             shift_x;
    float                             shift_y;

    float                             width;
    float                             height;

    float                             advance;
    float                             bearing;
    float                             lineheight;
    float                             lineascend;
    float                             linedescend;
    float                             linegap;

    XVECTOR<GRPFILETTFGLYPHVERTEX*>   vertexlist;

  private:

    void                              Clean                             ()
                                      {
                                        vertexlist.SetIsMulti(true);

                                        ID        = 0;
                                        scale_x   = 0.0f;
                                        scale_y   = 0.0f;
                                        shift_x   = 0.0f;
                                        shift_y   = 0.0f;

                                        width     = 0.0f;
                                        height    = 0.0f;
                                      }
};

enum GRPFILETTF_STATE
{
    GRPFILETTF_STATE_UNKNOWN=0,
    GRPFILETTF_STATE_OPEN,
    GRPFILETTF_STATE_INIT,
    GRPFILETTF_STATE_READ,
    GRPFILETTF_STATE_PARSED,
    GRPFILETTF_STATE_CLOSE,
    GRPFILETTF_STATE_END
};


class GRPFILETTF
{
  public:
                                        GRPFILETTF                      ();
    virtual                            ~GRPFILETTF                      ();

    bool                                OpenFile                        (XCHAR* path);
    bool                                LoadFile                        (XCHAR* path);
    bool                                LoadFile                        (XPATH& xpath);
    bool                                Init                            ();
    bool                                ParseGlyphs                     ();

    GRPSHAPE*                           GetGlyphShape                   (XCHAR character);

    void                                GetFontNameStrings              ();

    stbtt_fontinfo*                     GetInfo                         ()  { return &info;}

    int                                 FindGlyphIndex                  (int n);

    bool                                GetGlyphMetrics                 (int index=32, float *advanceWidth=NULL, float *leftSideBearing=NULL,float* ascend=NULL,float* descent=NULL,float * linegap=NULL);
    bool                                GetFontOS2Metrics               (float* ascend=NULL,float* descent=NULL,float * linegap=NULL);

    float                               GetKerning                      (int ch1, int ch2);

    XCHAR*                              GetString                       (GRPFILETTF_STRINGID);

    GRPFILETTF_STATE                    state;

  private:

    stbtt_fontinfo                      info;

    XMAP<XDWORD, XSTRING*>              strings;

    XBUFFER*                            xbuffer;
    XMAP<int, GRPFILETTFGLYPH*>         glyphs;
    XMAP<int, GRPSHAPE*>                shapes;

    void                                Clean                           ()
                                        {
                                          xbuffer   = NULL;
                                          state     = GRPFILETTF_STATE_UNKNOWN;
                                          memset(&info, 0, sizeof(info));
                                        }

};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif