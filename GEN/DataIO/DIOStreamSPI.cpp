//------------------------------------------------------------------------------------------
//  DIOSTREAMSPI.CPP
//
//  Data IO Stream SPI class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XBuffer.h"
#include "XThreadCollected.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOGPIO.h"
#include "DIOStreamXEvent.h"

#include "DIOStreamSPIConfig.h"
#include "DIOStreamSPI.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTREAMSPI::DIOSTREAMSPI
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/02/2013 7:51:23
//
//  @return
*/
/*-----------------------------------------------------------------*/
DIOSTREAMSPI::DIOSTREAMSPI() : DIOSTREAM(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOSTREAMSPI_FSMSTATE_NONE              ,
            DIOSTREAMSPI_FSMEVENT_CONNECTED       , DIOSTREAMSPI_FSMSTATE_CONNECTED        ,
            DIOSTREAMSPI_FSMEVENT_DISCONNECTING   , DIOSTREAMSPI_FSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);


  AddState( DIOSTREAMSPI_FSMSTATE_CONNECTED       ,
            DIOSTREAMSPI_FSMEVENT_WAITINGTOREAD   , DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD    ,
            DIOSTREAMSPI_FSMEVENT_SENDINGDATA     , DIOSTREAMSPI_FSMSTATE_SENDINGDATA      ,
            DIOSTREAMSPI_FSMEVENT_DISCONNECTING   , DIOSTREAMSPI_FSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);

  AddState( DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD   ,
            DIOSTREAMSPI_FSMEVENT_CONNECTED       , DIOSTREAMSPI_FSMSTATE_CONNECTED        ,
            DIOSTREAMSPI_FSMEVENT_SENDINGDATA     , DIOSTREAMSPI_FSMSTATE_SENDINGDATA      ,
            DIOSTREAMSPI_FSMEVENT_DISCONNECTING   , DIOSTREAMSPI_FSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);

  AddState( DIOSTREAMSPI_FSMSTATE_DISCONNECTING   ,
            DIOSTREAMSPI_FSMEVENT_CONNECTED       , DIOSTREAMSPI_FSMSTATE_CONNECTED        ,
            DIOSTREAMSPI_FSMEVENT_WAITINGTOREAD   , DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD    ,
            DIOSTREAMSPI_FSMEVENT_SENDINGDATA     , DIOSTREAMSPI_FSMSTATE_SENDINGDATA      ,
            EVENTDEFEND);


  threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMSPI, __L("DIOSTREAMSPI::DIOSTREAMSPI"),ThreadConnexion,(void*)this);
  threadconnexion->SetPriority(XTHREADPRIORITY_HIGH);

  xtimer = xfactory->CreateTimer();

}



//-------------------------------------------------------------------
//  DIOSTREAMSPI::~DIOSTREAMSPI
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:19:33
//
//  @return
//  */
//-------------------------------------------------------------------
DIOSTREAMSPI::~DIOSTREAMSPI()
{
  Close();

  if(threadconnexion) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMSPI, threadconnexion);
  if(xtimer)          xfactory->DeleteTimer(xtimer);

  Clean();
}




//-------------------------------------------------------------------
//  DIOSTREAMSPI::GetConfig
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2004 14:33:56
//
//  @return       DIOSTREAMCONFIG* :
//  */
//-------------------------------------------------------------------
DIOSTREAMCONFIG* DIOSTREAMSPI::GetConfig()
{
  return (DIOSTREAMCONFIG*)config;
}




//-------------------------------------------------------------------
//  DIOSTREAMSPI::SetConfig
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
//  @param        config :
*/
//-------------------------------------------------------------------
bool DIOSTREAMSPI::SetConfig(DIOSTREAMCONFIG* config)
{
  if(!config) return false;
  this->config = (DIOSTREAMSPICONFIG*)config;

  return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMSPI::Open
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/10/2012 10:03:42
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAMSPI::Open()
{
  if(!config) return false;
  if(!xtimer) return false;

  if(!threadconnexion)  return false;

  if(!inbuffer)         return false;
  if(!outbuffer)        return false;

  if(config->IsDirectAccess())
    {
      config->GetPins(pin_CLK,pin_MISO,pin_MOSI,pin_CS);

      if(pin_CS  !=DIOGPIO_PINNOTUSE) diogpio->SetMode(pin_CS   , false);
      if(pin_MISO!=DIOGPIO_PINNOTUSE) diogpio->SetMode(pin_MISO , true);
      if(pin_MOSI!=DIOGPIO_PINNOTUSE) diogpio->SetMode(pin_MOSI , false);
      if(pin_CLK !=DIOGPIO_PINNOTUSE) diogpio->SetMode(pin_CLK  , false);
    }

  bufferread   = new XBYTE[DIOSTREAMSPI_MAXSIZEBUFFER];

  mode              = config->GetMode();
  nbitsword         = config->GetNBitsWord();
  timedelay         = config->GetDelay();
  timedelaydouble  *= timedelay;
  CSline            = ((mode&DIOSTREAMSPI_CS_HIGH) == DIOSTREAMSPI_CS_HIGH);
  lbsfirst          = ((mode&DIOSTREAMSPI_LSB_FIRST) == DIOSTREAMSPI_LSB_FIRST);

  if(config->IsDirectAccess() && diogpio)
    {
      switch(mode)
        {
          case DIOSTREAMSPI_MODE_0:
          case DIOSTREAMSPI_MODE_1: diogpio->Set(pin_CLK  , false);
                                    break;

          case DIOSTREAMSPI_MODE_2:
          case DIOSTREAMSPI_MODE_3: diogpio->Set(pin_CLK  , true);
                                    break;
        }

      diogpio->Set(pin_CS, CSline);
    }

  SetEvent(DIOSTREAMSPI_FSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();

  ResetConnexionStatistics();

  return threadconnexion->Ini();
}




/*-------------------------------------------------------------------
//  DIOSTREAMSPI::Close
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/10/2012 10:03:47
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAMSPI::Close()
{
  if(config->IsDirectAccess()) diogpio->Set(pin_CS, !CSline);

  if(!threadconnexion) return false;

  threadconnexion->End();

  if(bufferread)
    {
      delete [] bufferread;
      bufferread = NULL;
    }

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAMSPI::TransferBuffer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/12/2012 20:48:57
//
//  @return       bool :
//  @param        bufferread :
//  @param        bufferwrite :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMSPI::TransferBuffer(XBYTE* bufferread, XBYTE* bufferwrite, XDWORD size)
{
  if(!config)     return false;
  if(!xtimer)     return false;

  if(pin_CLK<0)   return false;
  if(pin_CS<0)    return false;
  if(pin_MOSI<0)  return false;

  if(!diogpio) return false;

  if(config->GetNBitsWord()<=8)
    {
      for(XDWORD c=0;c<size;c++)
        {
          XWORD readdata;

          TransferOneData(bufferwrite[c], readdata);

          bufferread[c] = (XBYTE)readdata;
        }
    }
   else
    {
      for(XDWORD c=0;c<size;c+=2)
        {
          XWORD readdata;
          XWORD writedata = bufferwrite[c];

          writedata<<=8;
          writedata |= bufferwrite[c];

          TransferOneData(bufferwrite[c], readdata);

          bufferread[c]   = (XBYTE)(readdata>>8);
          bufferread[c+1] = (XBYTE)(readdata&0xFF);
        }

    }

  return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMSPI::Sleep
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/10/2012 12:50:33
//
//  @return       void :
//  @param        count :
*/
/*-----------------------------------------------------------------*/
void DIOSTREAMSPI::Sleep(int count)
{
  /*
  int i = 0;
  for(i=0;i<count;i++)
    {
      #ifdef LINUX
      asm("nop");
      #endif

      #ifdef XVISUALC
      __nop();
//      __asm nop;
      #endif

    }
  */
}




/*-------------------------------------------------------------------
//  DIOSTREAMSPI::GetDIOGPIO
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/10/2012 18:20:28
//
//  @return       DIOGPIO* :
//  */
/*-----------------------------------------------------------------*/
DIOGPIO* DIOSTREAMSPI::GetDIOGPIO()
{
  return diogpio;
}



/*-------------------------------------------------------------------
//  DIOSTREAMSPI::SetDIOGPIO
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/10/2012 18:32:36
//
//  @return       bool :
//  @param        diogpio :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMSPI::SetDIOGPIO(DIOGPIO* diogpio)
{
  this->diogpio = diogpio;

  return true;
}






//-------------------------------------------------------------------
//  DIOSTREAMSPI::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:19:50
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOSTREAMSPI::Clean()
{
  config          = NULL;
  xtimer          = NULL;

  diogpio     = NULL;

  bufferread      = NULL;

  threadconnexion = NULL;

  pin_CLK         = -1;
  pin_MISO        = -1;
  pin_MOSI        = -1;
  pin_CS          = -1;

  mode            = DIOSTREAMSPI_MODE_0;
  nbitsword       = 0;
  timedelay       = 0;
  timedelaydouble = 0;
  CSline          = false;
  lbsfirst        = false;
}




/*-------------------------------------------------------------------
//  DIOSTREAMSPI::TransferOneData
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/12/2012 20:19:16
//
//  @return       bool :
//  @param        writedata :
//  @param        readdata :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMSPI::TransferOneData(XWORD writedata,XWORD& readdata)
{
  if(!config->IsDirectAccess()) return false;

  XWORD mask = 0x0001;

  if(!lbsfirst) mask<<=(nbitsword-1);

  readdata = 0;

  for(int c=0;c<nbitsword;c++)
    {
      switch(mode)
        {
          case DIOSTREAMSPI_MODE_0: diogpio->Set(pin_MOSI , (writedata&mask)?true:false);
                                    if(diogpio->Get(pin_MISO)) readdata |=  mask;
                                    DIOSTREAMSPI_SLEEP;
                                    diogpio->Set(pin_CLK, true);
                                    DIOSTREAMSPI_SLEEPDOUBLE;
                                    diogpio->Set(pin_CLK, false);
                                    break;

          case DIOSTREAMSPI_MODE_1:
          case DIOSTREAMSPI_MODE_2:
                                    diogpio->Set(pin_CLK, true);
                                    diogpio->Set(pin_MOSI , (writedata&mask)?true:false);
                                    if(diogpio->Get(pin_MISO)) readdata |=  mask;
                                    DIOSTREAMSPI_SLEEP;
                                    diogpio->Set(pin_CLK, false);
                                    DIOSTREAMSPI_SLEEPDOUBLE;
                                    break;

          case DIOSTREAMSPI_MODE_3:
                                    diogpio->Set(pin_CLK, false);
                                    diogpio->Set(pin_MOSI , (writedata&mask)?true:false);
                                    if(diogpio->Get(pin_MISO)) readdata |=  mask;
                                    DIOSTREAMSPI_SLEEP;
                                    diogpio->Set(pin_CLK, true);
                                    DIOSTREAMSPI_SLEEPDOUBLE;
                                    break;
        }

      if(lbsfirst) mask<<=1; else mask>>=1;
    }

  return true;
}






/*-------------------------------------------------------------------
//  DIOSTREAMSPI::ThreadConnexion
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/09/2012 16:59:27
//
//  @return       void :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
void DIOSTREAMSPI::ThreadConnexion(void* data)
{
  DIOSTREAMSPI* diostream = (DIOSTREAMSPI*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOSTREAMSPI_FSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOSTREAMSPI_FSMSTATE_NONE                   : break;

          case DIOSTREAMSPI_FSMSTATE_CONNECTED                : break;

          case DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD            : { if(diostream->outbuffer->IsBlocked()) break;

                                                                  while(1)
                                                                    {
                                                                      XDWORD size  = diostream->outbuffer->GetSize();

                                                                      if(!size) break;
                                                                      if(size>(XDWORD)DIOSTREAMSPI_MAXSIZEBUFFER) size = DIOSTREAMSPI_MAXSIZEBUFFER;

                                                                      memset(diostream->bufferread, 0 , DIOSTREAMSPI_MAXSIZEBUFFER);

                                                                      if(diostream->TransferBuffer(diostream->bufferread, diostream->outbuffer->Get(), size))
                                                                        {
                                                                          if(!diostream->config->IsOnlyWrite())
                                                                            {
                                                                              diostream->inbuffer->Add(diostream->bufferread,size);
                                                                            }

                                                                          diostream->outbuffer->Extract(NULL, 0, size);
                                                                        }
                                                                    }
                                                                }
                                                                break;


          case DIOSTREAMSPI_FSMSTATE_SENDINGDATA              : break;

          case DIOSTREAMSPI_FSMSTATE_DISCONNECTING            : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOSTREAMSPI_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOSTREAMSPI_FSMSTATE_NONE                 : break;

              case DIOSTREAMSPI_FSMSTATE_CONNECTED            : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_CONNECTED);
                                                                  xevent.SetDIOStream(diostream);
                                                                  diostream->PostEvent(&xevent);

                                                                  diostream->SetEvent(DIOSTREAMSPI_FSMEVENT_WAITINGTOREAD);
                                                                }
                                                                break;

              case DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD        : break;

              case DIOSTREAMSPI_FSMSTATE_SENDINGDATA          : break;

              case DIOSTREAMSPI_FSMSTATE_DISCONNECTING        : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_DISCONNECTED);
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

