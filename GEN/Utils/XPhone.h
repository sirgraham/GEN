//------------------------------------------------------------------------------------------
//	XPHONE.H
//	
/**	
// \class 
//   
//  Phone Function class
//   
//	@author	 Abraham J. Velez
//	@version 25/10/2004 14:51:24
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _XPHONE_H_
#define _XPHONE_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"	
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------
	
enum XPHONEMODEL
{
	XPHONEMODEL_UNKNOWN								 = 0 ,

	XPHONEMODEL_SYMBIAN_EMULATORS60				 ,  
	XPHONEMODEL_SYMBIAN_NOKIA_7650		  	 ,	
	XPHONEMODEL_SYMBIAN_NOKIA_3650		  	 ,	
	XPHONEMODEL_SYMBIAN_NOKIA_NGAGE		  	 ,		
	XPHONEMODEL_SYMBIAN_NOKIA_6600		  	 ,	
	XPHONEMODEL_SYMBIAN_SIEMMENS_SX1	  	 ,	
	XPHONEMODEL_SYMBIAN_SENDO_X						 ,	
	XPHONEMODEL_SYMBIAN_SONYERIC_P800	  	 ,	
	XPHONEMODEL_SYMBIAN_SONYERIC_P900	  	 ,	
	XPHONEMODEL_SYMBIAN_NOKIA_9210	       ,
};

#define XPHONEMODEL_NOTBATTERY  -1

//---- CLASS -------------------------------------------------------------------------------

class XPHONELAC
{
	public:

		int													LAC;
		int													cellid;
		int													MCC;
		int													MNC;
};

	
class XPHONE 
{
	public:

																XPHONE								();
		virtual										 ~XPHONE								();
		
		
		virtual	bool								Ini										();	

		virtual bool								ResetScreenSaver			();

		virtual int									StrengthSignal				();
		virtual int									StrengthBattery				();

		virtual XPHONEMODEL					GetModel							();

		virtual bool								GetIMEI								(XBYTE* IMEI);				
		virtual bool								GetGSMLAC							(XPHONELAC* lac);

		virtual bool								SendSMS								(char* target,char* msg);

		virtual	int									GetPhoneBookNContacts	();
		virtual	char*								GetPhoneBookContact		(int index);

		virtual bool								End										();
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

