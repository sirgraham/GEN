/*------------------------------------------------------------------------------------------
//	DIOPING.H
*/	
/**	
// \class 
//   
//  Data IO Ping class
//   
//	@author	 Abraham J. Velez
//	@version 30/08/2012 9:13:13
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOPING_H_
#define _DIOPING_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"
#include "XVector.h"
#include "XEvent.h"
#include "XSubject.h"

#include "DIOURL.h"
#include "DIOStreamICMPConfig.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum DIOPINGXEVENTTYPE
{
	DIOPINGXEVENTTYPE_UNKNOWN		   						= XEVENTTYPE_PING				,	
	DIOPINGXEVENTTYPE_DOPING																					,	
};


#define DIOPING_DEFAULTNRETRYS							3				
#define DIOPING_DEFAULTTIMEBETWEENCHECKS    500				// Milliseconds

#define DIOPING_ICMP_ECHOREPLY							0
#define DIOPING_ICMP_ECHOREQ								8
#define DIOPING_REQ_DATASIZE								32				// Echo Request Data size



typedef struct 
{			
	XBYTE s_b1;
	XBYTE s_b2;
	XBYTE s_b3;
	XBYTE s_b4; 
		
} DIOPING_ADDR_BYTE;



typedef struct 			
{
	XWORD s_w1;
	XWORD s_w2; 

} DIOPING_ADDR_WORD;



typedef struct 
{
	union 
		{		    
			DIOPING_ADDR_BYTE		addrbyte;
			DIOPING_ADDR_WORD		addrword;
			XDWORD							addr;

		} INADDR_UNION;

} DIOPING_INADDR;




//--------------------------------------------------------
// IP Header -- RFC 791
typedef struct 
{
	XBYTE														VIHL;			     // Version and IHL
	XBYTE														typeservice;	 // Type Of Service
	short														fulllen;			 // Total Length
	short														ID;				     // Identification
	short													  flagoff;		   // Flags and Fragment Offset
	XBYTE													  TTL;			     // Time To Live
	XBYTE														protocol;		   // Protocol
	XWORD														checksum;		   // Checksum
	DIOPING_INADDR									iasource;	     // Internet Address - Source
	DIOPING_INADDR									iatarget;	     // Internet Address - Destination

} DIOPING_IPHDR;



//--------------------------------------------------------
// ICMP Header - RFC 792
typedef struct 
{
	XBYTE														type;			     // Type
	XBYTE         									code;			     // Code
	XWORD														checksum;		   // Checksum
	XWORD														ID;				     // Identification
	XWORD														seq;			     // Sequence
	char														data;			     // Data

} DIOPING_ICMPHDR;



//--------------------------------------------------------
// ICMP Echo Request
typedef struct 
{ 
	DIOPING_ICMPHDR									icmphdr;
	XDWORD                          applicationID;
	XDWORD													time;
	char														cdata[DIOPING_REQ_DATASIZE];

} DIOPING_ECHOREQUEST;



//--------------------------------------------------------
// ICMP Echo Reply
typedef struct
{
	DIOPING_IPHDR										iphdr;
	DIOPING_ECHOREQUEST							echorequest;
//char														cfiller[256];

} DIOPING_ECHOREPLY;




/*---- CLASS -----------------------------------------------------------------------------*/
	
class XFACTORY;
class XTIMER;
class XPUBLISHER;
class DIOFACTORY;
class DIOPINGREPLY;
class DIOSTREAMICMPCONFIG;
class DIOSTREAMICMP;



class DIOPINGXEVENT : public XEVENT
{
	public:
																	DIOPINGXEVENT								(XSUBJECT* subject, XDWORD type = DIOPINGXEVENTTYPE_UNKNOWN, XDWORD family = XEVENTTYPE_PING) : XEVENT(subject, type, family)
																	{
																		Clean();
																	}

		virtual			 							   ~DIOPINGXEVENT								()																{	Clean();																												}

		DIOPINGREPLY*									GetPingReply								()																{ return pingreply;																								}
		void													SetPingReply	 							(DIOPINGREPLY* pingreply)					{ this->pingreply = pingreply;																		}
		
	private:

		void													Clean												()
																	{
																		pingreply = NULL;		
																	}
		
		DIOPINGREPLY*									pingreply;
};	




class DIOPINGREPLY
{
	public:
																	DIOPINGREPLY	       				()                                { Clean();                                                      }    
		virtual											 ~DIOPINGREPLY	   						()																{ Clean();                                                      }  

		XDWORD                        GetSequence									()																{ return sequence;																							}
		void		                      SetSequence									(XDWORD sequence)									{ this->sequence	= sequence;                                   }							

		XSTRING*                      GetFromIP										()																{ return &fromIP;                                               }
		
		XDWORD                        GetSizeRequest							()																{ return sizerequest;                                           }
		void													SetSizeRequest							(XDWORD sizerequest)							{ this->sizerequest = sizerequest;                              } 

		XDWORD	                      GetTimeElapsed							()																{ return timeelapsed;                                           }
		void   	                      SetTimeElapsed							(XDWORD timeelapsed)						  { this->timeelapsed = timeelapsed;                              }

		XBYTE                         GetTTL											()																{ return TTL;                                                   }	
		void                          SetTTL											(XBYTE TTL)												{ this->TTL = TTL;                                              }

  private:

    void													Clean												()
																	{
																		sequence			= 0;

																		fromIP.Empty();

																		sizerequest		= 0;
																		timeelapsed		= 0;
																		TTL						= 0;
																	}
		
		XDWORD                        sequence;
    XSTRING                       fromIP;
    XDWORD                        sizerequest;
    XDWORD	                      timeelapsed;
    XBYTE                         TTL;
};




class DIOPING :  public XSUBJECT
{
	public:

																	DIOPING       							();                                  
		virtual											 ~DIOPING   									();

		DIOURL*												GetTarget                   ()																{ return urltarget;                                            }

    bool                          Set                         (XCHAR* urltarget , XCHAR* IPLocal = NULL);
    bool                          Set                         (DIOURL& urltarget, XCHAR* IPLocal = NULL);

		virtual bool									Do				                  (XDWORD nretries = DIOPING_DEFAULTNRETRYS, XDWORD timebetweenchecks = DIOPING_DEFAULTTIMEBETWEENCHECKS, bool exitfirstgoodreply = false);	
    
		
    XVECTOR<DIOPINGREPLY*>*				GetReplys										();
		bool                          WasConnected                ();  
    int                           CalculateMeanTime           ();  
		
		bool                          DeleteAllReplys             (); 
   		
	protected: 
		bool                          AddReply                    (XWORD sequence, XCHAR* IPfrom, XDWORD sizerequest, XDWORD elapsed, XBYTE TTL);
		XWORD													CalculeCheckSum							(XWORD *addr, int len); 		

		XTIMER*                       xtimer;

		XDWORD                        applicationID;
		
    DIOURL*                       urltarget;    
    XSTRING                       IPLocal; 
		
		XVECTOR<DIOPINGREPLY*>		    replys;
		XMUTEX*												xmutexreplys;

		XDWORD												nretries;
		XDWORD												timebetweenchecks;		

	private: 

		void													Clean												()
																	{
																		xtimer							= NULL;

																		urltarget						= NULL;

																		applicationID				= 0;
																		
																		IPLocal.Empty();

																		xmutexreplys				= NULL;

																		nretries						= 0;
																		timebetweenchecks		= 0;																		
																	}   
};


	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

