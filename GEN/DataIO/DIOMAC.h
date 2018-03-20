/*------------------------------------------------------------------------------------------
//	DIOMAC.H
*/	
/**	
// \class 
//   
//  Data IO MAC class
//   
//	@author	 Abraham J. Velez
//	@version 30/08/2012 9:13:13
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOMAC_H_
#define _DIOMAC_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XBase.h"
#include "XString.h"

	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define  DIOMAC_MAXSIZE					6
#define  DIOMAC_MAXSIZESTR			18

/*---- CLASS -----------------------------------------------------------------------------*/


class DIOMAC
{
	public:

																	DIOMAC												();
		virtual											 ~DIOMAC												();

		XBYTE*												Get														();
		XDWORD												GetManufactured								();
		bool													GetXString										(XSTRING& MACstring);						
		XQWORD												GetLongNumber									();
		
		bool													Set														(XBYTE* MAC);
		bool													Set														(XSTRING& MAC);
		bool													Set														(char* MAC);

	protected:

		XBYTE													MAC[DIOMAC_MAXSIZE];		
    
	private:

		void													Clean													();		
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

