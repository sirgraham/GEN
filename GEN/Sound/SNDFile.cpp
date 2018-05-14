/*------------------------------------------------------------------------------------------
//  SNDFILE.CPP
//
//  Generic sound file class
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 05/11/2015 9:16:24
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "SNDFile.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  SNDFILEOGG::WriteRaw
*/
/**
//
//  writes a decoded ogg file in raw form
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      03/11/2015 13:21:05
//
//  @return       bool :
//
//  @param        path :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool SNDFILE::WriteRaw(XCHAR* path, XCHAR* name)
{
  XFILE* xfile = NULL;

  xfile = xfactory->Create_File();
  if(!xfile)
    {
      return false;
    }

  if(!xfile->Open(path, false))
    {
      if(!xfile->Create(path))
        {
          delete(xfile);
          return false;
        }
    }

  xfile->Write(*xbufferdecodeddata);

  return true;
}




/*-------------------------------------------------------------------
//  SNDFILEOGG::WriteRaw
*/
/**
//
//  writes a decoded ogg file in raw form
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      03/11/2015 13:21:22
//
//  @return       bool :
//
//  @param        xpath :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool SNDFILE::WriteRaw(XPATH& xpath, XCHAR* name)
{
  return WriteRaw(xpath.Get(), name);
}
