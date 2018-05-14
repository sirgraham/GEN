
/*------------------------------------------------------------------------------------------
//  SQLMYSQLQUERY.CPP
//
//  MYSQL concrete query
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 19/08/2015 17:39:38
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#if defined(DBSQL_ACTIVE) && defined(DBSQLMYSQL_ACTIVE)

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "SQLMYSQLConnection.h"
#include "SQLMYSQLDatabase.h"
#include "SQLMYSQLQuery.h"
#include "SQLMYSQLResult.h"

#include "mysql.h"

#include "XBuffer.h"
#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  SQLMYSQLQUERY::~SQLMYSQLQUERY
*/
/**
//
//   Class Destructor SQLMYSQLQUERY
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/09/2015 16:12:13
//
*/
/*-----------------------------------------------------------------*/
SQLMYSQLQUERY::~SQLMYSQLQUERY()
{
  UnbindAll();

  Clean();
}



/*-------------------------------------------------------------------
//  SQLMYSQLQUERY::Execute
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/08/2015 17:41:55
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLQUERY::Execute()
{
  SQLMYSQLDATABASE* db    = static_cast<SQLMYSQLDATABASE*>  (database);
  MYSQL*            msql  = static_cast<SQLMYSQLCONNECTION*>(db->connection)->connection;

  if(this->value.FindCharacter(__C('*'))!= NOTFOUND && this->value.Find(__L("COUNT(*)"),true)==XSTRING_NOTFOUND)
    {
      DBSQLSTRING error;
      database->ClearPreviousErrors();
      error.Set(__L("Wildcard * is discouraged and therefore disabled in this implementation"));
      this->database->Error(error.Get());
      return false;
    }


  DBSQLSTRING query;
  query.Set(this->value);

  XDWORD size = query.GetSize();
  if(!size)
    {
      DBSQLERROR* error = new DBSQLERROR(DBSQLERROR_TYPE_STATEMENT_ERROR);
      if(!error) return false;

      database->ClearPreviousErrors();

      error->description.Set(__L("void statement"));
      database->GetErrorList()->Add(error);

      if(database->IsTransactionStarted())  database->Rollback();

      return false;
   }

  XBUFFER* querystringbuffer=new XBUFFER();
  query.ConvertToUTF8   (*querystringbuffer );
  buffers.Add           (querystringbuffer  );

  int RC = Exec(msql, (char*)querystringbuffer->Get());
  if(RC) return false;

  return true;
}




/*-------------------------------------------------------------------
//  SQLMYSQLQUERY::UnbindAll
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2015 10:05:50
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLQUERY::UnbindAll()
{
  if(result)
    {
      delete(result);
      result = NULL;
    }

  if(param!=NULL)
    {
      for(XDWORD e=0; e<statementbindings.GetSize(); e++)
        {
          if(param[e].buffer)
            {
              param[e].buffer=NULL;
            }
        }

      delete[] param;
      param=NULL;
    }

  return DBSQLQUERY::UnbindAll();
}




/*-------------------------------------------------------------------
//  DBSQLQUERY::Bind
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2015 10:44:13
//
//  @return       bool :
//
//  @param        ID :
//  @param        datetime :
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLQUERY::Bind(XDWORD ID, DBSQLDATETIME& datetime)
{
  SQLMYSQLVARIANT* variant=new SQLMYSQLVARIANT();
  if(!variant)  return true;

  (*variant) = datetime;

  return statementbindings.Add(ID, variant);
}




/*-------------------------------------------------------------------
//  SQLMYSQLQUERY::exec
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/09/2015 16:11:00
//
//  @return       bool :
//
//  @param        db :
//  @param        sql :
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLQUERY::Exec(MYSQL* db,char* sql)
{
  if(!db)
    {
      database->Error(__L("database not initialized"));
      return true;
    }

  this->mysqldb=db;

  //----------------------- check statement

  stmt = mysql_stmt_init(db);

  //----------------------- create statement

  if(!stmt)
    {
      DBSQLERROR* error=new DBSQLERROR(DBSQLERROR_TYPE_STATEMENT_ERROR);
      if(!error) return false;

      error->description.Set(mysql_error(db));
      database->GetErrorList()->Add(error);
      if(database->IsTransactionStarted())  database->Rollback();

      return true;
    }
   else
    {
      //--------------- set up statement
      if(mysql_stmt_prepare(stmt, sql, strlen(sql)) != 0)
        {
          DBSQLERROR* error=new DBSQLERROR(DBSQLERROR_TYPE_STATEMENT_ERROR);
          if(!error) return false;

          error->description.Set(mysql_error(db));
          database->GetErrorList()->Add(error);
          if(database->IsTransactionStarted())  database->Rollback();

          return true;
       }

      //----------------------- statement ready, bind parameters

      BindParametersToQuery();

      //----------------------- clear old result sets

      if(result)
        {
          delete result;
          result = NULL;
        }

      //----------------------- clear old result sets

      int RC = mysql_stmt_execute(stmt);

      if(RC != DBSQLMYSQL_OK)
        {
          DBSQLERROR* error = new DBSQLERROR(DBSQLERROR_TYPE_STATEMENT_ERROR);
          if(!error) return false;

          error->description.Set(mysql_error(db));
          database->GetErrorList()->Add(error);
          if (database->IsTransactionStarted()) database->Rollback();

          return true;
        }

      //---------------------- prepare result

      if(!result)   // prepare resultset
        {
          result = ConstructResult();

          if(!result)
            {
              DBSQLERROR* error=new DBSQLERROR(DBSQLERROR_TYPE_MEMORY_ERROR);
              if(!error) return false;

              error->description.Set(__L("not enought memory for result"));
              database->GetErrorList()->Add(error);
              mysql_stmt_free_result(stmt);
              mysql_stmt_close(stmt);

              if(database->IsTransactionStarted())  database->Rollback();

              ClearBuffers();

              return true;
            }
        }

      //---------------------- bind results

      BindParametersToResult();

      RC = mysql_stmt_store_result(stmt);
      if(RC!= DBSQLMYSQL_OK)
        {
          DBSQLERROR* error = new DBSQLERROR(DBSQLERROR_TYPE_STATEMENT_ERROR);
          if(!error) return false;

          error->description.Set(mysql_error(db));
          database->GetErrorList()->Add(error);
          if(database->IsTransactionStarted())      database->Rollback();

          return true;
        }

      //-------------------- bind result and store

      mysql_stmt_fetch(stmt);
      if(mysql_stmt_num_rows(stmt) == 0)
        {
          if (mysql_stmt_free_result(stmt)==DBSQLMYSQL_OK)
              mysql_stmt_close(stmt);

          if(result)
            {
              delete(result);
              result = NULL;
            }
        }

      ClearBuffers();
    }

  return false;
}




/*-------------------------------------------------------------------
//  SQLMYSQLQUERY::ConstructResult
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/08/2015 17:41:50
//
//  @return       DBSQLRESULT* :
//
*/
/*-----------------------------------------------------------------*/
DBSQLRESULT* SQLMYSQLQUERY::ConstructResult()
{
  SQLMYSQLRESULT* result = new SQLMYSQLRESULT();
  if(!result) return NULL;

  result->query = this;

  return result;
}



/*-------------------------------------------------------------------
//  SQLMYSQLQUERY::BindParametersToQuery
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/08/2015 17:42:00
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLQUERY::BindParametersToQuery()
{
  if(!statementbindings.GetSize())  return true;

  //----------------------- prepare memory

  if(param)
    {
      delete[] param;
      param=NULL;
    }

  param = new MYSQL_BIND[statementbindings.GetSize()];
  if(!param)
    {
      database->Error(__L("Not enought memory"));
      return false;
    }

  memset(param,0,statementbindings.GetSize()*sizeof(MYSQL_BIND));

  static_cast<SQLMYSQLDATABASE*>(this->database)->isnull=false;

  //----------------------- iterate

  for(XDWORD e=0; e<statementbindings.GetSize(); e++)
    {
      XDWORD        statementindex = statementbindings.GetKey(e);
      DBSQLVARIANT* variant        = statementbindings.GetElement(e);

      if(variant)
        {
          param[e].is_unsigned      = 0;
          param[e].is_null          = &static_cast<SQLMYSQLDATABASE*>(this->database)->isnull;
          param[e].length           = 0;

          switch(variant->GetType())
            {
              case DBSQLVARIANT_TYPE_INTEGER    : param[e].buffer_type      = MYSQL_TYPE_SHORT;
                                                  param[e].buffer           = (unsigned char*)variant->GetData();
                                                  param[e].buffer_length    = variant->GetSize();
                                                  break;

              case DBSQLVARIANT_TYPE_DATETIME   : param[e].buffer_type      = MYSQL_TYPE_DATETIME;
                                                  param[e].buffer           = (unsigned char*)variant->GetData();
                                                  param[e].is_null          = 0;
                                                  param[e].buffer_length    = 0;
                                                  break;

              case DBSQLVARIANT_TYPE_STRING     : { DBSQLSTRING variantdata;
                                                    variantdata.Set((XCHAR*)*variant);

                                                    XBUFFER*  variantdatabuffer = new XBUFFER();

                                                    variantdata.ConvertToUTF8(*variantdatabuffer);
                                                    buffers.Add(variantdatabuffer);

                                                    XDWORD size = variantdatabuffer->GetSize();

                                                    param[e].buffer_type      = MYSQL_TYPE_VARCHAR;
                                                    param[e].buffer           = variantdatabuffer->Get();
                                                    param[e].buffer_length    = size;

                                                  }
                                                  break;

                case DBSQLVARIANT_TYPE_FLOAT    : { float f = (*variant);
                                                    param[e].buffer_type      = MYSQL_TYPE_FLOAT;
                                                    param[e].buffer           = variant->GetData();
                                                  }
                                                  break;

                                  default       : return false;
            }
        }

      //------------------- bind param block

      if(mysql_stmt_bind_param(stmt, param) != 0)
        {
          DBSQLERROR* error = new DBSQLERROR(DBSQLERROR_TYPE_STATEMENT_ERROR);
          if(!error) return false;

          error->description.Set(mysql_error(mysqldb));
          database->GetErrorList()->Add(error);
          if (database->IsTransactionStarted()) database->Rollback();

          return true;
        }
   }

  return true;
}



/*-------------------------------------------------------------------
//  SQLMYSQLQUERY::BindParametersToResult
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      20/08/2015 16:53:00
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLQUERY::BindParametersToResult()
{
  MYSQL_RES* mysqlresultfromstmt = mysql_stmt_result_metadata(stmt);
  if(!mysqlresultfromstmt)  return true;   //this statement gives no result set

  //----------------------- prepare memory
  int ncols=mysql_num_fields(mysqlresultfromstmt);
  if(!ncols) return true;

  SQLMYSQLRESULT* result = static_cast<SQLMYSQLRESULT*>(this->result);
  result->bind_results_size = ncols;

  result->bindresults = NULL;
  result->bindresults = new MYSQL_BIND[ncols];
  if(!result->bindresults)
    {
      this->database->Error(__L("Not enought memory"));
      return false;
    }

  memset(result->bindresults, 0, ncols*sizeof(MYSQL_BIND));

  //----------------------- iterate

  for(int e=0; e<ncols; e++)
    {
      MYSQL_FIELD* field = mysql_fetch_field(mysqlresultfromstmt);

      result->bindresults[e].buffer_type      = field->type;
      switch(field->type)
        {
          case MYSQL_TYPE_DATETIME  : result->bindresults[e].buffer = new MYSQL_TIME;                   break;
                            default : result->bindresults[e].buffer = new unsigned char[field->length]; break;
        }

      if(!result->bindresults[e].buffer)
        {
          delete (result->bindresults);
          return false;
        }

      result->bindresults[e].buffer_length    = field->length;
      result->bindresults[e].is_unsigned      = 0;
      result->bindresults[e].is_null          = 0;
      result->bindresults[e].length           = 0;
    }

  //------------------- bind param block

  int RC = mysql_stmt_bind_result(stmt, result->bindresults);
  if(RC)
    {
      DBSQLERROR* error=new DBSQLERROR(DBSQLERROR_TYPE_STATEMENT_ERROR);
      if(!error) return false;

      error->description.Set(mysql_stmt_error(stmt));
      database->GetErrorList()->Add(error);
      if (database->IsTransactionStarted()) database->Rollback();

      return true;
    }

  return true;
}


#endif
