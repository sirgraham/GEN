//------------------------------------------------------------------------------------------
//  XLANGUAGE.CPP
//
//  Language Funtions
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 16/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "XFileXML.h"

#include "XLanguage.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


XLANGUAGE* xlanguage  = NULL;

//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  XLANGUAGEAVAILABLE::XLANGUAGEAVAILABLE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/01/2011 23:29:14
//
//  @return
//  */
/*-----------------------------------------------------------------*/
XLANGUAGEAVAILABLE::XLANGUAGEAVAILABLE()
{
  Clean();

  ID   = new XSTRING();
  name = new XSTRING();
}


/*-------------------------------------------------------------------
//  XLANGUAGEAVAILABLE::~XLANGUAGEAVAILABLE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/01/2011 23:29:37
//
//  @return       virtual :
//  */
/*-----------------------------------------------------------------*/
XLANGUAGEAVAILABLE::~XLANGUAGEAVAILABLE()
{
  delete ID;
  delete name;

  Clean();
}



/*-------------------------------------------------------------------
//  XLANGUAGEAVAILABLE::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/01/2011 23:32:20
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void XLANGUAGEAVAILABLE::Clean()
{
  code = -1;
  ID   = NULL;
  name = NULL;
}



//-------------------------------------------------------------------
//  XLANGUAGESTRING::XLANGUAGESTRING
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/10/2004 16:30:18
//
//  @return
//  */
//-------------------------------------------------------------------
XLANGUAGESTRING::XLANGUAGESTRING()
{
  Clean();

  ID     = new XSTRING();
  string = new XSTRING();
}


//-------------------------------------------------------------------
//  XLANGUAGESTRING::~XLANGUAGESTRING
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/10/2004 16:30:39
//
//  @return
//  */
//-------------------------------------------------------------------
XLANGUAGESTRING::~XLANGUAGESTRING()
{
  delete ID;
  delete string;

  Clean();
}




//-------------------------------------------------------------------
//  XLANGUAGESTRING::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/10/2004 16:30:59
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void XLANGUAGESTRING::Clean()
{
  fixed     = false;
  ID        = NULL;
  string    = NULL;
}





/*-------------------------------------------------------------------
//  XLANGUAGE::XLANGUAGE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/01/2011 1:32:49
//
//  @return
*/
/*-----------------------------------------------------------------*/
XLANGUAGE::XLANGUAGE()
{
  Clean();
}



//-------------------------------------------------------------------
//  XLANGUAGE::~XLANGUAGE
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/04/2002 10:32:08
//
//  @return       void :
//  */
//-------------------------------------------------------------------
XLANGUAGE::~XLANGUAGE()
{
  End();

  Clean();
}



/*-------------------------------------------------------------------
//  XLANGUAGE::Ini
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/02/2013 7:02:25
//
//  @return       bool :

//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool XLANGUAGE::Ini(XPATH& xpath)
{
   this->xpath     = xpath;

   return LoadLanguagesAvailable();
}


/*-------------------------------------------------------------------
//  XLANGUAGE::End
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/02/2013 7:02:31
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XLANGUAGE::End()
{
  DeleteAllLanguagesAvailable();
  DeleteAllStrings();

  return true;
}



//-------------------------------------------------------------------
//  XLANGUAGE::GetLanguageCode
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/04/2002 10:32:08
//
//  @return       XLANGUAGE_CODE :
//  */
//-------------------------------------------------------------------
XLANGUAGE_CODE XLANGUAGE::GetLanguageCode()
{
  return languagecode;
}



//-------------------------------------------------------------------
//  XLANGUAGE::ChangeLanguage
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/04/2002 11:52:57
//
//  @return       bool :
//  @param        language :
*/
//-------------------------------------------------------------------
bool XLANGUAGE::ChangeLanguage(XLANGUAGE_CODE languagecode)
{
  if(!IsLanguageAvailable(languagecode)) return false;

  this->languagecode = languagecode;

  bool status = LoadTranslation(languagecode);

  return status;
}



/*-------------------------------------------------------------------
//  XLANGUAGE::LoadLanguagesAvailable
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/01/2011 17:39:13
//
//  @return       bool :
*/
/*-----------------------------------------------------------------*/
bool XLANGUAGE::LoadLanguagesAvailable()
{
  XFILEXML* xmlfile = new XFILEXML();
  if(!xmlfile) return false;

  bool status = false;

  if(xmlfile->Open(xpath))
    {
      xmlfile->ReadAndDecodeAllLines();

      int index = 0;

      XFILEXMLELEMENT* father = xmlfile->SearchElement(__L("languages"),index);
      if(father)
        {
          for(int c=0;c<father->GetNElements();c++)
            {
              XFILEXMLELEMENT* element = father->GetElement(c);
              if(element)
                {
                  XLANGUAGEAVAILABLE* languageavailable = new XLANGUAGEAVAILABLE();
                  if(languageavailable)
                    {
                      XSTRING value;

                      if(xmlfile->SearchAttributeValue(element,__L("code"),value)) languageavailable->code = value.ConvertToInt();
                      if(xmlfile->SearchAttributeValue(element,__L("ID"),value))   languageavailable->ID->Set(value);
                      if(xmlfile->SearchAttributeValue(element,__L("name"),value)) languageavailable->name->Set(value);

                      languageavailables.Add(languageavailable);
                      status = true;
                    }
                }
            }
        }


      xmlfile->Close();
    }

  delete xmlfile;

  return status;
}



//-------------------------------------------------------------------
//  XLANGUAGE::DeleteAllStrings
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/12/2004 11:40:35
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XLANGUAGE::DeleteAllLanguagesAvailable()
{
  if(languageavailables.IsEmpty())  return false;

  languageavailables.DeleteContents();
  languageavailables.DeleteAll();

  return true;
}


/*-------------------------------------------------------------------
//  XLANGUAGE::IsLanguageAvailable
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/01/2011 0:55:26
//
//  @return       bool :
//  @param        languagecode :
*/
/*-----------------------------------------------------------------*/
bool XLANGUAGE::IsLanguageAvailable(XLANGUAGE_CODE languagecode)
{
  for(int c=0;c<(int)languageavailables.GetSize();c++)
    {
      XLANGUAGEAVAILABLE* languageavailable = (XLANGUAGEAVAILABLE*)languageavailables.Get(c);
      if(languageavailable)
        {
          if(languageavailable->code == languagecode) return true;
        }
    }

  return false;
}



/*-------------------------------------------------------------------
//  XLANGUAGE::GetNLanguagesAvailable
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/01/2011 1:23:08
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int XLANGUAGE::GetNLanguagesAvailable()
{
  return languageavailables.GetSize();
}



/*-------------------------------------------------------------------
//  XLANGUAGE::GetIndexLanguageAvailableCode
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/01/2011 1:25:56
//
//  @return       XLANGUAGE_CODE :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
XLANGUAGE_CODE XLANGUAGE::GetIndexLanguageAvailableCode(int index)
{
  XLANGUAGEAVAILABLE* languageavailable = (XLANGUAGEAVAILABLE*)languageavailables.Get(index);
  if(languageavailable) return (XLANGUAGE_CODE)languageavailable->code;

  return XLANGUAGE_UNKNOWN;
}



/*-------------------------------------------------------------------
//  XLANGUAGE::GetLanguageAvailableID
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/01/2011 0:58:34
//
//  @return       bool :
//  @param        languagecode :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
bool XLANGUAGE::GetLanguageAvailableID(XLANGUAGE_CODE languagecode,XSTRING& ID)
{
  for(int c=0;c<(int)languageavailables.GetSize();c++)
    {
      XLANGUAGEAVAILABLE* languageavailable = (XLANGUAGEAVAILABLE*)languageavailables.Get(c);
      if(languageavailable)
        {
          if(languageavailable->code == languagecode)
            {
              ID.Set((*languageavailable->ID));

              return true;
            }
        }
    }

  return false;
}



/*-------------------------------------------------------------------
//  XLANGUAGE::GetLanguageAvailableName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/01/2011 0:58:34
//
//  @return       bool :
//  @param        languagecode :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool XLANGUAGE::GetLanguageAvailableName(XLANGUAGE_CODE languagecode,XSTRING& name)
{
  for(int c=0;c<(int)languageavailables.GetSize();c++)
    {
      XLANGUAGEAVAILABLE* languageavailable = (XLANGUAGEAVAILABLE*)languageavailables.Get(c);
      if(languageavailable)
        {
          if(languageavailable->code == languagecode)
            {
              name.Set((*languageavailable->name));

              return true;
            }
        }
    }

  return false;
}




/*-------------------------------------------------------------------
//  XLANGUAGE::LoadTranslation
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/01/2011 1:40:02
//
//  @return       bool :
//  @param        languagecode :
*/
/*-----------------------------------------------------------------*/
bool XLANGUAGE::LoadTranslation(XLANGUAGE_CODE languagecode)
{
  if(!IsLanguageAvailable(languagecode)) return false;

  XSTRING languageID;

  if(!GetLanguageAvailableID(languagecode,languageID)) return false;

  DeleteAllStrings();

  XFILEXML* xmlfile = new XFILEXML();
  if(!xmlfile) return false;

  bool status = false;

  if(xmlfile->Open(xpath))
    {
      xmlfile->ReadAndDecodeAllLines();

      int index = 0;

      XFILEXMLELEMENT* father = xmlfile->SearchElement(__L("translations"),index);
      if(father)
        {
          XFILEXMLELEMENT* etranslation     = NULL;
          int              indextranslation = 0;

          do{ etranslation = xmlfile->SearchElement(__L("translation"),indextranslation,father);
              if(etranslation)
                {
                  int              indexcode = 0;
                  XFILEXMLELEMENT* ecode     =  xmlfile->SearchElement(languageID.Get(),indexcode,etranslation);
                  if(ecode)
                    {
                      XSTRING value;

                      if(xmlfile->SearchAttributeValue(ecode,__L("string"),value))
                        {
                          XLANGUAGESTRING* languagestring = new XLANGUAGESTRING();
                          if(languagestring)
                            {
                              languagestring->string->Set(value);
                              if(xmlfile->SearchAttributeValue(etranslation , __L("ID"),value))    languagestring->ID->Set(value);
                              if(xmlfile->SearchAttributeValue(etranslation , __L("fixed"),value)) languagestring->fixed = value.ConvertToInt()?true:false;

                              languagestrings.Add(languagestring);

                              status = true;
                            }
                        }
                    }

                  indextranslation++;
                }

            } while(etranslation);
        }

      xmlfile->Close();
    }

  delete xmlfile;

  return status;
}



//-------------------------------------------------------------------
//  XLANGUAGE::DeleteAllStrings
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/12/2004 11:40:35
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XLANGUAGE::DeleteAllStrings()
{
  if(languagestrings.IsEmpty())  return false;

  languagestrings.DeleteContents();
  languagestrings.DeleteAll();

  return true;
}



/*-------------------------------------------------------------------
//  XLANGUAGE::GetString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/01/2011 16:46:34
//
//  @return       XCHAR* :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
XCHAR* XLANGUAGE::GetString(char* ID)
{
  XSTRING xID(ID);

  XCHAR* result = GetString(xID);

  return result;
}



/*-------------------------------------------------------------------
//  XLANGUAGE::GetString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2011 11:31:40
//
//  @return       XCHAR* :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
XCHAR* XLANGUAGE::GetString(XCHAR* ID)
{
  XSTRING xID(ID);

  XCHAR* result = GetString(xID);

  return result;
}



//-------------------------------------------------------------------
//  XLANGUAGE::GetString
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/04/2002 11:53:28
//
//  @return       XCHAR* :
//  @param        code :
*/
//-------------------------------------------------------------------
XCHAR* XLANGUAGE::GetString(XSTRING& ID)
{
  for(XDWORD c=0;c<languagestrings.GetSize();c++)
    {
      XLANGUAGESTRING* languagestring = (XLANGUAGESTRING*)languagestrings.Get(c);

      if(languagestring)
        {
          if(languagestring->ID)
            {
              if(!languagestring->ID->Compare(ID,true))
                {
                  return (XCHAR*)languagestring->string->Get();
                }
            }
        }
    }

  return NULL;
}



//-------------------------------------------------------------------
//  XLANGUAGE::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/12/2004 10:05:20
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void XLANGUAGE::Clean()
{
  languagecode        = XLANGUAGE_KLINGON;
}

