//------------------------------------------------------------------------------------------
//  XCONSOLE.CPP
//
//  System resources Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 03/03/2004 12:28:40
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "XFactory.h"
#include "XDateTime.h"
#include "XBuffer.h"
#include "XTimer.h"
#include "XSleep.h"
#include "XConsole.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  XCONSOLE::XCONSOLE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/09/2012 9:17:20
//
//  @return
//  @param        xfactory :
*/
/*-----------------------------------------------------------------*/
XCONSOLE::XCONSOLE()
{
  Clean();
}



//-------------------------------------------------------------------
//  XCONSOLE::~XCONSOLE
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:29:20
//
//  @return
//  */
//-------------------------------------------------------------------
XCONSOLE::~XCONSOLE()
{
  Clean();
}







/*-------------------------------------------------------------------
//  XCONSOLE::ConsolePrint
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/03/2011 9:55:43
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XCONSOLE::Print(XCHAR* string)
{
  if(!string) return false;

  //Print(string);
  Printf(string);

  return true;
}




/*-------------------------------------------------------------------
//  XCONSOLE::Printf
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/10/2012 10:47:04
//
//  @return       bool :
//  @param        mask :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool XCONSOLE::Printf(XCHAR* mask,...)
{
  if(!mask) return false;

  XSTRING outstring;

  va_list arg;

  va_start(arg, mask);

  outstring.FormatArg(mask, &arg);

  va_end(arg);

  Print(outstring.Get());

  return true;
}




/*-------------------------------------------------------------------
//  XCONSOLE::PrintDataBlock
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/02/2015 9:26:25
//
//  @return       bool :
//
//  @param        data :
//  @param        size :
//  @param        marginsize :
//  @param        sizeline :
//  @param        showoffset :
//  @param        showtext :
*/
/*-----------------------------------------------------------------*/
bool XCONSOLE::PrintDataBlock(XBYTE* data, XDWORD size, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
{
  XSTRING    margin;
  XSTRING    strdata;
  XDWORD     _size     = 0;
  XDWORD     _sizeline = sizeline;
  int        index     = 0;

  for(XDWORD c=0;c<marginsize;c++)
    {
      margin += __L(" ");
    }

  while(_size<size)
    {
      XSTRING string;

      string   = margin;
      if(_sizeline > (size - _size)) _sizeline = (size - _size);

      if(showoffset)
        {
          strdata.Format(__L("%04X"),index);
          string += strdata;
          string += __L("   ");
        }

      for(XDWORD c=0; c<_sizeline; c++)
        {
          strdata.Format(__L("%02X "),data[index]);
          string += strdata;
          _size++;
          index++;
        }

      if(_sizeline != sizeline)
        {
          for(XDWORD c=0; c<(sizeline-_sizeline); c++)
            {
              strdata.Format(__L("   "));
              string += strdata;
            }
        }

      if(showtext)
        {
          index -= _sizeline;
          string += __L(" ");

          for(XDWORD c=0; c<_sizeline; c++)
            {
              XCHAR character = (XCHAR)data[index];

              if((character<0x20) || (character>0x80) || (character==__C('%')) || (character==__C('\\'))) character = __C('.');

              strdata.Format(__L("%c"),character);
              string += strdata;
              index++;
            }
        }

      Print(string.Get());
      Print(__L("\n"));

      if(_size > XCONSOLE_MAXSIZEDATABLOCK) break;
    }

  return true;
}




/*-------------------------------------------------------------------
//  XCONSOLE::PrintDataBlock
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/02/2015 9:27:37
//
//  @return       bool :
//
//  @param        data :
//  @param        marginsize :
//  @param        sizeline :
//  @param        showoffset :
//  @param        showtext :
*/
/*-----------------------------------------------------------------*/
bool XCONSOLE::PrintDataBlock(XBUFFER& data, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
{
  return PrintDataBlock(data.Get(), data.GetSize(), marginsize, sizeline, showoffset, showtext);
}




/*-------------------------------------------------------------------
//  XCONSOLE::FormatMessage
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      23/08/2016 11:03:33
//
//  @return       bool :
//
//  @param        message :
//  @param        margin :
//  @param        prelude :
//  @param        returnline :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XCONSOLE::FormatMessage(XCHAR* message, XDWORD margin, bool prelude, bool returnline, XSTRING& string)
{
  string.Empty();

  for(XDWORD c=0; c<margin; c++)
    {
      string += __L(" ");
    }

  if(prelude) string += __L("> ");

  string += message;

  if(returnline) string+=__L("\n");

  return true;
}




/*-------------------------------------------------------------------
//  XCONSOLE::PrintMessage
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      23/08/2016 11:03:50
//
//  @return       bool :
//
//  @param        message :
//  @param        margin :
//  @param        prelude :
//  @param        returnline :
*/
/*-----------------------------------------------------------------*/
bool XCONSOLE::PrintMessage(XCHAR* message, XDWORD margin, bool prelude, bool returnline)
{
  XSTRING string;

  FormatMessage(message, margin, prelude, returnline, string);

  return Print(string.Get());
}




/*-------------------------------------------------------------------
//  XCONSOLE::TipicalHeader_Create
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      23/08/2016 11:04:23
//
//  @return       bool :
//
//  @param        yearorigin :
//  @param        nameapp :
//  @param        version :
//  @param        subversion :
//  @param        subversionerr :
//  @param        enterprise :
//  @param        header :
*/
/*-----------------------------------------------------------------*/
bool XCONSOLE::TipicalHeader_Create(int yearorigin, XCHAR* nameapp,int version,int subversion,int subversionerr,XCHAR* enterprise,XSTRING& header)
{
  if(!nameapp)      return false;
  if(!enterprise)   return false;

  XDATETIME* xdatetime = xfactory->CreateDateTime();
  if(!xdatetime) return false;

  xdatetime->Read();

  header.Empty();

  header.Format(__L(" %s %d.%d.%d "), nameapp, version, subversion, subversionerr);

  XSTRING statusstr;

  if(version<1) statusstr += __L("Beta");

  #ifdef XDEBUG
  if(statusstr.GetSize()) statusstr += __L("+");
  statusstr += __L("Debug");
  #endif

  if(statusstr.GetSize())
    {
      header += __L("(");
      header += statusstr;
      header += __L(") ");
    }

  header += __L("Copyright (c) ");

  XSTRING string2;
  string2.Format((xdatetime->GetYear()>yearorigin)?__L("%d-%d "):__L("%d "), yearorigin, xdatetime->GetYear());

  header += string2;
  header += enterprise;
  //header += __L("\n\n");

  xfactory->DeleteDateTime(xdatetime);

  return true;
}




/*-------------------------------------------------------------------
//  XAPPLICATION::TipicalHeader_Show
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/03/2013 14:55:45
//
//  @return       bool :
//  @param        yearorigin :
//  @param        nameapp :
//  @param        version :
//  @param        subversion :
//  @param        subversionerr :
//  @param        enterprise :
*/
/*-----------------------------------------------------------------*/
bool XCONSOLE::TipicalHeader_Show(int yearorigin, XCHAR* nameapp,int version,int subversion,int subversionerr, XCHAR* enterprise)
{
  XSTRING header;

  if(!TipicalHeader_Create(yearorigin, nameapp, version, subversion, subversionerr, enterprise, header)) return false;

  Printf(header.Get());

  return true;
}



/*-------------------------------------------------------------------
//  XCONSOLE::WaitKey
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      23/08/2016 11:05:18
//
//  @return       bool :
//
//  @param        text :
//  @param        margin :
//  @param        prelude :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool XCONSOLE::WaitKey(XCHAR* text, XDWORD margin, bool prelude, XDWORD timeout)
{
  XTIMER* xtimer = xfactory->CreateTimer();
  if(!xtimer) return false;

  xtimer->Reset();

  XSTRING string;
  XSTRING string2;
  bool    status = true;

  while(!KBHit())
    {
      string  = text;
      string += __L("                            \r");

      string2.Format(string.Get(), (timeout - xtimer->GetMeasureSeconds()));

      xsleep->MilliSeconds(100);

      PrintMessage(string2.Get(), margin, prelude, false);

      if(timeout)
        {
          if((int)xtimer->GetMeasureSeconds() > timeout)
            {
              status = false;
              break;
            }
        }
    }

  while(KBHit())
    {
      GetChar();
    }

  string.Empty();

  for(int c=0; c<78; c++)
    {
      string += __L(" ");
    }

  string += __L("\r");
  PrintMessage(string.Get(), 0, false, false);

  xfactory->DeleteTimer(xtimer);

  return status;
}

