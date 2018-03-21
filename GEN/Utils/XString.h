/*------------------------------------------------------------------------------------------
//	XSTRING.H
*/	
/**	
// \class 
//   
//  Unicode String Class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//	@version 23/11/2010 9:48:03
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _XSTRING_H_
#define _XSTRING_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <wchar.h>

#include "XBase.h"
#include "XVector.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

typedef wchar_t	XCHAR;

#define XSTRING_NOTFOUND			-1

enum XSTRINGCONTEXT
{
	XSTRINGCONTEXT_ATFIRST    = 0 ,
	XSTRINGCONTEXT_ATEND					,
	XSTRINGCONTEXT_ALLSTRING			,
};


#pragma pack(push, r1, 1)
typedef struct  
{
	XBYTE size;
	char	data[255];

}	SHORTSTRING;
#pragma pack(pop, r1)


#ifdef MICROCONTROLLER 
#define XSTRING_BLOCKMEM					8
#define XSTRING_MAXTEMPOSTR				128
#define XSTRING_STANDARDSIZE			64
#else
#define XSTRING_BLOCKMEM					64
#define XSTRING_MAXTEMPOSTR				1024
#define XSTRING_STANDARDSIZE			256
#endif

#define XSTRING_SIZETABLE64BITS		64
#define XSTRING_TABLE64BITS				__L("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/")

#define XSTRING_VALIDCHARSINT			__L("0123456789-+");
#define XSTRING_VALIDCHARSFLOAT		__L("0123456789-+.eE");

#define __L(x)					(XCHAR*)L##x
#define __C(x)					(XCHAR)L##x

#if (defined(LINUX) || defined(ANDROID)) 
	#define SPRINTF(str, ...)			      sprintf(str, ## __VA_ARGS__)
	#define SSCANF(str, ...)					  sscanf(str, ## __VA_ARGS__)
	#define UNICODESSCANF(str, ...)		  swscanf(str, ## __VA_ARGS__)
#endif

#if defined(BUILDER)
	#define SPRINTF		                  sprintf
	#define SSCANF					            sscanf
	#define UNICODESSCANF           	  swscanf
#endif

#ifdef WINDOWS
	#define SPRINTF(str, ...)					  sprintf_s(str, XSTRING_MAXTEMPOSTR-1, ## __VA_ARGS__)
	#define SSCANF(str, ...)						sscanf_s(str, ## __VA_ARGS__)
	#define UNICODESSCANF(str, ...)		  swscanf_s(str, ## __VA_ARGS__)
#endif

#ifdef MICROCONTROLLER
	#define SPRINTF(str, ...)			  		sprintf(str, ## __VA_ARGS__)
	#define SSCANF(str, ...)						sscanf(str, ## __VA_ARGS__)
	#define UNICODESSCANF(str, ...)		
#endif

#define XSTRING_CREATEOEM(xstring, variable)													char* variable = NULL;							\
																																			xstring.CreateOEM(variable);  

#define	XSTRING_DELETEOEM(variable)																		delete [] variable;									\
																																			variable = NULL;  

#define XSTRING_CREATENORMALIZE(xstring, buffernormalize, inverse)		XWORD* buffernormalize = NULL;			\
																																			xstring.CreateNormalize(buffernormalize, inverse);  

#define XSTRING_DELETENORMALIZE(buffernormalize)											delete [] buffernormalize;					\
																																			buffernormalize = NULL;  

#define XSTRING_COPYOEMMAXSIZE(xstring, charstr, maxsize)							{ XSTRING_CREATEOEM(xstring, charstrbase)																												\
																																				memcpy(charstr, charstrbase, (xstring.GetSize()>(maxsize-1)?(maxsize-1):xstring.GetSize()));	\
																																				XSTRING_DELETEOEM(charstrbase)																																\
																																			}

#define XSTRING_COPYOEMARRAYMAXSIZE(xstring, chararray, maxsize)			{ XSTRING_CREATEOEM(xstring, chararraybase)																										  \
																																				memcpy(chararray, chararraybase, (xstring.GetSize()>=maxsize?maxsize:xstring.GetSize()));			\
																																				XSTRING_DELETEOEM(chararraybase)																															\
																																			}
/*---- CLASS -----------------------------------------------------------------------------*/

class XBUFFER;

class XSTRING
{
	public:
													XSTRING													();
													XSTRING													(int size);
													XSTRING													(const char* string);
													XSTRING													(const XCHAR* string);										
													XSTRING													(const XCHAR* string,int size);										
													XSTRING													(const XSTRING& string);												
													XSTRING													(XWORD* string);
												
		virtual							 ~XSTRING													();

		XCHAR*								Get															() const;
		XDWORD								GetSize													() const;
		static XDWORD					GetSize													(const XCHAR* string);
		static XDWORD					GetSize													(XWORD* string);		

		bool									IsOEM														();

		bool									CreateOEM												(char*& text) const;
		bool									CreateNormalize									(XWORD*& text, bool inverse = false);
			
		bool									Set															(int size);
		bool									Set															(const char* string);
		bool									Set															(const XCHAR* string);
		bool									Set															(const XCHAR* string, int size);
		bool									Set															(const XSTRING& string);
		bool									Set															(XWORD* string);
		bool									Set															(const XCHAR character);
		static bool						Set															(XCHAR* str1, const XCHAR* str2);
		bool									Set															(const XBYTE* buffer, int size);
		bool									Set															(XBUFFER& buffer);
		bool									Set															(const XDWORD* buffer, int size);
		
		bool									Add															(const char* string);
		bool									Add															(const XCHAR* string);
		bool									Add															(XSTRING& string);
		bool									Add															(XCHAR character);
		static bool						Add															(XCHAR* str1, const XCHAR* str2);
		bool									Add															(const XBYTE* buffer, int size);
		bool									Add															(XBUFFER& buffer);
		

		void									operator =											(const char* string)								{ Set(string); };
		void									operator =											(const XCHAR* string)								{ Set(string); };
		void									operator =											(const XSTRING& string)							{ Set(string); };
		void									operator =											(XWORD* string)											{ Set(string); };
		void									operator =											(XBUFFER& string)										{ Set(string); };
		
		void									operator +=											(const char* string)								{ Add(string);		};
		void									operator +=											(const XCHAR* string)								{ Add(string);		};
		void									operator +=											(XSTRING& string)										{ Add(string);		};
		void									operator +=											(XCHAR character)										{ Add(character);	};

		bool									operator <											(const XSTRING& string) const 			{ return (bool)(Compare(string) == -1);	}
		bool									operator >											(XSTRING& string)										{ return (bool)(Compare(string) ==  1);	}
		bool									operator <=											(XSTRING& string) 									{ return (bool)(Compare(string) !=  1);	}
		bool									operator >=											(XSTRING& string)										{ return (bool)(Compare(string) != -1);	}
		bool									operator ==											(const XSTRING& string) const				{ return (bool)(Compare(string) ==  0);	}
		bool									operator !=											(XSTRING& string)										{ return (bool)(Compare(string) !=  0);	}

		XCHAR									operator []											(int position)											{ if(IsEmpty())						return 0;
																																																if(position<0)					return text[0];
																																																if(position>=(int)size)	return text[size-1];	

																																																return text[position];																																			
																																															}
		
		bool									FillChar												(XCHAR character = 0);
		bool									Empty														();
		bool									IsEmpty													();

		bool									HaveCharacters									();    
		bool									HaveOnlyNumbers									();

		bool									DeleteCharacters								(XDWORD index, XDWORD ncharacteres);
		bool									DeleteCharacter									(XCHAR character, XSTRINGCONTEXT context = XSTRINGCONTEXT_ALLSTRING);
		bool									DeleteNoCharacters							(XSTRINGCONTEXT context);
		int										DeleteNoCharacters							(XCHAR* needle=__L("\t\r\n"), int start=0, XSTRINGCONTEXT context= XSTRINGCONTEXT_ALLSTRING);
		bool									Fast_DeleteNoCharacters					(XSTRINGCONTEXT context = XSTRINGCONTEXT_ALLSTRING);

		bool									Character_IsAlpha								(XCHAR character) const;
		bool									Character_IsUpperCase						(XCHAR character);
		bool									Character_IsLowerCase						(XCHAR character);
		bool									Character_IsNumber							(XCHAR character);
		bool									Character_IsNOT 								(XCHAR character);
		XCHAR									Character_ToUpper								(XCHAR character) const;
		XCHAR									Character_ToLower								(XCHAR character) const;
		XCHAR									Character_GetFirst							();
		XCHAR									Character_GetLast								();
		bool									Character_Change								(XCHAR source, XCHAR target);

		bool									ToUpperCase											();
		bool									ToLowerCase											();
		
		int										Compare													(const XCHAR* string, bool ignorecase = false);		
		int										Compare													(const XCHAR* string, int csize, bool ignorecase= false);
		int										Compare													(const XSTRING& string, bool ignorecase = false) const;
		int										Compare													(const char* string, bool ignorecase = false);
		static int						Compare													(const XCHAR* string, const XCHAR* string2, bool ignorecase = false);

		int										Find														(const XCHAR* string, bool ignorecase, int startindex = 0);
		int										Find														(XSTRING& string, bool ignorecase, int startindex = 0);
		int										FindCharacter										(XCHAR character, XDWORD startindex = 0, bool endtostart = false);
    int                   FindDiferentCharacter						(XCHAR character, int startindex = 0, bool endtostart = false);
		XDWORD								CountCharacter									(XCHAR character, int startindex = 0, bool endtostart = false);

		int										GetNCharacters									(XCHAR character);

		bool									AreValidCharacters							(XCHAR* validcharacterslist);

		int										Copy     												(const XCHAR* startmark, const XCHAR* endmark, bool ignorecase , int addstartindex, XSTRING& string);
		int										Copy														(int startindex, const XCHAR* endmark, bool ignorecase, XSTRING& string);
		int										Copy														(const XCHAR* startmark, int endindex, bool ignorecase, int startindex, XSTRING& string);
		int										Copy     												(int startindex, int endindex, XSTRING& string);
		int										Copy														(int startindex, XSTRING& string);

		int										Replace													(XCHAR* findWhat, XCHAR* replaceBy);
		int										ReplaceFirst										(XCHAR* findWhat, XCHAR* replaceBy);
		bool									Insert													(XSTRING str,			XDWORD position);

		bool									ConvertFromInt									(int value,				const XCHAR* mask = NULL);
		bool									ConvertFromDWord								(XDWORD value,		const XCHAR* mask = NULL);
		bool									ConvertFromQWord								(XQWORD value,		const XCHAR* mask = NULL);
		bool									ConvertFromLongLong							(long long value, const XCHAR* mask = NULL);
		bool									ConvertFromFloat								(float value,			const XCHAR* mask = NULL);
		bool									ConvertFromDouble								(double value,		const XCHAR* mask = NULL);		
		bool									ConvertFromBoolean							(bool boolean,		bool uppercase = false);	
		bool									ConvertFromXBUFFER							(XBUFFER& buffer);		
		
		bool									ConvertHexFormatChars						();
		bool									ConvertToHexString							(XSTRING& string, bool uppercase = true);
		bool									ConvertHexStringToBuffer				(XBUFFER& xbuffer);
		bool									ConvertHexStringFromBuffer			(XBUFFER& xbuffer,  bool upercase = true);

		int										ConvertToInt										(int index = 0, const XCHAR* mask = NULL, bool checkvalidchars = true);
		XDWORD								ConvertToDWord			  					(int index = 0, const XCHAR* mask = NULL, bool checkvalidchars = true);
		XQWORD								ConvertToQWord									(int index = 0, const XCHAR* mask = NULL, bool checkvalidchars = true);
		float									ConvertToFloat									(int index = 0, const XCHAR* mask = NULL, bool checkvalidchars = true);
		double								ConvertToDouble									(int index = 0, const XCHAR* mask = NULL, bool checkvalidchars = true);
		bool									ConvertToXBUFFER								(XBUFFER& buffer);
		
		int										Fast_ConvertToInt								(int index = 0)
													{
														int	result	= 0;
														UNICODESSCANF(&text[index], __L("%d"), &result);
														return result;
													}

		XDWORD								Fast_ConvertToDWord				 			(int index = 0)
													{
														XDWORD result	= 0;
														UNICODESSCANF(&text[index], __L("%u"), &result);
														return result;
													}

		XQWORD								Fast_ConvertToQWord							(int index = 0)
													{
														XQWORD result	= 0;
														UNICODESSCANF(&text[index], __L("%lld"), &result);
														return result;
													}
		XQWORDSIG							Fast_ConvertToSQWord						(int index = 0)
													{
														XQWORDSIG result	= 0;
														UNICODESSCANF(&text[index], __L("%lld"), &result);
														return result;
													}
		float									Fast_ConvertToFloat							(int index = 0)
													{
														float result	= 0;
														UNICODESSCANF(&text[index], __L("%f"), &result);
														return result;
													}

		double								Fast_ConvertToDouble						(int index = 0)
													{
														double result	= 0;
														UNICODESSCANF(&text[index], __L("%lf"), &result);
														return result;
													}

		bool									Fast_AddCharacter								(XCHAR character)
													{
														if(!ReAllocBuffer(size+1)) return false;	

														memcpy(&text[size-1], &character, sizeof(XCHAR));

														return true;
													}

		bool									Fast_Empty()
													{
														if(!size) return true;

														memset(&text[0], 0, size*sizeof(XCHAR));

														size=0;
														return true;
													}
				
		bool									ConvertToBoolean								();	
		
		bool									ConvertFromUTF8									(XBYTE* data, int size);
		bool									ConvertFromUTF8									(XBUFFER& xbuffer);
		int										GetSizeConvertToUTF8						();
		bool									ConvertToUTF8										(XBYTE* data, int& size);
		bool									ConvertToUTF8										(XBUFFER& xbuffer);

		bool									ConvertToBase64									(XSTRING& string);		
		bool									ConvertFromBase64								(XSTRING& string);
				
		bool									ConvertToPascal									(SHORTSTRING& pascalstring);
		bool									ConvertFromPascal								(SHORTSTRING& pascalstring);
		
		bool									ConvertFromDoubleToSpanishText	(double value, bool withintegerpart, bool withdecimalpart, double& integerpart, double& decimalpart);

		bool									Swab														();
		
		bool									FormatArg												(const XCHAR* mask, va_list* arg, bool isspecialweb = false);

		bool									Format													(const XCHAR* mask, ...);
		bool									AddFormat												(const XCHAR* mask, ...);
		bool									UnFormat												(const XCHAR* mask, ...);
		bool									IsSpace													(XCHAR xchar);
		bool									IsDigit													(XCHAR xchar);		
		int										FindCharacterFromSet						(const XCHAR* set);
				
    bool									AdjustSize											(XDWORD size,bool ahead = false, XCHAR* characters = NULL, bool addstring = false);
		bool									AdjustSize											();		

		bool									BinaryCompare										(XSTRING& string);

		bool									Explode													(XCHAR character, XVECTOR<XSTRING*>* receive);
		

	protected: 

		XCHAR*								text;
		XDWORD								size;

		bool									ReAllocBuffer										(XDWORD size);
		bool									FreeBuffer											();	
		
		XCHAR									ConvertIndexBase64ToXCHAR				(int index);
		XBYTE									ConvertXCHARToBase64Index				(XCHAR character);	
		bool									ConvertStringWithMask						(XCHAR* mask, XCHAR* string, XCHAR* result);
		
		XDWORD								sizemem;

		int*									intvalue;
		double*								doublevalue;
		XCHAR*								xcharvalue;

		static XCHAR					table64bits[XSTRING_SIZETABLE64BITS];
		static bool						istable64bitsinit;

	private:

					void						Clean														()
													{
														text				= NULL;
														size				= 0;

														sizemem			= 0;

														intvalue		= NULL;
														doublevalue = NULL;
														xcharvalue	= NULL;

														if(!istable64bitsinit)
															{
																istable64bitsinit = true;
																memcpy(table64bits, (XCHAR*)XSTRING_TABLE64BITS, XSTRING_SIZETABLE64BITS * sizeof(XCHAR));
															}
													}
};
	


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	

#endif






