//------------------------------------------------------------------------------------------
//	XLINUXSYSTEM.CPP
//	
//	LINUX system class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 03/03/2004 12:28:40
//	Lsdddast Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/wait.h> 
#include <linux/reboot.h>


#include "XLINUXFactory.h"
#include "XBuffer.h"
#include "XFileTXT.h"
#include "XDebug.h"
#include "XString.h"
#include "XPath.h"

#include "XLINUXSystem.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XLINUXSYSTEM::XLINUXSYSTEM
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/03/2004 12:28:59
//	
//	@return				
//	*/
//-------------------------------------------------------------------
XLINUXSYSTEM::XLINUXSYSTEM() : XSYSTEM()
{

}



//-------------------------------------------------------------------
//  XLINUXSYSTEM::~XLINUXSYSTEM
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/03/2004 12:29:20
//	
//	@return				
//	*/
//-------------------------------------------------------------------
XLINUXSYSTEM::~XLINUXSYSTEM()
{

}



/*-------------------------------------------------------------------
//  XLINUXSYSTEM::GetTypeHardware
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/08/2009 05:01:13 p.m.
//	
//	@return				XSYSTEM_HARDWARE : 
//	*/
/*-----------------------------------------------------------------*/
XSYSTEM_HARDWARETYPE XLINUXSYSTEM::GetTypeHardware(int* revision)
{
	#define CPUINFOREVISION __L("Revision")

	if(revision)
		{
			(*revision) = -1;

			XFILETXT* xfileTXT = new XFILETXT();
			if(xfileTXT)
				{
					XPATH xpath;

					xpath = __L("/proc/cpuinfo");

					if(xfileTXT->Open(xpath))
						{
							if(xfileTXT->ReadAllFile())
								{
									for(int c=0; c<xfileTXT->GetNLines(); c++)
										{
											XSTRING* line = xfileTXT->GetLine(c);
											if(line)
												{
													if(line->Find(CPUINFOREVISION, false) != XSTRING_NOTFOUND)
														{
															int position = line->Find(__L(":"), false);
															if(position != XSTRING_NOTFOUND)
																{
																	XSTRING number;																
																	
																	number = &line->Get()[position+1];																																		

																	number.UnFormat(__L(" %04x"), revision);
																}
														}
												}
										}
								}

							xfileTXT->Close();
						}
	
					delete xfileTXT;
				}
		}
	

	#ifdef HW_PC
	return XSYSTEM_HARDWARETYPE_PC;
	#endif
	
	#ifdef HW_LEX3V700A
	return XSYSTEM_HARDWARETYPE_LEX3V700A;
	#endif

  #ifdef HW_GUMSTIXCONNEX
	return XSYSTEM_HARDWARETYPE_GUMSTIXCONNEX;
	#endif

	#ifdef HW_RASPBERRYPI
	return XSYSTEM_HARDWARETYPE_RASPBERRYPI;
	#endif

	#ifdef HW_ODROIDC1
	return XSYSTEM_HARDWARETYPE_ODROIDC1;
	#endif

  #ifdef HW_ARTILA500
  return XSYSTEM_HARDWARETYPE_ARTILA500;
  #endif
	
	return XSYSTEM_HARDWARETYPE_UNKNOWN;
}




//-------------------------------------------------------------------
//  XLINUXSYSTEM::GetSO
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/03/2004 12:29:40
//	
//	@return				XLINUXSYSTEM_SO : 
//	*/
//-------------------------------------------------------------------
XSYSTEM_SO XLINUXSYSTEM::GetSO()
{	
	#ifdef HW_PC
	return XSYSTEM_SO_LINUX;
	#endif

	#ifdef HW_LEX3V700A
	return XSYSTEM_SO_LINUX;
	#endif
		
  #ifdef HW_GUMSTIXCONNEX
	return XSYSTEM_SO_LINUX_EMBEDDED;
	#endif

	#ifdef HW_RASPBERRYPI
	return XSYSTEM_SO_LINUX_EMBEDDED;
	#endif

	#ifdef HW_ODROIDC1
	return XSYSTEM_SO_LINUX_EMBEDDED;
	#endif

  #ifdef HW_ARTILA500
  return XSYSTEM_SO_LINUX_EMBEDDED;
  #endif

  return XSYSTEM_SO_UNKNOWN;
}


//-------------------------------------------------------------------
//  XLINUXSYSTEM::GetLanguage
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/03/2004 12:29:55
//	
//	@return				XLANGUAGE_CODE : 
//	*/
//-------------------------------------------------------------------
XLANGUAGE_CODE XLINUXSYSTEM::GetLanguage()
{
	return XLANGUAGE_UNKNOWN;
}



/*-------------------------------------------------------------------
//  XLINUXSYSTEM::GetMemoryInfo
*/ 
/**
//  
//  
//  @author       Abraham J. Velez
//  @version      18/09/2012 9:22:44
//  
//  @return       bool : 
//  @param        total : 
//  @param        free : 
*/
/*-----------------------------------------------------------------*/
bool XLINUXSYSTEM::GetMemoryInfo(XDWORD& total,XDWORD& free)
{
  if(!xfactory) return false;

	struct sysinfo meminfo;

	if(sysinfo(&meminfo) == -1) return false;

	long long totalvirtualmem = meminfo.totalram;
	//Add other values in next statement to avoid int overflow on right hand side...
	//totalvirtualmem += meminfo.totalswap;
	totalvirtualmem *= meminfo.mem_unit;
	
	long long virtualmemused = meminfo.totalram - meminfo.freeram;
	//Add other values in next statement to avoid int overflow on right hand side...
	virtualmemused += meminfo.totalswap - meminfo.freeswap;
	virtualmemused *= meminfo.mem_unit;

	total = (XDWORD)(totalvirtualmem / 1024);
	free  = (XDWORD)((totalvirtualmem - virtualmemused)  / 1024);

	return true;


	/*
  total = 0;
  free  = 0;
 
	XFILETXT* meminfofile = new XFILETXT();
  if(!meminfofile) return false; 

	XPATH   xpath;
  bool    status = false;  
	xpath = __L("/proc/meminfo");
   
 //xpath = __L("D:/Proyects/XTrap/Visual/meminfo");

	if(meminfofile->Open(xpath)) 
    {
      if(meminfofile->ReadAllFile())
        {
          XSTRING* stringmemtotal =  meminfofile->GetLine(0);
          XSTRING* stringmemfree  =  meminfofile->GetLine(1);

          if(stringmemtotal) stringmemtotal->UnFormat(__L("MemTotal: %d kB"),&total);
          if(stringmemfree)  stringmemfree->UnFormat (__L("MemFree: %d kB") ,&free);
        }

      meminfofile->Close();

      status = true;
    }

  delete meminfofile;
	return status;
	*/

  
}



/*-------------------------------------------------------------------
//	XLINUXSYSTEM::MakeCommand
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/11/2014 20:48:59
//	
//	@return 			bool : 
//
//  @param				command : 
//  @param				returncode : 
*/
/*-----------------------------------------------------------------*/
bool XLINUXSYSTEM::MakeCommand(XCHAR* command, int* returncode)
{
	int status;

	XSTRING _command;

	_command = command;

	XSTRING_CREATEOEM(_command, charOEM)	
	status = system(charOEM);		
	XSTRING_DELETEOEM(charOEM)	
	if(status	== -1) return false;

	if(returncode) (*returncode) = status;
   
	return true;
}



/*-------------------------------------------------------------------
//	XLINUXSYSTEM::ExecuteApplication
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			06/08/2015 16:06:58
//	
//	@return 			bool : 
//
//  @param				command : 
//  @param				params : 
*/
/*-----------------------------------------------------------------*/
bool XLINUXSYSTEM::ExecuteApplication(XCHAR* command, XCHAR* params, bool special)
{ 
	if(!command) return false;

	pid_t pID;
	bool  exist  = false;
	bool  status = false;

	XFILE* xfile = xfactory->Create_File();
	if(xfile)
		{
			exist = xfile->Open(command);
			xfile->Close();
		}

	xfactory->Delete_File(xfile);
	
	if(!exist) return false;

	//-----------------------------------------
	// exec attributtes

	XSTRING cmd;

	cmd  = __L("chmod 775 ");
	cmd += command;

	XSTRING_CREATEOEM(cmd, charOEM)	
	status = system(charOEM);		
	XSTRING_DELETEOEM(charOEM)	
		
	//if(status	== -1) return false;
	
	//------------------------------------------
	
	XSTRING _params;

	if(!special)
		{
			cmd			= command;		

			_params.Add(cmd);
			if(params) 
				{
					_params.Add(" "); 
					_params.Add(params);				
				}
		}
   else
	  {
			XPATH xpath;
			XPATH namefileapp;
			
			xpath = command;
			xpath.GetNamefile(namefileapp);

 		  cmd = __L("/usr/bin/screen");								
			
			_params.Add(cmd);

			_params.Add(" ");						
			_params.Add("-S ");						
			_params.Add(namefileapp);
			_params.Add(" -d -m ");
			_params.Add(command);
			
			if(params)
				{
					_params.Add(" "); 
					_params.Add(params);
				}
	  }
 																	
	/*
	XSTRING_CREATEOEM(cmd, charOEM2)	
	status = system(charOEM2);		
	XSTRING_DELETEOEM(charOEM2)	
	*/

	//XDEBUG_PRINTCOLOR(3, __L("EXEC PARAMS: cmd [%s] params [%s]"), cmd.Get(), _params.Get());

	#define MAXNPARAMS  20

	char* param[MAXNPARAMS] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
															NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL 
														};
	int   start							= 0;
	bool  endfound					= false;
																																			
	for(int c=0; c<MAXNPARAMS; c++)
		{
			XSTRING string;			
			int			found = _params.Find(__L(" "), true, start);

			if(found == XSTRING_NOTFOUND)
				{
					_params.Copy(start, string);					
					endfound = true;
				}
			 else
				{
					_params.Copy(start, found, string);
					start = found+1;
				}

			string.CreateOEM(param[c]);  

			//XDEBUG_PRINTCOLOR(3, __L("EXEC PARAMS: param%02d [%s]"), c, string.Get());

			if(endfound) break;
		}
		
	pID = fork();

	switch(pID)
		{
			case -1	:	break;

			case  0	: { // Child 																																															
									XSTRING_CREATEOEM(cmd, charcmd) 

									int _status = execl(charcmd	, param[ 0], param[ 1], param[ 2], param[ 3], param[ 4], param[ 5], param[ 6], param[ 7], param[ 8], param[ 9]
																							, param[10], param[11], param[12], param[13], param[14], param[15], param[16], param[17], param[18], param[19]
																							, NULL);  
																																																		
									if(_status == -1)
										{
											XDEBUG_PRINTCOLOR(4,__L("Error en EXEC: %d"), errno);											
										}
									else
									 {
										 status = true;										 
										}
									
									XSTRING_DELETEOEM(charcmd)										
								}						 	
								break;

		  default	:	status = true;
								break;
	  }

	for(int c=0; c<MAXNPARAMS; c++)
		{
			delete [] param[c];
		}
 
	return status;												
}	




/*-------------------------------------------------------------------
//	 XLINUXSYSTEM::IsApplicationRunning
*/
/**
//
//	
//
//	@author		Abraham J. Velez 
//	@version		
//
//	@return		bool : 
//
//	@param		XCHAR* : 
//
*//*-----------------------------------------------------------------*/
bool XLINUXSYSTEM::IsApplicationRunning(XCHAR* command, XDWORD* ID)
{
  DIR*						dir;
  struct dirent*  ent;
  char*						endptr;
  char						buf[512];

  if(!(dir = opendir("/proc"))) return -1;
		
  while((ent = readdir(dir)) != NULL) 
		{
      // if endptr is not a null character, the directory is not entirely numeric, so ignore it         
			long lpid = strtol(ent->d_name, &endptr, 10);
      if(*endptr != '\0') 
				{
          continue;
        }

			// try to open the cmdline file 
      snprintf(buf, sizeof(buf), "/proc/%ld/status", lpid);
      FILE* fp = fopen(buf, "r");
			if(fp) 
				{
					if(fgets(buf, sizeof(buf), fp) != NULL) 
						{
              // check the first token in the file, the program name 
              char* first = strtok(buf, " ");
							
							XPATH nameall;
							
							nameall = first;				
							nameall.SetOnlyNamefile();
							nameall.DeleteCharacter(__C(' ') , XSTRINGCONTEXT_ATFIRST);
							nameall.DeleteCharacter(__C('\t'), XSTRINGCONTEXT_ATFIRST);
							nameall.DeleteCharacter(__C('\n'), XSTRINGCONTEXT_ATEND);
							nameall.DeleteCharacter(__C('\r'), XSTRINGCONTEXT_ATEND);
														
							if(!nameall.Compare(command,false))					
								{
									if(ID) (*ID) = (XDWORD)lpid;

                  fclose(fp);
                  closedir(dir);
										
									return true;
                  
                }
            }

          fclose(fp);
        }
    }

	closedir(dir);


  return false;
}




/*-------------------------------------------------------------------
//	XLINUXSYSTEM::ShutDown
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/11/2014 20:48:43
//	
//	@return 			bool : 
//
//  @param				type : 
*/
/*-----------------------------------------------------------------*/
bool XLINUXSYSTEM::ShutDown(XSYSTEM_SHUTDOWNTYPE type)
{
	switch(type)
		{
			case XSYSTEM_SHUTDOWNTYPE_REBOOT	 :	
																						sync();
																						reboot(LINUX_REBOOT_CMD_RESTART);
																						break;

			case XSYSTEM_SHUTDOWNTYPE_LOGOFF	 :	break;

			case XSYSTEM_SHUTDOWNTYPE_POWEROFF :	sync();
																						reboot(LINUX_REBOOT_CMD_POWER_OFF);
																						break;
		}
	
	return true;
}





