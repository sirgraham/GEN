/*------------------------------------------------------------------------------------------
//  INPLINUXFACTORY.H
*/
/**
// \class
//
//  linux input factory
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 05/09/2016 17:55:52
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _INPLINUXFACTORY_H_
#define _INPLINUXFACTORY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "INPDevice.h"
#include "INPFactory.h"
#include "INPLINUXDeviceKeyboardDirect.h"
#include "INPLINUXDeviceMouseDirect.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class INPLINUXFACTORY : public INPFACTORY
{
public:

      virtual INPDEVICE* CreateDevice(XCHAR* device,void* screenpointer=NULL);

private:

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

