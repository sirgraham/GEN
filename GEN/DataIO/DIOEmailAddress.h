/*------------------------------------------------------------------------------------------
//	DIOEMAILADDRESS.H
*/	
/**	
// \class 
//   
//  Data IO Email Address class
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 27/05/2014 13:57:43
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOEMAILADDRESS_H_
#define _DIOEMAILADDRESS_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XString.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
#define DIOEMAILADDRESS_SEPARATOR  __L("@")
	
/*---- CLASS -----------------------------------------------------------------------------*/

class DIOFACTORY;

class DIOEMAILADDRESS: public XSTRING
{
	public:

											DIOEMAILADDRESS														(): XSTRING()																				{ Clean();											}
		virtual				   ~DIOEMAILADDRESS														()																									{ Clean();											}

		                  DIOEMAILADDRESS														(int size) : XSTRING(size)													{ Clean();											}
											DIOEMAILADDRESS														(const char* url) : XSTRING(url)										{ Clean();											}
											DIOEMAILADDRESS														(const XCHAR* url) : XSTRING(url)		  							{ Clean();											}
											DIOEMAILADDRESS														(const XCHAR* url,int size) : XSTRING(url, size)		{ Clean();											}
											DIOEMAILADDRESS														(DIOEMAILADDRESS& url) : XSTRING(url)								{ Clean();											}
											DIOEMAILADDRESS														(XSTRING& url) : XSTRING(url)												{ Clean();											}	
											DIOEMAILADDRESS														(XWORD* url) : XSTRING(url)													{ Clean();											}
		
		using							XSTRING::operator =;
		using							XSTRING::operator +=;			
		using							XSTRING::operator <;			
		using							XSTRING::operator >;			
		using							XSTRING::operator <=;			
		using							XSTRING::operator >=;			
		using							XSTRING::operator ==;			
		using							XSTRING::operator !=;			
		using							XSTRING::operator [];	

		bool							GetUser																		(XSTRING& user);
		bool							GetDomain																	(XSTRING& domain);
		bool							IsValid																		();

	private:

		void							Clean																			()
											{

											}



};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

