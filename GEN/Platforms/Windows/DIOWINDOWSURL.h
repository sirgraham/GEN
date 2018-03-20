/*------------------------------------------------------------------------------------------
//	DIOWINDOWSURL.H
*/	
/**	
// \class 
//   
//  Windows Data I/O URL class
//   
//	@author	 Abraham J. Velez
//	@version 13/02/2012 10:37:04
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOWINDOWSURL_H_
#define _DIOWINDOWSURL_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOURL.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	


/*---- CLASS -----------------------------------------------------------------------------*/


class DIOWINDOWSURL : public DIOURL
{
	public:	
															DIOWINDOWSURL											();
		virtual									 ~DIOWINDOWSURL											();

		                          DIOWINDOWSURL											(int size);
															DIOWINDOWSURL											(const char* url);
															DIOWINDOWSURL											(const XCHAR* url);										
															DIOWINDOWSURL											(const XCHAR* url,int size);										
															DIOWINDOWSURL											(DIOURL& url);												
															DIOWINDOWSURL											(XSTRING& url);												
															DIOWINDOWSURL											(XWORD* url);
				
		virtual	bool							ResolveURL												(XSTRING& IPstring);		
			
	private:

		void											Clean															();
		
};


	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif










