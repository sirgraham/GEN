
/*------------------------------------------------------------------------------------------
//  DBSQLFACTORY.CPP
//
//  sdkflñsdkfñsd
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 01/10/2015 16:08:36
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#ifdef DBSQL_ACTIVE

#ifdef DBSQLMYSQL_ACTIVE
#include "SQLMYSQLDatabase.h"
#endif

#ifdef DBSQLPOSTGRESQL_ACTIVE
#include "SQLPostgreSQLDatabase.h"
#endif

#ifdef DBSQLSQLITE_ACTIVE
#include "SQLSQLITEDatabase.h"
#endif

#include "DBSQLFactory.h"

#include "XDebug.h"
#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DBSQLFACTORY::Create
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/10/2015 16:12:20
//
//  @return       DBSQLDATABASE* :
//

//  @param        databasetype :
*/
/*-----------------------------------------------------------------*/
DBSQLDATABASE* DBSQLFACTORY::Create( DBSQLDATABASE_TYPE databasetype)
{
  DBSQLDATABASE* db = NULL;

  switch(databasetype)
    {
      case DBSQLDATABASE_TYPE_MYSQL             :
                                                  #ifdef DBSQLMYSQL_ACTIVE
                                                  db = new SQLMYSQLDATABASE();
                                                  return db;
                                                  #endif
                                                  break;

      case DBSQLDATABASE_TYPE_POSTGRESQL  :
                                                  #ifdef DBSQLPOSTGRESQL_ACTIVE
                                                  db = new SQLPOSTGRESQLDATABASE();
                                                  return db;
                                                  #endif
                                                  break;

      case DBSQLDATABASE_TYPE_SQLITE            :
                                                  #ifdef DBSQLSQLITE_ACTIVE
                                                  db = new SQLSQLITEDATABASE();
                                                  return db;
                                                  #endif
                                                  break;

                              default           : break;
    }

  XDEBUG_PRINTCOLOR(4, __L("Unrecognized database type"));
  return NULL;
}

#endif
