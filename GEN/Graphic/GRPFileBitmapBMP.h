//------------------------------------------------------------------------------------------
//  GRPFILEBITMAPBMP.H
//
/**
// \class
//
//  Graphic File Bitmap BMP class
//
//  @author  Diego Martinez Ruiz de Gaona
//  @version 19/11/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _GRPFILEBITMAPBMP_H_
#define _GRPFILEBITMAPBMP_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XFile.h"
#include "XPath.h"
#include "XMap.h"

#include "GRPProperties.h"
#include "GRPBufferColorRGB.h"
#include "GRPFileBitmap.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum GRPFILEBITMAPBMPCOMPRESSION
{
  GRPFILEBITMAPBMPCOMPRESSION_NOT   = 0 ,
  GRPFILEBITMAPBMPCOMPRESSION_8RLE      ,
  GRPFILEBITMAPBMPCOMPRESSION_4RLE      ,
  GRPFILEBITMAPBMPCOMPRESSION_RGB
};


#pragma pack(push, r1, 1)
typedef struct
{
    XWORD                               type;             // Signature - 'BM'
    XDWORD                              size;             // File size in bytes
    XWORD                               reserved1;        // 0
    XWORD                               reserved2;        // 0
    XDWORD                              offsetdata;       // Offset to data of bitmap

    XDWORD                              headersize;       // size of this struct (40)
    XDWORD                              width;            // bmap width in pixels
    XDWORD                              height;           // bmap height in pixels

    XWORD                               planes;           // num planes - always 1
    XWORD                               bitsperpixel;     // bits per pixel

    XDWORD                              compression;      // compression flag
    XDWORD                              sizebmp;          // image size in bytes
    XDWORD                              xpixelspermeter;  // horz resolution
    XDWORD                              ypixelspermeter;  // vert resolution
    XDWORD                              colorused;        // 0 -> color table size
    XDWORD                              colorimportant;   // important color count

} GRPFILEBITMAPBMPHEADERINFO;
#pragma pack(pop, r1)


//---- CLASS -------------------------------------------------------------------------------

class GRPFACTORY;
class GRPBITMAP;



class GRPFILEBITMAPBMPCOLORPALETE
{
  public:

                                        GRPFILEBITMAPBMPCOLORPALETE       ()
                                        {
                                          Clean();
                                        }

                                        GRPFILEBITMAPBMPCOLORPALETE       (XBYTE red, XBYTE green, XBYTE blue, XBYTE alpha)
                                        {
                                          Clean();

                                          this->red   = red;
                                          this->green = green;
                                          this->blue  = blue;
                                          this->alpha = alpha;

                                        }

    virtual                            ~GRPFILEBITMAPBMPCOLORPALETE       ()
                                        {
                                          Clean();
                                        }

    XBYTE                               red;
    XBYTE                               green;
    XBYTE                               blue;
    XBYTE                               alpha;

  private:

    void                                Clean                             ()
                                        {
                                          red   = 0;
                                          green = 0;
                                          blue  = 0;
                                          alpha = 0;
                                        }

};



class GRPFILEBITMAPBMPHEADER
{
  public:
                                        GRPFILEBITMAPBMPHEADER            ();
    virtual                            ~GRPFILEBITMAPBMPHEADER            ();

    bool                                Load                              (XFILE* file);


    GRPFILEBITMAPBMPHEADERINFO*               GetHeaderInfo               ()                    { return &headerinfo;               }

    int                                 GetWidth                          ()                    { return headerinfo.width;          }
    int                                 GetHeight                         ()                    { return headerinfo.height;         }
    int                                 GetBitsPerPixel                   ()                    { return headerinfo.bitsperpixel;   }
    int                                 GetOffsetData                     ()                    { return headerinfo.offsetdata;     }

    GRPPROPERTYMODE                     GetMode                           ()                    { return mode;                      }

    bool                                GetColorPalette                   (XBYTE index, GRPBUFFERCOLORRGBA8& color)
                                        {
                                          GRPFILEBITMAPBMPCOLORPALETE* colorpalette = (GRPFILEBITMAPBMPCOLORPALETE*)palette.GetElement(index);
                                          if(!colorpalette) return false;

                                          color.r = colorpalette->red;
                                          color.g = colorpalette->green;
                                          color.b = colorpalette->blue;
                                          color.a = colorpalette->alpha;

                                          return true;
                                        }

    bool                                DeleteAllPalette                  ()
                                        {
                                          if(palette.IsEmpty()) return true;

                                          palette.DeleteElementContents();

                                          palette.DeleteAll();

                                          return true;
                                        }



  private:

    void                                Clean                             ();

    GRPPROPERTYMODE                     mode;
    GRPFILEBITMAPBMPHEADERINFO                headerinfo;
    XMAP<XBYTE, GRPFILEBITMAPBMPCOLORPALETE*> palette;
};




class GRPFILEBITMAPBMP : public GRPFILEBITMAPBASE
{
  public:
                                        GRPFILEBITMAPBMP                  ( );
    virtual                            ~GRPFILEBITMAPBMP                  ();

    GRPBITMAP*                          CreateBitmapFromFile              (XPATH& xpath);
    bool                                CreateFileFromBitmap              (XPATH& xpath, GRPBITMAP* bitmap);

  private:

    void                                Clean                             ();

    bool                                ReadHeader                        (GRPFILEBITMAPBMPHEADER* header);
    bool                                Write                             (int width, int height, XBYTE* buffer);
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

