//------------------------------------------------------------------------------------------
//	DIOSTREAMUDPCONFIG.H
//	
/**	
// \class 
//   
//  Data IO Stream UDP Config class
//   
//	@author	 Abraham J. Velez
//	@version 23/05/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOSTREAMUDPCONFIG_H_
#define _DIOSTREAMUDPCONFIG_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XThread.h"

#include "DIOIP.h"
#include "DIOURL.h"

#include "DIOStreamConfig.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XSTRING;
class DIOFACTORY;


class DIOSTREAMUDPCONFIG : public DIOSTREAMCONFIG
{
	public:					
														DIOSTREAMUDPCONFIG					();
		virtual								 ~DIOSTREAMUDPCONFIG					();

    DIOIP*									GetLocalIP									();		
		DIOURL*									GetRemoteURL								();
		
		int											GetRemotePort								(); 
		bool										SetRemotePort								(int port);	

		bool										IsUsedDatagrams			        ()																	{ return isuseddatagrams;                           }
		void										SetIsUsedDatagrams			    (bool active)												{ isuseddatagrams = active;													}
		
		bool										IsBroadcastModeActive				()																	{	return isbroadcastmodeactive;											}
		void										SetBroadcastModeActive			(bool active)												{	isbroadcastmodeactive = active;										}	
					
	protected:

    DIOIP										localIP;
		
		DIOURL*									remoteURL;
		int 										remoteport;
		int											remoteoffsetport;

		bool										isuseddatagrams;
		bool										isbroadcastmodeactive;

	private:
		
		void										Clean												()
														{
															type 										= DIOSTREAMTYPE_UDP;
		
															remoteURL								= NULL;
															remoteport							= 0;
															remoteoffsetport				= 0;

															isuseddatagrams					= false;	
															isbroadcastmodeactive		= false;															
														}

};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
