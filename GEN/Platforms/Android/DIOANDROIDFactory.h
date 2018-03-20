//------------------------------------------------------------------------------------------
//	DIOANDROIDFACTORY.H
//	
/**	
// \class 
//   
//  ANDROID DIO factory class
//   
//	@author	 Abraham J. Velez
//	@version 15/07/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOANDROIDFACTORY_H_
#define _DIOANDROIDFACTORY_H_
		

#ifdef DIO_ACTIVE
	
//---- INCLUDES ----------------------------------------------------------------------------	

#include "DIOFactory.h"
	
//---- DEFINES & ENUMS  ------------------------------------------------------------------
	
	
//---- CLASS -------------------------------------------------------------------------------
	
class DIOANDROIDFACTORY : public DIOFACTORY
{
	public:

		DIOURL*										CreateURL										();
		bool											DeleteURL										(DIOURL* url);		

		#ifdef ANYTYPEOFDIOSTREAMIO
		DIOSTREAMENUMDEVICES*			CreateStreamEnumDevices			(  DIOSTREAMENUMTYPE type);
		bool											DeleteStreamEnumDevices			(DIOSTREAMENUMDEVICES* enumdevices);		
				
		DIOSTREAM*								CreateStreamIO							(  DIOSTREAMCONFIG* config);
		bool											DeleteStreamIO							(DIOSTREAM* diostream);		
		#endif
		
		#ifdef DIOPING_ACTIVE
		DIOPING*									CreatePing    							();
		bool											DeletePing    							(DIOPING* ping);
		#endif

    #ifdef DIOPCAP_ACTIVE
		virtual DIOPCAP*					CreatePCap    							();
		virtual bool							DeletePCap     							(DIOPCAP* pcap);
		#endif				
		
		#ifdef DIOGPIO_ACTIVE
		DIOGPIO*							CreateGPIO							();
		bool											DeleteGPIO							(DIOGPIO* gpio);
		#endif
};
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

#endif