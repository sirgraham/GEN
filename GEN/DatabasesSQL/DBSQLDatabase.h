/*------------------------------------------------------------------------------------------
//  DBSQLDATABASE.H
*/
/**
// \file
//
//  Holds database data
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 10/08/2015 11:24:24
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifdef DBSQL_ACTIVE

#ifndef _DBSQLDATABASE_H_
#define _DBSQLDATABASE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"
#include "XString.h"

#include "DBSQLBase.h"
#include "DBSQLConnection.h"
#include "DBSQLError.h"
#include "DBSQLQuery.h"
#include "DBSQLVariant.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum DBSQLDATABASE_FLAGS
{
  DBSQLDATABASE_FLAGS_NONE            = 0     ,
  DBSQLDATABASE_FLAGS_SQL             = 1<<0  ,
  DBSQLDATABASE_FLAGS_TRANSACTIONAL   = 1<<1  ,
  DBSQLDATABASE_FLAGS_SERVERBASED     = 1<<2
};


enum DBSQLDATABASE_STATUS
{
  DBSQLDATABASE_FLAGS_UNKNOWN         = 0     ,
  DBSQLDATABASE_FLAGS_CONNECTED               ,
  DBSQLDATABASE_FLAGS_DISCONNECTED
};


enum DBSQLDATABASE_ENCODING
{
  DBSQLDATABASE_ENCODING_UNKNOWN      = -1    ,
  DBSQLDATABASE_ENCODING_SQL_ASCII            , //  unspecified (see text)              any
  DBSQLDATABASE_ENCODING_EUC_JP               , //  Extended UNIX Code-JP               Japanese
  DBSQLDATABASE_ENCODING_EUC_CN               , //  Extended UNIX Code-CN               Simplified  Chinese
  DBSQLDATABASE_ENCODING_EUC_KR               , //  Extended UNIX Code-KR               Korean
  DBSQLDATABASE_ENCODING_EUC_TW               , //  Extended UNIX Code-TW               Traditional Chinese, Taiwanese
  DBSQLDATABASE_ENCODING_EUC_JIS_2004         , //  Extended UNIX Code-JP, JIS X 0213   Japanese
  DBSQLDATABASE_ENCODING_UTF8                 , //  Unicode, 8-bit                      all
  DBSQLDATABASE_ENCODING_MULE_INTERNAL        , //  Mule internal code                  Multilingual
  DBSQLDATABASE_ENCODING_LATIN1               , //  ISO 8859-1, ECMA 94                 Western European
  DBSQLDATABASE_ENCODING_LATIN2               , //  ISO 8859-2, ECMA 94                 Central European
  DBSQLDATABASE_ENCODING_LATIN3               , //  ISO 8859-3, ECMA 94                 South European
  DBSQLDATABASE_ENCODING_LATIN4               , //  ISO 8859-4, ECMA 94                 North European
  DBSQLDATABASE_ENCODING_LATIN5               , //  ISO 8859-9, ECMA 128                Turkish
  DBSQLDATABASE_ENCODING_LATIN6               , //  ISO 8859-10,ECMA 144                Nordic
  DBSQLDATABASE_ENCODING_LATIN7               , //  ISO 8859-13                         Baltic
  DBSQLDATABASE_ENCODING_LATIN8               , //  ISO 8859-14                         Celtic
  DBSQLDATABASE_ENCODING_LATIN9               , //  ISO 8859-15                         LATIN1 with Euro and accents
  DBSQLDATABASE_ENCODING_LATIN10              , //  ISO 8859-16,ASRO SR 14111           Romanian
  DBSQLDATABASE_ENCODING_WIN1256              , //  Windows CP1256                      Arabic
  DBSQLDATABASE_ENCODING_WIN1258              , //  Windows CP1258                      Vietnamese
  DBSQLDATABASE_ENCODING_WIN866               , //  Windows CP866                       Cyrillic
  DBSQLDATABASE_ENCODING_WIN874               , //  Windows CP874                       Thai
  DBSQLDATABASE_ENCODING_KOI8R                , //  KOI8-R                              Cyrillic (Russian)
  DBSQLDATABASE_ENCODING_WIN1251              , //  Windows CP1251                      Cyrillic
  DBSQLDATABASE_ENCODING_WIN1252              , //  Windows CP1252                      Western European
  DBSQLDATABASE_ENCODING_ISO_8859_5           , //  ISO 8859-5, ECMA 113                Latin/Cyrillic  Yes 1
  DBSQLDATABASE_ENCODING_ISO_8859_6           , //  ISO 8859-6, ECMA 114                Latin/Arabic  Yes 1
  DBSQLDATABASE_ENCODING_ISO_8859_7           , //  ISO 8859-7, ECMA 118                Latin/Greek Yes 1
  DBSQLDATABASE_ENCODING_ISO_8859_8           , //  ISO 8859-8, ECMA 121                Latin/Hebrew  Yes 1
  DBSQLDATABASE_ENCODING_WIN1250              , //  Windows CP1250                      Central European
  DBSQLDATABASE_ENCODING_WIN1253              , //  Windows CP1253                      Greek
  DBSQLDATABASE_ENCODING_WIN1254              , //  Windows CP1254                      Turkish
  DBSQLDATABASE_ENCODING_WIN1255              , //  Windows CP1255                      Hebrew
  DBSQLDATABASE_ENCODING_WIN1257              , //  Windows CP1257                      Baltic
  DBSQLDATABASE_ENCODING_KOI8U                , //  KOI8-U                              Cyrillic (Ukrainian)
  DBSQLDATABASE_ENCODING_SJIS                 , //  Shift JIS                           Japanese
  DBSQLDATABASE_ENCODING_BIG5                 , //  Big Five                            Traditional Chinese
  DBSQLDATABASE_ENCODING_GBK                  , //  Extended National                   Standard  Simplified Chinese
  DBSQLDATABASE_ENCODING_UHC                  , //  Unified Hangul Code                 Korean
  DBSQLDATABASE_ENCODING_GB18030              , //  National Standard                   Chinese
  DBSQLDATABASE_ENCODING_JOHAB                , //  JOHAB                               Korean (Hangul)
  DBSQLDATABASE_ENCODING_SHIFT_JIS_2004       , //  Shift JIS, JIS X 0213               Japanese

  DBSQLDATABASE_ENCODING__LAST_ENCODING       ,
  DBSQLDATABASE_ENCODING_BE_LAST              = DBSQLDATABASE_ENCODING_KOI8U
};


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XMUTEX;
class DBSQLCONNECTION;

class DBSQLDATABASE
{
  public:

    friend  class                             DBSQLVARIANT;
    friend  class                             DBSQLQUERY;



                                              DBSQLDATABASE         ();
    virtual                                  ~DBSQLDATABASE         ();

    //--- abstract --

    virtual   bool                            Transaction           ()                                      = 0;
    virtual   bool                            Commit                ()                                      = 0;
    virtual   bool                            Rollback              (XCHAR* savepointname = NULL)           = 0;
    virtual   bool                            Savepoint             (XCHAR* savepoint)                      = 0;
    virtual   bool                            ReleaseSavepoint      (XCHAR* savepoint)                      = 0;

    virtual   DBSQLQUERY*                     CreateQuery           ()                                      = 0;
    virtual   DBSQLCONNECTION*                CreateConnection      ()                                      = 0;
    virtual   bool                            GetTables             ()                                      = 0;

    virtual   DBSQLDATABASE_ENCODING          GetDatabaseEncoding   ()                                      = 0;
    virtual   bool                            SetDatabaseEncoding   (DBSQLDATABASE_ENCODING encodingtouse)  = 0;

    virtual   DBSQLDATABASE_ENCODING          GetCurrentEncoding    ()                                  { return this->encodingtype;                              }
    virtual   XCHAR*                          GetName               ()                                  { return __L("Undefined");                                }

    virtual   bool                            IsOpen                ()                                  { return (status == DBSQLDATABASE_FLAGS_CONNECTED);       }
    virtual   bool                            SetConnection         (DBSQLCONNECTION* connection)       { this->connection = connection;  return true;            }

    virtual   bool                            IsTransactionStarted  ()                                  { return transactionstarted;                              }

    virtual   DBSQLVECTOR<DBSQLSTRING*>*      GetTableList          ()                                  { return &tables;                                         }
    virtual   DBSQLVECTOR<DBSQLERROR*>*       GetErrorList          ()                                  { return &errorstack;                                     }
    virtual   DBSQLVARIANT*                   CreateVariant         ();

    virtual   bool                            OnConnection          ();
    virtual   bool                            OnDisconnection       ();

    virtual   bool                            Open                  ();
    virtual   bool                            Close                 ();

    virtual   XCHAR*                          GetLastError          ();

    virtual   bool                            Execute               (DBSQLQUERY* constructedquery);

    virtual   void                            Error                 (XCHAR* errorstring);
    virtual   void                            Error                 (char* errorstring);
    virtual   void                            ClearPreviousErrors   ();


    //-------------- thread safety

    virtual   bool                            IsThreadSafe          ()                                  { return threadsafe;                                        }

    virtual   bool                            Lock                  ();
    virtual   bool                            Unlock                ();


    XMUTEX*                                   mutex;
    bool                                      threadsafe;

protected:

    virtual   bool                            ExecuteQuery          (DBSQLQUERY* wellconstructedquery);

    bool                                      transactionstarted;
    DBSQLCONNECTION*                          connection;
    XDWORD                                    type;
    DBSQLDATABASE_STATUS                      status;
    DBSQLVECTOR<DBSQLSTRING*>                 tables;
    DBSQLVECTOR<DBSQLERROR*>                  errorstack;

    DBSQLDATABASE_ENCODING                    encodingtype;

  private:

    void                                      Clean                 ()
                                              {
                                                connection        = 0;
                                                type              = 0;
                                                transactionstarted= false;
                                                status            = DBSQLDATABASE_FLAGS_UNKNOWN;
                                                encodingtype      = DBSQLDATABASE_ENCODING_UNKNOWN;
                                                threadsafe        = false;
                                              }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif


#endif
