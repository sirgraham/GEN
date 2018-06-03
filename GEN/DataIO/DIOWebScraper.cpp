/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPER.CPP
//
//  Web Scraper Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/11/2007 16:26:37
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XTimer.h"
#include "XBuffer.h"
#include "XPath.h"
#include "XFile.h"
#include "XFileXML.h"
#include "XFileJSON.h"
#include "XDebugTrace.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOStream.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"
#include "DIOWebClient.h"
#include "DIOCheckInternetConnexion.h"

#include "DIOWebScraper.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::DIOWEBSCRAPER
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/02/2014 16:17:12
//
//  @return
//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
DIOWEBSCRAPER::DIOWEBSCRAPER()
{
  Clean();

  xml = new XFILEXML();
  if(!xml)  return;

  xmutexdo = xfactory->Create_Mutex();
  if(!xmutexdo) return;

  if(diofactory) webclient = new DIOWEBCLIENT;

  cache = new DIOWEBSCRAPERCACHE();
  if(!cache) return;

  URLdownload = diofactory->CreateURL();
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::~DIOWEBSCRAPER
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/11/2007 16:28:53
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWEBSCRAPER::~DIOWEBSCRAPER()
{
  DeleteAllValues();

  if(URLdownload)
    {
      diofactory->DeleteURL(URLdownload);
    }

  if(cache)
    {
      cache->DeleteAll();
      delete cache;
    }

  if(xml)
    {
      if(xml->IsOpen()) xml->Close();
      delete xml;
    }

  if(webclient) delete webclient;

  if(xmutexdo) xfactory->Delete_Mutex(xmutexdo);

  Clean();
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::Load
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/03/2013 23:00:52
//
//  @return       bool :
//  @param        namefile :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::Load(XCHAR* namefile)
{
  static bool webupdate = false;

  if(!xml)  return false;

  XPATH xpath;

  xpath.Empty();

  XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_WEBSCRAPER, xpath);
  if(!xpath.IsEmpty()) xpath.Slash_Add();
  xpath += DIOWEBSCRAPER_NAMEFILE;

  if((!webupdate) && (URLdownload))
    {
      webupdate = Download((*URLdownload), xpath);
    }

  if(!xml->Open(xpath, true))
    {
      XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_WEBSCRAPER, xpath);
      if(!xpath.IsEmpty()) xpath.Slash_Add();
      xpath += namefile;

      if(!xml->Open(xpath, true)) return false;
    }

  xml->ReadAndDecodeAllLines();

  xml->Close();

  return true;
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::Load
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/03/2013 18:46:10
//
//  @return       bool :
//  @param        path :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::Load(XPATH& namefile)
{
  return Load(namefile.Get());
}




/*-------------------------------------------------------------------
//   DIOWEBSCRAPER::Download
*/
/**
//
//
//
//  @author   Abraham J. Velez
//  @version  25/01/2018 13:50:57
//  @return   bool :
//
//  @param    DIOURL& :
//  @param    XPATH& :
//
*//*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::Download(DIOURL& URL, XPATH& xpath)
{
  if(URL.IsEmpty())   return false;

  DIOURL downloadURL;
  bool   status;

  DIOWEBCLIENT* webclient = new DIOWEBCLIENT();
  if(!webclient)  return false;

  downloadURL.Set(URL.Get());
  downloadURL.Slash_Add();
  downloadURL.Add(DIOWEBSCRAPER_NAMEFILE);

  status = webclient->Get(downloadURL, xpath);

  delete webclient;

  return status;
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::Do
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2014 12:49:43
//
//  @return       bool :
//  @param        namewebservice :
//  @param        timeoutforurl :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::Do(XCHAR* namewebservice, int timeoutforurl, XSTRING* localIP)
{
  if(!xml) return false;

  XFILEXMLELEMENT* root = xml->GetRoot();
  if(!root) return false;

  XSTRING namews;

  XFILEXMLELEMENT* nodewebservice = NULL;

  for(int c=0; c<root->GetNElements(); c++)
    {
      nodewebservice = root->GetElement(c);
      if(nodewebservice)
        {
          namews = nodewebservice->GetValueAttribute(__L("name"));
          if(!namews.Compare(namewebservice)) break;

          nodewebservice = NULL;
        }
    }

  if(nodewebservice)
    {
      DeleteAllValues();

      for(int c=0; c<nodewebservice->GetNElements(); c++)
        {
          bool priorityvaluesobtain = true;

          XFILEXMLELEMENT* nodeweb = nodewebservice->GetElement(c);
          if(nodeweb)
            {
              XBUFFER  webpage;
              DIOURL   url;

              if(!ChangeURL(nodeweb->GetValueAttribute(__L("url")), url))
                {
                  return false;
                }

              if(webclient->Get(url, webpage, DIOWEBSCRAPER_DEFAULTUSERAGENT, timeoutforurl, localIP))
                {
                  if(webpage.GetSize())
                    {
                      XSTRING format;

                      format = nodeweb->GetValueAttribute(__L("format"));

                      if(!format.Compare(__L("JSON"),true))
                        {
                          XFILEJSON* json = new XFILEJSON();
                          if(json)
                            {
                              json->DeleteAllLines();
                              json->AddBufferLines(XFILETXTFORMATCHAR_ASCII, webpage);
                              json->DecodeAllLines();

                              for(int d=0; d<nodeweb->GetNElements(); d++)
                                {
                                  XFILEXMLELEMENT* nodevalue = nodeweb->GetElement(d);
                                  if(nodevalue)
                                    {
                                      XSTRING  name;
                                      XSTRING  prioritystring;
                                      int      priority;
                                      XSTRING  namevalue;

                                      name           = nodevalue->GetValueAttribute(__L("name"));

                                      prioritystring = nodevalue->GetValueAttribute(__L("priority"));
                                      priority       = prioritystring.ConvertToInt();

                                      namevalue      = nodevalue->GetValueAttribute(__L("namevalue"));

                                      XFILEJSONVALUE* jsonvalue = json->GetValue(namevalue,NULL);
                                      if(jsonvalue)
                                        {
                                          switch(jsonvalue->GetType())
                                            {
                                              case XFILEJSONVALUETYPE_UNKNOWN          : break;

                                              case XFILEJSONVALUETYPE_NUMBER          : { int     pvalue = (int)jsonvalue->GetValueInteger();
                                                                                          XSTRING string;

                                                                                          string.Format(__L("%d"),pvalue);

                                                                                          AddValue(name, string);
                                                                                        }
                                                                                        break;

                                              case XFILEJSONVALUETYPE_NUMBERSPECIAL   : { float   pvalue = (float)jsonvalue->GetValueFloating();
                                                                                          XSTRING string;

                                                                                          string.Format(__L("%f"),pvalue);

                                                                                          AddValue(name, string);
                                                                                        }
                                                                                        break;

                                              case XFILEJSONVALUETYPE_STRING          : { XSTRING* pvalue = (XSTRING*)jsonvalue->GetValuePointer();
                                                                                          if(pvalue) AddValue(name, (*pvalue));
                                                                                        }
                                                                                        break;

                                              case XFILEJSONVALUETYPE_OBJECT          : break;
                                              case XFILEJSONVALUETYPE_ARRAY           : break;
                                              case XFILEJSONVALUETYPE_BOOLEAN         : break;
                                              case XFILEJSONVALUETYPE_NULL            : break;

                                            }
                                        }
                                       else
                                        {
                                          if(priority) priorityvaluesobtain = false;
                                        }
                                    }
                                }

                              delete json;
                            }
                        }

                      if(!format.Compare(__L("HTML"),true))
                        {
                          for(int d=0; d<nodeweb->GetNElements(); d++)
                            {
                              XFILEXMLELEMENT* nodevalue = nodeweb->GetElement(d);
                              if(nodevalue)
                                {
                                  XSTRING  searchini;
                                  XSTRING  searchend;
                                  XSTRING  namevalue;
                                  XSTRING  prioritystring;
                                  int      priority;

                                  XSTRING  value;

                                  namevalue = nodevalue->GetValueAttribute(__L("name"));
                                  namevalue.ConvertHexFormatChars();

                                  prioritystring = nodevalue->GetValueAttribute(__L("priority"));
                                  priority = prioritystring.ConvertToInt();

                                  searchini = nodevalue->GetValueAttribute(__L("start"));
                                  searchini.ConvertHexFormatChars();

                                  searchend = nodevalue->GetValueAttribute(__L("end"));;
                                  searchend.ConvertHexFormatChars();

                                  if(ExtractString(searchini, searchend, &webpage, false, value))
                                    {
                                      if(!GetValue(namevalue))
                                        {
                                          AddValue(namevalue, value);
                                        }
                                    }
                                   else
                                    {
                                      if(priority) priorityvaluesobtain = false;
                                    }
                                }
                            }
                        }

                      if(values.GetSize() && priorityvaluesobtain)
                        {
                          return true;
                        }
                    }
                }
            }
        }
    }

  return false;
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::Do
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/02/2014 17:26:05
//
//  @return       bool :
//  @param        namewebservice :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::Do(XSTRING& namewebservice, int timeoutforurl, XSTRING* localIP)
{
  return Do(namewebservice.Get(), timeoutforurl, localIP);
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::ChangeURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2013 13:38:58
//
//  @return       bool :
//  @param        maskurl :
//  @param        url :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::ChangeURL(XCHAR* maskurl, DIOURL& url)
{
  url = maskurl;

  if(url.GetSize()) return true;

  return false;
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::AddValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2013 12:54:50
//
//  @return       bool :
//  @param        name :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::AddValue(XCHAR* name, XCHAR* value)
{
  XSTRING* _name  = new XSTRING();
  XSTRING* _value = new XSTRING();

  if((!_name )||(!_value))
    {
      delete _name;
      delete _value;

      return false;
    }

  (*_name)  = name;
  (*_value) = value;

  values.Add(_name,_value);

  return true;
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::AddValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2013 12:54:57
//
//  @return       bool :
//  @param        name :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::AddValue(XCHAR* name, XSTRING& value)
{
  return AddValue(name, value.Get());
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::AddValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2013 12:55:01
//
//  @return       bool :
//  @param        name :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::AddValue(XSTRING& name,XCHAR*  value)
{
  return AddValue(name.Get(), value);
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::AddValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2013 12:55:06
//
//  @return       bool :
//  @param        name :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::AddValue(XSTRING& name, XSTRING& value)
{
  return AddValue(name.Get(), value.Get());
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::GetValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2013 12:36:07
//
//  @return       XCHAR* :
//  @param        namevalue :
*/
/*-----------------------------------------------------------------*/
XCHAR* DIOWEBSCRAPER::GetValue(XCHAR* namevalue)
{
  for(XDWORD c=0;c<values.GetSize();c++)
    {
      XSTRING* name = (XSTRING*)values.GetKey(c);
      if(name)
        {
          if(!name->Compare(namevalue))
            {
              XSTRING* value  = values.GetElement(c);

              if(value)
                {
                  value->ConvertHexFormatChars();
                  return value->Get();
                }

              return NULL;
            }
        }
    }

  return NULL;
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::GetValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2013 12:35:58
//
//  @return       XCHAR* :
//  @param        namevalue :
*/
/*-----------------------------------------------------------------*/
XCHAR* DIOWEBSCRAPER::GetValue(XSTRING& namevalue)
{
  return DIOWEBSCRAPER::GetValue(namevalue.Get());
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::DeleteAllValues
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2013 12:35:54
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::DeleteAllValues()
{
  if(values.IsEmpty())  return false;

  values.DeleteKeyContents();
  values.DeleteElementContents();
  values.DeleteAll();

  return true;
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::GetWebClient
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/03/2013 16:00:52

//
//  @return       DIOWEBCLIENT* :
//  */
/*-----------------------------------------------------------------*/
DIOWEBCLIENT* DIOWEBSCRAPER::GetWebClient()
{
  return webclient;
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::ExtractString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2012 18:05:01
//
//  @return       bool :
//  @param        searchini :
//  @param        searchend :
//  @param        xbuffer :
//  @param        usebufferpos :
//  @param        result :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::ExtractString(XCHAR* searchini,XCHAR* searchend,XBUFFER* xbuffer,bool usebufferpos,XSTRING& result)
{
  if((!searchini) || (!searchend)) return false;

  XSTRING _searchini;
  XSTRING _searchend;

  _searchini = searchini;
  _searchend = searchend;

  return ExtractString(_searchini,_searchend,xbuffer,usebufferpos,result);
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::ExtractString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2012 18:05:45
//
//  @return       bool :
//  @param        searchini :
//  @param        searchend :
//  @param        xbuffer :
//  @param        usebufferpos :
//  @param        result :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::ExtractString(XCHAR* searchini,XSTRING& searchend,XBUFFER* xbuffer,bool usebufferpos,XSTRING& result)
{
  if(!searchini) return false;

  XSTRING _searchini;

  _searchini = searchini;

  return ExtractString(_searchini,searchend,xbuffer,usebufferpos,result);
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::ExtractString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2012 18:06:24
//
//  @return       bool :
//  @param        searchini :
//  @param        searchend :
//  @param        xbuffer :
//  @param        usebufferpos :
//  @param        result :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::ExtractString(XSTRING& searchini,XCHAR* searchend,XBUFFER* xbuffer,bool usebufferpos,XSTRING& result)
{
  if(!searchend) return false;

  XSTRING _searchend;

  _searchend = searchend;

  return ExtractString(searchini,_searchend,xbuffer,usebufferpos,result);

}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::ExtractString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/04/2011 22:05:13
//
//  @return       bool :
//  @param        searchini :
//  @param        searchend :
//  @param        xbuffer :
//  @param        usebufferpos :
//  @param        result :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPER::ExtractString(XSTRING& searchini,XSTRING& searchend,XBUFFER* xbuffer,bool usebufferpos,XSTRING& result)
{
  result.Empty();

  if(!xbuffer)            return false;

  if(searchini.IsEmpty()) return false;
  if(searchend.IsEmpty()) return false;

  XSTRING html;
  int        indexini;
  int        indexend;

  //html = (char*)xbuffer->Get();
  html = *xbuffer; // #Imanol, changed to avoid a buffer overrun

  indexini = html.Find(searchini,false,usebufferpos?xbuffer->GetPosition():0);
  if(indexini==XSTRING_NOTFOUND) return false;

  indexend = html.Find(searchend,false,indexini + searchini.GetSize());
  if(indexend==XSTRING_NOTFOUND) return false;

  if((int)(indexini + searchini.GetSize())>=indexend) return false;

  html.Copy(indexini + searchini.GetSize(),indexend,result);

  result.DeleteCharacter(__C(' '),XSTRINGCONTEXT_ATFIRST);
  result.DeleteCharacter(__C(' '),XSTRINGCONTEXT_ATEND);

  if(result.IsEmpty()) return false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPER::HandleEvent
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/03/2013 17:18:47
//
//  @return       void :
//  @param        xevent :
*/
/*-----------------------------------------------------------------*/
void DIOWEBSCRAPER::HandleEvent(XEVENT* xevent)
{
  if(!xevent) return;
}


