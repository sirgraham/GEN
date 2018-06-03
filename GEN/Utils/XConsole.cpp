/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XConsole.cpp
*
* @class       XCONSOLE
* @brief       Console base class
* @ingroup     UTILS
*
* @author      Abraham J. Velez 
* @date        28/05/2018 20:55:23
*
* @copyright   Copyright(c) 2005 - 2018 GEN Group.
*
* @cond
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* @endcond
*
*---------------------------------------------------------------------------------------------------------------------*/

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include "XFactory.h"
#include "XDateTime.h"
#include "XBuffer.h"
#include "XTimer.h"
#include "XSleep.h"

#include "XConsole.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XCONSOLE::XCONSOLE()
* @brief      Constructor
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       28/05/2018 20:56:23
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XCONSOLE::XCONSOLE()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XCONSOLE::~XCONSOLE()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:23:49
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XCONSOLE::~XCONSOLE()

{
  Clean();
}


 
/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::GetSize(int& width, int& height)
* @brief      Get size Of Console
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:21:09
*
* @param[out]  width : return width of console
* @param[out]  height : return height of console
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XCONSOLE::GetSize(int& width, int& height)
{
  width  = 0;
  height = 0;
 
  return false;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::SetSize(int width, int height)
* @brief      Set Size oif console
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:23:08
*
* @param[in]  width : set width of console
* @param[in]  height : swet height of console
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XCONSOLE::SetSize(int width, int height) 
{ 
  return false;         
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::Maximize()
* @brief      Maximize console
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:24:11
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XCONSOLE::Maximize() 
{ 
  return false;         
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::Minimize()
* @brief      Minimize console
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:24:31
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XCONSOLE::Minimize() 
{ 
  return false;         
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::Hide()
* @brief      Hide console
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:24:57
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XCONSOLE::Hide()
{ 
  return false;         
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::IsHide()
* @brief      Check if the console is hide
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:25:16
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XCONSOLE::IsHide()
{ 
  return false;         
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::UnHide()
* @brief      unhide console
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:25:31
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XCONSOLE::UnHide() 
{ 
  return false;         
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::Clear()
* @brief      Clear console
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:25:44
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XCONSOLE::Clear()  
{ 
  return false;         
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::KBHit()
* @brief      Kbhit in console
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:26:07
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XCONSOLE::KBHit() 
{ 
  return false;         
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XCONSOLE::GetChar()
* @brief      GetChar Console
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:26:36
*
* @return     int : 
*
*---------------------------------------------------------------------------------------------------------------------*/
int XCONSOLE::GetChar()
{ 
  return 0;             
}





/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::Print(XCHAR* string)
* @brief      Print a string
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       28/05/2018 20:56:44
*
* @param[in]  string : string to print
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XCONSOLE::Print(XCHAR* string)
{
  if(!string) return false;

  //Print(string);
  Printf(string);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::Printf(XCHAR* mask,...)
* @brief      Print variables with a mask
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       28/05/2018 20:57:12
*
* @param[in]  mask : mask to use
* @param[in]  ... : variables
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
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



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::PrintDataBlock(XBYTE* data, XDWORD size, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
* @brief      Print data block in HEX and Character
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       28/05/2018 20:58:53
*
* @param[in]  data : data to print
* @param[in]  size : size of data
* @param[in]  marginsize : size of margin
* @param[in]  sizeline :  size for the line.
* @param[in]  showoffset : true if show offset
* @param[in]  showtext : true if show the data in char format.
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
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



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::PrintDataBlock(XBUFFER& data, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
* @brief      Print data block in HEX and Character
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       28/05/2018 21:00:08
*
* @param[in]  data : buffer data to print
* @param[in]  marginsize : size of margin
* @param[in]  sizeline :  size of line
* @param[in]  showoffset : true if show offset
* @param[in]  showtext : true if show the data in char format. 
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XCONSOLE::PrintDataBlock(XBUFFER& data, XDWORD marginsize, XDWORD sizeline, bool showoffset, bool showtext)
{
  return PrintDataBlock(data.Get(), data.GetSize(), marginsize, sizeline, showoffset, showtext);
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::FormatMessage(XCHAR* message, XDWORD margin, bool prelude, bool returnline, XSTRING& string)
* @brief      Format a message to string.
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       28/05/2018 21:01:43
*
* @param[in]  message : Message to format.
* @param[in]  margin : size margin
* @param[in]  prelude : true if has prelude
* @param[in]  returnline : true if has return
* @param[out] string : result string formated
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
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




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::PrintMessage(XCHAR* message, XDWORD margin, bool prelude, bool returnline)
* @brief      Print Message 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       28/05/2018 21:05:55
*
* @param[in]  message : Message to print
* @param[in]  margin : size of margin
* @param[in]  prelude : true if has prelude
* @param[in]  returnline : true if return line
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XCONSOLE::PrintMessage(XCHAR* message, XDWORD margin, bool prelude, bool returnline)
{
  XSTRING string;

  FormatMessage(message, margin, prelude, returnline, string);

  return Print(string.Get());
}





/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::TipicalHeader_Create(int yearorigin, XCHAR* nameapp,int version,int subversion,int subversionerr,XCHAR* enterprise,XSTRING& header)
* @brief      Create in string a tipical header in console
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       28/05/2018 21:07:06
*
* @param[in]  yearorigin : year of origin
* @param[in]  nameapp : name of application
* @param[in]  version : veraion of application
* @param[in]  subversion : suversion of application
* @param[in]  subversionerr : sub version error of application
* @param[in]  enterprise : enterprise
* @param[out] header : string result
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
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

  #ifdef XDEBUG_TRACE
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





/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::TipicalHeader_Show(int yearorigin, XCHAR* nameapp,int version,int subversion,int subversionerr, XCHAR* enterprise)
* @brief      Print a tipical header in console
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       28/05/2018 21:09:11
*
* @param[in]  yearorigin : year of origin
* @param[in]  nameapp : name of application
* @param[in]  version : veraion of application
* @param[in]  subversion : suversion of application
* @param[in]  subversionerr : sub version error of application
* @param[in]  enterprise : enterprise
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XCONSOLE::TipicalHeader_Show(int yearorigin, XCHAR* nameapp,int version,int subversion,int subversionerr, XCHAR* enterprise)
{
  XSTRING header;

  if(!TipicalHeader_Create(yearorigin, nameapp, version, subversion, subversionerr, enterprise, header)) return false;

  Printf(header.Get());

  return true;
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XCONSOLE::WaitKey(XCHAR* text, XDWORD margin, bool prelude, XDWORD timeout)
* @brief      Prints a Wait menssage with timeout
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       28/05/2018 21:10:04
*
* @param[in]  text : Text of the wait
* @param[in]  margin : size of margin
* @param[in]  prelude : true if has prelude
* @param[in]  timeout : timeout of the wait
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
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

      XSLEEP::GetInstance().MilliSeconds(100);

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




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XCONSOLE::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:29:41
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XCONSOLE::Clean()
{

}