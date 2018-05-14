
/*------------------------------------------------------------------------------------------
//  DBSQLCONNECTION.CPP
//
//  Connection abstract
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 08/09/2015 10:01:12
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#ifdef DBSQL_ACTIVE

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XThread.h"
#include "XFactory.h"
#include "XThread.h"

#include "DBSQLConnection.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DBSQLCONNECTION::DBSQLCONNECTION
*/
/**
//
//   Class Constructor DBSQLCONNECTION
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/08/2015 12:55:43
//
*/
/*-----------------------------------------------------------------*/
DBSQLCONNECTION::DBSQLCONNECTION(DBSQLDATABASE* database)
{
  Clean();

  this->database = database;

  database->SetConnection(this);

  options.SetIsMulti(false);
}



/*-------------------------------------------------------------------
//  DBSQLCONNECTION::~DBSQLCONNECTION
*/
/**
//
//   Class Destructor DBSQLCONNECTION
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 10:02:07
//
*/
/*-----------------------------------------------------------------*/
DBSQLCONNECTION::~DBSQLCONNECTION()
{
  options.DeleteKeyContents();
  options.DeleteElementContents();

  options.DeleteAll();

  Clean();
}



/*-------------------------------------------------------------------
//  DBSQLCONNECTION::SetOption
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 10:03:35
//
//  @return       bool :
//
//  @param        key :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool DBSQLCONNECTION::SetOption(XCHAR* key, XCHAR* value)
{
  DBSQLSTRING* newkey = new DBSQLSTRING();
  if(newkey)
    {
      DBSQLSTRING* newvalue = new DBSQLSTRING();
      if(newvalue)
        {
          newkey->Set   (key);
          newvalue->Set (value);

          return options.Add(newkey,newvalue);
        }

      delete newkey;
    }

  return false;
}




/*-------------------------------------------------------------------
//  DBSQLCONNECTION::FindOption
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      08/09/2015 10:04:11
//
//  @return       bool :
//
//  @param        key :
//  @param        result :
*/
/*-----------------------------------------------------------------*/
bool  DBSQLCONNECTION::FindOption(XCHAR* key, DBSQLSTRING* result)
{
  for(XDWORD e=0; e<this->options.GetSize(); e++)
    {
      if(!options.GetKey(e)->Compare(key))
        {
          result->Set(options.GetElement(e)->Get());

          return true;
        }
    }

  return false;
}

#endif
