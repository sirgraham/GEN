/*------------------------------------------------------------------------------------------
//	DIONTP.H
*/	
/**	
// \class 
//   
//  Data IO Network Time Protocol Class
//   
//	@author	 Abraham J. Velez
//	@version 09/02/2013 15:27:41
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIONTP_H_
#define _DIONTP_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOURL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIONTP_DEFAULTPORT 	123
#define DIONTP_JAN1ST1900  	2415021

/*---- CLASS -----------------------------------------------------------------------------*/
	
class XFACTORY;
class XTIMER;
class DIOFACTORY;
class DIOSTREAMUDPCONFIG;
class	DIOSTREAMUDP;


#pragma pack(push, r1, 1)
class DIOTIMEPACKET
{
	public:
	
		XDWORD												integer;
		XDWORD												fractional;
};
#pragma pack(pop, r1)


#pragma pack(push, r2, 1)
class DIONTPBASICINFO
{
	public:
	
		XBYTE													livnmode;
		XBYTE													stratum;
		char													poll;
		char													precision;
		long													rootdelay;
		long													rootdispersion;
		char													referenceID[4];
		DIOTIMEPACKET									referencetimestamp;
		DIOTIMEPACKET									originatetimestamp;
		DIOTIMEPACKET									receivetimestamp;
		DIOTIMEPACKET									transmittimestamp;
};
#pragma pack(pop, r2)


#pragma pack(push, r3, 1)
class DIONTPAUTHENTICATIONINFO
{
	public:

		XDWORD												keyID;
		XBYTE													messagedigest[16];
};
#pragma pack(pop, r3)


#pragma pack(push, r4, 1)
class DIONTPFULLPACKET
{
	public:

		DIONTPBASICINFO								basic;
		DIONTPAUTHENTICATIONINFO			authentication;
};
#pragma pack(pop, r4)


#pragma pack(push, r5, 1)
class DIONTPRESPONSE
{
	public:

		int														leapindicator;				// (0: no warning, 1: last minute in day has 61 seconds, 2: last minute has 59 seconds, 3: clock not synchronized)
		int														stratum;							// (0: unspecified or unavailable, 1: primary reference (e.g., radio clock),  2- 15: secondary reference (via NTP or SNTP), 16-255: reserved)
		XQWORD												originatetime;				// Time when the request was sent from the client to the SNTP server
		XQWORD										    receivetime;					// Time when the request was received by the server
		XQWORD										    transmittime;					// Time when the server sent the request back to the client
		XQWORD												destinationtime;			// Time when the reply was received by the client
		double												roundtripdelay;				// Round trip time in seconds
		double												localclockoffset;			// Local clock offset relative to the server
};
#pragma pack(pop, r5)



class DIONTP
{
	public:

																	DIONTP       									  		();                                  
		virtual											 ~DIONTP   														();

		bool													GetTimeResponse											(XCHAR*  urlntpserver, int timeout, bool hardwareuselittleendian, DIONTPRESPONSE& response);
		bool													GetTimeResponse											(DIOURL& urlntpserver, int timeout, bool hardwareuselittleendian, DIONTPRESPONSE& response);

		bool													GetTimeSeconds											(XCHAR*  urlntpserver, int timeout, bool hardwareuselittleendian, XQWORD& timeseconds);
		bool													GetTimeSeconds											(DIOURL& urlntpserver, int timeout, bool hardwareuselittleendian, XQWORD& timeseconds);

		bool													GetTime															(XCHAR*  urlntpserver, int timeout, bool hardwareuselittleendian, XDATETIME& xtime);
		bool													GetTime															(DIOURL& urlntpserver, int timeout, bool hardwareuselittleendian, XDATETIME& xtime);
       
	private:

		void													Clean																();

		XQWORD												ConvertTimePacket										(DIOTIMEPACKET& timepacket, bool hardwareuselittleendian);
   
    

		XTIMER*												xtimer;

		DIOSTREAMUDPCONFIG*						diostreamudpcfg;
		DIOSTREAMUDP*									diostreamudp;
};


	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

