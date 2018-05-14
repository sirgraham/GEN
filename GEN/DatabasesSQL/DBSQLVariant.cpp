
/*------------------------------------------------------------------------------------------
//  DBSQLVARIANT.CPP
//
//  Variant type
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 12/08/2015 13:49:37
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifdef DBSQL_ACTIVE

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DBSQLVariant.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DBSQLVARIANT::DBSQLVARIANT
*/
/**
//
//  Class Constructor DBSQLVARIANT
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      22/01/2016 16:29:00
//
//  @param        variant :
*/
/*-----------------------------------------------------------------*/
/*
DBSQLVARIANT::DBSQLVARIANT(DBSQLVARIANT& variant)
{
  Clean();
  *this=variant;
}
*/

/*-------------------------------------------------------------------
//  DBSQLVARIANT::DBSQLVARIANT
*/
/**
//
//  Class Constructor DBSQLVARIANT
//
//  @author       Abraham J. Velez
//  @version      22/01/2016 16:55:17
//
//  @param        variant :
*/
/*-----------------------------------------------------------------*/
DBSQLVARIANT::DBSQLVARIANT(const DBSQLVARIANT& variant) : XVARIANT()
{
  Clean();

  *this = variant;
}



/*-------------------------------------------------------------------
//  DBSQLVARIANT::~DBSQLVARIANT
*/
/**
//
//   Class Destructor DBSQLVARIANT
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/08/2015 13:50:25
//
*/
/*-----------------------------------------------------------------*/
DBSQLVARIANT::~DBSQLVARIANT ()
{
  if(data)  Destroy();

  Clean();
}


//-------------------------------------------- DBSQLROW-----------------------------------------






/*-------------------------------------------------------------------
//  DBSQLROW::~DBSQLROW
*/
/**
//
//   Class Destructor DBSQLROW
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      22/01/2016 16:42:53
//
*/
/*-----------------------------------------------------------------*/
DBSQLROW::~DBSQLROW()
{
  Clear();
}



/*-------------------------------------------------------------------
//  DBSQLROW::Get
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      22/01/2016 16:43:12
//
//  @return       DBSQLVARIANT :
//
//  @param        index :
*/
/*-----------------------------------------------------------------*/
DBSQLVARIANT DBSQLROW::Get(int index)
{
  if (this->row.Get(index)==NULL)
  {
    DBSQLVARIANT  result;
    return        result;
  }

  DBSQLVARIANT  copy  = *this->row.FastGet(index);

  return copy;
}


/*-------------------------------------------------------------------
//  DBSQLROW::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      22/01/2016 16:43:37
//
//  @return       bool :
//
//  @param        v :
*/
/*-----------------------------------------------------------------*/
bool DBSQLROW::Add (DBSQLVARIANT* v)
{
  return row.Add(v);
}

/*-------------------------------------------------------------------
//  DBSQLROW::Clear
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      22/01/2016 16:43:42
//
*/
/*-----------------------------------------------------------------*/
void  DBSQLROW::Clear()
{
row.DeleteContents();
row.DeleteAll();
}



#endif
