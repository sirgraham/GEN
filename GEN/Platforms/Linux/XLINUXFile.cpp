//------------------------------------------------------------------------------------------
//  XLINUXFILE.CPP
//
//  LINUX file class
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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "XPath.h"
#include "XString.h"

#include "XDebug.h"

#include "Cipher.h"

#include "XLINUXFactory.h"

#include "XLINUXFile.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XLINUXFILE::XLINUXFILE
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
XLINUXFILE::XLINUXFILE(): XFILE()
{
  Clean();

}



//-------------------------------------------------------------------
//  XLINUXFILE::~XLINUXFILE
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
XLINUXFILE::~XLINUXFILE()
{
  if(isopen) this->Close();

  Clean();
}



/*-------------------------------------------------------------------
//  XLINUXFILE::Exist
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
bool XLINUXFILE::Exist(XCHAR* xpath)
{
  bool status = Open(xpath, true);
  if(status) Close();

  return status;
}



/*-------------------------------------------------------------------
//  XLINUXFILE::Open
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
bool XLINUXFILE::Open(XCHAR* xpath, bool isreadonly)
{
  return ExtendedOpen(xpath, (isreadonly)? __L("rb") : __L("r+b"));
}



/*-------------------------------------------------------------------
//  XLINUXFILE::Create
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
bool XLINUXFILE::Create(XCHAR* xpath)
{
  return ExtendedOpen(xpath, __L("w+b"));
}





/*-------------------------------------------------------------------
//  XLINUXFILE::SetSize
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
bool XLINUXFILE::SetSize(int size)
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  if(ftruncate(fileno(filehandle) , size)) return false;

  ActualizeSize();

  return true;
}




/*-------------------------------------------------------------------
//  XLINUXFILE::GetPosition
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
bool XLINUXFILE::GetPosition(int& position)
{
  if(!isopen)     return false;
  if(!filehandle) return false;

  position = ftell(filehandle);
  if(position == -1) return false;

  return true;
}




/*-------------------------------------------------------------------
//  XLINUXFILE::SetPosition
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
bool XLINUXFILE::SetPosition(int position)
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
//  XLINUXFILE::Read
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
bool XLINUXFILE::Read(XBYTE* buffer, int size, CIPHER* cipher)
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
//  XLINUXFILE::Read
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
bool XLINUXFILE::Read(XBYTE* buffer, int* size, CIPHER* cipher)
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
//  XLINUXFILE::Write
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
bool XLINUXFILE::Write(XBYTE* buffer, int size, CIPHER* cipher)
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
//  XLINUXFILE::Flush
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/04/2000 12:25:42
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XLINUXFILE::Flush()
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  fflush(filehandle);

  return true;
}




//-------------------------------------------------------------------
//  XLINUXFILE::Close
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 17:12:38
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool XLINUXFILE::Close()
{
  if(!isopen)       return false;
  if(!filehandle)   return false;

  Flush();

  if(filehandle)
    {
      fclose(filehandle);

      #ifdef XDEBUG
      XFileOpenList.Del(this);
      #endif

      filehandle = NULL;
    }

  return true;
}




//-------------------------------------------------------------------
//  XLINUXFILE::Erase
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
bool XLINUXFILE::Erase(XCHAR* xpath,bool overwrite)
{
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
//  XLINUXFILE::Rename
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
bool XLINUXFILE::Rename(XCHAR* xpathold, XCHAR* xpathnew)
{
  XBUFFER _xpathbufferold;
  XBUFFER _xpathbuffernew;
  XPATH   _xpathold;
  XPATH   _xpathnew;

  _xpathold = xpathold;
  _xpathnew = xpathnew;

  _xpathold.ConvertToUTF8(_xpathbufferold);
  _xpathnew.ConvertToUTF8(_xpathbuffernew);

  // need to manually reset errno as no call is going to do it
  errno = 0;
  int status  = rename((char*)_xpathbufferold.Get(), (char*)_xpathbuffernew.Get());
  if (status != 0)
    {
      XSTRING err;
      err.Set(strerror(errno));
      XDEBUG_PRINTCOLOR(4,__L("Error renaming file %s -> %s : %s"),xpathold,xpathnew,err.Get());
    }

  return (!status)?true:false;
}




/*-------------------------------------------------------------------
//  XLINUXFILE::ChangeAttributes
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/10/2016 16:48:00
//
//  @param        attributes :
*/
/*-----------------------------------------------------------------*/
void XLINUXFILE::ChangeAttributes(XCHAR* attributes)
{
  XSTRING mode;
  mode.Set(attributes);
  XSTRING_CREATEOEM(mode, charOEM0)
  int i = strtol(charOEM0, 0, 8);
  XSTRING_DELETEOEM(charOEM0)

  XSTRING str(xpathnamefile.Get());
  XSTRING_CREATEOEM(str, charOEM)
  chmod(charOEM, i);
  XSTRING_DELETEOEM(charOEM)
}




/*-------------------------------------------------------------------
//  XLINUXFILE::ActualizeSize
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
bool XLINUXFILE::ActualizeSize()
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
//  XLINUXFILE::ExtendedOpen
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
bool XLINUXFILE::ExtendedOpen(XCHAR* xpath, XCHAR* mode)
{
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

      #ifdef XDEBUG
      XFileOpenList.Add(this);
      #endif
    }

  return status;








}




