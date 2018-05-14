/*------------------------------------------------------------------------------------------
//  SQLSQLITERESULT.H
*/
/**
// \class
//
//  SQLITE concrete result set
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 11/08/2015 16:39:01
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#if defined(DBSQL_ACTIVE) && defined(DBSQLSQLITE_ACTIVE)

#ifndef _SQLSQLITERESULT_H_
#define _SQLSQLITERESULT_H_

#ifndef SQLITE_CORE
#define SQLITE_CORE
#endif

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DBSQLBase.h"
#include "DBSQLResult.h"

#include "SQLSQLITEQuery.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class SQLSQLITEQUERY;

class SQLSQLITERESULT : public DBSQLRESULT
{
  public:

    friend class            SQLSQLITEQUERY;

    virtual                ~SQLSQLITERESULT         ();

    virtual XQWORD          GetNumRows              ();
    virtual XQWORD          GetNumColumns           ();
    virtual bool            Seek                    (int index);
    virtual bool            First                   ();
    virtual bool            Last                    ();
    virtual bool            Next                    ();
    virtual bool            ProcessRow              ();
    virtual bool            Abort                   ();
    virtual bool            HasNext                 ();

  protected:
                            SQLSQLITERESULT         () : DBSQLRESULT()      { Clean();                          }

    SQLSQLITEQUERY*         query;
    int                     lastresult;

  private:

    void                    Clean                   ()
                            {
                              lastresult  = SQLITE_ROW;
                              query       = NULL;
                            }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/



#endif

#endif
