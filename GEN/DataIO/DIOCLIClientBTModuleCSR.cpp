//------------------------------------------------------------------------------------------
//	DIOCLICLIENTBTMODULECSR.CPP
//	
//	CLI Client functions
//
//	Author						: Abraham J. Velez
//	Date Of Creation	: 13/05/2002
//	Last Mofificacion	:	
//
//	GEN  Copyright (C).  All right reserved.		 			
//------------------------------------------------------------------------------------------
	

//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#include "XFactory.h"
#include "XFSMachine.h"

#include "DIOStream.h"
#include "DIOCLIClientBTModuleCSR.h"

#include "XMemory.h"

	
//---- GENERAL VARIABLE -------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------

	



/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::DIOCLICLIENTBTMODULECSR
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/10/2011 15:55:49
//	
//	@return				 

//  @param				diostream : 
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENTBTMODULECSR::DIOCLICLIENTBTMODULECSR(DIOSTREAM* diostream) : DIOCLICLIENT(diostream)
{  
	AddCommand(DIOCLICLIENT_TYPECOMMAND_VERSION							,__L("version"));
	
	AddCommand(DIOCLICLIENT_TYPECOMMAND_RSSI								,__L("rssi"));

	AddCommand(DIOCLICLIENT_TYPECOMMAND_GETCFGIOD						,__L("getcfgiod"));
	AddCommand(DIOCLICLIENT_TYPECOMMAND_SETCFGIOD						,__L("setcfgiod"));
	AddCommand(DIOCLICLIENT_TYPECOMMAND_GETIOD							,__L("getiod"));
	AddCommand(DIOCLICLIENT_TYPECOMMAND_SETIOD							,__L("setiod"));
	AddCommand(DIOCLICLIENT_TYPECOMMAND_GETIOA1							,__L("getioa1"));

	AddCommand(DIOCLICLIENT_TYPECOMMAND_ISBATTERYCHARGING		,__L("isbatterycharging"));
	AddCommand(DIOCLICLIENT_TYPECOMMAND_BATTERYCHARGE				,__L("batterycharge"));

	AddCommand(DIOCLICLIENT_TYPECOMMAND_I2CMEMREAD					,__L("i2cmemread"));
	AddCommand(DIOCLICLIENT_TYPECOMMAND_I2CMEMWRITE					,__L("i2cmemwrite"));

	AddCommand(DIOCLICLIENT_TYPECOMMAND_I2CADC							,__L("i2cadc"));
	AddCommand(DIOCLICLIENT_TYPECOMMAND_I2CDAC							,__L("i2cdac"));
}




/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::~DIOCLICLIENTBTMODULECSR
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/10/2011 15:57:52
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
DIOCLICLIENTBTMODULECSR::~DIOCLICLIENTBTMODULECSR()
{
	
}




/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::Version
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/10/2011 15:58:16
//	
//	@return				DIOCLICLIENT_ERROR : 
//	@param				version : 
//  @param				subversion : 
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENTBTMODULECSR::Version(int& version,int& subversion)
{
	XSTRING				 answer;
	DIOCLICLIENT_ERROR error;	

	error = SendCommandWithAnswer(DIOCLICLIENT_TYPECOMMAND_VERSION,answer);	
	if(error==DIOCLICLIENT_ERROR_NONE) answer.UnFormat(__L("%d.%d"),&version,&subversion);
		
	return error;	
}



/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::RSSI
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			21/10/2011 13:08:03
//	
//	@return				DIOCLICLIENT_ERROR : 
//	@param				RSSI : 
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENTBTMODULECSR::RSSI(int& RSSI)
{
	XSTRING				 answer;
	DIOCLICLIENT_ERROR error;	

	error = SendCommandWithAnswer(DIOCLICLIENT_TYPECOMMAND_RSSI,answer);	
	if(error==DIOCLICLIENT_ERROR_NONE) RSSI = answer.ConvertToInt();

	return error;	
}




/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::GetCFGIOD
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/10/2011 10:48:32
//	
//	@return				DIOCLICLIENT_ERROR : 
//	@param				portio : 
//  @param				isoutput : 
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENTBTMODULECSR::GetCFGIOD(int portio,bool& isoutput)
{
	XSTRING				 param;
	XSTRING				 answer;
	DIOCLICLIENT_ERROR error;	

	param.Format(__L("%d"),portio);
		
	error = SendCommandWithAnswer(DIOCLICLIENT_TYPECOMMAND_GETCFGIOD,answer,param.Get());	
	if(error==DIOCLICLIENT_ERROR_NONE) isoutput = (XBYTE)answer.ConvertToInt()?false:true;

	return error;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::SetCFGIOD
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/10/2011 11:30:20
//	
//	@return				DIOCLICLIENT_ERROR : 
//	@param				portio : 
//  @param				isoutput : 
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENTBTMODULECSR::SetCFGIOD(int portio,bool isoutput)
{
	XSTRING param;
	
	param.Format(__L("%d,%d"),portio,isoutput?0:1);
		
	return SendCommandWithoutAnswer(DIOCLICLIENT_TYPECOMMAND_SETCFGIOD, param.Get());	
}




/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::GetIOD
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/10/2011 16:44:26
//	
//	@return				DIOCLICLIENT_ERROR : 
//	@param				portio : 
//  @param				isactive : 
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENTBTMODULECSR::GetIOD(int portio,bool& isactive)
{
	XSTRING				 param;
	XSTRING				 answer;
	DIOCLICLIENT_ERROR error;	

	param.Format(__L("%d"),portio);
		
	error = SendCommandWithAnswer(DIOCLICLIENT_TYPECOMMAND_GETIOD,answer,param.Get());	
	if(error==DIOCLICLIENT_ERROR_NONE) isactive = (XBYTE)answer.ConvertToInt()?true:false;

	return error;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::SetIOD
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/10/2011 16:47:31
//	
//	@return				DIOCLICLIENT_ERROR : 
//	@param				portio : 
//  @param				isactive : 
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENTBTMODULECSR::SetIOD(int portio,bool isactive)
{
	XSTRING	param;

	param.Format(__L("%d,%d"),portio,isactive?1:0);
		
	return SendCommandWithoutAnswer(DIOCLICLIENT_TYPECOMMAND_SETIOD, param.Get());	
}



/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::GetIOA1
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/10/2011 16:52:15
//	
//	@return				DIOCLICLIENT_ERROR : 
//	@param				value : 
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENTBTMODULECSR::GetIOA1(XBYTE& value)
{
	XSTRING				 answer;
	DIOCLICLIENT_ERROR error;	
		
	error = SendCommandWithAnswer(DIOCLICLIENT_TYPECOMMAND_GETIOA1,answer);	
	if(error==DIOCLICLIENT_ERROR_NONE) value = (XBYTE)answer.ConvertToInt();

	return error;
}


/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::IsBatteryCharging
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/11/2011 0:44:01
//	
//	@return				DIOCLICLIENT_ERROR : 
//	@param				ischarging : 
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENTBTMODULECSR::IsBatteryCharging(bool& ischarging)
{
	XSTRING				 answer;
	DIOCLICLIENT_ERROR error;	
		
	error = SendCommandWithAnswer(DIOCLICLIENT_TYPECOMMAND_ISBATTERYCHARGING,answer);	
	if(error==DIOCLICLIENT_ERROR_NONE) ischarging = (XBYTE)answer.ConvertToInt()?true:false;

	return error;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::BatteryCharge
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/11/2011 0:44:07
//	
//	@return				DIOCLICLIENT_ERROR : 
//	@param				&mVolts : 
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENTBTMODULECSR::BatteryCharge(int& mVolts)
{
	XSTRING				 answer;
	DIOCLICLIENT_ERROR error;	
		
	error = SendCommandWithAnswer(DIOCLICLIENT_TYPECOMMAND_BATTERYCHARGE,answer);	
	if(error==DIOCLICLIENT_ERROR_NONE) mVolts = (XBYTE)answer.ConvertToInt();

	return error;
}


/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::I2CMemRead
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/10/2011 16:55:05
//	
//	@return				DIOCLICLIENT_ERROR : 
//	@param				address : 
//  @param				index : 
//  @param				data : 
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENTBTMODULECSR::I2CMemRead(int address,int index,XBYTE& data)
{
	XSTRING				 param;
	XSTRING				 answer;
	DIOCLICLIENT_ERROR error;	

	param.Format(__L("%d,%d"),address,index);
		
	error = SendCommandWithAnswer(DIOCLICLIENT_TYPECOMMAND_I2CMEMREAD,answer,param.Get());	
	if(error==DIOCLICLIENT_ERROR_NONE) data = (XBYTE)answer.ConvertToInt();

	return error;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::I2CMemWrite
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/10/2011 16:55:44
//	
//	@return				DIOCLICLIENT_ERROR : 
//	@param				address : 
//  @param				index : 
//  @param				data : 
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENTBTMODULECSR::I2CMemWrite(int address,int index,XBYTE data)
{
	XSTRING param;
	XSTRING string;	

	param.Format(__L("%d,%d,%d"),address,index,data);
		
	return SendCommandWithoutAnswer(DIOCLICLIENT_TYPECOMMAND_I2CMEMWRITE, param.Get());	
}




/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::I2CADC
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/10/2011 17:26:28
//	
//	@return				DIOCLICLIENT_ERROR : 
//	@param				address : 
//  @param				port : 
//  @param				data : 
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENTBTMODULECSR::I2CADC(int address,int port,XBYTE& data)
{
	XSTRING				 param;
	XSTRING				 answer;
	DIOCLICLIENT_ERROR error;	

	param.Format(__L("%d,%d"),address,port);
		
	error = SendCommandWithAnswer(DIOCLICLIENT_TYPECOMMAND_I2CADC,answer,param.Get());	
	if(error==DIOCLICLIENT_ERROR_NONE) data = (XBYTE)answer.ConvertToInt();

	return error;
}



/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::I2CDAC
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/10/2011 17:26:28
//	
//	@return				DIOCLICLIENT_ERROR : 
//	@param				address : 
//  @param				port : 
//  @param				data : 
*/
/*-----------------------------------------------------------------*/
DIOCLICLIENT_ERROR DIOCLICLIENTBTMODULECSR::I2CDAC(int address,XBYTE data)
{
	XSTRING param;
	
	param.Format(__L("%d,%d"),address,data);
		
	return SendCommandWithoutAnswer(DIOCLICLIENT_TYPECOMMAND_I2CDAC,param.Get());	
}



/*-------------------------------------------------------------------
//  DIOCLICLIENTBTMODULECSR::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			31/10/2011 15:58:34
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
void DIOCLICLIENTBTMODULECSR::Clean()
{	

}





