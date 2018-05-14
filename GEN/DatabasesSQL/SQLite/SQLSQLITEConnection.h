/*------------------------------------------------------------------------------------------
//  SQLSQLITECONNECTION.H
*/
/**
// \class
//
//  Conection to SQLite DB
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 10/08/2015 12:15:38
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#if defined(DBSQL_ACTIVE) && defined(DBSQLSQLITE_ACTIVE)

#ifndef _SQLSQLITECONNECTION_H_
#define _SQLSQLITECONNECTION_H_

#ifndef SQLITE_CORE
#define SQLITE_CORE
#endif


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DBSQLConnection.h"
#include "DBSQLDatabase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class SQLSQLITECONNECTION : public DBSQLCONNECTION
{
  public:
                          SQLSQLITECONNECTION       (DBSQLDATABASE* database) : DBSQLCONNECTION(database)
                          {
                            Clean();
                          }

    virtual              ~SQLSQLITECONNECTION       ()                { Clean();                          }

    virtual bool          Connect                   ();
    virtual bool          Disconnect                ();

  protected:

  private:

    void                  Clean                     ()                {                                   }

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif


#endif
