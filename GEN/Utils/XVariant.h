/*------------------------------------------------------------------------------------------
//	XVARIANT.H
*/	
/**	
// \file 
//   
//  generic variant class
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 03/02/2016 15:51:59
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _XVARIANT_H_
#define _XVARIANT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XDebug.h"
#include "XBase.h"
#include "XString.h"
#include "XDateTime.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4522)
#endif

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum XVARIANT_TYPE
{
	XVARIANT_TYPE_NULL												= 0			,
	XVARIANT_TYPE_SERIALIZABLE												,
	XVARIANT_TYPE_INTEGER															,
	XVARIANT_TYPE_CHAR																,
	XVARIANT_TYPE_XCHAR																,
	XVARIANT_TYPE_FLOAT																,
	XVARIANT_TYPE_STRING															,
	XVARIANT_TYPE_DATE																,
	XVARIANT_TYPE_TIME																,
	XVARIANT_TYPE_DATETIME
};
	
/*---- CLASS -----------------------------------------------------------------------------*/

class XSERIALIZABLE;
class XFACTORY;

class XVARIANT
{
	public:
																		XVARIANT								()
																		{
																			Clean();																			
																		}

																		XVARIANT								(const XVARIANT& variant);
		virtual												 ~XVARIANT								();
															
		
		const XVARIANT&									operator =							(int integer);
		const XVARIANT&									operator =							(XDWORD integer);
		const XVARIANT&									operator =							(XQWORD integer);				
		const XVARIANT&									operator =							(float value);
		const XVARIANT&									operator =							(XCHAR value);
		const XVARIANT&									operator =							(XCHAR* string);
		const XVARIANT&									operator =							(XSTRING& string);
		const XVARIANT&									operator =							(char* string);
		const XVARIANT&									operator =							(char character);
		const XVARIANT&									operator =							(const XVARIANT& variant);
		const XVARIANT&									operator =							(const XSERIALIZABLE& serializable);
		const XVARIANT&									operator =							(XDATETIME datetime);		
		
		operator												int											();
		operator												XDWORD									();
		operator												XQWORD									();	
		operator												float										();	
		operator												XCHAR										();	
		operator												XCHAR*									();
		operator												char										();			
		operator												const XCHAR*						();
		operator												XDATETIME								();


		void														GetSerializable					(XSERIALIZABLE& serializable);
		void														GetSerializable					(XSERIALIZABLE* serializable);
		bool														GetDataFromString				(XCHAR* string);	

		XVARIANT_TYPE										GetType									()																							{ return type;											}
		void														SetType									(XVARIANT_TYPE type)														{ this->type=type;									}
		virtual XDWORD									GetSize									()																							{ return size;											}
		virtual void*										GetData									()																							{ return (void*)data;								}

		virtual bool										Set                     (XVARIANT_TYPE type=XVARIANT_TYPE_NULL, void* data=NULL, XDWORD size=0);

		virtual bool										ToString								(XSTRING& to);
				

		bool														IsNull									()																							{ return (this->data==NULL);				}

		#ifdef XDEBUG
		void														PrintDebug							()																							
																		{
																			switch(this->type)
																				{
																					case XVARIANT_TYPE_NULL					:	 XDEBUG_PRINTCOLOR(XDEBUG_COLORINFO,__L("NULL"));											break;
																					case XVARIANT_TYPE_SERIALIZABLE	:  XDEBUG_PRINTCOLOR(XDEBUG_COLORINFO,__L("SERIALIZABLE"));							break;
																					case XVARIANT_TYPE_INTEGER			:	 XDEBUG_PRINTCOLOR(XDEBUG_COLORINFO,__L("INTEGER %d"),(int)*this);		break;
																					case XVARIANT_TYPE_XCHAR				:	 XDEBUG_PRINTCOLOR(XDEBUG_COLORINFO,__L("XCHAR %c"),(XCHAR)*this);		break;
																					case XVARIANT_TYPE_FLOAT				:	 XDEBUG_PRINTCOLOR(XDEBUG_COLORINFO,__L("FLOAT %f"),(float)*this);		break;
																					case XVARIANT_TYPE_STRING				:	 XDEBUG_PRINTCOLOR(XDEBUG_COLORINFO,__L("STRING %s"),(XCHAR*)*this);	break;
																				}
																		}
		#endif


	protected:
		
		virtual bool										Destroy									();

		XVARIANT_TYPE										type;
		XDWORD													size;
		void*														data;
		
	private:

			void													Clean                   ()
																		{																			
																			type			= XVARIANT_TYPE_NULL;
																			size			= 0;
																			data			= NULL;																																		
																		}
};


	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif

