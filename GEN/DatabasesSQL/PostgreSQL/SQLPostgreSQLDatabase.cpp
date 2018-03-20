/*------------------------------------------------------------------------------------------
//	SQLPOSTGRESQLDATABASE.cpp
*/	
/**	
// \class 
//   
//  Postgres database concrete class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 10/08/2015 12:26:06
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
		

#if defined(DBSQL_ACTIVE) && defined(DBSQLPOSTGRESQL_ACTIVE)

/*---- INCLUDES --------------------------------------------------------------------------*/
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
/*---- CLASS -----------------------------------------------------------------------------*/

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "libpq-fe.h"

#include "SQLPostgreSQLDatabase.h"
#include "SQLPostgreSQLConnection.h"
#include "SQLPostgreSQLQuery.h"

#include "XMemory.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
		
/*---- CLASS -----------------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	SQLPOSTGRESQLDATABASE::SQLPOSTGRESQLDATABASE
*/	
/**	
//	
//	Class Constructor SQLPOSTGRESQLDATABASE
//	
//	@author				Abraham J. Velez
//	@version			05/05/2017 13:15:42
//	
*/
/*-----------------------------------------------------------------*/
SQLPOSTGRESQLDATABASE::SQLPOSTGRESQLDATABASE() : DBSQLDATABASE() 
{
	Clean();

	int version = PQlibVersion();
	//XDEBUG_PRINTCOLOR(0,__L("SQL DATABASE: PostGres Version %d.%d"),version/10000,(version%1000)/100);
	type	=	DBSQLDATABASE_FLAGS_SQL | DBSQLDATABASE_FLAGS_TRANSACTIONAL;														
}



/*-------------------------------------------------------------------
//	SQLPOSTGRESQLDATABASE::Transaction
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/09/2015 10:53:45
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLDATABASE::Transaction()
{
	PGconn*   conn = static_cast<SQLPOSTGRESQLCONNECTION*>(this->connection)->connection;		
	PGresult* res  = PQexec(conn, "BEGIN");

	if(PQresultStatus(res) != PGRES_COMMAND_OK)
		{
			Error(PQerrorMessage(conn));
			PQclear(res);

			return false;
		}

	PQclear(res);

	return true;
}



/*-------------------------------------------------------------------
//	SQLPOSTGRESQLDATABASE::Commit
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/09/2015 10:53:50
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLDATABASE::Commit()
{
	PGconn* conn=static_cast<SQLPOSTGRESQLCONNECTION*>(this->connection)->connection;		
	PGresult* res = PQexec(conn, "END");

	if(PQresultStatus(res) != PGRES_COMMAND_OK)
		{
			Error(PQerrorMessage(conn));
			PQclear(res);

			return false;
	}

	PQclear(res);

	return true;
}




/*-------------------------------------------------------------------
//	SQLPOSTGRESQLDATABASE::Rollback
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/09/2015 10:53:55
//	
//	@return 			bool : 
//
//  @param				savepointname : 
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLDATABASE::Rollback(XCHAR* savepointname)
{
	PGconn*		conn = static_cast<SQLPOSTGRESQLCONNECTION*>(this->connection)->connection;		
	PGresult* res  = PQexec(conn, "ABORT");
	if(PQresultStatus(res) != PGRES_COMMAND_OK)
		{
			Error(PQerrorMessage(conn));
			PQclear(res);

			return false;
		}

	PQclear(res);

	return true;
}




/*-------------------------------------------------------------------
//	SQLPOSTGRESQLDATABASE::Savepoint
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/09/2015 10:54:11
//	
//	@return 			bool : 
//
//  @param				savepoint : 
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLDATABASE::Savepoint(XCHAR* savepoint)
{
	PGconn* conn						=	static_cast<SQLPOSTGRESQLCONNECTION*>(this->connection)->connection;	
	XSTRING savepointstring;
	bool		success;

	savepointstring.Format(__L("SAVEPOINT %s"),savepoint);

	XSTRING_CREATEOEM(savepointstring,oem);

	PGresult* res = PQexec(conn, oem);
	if(PQresultStatus(res) != PGRES_COMMAND_OK)
		{
			Error(PQerrorMessage(conn));		
			success=false;
		}
	 else success=true;

	PQclear(res);

	XSTRING_DELETEOEM(oem);

	return success;
}



/*-------------------------------------------------------------------
//	SQLPOSTGRESQLDATABASE::ReleaseSavepoint
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/09/2015 10:54:18
//	
//	@return 			bool : 
//
//  @param				savepoint : 
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLDATABASE::ReleaseSavepoint(XCHAR* savepoint)
{
	PGconn* conn							=	static_cast<SQLPOSTGRESQLCONNECTION*>(this->connection)->connection;	
	XSTRING savepointstring;	
	bool		success;

	savepointstring.Format(__L("RELEASE %s"),savepoint);

	XSTRING_CREATEOEM(savepointstring,oem);
	PGresult* res = PQexec(conn, oem);

	if(PQresultStatus(res) != PGRES_COMMAND_OK)
		{
			Error(PQerrorMessage(conn));		
			success=false;

		} else success=true;

	PQclear(res);

	XSTRING_DELETEOEM(oem);

	return success;
}



/*-------------------------------------------------------------------
//	SQLPOSTGRESQLDATABASE::CreateQuery
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/09/2015 10:54:24
//	
//	@return 			DBSQLQUERY* : 
//
*/
/*-----------------------------------------------------------------*/
DBSQLQUERY*	SQLPOSTGRESQLDATABASE::CreateQuery()
{
	SQLPOSTGRESQLQUERY* query=new SQLPOSTGRESQLQUERY(this);

	return query;
}



/*-------------------------------------------------------------------
//	SQLPOSTGRESQLDATABASE::CreateConnection
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/09/2015 10:54:29
//	
//	@return 			DBSQLCONNECTION* : 
//
*/
/*-----------------------------------------------------------------*/
DBSQLCONNECTION* SQLPOSTGRESQLDATABASE::CreateConnection()
{
	SQLPOSTGRESQLCONNECTION* pgconnection= new SQLPOSTGRESQLCONNECTION(this);

	return pgconnection;
}



/*-------------------------------------------------------------------
//	SQLPOSTGRESQLDATABASE::GetTables
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/09/2015 10:54:36
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLDATABASE::GetTables()
{
	DBSQLSTRING querystring;
	querystring.Format(__L("SELECT relname FROM pg_class WHERE relkind='r';"));
	
	DBSQLQUERY*		query=this->CreateQuery();
	DBSQLVARIANT	variant;

	if (query!=NULL)
	{
		query->Set(querystring.Get());
		if (this->Execute(query))
		{
				DBSQLRESULT* result=query->GetResult();
				if(result!=NULL) //hay un result
					{	
						result->GetNumRows();										
						while(result->HasNext())
							{													
								result->ProcessRow();					
						
								DBSQLROW* row = result->GetRow();
								for(XDWORD e=0; e<result->GetNumColumns(); e++)
									{
										variant=row->Get(e);
										if(variant.GetType()==(XVARIANT_TYPE)DBSQLVARIANT_TYPE_STRING)
											{
												DBSQLSTRING* tablename = new DBSQLSTRING();
												if(tablename)
													{
														tablename->Set((XCHAR*)variant);
														tables.Add(tablename);								
													}
											}
									}
								result->Next();
							}
					}
					else return false;				
		}
		delete(query);
	}
	else return false;
		
	return true;
}


/*-------------------------------------------------------------------
//	SQLPOSTGRESQLDATABASE::GetDatabaseEncoding
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/10/2015 14:46:30
//	
//	@return 			DBSQLDATABASE_ENCODING : 
//
*/
/*-----------------------------------------------------------------*/
DBSQLDATABASE_ENCODING SQLPOSTGRESQLDATABASE::GetDatabaseEncoding		()
{
	int			enc							=PQclientEncoding(static_cast<SQLPOSTGRESQLCONNECTION*>(this->connection)->connection);
					encodingtype		=(DBSQLDATABASE_ENCODING)enc;
	return	encodingtype;
}

/*-------------------------------------------------------------------
//	SQLPOSTGRESQLDATABASE::SetDatabaseEncoding
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/10/2015 14:46:36
//	
//	@return 			bool : 
//
//  @param				encodingtouse : 
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLDATABASE::SetDatabaseEncoding		(DBSQLDATABASE_ENCODING encodingtouse)
{
	PGconn* conn=static_cast<SQLPOSTGRESQLCONNECTION*>(this->connection)->connection;

	switch(encodingtouse)
	{
	case DBSQLDATABASE_ENCODING_BIG5							:
	case DBSQLDATABASE_ENCODING_EUC_CN						:
	case DBSQLDATABASE_ENCODING_EUC_JP						:
	case DBSQLDATABASE_ENCODING_EUC_JIS_2004			:
	case DBSQLDATABASE_ENCODING_EUC_KR						:
	case DBSQLDATABASE_ENCODING_EUC_TW						:
	case DBSQLDATABASE_ENCODING_GB18030						:
	case DBSQLDATABASE_ENCODING_GBK								:	
	case DBSQLDATABASE_ENCODING_ISO_8859_5				:	 
	case DBSQLDATABASE_ENCODING_ISO_8859_6				: 
	case DBSQLDATABASE_ENCODING_ISO_8859_7				:	 
	case DBSQLDATABASE_ENCODING_ISO_8859_8				: 
	case DBSQLDATABASE_ENCODING_JOHAB							:
	case DBSQLDATABASE_ENCODING_KOI8R							:
	case DBSQLDATABASE_ENCODING_KOI8U							:
	case DBSQLDATABASE_ENCODING_LATIN1						:
	case DBSQLDATABASE_ENCODING_LATIN2						:
	case DBSQLDATABASE_ENCODING_LATIN3						:
	case DBSQLDATABASE_ENCODING_LATIN4						:
	case DBSQLDATABASE_ENCODING_LATIN5						:
	case DBSQLDATABASE_ENCODING_LATIN6						:
	case DBSQLDATABASE_ENCODING_LATIN7						:
	case DBSQLDATABASE_ENCODING_LATIN8						:
	case DBSQLDATABASE_ENCODING_LATIN9						:
	case DBSQLDATABASE_ENCODING_LATIN10						:
	case DBSQLDATABASE_ENCODING_MULE_INTERNAL			:
	case DBSQLDATABASE_ENCODING_SJIS							:
	case DBSQLDATABASE_ENCODING_SHIFT_JIS_2004		:
	case DBSQLDATABASE_ENCODING_SQL_ASCII					:
	case DBSQLDATABASE_ENCODING_UHC								:			break;
	case DBSQLDATABASE_ENCODING_UTF8							:			
																											{
																											int rc=PQsetClientEncoding(conn,"UTF8"); 
																											if (rc==0) 
																												return true;																																																						
																											}
																											break;
	case DBSQLDATABASE_ENCODING_WIN866						:
	case DBSQLDATABASE_ENCODING_WIN874						:
	case DBSQLDATABASE_ENCODING_WIN1250						:
	case DBSQLDATABASE_ENCODING_WIN1251						:
	case DBSQLDATABASE_ENCODING_WIN1252						:
	case DBSQLDATABASE_ENCODING_WIN1253						:
	case DBSQLDATABASE_ENCODING_WIN1254						:
	case DBSQLDATABASE_ENCODING_WIN1255						:
	case DBSQLDATABASE_ENCODING_WIN1256						:
	case DBSQLDATABASE_ENCODING_WIN1257						:
	case DBSQLDATABASE_ENCODING_WIN1258						:			break;

	}

	Error (__L("Unable to set the charset encoding"));
	return false;
}


/*-------------------------------------------------------------------
//	SQLPOSTGRESQLDATABASE::OnConnection
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/10/2015 15:56:04
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLDATABASE::OnConnection								()
{
	SetDatabaseEncoding(DBSQLDATABASE_ENCODING_UTF8);
	GetDatabaseEncoding();
	return true;
}


/*-------------------------------------------------------------------
//	SQLPOSTGRESQLDATABASE::OnDisconnection
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/10/2015 15:56:11
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLDATABASE::OnDisconnection							()
{
		return true;
}


bool SQLPOSTGRESQLDATABASE::IsThreadSafe								()
{
	return PQisthreadsafe()==1;
}

#endif
