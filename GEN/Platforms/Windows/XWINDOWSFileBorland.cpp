//------------------------------------------------------------------------------------------
//	XWINDOWSFILE.CPP
//	
//	WINDOWS file class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 13/03/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------

	
//---- INCLUDES ----------------------------------------------------------------------------

#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>    
#include <share.h>

#include "XPath.h"
#include "XString.h"

#include "Cipher.h"

#include "XWINDOWSFileBorland.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XWINDOWSFILE::XWINDOWSFILE
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:06:59
//	
//	@return 			
//	@param				void : 
*/
//-------------------------------------------------------------------
XWINDOWSFILE::XWINDOWSFILE(): XFILE()
{
	Clean();
}



//-------------------------------------------------------------------
//  XWINDOWSFILE::~XWINDOWSFILE
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:09:50
//	
//	@return 			void : 
//	@param				void : 
*/
//-------------------------------------------------------------------
XWINDOWSFILE::~XWINDOWSFILE()
{
	Clean();
}



/*-------------------------------------------------------------------
//	XWINDOWSFILE::Exist
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/06/2014 16:21:58
//	
//	@return 			bool : 
//
//  @param				xpath : 
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSFILE::Exist(XCHAR* xpath)
{	
	bool status = Open(xpath, true);
	if(status) Close();

  return status;
}



/*-------------------------------------------------------------------
//	XWINDOWSFILE::Open
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/06/2014 16:27:07
//	
//	@return 			bool : 
//
//  @param				xpath : 
//  @param				isreadonly : 
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSFILE::Open(XCHAR* xpath, bool isreadonly)
{	
  return ExtendedOpen(xpath, (isreadonly)? __L("rb") : __L("r+b"));
}



/*-------------------------------------------------------------------
//	XWINDOWSFILE::Create
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/06/2014 16:39:47
//	
//	@return 			bool : 
//
//  @param				xpath : 
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSFILE::Create(XCHAR* xpath)
{
	return ExtendedOpen(xpath, __L("w+b"));	
}





/*-------------------------------------------------------------------
//	XWINDOWSFILE::SetSize
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/06/2014 16:21:28
//	
//	@return 			bool : 
//
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSFILE::SetSize(int size)
{  
  if(!isopen)				return false; 
	if(!filehandle)		return false;
	
	#ifdef XVISUALC
  if(_chsize(_fileno(filehandle),size)) return false;
	#else
	if(chsize(_fileno(filehandle),size)) return false;
	#endif
    
	ActualizeSize();
  
  return true;	
}




/*-------------------------------------------------------------------
//	XWINDOWSFILE::GetPosition
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/06/2014 16:58:56
//	
//	@return 			bool : 
//
//  @param				position : 
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSFILE::GetPosition(int& position)
{  
	if(!isopen)			return false;
	if(!filehandle) return false; 
		
  position = ftell(filehandle);
	
  if(position == -1) return false;
	
	return true;
}




/*-------------------------------------------------------------------
//	XWINDOWSFILE::SetPosition
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/06/2014 17:11:15
//	
//	@return 			bool : 
//
//  @param				position : 
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSFILE::SetPosition(int position)
{
  if(!isopen)			return false;
	if(!filehandle) return false; 

  int	 _position		=	position;
	bool status;

	if(_position == XFILE_SEEKEND) _position = cachesize;

	if(_position > cachesize) return false;
	
	status=fseek(filehandle, _position, SEEK_SET)?false:true; 	
  
  return status;     
}




//-------------------------------------------------------------------
//  XWINDOWSFILE::Read
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/07/2002 13:15:07
//	
//	@return 			bool : 
//	@param				buffer : 
//  @param				size :
//  @param				cipher : 
*/
//-------------------------------------------------------------------
bool XWINDOWSFILE::Read(XBYTE* buffer, int size, CIPHER* cipher)
{
	if(!isopen)				return false; 
	if(!filehandle)		return false;

  int _size;

	_size=(int)fread((void *)buffer,1,(size_t)size, filehandle);
  if(_size!=size)  return false;

	if(cipher) cipher->Uncipher(buffer,_size);
		  
	return true;
}





//-------------------------------------------------------------------
//  XWINDOWSFILE::Read
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/07/2002 13:16:17
//	
//	@return 			bool : 
//	@param				buffer : 
//  @param				size : 
//  @param				cipher : 
*/
//-------------------------------------------------------------------
bool XWINDOWSFILE::Read(XBYTE* buffer, int* size, CIPHER* cipher)
{
  if(!isopen)				return false; 
	if(!filehandle)		return false;

   int	 _size;
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
//  XWINDOWSFILE::Write
/**
//
//
//	@author				Abraham J. Velez
//	@version			10/07/2002 13:16:40
//
//	@return 			bool :
//	@param				buffer :
//  @param				size :
//  @param				cipher :
*/
//-------------------------------------------------------------------
bool XWINDOWSFILE::Write(XBYTE* buffer, int size, CIPHER* cipher)
{
	if(!isopen)				return false; 
	if(!filehandle)		return false;

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
//  XWINDOWSFILE::Flush
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/04/2000 12:25:42
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool XWINDOWSFILE::Flush()
{
	if(!isopen)				return false; 
	if(!filehandle)		return false;

	fflush(filehandle);

	return true;
}




//-------------------------------------------------------------------
//  XWINDOWSFILE::Close
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:12:38
//
//	@return 			bool : 
*/
//-------------------------------------------------------------------
bool XWINDOWSFILE::Close()
{  
	if(!isopen)				return false; 
	if(!filehandle)		return false;

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
//  XWINDOWSFILE::Erase
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:10:52
//	
//	@return 			bool :
//	@param				pathname :
//  @param				overwrite :
*/
//-------------------------------------------------------------------
bool XWINDOWSFILE::Erase(XCHAR* xpath,bool overwrite)
{
	bool status=true; 

  if(Exist(xpath)!=true)  return false;	

	if(overwrite) status = OverwriteContent(xpath);
			
	if(status) status = DeleteFile(xpath)?true:false;

	return status;	
}





/*-------------------------------------------------------------------
//  XWINDOWSFILE::Rename
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/04/2011 10:09:39
//	
//	@return				bool : 
//	@param				xpathold : 
//  @param				xpathnew : 
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSFILE::Rename(XCHAR* xpathold, XCHAR* xpathnew)
{
	if(!_wrename(xpathold, xpathnew)) return true;

	return false;
}



/*-------------------------------------------------------------------
//	XWINDOWSFILE::ActualizeSize
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/06/2014 17:59:09
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSFILE::ActualizeSize()
{
	if(!isopen)				return false;
 	if(!filehandle)   return false;
				
	Flush();

	int position = ftell(filehandle);

	fseek(filehandle, 0, SEEK_END);	
	cachesize = ftell(filehandle);
	fseek(filehandle, position, SEEK_SET);
			
	return true;	
}





/*-------------------------------------------------------------------
//	XWINDOWSFILE::ExtendedOpen
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/06/2014 16:55:39
//	
//	@return 			bool : 
//
//  @param				xpath : 
//  @param				mode : 
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSFILE::ExtendedOpen(XCHAR* xpath, XCHAR* mode)
{
	if(isopen) Close();

	this->xpathnamefile = xpath;
	
  int error = 0;

  #ifdef BUILDER
  filehandle = _wfopen(xpath, mode);
  if(!filehandle) return false;
  #else
	error = _wfopen_s(&filehandle, xpath, mode);
  #endif

  if(error)				 return false;
	if(!filehandle)  return false;
	
	isopen = true;
  
	ActualizeSize();

	#ifdef XDEBUG
	XFileOpenList.Add(this);
	#endif

  return true;
}




