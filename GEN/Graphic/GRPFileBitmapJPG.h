//------------------------------------------------------------------------------------------
//  GRPFILEBITMAPJPG.H
//
/**
// \class
//
//  Graphic File Bitmap JPG class
//
//  @author  Diego Martinez Ruiz de Gaona
//  @version 19/11/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _GRPFILEBITMAPJPG_H_
#define _GRPFILEBITMAPJPG_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <setjmp.h>

#include "XBase.h"
#include "XFile.h"
#include "XPath.h"

#include "GRPFileBitmap.h"

extern "C"
{
 #include "jpeglib.h"
}

//---- DEFINES & ENUMS  --------------------------------------------------------------------

struct GRPFILEBITMAPJPG_ERROR
{
  struct jpeg_error_mgr             pub;
  jmp_buf                           setjmp_buffer;
};

typedef struct GRPFILEBITMAPJPG_ERROR*  GRPFILEBITMAPJPG_ERROR_PTR;

//---- CLASS -------------------------------------------------------------------------------

class GRPFACTORY;
class GRPBITMAP;


class GRPFILEBITMAPJPGHEADER
{
  public:

    XDWORD                          width;
    XDWORD                          height;
    XDWORD                          size;
};



class GRPFILEBITMAPJPG : public GRPFILEBITMAPBASE
{
  public:
                                    GRPFILEBITMAPJPG          ( );
    virtual                        ~GRPFILEBITMAPJPG          ();

    GRPBITMAP*                      CreateBitmapFromFile      (XPATH& xpath);
    bool                            CreateFileFromBitmap      (XPATH& xpath, GRPBITMAP* bitmap);

  private:

    void                            Clean                     ();

    bool                            ReadHeader                (GRPFILEBITMAPJPGHEADER* header);
    bool                            Read                      (XBYTE* buffer);
    bool                            Write                     (int width, int height, XBYTE* buffer, int quality, bool baseline);

    struct jpeg_compress_struct     cinfo;
    struct jpeg_decompress_struct   dinfo;
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

