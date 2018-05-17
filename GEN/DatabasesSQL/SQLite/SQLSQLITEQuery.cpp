
/*------------------------------------------------------------------------------------------
//  SQLSQLITEQUERY.CPP
//
//  SQLITE query concrete class
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 11/08/2015 10:36:53
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#if defined(DBSQL_ACTIVE) && defined(DBSQLSQLITE_ACTIVE)

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "sqlite3.h"

#include "SQLSQLITEDatabase.h"
#include "SQLSQLITEConnection.h"
#include "SQLSQLITEResult.h"
#include "SQLSQLITEQuery.h"

#include "XFactory.h"
#include "XBuffer.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  SQLSQLITEQUERY::~SQLSQLITEQUERY
*/
/**
//
//   Class Destructor SQLSQLITEQUERY
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/09/2015 16:21:45
//
*/
/*-----------------------------------------------------------------*/
SQLSQLITEQUERY::~SQLSQLITEQUERY()
{
  if(result)
    {
      delete(result);
      result = NULL;
    }

  UnbindAll();

  ClearBuffers();
  Clean();
}




/*-------------------------------------------------------------------
//  SQLSQLITEQUERY::Execute
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2015 10:53:29
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITEQUERY::Execute()
{
  sqlite3* db = static_cast<SQLSQLITEDATABASE*>(database)->sqlite3database;
  if(!db)
    {
      database->Error(__L("database not initialized"));
      return 1;
    }


  XBUFFER* buffer=new XBUFFER();
  GetValue()->ConvertToUTF8(*buffer);
  this->buffers.Add(buffer);

  //----------------------- check statement
  XDWORD size=buffer->GetSize();
  if(!size)
    {
      DBSQLERROR* error = new DBSQLERROR(DBSQLERROR_TYPE_STATEMENT_ERROR);
      if(!error) return 1;
      error->description.Set(__L("void statement"));

      database->ClearPreviousErrors();

      database->GetErrorList()->Add(error);

      if (database->IsTransactionStarted()) database->Rollback();
      return 1;
    }


  int rc = this->Exec((char*)buffer->Get(),size);

  busyflag=(rc==2);

  if(rc!=0) return false;

  return true;
}





/*-------------------------------------------------------------------
//  SQLSQLITEQUERY::Exec
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/10/2015 13:21:56
//
//  @return       int :
//
//  @param        zsql :
*/
/*-----------------------------------------------------------------*/
int SQLSQLITEQUERY::Exec(const char* sql,XDWORD size)
{
  sqlite3* db = static_cast<SQLSQLITEDATABASE*>(database)->sqlite3database;

  ppstmt = NULL;

      //----------------------- prepare current statement
  int preparereturncode = sqlite3_prepare_v2(db, sql, size, &ppstmt, 0);
  if(preparereturncode)
    {
      DBSQLERROR* error=new DBSQLERROR(DBSQLERROR_TYPE_STATEMENT_ERROR);
      if(!error) return 1;

      error->description.Set(sqlite3_errmsg(db));

      database->ClearPreviousErrors();

      database->GetErrorList()->Add(error);
      if (database->IsTransactionStarted())     database->Rollback();
      return 1;
    }
   else
    {

      //----------------------- clear old result sets

      if(result)
        {
          UnbindAll();
          delete result;
          result = NULL;
        }

      //----------------------- statement ready, bind parameters

      BindParametersToQuery();


      int rc = sqlite3_step(ppstmt);

      switch (rc)
      {
          case SQLITE_LOCKED:
          {
              DBSQLERROR* error = new DBSQLERROR(DBSQLERROR_TYPE_CONNECTION_ERROR);
              if (!error) return 1;

              error->description.Set(sqlite3_errmsg(db));

              database->GetErrorList()->Add(error);

              sqlite3_finalize(ppstmt);
              return 2;
          }
          break;

          case SQLITE_PERM:
          {
              DBSQLERROR* error = new DBSQLERROR(DBSQLERROR_TYPE_CONNECTION_ERROR);
              if (!error) return 1;

              error->description.Set(sqlite3_errmsg(db));

              database->GetErrorList()->Add(error);

              sqlite3_finalize(ppstmt);
              return 2;
          }
          break;

          case SQLITE_BUSY:
          {
              sqlite3_finalize(ppstmt);
              return 2;
          }
          break;

          case SQLITE_ERROR:                  //----------------------- statment failed
          {
              DBSQLERROR* error = new DBSQLERROR(DBSQLERROR_TYPE_STATEMENT_ERROR);
              if (!error) return 1;

              error->description.Set(sqlite3_errmsg(db));

              database->GetErrorList()->Add(error);
              sqlite3_finalize(ppstmt);

              if (database->IsTransactionStarted()) database->Rollback();

              ClearBuffers();
              return 1;
          }
          break;

          case SQLITE_DONE:
          {
              sqlite3_finalize(ppstmt);
              return 0;
          }
          break;


          case SQLITE_ROW:                          //--------------------multirow statement (select)
          {
              int ncount = sqlite3_column_count(ppstmt);

              if (!result)    // prepare resultset
              {
                  result = ConstructResult();
                  if (!result)
                  {
                      DBSQLERROR* error = new DBSQLERROR(DBSQLERROR_TYPE_MEMORY_ERROR);
                      if (!error) return 1;

                      error->description.Set(__L("not enought memory for result"));

                      database->ClearPreviousErrors();

                      database->GetErrorList()->Add(error);
                      sqlite3_finalize(ppstmt);

                      if (database->IsTransactionStarted())     database->Rollback();

                      ClearBuffers();
                      return 1;
                  }
              }

              return 0;
      }
      break;
      }

      return 1;
    }


  return 0;
}



/*-------------------------------------------------------------------
//  SQLSQLITEQUERY::BindParametersToQuery
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2015 14:27:24
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITEQUERY::BindParametersToQuery()
{
  for(XDWORD e=0; e<statementbindings.GetSize(); e++)
    {
      XDWORD        statementindex  = statementbindings.GetKey(e);
      DBSQLVARIANT* variant         = statementbindings.GetElement(e);

      switch(variant->GetType())
        {
          case DBSQLVARIANT_TYPE_INTEGER    : { int rc = sqlite3_bind_int(ppstmt, statementindex, (int)*variant);
                                                switch(rc)
                                                  {
                                                    case SQLITE_OK      : break;
                                                    case SQLITE_TOOBIG  : return false;
                                                    case SQLITE_NOMEM   : return false;
                                                            default     : break;
                                                  }
                                              }
                                              break;

          case DBSQLVARIANT_TYPE_DATETIME   : { XSTRING   string;
                                                XDATETIME datetime    = (*variant);

                                                datetime.GetDateTimeToString(XDATETIME_FORMAT_STANDARD, string);

                                                XBUFFER* variantdatabuffer = new XBUFFER();
                                                if(!variantdatabuffer) break;

                                                string.ConvertToUTF8(*variantdatabuffer);
                                                buffers.Add(variantdatabuffer);

                                                XDWORD size = variantdatabuffer->GetSize();
                                                int rc=sqlite3_bind_text(ppstmt, statementindex, (char*)variantdatabuffer->Get(), size, 0);
                                                switch(rc)
                                                  {
                                                    case SQLITE_OK      : break;
                                                    case SQLITE_TOOBIG  : return false;
                                                    case SQLITE_NOMEM   : return false;
                                                            default     : break;
                                                  }

                                              }
                                              break;

          case DBSQLVARIANT_TYPE_STRING     : { XBUFFER* variantdatabuffer = new XBUFFER();
                                                if(!variantdatabuffer) break;

                                                XSTRING string;

                                                string.Set((XCHAR*)*variant);
                                                string.ConvertToUTF8(*variantdatabuffer);

                                                buffers.Add(variantdatabuffer);

                                                XDWORD size = variantdatabuffer->GetSize();

                                                int rc=sqlite3_bind_text(ppstmt, statementindex, (char*)variantdatabuffer->Get(), size, 0);
                                                switch(rc)
                                                  {
                                                    case SQLITE_OK      : break;
                                                    case SQLITE_TOOBIG  : return false;
                                                    case SQLITE_NOMEM   : return false;
                                                            default     : break;
                                                  }

                                              }
                                              break;

          case DBSQLVARIANT_TYPE_FLOAT      : { int rc = sqlite3_bind_double(ppstmt, statementindex,(double)*variant);
                                                switch(rc)
                                                  {
                                                    case SQLITE_OK      : break;
                                                    case SQLITE_TOOBIG  : return false;
                                                    case SQLITE_NOMEM   : return false;
                                                            default     : break;
                                                  }
                                              }
                                              break;

        }
    }

  return true;
}



/*-------------------------------------------------------------------
//  SQLSQLITEQUERY::ConstructResult
*/
/**
//
//  Constructs concrete SQLITE ResultSet
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2015 16:42:53
//
//  @return       DBSQLRESULT* :
//
*/
/*-----------------------------------------------------------------*/
DBSQLRESULT* SQLSQLITEQUERY::ConstructResult()
{
  SQLSQLITERESULT* set = new SQLSQLITERESULT();
  if(set)
  {
    set->query = this;
    set->lastresult = SQLITE_ROW;
  }

  return set;
}



#endif

