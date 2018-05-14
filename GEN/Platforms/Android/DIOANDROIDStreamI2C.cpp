//------------------------------------------------------------------------------------------
//  DIOANDROIDSTREAMI2C.CPP
//
//  ANDROID Data IO Stream I2C class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#if defined(DIO_ACTIVE) && defined(DIOI2C_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <linux/types.h>


#include "XFactory.h"
#include "XBuffer.h"
#include "XThreadCollected.h"
#include "XSystem.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOStreamXEvent.h"
#include "DIOStreamI2CConfig.h"

#include "DIOANDROIDStreamI2C.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMI2C::DIOANDROIDSTREAMI2C
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
DIOANDROIDSTREAMI2C::DIOANDROIDSTREAMI2C() : DIOSTREAMI2C()
{
  Clean();

  threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMI2C, __L("DIOANDROIDSTREAMI2C::DIOANDROIDSTREAMI2C"), ThreadConnexion, (void*)this);
}




//-------------------------------------------------------------------
//  DIOANDROIDSTREAMI2C::~DIOANDROIDSTREAMI2C
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:19:33
//
//  @return
//  */
//-------------------------------------------------------------------
DIOANDROIDSTREAMI2C::~DIOANDROIDSTREAMI2C()
{
  if(threadconnexion)
    {
      threadconnexion->End();
      DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMI2C, threadconnexion);
      threadconnexion = NULL;
    }

  Clean();
}





/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMI2C::Open
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
bool DIOANDROIDSTREAMI2C::Open()
{
  if(!config) return false;

  if(!threadconnexion)  return false;

  XSTRING_CREATEOEM((*config->GetLocalDeviceName()), charOEM)
  handle = open(charOEM, O_RDWR);
  XSTRING_DELETEOEM(charOEM)

  if(handle<0) return false;

  if(ioctl(handle, DIOANDROIDSTREAMI2C_SLAVE, config->GetRemoteAddress()) < 0) return false;

  SetEvent(DIOSTREAMI2C_FSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();
  ResetConnexionStatistics();

  return threadconnexion->Ini();
}




/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMI2C::WaitToFilledReadingBuffer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/06/2014 23:37:38
//
//  @return       bool :
//
//  @param        DIOSTREAM_SOMETHINGTOREAD :
//  @param        XTIMER_INFINITE :
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMI2C::WaitToFilledReadingBuffer(int filledto, int timeout)
{
  if(filledto == DIOSTREAM_SOMETHINGTOREAD) sizeread = 1; else sizeread = filledto;

  bool status = DIOSTREAM::WaitToFilledReadingBuffer(filledto, timeout);

  return status;
}




/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMI2C::Close
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
bool DIOANDROIDSTREAMI2C::Close()
{
  if(!threadconnexion) return false;

  threadconnexion->End();

  if(handle>=0)
    {
      close(handle);
      handle  = -1;
    }


  return true;
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMI2C::Clean
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      26/04/2014 21:05:24
//
*/
/*-----------------------------------------------------------------*/
void DIOANDROIDSTREAMI2C::Clean()
{
  handle   = -1;
  sizeread = 0;
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMI2C::ThreadConnexion
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
void DIOANDROIDSTREAMI2C::ThreadConnexion(void* data)
{
  DIOANDROIDSTREAMI2C* diostream = (DIOANDROIDSTREAMI2C*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOSTREAMI2C_FSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOSTREAMI2C_FSMSTATE_NONE                     : break;

          case DIOSTREAMI2C_FSMSTATE_CONNECTED                : break;

          case DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD            : { if(diostream->outbuffer->IsBlocked()) break;

                                                                  if(!diostream->IsBlockWrite())
                                                                    {
                                                                      XDWORD size  = diostream->outbuffer->GetSize();
                                                                      if(size)
                                                                        {
                                                                          diostream->outbuffer->SetBlocked(true);
                                                                          XDWORD bw = diostream->I2C_Write(diostream->config->GetRemoteAddress(), diostream->outbuffer->Get(), size);
                                                                          diostream->outbuffer->SetBlocked(false);

                                                                          if(bw)
                                                                            {
                                                                              diostream->outbuffer->Extract(NULL, 0 , size);
                                                                            }
                                                                        }
                                                                    }


                                                                  if(!diostream->IsBlockRead())
                                                                    {
                                                                      if(diostream->sizeread)
                                                                        {
                                                                          XBYTE* _data = new XBYTE[diostream->sizeread];

                                                                          if(_data)
                                                                            {
                                                                              if(diostream->I2C_Read(diostream->config->GetRemoteAddress(), _data, diostream->sizeread))
                                                                                {
                                                                                  diostream->inbuffer->Add(_data, diostream->sizeread);
                                                                                  //XDEBUG_PRINTCOLOR(3, __L("(Addr %04X, size:%d)"), diostream->config->GetRemoteAddress(), diostream->inbuffer->GetSize());
                                                                                  //XDEBUG_PRINTDATABLOCK(1, (*diostream->inbuffer));

                                                                                  diostream->sizeread = 0;
                                                                                }

                                                                              delete [] _data;
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                                break;


          case DIOSTREAMI2C_FSMSTATE_SENDINGDATA              : break;

          case DIOSTREAMI2C_FSMSTATE_DISCONNECTING            : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOSTREAMI2C_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOSTREAMI2C_FSMSTATE_NONE                 : break;

              case DIOSTREAMI2C_FSMSTATE_CONNECTED            : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_CONNECTED);
                                                                  xevent.SetDIOStream(diostream);

                                                                  diostream->PostEvent(&xevent);

                                                                  diostream->SetEvent(DIOSTREAMI2C_FSMEVENT_WAITINGTOREAD);
                                                                }
                                                                break;

              case DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD        : break;

              case DIOSTREAMI2C_FSMSTATE_SENDINGDATA          : break;

              case DIOSTREAMI2C_FSMSTATE_DISCONNECTING        : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_DISCONNECTED);
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





/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMI2C::I2C_Read
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/05/2014 13:58:34
//
//  @return       bool :
//
//  @param        address :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMI2C::I2C_Read(XWORD address, XBYTE* buffer, XWORD size)
{
  /*
  int _size = read(handle, buffer, size);

  XDEBUG_PRINTCOLOR(3, __L("(Read size: %d)"), _size);

  return false;
  */

  DIOANDROIDSTREAMI2C_RDWR_IOCTL_DATA   msg_rdwr;
  DIOANDROIDSTREAMI2C_MSG               msg;

  msg.addr        = address;
  msg.flags       = DIOANDROIDSTREAMI2C_MSG_RD;
  msg.buffer      = buffer;
  msg.size        = size;

  msg_rdwr.msgs   = &msg;
  msg_rdwr.nmsgs  = 1;

  //XDEBUG_PRINTCOLOR(3, __L("[Addr %04X, size:%d]"), address, size);

  if(ioctl(handle, DIOANDROIDSTREAMI2C_RDWR, &msg_rdwr) < 0) return false;


  return true;
}





/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMI2C::I2C_Write
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/05/2014 14:04:06
//
//  @return       bool :
//
//  @param        address :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMI2C::I2C_Write(XWORD address, XBYTE* buffer, XWORD size)
{
  /*
  if(write(handle, buffer, size) != size) return false;

  return true;
  */


  DIOANDROIDSTREAMI2C_RDWR_IOCTL_DATA   msg_rdwr;
  DIOANDROIDSTREAMI2C_MSG               msg;

  msg.addr        = address;
  msg.flags       = 0;
  msg.buffer      = buffer;
  msg.size        = size;

  msg_rdwr.msgs   = &msg;
  msg_rdwr.nmsgs  = 1;

  if(ioctl(handle, DIOANDROIDSTREAMI2C_RDWR, &msg_rdwr) < 0)
  {
      XDEBUG_PRINTCOLOR(4,__L("IOCTL ERROR : %d"),errno);
      return false;
  }


  return true;

}



#endif
