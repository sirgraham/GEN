/*------------------------------------------------------------------------------------------
//	MYSQLRESULT.H
*/	
/**	
// \class 
//   
//  MYSQL concrete result set
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 11/08/2015 16:39:01
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	

#if defined(DBSQL_ACTIVE) && defined(DBSQLMYSQL_ACTIVE)

#ifndef _MYSQLRESULT_H_
#define _MYSQLRESULT_H_
	

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "mysql.h"	

#include "DBSQLBase.h"
#include "DBSQLResult.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class SQLMYSQLQUERY;

class SQLMYSQLRESULT : public DBSQLRESULT
{
	public:

		friend									SQLMYSQLQUERY;
		
		virtual				 				 ~SQLMYSQLRESULT									();

		virtual XQWORD					GetNumRows											();
		virtual	XQWORD					GetNumColumns										();
		virtual	bool						Seek														(int index);
		virtual	bool						First														();
		virtual	bool						Last														();
		virtual	bool						Next														();
		virtual	bool						ProcessRow											();
		virtual	bool						Abort														();
		virtual	bool						HasNext													();

	protected:		
														SQLMYSQLRESULT									() : DBSQLRESULT() 
														{ 
															Clean();															
														}			

		SQLMYSQLQUERY*					query;														 
		int											hasnext;

		MYSQL_BIND*							bindresults;
		int											bind_results_size;

	private:
				
		void										Clean														()
														{
															query							= NULL;
															hasnext						= 0;				
															bind_results_size	= 0;
															bindresults				= NULL;
															currentrow				= 0;
														}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif
	
	



#endif
