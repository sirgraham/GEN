//------------------------------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMI2C.CPP
//
//  STM32Fxxx Data IO Stream I2C class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/01/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#if defined(DIO_ACTIVE) && defined(DIOI2C_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XThreadCollected.h"

#include "DIOStreamXEvent.h"
#include "DIOStreamI2CConfig.h"

#include "DIOSTM32FxxxStreamI2C.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------

DIOSTM32FXXXSTREAMI2CPORT*    DIOSTM32FXXXSTREAMI2C::ports[DIOSTM32FXXXSTREAMI2C_MAXPORTS]     = { NULL, NULL };
DIOSTM32FXXXSTREAMI2C*        DIOSTM32FXXXSTREAMI2C::handles[DIOSTM32FXXXSTREAMI2C_MAXHANDLES] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };

//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMI2C::DIOSTM32FXXXSTREAMI2C
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
DIOSTM32FXXXSTREAMI2C::DIOSTM32FXXXSTREAMI2C() : DIOSTREAMI2C()
{
  Clean();

  AddState( DIOSTREAMI2C_FSMSTATE_NONE            ,
            DIOSTREAMI2C_FSMEVENT_CONNECTED       , DIOSTREAMI2C_FSMSTATE_CONNECTED        ,
            DIOSTREAMI2C_FSMEVENT_DISCONNECTING   , DIOSTREAMI2C_FSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);

  AddState( DIOSTREAMI2C_FSMSTATE_CONNECTED       ,
            DIOSTREAMI2C_FSMEVENT_WAITINGTOREAD   , DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD    ,
            DIOSTREAMI2C_FSMEVENT_DISCONNECTING   , DIOSTREAMI2C_FSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);

  AddState( DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD   ,
            DIOSTREAMI2C_FSMEVENT_CONNECTED       , DIOSTREAMI2C_FSMSTATE_CONNECTED        ,
            DIOSTREAMI2C_FSMEVENT_DISCONNECTING   , DIOSTREAMI2C_FSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);

  AddState( DIOSTREAMI2C_FSMSTATE_DISCONNECTING   ,
            DIOSTREAMI2C_FSMEVENT_CONNECTED       , DIOSTREAMI2C_FSMSTATE_CONNECTED        ,
            DIOSTREAMI2C_FSMEVENT_WAITINGTOREAD   , DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD    ,
            EVENTDEFEND);

  threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMI2C, __L("DIOSTM32FXXXSTREAMI2C::DIOSTM32FXXXSTREAMI2C"), ThreadConnexion, (void*)this);
}


//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMI2C::~DIOSTM32FXXXSTREAMI2C
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
DIOSTM32FXXXSTREAMI2C::~DIOSTM32FXXXSTREAMI2C()
{
  Close();

  if(threadconnexion) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMI2C, threadconnexion);

  Clean();
}


//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMI2C::GetConnectStatus
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       DIOSTREAMSTATUS :
*/
//-------------------------------------------------------------------
DIOSTREAMSTATUS DIOSTM32FXXXSTREAMI2C::GetConnectStatus()
{
  if(!config) return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}


//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMI2C::Open
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool DIOSTM32FXXXSTREAMI2C::Open()
{
  if(!threadconnexion)  return false;
  if(!config)           return false;

  for(int c=0; c<DIOSTM32FXXXSTREAMI2C_MAXHANDLES; c++)
    {
      if(DIOSTM32FXXXSTREAMI2C::handles[c] == this) return false;

      if(!DIOSTM32FXXXSTREAMI2C::handles[c])
        {
          indexhandle = c;
          DIOSTM32FXXXSTREAMI2C::handles[indexhandle] = this;
          break;
        }
    }

  if(indexhandle == -1) return false;

  if(config->GetLocalAddress()  != DIOSTREAMI2CCONFIG_ADDRESSINVALID)  localaddress  = (config->GetLocalAddress()  << 1);
  if(config->GetRemoteAddress() != DIOSTREAMI2CCONFIG_ADDRESSINVALID)  remoteaddress = (config->GetRemoteAddress() << 1);

  indexport = config->GetPort()-1;

  if(!DIOSTM32FXXXSTREAMI2C::ports[indexport])
    {
      DIOSTM32FXXXSTREAMI2CPORT* port = new DIOSTM32FXXXSTREAMI2CPORT();
      if(!port) return false;

      hi2c = new I2C_HandleTypeDef();
      if(!hi2c)
        {
          delete port;
          return false;
        }

      switch(config->GetPort())
        {
          case  1 : hi2c->Instance = I2C1;       break;
          case  2 : hi2c->Instance = I2C2;       break;
          default : return false;
        }

      #if defined(HW_STM32F0XX) || defined(HW_STM32F3XX)
      hi2c->Init.Timing           = 0x2000090E;
      #endif

      #if defined(STM32F411xE) || defined(STM32F446x)
      hi2c->Init.ClockSpeed      = 100000;
      #endif

      hi2c->Init.OwnAddress1      = localaddress;

      switch(config->GetAddressSize())
        {
          case DIOSTREAMI2CCONFIG_ADDRESSSIZE_DEFAULT :
          case DIOSTREAMI2CCONFIG_ADDRESSSIZE_7BITS   : hi2c->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;      break;
          case DIOSTREAMI2CCONFIG_ADDRESSSIZE_10BITS  : hi2c->Init.AddressingMode   = I2C_ADDRESSINGMODE_10BIT;   break;
        }

      hi2c->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
      hi2c->Init.OwnAddress2      = 0;
      #if defined(HW_STM32F0XX) || defined(HW_STM32F3XX)
      hi2c->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
      #endif
      hi2c->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
      hi2c->Init.NoStretchMode    = config->IsStretchActive()?I2C_NOSTRETCH_ENABLE:I2C_NOSTRETCH_DISABLE;

      if(HAL_I2C_Init(hi2c) != HAL_OK)
        {
          return false;
        }

      #if defined(HW_STM32F0XX) || defined(HW_STM32F3XX)
      if(HAL_I2CEx_ConfigAnalogFilter(hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
        {
          return false;
        }
      #endif

      if(config->GetMode() == DIOSTREAMMODE_MASTER)
        {
          /*
          if(HAL_I2C_IsDeviceReady(hi2c, (uint16_t)remoteaddress, 3, 1000) != HAL_OK)
            {
              return false;
            }
          */
        }

      port->SetHandleI2C(hi2c);
      DIOSTM32FXXXSTREAMI2C::ports[indexport] = port;
    }
   else
    {
      hi2c = DIOSTM32FXXXSTREAMI2C::ports[indexport]->GetHandleI2C();
      DIOSTM32FXXXSTREAMI2C::ports[indexport]->SetCounterRef(DIOSTM32FXXXSTREAMI2C::ports[indexport]->GetCounterRef()+1);
    }


  SetEvent(DIOSTREAMI2C_FSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  memset(bufferdata, 0, DIOSTREAM_MAXBUFFER);

  ResetXBuffers();
  ResetConnexionStatistics();

  return threadconnexion->Ini();
}



/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMI2C::WaitToFilledReadingBuffer
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
bool DIOSTM32FXXXSTREAMI2C::WaitToFilledReadingBuffer(int filledto, int timeout)
{
  if(filledto == DIOSTREAM_SOMETHINGTOREAD) sizebufferdata = 1; else sizebufferdata = filledto;

  switch(config->GetMode())
    {
      case DIOSTREAMMODE_MASTER     : HAL_I2C_Master_Receive_IT(hi2c, (uint16_t)remoteaddress, (uint8_t*)bufferdata, sizebufferdata);
                                      break;

      case DIOSTREAMMODE_SEMIMASTER :
      case DIOSTREAMMODE_SLAVE      : HAL_I2C_Slave_Receive_IT(hi2c, (uint8_t*)bufferdata, sizebufferdata);
                                      break;
    }

  return DIOSTREAM::WaitToFilledReadingBuffer(sizebufferdata, timeout);
}






/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMI2C::ReadDirect
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
XDWORD DIOSTM32FXXXSTREAMI2C::ReadDirect(XBYTE* buffer, XDWORD size)
{
  if(!config)                                          return 0;
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  XDWORD br = 0;

  switch(config->GetMode())
    {
      case DIOSTREAMMODE_MASTER     : if(HAL_I2C_Master_Receive(hi2c, (uint16_t)remoteaddress, buffer, size, 1000) == HAL_OK) br = size;
                                      break;

      case DIOSTREAMMODE_SEMIMASTER :
      case DIOSTREAMMODE_SLAVE      : if(HAL_I2C_Slave_Receive(hi2c, buffer, size, 1000) == HAL_OK) br = size;
                                      break;
    }

  return br;
}




/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMI2C::WriteDirect
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
XDWORD DIOSTM32FXXXSTREAMI2C::WriteDirect(XBYTE* buffer, XDWORD size)
{
  if(!config)                                          return 0;
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  XDWORD bw = 0;

  if(!size) return 0;

   switch(config->GetMode())
    {
      case DIOSTREAMMODE_MASTER       :
      case DIOSTREAMMODE_SEMIMASTER   : if(HAL_I2C_Master_Transmit(hi2c, (uint16_t)remoteaddress, buffer, size, 1000) == HAL_OK) bw = size;
                                        break;

      case DIOSTREAMMODE_SLAVE        : if(HAL_I2C_Slave_Transmit(hi2c, buffer, size, 1000) == HAL_OK) bw = size;
                                        break;
    }

  return bw;
}




/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMI2C::Write
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/02/2017 12:29:57
//
//  @return       XDWORD :
//
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTM32FXXXSTREAMI2C::Write(XBYTE* buffer, XDWORD size)
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
//  DIOSTM32FXXXSTREAMI2C::Close
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
bool DIOSTM32FXXXSTREAMI2C::Close()
{
  if(!threadconnexion) return false;

  threadconnexion->End();

  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  if(!DIOSTM32FXXXSTREAMI2C::handles[indexhandle]) return false;

  DIOSTM32FXXXSTREAMI2C::handles[indexhandle] = NULL;

  if(!DIOSTM32FXXXSTREAMI2C::ports[indexport]->GetCounterRef())
    {
      HAL_I2C_DeInit(DIOSTM32FXXXSTREAMI2C::ports[indexport]->GetHandleI2C());

      delete DIOSTM32FXXXSTREAMI2C::ports[indexport]->GetHandleI2C();
      delete DIOSTM32FXXXSTREAMI2C::ports[indexport];

      DIOSTM32FXXXSTREAMI2C::ports[indexport] = NULL;
    }
   else
   {
     DIOSTM32FXXXSTREAMI2C::ports[indexport]->SetCounterRef(DIOSTM32FXXXSTREAMI2C::ports[indexport]->GetCounterRef()-1);
   }

  return true;
}





//-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMI2C::CleanBuffers
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
bool DIOSTM32FXXXSTREAMI2C::CleanBuffers()
{

  return false;
}




/*-------------------------------------------------------------------
//  HAL_I2C_MasterRxCpltCallback
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      13/09/2016 17:04:42
//
//  @param        hi2c :
*/
/*-----------------------------------------------------------------*/
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c)
{
  int indexport = -1;

  if(hi2c->Instance == I2C1) indexport = 0;
  if(hi2c->Instance == I2C2) indexport = 1;

  for(int c=0; c<DIOSTM32FXXXSTREAMI2C_MAXHANDLES; c++)
    {
      if(DIOSTM32FXXXSTREAMI2C::handles[c])
        {
          if(DIOSTM32FXXXSTREAMI2C::handles[c]->indexport  == indexport)
            {
              if(DIOSTM32FXXXSTREAMI2C::handles[c]->GetConnectStatus() == DIOSTREAMSTATUS_CONNECTED)
                {
                  if(!DIOSTM32FXXXSTREAMI2C::handles[c]->IsBlockRead())
                    {
                      if(!DIOSTM32FXXXSTREAMI2C::handles[c]->inbuffer->IsBlocked())
                        {
                          if(DIOSTM32FXXXSTREAMI2C::handles[c]->readcache[indexport].GetSize())
                            {
                              DIOSTM32FXXXSTREAMI2C::handles[c]->inbuffer->Add(DIOSTM32FXXXSTREAMI2C::handles[c]->readcache);
                              DIOSTM32FXXXSTREAMI2C::handles[c]->readcache[indexport].Delete();
                            }

                          DIOSTM32FXXXSTREAMI2C::handles[c]->inbuffer->Add(DIOSTM32FXXXSTREAMI2C::handles[c]->bufferdata, DIOSTM32FXXXSTREAMI2C::handles[c]->sizebufferdata);
                        }
                       else
                        {
                          DIOSTM32FXXXSTREAMI2C::handles[c]->readcache[indexport].Add(DIOSTM32FXXXSTREAMI2C::handles[c]->bufferdata, DIOSTM32FXXXSTREAMI2C::handles[c]->sizebufferdata);
                        }
                    }
                }

              DIOSTM32FXXXSTREAMI2C::handles[c]->sizebufferdata = 0;
            }
        }
    }
}




/*-------------------------------------------------------------------
//  HAL_I2C_SlaveRxCpltCallback
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      13/09/2016 17:04:42
//
//  @param        hi2c :
*/
/*-----------------------------------------------------------------*/
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef* hi2c)
{
  HAL_I2C_MasterRxCpltCallback(hi2c);
}



/*-------------------------------------------------------------------
//  HAL_I2C_ErrorCallback
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2016 11:50:34
//
//  @param        *hi2c :
*/
/*-----------------------------------------------------------------*/
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
  XDEBUG_PRINTCOLOR(XDEBUG_COLORRED, __L("Error comunication %d"), hi2c->ErrorCode);
}



/*-------------------------------------------------------------------
//  DIOSTM32FXXXSTREAMI2C::ThreadConnexion
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
void DIOSTM32FXXXSTREAMI2C::ThreadConnexion(void* param)
{
  DIOSTM32FXXXSTREAMI2C* diostream = (DIOSTM32FXXXSTREAMI2C*)param;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOSTREAMI2C_FSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOSTREAMI2C_FSMSTATE_NONE                   : break;

          case DIOSTREAMI2C_FSMSTATE_CONNECTED              : break;

          case DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD          : if(!diostream->IsBlockRead())
                                                                {
                                                                  if(!diostream->inbuffer->IsBlocked())
                                                                    {
                                                                      if(diostream->readcache[diostream->indexport].GetSize())
                                                                        {
                                                                          diostream->inbuffer->Add(diostream->readcache[diostream->indexport]);
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
                                                                          diostream->outbuffer->Extract(diostream->writebuffer, 0, esize);
                                                                          diostream->WriteDirect(diostream->writebuffer, esize);
                                                                        }
                                                                    }
                                                                }
                                                              break;

          case DIOSTREAMI2C_FSMSTATE_DISCONNECTING          : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOSTREAMI2C_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOSTREAMI2C_FSMSTATE_NONE               : break;

              case DIOSTREAMI2C_FSMSTATE_CONNECTED          : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                diostream->PostEvent(&xevent);

                                                                diostream->SetEvent(DIOSTREAMI2C_FSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD      : break;

              case DIOSTREAMI2C_FSMSTATE_DISCONNECTING      : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_DISCONNECTED);
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
