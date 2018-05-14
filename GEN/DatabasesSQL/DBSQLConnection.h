/*------------------------------------------------------------------------------------------
//  DBSQLCONNECTION.H
*/
/**
// \class
//
//  Holds connection data for database
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 10/08/2015 11:24:00
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifdef DBSQL_ACTIVE


#ifndef _DBSQLCONNECTION_H_
#define _DBSQLCONNECTION_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DBSQLBase.h"
#include "DBSQLDatabase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class DBSQLDATABASE;

class DBSQLCONNECTION
{
  public:

                                            DBSQLCONNECTION       (DBSQLDATABASE* database);
    virtual                                ~DBSQLCONNECTION       ();

    virtual     bool                        Connect               ()  = 0;
    virtual     bool                        Disconnect            ()  = 0;

    virtual     bool                        SetOption             (XCHAR* key, XCHAR* value);
    virtual     bool                        FindOption            (XCHAR* key, DBSQLSTRING* result);

  protected:
                                            DBSQLCONNECTION       ()                               {  Clean();                }

    DBSQLDATABASE*                          database;
    DBSQLMAP<DBSQLSTRING*,DBSQLSTRING*>     options;


  private:

    void                                    Clean                 ()
                                            {
                                              database = NULL;
                                            }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif


#endif
