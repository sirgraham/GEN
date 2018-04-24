
//------------------------------------------------------------------------------------------
//	XLINUXDEBUG.CPP
//	
//	LINUX debug class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 05/04/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------

#ifdef XDEBUG	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "XBase.h"	
#include "XBuffer.h"	

#include "XLINUXSystem.h"
#include "XLINUXDateTime.h"

#include "DIOLINUXURL.h"	

#include "XLINUXDebug.h"

	
//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XLINUXDEBUGCTRL::XLINUXDEBUGCTRL
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:03:05
//	
//	@return 			
*/
//-------------------------------------------------------------------
XLINUXDEBUGCTRL::XLINUXDEBUGCTRL() 
{	
	Clean();
	
	XDebugCtrl = this;	

  pthread_mutexattr_init(&attr);
	if(!pthread_mutex_init(&mutex, &attr)) mutexhandle = 1;  
};				



//-------------------------------------------------------------------
//  XDEBUGCTRL::~XDEBUGCTRL
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/01/2001 17:03:22
//	
//	@return 			
*/
//-------------------------------------------------------------------
XLINUXDEBUGCTRL::~XLINUXDEBUGCTRL()
{		
  if(mutexhandle)
		{			
			pthread_mutex_destroy(&mutex);
			mutexhandle = 0;			
		}

	for(int c=0; c<XDEBUG_MAXNTARGETS; c++)
		{
			if(targets[c].GetType() == XDEBUGCTRLTYPE_NET) 
				{
				  CloseHandleNet(&targets[c]);
				}
		}
	  
	Clean();

	XDebugCtrl =  NULL;
}




/*-------------------------------------------------------------------
//	XLINUXDEBUGCTRL::PrintSpecial
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			15/07/2014 20:54:46
//	
//  @param				level : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
void XLINUXDEBUGCTRL::PrintSpecial(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string)
{		
	if(!target)							 return;
	if(!target->GetAim()[0]) return;

  Lock();

	XSTRING  _aim;
	XBYTE		 aim[_MAXPATH];		
	int		   filehdl;
	int			 size = 0;			

	memset(aim, 0, _MAXPATH);
			
	_aim.Set(target->GetAim());	
	_aim.ConvertToUTF8(aim, size);
			
	signal(SIGPIPE,SIG_IGN);

	filehdl = open((char*)aim, O_RDWR | O_NONBLOCK);
	if(filehdl == -1) 
		{
			if(mkfifo((char*)aim, 0777 /*S_IRUSR | S_IWUSR*/)!=0) return;

			filehdl = open((char*)aim, O_RDWR | O_NONBLOCK);	
			if(filehdl == -1) 	
				{
					UnLock();
					return;
				}
		} 


	// Not Limit of Size ...

	XSTRING line;

  /*
	if(level) 
		{
			XSTRING codestr;

			codestr.Format(__L("%03d: "), level);
			write(filehdl,codestr.GetOEM(),strlen(codestr.GetOEM()));
		}
  */

	line = string;
	
	XSTRING_CREATEOEM(line, charOEM)	
	write(filehdl, charOEM, line.GetSize());   
	XSTRING_DELETEOEM(charOEM)	

	write(filehdl,__L("\n\r"),2);  
  fsync(filehdl);


	close(filehdl);

  UnLock();
}



/*-------------------------------------------------------------------
//	XLINUXDEBUGCTRL::PrintFile
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			15/07/2014 20:55:07
//	
//  @param				level : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
void XLINUXDEBUGCTRL::PrintFile(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string)
{
	if(!target)							 return;
	if(!target->GetAim()[0]) return;

  Lock();

	XSTRING  _aim;
	XBYTE		 aim[_MAXPATH];	
	XSTRING  _mode;
	XBYTE		 mode[32];
	XSTRING  line;
	FILE*    file;
	int			 size = 0;

	_aim.Set(target->GetAim());
	_aim.ConvertToUTF8(aim, size);
	
	_mode.Set(__L("at"));
	_mode.ConvertToUTF8(mode,size);

	file=fopen((char*)aim, (char*)mode);
  if(!file) 
    {
      UnLock();
      return;
    }

	// --- Check File size --------------
	
	int sizefile = 0;

	fflush(file);

	int position = ftell(file);
	fseek(file,0,SEEK_END);	
	sizefile = ftell(file);
	fseek(file,position,SEEK_SET);
	
	if((sizefile+(int)line.GetSize())>sizelimit)
		{
			fclose(file);	
		
		  if(unlink((char*)target)==-1) 
			  {
				  int a=0;
				  a++;
			  }

			file=fopen((char*)target,(char*)mode);
			if(!file) 
        {
          UnLock();
          return;
        }
		}

	// --- Check File size --------------
  
	if(level) 
		{
			if((level & XDEBUGCTRLLEVEL_WITHCOLOR) == XDEBUGCTRLLEVEL_WITHCOLOR)
				{
					
				}
			 else
			  {
					if((level & XDEBUGCTRLLEVEL_WITHTAB) == XDEBUGCTRLLEVEL_WITHTAB)
						{
							XSTRING tabstr;

							GenerateTab(level&0x0F,tabstr);					

							XSTRING_CREATEOEM(tabstr, charOEM)	
							fwrite(charOEM, 1, tabstr.GetSize(), file);
							XSTRING_DELETEOEM(charOEM)	
						}
					 else
					  { 
							if((level & XDEBUGCTRLLEVEL_WITHCODE) == XDEBUGCTRLLEVEL_WITHCODE)
								{
									XSTRING codestr;

									codestr.Format(__L("%02d: "), (level&0x0F));

									XSTRING_CREATEOEM(codestr, charOEM)	
									fwrite(charOEM, 1, codestr.GetSize(), file);
									XSTRING_DELETEOEM(charOEM)	
								}
						}
				}			
		}
	

	line = string;
	
	XSTRING_CREATEOEM(line, charOEM)	
	fwrite(charOEM, 1, line.GetSize(), file);
	XSTRING_DELETEOEM(charOEM)	

	fwrite(__L("\n\r"),1,1,file);
 
  fclose(file);	

  UnLock();
}




/*-------------------------------------------------------------------
//	XLINUXDEBUGCTRL::PrintNet
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			15/07/2014 20:55:35
//	
//  @param				level : 
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
void XLINUXDEBUGCTRL::PrintNet(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string)
{	
	if(!target)	return;

	Lock();

	XBUFFER	xbufferpacket;
	
	xdatetime.Read();
		
	SetDebugToXBuffer(publicIP, localIP, level, sequence, &xdatetime,  string, xbufferpacket);

	int handle = (int)target->GetNETHandle();

	send(handle,(const char*)xbufferpacket.Get(), xbufferpacket.GetSize(), 0);	

	UnLock();
}




/*-------------------------------------------------------------------
//	XLINUXDEBUGCTRL::GetHandleNet
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			28/12/2016 14:18:19
//	
//	@return 			bool : 
//
//  @param				target : 
*/
/*-----------------------------------------------------------------*/
bool XLINUXDEBUGCTRL::GetHandleNet(XDEBUGCTRLTARGET* target)																					
{
	struct sockaddr_in  addr;
	int						 		  handle;
													
	memset(&addr, 0, sizeof(struct sockaddr));
	
	handle = socket(AF_INET, SOCK_DGRAM, 0);
	if(handle < 0)  return false;
							
	if(!target->GetIP()[0]) return false; 
	
	addr.sin_family				= AF_INET;
	addr.sin_addr.s_addr	= inet_addr(target->GetIP());	  
	addr.sin_port					= htons(target->GetPort());				
																																																				
	connect(handle, (struct sockaddr*)&addr, sizeof(struct sockaddr));

	XQWORD NEThandle = (XQWORD)handle;

	target->SetNETHandle(NEThandle);

	return true;                              
}



/*-------------------------------------------------------------------
//	XLINUXDEBUGCTRL::CloseHandleNet
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			28/12/2016 14:39:11
//	
//	@return 			bool : 
//
//  @param				target : 
*/
/*-----------------------------------------------------------------*/
bool XLINUXDEBUGCTRL::CloseHandleNet(XDEBUGCTRLTARGET* target)
{
	int handle = (int)target->GetNETHandle(); 	
	if(!handle) return false;
		
	close(handle);
	target->SetNETHandle(0); 
	
	return true;
}




/*-------------------------------------------------------------------
//  XLINUXDEBUGCTRL::Lock
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/06/2011 10:48:04
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool XLINUXDEBUGCTRL::Lock()
{
  if(!mutexhandle)						   return false;
	if(pthread_mutex_lock(&mutex)) return false;
 
	return true;
}




/*-------------------------------------------------------------------
//  XLINUXDEBUGCTRL::UnLock
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      08/10/2012 17:59:20
//  
//  @return       bool : 
//  */
/*-----------------------------------------------------------------*/
bool XLINUXDEBUGCTRL::UnLock()
{	
  if(!mutexhandle)			      			 return false;	
	if(pthread_mutex_unlock(&mutex))   return false;

  return true;
}


#endif
