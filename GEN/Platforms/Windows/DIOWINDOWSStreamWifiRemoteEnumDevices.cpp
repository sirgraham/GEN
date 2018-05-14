//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES.CPP
//
//  WINDOWS Data IO Stream Wifi Remote Enum Devices class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/01/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#if defined(DIO_ACTIVE) && defined(DIOWIFI_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------


#include <windows.h>

#include "XFactory.h"
#include "XThreadCollected.h"

#include "DIOStream.h"
#include "DIOStreamDeviceBluetooth.h"

#include "DIOWINDOWSStreamWifiRemoteEnumDevices.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIPREMOTEENUMDEVICES::DIOWINDOWSSTREAMTCPIPREMOTEENUMDEVICES
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 19:00:01
//
//  @return


*/
/*-----------------------------------------------------------------*/
DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES() : DIOSTREAMWIFIREMOTEENUMDEVICES(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOWINDOWSWIFIENUMFSMSTATE_NONE             ,
            DIOWINDOWSWIFIENUMFSMEVENT_SEARCH           , DIOWINDOWSWIFIENUMFSMSTATE_SEARCH           ,
            DIOWINDOWSWIFIENUMFSMEVENT_SEARCHEND        , DIOWINDOWSWIFIENUMFSMSTATE_SEARCHEND        ,
            EVENTDEFEND);

  AddState( DIOWINDOWSWIFIENUMFSMSTATE_SEARCH           ,
            DIOWINDOWSWIFIENUMFSMEVENT_NONE             , DIOWINDOWSWIFIENUMFSMSTATE_NONE             ,
            DIOWINDOWSWIFIENUMFSMEVENT_SEARCHEND        , DIOWINDOWSWIFIENUMFSMSTATE_SEARCHEND        ,
            EVENTDEFEND);

  AddState( DIOWINDOWSWIFIENUMFSMSTATE_SEARCHEND        ,
            DIOWINDOWSWIFIENUMFSMEVENT_NONE             , DIOWINDOWSWIFIENUMFSMSTATE_NONE             ,
            DIOWINDOWSWIFIENUMFSMEVENT_SEARCH           , DIOWINDOWSWIFIENUMFSMSTATE_SEARCH           ,
            EVENTDEFEND);

  threadenumdevices = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMWIFI, __L("DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES"), ThreadEnumDevices, (void*)this);
  if(threadenumdevices) threadenumdevices->Ini();
}



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIPREMOTEENUMDEVICES::~DIOWINDOWSSTREAMTCPIPREMOTEENUMDEVICES
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 19:00:12
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::~DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES()
{
  StopSearch(true);

  if(threadenumdevices)
    {
      threadenumdevices->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMWIFI, threadenumdevices);
    }


  Clean();
}



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIPREMOTEENUMDEVICES::Search
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 19:00:27
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::Search()
{
  if(!threadenumdevices)       return false;

  DelAllDevices();

  SetEvent(DIOWINDOWSWIFIENUMFSMEVENT_SEARCH);

  threadenumdevices->Run(true);

  return true;
}



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::StopSearch
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2013 23:41:35
//
//  @return       bool :
//  @param        waitend :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::StopSearch(bool waitend)
{
  if(!IsSearching()) return false;

  SetEvent(DIOWINDOWSWIFIENUMFSMEVENT_SEARCHEND);

  if(waitend)
    {
      while(GetCurrentState()!=DIOWINDOWSWIFIENUMFSMEVENT_SEARCHEND)
        {
          Sleep(10);
        }
    }

  if(threadenumdevices) threadenumdevices->Run(false);

  Sleep(10);

  return true;
};




/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::IsSearching
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2013 23:41:40
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::IsSearching()
{
  if(!threadenumdevices) return false;

  if(!threadenumdevices->IsRunning()) return false;

  if(GetCurrentState() == DIOWINDOWSWIFIENUMFSMSTATE_SEARCH) return true;

  return false;
};




/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2013 23:25:03
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::Clean()
{
  threadenumdevices = NULL;

}




/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::ThreadEnumDevices
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2013 23:31:49
//
//  @return       void :
//  @param        thread :
*/
/*-----------------------------------------------------------------*/
void DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES::ThreadEnumDevices(void* data)
{
  DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES* func = (DIOWINDOWSSTREAMWIFIREMOTEENUMDEVICES*)data;
  if(!func) return;

  if(func->GetEvent()==DIOWINDOWSWIFIENUMFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(func->GetCurrentState())
        {
          case DIOWINDOWSWIFIENUMFSMSTATE_NONE            : break;

          case DIOWINDOWSWIFIENUMFSMSTATE_SEARCH          : func->SetEvent(DIOWINDOWSWIFIENUMFSMEVENT_SEARCHEND);
                                                            break;


          case DIOWINDOWSWIFIENUMFSMSTATE_SEARCHEND       : return;
        }
    }
   else
    {
      if(func->GetEvent() < DIOWINDOWSWIFIENUM_LASTEVENT)
        {
          func->CheckTransition();

          switch(func->GetCurrentState())
            {
              case DIOWINDOWSWIFIENUMFSMSTATE_NONE          : break;

              case DIOWINDOWSWIFIENUMFSMSTATE_SEARCH        : break;

              case DIOWINDOWSWIFIENUMFSMSTATE_SEARCHEND     : func->threadenumdevices->Run(false);
                                                              break;
            }
        }
    }
}

#endif