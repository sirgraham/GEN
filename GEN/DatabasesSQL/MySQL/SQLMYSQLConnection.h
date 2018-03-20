/*------------------------------------------------------------------------------------------
//	SQLMYSQLCONNECTION.H
*/	
/**	
// \class 
//   
//  Conection to Mysql
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 10/08/2015 12:15:38
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#if defined(DBSQL_ACTIVE) && defined(DBSQLMYSQL_ACTIVE)

#ifndef _SQLMYSQLCONNECTION_H_
#define _SQLMYSQLCONNECTION_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DBSQLConnection.h"
#include "DBSQLDatabase.h"

#include "mysql.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class SQLMYSQLQUERY;
class SQLMYSQLDATABASE;

class SQLMYSQLCONNECTION : public DBSQLCONNECTION
{
	public:

		friend												SQLMYSQLQUERY;
		friend												SQLMYSQLDATABASE;

																	SQLMYSQLCONNECTION			(DBSQLDATABASE* database) : DBSQLCONNECTION(database)			{ Clean();										}
		virtual											 ~SQLMYSQLCONNECTION			()																												{ Clean();										}

		virtual		bool								Connect									();
		virtual		bool								Disconnect							();

	protected:

		MYSQL*												connection;

	private:

		void													Clean										()																												
																	{															
																	
																	}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

#endif


