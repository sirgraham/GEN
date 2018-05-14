//------------------------------------------------------------------------------------------
//  XLANGUAGE.H
//
/**
// \class
//
//  Language funtions
//
//  @author  Abraham J. Velez
//  @version 16/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XLANGUAGE_H_
#define _XLANGUAGE_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XVector.h"
#include "XString.h"
#include "XPath.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum XLANGUAGE_CODE
{
  XLANGUAGE_KLINGON         = 0 ,

  XLANGUAGE_AFRIKAANS           ,
  XLANGUAGE_ALBANIAN            ,
  XLANGUAGE_ARABIC              ,
  XLANGUAGE_AZERBAIJANI         ,
  XLANGUAGE_BASQUE              ,
  XLANGUAGE_BENGALI             ,
  XLANGUAGE_BELARUSIAN          ,
  XLANGUAGE_BULGARIAN           ,
  XLANGUAGE_CATALAN             ,
  XLANGUAGE_CHINESE_SIMPLIFIED  ,
  XLANGUAGE_CHINESE_TRADITIONAL ,
  XLANGUAGE_CROATIAN            ,
  XLANGUAGE_CZECH               ,
  XLANGUAGE_DANISH              ,
  XLANGUAGE_DUTCH               ,
  XLANGUAGE_ENGLISH             ,
  XLANGUAGE_ESPERANTO           ,
  XLANGUAGE_ESTONIAN            ,
  XLANGUAGE_FILIPINO            ,
  XLANGUAGE_FINNISH             ,
  XLANGUAGE_FRENCH              ,
  XLANGUAGE_GALICIAN            ,
  XLANGUAGE_GEORGIAN            ,
  XLANGUAGE_GERMAN              ,
  XLANGUAGE_GREEK               ,
  XLANGUAGE_GUJARATI            ,
  XLANGUAGE_HAITIAN_CREOLE      ,
  XLANGUAGE_HEBREW              ,
  XLANGUAGE_HINDI               ,
  XLANGUAGE_HUNGARIAN           ,
  XLANGUAGE_ICELANDIC           ,
  XLANGUAGE_INDONESIAN          ,
  XLANGUAGE_IRISH               ,
  XLANGUAGE_ITALIAN             ,
  XLANGUAGE_JAPANESE            ,
  XLANGUAGE_KANNADA             ,
  XLANGUAGE_KOREAN              ,
  XLANGUAGE_LATIN               ,
  XLANGUAGE_LATVIAN             ,
  XLANGUAGE_LITHUANIAN          ,
  XLANGUAGE_MACEDONIAN          ,
  XLANGUAGE_MALAY               ,
  XLANGUAGE_MALTESE             ,
  XLANGUAGE_NORWEGIAN           ,
  XLANGUAGE_PERSIAN             ,
  XLANGUAGE_POLISH              ,
  XLANGUAGE_PORTUGUESE          ,
  XLANGUAGE_ROMANIAN            ,
  XLANGUAGE_RUSSIAN             ,
  XLANGUAGE_SERBIAN             ,
  XLANGUAGE_SLOVAK              ,
  XLANGUAGE_SLOVENIAN           ,
  XLANGUAGE_SPANISH             ,
  XLANGUAGE_SWAHILI             ,
  XLANGUAGE_SWEDISH             ,
  XLANGUAGE_TAMIL               ,
  XLANGUAGE_TELUGU              ,
  XLANGUAGE_THAI                ,
  XLANGUAGE_TURKISH             ,
  XLANGUAGE_UKRAINIAN           ,
  XLANGUAGE_URDU                ,
  XLANGUAGE_VIETNAMESE          ,
  XLANGUAGE_WELSH               ,
  XLANGUAGE_YIDDISH             ,

  XLANGUAGE_UNKNOWN             ,
  XLANGUAGE_LAST                ,
};


#define XLANGUAGE_LANGUAGES       "languages"
#define XLANGUAGE_LANGUAGE        "language"
#define XLANGUAGE_TRANSLATIONS    "translations"
#define XLANGUAGE_TRANSLATION     "translation"

#define XLANGUAGE_NAMEFILEEXT     __L(".lng")

#define XLNG(ID)                   XLanguage->GetString(__L(ID))

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPATH;


class XLANGUAGEAVAILABLE
{
  public:
                                      XLANGUAGEAVAILABLE                  ();
    virtual                          ~XLANGUAGEAVAILABLE                  ();

    int                               code;
    XSTRING*                          ID;
    XSTRING*                          name;

  private:

    void                              Clean                               ();

};


class XLANGUAGESTRING
{
  public:
                                      XLANGUAGESTRING                     ();
    virtual                          ~XLANGUAGESTRING                     ();

    bool                              fixed;
    XSTRING*                          ID;
    XSTRING*                          string;

  private:

    void                              Clean                               ();

};


class XLANGUAGE
{
  public:
                                      XLANGUAGE                           ();
    virtual                          ~XLANGUAGE                           ();

    bool                              Ini                                 (XPATH& xpath);
    bool                              End                                 ();

    XLANGUAGE_CODE                    GetLanguageCode                     ();

    virtual bool                      ChangeLanguage                      (XLANGUAGE_CODE languagecode);

    bool                              LoadLanguagesAvailable              ();
    bool                              DeleteAllLanguagesAvailable         ();

    bool                              IsLanguageAvailable                 (XLANGUAGE_CODE languagecode);
    int                               GetNLanguagesAvailable              ();
    XLANGUAGE_CODE                    GetIndexLanguageAvailableCode       (int index);
    bool                              GetLanguageAvailableID              (XLANGUAGE_CODE languagecode,XSTRING& ID);
    bool                              GetLanguageAvailableName            (XLANGUAGE_CODE languagecode,XSTRING& name);

    bool                              LoadTranslation                     (XLANGUAGE_CODE languagecode);
    bool                              DeleteAllStrings                    ();

    XCHAR*                            GetString                           (char* ID);
    XCHAR*                            GetString                           (XCHAR* ID);
    XCHAR*                            GetString                           (XSTRING& ID);

  protected:

    XPATH                             xpath;
    XLANGUAGE_CODE                    languagecode;

  private:

    void                              Clean                               ();

    XVECTOR<XLANGUAGEAVAILABLE*>      languageavailables;
    XVECTOR<XLANGUAGESTRING*>         languagestrings;
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------


extern XLANGUAGE* xlanguage;

#endif

