/*------------------------------------------------------------------------------------------
//	DIOSNMP.H
*/	
/**	
// \class 
//   
//  Data IO SNMP clases
//   
//	@author	 Abraham J. Velez
//	@version 03/09/2012 10:55:16
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOSNMP_H_
#define _DIOSNMP_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBER.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOSNMPBERTYPE_IPADDRESS								0x00
#define DIOSNMPBERTYPE_COUNTER32								0x01
#define DIOSNMPBERTYPE_GAUGE32									0x02
#define DIOSNMPBERTYPE_TIMETICKS								0x03
#define DIOSNMPBERTYPE_OPAQUE										0x04
#define DIOSNMPBERTYPE_NSAPADDRESS							0x05
#define DIOSNMPBERTYPE_COUNTER64								0x06	// available only in SNMPv2
#define DIOSNMPBERTYPE_UINTEGER32								0x07  // available only in SNMPv2

#define DIOSNMPBERTYPE_SEQUENCE									0x10	

#define DIOSNMPBERTYPE_GETRESQUESTPDU						0x00
#define DIOSNMPBERTYPE_GETNEXTRESQUESTPDU				0x01
#define DIOSNMPBERTYPE_GETRESPONSEPDU						0x02  // Response-PDU in SNMPv 2
#define DIOSNMPBERTYPE_SETREQUESTPDU						0x03
#define DIOSNMPBERTYPE_TRAPPDU									0x04  // obsolete in SNMPv 2
#define DIOSNMPBERTYPE_GETBULKREQUESTPDU				0x05  // added in SNMPv 2
#define DIOSNMPBERTYPE_INFORMRESQUESTPDU				0x06  // added in SNMPv 2
#define DIOSNMPBERTYPE_SNMPV2TRAPPDU						0x07	// added in SNMPv 2 	


/*---- CLASS -----------------------------------------------------------------------------*/
	
class XFACTORY;
class XPUBLISHER;
class DIOFACTORY;
class DIOSTREAMUDPCONFIG;
class DIOSTREAMUDP;


class DIOSNMPXBER :  public XBER
{ 
	public:
		         
			                        DIOSNMPXBER										();
		virtual									 ~DIOSNMPXBER										();

		bool											SetIPADDRESS									(XCHAR* IPstring);
		bool											SetTIMETICKS									(XDWORD ticks);

		bool											Sequence_AddTo								(XBER& newxber);
		bool											TrapData_AddTo								(XBER& newxber);
};




class DIOSNMPTRAP
{
	 public:
		                          DIOSNMPTRAP										();
		virtual									 ~DIOSNMPTRAP										();

		bool											Open													(XCHAR* IPtarget, int port, bool checkavailableNMS = false, XCHAR* localIP = NULL);
		bool											Send													(XCHAR* community, XCHAR* enterprise, XCHAR* agentip, int generictrap, int specifictrap, XDWORD timestamp, int nvar, ...);
		bool											Close													();

	 private:

		 void											Clean													()
															{
																diostreamudp = NULL;
															}

		 		 
		 DIOSTREAMUDPCONFIG*			diostreamudpcfg;
		 DIOSTREAMUDP*						diostreamudp;
};




/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

