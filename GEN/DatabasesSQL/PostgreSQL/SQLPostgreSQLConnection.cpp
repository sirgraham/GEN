/*------------------------------------------------------------------------------------------
//  SQLPOSTGRESQLSCONNECTION.cpp
*/
/**
// \class
//
//  Conection to PostgreSQL
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 10/08/2015 12:15:38
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#if defined(DBSQL_ACTIVE) && defined(DBSQLPOSTGRESQL_ACTIVE)


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "SQLPostgreSQLConnection.h"
#include "XMemory.h"

#if _MSC_VER == 1900

#pragma comment(lib, "legacy_stdio_definitions.lib")

#ifndef __iob_func
extern "C" FILE* __cdecl __iob_func()
{
  struct _iobuf_VS2012 { // ...\Microsoft Visual Studio 11.0\VC\include\stdio.h #56
    char *_ptr;
    int   _cnt;
    char *_base;
    int   _flag;
    int   _file;
    int   _charbuf;
    int   _bufsiz;
    char *_tmpfname;
  };
  // VS2015 has only FILE = struct {void*}

  int const count = sizeof(_iobuf_VS2012) / sizeof(FILE);

  //// stdout
  //return (FILE*)(&(__acrt_iob_func(1)->_Placeholder) - count);

  // stderr
  return (FILE*)(&(__acrt_iob_func(2)->_Placeholder) - 2 * count);
}
#endif
#endif
//*/




//#pragma comment(lib,"libpqd.lib")
#pragma comment(lib,"libpq.lib")



/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  SQLPOSTGRESQLSCONNECTION::Connect
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/08/2015 16:52:26
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLCONNECTION::Connect()
{

  DBSQLSTRING connectionstring;

  DBSQLSTRING databaseUrl;      if (!FindOption(__L("URL"),     &databaseUrl))          {   database->Error(__L("DATABASE URL argument not supplied"));           return false; }
  DBSQLSTRING databaseUser;     if (!FindOption(__L("USER"),    &databaseUser))         {   database->Error(__L("DATABASE User Name argument not supplied"));     return false; }
  DBSQLSTRING databasePassword; if (!FindOption(__L("PASSWORD"),&databasePassword))     {   database->Error(__L("DATABASE User Password argument not supplied")); return false; }
  DBSQLSTRING databaseName;     if (!FindOption(__L("DATABASE"),&databaseName))         {   database->Error(__L("DATABASE Name argument not supplied"));          return false; }
  DBSQLSTRING databasePort;     if (!FindOption(__L("PORT"),    &databasePort))         {   database->Error(__L("DATABASE Port argument not supplied"));          return false; }
  DBSQLSTRING databaseTimeOut;       FindOption(__L("TIMEOUT"), &databaseTimeOut);

  if(!databaseUrl.IsEmpty())
    {
      connectionstring.Add(__L("host = "));
      connectionstring.Add(databaseUrl);
      connectionstring.Add(__L(" "));
    }

  if(!databaseUser.IsEmpty())
    {
      connectionstring.Add(__L("user = "));
      connectionstring.Add(databaseUser);
      connectionstring.Add(__L(" "));
    }

  if(!databasePassword.IsEmpty())
    {
      connectionstring.Add(__L("password = "));
      connectionstring.Add(databasePassword);
      connectionstring.Add(__L(" "));
    }

  if(!databaseName.IsEmpty())
    {
      connectionstring.Add(__L("dbname = "));
      connectionstring.Add(databaseName);
      connectionstring.Add(__L(" "));
    }

  if(!databasePort.IsEmpty())
    {
      connectionstring.Add(__L("port = "));
      connectionstring.Add(databasePort);
      connectionstring.Add(__L(" "));
    }

  if(!databaseTimeOut.IsEmpty())
  {
    connectionstring.Add(__L("connect_timeout = "));
    connectionstring.Add(databaseTimeOut);
    connectionstring.Add(__L(" "));
  }



  XSTRING_CREATEOEM(connectionstring,conninfo);

  connection = PQconnectdb(conninfo);

  bool status = true;

  if(PQstatus(connection) != CONNECTION_OK)
    {
      XSTRING error;

      error.Set(PQerrorMessage(connection));
      database->Error(error.Get());

      status = false;
    }

  XSTRING_DELETEOEM(conninfo);

  return status;
}




/*-------------------------------------------------------------------
//  SQLPOSTGRESQLSCONNECTION::Disconnect
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      19/08/2015 16:52:21
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLCONNECTION::Disconnect()
{
   PQfinish(connection);

  return true;
}

#endif
