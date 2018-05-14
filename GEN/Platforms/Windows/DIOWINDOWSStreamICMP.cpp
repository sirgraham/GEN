//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMICMP.CPP
//
//  WINDOWS Data IO Stream ICMP class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/01/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#if defined(DIO_ACTIVE) && defined(DIOICMP_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>


#include "XFactory.h"
#include "XBuffer.h"
#include "XDebug.h"
#include "XThreadCollected.h"

#include "DIOIP.h"
#include "DIOURL.h"
#include "DIOStreamXEvent.h"
#include "DIOStreamICMPConfig.h"

#include "DIOWINDOWSStreamICMP.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMICMP::DIOWINDOWSSTREAMICMP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/02/2013 7:42:19
//
//  @return       void :

*/
/*-----------------------------------------------------------------*/
DIOWINDOWSSTREAMICMP::DIOWINDOWSSTREAMICMP() : DIOSTREAMICMP(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOWINDOWSICMPFSMSTATE_NONE                 ,
            DIOWINDOWSICMPFSMEVENT_GETTINGCONNEXION     , DIOWINDOWSICMPFSMSTATE_GETTINGCONNEXION   ,
            DIOWINDOWSICMPFSMEVENT_CONNECTED            , DIOWINDOWSICMPFSMSTATE_CONNECTED          ,
            DIOWINDOWSICMPFSMEVENT_DISCONNECTING        , DIOWINDOWSICMPFSMSTATE_DISCONNECTING      ,
            EVENTDEFEND);

  AddState( DIOWINDOWSICMPFSMSTATE_GETTINGCONNEXION     ,
            DIOWINDOWSICMPFSMEVENT_CONNECTED            , DIOWINDOWSICMPFSMSTATE_CONNECTED          ,
            DIOWINDOWSICMPFSMEVENT_WAITINGTOREAD        , DIOWINDOWSICMPFSMSTATE_WAITINGTOREAD      ,
            DIOWINDOWSICMPFSMEVENT_SENDINGDATA          , DIOWINDOWSICMPFSMSTATE_SENDINGDATA        ,
            DIOWINDOWSICMPFSMEVENT_DISCONNECTING        , DIOWINDOWSICMPFSMSTATE_DISCONNECTING      ,
            EVENTDEFEND);

  AddState( DIOWINDOWSICMPFSMSTATE_CONNECTED            ,
            DIOWINDOWSICMPFSMEVENT_GETTINGCONNEXION     , DIOWINDOWSICMPFSMSTATE_GETTINGCONNEXION   ,
            DIOWINDOWSICMPFSMEVENT_WAITINGTOREAD        , DIOWINDOWSICMPFSMSTATE_WAITINGTOREAD      ,
            DIOWINDOWSICMPFSMEVENT_SENDINGDATA          , DIOWINDOWSICMPFSMSTATE_SENDINGDATA        ,
            DIOWINDOWSICMPFSMEVENT_DISCONNECTING        , DIOWINDOWSICMPFSMSTATE_DISCONNECTING      ,
            EVENTDEFEND);

  AddState( DIOWINDOWSICMPFSMSTATE_WAITINGTOREAD        ,
            DIOWINDOWSICMPFSMEVENT_GETTINGCONNEXION     , DIOWINDOWSICMPFSMSTATE_GETTINGCONNEXION   ,
            DIOWINDOWSICMPFSMEVENT_CONNECTED            , DIOWINDOWSICMPFSMSTATE_CONNECTED          ,
            DIOWINDOWSICMPFSMEVENT_SENDINGDATA          , DIOWINDOWSICMPFSMSTATE_SENDINGDATA        ,
            DIOWINDOWSICMPFSMEVENT_DISCONNECTING        , DIOWINDOWSICMPFSMSTATE_DISCONNECTING      ,
            EVENTDEFEND);

  AddState( DIOWINDOWSICMPFSMSTATE_DISCONNECTING        ,
            DIOWINDOWSICMPFSMEVENT_GETTINGCONNEXION     , DIOWINDOWSICMPFSMSTATE_GETTINGCONNEXION   ,
            DIOWINDOWSICMPFSMEVENT_CONNECTED            , DIOWINDOWSICMPFSMSTATE_CONNECTED          ,
            DIOWINDOWSICMPFSMEVENT_WAITINGTOREAD        , DIOWINDOWSICMPFSMSTATE_WAITINGTOREAD      ,
            DIOWINDOWSICMPFSMEVENT_SENDINGDATA          , DIOWINDOWSICMPFSMSTATE_SENDINGDATA        ,
            EVENTDEFEND);

  threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMICMP, __L("DIOWINDOWSSTREAMICMP::DIOWINDOWSSTREAMICMP"),ThreadConnexion,(void*)this);
}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMICMP::~DIOWINDOWSSTREAMICMP
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
DIOWINDOWSSTREAMICMP::~DIOWINDOWSSTREAMICMP()
{
  if(threadconnexion) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMICMP, threadconnexion);

  Clean();
}




//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMICMP::Open
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool DIOWINDOWSSTREAMICMP::Open()
{
  if(!threadconnexion)  return false;

  if(!inbuffer)         return false;
  if(!outbuffer)        return false;

  SetEvent(DIOWINDOWSICMPFSMEVENT_GETTINGCONNEXION);

  status = DIOSTREAMSTATUS_GETTINGCONNEXION;

  ResetXBuffers();

  ResetConnexionStatistics();

  return threadconnexion->Ini();
}




/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMICMP::Disconnect
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/12/2010 23:10:56
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOWINDOWSSTREAMICMP::Disconnect()
{
  if((GetConnectStatus()==DIOSTREAMSTATUS_GETTINGCONNEXION)||
     (GetConnectStatus()==DIOSTREAMSTATUS_CONNECTED))
     {
       SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);

       while(GetConnectStatus()!=DIOSTREAMSTATUS_DISCONNECTED)
        {
          Sleep(10);
        }
     }

  return true;
}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMICMP::Close
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
bool DIOWINDOWSSTREAMICMP::Close()
{
  if(!threadconnexion) return false;

  threadconnexion->End();

  if(handle!=INVALID_SOCKET)
    {
      shutdown(handle,SD_BOTH);
      closesocket(handle);
      handle  = INVALID_SOCKET;
    }

  return true;
}




//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMICMP::IsReadyConnect
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/03/2006 15:36:59
//
//  @return       int :
//  @param        sock :
*/
//-------------------------------------------------------------------
int DIOWINDOWSSTREAMICMP::IsReadyConnect(SOCKET socket)
/*
{
  struct timeval  tv;
  int             rc;
  fd_set          fdr;
  fd_set          fdw;
  fd_set          fds;

  if(socket==INVALID_SOCKET) return -1;

  FD_ZERO(&fdr);
  FD_ZERO(&fdw);
  FD_ZERO(&fds);

  FD_SET((unsigned int)socket,&fdr);
  FD_SET((unsigned int)socket,&fdw);
  FD_SET((unsigned int)socket,&fds);

  tv.tv_sec  = 0;
  tv.tv_usec = 100;

  rc = select((int)(socket)+1, &fdr, &fdw, &fds, &tv);
  if(rc==SOCKET_ERROR) return -1;
  //if(rc>1)             return -1;

  int status1 = FD_ISSET(socket,&fdr) ? 1 : 0;
  int status2 = FD_ISSET(socket,&fdw) ? 1 : 0;
  int status3 = FD_ISSET(socket,&fds) ? 1 : 0;

  if(status1 || status2) return  1;
  if(status3)            return -1;

  return 0;
}
*/
{
  struct timeval  tv;
  int             rc;
  fd_set          fdr;
  fd_set          fdw;
  fd_set          fds;

  if(socket == INVALID_SOCKET) return -1;

  FD_ZERO(&fdr);
  FD_ZERO(&fdw);
  FD_ZERO(&fds);

  FD_SET((unsigned int)socket, &fdr);
  FD_SET((unsigned int)socket, &fdw);
  FD_SET((unsigned int)socket, &fds);

  tv.tv_sec  = 0;
  tv.tv_usec = 100;

  rc = select((int)(socket)+1,&fdr, &fdw, &fds, &tv);
  if(rc == SOCKET_ERROR) return -1;

  int status1 = FD_ISSET(socket, &fdr) ? 1 : 0;
  int status2 = FD_ISSET(socket, &fdw) ? 1 : 0;
  int status3 = FD_ISSET(socket, &fds) ? 1 : 0;

  if(config->IsServer())
    {
      if(status1 || status2 )   return  1;
      if(status3)               return -1;
    }
   else
    {
      if((!status1) && status2) return  1;
      if(status3)               return -1;
    }

  return 0;
}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMICMP::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
void DIOWINDOWSSTREAMICMP::Clean()
{
  threadconnexion   = NULL;
  status            = DIOSTREAMSTATUS_DISCONNECTED;
  handle            = INVALID_SOCKET;
}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMICMP::ThreadRunFunction
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2006 15:44:00
//
//  @return       void :
//  @param        data :
*/
//-------------------------------------------------------------------
void DIOWINDOWSSTREAMICMP::ThreadConnexion(void* data)
{
  DIOWINDOWSSTREAMICMP* diostream = (DIOWINDOWSSTREAMICMP*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOWINDOWSICMPFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOWINDOWSICMPFSMSTATE_NONE                : break;

          case DIOWINDOWSICMPFSMSTATE_GETTINGCONNEXION    : switch(diostream->IsReadyConnect(diostream->handle))
                                                            {
                                                              case -1:  diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                        break;

                                                              case  1:  diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_CONNECTED);
                                                                        diostream->status = DIOSTREAMSTATUS_CONNECTED;
                                                                        break;

                                                              default:  diostream->status = DIOSTREAMSTATUS_GETTINGCONNEXION;
                                                                        break;
                                                            }
                                                          break;

          case DIOWINDOWSICMPFSMSTATE_CONNECTED         : break;

          case DIOWINDOWSICMPFSMSTATE_WAITINGTOREAD     : { struct timeval  waitd;
                                                            fd_set          read_flags;
                                                            fd_set          write_flags;

                                                            if(diostream->handle==INVALID_SOCKET)
                                                              {
                                                                diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                break;
                                                              }

                                                            waitd.tv_sec  = 0;
                                                            waitd.tv_usec = 100;

                                                            FD_ZERO(&read_flags);
                                                            FD_ZERO(&write_flags);

                                                            FD_SET(diostream->handle, &read_flags);
                                                            if(diostream->GetFirstDatagram(true) != DIOSTREAMICMP_NOTFOUND) FD_SET(diostream->handle, &write_flags);

                                                            int error = select((int)(diostream->handle)+1, &read_flags, &write_flags, (fd_set*)0, &waitd);
                                                            if(error==SOCKET_ERROR)
                                                              {
                                                                diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                break;
                                                              }


                                                            if(FD_ISSET(diostream->handle, &read_flags))  //Socket ready for reading
                                                              {
                                                                FD_CLR(diostream->handle, &read_flags);

                                                                if(!diostream->IsBlockRead())
                                                                  {
                                                                    XBYTE buffer[DIOSTREAM_MAXBUFFER];

                                                                    SOCKADDR_IN  origin_addr;
                                                                    int          size_addr = sizeof(SOCKADDR_IN);

                                                                    memset(&origin_addr, 0, size_addr);

                                                                    int size = recvfrom(diostream->handle, (char*)buffer, DIOSTREAM_MAXBUFFER, 0,(sockaddr*)&origin_addr, &size_addr);

                                                                    if(size == SOCKET_ERROR)
                                                                      {
                                                                        diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                        break;
                                                                      }

                                                                    if(size)
                                                                      {
                                                                        XSTRING address;

                                                                        address.Format(__L("%d.%d.%d.%d") , origin_addr.sin_addr.S_un.S_un_b.s_b1
                                                                                                          , origin_addr.sin_addr.S_un.S_un_b.s_b2
                                                                                                          , origin_addr.sin_addr.S_un.S_un_b.s_b3
                                                                                                          , origin_addr.sin_addr.S_un.S_un_b.s_b4);

                                                                        diostream->AddDatagram(false, address.Get(), (XBYTE*)buffer,size);
                                                                        diostream->inbuffer->Add(buffer, size);

                                                                      }
                                                                     else
                                                                      {
                                                                        diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                        break;
                                                                      }
                                                                  }

                                                              }

                                                            if(FD_ISSET(diostream->handle, &write_flags))  //Socket ready for writing
                                                              {
                                                                FD_CLR(diostream->handle, &write_flags);

                                                                if(!diostream->IsBlockWrite())
                                                                  {
                                                                    int indexdatagram  = diostream->GetFirstDatagram(true);
                                                                    if(indexdatagram!=DIOSTREAMICMP_NOTFOUND)
                                                                      {
                                                                        DIOSTREAMICMPDATAGRAM* datagram = (DIOSTREAMICMPDATAGRAM*)diostream->GetDatagram(indexdatagram);
                                                                        if(datagram)
                                                                          {
                                                                            SOCKADDR_IN  target_addr;
                                                                            int          size_addr = sizeof(SOCKADDR_IN);
                                                                            int          size;
                                                                            XSTRING      tmpremoteaddress;

                                                                            memset(&target_addr, 0, size_addr);

                                                                            target_addr.sin_family = AF_INET;

                                                                            if(datagram->GetAddress()->IsEmpty())
                                                                                    tmpremoteaddress = diostream->remoteaddress.Get();
                                                                              else  tmpremoteaddress = datagram->GetAddress()->Get();

                                                                            XSTRING_CREATEOEM(tmpremoteaddress, charstr)
                                                                            #ifndef BUILDER
                                                                            inet_pton(target_addr.sin_family, charstr, &target_addr.sin_addr.s_addr);
                                                                            #else
                                                                            target_addr.sin_addr.s_addr   = inet_addr(charstr);
                                                                            #endif
                                                                            XSTRING_DELETEOEM(charstr)

                                                                            target_addr.sin_port  = 0;

                                                                            size = sendto(diostream->handle,(char*)datagram->GetData()->Get(), datagram->GetData()->GetSize(), 0, (sockaddr*)&target_addr, size_addr);

                                                                            if(size == SOCKET_ERROR)
                                                                              {
                                                                                //XDEBUG_PRINTCOLOR(4, __L("Write ICMP to [%s] (%d) ERROR!"), tmpremoteaddress.Get(), size);

                                                                                diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                                break;
                                                                              }

                                                                            if(size)
                                                                              {
                                                                                //XDEBUG_PRINTCOLOR(1, __L("Write ICMP to [%s] (%d)"), tmpremoteaddress.Get(), size);

                                                                                diostream->outbuffer->Extract(NULL, 0 , datagram->GetData()->GetSize());
                                                                                diostream->DeleteDatagram(indexdatagram);
                                                                              }
                                                                          }
                                                                      }
                                                                  }
                                                              }
                                                          }
                                                          break;

          case DIOWINDOWSICMPFSMSTATE_SENDINGDATA       : break;

          case DIOWINDOWSICMPFSMSTATE_DISCONNECTING     : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOWINDOWSICMP_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOWINDOWSICMPFSMSTATE_NONE                : break;

              case DIOWINDOWSICMPFSMSTATE_GETTINGCONNEXION    : { SOCKADDR_IN loc_addr;

                                                                  memset(&loc_addr, 0, sizeof(SOCKADDR_IN));

                                                                  diostream->handle = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
                                                                  if(diostream->handle == INVALID_SOCKET)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }

                                                                  if(diostream->config->IsServer() || (!diostream->config->GetLocalIP()->IsEmpty()))
                                                                    {
                                                                      XSTRING IPstring;

                                                                      diostream->config->GetLocalIP()->GetXString(IPstring);

                                                                      loc_addr.sin_family = AF_INET;

                                                                      if(!diostream->config->GetLocalIP()->IsEmpty())
                                                                        {
                                                                          XSTRING_CREATEOEM(IPstring, charstr)
                                                                          #ifndef BUILDER
                                                                          inet_pton(loc_addr.sin_family, charstr, &loc_addr.sin_addr.s_addr);
                                                                          #else
                                                                          loc_addr.sin_addr.s_addr  = inet_addr(charstr);
                                                                          #endif
                                                                          XSTRING_DELETEOEM(charstr)

                                                                        } else loc_addr.sin_addr.s_addr = htonl(INADDR_ANY);

                                                                      loc_addr.sin_port = 0;

                                                                      if(bind(diostream->handle, (LPSOCKADDR)&loc_addr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
                                                                        {
                                                                          diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                          break;
                                                                        }
                                                                    }


                                                                  if(!diostream->config->IsServer())
                                                                    {
                                                                      if(diostream->config->GetRemoteURL()->GetSize()) diostream->config->GetRemoteURL()->ResolveURL(diostream->remoteaddress);
                                                                    }

                                                                  /*
                                                                  int opt = 1;
                                                                  if(setsockopt(diostream->handle, IPPROTO_IP, IP_HDRINCL, (char*)&opt, sizeof(opt)) == SOCKET_ERROR)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }
                                                                  */

                                                                  DWORD nonblock = 1;
                                                                  if (ioctlsocket(diostream->handle, FIONBIO, &nonblock) == SOCKET_ERROR)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }
                                                                }
                                                                break;

              case DIOWINDOWSICMPFSMSTATE_CONNECTED         : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);

                                                                diostream->PostEvent(&xevent);

                                                                diostream->SetEvent(DIOWINDOWSICMPFSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOWINDOWSICMPFSMSTATE_WAITINGTOREAD     : break;

              case DIOWINDOWSICMPFSMSTATE_SENDINGDATA       : break;

              case DIOWINDOWSICMPFSMSTATE_DISCONNECTING     : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_DISCONNECTED);
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

#endif