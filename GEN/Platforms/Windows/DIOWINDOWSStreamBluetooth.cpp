//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTH.CPP
//
//  WINDOWS Data IO Stream Bluetooth class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/01/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#if defined(DIO_ACTIVE) && defined(DIOBLUETOOTH_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <winsock2.h>
#include <windows.h>
#include <ws2bth.h>
#include <bthsdpdef.h>
#include <bluetoothapis.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "XFactory.h"
#include "XDebug.h"
#include "XBuffer.h"
#include "XThreadCollected.h"

#include "DIOStreamBluetoothConfig.h"

#include "DIOWINDOWSStreamBluetooth.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTH::DIOWINDOWSSTREAMBLUETOOTH
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/06/2011 16:38:21
//
//  @return


*/
/*-----------------------------------------------------------------*/
DIOWINDOWSSTREAMBLUETOOTH::DIOWINDOWSSTREAMBLUETOOTH() : DIOSTREAMBLUETOOTH() , XFSMACHINE(0)
{
  Clean();

  AddState( DIOWINDOWSBTFSMSTATE_NONE               ,
            DIOWINDOWSBTFSMEVENT_GETTINGCONNEXION   , DIOWINDOWSBTFSMSTATE_GETTINGCONNEXION ,
            DIOWINDOWSBTFSMEVENT_CONNECTED            , DIOWINDOWSBTFSMSTATE_CONNECTED          ,
            DIOWINDOWSBTFSMEVENT_DISCONNECTING        , DIOWINDOWSBTFSMSTATE_DISCONNECTING      ,
            EVENTDEFEND);

  AddState( DIOWINDOWSBTFSMSTATE_GETTINGCONNEXION   ,
            DIOWINDOWSBTFSMEVENT_CONNECTED            , DIOWINDOWSBTFSMSTATE_CONNECTED          ,
            DIOWINDOWSBTFSMEVENT_WAITINGTOREAD        , DIOWINDOWSBTFSMSTATE_WAITINGTOREAD      ,
            DIOWINDOWSBTFSMEVENT_SENDINGDATA          , DIOWINDOWSBTFSMSTATE_SENDINGDATA        ,
            DIOWINDOWSBTFSMEVENT_DISCONNECTING        , DIOWINDOWSBTFSMSTATE_DISCONNECTING      ,
            EVENTDEFEND);

  AddState( DIOWINDOWSBTFSMSTATE_CONNECTED            ,
            DIOWINDOWSBTFSMEVENT_GETTINGCONNEXION   , DIOWINDOWSBTFSMSTATE_GETTINGCONNEXION ,
            DIOWINDOWSBTFSMEVENT_WAITINGTOREAD        , DIOWINDOWSBTFSMSTATE_WAITINGTOREAD      ,
            DIOWINDOWSBTFSMEVENT_SENDINGDATA          , DIOWINDOWSBTFSMSTATE_SENDINGDATA        ,
            DIOWINDOWSBTFSMEVENT_DISCONNECTING        , DIOWINDOWSBTFSMSTATE_DISCONNECTING      ,
            EVENTDEFEND);

  AddState( DIOWINDOWSBTFSMSTATE_WAITINGTOREAD        ,
            DIOWINDOWSBTFSMEVENT_GETTINGCONNEXION   , DIOWINDOWSBTFSMSTATE_GETTINGCONNEXION ,
            DIOWINDOWSBTFSMEVENT_CONNECTED            , DIOWINDOWSBTFSMSTATE_CONNECTED          ,
            DIOWINDOWSBTFSMEVENT_SENDINGDATA          , DIOWINDOWSBTFSMSTATE_SENDINGDATA        ,
            DIOWINDOWSBTFSMEVENT_DISCONNECTING        , DIOWINDOWSBTFSMSTATE_DISCONNECTING      ,
            EVENTDEFEND);

  AddState( DIOWINDOWSBTFSMSTATE_DISCONNECTING        ,
            DIOWINDOWSBTFSMEVENT_GETTINGCONNEXION   , DIOWINDOWSBTFSMSTATE_GETTINGCONNEXION ,
            DIOWINDOWSBTFSMEVENT_CONNECTED            , DIOWINDOWSBTFSMSTATE_CONNECTED          ,
            DIOWINDOWSBTFSMEVENT_WAITINGTOREAD        , DIOWINDOWSBTFSMSTATE_WAITINGTOREAD      ,
            DIOWINDOWSBTFSMEVENT_SENDINGDATA          , DIOWINDOWSBTFSMSTATE_SENDINGDATA        ,
            EVENTDEFEND);

  threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMBLUETOOTH, __L("DIOWINDOWSSTREAMBLUETOOTH::DIOWINDOWSSTREAMBLUETOOTH"), ThreadConnexion, (void*)this);
}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTH::~DIOWINDOWSSTREAMBLUETOOTH
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
DIOWINDOWSSTREAMBLUETOOTH::~DIOWINDOWSSTREAMBLUETOOTH()
{
  if(threadconnexion)
    {
      threadconnexion->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMBLUETOOTH, threadconnexion);
    }

  Clean();
}


//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTH::Open
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool DIOWINDOWSSTREAMBLUETOOTH::Open()
{
  if(!threadconnexion) return false;

  SetEvent(DIOWINDOWSBTFSMEVENT_GETTINGCONNEXION);
  status = DIOSTREAMSTATUS_GETTINGCONNEXION;

  ResetXBuffers();

  return threadconnexion->Ini();
}


/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTH::Disconnect
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
bool DIOWINDOWSSTREAMBLUETOOTH::Disconnect()
{
  if(!IsConnected()) return false;

  if((GetConnectStatus()==DIOSTREAMSTATUS_GETTINGCONNEXION) ||
     (GetConnectStatus()==DIOSTREAMSTATUS_CONNECTED))
     {
       SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);

       while(GetConnectStatus()!=DIOSTREAMSTATUS_DISCONNECTED)
        {
          Sleep(10);
        }
     }

  return true;
}


//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTH::Close
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
bool DIOWINDOWSSTREAMBLUETOOTH::Close()
{
  if(!threadconnexion) return false;

  threadconnexion->End();

  if(config->IsServer())
    {
      if(addr)
        {
          SDP_RegisterService(false,(char*)config->GetServerPropertys(0),
                                    (char*)config->GetServerPropertys(1),
                                    (char*)config->GetServerPropertys(2),
                                    (void*)addr);

          delete (SOCKADDR_BTH*)addr;
          addr = NULL;
        }

      if(handleserver!=INVALID_SOCKET)
        {
          shutdown(handleserver,SD_BOTH);
          closesocket(handleserver);
          handleserver  = INVALID_SOCKET;
        }
    }


  if(handlesocket!=INVALID_SOCKET)
    {
      DWORD nonblock = 0;
      if(ioctlsocket(handlesocket, FIONBIO, &nonblock) != SOCKET_ERROR)
        {
          shutdown(handlesocket,SD_BOTH);
          closesocket(handlesocket);

          handlesocket  = INVALID_SOCKET;
        }
    }

  return true;
}


/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTH::IsReadyConnect
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/05/2013 11:40:25
//
//  @return       int :
//  @param        socket :
*/
/*-----------------------------------------------------------------*/
int DIOWINDOWSSTREAMBLUETOOTH::IsReadyConnect(SOCKET socket)
{
  struct timeval  tv;
  SOCKET          rc;
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

  rc = select((int)socket+1, &fdr, &fdw, &fds, &tv);
  if(rc==SOCKET_ERROR) return -1;
  //if(rc>1)             return -1;

  int status1 = FD_ISSET(socket,&fdr) ? 1 : 0;
  int status2 = FD_ISSET(socket,&fdw) ? 1 : 0;
  int status3 = FD_ISSET(socket,&fds) ? 1 : 0;

  if(status3) return -1;
  if(((status1) || (status2)) && (rc==1))  return  1;

  return 0;
}




//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTH::SDP_RegisterService
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/04/2006 12:40:43
//
//  @return       bool :
//  @param        reg :
//  @param        service_name :
//  @param        service_dsc :
//  @param        service_prov :
//  @param        addr :
*/
//-------------------------------------------------------------------
bool DIOWINDOWSSTREAMBLUETOOTH::SDP_RegisterService(bool reg,char* service_name,char* service_dsc,char* service_prov,void* addr)
{
  CSADDR_INFO csaddr;
  memset(&csaddr, 0, sizeof(csaddr));

  csaddr.LocalAddr.iSockaddrLength  = sizeof(SOCKADDR_BTH);
  csaddr.LocalAddr.lpSockaddr       = (sockaddr*)addr;
  csaddr.iSocketType                = SOCK_STREAM;
  csaddr.iProtocol                  = BTHPROTO_RFCOMM;

  WSAQUERYSET   service;
  GUID          serviceID = { 0x00001101, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB }; //SerialPortServiceClass_UUID;
  memset(&service, 0, sizeof(service));

  XSTRING stringservicename(service_name);
  XSTRING stringservicedsc(service_dsc);

  service.dwSize                    = sizeof(service);

  #ifdef UNICODE
  service.lpszServiceInstanceName   = stringservicename.Get();
  service.lpszComment               = stringservicedsc.Get();
  #else
  service.lpszServiceInstanceName   = service_name;
  service.lpszComment               = service_dsc;
  #endif

  service.lpServiceClassId          = &serviceID;
  service.dwNameSpace               = NS_BTH;
  service.dwNumberOfCsAddrs         = 1;
  service.lpcsaBuffer               = &csaddr;

  if(WSASetService(&service,reg?RNRSERVICE_REGISTER:RNRSERVICE_DELETE,0) == SOCKET_ERROR) return false;

  return true;
}



//-------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTH::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
void DIOWINDOWSSTREAMBLUETOOTH::Clean()
{
  threadconnexion         = NULL;
  status                  = DIOSTREAMSTATUS_DISCONNECTED;

  addr                    = NULL;
  handlesocket            = INVALID_SOCKET;
  handleserver            = INVALID_SOCKET;
}





/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTH::ThreadConnexion
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/05/2013 12:12:27
//
//  @return       void :
//  @param        param :
*/
/*-----------------------------------------------------------------*/
void DIOWINDOWSSTREAMBLUETOOTH::ThreadConnexion(void* param)
{
  DIOWINDOWSSTREAMBLUETOOTH* diostream = (DIOWINDOWSSTREAMBLUETOOTH*)param;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOWINDOWSBTFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOWINDOWSBTFSMSTATE_NONE              : break;

          case DIOWINDOWSBTFSMSTATE_GETTINGCONNEXION  : switch(diostream->IsReadyConnect(diostream->handlesocket))
                                                          {
                                                            case -1:  { int sockerr    = 0;
                                                                        int sockerrlen = sizeof(sockerr);

                                                                        if(!getsockopt(diostream->handlesocket, SOL_SOCKET, SO_ERROR,(char *)&sockerr, &sockerrlen))
                                                                          {
                                                                            if(sockerr==WSAEHOSTDOWN) diostream->SetIsRefusedConnexion(true);
                                                                          }

                                                                        diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                      }
                                                                      break;

                                                            case  1:  diostream->SetEvent(DIOWINDOWSBTFSMEVENT_CONNECTED);
                                                                      diostream->status = DIOSTREAMSTATUS_CONNECTED;
                                                                      break;

                                                            default:  diostream->status = DIOSTREAMSTATUS_GETTINGCONNEXION;
                                                                      break;
                                                          }
                                                        break;

          case DIOWINDOWSBTFSMSTATE_CONNECTED         : break;

          case DIOWINDOWSBTFSMSTATE_WAITINGTOREAD     : { struct timeval  waitd;
                                                          fd_set          read_flags;
                                                          fd_set          write_flags;

                                                          waitd.tv_sec  = 0;
                                                          waitd.tv_usec = 100;

                                                          FD_ZERO(&read_flags);
                                                          FD_ZERO(&write_flags);

                                                          if(diostream->handlesocket == INVALID_SOCKET)
                                                            {
                                                              diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                              break;
                                                            }

                                                          if(!diostream->IsBlockRead()) FD_SET((unsigned int)diostream->handlesocket, &read_flags);
                                                          if(!diostream->IsBlockWrite())
                                                            {
                                                              if(diostream->outbuffer->GetSize()) FD_SET((unsigned int)diostream->handlesocket, &write_flags);
                                                            }

                                                          int error = select((int)(diostream->handlesocket)+1, &read_flags,&write_flags,(fd_set*)0,&waitd);
                                                          if(error==SOCKET_ERROR)
                                                            {
                                                              diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                              break;
                                                            }

                                                          if(FD_ISSET(diostream->handlesocket, &read_flags))  //Socket ready for reading
                                                            {
                                                              XBYTE buffer[DIOSTREAM_MAXBUFFER];


                                                              XDWORD size = recv(diostream->handlesocket, (char*)buffer, DIOSTREAM_MAXBUFFER, 0);
                                                              if(size<=0)
                                                                {
                                                                  diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                  break;
                                                                }
                                                               else diostream->inbuffer->Add(buffer,size);

                                                              FD_CLR((unsigned int)diostream->handlesocket, &read_flags);
                                                            }

                                                          if(FD_ISSET(diostream->handlesocket, &write_flags))  //Socket ready for writing
                                                            {
                                                              int esize = diostream->outbuffer->GetSize();
                                                              if(esize>DIOSTREAM_MAXBUFFER) esize = DIOSTREAM_MAXBUFFER;

                                                              if(esize)
                                                                {
                                                                  diostream->outbuffer->SetBlocked(true);
                                                                  XDWORD size = send(diostream->handlesocket, (char*)diostream->outbuffer->Get(), esize, 0);
                                                                  diostream->outbuffer->SetBlocked(false);

                                                                  if(size<=0)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                      break;

                                                                    }
                                                                  else diostream->outbuffer->Extract(NULL, 0, size);

                                                                  FD_CLR((unsigned int)diostream->handlesocket, &write_flags);
                                                                }
                                                            }
                                                        }
                                                        break;

          case DIOWINDOWSBTFSMSTATE_SENDINGDATA       : break;

          case DIOWINDOWSBTFSMSTATE_DISCONNECTING     : break;
        }
    }
   else
    {
      if(diostream->GetEvent()<DIOWINDOWSBT_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOWINDOWSBTFSMSTATE_NONE              : break;

              case DIOWINDOWSBTFSMSTATE_GETTINGCONNEXION  : { if(diostream->config->IsServer())
                                                                {
                                                                  SOCKADDR_BTH  loc_addr;
                                                                  SOCKADDR_BTH  rem_addr;

                                                                  memset(&loc_addr, 0, sizeof(SOCKADDR_BTH));
                                                                  memset(&rem_addr, 0, sizeof(SOCKADDR_BTH));

                                                                  diostream->handleserver = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
                                                                  if(diostream->handleserver== INVALID_SOCKET)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }

                                                                  loc_addr.addressFamily  = AF_BTH;
                                                                  loc_addr.btAddr         = 0;
                                                                  loc_addr.serviceClassId = GUID_NULL;
                                                                  loc_addr.port           = (ULONG) -1; // config->port & 0xFF;

                                                                  bind(diostream->handleserver, (LPSOCKADDR)&loc_addr, sizeof(SOCKADDR_BTH));

                                                                  int saddr = sizeof(SOCKADDR_BTH);
                                                                  if(getsockname(diostream->handleserver, (sockaddr *)&loc_addr, &saddr)==SOCKET_ERROR)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }


                                                                  diostream->SDP_RegisterService(true,  (char*)diostream->config->GetServerPropertys(0),
                                                                                                        (char*)diostream->config->GetServerPropertys(1),
                                                                                                        (char*)diostream->config->GetServerPropertys(2),
                                                                                                        (void* )diostream->addr);

                                                                  listen(diostream->handleserver, 5);

                                                                  int sizeaddr = sizeof(SOCKADDR_BTH);
                                                                  diostream->handlesocket = accept(diostream->handleserver,(LPSOCKADDR)&rem_addr, &sizeaddr);

                                                                  DWORD nonblock = 1;
                                                                  if(ioctlsocket(diostream->handlesocket, FIONBIO, &nonblock) == SOCKET_ERROR)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                    }
                                                                }
                                                               else
                                                                {
                                                                  if(!diostream->config->GetPIN()->IsEmpty())
                                                                    {
                                                                      WCHAR pass[BLUETOOTH_MAX_PASSKEY_SIZE];

                                                                      memset(pass, 0 , (BLUETOOTH_MAX_PASSKEY_SIZE*sizeof(WCHAR)));

                                                                      XSTRING_CREATENORMALIZE((*diostream->config->GetPIN()), buffnormalize, false)
                                                                      wcsncpy_s(pass, (WCHAR*)buffnormalize, BLUETOOTH_MAX_PASSKEY_SIZE-sizeof(WCHAR));
                                                                      XSTRING_DELETENORMALIZE(buffnormalize)

                                                                      BLUETOOTH_DEVICE_INFO btdi;

                                                                      ZeroMemory(&btdi, sizeof(BLUETOOTH_DEVICE_INFO));

                                                                      btdi.dwSize           = sizeof(BLUETOOTH_DEVICE_INFO);
                                                                      btdi.Address.ullLong  = diostream->config->GetRemoteMAC()->GetLongNumber();

                                                                      DWORD error = BluetoothAuthenticateDevice(NULL, NULL, &btdi, pass, 4);
                                                                      if(error != ERROR_SUCCESS)
                                                                        {
                                                                          diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                          break;
                                                                        }
                                                                    }

                                                                  SOCKADDR_BTH  loc_addr;
                                                                  SOCKADDR_BTH  rem_addr;

                                                                  memset(&loc_addr, 0, sizeof(SOCKADDR_BTH));
                                                                  memset(&rem_addr, 0, sizeof(SOCKADDR_BTH));

                                                                  diostream->handlesocket = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
                                                                  if(diostream->handlesocket == INVALID_SOCKET)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }

                                                                  loc_addr.addressFamily    = AF_BTH;
                                                                  loc_addr.serviceClassId   = GUID_NULL;
                                                                  loc_addr.btAddr           = diostream->config->GetLocalMAC()->GetLongNumber();
                                                                  loc_addr.port             = 0;

                                                                  if(bind(diostream->handlesocket, (SOCKADDR *)&loc_addr, sizeof(SOCKADDR_BTH)))
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }

                                                                  rem_addr.addressFamily    = AF_BTH;
                                                                  rem_addr.serviceClassId   = GUID_NULL;
                                                                  rem_addr.btAddr           = diostream->config->GetRemoteMAC()->GetLongNumber();
                                                                  rem_addr.port             = (diostream->config->GetRemoteChannel() & 0xFF);

                                                                  diostream->SetIsRefusedConnexion(false);

                                                                  DWORD nonblock = 1;
                                                                  if(ioctlsocket(diostream->handlesocket, FIONBIO, &nonblock) == SOCKET_ERROR)
                                                                    {
                                                                      diostream->SetEvent(DIOWINDOWSBTFSMEVENT_DISCONNECTING);
                                                                      break;
                                                                    }

                                                                  connect(diostream->handlesocket, (SOCKADDR *)&rem_addr, sizeof(SOCKADDR_BTH));
                                                                }
                                                            }
                                                            break;

              case DIOWINDOWSBTFSMSTATE_CONNECTED         : diostream->SetEvent(DIOWINDOWSBTFSMEVENT_WAITINGTOREAD);
                                                            break;

              case DIOWINDOWSBTFSMSTATE_WAITINGTOREAD     : break;

              case DIOWINDOWSBTFSMSTATE_SENDINGDATA       : break;

              case DIOWINDOWSBTFSMSTATE_DISCONNECTING     : diostream->threadconnexion->Run(false);
                                                            diostream->status = DIOSTREAMSTATUS_DISCONNECTED;

                                                            if(!diostream->config->GetPIN()->IsEmpty())
                                                              {
                                                                BLUETOOTH_DEVICE_INFO btdi;

                                                                ZeroMemory(&btdi, sizeof(BLUETOOTH_DEVICE_INFO));
                                                                btdi.dwSize           = sizeof(BLUETOOTH_DEVICE_INFO);
                                                                btdi.Address.ullLong  = diostream->config->GetRemoteMAC()->GetLongNumber();

                                                                BluetoothRemoveDevice(&btdi.Address);
                                                              }

                                                            break;
            }
        }
    }
}


#endif

