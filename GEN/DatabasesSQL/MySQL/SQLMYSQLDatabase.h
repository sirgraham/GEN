/*------------------------------------------------------------------------------------------
//	SQLMYSQLDATABASE.H
*/	
/**	
// \class 
//   
//  MySQL database concrete class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 10/08/2015 12:26:06
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#if defined(DBSQL_ACTIVE) && defined(DBSQLMYSQL_ACTIVE)

#ifndef _SQLMYSQLDATABASE_H_
#define _SQLMYSQLDATABASE_H_
	

#pragma comment (linker,"/NODEFAULTLIB:libcmtd.lib")
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XDebug.h"

#include "DBSQLBase.h"
#include "DBSQLDatabase.h"

#include "SQLMYSQLVariant.h"

#include "mysql.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/


class SQLMYSQLQUERY;
class SQLMYSQLCONNECTION;


class SQLMYSQLDATABASE : public DBSQLDATABASE
{
	public:

		friend class											SQLMYSQLCONNECTION;
		friend class											SQLMYSQLQUERY;

	
																			SQLMYSQLDATABASE				() : DBSQLDATABASE() 
																			{
																				Clean();
																				type	=	DBSQLDATABASE_FLAGS_SQL | DBSQLDATABASE_FLAGS_TRANSACTIONAL;																				
																			}

		virtual														~SQLMYSQLDATABASE				()																{ Clean();																							}
			
		virtual		XCHAR*									GetName									()																{ return __L("MYSQL");																	}
		virtual   DBSQLVARIANT*						CreateVariant						()																{ return new DBSQLVARIANT(); 												}

		virtual		bool										Transaction							();
		virtual		bool										Commit									();
		virtual		bool										Rollback								(XCHAR* savepointname=NULL);
		virtual		bool										Savepoint								(XCHAR* savepoint);
		virtual		bool										ReleaseSavepoint				(XCHAR* savepoint);
		virtual		DBSQLQUERY*							CreateQuery							();
		virtual   DBSQLCONNECTION*				CreateConnection				();
		virtual   bool										GetTables								();
		virtual		DBSQLDATABASE_ENCODING	GetDatabaseEncoding			();
		virtual		bool										SetDatabaseEncoding			(DBSQLDATABASE_ENCODING encodingtouse);

	

	protected:
		
		my_bool														isnull;
		
	private:

		void															Clean								()
																			{
																				connection	= NULL;
																				isnull			= false;
																			}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif


#endif
