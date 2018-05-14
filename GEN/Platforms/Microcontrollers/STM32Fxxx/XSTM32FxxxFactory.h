//------------------------------------------------------------------------------------------
//  XSTM32FXXXFACTORY.H
//
/**
// \class
//
//  STM32Fxxx utils platform factory class
//
//  @author  Abraham J. Velez
//  @version 15/07/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XSTM32FXXXFACTORY_H_
#define _XSTM32FXXXFACTORY_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XSTM32FXXXFACTORY : public XFACTORY
{
  public:

    XTIMER*             CreateTimer             ();
    bool                DeleteTimer             (XTIMER* timer);

    XDATETIME*          CreateDateTime          ();
    bool                DeleteDateTime          (XDATETIME* time);

    XRAND*              CreateRand              ();
    bool                DeleteRand              (XRAND* rand);

    XSYSTEM*            CreateSystem            ();
    bool                DeleteSystem            (XSYSTEM* system);

    XMUTEX*             Create_Mutex            ();
    bool                Delete_Mutex            (XMUTEX* phone);

    XTHREAD*            CreateThread            (XTHREADGROUPID groupID, XCHAR* ID,XTHREADFUNCTION function = NULL,void* data = NULL);
    bool                DeleteThread            (XTHREADGROUPID groupID, XTHREAD* xthread);


};

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

