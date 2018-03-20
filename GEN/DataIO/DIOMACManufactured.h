/*------------------------------------------------------------------------------------------
//	DIOMACMANUFACTURED.H
*/	
/**	
// \class 
//   
//  Data IO MAC MANUFACTURED Device class
//   
//	@author	 Abraham J. Velez
//	@version 30/08/2012 9:13:13
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOMACMANUFACTURED_H_
#define _DIOMACMANUFACTURED_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XBase.h"
#include "XString.h"

#include "DIOMAC.h"
#include "DIOWebClient.h"
#include "DIOWebScraper.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOMACMANUFACTURED_URL						 __L("http://standards.ieee.org/develop/regauth/oui/")
#define DIOMACMANUFACTURED_URLNAMEFILE		 __L("oui.txt")

#define DIOMACMANUFACTURED_FILE_VERSION		 0x0100
#define DIOMACMANUFACTURED_FILE_IDSTRING 	 __L("[MAC Manufactured ID]")
#define DIOMACMANUFACTURED_FILE_NAME		 	 __L("macmanufacturedid.dat")

/*---- CLASS -----------------------------------------------------------------------------*/


class DIOMACMANUFACTURED 
{
	public:
																	DIOMACMANUFACTURED							();
		virtual											 ~DIOMACMANUFACTURED							();
		
		bool													Web_GetManufactured							(DIOMAC& MAC, XSTRING& manufactured);

		bool													File_GetManufactured						(XPATH& xpath, DIOMAC& MAC, XSTRING& manufactured);
		bool													File_GetManufacturedMACs				(XPATH& xpath, XSTRING& manufactured, XVECTOR<XDWORD>& MACs);

		bool													File_Download										(XBUFFER& xbuffer, int timeout = 10);
		bool													File_Convert										(XBUFFER& xbuffer, XPATH& xpath);
				
	private:

		void													Clean														()
																	{
	
																	}
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

