//------------------------------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMSPI.CPP
//
//  STM32Fxxx Data IO Stream SPI class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/01/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#if defined(DIO_ACTIVE) && defined(DIOSPI_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------


#include "XFactory.h"
#include "XThreadCollected.h"

#include "DIOStreamXEvent.h"
#include "DIOStreamSPIConfig.h"

#include "DIOSTM32FxxxStreamSPI.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------

DIOSTM32FXXXSTREAMSPIPORT*    DIOSTM32FXXXSTREAMSPI::ports[DIOSTM32FXXXSTREAMSPI_MAXPORTS]     = { NULL, NULL };
DIOSTM32FXXXSTREAMSPI*        DIOSTM32FXXXSTREAMSPI::handles[DIOSTM32FXXXSTREAMSPI_MAXHANDLES] = { NULL, NULL };

//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMSPI::DIOSTM32FXXXSTREAMSPI
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
DIOSTM32FXXXSTREAMSPI::DIOSTM32FXXXSTREAMSPI() : DIOSTREAMSPI()
{
  Clean();

  AddState( DIOSTREAMSPI_FSMSTATE_NONE            ,
            DIOSTREAMSPI_FSMEVENT_CONNECTED       , DIOSTREAMSPI_FSMSTATE_CONNECTED        ,
            DIOSTREAMSPI_FSMEVENT_DISCONNECTING   , DIOSTREAMSPI_FSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);

  AddState( DIOSTREAMSPI_FSMSTATE_CONNECTED       ,
            DIOSTREAMSPI_FSMEVENT_WAITINGTOREAD   , DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD    ,
            DIOSTREAMSPI_FSMEVENT_DISCONNECTING   , DIOSTREAMSPI_FSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);

  AddState( DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD   ,
            DIOSTREAMSPI_FSMEVENT_CONNECTED       , DIOSTREAMSPI_FSMSTATE_CONNECTED        ,
            DIOSTREAMSPI_FSMEVENT_DISCONNECTING   , DIOSTREAMSPI_FSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);

  AddState( DIOSTREAMSPI_FSMSTATE_DISCONNECTING   ,
            DIOSTREAMSPI_FSMEVENT_CONNECTED       , DIOSTREAMSPI_FSMSTATE_CONNECTED        ,
            DIOSTREAMSPI_FSMEVENT_WAITINGTOREAD   , DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD    ,
            EVENTDEFEND);

  threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMSPI, __L("DIOSTM32FXXXSTREAMSPI::DIOSTM32FXXXSTREAMSPI"), ThreadConnexion, (void*)this);
}


//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMSPI::~DIOSTM32FXXXSTREAMSPI
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
DIOSTM32FXXXSTREAMSPI::~DIOSTM32FXXXSTREAMSPI()
{
  Close();

  if(threadconnexion) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMSPI, threadconnexion);

  Clean();
}


//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMSPI::GetConnectStatus
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       DIOSTREAMSTATUS :
*/
//-------------------------------------------------------------------
DIOSTREAMSTATUS DIOSTM32FXXXSTREAMSPI::GetConnectStatus()
{
  if(!config) return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}


//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMSPI::Open
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool DIOSTM32FXXXSTREAMSPI::Open()
{
  if(!threadconnexion)  return false;
  if(!config)           return false;

 for(int c=0; c<DIOSTM32FXXXSTREAMSPI_MAXHANDLES; c++)
    {
      if(DIOSTM32FXXXSTREAMSPI::handles[c] == this) return false;

      if(!DIOSTM32FXXXSTREAMSPI::handles[c])
        {
          indexhandle = c;
          DIOSTM32FXXXSTREAMSPI::handles[indexhandle] = this;
          break;
        }
    }

  if(indexhandle == -1) return false;

  indexport = config->GetPort()-1;

  if(!DIOSTM32FXXXSTREAMSPI::ports[indexport])
    {
      DIOSTM32FXXXSTREAMSPIPORT* port = new DIOSTM32FXXXSTREAMSPIPORT();
      if(!port) return false;

      hspi = new SPI_HandleTypeDef();
      if(!hspi)
        {
          delete port;
          return false;
        }

      switch(config->GetPort())
        {
          case  1 : hspi->Instance = SPI1;       break;
          case  2 : hspi->Instance = SPI2;       break;
          default : return false;
        }

      hspi->Init.Mode               = SPI_MODE_MASTER;
      hspi->Init.Direction          = SPI_DIRECTION_1LINE; //SPI_DIRECTION_2LINES;
      hspi->Init.DataSize           = SPI_DATASIZE_8BIT;
      hspi->Init.CLKPolarity        = SPI_POLARITY_LOW;
      hspi->Init.CLKPhase           = SPI_PHASE_1EDGE;
      hspi->Init.NSS                = SPI_NSS_SOFT;
      hspi->Init.BaudRatePrescaler  = SPI_BAUDRATEPRESCALER_2;
      hspi->Init.FirstBit           = SPI_FIRSTBIT_MSB;
      hspi->Init.TIMode             = SPI_TIMODE_DISABLE;
      hspi->Init.CRCCalculation     = SPI_CRCCALCULATION_DISABLE;
      hspi->Init.CRCPolynomial      = 7;
      #if defined(HW_STM32F0XX) || defined(HW_STM32F3XX)
      hspi->Init.CRCLength          = SPI_CRC_LENGTH_DATASIZE;
      hspi->Init.NSSPMode           = SPI_NSS_PULSE_ENABLE;
      #endif

      if(HAL_SPI_Init(hspi) != HAL_OK)
        {
          return false;
        }

      port->SetHandleSPI(hspi);
      DIOSTM32FXXXSTREAMSPI::ports[indexport] = port;
    }
   else
    {
      hspi = DIOSTM32FXXXSTREAMSPI::ports[indexport]->GetHandleSPI();
      DIOSTM32FXXXSTREAMSPI::ports[indexport]->SetCounterRef(DIOSTM32FXXXSTREAMSPI::ports[indexport]->GetCounterRef()+1);
    }

  SetEvent(DIOSTREAMSPI_FSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  memset(bufferdata, 0, DIOSTREAM_MAXBUFFER);

  ResetXBuffers();
  ResetConnexionStatistics();

  return threadconnexion->Ini();
}


/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMSPI::WaitToFilledReadingBuffer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/09/2016 16:39:05
//
//  @return       bool :
//
//  @param        filledto :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOSTM32FXXXSTREAMSPI::WaitToFilledReadingBuffer(int filledto, int timeout)
{
  if(filledto == DIOSTREAM_SOMETHINGTOREAD) sizebufferdata = 1; else sizebufferdata = filledto;

  switch(config->GetMode())
    {
      case DIOSTREAMMODE_MASTER     :
      case DIOSTREAMMODE_SEMIMASTER :
      case DIOSTREAMMODE_SLAVE      : HAL_SPI_Transmit_IT(hspi, (uint8_t*)bufferdata, sizebufferdata);
                                      break;
    }



  return DIOSTREAM::WaitToFilledReadingBuffer(sizebufferdata, timeout);
}






/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMSPI::ReadDirect
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
XDWORD DIOSTM32FXXXSTREAMSPI::ReadDirect(XBYTE* buffer, XDWORD size)
{
  if(!config)                                          return 0;
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  XDWORD br = 0;

  switch(config->GetMode())
    {
      case DIOSTREAMMODE_MASTER     :
      case DIOSTREAMMODE_SEMIMASTER :
      case DIOSTREAMMODE_SLAVE      : if(HAL_SPI_Receive(hspi, buffer, size, 100) == HAL_OK) br = size;
                                      break;
    }

  return br;
}




/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMSPI::WriteDirect
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
XDWORD DIOSTM32FXXXSTREAMSPI::WriteDirect(XBYTE* buffer, XDWORD size)
{
  if(!config)                                          return 0;
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  XDWORD bw = 0;

  if(!size) return 0;

   switch(config->GetMode())
    {
      case DIOSTREAMMODE_MASTER       :
      case DIOSTREAMMODE_SEMIMASTER   :
      case DIOSTREAMMODE_SLAVE        : if(HAL_SPI_Transmit(hspi, buffer, size, 100) == HAL_OK) bw = size;
                                        break;
    }

  return bw;
}



/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMSPI::Write
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/02/2017 12:29:44
//
//  @return       XDWORD :
//
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTM32FXXXSTREAMSPI::Write(XBYTE* buffer, XDWORD size)
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
//  DIOSTM32FXXXSTREAMSPI::Close
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
bool DIOSTM32FXXXSTREAMSPI::Close()
{
  if(!threadconnexion) return false;

  threadconnexion->End();

  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  if(!DIOSTM32FXXXSTREAMSPI::handles[indexhandle]) return false;

  DIOSTM32FXXXSTREAMSPI::handles[indexhandle] = NULL;

  if(!DIOSTM32FXXXSTREAMSPI::ports[indexport]->GetCounterRef())
    {
      HAL_SPI_DeInit(DIOSTM32FXXXSTREAMSPI::ports[indexport]->GetHandleSPI());

      delete DIOSTM32FXXXSTREAMSPI::ports[indexport]->GetHandleSPI();
      delete DIOSTM32FXXXSTREAMSPI::ports[indexport];

      DIOSTM32FXXXSTREAMSPI::ports[indexport] = NULL;
    }
   else
   {
     DIOSTM32FXXXSTREAMSPI::ports[indexport]->SetCounterRef(DIOSTM32FXXXSTREAMSPI::ports[indexport]->GetCounterRef()-1);
   }

  return true;
}





//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMSPI::CleanBuffers
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
bool DIOSTM32FXXXSTREAMSPI::CleanBuffers()
{

  return false;
}




/*-------------------------------------------------------------------
//  HAL_SPI_MasterRxCpltCallback
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      13/09/2016 17:04:42
//
//  @param        hspi :
*/
/*-----------------------------------------------------------------*/
void HAL_SPI_MasterRxCpltCallback(SPI_HandleTypeDef* hspi)
{
  int port = -1;

  if(hspi->Instance == SPI1) port = 0;
  if(hspi->Instance == SPI2) port = 1;

  for(int c=0; c<DIOSTM32FXXXSTREAMSPI_MAXHANDLES; c++)
    {
      if(DIOSTM32FXXXSTREAMSPI::handles[c])
        {
          if(DIOSTM32FXXXSTREAMSPI::handles[c]->indexport  == port)
            {
              if(DIOSTM32FXXXSTREAMSPI::handles[c]->GetConnectStatus() == DIOSTREAMSTATUS_DISCONNECTED) return;

              if(!DIOSTM32FXXXSTREAMSPI::handles[c]->IsBlockRead())
                {
                   DIOSTM32FXXXSTREAMSPI::handles[c]->inbuffer->Add(DIOSTM32FXXXSTREAMSPI::handles[c]->bufferdata, DIOSTM32FXXXSTREAMSPI::handles[c]->sizebufferdata);
                }

              DIOSTM32FXXXSTREAMSPI::handles[c]->sizebufferdata = 0;
            }
        }
    }
}




/*-------------------------------------------------------------------
//  HAL_SPI_SlaveRxCpltCallback
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      13/09/2016 17:04:42
//
//  @param        hspi :
*/
/*-----------------------------------------------------------------*/
void HAL_SPI_SlaveRxCpltCallback(SPI_HandleTypeDef* hspi)
{
  HAL_SPI_MasterRxCpltCallback(hspi);
}




/*-------------------------------------------------------------------
//  HAL_SPI_ErrorCallback
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2016 11:50:34
//
//  @param        *hspi :
*/
/*-----------------------------------------------------------------*/
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  XDEBUG_PRINTCOLOR(XDEBUG_COLORRED, __L("Error comunication %d"), hspi->ErrorCode);
}



/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMSPI::ThreadConnexion
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
void DIOSTM32FXXXSTREAMSPI::ThreadConnexion(void* param)
{
  DIOSTM32FXXXSTREAMSPI* diostream = (DIOSTM32FXXXSTREAMSPI*)param;
  if(!diostream) return;

  //HAL_NVIC_DisableIRQ(TIM3_IRQn);

  if(diostream->GetEvent()==DIOSTREAMSPI_FSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOSTREAMSPI_FSMSTATE_NONE                   : break;

          case DIOSTREAMSPI_FSMSTATE_CONNECTED              : break;

          case DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD          : break;

          case DIOSTREAMSPI_FSMSTATE_DISCONNECTING          : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOSTREAMSPI_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOSTREAMSPI_FSMSTATE_NONE               : break;

              case DIOSTREAMSPI_FSMSTATE_CONNECTED          : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                diostream->PostEvent(&xevent);

                                                                diostream->SetEvent(DIOSTREAMSPI_FSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD      : break;

              case DIOSTREAMSPI_FSMSTATE_DISCONNECTING      : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_DISCONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                diostream->PostEvent(&xevent);

                                                                diostream->status = DIOSTREAMSTATUS_DISCONNECTED;
                                                              }
                                                              break;
            }
        }
    }

  //HAL_NVIC_EnableIRQ(TIM3_IRQn);

}


#endif
