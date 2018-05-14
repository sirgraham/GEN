//------------------------------------------------------------------------------------------
//  GRPFILEBITMAPJPG.CPP
//
//  Graphic File Bitmap JPG class
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 19/11/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"

#include "GRPFactory.h"
#include "GRPBitmap.h"

#include "GRPFileBitmapJPG.h"

#include "XMemory.h"

//---- GENERAL VARIABL+E --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  GRPFILEBITMAPJPG_ErrorExit
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/11/2002 18:37:22
//
//  @return       METHODDEF(void) :
//  @param        info :
*/
//-------------------------------------------------------------------
METHODDEF(void) GRPFILEBITMAPJPG_ErrorExit(j_common_ptr info)
{
  GRPFILEBITMAPJPG_ERROR_PTR myerr = (GRPFILEBITMAPJPG_ERROR_PTR) info->err;

  (*info->err->output_message)(info);
  longjmp(myerr->setjmp_buffer, 1);
}



/*-------------------------------------------------------------------
//  GRPFILEBITMAPJPG::GRPFILEBITMAPJPG
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/11/2013 10:58:17
//
//  @return
//
*/
/*-----------------------------------------------------------------*/
GRPFILEBITMAPJPG::GRPFILEBITMAPJPG() : GRPFILEBITMAPBASE()
{
  Clean();

  memset(&cinfo, 0, sizeof(struct jpeg_compress_struct));
  memset(&dinfo, 0, sizeof(struct jpeg_decompress_struct));
}




/*-------------------------------------------------------------------
//  GRPFILEBITMAPJPG::~GRPFILEBITMAPJPG
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      31/10/2013 14:23:31
//
//  @return     21º-*
-+
º/  */
/*-----------------------------------------------------------------*/
GRPFILEBITMAPJPG::~GRPFILEBITMAPJPG()
{
  memset(&cinfo, 0, sizeof(struct jpeg_compress_struct));
  memset(&dinfo, 0, sizeof(struct jpeg_decompress_struct));

  Clean();
}



/*-------------------------------------------------------------------
//  GRPFILEBITMAPJPG::CreateBitmapFromFile
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/11/2013 9:24:38
//
//  @return       GRPBITMAP* :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
GRPBITMAP* GRPFILEBITMAPJPG::CreateBitmapFromFile(XPATH& xpath)
{
  GRPBITMAP*        bitmap = NULL;

  if(!grpfactory)   return NULL;
  if(!Open(xpath))  return NULL;


  GRPFILEBITMAPJPGHEADER  header;
  XBYTE*            buffer = NULL;
  bool              status = true;

  if(!ReadHeader(&header))
    {
      Close();
      return NULL;
    }

  bitmap = grpfactory->CreateBitmap(header.width, header.height, GRPPROPERTYMODE_24_RGB_888);
  if(bitmap)
    {
      XBYTE* databuffer = new XBYTE[header.size];
      if(databuffer)
        {
          if(Read(databuffer))
            {
              int index = 0;

              for (int y = header.height - 1; y > -1; y--)
              {
                  for (XDWORD x = 0; x < header.width; x++)
                  {
                      GRPBUFFERCOLORRGBA8 color(databuffer[index + 0],
                          databuffer[index + 1],
                          databuffer[index + 2], 255);

                      bitmap->PutPixel(x, y, (GRPBUFFERCOLOR*)&color);

                      index += 3;
                  }
              }

            } else status = false;

          delete [] databuffer;

        } else status = false;

    } else status = false;

  if(!status)
    {
      if(bitmap)
        {
          grpfactory->DeleteBitmap(bitmap);
          bitmap = NULL;
        }
    }

  Close();

  return bitmap;
}




/*-------------------------------------------------------------------
//  GRPFILEBITMAPJPG::CreateFileFromBitmap
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/11/2013 13:35:06
//
//  @return       bool :
//  @param        xpath :
//  @param        bitmap :
*/
/*-----------------------------------------------------------------*/
bool GRPFILEBITMAPJPG::CreateFileFromBitmap(XPATH& xpath, GRPBITMAP* bitmap)
{
  if(!grpfactory) return false;
  if(!bitmap)     return false;

  if(!Create(xpath)) return false;

  int size = bitmap->GetWidth() * bitmap->GetHeight() * 3;
  bool status = true;

  XBYTE* databuffer = new XBYTE[size];
  if(databuffer)
    {
      int index = 0;

      for(int y=(int)(bitmap->GetHeight()-1); y>-1; y--)
        {
          for(int x=0; x<(int)bitmap->GetWidth(); x++)
            {
              GRPBUFFERCOLORRGBA8* color = (GRPBUFFERCOLORRGBA8*)bitmap->GetPixel(x, y);

              databuffer[index + 0] = color->r;
              databuffer[index + 1] = color->g;
              databuffer[index + 2] = color->b;

              index+=3;
            }
        }

    } else status = false;

  status = Write(bitmap->GetWidth(), bitmap->GetHeight(),  databuffer, 100, false);

  delete [] databuffer;

  Close();

  return true;
}




//-------------------------------------------------------------------
//  GRPFILEBITMAPJPG::Clean
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/11/2002 11:08:06
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void GRPFILEBITMAPJPG::Clean()
{

}


/*-------------------------------------------------------------------
//  GRPFILEBITMAPJPG::ReadHeader
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/11/2013 12:15:40
//
//  @return       bool :
//  @param        header :
*/
/*-----------------------------------------------------------------*/
bool GRPFILEBITMAPJPG::ReadHeader(GRPFILEBITMAPJPGHEADER* header)
{
  if(!header) return false;

  struct GRPFILEBITMAPJPG_ERROR jerr;

  if(!GetPrimaryFile()) return false;
  GetPrimaryFile()->SetPosition(0);

  dinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = GRPFILEBITMAPJPG_ErrorExit;

  if(setjmp(jerr.setjmp_buffer))
    {
      jpeg_destroy_decompress(&dinfo);
      Close();
      return false;
    }

  jpeg_create_decompress(&dinfo);                                     /* Now we can initialize the JPEG decompression object. */
//  jpeg_stdio_src(&dinfo, GetPrimaryFile()->GetFileStructHandle());        /* Step 2: specify data source (eg, a file) */

  //DIEGO: el uso de la entrada de stdio_src para jpeg tiene mas viejo que espinete. Y crear el dup del handle de fichero de windows era catastrofico con el nuevo VS2015
  //       leo todo el fichero en memoria y lo descomprimo usando el manejador mem_src.

  XDWORD file_size = GetPrimaryFile()->GetSize();
  XBYTE* file_buffer = new XBYTE[file_size];

  GetPrimaryFile()->SetPosition(0);
  GetPrimaryFile()->Read(file_buffer, file_size);

  jpeg_mem_src(&dinfo, file_buffer, file_size);
  jpeg_read_header(&dinfo, TRUE);

  header->width  = dinfo.image_width;
  header->height = dinfo.image_height;
  header->size   = (header->height*header->width*3);

  jpeg_destroy_decompress(&dinfo);

  GetPrimaryFile()->SetPosition(0);
  delete(file_buffer);
  return true;
}





/*-------------------------------------------------------------------
//  GRPFILEBITMAPJPG::Read
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/11/2011 17:31:59
//
//  @return       bool :
//  @param        header :
//  @param        buffer :
*/
/*-----------------------------------------------------------------*/
bool GRPFILEBITMAPJPG::Read(XBYTE* buffer)
{
  if(!buffer)           return false;
  if(!GetPrimaryFile()) return false;

  JSAMPARRAY  line;         /* Output row buffer */
  int         rowstride;    /* physical row width in output buffer */

  struct GRPFILEBITMAPJPG_ERROR jerr;

  GetPrimaryFile()->SetPosition(0);

  dinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = GRPFILEBITMAPJPG_ErrorExit;

  if(setjmp(jerr.setjmp_buffer))
    {
      jpeg_destroy_decompress(&dinfo);
      Close();
      return false;
    }

  jpeg_create_decompress(&dinfo);                                     /* Now we can initialize the JPEG decompression object. */
  //jpeg_stdio_src(&dinfo, GetPrimaryFile()->GetFileStructHandle());        /* Step 2: specify data source (eg, a file) */

  //DIEGO: el uso de la entrada de stdio_src para jpeg es mas viejo que espinete. Y crear el dup del handle de fichero de windows era catastrofico con el nuevo VS2015
  //       leo todo el fichero en memoria y lo descomprimo usando el manejador mem_src.
  // es el mismo problema que arriba, por lo que se lee 2 veces. Deberia contenerse en memoria la primera vez que se accede.

  XDWORD file_size = GetPrimaryFile()->GetSize();
  XBYTE* file_buffer=new XBYTE[file_size];

  GetPrimaryFile()->SetPosition(0);
  GetPrimaryFile()->Read(file_buffer, file_size);
  jpeg_mem_src(&dinfo, file_buffer, file_size);

  jpeg_read_header(&dinfo, TRUE);

  jpeg_start_decompress(&dinfo);

  rowstride = dinfo.output_width * dinfo.output_components;

  line = (*dinfo.mem->alloc_sarray)((j_common_ptr) &dinfo, JPOOL_IMAGE, rowstride, 1);

  int size  = dinfo.output_width * dinfo.output_height * dinfo.output_components;
  int index = (size-rowstride);

  memset(buffer,0,size);

  while (dinfo.output_scanline < dinfo.output_height)
    {
      jpeg_read_scanlines(&dinfo, line, 1);

      memcpy((XBYTE*)&buffer[index],(XBYTE*)(*line),rowstride);
      index-=rowstride;
    }

  jpeg_finish_decompress(&dinfo);

  jpeg_destroy_decompress(&dinfo);

  GetPrimaryFile()->SetPosition(0);
  delete(file_buffer);
  return true;
}




/*-------------------------------------------------------------------
//  GRPFILEBITMAPJPG::Write
*/
/**
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/11/2013 12:19:46
//
//  @return       bool :
//  @param        width :
//  @param        height :
//  @param        buffer :
//  @param        quality :
//  @param        baseline :
*/
/*-----------------------------------------------------------------*/
bool GRPFILEBITMAPJPG::Write(int width, int height,  XBYTE* buffer, int quality, bool baseline)
{
  if(!buffer)           return false;
  if(!GetPrimaryFile()) return false;

  struct jpeg_error_mgr jerr;

  JSAMPROW rowpointer[1]; /* pointer to JSAMPLE row[s] */
  int      rowstride;     /* physical row width in image buffer */

  GetPrimaryFile()->SetPosition(0);

  cinfo.err = jpeg_std_error(&jerr);

  jpeg_create_compress(&cinfo);

  jpeg_stdio_dest(&cinfo, GetPrimaryFile()->GetFileStructHandle());

  cinfo.image_width       = width;          /* image width and height, in pixels */
  cinfo.image_height      = height;
  cinfo.input_components  = 3;              /* # of color components per pixel */
  cinfo.in_color_space    = JCS_RGB;        /* colorspace of input image */

  jpeg_set_defaults(&cinfo);

  jpeg_set_quality(&cinfo, quality, baseline);

  jpeg_start_compress(&cinfo, TRUE);

  rowstride = width * 3;  /* JSAMPLEs per row in image_buffer */

  int size  = width * height * 3;

  size -= rowstride;

  while (cinfo.next_scanline < cinfo.image_height)
    {
      rowpointer[0] = &buffer[size-(cinfo.next_scanline * rowstride)];

      jpeg_write_scanlines(&cinfo, rowpointer, 1);
    }

  jpeg_finish_compress(&cinfo);

  jpeg_destroy_compress(&cinfo);

  GetPrimaryFile()->SetPosition(0);

  return true;
}


