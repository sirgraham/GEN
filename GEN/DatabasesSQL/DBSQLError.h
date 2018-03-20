/*------------------------------------------------------------------------------------------
//	DBSQLERROR.H
*/	
/**	
// \file
//   
//  Database Errors
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 10/08/2015 13:20:58
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifdef DBSQL_ACTIVE

#ifndef _DBSQLERROR_H_
#define _DBSQLERROR_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "DBSQLBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
enum DBSQLERROR_TYPE
{
	DBSQLERROR_TYPE_UNKNOWN						= -1	,
	DBSQLERROR_TYPE_NONE							=  0	,
	DBSQLERROR_TYPE_MEMORY_ERROR						,
	DBSQLERROR_TYPE_CONNECTION_ERROR			  ,
	DBSQLERROR_TYPE_STATEMENT_ERROR			    ,
	DBSQLERROR_TYPE_TRANSACTION_ERROR	
};
	
/*---- CLASS -----------------------------------------------------------------------------*/

class DBSQLERROR
{
	public:
													DBSQLERROR						() 
													{
														Clean();
													}
														
													DBSQLERROR						(DBSQLERROR_TYPE type)
													{
														this->type = type;
													}

		virtual							 ~DBSQLERROR						()														{														}

		DBSQLERROR_TYPE				type;
		DBSQLSTRING						description;

	private:

		void                  Clean									()
													{
														type = (DBSQLERROR_TYPE)0;											
													}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

#endif
