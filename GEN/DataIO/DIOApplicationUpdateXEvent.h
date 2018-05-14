//------------------------------------------------------------------------------------------
//  DIOAPPLICATIONUPDATEXEVENT.H
//
/**
// \class
//
//  WEB Server Event class
//
//  @author  Abraham J. Velez
//  @version 20/01/2007 21:05:48
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOAPPLICATIONUPDATEXEVENT_H_
#define _DIOAPPLICATIONUPDATEXEVENT_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XEvent.h"
#include "XString.h"
#include "XVector.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOAPPLICATIONUPDATEXEVENTTYPE
{
  DIOAPPLICATIONUPDATEXEVENTTYPE_UNKNOWN                 = XEVENTTYPE_APPLICATIONUPDATE ,
  DIOAPPLICATIONUPDATEXEVENTTYPE_GETVERSIONAVAILABLE                                    ,
  DIOAPPLICATIONUPDATEXEVENTTYPE_DOWNLOADFILE                                           ,
  DIOAPPLICATIONUPDATEXEVENTTYPE_UNZIPFILE                                              ,
  DIOAPPLICATIONUPDATEXEVENTTYPE_BACKUPORIGINALFILE                                     ,
  DIOAPPLICATIONUPDATEXEVENTTYPE_COPYUPDATEFILE                                         ,
  DIOAPPLICATIONUPDATEXEVENTTYPE_DELETEBACKUPORIGINALFILE                               ,
  DIOAPPLICATIONUPDATEXEVENTTYPE_RESTOREUPDATE                                          ,
};


//---- CLASS -------------------------------------------------------------------------------

class XSUBJECT;
class XSTRING;
class DIOAPPLICATIONUPDATEVERSIONDATA;
class DIOAPPLICATIONUPDATEFILE;


class DIOAPPLICATIONUPDATEXEVENT : public XEVENT
{
  public:
                                            DIOAPPLICATIONUPDATEXEVENT        (XSUBJECT* subject, XDWORD family = XEVENTTYPE_APPLICATIONUPDATE, XDWORD type = DIOAPPLICATIONUPDATEXEVENTTYPE_UNKNOWN) : XEVENT(subject, family, type)
                                            {
                                              Clean();
                                            }

    virtual                                ~DIOAPPLICATIONUPDATEXEVENT        ()                                                                                        { Clean();                                        }

    DIOAPPLICATIONUPDATEVERSIONDATA*        GetVersionData                    ()                                                                                        { return versiondata;                             }
    void                                    SetVersionData                    (DIOAPPLICATIONUPDATEVERSIONDATA* versiondata)                                            { this->versiondata = versiondata;                }


    XVECTOR<DIOAPPLICATIONUPDATEFILE*>*     GetFilesToUpdate                  ()                                                                                        { return filestoupdate;                           }
    void                                    SetFilesToUpdate                  (XVECTOR<DIOAPPLICATIONUPDATEFILE*>* filestoupdate)                                       { this->filestoupdate = filestoupdate;            }

    XDWORD                                  GetIndexOfOperation               ()                                                                                        { return indexoperation;                          }
    void                                    SetIndexOfOperation               (XDWORD indexoperation)                                                                   { this->indexoperation = indexoperation;          }

    bool                                    GetStatusOperation                ()                                                                                        { return statusoperation;                         }
    void                                    SetStatusOperation                (bool statusoperation)                                                                    { this->statusoperation = statusoperation;        }


    DIOAPPLICATIONUPDATEFILE*               GetActualFileToUpdate             ()
                                            {
                                              if(!filestoupdate) return NULL;
                                              return filestoupdate->Get(indexoperation);
                                            }

    XSTRING*                                GetActualUnzipFileName            ()                                                                                        { return &actualunzipfilename;                    }

  private:

    void                                    Clean                             ()
                                            {
                                              versiondata     = NULL;
                                              filestoupdate   = NULL;
                                              indexoperation  = 0;
                                              statusoperation = false;
                                            }

    DIOAPPLICATIONUPDATEVERSIONDATA*        versiondata;
    XVECTOR<DIOAPPLICATIONUPDATEFILE*>*     filestoupdate;
    XDWORD                                  indexoperation;
    bool                                    statusoperation;
    XSTRING                                 actualunzipfilename;
};






//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

