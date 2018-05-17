//------------------------------------------------------------------------------------------
//  DIOPROTOCOL.H
//
/**
// \class
//
//  eXtended class for generic protocols
//
//  @author  Abraham J. Velez
//  @version 13/04/2006 11:41:55
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOPROTOCOL_H_
#define _DIOPROTOCOL_H_


//----INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XDir.h"
#include "XPath.h"
#include "XEvent.h"
#include "XSubject.h"
#include "XFactory.h"
#include "XBuffer.h"

#include "DIOStream.h"
#ifdef DIOALERTS_ACTIVE
#include "DIOAlerts.h"
#endif



//---- DEFINES & ENUMS  --------------------------------------------------------------------




enum DIOPROTOCOL_CMDTYPE
{
  DIOPROTOCOL_CMDTYPE_NONE                        =   00 ,
  DIOPROTOCOL_CMDTYPE_GETPROTOCOLVERSION          =   10 ,
  DIOPROTOCOL_CMDTYPE_GETAPPLICATIONVERSION       =   20 ,
  DIOPROTOCOL_CMDTYPE_PING                        =   30 ,
  DIOPROTOCOL_CMDTYPE_SENDISINITSERVERPROTOCOL    =   40 ,
  DIOPROTOCOL_CMDTYPE_GETDATETIME                 =   50 ,
  DIOPROTOCOL_CMDTYPE_SETDATETIME                 =   60 ,
  DIOPROTOCOL_CMDTYPE_INITFILEBLOCK               =   70 ,
  DIOPROTOCOL_CMDTYPE_GETFILEBLOCK                =   80 ,
  DIOPROTOCOL_CMDTYPE_SENDFILEBLOCK               =   90 ,
  DIOPROTOCOL_CMDTYPE_DELETEINACTIVEFILEPROGRESS  =  100 ,
  DIOPROTOCOL_CMDTYPE_DELETEFILE                  =  110 ,
  DIOPROTOCOL_CMDTYPE_MAKEDIR                     =  120 ,
  DIOPROTOCOL_CMDTYPE_DELETEDIR                   =  130 ,
  DIOPROTOCOL_CMDTYPE_GETFIRSTDIRELEMENT          =  140 ,
  DIOPROTOCOL_CMDTYPE_GETNEXTDIRELEMENT           =  150 ,
  DIOPROTOCOL_CMDTYPE_SENDALERT                   =  160 ,

  DIOPROTOCOL_CMDTYPE_END                         = 1000 ,
};



enum DIOPROTOCOL_DIRECTION
{
  DIOPROTOCOL_DIRECTION_UNKNOWN                 = 0 ,
  DIOPROTOCOL_DIRECTION_GET                         ,
  DIOPROTOCOL_DIRECTION_SEND                        ,
};

enum DIOPROTOCOL_RESULT
{
  DIOPROTOCOL_RESULT_OK                         = 0 ,
  DIOPROTOCOL_RESULT_UNKNOWNCMD                     ,
  DIOPROTOCOL_RESULT_NOTMEM                         ,
  DIOPROTOCOL_RESULT_NOTFILE                        ,
  DIOPROTOCOL_RESULT_NOTCONNEX                      ,
  DIOPROTOCOL_RESULT_ERRORCMD                       ,
};


enum DIOPROTOCOL_FILESTATUS
{
  DIOPROTOCOL_FILESTATUS_UNKNOWN                = 0 ,
  DIOPROTOCOL_FILESTATUS_REQUEST                    ,
  DIOPROTOCOL_FILESTATUS_WAIT                       ,
  DIOPROTOCOL_FILESTATUS_END                        ,
  DIOPROTOCOL_FILESTATUS_ERROR                      ,
};


enum DIOPROTOCOLXEVENTTYPE
{
  DIOPROTOCOLXEVENTTYPE_UNKNOWN         = XEVENTTYPE_PROTOCOL ,
  DIOPROTOCOLXEVENTTYPE_COMMAND                               ,
  DIOPROTOCOLXEVENTTYPE_FILEBLOCK                             ,
  DIOPROTOCOLXEVENTTYPE_ADDALERT                              ,
};


#define DIOPROTOCOL_MSGIDHEAD                   0xAAA00555
#define DIOPROTOCOL_CMDTYPE_ANSWER              0x80000000
#define DIOPROTOCOL_CMDTYPE_ISSERVER            0x04000000
#define DIOPROTOCOL_CMDTYPE_SECUENCIALLIMIT     0x0000FFFF

#define DIOPROTOCOL_MAXWATCHDOG                 3
#define DIOPROTOCOL_TIMEOUT                     5
#define DIOPROTOCOL_SIZEOFHEAD                  (sizeof(XDWORD)*6)

#define DIOPROTOCOL_MAXBUFFER                   (DIOSTREAM_MAXBUFFER/2)

#define DIOPROTOCOL_TIMERPING                   60

#define DIOPROTOCOL_MAXFILEBLOCK                1024  //(DIOPROTOCOL_MAXBUFFER/2)

#define DIOPROTOCOL_TIMEFILEDELETE              0x00000100
#define DIOPROTOCOL_TIMEOUTFILEBLOCK            60

#define DIOPROTOCOL_MAXDESCRIPTIONSTR           64

#define DIOPROTOCOL_VERSION                     0
#define DIOPROTOCOL_SUBVERSION                  1
#define DIOPROTOCOL_SUBVERSIONERR               2

#define DIOPROTOCOL_ERRORRECEIVED              -1


class DIOPROTOCOL_COMMAND;
class DIOPROTOCOL;
typedef int (*DIOPROTOCOL_RECEIVEDFUNC)   (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& buffer, XDWORD rID, XDWORD& param);


//---- CLASS -------------------------------------------------------------------------------

class XBUFFER;
class XRAND;
class XDATETIME;
class XTIMER;
class XSYSTEM;
class XDIR;
class XDIRELEMENT;
class XFILE;
class XFACTORY;
class XPUBLISHER;
class XTHREADCOLLECTED;
class XMUTEX;
class HASHCRC32;
class CIPHER;
class DIOFACTORY;
class DIOPROTOCOL;
class DIOALERT;


class DIOPROTOCOL_ANSWER
{
  public:
                                              DIOPROTOCOL_ANSWER              (XDWORD type, XDWORD ID, XBUFFER& xbuffer)
                                              {
                                                Clean();

                                                this->type    = type;
                                                this->ID      = ID;

                                                if(!xbuffer.IsEmpty()) this->xbuffer.Add(xbuffer);

                                              }

                                             ~DIOPROTOCOL_ANSWER              ()
                                              {
                                                Clean();
                                              }


    XDWORD                                    GetType                         ()                                        { return type;                                                }
    XDWORD                                    GetID                           ()                                        { return ID;                                                  }
    XBUFFER&                                  GetBuffer                       ()                                        { return xbuffer;                                             }

  private:

    void                                      Clean                           ()
                                              {
                                                type      = DIOPROTOCOL_CMDTYPE_NONE;
                                                ID        = 0;
                                              }

    XDWORD                                    type;
    XDWORD                                    ID;
    XBUFFER                                   xbuffer;
};



class DIOPROTOCOL_COMMAND
{
  public:
                                              DIOPROTOCOL_COMMAND             (XDWORD type, XCHAR* inmask, XCHAR* outmask, DIOPROTOCOL_RECEIVEDFUNC receivedfunc, XCHAR* description)
                                              {
                                                Clean();

                                                this->type    = type;
                                                this->inmask  = inmask;
                                                this->outmask = outmask;

                                                if(receivedfunc)  this->receivedfunc  = receivedfunc;
                                                if(description)   this->description   = description;
                                              }

                                             ~DIOPROTOCOL_COMMAND             ()                                        { Clean();                                                    }

    XDWORD                                    GetType                         ()                                        { return type;                                                }
    XCHAR*                                    GetINMask                       ()                                        { return inmask.Get();                                        }
    XCHAR*                                    GetOUTMask                      ()                                        { return outmask.Get();                                       }

    DIOPROTOCOL_RECEIVEDFUNC                  GetReceivedFunc                 ()                                        { return receivedfunc;                                        }
    XCHAR*                                    GetDescription                  ()                                        { return description.Get();                                   }

    XSTRING*                                  GetSINMask                      ()                                        { return &inmask;                                             }
    XSTRING*                                  GetSOUTMask                     ()                                        { return &outmask;                                            }

    XSTRING*                                  GetSDescription                 ()                                        { return &description;                                        }

  private:

    void                                      Clean                           ()
                                              {
                                                type         = DIOPROTOCOL_CMDTYPE_NONE;

                                                inmask.Empty();
                                                outmask.Empty();

                                                receivedfunc = NULL;

                                                description.Empty();
                                              }


    XDWORD                                    type;

    XSTRING                                   inmask;
    XSTRING                                   outmask;
    DIOPROTOCOL_RECEIVEDFUNC                  receivedfunc;
    XSTRING                                   description;
};



class DIOPROTOCOL_FILE
{
  public:
                                              DIOPROTOCOL_FILE                ()
                                              {
                                                Clean();

                                                xfile = xfactory->Create_File();

                                              }

                                             ~DIOPROTOCOL_FILE                ()
                                              {
                                                xfactory->Delete_File(xfile);

                                                Clean();
                                              }

    bool                                      IsActive                        ()                                        { return isactive;                                            }
    void                                      SetActive                       (bool isactive)                           { this->isactive = isactive;                                  }

    DIOPROTOCOL_DIRECTION                     GetDirection                    ()                                        { return direction;                                           }
    void                                      SetDirection                    (DIOPROTOCOL_DIRECTION direction)         { this->direction = direction;                                }

    XPATH*                                    GetSourceName                   ()                                        { return &sourcename;                                         }
    bool                                      SetSourceName                   (XCHAR* sourcename)
                                              {
                                                if(!sourcename) return false;
                                                this->sourcename.Set(sourcename);
                                                return true;
                                              }

    bool                                      SetSourceName                   (XSTRING& sourcename)                     { return SetSourceName(sourcename.Get());                     }

    XPATH*                                    GetTargetName                   ()                                        { return &targetname;                                         }
    bool                                      SetTargetName                   (XCHAR* targetname)
                                              {
                                                if(!targetname) return false;
                                                this->targetname.Set(targetname);
                                                return true;
                                              }

    bool                                      SetTargetName                   (XSTRING& targetname)                     { return SetTargetName(targetname.Get());                     }

    XDWORD                                    GetSize                         ()                                        { return size;                                                }
    void                                      SetSize                         (XDWORD size)                             { this->size = size;                                          }


    XDWORD                                    GetCRC32                        ()                                        { return crc32;                                               }
    void                                      SetCRC32                        (XDWORD crc32)                            { this->crc32 = crc32;                                        }

    int                                       GetSizeBlock                    ()                                        { return sizeblock;                                           }
    void                                      SetSizeBlock                    (XDWORD sizeblock)                           { this->sizeblock = sizeblock;                                }

    int                                       GetPosition                     ()                                        { return position;                                            }
    void                                      SetPosition                     (int position)                            { this->position = position;                                  }

    XBYTE                                     GetPercent                      ()                                        { return percent;                                             }
    void                                      SetPercent                      (XBYTE percent)                           { this->percent = percent;                                    }

    bool                                      ReadBlock                       (XBYTE* buffer);
    bool                                      WriteBlock                      (XBYTE* buffer);

    bool                                      Erase                           ();
    void                                      Reset                           ()
                                              {
                                                isactive        = false;
                                                direction       = DIOPROTOCOL_DIRECTION_UNKNOWN;
                                                size            = 0;
                                                sizeblock       = 0;
                                                position        = 0;
                                                percent         = 0;
                                              }



  private:

    XFILE*                                    xfile;

    bool                                      isactive;
    DIOPROTOCOL_DIRECTION                     direction;

    XPATH                                     sourcename;
    XPATH                                     targetname;

    XDWORD                                    size;
    XDWORD                                    crc32;
    XDWORD                                    sizeblock;
    XDWORD                                    position;
    XBYTE                                     percent;

  private:

    void                                      Clean                           ()
                                              {
                                                xfile           = NULL;

                                                isactive        = false;
                                                direction       = DIOPROTOCOL_DIRECTION_UNKNOWN;

                                                size            = 0;
                                                crc32           = 0;
                                                sizeblock       = 0;
                                                position        = 0;
                                                percent         = 0;
                                              }
};



class DIOPROTOCOLXEVENT : public XEVENT
{
  public:
                                              DIOPROTOCOLXEVENT               (XSUBJECT* subject, XDWORD type = DIOPROTOCOLXEVENTTYPE_UNKNOWN, XDWORD family = XEVENTTYPE_PROTOCOL) : XEVENT(subject, type, family)
                                              {
                                                Clean();
                                              }
    virtual                                  ~DIOPROTOCOLXEVENT               ()
                                              {
                                                Clean();
                                              }

    DIOPROTOCOL*                              GetProtocol                     ()                                        { return protocol;                                            }
    bool                                      SetProtocol                     (DIOPROTOCOL* protocol)
                                              {
                                                if(!protocol) return false;
                                                this->protocol = protocol;

                                                return true;
                                              }

    DIOPROTOCOL_DIRECTION                     GetDirection                    ()                                        { return direction;                                           }
    void                                      SetDirection                    (DIOPROTOCOL_DIRECTION direction)         { this->direction = direction;                                }

    XDWORD                                    GetType                         ()                                        { return type;                                                }
    void                                      SetType                         (XDWORD type)                             { this->type = type;                                          }

    bool                                      GetResult                       ()                                        { return result;                                              }
    void                                      SetResult                       (bool result)                             { this->result = result;                                      }

    DIOPROTOCOL_FILESTATUS                    GetFileStatus                   ()                                        { return filestatus;                                          }
    void                                      SetFileStatus                   (DIOPROTOCOL_FILESTATUS filestatus)       { this->filestatus = filestatus;                              }

    bool                                      FileIsSend                      ()                                        { return fileissend;                                          }
    void                                      SetFileIsSend                   (bool fileissend)                         { this->fileissend = fileissend;                              }

    XPATH*                                    GetFileSource                   ()                                        { return &filesource;                                         }
    bool                                      SetFileSource                   (XCHAR* filesource)
                                              {
                                                if(!filesource) return false;
                                                this->filesource = filesource;
                                                return true;
                                              }

    XPATH*                                    GetFileTarget                   ()                                        { return &filetarget;                                         }
    bool                                      SetFileTarget                   (XCHAR* filetarget)
                                              {
                                                if(!filetarget) return false;
                                                this->filetarget = filetarget;
                                                return true;
                                              }

    int                                       GetFilePercent                  ()                                        { return filepercent;                                         }
    void                                      SetFilePercent                  (int filepercent)                         { this->filepercent = filepercent;                            }

    void*                                     GetParam1                       ()                                        { return param1;                                              }
    void                                      SetParam1                       (void* param1)                            { this->param1 = param1;                                      }

    XDWORD                                    GetParam2                       ()                                        { return param2;                                              }
    void                                      SetParam2                       (XDWORD param2)                           { this->param2 = param2;                                      }


    DIOALERT*                                 GetAlert                        ()                                        { return alert;                                               }
    void                                      SetAlert                        (DIOALERT* alert)                         { this->alert = alert;                                        }

  private:

    void                                      Clean                           ()
                                              {
                                                protocol    = NULL;
                                                direction   = DIOPROTOCOL_DIRECTION_UNKNOWN;
                                                type        = 0;
                                                result      = false;

                                                filestatus  = DIOPROTOCOL_FILESTATUS_UNKNOWN;
                                                fileissend  = false;

                                                filesource.Empty();
                                                filetarget.Empty();

                                                filepercent = 0;

                                                param1      = NULL;
                                                param2      = 0;

                                                alert       = NULL;
                                              }

    DIOPROTOCOL*                              protocol;
    DIOPROTOCOL_DIRECTION                     direction;
    XDWORD                                    type;
    bool                                      result;

    DIOPROTOCOL_FILESTATUS                    filestatus;
    bool                                      fileissend;
    XPATH                                     filesource;
    XPATH                                     filetarget;
    int                                       filepercent;

    void*                                     param1;
    XDWORD                                    param2;

    DIOALERT*                                 alert;
};




class DIOPROTOCOL : public XSUBJECT
{
  public:
                                              DIOPROTOCOL                     (DIOSTREAM* diostream);
     virtual                                 ~DIOPROTOCOL                     ();

     XSTRING*                                 GetDescription                  ()                                        { return &description;                                       }

     bool                                     Connect                         ();
     bool                                     Disconnect                      ();

     bool                                     Reset                           ();

     bool                                     IsConnected                     ();
     bool                                     IsConnecting                    ();
     bool                                     IsDisconnected                  ();

     bool                                     IsInitialized                   ()                                        { return isinitialized;                                       }
     void                                     SetIsInitialized                (bool isinitialized)                      { this->isinitialized = isinitialized;                        }

     bool                                     IsServerInitialized             ()                                        { return isserverinitialized;                                 }

     bool                                     IsOperational                   ()
                                              {
                                                if(!IsConnected())    return false;
                                                if(!IsInitialized())  return false;

                                                return true;
                                              }

     XDWORD                                   GetMaskID                       ()                                        { return maskID;                                              }
     bool                                     SetMaskID                       (XDWORD maskID)
                                              {
                                                this->maskID = (maskID&~DIOPROTOCOL_CMDTYPE_ANSWER);
                                                return (this->maskID > 65535)?false:true;
                                              }

     void*                                    GetConnexionsManager            ()                                        { return connexionsmanager;                                   }
     void                                     SetConnexionsManager            (void* connexionsmanager)                 { this->connexionsmanager = connexionsmanager;                }

     void*                                    GetConnexion                    ()                                        { return connexion;                                           }
     void                                     SetConnexion                    (void* connexion)                         { this->connexion = connexion;                                }

     void*                                    GetApplicationData              ()                                        { return applicationdata;                                     }
     void                                     SetApplicationData              (void* applicationdata)                   { this->applicationdata = applicationdata;                    }

     bool                                     IsOwnApplicationData            ()                                        { return isownapplicationdata;                                }
     void                                     SetIsOwnApplicationData         (bool isownapplicationdata)               { this->isownapplicationdata = isownapplicationdata;          }

     DIOSTREAM*                               GetDIOStream                    ()                                        { return diostream;                                           }

     XDATETIME*                               GetDateTime                     ()                                        { return xdatetime;                                           }

     bool                                     AddCommand                      (XDWORD type ,XCHAR* inmask,XCHAR* outmask,DIOPROTOCOL_RECEIVEDFUNC receivedfunc,XCHAR* description);
     DIOPROTOCOL_COMMAND*                     GetCommand                      (XDWORD type);


     DIOPROTOCOL_ANSWER*                      AddAnswer                       (XDWORD type, XDWORD ID, XBUFFER& xbuffer);
     DIOPROTOCOL_ANSWER*                      GetAnswer                       (XDWORD type, XDWORD ID);
     bool                                     DeleteAnswer                    (XDWORD type,XDWORD ID);
     bool                                     DeleteAllAnswers                ();

     DIOPROTOCOL_ANSWER*                      AddAsk                          (XDWORD type, XDWORD ID, XBUFFER& xbuffer);
     DIOPROTOCOL_ANSWER*                      GetAsk                          (XDWORD index);
     bool                                     DeleteAsk                       (XDWORD index);
     bool                                     DeleteAllAsks                   ();

     int                                      ReadCommand                     (XDWORD& type, XDWORD& ID, XBUFFER& xbuffer);
     DIOPROTOCOL_RESULT                       Received                        ();
     DIOPROTOCOL_RESULT                       ReceivedHandle                  (XDWORD type, XDWORD ID, XBUFFER& xbuffer);

     bool                                     SendCommand                     (XDWORD type, XDWORD& ID, XBUFFER& xbuffer);
     bool                                     Command                         (XDWORD type, XBUFFER& xbuffer, int timeout = DIOPROTOCOL_TIMEOUT);

     bool                                     SendEvent                       (DIOPROTOCOLXEVENTTYPE type, DIOPROTOCOL* protocol);
     bool                                     SendEventCommand                (DIOPROTOCOL* protocol, DIOPROTOCOL_DIRECTION direction,XDWORD type,bool result,void* param1,XDWORD param2);
     bool                                     SendEventFileBlock              (DIOPROTOCOL_FILESTATUS filestatus, bool fileissend, XPATH* filesource, XPATH* filetarget, int filepercent, void* param);


     void                                     GetProtocolVersion              (XDWORD& version, XDWORD& subversion, XDWORD& subversionerr)
                                              {
                                                version         = this->protocolversion;
                                                subversion      = this->protocolsubversion;
                                                subversionerr   = this->protocolsubversionerr;
                                              }

     void                                     SetProtocolVersion              (XDWORD version, XDWORD subversion, XDWORD subversionerr)
                                              {
                                                this->protocolversion           = version;
                                                this->protocolsubversion        = subversion;
                                                this->protocolsubversionerr     = subversionerr;
                                              }

     void                                     GetApplicationVersion           (XDWORD& version, XDWORD& subversion, XDWORD& subversionerr)
                                              {
                                                version         = this->applicationversion;
                                                subversion      = this->applicationsubversion;
                                                subversionerr   = this->applicationsubversionerr;
                                              }

     void                                     SetApplicationVersion           (XDWORD version, XDWORD subversion, XDWORD subversionerr)
                                              {
                                                this->applicationversion        = version;
                                                this->applicationsubversion     = subversion;
                                                this->applicationsubversionerr  = subversionerr;
                                              }


     bool                                     SetFileProgress                 (DIOPROTOCOL_DIRECTION direction,XPATH& sourcename,XPATH& targetname,XDWORD CRC32);
     DIOPROTOCOL_FILE*                        GetFileProgress                 ()                                        { return fileprogress;                                      }

     XPATH*                                   GetPathSearch                   ()                                        { return &pathsearch;                                       }
     XDIRELEMENT*                             GetSearchElement                ()                                        { return &searchelement;                                    }
     XDIR*                                    GetXDir                         ()                                        { return xdir;                                              }

     bool                                     CMD_GetProtocolVersion          (XWORD& version, XWORD& subversion, XWORD& subversionerr);
     static int                               RCV_GetProtocolVersion          (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_GetApplicationVersion       (XWORD& version, XWORD& subversion, XWORD& subversionerr);
     static int                               RCV_GetApplicationVersion       (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_Ping                        (XDWORD& milliseconds, int timeout = DIOPROTOCOL_TIMEOUT);
     static int                               RCV_Ping                        (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_SendIsInitServerProtocol    (bool isinit);
     static int                               RCV_SendIsInitServerProtocol    (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_GetDateTime                 (XSTRING& datestring);
     static int                               RCV_GetDateTime                 (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_SetDateTime                 (XCHAR* datestring);
     static int                               RCV_SetDateTime                 (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_File                        (DIOPROTOCOL_DIRECTION direction, XCHAR* sourcename, XCHAR* targetname, int timeout = DIOPROTOCOL_TIMEOUTFILEBLOCK, void* param = NULL);

     bool                                     CMD_DeleteFile                  (XCHAR* namefile, bool overwrite);
     static int                               RCV_DeleteFile                  (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_MakeDir                     (XCHAR* namedir);
     static int                               RCV_MakeDir                     (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_DeleteDir                   (XCHAR* namedir, bool all);
     static int                               RCV_DeleteDir                   (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_GetDirElement               (XCHAR* pathsearch, XCHAR* mask, XPATH& namefile, XBYTE& type, int& size, XDWORD& CRC32);
     static int                               RCV_GetDirElement               (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     #ifdef DIOALERTS_ACTIVE
     bool                                     CMD_SendAlert                   (DIOALERT* alert);
     static int                               RCV_SendAlert                   (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);
     #endif

  protected:

     DIOSTREAM*                               diostream;
     XSTRING                                  description;

     XWORD                                    protocolversion;
     XWORD                                    protocolsubversion;
     XWORD                                    protocolsubversionerr;

     XWORD                                    applicationversion;
     XWORD                                    applicationsubversion;
     XWORD                                    applicationsubversionerr;

     bool                                     isinitialized;
     bool                                     isserverinitialized;

     XDATETIME*                               xdatetime;
     XTIMER*                                  xtimer;
     XTIMER*                                  watchdog;

     void*                                    connexionsmanager;
     void*                                    connexion;
     void*                                    applicationdata;
     bool                                     isownapplicationdata;

     XVECTOR<DIOPROTOCOL_COMMAND*>            commands;
     XVECTOR<DIOPROTOCOL_ANSWER*>             answers;
     XVECTOR<DIOPROTOCOL_ANSWER*>             asks;

     XDWORD                                   maskID;

     DIOPROTOCOL_FILE*                        fileprogress;


  private:

     void                                     Clean                           ()
                                              {
                                                diostream                     = NULL;

                                                protocolversion               = DIOPROTOCOL_VERSION;
                                                protocolsubversion            = DIOPROTOCOL_SUBVERSION;
                                                protocolsubversionerr         = DIOPROTOCOL_SUBVERSIONERR;

                                                applicationversion            = 0;
                                                applicationsubversion         = 0;
                                                applicationsubversionerr      = 0;

                                                isinitialized                 = false;
                                                isserverinitialized           = false;

                                                xdatetime                     = NULL;
                                                xtimer                        = NULL;
                                                watchdog                      = NULL;

                                                xdir                          = NULL;

                                                connexionsmanager             = NULL;
                                                connexion                     = NULL;
                                                applicationdata               = NULL;
                                                isownapplicationdata          = false;

                                                maskID                        = 0;

                                                IDmsgcounter                  = 1;
                                                timerfileprogress             = NULL;

                                                xdir                          = NULL;

                                                doexit                        = false;

                                                xmutexanswers                 = NULL;
                                                xmutexasks                    = NULL;
                                                xmutexcommands                = NULL;
                                                xmutexreads                   = NULL;
                                                xmutexwrites                  = NULL;
                                                xmutexfilesprogress           = NULL;

                                                xthreadreceived               = NULL;
                                                xthreadasks                   = NULL;
                                              }


     bool                                     DeleteAllCommands               ();

     XDWORD                                   CRC32OfFile                     (XPATH& xpath);

     bool                                     CMD_InitFileBlock               (DIOPROTOCOL_FILE* fp);
     static int                               RCV_InitFileBlock               (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_GetFileBlock                (DIOPROTOCOL_FILE* fp);
     static int                               RCV_GetFileBlock                (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     bool                                     CMD_SendFileBlock               (DIOPROTOCOL_FILE* fp);
     static int                               RCV_SendFileBlock               (DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

     static void                              ThreadReceived                  (void* data);
     static void                              ThreadAsks                      (void* data);

     XDWORD                                   IDmsgcounter;
     XTIMER*                                  timerfileprogress;

     XPATH                                    pathsearch;
     XDIR*                                    xdir;
     XDIRELEMENT                              searchelement;

     bool                                     doexit;

     XMUTEX*                                  xmutexanswers;
     XMUTEX*                                  xmutexasks;

     XMUTEX*                                  xmutexcommands;
     XMUTEX*                                  xmutexreads;
     XMUTEX*                                  xmutexwrites;
     XMUTEX*                                  xmutexfilesprogress;

     XTHREADCOLLECTED*                        xthreadreceived;
     XTHREADCOLLECTED*                        xthreadasks;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

