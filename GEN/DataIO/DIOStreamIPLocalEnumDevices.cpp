//------------------------------------------------------------------------------------------
//  DIOSTREAMIPLOCALENUMDEVICES.CPP
//
//  Data IO Stream IP Local Enum Devices class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStreamDeviceIP.h"

#include "DIOStreamIPLocalEnumDevices.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTREAMIPLOCALENUMDEVICES::DIOSTREAMIPLOCALENUMDEVICES
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 18:37:55
//
//  @return       void :

//  @param        publisher :
*/
/*-----------------------------------------------------------------*/
DIOSTREAMIPLOCALENUMDEVICES::DIOSTREAMIPLOCALENUMDEVICES() : DIOSTREAMENUMDEVICES()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMIPLOCALENUMDEVICES::~DIOSTREAMIPLOCALENUMDEVICES
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 18:38:48
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOSTREAMIPLOCALENUMDEVICES::~DIOSTREAMIPLOCALENUMDEVICES()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOSTREAMIPLOCALENUMDEVICES::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/04/2013 18:42:16
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOSTREAMIPLOCALENUMDEVICES::Clean()
{
  type        = DIOSTREAMENUMTYPE_IP_LOCAL;
  issearching = false;
}



/*-------------------------------------------------------------------
//  DIOSTREAMIPLOCALENUMDEVICES::GetFirstActiveDevice
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/05/2013 15:37:07
//
//  @return       DIOSTREAMDEVICE* :
//  */
/*-----------------------------------------------------------------*/
DIOSTREAMDEVICE* DIOSTREAMIPLOCALENUMDEVICES::GetFirstActiveDevice()
{
  Search();

  for(int c=0;c<(int)GetDevices()->GetSize();c++)
    {
      DIOSTREAMDEVICEIP* device = (DIOSTREAMDEVICEIP*)GetDevices()->Get(c);
      if(device)
        {
          if((!device->GetIP()->IsEmpty()) &&
             ((device->GetIPType() == DIOSTREAMIPDEVICE_TYPE_ETHERNET) ||
              (device->GetIPType() == DIOSTREAMIPDEVICE_TYPE_WIFI)     ||
              (device->GetIPType() == DIOSTREAMIPDEVICE_TYPE_PPP)))
            {
              return device;
            }
        }
    }

  return NULL;
}