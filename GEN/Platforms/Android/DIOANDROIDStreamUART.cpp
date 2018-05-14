//------------------------------------------------------------------------------------------
//  DIOANDROIDSTREAMUART.CPP
//
//  ANDROID Data IO Stream UART class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/01/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#if defined(DIO_ACTIVE) && defined(DIOUART_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <iostream>
#include <errno.h>
#include <termios.h>

#include <sys/wait.h>
#include <string.h>


#include "XBase.h"
#include "XFactory.h"
#include "XString.h"
#include "XDebug.h"
#include "XThreadCollected.h"

#include "DIOStreamXEvent.h"
#include "DIOStreamUARTConfig.h"
#include "DIOANDROIDStreamUART.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::DIOANDROIDSTREAMUART
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
DIOANDROIDSTREAMUART::DIOANDROIDSTREAMUART() : DIOSTREAMUART() , XFSMACHINE(0)
{
  Clean();

  AddState( DIOANDROIDUARTFSMSTATE_NONE             ,
            DIOANDROIDUARTFSMEVENT_CONNECTED        , DIOANDROIDUARTFSMSTATE_CONNECTED         ,
            DIOANDROIDUARTFSMEVENT_DISCONNECTING    , DIOANDROIDUARTFSMSTATE_DISCONNECTING     ,
            EVENTDEFEND);

  AddState( DIOANDROIDUARTFSMSTATE_CONNECTED        ,
            DIOANDROIDUARTFSMEVENT_WAITINGTOREAD    , DIOANDROIDUARTFSMSTATE_WAITINGTOREAD     ,
            DIOANDROIDUARTFSMEVENT_DISCONNECTING    , DIOANDROIDUARTFSMSTATE_DISCONNECTING     ,
            EVENTDEFEND);

  AddState( DIOANDROIDUARTFSMSTATE_WAITINGTOREAD    ,
            DIOANDROIDUARTFSMEVENT_CONNECTED        , DIOANDROIDUARTFSMSTATE_CONNECTED         ,
            DIOANDROIDUARTFSMEVENT_DISCONNECTING    , DIOANDROIDUARTFSMSTATE_DISCONNECTING     ,
            EVENTDEFEND);

  AddState( DIOANDROIDUARTFSMSTATE_DISCONNECTING    ,
            DIOANDROIDUARTFSMEVENT_CONNECTED        , DIOANDROIDUARTFSMSTATE_CONNECTED         ,
            DIOANDROIDUARTFSMEVENT_WAITINGTOREAD    , DIOANDROIDUARTFSMSTATE_WAITINGTOREAD     ,
            EVENTDEFEND);

  threadconnexion = CREATEXTHREAD(XTHREADGROUPID_DIOSTREAMUART, __L("DIOANDROIDSTREAMUART::DIOANDROIDSTREAMUART"), ThreadConnexion, (void*)this);

  //threadconnexion->SetPriority(THREADPRIORITY_REALTIME);
}


//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::~DIOANDROIDSTREAMUART
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
DIOANDROIDSTREAMUART::~DIOANDROIDSTREAMUART()
{
  Close();

  if(threadconnexion) DELETEXTHREAD(XTHREADGROUPID_DIOSTREAMUART, threadconnexion);

  Clean();
}





//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::GetConnectStatus
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       DIOSTREAMSTATUS :
*/
//-------------------------------------------------------------------
DIOSTREAMSTATUS DIOANDROIDSTREAMUART::GetConnectStatus()
{
  if(fd<0)    return DIOSTREAMSTATUS_DISCONNECTED;
  if(!config) return DIOSTREAMSTATUS_DISCONNECTED;

  return status;
}


//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::Open
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMUART::Open()
{
  if(!config) return false;

  XSTRING ttyS;

  if(!config->GetLocalDeviceName()->IsEmpty())
    {
      ttyS = config->GetLocalDeviceName()->Get();
    }
   else
    {
      ttyS.Format(__L("/dev/ttyS%d"),config->GetPort());
    }

  XSTRING_CREATEOEM(ttyS, charOEM)
  fd= open(charOEM, O_RDWR | O_NOCTTY | O_NONBLOCK);
  XSTRING_DELETEOEM(charOEM)

  if(fd<0) return false;

  SetRTS(false);
  SetDTR(false);

  CleanBuffers();

  //fcntl(fd, F_SETFL, FNDELAY);
 //fcntl(fd, F_SETFL, 0);

  if(!Config()) return false;

  SetEvent(DIOANDROIDUARTFSMEVENT_CONNECTED);

  status = DIOSTREAMSTATUS_CONNECTED;

  ResetXBuffers();
  ResetConnexionStatistics();

  return threadconnexion->Ini();
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::Config
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
bool DIOANDROIDSTREAMUART::Config(XWORD mask)
{
  if(fd<0) return false;

  struct termios cfg;
  speed_t        speed;

  tcgetattr(fd, &cfg);      // save current port settings

  if((mask&DIOSTREAMUARTMASK_BAUDRATE) == DIOSTREAMUARTMASK_BAUDRATE)
    {
      switch(this->config->GetBaudRate())
        {
          case 50     : speed = B50;        break;
          case 300    : speed = B300;       break;
          case 600    : speed = B600;       break;
          case 1200   : speed = B1200;      break;
          case 4800   : speed = B4800;      break;
          case 9600   : speed = B9600;      break;
          case 19200  : speed = B19200;     break;
          case 38400  : speed = B38400;     break;
          case 57600  : speed = B57600;     break;
          case 115200 : speed = B115200;    break;
        }

      cfsetispeed(&cfg, speed);
      cfsetospeed(&cfg, speed);
    }

  if((mask&DIOSTREAMUARTMASK_DATABITS ) == DIOSTREAMUARTMASK_DATABITS)
    {
      cfg.c_cflag&= ~CSIZE;

      switch(this->config->GetDataBits())
        {
          case DIOSTREAMUARTDATABIT_5           : cfg.c_cflag|= CS5; break;
          case DIOSTREAMUARTDATABIT_6           : cfg.c_cflag|= CS6; break;
          case DIOSTREAMUARTDATABIT_7           : cfg.c_cflag|= CS7; break;
          case DIOSTREAMUARTDATABIT_8           : cfg.c_cflag|= CS8; break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_PARITY) == DIOSTREAMUARTMASK_PARITY)
    {
      cfg.c_iflag  = (IGNBRK| IGNPAR);

      cfg.c_cflag &= ~CMSPAR;
      cfg.c_cflag &= ~PARENB;
      cfg.c_cflag &= ~PARODD;

      switch(this->config->GetParity())
        {
          case DIOSTREAMUARTPARITY_NONE         : cfg.c_cflag &= ~CMSPAR;
                                                  cfg.c_cflag &= ~PARENB;
                                                  cfg.c_cflag &= ~PARODD;
                                                  break;

          case DIOSTREAMUARTPARITY_MARK         : cfg.c_cflag |= CMSPAR;       // Not available on many systems.

          case DIOSTREAMUARTPARITY_ODD          : cfg.c_cflag |= PARENB;
                                                  cfg.c_cflag |= PARODD;
                                                  break;

          case DIOSTREAMUARTPARITY_SPACE        : cfg.c_cflag |= CMSPAR;      // Not available on many systems.

          case DIOSTREAMUARTPARITY_EVEN         : cfg.c_cflag |= PARENB;
                                                  cfg.c_cflag &= ~PARODD;
                                                  break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_STOPBITS ) == DIOSTREAMUARTMASK_STOPBITS)
    {
      cfg.c_cflag &= ~CSTOPB;

      switch(this->config->GetStopBits())
        {
          case DIOSTREAMUARTSTOPBITS_ONE          : cfg.c_cflag &= ~CSTOPB;   break;
          case DIOSTREAMUARTSTOPBITS_ONEANDAHALF  :
          case DIOSTREAMUARTSTOPBITS_TWO          : cfg.c_cflag |=  CSTOPB;   break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_FLOWCONTROL) == DIOSTREAMUARTMASK_FLOWCONTROL)
    {
      switch(config->GetFlowControl())
        {
          case DIOSTREAMUARTFLOWCONTROL_NONE    : cfg.c_cflag &= ~CRTSCTS;
                                                  cfg.c_iflag &= ~(IXON | IXOFF | IXANY);
                                                  break;

          case DIOSTREAMUARTFLOWCONTROL_HARD    : cfg.c_cflag |=  CRTSCTS;
                                                  cfg.c_iflag &= ~(IXON | IXOFF | IXANY);
                                                  break;

          case DIOSTREAMUARTFLOWCONTROL_SOFT    : cfg.c_cflag &= ~CRTSCTS;
                                                  cfg.c_iflag |= (IXON | IXOFF | IXANY);
                                                  break;
        }
    }

  if((mask&DIOSTREAMUARTMASK_ALL) == DIOSTREAMUARTMASK_ALL)
    {

      cfg.c_cflag |= (CLOCAL | CREAD);

      cfg.c_oflag &= ~OPOST;
      cfg.c_lflag &= ~(ICANON | ECHO | ISIG);


      cfg.c_cc[VMIN]  = 0;      // 0 means use-vtime
      cfg.c_cc[VTIME] = 1;      // time to wait until
    }

  tcsetattr(fd, TCSANOW, &cfg);

  return true;
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::ReadDirect
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
XDWORD DIOANDROIDSTREAMUART::ReadDirect(XBYTE* buffer, XDWORD size)
{
  if(fd<0)                                             return 0;
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
          /*
          if(errno == EAGAIN)
            {

            }
          else
            {
              return 0;
            }
          */

          return 0;
        }
    }

  return (XDWORD)br;
}




/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::WriteDirect
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
XDWORD DIOANDROIDSTREAMUART::WriteDirect(XBYTE* buffer, XDWORD size)
{
  if(fd<0)                                             return 0;
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



//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::Close
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMUART::Close()
{
  //if(threadconnexion) threadconnexion->End();

  //if(GetConnectStatus()==DIOSTREAMSTATUS_DISCONNECTED) return false;

  if(fd != -1)
    {
      close(fd);
      fd = -1;
    }

  return true;
}


//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::GetCTS
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMUART::GetCTS()
{
  if(fd<0) return false;

  int _status;

  ioctl(fd, TIOCMGET, &_status);

  return ((_status & TIOCM_CTS) != 0);
}



//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::GetDSR
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMUART::GetDSR()
{
  if(fd<0) return false;

  int _status;

  ioctl(fd, TIOCMGET, &_status);

  return ((_status & TIOCM_DSR) != 0);
}




//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::GetRing
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMUART::GetRing()
{
  if(fd<0) return false;

  int _status;

  ioctl(fd, TIOCMGET, &_status);

  return ((_status & TIOCM_RNG) != 0);
}




/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::GetRLSD
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/11/2014 17:29:42
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMUART::GetRLSD()
{
  if(fd<0) return false;

  return true;
}




//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::SetRTS
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
//  @param        on :
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMUART::SetRTS(bool on)
{
  if(fd<0) return false;

  int _status;

  ioctl(fd, TIOCMGET, &_status); // get the serial port status

  if(on) _status &= ~TIOCM_RTS; else _status |= TIOCM_RTS;

  ioctl(fd, TIOCMSET, &_status); // set the serial port status

  return true;
}




//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::SetDTR
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
//  @param        on :
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMUART::SetDTR(bool on)
{
  if(fd<0) return false;

  int _status;

  ioctl(fd, TIOCMGET, &_status);  // get the serial port status

  if(on) _status &= ~TIOCM_DTR; else  _status |= TIOCM_DTR;

  ioctl(fd, TIOCMSET, &_status);  // set the serial port status

  return true;
}



//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::CleanBuffers
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
bool DIOANDROIDSTREAMUART::CleanBuffers()
{
  if(fd<0) return false;

  tcflush(fd, TCIOFLUSH);

  return true;
}



//-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return
*/
//-------------------------------------------------------------------
void DIOANDROIDSTREAMUART::Clean()
{
  fd            = -1;
  readtimeout   = 3000;
  writetimeout  = 3000;
}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMUART::ThreadConnexion
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
void DIOANDROIDSTREAMUART::ThreadConnexion(void* param)
{
  DIOANDROIDSTREAMUART* diostream = (DIOANDROIDSTREAMUART*)param;
  if(!diostream) return;

  if(diostream->GetEvent()==DIOANDROIDUARTFSMEVENT_NONE) // No hay nuevos Eventos
    {
      switch(diostream->GetCurrentState())
        {
          case DIOANDROIDUARTFSMSTATE_NONE                    : break;

          case DIOANDROIDUARTFSMSTATE_CONNECTED               : break;

          case DIOANDROIDUARTFSMSTATE_WAITINGTOREAD           : {
                                                                  if(!diostream->IsBlockRead())
                                                                    {
                                                                      XBYTE buffer[DIOSTREAM_MAXBUFFER];

                                                                      XDWORD br = diostream->ReadDirect(buffer, DIOSTREAM_MAXBUFFER);
                                                                      if(br)
                                                                        {
                                                                          //XDEBUG_PRINTDATABLOCK(XDEBUGLEVELCOLOR(2), (XBYTE*)diostream->buffer, (int)br);
                                                                          diostream->inbuffer->Add(buffer, br);
                                                                        }
                                                                    }

                                                                  if(!diostream->IsBlockWrite())
                                                                    {
                                                                      int esize = diostream->outbuffer->GetSize();
                                                                      if(esize)
                                                                        {
                                                                          if(esize>DIOSTREAM_MAXBUFFER) esize = DIOSTREAM_MAXBUFFER;

                                                                          diostream->outbuffer->SetBlocked(true);
                                                                          XDWORD bw = diostream->WriteDirect(diostream->outbuffer->Get(), esize);
                                                                          diostream->outbuffer->SetBlocked(false);

                                                                          if(bw)
                                                                            {
                                                                              //XDEBUG_PRINTDATABLOCK(XDEBUGLEVELCOLOR(4), (XBYTE*)diostream->buffer, (int)bw);
                                                                              diostream->outbuffer->Extract(NULL, 0, bw);
                                                                            }
                                                                        }
                                                                    }
                                                              }
                                                              break;

          case DIOANDROIDUARTFSMSTATE_DISCONNECTING           : break;

        }
    }
   else
    {
      if(diostream->GetEvent()<DIOANDROIDUART_LASTEVENT)
        {
          diostream->CheckTransition();

          switch(diostream->GetCurrentState())
            {
              case DIOANDROIDUARTFSMSTATE_NONE                : break;

              case DIOANDROIDUARTFSMSTATE_CONNECTED           : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_CONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                diostream->PostEvent(&xevent);

                                                                diostream->SetEvent(DIOANDROIDUARTFSMEVENT_WAITINGTOREAD);
                                                              }
                                                              break;

              case DIOANDROIDUARTFSMSTATE_WAITINGTOREAD       : break;

              case DIOANDROIDUARTFSMSTATE_DISCONNECTING       : { DIOSTREAMXEVENT xevent(diostream,DIOSTREAMXEVENTTYPE_DISCONNECTED);
                                                                xevent.SetDIOStream(diostream);
                                                                diostream->PostEvent(&xevent);

                                                                diostream->threadconnexion->Run(false);
                                                                diostream->status = DIOSTREAMSTATUS_DISCONNECTED;
                                                              }
                                                              break;
            }
        }
    }
}


#endif
