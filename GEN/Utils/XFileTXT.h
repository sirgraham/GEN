//------------------------------------------------------------------------------------------
//	XFILETXT.H
//	
/**	
// \class 
//   
//  TXT File Class
//   
//	@author	 Abraham J. Velez
//	@version 19/11/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XFILETXT_H_
#define _XFILETXT_H_
		
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"	
#include "XFile.h"	
#include "XString.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum XFILETXTFORMATCHAR
{
	XFILETXTFORMATCHAR_UNKNOWN							= 0 ,
	XFILETXTFORMATCHAR_ASCII							    ,
	XFILETXTFORMATCHAR_UTF8										,
	XFILETXTFORMATCHAR_UTF16_BE								,
	XFILETXTFORMATCHAR_UTF16_LE               
};


enum XFILETXTTYPELF
{
	XFILETXTTYPELF_UNKNOWN								 = 0 ,
	XFILETXTTYPELF_0A											   ,
	XFILETXTTYPELF_0D											   ,
	XFILETXTTYPELF_0A0D										   ,
	XFILETXTTYPELF_0D0A										   ,
};

#define XFILETXT_MAXBUFFER		  (256*1024)

#define XFILETXT_TOLASTLINE			-1

#ifdef LINUX
#define XFILETXTTYPELF_DEFAULT  XFILETXTTYPELF_0A
#else
#define XFILETXTTYPELF_DEFAULT  XFILETXTTYPELF_0D0A
#endif
	
//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XSTRING;
class XBUFFER;


class XFILETXT : public XFILECONTAINER 
{
	public:
													XFILETXT								();													
		virtual							 ~XFILETXT								();

		virtual bool					Open										(XPATH& xpath,bool readonly = true);	
		virtual bool					Create									(XPATH& xpath,XFILETXTFORMATCHAR formatchar = XFILETXTFORMATCHAR_ASCII ,XFILETXTTYPELF typeLF = XFILETXTTYPELF_DEFAULT);	
		virtual bool					IsOpen									();
		virtual bool					Close										(void);
		
		XFILETXTFORMATCHAR 		GetFormatChar						();		
		bool 									SetFormatChar						(XFILETXTFORMATCHAR formatchar);
		XFILETXTFORMATCHAR		GetFormatCharFromFile		(int* sizeBOM = NULL);		
		bool									CreateBOMFormatChar			(XFILETXTFORMATCHAR formatchar,XBYTE* BOM,int& sizeBOM);
		int										SizeOfCharacter         (XFILETXTFORMATCHAR formatchar)
													{														
														switch(formatchar)
															{
																case XFILETXTFORMATCHAR_UNKNOWN		:
																case XFILETXTFORMATCHAR_ASCII			:		
																case XFILETXTFORMATCHAR_UTF8			: return 1;

																case XFILETXTFORMATCHAR_UTF16_BE	:
																case XFILETXTFORMATCHAR_UTF16_LE	: return 2;

															}		

														return 0;
													}

		XFILETXTTYPELF				GetTypeLF								();    
		bool									SetTypeLF								(XFILETXTTYPELF typeLF);
		bool									CreateTypeLF						(XFILETXTFORMATCHAR formatchar,XFILETXTTYPELF typeLF,XBYTE* LF,int& sizeLF);
    bool                  GetLF                   (XBUFFER& lfdata); 
		XSTRING*              GetLF                   (); 
		
		int										GetNLines								();
		XSTRING*							GetLine									(int index);
		XCHAR*								GetLineText							(int index);

    bool                  GetAllInOneLine         (XSTRING& alllines , int start = 0, int end = XFILETXT_TOLASTLINE);
		bool                  GetAllInBuffer	        (XBUFFER& xbuffer, int start = 0, int end = XFILETXT_TOLASTLINE);

		bool								  ReadAllFile							();
		bool									WriteAllFile						();
		
		bool									DeleteFormatFile				();

		bool									AddLine									(XCHAR* line);
		bool									AddLine									(XSTRING& line);

		bool									GenerateLineFromBuffer	(XFILETXTFORMATCHAR formatchar, XBYTE* line, int sizeline, XSTRING& string);
		bool									AddLine									(XFILETXTFORMATCHAR type, XBYTE* line, int sizeline);

		bool									AddLineAlready					(XCHAR* line, int* sizeline = NULL, int* sizeLF = NULL);
		bool									AddLineAlready					(XSTRING& line, int* sizeline = NULL, int* sizeLF = NULL);	

    bool									InsertLine							(int index, XCHAR* line);
		bool									InsertLine							(int index, XSTRING& line);
	
		bool									DeleteLine							(int index);		
		bool									DeleteAllLines					();

		bool									AddBufferLines					(XFILETXTFORMATCHAR formatchar, XBUFFER& xbuffer);

		bool									GetSizeOfLine						(XFILETXTFORMATCHAR formatchar, XBYTE* buffer, XFILETXTTYPELF& typeLF, int& sizeLF, int& size, int maxsize);

		XVECTOR<XSTRING*>*		GetLines								()																{ return &lines;																}

		
		XFILETXTFORMATCHAR    formatchar;
		XFILETXTTYPELF				typeLF;
		XSTRING								LF;	
		XVECTOR<XSTRING*>			lines;

	private:

		void									Clean										();


};

	
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

