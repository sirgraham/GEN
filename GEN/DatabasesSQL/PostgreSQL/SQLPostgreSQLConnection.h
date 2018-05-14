/*------------------------------------------------------------------------------------------
//  SQLPOSTGRESQLSCONNECTION.H
*/
/**
// \class
//
//  Conection to PostgreSQL
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 10/08/2015 12:15:38
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#if defined(DBSQL_ACTIVE) && defined(DBSQLPOSTGRESQL_ACTIVE)

#ifndef _SQLPOSTGRESQLSCONNECTION_H_
#define _SQLPOSTGRESQLSCONNECTION_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "libpq-fe.h"

#include "DBSQLConnection.h"
#include "DBSQLDatabase.h"

#include "SQLPostgreSQLQuery.h"
#include "SQLPostgreSQLDatabase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

//class SQLPOSTGRESQLQUERY;
//class SQLPOSTGRESQLDATABASE;

class SQLPOSTGRESQLCONNECTION : public DBSQLCONNECTION
{
  public:

    friend class                  SQLPOSTGRESQLQUERY;
    friend class                  SQLPOSTGRESQLDATABASE;

                                  SQLPOSTGRESQLCONNECTION   (DBSQLDATABASE* database) : DBSQLCONNECTION(database)
                                  {
                                    Clean();
                                  }

    virtual                      ~SQLPOSTGRESQLCONNECTION   ()
                                  {
                                    Clean();
                                  }

    virtual bool                  Connect                   ();
    virtual bool                  Disconnect                ();

  protected:

    PGconn*                       connection;

  private:

    void                          Clean                     ()
                                  {

                                  }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif


#endif
