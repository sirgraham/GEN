
/*------------------------------------------------------------------------------------------
//  DBSQLQUERY.CPP
//
//  Database Abstratct Query
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 12/08/2015 13:33:12
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#ifdef DBSQL_ACTIVE

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DBSQLResult.h"
#include "DBSQLDatabase.h"

#include "DBSQLQuery.h"

#include "XBuffer.h"
#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DBSQLQUERY::~DBSQLQUERY
*/
/**
//
//   Class Destructor DBSQLQUERY
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/08/2015 13:33:47
//
*/
/*-----------------------------------------------------------------*/
DBSQLQUERY::~DBSQLQUERY()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DBSQLQUERY::Set
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 11:26:01
//
//  @return       bool :
//
//  @param        query :
*/
/*-----------------------------------------------------------------*/
bool DBSQLQUERY::Set(XCHAR* query)
{
  UnbindAll();

  value.Set(query);

  return IsWellConstructedQuery();
}



/*-------------------------------------------------------------------
//  DBSQLQUERY::UnbindAll
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 11:26:35
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DBSQLQUERY::UnbindAll()
{
  return (statementbindings.DeleteElementContents() && statementbindings.DeleteAll());
}



/*-------------------------------------------------------------------
//  DBSQLQUERY::Bind
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 11:27:04
//
//  @return       bool :
//
//  @param        ID  :
//  @param        variant :
*/
/*-----------------------------------------------------------------*/
bool DBSQLQUERY::Bind(XDWORD ID, DBSQLVARIANT* variant)
{
  return statementbindings.Set(ID, variant);
}



/*-------------------------------------------------------------------
//  DBSQLQUERY::Bind
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 11:28:17
//
//  @return       bool :
//
//  @param        ID :
//  @param        integer :
*/
/*-----------------------------------------------------------------*/
bool DBSQLQUERY::Bind(XDWORD ID, int integer)
{
  DBSQLVARIANT* variant = new DBSQLVARIANT();
  if(!variant) return false;

  (*variant) = integer;

  return statementbindings.Add(ID, variant);
}



/*-------------------------------------------------------------------
//  DBSQLQUERY::Bind
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 11:28:25
//
//  @return       bool :
//
//  @param        id :
//  @param        i :
*/
/*-----------------------------------------------------------------*/
bool DBSQLQUERY::Bind(XDWORD ID, XCHAR* string)
{
  DBSQLVARIANT* variant = new DBSQLVARIANT();
  if(!variant) return false;

  (*variant) = string;

  return statementbindings.Add(ID, variant);
}




/*-------------------------------------------------------------------
//  DBSQLQUERY::Bind
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 11:28:32
//
//  @return       bool :
//
//  @param        id :
//  @param        i :
*/
/*-----------------------------------------------------------------*/
bool DBSQLQUERY::Bind(XDWORD ID, float decimal)
{
  DBSQLVARIANT* variant = new DBSQLVARIANT();
  if(!variant) return false;

  (*variant) = decimal;

  return statementbindings.Add(ID, variant);
}




/*-------------------------------------------------------------------
//  DBSQLQUERY::Bind
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 11:28:39
//
//  @return       bool :
//
//  @param        id :
//  @param        i :
*/
/*-----------------------------------------------------------------*/
bool DBSQLQUERY::Bind(XDWORD ID, char* string)
{
  DBSQLVARIANT* variant = new DBSQLVARIANT();
  if(!variant) return false;

  (*variant) = string;

  return statementbindings.Add(ID, variant);
}




/*-------------------------------------------------------------------
//  DBSQLQUERY::Bind
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2015 10:44:13
//
//  @return       bool :
//
//  @param        id :
//  @param        datetime :
*/
/*-----------------------------------------------------------------*/
bool DBSQLQUERY::Bind(XDWORD id, XDATETIME& datetime)
{
  DBSQLVARIANT* variant = new DBSQLVARIANT();
  if(!variant) return false;

  (*variant) = datetime;

  return statementbindings.Add(id,variant);
}




/*-------------------------------------------------------------------
//  DBSQLQUERY::ClearBuffers
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 11:27:59
//
*/
/*-----------------------------------------------------------------*/
void DBSQLQUERY::ClearBuffers()
{
  for(XDWORD e=0; e<buffers.GetSize(); e++)
    {
      XBUFFER * buffer = buffers.Get(e);
      delete buffer;
    }

  buffers.DeleteAll();
}



/*-------------------------------------------------------------------
//  DBSQLQUERY::UseCursor
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 12:50:19
//
//  @return       bool :
//
//  @param        cursor :
*/
/*-----------------------------------------------------------------*/
bool DBSQLQUERY::UseCursor (XCHAR* cursorname)
{
  this->cursorname.Set(cursorname);

  hascursor=true;

  return true;
}




/*-------------------------------------------------------------------
//  DBSQLQUERY::GetOneResult
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/11/2015 9:25:14
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DBSQLQUERY::GetOneResult()
{
  DBSQLRESULT* result = GetResult();
  if(!result) return false;

  result->First();
  result->ProcessRow();

  DBSQLROW* row = result->GetRow();
  if(row)
    {
      int found = (row->Get(0));
      return (found!=0)?true:false;
    }

  return false;
}



#endif
