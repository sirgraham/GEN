//------------------------------------------------------------------------------------------
//  XWINDOWSPLFFACTORY.H
//
/**
// \class
//
//  WINDOWS utils factory class
//
//  @author  Abraham J. Velez
//  @version 15/07/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XWINDOWSPLFFACTORY_H_
#define _XWINDOWSPLFFACTORY_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XWINDOWSFACTORY : public XFACTORY
{
  public:

      XTIMER*             CreateTimer             ();
      bool                DeleteTimer             (XTIMER* timer);

      XDATETIME*          CreateDateTime          ();
      bool                DeleteDateTime          (XDATETIME* datetime);

      XRAND*              CreateRand              ();
      bool                DeleteRand              (XRAND* rand);

      XFILE*              Create_File             ();
      bool                Delete_File             (XFILE* file);

      XDIR*               Create_Dir              ();
      bool                Delete_Dir              (XDIR* dir);

      XSYSTEM*            CreateSystem            ();
      bool                DeleteSystem            (XSYSTEM* xsystem);

      XCONSOLE*           CreateConsole           ();
      bool                DeleteConsole           (XCONSOLE* xconsole);

      XMUTEX*             Create_Mutex            ();
      bool                Delete_Mutex            (XMUTEX* phone);

      XTHREAD*            CreateThread            (XTHREADGROUPID groupID, XCHAR* ID,XTHREADFUNCTION function = NULL,void* data = NULL);
      bool                DeleteThread            (XTHREADGROUPID groupID, XTHREAD* xthread);
      
      XSHAREDMEMORY*      Create_SharedMemory     ();   
      bool                Delete_SharedMemory     (XSHAREDMEMORY* sharedmemory); 


};

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

