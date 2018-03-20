//------------------------------------------------------------------------------------------
//	DIOSTREAMUSB.H
//	
/**	
// \class 
//   
//  WINDOWS Data IO Stream USB class
//   
//	@author	 Abraham J. Velez
//	@version 23/05/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTREAMUSB_H_
#define _DIOSTREAMUSB_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStream.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMCONFIG;
class DIOSTREAMUSBCONFIG;


class DIOSTREAMUSB : public DIOSTREAM
{
	public:
																	DIOSTREAMUSB											();
		virtual											 ~DIOSTREAMUSB											();

		DIOSTREAMCONFIG*							GetConfig													();
		virtual bool								  SetConfig													(DIOSTREAMCONFIG* config);

		virtual DIOSTREAMSTATUS				GetConnectStatus									() 																= 0;
		virtual bool									Open															() 																= 0;

		virtual bool									Close															() 																= 0;
    
		virtual bool								  CleanBuffers											() 																						{	return false;  };

	protected:

		DIOSTREAMUSBCONFIG*						config;
		
	private:

		void												  Clean															();
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif
