//------------------------------------------------------------------------------------------
//	DIOSTREAMENUMSERVERS.H
//	
/**	
// \class 
//   
//  Data IO Stream Local Enum Servers class
//   
//	@author	 Abraham J. Velez
//	@version 23/05/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTREAMENUMSERVERS_H_
#define _DIOSTREAMENUMSERVERS_H_
		
//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStreamEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XBUFFER;
class XTIMER;
class XTHREADCOLLECTED;
class DIOFACTORY;


class DIOSTREAMENUMSERVERS :  public DIOSTREAMENUMDEVICES
{
	public:
															DIOSTREAMENUMSERVERS						();
		virtual									 ~DIOSTREAMENUMSERVERS						();

		XDWORD										GetSearchTimeout								()																		{ return searchtimeout;														}	
		void											SetSearchTimeout								(int searchtimeout)										{ this->searchtimeout = searchtimeout;						}

		bool											Search													(int searchtimeout);
		
		virtual bool							Search													();
		virtual bool							StopSearch											(bool waitend);
		
		bool											IsSearching											();

		virtual bool							IsDeviceAvailable								(XCHAR* resource);		

		virtual bool							SendSignal											();

		XTIMER*										GetXTimer												();
		
		XDWORD										GetID														();  
		bool											SetID														(XDWORD ID);	 


	protected: 

		

		XTIMER*										xtimer;		
		XDWORD										searchtimeout;
		
		XDWORD										ID;		
		
		bool											issearching;		
	
	private:

		void											Clean														();
					
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
