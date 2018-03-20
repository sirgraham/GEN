/*------------------------------------------------------------------------------------------
//	APPUPDATE.H
*/	
/**	
// \class 
//   
//  DNS Manager class
//   
//	@author	 Abraham J. Velez
//	@version 13/11/2013 12:20:06
*/	
/*	EndoraSoft (C) Copyright. All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _APPUPDATE_H_
#define _APPUPDATE_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XDateTime.h"
#include "XTimer.h"
#include "XFSMachine.h"
#include "XApplication.h"
#include "XString.h"
#include "XScheduler.h"
#include "XDir.h"

#include "DIOStream.h"
#include "DIOURL.h"
#include "DIOWebServerQueryString.h"

	
//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum APPUPDATEXFSMEVENTS
{
	APPUPDATE_XFSMEVENT_NONE										= 0 ,
	APPUPDATE_XFSMEVENT_INI													,
	APPUPDATE_XFSMEVENT_RUN													,
	APPUPDATE_XFSMEVENT_END													,
	
	APPUPDATE_LASTEVENT
};


enum APPUPDATEXFSMSTATES
{
	APPUPDATE_XFSMSTATE_NONE										= 0 ,	
	APPUPDATE_XFSMSTATE_INI													,
	APPUPDATE_XFSMSTATE_RUN													,
	APPUPDATE_XFSMSTATE_END													,
	
	APPUPDATE_LASTSTATE													
};


enum APPUPDATETASKID							
{
	APPUPDATETASKID_UNKNOWN											= 0 ,
	APPUPDATETASKID_CHECKINTERNETCONNEXION					,
	APPUPDATETASKID_CHECKMEMORYSTATUS								,	
};




#define APPUPDATE_VERSION												0
#define APPUPDATE_SUBVERSION										2
#define APPUPDATE_SUBVERSIONERR									0

#define APPUPDATE_NAMEAPP			 									__L("APPUpdate")
#define APPUPDATE_ENTERPRISE	 									__L("EndoraSoft")

#define APPUPDATE_CFGNAMEFILE	 									__L("appupdate")
#define APPUPDATE_LNGNAMEFILE	 									APPUPDATE_CFGNAMEFILE
#define APPUPDATE_LOGNAMEFILE	 									APPUPDATE_CFGNAMEFILE

#define APPUPDATE_DIRECTORYMAIN									__L("resources")

#define APPUPDATE_MESSAGEMASK										__L("%-36s ... ")


//---- CLASS -------------------------------------------------------------------------------

class XRAND;
class XTHREAD;
class XSCHEDULER;
class DIOCHECKCONNEXIONS;
class DIOCHECKINTERNETCONNEXION;
class DIOWEBCLIENT;
class DIOWEBSCRAPERPUBLICIP;
class DIOWEBSCRAPERGEOLOCATIONIP;				
class APPUPDATECFG;



class APPUPDATE : public XAPPLICATION , public XFSMACHINE
{
	public:	
																			APPUPDATE													();													
		virtual													 ~APPUPDATE													();

		bool															Ini 																();

		bool															InitFSMachine												();
		
		bool															FirstUpdate													();
    bool															PrevUpdate													();		

		bool															Update															();

		bool															End																	();

		bool															IsMemoryFreeAvaible									(int minimumpercent);

		bool															ShowHeader													(bool separator);			
		bool															ShowLine														(XSTRING& string, XSTRING& string2, bool dolog = true, int tab = 3, bool lf = true);
		bool															ShowConfig													(bool dolog = true);
		bool															ShowAppStatus												(bool dolog = true);
		bool															ShowInternetStatus									(bool dolog = true);

		bool															GetApplicationParam									(XPATH& xpath, DIOAPPLICATIONUPDATEVERSIONDATA& applicationversiondata);
		bool															CreateListOfFiles										(XPATH& xpath, XVECTOR<XPATH*>* updatefiles);
		bool															GenerateUpdateFile									(XPATH& xpath, DIOAPPLICATIONUPDATEVERSIONDATA& applicationversiondata, XDWORD& nfiles, XDWORD& sizetotal);

				
	private:

		void															Clean																()
																			{
																				xtimer														= NULL;
																				xtimerupdate											= NULL;
		
																				cfg																= NULL;		

																				xconsole													= NULL;	

																				checkconnexions										= NULL;
																				checkinternetconnexion						= NULL;

																				publicip													= NULL;
																				geolocationip											= NULL;

																				xscheduler												= NULL;		
																				haveinternet											= false;
																			}


		
		bool															CheckInternetStatus									();
		bool															CheckApplicationStatus							();

		void															HandleEvent													(XEVENT* xevent);				

		XTIMER*														xtimer;		
		XTIMER*														xtimerupdate;	
		
		XDATETIME*												xdatetime;

		APPUPDATECFG*											cfg;

		XCONSOLE*													xconsole;

		DIOCHECKCONNEXIONS*								checkconnexions;
		DIOCHECKINTERNETCONNEXION*				checkinternetconnexion;
		
		DIOWEBSCRAPERPUBLICIP*						publicip;
		DIOWEBSCRAPERGEOLOCATIONIP*				geolocationip;			
		
		XSCHEDULER*												xscheduler;
							
		bool															haveinternet;		
		
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------

extern APPUPDATE* appupdate;

#endif

