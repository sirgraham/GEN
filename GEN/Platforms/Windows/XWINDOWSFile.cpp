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

#include "XWINDOWSFile.h"
#include "XDebug.h"

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
	if(isopen) this->Close();

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
	bool status = ExtendedOpen(xpath, (isreadonly)? XWINDOWSFILEMODE_READONLY: XWINDOWSFILEMODE_READWRITE);	
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
	this->isreadonly = isreadonly;

  return ExtendedOpen(xpath, (isreadonly)? XWINDOWSFILEMODE_READONLY: XWINDOWSFILEMODE_READWRITE);
	
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
	this->isreadonly = false;

	return ExtendedOpen(xpath, XWINDOWSFILEMODE_CREATE);	
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
  if(!isopen)										return false; 
	if(!filehandle)								return false;
	if(!SetPosition(size))				return false;
	if(!SetEndOfFile(filehandle)) return false;

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
		
	position			= SetFilePointer(filehandle, 0, NULL, FILE_CURRENT);
	if(position == INVALID_SET_FILE_POINTER) return false;

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

	if(_position == XFILE_SEEKEND) _position = cachesize;
	if(_position > cachesize) return false;

	if(SetFilePointer(filehandle,_position,0, FILE_BEGIN) == INVALID_SET_FILE_POINTER) return false;  

	return true;
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

	DWORD  br;
  
	if(!ReadFile(filehandle, buffer, size, &br, NULL)) return false;
  if(size!=br)  return false;

	if(cipher) cipher->Uncipher(buffer, br);
	
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

	DWORD br = 0;  
	bool	status = true;

	if(!ReadFile(filehandle, buffer, (*size), &br, NULL)) 
		{
			(*size)= br;		
			return false;	
		}

  if(br!=(*size))  status=false;

	(*size)= br;

	if(cipher) 
		{
			if(cipher->Uncipher(buffer, br))
				{
					memcpy(buffer, cipher->GetResult()->Get(), br);

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
	
	DWORD bw;

	if(cipher) 
		{		
			if(!cipher->Cipher(buffer,size)) return false;
			if(!WriteFile(filehandle,(void *)cipher->GetResult()->Get(), size,&bw, NULL)) return false;
			if(bw!=size) return false;			
		}
	 else
	  {
			if(!WriteFile(filehandle,(void *)buffer, size,&bw, NULL)) return false;
			if(bw!=size) return false;						
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
	
	if(!FlushFileBuffers(filehandle)) return false;
  
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

	if(!isreadonly) Flush();

	if(filehandle) 
		{			
				CloseHandle(filehandle);

			#ifdef XDEBUG
			XFileOpenList.Del(this);
			#endif


			isopen     = false;
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
	
	return true;
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
	if(!MoveFile(xpathold, xpathnew)) return false;
  
	return true;
}



/*-------------------------------------------------------------------
//	XWINDOWSFILE::GetFileStructHandle
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			13/06/2014 16:21:30
//	
//	@return 			FILE* : 
//
*/
/*-----------------------------------------------------------------*/
FILE* XWINDOWSFILE::GetFileStructHandle()
{	

		if (fp != NULL) return fp;

	switch(mode)
		{
			case XWINDOWSFILEMODE_READONLY	: fd = _open_osfhandle((intptr_t)filehandle, _O_RDONLY);
																				if (fd == -1) return NULL;																				
																				fp = _fdopen(fd, "rb");
																				break;

			case XWINDOWSFILEMODE_READWRITE	: fd = _open_osfhandle((intptr_t)filehandle, _O_RDWR);
																				if (fd == -1) return NULL;																				
																				fp = _fdopen(fd, "r+b");
																				break;

			case XWINDOWSFILEMODE_CREATE		: fd = _open_osfhandle((intptr_t)filehandle, _O_CREAT);
																				if (fd == -1) return NULL;																			
																				fp = _fdopen(fd, "w+b");
																				break;
		}		
	return fp;
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
			
	if(!isreadonly)
		{
			if(!Flush()) return false;
		}

	LARGE_INTEGER exsize;
	if(GetFileSizeEx(filehandle, &exsize) == INVALID_FILE_SIZE) return false;

	cachesize = exsize.LowPart;

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
bool XWINDOWSFILE::ExtendedOpen(XCHAR* xpath, XWINDOWSFILEMODE mode)
{
 	if(isopen) Close();

	this->xpathnamefile = xpath;
	this->mode          = mode;

	switch(mode)
		{
			case XWINDOWSFILEMODE_READONLY	: filehandle = CreateFile(xpath, GENERIC_READ								 , FILE_SHARE_READ									 , NULL, OPEN_EXISTING	, FILE_ATTRIBUTE_NORMAL, NULL);	 break;
			case XWINDOWSFILEMODE_READWRITE	: filehandle = CreateFile(xpath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING	, FILE_ATTRIBUTE_NORMAL, NULL);	 break;
			case XWINDOWSFILEMODE_CREATE		: filehandle = CreateFile(xpath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS	,	FILE_ATTRIBUTE_NORMAL, NULL);	 break;
		}
				
	if (filehandle == INVALID_HANDLE_VALUE)
	{
			XDWORD err=GetLastError();
			return false;
	}
	
	isopen = true;
  
	ActualizeSize();

	#ifdef XDEBUG
	XFileOpenList.Add(this);
	#endif
  return true;
}




