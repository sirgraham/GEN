/*------------------------------------------------------------------------------------------
//  DIOANDROIDPING.CPP
//
//  Data IO ANDROID Ping class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 30/08/2012 9:14:11
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <malloc.h>
#include <syslog.h>
#include <signal.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
//#include <sys/fcntl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>
#include <linux/if_ether.h>


#include "HashCRC32.h"

#include "XRand.h"
#include "XDebug.h"
#include "XBuffer.h"
#include "XTimer.h"


#include "DIOANDROIDPing.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOANDROIDPING::DIOANDROIDPING
*/
/**
//
//  Class Constructor DIOANDROIDPING
//
//  @author       Abraham J. Velez
//  @version      30/03/2016 12:55:40
//
*/
/*-----------------------------------------------------------------*/
DIOANDROIDPING::DIOANDROIDPING()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOANDROIDPING::~DIOANDROIDPING
*/
/**
//
//   Class Destructor DIOANDROIDPING
//
//  @author       Abraham J. Velez
//  @version      30/03/2016 12:55:33
//
*/
/*-----------------------------------------------------------------*/
DIOANDROIDPING::~DIOANDROIDPING()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOANDROIDPING::Do
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      31/03/2016 10:38:44
//
//  @return       bool :
//
//  @param        nretries :
//  @param        timebetweenchecks :
//  @param        exitfirstgoodreply :
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDPING::Do(XDWORD nretries, XDWORD timebetweenchecks, bool exitfirstgoodreply)
{
  return false;
}

/*
bool DIOANDROIDPING::Do(XDWORD nretries, XDWORD timebetweenchecks, bool exitfirstgoodreply)
{
  #define DIOPING_DEFAULTTIMEOUT 3

  if(!urltarget)         return false;
  if(!xtimer)            return false;

  XSTRING   targetIP;
  int       handle;

  DeleteAllReplys();

  urltarget->ResolveURL(targetIP);

  handle = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if(handle < 0) return false;

  struct sockaddr_in targetaddr = { 0 };

  targetaddr.sin_family = AF_INET;

  XSTRING_CREATEOEM(targetIP, charstr)
  targetaddr.sin_addr.s_addr = inet_addr(charstr);
  XSTRING_DELETEOEM(charstr)

  DIOPING_ECHOREQUEST echorequest;
  int                 ID    = 0xCAFE;
  int                 nseq  = 2;
  int                 nloop = nretries;

  while(nloop)
    {
      HASHCRC32 crc32;
      XRAND*    xrand        = xfactory->CreateRand();
      XDWORD    crc32result;
      XSTRING   address;
      XBUFFER   xbuffer;
      bool      status       = false;

      if(!xrand) break;

      memset((XBYTE*)&echorequest, 0, sizeof(DIOPING_ECHOREQUEST));

      // Fill in echo request
      echorequest.icmphdr.type      = DIOPING_ICMP_ECHOREQ;
      echorequest.icmphdr.code      = 0;
      echorequest.icmphdr.ID        = ID;
      echorequest.icmphdr.seq       = nseq++;

      //Fill in some data to send
      for(int nret=0; nret<DIOPING_REQ_DATASIZE; nret++)
        {
          echorequest.cdata[nret] = xrand->Max(255);
        }

      xfactory->DeleteRand(xrand);

      crc32.ResetResult();
      crc32.Do((XBYTE*)echorequest.cdata, DIOPING_REQ_DATASIZE);
      crc32result = crc32.GetResultCRC32();

      echorequest.applicationID     = applicationID;
      echorequest.time              = (XDWORD)xtimer->GetMicroSecondsTickCounter();                           // Save tick count when sent
      echorequest.icmphdr.checksum  = CalculeCheckSum((XWORD *)&echorequest, sizeof(DIOPING_ECHOREQUEST));    // Put data in packet and compute checksum

      if(echorequest.icmphdr.checksum)
        {
          int sizeaddr = sizeof(struct sockaddr_in);

          int size = sendto(handle,(char*)&echorequest, sizeof(DIOPING_ECHOREQUEST), 0, (sockaddr*)&targetaddr, sizeaddr);
          if(size) status = true;
        }

      if(!status)
        {
          XDEBUG_PRINTCOLOR(4, __L("Ping: not write packet! %s"), targetIP.Get());
          break;
        }

      while(1)
        {
          struct sockaddr_in  originaddr;
          socklen_t           sizeaddr = sizeof(struct sockaddr_in);
          int                 size     = 0;

          memset(&originaddr, 0, sizeaddr);

          xtimer->Reset();

          do{  xbuffer.Resize(sizeof(DIOPING_ECHOREPLY));
               size = recvfrom(handle, (char*)xbuffer.Get(), xbuffer.GetSize(), 0, (struct sockaddr *)&originaddr, &sizeaddr);
               xbuffer.Resize(size);

               if((xtimer->GetMeasureSeconds() > DIOPING_DEFAULTTIMEOUT) && (!size))
                 {
                   if(exitfirstgoodreply)  nloop = 0; else nloop--;
                   break;
                 }

            } while(!size);

          if(xbuffer.GetSize() >= sizeof(DIOPING_ECHOREPLY))
            {
              DIOPING_ECHOREPLY echoreply;

              memcpy((XBYTE*)&echoreply.iphdr, xbuffer.Get(), sizeof(DIOPING_ECHOREPLY));

              if(echoreply.echorequest.icmphdr.type == DIOPING_ICMP_ECHOREPLY)
                {
                  crc32.ResetResult();
                  crc32.Do((XBYTE*)echoreply.echorequest.cdata, DIOPING_REQ_DATASIZE);

                  if(crc32result == crc32.GetResultCRC32())
                    {
                      XWORD checksum = CalculeCheckSum((XWORD *)&echoreply.echorequest, sizeof(DIOPING_ECHOREQUEST));
                      if(!checksum)
                        {
                          DIOIP    ip;
                          XSTRING  fromIP;
                          XDWORD   elapsed;

                          //  Calculate elapsed time
                          elapsed = (XDWORD)(xtimer->GetMicroSecondsTickCounter() - echoreply.echorequest.time);

                          ip.Set(echoreply.iphdr.iasource.INADDR_UNION.addrbyte.s_b1,
                                 echoreply.iphdr.iasource.INADDR_UNION.addrbyte.s_b2,
                                 echoreply.iphdr.iasource.INADDR_UNION.addrbyte.s_b3,
                                 echoreply.iphdr.iasource.INADDR_UNION.addrbyte.s_b4);

                          ip.GetXString(fromIP);

                          if(!fromIP.Compare(targetIP))
                            {
                              if(echoreply.echorequest.applicationID == applicationID)
                                {
                                  AddReply(nloop+1, fromIP.Get(), DIOPING_REQ_DATASIZE, (elapsed/1000), echoreply.iphdr.TTL);

                                  DIOPINGXEVENT xevent(this,DIOPINGXEVENTTYPE_DOPING);
                                  xevent.SetPingReply((DIOPINGREPLY*)replys.Get(replys.GetSize()-1));
                                  PostEvent(&xevent);

                                  //XDEBUG_PRINTCOLOR(1, __L("Ping: received! %s %s"), fromIP.Get(), targetIP.Get());

                                  if(exitfirstgoodreply)
                                    {
                                      nloop = 0;
                                      break;
                                    }

                                  xtimer->SleepMilliSeconds(timebetweenchecks);

                                  nloop--;
                                  break;
                                }
                               else
                                {
                                  XDEBUG_PRINTCOLOR(4, __L("Ping: application ID not equal! %s"), fromIP.Get());
                                }
                            }
                           else
                            {
                              XDEBUG_PRINTCOLOR(4, __L("Ping: not IP equal! %s %s"), fromIP.Get(), targetIP.Get());
                            }

                        }
                       else
                        {
                          XDEBUG_PRINTCOLOR(4, __L("Ping: error Checksum! %s "), targetIP.Get());
                        }
                    }
                   else
                    {
                      XDEBUG_PRINTCOLOR(4, __L("Ping: error CRC! %s "), targetIP.Get());
                    }
                }
               else
                {
                  XDEBUG_PRINTCOLOR(4, __L("Ping: Is not reply packet! %s "), targetIP.Get());
                }
            }
           else
            {
              XDEBUG_PRINTCOLOR(4, __L("Ping: invalid size packet! %s "), targetIP.Get());
              if(exitfirstgoodreply) nloop = 0; else nloop--;
            }
        }
   }

  close(handle);

  bool status = WasConnected();

  //if(!status)
    {
      XDEBUG_PRINTCOLOR((status?1:4), __L("Ping to [%s]: %s"), targetIP.Get(), (status?__L("Ok."): __L("Error!")));
    }

  return status;

}
*/
