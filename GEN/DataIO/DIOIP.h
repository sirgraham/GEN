/*------------------------------------------------------------------------------------------
//	DIOIP.H
*/	
/**	
// \class 
//   
//  Data IO IP class
//   
//	@author	 Abraham J. Velez
//	@version 30/08/2012 9:13:13
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOIP_H_
#define _DIOIP_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XBase.h"
#include "XString.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define  DIOIP_MAXIP								4

/*---- CLASS -----------------------------------------------------------------------------*/

class DIOURL;

class DIOIP
{
	public:

																	DIOIP																();
		virtual											 ~DIOIP																();


		XBYTE*												Get																	();	
		bool													GetXString													(XSTRING& IPString);
		bool													GetDIOURL														(DIOURL* url);
		
		bool													IsEmpty															();
		bool													IsLocal															();
		
		bool													Set 																(XBYTE* IP);
		bool													Set 																(XBYTE byte1, XBYTE byte2, XBYTE byte3, XBYTE byte4);
		bool													Set																	(XCHAR* IPchar);
		bool													Set																	(XSTRING& IPstring);
		bool													Set																	(char* IPchar);

		bool													Compare                             (DIOIP& IP);		

	protected:

		XBYTE													IP[DIOIP_MAXIP];
		
	private:

		void													Clean																();
};



	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

