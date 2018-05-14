/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPERGEOLOCATIONIP.CPP
//
//  Geolocation IP (Web Service)
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/11/2007 16:26:37
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XThread.h"
#include "DIOWebScraperGeolocationIP.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::DIOGEOLOCATIONIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/10/2008 10:38:16
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOGEOLOCATIONIP::DIOGEOLOCATIONIP() : DIOWEBSCRAPERCACHERESULT()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::~DIOGEOLOCATIONIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/10/2008 10:38:19
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOGEOLOCATIONIP::~DIOGEOLOCATIONIP()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::GetCountry
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/04/2011 15:50:12
//
//  @return       XCHAR* :
//  */
/*-----------------------------------------------------------------*/
XCHAR* DIOGEOLOCATIONIP::GetCountry()
{
  return country.Get();
}



/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::GetState
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/04/2011 15:50:47
//
//  @return       XCHAR* :
//  */
/*-----------------------------------------------------------------*/
XCHAR* DIOGEOLOCATIONIP::GetState()
{
  return state.Get();
}



/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::GetCity
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/04/2011 15:52:04
//
//  @return       XCHAR* :
//  */
/*-----------------------------------------------------------------*/
XCHAR* DIOGEOLOCATIONIP::GetCity()
{
  return city.Get();
}



/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::GetISP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/04/2011 15:52:33
//
//  @return       XCHAR* :
//  */
/*-----------------------------------------------------------------*/
XCHAR* DIOGEOLOCATIONIP::GetISP()
{
  return ISP.Get();
}



/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::GetOrganization
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/04/2013 13:47:04
//
//  @return       XCHAR* :
//  */
/*-----------------------------------------------------------------*/
XCHAR* DIOGEOLOCATIONIP::GetOrganization()
{
  return organization.Get();
}



/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::GetLatitude
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/04/2011 15:53:17
//
//  @return       float :
//  */
/*-----------------------------------------------------------------*/
float DIOGEOLOCATIONIP::GetLatitude()
{
  return latitude;
}



/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::GetLongitude
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/04/2011 15:53:33
//
//  @return       float :
//  */
/*-----------------------------------------------------------------*/
float DIOGEOLOCATIONIP::GetLongitude()
{
  return longitude;
}



/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::IsEmpty
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/04/2011 22:11:46
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOGEOLOCATIONIP::IsEmpty()
{
  if(country.IsEmpty() || state.IsEmpty() || city.IsEmpty()) return true;

  return false;
}



/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::Copy
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/10/2008 11:40:10
//
//  @return       bool :
//  @param        geolocationIP :
*/
/*-----------------------------------------------------------------*/
bool DIOGEOLOCATIONIP::CopyFrom(DIOWEBSCRAPERCACHERESULT* result)
{
  if(!result)   return false;

  DIOGEOLOCATIONIP* geolocationIP = (DIOGEOLOCATIONIP*)result;

  if(geolocationIP->IsEmpty()) return false;

  country       = geolocationIP->country;
  state         = geolocationIP->state;
  city          = geolocationIP->city;
  ISP           = geolocationIP->ISP;
  organization  = geolocationIP->organization;

  latitude      = geolocationIP->latitude;
  longitude     = geolocationIP->longitude;

  return true;
}





/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::CopyTo
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 17:16:33
//
//  @return       bool :
//  @param        result :
*/
/*-----------------------------------------------------------------*/
bool DIOGEOLOCATIONIP::CopyTo(DIOWEBSCRAPERCACHERESULT* result)
{
  if(!result)   return false;
  if(IsEmpty()) return false;

  DIOGEOLOCATIONIP* geolocationIP = (DIOGEOLOCATIONIP*)result;

  geolocationIP->country       = country;
  geolocationIP->state         = state;
  geolocationIP->city          = city;
  geolocationIP->ISP           = ISP;
  geolocationIP->organization  = organization;

  geolocationIP->latitude      = latitude;
  geolocationIP->longitude     = longitude;

  return true;
}



/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/04/2013 15:36:39
//
//  @return       bool :
//  @param        country :
//  @param        state :
//  @param        city :
//  @param        ISP :
//  @param        organization :
*/
/*-----------------------------------------------------------------*/
bool DIOGEOLOCATIONIP::Set(XSTRING& country, XSTRING& state, XSTRING& city, XSTRING& ISP, XSTRING& organization)
{
  return Set(country.Get(), state.Get(), city.Get(), ISP.Get(), organization.Get());
}




/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/04/2013 15:36:48
//
//  @return       bool :
//  @param        country :
//  @param        state :
//  @param        city :
//  @param        ISP :
//  @param        organization :
*/
/*-----------------------------------------------------------------*/
bool DIOGEOLOCATIONIP::Set(XCHAR* country, XCHAR* state, XCHAR* city, XCHAR* ISP, XCHAR* organization)
{
  if(country)       this->country       = country;
  if(state)         this->state         = state;
  if(city)          this->city          = city;
  if(ISP)           this->ISP           = ISP;
  if(organization)  this->organization  = organization;

  XCHAR character[3] = { 0x09, 0x0A, 0x0D };

  for(int c=0;c<3;c++)
    {
      this->country.DeleteCharacter(character[c],XSTRINGCONTEXT_ALLSTRING);
      this->state.DeleteCharacter(character[c],XSTRINGCONTEXT_ALLSTRING);
      this->city.DeleteCharacter(character[c],XSTRINGCONTEXT_ALLSTRING);
      this->ISP.DeleteCharacter(character[c],XSTRINGCONTEXT_ALLSTRING);
      this->organization.DeleteCharacter(character[c],XSTRINGCONTEXT_ALLSTRING);
    }

  return true;
}



/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/04/2011 15:57:54
//
//  @return       bool :
//  @param        latitude :
//  @param        longitude :
*/
/*-----------------------------------------------------------------*/
bool DIOGEOLOCATIONIP::Set(float latitude,float longitude)
{
  this->latitude  = latitude;
  this->longitude = longitude;

  return true;
}




/*-------------------------------------------------------------------
//  DIOGEOLOCATIONIP::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/03/2013 0:19:56
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOGEOLOCATIONIP::Clean()
{
  latitude  = 0;
  longitude = 0;
}






/*-------------------------------------------------------------------
//  DIOWEBSCRAPERGEOLOCATIONIP::DIOWEBSCRAPERGEOLOCATIONIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/02/2014 17:24:03
//
//  @return       void :



//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
DIOWEBSCRAPERGEOLOCATIONIP::DIOWEBSCRAPERGEOLOCATIONIP(DIOWEBCLIENT* webclient) : DIOWEBSCRAPER()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERGEOLOCATIONIP::~DIOWEBSCRAPERGEOLOCATIONIP
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
DIOWEBSCRAPERGEOLOCATIONIP::~DIOWEBSCRAPERGEOLOCATIONIP()
{
  //DeleteAllCache();

  Clean();
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPERGEOLOCATIONIP::ChangeURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/03/2013 18:36:20
//
//  @return       bool :
//  @param        maskurl :
//  @param        url :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERGEOLOCATIONIP::ChangeURL(XCHAR* maskurl, DIOURL& url)
{
  XSTRING IPstring;

  IP.GetXString(IPstring);

  url.Format(maskurl, IPstring.Get());

  return true;
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERGEOLOCATIONIP::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2014 12:50:59
//
//  @return       bool :
//  @param        IP :
//  @param        geolocationIP :
//  @param        timeoutforurl :
//  @param        localIP :
//  @param        usecache :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERGEOLOCATIONIP::Get(XCHAR* IP, DIOGEOLOCATIONIP& geolocationIP, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  DIOIP _IP;

  _IP.Set(IP);

  return Get(_IP, geolocationIP, timeoutforurl, localIP, usecache);
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERGEOLOCATIONIP::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2014 12:51:21
//
//  @return       bool :
//  @param        IP :
//  @param        geolocationIP :
//  @param        timeoutforurl :
//  @param        localIP :
//  @param        usecache :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERGEOLOCATIONIP::Get(XSTRING& IP, DIOGEOLOCATIONIP& geolocationIP, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  return Get(IP.Get(), geolocationIP, timeoutforurl, localIP, usecache);
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERGEOLOCATIONIP::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/02/2014 17:34:45
//
//  @return       bool :
//  @param        IP :
//  @param        geolocationIP :
//  @param        timeoutforurl :
//  @param        localIP :
//  @param        usecache :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERGEOLOCATIONIP::Get(DIOIP& IP, DIOGEOLOCATIONIP& geolocationIP, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  bool status = false;

  if(xmutexdo) xmutexdo->Lock();

  this->IP.Set(IP.Get());

  if(usecache)
    {
      XSTRING IPstring;

      IP.GetXString(IPstring);

      DIOGEOLOCATIONIP* geoIP = (DIOGEOLOCATIONIP*)cache->Get(IPstring);
      if(geoIP)
        {
          geolocationIP.CopyFrom(geoIP);

          if(xmutexdo) xmutexdo->UnLock();
          return true;
        }
    }

  if(Load(DIOWEBSCRAPERGEOLOCATIONIP_NAMEFILE))
    {
      if(Do(DIOWEBSCRAPERGEOLOCATIONIP_NAMESERVICE, timeoutforurl, localIP))
       {
          XSTRING           country;
          XSTRING           state;
          XSTRING           city;
          XSTRING           ISP;
          XSTRING           organization;
          double            latitude  = 0;
          double            longitude = 0;
          XSTRING           string;

          country       = GetValue(__L("COUNTRY"));
          state         = GetValue(__L("STATE"));
          city          = GetValue(__L("CITY"));
          ISP           = GetValue(__L("ISP"));
          organization  = GetValue(__L("ORGANIZATION"));

          string        = GetValue(__L("LATITUDE"));
          if(!string.IsEmpty()) string.UnFormat(__L("%f"), &latitude);

          string  = GetValue(__L("LONGITUDE"));
          if(!string.IsEmpty()) string.UnFormat(__L("%f"), &longitude);

          geolocationIP.Set(country, state, city, ISP, organization);
          geolocationIP.Set((float)latitude, (float)longitude);

          if(!geolocationIP.IsEmpty())
            {
              if(usecache)
                {
                  DIOGEOLOCATIONIP* geoIP = new DIOGEOLOCATIONIP();
                  if(geoIP)
                    {
                      XSTRING IPstring;

                      IP.GetXString(IPstring);

                      geoIP->CopyFrom(&geolocationIP);
                      cache->Add(IPstring, geoIP);
                    }
                }

              status = true;
            }
        }
    }

  if(xmutexdo) xmutexdo->UnLock();

  return status;
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPERGEOLOCATIONIP::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/11/2007 16:28:12
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOWEBSCRAPERGEOLOCATIONIP::Clean()
{

}


