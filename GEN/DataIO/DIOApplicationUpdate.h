/*------------------------------------------------------------------------------------------
//  DIOAPPLICATIONUPDATE.H
*/
/**
// \class
//
//  Update Functions
//
//  @author  Abraham J. Velez
//  @version 26/02/2009 10:39:12
*/
/*  GEN (C) Copyright  (All right reserved).
//----------------------------------------------------------------------------------------*/

#ifndef _DIOAPPLICATIONUPDATE_H_
#define _DIOAPPLICATIONUPDATE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOURL.h"
#include "XPath.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


enum DIOAPPLICATIONUPDATEFUNC_STATUS
{
  DIOAPPLICATIONUPDATEFUNC_STATUS_UNKNOW                = 0 ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_MAKEDIR                   ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEUNKNOW             ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEREQUEST            ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEOPEN               ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILECONNECT            ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEGETINFO            ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEGETFILE            ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEWAIT               ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEEND                ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_GETFILEERROR              ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_CHANGEFILEUPLOAD          ,
  DIOAPPLICATIONUPDATEFUNC_STATUS_ERASEFILEUPLOAD           ,
};


enum DIOAPPLICATIONUPDATE_ERROR
{
  DIOAPPLICATIONUPDATE_ERROR_OK                         = 0 ,
  DIOAPPLICATIONUPDATE_ERROR_NOTFILESTOUPDATE               ,
  DIOAPPLICATIONUPDATE_ERROR_INVALIDOPERATION               ,
  DIOAPPLICATIONUPDATE_ERROR_BREAKFORUSER                   ,
  DIOAPPLICATIONUPDATE_ERROR_MAKEDIR                        ,
  DIOAPPLICATIONUPDATE_ERROR_DOWNLOADFILE                   ,
  DIOAPPLICATIONUPDATE_ERROR_RENAMEORIGINALFILE             ,
  DIOAPPLICATIONUPDATE_ERROR_COPYORIGINALFILE               ,
  DIOAPPLICATIONUPDATE_ERROR_SPECIALORIGINALFILE            ,
};


#define DIOAPPLICATIONUPDATE_INIFILE                                __L("update.ini")
#define DIOAPPLICATIONUPDATE_BACKUPPREFIX                           __L("BAK_")

#define DIOAPPLICATIONUPDATE_PORTDEFAULT                            80
#define DIOAPPLICATIONUPDATE_TIMEOUT                                60

#define DIOAPPLICATIONUPDATE_MAXINDEXFILES                          10000

#define DIOAPPLICATIONUPDATE_GENERALSECTION                         __L("general")
#define DIOAPPLICATIONUPDATE_GENERALSECTION_VERSION                 __L("version")
#define DIOAPPLICATIONUPDATE_GENERALSECTION_SUBVERSION              __L("subversion")
#define DIOAPPLICATIONUPDATE_GENERALSECTION_ERRORCONTROL            __L("errorcontrol")
#define DIOAPPLICATIONUPDATE_GENERALSECTION_SYSTEMMUSTBEINIT        __L("systemmustbeinit")

#define DIOAPPLICATIONUPDATE_DIRSECTION                             __L("directorys")
#define DIOAPPLICATIONUPDATE_FILESSECTION                           __L("files")
#define DIOAPPLICATIONUPDATE_FILESSECTION_FILES                     __L("file")

#define DIOAPPLICATIONUPDATE_LOGSECTIONIDAPPUPDATE                  __L("AppUpdate")


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XDIR;
class DIOFACTORY;
class DIOWEBCLIENT;


class DIOAPPLICATIONUPDATEVERSIONDATA
{
  public:
                                          DIOAPPLICATIONUPDATEVERSIONDATA   ()                              { Clean();                                              }
    virtual                              ~DIOAPPLICATIONUPDATEVERSIONDATA   ()                              { Clean();                                              }

    XDWORD                                GetVersion                        ()                              { return version;                                       }
    void                                  SetVersion                        (XDWORD version)                { this->version = version;                              }

    XDWORD                                GetSubVersion                     ()                              { return subversion;                                    }
    void                                  SetSubVersion                     (XDWORD subversion)             { this->subversion = subversion;                        }

    XDWORD                                GetErrorControl                   ()                              { return errorcontrol;                                  }
    void                                  SetErrorControl                   (XDWORD errorcontrol)           { this->errorcontrol = errorcontrol;                    }

    bool                                  SystemMustBeInit                  ()                              { return systemmustbeinit;                              }
    void                                  SetSystemMustBeInit               (bool applicationmustbeinit)    { this->systemmustbeinit = applicationmustbeinit;       }


    void                                  Clean                             ()
                                          {
                                            version                 = 0;
                                            subversion              = 0;
                                            errorcontrol            = 0;
                                            systemmustbeinit        = false;
                                          }
  private:

    XDWORD                                version;
    XDWORD                                subversion;
    XDWORD                                errorcontrol;
    bool                                  systemmustbeinit;
};





class DIOAPPLICATIONUPDATEFILE
{
  public:
                                          DIOAPPLICATIONUPDATEFILE          (XPATH& xpathfile, XDWORD CRC32, int size)
                                          {
                                            Clean();

                                            this->xpathfile = xpathfile;
                                            this->CRC32     = CRC32;
                                            this->size      = size;
                                          }

    virtual                              ~DIOAPPLICATIONUPDATEFILE          ()
                                          {
                                            Clean();
                                          }

    XPATH*                                GetXPathFile                      ()                              { return &xpathfile;                                  }
    XDWORD                                GetCRC32                          ()                              { return CRC32;                                       }
    int                                   GetSize                           ()                              { return size;                                        }
    bool                                  HasSpecialControlUpdate           ()                              { return specialcontrolupdate;                        }
    void                                  SetHasSpecialControlUpdate        (bool specialcontrolupdate)     { this->specialcontrolupdate = specialcontrolupdate;  }

  private:

    void                                  Clean                             ()
                                          {
                                            CRC32                 = 0;
                                            size                  = 0;
                                            specialcontrolupdate  = false;
                                          }

    XPATH                                 xpathfile;
    XDWORD                                CRC32;
    int                                   size;
    bool                                  specialcontrolupdate;
};




class DIOAPPLICATIONUPDATE : public XSUBJECT
{
  public:
                                          DIOAPPLICATIONUPDATE            (XDWORD applicationversion, XDWORD applicationsubversion, XDWORD applicationerrorcontrol, XCHAR* applicationname, XPATH& xpathrootapp);
    virtual                              ~DIOAPPLICATIONUPDATE            ();

    bool                                  GetVersionAvailable             (DIOURL& url, int port, DIOAPPLICATIONUPDATEVERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error);

    bool                                  Download                        (DIOURL& url, int port, XCHAR* addtargetpath, DIOAPPLICATIONUPDATEVERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error);
    bool                                  Download                        (DIOURL& url, int port, XPATH& addtargetpath, DIOAPPLICATIONUPDATEVERSIONDATA& updateversiondata, int timeout, DIOAPPLICATIONUPDATE_ERROR& error);
    bool                                  ChangeFilesFromDownload         (DIOAPPLICATIONUPDATEVERSIONDATA& updateversiondata, XPATH& xpathupdate, DIOAPPLICATIONUPDATE_ERROR& error);
    bool                                  RestartApplication              (DIOAPPLICATIONUPDATE_ERROR& error, bool specialexecution = true);
    bool                                  RestoreUpdate                   (DIOAPPLICATIONUPDATE_ERROR& error);

    bool                                  IsNewVersion                    (DIOAPPLICATIONUPDATEVERSIONDATA& updateversiondata);

    XDWORD                                GetApplicationVersion           ()                                { return this->applicationversion;                  }
    XDWORD                                GetApplicationSubversion        ()                                { return this->applicationsubversion;               }
    XDWORD                                GetApplicationErrorControl      ()                                { return this->applicationerrorcontrol;             }

  private:

    void                                  Clean                           ()
                                          {
                                            xdir                      = NULL;
                                            webclient                 = NULL;

                                            applicationversion        = 0;
                                            applicationsubversion     = 0;
                                            applicationerrorcontrol   = 0;

                                            progressfiletoupdate      = -1;
                                            stopproccess              = false;
                                          }


    bool                                  DeleteAllDirToCreate            ();
    bool                                  DeleteAllFilesToUpload          ();

    bool                                  GetFilesUpload                  (XPATH& xpath);

    bool                                  RenameOriginalFilesToBackup     (XPATH& xpathupdate, DIOAPPLICATIONUPDATE_ERROR& error);
    bool                                  CopyOriginalFilesFromUpdate     (XPATH& xpathupdate, DIOAPPLICATIONUPDATE_ERROR& error);
    bool                                  DeleteFilesBackup               (DIOAPPLICATIONUPDATE_ERROR& error);


    bool                                  CopyFile                        (XPATH& origin, XPATH& target);


    XSTRING                               applicationname;
    XPATH                                 xpathrootapp;

    XDIR*                                 xdir;
    DIOWEBCLIENT*                         webclient;

    XDWORD                                applicationversion;
    XDWORD                                applicationsubversion;
    XDWORD                                applicationerrorcontrol;

    XVECTOR<DIOAPPLICATIONUPDATEFILE*>    filestoupdate;

    int                                   progressfiletoupdate;
    bool                                  stopproccess;
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/


#endif



