/*------------------------------------------------------------------------------------------
//  DIOPCAP.H
*/
/**
// \class
//
//  Interface PCap Library class
//
//  @author  Abraham J. Velez
//  @version 22/10/2012 13:25:51
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifdef DIOPCAP_ACTIVE


#ifndef _DIOPCAP_H_
#define _DIOPCAP_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"
#include "XString.h"
#include "XBuffer.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XTHREADCOLLECTED;
class XMUTEX;


#define DIOPCAPETHERNETMACSIZE        6

#define DIOPCAPETHERNETTYPE_PUP       0x0002    // Xerox PUP
#define DIOPCAPETHERNETTYPE_IP        0x0800    // IP
#define DIOPCAPETHERNETTYPE_ARP       0x0860    // Address resolution
#define DIOPCAPETHERNETTYPE_REVARP    0x8035    // Reverse ARP

#define DIOPCAPIPPROTOCOLTYPE_TCP     6         // TCP
#define DIOPCAPIPPROTOCOLTYPE_UDP     17        // UDP


typedef struct
{
  XBYTE                               byte1;
  XBYTE                               byte2;
  XBYTE                               byte3;
  XBYTE                               byte4;

} DIOPCAPIPADDRESS;


typedef struct
{
  XBYTE                               MACtarget[DIOPCAPETHERNETMACSIZE];
  XBYTE                               MACsource[DIOPCAPETHERNETMACSIZE];
  XWORD                               type;

} DIOPCAPETHERNETHEADER;


typedef struct
{
  XBYTE                               ver_ihl;           // Version (4 bits) + Internet header length (4 bits)
  XBYTE                               typeservice;       // Type of service
  XWORD                               len;               // Total length
  XWORD                               identification;    // Identification
  XWORD                               flags_fo;          // Flags (3 bits) + Fragment offset (13 bits)
  XBYTE                               ttl;               // Time to live
  XBYTE                               protocol;          // Protocol
  XWORD                               CRC;               // Header checksum
  DIOPCAPIPADDRESS                    sourceaddr;        // Source address
  DIOPCAPIPADDRESS                    targetaddr;        // Target address

} DIOPCAPIPHEADER;



typedef struct
{
  XWORD                               sourceport;        // Source port
  XWORD                               targetport;        // Target port
  XWORD                               len;               // Datagram length
  XWORD                               CRC;               // Checksum

} DIOPCAPUDPHEADER;



typedef struct
{
  XWORD                               sourceport;
  XWORD                               targetport;
  XDWORD                              seq;
  XDWORD                              ack_seq;
  XWORD                               dataoffctrl;
  XWORD                               window;
  XWORD                               check;
  XWORD                               urg_ptr;

} DIOPCAPTCPHEADER;



class DIOPCAPNETINTERFACE
{
  public:
                                      DIOPCAPNETINTERFACE           ();
    virtual                          ~DIOPCAPNETINTERFACE           ();

    XSTRING*                          GetName                       ();
    XSTRING*                          GetDescription                ();

    bool                              SetName                       (XCHAR* name);
    bool                              SetName                       (XSTRING& name);

    bool                              SetDescription                (XCHAR* name);
    bool                              SetDescription                (XSTRING& name);

  private:

    void                              Clean                         ();

    XSTRING                           name;
    XSTRING                           description;
};




class DIOPCAPFRAME
{
  public:
                                      DIOPCAPFRAME                  ();
    virtual                          ~DIOPCAPFRAME                  ();

    bool                              GetHeaderEthernet             (DIOPCAPETHERNETHEADER& ethernetheader);
    bool                              GetHeaderIP                   (DIOPCAPIPHEADER& ipheader);
    bool                              GetHeaderUDP                  (DIOPCAPUDPHEADER& udpheader);
    bool                              GetHeaderTCP                  (DIOPCAPTCPHEADER& tcpheader);

    XBYTE*                            UserData_Get                  ();
    int                               UserData_GetSize              ();

    XBUFFER*                          GetData                       ();
    bool                              SetData                       (XBYTE* data, int size);

  private:

    void                              Clean                         ();

    XBUFFER*                          data;
};




class DIOPCAP
{
  public:

                                      DIOPCAP                       ();
    virtual                          ~DIOPCAP                       ();

    XVECTOR<DIOPCAPNETINTERFACE*>*    GetNetInterfaces              ();
    DIOPCAPNETINTERFACE*              GetNetInterface               (int index);

    virtual bool                      Ini                           ();

    bool                              Capture_Start                 (int index, bool promiscuousmode = true, int timeout = 1000);
    virtual bool                      Capture_Start                 (DIOPCAPNETINTERFACE* netinterface, bool promiscuousmode = true, int timeout = 1000) = 0;
    virtual bool                      Capture_End                   () = 0;

    DIOPCAPFRAME*                     Frames_Get                    (int index);
    bool                              Frames_Delete                 (int index);
    XVECTOR<DIOPCAPFRAME*>*           Frames_Get                    ();
    bool                              Frames_DeleteAll              ();

    virtual bool                      End                           ();

  protected:

    bool                              Frames_Add                    (XBYTE* data,int size);

    virtual bool                      CreateListNetInterfaces       () = 0;
    bool                              DeleteListNetInterfaces       ();

    XVECTOR<DIOPCAPNETINTERFACE*>     netinterfaces;
    XMUTEX*                           xmutexframes;
    XVECTOR<DIOPCAPFRAME*>            frames;

  private:

    void                              Clean                         ();
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

#endif