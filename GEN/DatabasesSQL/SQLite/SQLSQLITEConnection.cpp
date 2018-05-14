
/*------------------------------------------------------------------------------------------
//  SQLSQLITECONNECTION.CPP
//
//  SQLite connection concrete class
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 10/08/2015 12:53:51
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#if defined(DBSQL_ACTIVE) && defined(DBSQLSQLITE_ACTIVE)

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "sqlite3.h"


#include "SQLSQLITEDatabase.h"

#include "DBSQLError.h"
#include "DBSQLDatabase.h"
#include "DBSQLError.h"

#include "SQLSQLITEConnection.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  SQLSQLITECONNECTION::Connect
*/
/**
//  Connects to the database, local or remote, using options. If not enought options provided,
//  or connection fails, a DBError will be pushed into the error stack
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/08/2015 13:06:52
//
//  @return       bool : connection failed, DBError pushed to the stack
//
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITECONNECTION::Connect()
{
  if(!database)
    {
      database->Error(__L("DATABASE not initialized"));
      return false;
    }

  //--------------- first check if options provided a db name

  DBSQLSTRING databasename;
  DBSQLSTRING databasetimeout;
  if(!FindOption(__L("DATABASE"), &databasename))
    {
      database->Error(__L("DATABASE argument not supplied"));
      return false;
    }

  //--------------- attempt connection
  XPATH xpathdatabase;

  if(!FindOption(__L("PATH"), &xpathdatabase))
    {
      database->Error(__L("DATABASE argument not supplied"));
      return false;
    }

  xpathdatabase.Add(databasename);



  XSTRING_CREATEOEM(xpathdatabase, dbname);
  int returncode = sqlite3_open(dbname, &static_cast<SQLSQLITEDATABASE*>(database)->sqlite3database);
  XSTRING_DELETEOEM(dbname);

  if(returncode)
  {
      database->Error((char*)sqlite3_errmsg(static_cast<SQLSQLITEDATABASE*>(database)->sqlite3database));
      sqlite3_close(static_cast<SQLSQLITEDATABASE*>(database)->sqlite3database);
      return false;
  }

  if (FindOption(__L("TIMEOUT"), &databasetimeout))
  {
      int timeoutseconds=databasetimeout.ConvertToInt();
      returncode = sqlite3_busy_timeout(static_cast<SQLSQLITEDATABASE*>(database)->sqlite3database,timeoutseconds*1000);
       if(returncode)
        {
          database->Error((char*)sqlite3_errmsg(static_cast<SQLSQLITEDATABASE*>(database)->sqlite3database));
          sqlite3_close(static_cast<SQLSQLITEDATABASE*>(database)->sqlite3database);
          return false;
        }
  }

  return true;
}




/*-------------------------------------------------------------------
//  SQLSQLITECONNECTION::Disconnect
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/08/2015 13:48:02
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITECONNECTION::Disconnect()
{
  if(!database) return false;

  if(static_cast<SQLSQLITEDATABASE*>(database)->sqlite3database)
            sqlite3_close(static_cast<SQLSQLITEDATABASE*>(database)->sqlite3database);
      else return false;

  return true;
}


#endif

