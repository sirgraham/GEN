//------------------------------------------------------------------------------------------
//  XFILECONFIG.CPP
//
//  Application CFG class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/05/2004 20:55:19
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XPath.h"
#include "XFileINI.h"
#include "XVector.h"

#include "XFileConfig.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  XFILECONFIGVALUE::XFILECONFIGVALUE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2013 11:10:34
//
//  @return
//  */
/*-----------------------------------------------------------------*/
XFILECONFIGVALUE::XFILECONFIGVALUE()
{
  Clean();
}




/*-------------------------------------------------------------------
//  XFILECONFIGVALUE::~XFILECONFIGVALUE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2013 11:10:11
//
//  @return
//  */
/*-----------------------------------------------------------------*/
XFILECONFIGVALUE::~XFILECONFIGVALUE()
{
  Clean();
}




/*-------------------------------------------------------------------
//  XFILECONFIGVALUE::GetType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2013 11:10:08
//
//  @return       XFILECONFIGVALUETYPE :
//  */
/*-----------------------------------------------------------------*/
XFILECONFIGVALUETYPE XFILECONFIGVALUE::GetType()
{
  return type;
}




/*-------------------------------------------------------------------
//  XFILECONFIGVALUE::SetType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2013 11:09:50
//
//  @return       bool :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
bool XFILECONFIGVALUE::SetType(XFILECONFIGVALUETYPE type)
{
  this->type = type;
  return true;
}




/*-------------------------------------------------------------------
//  XFILECONFIGVALUE::GetGroup
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2013 11:09:24
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* XFILECONFIGVALUE::GetGroup()
{
  return &group;
}




/*-------------------------------------------------------------------
//  XFILECONFIGVALUE::GetID
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2013 10:11:18
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* XFILECONFIGVALUE::GetID()
{
  return &ID;
}




/*-------------------------------------------------------------------
//  XFILECONFIGVALUE::GetValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2013 10:11:01
//
//  @return       void* :
//  */
/*-----------------------------------------------------------------*/
void* XFILECONFIGVALUE::GetValue()
{
  return value;
}




/*-------------------------------------------------------------------
//  XFILECONFIGVALUE::SetValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2013 8:26:56
//
//  @return       bool :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool XFILECONFIGVALUE::SetValue(void* value)
{
  this->value = value;
  return true;
}







/*-------------------------------------------------------------------
//  XFILECONFIGVALUE::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2013 8:26:17
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void XFILECONFIGVALUE::Clean()
{
  type = XFILECONFIGVALUETYPE_UNKNOWN;
  value = NULL;
}







/*-------------------------------------------------------------------
//  XFILECONFIG::XFILECONFIG
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/09/2012 10:41:04
//
//  @return       void :


//  @param        namefile :
*/
/*-----------------------------------------------------------------*/
XFILECONFIG::XFILECONFIG(XCHAR* namefile)
{
  Clean();

  if(!namefile) return;

  this->namefile = namefile;

  if(!this->namefile.IsEmpty())
    {
      XPATH xpathroot;

      XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathroot);

      xpathfile.Add(xpathroot.Get());
      if(!xpathfile.IsEmpty()) xpathfile.Slash_Add();
      xpathfile.Add(this->namefile.Get());
      xpathfile.Add(XFILECONFIG_EXTENSIONFILE);

      fileini = new XFILEINI();
    }
}




//-------------------------------------------------------------------
//  XFILECONFIG::~XFILECONFIG
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/05/2004 20:56:09
//
//  @return
//  */
//-------------------------------------------------------------------
XFILECONFIG::~XFILECONFIG()
{
  if(!namefile.IsEmpty())
    {
      if(fileini) delete fileini;
    }

  DeleteAllValues();

  Clean();
}



//-------------------------------------------------------------------
//  XFILECONFIG::Ini
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/08/2004 16:27:54
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XFILECONFIG::Ini()
{
  if(namefile.IsEmpty())   return false;
  if(xpathfile.IsEmpty())  return false;

  bool status[2];

  status[0] = Load();
  status[1] = Save();

  return (status[0] && status[1]);
}




//-------------------------------------------------------------------
//  XFILECONFIG::Load
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/05/2004 20:56:44
//
//  @return       bool :
//  @param        xpath :
*/
//-------------------------------------------------------------------
bool XFILECONFIG::Load(XPATH& xpath)
{
  if(values.IsEmpty()) return false;

  if(!IniFile(xpath)) return false;

  for(int c=0;c<(int)values.GetSize();c++)
    {
      XFILECONFIGVALUE* cfgvalue = (XFILECONFIGVALUE*)values.Get(c);
      if(cfgvalue)
        {
          XSTRING strvalue;

          if(fileini->ReadValue(cfgvalue->GetGroup()->Get(), cfgvalue->GetID()->Get() , strvalue))
            {
              switch(cfgvalue->GetType())
                {
                  case XFILECONFIGVALUETYPE_UNKNOWN : break;

                  case XFILECONFIGVALUETYPE_INT     : { int* value = (int*)cfgvalue->GetValue();
                                                        if(!value) return false;

                                                        (*value) = strvalue.ConvertToInt();
                                                      }
                                                      break;

                  case XFILECONFIGVALUETYPE_MASK    : { XWORD* value = (XWORD*)cfgvalue->GetValue();
                                                        if(!value) return false;

                                                        strvalue.UnFormat(__L("%04X"), value);
                                                      }
                                                      break;

                  case XFILECONFIGVALUETYPE_FLOAT   : { float* value = (float*)cfgvalue->GetValue();
                                                        if(!value) return false;

                                                        (*value) = strvalue.ConvertToFloat();
                                                      }
                                                      break;

                  case XFILECONFIGVALUETYPE_STRING  : { XSTRING* value = (XSTRING*)cfgvalue->GetValue();
                                                        if(!value) return false;

                                                        (*value) = strvalue.Get();

                                                        value->DeleteCharacter(__C('\t'));
                                                        value->DeleteCharacter(__C('\n'));
                                                        value->DeleteCharacter(__C('\r'));

                                                        value->DeleteCharacter(__C(' '), XSTRINGCONTEXT_ATEND);
                                                      }
                                                      break;

                  case XFILECONFIGVALUETYPE_BOOLEAN : { bool* value = (bool*)cfgvalue->GetValue();
                                                        if(!value) return false;

                                                        (*value) = strvalue.ConvertToBoolean();
                                                      }
                                                      break;
              }
            }

        }
    }

  return EndFile();
}



/*-------------------------------------------------------------------
//  XFILECONFIG::Save
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/10/2008 12:35:41 p.m.
//
//  @return       bool :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool XFILECONFIG::Save(XPATH& xpath)
{
  if(values.IsEmpty()) return false;

  if(!IniFile(xpath)) return false;

  fileini->DeleteAllSections();

  for(int c=0;c<(int)values.GetSize();c++)
    {
      XSTRING strvalue;

      XFILECONFIGVALUE* cfgvalue = (XFILECONFIGVALUE*)values.Get(c);
      if(cfgvalue)
        {
          switch(cfgvalue->GetType())
            {
              case XFILECONFIGVALUETYPE_UNKNOWN : break;

              case XFILECONFIGVALUETYPE_INT     : { int* value = (int*)cfgvalue->GetValue();
                                                    if(!value) return false;

                                                    strvalue.Format(__L("%d"), (*value));
                                                  }
                                                  break;

              case XFILECONFIGVALUETYPE_MASK    : { XWORD* value = (XWORD*)cfgvalue->GetValue();
                                                    if(!value) return false;

                                                    strvalue.Format(__L("%04X"), (*value));
                                                  }
                                                  break;

              case XFILECONFIGVALUETYPE_FLOAT   : { float* value = (float*)cfgvalue->GetValue();
                                                    if(!value) return false;

                                                    strvalue.Format(__L("%f"), (*value));
                                                  }
                                                  break;

              case XFILECONFIGVALUETYPE_STRING  : { XSTRING* value = (XSTRING*)cfgvalue->GetValue();
                                                    if(!value) return false;

                                                    strvalue = value->Get();
                                                  }
                                                  break;

              case XFILECONFIGVALUETYPE_BOOLEAN : { bool* value = (bool*)cfgvalue->GetValue();
                                                    if(!value) return false;

                                                    strvalue = (*value)?__L("yes"):__L("no");
                                                  }
                                                  break;
            }

          if(!fileini->WriteValue(cfgvalue->GetGroup()->Get(), cfgvalue->GetID()->Get() , strvalue))
            {
              return false;
            }

        }
    }

  return EndFile();
}



//-------------------------------------------------------------------
//  XFILECONFIG::End
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/01/2005 16:55:30
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XFILECONFIG::End()
{
  return true;
}




/*-------------------------------------------------------------------
//  XFILECONFIG::AddValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2013 11:15:36
//
//  @return       bool :
//  @param        type :
//  @param        group :
//  @param        ID :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool XFILECONFIG::AddValue(XFILECONFIGVALUETYPE type, XCHAR* group, XCHAR* ID, void* value)
{
  XFILECONFIGVALUE* cfgvalue = new XFILECONFIGVALUE();
  if(!cfgvalue) return false;

  cfgvalue->SetType(type);
  cfgvalue->GetGroup()->Set(group);
  cfgvalue->GetID()->Set(ID);
  cfgvalue->SetValue(value);

  values.Add(cfgvalue);

  return true;
}




/*-------------------------------------------------------------------
//  XFILECONFIG::DeleteAllValues
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2013 11:15:42
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XFILECONFIG::DeleteAllValues()
{
  if(values.IsEmpty()) return false;

  values.DeleteContents();
  values.DeleteAll();

  return true;
}



//-------------------------------------------------------------------
//  XFILECONFIG::IniFile
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/05/2004 20:56:28
//
//  @return       bool :
//  @param        xpath :
*/
//-------------------------------------------------------------------
bool XFILECONFIG::IniFile(XPATH& xpath)
{
  if(!fileini)           return false;
  if(xpath.IsEmpty())    return false;

  bool status = false;

  if(!fileini->Open(xpath, false)) fileini->Create(xpath);

  if(fileini->IsOpen())
    {
      if(fileini->GetFormatCharFromFile()==XFILETXTFORMATCHAR_UNKNOWN) fileini->SetFormatChar(XFILETXTFORMATCHAR_ASCII);
      if(fileini->GetTypeLF()==XFILETXTTYPELF_UNKNOWN)                 fileini->SetTypeLF(XFILETXTTYPELF_DEFAULT);

      status = true;
    }

  return status;
}




//-------------------------------------------------------------------
//  XFILECONFIG::EndFile()
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/05/2004 20:56:28
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool XFILECONFIG::EndFile()
{
  if(!fileini) return false;

  return fileini->Close();
}


