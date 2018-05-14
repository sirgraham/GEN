
/*------------------------------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFER.CPP
//
//  LINUX graphics screen class
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 18/03/2014 10:39:29
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>

#include "XDebug.h"

#include "GRPLinuxScreenFrameBuffer.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFER::GRPLINUXSCREENFRAMEBUFFER
*/
/**
//
//  Class Constructor GRPLINUXSCREENFRAMEBUFFER
//
//  @author       Abraham J. Velez
//  @version      21/05/2014 15:58:49
//


*/
/*-----------------------------------------------------------------*/
GRPLINUXSCREENFRAMEBUFFER::GRPLINUXSCREENFRAMEBUFFER() : GRPSCREEN()
{
  Clean();

  type = GRPSCREENTYPE_LINUX_FRAMEBUFFER;

  handlefb = open(GRPLINUXSCREENFRAMEBUFFER_DEVICE, O_RDWR);
  if(handlefb!=-1)
    {
      struct fb_var_screeninfo varinfo;
      struct fb_fix_screeninfo fixinfo;
      int                      width  = 0;
      int                      height = 0;

      if((ioctl(handlefb, FBIOGET_FSCREENINFO, &fixinfo) != -1) && (ioctl(handlefb, FBIOGET_VSCREENINFO, &varinfo) != -1))
        {
          SetWidth(varinfo.xres);
          SetHeight(varinfo.yres);

          switch(varinfo.bits_per_pixel)
            {
              case   8 : SetMode(GRPPROPERTYMODE_08_INDEX);     break;
              case  16 : SetMode(GRPPROPERTYMODE_16_RGB_565);   break;
              case  24 : SetMode(GRPPROPERTYMODE_24_BGR_888);   break;
              case  32 : SetMode(GRPPROPERTYMODE_32_BGRA_8888); break;
            }

          XDEBUG_PRINTCOLOR(0, __L("X: %d, Y: %d  mode: %d (%d) %d"), varinfo.xres, varinfo.yres, GetMode(), varinfo.bits_per_pixel,this->GetBytesperPixel());
        }
    }
}



/*-------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFER::~GRPLINUXSCREENFRAMEBUFFER
*/
/**
//
//   Class Destructor GRPLINUXSCREENFRAMEBUFFER
//
//  @author       Abraham J. Velez
//  @version      27/03/2014 13:43:07
//
*/
/*-----------------------------------------------------------------*/
GRPLINUXSCREENFRAMEBUFFER::~GRPLINUXSCREENFRAMEBUFFER()
{
  Delete();

  if(handlefb!=-1) close(handlefb);

  Clean();
}



/*-------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFER::Create
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      16/04/2014 17:53:29
//
//  @return       bool :
//
//  @param        handle :
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENFRAMEBUFFER::Create(void* handle)
{
  isactive = true;

  buffersize = GetWidth() * GetHeight() * GetBytesperPixel();

  XDEBUG_PRINTCOLOR(0, __L("Creating Buffer: %d bytes"), buffersize);

  buffer = new XBYTE[buffersize];

  XDEBUG_PRINTCOLOR(1,__L("memory located"));

  if(buffer == NULL) return false;

  return true;
}



/*-------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFER::Update
*/
/**
//
//  refreshes the buffer on screen
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/03/2014 10:47:12
//
//  @return       bool : true = success
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENFRAMEBUFFER::Update  ()
{
  if(handlefb == -1) return false;

  XBYTE* fbp = (XBYTE *)mmap(0, buffersize, PROT_READ | PROT_WRITE, MAP_SHARED, handlefb, 0);
  if((int)fbp == -1)  return false;

  munmap(fbp, buffersize);

  // XDEBUG_PRINTCOLOR(1,__L("update memory %d"),buffersize);

  return true;
}




/*-------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFER::Delete
*/
/**
//
//  Sets the Buffer has inactive for destruction
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/03/2014 10:47:45
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENFRAMEBUFFER::Delete()
{
  if(buffer!=NULL) delete(buffer);
  isactive = false;

  return true;
}



/*-------------------------------------------------------------------
//  GRPLINUXSCREENFRAMEBUFFER::Clean
*/
/**
//
//  Disables the handle and cleans
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/03/2014 10:48:05
//
*/
/*-----------------------------------------------------------------*/
void GRPLINUXSCREENFRAMEBUFFER::Clean()
{
  handlefb  = -1;
  buffer    = NULL;
}


