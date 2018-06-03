
/*------------------------------------------------------------------------------------------
//  DIOSTREAMUDPACKNOWLEDGE.CPP
//
//  Data IO Stream UDP with Acknowledge
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 24/11/2014 13:32:35
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XBuffer.h"
#include "XDebugTrace.h"
#include "XThreadCollected.h"

#include "DIOFactory.h"
#include "DIOStreamUDPConfig.h"
#include "DIOStreamUDP.h"

#include "HashCRC32.h"

#include "DIOStreamUDPAcknowledge.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOSTREAMUDPACKNOWLEDGE::DIOSTREAMUDPACKNOWLEDGE
*/
/**
//
//  Class Constructor DIOSTREAMUDPACKNOWLEDGE
//
//  @author       Abraham J. Velez
//  @version      24/11/2014 16:20:05
//



//  @param        config :
//  @param        cipher :
*/
/*-----------------------------------------------------------------*/
DIOSTREAMUDPACKNOWLEDGE::DIOSTREAMUDPACKNOWLEDGE(DIOSTREAMUDPCONFIG* config) : DIOSTREAMUDP()
{
  Clean();

  this->config      = config;

  timeout           = DIOSTREAMUDPACKNOWLEDGE_DEFAULTTIMEOUT;

  diostream = (DIOSTREAMUDP*)diofactory->CreateStreamIO((DIOSTREAMCONFIG*)config);
  if(diostream)  URLremote = diofactory->CreateURL();
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDPACKNOWLEDGE::~DIOSTREAMUDPACKNOWLEDGE
*/
/**
//
//   Class Destructor DIOSTREAMUDPACKNOWLEDGE
//
//  @author       Abraham J. Velez
//  @version      24/11/2014 13:34:46
//
*/
/*-----------------------------------------------------------------*/
DIOSTREAMUDPACKNOWLEDGE::~DIOSTREAMUDPACKNOWLEDGE()
{
  Close();

  if(diostream) diofactory->DeleteStreamIO(diostream);
  if(URLremote) diofactory->DeleteURL(URLremote);

  Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMUDPACKNOWLEDGE::Open
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/03/2015 12:23:23
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDPACKNOWLEDGE::Open()
{
  if(!diostream) return false;
  if(!URLremote) return false;
  if(!config) return false;

  if(config->GetRemoteURL()->GetSize())
    {
      URLremote->Set(config->GetRemoteURL()->Get());
      URLremote->ResolveURL(addressremote);
    }

  portremote = config->GetRemotePort();

  if(!diostream->Open()) return false;

  if(config->IsServer())
    {
      xthreadconnexion  = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUDP, __L("DIOSTREAMUDPACKNOWLEDGE::Open"), ThreadRunFunction, (void*)this);
      if(xthreadconnexion) xthreadconnexion->Ini();
    }

  return true;
}


/*-------------------------------------------------------------------
//  DIOSTREAMUDPACKNOWLEDGE::Disconnect
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/03/2015 12:23:19
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDPACKNOWLEDGE::Disconnect()
{
  if(!diostream) return false;

  if(!config) return false;

  if(config->IsServer())
    {
      if(xthreadconnexion)
        {
          xthreadconnexion->End();
          DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUDP, xthreadconnexion);
          xthreadconnexion = NULL;
        }
    }

  if(!diostream->Disconnect()) return false;

  return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMUDPACKNOWLEDGE::Close
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/03/2015 12:25:41
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDPACKNOWLEDGE::Close()
{
  if(!diostream) return false;

  if(!config) return false;

  if(config->IsServer())
    {
      if(xthreadconnexion)
        {
          xthreadconnexion->End();
          DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUDP, xthreadconnexion);
          xthreadconnexion = NULL;
        }
    }

  if(!diostream->Close()) return false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDPACKNOWLEDGE::Write
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/02/2015 12:31:00
//
//  @return       XDWORD :
//
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTREAMUDPACKNOWLEDGE::Write(XBYTE* buffer, XDWORD size)
{
  if(!diostream)      return 0;
  if(!size)           return 0;

  if(addressremote.IsEmpty()) return 0;

  DIOSTREAMUDP* diostreamudp = (DIOSTREAMUDP*)diostream;
  XDWORD        bw           = 0;

  if(diostreamudp->WriteDatagram(addressremote, portremote, buffer, size)) bw = size;

  if(diostreamudp->WaitToWriteDatagramsEmpty(timeout))
    {
      //XDEBUGTRACE_PRINTCOLOR(1, __L("Write UDP %s:%d : %s"), addressremote.Get(), portremote, bw==size?__L("Ok"):__L("Error!"));

      if(diostreamudp->WaitToGetFirstDatagram(false, timeout) != DIOSTREAMUDP_NOTFOUND)
        {
          XSTRING address;
          XWORD   port       = 0;
          XBUFFER xbuffer;
          XDWORD  hashresult[2] = { 0, 0 };

          diostreamudp->ReadDatagram(address, port, xbuffer);

          xbuffer.Extract(hashresult[0]);

          HASHCRC32  hashcrc32;

          hashcrc32.ResetResult();
          hashcrc32.Do(buffer, size);

          hashresult[1] = hashcrc32.GetResultCRC32();

          if(hashresult[0] != hashresult[1])
            {
              //XDEBUGTRACE_PRINTCOLOR(1, __L("Write received bad CRC!! %08X <> %08X"), hashresult, hashcrc32.GetResultCRC32());
              bw = 0;
            }
        }
       else
        {
          //XDEBUGTRACE_PRINTCOLOR(1, __L("Write received NOT read"));
          bw = 0;
        }
    }
   else
    {
      //XDEBUGTRACE_PRINTCOLOR(1, __L("Do not write"));
      bw = 0;
    }

  if(!bw)
    {
      diostream->Close();
      diostream->Open();
      diostream->WaitToConnected(5);
    }

  return bw;
}



/*-------------------------------------------------------------------
//  DIOSTREAMUDPACKNOWLEDGE::WriteDatagram
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      16/03/2015 13:27:19
//
//  @return       bool :
//
//  @param        address :
//  @param        port :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDPACKNOWLEDGE::WriteDatagram(XSTRING& address, XWORD port, XBYTE* buffer, XDWORD size)
{
  XSTRING old_address;
  int     old_port;

  old_address   = addressremote.Get();
  old_port      = portremote;

  addressremote = address.Get();
  portremote    = port;

  int _size = Write(buffer, size);

  addressremote = old_address.Get();
  portremote    = old_port;

  return (size == _size)?true:false;
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDPACKNOWLEDGE::WriteDatagram
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      16/03/2015 13:27:14
//
//  @return       bool :
//
//  @param        address :
//  @param        port :
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDPACKNOWLEDGE::WriteDatagram(XSTRING& address, XWORD port, XBUFFER& xbuffer)
{
  return WriteDatagram(address, port, xbuffer.Get(), xbuffer.GetSize());
}





/*-------------------------------------------------------------------
//  DIOSTREAMUDPACKNOWLEDGE::ThreadRunFunction
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/02/2015 8:34:17
//
//  @param        param :
*/
/*-----------------------------------------------------------------*/
void DIOSTREAMUDPACKNOWLEDGE::ThreadRunFunction(void* param)
{
  DIOSTREAMUDPACKNOWLEDGE* diostreamudpacknowledge = (DIOSTREAMUDPACKNOWLEDGE*)param;
  if(!diostreamudpacknowledge) return;

  DIOSTREAMUDP* diostreamudp = diostreamudpacknowledge->diostream;
  if(!diostreamudp) return;

  XBUFFER  data;
  XSTRING  address;
  XWORD    port;
  bool     status = false;

  if(diostreamudp->ReadDatagram(address, port, data))
    {
      HASHCRC32 hashcrc32;

      hashcrc32.ResetResult();
      if(hashcrc32.Do(data.Get(), data.GetSize()))
        {
          XBUFFER result;
          XDWORD  hashresult = hashcrc32.GetResultCRC32();

          result.Add(hashresult);

          diostreamudp->WriteDatagram(address, port, result);
          if(diostreamudp->WaitToWriteDatagramsEmpty(diostreamudpacknowledge->timeout))
            {
              diostreamudpacknowledge->AddDatagram(false, address, port, data.Get(), data.GetSize());
            }
        }
  }
}

