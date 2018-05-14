/*------------------------------------------------------------------------------------------
//  DIONTP.CPP
//
//  Data IO Network Time Protocol Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 09/02/2013 15:28:15
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XDateTime.h"
#include "XTimer.h"

#include "DIOFactory.h"
#include "DIOStreamUDPConfig.h"
#include "DIOStreamUDP.h"

#include "DIONTP.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/






/*-------------------------------------------------------------------
//  DIONTP::DIONTP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2013 18:34:32
//
//  @return


//  @param        publisher :
*/
/*-----------------------------------------------------------------*/
DIONTP::DIONTP()
{
  Clean();

  xtimer = xfactory->CreateTimer();

  if(diofactory)
    {
      diostreamudpcfg = new DIOSTREAMUDPCONFIG();
      diostreamudp    = (DIOSTREAMUDP*)diofactory->CreateStreamIO(diostreamudpcfg);
    }
}



/*-------------------------------------------------------------------
//  DIONTP::~DIONTP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/09/2012 11:21:17
//
//  @return       virtual :
//  */
/*-----------------------------------------------------------------*/
DIONTP::~DIONTP()
{

  xfactory->DeleteTimer(xtimer);
  xtimer = NULL;

  if(diofactory)
    {
      if(diostreamudpcfg)
        {
          delete diostreamudpcfg;
          diostreamudpcfg = NULL;
        }

      if(diostreamudp)
        {
          diofactory->DeleteStreamIO(diostreamudp);
          diostreamudp    = NULL;
        }
    }

  Clean();
}




/*-------------------------------------------------------------------
//  DIONTP::GetTimeReponse
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/02/2013 19:39:47
//
//  @return       bool :
//  @param        urlntpserver :
//  @param        timeout :
//  @param        hardwareuselittleendian :
//  @param        response :
*/
/*-----------------------------------------------------------------*/
bool DIONTP::GetTimeResponse(XCHAR* urlntpserver, int timeout, bool hardwareuselittleendian, DIONTPRESPONSE& response)
{
  if(!xtimer)                               return false;
  if((!diostreamudpcfg) || (!diostreamudp)) return false;

  bool status  = false;

  memset(&response, 0, sizeof(DIONTPRESPONSE));

  diostreamudpcfg->SetMode(DIOSTREAMMODE_CLIENT);
  diostreamudpcfg->SetIsUsedDatagrams(false);
  diostreamudpcfg->GetRemoteURL()->Set(urlntpserver);
  diostreamudpcfg->SetRemotePort(DIONTP_DEFAULTPORT);

  if(!diostreamudp->Open()) return false;

  DIONTPBASICINFO basicinfo;

  memset((XBYTE*)&basicinfo, 0, sizeof(DIONTPBASICINFO));

  basicinfo.livnmode = 27; //Encoded representation which represents NTP Client Request & NTP version 3.0

  if(diostreamudp->Write((XBYTE*)&basicinfo,sizeof(DIONTPBASICINFO)))
    {
      xtimer->Reset();

      DIONTPFULLPACKET fullpacket;

      XDWORD br = 0;
      while(!br)
        {
          memset((XBYTE*)&fullpacket,0 ,sizeof(DIONTPFULLPACKET));

          br = diostreamudp->Read((XBYTE*)&fullpacket, sizeof(DIONTPBASICINFO));
          if(br>=sizeof(DIONTPBASICINFO))
            {
              response.stratum           = fullpacket.basic.stratum;
              response.leapindicator     = (fullpacket.basic.livnmode & 0xC0) >> 6;
              response.originatetime     = ConvertTimePacket(fullpacket.basic.originatetimestamp  , hardwareuselittleendian);
              response.receivetime       = ConvertTimePacket(fullpacket.basic.receivetimestamp    , hardwareuselittleendian);
              response.transmittime      = ConvertTimePacket(fullpacket.basic.transmittimestamp , hardwareuselittleendian);

              long long roundtripdelay   = (long long)(response.destinationtime - response.originatetime) - (long long)(response.receivetime - response.transmittime);
              long long localclockoffset = (long long)(response.receivetime - response.originatetime)     + (long long)(response.transmittime - response.destinationtime);
              response.roundtripdelay    = (double)(roundtripdelay);
              response.localclockoffset  = (double)(localclockoffset/2);

              status  = true;
              break;
            }

          if((int)xtimer->GetMeasureSeconds()>= timeout)
            {
              status = false;
              break;
            }
        }
    }

  diostreamudp->Close();

  return status;
}




/*-------------------------------------------------------------------
//  DIONTP::GetTimeReponse
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/02/2013 19:39:58
//
//  @return       bool :
//  @param        urlntpserver :
//  @param        timeout :
//  @param        hardwareuselittleendian :
//  @param        response :
*/
/*-----------------------------------------------------------------*/
bool DIONTP::GetTimeResponse(DIOURL& urlntpserver, int timeout, bool hardwareuselittleendian, DIONTPRESPONSE& response)
{
  return GetTimeResponse(urlntpserver.Get(), timeout, hardwareuselittleendian, response);
}




/*-------------------------------------------------------------------
//  DIONTP::GetTimeSeconds
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2013 19:27:25
//
//  @return       bool :
//  @param        urlntpserver :
//  @param        timeout :
//  @param        hardwareuselittleendian :
//  @param        timeseconds :
*/
/*-----------------------------------------------------------------*/
bool DIONTP::GetTimeSeconds(XCHAR* urlntpserver, int timeout, bool hardwareuselittleendian, XQWORD& timeseconds)
{
  DIONTPRESPONSE response;

  timeseconds = 0;

  if(GetTimeResponse(urlntpserver,timeout, hardwareuselittleendian, response))
    {
      timeseconds = (response.receivetime >> 32);

    } else return false;

  return timeseconds?true:false;
}





/*-------------------------------------------------------------------
//  DIONTP::GetTimeSeconds
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2013 19:27:39
//
//  @return       bool :
//  @param        urlntpserver :
//  @param        timeout :
//  @param        hardwareuselittleendian :
//  @param        timeseconds :
*/
/*-----------------------------------------------------------------*/
bool DIONTP::GetTimeSeconds(DIOURL& urlntpserver, int timeout, bool hardwareuselittleendian, XQWORD& timeseconds)
{
  return GetTimeSeconds(urlntpserver.Get(), timeout, hardwareuselittleendian, timeseconds);
}



/*-------------------------------------------------------------------
//  DIONTP::GetTime
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/09/2014 9:50:00
//
//  @return       bool :
//
//  @param        urlntpserver :
//  @param        timeout :
//  @param        hardwareuselittleendian :
//  @param        xdatetime :
*/
/*-----------------------------------------------------------------*/
bool DIONTP::GetTime(XCHAR* urlntpserver, int timeout, bool hardwareuselittleendian, XDATETIME& xdatetime)
{
  XQWORD timeseconds;

  bool status = GetTimeSeconds(urlntpserver, timeout, hardwareuselittleendian, timeseconds);
  if(status)
    {
      XDWORD sec = (XDWORD)timeseconds;

      if(xdatetime.IsDayLigthSavingTime()) sec += 3600;

      sec += (xdatetime.GetDifferenceGMT()*3600);

      xdatetime.SetSeconds((XWORD)(sec % 60));   sec /= 60;
      xdatetime.SetMinutes((XWORD)(sec % 60));   sec /= 60;
      xdatetime.SetHours((XWORD)(sec % 24));     sec /= 24;

      long JD = sec + DIONTP_JAN1ST1900;

      xdatetime.GetDateFromNDays(JD,false);
    }

  return status;
}




/*-------------------------------------------------------------------
//  DIONTP::GetTime
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/02/2013 20:12:25
//
//  @return       bool :
//  @param        urlntpserver :
//  @param        timeout :
//  @param        hardwareuselittleendian :
//  @param        xtime :
*/
/*-----------------------------------------------------------------*/
bool DIONTP::GetTime(DIOURL& urlntpserver, int timeout, bool hardwareuselittleendian, XDATETIME& xtime)
{
  return GetTime(urlntpserver.Get(), timeout, hardwareuselittleendian, xtime);
}



/*-------------------------------------------------------------------
//  DIONTP::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/09/2012 11:22:15
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIONTP::Clean()
{

  diostreamudpcfg = NULL;
  diostreamudp    = NULL;
}




/*-------------------------------------------------------------------
//  DIONTP::ConvertTimePacket
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/02/2013 19:50:45
//
//  @return       XQWORD :
//  @param        timepacket :
//  @param        hardwareuselittleendian :
*/
/*-----------------------------------------------------------------*/
XQWORD DIONTP::ConvertTimePacket(DIOTIMEPACKET& timepacket, bool hardwareuselittleendian)
{
  XDWORD  dwhigh   =  (hardwareuselittleendian)?SwapDWORD(timepacket.integer)    : timepacket.integer;
  XDWORD  dwlow    =  (hardwareuselittleendian)?SwapDWORD(timepacket.fractional) : timepacket.fractional;
  XQWORD  timedata;

  timedata  = ((XQWORD) dwhigh) << 32;
  timedata |= dwlow;

  return timedata;
}