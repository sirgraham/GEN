

/*------------------------------------------------------------------------------------------
//  SNDFILEOGG.CPP
//
//  Loads and decodes an ogg file
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 03/11/2015 10:40:46
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XDebugTrace.h"

#include "stb_vorbis.h"

#include "SNDFileOGG.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  SNDFILEOGG::LoadFile
*/
/**
//
//  loads an ogg file from disk and decodes in memory
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      03/11/2015 13:20:03
//
//  @return       bool :
//
//  @param        path :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool SNDFILEOGG::LoadFile(XCHAR* path, XCHAR* name, bool streaming)
{
  XFILE* xfile = NULL;

  xbuffer = new XBUFFER(false);
  if(!xbuffer)
    {
      return false;
    }

  bool    status = false;

  xfile = xfactory->Create_File();
  if(!xfile)
    {
      xbuffer->Delete();
      delete xbuffer;
      xbuffer = NULL;
      return false;
    }

  if(!xfile->Open(path))
    {
      xbuffer->Delete();
      delete xbuffer;
      xbuffer = NULL;

      delete xfile;
      xfile = NULL;

      return false;
    }

  //XDEBUGTRACE_PRINTCOLOR(0,__L("Loading file %s"), xfile->GetPathNameFile());

  xbuffer->Resize(xfile->GetSize());
  status = xfile->Read(xbuffer->Get(), xbuffer->GetSize());
  xfile->Close();

  xfactory->Delete_File(xfile);

  // now perform decoding
  stb_vorbis_info info;

  stream = stb_vorbis_open_memory(xbuffer->Get(), xbuffer->GetSize(), NULL, NULL);
  if(!stream)
    {
     XDEBUGTRACE_PRINTCOLOR(4,__L("File Load Failed"), xfile->GetPathNameFile());
      return false; // need to check the specific error
    }

  info = stb_vorbis_get_info(stream);

  channels = info.channels;
  samplerate = info.sample_rate;
  samples = stb_vorbis_stream_length_in_samples(stream)*channels; // this product is possibly redundant

  duration = stb_vorbis_stream_length_in_seconds(stream);

  xbufferdecodeddata=new XBUFFER(false);
  if(!xbufferdecodeddata)
    {
      return false;
    }

  xbufferdecodeddata->Resize(samples*2); // we must multiply by 2 because ogg expects shorts

  // perform the actual decoding
  int read = stb_vorbis_get_samples_short_interleaved(stream, channels, (short*)(xbufferdecodeddata->Get()), samples);

  // try to deallocate the allocacated memory
  stb_vorbis_close(stream);

  this->name.Set(name);

  return true;
}




/*-------------------------------------------------------------------
//  SNDFILEOGG::LoadFile
*/
/**
//
//  loads an ogg file from disk and decodes in memory
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      03/11/2015 13:20:27
//
//  @return       bool :
//
//  @param        xpath :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool SNDFILEOGG::LoadFile(XPATH& xpath, XCHAR* name, bool streaming)
{
  return LoadFile(xpath.Get(), name, streaming);
}



