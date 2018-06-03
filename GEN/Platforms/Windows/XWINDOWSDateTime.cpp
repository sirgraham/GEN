//------------------------------------------------------------------------------------------
//  XWINDOWSDATETIME.CPP
//
//  WINDOWS date time class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 04/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <math.h>

#include "XString.h"
#include "XDebugTrace.h"
#include "XPath.h"

#include "XWINDOWSDateTime.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------




//-------------------------------------------------------------------
//  XWINDOWSDATETIME::XWINDOWSDATETIME
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2002 9:36:29
//
//  @return       :
//  */
//-------------------------------------------------------------------
XWINDOWSDATETIME::XWINDOWSDATETIME()
{
  year          = 0;
  month         = 0;
  day           = 0;

  hours         = 0;
  minutes       = 0;
  seconds       = 0;
  milliseconds  = 0;

  GetActualDateTime(this);
}


/*-------------------------------------------------------------------
//  XWINDOWSDATETIME::~XWINDOWSDATETIME
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/06/2010 10:07:23
//
//  @return
//  */
/*-----------------------------------------------------------------*/
XWINDOWSDATETIME::~XWINDOWSDATETIME()
{

}


//-------------------------------------------------------------------
//  XWINDOWSDATETIME::Read
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/04/2004 10:10:58
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XWINDOWSDATETIME::Read()
{
  GetActualDateTime(this);

  return true;
}


//-------------------------------------------------------------------
//  XWINDOWSDATETIME::Write
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/06/2007 13:26:36
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XWINDOWSDATETIME::Write()
{
  SetActualDateTime(this);

  return true;
}



/*-------------------------------------------------------------------
//  XWINDOWSDATETIME::GetFileDateTime
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/02/2010 05:56:05 p.m.
//
//  @return       bool :
//  @param        path :
//  @param        tmzip :
//  @param        dt :
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSDATETIME::GetFileDateTime(XPATH& xpath, void* tmzip, XDWORD* dt)
{
  HANDLE           hfind;
  FILETIME         ftlocal;
  WIN32_FIND_DATA  ff32;

  hfind = FindFirstFile(xpath.Get(),&ff32);
  if(hfind != INVALID_HANDLE_VALUE)
    {
      FileTimeToLocalFileTime(&(ff32.ftLastWriteTime),&ftlocal);
      FileTimeToDosDateTime(&ftlocal,((LPWORD)dt)+1,((LPWORD)dt)+0);
      FindClose(hfind);

      return true;
    }

  return false;
}



/*-------------------------------------------------------------------
//  XWINDOWSDATETIME::GetFileDateTime
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/01/2015 0:23:03
//
//  @return       bool :
//
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSDATETIME::GetFileDateTime(XPATH& xpath)
{
  HANDLE           hfind;
  FILETIME         ftlocal;
  WIN32_FIND_DATA  ff32;
  SYSTEMTIME       systemtime;

  hfind = FindFirstFile(xpath.Get(),&ff32);
  if(hfind != INVALID_HANDLE_VALUE)
    {
      FileTimeToLocalFileTime(&(ff32.ftLastWriteTime),&ftlocal);
      FileTimeToSystemTime(&ftlocal, &systemtime);
      FindClose(hfind);

      year          = systemtime.wYear;
      month         = systemtime.wMonth;
      day           = systemtime.wDay;

      hours         = systemtime.wHour;
      minutes       = systemtime.wMinute;
      seconds       = systemtime.wSecond;
      milliseconds  = systemtime.wMilliseconds;

      return true;
    }

  return false;
}





/*-------------------------------------------------------------------
//  XWINDOWSDATETIME::GetDifferenceGMT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/02/2013 19:50:40
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int XWINDOWSDATETIME::GetDifferenceGMT()
{
  TIME_ZONE_INFORMATION timezoneinfo = { 0 };

  ::GetTimeZoneInformation(&timezoneinfo);

  float difference = -(float(timezoneinfo.Bias)/60);

  return (int)difference;
}






/*-------------------------------------------------------------------
//  XWINDOWSDATETIME::IsDayLigthSavingTime
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/02/2013 20:31:47
//
//  @return       bool :
//  @param        bias :
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSDATETIME::IsDayLigthSavingTime(int* bias)
{
  TIME_ZONE_INFORMATION timezoneinfo = { 0 };

  int ret = ::GetTimeZoneInformation(&timezoneinfo);

  switch(ret)
    {
      case TIME_ZONE_ID_UNKNOWN   : break;

      case TIME_ZONE_ID_INVALID   :
      case TIME_ZONE_ID_STANDARD  : if(bias)
                                      {
                                        (*bias)  = timezoneinfo.StandardBias;
                                        (*bias) *= 60;
                                      }
                                    break;

      case TIME_ZONE_ID_DAYLIGHT  : if(bias)
                                      {
                                        (*bias)  = timezoneinfo.DaylightBias;
                                        (*bias) *= 60;
                                      }
                                    return true;
    }

  return false;
}





//-------------------------------------------------------------------
//  XWINDOWSDATETIME::GetActualDateTime
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2002 9:38:40
//
//  @return       void :
//  @param        time :
*/
//-------------------------------------------------------------------
void XWINDOWSDATETIME::GetActualDateTime(XWINDOWSDATETIME* time)
{
  SYSTEMTIME st;

  GetLocalTime(&st);

  time->year            = st.wYear;
  time->month         = st.wMonth;
  time->day           = st.wDay;

  time->hours         = st.wHour;
  time->minutes       = st.wMinute;
  time->seconds       = st.wSecond;
  time->milliseconds    = st.wMilliseconds;

};



//-------------------------------------------------------------------
//  XWINDOWSDATETIME::SetActualDateTime
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/06/2007 13:29:32
//
//  @return       void :
//  @param        time :
*/
//-------------------------------------------------------------------
void XWINDOWSDATETIME::SetActualDateTime(XWINDOWSDATETIME* time)
{
  SYSTEMTIME st;

  st.wYear          = time->year;
  st.wMonth         = time->month;
  st.wDay           = time->day;

  st.wHour          = time->hours;
  st.wMinute        = time->minutes;
  st.wSecond        = time->seconds;
  st.wMilliseconds  = time->milliseconds;

  SetLocalTime(&st);
};

