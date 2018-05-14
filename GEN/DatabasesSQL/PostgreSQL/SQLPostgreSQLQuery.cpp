
/*------------------------------------------------------------------------------------------
//  SQLPOSTGRESQLQUERY.CPP
//
//  postgres query concrete
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 08/09/2015 11:08:18
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#if defined(DBSQL_ACTIVE) && defined(DBSQLPOSTGRESQL_ACTIVE)

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "libpq-fe.h"

#include "SQLPOSTGRESQLDatabase.h"
#include "SQLPOSTGRESQLResult.h"
#include "SQLPOSTGRESQLConnection.h"

#include "SQLPOSTGRESQLQuery.h"

#include "XBuffer.h"
#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  SQLPOSTGRESQLQUERY::SQLPOSTGRESQLQUERY
*/
/**
//
//  Class Constructor SQLPOSTGRESQLQUERY
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 12:36:46
//
//  @param        database :
*/
/*-----------------------------------------------------------------*/
SQLPOSTGRESQLQUERY::SQLPOSTGRESQLQUERY(DBSQLDATABASE* database) : DBSQLQUERY(database)
{
  Clean();

  conn = static_cast<SQLPOSTGRESQLCONNECTION*>(static_cast<SQLPOSTGRESQLDATABASE*>(database)->connection)->connection;
}



/*-------------------------------------------------------------------
//  SQLPOSTGRESQLQUERY::~SQLPOSTGRESQLQUERY
*/
/**
//
//   Class Destructor SQLPOSTGRESQLQUERY
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 12:36:51
//
*/
/*-----------------------------------------------------------------*/
SQLPOSTGRESQLQUERY::~SQLPOSTGRESQLQUERY()
{
  if(result)
    {
      delete(result);
      result = NULL;
    }

  if(paramformats)
    {
      delete[] paramformats;
      paramformats = NULL;
    }

  if(paramvalues)
    {
      delete[] paramvalues;
      paramvalues = NULL;
    }

  if(paramlengths)
    {
      delete[] paramlengths;
      paramlengths = NULL;
    }

  ClearBuffers();

  UnbindAll();
  Clean();
}




/*-------------------------------------------------------------------
//  SQLPOSTGRESQLQUERY::Execute
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 12:27:05
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLQUERY::Execute()
{
  if(!database) return false;
  if(!database->Lock()) return false;

  //----------------------- check statement

  XDWORD size = value.GetSize();
  if(!size)
    {
      DBSQLERROR* error = new DBSQLERROR(DBSQLERROR_TYPE_STATEMENT_ERROR);
      if(!error) return false;

      error->description.Set(__L("void statement"));
      database->GetErrorList()->Add(error);

      if(database->IsTransactionStarted())  database->Rollback();

      return false;
    }
   else
    {
        if(this->value.FindCharacter(__C('*'))!= NOTFOUND && this->value.Find(__L("COUNT(*)"),true)==XSTRING_NOTFOUND)
        {
          DBSQLSTRING error;
          database->ClearPreviousErrors();
          error.Set(__L("Wildcard * is discouraged and therefore disabled in this implementation"));
          this->database->Error(error.Get());
          return false;
        }


      bool success=true;

      DBSQLSTRING cursordeclare;

      if(this->database->IsTransactionStarted() && hascursor && this->cursorname.GetSize())
        {
          cursordeclare.Format(__L("DECLARE %s CURSOR FOR "),this->cursorname.Get());
        }

      DBSQLSTRING query;

      query.Add(cursordeclare);
      query.Add(value);

      XBUFFER* querystringbuffer=new XBUFFER();
      query.ConvertToUTF8   (*querystringbuffer );
      buffers.Add           (querystringbuffer  );

      if (!this->BindParametersToQuery())
      return false;

      int nParams = statementbindings.GetSize();

      PGresult* res=0;

      res= PQexecParams(conn,(char*)querystringbuffer->Get(),nParams,NULL,paramvalues,NULL,NULL,1);
      ExecStatusType exec=PQresultStatus(res);

      if((exec != PGRES_COMMAND_OK) && (exec != PGRES_TUPLES_OK))
        {
          database->ClearPreviousErrors();
          database->Error(PQerrorMessage(conn));
          success = false;
        }

      //------------------- clear previous statement

        UnbindAll();

        if(result)
          delete(result);

        result=NULL;

      //------------------- has tuples?


          char * cmdstatus=PQcmdStatus(res);
          XSTRING   statementtype;
                statementtype.Set(cmdstatus);


          //---------------------- prepare result
                if (statementtype.Find(__L("select"), true) != NOTFOUND)
                {
                  if (result)
                  {
                    delete(result);
                    result = NULL;
                  }

                  if (!result)    // prepare resultset
                  {
                    result = ConstructResult();

                    if (!result)
                    {
                      DBSQLERROR* error = new DBSQLERROR(DBSQLERROR_TYPE_MEMORY_ERROR);
                      if (!error) return false;

                      error->description.Set(__L("not enought memory for result"));
                      database->GetErrorList()->Add(error);

                      if (database->IsTransactionStarted())     database->Rollback();

                      ClearBuffers();
                      database->Unlock();
                      PQclear(res);
                      return 1;
                    }
                  }
                  static_cast<SQLPOSTGRESQLRESULT*>(this->result)->resultset = res;
                }
                else
                {
                  if (result)
                  {
                    delete(result);
                    result = NULL;
                  }
                  PQclear(res);
                }



    if (paramvalues)    delete(paramvalues);
    if (paramlengths)   delete(paramlengths);
    if (paramformats)   delete(paramformats);

    ClearBuffers();

    database->Unlock();
    return success;
  }

  database->Unlock();
  return true;
}




/*-------------------------------------------------------------------
//  SQLPOSTGRESQLQUERY::ConstructResult
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 12:27:13
//
//  @return       DBSQLRESULT* :
//
*/
/*-----------------------------------------------------------------*/
DBSQLRESULT* SQLPOSTGRESQLQUERY::ConstructResult()
{
  SQLPOSTGRESQLRESULT* result = new SQLPOSTGRESQLRESULT();
  result->query = this;

  return result;
}




/*-------------------------------------------------------------------
//  SQLPOSTGRESQLQUERY::BindParametersToQuery
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 12:27:18
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLQUERY::BindParametersToQuery()
{
  XDWORD nparams = this->value.CountCharacter(__C('$'));

  paramvalues  = NULL;
  paramlengths = NULL;
  paramformats = NULL;

  if(nparams>0)     //============================== there are params
    {
      //----------

      paramvalues=(char**)new char*[nparams];
      if(!paramvalues)
        {
          DBSQLERROR* error = new DBSQLERROR(DBSQLERROR_TYPE_MEMORY_ERROR);
          if(!error) return false;

          error->description.Set(__L("not enought memory for statements"));
          database->GetErrorList()->Add(error);

          if(database->IsTransactionStarted())  database->Rollback();

          return false;
        }

      //-----------

      paramlengths = new int[nparams];
      if(!paramlengths)
        {
          DBSQLERROR* error=new DBSQLERROR(DBSQLERROR_TYPE_MEMORY_ERROR);
          if(!error) return false;

          error->description.Set(__L("not enought memory for statements"));
          database->GetErrorList()->Add(error);

          if(database->IsTransactionStarted())  database->Rollback();

          delete(paramvalues);

          return false;
        }

      //-----------

      paramformats = new int[nparams];
      if(!paramformats)
        {
          DBSQLERROR* error = new DBSQLERROR(DBSQLERROR_TYPE_MEMORY_ERROR);
          if(!error) return false;

          error->description.Set(__L("not enought memory for statements"));
          database->GetErrorList()->Add(error);

          if(database->IsTransactionStarted()) database->Rollback();

          delete(paramvalues);
          delete(paramlengths);

          return false;
        }

      //------------

      for(XDWORD e=0;(e<nparams) && (e<this->statementbindings.GetSize());e++)
        {
          XSTRING bindstr;
          if(!statementbindings.Get(e)->ToString(bindstr))
            {
              DBSQLERROR* error = new DBSQLERROR(DBSQLERROR_TYPE_UNKNOWN);
              if(!error) return false;

              error->description.Set(__L("unknown type for binding"));
              database->GetErrorList()->Add(error);

              if (database->IsTransactionStarted())
                database->Rollback();
              return false;
            }

          XBUFFER* buf = new XBUFFER();
          if(buf)
            {
              bindstr.ConvertToUTF8(*buf);
              paramvalues [e]=(char*)buf->Get();
              buffers.Add(buf);
            }
        }

      //------------

    }

  return true;
}



/*-------------------------------------------------------------------
//  SQLPOSTGRESQLQUERY::BindParametersToResult
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 12:27:24
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLQUERY::BindParametersToResult()
{
  return false;
}



/*-------------------------------------------------------------------
//  SQLPOSTGRESQLQUERY::ClearCursor
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 12:54:46
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLQUERY::ClearCursor()
{
  bool success = false;

  if(hascursor && cursorname.GetSize()>0)
    {
      DBSQLSTRING close;
      close.Format(__L("CLOSE %s"),this->cursorname.Get());

      XSTRING_CREATEOEM(close,oem);

      PGresult* res = PQexec(conn, oem);
      if(PQresultStatus(res) != PGRES_COMMAND_OK) success=false; else success=true;
      PQclear(res);

      XSTRING_DELETEOEM(oem);
    }

  return success;
};



#endif
