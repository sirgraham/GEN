//------------------------------------------------------------------------------------------
//  XLINUXDATETIME.CPP
//
//  LINUX date time class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 04/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/rtc.h>

#include "XDebug.h"
#include "XLINUXFile.h"

#include "XLINUXDateTime.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  XLINUXDATETIME::XLINUXDATETIME
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2002 9:36:29
//
//  @return       :
//  */
//-------------------------------------------------------------------
XLINUXDATETIME::XLINUXDATETIME()
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
//  XLINUXDATETIME::~XLINUXDATETIME
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/06/2010 10:05:29
//
//  @return
//  */
/*-----------------------------------------------------------------*/
XLINUXDATETIME::~XLINUXDATETIME()
{


}









//-------------------------------------------------------------------
//  XLINUXDATETIME::Read
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/04/2004 10:10:58
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XLINUXDATETIME::Read()
{
  GetActualDateTime(this);

  return true;
}




//-------------------------------------------------------------------
//  XLINUXDATETIME::Write
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/06/2007 13:31:22
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XLINUXDATETIME::Write()
{
  SetActualDateTime(this);

  return true;
}



/*-------------------------------------------------------------------
//  XLINUXDATETIME::FileTime
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
bool XLINUXDATETIME::FileTime(XPATH& xpath,void* tmzip, XDWORD* dt)
{
  typedef struct tm_zip_s
  {
    XDWORD tm_sec;            /* seconds after the minute - [0,59] */
    XDWORD tm_min;            /* minutes after the hour - [0,59] */
    XDWORD tm_hour;           /* hours since midnight - [0,23] */
    XDWORD tm_mday;           /* day of the month - [1,31] */
    XDWORD tm_mon;            /* months since January - [0,11] */
    XDWORD tm_year;           /* years - [1980..2044] */

  } tm_zip;


  XPATH        xpathfile;
  struct stat  s;
  struct tm*   filedate;
  time_t       tm_t       = 0;
  tm_zip*      tmzip2     = (tm_zip*)tmzip;
  bool         status     = false;

  /*
  if(!path)     return false;
  if(!path[0])  return false;

  char name[_MAXPATH+1];
  int len = strlen(path);

  if(len > _MAXPATH) len = _MAXPATH;

  strncpy(name, path,_MAXPATH-1);

  name[_MAXPATH] = '\0';

  if(name[len - 1] == '/')  name[len - 1] = '\0';

  */


  if(xpath.IsEmpty())     return false;
  xpathfile = xpath;

  xpathfile.Slash_Delete();

  XSTRING_CREATEOEM(xpathfile, charvar)

  if(stat(charvar, &s)==0)
    {
      tm_t = s.st_mtime;
      status = true;
    }

  XSTRING_DELETEOEM(charvar)

  filedate = localtime(&tm_t);

  tmzip2->tm_sec  = filedate->tm_sec;
  tmzip2->tm_min  = filedate->tm_min;
  tmzip2->tm_hour = filedate->tm_hour;
  tmzip2->tm_mday = filedate->tm_mday;
  tmzip2->tm_mon  = filedate->tm_mon ;
  tmzip2->tm_year = filedate->tm_year;

  return status;
}




/*-------------------------------------------------------------------
//  XLINUXDATETIME::FileTime
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/01/2015 0:02:54
//
//  @return       bool :
//
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool XLINUXDATETIME::FileTime(XPATH& xpath)
{
  XPATH        xpathfile;
  struct stat  s;
  struct tm*   filedate;
  time_t       tm_t       = 0;
  bool         status     = false;


  /*
  char name[_MAXPATH+1];
  int len = strlen(path);

  if(len > _MAXPATH) len = _MAXPATH;

  strncpy(name, path,_MAXPATH-1);

  name[_MAXPATH] = '\0';


  if(name[len - 1] == '/')  name[len - 1] = '\0';
  */

  if(xpath.IsEmpty())     return false;
  xpathfile = xpath;

  xpathfile.Slash_Delete();

  XSTRING_CREATEOEM(xpathfile, charvar)
  if(stat(charvar,&s)==0)
    {
      tm_t = s.st_mtime;
      status = true;
    }
  XSTRING_DELETEOEM(charvar)

  filedate = localtime(&tm_t);

  year          = filedate->tm_year;
  month         = filedate->tm_mon;
  day           = filedate->tm_mday;
  hours         = filedate->tm_hour;
  minutes       = filedate->tm_min;
  seconds       = filedate->tm_sec;
  milliseconds  = 0;

  return status;
}



/*-------------------------------------------------------------------
//  XLINUXDATETIME::GetDifferenceGMT
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
int XLINUXDATETIME::GetDifferenceGMT()
{
  time_t    t  = time(NULL);
  struct tm lt =  { 0 };

  localtime_r(&t, &lt);

  return (lt.tm_gmtoff/3600) + (lt.tm_isdst?-1:0);

  /*
  time_t    t;
  struct tm tm_local;
  struct tm tm_gmt;
  struct tm tm_aux;
  long      diff;

  tzset();

  time(&t);

  memcpy(&tm_gmt, gmtime(&t), sizeof(struct tm));

  localtime_r(&t,&tm_aux);
  memcpy(&tm_local,&tm_aux, sizeof(struct tm));

  tm_local.tm_isdst = 0;

  diff = -(mktime(&tm_gmt) - mktime(&tm_local));

  return (int)(diff/3600);
  */
}




/*-------------------------------------------------------------------
//  XLINUXDATETIME::IsDayLigthSavingTime
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
bool XLINUXDATETIME::IsDayLigthSavingTime(int* bias)
{
  struct tm*  currentdate;
  long        currentdatesecs;

  time(&currentdatesecs);
  currentdate = localtime(&currentdatesecs);

  if(bias) (*bias) = 0;

  bool dls = currentdate->tm_isdst?true:false;

  if(dls)
    {
      if(bias) (*bias) = -3600;
    }

  return currentdate->tm_isdst?true:false;
}




//-------------------------------------------------------------------
//  XLINUXDATETIME::GetActualDateTime
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
void XLINUXDATETIME::GetActualDateTime(XLINUXDATETIME* timed)
{
  time_t      now;
  struct tm*  datatime;

  now = time(0);

  datatime = localtime(&now);
  if(!datatime) return;

  timed->year           = datatime->tm_year + 1900;
  timed->month          = datatime->tm_mon+1;
  timed->day            = datatime->tm_mday;
  timed->hours          = datatime->tm_hour;
  timed->minutes        = datatime->tm_min;
  timed->seconds        = datatime->tm_sec;

  struct timeval tv;
  gettimeofday(&tv, NULL);

  timed->milliseconds   = (tv.tv_usec /1000);
};



//-------------------------------------------------------------------
//  XLINUXDATETIME::SetActualDateTime
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/06/2007 13:31:40
//
//  @return       void :
//  @param        timed :
*/
//-------------------------------------------------------------------
void XLINUXDATETIME::SetActualDateTime(XLINUXDATETIME* timed)
{
  struct tm      mytime;
  struct timeval systime;

  mytime.tm_sec   = timed->seconds;       // Seconds
  mytime.tm_min   = timed->minutes;       // Minutes
  mytime.tm_hour  = timed->hours;         // Hours
  mytime.tm_mday  = timed->day;           // Day of Month
  mytime.tm_mon   = timed->month - 1;     // Month
  mytime.tm_year  = timed->year  - 1900;  // Year
  mytime.tm_isdst = IsDayLigthSavingTime();

  systime.tv_sec  = mktime(&mytime);
  systime.tv_usec = 0;

  settimeofday(&systime, NULL);
}




