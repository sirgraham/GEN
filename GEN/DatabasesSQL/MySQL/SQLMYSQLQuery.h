/*------------------------------------------------------------------------------------------
//  SQLMYSQLQUERY.H
*/
/**
// \class
//
//  MYSQL query concrete class
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 11/08/2015 9:50:39
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#if defined(DBSQL_ACTIVE) && defined(DBSQLMYSQL_ACTIVE)

#ifndef _SQLMYSQLQUERY_H_
#define _SQLMYSQLQUERY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DBSQLQuery.h"
#include "DBSQLDatabase.h"
#include "DBSQLResult.h"

#include "SQLMYSQLResult.h"

#include "mysql.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class SQLMYSQLRESULT;


class SQLMYSQLQUERY : public DBSQLQUERY
{
  public:

    friend class                SQLMYSQLRESULT;

                                SQLMYSQLQUERY               (DBSQLDATABASE* database) : DBSQLQUERY(database)
                                {
                                  Clean();
                                }

    virtual                    ~SQLMYSQLQUERY               ();

    virtual bool                Execute                     ();
    virtual bool                UnbindAll                   ();

    virtual bool                Bind                        (XDWORD ID, DBSQLDATETIME& datetime);

  protected:

    virtual bool                Exec                        (MYSQL* db, char* sql);

    virtual DBSQLRESULT*        ConstructResult             ();
    virtual bool                BindParametersToQuery       ();
    virtual bool                BindParametersToResult      ();


  private:

    void                        Clean                       ()
                                {
                                  stmt    = NULL;
                                  mysqldb = NULL;
                                  param   = NULL;
                                  result  = NULL;
                                }

    MYSQL_STMT*                 stmt;
    MYSQL*                      mysqldb;
    MYSQL_BIND*                 param;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif


#endif


