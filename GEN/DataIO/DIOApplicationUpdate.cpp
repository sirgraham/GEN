/*------------------------------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE.CPP
//
//  Update Functions
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 26/02/2009 10:40:01
//  Last Mofificacion :
//
//  GEN (C) Copyright  (All right reserved).
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XPath.h"
#include "XPathsManager.h"
#include "XDir.h"
#include "XFileINI.h"
#include "XFileZIP.h"
#include "XString.h"
#include "XSystem.h"
#include "XDebug.h"

#include "HashCRC32.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOWebClient.h"
#include "DIOApplicationUpdateXEvent.h"

#include "DIOApplicationUpdate.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE::DIOAPPLICATIONUPDATE
*/
/**
//
//  Class Constructor DIOAPPLICATIONUPDATE
//
//  @author       Abraham J. Velez
//  @version      16/07/2015 13:58:09
//


//  @param        xpublisher :
//  @param        applicationversion :
//  @param        applicationsubversion :
//  @param        applicationerrorcontrol :
//  @param        applicationname :
//  @param        xpathrootapp :
*/
/*-----------------------------------------------------------------*/
DIOAPPLICATIONUPDATE::DIOAPPLICATIONUPDATE(XDWORD applicationversion, XDWORD applicationsubversion, XDWORD applicationerrorcontrol, XCHAR* applicationname, XPATH& xpathrootapp)
{
  Clean();

  this->applicationversion      = applicationversion;
  this->applicationsubversion   = applicationsubversion;
  this->applicationerrorcontrol = applicationerrorcontrol;

  this->applicationname         = applicationname;
  this->xpathrootapp            = xpathrootapp;


  { XPATH xpathexecutable;

    xpathexecutable  = xpathrootapp;
    xpathexecutable += DIOAPPLICATIONUPDATE_BACKUPPREFIX;
    xpathexecutable += applicationname;
    xpathexecutable += __L(".exe");

    XFILE* xfile = xfactory->Create_File();
    if(xfile)
      {
        xfile->Erase(xpathexecutable);
        xfactory->Delete_File(xfile);
      }
  }

  xdir = xfactory->Create_Dir();
  if(diofactory)  webclient = new DIOWEBCLIENT;

  RegisterEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_GETVERSIONAVAILABLE);
  RegisterEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_DOWNLOADFILE);
  RegisterEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_UNZIPFILE);
  RegisterEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_BACKUPORIGINALFILE);
  RegisterEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_COPYUPDATEFILE);
  RegisterEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_DELETEBACKUPORIGINALFILE);
  RegisterEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_RESTOREUPDATE);

}



/*-------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE::~DIOAPPLICATIONUPDATE
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/02/2009 10:46:10
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOAPPLICATIONUPDATE::~DIOAPPLICATIONUPDATE()
{
  DeRegisterEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_GETVERSIONAVAILABLE);
  DeRegisterEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_DOWNLOADFILE);
  DeRegisterEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_UNZIPFILE);
  DeRegisterEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_BACKUPORIGINALFILE);
  DeRegisterEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_COPYUPDATEFILE);
  DeRegisterEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_DELETEBACKUPORIGINALFILE);
  DeRegisterEvent(DIOAPPLICATIONUPDATEXEVENTTYPE_RESTOREUPDATE);

  DeleteAllFilesToUpload();

  if(xdir)      xfactory->Delete_Dir(xdir);
  if(webclient) delete webclient;

  Clean();
}




/*-------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE::GetVersionAvailable
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/07/2015 13:08:44
//
//  @return       bool :
//
//  @param        url :
//  @param        port :
//  @param        updateversiondata :
//  @param        timeout :
//  @param        error :
*/
/*-----------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::GetVersionAvailable(DIOURL& url, int port, DIOAPPLICATIONUPDATEVERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error)
{
  error  = DIOAPPLICATIONUPDATE_ERROR_INVALIDOPERATION;

  if(!webclient) return false;

  bool  status = false;

  DIOURL urlfile;
  XPATH  xpath;

  urlfile  = url;
  urlfile.Slash_Add();
  urlfile += DIOAPPLICATIONUPDATE_INIFILE;

  urlfile.Slash_Normalize();

  XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
  xpath.Slash_Add();
  xpath += DIOAPPLICATIONUPDATE_INIFILE;

  webclient->SetPort(port);

  status = webclient->Get(urlfile, xpath ,NULL, timeout);
  if(!status)
    {
      if(stopproccess)
        {
          error = DIOAPPLICATIONUPDATE_ERROR_BREAKFORUSER;
        }
       else
        {
          error = DIOAPPLICATIONUPDATE_ERROR_DOWNLOADFILE;
        }
    }
   else
    {
      error = DIOAPPLICATIONUPDATE_ERROR_INVALIDOPERATION;

      XFILEINI* ini = new XFILEINI();
      if(ini)
        {
          if(ini->Open(xpath))
            {
              XSTRING key;
              XSTRING value;
              int     index;

              if(ini->ReadValue(DIOAPPLICATIONUPDATE_GENERALSECTION , DIOAPPLICATIONUPDATE_GENERALSECTION_VERSION           , value))  updateversiondata.SetVersion(value.ConvertToInt());
              if(ini->ReadValue(DIOAPPLICATIONUPDATE_GENERALSECTION , DIOAPPLICATIONUPDATE_GENERALSECTION_SUBVERSION        , value))  updateversiondata.SetSubVersion(value.ConvertToInt());
              if(ini->ReadValue(DIOAPPLICATIONUPDATE_GENERALSECTION , DIOAPPLICATIONUPDATE_GENERALSECTION_ERRORCONTROL      , value))  updateversiondata.SetErrorControl(value.ConvertToInt());
              if(ini->ReadValue(DIOAPPLICATIONUPDATE_GENERALSECTION , DIOAPPLICATIONUPDATE_GENERALSECTION_SYSTEMMUSTBEINIT  , value))  updateversiondata.SetSystemMustBeInit(value.ConvertToBoolean());

              //----------------------------------------------------------------------------------------

              DeleteAllFilesToUpload();

              index = 0;

              do{ key.Format(__L("%s%d"), DIOAPPLICATIONUPDATE_FILESSECTION_FILES, index);

                  ini->ReadValue(DIOAPPLICATIONUPDATE_FILESSECTION, key, value);
                  if(!value.IsEmpty())
                    {
                      XPATH     xpathfile(XSTRING_STANDARDSIZE);
                      int       sizefile = 0;
                      XDWORD    CRC32    = 0;

                      int comma=value.FindCharacter(__C(','));
                      XSTRING name;
                      value.Copy(0,comma,name);
                      value.DeleteCharacters(0,comma+1);
                      value.UnFormat(__L("%d,%08X"), &sizefile, &CRC32);
                      xpathfile.Set(name.Get());

                      if(xpathfile.GetSize() && sizefile && CRC32)
                        {
                          DIOAPPLICATIONUPDATEFILE* updatefile = new DIOAPPLICATIONUPDATEFILE(xpathfile, CRC32, sizefile);
                          if(updatefile) filestoupdate.Add(updatefile);
                        }
                    }

                  index++;

                } while(index<DIOAPPLICATIONUPDATE_MAXINDEXFILES);

              //----------------------------------------------------------------------------------------

              ini->Close();
              error = DIOAPPLICATIONUPDATE_ERROR_OK;
            }

          delete ini;

          DIOAPPLICATIONUPDATEXEVENT xevent(this, DIOAPPLICATIONUPDATEXEVENTTYPE_GETVERSIONAVAILABLE);

          xevent.SetVersionData(&updateversiondata);
          xevent.SetFilesToUpdate(&filestoupdate);
          xevent.SetStatusOperation(status);

          PostEvent(&xevent);
       }
    }

  XFILE* file = xfactory->Create_File();
  if(file)
    {
      file->Erase(xpath);
      xfactory->Delete_File(file);
    }

  return status;
}




/*-------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE::Download
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/07/2015 16:35:16
//
//  @return       bool :
//
//  @param        url :
//  @param        port :
//  @param        addtargetpath :
//  @param        updateversiondata :
//  @param        timeout :
//  @param        error :
*/
/*-----------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::Download(DIOURL& url, int port, XCHAR* addtargetpath, DIOAPPLICATIONUPDATEVERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error)
{
  error  = DIOAPPLICATIONUPDATE_ERROR_INVALIDOPERATION;
  if(!xdir) return false;

  XPATH   namefile;
  XPATH   extenxion;
  DIOURL  urlfile;
  int     c;
  bool    status = false;


  if(!filestoupdate.GetSize())
  {
    XDEBUG_PRINTCOLOR(4,__L("No files to update"));
    return DIOAPPLICATIONUPDATE_ERROR_NOTFILESTOUPDATE;
  }


  for(c=0;c<(int)filestoupdate.GetSize();c++)
    {
      XPATH xpathfile;

      progressfiletoupdate = c;

      DIOAPPLICATIONUPDATEFILE* updatefile = (DIOAPPLICATIONUPDATEFILE*)filestoupdate.Get(c);
      if(!updatefile)  continue;

      urlfile  = url;
      urlfile.Slash_Add();
      urlfile += updatefile->GetXPathFile()->Get();
      urlfile.Slash_Normalize();

      xpathfile.Empty();

      if(addtargetpath)
        {
          if(addtargetpath[0])
            {
              xpathfile = addtargetpath;
              xpathfile.Slash_Add();
            }
        }

      xpathfile    += updatefile->GetXPathFile()->Get();
      xpathfile.Slash_Normalize();

      if(!xpathfile.IsEmpty())
        {
          XPATH xpath;

          xpathfile.GetDriveAndPath(xpath);

          if(!xpath.IsEmpty())
            {
              if(xdir) xdir->Make(xpath);
            }
        }

      xpathfile.GetNamefileExt(namefile);
      xpathfile.GetExt(extenxion);

      HASHCRC32 hashCRC32;
      int       sizetarget = 0;

      hashCRC32.ResetResult();
      hashCRC32.Do(xpathfile);

      if(updatefile->GetCRC32() != hashCRC32.GetResultCRC32())
        {
          webclient->SetPort(port);

          XDEBUG_PRINTCOLOR(1,__L("Downloading update file : %s"),namefile.Get());

          status = webclient->Get(urlfile, xpathfile, NULL, timeout);
          if(!status)
            {
              XDEBUG_PRINTCOLOR(4,__L("webclient failed to retrieve file %s:%s"),xpathfile.Get(),urlfile.Get());
              if(stopproccess)
                        error = DIOAPPLICATIONUPDATE_ERROR_BREAKFORUSER;
                  else  error = DIOAPPLICATIONUPDATE_ERROR_DOWNLOADFILE;
              break;
            }

          { DIOAPPLICATIONUPDATEXEVENT xevent(this, DIOAPPLICATIONUPDATEXEVENTTYPE_DOWNLOADFILE);

            xevent.SetVersionData(&updateversiondata);
            xevent.SetFilesToUpdate(&filestoupdate);
            xevent.SetIndexOfOperation(c);
            xevent.SetStatusOperation(status);

            //if(PostEvent(&xevent)) xevent.WaitForCompleted( timeout);
            PostEvent(&xevent);
          }

          hashCRC32.ResetResult();
          hashCRC32.Do(xpathfile);

          if(updatefile->GetCRC32() != hashCRC32.GetResultCRC32())
            {
              XDEBUG_PRINTCOLOR(4,__L("webclient CRC error on file %s:%s"),xpathfile.Get(),urlfile.Get());
              status = false;
              break;

            } else status = true;

        } else status = true;


      //-------------------------------------------------------------
      // Descompress File
      if(!extenxion.Compare(__L(".zip"), true))
        {
          XFILEUNZIP* unzip = new XFILEUNZIP();
          if(unzip)
            {
              if(unzip->Open(xpathfile))
                {
                  XPATH xpath;
                  int   sourceindex = 0;

                  xpath = xpathfile;
                  xpath.SetOnlyDriveAndPath();

                  status = true;

                  do{ XFILECMPZIP* xfilecmp =  unzip->GotoFile(sourceindex);
                      if(!xfilecmp) break;

                      XPATH xpathcomplete;

                      xpathcomplete  = xpath;
                      xpathcomplete += xfilecmp->GetName();

                      if(xfilecmp->IsDirectory())
                        {
                          xdir->Make(xpathcomplete);
                          status = true;
                        }
                       else
                        {

                          { DIOAPPLICATIONUPDATEXEVENT xevent(this, DIOAPPLICATIONUPDATEXEVENTTYPE_UNZIPFILE);

                            xevent.SetVersionData(&updateversiondata);
                            xevent.SetFilesToUpdate(&filestoupdate);
                            xevent.SetIndexOfOperation(c);
                            xevent.GetActualUnzipFileName()->Set(xfilecmp->GetName());
                            xevent.SetStatusOperation(true);

                            //if(PostEvent(&xevent)) xevent.WaitForCompleted( timeout);
                            PostEvent(&xevent);
                          }

                          hashCRC32.ResetResult();
                          hashCRC32.Do(xpathcomplete);

                          if(xfilecmp->GetCRC32() != hashCRC32.GetResultCRC32())
                            {
                              status = unzip->DecompressFile(sourceindex, xpath);

                            } else status = true;
                        }

                      sourceindex++;

                      delete xfilecmp;

                    } while(status);

                  unzip->Close();

                  XFILE* file = xfactory->Create_File();
                  if(file)
                    {
                      file->Erase(xpathfile);
                      xfactory->Delete_File(file);
                    }
                }

              delete unzip;
            }
        }

      //-------------------------------------------------------------
    }

  if(status) error = DIOAPPLICATIONUPDATE_ERROR_OK;

  return status;
}



/*-------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE::Download
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/07/2015 16:35:33
//
//  @return       bool :
//
//  @param        url :
//  @param        port :
//  @param        addtargetpath :
//  @param        updateversiondata :
//  @param        timeout :
//  @param        error :
*/
/*-----------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::Download(DIOURL& url, int port, XPATH& addtargetpath, DIOAPPLICATIONUPDATEVERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error)
{
  return Download(url, port, addtargetpath.Get(), updateversiondata, timeout, error);
}



/*-------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE::ChangeFilesFromDownload
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      16/07/2015 14:02:09
//
//  @return       bool :
//
//  @param        updateversiondata :
//  @param        xpathupdate :
//  @param        error :
*/
/*-----------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::ChangeFilesFromDownload(DIOAPPLICATIONUPDATEVERSIONDATA& updateversiondata, XPATH& xpathupdate, DIOAPPLICATIONUPDATE_ERROR& error)
{
  bool status = false;

  DeleteAllFilesToUpload();

  if(GetFilesUpload(xpathupdate))
    {
      XDWORD c = 0;
      while(c  < filestoupdate.GetSize())
        {
          DIOAPPLICATIONUPDATEFILE* updatefile = filestoupdate.Get(c);
          if(updatefile)
            {
              updatefile->GetXPathFile()->DeleteCharacters(0, xpathupdate.GetSize()+1);

              HASHCRC32 CRC32file;
              XPATH     xpath;

              xpath = xpathrootapp;
              xpath.Slash_Add();
              xpath += updatefile->GetXPathFile()->Get();

              CRC32file.Do(xpath);

              if(CRC32file.GetResultCRC32() == updatefile->GetCRC32())
                {
                  filestoupdate.Delete(updatefile);
                  delete updatefile;
                }
               else
                {

                  XSTRING filename;
                  XSTRING fileext;

                  updatefile->GetXPathFile()->GetNamefile(filename);
                  updatefile->GetXPathFile()->GetExt(fileext);

                  if(!filename.Compare(applicationname, true))
                    {
                      //if((!fileext.Compare(__L(".exe") , true)) || (!fileext.Compare(__L(".pdb") , true)))
                      if(!fileext.Compare(__L(".exe") , true))
                        {
                          updatefile->SetHasSpecialControlUpdate(true);
                        }
                    }

                  c++;
                }
            }
        }

      if(filestoupdate.GetSize())
        {
           RenameOriginalFilesToBackup(xpathupdate, error);
           if(CopyOriginalFilesFromUpdate(xpathupdate, error))
             {
               DeleteFilesBackup(error);
               status = true;
             }

        } else status = true;
    }

  DeleteAllFilesToUpload();

  return status;
}



/*-------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE::RestartApplication
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      16/07/2015 14:02:22
//
//  @return       bool :
//
//  @param        error :
*/
/*-----------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::RestartApplication(DIOAPPLICATIONUPDATE_ERROR& error, bool specialexecution)
{
  XSYSTEM* xsystem = xfactory->CreateSystem();
  if(!xsystem) return  false;

  XSTRING command;
  XSTRING _applicationname;
  int     returncode = 0;

  _applicationname = applicationname.Get();
  _applicationname.ToLowerCase();

  command.Format(__L("%s%s"), xpathrootapp.Get(), _applicationname.Get());

  #ifdef WINDOWS
  command += __L(".exe");
  #endif

  xsystem->ExecuteApplication(command.Get(), NULL, specialexecution);

  xfactory->DeleteSystem(xsystem);


  return true;
}




/*-------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE::RestoreUpdate
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      16/07/2015 16:23:25
//
//  @return       bool :
//
//  @param        error :
*/
/*-----------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::RestoreUpdate(DIOAPPLICATIONUPDATE_ERROR& error)
{
  error = DIOAPPLICATIONUPDATE_ERROR_OK;

  DeleteAllFilesToUpload();

  GetFilesUpload(xpathrootapp);

  XFILE* xfile = xfactory->Create_File();
  if(!xfile)  return false;

  bool   status = false;
  XDWORD c      = 0;

  while(c  < filestoupdate.GetSize())
    {
      DIOAPPLICATIONUPDATEFILE* updatefile = filestoupdate.Get(c);
      if(updatefile)
        {
          XPATH namefileext;

          namefileext = updatefile->GetXPathFile()->Get();
          namefileext.SetOnlyNamefileExt();

          if(!namefileext.Find(DIOAPPLICATIONUPDATE_BACKUPPREFIX, false))
            {
              XPATH xpathoriginal;

              xpathoriginal = updatefile->GetXPathFile()->Get();
              int index = xpathoriginal.Find(DIOAPPLICATIONUPDATE_BACKUPPREFIX, false);
              if(index != XSTRING_NOTFOUND)
                {
                  XSTRING backupprefix;

                  backupprefix = DIOAPPLICATIONUPDATE_BACKUPPREFIX;
                  xpathoriginal.DeleteCharacters(index, backupprefix.GetSize());

                  xfile->Erase(xpathoriginal);

                  status = xfile->Rename((*updatefile->GetXPathFile()), xpathoriginal);
                  if(!status) break;
                }
            }

           c++;
        }
    }

  xfactory->Delete_File(xfile);

  DeleteAllFilesToUpload();

  return status;
}






/*-------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE::IsNewVersion
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/06/2015 17:45:30
//
//  @return       bool :
//
//  @param        updateversiondata :
*/
/*-----------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::IsNewVersion(DIOAPPLICATIONUPDATEVERSIONDATA& updateversiondata)
{
  if(updateversiondata.GetVersion() > applicationversion) return true;
  if(updateversiondata.GetVersion() < applicationversion) return false;

  if(updateversiondata.GetVersion() == applicationversion)
    {
       if(updateversiondata.GetSubVersion() > applicationsubversion)  return true;
       if(updateversiondata.GetSubVersion() < applicationsubversion)  return false;

       if(updateversiondata.GetSubVersion() == applicationsubversion)
         {
           if(updateversiondata.GetErrorControl() > applicationerrorcontrol)  return true;
           if(updateversiondata.GetErrorControl() < applicationerrorcontrol)  return false;

           if(updateversiondata.GetErrorControl() == applicationerrorcontrol)  return false;
         }
    }

  return false;
}






//-------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE::DeleteFilesToUpload
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/02/2007 23:48:40
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool DIOAPPLICATIONUPDATE::DeleteAllFilesToUpload()
{
  if(filestoupdate.IsEmpty()) return false;

  filestoupdate.DeleteContents();
  filestoupdate.DeleteAll();

  return true;
}





/*-------------------------------------------------------------------
//  GetFilesDownload
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/07/2015 15:49:30
//
//  @return       bool :
//
//  @param        xpathupdate :
//  @param        error :
*/
/*-----------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::GetFilesUpload(XPATH& xpath)
{
  XDIRELEMENT xdirelement;
  XDIR*       xdir;
  bool        status = false;

  xdir = xfactory->Create_Dir();
  if(!xdir) return false;

  if(xdir->FirstSearch(xpath, __L("*") , &xdirelement))
    {
      XPATH xpathadd;

      do{ xpathadd   = xpath;
          xpathadd.Slash_Add();
          xpathadd  += xdirelement.GetNameFile()->Get();

          if(xdirelement.GetType() == XDIRELEMENTTYPE_FILE)
            {
              HASHCRC32 CRC32file;
              XDWORD    sizefile = 0;

              CRC32file.Do(xpathadd);

              DIOAPPLICATIONUPDATEFILE* updatefile = new DIOAPPLICATIONUPDATEFILE(xpathadd, CRC32file.GetResultCRC32(), sizefile);
              if(updatefile) filestoupdate.Add(updatefile);
            }

          if(xdirelement.GetType() == XDIRELEMENTTYPE_DIR)
            {
              if(!GetFilesUpload(xpathadd))
                {
                  status = false;
                  break;
                }
            }

        } while(xdir->NextSearch(&xdirelement));
    }

  xfactory->Delete_Dir(xdir);

  return true;
}



/*-------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE::RenameOriginalFilesToBackup
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      16/07/2015 14:02:54
//
//  @return       bool :
//
//  @param        xpathupdate :
//  @param        error :
*/
/*-----------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::RenameOriginalFilesToBackup(XPATH& xpathupdate, DIOAPPLICATIONUPDATE_ERROR& error)
{
  error  = DIOAPPLICATIONUPDATE_ERROR_OK;

  if(!filestoupdate.GetSize()) return false;

  XFILE* xfileorigin = xfactory->Create_File();
  if(!xfileorigin)  return false;

  for(XDWORD c=0; c<filestoupdate.GetSize(); c++)
    {
      XPATH xpathupdateadd;
      XPATH xpathrootappadd;

      xpathupdateadd   = xpathupdate;
      xpathupdateadd.Slash_Add();
      xpathupdateadd  += filestoupdate.Get(c)->GetXPathFile()->Get();

      xpathrootappadd  = xpathrootapp;
      xpathrootappadd.Slash_Add();
      xpathrootappadd += filestoupdate.Get(c)->GetXPathFile()->Get();

      XPATH    originxpathrenamed;
      XSTRING  originfilename;
      XSTRING  originfilenamerenamed;
      XPATH    xpath[2];

      originxpathrenamed    = xpathrootappadd;
      originxpathrenamed.SetOnlyDriveAndPath();

      xpathrootappadd.GetNamefileExt(originfilename);

      originfilenamerenamed  = DIOAPPLICATIONUPDATE_BACKUPPREFIX;
      originfilenamerenamed += originfilename.Get();

      xpath[0] = originxpathrenamed;  xpath[0] += originfilename;
      xpath[1] = originxpathrenamed;  xpath[1] += originfilenamerenamed;

      xfileorigin->Erase(xpath[1]);

      bool status = xfileorigin->Rename(xpath[0], xpath[1]);

      { DIOAPPLICATIONUPDATEXEVENT xevent(this, DIOAPPLICATIONUPDATEXEVENTTYPE_BACKUPORIGINALFILE);

        xevent.SetFilesToUpdate(&filestoupdate);
        xevent.SetIndexOfOperation(c);
        xevent.SetStatusOperation(status);

        //if(PostEvent(&xevent)) xevent.WaitForCompleted( 5);
        PostEvent(&xevent);
      }

    }

  xfactory->Delete_File(xfileorigin);

  return true;
}



/*-------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE::CopyOriginalFilesFromUpdate
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      16/07/2015 14:03:05
//
//  @return       bool :
//
//  @param        xpathupdate :
//  @param        error :
*/
/*-----------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::CopyOriginalFilesFromUpdate(XPATH& xpathupdate,  DIOAPPLICATIONUPDATE_ERROR& error)
{
  error  = DIOAPPLICATIONUPDATE_ERROR_OK;

  if(!filestoupdate.GetSize()) return false;

  bool status = false;

  for(XDWORD c=0; c<filestoupdate.GetSize(); c++)
    {
      XPATH   xpathupdateadd;
      XPATH   xpathrootappadd;
      XSTRING drivepath;


      xpathupdateadd   = xpathupdate;
      xpathupdateadd.Slash_Add();
      xpathupdateadd  += filestoupdate.Get(c)->GetXPathFile()->Get();

      xpathrootappadd  = xpathrootapp;
      xpathrootappadd.Slash_Add();
      xpathrootappadd += filestoupdate.Get(c)->GetXPathFile()->Get();

      xpathrootappadd.GetDriveAndPath(drivepath);

      XDIR* xdir;
      xdir = xfactory->Create_Dir();
      if(xdir)
        {
          xdir->Make(drivepath.Get());
          xfactory->Delete_Dir(xdir);
        }

      status = CopyFile(xpathupdateadd, xpathrootappadd);

      { DIOAPPLICATIONUPDATEXEVENT xevent(this, DIOAPPLICATIONUPDATEXEVENTTYPE_COPYUPDATEFILE);

        xevent.SetFilesToUpdate(&filestoupdate);
        xevent.SetIndexOfOperation(c);
        xevent.SetStatusOperation(status);

        //if(PostEvent(&xevent)) xevent.WaitForCompleted( 5);
        PostEvent(&xevent);
      }

      if(!status)
        {
          error  = DIOAPPLICATIONUPDATE_ERROR_COPYORIGINALFILE;
          break;
        }
    }

  return status;
}




/*-------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE::DeleteFilesBackup
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      16/07/2015 14:03:17
//
//  @return       bool :
//
//  @param        error :
*/
/*-----------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::DeleteFilesBackup(DIOAPPLICATIONUPDATE_ERROR& error)
{
  error  = DIOAPPLICATIONUPDATE_ERROR_OK;

  if(!filestoupdate.GetSize()) return false;

  bool status = false;

  XFILE* xfileorigin = xfactory->Create_File();
  if(!xfileorigin)  return false;

  for(XDWORD c=0; c<filestoupdate.GetSize(); c++)
    {
      XPATH xpathrootappadd;

      if(!filestoupdate.Get(c)->HasSpecialControlUpdate())
        {
          xpathrootappadd  = xpathrootapp;
          xpathrootappadd.Slash_Add();
          xpathrootappadd += filestoupdate.Get(c)->GetXPathFile()->Get();

          XPATH    originxpathrenamed;
          XSTRING  originfilename;
          XSTRING  originfilenamerenamed;
          XPATH    xpath;

          originxpathrenamed    = xpathrootappadd;
          originxpathrenamed.SetOnlyDriveAndPath();

          xpathrootappadd.GetNamefileExt(originfilename);

          originfilenamerenamed  = DIOAPPLICATIONUPDATE_BACKUPPREFIX;
          originfilenamerenamed += originfilename.Get();

          xpath  = originxpathrenamed;
          xpath += originfilenamerenamed;

          status = xfileorigin->Erase(xpath);

          { DIOAPPLICATIONUPDATEXEVENT xevent(this, DIOAPPLICATIONUPDATEXEVENTTYPE_DELETEBACKUPORIGINALFILE);

            xevent.SetFilesToUpdate(&filestoupdate);
            xevent.SetIndexOfOperation(c);
            xevent.SetStatusOperation(status);

            //if(PostEvent(&xevent)) xevent.WaitForCompleted( 5);
            PostEvent(&xevent);
          }

          if(!status) break;

        } else status = true;

    }

  xfactory->Delete_File(xfileorigin);

  return status;
}





/*-------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE::CopyFile
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/07/2015 13:19:51
//
//  @return       bool :
//
//  @param        origin :
//  @param        target :
*/
/*-----------------------------------------------------------------*/
bool DIOAPPLICATIONUPDATE::CopyFile(XPATH& origin, XPATH& target)
{
  XFILE* xfile[2] = { NULL, NULL };
  bool   status   = false;

  xfile[0] = xfactory->Create_File();
  xfile[1] = xfactory->Create_File();

  if(xfile[0] && xfile[1])
    {
      if(xfile[0]->Open(origin))
        {
          if(xfile[1]->Create(target))
            {
              #define COPYFILEBLOCKSIZE  500*1024

              XBUFFER xbuffer;
              XDWORD  br = COPYFILEBLOCKSIZE;

              xbuffer.Resize(COPYFILEBLOCKSIZE);

              do{
                  xfile[0]->Read(xbuffer.Get(), &br);
                  if(!br) break;

                  if(!xfile[1]->Write(xbuffer.Get(), br)) break;

                } while(br == COPYFILEBLOCKSIZE);


              xfile[1]->Close();

              status = true;
            }

          xfile[0]->Close();
        }
    }

  xfactory->Delete_File(xfile[0]);
  xfactory->Delete_File(xfile[1]);

  if(status)
    {
      status = false;

      HASHCRC32* CRC32file[2];

      CRC32file[0] = new HASHCRC32();
      CRC32file[1] = new HASHCRC32();

      if(CRC32file[0] && CRC32file[1])
        {
          CRC32file[0]->Do(origin);
          CRC32file[1]->Do(target);

          if(CRC32file[0]->GetResultCRC32() == CRC32file[1]->GetResultCRC32()) status = true;
        }

      delete CRC32file[0];
      delete CRC32file[1];
    }

  return status;
}