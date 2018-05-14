/*------------------------------------------------------------------------------------------
//  SQLSQLITEQUERY.H
*/
/**
// \class
//
//  SQLITE query concrete class
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 11/08/2015 9:50:39
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#if defined(DBSQL_ACTIVE) && defined(DBSQLSQLITE_ACTIVE)

#ifndef _SQLSQLITEQUERY_H_
#define _SQLSQLITEQUERY_H_

#ifndef SQLITE_CORE
#define SQLITE_CORE
#endif


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "sqlite3.h"

#include "DBSQLQuery.h"
#include "DBSQLDatabase.h"
#include "SQLSQLITEResult.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/


class SQLSQLITEQUERY : public DBSQLQUERY
{
  public:
    friend class            SQLSQLITERESULT;

                            SQLSQLITEQUERY            (DBSQLDATABASE* database) : DBSQLQUERY(database)
                            {
                              Clean();
                            }

    virtual                ~SQLSQLITEQUERY            ();

    virtual bool            Execute                   ();
    virtual bool            ClearCursor               ()                                                { return false;                             }

  protected:

    virtual bool            Bind                      (XDWORD ID, int integer)                          { return DBSQLQUERY::Bind(ID+1, integer);   }
    virtual bool            Bind                      (XDWORD ID, XCHAR* string)                        { return DBSQLQUERY::Bind(ID+1, string);    }
    virtual bool            Bind                      (XDWORD ID, float decimal)                        { return DBSQLQUERY::Bind(ID+1, decimal );  }
    virtual bool            Bind                      (XDWORD ID, XDATETIME& datetime)                  { return DBSQLQUERY::Bind(ID+1, datetime);  }
    virtual bool            IsWellConstructedQuery    ()                                                { return true;                              }   //pending checks

    virtual DBSQLRESULT*    ConstructResult           ();
    virtual bool            BindParametersToQuery     ();

    int                     Exec                      (const char* zsql,int size);

    sqlite3_stmt*           ppstmt;

  private:

    void                    Clean                     ()
                            {
                              ppstmt = NULL;
                            }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

#endif
