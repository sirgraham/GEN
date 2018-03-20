//------------------------------------------------------------------------------------------
//	XWINDOWSFILE.H
//	
/**	
// \class 
//   
//  WINDOWS file class
//   
//	@author	 Abraham J. Velez
//	@version 13/03/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XWINDOWSFILE_H_
#define _XWINDOWSFILE_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XBase.h"
#include "XFile.h"
#include "XPath.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

	
//---- CLASS -------------------------------------------------------------------------------

class CIPHER;

class XWINDOWSFILE : public XFILE
{
	public:
											XWINDOWSFILE					();											
		virtual					 ~XWINDOWSFILE					();		

		bool							Exist									(XCHAR* xpath);

		bool							Open									(XCHAR* xpath, bool readonly = true);
		bool							Create								(XCHAR* xpath);
		
		bool							SetSize								(int size);

		bool							GetPosition						(int& position);
		bool							SetPosition						(int position);
		
		bool							Read									(XBYTE* buffer, int size , CIPHER* cipher = NULL);
		bool							Read									(XBYTE* buffer, int* size, CIPHER* cipher = NULL);

		bool							Write				 					(XBYTE* buffer, int size , CIPHER* cipher = NULL);
		
		bool							Flush									();

		bool							Close									();

		bool							Erase									(XCHAR* xpath, bool overwrite = false);
		bool							Rename								(XCHAR* xpathold, XCHAR* xpathnew);

		FILE*							GetFileStructHandle		()																						{ return filehandle;    };


	protected:

		bool							ActualizeSize					();
						
	private:

		void							Clean									()
											{
												filehandle  = NULL;
											}

		bool							ExtendedOpen				(XCHAR* xpath, XCHAR* mode);
		
		FILE*							filehandle;
};	
	

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

