/*------------------------------------------------------------------------------------------
//  DIOWEBHEADER.CPP
//
//  DIO WEB (HTTP) Header class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/04/2013 19:36:22
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XSleep.h"
#include "XDebug.h"
#include "XTimer.h"

#include "DIOStreamTCPIP.h"
#include "DIOWebServer.h"

#include "DIOWebHeader.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOWEBHEADER::DIOWEBHEADER
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/04/2013 19:37:49
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWEBHEADER::DIOWEBHEADER()
{
  Clean();

}



/*-------------------------------------------------------------------
//  DIOWEBHEADER::~DIOWEBHEADER
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/04/2013 19:37:59
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWEBHEADER::~DIOWEBHEADER()
{
  DeleteAllLines();

  Clean();
}



/*-------------------------------------------------------------------
//  DIOWEBHEADER::Read
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/04/2013 7:02:40
//
//  @return       bool :
//  @param        DIOSTREAMTCPIP* diostream :
//  @param        xtimer :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBHEADER::Read(DIOSTREAMTCPIP* diostream, XTIMER* xtimer, int timeout)
{
  if(!diostream) return false;
  if(!xtimer)    return false;

  bool status;

  DeleteAllLines();

  XSTRING line;

  xtimer->Reset();

  do{ status =  diostream->ReadStr(line);
      if(status)
        {
          if(line.IsEmpty())
            {
              break;
            }
           else
            {
              //XDEBUG_PRINTCOLOR(3, __L("WEB CLIENT HEADER LINE: %s"), line.Get());
              AddLine(line);
            }
        }

      if(xtimer && timeout)
        {
          if(xtimer->GetMeasureSeconds()>(XDWORD)timeout) break;

          xsleep->MilliSeconds(1);
        }

    } while((diostream->GetConnectStatus() == DIOSTREAMSTATUS_CONNECTED) || diostream->GetInXBuffer()->GetSize());

  //if(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED) return false;

  if(lines.IsEmpty()) return false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOWEBHEADER::Write
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/04/2013 10:45:28
//
//  @return       bool :
//  @param        DIOSTREAMTCPIP* diostream :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBHEADER::Write(DIOSTREAMTCPIP* diostream, int timeout)
{
  if(!diostream) return false;

  for(int c=0;c<(int)lines.GetSize();c++)
    {
      XSTRING* line = (XSTRING*)lines.Get(c);
      if(line)
        {
          //XDEBUG_PRINTCOLOR(2, __L("%s"), line->Get());

          diostream->WriteStr((*line));
          diostream->WriteStr(__L("\r\n"));
        }
    }

  diostream->WriteStr(__L("\r\n"));
  diostream->WaitToFlushOutXBuffer(timeout);

  if(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED) return false;

  return true;
}





/*-------------------------------------------------------------------
//  DIOWEBHEADER::AddLine
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/04/2013 19:42:39
//
//  @return       bool :
//  @param        line :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBHEADER::AddLine(XCHAR* line)
{
  XSTRING* _line;

  _line  = new XSTRING();
  if(!_line) return false;

  (*_line) = line;

  lines.Add(_line);

  return true;
}



/*-------------------------------------------------------------------
//  DIOWEBHEADER::AddLine
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/04/2013 19:42:44
//
//  @return       bool :
//  @param        line :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBHEADER::AddLine(XSTRING& line)
{
  return AddLine(line.Get());
}




/*-------------------------------------------------------------------
//  DIOWEBHEADER::GetLines
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/04/2013 19:42:50
//
//  @return       XVECTOR* :
//  */
/*-----------------------------------------------------------------*/
XVECTOR<XSTRING*>* DIOWEBHEADER::GetLines()
{
  return &lines;
}



/*-------------------------------------------------------------------
//  DIOWEBHEADER::GetFieldValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2013 23:56:49
//
//  @return       XCHAR* :
//  @param        field :
*/
/*-----------------------------------------------------------------*/
XCHAR* DIOWEBHEADER::GetFieldValue(XCHAR* field)
{
  for(int c=0;c<(int)lines.GetSize();c++)
    {
       XSTRING* line = (XSTRING*)lines.Get(c);
       if(line)
         {
           int index = line->Find(field, true);
           if(index!= XSTRING_NOTFOUND)
             {
               int size = (int)XSTRING::GetSize(field);

               return line->Get() + index + size + 1;
             }
         }
    }

  return NULL;
}



/*-------------------------------------------------------------------
//  DIOWEBHEADER::GetFieldValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2013 23:56:39
//
//  @return       XCHAR* :
//  @param        field :
*/
/*-----------------------------------------------------------------*/
XCHAR* DIOWEBHEADER::GetFieldValue(XSTRING& field)
{
  return GetFieldValue(field.Get());
}



/*-------------------------------------------------------------------
//  DIOWEBHEADER::DeleteAllLines
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/04/2013 19:42:55
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOWEBHEADER::DeleteAllLines()
{
  if(lines.IsEmpty()) return false;

  lines.DeleteContents();

  lines.DeleteAll();

  return true;
}





/*-------------------------------------------------------------------
//   DIOWEBHEADER:GetFormat
*/
/**
//
//
//
//  @author   Abraham J. Velez
//  @version
//
//  @return   bool :
//
//  @param    bool :
//  @param    int :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
bool DIOWEBHEADER::GetFormat(bool isserver, int type, void* data)
{
  if(lines.IsEmpty()) return false;

  if(!data) return false;

  XSTRING line;
  int     version    = 0;
  int     subversion = 0;

  line =((XSTRING*)lines.Get(0))->Get();

  if(isserver)
    {
      int     code;
      XSTRING codestring(DIOWEBHEADER_MAXLINE);

      line.UnFormat(__L("HTTP/%d.%d %d %s"),&version,&subversion,&code,codestring.Get());
      codestring.AdjustSize();

      switch(type)
        {
          case  0: (*(int*)data)     = version;     break;
          case  1: (*(int*)data)     = subversion;  break;
          case  2: (*(int*)data)     = code;        break;
          case  3: (*(XSTRING*)data) = codestring;  break;
        }
    }
   else
    {
      XSTRING method(DIOWEBHEADER_MAXLINE);
      XSTRING resource(DIOWEBHEADER_MAXLINE);

      if(line.GetSize())
        {
          int foundindex = line.Find(__L("HTTP/"), true);

          if(foundindex != XSTRING_NOTFOUND) line.UnFormat(__L("%s %s HTTP/%d.%d"), method.Get(), resource.Get(), &version, &subversion);
        }

      method.AdjustSize();
      resource.AdjustSize();

      switch(type)
        {
          case  0: if(data) (*(XSTRING*)data) = method;     break;
          case  1: if(data) (*(XSTRING*)data) = resource;   break;
          case  2: if(data) (*(int*)data)     = version;    break;
          case  3: if(data) (*(int*)data)     = subversion; break;
        }
    }

  return true;
}





