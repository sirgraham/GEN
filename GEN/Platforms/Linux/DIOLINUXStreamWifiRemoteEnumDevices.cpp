//------------------------------------------------------------------------------------------
//  DIOLINUXSTREAMWIFIREMOTEENUMDEVICES.CPP
//
//  LINUX Data IO Stream Wifi Remote Enum Devices class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/01/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#if defined(DIO_ACTIVE) && defined(DIOWIFI_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <syslog.h>
#include <signal.h>
#include <time.h>
#include <iwlib.h>

#include "XFactory.h"
#include "XVector.h"
#include "XString.h"
#include "XThreadCollected.h"
#include "XDebugTrace.h"

#include "DIOStream.h"
#include "DIOStreamDeviceWifi.h"

#include "DIOLINUXStreamWifiRemoteEnumDevices.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------





/*-------------------------------------------------------------------
//  DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::DIOLINUXSTREAMWIFIREMOTEENUMDEVICES
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/01/2014 19:22:27
//
//  @return


*/
/*-----------------------------------------------------------------*/
DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::DIOLINUXSTREAMWIFIREMOTEENUMDEVICES() : DIOSTREAMWIFIREMOTEENUMDEVICES() , XFSMACHINE(0)
{
  Clean();

  AddState( DIOLINUXWIFIENUMFSMSTATE_NONE               ,
            DIOLINUXWIFIENUMFSMEVENT_SEARCH             , DIOLINUXWIFIENUMFSMSTATE_SEARCH           ,
            DIOLINUXWIFIENUMFSMEVENT_SEARCHEND          , DIOLINUXWIFIENUMFSMSTATE_SEARCHEND        ,
            EVENTDEFEND);

  AddState( DIOLINUXWIFIENUMFSMSTATE_SEARCH             ,
            DIOLINUXWIFIENUMFSMEVENT_SEARCHEND          , DIOLINUXWIFIENUMFSMSTATE_SEARCHEND        ,
            EVENTDEFEND);

  AddState( DIOLINUXWIFIENUMFSMSTATE_SEARCHEND          ,
            DIOLINUXWIFIENUMFSMEVENT_SEARCH             , DIOLINUXWIFIENUMFSMSTATE_SEARCH           ,
            EVENTDEFEND);

  threadenumdevices = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMWIFI, __L("DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::DIOLINUXSTREAMWIFIREMOTEENUMDEVICES"), ThreadEnumDevices, (void*)this);
  if(threadenumdevices) threadenumdevices->Ini();
}



/*-------------------------------------------------------------------
//  DIOLINUXSTREAMTCPIPREMOTEENUMDEVICES::~DIOLINUXSTREAMTCPIPREMOTEENUMDEVICES
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
DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::~DIOLINUXSTREAMWIFIREMOTEENUMDEVICES()
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
//  DIOLINUXSTREAMTCPIPREMOTEENUMDEVICES::Search
*/
/**
//
//
//  @author       Abraham J. Velezifconfig
//  @version      28/04/2013 19:00:27
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::Search()
{
  if(!threadenumdevices)       return false;

  DelAllDevices();

  SetEvent(DIOLINUXWIFIENUMFSMEVENT_SEARCH);

  threadenumdevices->Run(true);

  sleep(1);

  return true;
}



/*-------------------------------------------------------------------
//  DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::StopSearch
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
bool DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::StopSearch(bool waitend)
{
  if(IsSearching())
    {
      SetEvent(DIOLINUXWIFIENUMFSMEVENT_SEARCHEND);

      if(waitend)
        {
          while(GetCurrentState()!=DIOLINUXWIFIENUMFSMEVENT_SEARCHEND)
            {
              usleep(1000);
            }
        }
    }

  if(threadenumdevices) threadenumdevices->Run(false);

  usleep(1000);

  return true;
};




/*-------------------------------------------------------------------
//  DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::IsSearching
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
bool DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::IsSearching()
{
  if(!threadenumdevices)              return false;
  if(!threadenumdevices->IsRunning()) return false;

  if(GetCurrentState()!=DIOLINUXWIFIENUMFSMSTATE_SEARCH) return false;

  return true;
};




/*-------------------------------------------------------------------
//  DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::Clean
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
void DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::Clean()
{
  threadenumdevices = NULL;

}



/*-------------------------------------------------------------------
//  DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::ThreadEnumDevices
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/05/2013 17:02:14
//
//  @return       void :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
void DIOLINUXSTREAMWIFIREMOTEENUMDEVICES::ThreadEnumDevices(void* param)
{
  DIOLINUXSTREAMWIFIREMOTEENUMDEVICES* enumeration = (DIOLINUXSTREAMWIFIREMOTEENUMDEVICES*)param;
  if(!enumeration) return;

  if(enumeration->GetEvent()==DIOLINUXWIFIENUMFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(enumeration->GetCurrentState())
        {
          case DIOLINUXWIFIENUMFSMSTATE_NONE            : break;

          case DIOLINUXWIFIENUMFSMSTATE_SEARCH          : enumeration->SetEvent(DIOLINUXWIFIENUMFSMEVENT_SEARCHEND);
                                                          break;

          case DIOLINUXWIFIENUMFSMSTATE_SEARCHEND       : break;


        }
    }
   else
    {
      if(enumeration->GetEvent()<DIOLINUXWIFIENUM_LASTEVENT)
        {
          enumeration->CheckTransition();

          switch(enumeration->GetCurrentState())
            {
              case DIOLINUXWIFIENUMFSMSTATE_NONE            : break;

              case DIOLINUXWIFIENUMFSMSTATE_SEARCH          : { wireless_scan_head  head;
                                                                wireless_scan*      result = NULL;
                                                                iwrange             range;
                                                                int                 sock;
                                                                int                 index = 0;

                                                                memset(&head, 0, sizeof(wireless_scan_head));
                                                                memset(&range, 0, sizeof(iwrange));

                                                                /* Open socket to kernel */
                                                                sock = iw_sockets_open();
                                                                if(sock==-1) break;

                                                                /* Get some metadata to use for scanning */
                                                                if (iw_get_range_info(sock, "wlan0", &range)<0) break;

                                                                /* Perform the scan */
                                                                if(iw_scan(sock, (char*)"wlan0", range.we_version_compiled, &head)<0) break;

                                                                /* Traverse the results */
                                                                result = head.result;
                                                                while (NULL != result)
                                                                  {
                                                                    DIOSTREAMDEVICEWIFI* device = new DIOSTREAMDEVICEWIFI();
                                                                    if(device)
                                                                      {
                                                                        device->SetIndex(index);
                                                                        (*device->GetName()) = result->b.essid;

                                                                        enumeration->devices.Add(device);

                                                                        index++;
                                                                      }

                                                                    result = result->next;
                                                                  }
                                                              }
                                                              break;

              case DIOLINUXWIFIENUMFSMSTATE_SEARCHEND       : break;
           }
        }
    }

}


#endif