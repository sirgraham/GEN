/*------------------------------------------------------------------------------------------
//	DBSQLVARIANT.H
*/	
/**	
// \class 
//   
//  Variant type
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 12/08/2015 13:48:07
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	

#ifdef DBSQL_ACTIVE

#ifndef _DBSQLVARIANT_H_
#define _DBSQLVARIANT_H_

/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XDebug.h"

#include "XVariant.h"
#include "DBSQLBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum DBSQLVARIANT_TYPE
{
	DBSQLVARIANT_TYPE_NULL												= XVARIANT_TYPE_NULL		,
	DBSQLVARIANT_TYPE_INTEGER											= XVARIANT_TYPE_INTEGER	,
	DBSQLVARIANT_TYPE_FLOAT												= XVARIANT_TYPE_FLOAT		,
	DBSQLVARIANT_TYPE_STRING											= XVARIANT_TYPE_STRING	,
	DBSQLVARIANT_TYPE_DATE																								,
	DBSQLVARIANT_TYPE_TIME																								,
	DBSQLVARIANT_TYPE_DATETIME
};

enum DBSQLVARIANT_FLAGS
{
	DBSQLVARIANT_FLAGS_NONE												= 0			,
	DBSQLVARIANT_FLAGS_NOT_NULL										= 1<<1	,
	DBSQLVARIANT_FLAGS_PRIMARY_KEY								= 1<<2	,
	DBSQLVARIANT_FLAGS_UNIQUE_KEY_FLAG						= 1<<3	,
	DBSQLVARIANT_FLAGS_UNIQUE_MULTIPLE_KEY_FLAG		= 1<<4	,
	DBSQLVARIANT_FLAGS_UNSIGNED_FLAG							= 1<<5	,
	DBSQLVARIANT_FLAGS_ZEROFILL_FLAG							= 1<<6	,
	DBSQLVARIANT_FLAGS_BINARY_FLAG								= 1<<7	,
	DBSQLVARIANT_FLAGS_AUTO_INCREMENT_FLAG				= 1<<8	,
	DBSQLVARIANT_FLAGS_ENUM_FLAG									= 1<<9	,
	DBSQLVARIANT_FLAGS_SET_FLAG										= 1<<10	,
	DBSQLVARIANT_FLAGS_BLOB_FLAG									= 1<<11	,
	DBSQLVARIANT_FLAGS_TIMESTAMP_FLAG							= 1<<12	,
	DBSQLVARIANT_FLAGS_NUM_FLAG										= 1<<13	,
	DBSQLVARIANT_FLAGS_NO_DEFAULT_VALUE_FLAG			= 1<<14
};


#define  DBSQLROWHEADER				DBSQLVECTOR<DBSQLSTRING*>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4522)
#endif

/*---- CLASS -----------------------------------------------------------------------------*/

class DBSQLVARIANT : public XVARIANT
{
	public:
																		DBSQLVARIANT						() : XVARIANT()				
																		{ 
																			Clean();													
																		}
																
																		DBSQLVARIANT						(const DBSQLVARIANT& variant);
		virtual												 ~DBSQLVARIANT						();
																		
	
		using XVARIANT::operator=;
		
		DBSQLVARIANT_FLAGS*							GetFlags								()																							{ return &flags;										}
		void														SetFlags								(DBSQLVARIANT_FLAGS flags)											{ this->flags = flags;							}

	protected:
		
		DBSQLVARIANT_FLAGS							flags;

	private:

			void													Clean                   ()
																		{																			
																			flags		= DBSQLVARIANT_FLAGS_NONE;
																			size    = 0;																		
																		}
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

//----------------------------------DBSQLROW -------------------------------------------------------

class DBSQLROW							
{
public:
									DBSQLROW	()
									{
										
									}

									~DBSQLROW	();		

		DBSQLVARIANT	Get				(int index);
		bool					Add				(DBSQLVARIANT* v);
		void					Clear			();

protected:

		DBSQLVECTOR<DBSQLVARIANT*> row;
};
	
#endif

#endif