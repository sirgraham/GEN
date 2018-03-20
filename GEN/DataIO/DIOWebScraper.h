/*------------------------------------------------------------------------------------------
//	DIOWEBSCRAPER.H
*/	
/**	
// \class 
//   
//  Web Scraper Class
//   
//	@author	 Abraham J. Velez
//	@version 05/11/2007 16:20:28
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOWEBSCRAPER_H_
#define _DIOWEBSCRAPER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XVector.h"
#include "XMap.h"

#include "DIOWebClient.h"
#include "DIOWebScraperCache.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOWEBSCRAPER_MAXTIMEOUTFORURL				 10
#define DIOWEBSCRAPER_NAMEFILE								 __L("webscraper.xml")
#define DIOWEBSCRAPER_DEFAULTUSERAGENT       	 DIOWEBCLIENT_DEFAULTUSERAGENT

/*---- CLASS -----------------------------------------------------------------------------*/
	
class XFACTORY;
class XPUBLISHER;
class XDATETIME;
class XTIMER;
class XRAND;
class XBUFFER;
class XFILEXML;
class XPATHS;
class DIOFACTORY;
class DIOCHECKINTERNETCONNEXION;


class DIOWEBSCRAPER 
{
	public:	
																		DIOWEBSCRAPER													();
		virtual												 ~DIOWEBSCRAPER													();
		
		bool														Load																	(XCHAR* namefile);
		bool														Load																	(XPATH& namefile);

		DIOURL*                         GetURLDownload                        ()                                   { return URLdownload;                               }

		bool														Download															(DIOURL& URL, XPATH& xpath);

		bool														Do																		(XCHAR* namewebservice  , int timeoutforurl = DIOWEBSCRAPER_MAXTIMEOUTFORURL, XSTRING* localIP = NULL);		
		bool														Do																		(XSTRING& namewebservice, int timeoutforurl = DIOWEBSCRAPER_MAXTIMEOUTFORURL, XSTRING* localIP = NULL);

		virtual bool										ChangeURL															(XCHAR* maskurl, DIOURL& url);			

		bool														AddValue															(XCHAR* name, XCHAR* value);
		bool														AddValue															(XCHAR* name, XSTRING& value);
		bool														AddValue															(XSTRING& name,XCHAR*  value);
		bool														AddValue															(XSTRING& name, XSTRING& value);
		XCHAR*													GetValue															(XCHAR* namevalue);
		XCHAR*													GetValue															(XSTRING& namevalue);
		bool														DeleteAllValues												();
		
		DIOWEBCLIENT*										GetWebClient													();

		bool														ExtractString													(XCHAR* searchini, XCHAR* searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result);
		bool														ExtractString													(XCHAR* searchini, XSTRING& searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result);
		bool														ExtractString													(XSTRING& searchini,XCHAR* searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result);
		bool														ExtractString													(XSTRING& searchini, XSTRING& searchend, XBUFFER* xbuffer, bool usebufferpos, XSTRING& result);

	protected:
							
		XMUTEX*													xmutexdo;
		DIOWEBCLIENT*										webclient;				
		DIOWEBSCRAPERCACHE*							cache;

	private:

		void														Clean																	()
																		{
																			xmutexdo								= NULL;
																			webclient								= NULL;
																			cache                   = NULL;

																			URLdownload							= NULL;
	
																			xml											= NULL;
																		}

		void														HandleEvent														(XEVENT* xevent);	
					
		DIOURL*													URLdownload;
		XFILEXML*												xml;
		XMAP<XSTRING*,XSTRING*>					values;

};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

