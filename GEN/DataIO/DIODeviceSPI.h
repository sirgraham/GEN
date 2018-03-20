/*------------------------------------------------------------------------------------------
//	DIOSPIDEVICE.H
*/	
/**	
// \class 
//   
//  DIO SPI Device
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 24/09/2014 8:49:43
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOSPIDEVICE_H_
#define _DIOSPIDEVICE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"

#include "DIODevice.h"
#include "DIOStreamSPIConfig.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/


class DIODEVICESPI : public DIODEVICE
{
	public:

															DIODEVICESPI					()						
															{ 
																Clean();	

																xtimerout = xfactory->CreateTimer();
															}
																	

		virtual									 ~DIODEVICESPI					()						
															{ 
																if(xtimerout) xfactory->DeleteTimer(xtimerout);

																if(diostreamcfg) delete diostreamcfg;

																Clean();															
															}

		using DIODEVICE::Ini;
		bool											Ini										(XCHAR* localdevicename, int timeout)
															{																
																if(!xtimerout) return false;

																this->timeout	= timeout;

																diostreamcfg = CreateConfig(localdevicename);
																if(!diostreamcfg)  return false;   	

																return IniDevice();	
															}

		bool											Ini										(int port, int chipselect, int timeout)
															{																
																if(!xtimerout) return false;

																this->timeout	= timeout;

																diostreamcfg = CreateConfig(port, chipselect);
																if(!diostreamcfg)  return false;   
																
																return IniDevice();	
															}
    
		DIOSTREAMSPICONFIG*				CreateConfig          (XCHAR* localdevicename)
															{
																DIOSTREAMSPICONFIG* diostreamcfg = new DIOSTREAMSPICONFIG();
																if(!diostreamcfg)  return NULL;
		
															
																diostreamcfg->SetSPIMode(DIOSTREAMSPI_MODE_0);																	
																diostreamcfg->SetNBitsWord(8);		
																diostreamcfg->SetSpeed(80*1000*1000);	
																diostreamcfg->SetDelay(0);
																diostreamcfg->SetIsOnlyWrite(true);
 		
																diostreamcfg->GetLocalDeviceName()->Set(localdevicename);
																																
																return diostreamcfg;		
															}

		DIOSTREAMSPICONFIG*				CreateConfig          (int port, int chipselect)   
															{
																DIOSTREAMSPICONFIG* diostreamcfg = new DIOSTREAMSPICONFIG();
																if(!diostreamcfg)  return NULL;
																		
																diostreamcfg->SetPort(port);																
																diostreamcfg->SetSPIMode(DIOSTREAMSPI_MODE_0);	
																diostreamcfg->SetChipSelect(chipselect);
																diostreamcfg->SetNBitsWord(8);		
																diostreamcfg->SetSpeed(80*1000*1000);	
																diostreamcfg->SetDelay(0);
																diostreamcfg->SetIsOnlyWrite(true);
			
																diostreamcfg->SetLocalDeviceNameByPort();
																				
																return diostreamcfg;		
															}	

		virtual bool              IniDevice            ()																							=  0;


	protected:

		int												timeout;
		XTIMER*                   xtimerout;
		DIOSTREAMSPICONFIG*				diostreamcfg;
		
	private:

		void											Clean									()
															{
																timeout       = 0;
																xtimerout			= NULL;
																diostreamcfg	= NULL;
															}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

