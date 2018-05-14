//------------------------------------------------------------------------------------------
//  XANDROIDFILE.CPP
//
//  ANDROID file class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 13/03/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <unistd.h>
#include <locale.h>
#include <sys/types.h>

#include "XPath.h"
#include "XString.h"
#include "XDebug.h"

#include "Cipher.h"

#include "XANDROIDFactory.h"

#include "XANDROIDFile.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XANDROIDFILE::XANDROIDFILE
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 17:06:59
//
//  @return
//  @param        void :
*/
//-------------------------------------------------------------------
XANDROIDFILE::XANDROIDFILE(): XFILE()
{
  Clean();

  xfactory = new XANDROIDFACTORY();
}



//-------------------------------------------------------------------
//  XANDROIDFILE::~XANDROIDFILE
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 17:09:50
//
//  @return       void :
//  @param        void :
*/
//-------------------------------------------------------------------
XANDROIDFILE::~XANDROIDFILE()
{
  if(isopen) this->Close();

  delete xfactory;

  Clean();
}



/*-------------------------------------------------------------------
//  XANDROIDFILE::Exist
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/06/2014 16:21:58
//
//  @return       bool :
//
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool XANDROIDFILE::Exist(XCHAR* xpath)
{
  bool status = Open(xpath, true);
  if(status) Close();

  return status;
}



/*-------------------------------------------------------------------
//  XANDROIDFILE::Open
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/06/2014 16:27:07
//
//  @return       bool :
//
//  @param        xpath :
//  @param        isreadonly :
*/
/*-----------------------------------------------------------------*/
bool XANDROIDFILE::Open(XCHAR* xpath, bool isreadonly)
{
  return ExtendedOpen(xpath, (isreadonly)? __L("rb") : __L("r+b"));
}



/*-------------------------------------------------------------------
//  XANDROIDFILE::Create
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/06/2014 16:39:47
//
//  @return       bool :
//
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool XANDROIDFILE::Create(XCHAR* xpath)
{
  return ExtendedOpen(xpath, __L("w+b"));
}





/*-------------------------------------------------------------------
//  XANDROIDFILE::SetSize
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/06/2014 16:21:28
//
//  @return       bool :
//
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool XANDROIDFILE::SetSize(int size)
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  if(ftruncate(fileno(filehandle) , size)) return false;

  ActualizeSize();

  return true;
}




/*-------------------------------------------------------------------
//  XANDROIDFILE::GetPosition
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/06/2014 16:58:56
//
//  @return       bool :
//
//  @param        position :
*/
/*-----------------------------------------------------------------*/
bool XANDROIDFILE::GetPosition(int& position)
{
  if(!isopen)     return false;
  if(!filehandle) return false;

  position = ftell(filehandle);

  if(position == -1) return false;

  return true;
}




/*-------------------------------------------------------------------
//  XANDROIDFILE::SetPosition
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/06/2014 17:11:15
//
//  @return       bool :
//
//  @param        position :
*/
/*-----------------------------------------------------------------*/
bool XANDROIDFILE::SetPosition(int position)
{
  if(!isopen)     return false;
  if(!filehandle) return false;

  int  _position    = position;
  bool status;

  if(_position == XFILE_SEEKEND) _position = cachesize;
  if(_position > cachesize) return false;

  status = fseek(filehandle, _position, SEEK_SET)?false:true;

  return status;
}




//-------------------------------------------------------------------
//  XANDROIDFILE::Read
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/07/2002 13:15:07
//
//  @return       bool :
//  @param        buffer :
//  @param        size :
//  @param        cipher :
*/
//-------------------------------------------------------------------
bool XANDROIDFILE::Read(XBYTE* buffer, int size, CIPHER* cipher)
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  int _size;

  _size=(int)fread((void *)buffer,1,(size_t)size, filehandle);
  if(_size!=size)  return false;

  if(cipher) cipher->Uncipher(buffer,_size);

  return true;
}





//-------------------------------------------------------------------
//  XANDROIDFILE::Read
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/07/2002 13:16:17
//
//  @return       bool :
//  @param        buffer :
//  @param        size :
//  @param        cipher :
*/
//-------------------------------------------------------------------
bool XANDROIDFILE::Read(XBYTE* buffer, int* size, CIPHER* cipher)
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

   int   _size;
  bool status = true;

  _size=(int)fread((void *)buffer, (size_t)1, (size_t)(*size), filehandle);

  if(_size!=(*size))  status=false;

  (*size)=_size;

  if(cipher)
    {
      if(cipher->Uncipher(buffer,_size))
        {
          memcpy(buffer, cipher->GetResult()->Get(), _size);

        } else status = false;
    }

  return status;
}





//-------------------------------------------------------------------
//  XANDROIDFILE::Write
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/07/2002 13:16:40
//
//  @return       bool :
//  @param        buffer :
//  @param        size :
//  @param        cipher :
*/
//-------------------------------------------------------------------
bool XANDROIDFILE::Write(XBYTE* buffer, int size, CIPHER* cipher)
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  if(cipher)
    {
      if(!cipher->Cipher(buffer,size)) return false;
      if(fwrite((void *)cipher->GetResult()->Get(), 1, size, filehandle)!=(unsigned)size)  return false;
    }
   else
    {
      if(fwrite((void *)buffer,1,size, filehandle)!=(unsigned)size)  return false;
    }

  ActualizeSize();

  return true;
}



//-------------------------------------------------------------------
//  XANDROIDFILE::Flush
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/04/2000 12:25:42
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XANDROIDFILE::Flush()
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  fflush(filehandle);

  return true;
}




//-------------------------------------------------------------------
//  XANDROIDFILE::Close
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 17:12:38
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool XANDROIDFILE::Close()
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  Flush();

  if(filehandle)
    {
      fclose(filehandle);

      //XDEBUG_PRINTCOLOR(1,__L("Close File: %s"), xpathnamefile.Get());

      #ifdef XDEBUG
      XFileOpenList.Del(this);
      #endif

      filehandle = NULL;
    }

  return true;
}




//-------------------------------------------------------------------
//  XANDROIDFILE::Erase
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 17:10:52
//
//  @return       bool :
//  @param        pathname :
//  @param        overwrite :
*/
//-------------------------------------------------------------------
bool XANDROIDFILE::Erase(XCHAR* xpath,bool overwrite)
{
  if(!xfactory)           return false;

  if(Exist(xpath)!=true)  return false;

  bool status = true;

  if(overwrite) status = OverwriteContent(xpath);

  XBUFFER _xpathbuffer;
  XPATH   _xpath;

  _xpath = xpath;

  _xpath.ConvertToUTF8(_xpathbuffer);

  if(unlink((char*)_xpathbuffer.Get())==-1) status=false;

  return status;
}





/*-------------------------------------------------------------------
//  XANDROIDFILE::Rename
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2011 10:09:39
//
//  @return       bool :
//  @param        xpathold :
//  @param        xpathnew :
*/
/*-----------------------------------------------------------------*/
bool XANDROIDFILE::Rename(XCHAR* xpathold, XCHAR* xpathnew)
{
  if(!xfactory) return false;

  XBUFFER _xpathbufferold;
  XBUFFER _xpathbuffernew;
  XPATH   _xpathold;
  XPATH   _xpathnew;

  _xpathold = xpathold;
  _xpathnew = xpathnew;

  _xpathold.ConvertToUTF8(_xpathbufferold);
  _xpathnew.ConvertToUTF8(_xpathbuffernew);

  bool status  = rename((char*)_xpathbufferold.Get(), (char*)_xpathbuffernew.Get());

  return (!status)?true:false;
}



/*-------------------------------------------------------------------
//  XANDROIDFILE::ActualizeSize
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/06/2014 17:59:09
//
//  @return       int :
//
*/
/*-----------------------------------------------------------------*/
bool XANDROIDFILE::ActualizeSize()
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  Flush();

  int position = ftell(filehandle);

  fseek(filehandle, 0, SEEK_END);
  cachesize = ftell(filehandle);
  fseek(filehandle, position, SEEK_SET);

  return true;
}





/*-------------------------------------------------------------------
//  XANDROIDFILE::ExtendedOpen
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/06/2014 16:55:39
//
//  @return       bool :
//
//  @param        xpath :
//  @param        mode :
*/
/*-----------------------------------------------------------------*/
bool XANDROIDFILE::ExtendedOpen(XCHAR* xpath, XCHAR* mode)
{
  if(!xfactory) return false;

  if(isopen) Close();

  this->xpathnamefile = xpath;

  XPATH   _xpath;
  XSTRING _mode;
  XBUFFER _xpathbuffer;
  XBUFFER _modebuffer;
  bool    status = false;

  _xpath  = xpath;
  _mode   = mode;

  _xpath.ConvertToUTF8(_xpathbuffer); _xpathbuffer.Add((XDWORD)0);
  _mode.ConvertToUTF8(_modebuffer);   _modebuffer.Add((XDWORD)0);

  filehandle = fopen((char*)_xpathbuffer.Get(), (char*)_modebuffer.Get());
  if(filehandle)
    {
      isopen = true;

      ActualizeSize();
      status = true;

      //XDEBUG_PRINTCOLOR(1,__L("Open File: %s"), xpathnamefile.Get());

      #ifdef XDEBUG
      XFileOpenList.Add(this);
      #endif
    }

  return status;
}




