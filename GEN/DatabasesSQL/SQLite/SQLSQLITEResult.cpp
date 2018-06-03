
/*------------------------------------------------------------------------------------------
//  SQLSQLITERESULT.CPP
//
//  SQLITE concrete result
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 11/08/2015 16:52:19
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#if defined(DBSQL_ACTIVE) && defined(DBSQLSQLITE_ACTIVE)

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XDebugTrace.h"

#include "SQLSQLITEResult.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  SQLSQLITERESULT::~SQLSQLITERESULT
*/
/**
//
//   Class Destructor SQLSQLITERESULT
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/08/2015 17:53:56
//
*/
/*-----------------------------------------------------------------*/
SQLSQLITERESULT::~SQLSQLITERESULT ()
{
  Clean();

  row.Clear();

}




/*-------------------------------------------------------------------
//  SQLSQLITERESULT::GetNumRows
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2015 17:35:53
//
//  @return       XDWORD :
//
//  NOTE: sqlite doesn't know the number of rows of a resultset, as it is fetched one at a time, return -1 to notify upper instance
*/
/*-----------------------------------------------------------------*/
XQWORD SQLSQLITERESULT::GetNumRows()
{
  return -1;
}

/*-------------------------------------------------------------------
//  SQLSQLITERESULT::GetNumColumns
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2015 17:35:58
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XQWORD SQLSQLITERESULT::GetNumColumns()
{
  return sqlite3_column_count(query->ppstmt);
}



/*-------------------------------------------------------------------
//  SQLSQLITERESULT::Seek
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2015 16:54:33
//
//  @return       bool :
//
//  @param        i :
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITERESULT::Seek(int index)
{
  // SQLITE Can't seek a result set
  return false;
}



/*-------------------------------------------------------------------
//  SQLSQLITERESULT::First
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2015 16:54:39
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITERESULT::First ()
{
  // SQLITE Can't seek a result set but, if not started, we can get first step as first row
  if(!initialized)
    {
      Next();
      GetRow();

      initialized = false;
      currentrow  = 0;

      return true;
    }

  return false;
}




/*-------------------------------------------------------------------
//  SQLSQLITERESULT::Last
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2015 16:54:44
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITERESULT::Last    ()
{
  // SQLITE Can't seek a result set
  return false;
}



/*-------------------------------------------------------------------
//  SQLSQLITERESULT::Next
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2015 16:54:53
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITERESULT::Next    ()
{
  int RC = sqlite3_step(query->ppstmt);

  lastresult = RC;

  if(RC!=SQLITE_ROW && RC!=SQLITE_DONE) return false;

  currentrow++;

  return true;
}




/*-------------------------------------------------------------------
//  SQLSQLITERESULT::ProcessRow
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/08/2015 10:06:55
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITERESULT::ProcessRow()
{
  row.Clear();
  header.DeleteContents();
  header.DeleteAll();

  if (!HasNext())
    return true;

  int ncol = (int)this->GetNumColumns();

  for(int e=0; e<ncol; e++)
    {
      DBSQLVARIANT* variant=new DBSQLVARIANT();
      if(!variant)    return false;

      DBSQLSTRING columnvalue;
      char* text=(char*)sqlite3_column_text(query->ppstmt, (int)e);
      int   len = sqlite3_column_bytes(query->ppstmt, (int)e);
      columnvalue.Set(text);



      DBSQLSTRING* columnname=new DBSQLSTRING();
      if(!columnname)
        {
          delete(variant);
          return false;
        }

      columnname->Set(sqlite3_column_name(query->ppstmt, (int)e));

      int sqtype=sqlite3_column_type(query->ppstmt, (int)e);
      switch(sqtype)
        {
          case SQLITE_INTEGER   : { int i = columnvalue.ConvertToInt();
                                    (*variant) = i;
                                  }
                                  break;

          case SQLITE_FLOAT     : { float i = columnvalue.ConvertToFloat();
                                    (*variant) = i;
                                  }
                                  break;

          case SQLITE_BLOB      : { //unsupported
                                   XDEBUGTRACE_PRINTCOLOR(0,__L("Unsupported BLOB type"));
                                  }
                                  break;

          case SQLITE_NULL      : {
                                  }
                                  break;

          case SQLITE_TEXT      : {
                                    XSTRING received;
                                    received.ConvertFromUTF8((XBYTE*)text,len);
                                    (*variant) = received;
                                  }
                                  break;
        }

      header.Add(columnname);
      row.Add(variant);
    }

  return true;
}




/*-------------------------------------------------------------------
//  SQLSQLITERESULT::Abort
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/08/2015 10:56:46
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITERESULT::Abort()
{
  return false;
}



/*-------------------------------------------------------------------
//  SQLSQLITERESULT::HasNext
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/08/2015 10:56:51
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLSQLITERESULT::HasNext()
{
  return (lastresult == SQLITE_ROW);
}

#endif

