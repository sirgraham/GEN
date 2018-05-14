/*------------------------------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT.H
*/
/**
// \file
//
//  linux ilclient h264 codec
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 11/04/2016 15:43:02
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _VIDEOLINUXCODECH264ILCLIENT_H_
#define _VIDEOLINUXCODECH264ILCLIENT_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "VIDEOCodecH264.h"

#include "XList.h"
#include "XThread.h"
#include "XFactory.h"

#include "bcm_host.h"
#include "ilclient.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class XTIMER;

struct MEDIAPACKET;

class VIDEOLINUXCODECH264ILCLIENT : public VIDEOCODECH264
{
public:

                                    VIDEOLINUXCODECH264ILCLIENT                     (VIDEOPLAYER* videoplayer);
  virtual                          ~VIDEOLINUXCODECH264ILCLIENT                     ();

  virtual bool                      Ini                                             ();
  virtual bool                      End                                             ();

  virtual bool                      SendFrame                                       (MEDIAPACKET* packet);

  virtual void                      Update                                          ();
  virtual void                      Decode                                          ();

protected:

  bool                              ended;

  OMX_VIDEO_PARAM_PORTFORMATTYPE    format;
  OMX_TIME_CONFIG_CLOCKSTATETYPE    cstate;

  COMPONENT_T*                      list[5]; // NULL terminated list of components
  TUNNEL_T                          tunnels[4];

  ILCLIENT_T*                       client;
  FILE*                             in;
  int                               status;
  unsigned int                      data_len;

  OMX_BUFFERHEADERTYPE *            buf;
  int                               port_settings_changed;
  int                               first_packet;

  XQWORD                            timestamp;

  long long                         ppts;
  long long                         pdts;

  XMUTEX*                           packetmutex;
  XLIST<MEDIAPACKET*>               packets;
  XDWORD                            bufferedsize;

  bool                              eossent;

  pthread_cond_t                    packetcond;

private:

  bool                              SendDecoderConfig                               (COMPONENT_T* component);
  bool                              CopyToBufferAndEmpty                            (XBYTE* data, XDWORD size, COMPONENT_T* component, OMX_BUFFERHEADERTYPE* buffer, long long pts, long long dts);
  bool                              StartClock                                      (COMPONENT_T* component);

  bool                              CreateDecoderComponent                          (ILCLIENT_T* client, char* componentname, COMPONENT_T** component);
  bool                              CreateRenderComponent                           (ILCLIENT_T* client, char* componentname, COMPONENT_T** component);
  bool                              CreateSchedulerComponent                        (ILCLIENT_T* client, char* componentname, COMPONENT_T** component);
  bool                              CreateClockComponent                            (ILCLIENT_T* client, char* componentname, COMPONENT_T** component);

  bool                              SendEOSSignal                                   (COMPONENT_T* component);

  OMX_BUFFERHEADERTYPE*             GetInputBuffer                                  (COMPONENT_T* component, int port);

  static void                       callback                                        (void* data, COMPONENT_T* comp);

  void                              Clean                                           ()
                                    {
                                      ended                   = false;
                                      client                  = NULL;
                                      buf                     = NULL;
                                      port_settings_changed   = 0;
                                      first_packet            = 1;
                                      status                  = 0;
                                      data_len                = 0;
                                      packetmutex             = NULL;
                                      bufferedsize            = 0;
                                      timestamp               = 0;
                                      eossent                 = false;
                                    }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

