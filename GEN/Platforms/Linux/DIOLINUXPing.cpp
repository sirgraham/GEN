/*------------------------------------------------------------------------------------------
//  DIOLINUXPING.CPP
//
//  Data IO LINUX Ping class
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
#include <sys/fcntl.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <net/if.h>
#include <netdb.h>
#include <linux/if_ether.h>


#include "HashCRC32.h"

#include "XSleep.h"
#include "XRand.h"
#include "XDebugTrace.h"
#include "XBuffer.h"
#include "XTimer.h"


#include "DIOLINUXPing.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOLINUXPING::DIOLINUXPING
*/
/**
//
//  Class Constructor DIOLINUXPING
//
//  @author       Abraham J. Velez
//  @version      30/03/2016 12:55:40
//
*/
/*-----------------------------------------------------------------*/
DIOLINUXPING::DIOLINUXPING()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOLINUXPING::~DIOLINUXPING
*/
/**
//
//   Class Destructor DIOLINUXPING
//
//  @author       Abraham J. Velez
//  @version      30/03/2016 12:55:33
//
*/
/*-----------------------------------------------------------------*/
DIOLINUXPING::~DIOLINUXPING()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOLINUXPING::Do
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
bool DIOLINUXPING::Do(XDWORD nretries, XDWORD timebetweenchecks, bool exitfirstgoodreply)
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

      XDWORD size = sendto(handle,(char*)&echorequest, sizeof(DIOPING_ECHOREQUEST), 0, (sockaddr*)&targetaddr, sizeof(struct sockaddr_in));
      if(size != sizeof(DIOPING_ECHOREQUEST))
        {
         XDEBUGTRACE_PRINTCOLOR(4, __L("Ping: not write packet! %s"), targetIP.Get());
          break;
        }

      xbuffer.Resize(sizeof(DIOPING_ECHOREPLY));

      DIOPING_ECHOREPLY   echoreply;
      struct sockaddr_in  originaddr;
      socklen_t           sizeaddr      = sizeof(struct sockaddr_in);
      XSTRING             fromIP;
      bool                isreplypacket = false;

      memset(&echoreply,  0, sizeof(DIOPING_ECHOREPLY));
      memset(&originaddr, 0, sizeaddr);

      xtimer->Reset();

      int flags = fcntl(handle, F_GETFL, 0);
      fcntl(handle, F_SETFL, flags | O_NONBLOCK);

      do{  size = recvfrom(handle, (char*)xbuffer.Get(), xbuffer.GetSize(), 0, (struct sockaddr *)&originaddr, &sizeaddr);
           if(size == sizeof(DIOPING_ECHOREPLY))
             {
               memcpy((XBYTE*)&echoreply.iphdr, xbuffer.Get(), xbuffer.GetSize());

               if(echoreply.echorequest.icmphdr.type == DIOPING_ICMP_ECHOREPLY)
                 {
                   DIOIP    ip;
                   ip.Set(echoreply.iphdr.iasource.INADDR_UNION.addrbyte.s_b1,
                          echoreply.iphdr.iasource.INADDR_UNION.addrbyte.s_b2,
                          echoreply.iphdr.iasource.INADDR_UNION.addrbyte.s_b3,
                          echoreply.iphdr.iasource.INADDR_UNION.addrbyte.s_b4);

                   ip.GetXString(fromIP);

                   if(!fromIP.Compare(targetIP))  isreplypacket = true;
                 }
             }

           if(xtimer->GetMeasureSeconds() >= DIOPING_DEFAULTTIMEOUT)  break;

        } while(!isreplypacket);

      if(isreplypacket)
        {
          crc32.ResetResult();
          crc32.Do((XBYTE*)echoreply.echorequest.cdata, DIOPING_REQ_DATASIZE);

          if(crc32result == crc32.GetResultCRC32())
            {
              XWORD checksum = CalculeCheckSum((XWORD *)&echoreply.echorequest, sizeof(DIOPING_ECHOREQUEST));
              if(!checksum)
                {
                  if(echoreply.echorequest.applicationID == applicationID)
                    {
                      XDWORD elapsed = (XDWORD)(xtimer->GetMicroSecondsTickCounter() - echoreply.echorequest.time);

                      AddReply(nloop+1, fromIP.Get(), DIOPING_REQ_DATASIZE, (elapsed/1000), echoreply.iphdr.TTL);

                      DIOPINGXEVENT xevent(this,DIOPINGXEVENTTYPE_DOPING);
                      xevent.SetPingReply((DIOPINGREPLY*)replys.Get(replys.GetSize()-1));
                      PostEvent(&xevent);

                      //XDEBUGTRACE_PRINTCOLOR(1, __L("Ping: received! %s %s"), fromIP.Get(), targetIP.Get());

                      if(!exitfirstgoodreply) XSLEEP::GetInstance().MilliSeconds(timebetweenchecks);
                    }
                   else
                    {
                     XDEBUGTRACE_PRINTCOLOR(4, __L("Ping: application ID not equal! %s"), fromIP.Get());
                    }
                }
               else
                {
                 XDEBUGTRACE_PRINTCOLOR(4, __L("Ping: error Checksum! %s "), targetIP.Get());
                }
            }
           else
            {
             XDEBUGTRACE_PRINTCOLOR(4, __L("Ping: error CRC! %s "), targetIP.Get());
            }
        }
       else
        {
         XDEBUGTRACE_PRINTCOLOR(4, __L("Ping: invalid packet! %s "), targetIP.Get());
        }

      if(exitfirstgoodreply) nloop = 0; else nloop--;
   }

  close(handle);

  bool status = WasConnected();

  if(!status)
    {
     XDEBUGTRACE_PRINTCOLOR((status?1:4), __L("Ping to [%s]: %s"), targetIP.Get(), (status?__L("Ok."): __L("Error!")));
    }

  return status;
}