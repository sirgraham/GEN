/*------------------------------------------------------------------------------------------
//	DIOWEBSCRAPERTRANSLATION.CPP
//	
//	Translation (Web Service)
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 05/11/2007 16:26:37
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XThread.h"
#include "DIOWebScraperTranslation.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOTRANSLATIONRESULT::DIOTRANSLATIONRESULT
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/06/2013 20:33:57
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
DIOTRANSLATIONRESULT::DIOTRANSLATIONRESULT()
{
	Clean();
}



/*-------------------------------------------------------------------
//  DIOTRANSLATIONRESULT::~DIOTRANSLATIONRESULT
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/06/2013 20:34:10
//	
//	@return 			virtual : 
//	*/
/*-----------------------------------------------------------------*/
DIOTRANSLATIONRESULT::~DIOTRANSLATIONRESULT()
{
	Clean();
}

	

/*-------------------------------------------------------------------
//  DIOTRANSLATIONRESULT::GetLanguageTextCode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/06/2013 20:34:25
//	
//	@return 			XLANGUAGE_CODE : 
//	*/
/*-----------------------------------------------------------------*/
XLANGUAGE_CODE DIOTRANSLATIONRESULT::GetLanguageTextCode()
{
	return languagetextcode;
}
		


/*-------------------------------------------------------------------
//  DIOTRANSLATIONRESULT::SetLanguageTextCode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/06/2013 20:34:44
//	
//	@return 			bool : 
//	@param				code : 
*/
/*-----------------------------------------------------------------*/
bool DIOTRANSLATIONRESULT::SetLanguageTextCode(XLANGUAGE_CODE code)
{
	languagetextcode = code;

	return true;
}
	


/*-------------------------------------------------------------------
//  DIOTRANSLATIONRESULT::GetLanguageTranslationCode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/06/2013 20:35:00
//	
//	@return 			XLANGUAGE_CODE : 
//	*/
/*-----------------------------------------------------------------*/
XLANGUAGE_CODE DIOTRANSLATIONRESULT::GetLanguageTranslationCode()
{
	return languagetranslationcode;
}
		


/*-------------------------------------------------------------------
//  DIOTRANSLATIONRESULT::SetLanguageTranslationCode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/06/2013 20:35:25
//	
//	@return 			bool : 
//	@param				code : 
*/
/*-----------------------------------------------------------------*/
bool DIOTRANSLATIONRESULT::SetLanguageTranslationCode(XLANGUAGE_CODE code)
{
	languagetranslationcode = code;

	return true;
}




/*-------------------------------------------------------------------
//  DIOTRANSLATIONRESULT::GetTranslation
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/06/2013 20:35:43
//	
//	@return 			XSTRING* : 
//	*/
/*-----------------------------------------------------------------*/
XSTRING* DIOTRANSLATIONRESULT::GetTranslation()
{
	return &translation;
}
		
	

/*-------------------------------------------------------------------
//  DIOTRANSLATIONRESULT::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/06/2013 20:33:15
//	
//	@return 			void : 
//	*/
/*-----------------------------------------------------------------*/
void DIOTRANSLATIONRESULT::Clean()
{	
	languagetextcode				= XLANGUAGE_UNKNOWN;
	languagetranslationcode	= XLANGUAGE_UNKNOWN;
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPERTRANSLATION::DIOWEBSCRAPERTRANSLATION
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/02/2014 17:17:05
//	
//	@return 			void : 

 
 
//  @param				xpublisher : 
//  @param				DIOWEBSCRAPER(xfactory : 
 
 
//  @param				xpublisher : 
*/
/*-----------------------------------------------------------------*/
DIOWEBSCRAPERTRANSLATION::DIOWEBSCRAPERTRANSLATION(  ) : DIOWEBSCRAPER()
{
	Clean();
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERTRANSLATION::~DIOWEBSCRAPERTRANSLATION
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/11/2007 16:28:53
//	
//	@return
//	*/
/*-----------------------------------------------------------------*/
DIOWEBSCRAPERTRANSLATION::~DIOWEBSCRAPERTRANSLATION()
{		
	Clean();
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPERTRANSLATION::ChangeURL
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/03/2013 18:36:20
//	
//	@return 			bool : 
//	@param				maskurl : 
//  @param				url : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERTRANSLATION::ChangeURL(XCHAR* maskurl, DIOURL& url)
{			
	XSTRING _maskurl;

	_maskurl = maskurl;

	int found = _maskurl.Find(__L("google"),true);
	if(found != XSTRING_NOTFOUND)
		{
			DIOURL	urltext;
			XBUFFER	urlbuffer;
			XSTRING googlecodetext;
			XSTRING googlecodetranslate;

			urltext = text.Get();		
			urltext.ConvertToUTF8(urlbuffer);
			urltext.EncodeUnsafeCharsFromBuffer(urlbuffer);

			GoogleLanguageCode(languagetextcode				, googlecodetext);
			GoogleLanguageCode(languagetranslationcode, googlecodetranslate);

			url.Format(maskurl, urltext.Get(), googlecodetext.Get(), googlecodetext.Get(), googlecodetranslate.Get());

			return true;
		}


	return false;
}




/*-------------------------------------------------------------------
//	DIOWEBSCRAPERTRANSLATION::Get
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/05/2014 16:48:18
//	
//	@return 			bool : 
//
//  @param				text : 
//  @param				languagetextcode : 
//  @param				languagetranslationcode : 
//  @param				translation : 
//  @param				timeoutforurl : 
//  @param				localIP : 
//  @param				usecache : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERTRANSLATION::Get(XCHAR* text, XLANGUAGE_CODE languagetextcode, XLANGUAGE_CODE languagetranslationcode, XSTRING& translation, int timeoutforurl, XSTRING* localIP, bool usecache)
{
	XSTRING _text;	
	bool		status = false;

	if(xmutexdo) xmutexdo->Lock();

	_text.Format(__L("%s %02d %02d"), text, languagetextcode, languagetranslationcode);

	this->text											= text;
	this->languagetextcode					= languagetextcode;
	this->languagetranslationcode		= languagetranslationcode;

	DIOTRANSLATIONRESULT* translationresult;
	
	if(usecache)
		{
			translationresult= (DIOTRANSLATIONRESULT*)cache->Get(_text);
			if(translationresult)  
				{
					translation = (*translationresult->GetTranslation()); 

					if(xmutexdo) xmutexdo->UnLock();
					return true;
				}
		}

	if(Load(DIOWEBSCRAPERTRANSLATION_NAMEFILE)) 
		{	
			if(Do(DIOWEBSCRAPERTRANSLATION_NAMESERVICE, timeoutforurl, localIP))
				{	
					translation = GetValue(__L("TRANSLATION"));  

					if(usecache)
						{
							translationresult = new DIOTRANSLATIONRESULT();
							if(translationresult)
								{
									translationresult->SetLanguageTextCode(languagetextcode);
									translationresult->SetLanguageTranslationCode(languagetranslationcode);
									translationresult->GetTranslation()->Set(translation);	

									cache->Add(_text, translationresult);														
								} 
						}

					status = true;
				} 
		} 

	if(xmutexdo) xmutexdo->UnLock();

	return status;
}



/*-------------------------------------------------------------------
//	DIOWEBSCRAPERTRANSLATION::Get
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			12/05/2014 16:48:11
//	
//	@return 			bool : 
//
//  @param				text : 
//  @param				languagetextcode : 
//  @param				languagetranslationcode : 
//  @param				translation : 
//  @param				timeoutforurl : 
//  @param				localIP : 
//  @param				usecache : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERTRANSLATION::Get(XSTRING& text, XLANGUAGE_CODE languagetextcode, XLANGUAGE_CODE languagetranslationcode, XSTRING& translation, int timeoutforurl, XSTRING* localIP, bool usecache)
{
	return Get(text.Get(), languagetextcode, languagetranslationcode, translation, timeoutforurl, localIP, usecache);
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPERTRANSLATION::Clean
*/
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/11/2007 16:28:12
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
void DIOWEBSCRAPERTRANSLATION::Clean()
{

}





/*-------------------------------------------------------------------
//  DIOWEBSCRAPERTRANSLATION::GoogleLanguageCode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/04/2013 12:04:25
//	
//	@return 			bool : 
//	@param				languagecode : 
//  @param				googlecode : 
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERTRANSLATION::GoogleLanguageCode(XLANGUAGE_CODE languagecode, XSTRING& googlecode)
{
	switch(languagecode)
		{
			case XLANGUAGE_KLINGON							: return false;						
			case XLANGUAGE_AFRIKAANS						: googlecode = __L("af");			break;
			case XLANGUAGE_ALBANIAN							: googlecode = __L("sq");			break;
			case XLANGUAGE_ARABIC								: googlecode = __L("ar");			break;
			case XLANGUAGE_AZERBAIJANI					: googlecode = __L("az");			break;
			case XLANGUAGE_BASQUE								: googlecode = __L("eu");			break;
			case XLANGUAGE_BENGALI							: googlecode = __L("bn");			break;
			case XLANGUAGE_BELARUSIAN						: googlecode = __L("be");			break;
			case XLANGUAGE_BULGARIAN						: googlecode = __L("bg");			break;
			case XLANGUAGE_CATALAN							: googlecode = __L("ca");			break;
			case XLANGUAGE_CHINESE_SIMPLIFIED		: googlecode = __L("zh-CN");	break;
			case XLANGUAGE_CHINESE_TRADITIONAL	: googlecode = __L("zh-TW");	break;
			case XLANGUAGE_CROATIAN							: googlecode = __L("hr");			break;
			case XLANGUAGE_CZECH								: googlecode = __L("cs");			break;
			case XLANGUAGE_DANISH								: googlecode = __L("da");			break;
			case XLANGUAGE_DUTCH								: googlecode = __L("nl");			break;
			case XLANGUAGE_ENGLISH							: googlecode = __L("en");			break;	
			case XLANGUAGE_ESPERANTO						: googlecode = __L("eo");			break;
			case XLANGUAGE_ESTONIAN							: googlecode = __L("et");			break;
			case XLANGUAGE_FILIPINO							: googlecode = __L("tl");			break;
			case XLANGUAGE_FINNISH							: googlecode = __L("fi");			break;
			case XLANGUAGE_FRENCH								: googlecode = __L("fr");			break;
			case XLANGUAGE_GALICIAN							: googlecode = __L("gl");			break;
			case XLANGUAGE_GEORGIAN							: googlecode = __L("ka");			break;
			case XLANGUAGE_GERMAN								: googlecode = __L("de");			break;
			case XLANGUAGE_GREEK								: googlecode = __L("el");			break;
			case XLANGUAGE_GUJARATI							: googlecode = __L("gu");			break;	
			case XLANGUAGE_HAITIAN_CREOLE				: googlecode = __L("ht");			break;
			case XLANGUAGE_HEBREW								: googlecode = __L("iw");			break;
			case XLANGUAGE_HINDI								: googlecode = __L("hi");			break;
			case XLANGUAGE_HUNGARIAN						: googlecode = __L("hu");			break;
			case XLANGUAGE_ICELANDIC						: googlecode = __L("is");			break;
			case XLANGUAGE_INDONESIAN						: googlecode = __L("id");			break;
			case XLANGUAGE_IRISH								: googlecode = __L("ga");			break;
			case XLANGUAGE_ITALIAN							: googlecode = __L("it");			break;
			case XLANGUAGE_JAPANESE							: googlecode = __L("ja");			break;
			case XLANGUAGE_KANNADA							: googlecode = __L("kn");			break;
			case XLANGUAGE_KOREAN								: googlecode = __L("ko");			break;
			case XLANGUAGE_LATIN								: googlecode = __L("la");			break;
			case XLANGUAGE_LATVIAN							: googlecode = __L("lv");			break;	
			case XLANGUAGE_LITHUANIAN						: googlecode = __L("lt");			break;
			case XLANGUAGE_MACEDONIAN						: googlecode = __L("mk");			break;
			case XLANGUAGE_MALAY								: googlecode = __L("ms");			break;
			case XLANGUAGE_MALTESE							: googlecode = __L("mt");			break;	
			case XLANGUAGE_NORWEGIAN						: googlecode = __L("no");			break;
			case XLANGUAGE_PERSIAN							: googlecode = __L("fa");			break;
			case XLANGUAGE_POLISH								: googlecode = __L("pl");			break;
			case XLANGUAGE_PORTUGUESE						: googlecode = __L("pt");			break;
			case XLANGUAGE_ROMANIAN							: googlecode = __L("ro");			break;
			case XLANGUAGE_RUSSIAN							: googlecode = __L("ru");			break;
			case XLANGUAGE_SERBIAN							: googlecode = __L("sr");			break;
			case XLANGUAGE_SLOVAK								: googlecode = __L("sk");			break;
			case XLANGUAGE_SLOVENIAN						: googlecode = __L("sl");			break;		
			case XLANGUAGE_SPANISH							: googlecode = __L("es");			break;
			case XLANGUAGE_SWAHILI							: googlecode = __L("sw");			break;
			case XLANGUAGE_SWEDISH							: googlecode = __L("sv");			break;
			case XLANGUAGE_TAMIL								: googlecode = __L("ta");			break;
			case XLANGUAGE_TELUGU								: googlecode = __L("te");			break;
			case XLANGUAGE_THAI									: googlecode = __L("th");			break;
			case XLANGUAGE_TURKISH							: googlecode = __L("tr");			break;		
			case XLANGUAGE_UKRAINIAN						: googlecode = __L("uk");			break; 
			case XLANGUAGE_URDU									: googlecode = __L("ur");			break;
			case XLANGUAGE_VIETNAMESE						: googlecode = __L("vi");			break;
			case XLANGUAGE_WELSH								: googlecode = __L("cy");			break;
			case XLANGUAGE_YIDDISH							: googlecode = __L("yi");			break;																			
		}

	return true;

}