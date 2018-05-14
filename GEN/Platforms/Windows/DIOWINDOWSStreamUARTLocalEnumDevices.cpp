//------------------------------------------------------------------------------------------
//  DIOWINDOWSSTREAMUARTLOCALENUMDEVICES.CPP
//
//  WINDOWS Data IO Stream UART Local Enum Devices class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 02/01/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#if defined(DIO_ACTIVE) && defined(DIOUART_ACTIVE)

//---- INCLUDES ----------------------------------------------------------------------------

#include <windows.h>

#include "DIOStreamDevice.h"
#include "DIOWINDOWSStreamUARTLocalEnumDevices.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUARTLOCALENUMDEVICES::DIOWINDOWSSTREAMUARTLOCALENUMDEVICES
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
DIOWINDOWSSTREAMUARTLOCALENUMDEVICES::DIOWINDOWSSTREAMUARTLOCALENUMDEVICES() : DIOSTREAMUARTLOCALENUMDEVICES()
{

}



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUARTLOCALENUMDEVICES::~DIOWINDOWSSTREAMUARTLOCALENUMDEVICES
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
DIOWINDOWSSTREAMUARTLOCALENUMDEVICES::~DIOWINDOWSSTREAMUARTLOCALENUMDEVICES()
{

}



/*-------------------------------------------------------------------
//  DIOWINDOWSSTREAMUARTLOCALENUMDEVICES::Search
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
bool DIOWINDOWSSTREAMUARTLOCALENUMDEVICES::Search()
{
  DelAllDevices();

  HKEY  hkey;
  LONG  status;

  status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, __L("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_QUERY_VALUE, &hkey);
  if(status==ERROR_SUCCESS)
    {
      DWORD   index  = 0;
      XCHAR*  name   = new XCHAR[_MAXSTR];
      DWORD   sizename;
      DWORD   type;
      XCHAR*  value  = new XCHAR[_MAXSTR];
      DWORD   sizevalue;

      if((value)&&(name))
        {
          memset(name  ,0 ,sizeof(XCHAR)*_MAXSTR);
          memset(value ,0 ,sizeof(XCHAR)*_MAXSTR);

          do{ name[0]   = 0;
              sizename  = _MAXSTR;
              value[0]  = 0;
              sizevalue = _MAXSTR;
              type      = 0;

              status=RegEnumValue(hkey, index, name, &sizename, NULL, &type, (XBYTE*)value, &sizevalue);
              if(status==ERROR_SUCCESS)
                {
                  DIOSTREAMDEVICE* device = new DIOSTREAMDEVICE();
                  if(device)
                    {
                      device->SetIndex(index);
                      device->SetType(DIOSTREAMDEVICE_TYPE_UART);

                      device->GetResource()->Set(value);

                      devices.Add(device);
                    }

                  index++;
                }

            } while(status==ERROR_SUCCESS);
        }

      delete [] value;
      delete [] name;

      RegCloseKey(hkey);
    }

  return true;
}



#endif


