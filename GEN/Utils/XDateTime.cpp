//------------------------------------------------------------------------------------------
//  XDATETIME.CPP
//
//  Time Functions
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 09/04/2007 19:46:31
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "XBase.h"
#include "XFactory.h"

#include "XDateTime.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------



//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  XDATETIME::IsZero
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      02/04/2014 16:21:59
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::IsZero()
{
  if(!year && !month && !day && !hours && !minutes && !seconds) return true;

  return false;
}



/*-------------------------------------------------------------------
//  XDATETIME::IsLeapYear
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/12/2012 18:55:59
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XDATETIME::IsLeapYear()
{
  return (((year%4==0) & (year%100!=0)) | (year%400==0))?true:false;
}



/*-------------------------------------------------------------------
//  XDATETIME::GetDayOfWeek
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/08/2011 0:29:31
//
//  @return       XDATETIME_DAYWEEK :
//  */
/*-----------------------------------------------------------------*/
XDATETIME_DAYWEEK XDATETIME::GetDayOfWeek()
{
  int monthcode[] = { 0, 6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
  int result      = year%100 + (year%100)/4 + day + monthcode[month];

  if(year/100==17) result += 5;
    else if(year/100==18) result += 3;
      else if(year/100==19) result += 1;
        else if(year/100==20) result += 0;
          else if(year/100==21) result += -2;
            else if(year/100==22)  result += -4;

  if(IsLeapYear() & (month==1 || month==2)) result += -1;

  return (XDATETIME_DAYWEEK)(result%7);
}





/*-------------------------------------------------------------------
//  XDATETIME::GetNDaysInJulian
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/12/2012 22:31:26
//
//  @return       long :
//  */
/*-----------------------------------------------------------------*/
long XDATETIME::GetNDaysInJulian()
{
  int _year   = year;
  int _month  = month;
  int _day    = day;

  if(_month<3)
    {
      _year--;
      _month += 12;
    }

  _year  += 4800;
  _month -= 3;

  return (_year*365) + (_year/4) + (30*_month) + (_month*3+2)/5 + _day - 32083;
}



/*-------------------------------------------------------------------
//  XDATETIME::GetNDaysInGregorian
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/12/2012 22:31:42
//
//  @return       long :
//  */
/*-----------------------------------------------------------------*/
long XDATETIME::GetNDaysInGregorian()
{
  int _year   = year;
  int _month  = month;
  int _day    = day;

  if(_month<3)
    {
      _year--;
      _month+=12;
    }

  _year += 8000;

  return (_year*365) + (_year/4) - (_year/100) + (_year/400) + (_month*153+3)/5-92 + _day - 1 -1200820;
}




/*-------------------------------------------------------------------
//  GetDateFromNDays
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/12/2012 19:14:01
//
//  @return       void :
//  @param        ndays :
//  @param        injulian :
*/
/*-----------------------------------------------------------------*/
void XDATETIME::GetDateFromNDays(long ndays, bool injulian)
{
  int _year;
  int _month;
  int _day;

  if(injulian)
    {
      for(_year=ndays/366-4715; GetNDaysInJulian( 1, 1, _year+1)<=ndays             ; _year++);
      for(_month=1;             GetNDaysInJulian(1, _month+1, _year)<=ndays         ; _month++);
      for(_day=1;               GetNDaysInJulian( _day+1, _month, _year)<=ndays     ; _day++);
    }
   else
    {
      for(_year=ndays/366-4715; GetNDaysInGregorian( 1, 1, _year+1)<=ndays          ; _year++);
      for(_month=1;             GetNDaysInGregorian(1, _month+1, _year)<=ndays      ; _month++);
      for(_day=1;               GetNDaysInGregorian( _day+1, _month, _year)<=ndays  ; _day++);
    }

  year  = _year;
  month = _month;
  day   = _day;
}



/*-------------------------------------------------------------------
//  XDATETIME::GetSeconsFromDate
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/12/2012 16:21:56
//
//  @return       XDWORD :
//  */
/*-----------------------------------------------------------------*/
XQWORD XDATETIME::GetSeconsFromDate(bool injulian)
{
  XQWORD  allseconds = injulian?GetNDaysInJulian():GetNDaysInGregorian();

  allseconds *= (24*60*60);
  allseconds += (((hours*60)+minutes)*60)+seconds;

  return allseconds;
}




/*-------------------------------------------------------------------
//  XDATETIME::SetDateFromSeconds
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/12/2012 16:23:41
//
//  @return       bool :
//  @param        allseconds :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::SetDateFromSeconds(XQWORD allseconds, bool injulian)
{
  long   ndays  = (long)(allseconds / (24*60*60));
  XQWORD hinsec = (XDWORD)(allseconds - (ndays*(24*60*60)));

  hours   = (int)(hinsec / (60*60));
  minutes = (int)((hinsec % (60*60)) / 60);
  seconds = (int)((hinsec % (60*60)) % 60);

  GetDateFromNDays(ndays,injulian);

  return true;
}



/*-------------------------------------------------------------------
//  XDATETIME::GetSecondsFromADay
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/12/2013 10:23:32
//
//  @return       XDWORD :
//  */
/*-----------------------------------------------------------------*/
XDWORD XDATETIME::GetSecondsFromADay()
{
  XDWORD _seconds = 0;

  _seconds += (hours*3600);
  _seconds += (minutes*60);
  _seconds += seconds;

  return _seconds;
}




/*-------------------------------------------------------------------
//  XDATETIME::IsValidDate
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/12/2012 11:45:14
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XDATETIME::IsValidDate()
{
  XWORD monthlen[]={31,28,31,30,31,30,31,31,30,31,30,31};

  if((month<=0) || (day<=0)) return false;
  if(month>12)                return false;

  if(IsLeapYear() && month==2) monthlen[1]++;

  if(day>monthlen[month-1]) return false;

  return true;
}




/*-------------------------------------------------------------------
//  XDATETIME::CopyFrom
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/12/2013 9:52:56
//
//  @return       bool :
//  @param        xtimesource :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::CopyFrom(XDATETIME* xtimesource)
{
  if(!xtimesource)                 return false;
//if(!xtimesource->IsValidDate()) return false;

  year         = xtimesource->GetYear();
  month        = xtimesource->GetMonth();
  day          = xtimesource->GetDay();
  hours        = xtimesource->GetHours();
  minutes      = xtimesource->GetMinutes();
  seconds      = xtimesource->GetSeconds();
  milliseconds = xtimesource->GetMilliSeconds();

  return true;
}






/*-------------------------------------------------------------------
//  XDATETIME::CopyTo
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/12/2013 9:53:30
//
//  @return       bool :
//  @param        xtimetarget :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::CopyTo(XDATETIME* xtimetarget)
{
  if(!xtimetarget) return false;

  xtimetarget->SetYear(year);
  xtimetarget->SetMonth(month);
  xtimetarget->SetDay(day);
  xtimetarget->SetHours(hours);
  xtimetarget->SetMinutes(minutes);
  xtimetarget->SetSeconds(seconds);
  xtimetarget->SetMilliSeconds(milliseconds);

  return true;
}






/*-------------------------------------------------------------------
//  XDATETIME::IsTheSameDay
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/12/2013 9:45:43
//
//  @return       bool :
//  @param        xtime :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::IsTheSameDay(XDATETIME* xtime)
{
  if(year  != xtime->GetYear())   return false;
  if(month != xtime->GetMonth())  return false;
  if(day   != xtime->GetDay())    return false;

  return true;
}





/*-------------------------------------------------------------------
//  XDATETIME::AddYears
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/12/2012 22:08:02
//
//  @return       bool :
//  @param        years :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::AddYears(int years)
{
  this->year += years;

  int c=0;
  while(!IsValidDate())
    {
      if(years<0) AddDays(-1); else AddDays(1);
      c++;
      if(c>5) return false;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XDATETIME::AddMonths
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/12/2012 22:18:36
//
//  @return       bool :
//  @param        months :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::AddMonths(int months)
{
  int  years = (months / 12);
  int  rest  = (months % 12);

  if(years) AddYears(years);

  this->month +=  rest;
  if(this->month>12)
    {
      AddYears(1);
      this->month -= 12;
    }

  if(this->month<1)
    {
      AddYears(-1);
      this->month = 13 + rest;
    }

  int c=0;
  while(!IsValidDate())
    {
      if(months<0) AddDays(-1); else AddDays(1);
      c++;
      if(c>5) return false;
    }

  return true;
}


/*-------------------------------------------------------------------
//  XDATETIME::AddDays
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/12/2012 23:22:19
//
//  @return       bool :
//  @param        days :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::AddDays(int days)
{
  long ndays = GetNDaysInGregorian();

  ndays += days;

  GetDateFromNDays(ndays,false);

  return true;
}




/*-------------------------------------------------------------------
//  XDATETIME::AddHours
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/12/2012 17:31:05
//
//  @return       bool :
//  @param        hours :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::AddHours(XQWORD hours)
{
  XQWORD allseconds = GetSeconsFromDate();

  allseconds += hours*3600;

  return SetDateFromSeconds(allseconds);
}



/*-------------------------------------------------------------------
//  XDATETIME::AddMinutes
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/12/2012 17:44:42
//
//  @return       bool :
//  @param        minutes :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::AddMinutes(XQWORD minutes)
{
  XQWORD allseconds = GetSeconsFromDate();

  allseconds += minutes*60;

  return SetDateFromSeconds(allseconds);
}




/*-------------------------------------------------------------------
//  XDATETIME::AddSeconds
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/12/2012 17:45:27
//
//  @return       bool :
//  @param        seconds :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::AddSeconds(XQWORD seconds)
{
  XQWORD allseconds = GetSeconsFromDate();

  allseconds += seconds;

  return SetDateFromSeconds(allseconds);
}



/*-------------------------------------------------------------------
//  XDATETIME::SubtractYears
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/01/2017 12:38:17
//
//  @return       bool :
//
//  @param        years :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::SubtractYears(int years)
{
  this->year -= years;

  int c=0;
  while(!IsValidDate())
    {
      if(years<0) AddDays(-1); else AddDays(1);
      c++;
      if(c>5) return false;
    }

  return true;
}




/*-------------------------------------------------------------------
//  XDATETIME::SubtractMonths
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/01/2017 12:37:12
//
//  @return       bool :
//
//  @param        months :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::SubtractMonths(int months)
{
  int  years = (months / 12);
  int  rest  = (months % 12);

  if(years) SubtractYears(years);

  this->month -=  rest;
  if(this->month>12)
    {
      SubtractYears(1);
      this->month -= 12;
    }

  if(this->month<1)
    {
      SubtractYears(-1);
      this->month = 13 + rest;
    }

  int c=0;
  while(!IsValidDate())
    {
      if(months<0) SubtractDays(-1); else SubtractDays(1);
      c++;
      if(c>5) return false;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XDATETIME::SubtractDays
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/12/2012 23:22:19
//
//  @return       bool :
//  @param        days :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::SubtractDays(int days)
{
  long ndays = GetNDaysInGregorian();

  ndays -= days;

  GetDateFromNDays(ndays,false);

  return true;
}



/*-------------------------------------------------------------------
//  XDATETIME::SubtractHours
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/12/2012 17:31:05
//
//  @return       bool :
//  @param        hours :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::SubtractHours(XQWORD hours)
{
  XQWORD allseconds = GetSeconsFromDate();

  allseconds -= hours*3600;

  return SetDateFromSeconds(allseconds);
}



/*-------------------------------------------------------------------
//  XDATETIME::SubtractMinutes
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/01/2017 12:32:31
//
//  @return       bool :
//
//  @param        minutes :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::SubtractMinutes(XQWORD minutes)
{
  XQWORD allseconds = GetSeconsFromDate();

  allseconds -= minutes*60;

  return SetDateFromSeconds(allseconds);
}



/*-------------------------------------------------------------------
//  XDATETIME::SubtractSeconds
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/01/2017 12:31:45
//
//  @return       bool :
//
//  @param        seconds :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::SubtractSeconds(XQWORD seconds)
{
  XQWORD allseconds = GetSeconsFromDate();

  allseconds -= seconds;

  return SetDateFromSeconds(allseconds);
}



/*-------------------------------------------------------------------
//  ==
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2014 17:38:01
//
//  @return       bool :
//
//  @param        xtime :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::operator == (XDATETIME xtime)
{
  XQWORD s1 =  GetSeconsFromDate();
  XQWORD s2 =  xtime.GetSeconsFromDate();

  if(s1 == s2) return true;

  return false;
}



/*-------------------------------------------------------------------
//   XDATETIME::operator !=
*/
/**
//
//
//
//  @author   Abraham J. Velez
//  @version
//  @return   bool :
//
//  @param    XDATETIME :
//
*//*-----------------------------------------------------------------*/
bool XDATETIME::operator != (XDATETIME xtime)
{
  XQWORD s1 =  GetSeconsFromDate();
  XQWORD s2 =  xtime.GetSeconsFromDate();

  if(s1 != s2) return true;

  return false;
}



/*-------------------------------------------------------------------
//  >
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2014 17:38:23
//
//  @return       bool :
//
//  @param        xtime :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::operator >  (XDATETIME xtime)
{
  XQWORD s1 = GetSeconsFromDate();
  XQWORD s2 = xtime.GetSeconsFromDate();

  if(s1 > s2) return true;

  return false;
}



/*-------------------------------------------------------------------
//  >=
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/09/2014 9:39:17
//
//  @return       bool :
//
//  @param        xtime :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::operator >= (XDATETIME xtime)
{
  XQWORD s1 =  GetSeconsFromDate();
  XQWORD s2 =  xtime.GetSeconsFromDate();

  if(s1 >= s2) return true;

  return false;
}





/*-------------------------------------------------------------------
//  <
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2014 17:38:52
//
//  @return       bool :
//
//  @param        xtime :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::operator <  (XDATETIME xtime)
{
  XQWORD s1 =  GetSeconsFromDate();
  XQWORD s2 =  xtime.GetSeconsFromDate();

  if(s1 < s2) return true;

  return false;
}



/*-------------------------------------------------------------------
//  <=
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      04/09/2014 9:39:41
//
//  @return       bool :
//
//  @param        xtime :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::operator <= (XDATETIME xtime)
{
  XQWORD s1 =  GetSeconsFromDate();
  XQWORD s2 =  xtime.GetSeconsFromDate();

  if(s1 <= s2) return true;

  return false;
}




/*-------------------------------------------------------------------
//  XDATETIME::GetDateTimeToString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/12/2013 14:00:11
//
//  @return       bool :
//  @param        modificator :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::GetDateTimeToString(XWORD modificator, XSTRING& string)
{
  string.Empty();

  XSTRING datestr;
  XSTRING timestr;
  XCHAR   separator = ((modificator & XDATETIME_FORMAT_DATEWITHDASH)?__C('-'):__C('/'));

  if(modificator & XDATETIME_FORMAT_TEXTMONTH)
    {
      XSTRING monthtext;
      XSTRING preseparator;

      preseparator = __L("de");

      GetMonthString(monthtext);

      if(modificator & XDATETIME_FORMAT_FIRSTDATEDAY) datestr.Format(__L("%02d %s %s %s %04d"), day, preseparator.Get(), monthtext.Get(), preseparator.Get(), year);
        else if(modificator & XDATETIME_FORMAT_FIRSTDATEMONTH)    datestr.Format(__L("%s, %s 02d %s 04d"), monthtext.Get(), preseparator.Get(), day, preseparator.Get(), year);
                else if(modificator & XDATETIME_FORMAT_FIRSTDATEYEAR)   datestr.Format(__L("%04d %s %s %s 02d"), year, preseparator.Get(), monthtext.Get(), preseparator.Get(), day);
                        else datestr.Format(__L("%02d %s %s %s %04d"), day, preseparator.Get(), monthtext.Get(), preseparator.Get(), year);
    }
   else
    {
      if(modificator & XDATETIME_FORMAT_FIRSTDATEDAY) datestr.Format(__L("%02d%c%02d%c%04d") , day, separator,  month, separator, year);
        else if(modificator & XDATETIME_FORMAT_FIRSTDATEMONTH)    datestr.Format(__L("%02d%c%02d%c%04d") , month, separator, day, separator, year);
                else if(modificator & XDATETIME_FORMAT_FIRSTDATEYEAR)   datestr.Format(__L("%04d%c%02d%c%02d"), year, separator, month, separator, day);
                        else datestr.Format(__L("%02d%c%02d%c%04d") , day, separator, month, separator, year);
    }

  if(modificator & XDATETIME_FORMAT_TIMEWITHSECONDS)
          timestr.Format(__L("%02d:%02d:%02d") , hours  , minutes , seconds);
    else  timestr.Format(__L("%02d:%02d")      , hours  , minutes);

  if(modificator & XDATETIME_FORMAT_TIMEWITHMILLISECONDS)
    {
      XSTRING timemillisecondsstr;
      timemillisecondsstr.Format(__L(".%3d"), milliseconds);

      timestr += timemillisecondsstr;
    }

  if(modificator & XDATETIME_FORMAT_FIRSTTIME)
    {
      if(modificator & XDATETIME_FORMAT_ADDTIME) string += timestr;

      if(modificator & XDATETIME_FORMAT_ADDDATE)
        {
          if(!string.IsEmpty()) string += __L(" ");
          string += datestr;
        }
    }
   else
    {
      if(modificator & XDATETIME_FORMAT_ADDDATE) string += datestr;

      if(modificator & XDATETIME_FORMAT_ADDTIME)
        {
          if(!string.IsEmpty()) string += __L(" ");
          string += timestr;
        }
    }

  if(modificator & XDATETIME_FORMAT_ADDDAYOFWEEK)
    {
      if(modificator & XDATETIME_FORMAT_FIRSTDAYOFWEEK)
        {
          XSTRING dayofweek;
          XSTRING string2;

          GetDayOfWeekString(dayofweek);

          string2 += dayofweek;

          if(!string.IsEmpty()) string2 += __L(" ");

          string2 += string;

          string = string2;
        }
       else
        {
          XSTRING dayofweek;

          GetDayOfWeekString(dayofweek);

          if(!string.IsEmpty()) string += __L(" ");
          string += dayofweek;
        }
    }

  return true;
}



/*-------------------------------------------------------------------
//  XDATETIME::GetDateTimeFromString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/12/2013 14:00:17
//
//  @return       bool :
//  @param        string :
//  @param        modificator :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::GetDateTimeFromString(XSTRING& string, XWORD modificator)
{
  int           index        = 0;
  XVECTOR<int>  typeblock;
  int           nblock       = 0;
  int           startindex   = 0;
  XCHAR         separator    = ((modificator & XDATETIME_FORMAT_DATEWITHDASH)?__C('-'):__C('/'));
  XSTRING       validcharacters;

  validcharacters  = __L("0123456789 :");
  validcharacters += separator;

  if(!string.AreValidCharacters(validcharacters.Get())) return false;

  if((modificator & XDATETIME_FORMAT_ADDDAYOFWEEK) && (modificator & XDATETIME_FORMAT_FIRSTDAYOFWEEK))    typeblock.Add(1);
  if((modificator & XDATETIME_FORMAT_ADDTIME)      && (modificator & XDATETIME_FORMAT_FIRSTTIME))         typeblock.Add(2);
  if(modificator  & XDATETIME_FORMAT_ADDDATE)                                                             typeblock.Add(3);
  if((modificator & XDATETIME_FORMAT_ADDTIME)      && (!(modificator & XDATETIME_FORMAT_FIRSTTIME)))      typeblock.Add(2);
  if((modificator & XDATETIME_FORMAT_ADDDAYOFWEEK) && (!(modificator & XDATETIME_FORMAT_FIRSTDAYOFWEEK))) typeblock.Add(1);

  while(index<=(int)string.GetSize())
    {
      XCHAR character = string.Get()[index];

      if((character == 0x20) || (index==(int)string.GetSize()))
        {
          XSTRING partial;

          if(index<(int)string.GetSize())
                 string.Copy(startindex, index, partial);
            else string.Copy(startindex, partial);

          if(!partial.IsEmpty())
            {
              switch(typeblock.Get(nblock))
                {
                  case 1: break; // Day of Week not used (can be calculated).

                  case 2: if(modificator & XDATETIME_FORMAT_TIMEWITHMILLISECONDS)
                            {
                              if(modificator & XDATETIME_FORMAT_TIMEWITHSECONDS)
                                      partial.UnFormat(__L("%d:%d:%d.%d"), &hours ,&minutes ,&seconds, &milliseconds);
                                else  partial.UnFormat(__L("%d:%d:%d")   , &hours ,&minutes, &milliseconds);
                            }
                           else
                            {
                              if(modificator & XDATETIME_FORMAT_TIMEWITHSECONDS)
                                      partial.UnFormat(__L("%d:%d:%d"), &hours ,&minutes ,&seconds);
                                else  partial.UnFormat(__L("%d:%d")     , &hours ,&minutes);
                            }
                          break;

                  case 3: if(separator == __C('-'))
                            {
                              if(modificator & XDATETIME_FORMAT_FIRSTDATEDAY) partial.UnFormat(__L("%d-%d-%d"), &day  , &month, &year);
                                  else if(modificator & XDATETIME_FORMAT_FIRSTDATEMONTH)  partial.UnFormat(__L("%d-%d-%d"), &month,  &day, &year);
                                        else if(modificator & XDATETIME_FORMAT_FIRSTDATEYEAR) partial.UnFormat(__L("%d-%d-%d"), &year , &month, &day);
                                            else partial.UnFormat(__L("%d-%d-%d"), &day, &month, &year);
                            }

                          if(separator == __C('/'))
                            {
                              if(modificator & XDATETIME_FORMAT_FIRSTDATEDAY) partial.UnFormat(__L("%d/%d/%d"), &day, &month, &year);
                                  else if(modificator & XDATETIME_FORMAT_FIRSTDATEMONTH)  partial.UnFormat(__L("%d/%d/%d"), &month, &day, &year);
                                        else if(modificator & XDATETIME_FORMAT_FIRSTDATEYEAR) partial.UnFormat(__L("%d/%d/%d"), &year, &month, &day);
                                            else partial.UnFormat(__L("%d/%d/%d"), &day, &month, &year);
                            }

                          break;
                }

              nblock++;
            }

          startindex = index+1;
        }

      index++;
    }

  typeblock.DeleteAll();

  return IsValidDate();

}






/*-------------------------------------------------------------------
//  XDATETIME::GetMonthString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/12/2012 19:20:52
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::GetMonthString(XSTRING& string)
{
  XSTRING strmonth;

  int index = (month-1);

  if(index>=12) return false;
  if(index<0)   return false;

  string.Empty();

  switch(index)
    {
      case  0 : string = __L("Enero");      break;
      case  1 : string = __L("Febrero");    break;
      case  2 : string = __L("Marzo");      break;
      case  3 : string = __L("Abril");      break;
      case  4 : string = __L("Mayo");       break;
      case  5 : string = __L("Junio");      break;
      case  6 : string = __L("Julio");      break;
      case  7 : string = __L("Agosto");     break;
      case  8 : string = __L("Septiembre"); break;
      case  9 : string = __L("Octubre");    break;
      case 10 : string = __L("Noviembre");  break;
      case 11 : string = __L("Diciembre");; break;
    }

  return true;
}



/*-------------------------------------------------------------------
//  XDATETIME::GetDayOfWeekString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/12/2012 19:22:23
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XDATETIME::GetDayOfWeekString(XSTRING& string)
{
  XDATETIME_DAYWEEK dayweek = GetDayOfWeek();

  string.Empty();

  switch(dayweek)
    {
      case XDATETIME_DAYWEEK_SUNDAY    : string = __L("Domingo");   break;
      case XDATETIME_DAYWEEK_MONDAY    : string = __L("Lunes");     break;
      case XDATETIME_DAYWEEK_TUESDAY   : string = __L("Martes");    break;
      case XDATETIME_DAYWEEK_WEDNESDAY : string = __L("Miercoles"); break;
      case XDATETIME_DAYWEEK_THURSDAY  : string = __L("Jueves");    break;
      case XDATETIME_DAYWEEK_FRIDAY    : string = __L("Viernes");   break;
      case XDATETIME_DAYWEEK_SATURDAY  : string = __L("Sabado");    break;
    }

  return true;
}


/*-------------------------------------------------------------------
//  XDATETIME::SetToZero
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2013 20:32:14
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XDATETIME::SetToZero()
{
  Clean();

  return true;
}



/*-------------------------------------------------------------------
//  XDATETIME::SetDateToZero
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/12/2013 10:13:16
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XDATETIME::SetDateToZero()
{
  year          = 0;
  month         = 0;
  day           = 0;

  return true;
}



/*-------------------------------------------------------------------
//  XDATETIME::SetHourToZero
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/12/2013 10:13:20
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XDATETIME::SetHourToZero()
{
  hours         = 0;
  minutes       = 0;
  seconds       = 0;
  milliseconds  = 0;

  return true;
}



/*-------------------------------------------------------------------
//  XDATETIME::GetNDaysInJulian
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/12/2012 22:32:00
//
//  @return       long :
//  @param        day :
//  @param        month :
//  @param        year :
*/
/*-----------------------------------------------------------------*/
long XDATETIME::GetNDaysInJulian(int day, int month, int year)
{
  this->day   = day;
  this->month = month;
  this->year  = year;

  return GetNDaysInJulian();
}




/*-------------------------------------------------------------------
//  XDATETIME::GetNDaysInGregorian
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/12/2012 22:31:53
//
//  @return       long :
//  @param        day :
//  @param        month :
//  @param        year :
*/
/*-----------------------------------------------------------------*/
long XDATETIME::GetNDaysInGregorian(int day, int month, int year)
{
  this->day   = day;
  this->month = month;
  this->year  = year;

  return GetNDaysInGregorian();
}
