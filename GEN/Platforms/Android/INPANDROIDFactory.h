/*------------------------------------------------------------------------------------------
//	INPANDROIDFACTORY.H
*/	
/**	
// \class 
//   
//  Input factory android
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 06/09/2016 11:37:52
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _INPANDROIDFACTORY_H_
#define _INPANDROIDFACTORY_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "INPFactory.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class INPANDROIDFACTORY : public INPFACTORY
{
public:

virtual INPDEVICE* CreateDevice(XCHAR* device,void* screenpointer=NULL);

protected:

private:

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

