/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPERWEATHER.CPP
//
//  User Agent ID (Web Service)
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/11/2007 16:26:37
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XThread.h"
#include "XDebug.h"
#include "DIOWebScraperWeather.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//  DIOWEBSCRAPERWEATHER::DIOWEBSCRAPERWEATHER
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/02/2014 16:20:40
//
//  @return       void :



//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
DIOWEBSCRAPERWEATHER::DIOWEBSCRAPERWEATHER(  ) : DIOWEBSCRAPER()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERWEATHER::~DIOWEBSCRAPERWEATHER
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
DIOWEBSCRAPERWEATHER::~DIOWEBSCRAPERWEATHER()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERWEATHER::ChangeURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/04/2013 22:29:13
//
//  @return       bool :
//  @param        maskurl :
//  @param        url :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERWEATHER::ChangeURL(XCHAR* maskurl, DIOURL& url)
{
  url.Format(maskurl, location.Get(), iscelsius?__L("units=metric"):__L("units=imperial"));

  return true;
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPERWEATHER::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/05/2014 16:53:14
//
//  @return       bool :
//
//  @param        location :
//  @param        iscelsius :
//  @param        condition :
//  @param        temperature :
//  @param        humidity :
//  @param        timeoutforurl :
//  @param        localIP :
//  @param        usecache :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERWEATHER::Get(XCHAR* location, bool iscelsius, XSTRING& condition, float& temperature, float& humidity, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  bool status = false;
  if(xmutexdo) xmutexdo->Lock();

  this->location  = location;
  this->iscelsius = iscelsius;

  if(Load(DIOWEBSCRAPERWEATHER_NAMEFILE))
    {
      if(Do(DIOWEBSCRAPERWEATHER_NAMESERVICE, timeoutforurl, localIP))
        {
          XSTRING temperaturestring;
          XSTRING humiditystring;

          condition         = GetValue(__L("CONDITION"));
          temperaturestring = GetValue(__L("TEMPERATURE"));
          humiditystring    = GetValue(__L("HUMIDITY"));

          if((!temperaturestring.IsEmpty()) && (!humiditystring.IsEmpty()))
            {
              temperature = temperaturestring.ConvertToFloat();
              humidity    = humiditystring.ConvertToFloat();

              status = true;
            }
        }

    }

  if(xmutexdo) xmutexdo->UnLock();

  return status;
}




  /*-------------------------------------------------------------------
  //  DIOWEBSCRAPERWEATHER::Get
  */
  /**
  //
  //
  //
  //  @author       Abraham J. Velez
  //  @version      12/05/2014 16:57:44
  //
  //  @return       bool :
  //
  //  @param        location :
  //  @param        iscelsius :
  //  @param        condition :
  //  @param        temperature :
  //  @param        humidity :
  //  @param        timeout :
  //  @param        localIP :
  //  @param        usecache :
  */
  /*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERWEATHER::Get(XSTRING& location, bool iscelsius,  XSTRING& condition, float& temperature, float& humidity, int timeout, XSTRING* localIP, bool usecache)
{
  return Get(location.Get(), iscelsius, condition, temperature, humidity, timeout, localIP, usecache);
}