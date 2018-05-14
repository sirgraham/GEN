/*------------------------------------------------------------------------------------------
//  SQLSQLITEDATABASE.H
*/
/**
// \class
//
//  SQLite database concrete class
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 10/08/2015 12:26:06
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#if defined(DBSQL_ACTIVE) && defined(DBSQLSQLITE_ACTIVE)

#ifndef _SQLSQLITEDATABASE_H_
#define _SQLSQLITEDATABASE_H_

#ifndef SQLITE_CORE
#define SQLITE_CORE
#endif

/*---- INCLUDES --------------------------------------------------------------------------*/


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "sqlite3.h"

#include "DBSQLBase.h"
#include "DBSQLDatabase.h"

#include "SQLSQLITEQuery.h"
#include "SQLSQLITEConnection.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/



/*---- CLASS -----------------------------------------------------------------------------*/

//class SQLSQLITEQUERY;
//class SQLSQLITECONNECTION;


class SQLSQLITEDATABASE : public DBSQLDATABASE
{
  public:

    friend class                    SQLSQLITECONNECTION;
    friend class                    SQLSQLITEQUERY;


                                    SQLSQLITEDATABASE       () : DBSQLDATABASE()
                                    {
                                      Clean();

                                      type  = DBSQLDATABASE_FLAGS_SQL | DBSQLDATABASE_FLAGS_TRANSACTIONAL;
                                    }

    virtual                         ~SQLSQLITEDATABASE      ()                              { Clean();                                        }
    virtual XCHAR*                  GetName                 ()                              { return __L("SQLITE");                           }

    virtual bool                    Rollback                (XCHAR* savepointname = NULL);
    virtual bool                    Savepoint               (XCHAR* savepoint);
    virtual bool                    ReleaseSavepoint        (XCHAR* savepoint);
    virtual bool                    Transaction             ();
    virtual bool                    Commit                  ();
    virtual DBSQLQUERY*             CreateQuery             ();
    virtual DBSQLCONNECTION*        CreateConnection        ();
    virtual bool                    GetTables               ();

    virtual DBSQLDATABASE_ENCODING  GetDatabaseEncoding     ();
    virtual bool                    SetDatabaseEncoding     (DBSQLDATABASE_ENCODING encodingtouse);

  protected:

    sqlite3*                        sqlite3database;

  private:

    void                            Clean                   ()
                                    {
                                      type            = DBSQLDATABASE_FLAGS_NONE;
                                      connection      = NULL;
                                      sqlite3database = NULL;
                                    }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif


#endif
