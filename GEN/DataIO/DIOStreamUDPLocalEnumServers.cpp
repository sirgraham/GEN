//------------------------------------------------------------------------------------------
//  DIOSTREAMUDPLOCALENUMSERVERS.CPP
//
//  Data IO Stream TCP/IP Local Enum Servers class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XBuffer.h"
#include "XThreadCollected.h"

#include "HashCRC32.h"

#include "DIOFactory.h"
#include "DIOStreamDeviceIP.h"
#include "DIOStreamUDPConfig.h"
#include "DIOStreamUDP.h"

#include "DIOStreamUDPLocalEnumServers.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTREAMUDPLOCALENUMSERVERS::DIOSTREAMUDPLOCALENUMSERVERS
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2013 23:23:16
//
//  @return       void :
*/
/*-----------------------------------------------------------------*/
DIOSTREAMUDPLOCALENUMSERVERS::DIOSTREAMUDPLOCALENUMSERVERS() : DIOSTREAMENUMSERVERS()
{
  Clean();

  xthread  = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUDP, __L("DIOSTREAMUDPLOCALENUMSERVERS::DIOSTREAMUDPLOCALENUMSERVERS"), ThreadDiscoveryLocalServer, this);
  if(xthread) xthread->Ini(false);

  if(diofactory)
    {
      diostreamudpcfg = new DIOSTREAMUDPCONFIG();
      diostreamudp    = (DIOSTREAMUDP*)diofactory->CreateStreamIO(diostreamudpcfg);
    }
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDPLOCALENUMSERVERS::~DIOSTREAMUDPLOCALENUMSERVERS
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2013 23:24:57
//
//  @return       virtual :
//  */
/*-----------------------------------------------------------------*/
DIOSTREAMUDPLOCALENUMSERVERS::~DIOSTREAMUDPLOCALENUMSERVERS()
{
  if(diofactory)
    {
      diofactory->DeleteStreamIO(diostreamudp);
      delete diostreamudpcfg;
    }

  if(xthread)
    {
      xthread->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUDP, xthread);
    }

  Clean();
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDPLOCALENUMSERVERS::Search
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2013 23:36:43
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDPLOCALENUMSERVERS::Search()
{
  if(!xthread)    return false;
  if(issearching) return false;

  DelAllDevices();

  issearching = true;

  return xthread->Run(true);
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDPLOCALENUMSERVERS::StopSearch
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2013 23:36:52
//
//  @return       bool :
//  @param        waitend :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDPLOCALENUMSERVERS::StopSearch(bool waitend)
{
  if(!xthread)       return false;
  if(!issearching)   return false;

  issearching = false;

  xthread->Run(false);

  xthread->Wait();

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDPLOCALENUMSERVERS::IsDeviceAvailable
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/05/2013 23:37:16
//
//  @return       bool :
//  @param        resource :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDPLOCALENUMSERVERS::IsDeviceAvailable(XCHAR* resource)
{
  for(int c=0;c<(int)GetDevices()->GetSize();c++)
    {
      DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)GetDevices()->Get(c);
      if(device)
        {
          XSTRING IPstring;

          device->GetIP()->GetXString(IPstring);

          if(!IPstring.Compare(resource))
            {
              return true;
            }
        }
    }

  return false;
}



/*-------------------------------------------------------------------
//  DIOSTREAMUDPLOCALENUMSERVERS::GetDIOStreamUDPCFG
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/05/2013 1:30:26
//
//  @return       DIOSTREAMUDPCONFIG* :
//  */
/*-----------------------------------------------------------------*/
DIOSTREAMUDPCONFIG* DIOSTREAMUDPLOCALENUMSERVERS::GetDIOStreamUDPCFG()
{
  return diostreamudpcfg;
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDPLOCALENUMSERVERS::GetDIOStreamUDP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/05/2013 1:30:34
//
//  @return       DIOSTREAMUDP* :
//  */
/*-----------------------------------------------------------------*/
DIOSTREAMUDP* DIOSTREAMUDPLOCALENUMSERVERS::GetDIOStreamUDP()
{
  return diostreamudp;
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDPLOCALENUMSERVERS::SendSignal
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2013 23:37:48
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDPLOCALENUMSERVERS::SendSignal()
{
  if(!diostreamudp)     return false;
  if(!diostreamudpcfg)  return false;

  diostreamudpcfg->SetMode(DIOSTREAMMODE_CLIENT);
  diostreamudpcfg->SetBroadcastModeActive(true);
  diostreamudpcfg->SetRemotePort(port);

  if(diostreamudp->Open())
    {
      XSTRING     string;
      XBUFFER     xbuffer;
      HASHCRC32   crc32;

      string.Format(DIOSTREAMUDPLOCALENUMSERVERSID,ID);

      XSTRING_CREATENORMALIZE(string, buffnormalize, false)

      crc32.Do((XBYTE*)buffnormalize,  string.GetSize()*sizeof(XWORD));

      xbuffer.Add((XDWORD)crc32.GetResultCRC32());
      xbuffer.Add((XBYTE*)buffnormalize, string.GetSize()*sizeof(XWORD));
      xbuffer.Add((XWORD)0);

      XSTRING_DELETENORMALIZE(buffnormalize)

      diostreamudp->Write(xbuffer.Get(),xbuffer.GetSize());

      diostreamudp->WaitToFlushXBuffers(5);

      diostreamudp->Close();
    }

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDPLOCALENUMSERVERS::GetPort
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/05/2011 12:03:25
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOSTREAMUDPLOCALENUMSERVERS::GetPort()
{
  return port;
}



/*-------------------------------------------------------------------
//  DIOSTREAMUDPLOCALENUMSERVERS::SetPort
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/05/2011 11:55:20
//
//  @return       bool :
//  @param        port :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDPLOCALENUMSERVERS::SetPort(int port)
{
  this->port = port;

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDPLOCALENUMSERVERS::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2013 23:37:54
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOSTREAMUDPLOCALENUMSERVERS::Clean()
{
  type             = DIOSTREAMENUMTYPE_TCPIP_LOCAL_SERVERS;

  diostreamudpcfg  = NULL;
  diostreamudp     = NULL;

  port             = 0;

  xthread          = NULL;
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDPLOCALENUMSERVERS::ThreadDiscoveryLocalServer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/04/2013 23:38:10
//
//  @return       void :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
void DIOSTREAMUDPLOCALENUMSERVERS::ThreadDiscoveryLocalServer(void* data)
{
  DIOSTREAMUDPLOCALENUMSERVERS* discoveryenum = (DIOSTREAMUDPLOCALENUMSERVERS*)data;
  if(!discoveryenum) return;

  if(!discoveryenum->GetDIOStreamUDP())    return;
  if(!discoveryenum->GetDIOStreamUDPCFG()) return;

  discoveryenum->issearching = true;

  DIOSTREAMUDPCONFIG* diostreamudpcfg = discoveryenum->GetDIOStreamUDPCFG();
  DIOSTREAMUDP*       diostreamudp    = discoveryenum->GetDIOStreamUDP();

  diostreamudpcfg->SetMode(DIOSTREAMMODE_SERVER);
  diostreamudpcfg->SetBroadcastModeActive(true);
  diostreamudpcfg->SetRemotePort(discoveryenum->GetPort());

  discoveryenum->xtimer->Reset();

  if(diostreamudp->Open())
    {
        while(discoveryenum->issearching)
          {
            if(discoveryenum->searchtimeout)
              {
                if(discoveryenum->xtimer->GetMeasureSeconds()>=discoveryenum->searchtimeout) break;
              }

            XBUFFER  data;
            XSTRING  address;
            XWORD    port;

            if(diostreamudp->ReadDatagram(address, port, data))
              {
                XSTRING     string;
                XSTRING     string2(256);
                XDWORD      ID = discoveryenum->GetID();
                HASHCRC32   crc32;
                XDWORD      crc32result[2];

                data.Extract(crc32result[0]);

                string.Format(DIOSTREAMUDPLOCALENUMSERVERSID,ID);

                string2.Set((XWORD*)data.Get());
                string2.AdjustSize();

                if(!string.Compare(string2))
                  {
                    XSTRING_CREATENORMALIZE(string2, buffnormalize, false)
                    crc32.Do((XBYTE*)buffnormalize, string2.GetSize()*sizeof(XWORD));
                    XSTRING_DELETENORMALIZE(buffnormalize)

                    crc32result[1] = crc32.GetResultCRC32();

                    if(crc32result[0] == crc32result[1])
                      {
                        if(!discoveryenum->IsDeviceAvailable(address.Get()))
                          {
                            DIOSTREAMDEVICEIP* device = new DIOSTREAMDEVICEIP();
                            if(device)
                              {
                                device->GetIP()->Set(address);
                                discoveryenum->devices.Add(device);

                                if(!discoveryenum->searchtimeout) break;
                              }
                          }
                      }
                  }
              }
          }

      diostreamudp->WaitToFlushXBuffers(5);

      diostreamudp->Close();
    }

  discoveryenum->xthread->Run(false);

  discoveryenum->issearching = false;
}


