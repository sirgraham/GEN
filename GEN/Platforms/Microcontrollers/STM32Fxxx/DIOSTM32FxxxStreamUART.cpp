//------------------------------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUART.CPP
//
//  STM32Fxxx Data IO Stream UART class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/01/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#if defined(DIO_ACTIVE) && defined(DIOUART_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XThreadCollected.h"

#include "DIOStreamXEvent.h"
#include "DIOStreamUARTConfig.h"

#include "DIOSTM32FxxxStreamUART.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------

DIOSTM32FXXXSTREAMUART*   diostreamuartptrhandle[3] = { NULL, NULL, NULL };

//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUART::DIOSTM32FXXXSTREAMUART
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/02/2013 23:11:51
//
//  @return
*/
/*-----------------------------------------------------------------*/
DIOSTM32FXXXSTREAMUART::DIOSTM32FXXXSTREAMUART() : DIOSTREAMUART(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOSTM32FXXXUARTFSMSTATE_NONE             ,
            DIOSTM32FXXXUARTFSMEVENT_CONNECTED        , DIOSTM32FXXXUARTFSMSTATE_CONNECTED         ,
            DIOSTM32FXXXUARTFSMEVENT_DISCONNECTING    , DIOSTM32FXXXUARTFSMSTATE_DISCONNECTING     ,
            EVENTDEFEND);

  AddState( DIOSTM32FXXXUARTFSMSTATE_CONNECTED        ,
            DIOSTM32FXXXUARTFSMEVENT_WAITINGTOREAD    , DIOSTM32FXXXUARTFSMSTATE_WAITINGTOREAD     ,
            DIOSTM32FXXXUARTFSMEVENT_DISCONNECTING    , DIOSTM32FXXXUARTFSMSTATE_DISCONNECTING     ,
            EVENTDEFEND);

  AddState( DIOSTM32FXXXUARTFSMSTATE_WAITINGTOREAD    ,
            DIOSTM32FXXXUARTFSMEVENT_CONNECTED        , DIOSTM32FXXXUARTFSMSTATE_CONNECTED         ,
            DIOSTM32FXXXUARTFSMEVENT_DISCONNECTING    , DIOSTM32FXXXUARTFSMSTATE_DISCONNECTING     ,
            EVENTDEFEND);

  AddState( DIOSTM32FXXXUARTFSMSTATE_DISCONNECTING    ,
            DIOSTM32FXXXUARTFSMEVENT_CONNECTED        , DIOSTM32FXXXUARTFSMSTATE_CONNECTED         ,
            DIOSTM32FXXXUARTFSMEVENT_WAITINGTOREAD    , DIOSTM32FXXXUARTFSMSTATE_WAITINGTOREAD     ,
            EVENTDEFEND);

  threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUART, __L("DIOSTM32FXXXSTREAMUART::DIOSTM32FXXXSTREAMUART"), ThreadConnexion, (void*)this);
}


//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUART::~DIOSTM32FXXXSTREAMUART
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
DIOSTM32FXXXSTREAMUART::~DIOSTM32FXXXSTREAMUART()
{
  Close();

  if(threadconnexion) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUART, threadconnexion);

  Clean();
}





//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUART::GetConnectStatus
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       DIOSTREAMSTATUS :
*/
//-------------------------------------------------------------------
DIOSTREAMSTATUS DIOSTM32FXXXSTREAMUART::GetConnectStatus()
{
  if(!config) return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}


//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUART::Open
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool DIOSTM32FXXXSTREAMUART::Open()
{
  if(!threadconnexion)  return false;

  switch(config->GetPort())
    {
      case  1 : huart.Instance = USART1;      break;
      case  2 : huart.Instance = USART2;      break;
      #ifndef HW_STM32F4XX
      case  3 : huart.Instance = USART3;      break;
      #endif
      default : return false;
    }

  indexport = (config->GetPort()-1);
  if(indexport < 0) return false;

  if(!Config(DIOSTREAMUARTMASK_ALL)) return false;

  if(HAL_UART_Init(&huart) != HAL_OK) return false;

  diostreamuartptrhandle[indexport] = this;

  __HAL_UART_ENABLE_IT(&huart, UART_IT_RXNE);

   switch(config->GetPort())
    {
      case  1 : HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(USART1_IRQn);
                break;

      case  2 : HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(USART2_IRQn);
                break;

      #ifndef HW_STM32F4XX
      case  3 :
                #ifdef HW_STM32F0XX
                HAL_NVIC_SetPriority(USART3_4_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(USART3_4_IRQn);
                #else
                HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
                HAL_NVIC_EnableIRQ(USART3_IRQn);
                #endif

                break;
      #endif

      default : return false;
    }

  HAL_UART_Receive_IT(&huart, (uint8_t*)&receiveddata, 1);

  SetEvent(DIOSTM32FXXXUARTFSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();
  ResetConnexionStatistics();

  return threadconnexion->Ini();
}



/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUART::Config
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      01/11/2014 12:37:49
//
//  @return       bool :
//
//  @param        mask :
*/
/*-----------------------------------------------------------------*/
bool DIOSTM32FXXXSTREAMUART::Config(XWORD mask)
{
  if((mask&DIOSTREAMUARTMASK_BAUDRATE) == DIOSTREAMUARTMASK_BAUDRATE)
    {
      switch(this->config->GetBaudRate())
        {
          case 50     : huart.Init.BaudRate = 50;     break;
          case 300    : huart.Init.BaudRate = 300;    break;
          case 600    : huart.Init.BaudRate = 600;    break;
          case 1200   : huart.Init.BaudRate = 1200;   break;
          case 4800   : huart.Init.BaudRate = 4800;   break;
          case 9600   : huart.Init.BaudRate = 9600;   break;
          case 19200  : huart.Init.BaudRate = 19200;  break;
          case 38400  : huart.Init.BaudRate = 38400;  break;
          case 57600  : huart.Init.BaudRate = 57600;  break;
          case 115200 : huart.Init.BaudRate = 115200; break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_DATABITS ) == DIOSTREAMUARTMASK_DATABITS)
    {
      switch(this->config->GetDataBits())
        {
          case DIOSTREAMUARTDATABIT_5             : break;
          case DIOSTREAMUARTDATABIT_6             : break;
          case DIOSTREAMUARTDATABIT_7             : break;
          case DIOSTREAMUARTDATABIT_8             : huart.Init.WordLength = UART_WORDLENGTH_8B;     break;
          case DIOSTREAMUARTDATABIT_9             : huart.Init.WordLength = UART_WORDLENGTH_9B;     break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_PARITY) == DIOSTREAMUARTMASK_PARITY)
    {
      switch(this->config->GetParity())
        {
          case DIOSTREAMUARTPARITY_NONE           : huart.Init.Parity = UART_PARITY_NONE;           break;
          case DIOSTREAMUARTPARITY_MARK           :                                                 break;
          case DIOSTREAMUARTPARITY_ODD            : huart.Init.Parity = UART_PARITY_ODD;            break;
          case DIOSTREAMUARTPARITY_SPACE          :                                                 break;
          case DIOSTREAMUARTPARITY_EVEN           : huart.Init.Parity = UART_PARITY_EVEN;           break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_STOPBITS ) == DIOSTREAMUARTMASK_STOPBITS)
    {
      switch(this->config->GetStopBits())
        {
          case DIOSTREAMUARTSTOPBITS_ONE          : huart.Init.StopBits = UART_STOPBITS_1;          break;
          #if defined(HW_STM32F0XX) || defined(HW_STM32F3XX)
          case DIOSTREAMUARTSTOPBITS_ONEANDAHALF  : huart.Init.StopBits = UART_STOPBITS_1_5;        break;
          #endif
          case DIOSTREAMUARTSTOPBITS_TWO          : huart.Init.StopBits = UART_STOPBITS_2;          break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_FLOWCONTROL) == DIOSTREAMUARTMASK_FLOWCONTROL)
    {
      switch(config->GetFlowControl())
        {
          case DIOSTREAMUARTFLOWCONTROL_NONE      : huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;     break;
          case DIOSTREAMUARTFLOWCONTROL_HARD      : huart.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS;  break;
          case DIOSTREAMUARTFLOWCONTROL_SOFT      : break;
        }
    }

  huart.Init.Mode                   = UART_MODE_TX_RX;
  huart.Init.OverSampling           = UART_OVERSAMPLING_16;
  #if defined(HW_STM32F0XX) || defined(HW_STM32F3XX)
  huart.Init.OneBitSampling         = UART_ONEBIT_SAMPLING_DISABLED;
  huart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  #endif

  return true;
}



/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUART::ReadDirect
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      24/06/2015 13:25:13
//
//  @return       XDWORD :
//
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTM32FXXXSTREAMUART::ReadDirect(XBYTE* buffer, XDWORD size)
{
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  int br = 0;

  if(HAL_UART_Receive(&diostreamuartptrhandle[indexport]->huart, (uint8_t*)buffer, size, 1000) == HAL_OK)
    {
      br = size;
    }

  return (XDWORD)br;
}




/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUART::WriteDirect
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      24/06/2015 13:25:02
//
//  @return       XDWORD :
//
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTM32FXXXSTREAMUART::WriteDirect(XBYTE* buffer, XDWORD size)
{
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  if(HAL_UART_Transmit(&huart, (uint8_t*)buffer , (uint16_t)size, 1000) == HAL_OK) return size;

  return (XDWORD)0;
}




/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUSB::Write
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/02/2017 11:44:58
//
//  @return       XDWORD :
//
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTM32FXXXSTREAMUART::Write(XBYTE* buffer, XDWORD size)
{
  if(!outbuffer) return 0;

  while(outbuffer->IsBlocked())
    {

    }

  if(!outbuffer->Add(buffer,size))  return 0;

  nbyteswrite+=size;
  if(xtimernotactivity) xtimernotactivity->Reset();

  outbuffer->SetBlocked(false);

  return size;
}




//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUART::Close
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
bool DIOSTM32FXXXSTREAMUART::Close()
{
  if(!threadconnexion) return false;

  threadconnexion->End();

  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  diostreamuartptrhandle[indexport] = NULL;

  return true;
}





//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUART::CleanBuffers
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
bool DIOSTM32FXXXSTREAMUART::CleanBuffers()
{

  return false;
}



/*-------------------------------------------------------------------
//  HAL_UART_RxCpltCallback
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/08/2016 14:27:24
//
*/
/*-----------------------------------------------------------------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  int indexport = -1;

  if(huart->Instance == USART1) indexport = 0;
  if(huart->Instance == USART2) indexport = 1;
  #ifndef HW_STM32F4XX
  if(huart->Instance == USART3) indexport = 2;
  #endif

  if(indexport == -1)                                                                       return;
  if(!diostreamuartptrhandle[indexport])                                                    return;
  if(diostreamuartptrhandle[indexport]->GetConnectStatus() == DIOSTREAMSTATUS_DISCONNECTED) return;

  if(!diostreamuartptrhandle[indexport]->IsBlockRead())
    {
      if(!diostreamuartptrhandle[indexport]->inbuffer->IsBlocked())
        {
          if(diostreamuartptrhandle[indexport]->readcache[indexport].GetSize())
            {
              diostreamuartptrhandle[indexport]->inbuffer->Add(diostreamuartptrhandle[indexport]->readcache);
              diostreamuartptrhandle[indexport]->readcache[indexport].Delete();
            }

          diostreamuartptrhandle[indexport]->inbuffer->Add(diostreamuartptrhandle[indexport]->receiveddata);
        }
       else
        {
          diostreamuartptrhandle[indexport]->readcache[indexport].Add(diostreamuartptrhandle[indexport]->receiveddata);
        }
    }

  HAL_UART_Receive_IT(&diostreamuartptrhandle[indexport]->huart, (uint8_t*)&diostreamuartptrhandle[indexport]->receiveddata, 1);
}



/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMUART::ThreadConnexion
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
void DIOSTM32FXXXSTREAMUART::ThreadConnexion(void* param)
{
  DIOSTM32FXXXSTREAMUART* diostream = (DIOSTM32FXXXSTREAMUART*)param;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOSTM32FXXXUARTFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOSTM32FXXXUARTFSMSTATE_NONE                    : break;

          case DIOSTM32FXXXUARTFSMSTATE_CONNECTED               : break;

          case DIOSTM32FXXXUARTFSMSTATE_WAITINGTOREAD           : { if(!diostream->IsBlockRead())
                                                                      {
                                                                        if(!diostream->inbuffer->IsBlocked())
                                                                          {
                                                                            if(diostream->readcache[diostream->indexport].GetSize())
                                                                              {
                                                                                diostream->inbuffer->Add(diostream->readcache);
                                                                                diostream->readcache[diostream->indexport].Delete();
                                                                              }
                                                                          }
                                                                      }

                                                                    if(!diostream->IsBlockWrite())
                                                                      {
                                                                        if(!diostream->outbuffer->IsBlocked())
                                                                         {
                                                                            int esize = diostream->outbuffer->GetSize();
                                                                            if(esize>DIOSTREAM_MAXBUFFER) esize = DIOSTREAM_MAXBUFFER;

                                                                            if(esize)
                                                                              {
                                                                                diostream->outbuffer->Get(diostream->writebuffer, esize, 0);
                                                                                if(diostream->WriteDirect(diostream->writebuffer, esize))
                                                                                  {
                                                                                    diostream->outbuffer->Extract(NULL, 0, esize);
                                                                                  }
                                                                              }
                                                                          }
                                                                      }
                                                                  }
                                                                  break;

          case DIOSTM32FXXXUARTFSMSTATE_DISCONNECTING           : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOSTM32FXXXUART_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOSTM32FXXXUARTFSMSTATE_NONE                : break;

              case DIOSTM32FXXXUARTFSMSTATE_CONNECTED           : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_CONNECTED);
                                                                    xevent.SetDIOStream(diostream);
                                                                    diostream->PostEvent(&xevent);

                                                                    diostream->SetEvent(DIOSTM32FXXXUARTFSMEVENT_WAITINGTOREAD);
                                                                  }
                                                                  break;

              case DIOSTM32FXXXUARTFSMSTATE_WAITINGTOREAD       : break;

              case DIOSTM32FXXXUARTFSMSTATE_DISCONNECTING       : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_DISCONNECTED);
                                                                    xevent.SetDIOStream(diostream);
                                                                    diostream->PostEvent(&xevent);

                                                                    diostream->status = DIOSTREAMSTATUS_DISCONNECTED;
                                                                  }
                                                                  break;
            }
        }
    }

}


#endif
