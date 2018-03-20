//------------------------------------------------------------------------------------------
//	XFILECSV.H
//	
/**	
// \class 
//   
//  File CSV Functions
//   
//	@author	 Abraham J. Velez
//	@version 02/09/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XFILECSV_H_
#define _XFILECSV_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XString.h"	
#include "XFileTXT.h"	
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

	
//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;


class XFILECSV_RECORD
{	
	public:

																	XFILECSV_RECORD				  ();
		virtual											 ~XFILECSV_RECORD				  ();

		int														GetNElements					  ();
		bool													GetElement						  (int index, XSTRING& element);
		XCHAR*												GetElement						  (int index);

		bool													AddElement							(XSTRING& element);
		bool													AddElement							(XCHAR* element);
		
		bool													DeleteElement						(int index);
		bool													DeleteAllElements				();

		bool													Copy										(XFILECSV_RECORD* header);

	private:

		void													Clean										();

		XVECTOR<XSTRING*>							elements;
};



class XFILECSV : public XFILETXT
{
	public:												
																	XFILECSV								();
																	XFILECSV								(XPATH& xpath);
																	XFILECSV								(XFILE* file);
		virtual											 ~XFILECSV								();

		virtual bool									Open										(XPATH& xpath,bool readonly=true,bool haveheader = false);
		virtual bool									Close										(void);

		XCHAR													GetSeparator						();
		bool													SetSeparator						(XCHAR separator);

		bool													HaveHeader							();
		XFILECSV_RECORD*							GetHeader								();
		bool													SetHeader								(XFILECSV_RECORD* header);
		
		int														GetNRecords							();

		XFILECSV_RECORD*							ReadRecord							(XDWORD nrecord);
		bool													WriteRecord							(XDWORD nrecord,XFILECSV_RECORD* record);

		bool													AddRecord								(XFILECSV_RECORD* record);

		bool													DeleteRecord						(XDWORD nrecord);
		bool													DeleteAllRecords				();

	private:

		void													Clean										();

		bool													Serialization						(bool read,bool haveheader = false);

		bool													CreateRecordOfLine			(XSTRING& line,XFILECSV_RECORD* record);
		bool													CreateLineOfRecord			(XFILECSV_RECORD* record,XSTRING& line);
		
		XCHAR													separator;
		XFILECSV_RECORD								header;		
		XVECTOR<XFILECSV_RECORD*>			records;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

