//------------------------------------------------------------------------------------------
//  DIOANDROIDSTREAMUARTLOCALENUMDEVICES.CPP
//
//  ANDROID Data IO Stream UART Local Enum Devices class
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
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
//#include <linux/serial.h>
#include <serial.h>

#include "XDebug.h"
#include "XPath.h"
#include "DIOStreamDevice.h"

#include "DIOANDROIDStreamUARTLocalEnumDevices.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMUARTLOCALENUMDEVICES::DIOANDROIDSTREAMUARTLOCALENUMDEVICES
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 19:00:01
//
//  @return


*/
/*-----------------------------------------------------------------*/
DIOANDROIDSTREAMUARTLOCALENUMDEVICES::DIOANDROIDSTREAMUARTLOCALENUMDEVICES() : DIOSTREAMUARTLOCALENUMDEVICES()
{

}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMUARTLOCALENUMDEVICES::~DIOANDROIDSTREAMUARTLOCALENUMDEVICES
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 19:00:12
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOANDROIDSTREAMUARTLOCALENUMDEVICES::~DIOANDROIDSTREAMUARTLOCALENUMDEVICES()
{

}



/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMUARTLOCALENUMDEVICES::Search
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 19:00:27
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMUARTLOCALENUMDEVICES::Search()
{
  DelAllDevices();

  struct dirent** namelist;
  XSTRING         sysdir;
  int             ndevices;

  sysdir = __L("/sys/class/tty/");

  XSTRING_CREATEOEM(sysdir, charOEM)
  ndevices = scandir(charOEM, &namelist, NULL, NULL);
  XSTRING_DELETEOEM(charOEM)
  if(ndevices >= 0)
    {
      while(ndevices--)
        {
          if(strcmp(namelist[ndevices]->d_name,"..") && strcmp(namelist[ndevices]->d_name,"."))
            {
              XSTRING     devicedir;
              struct stat st;

              devicedir  = sysdir;
              devicedir += namelist[ndevices]->d_name;

              devicedir += __L("/device");

              XSTRING_CREATEOEM(devicedir, charOEM)
              bool result = (lstat(charOEM, &st)==0) && (S_ISLNK(st.st_mode));
              XSTRING_DELETEOEM(charOEM)
              if(result)
                {
                  char buffer[1024];

                  memset(buffer, 0, sizeof(buffer));

                  devicedir += __L("/driver");

                  XSTRING_CREATEOEM(devicedir, charOEM)
                  result = (readlink(charOEM, buffer, sizeof(buffer)) > 0);
                  XSTRING_DELETEOEM(charOEM)
                  if(result)
                    {
                      XSTRING tty;
                      XPATH   devicepath;
                      XPATH   devicefile;

                      tty = buffer;

                      devicepath  = sysdir;
                      devicepath += namelist[ndevices]->d_name;

                      devicepath.GetNamefile(devicefile);

                      devicepath  = __L("/dev/");
                      devicepath += devicefile;

                      // XDEBUG_PRINTCOLOR(4,__L(" %s %s %s"),devicedir.Get(), devicepath.Get(), tty.Get());

                      bool isvalid = false;

                      if(tty.Find(__L("serial8250"), true) != XSTRING_NOTFOUND)
                        {
                           struct serial_struct serinfo;

                           XSTRING_CREATEOEM(devicepath, charOEM)
                           int fd = open(charOEM, O_RDWR | O_NONBLOCK | O_NOCTTY);
                           XSTRING_DELETEOEM(charOEM)
                           if(fd >= 0)
                            {
                              // Get serial_info
                              if(ioctl(fd, TIOCGSERIAL, &serinfo)==0)
                                {
                                  // If device type is no PORT_UNKNOWN we accept the port
                                  if(serinfo.type != PORT_UNKNOWN) isvalid = true;
                                }

                              close(fd);
                            }
                        } else isvalid = true;

                      if(isvalid)
                        {
                          DIOSTREAMDEVICE* device = new DIOSTREAMDEVICE();
                          if(device)
                            {
                              //device->SetIndex(index);
                              device->GetResource()->Set(devicepath);

                              devices.Add(device);
                            }
                        }
                    }
                }
            }

          free(namelist[ndevices]);
        }

      free(namelist);
    }

  return true;
}




/*-------------------------------------------------------------------
//  DIOANDROIDSTREAMUARTLOCALENUMDEVICES::IsDeviceAvailable
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 19:00:32
//
//  @return       bool :
//  @param        resource :
*/
/*-----------------------------------------------------------------*/
bool DIOANDROIDSTREAMUARTLOCALENUMDEVICES::IsDeviceAvailable(XCHAR* resource)
{
  /*
  XSTRING strdevice;

  strdevice = resource;

  HANDLE hcomt = CreateFile(strdevice.Get(),
                            GENERIC_READ|GENERIC_WRITE,
                            0,0,
                            OPEN_EXISTING,
                            0,0);

  if(hcomt==INVALID_HANDLE_VALUE) return false;

  CloseHandle(hcomt);
  */
  return true;
}


#endif

