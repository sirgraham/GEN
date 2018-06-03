//------------------------------------------------------------------------------------------
//  DIOSTREAMDEVICEBLUETOOTH.H
//
/**
// \class
//
//  Data IO Stream Device Bluetooth class
//
//  @author  Abraham J. Velez
//  @version 23/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTREAMDEVICEBLUETOOTH_H_
#define _DIOSTREAMDEVICEBLUETOOTH_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XVector.h"
#include "XDebugTrace.h"

#include "DIOMAC.h"

#include "DIOStreamDevice.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

/*
enum DIOSTREAMDEVICEBLUETOOTHTYPE
{
  DIOSTREAMDEVICEBLUETOOTHTYPE_GENERIC                                      = 0 ,

  DIOSTREAMDEVICEBLUETOOTHTYPE_GENERICCOMPUTER                                  ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_WORKSTATION                                      ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_SERVER                                           ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_LAPTOP                                           ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_HANDHELDPDA                                      ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_PALMPDA                                          ,

  DIOSTREAMDEVICEBLUETOOTHTYPE_GENERICPHONE                                     ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_CELLULAR                                         ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_CORDLESS                                         ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_SMARTPHONE                                       ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_WIREDMODEM                                       ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_ISDNACCESS                                       ,

  DIOSTREAMDEVICEBLUETOOTHTYPE_GENERICLANNETWORK                                ,

  DIOSTREAMDEVICEBLUETOOTHTYPE_GENERICAUDIOVIDEO                                ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_WEARABLEHEADSET                                  ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_HANDSFREE                                        ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_MICROPHONE                                       ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_LOUDSPEAKER                                      ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_HEADPHONES                                       ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_PORTABLEAUDIO                                    ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_CARDAUDIO                                        ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_SETTOPBOX                                        ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_HIFIAUDIO                                        ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_VCR                                              ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_VIDEOCAMERA                                      ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_CAMCORDER                                        ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_VIDEOMONITOR                                     ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_VIDEODISPLAY                                     ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_VIDEOCONFERENCING                                ,

  DIOSTREAMDEVICEBLUETOOTHTYPE_GENERICPERIPHERAL                                ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_JOYSTICK                                         ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_GAMEPAD                                          ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_REMOTECONTROL                                    ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_SENSINGDEVICE                                    ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_DIGITALIZERTABLET                                ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_CARDREADER                                       ,

  DIOSTREAMDEVICEBLUETOOTHTYPE_GENERICIMAGING                                   ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_DISPLAY                                          ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_CAMERA                                           ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_SCANNER                                          ,
  DIOSTREAMDEVICEBLUETOOTHTYPE_PRINTER                                          ,

  DIOSTREAMDEVICEBLUETOOTHTYPE_UNCATEGORIZED
};
*/


#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_SDP                                0x0001
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_UDP                                0x0002
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_RFCOMM                             0x0003
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_TCP                                0x0004
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_TCS_BIN                            0x0005
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_TCS_AT                             0x0006
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_OBEX                               0x0008
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_IP                                 0x0009
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_FTP                                0x000A
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_HTTP                               0x000C
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_WSP                                0x000E
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_BNEP                               0x000F
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_UPNP                               0x0010
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_HIDP                               0x0011
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_HARDCOPYCONTROLCHANNEL             0x0012
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_HARDCOPYDATACHANNEL                0x0014
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_HARDCOPYNOTIFICATION               0x0016
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_AVCTP                              0x0017
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_AVDTP                              0x0019
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_CMTP                               0x001B
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_UDI_C_PLANE                        0x001D
#define DIOSTREAMDEVICEBLUETOOTHPROTOCOLTYPE_L2CAP                              0x0100


#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_SERVICEDISCOVERYPROTOCOL            0x1000
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_BROWSEGROUPDESCRIPTORSERVICE        0x1001
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_PUBLICBROWSEGROUP                   0x1002
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_SERIALPORT                          0x1101
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_LANACCESSUSINGPPP                   0x1102
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_DIALUPNETWORKING                    0x1103
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_IRMCSYNC                            0x1104
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_OBEXOBJECTPUSH                      0x1105
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_OBEXFILETRANSFER                    0x1106
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_IRMCSYNCCOMMAND                     0x1107
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_HEADSET                             0x1108
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_CORDLESSTELEPHONY                   0x1109
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_AUDIOSOURCE                         0x110A
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_AUDIOSINK                           0x110B
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_AV_REMOTECONTROLTARGET              0x110C
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_ADVANCEDAUDIODISTRIBUTION           0x110D
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_AV_REMOTECONTROL                    0x110E
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_VIDEOCONFERENCING                   0x110F
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_INTERCOM                            0x1110
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_FAX                                 0x1111
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_HEADSETAUDIOGATEWAY                 0x1112
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_WAP                                 0x1113
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_WAP_CLIENT                          0x1114
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_PANU                                0x1115
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_NAP                                 0x1116
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_GN                                  0x1117
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_DIRECTPRINTING                      0x1118
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_REFERENCEPRINTING                   0x1119
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_IMAGING                             0x111A
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_IMAGINGRESPONDER                    0x111B
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_IMAGINGAUTOMATICARCHIVE             0x111C
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_IMAGINGREFERENCEDOBJECTS            0x111D
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_HANDSFREE                           0x111E
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_HANDSFREEAUDIOGATEWAY               0x111F
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_DIRECTPRINTINGREFERENCEOBJECTS      0x1120
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_REFLECTEDUI                         0x1121
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_BASICPRINTING                       0x1122
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_PRINTINGSTATUS                      0x1123
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_HUMANINTERFACEDEVICESERVICE         0x1124
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_HARDCOPYCABLEREPLACEMENT            0x1125
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_HCR_PRINT                           0x1126
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_HCR_SCAN                            0x1127
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_COMMON_ISDN_ACCESS                  0x1128
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_VIDEOCONFERENCINGGW                 0x1129
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_UDI_MT                              0x112A
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_UDI_TA                              0x112B
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_AUDIOVIDEO                          0x112C
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_SIM_ACCESS                          0x112D
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_PNPINFORMATION                      0x1200
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_GENERICNETWORKING                   0x1201
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_GENERICFILETRANSFER                 0x1202
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_GENERICAUDIO                        0x1203
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_GENERICTELEPHONY                    0x1204
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_UPNP_SERVICE                        0x1205
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_UPNP_IP_SERVICE                     0x1206
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_ESDP_UPNP_IP_PAN                    0x1300
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_ESDP_UPNP_IP_LAP                    0x1301
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_ESDP_UPNP_L2CAP                     0x1302
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_VIDEOSOURCE                         0x1303
#define DIOSTREAMDEVICEBLUETOOTHSERVICETYPE_VIDEOSINK                           0x1304



#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICERECORDHANDLE                    0x0000
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICECLASSIDLIST                     0x0001
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICERECORDSTATE                     0x0002
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICEID                              0x0003
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_PROTOCOLDESCRIPTORLIST                 0x0004
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_BROWSEGROUPLIST                        0x0005
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_LANGUAGEBASEATTRIBUTEIDLIST            0x0006
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICEINFOTIMETOLIVE                  0x0007
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICEAVAILABILITY                    0x0008
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_BLUETOOTHPROFILEDESCRIPTORLIST         0x0009
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_DOCUMENTATIONURL                       0x000a
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_CLIENTEXECUTABLEURL                    0x000b
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_ICONURL                                0x000c
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_ADDITIONALPROTOCOLDESCRIPTORLISTS      0x000d
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_NAMELABELSERVICE                       0x0100
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_DESCRIPTIONSERVICE                     0x0101
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_PROVIDERNAMESERVICE                    0x0102
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_GROUPID                                0x0200
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_IPSUBNET                               0x0200
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_VERSIONNUMBERLIST                      0x0200
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICEDATABASESTATE                   0x0201
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SERVICEVERSION                         0x0300
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_EXTERNALNETWORK                        0x0301
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_NETWORK                                0x0301
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SUPPORTEDDATASTORESLIST                0x0301
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_FAXCLASS1SUPPORT                       0x0302
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_REMOTEAUDIOVOLUMECONTROL               0x0302
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_FAXCLASS2SUPPORT1                      0x0303
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SUPPORTEDFORMATSLIST                   0x0303
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_FAXCLASS2SUPPORT2                      0x0304
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_AUDIOFEEDBACKSUPPORT                   0x0305
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_NETWORKADDRESS                         0x0306
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_WAPGATEWAY                             0x0307
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_HOMEPAGEURL                            0x0308
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_WAPSTACKTYPE                           0x0309
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SECURITYDESCRIPTION                    0x030a
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_NETACCESSTYPE                          0x030b
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_MAXNETACCESSRATE                       0x030c
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_IPV4SUBNET                             0x030d
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_IPV6SUBNET                             0x030E
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SUPPORTEDCAPABALITIES                  0x0310
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SUPPORTEDFEATURES                      0x0311
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_SUPPORTEDFUNCTIONS                     0x0312
#define DIOSTREAMDEVICEBLUETOOTHATTRTYPE_TOTALIMAGINGDATACAPACITY               0x0313



#define DIOSTREAMDEVICEBLUETOOTHSDPSERVICE_MAXPROTOCOLS                         16
#define DIOSTREAMDEVICEBLUETOOTH_NOCHANNEL                                      -1


//---- CLASS -------------------------------------------------------------------------------

class DIOSTREAMDEVICEBLUETOOTHSDPSERVICEPROTOCOL
{
  public:
                                                  DIOSTREAMDEVICEBLUETOOTHSDPSERVICEPROTOCOL    ()                                  { Clean();                                   }
    virtual                                      ~DIOSTREAMDEVICEBLUETOOTHSDPSERVICEPROTOCOL    ()                                  { Clean();                                   }

    XDWORD                                        GetType                                       ()                                  { return type;                               }
    void                                          SetType                                       (XDWORD type)                       { this->type = type;                         }

    XWORD                                         GetChannel                                    ()                                  { return channel;                            }
    void                                          SetChannel                                    (XWORD channel)                     { this->channel = channel;                   }

  private:

    void                                          Clean                                         ()
                                                  {
                                                    type    = 0;
                                                    channel = 0;
                                                  }

    XDWORD                                        type;
    XWORD                                         channel;
};



class DIOSTREAMDEVICEBLUETOOTHSDPSERVICE
{
  public:

                                                  DIOSTREAMDEVICEBLUETOOTHSDPSERVICE            ()                                  { Clean();                                   }
    virtual                                      ~DIOSTREAMDEVICEBLUETOOTHSDPSERVICE            ()                                  { Clean();                                   }

    XDWORD                                        GetID                                         ()                                  { return ID;                                 }
    void                                          SetID                                         (XDWORD ID)                         { this->ID = ID;                             }

    XDWORD                                        GetRecHandle                                  ()                                  { return rechandle;                          }
    void                                          SetRecHandle                                  (XDWORD rechandle)                  { this->rechandle = rechandle;               }

    XSTRING*                                      GetName                                       ()                                  { return &name;                              }
    XSTRING*                                      GetDescription                                ()                                  { return &description;                       }
    XSTRING*                                      GetProvider                                   ()                                  { return &provider;                          }

    int                                           GetNProtocols                                 ()                                  { return nprotocols;                         }
    void                                          SetNProtocols                                 (int nprotocols)                    { this->nprotocols = nprotocols;             }

    DIOSTREAMDEVICEBLUETOOTHSDPSERVICEPROTOCOL*   GetProtocol                                   (int index)
                                                  {
                                                    if(index>=DIOSTREAMDEVICEBLUETOOTHSDPSERVICE_MAXPROTOCOLS) return NULL;
                                                    if(index<0)                                                return NULL;

                                                    return &protocols[index];
                                                  }

    bool                                          SetProtocol                                   (int index, DIOSTREAMDEVICEBLUETOOTHSDPSERVICEPROTOCOL* protocol)
                                                  {
                                                    if(!protocol)         return false;
                                                    if(nprotocols >= DIOSTREAMDEVICEBLUETOOTHSDPSERVICE_MAXPROTOCOLS) return false;
                                                    if(nprotocols <= 0)   return false;

                                                    protocols[index].SetType(protocol->GetType());
                                                    protocols[index].SetChannel(protocol->GetChannel());

                                                    return true;
                                                  }

  private:

    void                                          Clean                                         ()
                                                  {
                                                    ID          = 0;
                                                    rechandle   = 0;
                                                    nprotocols  = 0;
                                                  }

    XDWORD                                        ID;
    XDWORD                                        rechandle;
    XSTRING                                       name;
    XSTRING                                       description;
    XSTRING                                       provider;
    int                                           nprotocols;
    DIOSTREAMDEVICEBLUETOOTHSDPSERVICEPROTOCOL    protocols[DIOSTREAMDEVICEBLUETOOTHSDPSERVICE_MAXPROTOCOLS];

};




class DIOSTREAMDEVICEBLUETOOTH : public DIOSTREAMDEVICE
{
  public:

                                                    DIOSTREAMDEVICEBLUETOOTH                      ();
    virtual                                        ~DIOSTREAMDEVICEBLUETOOTH                      ();

    DIOMAC*                                         GetMAC                                        ();
    DIOGUID*                                        GetServiceClass                               ();

    bool                                            IsVisible                                     ();
    bool                                            SetIsVisible                                  (bool isvisible);

    bool                                            Service_Add                                   (DIOSTREAMDEVICEBLUETOOTHSDPSERVICE* service);
    XVECTOR<DIOSTREAMDEVICEBLUETOOTHSDPSERVICE*>*   Service_GetAll                                ();
    bool                                            Service_DeleteAll                             ();

    int                                             GetChannel                                    (XWORD servicetype);
    int                                             GetChannelSerialPort                          ();
    int                                             GetChannelObexPush                            ();
    int                                             GetChannelDialUpNetworking                    ();
    int                                             GetChannelHID                                 ();

    bool                                            DebugPrintInfo                                (XCONSOLE* xconsole);

  protected:

    DIOMAC                                          MAC;
    bool                                            isvisible;
    DIOGUID                                         serviceclass;

  private:

    void                                            Clean                                         ();

    XVECTOR<DIOSTREAMDEVICEBLUETOOTHSDPSERVICE*>    services;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
