/*------------------------------------------------------------------------------------------
//  DIOWINDOWSPCAP.CPP
//
//  Interface PCap Library (Capture Ethernet Packets)
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 22/10/2012 13:30:11
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifdef DIOPCAP_ACTIVE

#pragma comment(lib,"libwpcap.a")

/*---- INCLUDES --------------------------------------------------------------------------*/

#include <winsock2.h>
#include <windows.h>

#include "XDebugTrace.h"
#include "XString.h"
#include "XWINDOWSThread.h"

#include "DIOWINDOWSPCap.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOWINDOWSPCAP::DIOWINDOWSPCAP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/10/2012 13:09:05
//
//  @return       void :
//  @param        xfactory :
//  @param        uselittleendian :
*/
/*-----------------------------------------------------------------*/
DIOWINDOWSPCAP::DIOWINDOWSPCAP() :  DIOPCAP()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOWINDOWSPCAP::~DIOWINDOWSPCAP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/10/2012 15:18:36
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWINDOWSPCAP::~DIOWINDOWSPCAP()
{
  End();

  Clean();
}




/*-------------------------------------------------------------------
//  DIOWINDOWSPCAP::Capture_Start
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/10/2012 9:37:03
//
//  @return       bool :
//  @param        netinterface :
//  @param        promiscuousmode :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSPCAP::Capture_Start(DIOPCAPNETINTERFACE* netinterface, bool promiscuousmode, int timeout)
{
  if(!netinterface) return false;

  char errbuf[PCAP_ERRBUF_SIZE];


  XSTRING_CREATEOEM((*netinterface->GetName()), ni)

  handle= pcap_open_live(ni                      , // name of the device
                         65536                   , // portion of the packet to capture.  65536 grants that the whole packet will be captured on all the MACs.
                         promiscuousmode?1:0     , // promiscuous mode (nonzero means promiscuous)
                         timeout                 , // read timeout
                         errbuf);                  // error buffer

  XSTRING_DELETEOEM(ni)

  if(handle == NULL) return false;


  threadcapture = CREATEXTHREAD(XTHREADGROUPID_DIOPCAP, XTHREADGROUPID_DIOPCAP, __L("DIOWINDOWSPCAP::Capture_Start"), ThreadCapture, (void*)this);
  if(!threadcapture) return false;

  return threadcapture->Ini();
}



/*-------------------------------------------------------------------
//  DIOWINDOWSPCAP::Capture_End
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/10/2012 17:15:54
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOWINDOWSPCAP::Capture_End()
{
  if(handle) pcap_breakloop(handle);

  if(threadcapture)
    {
      threadcapture->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOPCAP, XTHREADGROUPID_DIOPCAP, threadcapture);
      threadcapture = NULL;
    }

  if(handle)
    {
      pcap_close(handle);
      handle = NULL;
    }

  return true;
}




/*-------------------------------------------------------------------
//  DIOWINDOWSPCAP::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/10/2012 15:18:56
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOWINDOWSPCAP::Clean()
{
  handle        = NULL;
  threadcapture = NULL;
}




/*-------------------------------------------------------------------
//  DIOWINDOWSPCAP::CreateListNetInterfaces
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/10/2012 16:52:27
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOWINDOWSPCAP::CreateListNetInterfaces()
{
  pcap_if_t* allnetinterfaces;
  pcap_if_t* netinterface;
  char       errbuf[PCAP_ERRBUF_SIZE];

  // Retrieve the device list
  if(pcap_findalldevs(&allnetinterfaces, errbuf) == -1) return false;

  // Create list
  for(netinterface=allnetinterfaces; netinterface; netinterface=netinterface->next)
    {
      DIOPCAPNETINTERFACE* _netinterface = new DIOPCAPNETINTERFACE();
      if(_netinterface)
        {
          XSTRING string;

          string = netinterface->name;          _netinterface->SetName(string);
          string = netinterface->description;   _netinterface->SetDescription(string);

          netinterfaces.Add(_netinterface);
        }
    }

  pcap_freealldevs(allnetinterfaces);

  return true;
}



/*-------------------------------------------------------------------
//  DIOWINDOWSPCAP::ThreadCapture
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/10/2012 9:03:44
//
//  @return       void :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
void DIOWINDOWSPCAP::ThreadCapture(void* data)
{
  DIOWINDOWSPCAP* diopcap = (DIOWINDOWSPCAP*)data;
  if(!diopcap) return;

  if(diopcap->handle)
    {
      int error = pcap_loop(diopcap->handle, 1, PacketHandler, (u_char*)diopcap);
      if(error)
        {

        }
    }
}




/*-------------------------------------------------------------------
//  DIOWINDOWSPCAP::PacketHandler
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/10/2012 9:42:38
//
//  @return       void :
//  @param        param :
//  @param        header :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
void DIOWINDOWSPCAP::PacketHandler(u_char* param, const struct pcap_pkthdr* header, const u_char* data)
{
  DIOWINDOWSPCAP* diopcap = (DIOWINDOWSPCAP*)param;
  if(!diopcap) return;

  diopcap->Frames_Add((XBYTE*)data,header->len);


  /*
  struct tm *ltime;
  char timestr[16];
  time_t local_tv_sec;


  //  unused parameters

  //(VOID)(param);
  //(VOID)(pkt_data);

  // convert the timestamp to readable format
  local_tv_sec = header->ts.tv_sec;
  ltime=localtime(&local_tv_sec);
  strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);

  printf("%s,%.6d len:%d\n", timestr, header->ts.tv_usec, header->len);
  */

}

#endif