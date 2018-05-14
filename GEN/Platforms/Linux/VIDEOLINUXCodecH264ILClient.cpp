
/*------------------------------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT.CPP
//
//  ilclient H264 decoder
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 11/04/2016 15:49:01
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "VIDEOLINUXCodecH264ILClient.h"
#include "MEDIAFrameReader.h"
#include "MEDIAFrameReaderLibAV.h"


#include "GRPRenderer.h"
#include "GRPContext.h"

#include "XLINUXThread.h"
#include "XFactory.h"
#include "XTimer.h"

#include "XDebug.h"

#pragma pack(push, 1)
#include "ilclient.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

#define BUFFEREDSIZEMAX 10*1024*1024 // 10MB

/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  eoscallback
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/05/2016 11:55:47
//
//  @param        userdata :
//  @param        comp :
//  @param        data : port reporting the flag, careful!!, we need to check it to see if it's the correct one
*/
/*-----------------------------------------------------------------*/
static void eoscallback(void* userdata, COMPONENT_T* comp, OMX_U32 data)
{
  VIDEOPLAYER* videoplayer = (VIDEOPLAYER*)userdata;
  //XDEBUG_PRINTCOLOR(2, __L("EOS Callback"));

  if(videoplayer->codec)
    {
      videoplayer->codec->eos = true;
    }
}




/*-------------------------------------------------------------------
//  gen_debug_output
*/
/**
//
//  To capture and print via xdebug all of the internal ilclient debug messages
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/05/2016 13:24:35
//
//  @param        *format :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
void gen_debug_output(char *format, ...)
{
  va_list arg;
  va_start(arg, format);

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  XSTRING output;

  XSTRING str;
  bool    openlabel = false;
  bool isspecialweb = false;

  output.Empty();

  if(!format) return;

  int c  = 0;

  char*mask = format;

  while(mask[c])
    {
      switch(mask[c])
        {
          case __C('%')   : { XCHAR  param[64];
                              int  nparam = 1;
                              bool end    = false;

                              memset(param,0, 64*sizeof(XCHAR));
                              param[0] = __C('%');

                              c++;

                              do{ param[nparam] = mask[c];

                                  nparam++;

                                  switch(mask[c])
                                    {
                                      case __C('c')   :
                                      case __C('C')   : { char character;

                                                          #if defined(LINUX) || defined(ANDROID)
                                                          character = (char)va_arg((arg), int);
                                                          #else
                                                          character = va_arg((arg), char);
                                                          #endif

                                                          str.Set(character);
                                                          end  = true;
                                                        }
                                                        break;

                                      case __C('d')   :
                                      case __C('i')   :
                                      case __C('o')   :
                                      case __C('u')   :
                                      case __C('x')   :
                                      case __C('X')   : { bool large = false;
                                                          for(int l=0; l<(int)XSTRING::GetSize(param); l++)
                                                            {
                                                              if(param[l] == 'l') large = true;
                                                            }

                                                          if(!large)
                                                            {
                                                              int value = (int)va_arg((arg), int);

                                                              str.ConvertFromInt(value, param);
                                                            }
                                                           else
                                                            {
                                                              str.ConvertFromDouble((double)va_arg((arg), double), param);
                                                            }
                                                          end  = true;
                                                        }
                                                        break;

                                      case __C('f')   : str.ConvertFromFloat((float)va_arg((arg), double), param);
                                                        end  = true;
                                                        break;

                                      case __C('g')   :
                                      case __C('G')   : str.ConvertFromDouble((double)va_arg((arg), double), param);
                                                        end  = true;
                                                        break;

                                      case __C('e')   :
                                      case __C('E')   :

                                      case __C('n')   :
                                      case __C('p')   : end = true;
                                                        break;

                                      case __C('s')   :
                                      case __C('S')   : //str = (XCHAR*)va_arg(arg,XCHAR*);
                                                        //end = true;

                                                        { str.Set((char*)va_arg((arg),char*));

                                                          XSTRING sizestr;

                                                          sizestr = param;
                                                          if(sizestr.GetSize()>2)
                                                            {
                                                              sizestr.ToUpperCase();
                                                              sizestr.DeleteCharacter(__C('%'));
                                                              sizestr.DeleteCharacter(__C('S'));

                                                              if(!sizestr.IsEmpty())
                                                                {
                                                                  int sizeadjust = sizestr.ConvertToInt();
                                                                  if(abs(sizeadjust)>(int)str.GetSize())
                                                                    {
                                                                      str.AdjustSize(abs(sizeadjust),(sizeadjust>0)?true:false, isspecialweb?__L("&nbsp"):__L(" "), isspecialweb);
                                                                    }
                                                                }
                                                            }

                                                          end = true;
                                                        }
                                                        break;

                                      case __C('%')   : str = __L("%");
                                                        end = true;
                                                        break;

                                      case __C('\x0') : end = true;
                                                        break;

                                          default     : break;
                                    }

                                  c++;

                                } while(!end);
                            }
                            break;

          case __C('<')   : { openlabel = true;
                              XCHAR character[2]={ mask[c], 0 };
                              str.Set(character);
                              c++;
                            }
                            break;

          case __C('>')   : { openlabel = false;
                              XCHAR character[2]={ mask[c], 0 };
                              str.Set(character);
                              c++;
                            }
                            break;

          case 0x0A       : if(isspecialweb && (!openlabel))
                              {
                                str = __L("<br />");
                              }
                             else
                              {
                                XCHAR character[2]={ mask[c], 0 };
                                str.Set(character);
                              }
                            c++;
                            break;

          case 0x20       : if(isspecialweb && (!openlabel))
                              {
                                str = __L("&nbsp");
                              }
                             else
                              {
                                XCHAR character[2]={ mask[c], 0 };
                                str.Set(character);
                              }
                            c++;
                            break;

             default      : { XCHAR   character[2]={ mask[c], 0 };
                              XDWORD  value       = (XDWORD)mask[c];

                              if(isspecialweb && (!openlabel) && (value > 128))
                                    str.Format(__L("&#%d"), value);
                               else str.Set(character);

                              c++;
                            }
                            break;
        }

      output.Add(str);
    }

  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  XDEBUG_PRINTCOLOR(3, output.Get());

  va_end(arg);

  va_list chainload;
  va_start(chainload, format);
  ilclient_debug_output(format, chainload);
  va_end(chainload);
}


const char *err2str(int err) {
    switch (err) {
    case OMX_ErrorInsufficientResources: return "OMX_ErrorInsufficientResources";
    case OMX_ErrorUndefined: return "OMX_ErrorUndefined";
    case OMX_ErrorInvalidComponentName: return "OMX_ErrorInvalidComponentName";
    case OMX_ErrorComponentNotFound: return "OMX_ErrorComponentNotFound";
    case OMX_ErrorInvalidComponent: return "OMX_ErrorInvalidComponent";
    case OMX_ErrorBadParameter: return "OMX_ErrorBadParameter";
    case OMX_ErrorNotImplemented: return "OMX_ErrorNotImplemented";
    case OMX_ErrorUnderflow: return "OMX_ErrorUnderflow";
    case OMX_ErrorOverflow: return "OMX_ErrorOverflow";
    case OMX_ErrorHardware: return "OMX_ErrorHardware";
    case OMX_ErrorInvalidState: return "OMX_ErrorInvalidState";
    case OMX_ErrorStreamCorrupt: return "OMX_ErrorStreamCorrupt";
    case OMX_ErrorPortsNotCompatible: return "OMX_ErrorPortsNotCompatible";
    case OMX_ErrorResourcesLost: return "OMX_ErrorResourcesLost";
    case OMX_ErrorNoMore: return "OMX_ErrorNoMore";
    case OMX_ErrorVersionMismatch: return "OMX_ErrorVersionMismatch";
    case OMX_ErrorNotReady: return "OMX_ErrorNotReady";
    case OMX_ErrorTimeout: return "OMX_ErrorTimeout";
    case OMX_ErrorSameState: return "OMX_ErrorSameState";
    case OMX_ErrorResourcesPreempted: return "OMX_ErrorResourcesPreempted";
    case OMX_ErrorPortUnresponsiveDuringAllocation: return "OMX_ErrorPortUnresponsiveDuringAllocation";
    case OMX_ErrorPortUnresponsiveDuringDeallocation: return "OMX_ErrorPortUnresponsiveDuringDeallocation";
    case OMX_ErrorPortUnresponsiveDuringStop: return "OMX_ErrorPortUnresponsiveDuringStop";
    case OMX_ErrorIncorrectStateTransition: return "OMX_ErrorIncorrectStateTransition";
    case OMX_ErrorIncorrectStateOperation: return "OMX_ErrorIncorrectStateOperation";
    case OMX_ErrorUnsupportedSetting: return "OMX_ErrorUnsupportedSetting";
    case OMX_ErrorUnsupportedIndex: return "OMX_ErrorUnsupportedIndex";
    case OMX_ErrorBadPortIndex: return "OMX_ErrorBadPortIndex";
    case OMX_ErrorPortUnpopulated: return "OMX_ErrorPortUnpopulated";
    case OMX_ErrorComponentSuspended: return "OMX_ErrorComponentSuspended";
    case OMX_ErrorDynamicResourcesUnavailable: return "OMX_ErrorDynamicResourcesUnavailable";
    case OMX_ErrorMbErrorsInFrame: return "OMX_ErrorMbErrorsInFrame";
    case OMX_ErrorFormatNotDetected: return "OMX_ErrorFormatNotDetected";
    case OMX_ErrorContentPipeOpenFailed: return "OMX_ErrorContentPipeOpenFailed";
    case OMX_ErrorContentPipeCreationFailed: return "OMX_ErrorContentPipeCreationFailed";
    case OMX_ErrorSeperateTablesUsed: return "OMX_ErrorSeperateTablesUsed";
    case OMX_ErrorTunnelingUnsupported: return "OMX_ErrorTunnelingUnsupported";
    default: return "unknown error";
    }
}


/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::VIDEOLINUXCODECH264ILCLIENT
*/
/**
//
//  Class Constructor VIDEOLINUXCODECH264ILCLIENT
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/04/2016 17:23:45
//
*/
/*-----------------------------------------------------------------*/
VIDEOLINUXCODECH264ILCLIENT::VIDEOLINUXCODECH264ILCLIENT(VIDEOPLAYER* videoplayer) : VIDEOCODECH264(videoplayer)
{
  Clean();

  framereader = new MEDIAFRAMEREADERLIBAV();

  memset(list, 0, sizeof(COMPONENT_T*)*5);
  memset(tunnels, 0, sizeof(TUNNEL_T)*4);
}




/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::~VIDEOLINUXCODECH264ILCLIENT
*/
/**
//
//   Class Destructor VIDEOLINUXCODECH264ILCLIENT
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/04/2016 17:24:01
//
*/
/*-----------------------------------------------------------------*/
VIDEOLINUXCODECH264ILCLIENT::~VIDEOLINUXCODECH264ILCLIENT()
{
  //// free memory for all pending ILPACKET
  delete framereader;
  framereader = NULL;


  XLIST<MEDIAPACKET*>::XITERATOR it;
  //for(XDWORD i = 0; i < packets.GetSize(); i++)
  for(it = packets.Begin(); it != packets.End(); it++)
    {
      //XDEBUG_PRINTCOLOR(2, __L("PACKET DELETE"));
      MEDIAPACKET* p = *it;

      delete p->buffer;
      delete p;
    }

  packets.DeleteAll();

  xfactory->Delete_Mutex(packetmutex);

  Clean();
}




/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::callback
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/05/2016 13:24:26
//
//  @param        data :
//  @param        comp :
*/
/*-----------------------------------------------------------------*/
void VIDEOLINUXCODECH264ILCLIENT::callback(void* data, COMPONENT_T* comp)
{
  MEDIAFRAMEREADERLIBAV* param = (MEDIAFRAMEREADERLIBAV*)data;

  XDEBUG_PRINTCOLOR(4, __L("Frame"));
}

/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::Ini
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/04/2016 17:24:09
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool VIDEOLINUXCODECH264ILCLIENT::Ini()
{
  //videoplayer->GetRenderer()->GetContext()->Destroy(); // here should work?

  //framereader->ResetStream();
  MEDIAFRAMEREADERLIBAV * avframereader = (MEDIAFRAMEREADERLIBAV*)framereader;
  packetmutex = xfactory->Create_Mutex();
  if(!packetmutex) return false;

  if(!(client = ilclient_init())) return false;

  // not sure
  //vcos_log_set_level(VCOS_LOG_CATEGORY, VCOS_LOG_TRACE);

  if(OMX_Init() != OMX_ErrorNone)
    {
      // deinitialize ilclient
      XDEBUG_PRINTCOLOR(4, __L("Failed to Initialize OpenMAX"));
      return false;
    }


  //debug_output = gen_debug_output;

  //ilclient_set_empty_buffer_done_callback(client, callback, this);

  ilclient_set_eos_callback(client, eoscallback, videoplayer);

  CreateDecoderComponent(client, const_cast<char*>("video_decode"), &list[0]);
  CreateRenderComponent(client, const_cast<char*>("video_render"), &list[1]);
  CreateSchedulerComponent(client, const_cast<char*>("video_scheduler"), &list[2]);
  CreateClockComponent(client, const_cast<char*>("clock"), &list[3]);

  //XDEBUG_PRINTCOLOR(1, __L("Created components"));

  int err;

  err = ilclient_enable_port_buffers(list[0], 130, NULL, NULL, NULL);

  //XDEBUG_PRINTCOLOR(1, __L("Enabled Port Buffers"));

  ilclient_enable_port(list[0], 130);

  ilclient_change_component_state(list[0], OMX_StateExecuting);

  //XDEBUG_PRINTCOLOR(1, __L("Changed State to executing"));

  SendDecoderConfig(list[0]);

  //XDEBUG_PRINTCOLOR(1, __L("Sent DecoderConfig"));

  // now we need to read frames to send to the decoder until we detect a change in prot settings and then setup the tunnel
  XVECTOR<XBYTE> frame;
  long long pts;
  long long dts;

  // first copy the extradata for the Codec
  OMX_BUFFERHEADERTYPE* config = ilclient_get_input_buffer(list[0], 130, 1);
  if(config)
    {
      memcpy(config->pBuffer, avframereader->GetSPS()->GetPointer(0), avframereader->GetSPS()->GetSize());
      config->nOffset = 0;
      config->nFilledLen = avframereader->GetSPS()->GetSize();
      config->nFlags = OMX_BUFFERFLAG_CODECCONFIG | OMX_BUFFERFLAG_ENDOFFRAME;
      OMX_EmptyThisBuffer(ILC_GET_HANDLE(list[0]), config);
    }

  //XDEBUG_PRINTCOLOR(1, __L("Port Settings Changed"));

  // now we need to setup tunnels as port settings changed
  set_tunnel(tunnels, list[0], 131, list[2], 10);



  if(ilclient_setup_tunnel(tunnels, 0, 0) < 0)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed to setup Decode tunnel"));
      return false;
    }

  //XDEBUG_PRINTCOLOR(1, __L("Decode Tunnel Set"));


  set_tunnel(tunnels + 1, list[2], 11, list[1], 90); // 220 for texture
  if(ilclient_setup_tunnel(tunnels + 1, 0, 0) < 0)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed to setup Scheduler tunnel"));
      return false;
    }

  //XDEBUG_PRINTCOLOR(1, __L("Scheduler Tunnel Set"));

  set_tunnel(tunnels + 2, list[3], 80, list[2], 12);
  if(ilclient_setup_tunnel(tunnels + 2, 0, 0) < 0)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed to setup Clock tunnel"));
      return false;
    }

  //XDEBUG_PRINTCOLOR(1, __L("Clock Tunnel Set"));

  //XDEBUG_PRINTCOLOR(1, __L("Tunnels Set"));

  // Start the Clock
  StartClock(list[3]);

  //XDEBUG_PRINTCOLOR(1, __L("Clock Started"));

  // Enable all the ports back
  OMX_SendCommand(ILC_GET_HANDLE(list[0]), OMX_CommandPortEnable, 131, NULL);
  ilclient_enable_port(list[0], 131);

  OMX_SendCommand(ILC_GET_HANDLE(list[3]), OMX_CommandPortEnable, 80, NULL);
  ilclient_enable_port(list[3], 80);

  OMX_SendCommand(ILC_GET_HANDLE(list[2]), OMX_CommandPortEnable, 10, NULL);
  ilclient_enable_port(list[2], 10);

  OMX_SendCommand(ILC_GET_HANDLE(list[2]), OMX_CommandPortEnable, 11, NULL);
  ilclient_enable_port(list[2], 11);

  OMX_SendCommand(ILC_GET_HANDLE(list[2]), OMX_CommandPortEnable, 12, NULL);
  ilclient_enable_port(list[2], 12);

  OMX_SendCommand(ILC_GET_HANDLE(list[1]), OMX_CommandPortEnable, 90, NULL);
  ilclient_enable_port(list[1], 90);

  // change components state
  err = ilclient_change_component_state(list[0], OMX_StateExecuting);
  if(err < 0)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed changing Decode State to Executing"));
      return false;
    }

  err = ilclient_change_component_state(list[1], OMX_StateExecuting);
  if(err < 0)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed changing Render State to Executing"));
      return false;
    }

  err = ilclient_change_component_state(list[2], OMX_StateExecuting);
  if(err < 0)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed changing Scheduler State to Executing"));
      return false;
    }

  err = ilclient_change_component_state(list[3], OMX_StateExecuting);
  if(err < 0)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed changing Clock State to Executing"));
      return false;
    }

  //XDEBUG_PRINTCOLOR(1, __L("States Changed, Init Finished"));

  initialized = true;
  XDEBUG_PRINTCOLOR(2, __L("IsPlaying True"));
  isplaying = true;
  //isplaying = false;

  return true;
}





/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::End
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/04/2016 17:24:20
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool VIDEOLINUXCODECH264ILCLIENT::End()
{
  if(!ended)
    {


      OMX_STATETYPE state;
      OMX_ERRORTYPE stateerror = OMX_GetState(ILC_GET_HANDLE(list[0]), &state);

      if(stateerror != OMX_ErrorNone)
        {
          const char* cstrerror = err2str(stateerror);
          XSTRING xstrerror(cstrerror);
          XDEBUG_PRINTCOLOR(4, __L("Failed Getting component State: %s"), xstrerror.Get());
        }
      else
        {
       XSTRING str;

       switch(state)
         {
         case OMX_StateInvalid:
           str.Set(__L("OMX_StateInvalid"));
           break;
         case OMX_StateLoaded:
           str.Set(__L("OMX_StateLoaded"));
           break;
         case OMX_StateIdle:
           str.Set(__L("OMX_StateIdle"));
           break;
         case OMX_StateExecuting:
           str.Set(__L("OMX_StateExecuting"));
           break;
         case OMX_StatePause:
           str.Set(__L("OMX_StatePause"));
           break;
         case OMX_StateWaitForResources:
           str.Set(__L("OMX_StateWaitForResources"));
           break;
         case OMX_StateKhronosExtensions:
           str.Set(__L("OMX_StateKhronosExtensions"));
           break;
         case OMX_StateVendorStartUnused:
           str.Set(__L("OMX_StateVendorStartUnused"));
           break;
         case OMX_StateMax:
           str.Set(__L("OMX_StateMax"));
           break;
         }

          //XDEBUG_PRINTCOLOR(3, __L("decode Component State: %s"), str.Get());
        }

       OMX_ERRORTYPE error = OMX_SendCommand(ILC_GET_HANDLE(list[0]), OMX_CommandStateSet, OMX_StateIdle, NULL);
       if(error != OMX_ErrorNone)
        {
          const char* cstrerror = err2str(error);
          XSTRING xstrerror(cstrerror);
          XDEBUG_PRINTCOLOR(4, __L("Failed Setting component State: %s"), xstrerror.Get());
          //XDEBUG_PRINTCOLOR(3, __L("OMX ERROR %u when setting state decoder"), error);
        }



       error = OMX_SendCommand(ILC_GET_HANDLE(list[1]), OMX_CommandStateSet, OMX_StateIdle, NULL);
       if(error != OMX_ErrorNone)
        {
          const char* cstrerror = err2str(error);
          XSTRING xstrerror(cstrerror);
          XDEBUG_PRINTCOLOR(4, __L("Failed Setting component State: %s"), xstrerror.Get());
        }

       state;
       stateerror = OMX_GetState(ILC_GET_HANDLE(list[1]), &state);

       if(stateerror != OMX_ErrorNone)
        {
          const char* cstrerror = err2str(stateerror);
          XSTRING xstrerror(cstrerror);
          XDEBUG_PRINTCOLOR(4, __L("Failed Getting component State: %s"), xstrerror.Get());
        }
       else
       {
         XSTRING str;

         switch(state)
         {
         case OMX_StateInvalid:
           str.Set(__L("OMX_StateInvalid"));
           break;
         case OMX_StateLoaded:
           str.Set(__L("OMX_StateLoaded"));
           break;
         case OMX_StateIdle:
           str.Set(__L("OMX_StateIdle"));
           break;
         case OMX_StateExecuting:
           str.Set(__L("OMX_StateExecuting"));
           break;
         case OMX_StatePause:
           str.Set(__L("OMX_StatePause"));
           break;
         case OMX_StateWaitForResources:
           str.Set(__L("OMX_StateWaitForResources"));
           break;
         case OMX_StateKhronosExtensions:
           str.Set(__L("OMX_StateKhronosExtensions"));
           break;
         case OMX_StateVendorStartUnused:
           str.Set(__L("OMX_StateVendorStartUnused"));
           break;
         case OMX_StateMax:
           str.Set(__L("OMX_StateMax"));
           break;
         }

         //XDEBUG_PRINTCOLOR(3, __L("render Component State: %s"), str.Get());
       }


       error = OMX_SendCommand(ILC_GET_HANDLE(list[2]), OMX_CommandStateSet, OMX_StateIdle, NULL);
       if(error != OMX_ErrorNone)
        {
          const char* cstrerror = err2str(error);
          XSTRING xstrerror(cstrerror);
          XDEBUG_PRINTCOLOR(4, __L("Failed Setting component State: %s"), xstrerror.Get());
        }

       error = OMX_SendCommand(ILC_GET_HANDLE(list[3]), OMX_CommandStateSet, OMX_StateIdle, NULL);
       if(error != OMX_ErrorNone)
        {
          const char* cstrerror = err2str(error);
          XSTRING xstrerror(cstrerror);
          XDEBUG_PRINTCOLOR(4, __L("Failed Setting component State: %s"), xstrerror.Get());
        }

       // transition to the real stop state
      ilclient_state_transition(list, OMX_StateLoaded);

      // remove callbacks beforehand ? worth a try
      ilclient_set_eos_callback(client, NULL, NULL);


      //XDEBUG_PRINTCOLOR(3, __L("Flushing IL tunnel"));
      ilclient_flush_tunnels(tunnels, 0);


      //XDEBUG_PRINTCOLOR(3, __L("Disabling port buffer"));
      ilclient_disable_port_buffers(list[0], 130, NULL, NULL, NULL);

      // this disables the ports correspoding to the tunnels
      //XDEBUG_PRINTCOLOR(3, __L("tunnel[0]"));
      ilclient_disable_tunnel(tunnels);
      //XDEBUG_PRINTCOLOR(3, __L("tunnel[1]"));
      ilclient_disable_tunnel(tunnels+1);
      //XDEBUG_PRINTCOLOR(3, __L("tunnel[2]"));
      ilclient_disable_tunnel(tunnels+2);

      //XDEBUG_PRINTCOLOR(3, __L("tunnels teardown"));
      ilclient_teardown_tunnels(tunnels);

      // Destroy all components, but beforehand they need tunnels to be destroyed
      // list if a NULL terminated array of components
      //XDEBUG_PRINTCOLOR(3, __L("Disabling the components"));
      //ilclient_state_transition(list, OMX_StateIdle);
      //XDEBUG_PRINTCOLOR(3, __L("Idle Transitioned"));
      //ilclient_state_transition(list, OMX_StateLoaded);
      //XDEBUG_PRINTCOLOR(3, __L("Loaded Transitioned"));
      ilclient_cleanup_components(list);

      // we need to destroy all components before calling these functions

      //XDEBUG_PRINTCOLOR(3, __L("DeInit OpenMAX"));
      error = OMX_Deinit(); // maybe? need to make sure if this is OK

      if(error != OMX_ErrorNone)
        {
          XDEBUG_PRINTCOLOR(4, __L("Couln't deinit OMX properly"));
        }

      //XDEBUG_PRINTCOLOR(3, __L("destroy ilclient"));
      ilclient_destroy(client);

      ended = true;
    }

  return true;
}



/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::SendFrame
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      11/04/2016 17:24:27
//
//  @return       bool :
//
//  @param        frame :
*/
/*-----------------------------------------------------------------*/
bool VIDEOLINUXCODECH264ILCLIENT::SendFrame(MEDIAPACKET* packet)
{
  OMX_BUFFERHEADERTYPE* buffer = GetInputBuffer(list[0], 130);

  if(buffer)
    {
      XBYTE* data = packet->buffer;
      XDWORD size = packet->size;
      long long pts = packet->pts;
      long long dts = packet->dts;

      //XDEBUG_PRINTCOLOR(2, __L("Copy and empty"));
      CopyToBufferAndEmpty(data, size, list[0], buffer, pts, dts);
      //XDEBUG_PRINTCOLOR(2, __L("Ended Copy and empty"));
    }
  else
    {
      return false;
    }
  return true;
}

void VIDEOLINUXCODECH264ILCLIENT::Update()
{
  if(bufferedsize < BUFFEREDSIZEMAX)
    {

      MEDIAPACKET* packet = new MEDIAPACKET;
      packet->buffer = NULL;
      packet->size = 0;
      packet->pts = 0;
      packet->dts = 0;

      MEDIAFRAMEREADERLIBAV * avframereader = (MEDIAFRAMEREADERLIBAV*)framereader;

      //XDEBUG_PRINTCOLOR(3, __L("Reading frame"));
      if(!avframereader->ReadFrame(packet))
      {
        if(packet->buffer)
          {
            delete [] packet->buffer;
          }

        delete packet;
        return;
      }

      // add it to the packet list
      if(packetmutex)
        {
          //if(!pkt)
          //  {
          //    // delete the package and don't add it to the list(but broadcast the condition
          //    #if LIBAVFORMAT_VERSION_MAJOR < 56
          //    av_free_packet(pkt);
          //    #else
          //    av_packet_unref(pkt);
          //    #endif
          //    delete packet->frame;
          //    delete packet;
          //    return;
          //  }

          packetmutex->Lock();
          packets.Add(packet);
          bufferedsize += packet->size;
          packetmutex->UnLock();
          //XDEBUG_PRINTCOLOR(0, __L("Update UnLock"));
        }
    }
  // will probably need to keep track of how big our cached frames are ?
}






/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::Decode
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/05/2016 13:24:11
//
*/
/*-----------------------------------------------------------------*/
void VIDEOLINUXCODECH264ILCLIENT::Decode()
{
  //XDEBUG_PRINTCOLOR(2, __L("Decode"));

  if(!initialized)
    {
      this->videoplayer->GetRenderer()->GetContext()->Destroy();
      Ini();

      initialized = true;
      XDEBUG_PRINTCOLOR(2, __L("IsPlaying False"));
      isplaying = true;
    }

  //XDEBUG_PRINTCOLOR(2, __L("Reading Frame"));
  if(!framereader)
    {
      XDEBUG_PRINTCOLOR(2, __L("No FrameReader"));
    }
  else
    {
      //// need to perform this to get a frame from the mp4
      //frame.Resize(0);
      //framereader->ReadFrame(frame, &ppts, &pdts);
      isplaying = true;
      ////XDEBUG_PRINTCOLOR(2, __L("Actual Frame Decoding"));

      MEDIAPACKET* packet = NULL;

      //XDEBUG_PRINTCOLOR(0, __L("Decode Lock"));
      packetmutex->Lock();

      if(framereader->IsEOF())
      {
        XDWORD packetsnum = packets.GetSize();
        //XDEBUG_PRINTCOLOR(0, __L("EOF: Packets %u"), packetsnum);
      }

      if(packets.GetSize() == 0)
      {
        // need to make sure we need to wait for condition

        //XDEBUG_PRINTCOLOR(3, __L("Prepare EOS Signal"));

        //XLINUXMUTEX* lmutex = (XLINUXMUTEX*)packetmutex;
        packetmutex->UnLock();

        if(framereader->IsEOF() && !eos)
        {
          SendEOSSignal(list[0]);

          // might need a wait for EOS event in here?

          //isplaying = false;
        }
        else if(framereader->IsEOF() && eos)
        {
          XDEBUG_PRINTCOLOR(2, __L("IsPlaying False"));
          isplaying = false;
          //this->End();
        }

        return;
      }

      //AVPacket pkt;

      if(packets.GetSize() > 0)
        {
          XDWORD packetssize = packets.GetSize();

          //XDEBUG_PRINTCOLOR(0, __L("packets list size: %d"), packetssize);

          packet=packets.GetFirst();
          packets.DeleteIndex(0);
          bufferedsize -= packet->size;
          //pkt.data = packet->buffer;
          //pkt.size = packet->size;
          //pkt.dts = packet->dts;
          //pkt.pts = packet->pts;
        }


      packetmutex->UnLock();
      //XDEBUG_PRINTCOLOR(0, __L("Decode UnLock"));

      if(packet)
        {
          //XDEBUG_PRINTCOLOR(0, __L("Sending Packet"));
          SendFrame(packet);
          //XDEBUG_PRINTCOLOR(0, __L("Packet Sent"));
          //AVPacket* pkt = (AVPacket*)packet->frame->GetPointer(0);
          //XDEBUG_PRINTCOLOR(0, __L("Deleting Packet"));
          //#if LIBAVFORMAT_VERSION_MAJOR < 56
          //av_free_packet(pkt);
          //#else
          //av_packet_unref(pkt);
          //#endif
          delete [] packet->buffer;
          delete packet;
        }
      else
        {
          XDEBUG_PRINTCOLOR(0, __L("Waiting Packet"));
        }
    }
}






/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::SendDecoderConfig
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/05/2016 13:24:04
//
//  @return       bool :
//
//  @param        component :
*/
/*-----------------------------------------------------------------*/
bool VIDEOLINUXCODECH264ILCLIENT::SendDecoderConfig(COMPONENT_T* component)
{
  OMX_ERRORTYPE err = OMX_ErrorNone;

  OMX_BUFFERHEADERTYPE* buffer = ilclient_get_input_buffer(component, 130, 1);

  MEDIAFRAMEREADERLIBAV * avframereader = (MEDIAFRAMEREADERLIBAV*)framereader;

  if(!framereader)
    {
      return false;
    }

  if(!avframereader->GetSPS())
    {
      return false;
    }

  buffer->nFilledLen = avframereader->GetSPS()->GetSize();
  memcpy(buffer->pBuffer, avframereader->GetSPS()->GetPointer(0), avframereader->GetSPS()->GetSize());
  buffer->nFlags = OMX_BUFFERFLAG_CODECCONFIG | OMX_BUFFERFLAG_ENDOFFRAME;

  err = OMX_EmptyThisBuffer(ILC_GET_HANDLE(component), buffer);

  if(err != OMX_ErrorNone)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed to send video extradata"));
      return false;
    }

  return true;
}





/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::CreateDecoderComponent
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/05/2016 13:23:57
//
//  @return       bool :
//
//  @param        client :
//  @param        componentname :
//  @param        component :
*/
/*-----------------------------------------------------------------*/
bool VIDEOLINUXCODECH264ILCLIENT::CreateDecoderComponent(ILCLIENT_T* client, char* componentname, COMPONENT_T** component)
{
  int err;

  err = ilclient_create_component(client, component, componentname, (ILCLIENT_CREATE_FLAGS_T)(ILCLIENT_DISABLE_ALL_PORTS | ILCLIENT_ENABLE_INPUT_BUFFERS | ILCLIENT_ENABLE_OUTPUT_BUFFERS));
  if(err == -1)
    {
      XDEBUG_PRINTCOLOR(4, __L("ILCLIENT: Create Decode Component Failed"));
      return false;
    }

  err = ilclient_change_component_state(*component, OMX_StateIdle);
  if(err < 0)
    {
      XDEBUG_PRINTCOLOR(4, __L("ILCLIENT: Changing Decode ComponentState to Idle Failed"));
      return false;
    }


  // we now set the video format
  OMX_VIDEO_PARAM_PORTFORMATTYPE videoPortFormat;

  memset(&videoPortFormat, 0, sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE));
  videoPortFormat.nSize = sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE);
  videoPortFormat.nVersion.nVersion = OMX_VERSION;
  videoPortFormat.nPortIndex = 130;

  err = OMX_GetParameter(ILC_GET_HANDLE(*component), OMX_IndexParamVideoPortFormat, &videoPortFormat);
  if(err != OMX_ErrorNone)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed Getting VideoPortFormat"));
      return false;
    }

  videoPortFormat.nPortIndex = 130;
  videoPortFormat.nIndex = 0;
  videoPortFormat.eCompressionFormat = OMX_VIDEO_CodingAVC;
  videoPortFormat.eColorFormat = OMX_COLOR_FormatUnused;
  videoPortFormat.xFramerate = 0; // ignore it like this for now

  err = OMX_SetParameter(ILC_GET_HANDLE(*component), OMX_IndexParamVideoPortFormat, &videoPortFormat);
  if(err != OMX_ErrorNone)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed Setting VideoPortFormat"));
      return false;
    }

  OMX_PARAM_PORTDEFINITIONTYPE portParam;
  memset(&portParam, 0, sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
  portParam.nSize=(sizeof(OMX_PARAM_PORTDEFINITIONTYPE));
  portParam.nVersion.nVersion = OMX_VERSION;
  portParam.nPortIndex = 130;

  err = OMX_GetParameter(ILC_GET_HANDLE(*component), OMX_IndexParamPortDefinition, &portParam);
  if(err != OMX_ErrorNone)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed Getting PortParam"));
      return false;
    }

  MEDIAFRAMEREADERLIBAV * avframereader = (MEDIAFRAMEREADERLIBAV*)framereader;

  portParam.nPortIndex = 130;
  portParam.format.video.nFrameWidth = avframereader->width;
  portParam.format.video.nFrameHeight = avframereader->height;

  err = OMX_SetParameter(ILC_GET_HANDLE(*component), OMX_IndexParamPortDefinition, &portParam);
  if(err != OMX_ErrorNone)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed Setting PortParam"));
      return false;
    }

  // set the NAL stream format type
  bool naluformat = false;

  int in_extrasize = avframereader->GetSPS()->GetSize();
  XBYTE* in_extradata = avframereader->GetSPS()->GetPointer(0);

  if (in_extrasize < 7 || in_extradata == NULL)
    {
      naluformat = true;
    }
  else if ( *in_extradata != 1 )
    {
      // valid avcC atom data always starts with the value 1 (version), otherwise annexb
      naluformat = true;
    }

  if(naluformat)
    {
      OMX_NALSTREAMFORMATTYPE nalstream;
      nalstream.nSize = sizeof(OMX_NALSTREAMFORMATTYPE);
      nalstream.nVersion.nVersion = OMX_VERSION;
      nalstream.nPortIndex = 130;
      nalstream.eNaluFormat = OMX_NaluFormatStartCodes;

      err = OMX_SetParameter(ILC_GET_HANDLE(*component), (OMX_INDEXTYPE)OMX_IndexParamNalStreamFormatSelect, &nalstream);
      if(err != OMX_ErrorNone)
        {
          XDEBUG_PRINTCOLOR(4, __L("Failed Setting NALU format"));
          return false;
        }
    }

  return true;
}






/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::CopyToBufferAndEmpty
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/05/2016 13:23:49
//
//  @return       bool :
//
//  @param        data :
//  @param        size :
//  @param        component :
//  @param        buffer :
//  @param        pts :
//  @param        dts :
*/
/*-----------------------------------------------------------------*/
bool VIDEOLINUXCODECH264ILCLIENT::CopyToBufferAndEmpty(XBYTE* data, XDWORD size, COMPONENT_T* component, OMX_BUFFERHEADERTYPE* buffer, long long pts, long long dts)
{
  OMX_ERRORTYPE err = OMX_ErrorNone;

  int buff_size = buffer->nAllocLen;
  XBYTE* content = data;

  while(size > 0)
    {
      buffer->nFilledLen = (size > buffer->nAllocLen-1) ? buffer->nAllocLen-1 : size;
      memset(buffer->pBuffer, 0x0, buffer->nAllocLen);
      memcpy(buffer->pBuffer, content, buffer->nFilledLen);
      size -= buffer->nFilledLen;
      content += buffer->nFilledLen;

      buffer->nFlags = 0;

      if(size <= 0)
        {
          buffer->nFlags |= OMX_BUFFERFLAG_ENDOFFRAME;
        }

      // need the AVPacket dts in here to setup correct flags
      if(dts == 0)
        {
          buffer->nFlags |= OMX_BUFFERFLAG_STARTTIME;
        }
      else
        {
          MEDIAFRAMEREADERLIBAV * avframereader = (MEDIAFRAMEREADERLIBAV*)framereader;
          buffer->nTimeStamp = ilclient_ticks_from_s64(pts*1000000/avframereader->time_base_den);
        }

      err = OMX_EmptyThisBuffer(ILC_GET_HANDLE(component), buffer);

      if(err != OMX_ErrorNone)
        {
          XDEBUG_PRINTCOLOR(4, __L("Failed to Empty the OpenMAX input buffer, error: %x"), err);
        }

      if(size > 0)
        {
          buffer = GetInputBuffer(component, 130);
        }
      }

  return true;
}





/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::StartClock
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/05/2016 13:23:42
//
//  @return       bool :
//
//  @param        component :
*/
/*-----------------------------------------------------------------*/
bool VIDEOLINUXCODECH264ILCLIENT::StartClock(COMPONENT_T* component)
{
  OMX_ERRORTYPE err = OMX_ErrorNone;
  OMX_TIME_CONFIG_CLOCKSTATETYPE clockState;

  memset(&clockState, 0, sizeof(OMX_TIME_CONFIG_CLOCKSTATETYPE));
  clockState.nSize = sizeof(OMX_TIME_CONFIG_CLOCKSTATETYPE);
  clockState.nVersion.nVersion = OMX_VERSION;

  err = OMX_GetConfig(ILC_GET_HANDLE(component), OMX_IndexConfigTimeClockState, &clockState);

  if(err != OMX_ErrorNone)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed to get the Clock Config"));
      return false;
    }

  clockState.eState = OMX_TIME_ClockStateRunning;
  err = OMX_SetConfig(ILC_GET_HANDLE(component), OMX_IndexConfigTimeClockState, &clockState);

  if(err != OMX_ErrorNone)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed to set the Clock Config"));
      return false;
    }

  return true;
}




/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::CreateRenderComponent
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/05/2016 13:23:33
//
//  @return       bool :
//
//  @param        client :
//  @param        componentname :
//  @param        component :
*/
/*-----------------------------------------------------------------*/
bool VIDEOLINUXCODECH264ILCLIENT::CreateRenderComponent(ILCLIENT_T* client, char* componentname, COMPONENT_T** component)
{
  int err;

  err = ilclient_create_component(client, component, componentname, (ILCLIENT_CREATE_FLAGS_T)(ILCLIENT_DISABLE_ALL_PORTS | ILCLIENT_ENABLE_INPUT_BUFFERS));

  if(err == -1)
    {
      XDEBUG_PRINTCOLOR(4, __L("ILCLIENT: Create Render Component Failed"));
      return false;
    }

  err = ilclient_change_component_state(*component, OMX_StateIdle);
  if(err < 0)
    {
      XDEBUG_PRINTCOLOR(4, __L("ILCLIENT: Changing Render ComponentState to Idle Failed"));
      return false;
    }

  return true;
}




/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::CreateSchedulerComponent
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/05/2016 13:23:27
//
//  @return       bool :
//
//  @param        client :
//  @param        componentname :
//  @param        component :
*/
/*-----------------------------------------------------------------*/
bool VIDEOLINUXCODECH264ILCLIENT::CreateSchedulerComponent(ILCLIENT_T* client, char* componentname, COMPONENT_T** component)
{
  int err;

  err = ilclient_create_component(client, component, componentname, (ILCLIENT_CREATE_FLAGS_T)(ILCLIENT_DISABLE_ALL_PORTS | ILCLIENT_ENABLE_INPUT_BUFFERS));

  if(err == -1)
    {
      XDEBUG_PRINTCOLOR(4, __L("ILCLIENT: Create Scheduler Component Failed"));
      return false;
    }

  err = ilclient_change_component_state(*component, OMX_StateIdle);
  if(err < 0)
    {
      XDEBUG_PRINTCOLOR(4, __L("ILCLIENT: Changing Scheduler ComponentState to Idle Failed"));
      return false;
    }

  return true;
}





/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::CreateClockComponent
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/05/2016 13:23:18
//
//  @return       bool :
//
//  @param        client :
//  @param        componentname :
//  @param        component :
*/
/*-----------------------------------------------------------------*/
bool VIDEOLINUXCODECH264ILCLIENT::CreateClockComponent(ILCLIENT_T* client, char* componentname, COMPONENT_T** component)
{
  int err;

  err = ilclient_create_component(client, component, componentname, (ILCLIENT_CREATE_FLAGS_T)(ILCLIENT_DISABLE_ALL_PORTS | ILCLIENT_ENABLE_INPUT_BUFFERS));

  if(err == -1)
    {
      XDEBUG_PRINTCOLOR(4, __L("ILCLIENT: Create Clock Component Failed"));
      return false;
    }

  err = ilclient_change_component_state(*component, OMX_StateIdle);
  if(err < 0)
    {
      XDEBUG_PRINTCOLOR(4, __L("ILCLIENT: Changing Clock ComponentState to Idle Failed"));
      return false;
    }

  //OMX_COMPONENTTYPE* clock = ILC_GET_HANDLE(*component);
  OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE refClock;
  refClock.nSize = sizeof(OMX_TIME_CONFIG_ACTIVEREFCLOCKTYPE);
  refClock.nVersion.nVersion = OMX_VERSION;
  refClock.eClock = OMX_TIME_RefClockVideo;

  err = OMX_SetConfig(ILC_GET_HANDLE(*component), OMX_IndexConfigTimeActiveRefClock, &refClock);

  if(err != OMX_ErrorNone)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed Setting ActiveRefClock"));
      return false;
    }

  OMX_TIME_CONFIG_SCALETYPE scaleType;
  scaleType.nSize = sizeof(OMX_TIME_CONFIG_SCALETYPE);
  scaleType.nVersion.nVersion = OMX_VERSION;
  scaleType.xScale = 0x00010000;

  err = OMX_SetConfig(ILC_GET_HANDLE(*component), OMX_IndexConfigTimeScale, &scaleType);

  if(err != OMX_ErrorNone)
    {
      XDEBUG_PRINTCOLOR(4, __L("Failed Setting ScaleType"));
      return false;
    }

  return true;
}




/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::GetInputBuffer
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      20/05/2016 13:23:10
//
//  @return       OMX_BUFFERHEADERTYPE* :
//
//  @param        component :
//  @param        port :
*/
/*-----------------------------------------------------------------*/
OMX_BUFFERHEADERTYPE* VIDEOLINUXCODECH264ILCLIENT::GetInputBuffer(COMPONENT_T* component, int port)
{
  OMX_BUFFERHEADERTYPE* buffer = NULL;

  while(!buffer && isplaying)
    {
      buffer = ilclient_get_input_buffer(component, port, 0);
    }

  return buffer;
}




/*-------------------------------------------------------------------
//  VIDEOLINUXCODECH264ILCLIENT::SendEOSSignal
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      24/05/2016 12:09:24
//
//  @return       bool :
//
//  @param        component :
*/
/*-----------------------------------------------------------------*/
bool VIDEOLINUXCODECH264ILCLIENT::SendEOSSignal(COMPONENT_T* component)
{
  if(!eossent)
    {
      OMX_BUFFERHEADERTYPE* buffer = GetInputBuffer(component, 130);
      if(!buffer) return false;

      buffer->nFilledLen = 0;
      buffer->nOffset = 0;
      buffer->nTimeStamp = ilclient_ticks_from_s64(0);
      buffer->nFlags = OMX_BUFFERFLAG_ENDOFFRAME | OMX_BUFFERFLAG_EOS | OMX_BUFFERFLAG_TIME_UNKNOWN;

      OMX_ERRORTYPE  err = OMX_EmptyThisBuffer(ILC_GET_HANDLE(component), buffer);
      if(err != OMX_ErrorNone)
        {
          XDEBUG_PRINTCOLOR(4, __L("Failed to Empty EOS buffer"));
        }
      eossent = true;
    }


  return true;
}

#pragma pack(pop)
