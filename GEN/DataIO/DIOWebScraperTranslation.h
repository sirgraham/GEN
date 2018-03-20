/*------------------------------------------------------------------------------------------
//	DIOWEBSCRAPERTRANSLATION.H
*/	
/**	
// \class 
//   
//  Translation (Web Service)
//   
//	@author	 Abraham J. Velez
//	@version 05/11/2007 16:20:28
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOWEBSCRAPERTRANSLATION_H_
#define _DIOWEBSCRAPERTRANSLATION_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XLanguage.h"

#include "DIOIP.h"
#include "DIOURL.h"
#include "DIOWebScraper.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOWEBSCRAPERTRANSLATION_NAMEFILE				__L("ws_translation.xml")
#define DIOWEBSCRAPERTRANSLATION_NAMESERVICE		__L("TRANSLATION")

/*---- CLASS -----------------------------------------------------------------------------*/


class DIOTRANSLATIONRESULT :  public DIOWEBSCRAPERCACHERESULT
{
	public:	
													  DIOTRANSLATIONRESULT						();
		virtual								 ~DIOTRANSLATIONRESULT						();

		XLANGUAGE_CODE					GetLanguageTextCode							();
		bool										SetLanguageTextCode							(XLANGUAGE_CODE code);
		
		XLANGUAGE_CODE					GetLanguageTranslationCode			();
		bool										SetLanguageTranslationCode			(XLANGUAGE_CODE code);

		XSTRING*								GetTranslation									();
		
	private:	
		
		void										Clean														();	
		
		XLANGUAGE_CODE					languagetextcode;
		XLANGUAGE_CODE					languagetranslationcode;
		XSTRING									translation;
};



class DIOWEBSCRAPERTRANSLATION : public DIOWEBSCRAPER
{
	public:	
														DIOWEBSCRAPERTRANSLATION				(   );
		virtual								 ~DIOWEBSCRAPERTRANSLATION				();

		bool										ChangeURL												(XCHAR* maskurl, DIOURL& url);

		bool										Get															(XCHAR* text	, XLANGUAGE_CODE languagetextcode, XLANGUAGE_CODE languagetranslationcode, XSTRING& translation, int timeoutforurl = DIOWEBSCRAPER_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);
		bool										Get															(XSTRING& text, XLANGUAGE_CODE languagetextcode, XLANGUAGE_CODE languagetranslationcode, XSTRING& translation, int timeoutforurl = DIOWEBSCRAPER_MAXTIMEOUTFORURL, XSTRING* localIP = NULL, bool usecache = true);
		
	private:
		
		void										Clean														();

		bool										GoogleLanguageCode							(XLANGUAGE_CODE languagecode, XSTRING& googlecode);
		
		XSTRING									text;	
		XLANGUAGE_CODE					languagetextcode;
		XLANGUAGE_CODE					languagetranslationcode;
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

