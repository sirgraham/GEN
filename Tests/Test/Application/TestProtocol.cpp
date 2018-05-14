/*------------------------------------------------------------------------------------------
//	PROTOCOL.CPP
//	
//	Test Protocol
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 08/02/2012 23:28:49
//	Last Mofificacion	:	
//	
//	GEN (C) Copyright  (All right reserved).			 
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVariant.h"

#include "TestProtocolApplicationData.h"
#include "TestProtocol.h"

#include "XMemory.h"


	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  TESTPROTOCOL::TESTPROTOCOL
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/02/2012 23:32:09
//	
//	@return				

 
//  @param				publisher : 
//  @param				poller : 
//  @param				diostream : 
*/
/*-----------------------------------------------------------------*/
TESTPROTOCOL::TESTPROTOCOL(DIOSTREAM* diostream) : DIOPROTOCOL(diostream)
{
	AddCommand(TESTPROTOCOL_CMDTYPE_GETSTRING						  , __L("")	 				  , __L("S")				, RCV_GetString					, __L("GetString")						);			
	AddCommand(TESTPROTOCOL_CMDTYPE_SENDSTRING						, __L("S")	 			  , __L("")	  			, RCV_SendString				, __L("SendString")						);			
	AddCommand(TESTPROTOCOL_CMDTYPE_GETVARIANT					  , __L("D")					, __L("V")				, RCV_GetVariant				, __L("GetVariant")						);			
	AddCommand(TESTPROTOCOL_CMDTYPE_SENDVARIANT			  		, __L("DV")					, __L("B")				, RCV_SendVariant				, __L("SendVariant")					);				
	
}



/*-------------------------------------------------------------------
//  TESTPROTOCOL::~TESTPROTOCOL
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/02/2012 23:34:37
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
TESTPROTOCOL::~TESTPROTOCOL()
{

}



/*-------------------------------------------------------------------
//	TESTPROTOCOL::IniProtocolCommands
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			21/03/2016 21:57:11
//	
//	@return 			bool : 
//
//  @param				ismaster : 
*/
/*-----------------------------------------------------------------*/
bool TESTPROTOCOL::IniProtocolCommands(bool ismaster)
{ 
	TESTPROTOCOLAPPLICATIONDATA* applicationdata = (TESTPROTOCOLAPPLICATIONDATA*)GetApplicationData();
	if(!applicationdata) return false;

	bool status = false;
	 
	if(ismaster)
		{				
 			XWORD protocolversion			   = 0; 
			XWORD protocolsubversion		 = 0;
			XWORD protocolsubversionerr  = 0;

			status = CMD_GetProtocolVersion(protocolversion, protocolsubversion, protocolsubversionerr);
			if(status) 
				{				

				}
			
		} 
	 else status = true;
	
	return status;                     
}

	

/*-------------------------------------------------------------------
//	TESTPROTOCOL::CMD_GetString
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/03/2016 12:32:44
//	
//	@return 			bool : 
//
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool TESTPROTOCOL::CMD_GetString(XSTRING& string)
{
	DIOPROTOCOL_COMMAND* cmd = GetCommand(TESTPROTOCOL_CMDTYPE_GETSTRING);
	if(!cmd) return false;

	XBUFFER	xbuffer;
	bool		result = false;
		
 	if(Command(cmd->GetType(), xbuffer)) 
		{	
			result = xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, &string);			
		} 
		
	SendEventCommand(this, DIOPROTOCOL_DIRECTION_GET, cmd->GetType(), result, NULL, 0);
	
	return result;
}



/*-------------------------------------------------------------------
//	TESTPROTOCOL::RCV_GetString
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/03/2016 12:34:14
//	
//	@return 			int : 
//
//  @param				protocol : 
//  @param				cmd : 
//  @param				xbuffer : 
//  @param				rID : 
//  @param				param : 
*/
/*-----------------------------------------------------------------*/
int TESTPROTOCOL::RCV_GetString(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
	TESTPROTOCOLAPPLICATIONDATA*  applicationdata = 	(TESTPROTOCOLAPPLICATIONDATA*)protocol->GetApplicationData();
 	if(!applicationdata) return DIOPROTOCOL_ERRORRECEIVED;

	XSTRING string;

	applicationdata->GetString(string);

	xbuffer.Delete();
	xbuffer.AddWithMask(cmd->GetSOUTMask(), &string);						

	return xbuffer.GetSize();
}




/*-------------------------------------------------------------------
//	TESTPROTOCOL::CMD_SendString
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/03/2016 12:41:45
//	
//	@return 			bool : 
//
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool TESTPROTOCOL::CMD_SendString(XSTRING& string)
{
	DIOPROTOCOL_COMMAND* cmd = GetCommand(TESTPROTOCOL_CMDTYPE_SENDSTRING);
	if(!cmd) return false;

	XBUFFER	xbuffer;
	bool		result = false;
	
	xbuffer.AddWithMask(cmd->GetSINMask(), &string);

	if(Command(cmd->GetType(), xbuffer)) 
		{	
			result = true;
		} 
		
	SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result, NULL, 0);
	
	return result;
}




/*-------------------------------------------------------------------
//	TESTPROTOCOL::RCV_SendString
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/03/2016 12:42:44
//	
//	@return 			int : 
//
//  @param				protocol : 
//  @param				cmd : 
//  @param				xbuffer : 
//  @param				rID : 
//  @param				param : 
*/
/*-----------------------------------------------------------------*/
int TESTPROTOCOL::RCV_SendString(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
	TESTPROTOCOLAPPLICATIONDATA*  applicationdata = 	(TESTPROTOCOLAPPLICATIONDATA*)protocol->GetApplicationData();
 	if(!applicationdata) return DIOPROTOCOL_ERRORRECEIVED;

	XSTRING string;
	
	xbuffer.GetWithMask(cmd->GetSINMask(), 0, &string);	

	applicationdata->SetString(string);		
	
	return 0;
}





/*-------------------------------------------------------------------
//	TESTPROTOCOL::CMD_GetVariant
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/02/2016 16:17:37
//	
//	@return 			bool : 
//
//  @param				variableID : 
//  @param				variant : 
*/
/*-----------------------------------------------------------------*/
bool TESTPROTOCOL::CMD_GetVariant(XDWORD variableID, XVARIANT* variant)
{
	if(!variant) return false;

	DIOPROTOCOL_COMMAND* cmd = GetCommand(TESTPROTOCOL_CMDTYPE_GETVARIANT);
	if(!cmd) return false;

	XBUFFER	xbuffer;
	bool		result = false;
	
	xbuffer.AddWithMask(cmd->GetSINMask(), variableID);

	if(Command(cmd->GetType(), xbuffer)) 
		{	
			result = xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, variant);			
		} 
		
	SendEventCommand(this, DIOPROTOCOL_DIRECTION_GET, cmd->GetType(), result, NULL, 0);
	
	return result;
}





/*-------------------------------------------------------------------
//	TESTPROTOCOL::RCV_GetVariant
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/02/2016 16:17:41
//	
//	@return 			int : 
//
//  @param				protocol : 
//  @param				cmd : 
//  @param				buffer : 
//  @param				size : 
//  @param				rID : 
//  @param				param : 
*/
/*-----------------------------------------------------------------*/
int	TESTPROTOCOL::RCV_GetVariant(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID,XDWORD& param)
{
	TESTPROTOCOLAPPLICATIONDATA*  applicationdata = 	(TESTPROTOCOLAPPLICATIONDATA*)protocol->GetApplicationData();
 	if(!applicationdata) return DIOPROTOCOL_ERRORRECEIVED;

	XDWORD variantID	 = 0;
	bool	 status;
	
	status = xbuffer.GetWithMask(cmd->GetSINMask(), 0, &variantID);

	xbuffer.Delete();

	if(status)
	  {
			XVARIANT* variant = applicationdata->GetData(variantID);
			if(variant)
				{
					xbuffer.AddWithMask(cmd->GetSOUTMask(), variant);						
				}
		}	

	return xbuffer.GetSize();
}




/*-------------------------------------------------------------------
//	PROTOCOL::CMD_SendVariant
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/02/2016 16:17:37
//	
//	@return 			bool : 
//
//  @param				variableID : 
//  @param				variant : 
*/
/*-----------------------------------------------------------------*/
bool TESTPROTOCOL::CMD_SendVariant(XDWORD variableID, XVARIANT* variant)
{
	if(!variant) return false;

	DIOPROTOCOL_COMMAND* cmd = GetCommand(TESTPROTOCOL_CMDTYPE_SENDVARIANT);
	if(!cmd) return false;

	XBUFFER	xbuffer;
	bool		result = false;
	
	xbuffer.AddWithMask(cmd->GetSINMask(), variableID, variant);

	if(Command(cmd->GetType(), xbuffer)) 
		{	
			//result = true;

			XBYTE _result = 0;
			result = xbuffer.GetWithMask(cmd->GetSOUTMask(), 0, (XBYTE*)&_result);				
		} 
		
	SendEventCommand(this, DIOPROTOCOL_DIRECTION_SEND, cmd->GetType(), result, NULL, 0);

	return result;
}





/*-------------------------------------------------------------------
//	TESTPROTOCOL::RCV_SendVariant
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			04/02/2016 16:17:41
//	
//	@return 			int : 
//
//  @param				protocol : 
//  @param				cmd : 
//  @param				buffer : 
//  @param				size : 
//  @param				rID : 
//  @param				param : 
*/
/*-----------------------------------------------------------------*/
int	TESTPROTOCOL::RCV_SendVariant(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param)
{
	TESTPROTOCOLAPPLICATIONDATA*  applicationdata = 	(TESTPROTOCOLAPPLICATIONDATA*)protocol->GetApplicationData();
 	if(!applicationdata) return DIOPROTOCOL_ERRORRECEIVED;

	XDWORD		variantID	 = 0;
	XVARIANT  variant;
	bool			status;
	XBYTE     result     = 0;
	
	status = xbuffer.GetWithMask(cmd->GetSINMask(), 0, &variantID, &variant);	 
	if(status)
	  {						
			status = applicationdata->SetData(variantID, variant);						
			if(status)
				{					
					result = 1;
				}
		}

	xbuffer.Delete();			
	return xbuffer.AddWithMask(cmd->GetSOUTMask(), result);
}


