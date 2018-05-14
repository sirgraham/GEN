/*------------------------------------------------------------------------------------------
//  DBSQLQUERY.H
*/
/**
// \class
//
//  SQL Query Abstract base class
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 10/08/2015 15:50:52
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifdef DBSQL_ACTIVE

#ifndef _DBSQLQUERY_H_
#define _DBSQLQUERY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DBSQLBase.h"
#include "DBSQLVariant.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class DBSQLRESULT;
class DBSQLDATABASE;

class DBSQLQUERY
{
  public:
                                          DBSQLQUERY              (DBSQLDATABASE* database)
                                          {
                                            Clean();

                                            this->database = database;
                                          }

    virtual                              ~DBSQLQUERY              ();

    virtual   bool                        Execute                 ()  = 0;
    virtual   bool                        ClearCursor             ()                                  { return false;                   }

    virtual   bool                        IsWellConstructedQuery  ()                                  { return true;                    }
    virtual   DBSQLRESULT*                GetResult               ()                                  { return result;                  }
    virtual   DBSQLSTRING*                GetValue                ()                                  { return &value;                  }
    virtual   bool                        Set                     (XCHAR* query);

    virtual   bool                        Bind                    (XDWORD ID, DBSQLVARIANT* variant);
    virtual   bool                        Bind                    (XDWORD ID, int integer);
    virtual   bool                        Bind                    (XDWORD ID, XCHAR* string);
    virtual   bool                        Bind                    (XDWORD ID, float decimal);
    virtual   bool                        Bind                    (XDWORD ID, char* string);
    virtual   bool                        Bind                    (XDWORD ID, XDATETIME& datatime);

    virtual   bool                        UnbindAll               ();
    virtual   void                        ClearBuffers            ();

    virtual   bool                        UseCursor               (XCHAR* cursor);

    bool                                  GetOneResult            ();

  protected:

    virtual   DBSQLRESULT*                ConstructResult         ()  = 0;
    virtual   bool                        BindParametersToQuery   ()  = 0;


    DBSQLDATABASE*                        database;
    DBSQLSTRING                           value;
    DBSQLRESULT*                          result;

    DBSQLMAP<XDWORD,DBSQLVARIANT*>        statementbindings;
    DBSQLVECTOR<XBUFFER*>                 buffers;

    DBSQLSTRING                           cursorname;
    bool                                  hascursor;

    bool                                  busyflag;

  private:

    void                                  Clean                   ()
                                          {
                                            database    = NULL;
                                            result      = NULL;

                                            statementbindings.DeleteElementContents();
                                            statementbindings.DeleteAll();

                                            value.Empty();
                                          }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

#endif
