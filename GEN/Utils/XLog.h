//------------------------------------------------------------------------------------------
//  XLOG.H
//
/**
// \class
//
//  Debug Funtions
//
//  @author  Abraham J. Velez
//  @version 05/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XLOG_H_
#define _XLOG_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XPath.h"
#include "XBuffer.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum XLOGLEVEL
{
  XLOGLEVEL_INFO          = 0x01  ,
  XLOGLEVEL_WARNING       = 0x02  ,
  XLOGLEVEL_ERROR         = 0x04  ,

  XLOGLEVEL_ALL           = 0x0F
};


enum XLOGTYPELIMIT
{
  XLOGTYPELIMIT_NONE      = 0 ,
  XLOGTYPELIMIT_NENTRYS       ,
  XLOGTYPELIMIT_SIZE
};

#define XLOG_DEFAULTMAXFILESBACKUP      10       // Percent to reduce default
#define XLOG_DEFAULTMAXNENTRYS          3000     // Max default number of entries
#define XLOG_DEFAULTMAXSIZE             50000    // Max default size
#define XLOG_DEFAULTREDUCTIONPERCENT    50       // Percent to reduce default

#define XLOG_MAXLINESTOFLUSH            1000     // Number lines to Flush

#define XLOG_PREFIX                     __L("[LOG]_")
#define XLOG_CREATEMASKBACKUPFILE       __L("[LOG]_%04d_%02d_%02d-%02d_%02d_%02d_")
#define XLOG_SEARCHMASKBACKUPFILE       __L("[LOG]_*-*")

#define XLOG_MAXDATETIMESIZE            20
#define XLOG_MAXLEVELSIZE                8
#define XLOG_MAXSECTIONIDSIZE           10

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XMUTEX;
class XDATETIME;
class XTIMER;
class XFILETXT;
class XTHREADCOLLECTED;



class XLOGENTRY
{
  public:

                            XLOGENTRY                         ()                                                                    { Clean();             }
    virtual                ~XLOGENTRY                         ()                                                                    { Clean();             }


    void                    Clean                             ()
                            {
                              position  = 0;
                              size      = 0;
                            }

    XDWORD                  position;
    XDWORD                  size;
};





class XLOG
{
  public:

    static bool             GetIsInstanced                    ()
                            {
                              return instance!=NULL;
                            }

    static XLOG&            GetInstance                       ()
                            {
                              if(!instance) instance = new XLOG();

                              return (*instance);
                            }

    static bool             DelInstance                       ()
                            {
                              if(instance)
                                {
                                  delete instance;
                                  instance = NULL;

                                  return true;
                                }

                              return false;
                            }

    bool                    Ini                               (XCHAR* pathfile, XCHAR* nameapplication = NULL, bool isdumpindebug = true);
    bool                    Ini                               (XPATH& xpathfile, XCHAR* nameapplication = NULL, bool isdumpindebug = true);
    bool                    Ini                               (XPATH& xpathfile, XSTRING& nameapplication, bool isdumpindebug = true);

    bool                    SetLimit                          (XLOGTYPELIMIT typelimit, int limit =  XLOG_DEFAULTMAXNENTRYS, int reductionpercentnentrys = XLOG_DEFAULTREDUCTIONPERCENT);
    bool                    SetBackup                         (bool isactive, int maxfiles = XLOG_DEFAULTMAXFILESBACKUP, bool iscompress = false);
    bool                    SetFilters                        (XCHAR* sectionsID, XBYTE level = XLOGLEVEL_ALL);

    XCHAR*                  GetPathFile                       ()                                                                    { return xpathfile.Get();                 }
    XSTRING*                GetNameApplication                ()                                                                    { return &nameapplication;                }

    bool                    IsActive                          ();

    void                    SetIsActive                       (bool       isactive)                                                 { this->isactive = isactive;              }
    XDWORD                  GetNEntrys                        ()                                                                    { return nentrys;                         }
    XDWORD                  GetNLines                         ()                                                                    { return nlines;                          }
    XDWORD                  GetSize                           ()                                                                    { return size;                            }
    bool                    GetLevelString                    (XLOGLEVEL level, XSTRING& strlevel, XDWORD size);
    XSTRING*                GetSectionsIDFilter               ();
    bool                    PassFilters                       (XLOGLEVEL level, XCHAR* sectionID);

    bool                    AddEntry                          (XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XCHAR*   mask, ...);
    bool                    AddEntry                          (XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XBYTE*   data, XDWORD size,           XDWORD sizeline = 16,       bool showoffset  = true, bool showtext = true);
    bool                    AddEntry                          (XLOGLEVEL level, XCHAR* sectionID, bool inmemory, XBUFFER& data, XDWORD sizeline = 16,  bool showoffset = true,  bool showtext    = true);

    bool                    Flush                             ();

    bool                    End                               (bool      doflush = true);

  protected:

    bool                    CalculateInitialStatus            ();

    bool                    FlushMemoryEntrys                 ();

    int                     Backup_GetNFiles                  ();
    bool                    Backup_GetNameFileMoreOld         (XPATH& xpath);
    bool                    Backup_CreateNameZIP              (XPATH& xpathzipfile);
    bool                    Backup_AdjustNFilesInCompressed   ();
    bool                    Backup_AdjustNFiles               (XCHAR* pathnamelog, bool iscompress);
    bool                    Backup_ControlLimits              ();

    XDATETIME*              xdatetime;
    XTIMER*                 xtimer;
    XMUTEX*                 mutex;
    XFILETXT*               filelog;

    XPATH                   xpathfile;
    XSTRING                 nameapplication;
    bool                    isdumpindebug;

    bool                    isactive;
    XLOGTYPELIMIT           typelimit;

    bool                    backupisactive;
    int                     backupmaxfiles;
    bool                    backupiscompress;
    XPATH                   backupxpathnamelog;

    XDWORD                  limit;
    XDWORD                  reductionpercent;

    XSTRING                 lastsectionID;

    XSTRING                 filtersectionsID;
    XBYTE                   filterlevel;

    XDWORD                  nentrys;
    XDWORD                  nlines;
    XDWORD                  size;
    XVECTOR<XLOGENTRY*>     entrys;
    XLOGENTRY*              lastentry;

  private:
                            XLOG                              ();
                            XLOG                              (XLOG const&);        // Don't implement
    virtual                ~XLOG                              ();

    void                    operator =                        (XLOG const&);        // Don't implement

    void                    Clean                             ()
                            {
                              xdatetime        = NULL;
                              xtimer           = NULL;
                              mutex            = NULL;
                              filelog          = NULL;

                              isdumpindebug    = false;

                              isactive         = false;
                              typelimit        = XLOGTYPELIMIT_NONE;

                              backupisactive   = false;
                              backupmaxfiles   = 0;
                              backupiscompress = false;

                              limit            = 0;
                              reductionpercent = XLOG_DEFAULTREDUCTIONPERCENT;

                              lastsectionID.Empty();

                              filtersectionsID.Empty();
                              filterlevel      = XLOGLEVEL_ALL;

                              nentrys          = 0;
                              nlines           = 0;
                              size             = 0;
                              lastentry        = NULL;
                            }

    static XLOG*            instance;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif