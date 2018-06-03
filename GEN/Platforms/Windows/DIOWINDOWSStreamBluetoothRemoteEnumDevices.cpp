//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES.CPP
//
//  WINDOWS Data IO Stream Bluetooth Remote Enum Devices class
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

#include "XFactory.h"
#include "XSleep.h"
#include "XBuffer.h"
#include "XThreadCollected.h"

#include "DIOStream.h"
#include "DIOStreamDeviceBluetooth.h"

#include "DIOWINDOWSStreamBluetoothRemoteEnumDevices.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------

//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIPREMOTEENUMDEVICES::DIOWINDOWSSTREAMTCPIPREMOTEENUMDEVICES
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 19:00:01
//
//  @return


*/
/*-----------------------------------------------------------------*/
DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES() : DIOSTREAMBLUETOOTHREMOTEENUMDEVICES(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOWINDOWSBTENUMFSMSTATE_NONE               ,
            DIOWINDOWSBTENUMFSMEVENT_SEARCHMAC          , DIOWINDOWSBTENUMFSMSTATE_SEARCHMAC        ,
            DIOWINDOWSBTENUMFSMEVENT_SEARCHEND          , DIOWINDOWSBTENUMFSMSTATE_SEARCHEND        ,
            EVENTDEFEND);

  AddState( DIOWINDOWSBTENUMFSMSTATE_SEARCHMAC          ,
            DIOWINDOWSBTENUMFSMEVENT_SEARCHSERVICES     , DIOWINDOWSBTENUMFSMSTATE_SEARCHSERVICES   ,
            DIOWINDOWSBTENUMFSMEVENT_SEARCHEND          , DIOWINDOWSBTENUMFSMSTATE_SEARCHEND        ,
            EVENTDEFEND);

  AddState( DIOWINDOWSBTENUMFSMSTATE_SEARCHSERVICES     ,
            DIOWINDOWSBTENUMFSMEVENT_SEARCHEND          , DIOWINDOWSBTENUMFSMSTATE_SEARCHEND        ,
            EVENTDEFEND);

  AddState( DIOWINDOWSBTENUMFSMSTATE_SEARCHEND          ,
            DIOWINDOWSBTENUMFSMEVENT_SEARCHMAC          , DIOWINDOWSBTENUMFSMSTATE_SEARCHMAC        ,
            EVENTDEFEND);

  threadenumdevices = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMBLUETOOTH, __L("DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES"), ThreadEnumDevices, (void*)this);
  if(threadenumdevices) threadenumdevices->Ini();

}



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIPREMOTEENUMDEVICES::~DIOWINDOWSSTREAMTCPIPREMOTEENUMDEVICES
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 19:00:12
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::~DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES()
{
  StopSearch(true);

  if(threadenumdevices)
    {
      threadenumdevices->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMBLUETOOTH, threadenumdevices);
    }


  Clean();
}



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMTCPIPREMOTEENUMDEVICES::Search
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 19:00:27
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::Search()
{
  if(!threadenumdevices)       return false;

  DelAllDevices();

  SetEvent(DIOWINDOWSBTENUMFSMEVENT_SEARCHMAC);

  threadenumdevices->Run(true);

  if(xtimerout)
    {
      xtimerout->Reset();
      while(GetCurrentState() == DIOWINDOWSBTENUMFSMSTATE_NONE)
        {
          XSLEEP::GetInstance().MilliSeconds(10);

          if(xtimerout->GetMeasureSeconds() >= 5)
            {
              return false;
            }
        }
    }

  return true;
}



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::StopSearch
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2013 23:41:35
//
//  @return       bool :
//  @param        waitend :
*/
/*-----------------------------------------------------------------*/
bool DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::StopSearch(bool waitend)
{
  if(!IsSearching()) return false;

  SetEvent(DIOWINDOWSBTENUMFSMEVENT_SEARCHEND);

  if(waitend)
    {
      while(GetCurrentState()!=DIOWINDOWSBTENUMFSMEVENT_SEARCHEND)
        {
          XSLEEP::GetInstance().MilliSeconds(10);
        }
    }

  if(threadenumdevices) threadenumdevices->Run(false);

  Sleep(10);

  return true;
};




/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::IsSearching
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2013 23:41:40
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::IsSearching()
{
  if(!threadenumdevices) return false;

  if(!threadenumdevices->IsRunning()) return false;

  if((GetCurrentState() == DIOWINDOWSBTENUMFSMSTATE_SEARCHMAC) || (GetCurrentState() == DIOWINDOWSBTENUMFSMSTATE_SEARCHSERVICES)) return true;

  return false;
};




/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2013 23:25:03
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::Clean()
{
  threadenumdevices = NULL;

}




/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::ThreadEnumDevices
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/05/2013 23:31:49
//
//  @return       void :
//  @param        thread :
*/
/*-----------------------------------------------------------------*/
void DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::ThreadEnumDevices(void* data)
{
  DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES* enumeration = (DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES*)data;
  if(!enumeration) return;

  LPWSAQUERYSET pwsaresults   = (LPWSAQUERYSET)enumeration->buffer;
  DWORD         dwsize        = sizeof(enumeration->buffer);
  XDWORD        flagsinquiry1 =  LUP_CONTAINERS | LUP_FLUSHCACHE;
  XDWORD        flagsinquiry2 =  LUP_CONTAINERS | LUP_FLUSHCACHE | LUP_RETURN_TYPE | LUP_RETURN_NAME | LUP_RETURN_ADDR | LUP_RETURN_BLOB;
  static int    index = 0;

  if(enumeration->GetEvent()==DIOWINDOWSBTENUMFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(enumeration->GetCurrentState())
        {
          case DIOWINDOWSBTENUMFSMSTATE_NONE            : break;

          case DIOWINDOWSBTENUMFSMSTATE_SEARCHMAC       : { ZeroMemory(pwsaresults, sizeof(WSAQUERYSET));
                                                            pwsaresults->dwSize       = sizeof(WSAQUERYSET);
                                                            pwsaresults->dwNameSpace  = NS_BTH;
                                                            pwsaresults->lpBlob       = NULL;

                                                            if(WSALookupServiceNext(enumeration->hlookup, flagsinquiry2 , &dwsize, pwsaresults)!=ERROR_SUCCESS)
                                                              {
                                                                WSALookupServiceEnd(enumeration->hlookup);
                                                                enumeration->SetEvent(DIOWINDOWSBTENUMFSMEVENT_SEARCHSERVICES);
                                                                break;
                                                              }

                                                            DIOSTREAMDEVICEBLUETOOTH* device = new DIOSTREAMDEVICEBLUETOOTH();
                                                            if(!device) break;

                                                            XSTRING MAC;

                                                            device->SetIndex(index++);

                                                            MAC.Format(__L("%02X:%02X:%02X:%02X:%02X:%02X") , (XBYTE)pwsaresults->lpcsaBuffer->RemoteAddr.lpSockaddr->sa_data[5]
                                                                                                            , (XBYTE)pwsaresults->lpcsaBuffer->RemoteAddr.lpSockaddr->sa_data[4]
                                                                                                            , (XBYTE)pwsaresults->lpcsaBuffer->RemoteAddr.lpSockaddr->sa_data[3]
                                                                                                            , (XBYTE)pwsaresults->lpcsaBuffer->RemoteAddr.lpSockaddr->sa_data[2]
                                                                                                            , (XBYTE)pwsaresults->lpcsaBuffer->RemoteAddr.lpSockaddr->sa_data[1]
                                                                                                            , (XBYTE)pwsaresults->lpcsaBuffer->RemoteAddr.lpSockaddr->sa_data[0]);
                                                            device->GetMAC()->Set(MAC);

                                                            device->GetName()->Set(pwsaresults->lpszServiceInstanceName);

                                                            device->GetServiceClass()->Set( pwsaresults->lpServiceClassId->Data1 ,
                                                                                            pwsaresults->lpServiceClassId->Data2 ,
                                                                                            pwsaresults->lpServiceClassId->Data3 ,
                                                                                            pwsaresults->lpServiceClassId->Data4 );
                                                            enumeration->devices.Add(device);
                                                          }
                                                          break;

          case DIOWINDOWSBTENUMFSMSTATE_SEARCHSERVICES  : enumeration->SetEvent(DIOWINDOWSBTENUMFSMEVENT_SEARCHEND);
                                                          break;

          case DIOWINDOWSBTENUMFSMSTATE_SEARCHEND       : return;
        }
    }
   else
    {
      if(enumeration->GetEvent() < DIOWINDOWSBTENUM_LASTEVENT)
        {
          enumeration->CheckTransition();

          switch(enumeration->GetCurrentState())
            {
              case DIOWINDOWSBTENUMFSMSTATE_NONE            : break;

              case DIOWINDOWSBTENUMFSMSTATE_SEARCHMAC       : { enumeration->DelAllDevices();

                                                                index = 0;

                                                                SOCKET sock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
                                                                if(sock == INVALID_SOCKET)
                                                                  {
                                                                    enumeration->SetEvent(DIOWINDOWSBTENUMFSMEVENT_SEARCHEND);
                                                                    break;
                                                                  }

                                                                WSAPROTOCOL_INFO protocolinfo;
                                                                int protocolinfosize = sizeof(protocolinfo);

                                                                if(getsockopt(sock , SOL_SOCKET
                                                                                   , SO_PROTOCOL_INFO
                                                                                   , (char*)&protocolinfo
                                                                                   , &protocolinfosize)!=0)
                                                                  {
                                                                    enumeration->SetEvent(DIOWINDOWSBTENUMFSMEVENT_SEARCHEND);
                                                                    break;
                                                                  }

                                                                ZeroMemory(&enumeration->wsaq, sizeof(enumeration->wsaq));

                                                                enumeration->wsaq.dwSize       = sizeof(enumeration->wsaq);
                                                                enumeration->wsaq.dwNameSpace = NS_BTH;
                                                                enumeration->wsaq.lpcsaBuffer = NULL;

                                                                int error = WSALookupServiceBegin(&enumeration->wsaq, flagsinquiry1, &enumeration->hlookup);
                                                                if(error!=ERROR_SUCCESS) break;
                                                              }
                                                              break;


              case DIOWINDOWSBTENUMFSMSTATE_SEARCHSERVICES  : enumeration->SearchServices();
                                                              break;

              case DIOWINDOWSBTENUMFSMSTATE_SEARCHEND       : enumeration->threadenumdevices->Run(false);
                                                              break;
            }
        }
    }
}



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::SearchServices
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/05/2013 23:16:48
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOWINDOWSSTREAMBLUETOOTHREMOTEENUMDEVICES::SearchServices()
{
  for(int c=0;c<(int)devices.GetSize();c++)
    {
      DIOSTREAMDEVICEBLUETOOTH* device  = (DIOSTREAMDEVICEBLUETOOTH*)devices.Get(c);
      if(!device) continue;

      HANDLE       hlookup;
      WSAQUERYSET* qs;
      DWORD        qssize;
      GUID         protocol = L2CAP_PROTOCOL_UUID;
      DWORD        flags    = LUP_FLUSHCACHE | LUP_RETURN_ALL;
      XSTRING      MACstring;
      bool         done;

      qssize = sizeof(WSAQUERYSET);
      qs = (WSAQUERYSET*)new XBYTE[qssize];

      ZeroMemory( qs, qssize );

      qs->dwSize            = sizeof(WSAQUERYSET);

      device->GetMAC()->GetXString(MACstring);

      qs->lpServiceClassId  = (LPGUID)&protocol;
    //qs->lpServiceClassId  = (LPGUID)&PublicBrowseGroupServiceClass_UUID;
      qs->dwNameSpace       = NS_BTH;
      qs->dwNumberOfCsAddrs = 0;
      qs->lpszContext       = (LPWSTR)MACstring.Get();

      if(WSALookupServiceBegin( qs, flags, &hlookup) == SOCKET_ERROR)
        {
          int error = WSAGetLastError();

          //XDEBUGTRACE_PRINT(__L("Search SDP Services Error %d"), error);

          delete [] qs;

          continue;
        }

      done = false;
      while(!done)
        {
          if(SOCKET_ERROR ==  WSALookupServiceNext(hlookup, flags, &qssize, qs))
            {
              int error = WSAGetLastError();
              if( error == WSAEFAULT )
                {
                  delete [] qs;
                  qs = (WSAQUERYSET*)new XBYTE[qssize];
                }
               else
                {
                  if(error == WSA_E_NO_MORE )
                    {
                      done = true;
                    }
                   else
                    {
                      break;
                    }
                }
            }
           else
            {
              DIOSTREAMDEVICEBLUETOOTHSDPSERVICE* service = new DIOSTREAMDEVICEBLUETOOTHSDPSERVICE();
              if(service)
                {
                  service->GetName()->Set(qs->lpszServiceInstanceName);
                  service->GetDescription()->Set(qs->lpszComment);

                  const BLOB* pblob = (BLOB*)qs->lpBlob;

                  if(pblob) BluetoothSdpEnumAttributes(pblob->pBlobData, pblob->cbSize, SDP_ServiceCallback, service);

                  device->Service_Add(service);
                }
            }
        }

      delete [] qs;

      WSALookupServiceEnd(hlookup);

    }
}


/*-------------------------------------------------------------------
//  SDP_ServiceCallback
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/05/2013 22:17:25
//
//  @return       BOOL :
//  @param        attribID :
//  @param        valuestream :
//  @param        cbstreamsize :
//  @param        vparam :
*/
/*-----------------------------------------------------------------*/
BOOL __stdcall SDP_ServiceCallback(ULONG attribID, LPBYTE valuestream, ULONG cbstreamsize, LPVOID vparam)
{
  DIOSTREAMDEVICEBLUETOOTHSDPSERVICE* service = (DIOSTREAMDEVICEBLUETOOTHSDPSERVICE*)vparam;
  if(!service) return false;

  switch(attribID)
    {
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICERECORDHANDLE                 : { SDP_ELEMENT_DATA  data;
                                                                                    int               result;

                                                                                    result = BluetoothSdpGetElementData(valuestream,  cbstreamsize, &data);
                                                                                    if(result==ERROR_NO_MORE_ITEMS) break;
                                                                                     else
                                                                                      {
                                                                                        if(result != ERROR_SUCCESS) break;
                                                                                      }

                                                                                     service->SetRecHandle(data.data.uint32);
                                                                                  }
                                                                                  break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICECLASSIDLIST                  : { SDP_ELEMENT_DATA              data;
                                                                                    HBLUETOOTH_CONTAINER_ELEMENT  element = NULL;
                                                                                    int                           result;

                                                                                    while(TRUE)
                                                                                      {
                                                                                        result = BluetoothSdpGetContainerElementData(valuestream,  cbstreamsize, (HBLUETOOTH_CONTAINER_ELEMENT*)&element, &data);
                                                                                        if(result==ERROR_NO_MORE_ITEMS) break;
                                                                                          else
                                                                                           {
                                                                                             if(result != ERROR_SUCCESS) break;
                                                                                           }

                                                                                        service->SetID((DWORD)data.data.uint32);
                                                                                      }
                                                                                  }
                                                                                  break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICERECORDSTATE                  : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICEID                           : break;


      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_PROTOCOLDESCRIPTORLIST              : { SDP_ELEMENT_DATA              data;
                                                                                    HBLUETOOTH_CONTAINER_ELEMENT  element = NULL;

                                                                                    while(TRUE)
                                                                                      {
                                                                                        int result = BluetoothSdpGetContainerElementData(valuestream,  cbstreamsize, (HBLUETOOTH_CONTAINER_ELEMENT*)&element, &data);

                                                                                        if(result==ERROR_NO_MORE_ITEMS) break;
                                                                                          else
                                                                                            {
                                                                                              if(result != ERROR_SUCCESS) break;
                                                                                            }


                                                                                        if(data.type == SDP_TYPE_SEQUENCE)
                                                                                          {

                                                                                            //---------------------------------------------------
                                                                                            /*
                                                                                            XBUFFER xbuffer(BLUETOOTH_xfactory);

                                                                                            for(ULONG c=0; c<data.data.sequence.length; c++)
                                                                                              xbuffer.Add((XBYTE)data.data.sequence.value[c]);

                                                                                           XDEBUGTRACE_PRINTCOLOR(1, __L("Sequence: "));
                                                                                            XDEBUGTRACE_PRINTDATABLOCK(1, xbuffer);
                                                                                            */
                                                                                            //---------------------------------------------------

                                                                                            BYTE*   buffer  = (BYTE*)element;
                                                                                            XWORD   channel = 0;

                                                                                            if(buffer[0] == 0x35)
                                                                                              {
                                                                                                DIOSTREAMDEVICEBLUETOOTHSDPSERVICEPROTOCOL* protocol = service->GetProtocol(service->GetNProtocols());
                                                                                                if(protocol)
                                                                                                  {
                                                                                                    XDWORD type    = (XDWORD)((buffer[3]<<8)|(buffer[4]));
                                                                                                    XBYTE  size    = (XBYTE)buffer[1];
                                                                                                    XWORD  channel = 0;

                                                                                                    if(size>3) channel = (WORD)buffer[7];

                                                                                                    protocol->SetType(type);
                                                                                                    protocol->SetChannel(channel);

                                                                                                    service->SetNProtocols(service->GetNProtocols()+1);
                                                                                                  }
                                                                                              }
                                                                                          }
                                                                                      }


                                                                                  }
                                                                                  break;


      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_BROWSEGROUPLIST                     : break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_LANGUAGEBASEATTRIBUTEIDLIST         : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICEINFOTIMETOLIVE               : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICEAVAILABILITY                 : break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_BLUETOOTHPROFILEDESCRIPTORLIST      : break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_DOCUMENTATIONURL                    : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_CLIENTEXECUTABLEURL                 : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_ICONURL                             : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_ADDITIONALPROTOCOLDESCRIPTORLISTS   : break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_NAMELABELSERVICE                    :
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_DESCRIPTIONSERVICE                  :
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_PROVIDERNAMESERVICE                 : { SDP_ELEMENT_DATA data;
                                                                                    int              result;

                                                                                    result = BluetoothSdpGetElementData(valuestream,  cbstreamsize, &data);

                                                                                    if(result==ERROR_NO_MORE_ITEMS) break;
                                                                                     else
                                                                                      {
                                                                                        if(result != ERROR_SUCCESS) break;
                                                                                      }

                                                                                    XSTRING name((char*)data.data.string.value);
                                                                                    name.AdjustSize(data.data.string.length, false, __L(" "), false);

                                                                                    switch(attribID)
                                                                                      {
                                                                                        case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_NAMELABELSERVICE    : service->GetName()->Set(name);         break;
                                                                                        case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_DESCRIPTIONSERVICE  : service->GetDescription()->Set(name);  break;
                                                                                        case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_PROVIDERNAMESERVICE : service->GetProvider()->Set(name);     break;
                                                                                      }
                                                                                  }
                                                                                  break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_GROUPID                             : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICEVERSION                      : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_EXTERNALNETWORK                     : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_REMOTEAUDIOVOLUMECONTROL            : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SUPPORTEDFORMATSLIST                : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_AUDIOFEEDBACKSUPPORT                : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_NETWORKADDRESS                      : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_WAPGATEWAY                          : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_HOMEPAGEURL                         : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_WAPSTACKTYPE                        : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SECURITYDESCRIPTION                 : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_NETACCESSTYPE                       : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_MAXNETACCESSRATE                    : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_IPV4SUBNET                          : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_IPV6SUBNET                          : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SUPPORTEDCAPABALITIES               : break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SUPPORTEDFEATURES                   : break;

      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SUPPORTEDFUNCTIONS                  : break;
      case DIOSTREAMDEVICEBLUETOOTHATTRTYPE_TOTALIMAGINGDATACAPACITY            : break;
    }

  return true;
}


#endif

