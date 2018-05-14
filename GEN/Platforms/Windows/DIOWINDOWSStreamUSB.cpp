//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSB.CPP
//
//  WINDOWS Data IO Stream USB class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/01/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


#if defined(DIO_ACTIVE) && defined(DIOUSB_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XString.h"
#include "XBuffer.h"
#include "XThreadCollected.h"

#include "DIOStreamXEvent.h"

#include "DIOWINDOWSStreamUSB.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSB::DIOWINDOWSSTREAMUSB
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/02/2013 7:47:09
//
//  @return       void :



*/
/*-----------------------------------------------------------------*/
DIOWINDOWSSTREAMUSB::DIOWINDOWSSTREAMUSB() : DIOSTREAMUSB() , XFSMACHINE(0)
{
  Clean();

  AddState( DIOWINDOWSUSBFSMSTATE_NONE                ,
            DIOWINDOWSUSBFSMEVENT_CONNECTED           , DIOWINDOWSUSBFSMSTATE_CONNECTED        ,
            DIOWINDOWSUSBFSMEVENT_DISCONNECTING       , DIOWINDOWSUSBFSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);

  AddState( DIOWINDOWSUSBFSMSTATE_CONNECTED           ,
            DIOWINDOWSUSBFSMEVENT_WAITINGTOREAD       , DIOWINDOWSUSBFSMSTATE_WAITINGTOREAD    ,
            DIOWINDOWSUSBFSMEVENT_SENDINGDATA         , DIOWINDOWSUSBFSMSTATE_SENDINGDATA      ,
            DIOWINDOWSUSBFSMEVENT_DISCONNECTING       , DIOWINDOWSUSBFSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);

  AddState( DIOWINDOWSUSBFSMSTATE_WAITINGTOREAD       ,
            DIOWINDOWSUSBFSMEVENT_CONNECTED           , DIOWINDOWSUSBFSMSTATE_CONNECTED        ,
            DIOWINDOWSUSBFSMEVENT_SENDINGDATA         , DIOWINDOWSUSBFSMSTATE_SENDINGDATA      ,
            DIOWINDOWSUSBFSMEVENT_DISCONNECTING       , DIOWINDOWSUSBFSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);

  AddState( DIOWINDOWSUSBFSMSTATE_DISCONNECTING       ,
            DIOWINDOWSUSBFSMEVENT_CONNECTED           , DIOWINDOWSUSBFSMSTATE_CONNECTED        ,
            DIOWINDOWSUSBFSMEVENT_WAITINGTOREAD       , DIOWINDOWSUSBFSMSTATE_WAITINGTOREAD    ,
            DIOWINDOWSUSBFSMEVENT_SENDINGDATA         , DIOWINDOWSUSBFSMSTATE_SENDINGDATA      ,
            EVENTDEFEND);

  threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUSB, __L("DIOWINDOWSSTREAMUSB::DIOWINDOWSSTREAMUSB"), ThreadConnexion, (void*)this);
}


//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSB::~DIOWINDOWSSTREAMUSB
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
DIOWINDOWSSTREAMUSB::~DIOWINDOWSSTREAMUSB()
{
  Close();

  if(threadconnexion) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUSB, threadconnexion);

  Clean();
}


//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSB::GetConnectStatus
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       DIOSTREAMSTATUS :
*/
//-------------------------------------------------------------------
DIOSTREAMSTATUS DIOWINDOWSSTREAMUSB::GetConnectStatus()
{
  if(handle==INVALID_HANDLE_VALUE)  return DIOSTREAMSTATUS_DISCONNECTED;
  if(!config)                       return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSB::Open
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool DIOWINDOWSSTREAMUSB::Open()
{
  if(!threadconnexion)  return false;

  if(!inbuffer)         return false;
  if(!outbuffer)        return false;

  if(!config)           return false;

  XSTRING comport;
  XSTRING cfg;


  handle = CreateFile(config->GetResource()->Get(), GENERIC_READ | GENERIC_WRITE
                                                  , FILE_SHARE_READ | FILE_SHARE_WRITE
                                                  , NULL
                                                  , OPEN_EXISTING
                                                  , FILE_FLAG_OVERLAPPED
                                                  , NULL);


  if(handle==INVALID_HANDLE_VALUE) return false;

  memset(&ovi,0,sizeof(ovi));

  SetEvent(DIOWINDOWSUSBFSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();

  ResetConnexionStatistics();

  return threadconnexion->Ini();
}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSB::Close
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
bool DIOWINDOWSSTREAMUSB::Close()
{
  if(!threadconnexion) return false;

  threadconnexion->End();

  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  CloseHandle(handle);

  handle=INVALID_HANDLE_VALUE;

  return true;
}




//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSB::CleanBuffers
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
bool DIOWINDOWSSTREAMUSB::CleanBuffers()
{
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  return true;
}




//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSB::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
void DIOWINDOWSSTREAMUSB::Clean()
{
  threadconnexion = NULL;

  handle            = INVALID_HANDLE_VALUE;
  hevent          = NULL;

  memset(&ovi,0,sizeof(ovi));

  readtimeout  = 3000;
  writetimeout = 3000;
}





/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSB::ThreadConnexion
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/07/2012 13:56:19
//
//  @return       void :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
void DIOWINDOWSSTREAMUSB::ThreadConnexion(void* data)
{
  DIOWINDOWSSTREAMUSB* diostream = (DIOWINDOWSSTREAMUSB*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOWINDOWSUSBFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOWINDOWSUSBFSMSTATE_NONE                 : break;

          case DIOWINDOWSUSBFSMSTATE_CONNECTED              : break;

          case DIOWINDOWSUSBFSMSTATE_WAITINGTOREAD          : { if(!diostream->IsBlockRead())
                                                                  {
                                                                    XBYTE buffer[DIOSTREAM_MAXBUFFER];

                                                                    XDWORD br = diostream->ReadBuffer(buffer, DIOSTREAM_MAXBUFFER);
                                                                    if(br)
                                                                      {
                                                                        diostream->inbuffer->Add(buffer, br);
                                                                      }
                                                                  }

                                                                if(::GetLastError() == 5)
                                                                  {
                                                                    diostream->SetEvent(DIOWINDOWSUSBFSMEVENT_DISCONNECTING);
                                                                    break;
                                                                  }

                                                                if(!diostream->IsBlockWrite())
                                                                  {
                                                                    int esize = diostream->outbuffer->GetSize();
                                                                    if(esize)
                                                                      {
                                                                        diostream->outbuffer->SetBlocked(true);
                                                                        XDWORD bw = diostream->WriteBuffer(diostream->outbuffer->Get(), esize);
                                                                        diostream->outbuffer->SetBlocked(false);
                                                                        if(bw) diostream->outbuffer->Extract(NULL, 0, bw);
                                                                      }
                                                                  }
                                                              }
                                                              break;

          case DIOWINDOWSUSBFSMSTATE_SENDINGDATA            : break;

          case DIOWINDOWSUSBFSMSTATE_DISCONNECTING          : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOWINDOWSUSB_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOWINDOWSUSBFSMSTATE_NONE             : break;

              case DIOWINDOWSUSBFSMSTATE_CONNECTED          : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);

                                                                diostream->PostEvent(&xevent);

                                                                diostream->SetEvent(DIOWINDOWSUSBFSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOWINDOWSUSBFSMSTATE_WAITINGTOREAD      : break;

              case DIOWINDOWSUSBFSMSTATE_SENDINGDATA        : break;

              case DIOWINDOWSUSBFSMSTATE_DISCONNECTING      : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_DISCONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                diostream->PostEvent(&xevent);

                                                                diostream->threadconnexion->Run(false);
                                                                diostream->status       = DIOSTREAMSTATUS_DISCONNECTED;
                                                              }
                                                              break;
            }
        }
    }

}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSB::ReadBuffer
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       XDWORD :
//  @param        buffer :
//  @param        size :
*/
//------------------------------------------------------------------
/*
XDWORD DIOWINDOWSSTREAMUSB::ReadBuffer(XBYTE* buffer,XDWORD size)
{
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;
  if(size == 0)                                        return 0;

  DWORD br    = 0;
  bool status = true;

  if(!hevent)
    {
      hevent = CreateEvent(0,TRUE,FALSE,0);
      if(!hevent) return 0;

      ovi.hEvent = hevent;

      if(!ReadFile(handle,buffer,size,&br,&ovi))
        {
          if(GetLastError()!=ERROR_IO_PENDING)
            {
              status = false;
            }
           else return ReadBuffer(buffer,size);
        }
    }
   else
    {
      switch(WaitForSingleObject(ovi.hEvent,readtimeout))
        {
          case WAIT_OBJECT_0: if(!GetOverlappedResult(handle,&ovi,&br,FALSE))
                                     status = false;
                                else status = true;
                              break;

          case WAIT_TIMEOUT : CancelIo(handle);
                              status=false;
                              break;

                   default  : status=false;
                              break;

         }
     }

  memset(&ovi,0,sizeof(ovi));

  if(hevent)
    {
      CloseHandle(hevent);
      hevent = NULL;
    }

  if(!status) return 0;

  return (XDWORD)br;
}
*/


XDWORD DIOWINDOWSSTREAMUSB::ReadBuffer(XBYTE* buffer,XDWORD size)
{
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;
  if(size == 0)                                        return 0;

  DWORD br    = 0;
  bool status = true;

  status = ReadFile(handle, buffer, size, &br, NULL)?true:false;

  if(!status) return 0;
  return (XDWORD)br;

}





//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUSB::Write
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       XDWORD :
//  @param        buffer :
//  @param        size :
*/
//--------------------------------------------------------------------
XDWORD DIOWINDOWSSTREAMUSB::WriteBuffer(XBYTE* buffer, XDWORD size)
{
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  if(size == 0)    return 0;

  DWORD      bw         = 0;
  OVERLAPPED oviwrite   = { 0 };
  bool       status;

  oviwrite.hEvent = CreateEvent(0,TRUE,FALSE,0);
  if(!oviwrite.hEvent) return bw;

  if(!WriteFile(handle,buffer,size,&bw,&oviwrite))
    {
      if(GetLastError()!=ERROR_IO_PENDING)
        {
          status = false;
        }
       else
        {
          switch(WaitForSingleObject(oviwrite.hEvent,writetimeout))
            {
              case WAIT_OBJECT_0 :  if(!GetOverlappedResult(handle,&oviwrite,&bw,TRUE))
                                           status = false;
                                      else status = true;
                                    break;

              case WAIT_TIMEOUT  : CancelIo(handle);
                                   status=false;
                                   break;

                       default   : status=false;
                                   break;

            }
        }

    } else status = true;

  CloseHandle(oviwrite.hEvent);

  if(!status) return 0;

  return (XDWORD)bw;
}


/*
XDWORD DIOWINDOWSSTREAMUSB::WriteBuffer(XBYTE* buffer, XDWORD size)
{
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;
  if(size == 0)                                        return 0;

  DWORD bw    = 0;
  bool status = true;

  status = WriteFile(handle, buffer, size, &bw, NULL)?true:false;

  if(!status) return 0;

  return (XDWORD)bw;
}
*/



#endif