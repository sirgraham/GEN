//------------------------------------------------------------------------------------------
//  DIOLINUXSTREAMI2C.CPP
//
//  LINUX Data IO Stream I2C class
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
#include "XDebugTrace.h"

#include "DIOFactory.h"
#include "DIOStreamXEvent.h"
#include "DIOStreamI2CConfig.h"

#include "DIOLINUXStreamI2C.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOLINUXSTREAMI2C::DIOLINUXSTREAMI2C
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
DIOLINUXSTREAMI2C::DIOLINUXSTREAMI2C() : DIOSTREAMI2C()
{
  Clean();

  threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMI2C, __L("DIOLINUXSTREAMI2C::DIOLINUXSTREAMI2C"), ThreadConnexion, (void*)this);
}




//-------------------------------------------------------------------
//  DIOLINUXSTREAMI2C::~DIOLINUXSTREAMI2C
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:19:33
//
//  @return
//  */
//-------------------------------------------------------------------
DIOLINUXSTREAMI2C::~DIOLINUXSTREAMI2C()
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
//  DIOLINUXSTREAMI2C::Open
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
bool DIOLINUXSTREAMI2C::Open()
{
  if(!config) return false;

  if(!threadconnexion)  return false;

  XSTRING_CREATEOEM((*config->GetLocalDeviceName()), charOEM)
  handle = open(charOEM, O_RDWR);
  XSTRING_DELETEOEM(charOEM)
  if(handle<0) return false;

  switch(config->GetAddressSize())
    {
      case DIOSTREAMI2CCONFIG_ADDRESSSIZE_DEFAULT :
      case DIOSTREAMI2CCONFIG_ADDRESSSIZE_7BITS   : break;
      case DIOSTREAMI2CCONFIG_ADDRESSSIZE_10BITS  : if(ioctl(handle, DIOLINUXSTREAMI2C_TENBIT, 1) < 0)  return false;
                                                    break;
    }

  if(ioctl(handle, DIOLINUXSTREAMI2C_SLAVE , config->GetRemoteAddress()) < 0)                 return false;

  SetEvent(DIOSTREAMI2C_FSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();
  ResetConnexionStatistics();

  return threadconnexion->Ini();
}




/*-------------------------------------------------------------------
//  DIOLINUXSTREAMI2C::WaitToFilledReadingBuffer
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
bool DIOLINUXSTREAMI2C::WaitToFilledReadingBuffer(int filledto, int timeout)
{
  if(filledto == DIOSTREAM_SOMETHINGTOREAD) sizeread = 1; else sizeread = filledto;

  bool status = DIOSTREAM::WaitToFilledReadingBuffer(sizeread, timeout);

  return status;
}




/*-------------------------------------------------------------------
//  DIOLINUXSTREAMI2C::ReadDirect
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/09/2016 13:23:30
//
//  @return       XDWORD :
//
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOLINUXSTREAMI2C::ReadDirect(XBYTE* buffer, XDWORD size)
{
  if(!config) return false;

  DIOLINUXSTREAMI2C_RDWR_IOCTL_DATA   msg_rdwr;
  DIOLINUXSTREAMI2C_MSG               msg;

  msg.addr        = config->GetRemoteAddress();
  msg.flags       = DIOLINUXSTREAMI2C_MSG_RD;
  msg.buffer      = buffer;
  msg.size        = size;

  msg_rdwr.msgs   = &msg;
  msg_rdwr.nmsgs  = 1;

  XDWORD br = 0;
  bool status = (ioctl(handle, DIOLINUXSTREAMI2C_RDWR, &msg_rdwr) < 0) ? false:true;
  if(status) br = size;

  //XDEBUGTRACE_PRINTCOLOR(1, __L("Write [Addr %04X, size:%d]  status: %s"), address, size, status?__L("true"):__L("false"));

  return br;
}






/*-------------------------------------------------------------------
//  DIOLINUXSTREAMI2C::WriteDirect
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/09/2016 13:23:44
//
//  @return       XDWORD :
//
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOLINUXSTREAMI2C::WriteDirect(XBYTE* buffer, XDWORD size)
{
  if(!config) return false;

  DIOLINUXSTREAMI2C_RDWR_IOCTL_DATA   msg_rdwr;
  DIOLINUXSTREAMI2C_MSG               msg;

  msg.addr        = config->GetRemoteAddress();
  msg.flags       = 0;
  msg.buffer      = buffer;
  msg.size        = size;

  msg_rdwr.msgs   = &msg;
  msg_rdwr.nmsgs  = 1;

  XDWORD bw = 0;

  bool status = (ioctl(handle, DIOLINUXSTREAMI2C_RDWR, &msg_rdwr) < 0) ? false:true;

  if(status) bw = size;

  //XDEBUGTRACE_PRINTCOLOR(1, __L("Write [Addr %04X, size:%d]  status: %s"), address, size, status?__L("true"):__L("false"));

  return bw;

}



/*-------------------------------------------------------------------
//  DIOLINUXSTREAMI2C::Close
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
bool DIOLINUXSTREAMI2C::Close()
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
//  DIOLINUXSTREAMI2C::Clean
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
void DIOLINUXSTREAMI2C::Clean()
{
  handle   = -1;
  sizeread = 0;
}



/*-------------------------------------------------------------------
//  DIOLINUXSTREAMI2C::ThreadConnexion
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
void DIOLINUXSTREAMI2C::ThreadConnexion(void* data)
{
  DIOLINUXSTREAMI2C* diostream = (DIOLINUXSTREAMI2C*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOSTREAMI2C_FSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOSTREAMI2C_FSMSTATE_NONE                     : break;

          case DIOSTREAMI2C_FSMSTATE_CONNECTED                : break;

          case DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD            : {
                                                                  if(!diostream->IsBlockWrite())
                                                                    {
                                                                      XDWORD size  = diostream->outbuffer->GetSize();
                                                                      if(size)
                                                                        {
                                                                          diostream->outbuffer->SetBlocked(true);
                                                                          XDWORD bw = diostream->WriteDirect(diostream->outbuffer->Get(), size);
                                                                          diostream->outbuffer->SetBlocked(false);

                                                                          if(bw)
                                                                            {
                                                                              //XDEBUGTRACE_PRINTDATABLOCKCOLOR(XDEBUG_COLORRED, diostream->outbuffer->Get(), size);
                                                                              diostream->outbuffer->Extract(NULL, 0 , bw);
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
                                                                              if(diostream->ReadDirect(_data, diostream->sizeread))
                                                                                {
                                                                                  //XDEBUGTRACE_PRINTDATABLOCKCOLOR(XDEBUG_COLORGREEN, _data, diostream->sizeread);
                                                                                  diostream->inbuffer->Add(_data, diostream->sizeread);
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






#endif
