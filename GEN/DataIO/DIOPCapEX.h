/*------------------------------------------------------------------------------------------
//  DIOPCAP.H
*/
/**
// \class
//
//  Interface PCap Extended Library class
//
//  @author  Abraham J. Velez
//  @version 22/10/2012 13:25:51
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifdef DIOPCAP_ACTIVE


#ifndef _DIOPCAPEX_H_
#define _DIOPCAPEX_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"

#include "DIOURL.h"

#include "DIOPCap.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

typedef struct
{
  XWORD            ID;

  XBYTE            QR:1;
  XBYTE            opcode:4;
  XBYTE            AA:1;
  XBYTE            TC:1;
  XBYTE            RD:1;

  XBYTE            RA:1;
  XBYTE            Z:3;
  XBYTE            rcode:4;

  XWORD            QDcount;
  XWORD            ANcount;
  XWORD            NScount;
  XWORD            ARcount;

} DIOPCAPEXDNSHEADER;


typedef struct
{
  DIOURL           url;
  XWORD            qtype;
  XWORD            qclass;

} DIOPCAPEXDNSASK;


typedef struct
{
  DIOURL           url;
  XWORD            qtype;
  XWORD            qclass;
  XWORD            ttl;
  XWORD            rlenght;
  XWORD            preference;
  XWORD            exchange;
  XWORD            answer;
  DIOPCAPIPADDRESS address;

} DIOPCAPEXDNSREQUEST;


/*---- CLASS -----------------------------------------------------------------------------*/

class DIOPCAPFRAMEEX : public DIOPCAPFRAME
{
  public:
                                  DIOPCAPFRAMEEX                (bool hardwareuselittleendian);
    virtual                      ~DIOPCAPFRAMEEX                ();

    bool                          GetDNSAsk                     (DIOPCAPEXDNSHEADER& header,DIOPCAPEXDNSASK& ask);
    bool                          GetDNSRequest                 (DIOPCAPEXDNSHEADER& header,DIOPCAPEXDNSREQUEST& request);

    int                           DNSnameFormatToString         (XBYTE* urlDNS,DIOURL& url);
    bool                          StringToDNSNameFormat         (XSTRING& name,XBUFFER& nameDNS);

  private:

    void                          Clean                         ();
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

#endif