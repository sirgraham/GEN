/*------------------------------------------------------------------------------------------
//  DIOTEMHUMSENSORAM2301.CPP
//
//  AOSONG AM2301 Sensor class (Temperature and humidity module)
//
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 25/11/2013 23:03:27
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>


#include "XFactory.h"
#include "XSleep.h"
#include "XTimer.h"
#include "XDebugTrace.h"
#include "XThreadCollected.h"

#include "DIOGPIO.h"

#include "DIOTemHumSensorAM2301.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOTEMHUMSENSORAM2301::DIOTEMHUMSENSORAM2301
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/03/2014 19:17:09
//
//  @return

//  @param        diogpio :
//  @param        pindata :
//  @param        activatecache :
*/
/*-----------------------------------------------------------------*/
DIOTEMHUMSENSORAM2301::DIOTEMHUMSENSORAM2301(DIOGPIO* diogpio, int pindata, bool activatecache) : DIODEVICE()
{
  Clean();

  this->diogpio = diogpio;
  this->pindata     = pindata;

  xtimer = xfactory->CreateTimer();

  if(activatecache)
    {
      xmutexread  = xfactory->Create_Mutex();
      if(xmutexread)
        {
          threadcache  = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAM, __L("DIOTEMHUMSENSORAM2301::DIOSENSORAOSONGAM2301"), ThreadRunFunction, (void*)this);
          if(!threadcache) return;

          threadcache->SetPriority(XTHREADPRIORITY_REALTIME);
        }
    }
}




/*-------------------------------------------------------------------
//  DIOTEMHUMSENSORAM2301::~DIOTEMHUMSENSORAM2301
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/11/2013 23:13:13
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOTEMHUMSENSORAM2301::~DIOTEMHUMSENSORAM2301()
{
  End();

  if(threadcache) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAM, threadcache);

  if(xmutexread)  xfactory->Delete_Mutex(xmutexread);
  if(xtimer)      xfactory->DeleteTimer(xtimer);

  Clean();
}



/*-------------------------------------------------------------------
//  DIOTEMHUMSENSORAM2301::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2014 22:25:05
//
//  @return       bool :
//
//  @param        cadenceread :
*/
/*-----------------------------------------------------------------*/
bool DIOTEMHUMSENSORAM2301::Ini(int cadenceread)
{
  if(!threadcache) return false;

  this->cadenceread = cadenceread;

  if(!threadcache->Ini()) return false;

  return DIODEVICE::Ini();
}




/*-------------------------------------------------------------------
//  DIOTEMHUMSENSORAM2301::ReadFromCache
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/11/2013 23:26:38
//
//  @return       bool :
//  @param        temperature :
//  @param        humidity :
//  @param        mode :
*/
/*-----------------------------------------------------------------*/
bool DIOTEMHUMSENSORAM2301::ReadFromCache(float& temperature, float& humidity)
{
  if(!xmutexread)      return false;
  if(!IsInitialized()) return false;

  if(!nreads) return false;

  xmutexread->Lock();

  temperature = temperaturecache;
  humidity    = humiditycache;

  xmutexread->UnLock();

  return true;
}




/*-------------------------------------------------------------------
//  DIOTEMHUMSENSORAM2301::ReadDirect
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/01/2014 16:57:11
//
//  @return       bool :
//  @param        temperature :
//  @param        humidity :
*/
/*-----------------------------------------------------------------*/
bool DIOTEMHUMSENSORAM2301::ReadDirect(float& temperature, float& humidity)
{
  if(!diogpio)      return false;
  if(!xtimer)           return false;
  if(!IsInitialized())  return false;

  int    timeelapsedlow;
  int    timeelapsedhigh;
  XBYTE  value[5] = { 0, 0, 0, 0, 0 };

  // Leave it high for a while
  diogpio->SetMode(pindata,  DIOTEMHUMSENSORAM2301_OUTPUT);
  diogpio->Set(pindata, DIOTEMHUMSENSORAM2301_HIGH);
  XSLEEP::GetInstance().MilliSeconds(1);

  // Set it low to give the start signal
  diogpio->Set(pindata, DIOTEMHUMSENSORAM2301_LOW);
  XSLEEP::GetInstance().MilliSeconds(1);
  diogpio->Set(pindata, DIOTEMHUMSENSORAM2301_HIGH);

  // Now set the pin high to let the sensor start communicating
  diogpio->SetMode(pindata,  DIOTEMHUMSENSORAM2301_INPUT);

  XSLEEP::GetInstance().NanoSeconds(30);

  // Read ACK
  WaitTo(false, 50);

  WaitTo(true , 100);
  WaitTo(false, 100);


  // Read values
  for(int d=0; d<5; d++)
    {
      value[d] = 0x00;

      for(int c=0; c<8; c++)
        {
          WaitTo(false, 50);
          WaitTo(true , 100, &timeelapsedlow);
          WaitTo(false, 100, &timeelapsedhigh);

          value[d]|=(timeelapsedlow>timeelapsedhigh?0:1);

          if(c<7)  value[d] <<= 1;
        }
    }


  // Final State of SDA
  diogpio->SetMode(pindata, DIOTEMHUMSENSORAM2301_OUTPUT);
  diogpio->Set(pindata    , DIOTEMHUMSENSORAM2301_HIGH);

  XSLEEP::GetInstance().MilliSeconds(1);

  /*
 XDEBUGTRACE_PRINTCOLOR(2, __L(" %X %X %X %X "), value[0], value[1], value[2],  value[3]);
  */

  // Verify checksum
  XBYTE x = value[0] + value[1] + value[2] + value[3];
  if(x != value[4]) return false;

  humidity     = (float)(((XWORD)value[0] << 8) | (XWORD)value[1]);
  humidity    /= 10.0f;

  temperature  = (float)(((XWORD)value[2] << 8) | (XWORD)value[3]);
  temperature /= 10.0f;

  if((humidity > 100.0) || (humidity <= 0.0) || (temperature > 80.0)  || (temperature < -40.0)) return false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOTEMHUMSENSORAM2301::End
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      24/09/2014 19:41:19
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOTEMHUMSENSORAM2301::End()
{
  if(!DIODEVICE::End()) return false;

  if(threadcache) threadcache->End();

  return true;
}




/*-------------------------------------------------------------------
//  DIOTEMHUMSENSORAM2301::WaitTo
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/01/2014 19:10:40
//
//  @return       bool :
//  @param        tohigh :
//  @param        timeout :
//  @param        timeelapsed :
*/
/*-----------------------------------------------------------------*/
bool DIOTEMHUMSENSORAM2301::WaitTo(bool tohigh, int timeout, int* timeelapsed)
{
  if(!xtimer) return false;

  bool state;
  bool status = true;

  int _timeelapsed = 0;

  do{ state = diogpio->Get(pindata);

      _timeelapsed++;

      if(tohigh)
        {
          if(state) break;
        }
       else
        {
          if(!state) break;
        }

      if(_timeelapsed > timeout)
        {
          status = false;
          break;
        }

    } while(1);

  if(timeelapsed) (*timeelapsed) = _timeelapsed;

  return status;
}



/*-------------------------------------------------------------------
//  DIOTEMHUMSENSORAM2301::ThreadRunFunction
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      29/09/2014 22:41:16
//
//  @param        param :
*/
/*-----------------------------------------------------------------*/
void DIOTEMHUMSENSORAM2301::ThreadRunFunction(void* param)
{
  DIOTEMHUMSENSORAM2301* sensor = (DIOTEMHUMSENSORAM2301*)param;
  if(!sensor) return;

  if(!sensor->IsInitialized()) return;

  float temperature;
  float humidity;

  bool  status    = sensor->ReadDirect(temperature, humidity);
  if(status)
    {
      if(sensor->xmutexread) sensor->xmutexread->Lock();

      if(sensor->nreads>1)
        {
          if((abs((int)sensor->temperaturecache - (int)temperature)>10) || (abs((int)sensor->humiditycache - (int)humidity)>10)) status = false;
        }

      if(status)
        {
          sensor->temperaturecache  = temperature;
          sensor->humiditycache     = humidity;
          sensor->nreads++;
        }

      if(sensor->xmutexread) sensor->xmutexread->UnLock();
    }

  if(sensor->xtimer) XSLEEP::GetInstance().MilliSeconds(500);

}





