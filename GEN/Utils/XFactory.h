//------------------------------------------------------------------------------------------
//  XFACTORY.H
//
/**
// \class
//
//  eXtended Platform Factory
//
//  @author  Abraham J. Velez
//  @version 15/07/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XFACTORY_H_
#define _XFACTORY_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XThread.h"

#include "XMemory.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XTIMER;
class XDATETIME;
class XRAND;
class XFILE;
class XDIR;
class XSYSTEM;
class XCONSOLE;
#ifdef XPHONE_ACTIVE
class XPHONE;
#endif
class XMUTEX;
class XTHREAD;


class XFACTORY
{
  public:
                                XFACTORY                    ()                                                              {                                                                   }
    virtual                    ~XFACTORY                    ()                                                              {                                                                   }

    virtual XTIMER*             CreateTimer                 ()                                                              { return NULL;                                                      }
    virtual bool                DeleteTimer                 (XTIMER* timer)                                                 { return false;                                                     }

    virtual XDATETIME*          CreateDateTime              ()                                                              { return NULL;                                                      }
    virtual bool                DeleteDateTime              (XDATETIME* datetime)                                           { return false;                                                     }

    virtual XRAND*              CreateRand                  ()                                                              { return NULL;                                                      }
    virtual bool                DeleteRand                  (XRAND* rand)                                                   { return false;                                                     }

    virtual XFILE*              Create_File                 ()                                                              { return NULL;                                                      }
    virtual bool                Delete_File                 (XFILE* file)                                                   { return false;                                                     }

    virtual XDIR*               Create_Dir                  ()                                                              { return NULL;                                                      }
    virtual bool                Delete_Dir                  (XDIR*  dir)                                                    { return false;                                                     }

    virtual XSYSTEM*            CreateSystem                ()                                                              { return NULL;                                                      }
    virtual bool                DeleteSystem                (XSYSTEM* xsystem)                                              { return false;                                                     }

    virtual XCONSOLE*           CreateConsole               ()                                                              { return NULL;                                                      }
    virtual bool                DeleteConsole               (XCONSOLE* xconsole)                                            { return false;                                                     }

    #ifdef XPHONE_ACTIVE
    virtual XPHONE*             CreatePhone                 ()                                                              { return NULL;                                                      }
    virtual bool                DeletePhone                 (XPHONE* phone)                                                 { return false;                                                     }
    #endif

    virtual XMUTEX*             Create_Mutex                ()                                                              { return NULL;                                                      }
    virtual bool                Delete_Mutex                (XMUTEX* phone)                                                 { return false;                                                     }

    virtual XTHREAD*            CreateThread                (XTHREADGROUPID groupID, XCHAR* ID,XTHREADFUNCTION function = NULL,void* data = NULL)   { return NULL;                              }
    virtual bool                DeleteThread                (XTHREADGROUPID groupID, XTHREAD* xthread)                      { return false;                                                     }

    bool                        HardwareUseLittleEndian     ()                                                              { return uselittleendian;                                           }
    bool                        HardwareUseLittleEndian     (bool uselittleendian)                                          { this->uselittleendian = uselittleendian;  return uselittleendian; }

  private:

    bool                        uselittleendian;
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------


extern XFACTORY* xfactory;


#endif

