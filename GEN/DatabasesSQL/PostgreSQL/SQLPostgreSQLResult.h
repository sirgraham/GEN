/*------------------------------------------------------------------------------------------
//	SQLPOSTGRESQLRESULT.H
*/	
/**	
// \class 
//   
//  SQLPOSTGRESQL concrete result set
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 11/08/2015 16:39:01
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#if defined(DBSQL_ACTIVE) && defined(DBSQLPOSTGRESQL_ACTIVE)

#ifndef _SQLPOSTGRESQLRESULT_H_
#define _SQLPOSTGRESQLRESULT_H_
	

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XSystem.h"

#include "DBSQLBase.h"
#include "DBSQLResult.h"

#include "SQLPostgreSQLQuery.h"

	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
/*---- CLASS -----------------------------------------------------------------------------*/


class SQLPOSTGRESQLRESULT : public DBSQLRESULT
{
	public:
			friend										SQLPOSTGRESQLQUERY;
		
			virtual									 ~SQLPOSTGRESQLRESULT						();

			virtual XQWORD						GetNumRows										();
			virtual	XQWORD						GetNumColumns									();
			virtual	bool							Seek													(int index);
			virtual	bool							First													();
			virtual	bool							Last													();
			virtual	bool							Next													();
			virtual	bool							ProcessRow										();
			virtual	bool							Abort													();
			virtual	bool							HasNext												();

	protected:		
																SQLPOSTGRESQLRESULT						() : DBSQLRESULT() 
																{
																	Clean(); 

																	system = xfactory->CreateSystem();				
																	is_le	 = system->HardwareUseLittleEndian();

																}			

			SQLPOSTGRESQLQUERY*				query;
			PGresult*									resultset;
			XSYSTEM*									system;
			bool											is_le;

	private:	


			void											Clean													()
																{
																	query			= NULL;
																	resultset	= NULL;
																}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

#endif
