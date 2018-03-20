//------------------------------------------------------------------------------------------
//	XFILEHEX.H
//	
/**	
// \class 
//   
//  HEXP File functions (EPROM file format)
//   
//	@author	 Abraham J. Velez
//	@version 19/11/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XFILEHEX_H_
#define _XFILEHEX_H_
		
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XPath.h"	
#include "XFileTXT.h"	
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

	
//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;

class XFILEHEX : public XFILETXT
{
	public:
													XFILEHEX								();													
		virtual							 ~XFILEHEX								();

		XBYTE*								GetBinaryBuffer					(int& size);
		
	private:

		int										GetSizeOfLine						(XSTRING& line);

		XBYTE									GetBYTEofLine						(XCHAR* line);
		XWORD									GetWORDofLine						(XCHAR* line);
		

		void									Clean										();		

};

	
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

