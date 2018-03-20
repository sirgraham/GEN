//------------------------------------------------------------------------------------------
//	XDEBUG.H
//	
/**	
// \class 
//   
//  Debug Funtions
//   
//	@author	 Abraham J. Velez
//	@version 05/04/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XDEBUG_H_
#define _XDEBUG_H_
	
//---- INCLUDES ----------------------------------------------------------------------------			

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "XBase.h"
#include "XDateTime.h"
#include "XString.h"
#include "XBuffer.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum XDEBUGCTRLTYPE	
{
	 XDEBUGCTRLTYPE_NONE			    = 0 ,
	 XDEBUGCTRLTYPE_SPECIAL			      ,
	 XDEBUGCTRLTYPE_FILE							,
	 XDEBUGCTRLTYPE_NET								,
};


#define XDEBUGCTRLLEVEL_WITHCOLOR					  0x80 
#define XDEBUGCTRLLEVEL_WITHTAB						  0x40 
#define XDEBUGCTRLLEVEL_WITHCODE						0x20 

#define XDEBUGLEVELCOLOR(level)							(level|XDEBUGCTRLLEVEL_WITHCOLOR)
#define XDEBUGLEVELTAB(level)								(level|XDEBUGCTRLLEVEL_WITHTAB)
#define XDEBUGLEVELCODE(level)							(level|XDEBUGCTRLLEVEL_WITHCODE)

#ifdef MICROCONTROLLER
	#define XDEBUG_SIZELIMITDEFAULT							512				// Bytes
	#define XDEBUG_MAXNTARGETS									1
	#define XDEBUG_MAXSIZEDATABLOCK							512
	#define XDEBUG_MAXNETBUFFER									512
#else
	#define XDEBUG_SIZELIMITDEFAULT							64*1024		// Bytes
	#define XDEBUG_MAXNTARGETS									5
	#define XDEBUG_MAXSIZEDATABLOCK							10240
	#define XDEBUG_MAXNETBUFFER									512
#endif

#define XDEBUG_DEFAULT_SPECIALAIM						__L("Debug Monitor")


#define XDEBUG_DEFAULT_NETURL		            __L("xdebug.kicks-ass.org")
#define XDEBUG_DEFAULT_NETAIM		            __L("xdebug.kicks-ass.org:10222")
#define XDEBUG_DEFAULT_NETAIM2		          __L("xdebug2.kicks-ass.org:10223")
#define XDEBUG_DEFAULT_NETAIM3	            __L("xdebug3.kicks-ass.org:10224")
#define XDEBUG_DEFAULT_NETAIM4		          __L("xdebug4.kicks-ass.org:10225")
#define XDEBUG_DEFAULT_NETAIM5	            __L("xdebug5.kicks-ass.org:10226")

#define XDEBUG_LIMITCTRLNETRESURCES					25

#define XDEBUG_MAXAPPLICATIONNAMESIZE  			20
#define XDEBUG_MAXAPPLICATIONVERSIONSIZE 		10
#define XDEBUG_MAXAPPLICATIONIDSIZE 				36

#define XDEBUG_IDPACKET											0xAADEB055
#define XDEBUG_MAXCACHEREMOTEUDPIP					100

#define XDEBUG_COLORBLACK										0
#define XDEBUG_COLORBLUE										1 
#define XDEBUG_COLORGREEN										2 
#define XDEBUG_COLORPURPLE									3 
#define XDEBUG_COLORRED											4 
#define XDEBUG_COLORGRAY										5 

#define XDEBUG_COLORINFO										XDEBUG_COLORBLACK						
#define XDEBUG_COLORWARNING									XDEBUG_COLORPURPLE
#define XDEBUG_COLORERROR										XDEBUG_COLORRED



#define XDEBUG_CRC16(ptrbuffer, buffersize, crc)     { XBYTE*	  buffer = ptrbuffer;																										\
																											 XDWORD   size   = buffersize;																									\
																											 XBYTE		x;																																		\
																											 crc = 0xFFFF;																																	\
																											 while(size--)																																	\
																												 {                                                                            \
																													 x = crc >> 8 ^ * buffer++;                                                 \
																													 x ^= x>>4;																																	\
																													 crc = (crc << 8) ^ ((XWORD)(x << 12)) ^ ((XWORD)(x <<5)) ^ ((XWORD)x);			\
																												 }																																						\
																										 }
	 

	
//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XMUTEX;
class DIOSTREAM;


class XDEBUGCTRLTARGET
{
	public:
														XDEBUGCTRLTARGET							()																																{ Clean();																		}										
		virtual							 	 ~XDEBUGCTRLTARGET							()																																{ Clean();																		}		

		XDEBUGCTRLTYPE					GetType												()																																{ return type;																}      
		void										SetType												(XDEBUGCTRLTYPE type)																							{ this->type = type;													}      
		
		XCHAR*									GetAim												()																																{ return aim;																	}
		bool										GetAim												(XSTRING& aim)                         
														{ 
															aim.Empty();
															aim = this->aim;

															return aim.IsEmpty()?false:true;
														}

		void										SetAim												(XSTRING& aim)
														{
															memset(this->aim, 0, (_MAXSTR * sizeof(XCHAR)));	
															memcpy(this->aim, aim.Get(), (aim.GetSize() * sizeof(XCHAR)));	 
														}

		void										SetAim												(XCHAR* aim)
														{
															int size = XSTRING::GetSize(aim);
															memset(this->aim, 0, (_MAXSTR * sizeof(XCHAR)));	
															memcpy(this->aim, aim, size * sizeof(XCHAR));	 
														}

		XWORD										GetPort												()																																{ return port;																}      
		void										SetPort												(XWORD port)																											{ this->port = port;													}   

		bool										ResolvedIPTarget							();
													
		XDWORD									GetNSend											()																																{ return nsend;																}
		void										AddNSend											()																																{ nsend++;																    }

		char*										GetIP                         ()																																{ return IP;																  }

		XQWORD									GetNETHandle									()																																{ return NEThandle;									 				  }
		void										SetNETHandle									(XQWORD NEThandle)																								{ this->NEThandle = NEThandle;							  }


	private:
		
		void										Clean													()
														{  														
															type					= XDEBUGCTRLTYPE_NONE;
															memset(aim	, 0, (_MAXSTR * sizeof(XCHAR)));	
															port          = 0;
															memset(IP, 0, _MAXSTR * sizeof(char));
															nsend					= 0;
															NEThandle			= 0;
														}

		XDEBUGCTRLTYPE					type;
		XCHAR										aim[_MAXSTR];
		XWORD										port;
		char										IP[_MAXSTR];
		XDWORD                  nsend;
		XQWORD									NEThandle;
};



class XDEBUGCTRL
{
  public:
														XDEBUGCTRL										()
                            {
                              Clean();
                            }

		virtual							 	 ~XDEBUGCTRL										()																																	{ Clean();																  }

		bool										SetTarget											(int index, XDEBUGCTRLTYPE type , XCHAR* aim);
		bool										AddTarget											(XDEBUGCTRLTYPE type , XCHAR*	aim);

		XDWORD									GetSizeLimit                  ()																																	{ return sizelimit;													}
		bool										SetSizeLimit                  (XDWORD sizelimit = XDEBUG_SIZELIMITDEFAULT) 
														{ 
															if(!sizelimit) return false;

															this->sizelimit = sizelimit;						 
															return true;		
														}			
		           		
		XCHAR*									GetApplicationName						()																																	{ return applicationname;									  }
		void										SetApplicationName					  (XCHAR* applicationname)
														{
															int size = 0; 
															if(applicationname) size=XSTRING::GetSize(applicationname);

															memset(this->applicationname, 0, _MAXSTR * sizeof(XCHAR));
															if(applicationname) memcpy(this->applicationname, applicationname, (size + 1) * sizeof(XCHAR));
														}

		void										SetApplicationName						(XCHAR* applicationname, int size)																				
														{ 
															memset(this->applicationname, 0, _MAXSTR*sizeof(XCHAR));
															if(applicationname) memcpy(this->applicationname, applicationname, (size+1)*sizeof(XCHAR)); 																															
														}

		void										SetApplicationName						(XSTRING& applicationname)																				
														{ 
															SetApplicationName(applicationname.Get(), applicationname.GetSize());																																
														}
		
		void										GetApplicationVersion					(int& applicationversion, int& applicationsubversion, int& applicationsubversionerr)
														{
															applicationversion				= this->applicationversion; 
															applicationsubversion			= this->applicationsubversion; 
															applicationsubversionerr	= this->applicationsubversionerr;
														}

		void										SetApplicationVersion         (int applicationversion, int applicationsubversion, int applicationsubversionerr)
														{
															this->applicationversion				= applicationversion; 
															this->applicationsubversion			= applicationsubversion; 
															this->applicationsubversionerr	= applicationsubversionerr;
														}

		XCHAR*									GetApplicationID							()																																	{ return applicationID;											}
		
		void										SetApplicationID							(XCHAR* applicationID, int size)																				
														{ 
															memset(this->applicationID, 0, _MAXSTR*sizeof(XCHAR)), 
															memcpy(this->applicationID, applicationID, (size+1)*sizeof(XCHAR)); 																															
														}
		void										SetApplicationID							(XSTRING& applicationID)																				
														{ 
															SetApplicationID(applicationID.Get(), applicationID.GetSize());																																
														}

		XSTRING*						    GetPublicIPString							()																																	{ return &publicIPstring;										}
		bool								    SetPublicIPString							(XSTRING* publicIPstring)																																	
														{ 
															int data[4];				
																
															publicIPstring->UnFormat(__L("%d.%d.%d.%d"), &data[0], &data[1], &data[2], &data[3]);

															publicIP  |= (data[0] << 24);
															publicIP  |= (data[1] << 16);
															publicIP  |= (data[2] << 8);
															publicIP  |= (data[3]);

															return true;
														}

		XSTRING*						    GetLocalIPString									()																																	{ return &localIPstring;										}
		bool								    SetLocalIPString									(XSTRING* localIPstring)																																	
														{ 
															int data[4];				
																
															localIPstring->UnFormat(__L("%d.%d.%d.%d"), &data[0], &data[1], &data[2], &data[3]);

															localIP  |= (data[0] << 24);
															localIP  |= (data[1] << 16);
															localIP  |= (data[2] << 8);
															localIP  |= (data[3]);

															return true;
														}

		XDWORD						      GetPublicIP												()																																	{ return publicIP;													}		
		XDWORD						      GetLocalIP												()																																	{ return localIP;														}
		

		XCHAR*									GetWebScraperScriptURLDownload		()																																	{ return webscraperscripturldownload;			  }
		void										SetWebScraperScriptURLDownload		(XCHAR* webscraperscripturldownload)
														{
															int size = 0; 
															if(webscraperscripturldownload) size=XSTRING::GetSize(webscraperscripturldownload);

															memset(this->webscraperscripturldownload, 0, _MAXSTR * sizeof(XCHAR));
															if(webscraperscripturldownload) memcpy(this->webscraperscripturldownload, webscraperscripturldownload, (size + 1) * sizeof(XCHAR));
														}

		
		void										PrintHeader												(XCHAR* header); 
		bool										Print															(XBYTE level, XCHAR* mask,...);
		bool										PrintDataBlock										(XBYTE level, XBYTE* data, int _size, int marginsize = 1, int sizeline = 32, bool showoffset = true, bool showtext = true);
		bool										PrintDataBlock										(XBYTE level, XBUFFER& data, int marginsize = 1, int sizeline = 32, bool showoffset = true, bool showtext = true);
				
		virtual void						PrintSpecial											(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string)							{																						}
		virtual void						PrintFile													(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string)							{																						}
		virtual void						PrintNet													(XDEBUGCTRLTARGET* target, XBYTE level, XCHAR* string)							{																						}

		virtual	bool            GetHandleNet											(XDEBUGCTRLTARGET* target)																					{ return false;                             }							
		virtual	bool            CloseHandleNet										(XDEBUGCTRLTARGET* target)																					{ return false;                             }							

		XDWORD									GetDebugFromXBuffer								(XBUFFER& xbufferpacket, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XSTRING& string);
		XDWORD									GetDebugFromStreamIO							(DIOSTREAM* streamIO, XDWORD& publicIP, XDWORD& localIP, XBYTE& level, XDWORD& sequence, XDATETIME* xtime, XSTRING& string);
		bool										SetDebugToXBuffer									(XDWORD publicIP, XDWORD localIP, XBYTE level, XDWORD sequence, XDATETIME* xtime, XCHAR* string, XBUFFER& xbufferpacket);
	
		bool										ResolvedAllNetTargets							(bool& changed);
		bool										ReConnectedAllNetTargets					();
		
		bool										ObtainLocalIP											();
		bool										ObtainPublicIP										();

		bool										ObtainResourcesIP									()
														{
															if(!ObtainLocalIP())	return false;
															if(!ObtainPublicIP()) return false;

															return true;
														}	

		bool                   ResolvedAllResources           ()
													 { 
														 bool changed;

														 ResolvedAllNetTargets(changed);		
														 if(changed) 
															 {	
																 ObtainResourcesIP();
																 ReConnectedAllNetTargets();				
															 }

														 return true;
														}

	
	protected:

		bool										GenerateTab										(int ntab,XSTRING& tab);
		
		XDEBUGCTRLTARGET        targets[XDEBUG_MAXNTARGETS];

		XDWORD									sizelimit;

    XCHAR										applicationname[_MAXSTR];
		int											applicationversion;
		int											applicationsubversion;
		int											applicationsubversionerr;
		XCHAR										applicationID[_MAXSTR];
		XDWORD                  publicIP;
		XSTRING                 publicIPstring;
		XDWORD                  localIP;
		XSTRING									localIPstring;
		XCHAR		                webscraperscripturldownload[_MAXSTR];
		
		bool										openheader;	
		XDWORD							    sequence;

	private:
		
		void										Clean													()
														{  
															sizelimit									= XDEBUG_SIZELIMITDEFAULT;

															for(int c=0; c<_MAXSTR; c++)
																{
																	applicationname[c] = 0;
																	applicationID[c]   = 0;
																}
															
															applicationversion				= 0;
															applicationsubversion			= 0;
															applicationsubversionerr	= 0;															
														
															publicIP									= 0;											
															localIP										= 0;
																																
															openheader								= false;	
															sequence									= 0;																																												
														}
		
};
	

//---- INLINE FUNCTIONS --------------------------------------------------------------------



#ifdef XDEBUG	

  extern XDEBUGCTRL* XDebugCtrl;

	#define XDEBUG_SETTARGET(index, type , aim)																																	XDebugCtrl->SetTarget(index, type , aim)
	#define XDEBUG_ADDTARGET(type , aim)																																				XDebugCtrl->AddTarget(type , aim)
	#define XDEBUG_SETSIZELIMIT(sizelimit)																																			XDebugCtrl->SetSizeLimit(sizelimit)
	#define XDEBUG_SETAPPLICATIONNAME(name)																																			XDebugCtrl->SetApplicationName(name)
	#define XDEBUG_SETAPPLICATIONVERSION(version, subversion, subversionerr)																		XDebugCtrl->SetApplicationVersion(version, subversion, subversionerr)
	#define XDEBUG_SETAPPLICATIONID(ID)																																					XDebugCtrl->SetApplicationID(ID)
	#define XDEBUG_SETWEBSCRAPERSCRIPURLDOWNLOAD(URL)																														XDebugCtrl->SetWebScraperScriptURLDownload(URL);
	#define XDEBUG_SETPUBLICIP(IP)																																							XDebugCtrl->SetPublicIPString(IP)
	#define XDEBUG_SETLOCALIP(IP)																																								XDebugCtrl->SetLocalIPString(IP)
	#define XDEBUG_RESOLVEDALLRESOURCES																																					XDebugCtrl->ResolvedAllResources()	
  #define XDEBUG_PRINTHEADER(header)																																					XDebugCtrl->PrintHeader(header)

  #ifdef BUILDER

    #define XDEBUG_PRINT
    #define XDEBUG_PRINTLEVEL
    #define XDEBUG_PRINTCOLOR
    #define XDEBUG_PRINTTAB
    #define XDEBUG_PRINTCODE

    #define XDEBUG_PRINTDATABLOCK
    #define XDEBUG_PRINTDATABLOCKLEVEL
		#define XDEBUG_PRINTDATABLOCKCOLOR
    #define XDEBUG_PRINTDATABLOCKTAB
		#define XDEBUG_PRINTDATABLOCKCODE

  #else

  	#define XDEBUG_PRINT(mask, ...)																																							XDebugCtrl->Print(0																	 , mask , ## __VA_ARGS__)
	  #define XDEBUG_PRINTLEVEL(level, mask, ...)																																	XDebugCtrl->Print(level															 , mask , ## __VA_ARGS__)
  	#define XDEBUG_PRINTCOLOR(level, mask, ...)																																	XDebugCtrl->Print((level|XDEBUGCTRLLEVEL_WITHCOLOR)  , mask , ## __VA_ARGS__)
  	#define XDEBUG_PRINTTAB(level, mask, ...)																																		XDebugCtrl->Print((level|XDEBUGCTRLLEVEL_WITHTAB)    , mask , ## __VA_ARGS__)
  	#define XDEBUG_PRINTCODE(level, mask, ...)																																	XDebugCtrl->Print((level|XDEBUGCTRLLEVEL_WITHCODE)   , mask , ## __VA_ARGS__)

  	#define XDEBUG_PRINTDATABLOCK(level, data, ...)																															XDebugCtrl->PrintDataBlock(0        													, data, ## __VA_ARGS__)
  	#define XDEBUG_PRINTDATABLOCKLEVEL(level, data, ...)																												XDebugCtrl->PrintDataBlock(level     													, data, ## __VA_ARGS__)
  	#define XDEBUG_PRINTDATABLOCKCOLOR(level, data, ...)																												XDebugCtrl->PrintDataBlock((level|XDEBUGCTRLLEVEL_WITHCOLOR)	, data, ## __VA_ARGS__)
  	#define XDEBUG_PRINTDATABLOCKTAB(level, data, ...)																													XDebugCtrl->PrintDataBlock((level|XDEBUGCTRLLEVEL_WITHTAB)		, data, ## __VA_ARGS__)
  	#define XDEBUG_PRINTDATABLOCKCODE(level, data, ...)																													XDebugCtrl->PrintDataBlock((level|XDEBUGCTRLLEVEL_WITHCODE)	  , data, ## __VA_ARGS__)

  #endif

	#define XDEBUG_GETDEBUGFROMXBUFFER(xbufferpacket, publicIP, localIP, level, sequence, xtime, string)				XDebugCtrl->GetDebugFromXBuffer(xbufferpacket, publicIP, localIP, level, sequence, xtime, string)
	#define XDEBUG_GETDEBUGFROMSTREAMIO(streamIO, publicIP, localIP, level, sequence, xtime, string)						XDebugCtrl->GetDebugFromStreamIO(streamIO, publicIP, localIP, level, sequence, xtime, string)
	#define XDEBUG_SETDEBUGTOXBUFFER(publicIP, localIP, level, sequence, xtime, string, xbufferpacket)					XDebugCtrl->SetDebugToXBuffer(publicIP, localIP, level, sequence, xtime, string, xbufferpacket)		

#else

	#define XDEBUG_SETTARGET(index, type , aim)
	#define XDEBUG_ADDTARGET(type, aim)
	#define XDEBUG_SETSIZELIMIT(sizelimit)
	#define XDEBUG_SETAPPLICATIONNAME(name)
	#define XDEBUG_SETAPPLICATIONVERSION(version, subversion, subversionerr)
	#define XDEBUG_SETAPPLICATIONID(ID)
	#define XDEBUG_SETWEBSCRAPERSCRIPURLDOWNLOAD(URL)
	#define XDEBUG_SETPUBLICIP(IP)
	#define XDEBUG_SETLOCALIP(IP)
	#define XDEBUG_RESOLVEDALLRESOURCES
  #define XDEBUG_PRINTHEADER(header)

  #ifdef BUILDER

    #define XDEBUG_PRINT
    #define XDEBUG_PRINTLEVEL
    #define XDEBUG_PRINTCOLOR
    #define XDEBUG_PRINTTAB
    #define XDEBUG_PRINTCODE

    #define XDEBUG_PRINTDATABLOCK
    #define XDEBUG_PRINTDATABLOCKLEVEL
		#define XDEBUG_PRINTDATABLOCKCOLOR
    #define XDEBUG_PRINTDATABLOCKTAB
		#define XDEBUG_PRINTDATABLOCKCODE
	
  #else

    #define XDEBUG_PRINT(level, mask, ...)
    #define XDEBUG_PRINTLEVEL(level, mask, ...)
		#define XDEBUG_PRINTCOLOR(level, mask, ...) {}
    #define XDEBUG_PRINTTAB(level, mask, ...)
		#define XDEBUG_PRINTCODE(level, mask, ...)

		#define XDEBUG_PRINTDATABLOCK(level, data, ...)
    #define XDEBUG_PRINTDATABLOCKLEVEL(level, data, ...)
		#define XDEBUG_PRINTDATABLOCKCOLOR(level, data, ...)
		#define XDEBUG_PRINTDATABLOCKTAB(level, data, ...)
		#define XDEBUG_PRINTDATABLOCKCODE(level, data, ...)		

  #endif

	#define XDEBUG_GETDEBUGFROMXBUFFER(xbufferpacket, publicIP, localIP, level, sequence, xtime, string)				XDebugCtrl->GetDebugFromXBuffer(xbufferpacket, publicIP, localIP, level, sequence, xtime, string)
	#define XDEBUG_GETDEBUGFROMSTREAMIO(streamIO, publicIP, localIP, level, sequence, xtime, string)						XDebugCtrl->GetDebugFromStreamIO(streamIO, publicIP, localIP, level, sequence, xtime, string)
	#define XDEBUG_SETDEBUGTOXBUFFER(publicIP, localIP, level, sequence, xtime, string, xbufferpacket)					XDebugCtrl->SetDebugToXBuffer(publicIP, localIP, level, sequence, xtime, string, xbufferpacket)		


#endif


#endif
