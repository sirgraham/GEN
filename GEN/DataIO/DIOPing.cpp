/*------------------------------------------------------------------------------------------
//  DIOPING.CPP
//
//  Data IO Ping class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 30/08/2012 9:14:11
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "XFactory.h"
#include "XTimer.h"
#include "XDebugTrace.h"
#include "XRand.h"

#include "DIOStreamICMPConfig.h"
#include "DIOStreamICMP.h"
#include "DIOFactory.h"
#include "HashCRC32.h"

#include "DIOPing.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOPING::DIOPING
*/
/**
//
//  Class Constructor DIOPING
//
//  @author       Abraham J. Velez
//  @version      18/05/2015 9:13:30
//



*/
/*-----------------------------------------------------------------*/
DIOPING::DIOPING()
{
  Clean();

  RegisterEvent(DIOPINGXEVENTTYPE_DOPING);

  xtimer            = xfactory->CreateTimer();
  xmutexreplys      = xfactory->Create_Mutex();

  if(diofactory)
    {
      urltarget = diofactory->CreateURL();
    }

  HASHCRC32 crc32;
  XRAND*    xrand   = xfactory->CreateRand();
  XBUFFER   xbuffer;
  if(xrand)
    {
      for(int c=0; c<128; c++)
        {
          xbuffer.Add((XBYTE)xrand->Max(255));
        }

      crc32.ResetResult();
      crc32.Do(xbuffer);
      applicationID = crc32.GetResultCRC32();

      xfactory->DeleteRand(xrand);
    }
}



/*-------------------------------------------------------------------
//  DIOPING::~DIOPING
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/09/2012 11:21:17
//
//  @return       virtual :
//  */
/*-----------------------------------------------------------------*/
DIOPING::~DIOPING()
{
  DeRegisterEvent(DIOPINGXEVENTTYPE_DOPING);

  DeleteAllReplys();


  if(urltarget) diofactory->DeleteURL(urltarget);

  if(xmutexreplys) xfactory->Delete_Mutex(xmutexreplys);

  if(xtimer) xfactory->DeleteTimer(xtimer);

  Clean();
}




/*-------------------------------------------------------------------
//  DIOPING::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/09/2012 12:07:30
//
//  @return       bool :
//  @param         :
//  @param        IPLocal :
*/
/*-----------------------------------------------------------------*/
bool DIOPING::Set(XCHAR* urltarget , XCHAR* IPLocal)
{
  if(this->urltarget) (*this->urltarget) = urltarget;

  if(IPLocal) this->IPLocal = IPLocal;

  return true;
}



/*-------------------------------------------------------------------
//  DIOPING::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/09/2012 12:07:41
//
//  @return       bool :
//  @param        urltarget :
//  @param        IPLocal :
*/
/*-----------------------------------------------------------------*/
bool DIOPING::Set(DIOURL& urltarget, XCHAR* IPLocal)
{
  return Set(urltarget.Get(), IPLocal);
}



/*-------------------------------------------------------------------
//  DIOPING::Do
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/05/2015 9:43:06
//
//  @return       bool :
//
//  @param        nretries :
//  @param        timebetweenchecks :
//  @param        exitfirstgoodreply :
*/
/*-----------------------------------------------------------------*/
bool DIOPING::Do(XDWORD nretries, XDWORD timebetweenchecks, bool exitfirstgoodreply)
{
  if(!diofactory)        return false;
  if(!urltarget)         return false;
  if(!xtimer)            return false;

  DIOSTREAMICMPCONFIG   diostreamICMPconfig;
  XSTRING               targetIP;

  DeleteAllReplys();

  urltarget->ResolveURL(targetIP);

  diostreamICMPconfig.SetMode(DIOSTREAMMODE_CLIENT);

  DIOSTREAMICMP* diostreamICMP  = (DIOSTREAMICMP*)diofactory->CreateStreamIO(&diostreamICMPconfig);
  if(!diostreamICMP) return false;

  if(!diostreamICMP->Open())
    {
      diofactory->DeleteStreamIO(diostreamICMP);
      return false;
    }

  if(!diostreamICMP->WaitToConnected(5))
    {
      diostreamICMP->Close();
      diofactory->DeleteStreamIO(diostreamICMP);
      return false;
    }


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
          diostreamICMP->GetInXBuffer()->Delete();
          diostreamICMP->GetOutXBuffer()->Delete();
          diostreamICMP->DeleteAllDatagrams();

          status = diostreamICMP->WriteDatagram(targetIP, (XBYTE*)&echorequest, sizeof(DIOPING_ECHOREQUEST));
          if(status) status = diostreamICMP->WaitToFlushOutXBuffer(3);
        }

      if(!status)
        {
          //XDEBUGTRACE_PRINTCOLOR(4, __L("Ping: not write packet! %s"), targetIP.Get());
          break;
        }

      while(1)
        {
          status = diostreamICMP->WaitToFilledReadingBuffer(sizeof(DIOPING_ECHOREPLY), 1);
          if(!status)
            {
              //XDEBUGTRACE_PRINTCOLOR(4, __L("Ping: not ready packet! %s"), targetIP.Get());
              if(exitfirstgoodreply)  nloop = 0;  else nloop--;
              break;
            }

          status = diostreamICMP->ReadDatagram(address, xbuffer);
          if(!status)
            {
              //XDEBUGTRACE_PRINTCOLOR(4, __L("Ping: not read packet! %s"), targetIP.Get());
              if(exitfirstgoodreply)  nloop = 0;  else nloop--;
              break;
            }

          if(xbuffer.GetSize() < sizeof(DIOPING_ECHOREPLY))
            {
              //XDEBUGTRACE_PRINTCOLOR(4, __L("Ping: invalid size packet! %s, "), targetIP.Get());
              continue;
            }

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

                              DIOPINGXEVENT xevent(this, DIOPINGXEVENTTYPE_DOPING);
                              xevent.SetPingReply((DIOPINGREPLY*)replys.Get(replys.GetSize()-1));
                              PostEvent(&xevent);

                              //XDEBUGTRACE_PRINTCOLOR(1, __L("Ping: received! %s %s"), fromIP.Get(), targetIP.Get());

                              if(exitfirstgoodreply)
                                {
                                  nloop = 0;
                                  break;
                                }

                              XSLEEP::GetInstance().MilliSeconds(timebetweenchecks);

                              nloop--;
                              break;
                            }
                           else
                            {
                              //XDEBUGTRACE_PRINTCOLOR(4, __L("Ping: application ID not equal! %s %s"), fromIP.Get(), targetIP.Get());
                            }
                        }
                       else
                        {
                          //XDEBUGTRACE_PRINTCOLOR(4, __L("Ping: not IP equal! %s %s"), fromIP.Get(), targetIP.Get());
                        }

                    }
                   else
                    {
                      //XDEBUGTRACE_PRINTCOLOR(4, __L("Ping: error Checksum! %s "), targetIP.Get());
                    }
                }
               else
                {
                  //XDEBUGTRACE_PRINTCOLOR(4, __L("Ping: error CRC! %s "), targetIP.Get());
                }
            }
           else
            {
              //XDEBUGTRACE_PRINTCOLOR(4, __L("Ping: Is not reply packet! %s "), targetIP.Get());
            }
        }
   }

  diostreamICMP->Close();
  diofactory->DeleteStreamIO(diostreamICMP);

  bool status = WasConnected();

  /*
  if(!status)
    {
     XDEBUGTRACE_PRINTCOLOR((status?1:4), __L("Ping to [%s]: %s"), targetIP.Get(), (status?__L("Ok."): __L("Error!")));
    }
  */

  return status;
}




/*-------------------------------------------------------------------
//  DIOPING::WasConnected
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/09/2012 15:58:04
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOPING::WasConnected()
{
  if(!xmutexreplys) return false;

  bool status = false;

  xmutexreplys->Lock();
  if(!replys.IsEmpty()) status = true;
  xmutexreplys->UnLock();

  return status;
}





/*-------------------------------------------------------------------
//  DIOPING::GetReplys
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/09/2012 15:38:28
//
//  @return       XVECTOR<DIOPINGREPLY*>* :
//  */
/*-----------------------------------------------------------------*/
XVECTOR<DIOPINGREPLY*>* DIOPING::GetReplys()
{
  return &replys;
}





/*-------------------------------------------------------------------
//  DIOPING::CalculateMeanTime
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/09/2012 9:55:26
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOPING::CalculateMeanTime()
{
  if(!xmutexreplys) return 0;

  int meantime = 0;

  xmutexreplys->Lock();

  if(!replys.IsEmpty())
    {
      for(XDWORD c=0;c<replys.GetSize();c++)
        {
          DIOPINGREPLY* pinganswer = (DIOPINGREPLY*)replys.Get(c);
          if(pinganswer)
            {
              meantime += pinganswer->GetTimeElapsed();
            }
        }

      meantime /= replys.GetSize();
    }

  xmutexreplys->UnLock();

  return meantime;
}




/*-------------------------------------------------------------------
//  DIOPING::DeleteAllReplys
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/09/2012 15:40:43
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOPING::DeleteAllReplys()
{
  if(!xmutexreplys) return false;

  bool status = false;

  xmutexreplys->Lock();

  if(!replys.IsEmpty())
    {
      replys.DeleteContents();
      replys.DeleteAll();

      status = true;
    }

  xmutexreplys->UnLock();

  return status;
}







/*-------------------------------------------------------------------
//  DIOPING::AddReply
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/02/2013 21:11:43
//
//  @return       bool :
//  @param        sequence :
//  @param        fromIP :
//  @param        sizerequest :
//  @param        timeelapsed :
//  @param        TTL :
*/
/*-----------------------------------------------------------------*/
bool DIOPING::AddReply(XWORD sequence, XCHAR* fromIP, XDWORD sizerequest, XDWORD timeelapsed, XBYTE TTL)
{
  if(!xmutexreplys) return false;

  bool status = false;

  xmutexreplys->Lock();

  DIOPINGREPLY* pinganswer = new DIOPINGREPLY();
  if(pinganswer)
    {
      pinganswer->SetSequence(sequence);
      pinganswer->GetFromIP()->Set(fromIP);
      pinganswer->SetSizeRequest(sizerequest);
      pinganswer->SetTimeElapsed(timeelapsed);
      pinganswer->SetTTL(TTL);

      replys.Add(pinganswer);

      status = true;
    }

  xmutexreplys->UnLock();

  return status;
}




/*-------------------------------------------------------------------
//  DIOPING::CalculeCheckSum
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/05/2015 10:20:25
//
//  @return       XWORD :
//
//  @param        *addr :
//  @param        len :
*/
/*-----------------------------------------------------------------*/
XWORD DIOPING::CalculeCheckSum(XWORD *addr, int len)
{
  int     nleft   = len;
  XWORD*  w       = addr;
  XWORD   answer;
  int     sum     = 0;

  // Our algorithm is simple, using a 32 bit accumulator (sum),
  // we add sequential 16 bit words to it, and at the end, fold
  // back all the carry bits from the top 16 bits into the lower
  // 16 bits.

  while(nleft>1)
    {
      sum   += *w++;
      nleft -= 2;
    }

  // mop up an odd byte, if necessary

  if( nleft == 1 )
    {
      XWORD u = 0;

      *(XBYTE*)(&u) = *(XBYTE*)w ;
      sum += u;
    }


  // add back carry outs from top 16 bits to low 16 bits

  sum     = (sum >> 16) + (sum & 0xffff);   // add hi 16 to low 16
  sum    += (sum >> 16);                    // add carry
  answer  = ~sum;                           // truncate to 16 bits

  return answer;
}
