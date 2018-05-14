
/*------------------------------------------------------------------------------------------
//  GRPTEXTUREEXT.CPP
//
//  Texture Xtended version
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 19/04/2016 13:17:46
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPTextureEXT.h"
#include "GRPOpenGL.h"
#include "GRPFactory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  GRPTEXTUREEXT::GRPTEXTUREEXT
*/
/**
//
//  Class Constructor GRPTEXTUREEXT
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/06/2016 16:58:20
//
*/
/*-----------------------------------------------------------------*/
GRPTEXTUREEXT::GRPTEXTUREEXT() : GRPTEXTURE()
{
  Clean();
}




/*-------------------------------------------------------------------
//  GRPTEXTUREEXT::~GRPTEXTUREEXT
*/
/**
//
//   Class Destructor GRPTEXTUREEXT
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/06/2016 16:58:13
//
*/
/*-----------------------------------------------------------------*/
GRPTEXTUREEXT::~GRPTEXTUREEXT()
{
  Clean();
}




/*-------------------------------------------------------------------
//  GRPTEXTUREEXT::Create
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/06/2016 16:58:29
//
*/
/*-----------------------------------------------------------------*/
void GRPTEXTUREEXT::Create()
{
  GLUINT handle;
  glGenTextures(1, &handle);
  this->handles.Add(handle);
}




/*-------------------------------------------------------------------
//  GRPTEXTUREEXT::Create
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/06/2016 16:58:36
//
//  @param        xbuffer :
//  @param        width :
//  @param        height :
//  @param        format :
//  @param        filtering :
*/
/*-----------------------------------------------------------------*/
void GRPTEXTUREEXT::Create(XBUFFER* xbuffer, XDWORD width, XDWORD height, GRPTEXTURE_FORMAT format, GRPTEXTURE_FILTERING filtering)
{
  this->Create();

  this->Resize(width, height);
  this->SetFormat(format);
  this->SetFiltering(filtering);

  this->LoadToGPU(xbuffer);
}




/*-------------------------------------------------------------------
//  GRPTEXTUREEXT::Resize
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/06/2016 16:58:44
//
//  @param        width :
//  @param        height :
*/
/*-----------------------------------------------------------------*/
void GRPTEXTUREEXT::Resize(XDWORD width, XDWORD height)
{
  this->width   = width;
  this->height  = height;
}





/*-------------------------------------------------------------------
//  GRPTEXTUREEXT::SetFormat
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/06/2016 16:58:52
//
//  @param        format :
*/
/*-----------------------------------------------------------------*/
void GRPTEXTUREEXT::SetFormat(GRPTEXTURE_FORMAT format)
{
  this->format = format;
}




/*-------------------------------------------------------------------
//  GRPTEXTUREEXT::LoadToGPU
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/06/2016 16:59:00
//
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
void GRPTEXTUREEXT::LoadToGPU(XBUFFER* xbuffer)
{
  // having UNSIGNED BYTE there has a possible conflict with floating point texture formats
  isloaded = true;


  //DIEGO: having this code duplicated is idiotical and bug prone
  this->Bind(0);

  void* ptr = NULL;

  if(xbuffer)
  {
    ptr = xbuffer->Get();
  }

  GLUINT tmpformat = grpfactory->GetTextureFormat(format);
  // these formats need to be balanced with the gl version
  glTexImage2D(GL_TEXTURE_2D, 0, tmpformat, width, height, 0, tmpformat, GL_UNSIGNED_BYTE, ptr); // need to be careful with the unsigned byte, int, float part

  GLUINT f = GL_LINEAR;

  if(filtering == GRPTEXTURE_FILTERING_NEAREST)
    {
      f = GL_NEAREST;
    }

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, f);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, f);
}





/*-------------------------------------------------------------------
//  GRPTEXTUREEXT::ReadFromGPU
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/06/2016 16:59:07
//
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
void GRPTEXTUREEXT::ReadFromGPU(XBUFFER* xbuffer)
{
  if(!xbuffer) return;
  // I'll take for granted that whoever calls this function knows what he's doing
  // commented for now, need to consider both desktop and ES
  //glGetTexImage(GL_TEXTURE_2D, 0, grpfactory->GetTextureFormat(format), GL_UNSIGNED_BYTE, xbuffer->Get());
}



/*-------------------------------------------------------------------
//  GRPTEXTUREEXT::Bind
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/06/2016 16:59:15
//
//  @param        unit :
*/
/*-----------------------------------------------------------------*/
void GRPTEXTUREEXT::Bind(XDWORD unit)
{
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, GetHandle());

  GRPOPENGLCTRL::TEXTURE::units[unit].handle =0;
}





/*-------------------------------------------------------------------
//  GRPTEXTUREEXT::SetFiltering
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/06/2016 16:59:21
//
//  @param        filtering :
*/
/*-----------------------------------------------------------------*/
void GRPTEXTUREEXT::SetFiltering(GRPTEXTURE_FILTERING filtering)
{
  this->filtering = filtering;
}




/*-------------------------------------------------------------------
//  GRPTEXTUREEXT::GetHandle
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/06/2016 16:59:28
//
//  @return       GRPTEXTUREHANDLE :
//
*/
/*-----------------------------------------------------------------*/
GRPTEXTUREHANDLE GRPTEXTUREEXT::GetHandle()
{
  return GRPTEXTURE::GetHandle();
}
