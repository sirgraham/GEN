/*------------------------------------------------------------------------------------------
//  SQLPOSTGRESQLDATABASE.H
*/
/**
// \class
//
//  Postgres database concrete class
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 10/08/2015 12:26:06
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#if defined(DBSQL_ACTIVE) && defined(DBSQLPOSTGRESQL_ACTIVE)

#ifndef _SQLPOSTGRESQLDATABASE_H_
#define _SQLPOSTGRESQLDATABASE_H_

#pragma comment (linker,"/NODEFAULTLIB:libcmtd.lib")

/*---- INCLUDES --------------------------------------------------------------------------*/


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XDebugTrace.h"

#include "DBSQLBase.h"
#include "DBSQLDatabase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#include "SQLPostgreSQLQuery.h"
#include "SQLPostgreSQLConnection.h"


/*---- CLASS -----------------------------------------------------------------------------*/

//class SQLPOSTGRESQLQUERY;
//class SQLPOSTGRESQLCONNECTION;


class SQLPOSTGRESQLDATABASE : public DBSQLDATABASE
{
  public:

    friend class                    SQLPOSTGRESQLCONNECTION;
    friend class                    SQLPOSTGRESQLQUERY;


                                    SQLPOSTGRESQLDATABASE       ();
    virtual                         ~SQLPOSTGRESQLDATABASE      ()                        { Clean();                                  }
    virtual XCHAR*                  GetName                     ()                        { return __L("POSTGRESQL");                 }

    virtual bool                    Transaction                 ();
    virtual bool                    Commit                      ();
    virtual bool                    Rollback                    (XCHAR* savepointname=NULL);
    virtual bool                    Savepoint                   (XCHAR* savepoint);
    virtual bool                    ReleaseSavepoint            (XCHAR* savepoint);
    virtual DBSQLQUERY*             CreateQuery                 ();
    virtual DBSQLCONNECTION*        CreateConnection            ();
    virtual bool                    GetTables                   ();
    virtual DBSQLDATABASE_ENCODING  GetDatabaseEncoding         ();
    virtual bool                    SetDatabaseEncoding         (DBSQLDATABASE_ENCODING encodingtouse);

    virtual   bool                  OnConnection                ();
    virtual   bool                  OnDisconnection             ();

    virtual   bool                  IsThreadSafe                ();


  protected:

  private:

    void                            Clean                       ()
                                    {
                                      type        = DBSQLDATABASE_FLAGS_NONE;
                                      connection  = NULL;
                                    }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

#endif

