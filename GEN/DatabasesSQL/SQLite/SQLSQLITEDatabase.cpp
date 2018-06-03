
/*------------------------------------------------------------------------------------------
//  SQLSQLITEDATABASE.CPP
//
//  SQLITE concrete class
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 10/08/2015 15:44:47
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#if defined(DBSQL_ACTIVE) && defined(DBSQLSQLITE_ACTIVE)

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XDebugTrace.h"

#include "SQLSQLITEConnection.h"
#include "SQLSQLITEQuery.h"

#include "DBSQLQuery.h"

#include "SQLSQLITEDatabase.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  SQLSQLITEDATABASE::Transaction
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2015 17:45:37
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITEDATABASE::Transaction()
{
  transactionstarted = true;

  int rc=sqlite3_exec(sqlite3database, "BEGIN IMMEDIATE", 0, 0, 0);

  return (rc==SQLITE_OK || rc==SQLITE_BUSY);
}



/*-------------------------------------------------------------------
//  SQLSQLITEDATABASE::Commit
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2015 17:45:56
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITEDATABASE::Commit()
{
  if(IsTransactionStarted())
    {
      int rc=sqlite3_exec(sqlite3database, "COMMIT", 0, 0, 0);
      transactionstarted = false;

      //XDEBUGTRACE_PRINTCOLOR(0,__L("Commiting %d"),rc);

      if(rc == SQLITE_OK)   return true;
      if(rc == SQLITE_BUSY) return true;
    }
   //elseXDEBUGTRACE_PRINTCOLOR(4,__L("transaction not started"));

  return false;
}




/*-------------------------------------------------------------------
//  SQLSQLITEDATABASE::Rollback
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2015 17:46:03
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITEDATABASE::Rollback(XCHAR *savepoint)
{
  if(IsTransactionStarted())
    {
      if(savepoint == NULL)
        {
          int rc = sqlite3_exec(sqlite3database, "ROLLBACK", 0, 0, 0);
          transactionstarted = false;
          return (rc==SQLITE_OK);
        }
       else
        {
          DBSQLSTRING text;
          text.Format(__L("ROLLBACK TO SAVEPOINT %s"), savepoint);

          XSTRING_CREATEOEM(text,rollbacktext);
          int rc = sqlite3_exec(sqlite3database, rollbacktext, 0, 0, 0);
          transactionstarted = false;
          XSTRING_DELETEOEM(rollbacktext);

          return (rc==SQLITE_OK);
        }
    }

  return false;
}



/*-------------------------------------------------------------------
//  SQLSQLITEDATABASE::Savepoint
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/08/2015 12:59:57
//
//  @return       bool :
//
//  @param        *savepoint :
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITEDATABASE::Savepoint(XCHAR* savepoint)
{
  DBSQLSTRING text;
  text.Format(__L("SAVEPOINT %s"),savepoint);

  XSTRING_CREATEOEM(text,rollbacktext);
  int rc=sqlite3_exec(sqlite3database, rollbacktext, 0, 0, 0);
  XSTRING_DELETEOEM(rollbacktext);

  return (rc==SQLITE_OK);
}




/*-------------------------------------------------------------------
//  SQLSQLITEDATABASE::Savepoint
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/08/2015 12:59:57
//
//  @return       bool :
//
//  @param        *savepoint :
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITEDATABASE::ReleaseSavepoint(XCHAR *savepoint)
{
  DBSQLSTRING text;
  text.Format(__L("RELEASE %s"),savepoint);

  XSTRING_CREATEOEM(text,rollbacktext);
  int rc=sqlite3_exec(sqlite3database, rollbacktext, 0, 0, 0);
  XSTRING_DELETEOEM(rollbacktext);

  return (rc==SQLITE_OK);
}




/*-------------------------------------------------------------------
//  SQLSQLITEDATABASE::CreateQuery
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2015 17:46:23
//
//  @return       DBSQLQUERY* :
//
*/
/*-----------------------------------------------------------------*/
DBSQLQUERY* SQLSQLITEDATABASE::CreateQuery()
{
  DBSQLQUERY* query = new SQLSQLITEQUERY(this);
  if(!query)
    {
     XDEBUGTRACE_PRINTCOLOR(4,__L("SQLSQLITEDATABASE::CreateQuery  unable to create SQLSQLITEQUERY"));
      return NULL;
    }

  return query;
}




/*-------------------------------------------------------------------
//  SQLSQLITEDATABASE::CreateConnection
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2015 17:46:29
//
//  @return       DBSQLCONNECTION* :
//
*/
/*-----------------------------------------------------------------*/
DBSQLCONNECTION* SQLSQLITEDATABASE::CreateConnection  ()
{
  DBSQLCONNECTION* conn = new SQLSQLITECONNECTION(this);
  return conn;
}




/*-------------------------------------------------------------------
//  SQLSQLITEDATABASE::GetTables
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/08/2015 12:40:15
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  SQLSQLITEDATABASE::GetTables  ()
{
  DBSQLQUERY* query = CreateQuery();
  if(!query) return false;

  bool status = true;

  tables.DeleteContents();
  tables.DeleteAll();

  query->Set(__L("SELECT name FROM sqlite_master WHERE type='table'"));
  if(query->Execute())
    {
      DBSQLRESULT* result=query->GetResult();
      if(result!=NULL) //hay un result
        {
          result->GetNumRows();
          while(result->HasNext())
            {
              result->ProcessRow();

              DBSQLROW* row = result->GetRow();
              for(XDWORD e=0; e<result->GetNumColumns(); e++)
                {
                  if(row->Get(e).GetType()==(XVARIANT_TYPE)DBSQLVARIANT_TYPE_STRING)
                    {
                      DBSQLSTRING* tablename = new DBSQLSTRING();
                      if(tablename)
                        {
                          tablename->Set((XCHAR*)row->Get(0));
                          tables.Add(tablename);
                        }
                    }
                }

              result->Next();
            }
        }
    }
   else
    {
      Error(__L("Unable to obtain Tables"));
      status = false;
    }

  delete(query);

  return status;
}



/*-------------------------------------------------------------------
//  SQLMYSQLDATABASE::GetDatabaseEncoding
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/10/2015 9:47:27
//
//  @return       DBSQLDATABASE_ENCODING :
//
*/
/*-----------------------------------------------------------------*/
DBSQLDATABASE_ENCODING SQLSQLITEDATABASE::GetDatabaseEncoding()
{
  return DBSQLDATABASE_ENCODING_UNKNOWN;
}



/*-------------------------------------------------------------------
//  SQLMYSQLDATABASE::SetDatabaseEncoding
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      05/10/2015 9:47:39
//
//  @return       bool :
//
//  @param        encodingtouse :
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITEDATABASE::SetDatabaseEncoding(DBSQLDATABASE_ENCODING encodingtouse)
{
  switch(encodingtouse)
    {
      case DBSQLDATABASE_ENCODING_BIG5              :
      case DBSQLDATABASE_ENCODING_EUC_CN            :
      case DBSQLDATABASE_ENCODING_EUC_JP            :
      case DBSQLDATABASE_ENCODING_EUC_JIS_2004      :
      case DBSQLDATABASE_ENCODING_EUC_KR            :
      case DBSQLDATABASE_ENCODING_EUC_TW            :
      case DBSQLDATABASE_ENCODING_GB18030           :
      case DBSQLDATABASE_ENCODING_GBK               :
      case DBSQLDATABASE_ENCODING_ISO_8859_5        :
      case DBSQLDATABASE_ENCODING_ISO_8859_6        :
      case DBSQLDATABASE_ENCODING_ISO_8859_7        :
      case DBSQLDATABASE_ENCODING_ISO_8859_8        :
      case DBSQLDATABASE_ENCODING_JOHAB             :
      case DBSQLDATABASE_ENCODING_KOI8R             :
      case DBSQLDATABASE_ENCODING_KOI8U             :
      case DBSQLDATABASE_ENCODING_LATIN1            :
      case DBSQLDATABASE_ENCODING_LATIN2            :
      case DBSQLDATABASE_ENCODING_LATIN3            :
      case DBSQLDATABASE_ENCODING_LATIN4            :
      case DBSQLDATABASE_ENCODING_LATIN5            :
      case DBSQLDATABASE_ENCODING_LATIN6            :
      case DBSQLDATABASE_ENCODING_LATIN7            :
      case DBSQLDATABASE_ENCODING_LATIN8            :
      case DBSQLDATABASE_ENCODING_LATIN9            :
      case DBSQLDATABASE_ENCODING_LATIN10           :
      case DBSQLDATABASE_ENCODING_MULE_INTERNAL     :
      case DBSQLDATABASE_ENCODING_SJIS              :
      case DBSQLDATABASE_ENCODING_SHIFT_JIS_2004    :
      case DBSQLDATABASE_ENCODING_SQL_ASCII         :
      case DBSQLDATABASE_ENCODING_UHC               : break;

      case DBSQLDATABASE_ENCODING_UTF8              : { DBSQLQUERY* query = CreateQuery();
                                                        if(!query) break;

                                                        query->Set(__L("PRAGMA encoding=\"UTF-8\""));
                                                        int rc = query->Execute();

                                                        delete query;

                                                        return rc==0;
                                                      }
                                                      break;
      case DBSQLDATABASE_ENCODING_WIN866            :
      case DBSQLDATABASE_ENCODING_WIN874            :
      case DBSQLDATABASE_ENCODING_WIN1250           :
      case DBSQLDATABASE_ENCODING_WIN1251           :
      case DBSQLDATABASE_ENCODING_WIN1252           :
      case DBSQLDATABASE_ENCODING_WIN1253           :
      case DBSQLDATABASE_ENCODING_WIN1254           :
      case DBSQLDATABASE_ENCODING_WIN1255           :
      case DBSQLDATABASE_ENCODING_WIN1256           :
      case DBSQLDATABASE_ENCODING_WIN1257           :
      case DBSQLDATABASE_ENCODING_WIN1258           : break;

    }

  Error (__L("Unable to set the charset encoding"));

  return false;
}




#endif
