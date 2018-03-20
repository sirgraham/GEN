//------------------------------------------------------------------------------------------
//	XLINUXFILE.H
//	
/**	
// \class 
//   
//  LINUX file class
//   
//	@author	 Abraham J. Velez
//	@version 13/03/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XLINUXFILE_H_
#define _XLINUXFILE_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XBase.h"
#include "XFile.h"
#include "XPath.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

	
//---- CLASS -------------------------------------------------------------------------------

class CIPHER;
class XLINUXFACTORY;

class XLINUXFILE : public XFILE
{
	public:
											XLINUXFILE						();											
		virtual					 ~XLINUXFILE						();		

		bool							Exist									(XCHAR* xpath);

		bool							Open								  (XCHAR* xpath, bool readonly = true);
		bool							Create							  (XCHAR* xpath);
		
		bool							SetSize							  (int size);

		bool							GetPosition					  (int& position);
		bool							SetPosition						(int position);
		
		bool							Read									(XBYTE* buffer, int size , CIPHER* cipher = NULL);
		bool							Read									(XBYTE* buffer, int* size, CIPHER* cipher = NULL);

		bool							Write				 					(XBYTE* buffer, int size , CIPHER* cipher = NULL);
		
		bool							Flush									();

		bool							Close									();

		bool							Erase									(XCHAR* xpath, bool overwrite = false);
		bool							Rename								(XCHAR* xpathold, XCHAR* xpathnew);

		FILE*							GetFileStructHandle		()																						{ return filehandle;    };

		virtual void			ChangeAttributes			(XCHAR* attributes);

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

