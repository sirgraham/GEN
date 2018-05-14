//------------------------------------------------------------------------------------------
//  DIOANDROIDSTREAMUSB.CPP
//
//  ANDROID Data IO Stream USB class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/01/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#if defined(DIO_ACTIVE) && defined(DIOUSB_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <errno.h>
#include <termios.h>

#include "XFactory.h"
#include "XThread.h"
#include "XBase.h"
#include "XString.h"
#include "XDebug.h"

#include "DIOStreamXEvent.h"
#include "DIOStreamUSBConfig.h"

#include "DIOANDROIDStreamUSB.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMUSB::DIOANDROIDSTREAMUSB
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
DIOANDROIDSTREAMUSB::DIOANDROIDSTREAMUSB() : DIOSTREAMUSB(), XFSMACHINE(0)
{
  Clean();

  AddState( DIOANDROIDUSBFSMSTATE_NONE              ,
            DIOANDROIDUSBFSMEVENT_CONNECTED       , DIOANDROIDUSBFSMSTATE_CONNECTED        ,
            DIOANDROIDUSBFSMEVENT_DISCONNECTING   , DIOANDROIDUSBFSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);

  AddState( DIOANDROIDUSBFSMSTATE_CONNECTED       ,
            DIOANDROIDUSBFSMEVENT_WAITINGTOREAD   , DIOANDROIDUSBFSMSTATE_WAITINGTOREAD    ,
            DIOANDROIDUSBFSMEVENT_SENDINGDATA     , DIOANDROIDUSBFSMSTATE_SENDINGDATA      ,
            DIOANDROIDUSBFSMEVENT_DISCONNECTING   , DIOANDROIDUSBFSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);

  AddState( DIOANDROIDUSBFSMSTATE_WAITINGTOREAD   ,
            DIOANDROIDUSBFSMEVENT_CONNECTED       , DIOANDROIDUSBFSMSTATE_CONNECTED        ,
            DIOANDROIDUSBFSMEVENT_SENDINGDATA     , DIOANDROIDUSBFSMSTATE_SENDINGDATA      ,
            DIOANDROIDUSBFSMEVENT_DISCONNECTING   , DIOANDROIDUSBFSMSTATE_DISCONNECTING    ,
            EVENTDEFEND);

  AddState( DIOANDROIDUSBFSMSTATE_DISCONNECTING   ,
            DIOANDROIDUSBFSMEVENT_CONNECTED       , DIOANDROIDUSBFSMSTATE_CONNECTED        ,
            DIOANDROIDUSBFSMEVENT_WAITINGTOREAD   , DIOANDROIDUSBFSMSTATE_WAITINGTOREAD    ,
            DIOANDROIDUSBFSMEVENT_SENDINGDATA     , DIOANDROIDUSBFSMSTATE_SENDINGDATA      ,
            EVENTDEFEND);

  threadconnexion = CREATEXTHREAD(__L("DIOANDROIDSTREAMUSB::DIOANDROIDSTREAMUSB"), ThreadConnexion, (void*)this);
}


//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUSB::~DIOANDROIDSTREAMUSB
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
DIOANDROIDSTREAMUSB::~DIOANDROIDSTREAMUSB()
{
  Close();

  if(threadconnexion) DELETEXTHREAD(threadconnexion);

  Clean();
}




//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUSB::GetConnectStatus
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       DIOSTREAMSTATUS :
*/
//-------------------------------------------------------------------
DIOSTREAMSTATUS DIOANDROIDSTREAMUSB::GetConnectStatus()
{
  if(fd<0)    return DIOSTREAMSTATUS_DISCONNECTED;
  if(!config) return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}


//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUSB::Open
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMUSB::Open()
{
  if(!config) return false;

  XSTRING_CREATEOEM((*config->GetDeviceID()), charOEM)
  fd= open(charOEM, O_RDWR | O_NOCTTY | O_NONBLOCK);
  XSTRING_DELETEOEM(charOEM)

  if(fd<0) return false;

  //fcntl(fd, F_SETFL, FNDELAY);
  //fcntl(fd, F_SETFL, 0);

  SetEvent(DIOANDROIDUSBFSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();

  ResetConnexionStatistics();

  return threadconnexion->Ini();
}




//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUSB::Close
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMUSB::Close()
{
  if(!threadconnexion) return false;

  threadconnexion->End();

  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  if(fd>=0)
    {
      close(fd);
      fd = -1;
    }

  return true;
}





//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUSB::CleanBuffers
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMUSB::CleanBuffers()
{
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  return true;
}



//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUSB::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
void DIOANDROIDSTREAMUSB::Clean()
{
  fd            = -1;
  readtimeout   = 3000;
  writetimeout  = 3000;
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMUSB::ThreadConnexion
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
void DIOANDROIDSTREAMUSB::ThreadConnexion(void* data)
{
  DIOANDROIDSTREAMUSB* diostream = (DIOANDROIDSTREAMUSB*)data;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOANDROIDUSBFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOANDROIDUSBFSMSTATE_NONE                   : break;

          case DIOANDROIDUSBFSMSTATE_CONNECTED                : break;

          case DIOANDROIDUSBFSMSTATE_WAITINGTOREAD            : {

                                                                  if(!diostream->IsBlockRead())
                                                                    {
                                                                      XBYTE buffer;

                                                                      XDWORD br = diostream->ReadBuffer(buffer, DIOSTREAMUSB_MAXBUFFER);
                                                                      if(br)
                                                                        {
                                                                          //XDEBUG_PRINTDATABLOCK(XDEBUGLEVELCOLOR(2), (XBYTE*)diostream->buffer, (int)br);
                                                                          diostream->inbuffer->Add(buffer,br);
                                                                        }
                                                                    }

                                                                  if(!diostream->IsBlockWrite())
                                                                    {
                                                                      int esize = diostream->outbuffer->GetSize();
                                                                      if(esize)
                                                                        {
                                                                          if(esize>DIOSTREAMUSB_MAXBUFFER) esize = DIOSTREAMUSB_MAXBUFFER;

                                                                          diostream->outbuffer->SetBlocked(true);
                                                                          XDWORD bw = diostream->WriteBuffer(diostream->outbuffer->Get(), esize);
                                                                          diostream->outbuffer->SetBlocked(false);

                                                                          if(bw)
                                                                            {
                                                                              //XDEBUG_PRINTDATABLOCK(XDEBUGLEVELCOLOR(4), (XBYTE*)diostream->buffer, (int)bw);
                                                                              diostream->outbuffer->Extract(NULL, 0, bw);
                                                                            }
                                                                           else
                                                                            {
                                                                              //XDEBUG_PRINTDATABLOCK(XDEBUGLEVELCOLOR(3), (XBYTE*)diostream->buffer, (int)bw);
                                                                            }
                                                                        }
                                                                    }
                                                              }
                                                              break;


          case DIOANDROIDUSBFSMSTATE_SENDINGDATA              : break;

          case DIOANDROIDUSBFSMSTATE_DISCONNECTING            : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOANDROIDUSB_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOANDROIDUSBFSMSTATE_NONE               : break;

              case DIOANDROIDUSBFSMSTATE_CONNECTED            : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                if(diostream->PostEvent(&xevent, true)) xevent.WaitForCompleted(5);

                                                                diostream->SetEvent(DIOANDROIDUSBFSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOANDROIDUSBFSMSTATE_WAITINGTOREAD        : break;

              case DIOANDROIDUSBFSMSTATE_SENDINGDATA          : break;

              case DIOANDROIDUSBFSMSTATE_DISCONNECTING        : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_DISCONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                if(diostream->PostEvent(&xevent, true)) xevent.WaitForCompleted(5);

                                                                diostream->threadconnexion->Run(false);
                                                                diostream->status       = DIOSTREAMSTATUS_DISCONNECTED;
                                                              }
                                                              break;
            }
        }
    }
}





//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUSB::ReadBuffer
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       XDWORD :
//  @param        buffer :
//  @param        size :
*/
//-------------------------------------------------------------------
XDWORD DIOANDROIDSTREAMUSB::ReadBuffer(XBYTE* buffer,XDWORD size)
{
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  fd_set         fds;
  struct timeval tv;

  tv.tv_sec  = 0;
  tv.tv_usec = readtimeout;

  FD_ZERO(&fds);
  FD_SET(fd, &fds);

  int br = 0;

  if(select(fd+1, &fds, NULL, NULL, &tv) > 0)
    {
      br = read(fd, buffer, size);

      if(br< 0)
        {
          return 0;
        }
    }

  return (XDWORD)br;
}




//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUSB::Write
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       XDWORD :
//  @param        buffer :
//  @param        size :
*/
//-------------------------------------------------------------------
XDWORD DIOANDROIDSTREAMUSB::WriteBuffer(XBYTE* buffer, XDWORD size)
{
  if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return 0;

  fd_set         fds;
  struct timeval tv;

  tv.tv_sec  = 0;
  tv.tv_usec = writetimeout;

  FD_ZERO(&fds);
  FD_SET(fd, &fds);

  int bw = 0;

  if(select(fd+1, NULL , &fds, NULL, &tv) > 0)
    {
      bw = write(fd, buffer, size);
      if(bw<0) return 0;
    }

  return (XDWORD)bw;
}



#endif