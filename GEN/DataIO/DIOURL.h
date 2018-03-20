/*------------------------------------------------------------------------------------------
//	DIOURL.H
*/	
/**	
// \class 
//   
//  Data I/O URL class
//   
//	@author	 Abraham J. Velez
//	@version 13/02/2012 10:37:04
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOURL_H_
#define _DIOURL_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XString.h"

#include "DIOIP.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
#define DIOURL_WEBURLID	__L("http://")
	
/*---- CLASS -----------------------------------------------------------------------------*/


class DIOURL : public XSTRING
{
	public:	
															DIOURL														(): XSTRING()																				{ Clean();											}
		virtual									 ~DIOURL														()																									{ Clean();											}

		                          DIOURL														(int size) : XSTRING(size)													{ Clean();											}
															DIOURL														(const char* url) : XSTRING(url)										{ Clean();											}
															DIOURL														(const XCHAR* url) : XSTRING(url)		  							{ Clean();											}
															DIOURL														(const XCHAR* url,int size) : XSTRING(url, size)		{ Clean();											}
															DIOURL														(DIOURL& url) : XSTRING(url)												{ Clean();											}
															DIOURL														(XSTRING& url) : XSTRING(url)												{ Clean();											}												
															DIOURL														(XWORD* url) : XSTRING(url)													{ Clean();											}
		
		using											XSTRING::operator =;
		using											XSTRING::operator +=;			
		using											XSTRING::operator <;			
		using											XSTRING::operator >;			
		using											XSTRING::operator <=;			
		using											XSTRING::operator >=;			
		using											XSTRING::operator ==;			
		using											XSTRING::operator !=;			
		using											XSTRING::operator [];						

		bool											GetHTTPServer											(XSTRING& server, XSTRING& login, XSTRING& password);
		bool											GetHTTPResource										(XSTRING& resource);
		
		bool											IsAURLResolved										();

		bool											ResolveURL												();
		virtual	bool							ResolveURL												(XSTRING& IP);
		bool											ResolveURL												(DIOIP& IP);

		bool											IsLocalAddress										();
			
		bool											Slash_Add													();
		bool											Slash_Normalize										();
		bool											Slash_Delete											();		

		bool											IsUnsafeChar											(XCHAR character);
		bool											EncodeUnsafeChar									(XCHAR character, XSTRING& encodechar);
		
		bool											EncodeUnsafeCharsFromString				(XSTRING& string);
		bool											EncodeUnsafeCharsFromString				(const char* string);
		bool											EncodeUnsafeCharsFromString				(const XCHAR* string);
		bool											EncodeUnsafeCharsFromBuffer				(XBUFFER& buffer);

		bool											EncodeUnsafeChars									();	

		bool											DecodeUnsafeCharsToString					(XSTRING& string);
		bool											DecodeUnsafeChars									();
				
		bool											Normalize													();

		bool											HaveHTTPID												();
		bool											AddHTTPID													();
		bool											DeleteHTTPID											();
			
	private:

		void											Clean															();
		
};


	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif










