//------------------------------------------------------------------------------------------
//  INPMANAGER.CPP
//
//  Input Manager
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 10/02/2003
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"

#include "INPButton.h"
#include "INPDevice.h"
#include "INPManager.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------

INPMANAGER* INPMANAGER::Instance=NULL;


/*-------------------------------------------------------------------
//  INPMANAGER::DelInstance
*/
/**
//
//  Deletes instance
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/09/2016 13:23:55
//
//  @return       INPMANAGER* :
//
*/
/*-----------------------------------------------------------------*/
void  INPMANAGER::DelInstance()
{
  if (INPMANAGER::Instance)
  {
    INPMANAGER::GetInstance()->DeleteAllDevices();
    delete  INPMANAGER::Instance;
  }
}

/*-------------------------------------------------------------------
//  INPMANAGER::GetInstance
*/
/**
//
//  Get single instance
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/09/2016 13:23:37
//
//  @return       INPMANAGER* :
//
*/
/*-----------------------------------------------------------------*/
INPMANAGER* INPMANAGER::GetInstance()
{
  if (INPMANAGER::Instance==NULL)
    INPMANAGER::Instance=new INPMANAGER();

  return Instance;
}




//-------------------------------------------------------------------
//  INPMANAGER::INPMANAGER
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 16:17:02
//
//  @return

*/
//-------------------------------------------------------------------
INPMANAGER::INPMANAGER()
{
  Clean();


}


//-------------------------------------------------------------------
//  INPMANAGER::~INPMANAGER
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2003 16:00:13
//
//  @return       virtual :
//  */
//-------------------------------------------------------------------
INPMANAGER::~INPMANAGER()
{
  Clean();
}


/*-------------------------------------------------------------------
//  INPMANAGER::DeleteAllDevices
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/09/2016 14:29:16
//
*/
/*-----------------------------------------------------------------*/
bool INPMANAGER::DeleteAllDevices()
{

  for (int e=(int)this->devicemap.GetSize();e>=0;e--)
  {
    INPDEVICE* device=this->devicemap.Get(e);

    if(!device)               continue;
    if(!device->IsCreated())  continue;

    devicemap.Delete(device->GetType(),device);
    delete(device);
  }

  return true;
}

//-------------------------------------------------------------------
//  INPMANAGER::AddDevice
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2003 16:01:04
//
//  @return       bool :
//  @param        device :
*/
//-------------------------------------------------------------------
bool INPMANAGER::AddDevice(INPDEVICE* device)
{
  if(!device)             return false;
  if(!device->IsCreated()) return false;

  devicemap.Add(device->GetType(),device);

  return true;
}


/*-------------------------------------------------------------------
//  INPMANAGER::DelDevice
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/10/2013 23:54:19
//
//  @return       bool :
//  @param        device :
*/
/*-----------------------------------------------------------------*/
bool INPMANAGER::DelDevice(INPDEVICE* device)
{
  if(!device)               return false;
  if(!device->IsCreated())  return false;

  devicemap.Delete(device->GetType(),device);

  return true;
}



/*-------------------------------------------------------------------
//  INPMANAGER::GetNDevices
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/12/2011 16:58:32
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int INPMANAGER::GetNDevices()
{
  return devicemap.GetSize();
}



/*-------------------------------------------------------------------
//  INPMANAGER::GetDevice
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/12/2011 16:57:59
//
//  @return       INPDEVICE* :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
INPDEVICE* INPMANAGER::GetDevice(int index)
{
  if(devicemap.IsEmpty()) return NULL;

  INPDEVICE* device = devicemap.GetElement(index);
  if(device) return device;

  return NULL;
}



//-------------------------------------------------------------------
//  INPMANAGER::GetDevice
/**
//
//  @author        Abraham J. Velez
//  @version       20/2/2003   10:55:4
//
//  @return        INPDEVICE* :
//  @param         type :
*/
//-------------------------------------------------------------------
INPDEVICE* INPMANAGER::GetDevice(INPDEVICE_TYPE type)
{
  if(devicemap.IsEmpty()) return NULL;

  int index =  devicemap.Find(type);
  if(index == -1) return NULL;

  INPDEVICE* device = devicemap.GetElement(index);
  if(device) return device;

  return NULL;
}



//-------------------------------------------------------------------
//  INPMANAGER::GetDevice
/**
//
//  @author        Abraham J. Velez
//  @version       20/2/2003   12:31:16
//
//  @return        INPDEVICE* :
//  @param         nameID :
*/
//-------------------------------------------------------------------
INPDEVICE* INPMANAGER::GetDevice(XSTRING& name)
{
  if(devicemap.IsEmpty())  return NULL;

  return NULL;

  //INPDEVICE_ID ID = GetDeviceIDByName(name);
  //if(ID==INPDEVICE_ID_NONE) return NULL;

  //return GetDevice(ID);
}




/*-------------------------------------------------------------------
//  INPMANAGER::GetDevice
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/05/2011 11:38:35
//
//  @return       INPDEVICE* :
//  @param        nameID :
*/
/*-----------------------------------------------------------------*/
INPDEVICE* INPMANAGER::GetDevice(XCHAR* name)
{
  if(!name) return NULL;

  XSTRING _name(name);

  return GetDevice(_name);
}




//-------------------------------------------------------------------
//  INPMANAGER::Update
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2003 16:02:39
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool INPMANAGER::Update()
{
  if(devicemap.IsEmpty())  return false;

  for(XDWORD c=0;c<devicemap.GetSize();c++)
    {
      INPDEVICE* device = devicemap.GetElement(c);
      if(device) device->Update();
    }

  return true;
}



//-------------------------------------------------------------------
//  INPMANAGER::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/02/2003 16:03:02
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void INPMANAGER::Clean()
{
  devicemap.DeleteAll();
}
