//------------------------------------------------------------------------------------------
//	XANDROIDFILE.H
//	
/**	
// \class 
//   
//  ANDROID file class
//   
//	@author	 Abraham J. Velez
//	@version 13/03/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XANDROIDFILE_H_
#define _XANDROIDFILE_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XBase.h"
#include "XFile.h"
#include "XPath.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

	
//---- CLASS -------------------------------------------------------------------------------

class CIPHER;
class XANDROIDFACTORY;

class XANDROIDFILE : public XFILE
{
	public:
												XANDROIDFILE					();											
		virtual						 ~XANDROIDFILE					();		

		bool								Exist								  (XCHAR* xpath);

		bool								Open								  (XCHAR* xpath, bool readonly = true);
		bool								Create							  (XCHAR* xpath);
		
		bool								SetSize								(int size);

		bool								GetPosition						(int& position);
		bool								SetPosition						(int position);
		
		bool								Read								  (XBYTE* buffer, int size , CIPHER* cipher = NULL);
		bool								Read									(XBYTE* buffer, int* size, CIPHER* cipher = NULL);

		bool								Write				 					(XBYTE* buffer, int size , CIPHER* cipher = NULL);
		
		bool								Flush									();

		bool								Close									();

		bool								Erase									(XCHAR* xpath, bool overwrite = false);
		bool								Rename								(XCHAR* xpathold, XCHAR* xpathnew);

		FILE*								GetFileStructHandle		()																	{ return filehandle;    };


	protected:

		bool								ActualizeSize					();
						
	private:

		void								Clean									()
												{
													filehandle  = NULL;
												}

		bool								ExtendedOpen					(XCHAR* xpath, XCHAR* mode);
		
		XANDROIDFACTORY*		xfactory;
		FILE*								filehandle;
};	
	

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

