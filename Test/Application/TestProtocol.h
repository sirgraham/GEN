/*------------------------------------------------------------------------------------------
//	TESTPROTOCOL.H
*/	
/**	
// \class 
//   
//  Test Protocol
//   
//	@author	 Abraham J. Velez
//	@version 08/02/2012 23:26:46
*/	
/*	GEN (C) Copyright  (All right reserved).			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTPROTOCOL_H_
#define _TESTPROTOCOL_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "DIOProtocol.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
enum TESTPROTOCOL_CMDTYPE
{	
	TESTPROTOCOL_CMDTYPE_GETSTRING								= DIOPROTOCOL_CMDTYPE_END , 
	TESTPROTOCOL_CMDTYPE_SENDSTRING																					,
	TESTPROTOCOL_CMDTYPE_GETVARIANT																					,
	TESTPROTOCOL_CMDTYPE_SENDVARIANT																				,

	TESTPROTOCOL_CMDTYPE_END
};

	
/*---- CLASS -----------------------------------------------------------------------------*/
	
class TESTPROTOCOL : public DIOPROTOCOL
{
	public:	
															TESTPROTOCOL							(DIOSTREAM* diostream);
		 virtual								 ~TESTPROTOCOL							();

		 bool											IniProtocolCommands				(bool ismaster);

		 bool											CMD_GetString							(XSTRING& string);
		 static int								RCV_GetString							(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);

		 bool											CMD_SendString						(XSTRING& string);
		 static int								RCV_SendString						(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);
		  		 		 
		 bool											CMD_GetVariant						(XDWORD variableID, XVARIANT* variant);
		 bool											CMD_GetVariant						(XDWORD variableID, XVARIANT& variant)				{ return CMD_GetVariant(variableID, &variant);								}
		 static int								RCV_GetVariant						(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);
		
		 bool											CMD_SendVariant						(XDWORD variableID, XVARIANT* variant);
		 bool											CMD_SendVariant						(XDWORD variableID, XVARIANT& variant)				{ return CMD_SendVariant(variableID, &variant);								}
		 static int								RCV_SendVariant						(DIOPROTOCOL* protocol, DIOPROTOCOL_COMMAND* cmd, XBUFFER& xbuffer, XDWORD rID, XDWORD& param);		

	private:

		 void											Clean											()
															{

															}
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

