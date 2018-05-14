/*------------------------------------------------------------------------------------------
//  DIOLINUXPCAP.H
*/
/**
// \class
//
//  Interface PCap Library (Capture Ethernet Packets)
//
//  @author  Abraham J. Velez
//  @version 22/10/2012 13:25:51
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifdef DIOPCAP_ACTIVE

#ifndef _DIOLINUXPCAP_H_
#define _DIOLINUXPCAP_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <pcap.h>

#include "DIOPCap.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XTHREADCOLLECTED;

class DIOLINUXPCAP : public DIOPCAP
{
  public:

                                  DIOLINUXPCAP                        ();
    virtual                      ~DIOLINUXPCAP                        ();

    bool                          Capture_Start                       (DIOPCAPNETINTERFACE* netinterface, bool promiscuousmode = true, int timeout = 1000);
    bool                          Capture_End                         ();

  private:

    void                          Clean                               ();

    bool                          CreateListNetInterfaces             ();

    static void                   ThreadCapture                       (void* data);
    static void                   PacketHandler                       (u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data);

    pcap_t*                       handle;
    XTHREADCOLLECTED*             threadcapture;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

#endif