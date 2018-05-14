/*------------------------------------------------------------------------------------------
//  SQLMYSQLVARIANT.H
*/
/**
// \class
//
//  Variant type
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 12/08/2015 13:48:07
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#if defined(DBSQL_ACTIVE) && defined(DBSQLMYSQL_ACTIVE)

#ifndef _SQLMYSQLVARIANT_H_
#define _SQLMYSQLVARIANT_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DBSQLVariant.h"

#include "mysql.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

class SQLMYSQLVARIANT : public DBSQLVARIANT
{
  public:
                                  SQLMYSQLVARIANT     () : DBSQLVARIANT()     {  }
    virtual DBSQLVARIANT&         operator =          (DBSQLDATETIME& datetime);

    virtual operator              DBSQLDATETIME       ();
    virtual XDWORD                GetSize             ();
    virtual void*                 GetData             ();

    MYSQL_TIME                    mysqltime;
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

#endif

