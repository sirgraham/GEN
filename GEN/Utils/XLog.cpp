//------------------------------------------------------------------------------------------
//  XLOG.CPP
//
//  Debug Funtions
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/04/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "XFactory.h"
#include "XDateTime.h"
#include "XTimer.h"
#include "XDir.h"
#include "XFileTXT.h"
#include "XFileZIP.h"
#include "XDebug.h"

#include "XLog.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------

XLOG*  XLOG::instance = NULL;

//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  XLOG::XLOG
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/11/2012 14:44:44
//
//  @return

*/
/*-----------------------------------------------------------------*/
XLOG::XLOG()
{
  Clean();

  xdatetime = xfactory->CreateDateTime();
  xtimer    = xfactory->CreateTimer();
  mutex     = xfactory->Create_Mutex();

  filelog = new XFILETXT();
}



//-------------------------------------------------------------------
//  XLOG::~XLOG
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 17:03:22
//
//  @return
*/
//-------------------------------------------------------------------
XLOG::~XLOG()
{
  if (filelog)    filelog->Close();
  if(filelog)     delete filelog;
  if(mutex)       xfactory->Delete_Mutex(mutex);
  if(xtimer)      xfactory->DeleteTimer(xtimer);
  if(xdatetime)   xfactory->DeleteDateTime(xdatetime);

  this->entrys.DeleteContents();
  this->entrys.DeleteAll();

  Clean();
}


/*-------------------------------------------------------------------
//  XLOG::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      22/02/2015 13:02:42
//
//  @return       bool :
//
//  @param        pathfile :
//  @param        nameapplication :
//  @param        isdumpindebug :
*/
/*-----------------------------------------------------------------*/
bool XLOG::Ini(XCHAR* pathfile, XCHAR* nameapplication, bool isdumpindebug)
{
  if(isactive) return false;

  this->xpathfile     = pathfile;
  this->isdumpindebug = isdumpindebug;

  if(nameapplication) this->nameapplication = nameapplication;

  if(!filelog) return false;

  bool status = filelog->Open(xpathfile,false);
  if(!status) status = filelog->Create(xpathfile);

  if(status) CalculateInitialStatus();

  isactive = status;

  return status;
}



/*-------------------------------------------------------------------
//  XLOG::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2015 13:59:03
//
//  @return       bool :
//
//  @param        xpathfile :
//  @param        nameapplication :
//  @param        isdumpindebug :
*/
/*-----------------------------------------------------------------*/
bool XLOG::Ini(XPATH& xpathfile, XCHAR* nameapplication, bool isdumpindebug)
{
  return Ini(xpathfile.Get(), nameapplication, isdumpindebug);
}





/*-------------------------------------------------------------------
//  XLOG::Ini
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/02/2013 20:24:57
//
//  @return       bool :
//  @param        XPATH&  pathfile :
//  @param        isdumpindebug :
*/
/*-----------------------------------------------------------------*/
bool XLOG::Ini(XPATH& xpathfile, XSTRING& nameapplication, bool isdumpindebug)
{
  return Ini(xpathfile.Get(), nameapplication.Get(), isdumpindebug);
}







/*-------------------------------------------------------------------
//  XLOG::SetLimit
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2013 22:57:23
//
//  @return       bool :
//  @param        typelimit :
//  @param        limit :
//  @param        reductionpercent :
*/
/*-----------------------------------------------------------------*/
bool XLOG::SetLimit(XLOGTYPELIMIT typelimit, int limit, int reductionpercent)
{
  this->typelimit        = typelimit;
  this->limit            = limit;
  this->reductionpercent = reductionpercent;

  return true;
}



/*-------------------------------------------------------------------
//  XLOG::SetBackup
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2014 14:11:25
//
//  @return       bool :
//
//  @param        isactive :
//  @param        maxfilesbackup :
//  @param        iscompress :
*/
/*-----------------------------------------------------------------*/
bool XLOG::SetBackup(bool isactive, int maxfiles, bool iscompress)
{
  this->backupisactive    = isactive;
  this->backupmaxfiles    = maxfiles;
  this->backupiscompress  = iscompress;

  return true;
}




/*-------------------------------------------------------------------
//  XLOG::SetFilters
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2013 0:21:17
//
//  @return       bool :
//  @param        sectionsID :
//  @param        level :
*/
/*-----------------------------------------------------------------*/
bool XLOG::SetFilters(XCHAR* sectionsID, XBYTE level)
{
  if(sectionsID) filtersectionsID = sectionsID;
  filterlevel = level;

  return true;
}



/*-------------------------------------------------------------------
//  XLOG::IsActive
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/11/2012 16:07:52
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XLOG::IsActive()
{
  if(!filelog)            return false;
  if(!filelog->IsOpen())  return false;
  if(!isactive)           return false;

  return true;
}




/*-------------------------------------------------------------------
//  XLOG::GetLevelString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2013 0:00:39
//
//  @return       bool :
//  @param        level :
//  @param        strlevel :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool XLOG::GetLevelString(XLOGLEVEL level, XSTRING& strlevel, int size)
{
  strlevel.Empty();

  switch(level)
    {
      case XLOGLEVEL_INFO     : strlevel = __L("INFO");       break;
      case XLOGLEVEL_WARNING  : strlevel = __L("WARNING");    break;
      case XLOGLEVEL_ERROR    : strlevel = __L("ERROR");      break;
      default: break;
    }

  if(strlevel.IsEmpty()) return false;

  if(size>0) strlevel.AdjustSize(size, false, __L(" "));

  return true;
}




/*-------------------------------------------------------------------
//  XLOG::GetSectionsIDFilter
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2013 17:30:59
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* XLOG::GetSectionsIDFilter()
{
  return &filtersectionsID;
}



/*-------------------------------------------------------------------
//  XLOG::PassFilters
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/07/2013 14:00:05
//
//  @return       bool :
//  @param        level :
//  @param        sectionID :
*/
/*-----------------------------------------------------------------*/
bool XLOG::PassFilters(XLOGLEVEL level, XCHAR* sectionID)
{
  if(!(filterlevel&level)) return false;

  if(sectionID) lastsectionID = sectionID;

  if(filtersectionsID.IsEmpty())  return false;

  if(filtersectionsID.Find(sectionID?sectionID:lastsectionID.Get(), true) == XSTRING_NOTFOUND)  return false;

  return true;
}




/*-------------------------------------------------------------------
//  XLOG::AddEntry
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2013 23:16:04
//
//  @return       bool :
//  @param        level :
//  @param        sectionID :
//  @param        inmemory :
//  @param        mask :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool XLOG::AddEntry(XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XCHAR* mask,...)
{
  //XDEBUG_PRINTCOLOR(3,__L(" XLOG::AddEntry %u"),XMEMORY_GETMEMORYUSED);

  if(!IsActive()) return false;
  if(!mask)       return false;
  if(!mutex)      return false;

  mutex->Lock();

  if(sectionID)
    {
      if(!PassFilters(level, sectionID))
        {
          mutex->UnLock();
          return false;
        }
    }

  //XDEBUG_PRINTCOLOR(3,__L(" XLOG::AddEntry  %u"),XMEMORY_GETMEMORYUSED);

  Backup_ControlLimits();

  XSTRING outstring;
  XSTRING string;

  XSTRING datestring;
  XSTRING levelstring;

  if(xdatetime)
    {
      xdatetime->Read();
      xdatetime->GetDateTimeToString(XDATETIME_FORMAT_STANDARD, datestring);
    }

  GetLevelString(level, levelstring, 7);
  //XDEBUG_PRINTCOLOR(3,__L(" XLOG::AddEntry %u"),XMEMORY_GETMEMORYUSED);
  if(sectionID)
    {
      string = datestring;
      string.AdjustSize(XLOG_MAXDATETIMESIZE, false, __L(" "));
      outstring += string;

      string = levelstring.Get();
      string.AdjustSize(XLOG_MAXLEVELSIZE, false, __L(" "));
      outstring += string;

      string = sectionID;
      string.AdjustSize(XLOG_MAXSECTIONIDSIZE, false, __L(" "));
    }
   else
    {
      string = __L(" ");
      string.AdjustSize(XLOG_MAXDATETIMESIZE +  XLOG_MAXLEVELSIZE + XLOG_MAXSECTIONIDSIZE, false, __L(" "));
    }
  //XDEBUG_PRINTCOLOR(3,__L(" XLOG::AddEntry  %u"),XMEMORY_GETMEMORYUSED);
  outstring += string;
  outstring += __L(" ");

  va_list arg;

  va_start(arg, mask);

  string.FormatArg(mask, &arg);

  va_end(arg);

  outstring += string;
  //XDEBUG_PRINTCOLOR(3,__L(" XLOG::AddEntry %u"),XMEMORY_GETMEMORYUSED);
  if(isdumpindebug)
    {
      int leveldebug = 0;

      switch(level)
        {
          case XLOGLEVEL_WARNING  : leveldebug = 3; break;
          case XLOGLEVEL_ERROR    : leveldebug = 4; break;

          default:
          case XLOGLEVEL_INFO     : leveldebug = 0; break;
        }

      XDEBUG_PRINTCOLOR(leveldebug , __L("[Log] %s "), outstring.Get());
    }
  //XDEBUG_PRINTCOLOR(3,__L(" XLOG::AddEntry  %u"),XMEMORY_GETMEMORYUSED);
  if(inmemory)
    {
      filelog->AddLine(outstring);
    }
   else
    {
      int resultsizeline = 0;
      int resultsizeLF   = 0;

      if(filelog->GetNLines()) FlushMemoryEntrys();

      if(outstring.Get()[0] != 0x20)
        {
          int position;

          filelog->GetPrimaryFile()->GetPosition(position);

          nentrys++;

          lastentry = new XLOGENTRY();
          if(lastentry)
            {
              lastentry->position = position;
              entrys.Add(lastentry);
            }
        }

      filelog->AddLineAlready(outstring.Get(), &resultsizeline, &resultsizeLF);

      if(lastentry) lastentry->size += (resultsizeline + resultsizeLF);

      size += (resultsizeline + resultsizeLF);

      nlines++;
    }
  //XDEBUG_PRINTCOLOR(3,__L(" XLOG::AddEntry %u"),XMEMORY_GETMEMORYUSED);
  mutex->UnLock();
  //XDEBUG_PRINTCOLOR(3,__L(" XLOG::AddEntry  %u"),XMEMORY_GETMEMORYUSED);
  return true;
}





/*-------------------------------------------------------------------
//  XLOG::AddEntry
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/01/2014 13:35:04
//
//  @return       bool :
//  @param        level :
//  @param        sectionID :
//  @param        inmemory :
//  @param        data :
//  @param        size :
//  @param        sizeline :
//  @param        showoffset :
//  @param        showtext :
*/
/*-----------------------------------------------------------------*/
bool XLOG::AddEntry(XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XBYTE* data, int size, int sizeline, bool showoffset, bool showtext)
{
  if(!IsActive()) return false;
  if(!mutex)      return false;

  mutex->Lock();

  if(!PassFilters(level, sectionID))
    {
      mutex->UnLock();
      return false;
    }

  XSTRING    strdata;
  int        _size     = 0;
  int        _sizeline = sizeline;
  int        index     = 0;
  int        lines     = 0;

  while(_size<size)
    {
      XSTRING string;

      if(_sizeline > (size - _size)) _sizeline = (size - _size);

      if(showoffset)
        {
          strdata.Format(__L("%04X"),index);
          string += strdata;
          string += __L("   ");
        }

      for(int c=0;c<_sizeline;c++)
        {
          strdata.Format(__L("%02X "),data[index]);
          string += strdata;
          _size++;
          index++;
        }

      if(_sizeline != sizeline)
        {
          for(int c=0;c<(sizeline-_sizeline);c++)
            {
              strdata.Format(__L("   "));
              string += strdata;
            }
        }

      if(showtext)
        {
          index -= _sizeline;
          string += __L(" ");

          for(int c=0;c<_sizeline;c++)
            {
              XCHAR character = (XCHAR)data[index];

              if((character<0x20) || (character>0x80) || (character==__C('%')) || (character==__C('\\'))) character = __C('.');

              strdata.Format(__L("%c"),character);
              string += strdata;
              index++;
            }
        }

      mutex->UnLock();

      AddEntry(level, lines?NULL:sectionID, inmemory, string.Get());

      mutex->Lock();

      lines++;
    }

  mutex->UnLock();

  return true;
}



/*-------------------------------------------------------------------
//  XLOG::AddEntry
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2013 23:16:55
//
//  @return       bool :
//  @param        level :
//  @param        sectionID :
//  @param        inmemory :
//  @param        data :
//  @param        sizeline :
//  @param        showoffset :
//  @param        showtext :
*/
/*-----------------------------------------------------------------*/
bool XLOG::AddEntry(XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XBUFFER& data, int sizeline, bool showoffset, bool showtext)
{
  return XLOG::AddEntry(level, sectionID, inmemory, data.Get(), data.GetSize(), sizeline, showoffset, showtext);
}



/*-------------------------------------------------------------------
//  XLOG::Flush
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/01/2013 0:09:20
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XLOG::Flush()
{
  if(!mutex)      return false;
  if(!IsActive()) return false;

  mutex->Lock();

  if(filelog->GetNLines()) FlushMemoryEntrys();

  mutex->UnLock();

  return true;
}



/*-------------------------------------------------------------------
//  XLOG::End
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/07/2012 9:41:19
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XLOG::End(bool doflush)
{
  if(!isactive) return false;

  if(doflush) Flush();

  if(!filelog) return false;

  filelog->Close();

  isactive = false;

  nentrys = 0;
  nlines  = 0;
  size    = 0;

  entrys.DeleteContents();
  entrys.DeleteAll();

  lastentry = NULL;

  return true;
}






/*-------------------------------------------------------------------
//  XLOG::CalculateInitialStatus
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/01/2014 11:53:11
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XLOG::CalculateInitialStatus()
{
  XFILE* xfile = filelog->GetPrimaryFile();
  if(!xfile) return false;

  nlines  = 0;
  nentrys = 0;
  size    = xfile->GetSize();

  entrys.DeleteContents();
  entrys.DeleteAll();

  int                sizeBOM              = 0;
  XFILETXTFORMATCHAR formatchar           = filelog->GetFormatCharFromFile(&sizeBOM);
  int                sizebytescharacter   = filelog->SizeOfCharacter(formatchar);

  xfile->SetPosition(sizeBOM);

  bool    endfile;
  int     br;
  XBUFFER dataline;
  XBYTE*  readbuffer = new XBYTE[XFILETXT_MAXBUFFER];
  if(!readbuffer) return false;

  memset(readbuffer, 0, XFILETXT_MAXBUFFER);

  do{ int bufferpos = 0;

      br            = XFILETXT_MAXBUFFER;
      endfile = !xfile->Read(readbuffer, &br);
      if(!br) break;

      do{ XFILETXTTYPELF typeLF;
          int            sizeLF   = 0;
          int            sizeline = 0;
          bool           endline = filelog->GetSizeOfLine(formatchar, &readbuffer[bufferpos], typeLF, sizeLF, sizeline, (br-bufferpos));

          if(endline)
            {
              if(readbuffer[bufferpos] != 0x20)
                {
                  nentrys++;

                  lastentry = new XLOGENTRY();
                  if(lastentry)
                    {
                      lastentry->position = bufferpos;
                      entrys.Add(lastentry);
                    }
                }

              if(lastentry) lastentry->size += (sizeline + sizeLF);
              nlines++;

              sizeline  *= sizebytescharacter;
              sizeline  += (sizeLF*sizebytescharacter);
              bufferpos += sizeline;
            }
           else
            {
              int lack = (br-bufferpos);
              bufferpos += lack;

              if(endfile && (bufferpos == br))
                {
                  if(readbuffer[bufferpos] != 0x20)
                    {
                      nentrys++;

                      lastentry = new XLOGENTRY();
                      if(lastentry)
                        {
                          lastentry->position = bufferpos;
                          entrys.Add(lastentry);
                        }
                    }

                  if(lastentry) lastentry->size += lack;
                  nlines++;
                }
            }

        } while(bufferpos < br);

    } while(!endfile);

  delete [] readbuffer;

  return true;


}


/*-------------------------------------------------------------------
//  XLOG::FlushMemoryEntrys
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/01/2013 0:09:37
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XLOG::FlushMemoryEntrys()
{
  int c=0;
  while(c<filelog->GetNLines())
    {
      XSTRING* line = filelog->GetLine(c);
      if(line)
        {
          int resultsizeline = 0;
          int resultsizeLF   = 0;

          if(line->Get()[0] != 0x20)
            {
              int position;

              filelog->GetPrimaryFile()->GetPosition(position);

              nentrys++;

              lastentry = new XLOGENTRY();
              if(lastentry)
                {
                  lastentry->position = position;
                  entrys.Add(lastentry);
                }
            }

          filelog->AddLineAlready(line->Get(), &resultsizeline, &resultsizeLF);

          if(lastentry) lastentry->size += (resultsizeline + resultsizeLF);

          size += (resultsizeline + resultsizeLF);

          nlines++;
        }

      c++;
    }

  filelog->DeleteAllLines();

  return true;
}



/*-------------------------------------------------------------------
//  XLOG::Backup_GetNFiles
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/01/2014 16:01:07
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int XLOG::Backup_GetNFiles()
{
  XPATH         xpathnamelog;
  XPATH         xpath;
  int           nfiles = 0;
  XDIRELEMENT   xdirelement;

  xpathnamelog = xpathfile.Get();

  xpathnamelog.GetPath(xpath);

  XDIR* xdir = xfactory->Create_Dir();
  if(!xdir) return 0;

  if(xdir->FirstSearch(xpath.Get(), XLOG_SEARCHMASKBACKUPFILE , &xdirelement))
    {
      do{ if(xdirelement.GetType() == XDIRELEMENTTYPE_FILE) nfiles++;

        } while(xdir->NextSearch(&xdirelement));
    }

  xfactory->Delete_Dir(xdir);

  return nfiles;
}




/*-------------------------------------------------------------------
//  XLOG::Backup_GetNameFileMoreOld
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/01/2014 16:24:57
//
//  @return       bool :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool XLOG::Backup_GetNameFileMoreOld(XPATH& xpathselect)
{
  if(!xdatetime) return false;

  XPATH         xpathnamelog;
  XPATH         xpath;
  XPATH         selectnamefile;
  int           nfiles = 0;
  XDIRELEMENT   xdirelement;
  XQWORD        lastseconds = 0x8FFFFFFFFFFFFFFFULL;

  xpathnamelog = xpathfile.Get();

  xpathnamelog.GetPath(xpath);

  XDIR* xdir = xfactory->Create_Dir();
  if(!xdir) return false;

  if(xdir->FirstSearch(xpath.Get(), XLOG_SEARCHMASKBACKUPFILE, &xdirelement))
    {
      do{ if(xdirelement.GetType() == XDIRELEMENTTYPE_FILE)
            {
              XSTRING* namefile = xdirelement.GetNameFile();
              if(namefile)
                {
                  int day     = 0;
                  int month   = 0;
                  int year    = 0;
                  int hours   = 0;
                  int minutes = 0;
                  int seconds = 0;

                  namefile->UnFormat(XLOG_CREATEMASKBACKUPFILE, &year, &month, &day, &hours, &minutes, &seconds);

                  xdatetime->SetDay(day);
                  xdatetime->SetMonth(month);
                  xdatetime->SetYear(year);
                  xdatetime->SetHours(hours);
                  xdatetime->SetMinutes(minutes);
                  xdatetime->SetSeconds(seconds);

                  if(lastseconds > xdatetime->GetSeconsFromDate())
                    {
                      lastseconds = xdatetime->GetSeconsFromDate();
                      selectnamefile = xdirelement.GetNameFile()->Get();
                    }
                }
            }

        } while(xdir->NextSearch(&xdirelement));
    }

  xfactory->Delete_Dir(xdir);

  xpathselect  = xpath;
  xpathselect += selectnamefile;

  return true;
}




/*-------------------------------------------------------------------
//  XLOG::Backup_CreateNameZIP
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2014 22:51:46
//
//  @return       bool :
//
//  @param        xpathzipfile :
*/
/*-----------------------------------------------------------------*/
bool XLOG::Backup_CreateNameZIP(XPATH& xpathzipfile)
{
  xpathzipfile.Empty();

  xpathzipfile  = xpathfile.Get();
  xpathzipfile.SetOnlyPathAndNamefile();
  xpathzipfile += __L(".zip");
  xpathzipfile.AddToNameFile(true, XLOG_PREFIX);

  return true;
}



/*-------------------------------------------------------------------
//  XLOG::Backup_AdjustNFilesInCompressed
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2014 15:50:29
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XLOG::Backup_AdjustNFilesInCompressed()
{
  XFILEUNZIP xfileunzip;
  XPATH      xpathzipfile;

  Backup_CreateNameZIP(xpathzipfile);

  if(xfileunzip.Open(xpathzipfile))
    {
      if(xfileunzip.GetNFiles()>=backupmaxfiles)
        {
          xfileunzip.DelFile(0);
        }

      xfileunzip.Close();
    }

  return true;
}




/*-------------------------------------------------------------------
//  XLOG::Backup_AdjustNFiles
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/09/2014 13:41:19
//
//  @return       bool :
//
//  @param        pathnamelog :
//  @param        iscompress :
*/
/*-----------------------------------------------------------------*/
bool XLOG::Backup_AdjustNFiles(XCHAR* pathnamelog, bool iscompress)
{
  XFILE* xfilebackup = xfactory->Create_File();
  if(!xfilebackup)  return false;

  XPATH    xpathnamelog;
  XSTRING  datestr;
  bool     status      = false;
  int      indexbackup = 0;

  xpathnamelog = pathnamelog;

  if(!iscompress)
    {
      if(Backup_GetNFiles()>backupmaxfiles)
        {
          Backup_GetNameFileMoreOld(xpathnamelog);
          xfilebackup->Erase(xpathnamelog);
        }
    }
   else
    {
      Backup_AdjustNFilesInCompressed();
    }

  if(iscompress)
    {
      XFILEZIP* xfilezip = new XFILEZIP();
      if(xfilezip)
        {
          XPATH xpathzipfile;

          Backup_CreateNameZIP(xpathzipfile);

          if(xfilezip->Open(xpathzipfile))
            {
              XPATH namefileinzip;

              xpathnamelog.GetNamefileExt(namefileinzip);

              if(!xfilezip->AddFile(xpathnamelog, namefileinzip)) status = false;

              xfilezip->Close();
            }

          delete xfilezip;

          status = xfilebackup->Erase(pathnamelog);
        }
    }

  xfactory->Delete_File(xfilebackup);

  return status;
}





/*-------------------------------------------------------------------
//  XLOG::Backup_ControlLimits
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/01/2014 16:25:12
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XLOG::Backup_ControlLimits()
{
  if(!IsActive()) return false;

  switch(typelimit)
    {
      case XLOGTYPELIMIT_NENTRYS  : if(nentrys<limit)  return false;
                                    break;

      case XLOGTYPELIMIT_SIZE     : if(size<limit)    return false;
                                    break;
      default: break;
    }

  Flush();

  bool status = true;

  if(backupisactive)
    {
      XPATH    xpathnamelog;
      XSTRING  datestr;

      //  If have backup DON´T HAVE reduction
      //  The actual log file start
      xpathnamelog       = xpathfile.Get();
      backupxpathnamelog = xpathfile.Get();

      xdatetime->Read();
      datestr.Format(XLOG_CREATEMASKBACKUPFILE, xdatetime->GetYear()
                                              , xdatetime->GetMonth()
                                              , xdatetime->GetDay()
                                              , xdatetime->GetHours()
                                              , xdatetime->GetMinutes()
                                              , xdatetime->GetSeconds());

      backupxpathnamelog.AddToNameFile(true, datestr.Get());

      End(false);

      XFILE* xfile = xfactory->Create_File();
      if(xfile)
        {
          xfile->Rename(xpathnamelog, backupxpathnamelog);
          xfactory->Delete_File(xfile);
        }

      status = Ini(xpathnamelog, NULL, isdumpindebug);
      if(status) status = Backup_AdjustNFiles(backupxpathnamelog.Get(), backupiscompress);
    }
   else
    {
      // Reduction control: NOT Backup.

      XDWORD nentrystoerase = nentrys - ((nentrys * reductionpercent) / 100);
      int    sizetocopy = 0;

      for(int c=0; c<(int)nentrystoerase; c++)
        {
          XLOGENTRY* entry = entrys.Get(c);
          if(entry) sizetocopy += entry->size;
        }

      if(!sizetocopy) return false;

      filelog->GetPrimaryFile()->SetPosition(0);
      filelog->GetPrimaryFile()->EraseBlock(sizetocopy);
    }

  if(status) status = CalculateInitialStatus();

  return status;
}

