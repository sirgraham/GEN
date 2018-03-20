/*------------------------------------------------------------------------------------------
//	DBSQLFACTORY.H
*/	
/**	
// \class 
//   
//  Database connector factory
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 11/08/2015 17:00:55
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
enum DBSQLDATABASE_TYPE
{
		DBSQLDATABASE_TYPE_UNKNOWN = 0,
		DBSQLDATABASE_TYPE_SQLITE,
		DBSQLDATABASE_TYPE_MYSQL,
		DBSQLDATABASE_TYPE_POSTGRESQL
};

#ifdef DBSQL_ACTIVE

#ifndef _DBSQLFACTORY_H_
#define _DBSQLFACTORY_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "DBSQLConnection.h"
#include "DBSQLDatabase.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	


/*---- CLASS -----------------------------------------------------------------------------*/

class DBSQLFACTORY
{
	public:

		static DBSQLDATABASE*					Create									(DBSQLDATABASE_TYPE databasetype);
	
};
		
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

#endif


