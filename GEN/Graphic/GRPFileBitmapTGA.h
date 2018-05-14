//------------------------------------------------------------------------------------------
//  GRPFILEBITMAPTGA.H
//
/**
// \class
//
//   Graphic File TGA class
//
//  @author  Diego Martinez Ruiz de Gaona
//  @version 19/11/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _GRPFILEBITMAPTGA_H_
#define _GRPFILEBITMAPTGA_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XFile.h"
#include "XPath.h"
#include "XMap.h"

#include "GRPProperties.h"
#include "GRPBufferColorRGB.h"
#include "GRPFileBitmap.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum GRPFILEBITMAPTGATYPE
{
  GRPFILEBITMAPTGATYPE_NOIMAGE                                =  0 ,
  GRPFILEBITMAPTGATYPE_UNCOMPRESS_INDEXCOLOR                  =  1 ,
  GRPFILEBITMAPTGATYPE_UNCOMPRESS_RGB                         =  2 ,
  GRPFILEBITMAPTGATYPE_UNCOMPRESS_BLACKWHITE                  =  3 ,
  GRPFILEBITMAPTGATYPE_RLECOMPRESS_INDEXCOLOR                 =  9 ,
  GRPFILEBITMAPTGATYPE_RLECOMPRESS_RGB                        = 10 ,
  GRPFILEBITMAPTGATYPE_COMPRESS_BLACKWHITE                    = 11 ,
  GRPFILEBITMAPTGATYPE_HUFFMANCOMPRESS_DATA_INDEXCOLOR        = 32 ,
  GRPFILEBITMAPTGATYPE_HUFFMANCOMPRESS4PASS_DATA_INDEXCOLOR   = 33
};


#pragma pack(push, r1, 1)
typedef struct
{
    XBYTE             IDlength;
    XBYTE             colourmaptype;
    XBYTE             datatypecode;
    XWORD             colourmaporigin;
    XWORD             colourmaplength;
    XBYTE             colourmapdepth;
    XWORD             xorigin;
    XWORD             yorigin;
    XWORD             width;
    XWORD             height;
    XBYTE             bitsperpixel;
    XBYTE             imagedescriptor;

} GRPFILEBITMAPTGAHEADERINFO;
#pragma pack(pop, r1)


//---- CLASS -------------------------------------------------------------------------------

class GRPFACTORY;
class GRPBITMAP;




class GRPFILEBITMAPTGAHEADER
{
  public:
                                        GRPFILEBITMAPTGAHEADER    ();
    virtual                            ~GRPFILEBITMAPTGAHEADER    ();

    bool                                Load                      (XFILE* file);


    GRPFILEBITMAPTGAHEADERINFO*         GetHeaderInfo             ()                    { return &headerinfo;                             }

    int                                 GetWidth                  ()                    { return headerinfo.width;                        }
    int                                 GetHeight                 ()                    { return headerinfo.height;                       }
    int                                 GetBitsPerPixel           ()                    { return headerinfo.bitsperpixel;                 }
    int                                 GetOffsetData             ()                    { return offsetdata;                              }

    GRPPROPERTYMODE                     GetMode                   ()                    { return mode;                                    }
    GRPFILEBITMAPTGATYPE                GetType                   ()                    { return (GRPFILEBITMAPTGATYPE)headerinfo.datatypecode;   }

  private:

    void                                Clean                     ();

    GRPPROPERTYMODE                     mode;
    GRPFILEBITMAPTGAHEADERINFO          headerinfo;
    XWORD                               offsetdata;
};




class GRPFILEBITMAPTGA : public GRPFILEBITMAPBASE
{
enum
{
  SET,
  CUR,
  END
};

enum
{
  itUndefined,
  itRGB,
  itRGBA,
  itGreyscale
};

enum IMAGE_ORIGIN
{
  BOTTOM_LEFT=0,
  BOTTOM_RIGHT,
  TOP_LEFT,
  TOP_RIGHT
};

  public:
                                        GRPFILEBITMAPTGA          ( );
    virtual                            ~GRPFILEBITMAPTGA          ();

    GRPBITMAP*                          CreateBitmapFromFile      (XPATH& xpath);
    bool                                CreateFileFromBitmap      (XPATH& xpath, GRPBITMAP* bitmap);

    XQWORD                              SeekBuffer                (XBYTE * Buffer,int size, unsigned int from,  int MaxSize);
    bool                                ReadData                  (XBYTE * file, void * dest,       int size,   int MaxSize);
    bool                                Decompress                (XBYTE * file, XQWORD BufferSize);

    XQWORD Pointer;
    XDWORD width;
    XDWORD height;
    XDWORD nChannels;
    XDWORD bitsperpixel;
    XBYTE* Data;
    XQWORD DataSize;

    IMAGE_ORIGIN imageOrigin;
    GRPBITMAP*  bitmap;

  private:

    void                                Clean                     ();

    bool                                ReadHeader                (GRPFILEBITMAPTGAHEADER* header);
    bool                                CreateColor               (GRPPROPERTYMODE mode, XBYTE* buffer, GRPBUFFERCOLORRGBA8& color);

};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

