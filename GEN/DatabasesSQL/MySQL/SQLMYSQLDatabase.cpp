
/*------------------------------------------------------------------------------------------
//	SQLMYSQLDATABASE.CPP
//	
//	Mysql Concrete database
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 19/08/2015 16:41:13
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	

#if defined(DBSQL_ACTIVE) && defined(DBSQLMYSQL_ACTIVE)


/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "SQLMYSQLConnection.h"	
#include "SQLMYSQLQuery.h"

#include "SQLMYSQLDatabase.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	SQLMYSQLDATABASE::Transaction
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 16:43:15
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLDATABASE::Transaction()
{
	MYSQL* conn=static_cast<SQLMYSQLCONNECTION*>(this->connection)->connection;
	if(!conn)  return false;

	mysql_autocommit(conn,false);

	int RC = mysql_query(conn,"BEGIN");

	if(RC==DBSQLMYSQL_OK)
		{
			transactionstarted=true;
			return true;
		}

	return false;
}



/*-------------------------------------------------------------------
//	SQLMYSQLDATABASE::Commit
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 16:43:21
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLDATABASE::Commit()
{
	if(transactionstarted)
		{
			MYSQL* conn=static_cast<SQLMYSQLCONNECTION*>(this->connection)->connection;
			int RC = mysql_commit(conn);		

			if(RC==DBSQLMYSQL_OK)
				{
					transactionstarted = false;
					mysql_autocommit(conn,true);

					return true;
				}		
			 else
				{
					Error((char*)mysql_error(conn));
					return false;
				}
		}

	return false;
}



/*-------------------------------------------------------------------
//	SQLMYSQLDATABASE::Rollback
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 16:43:34
//	
//	@return 			bool : 
//
//  @param				savepointname : (default pointname=NULL)
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLDATABASE::Rollback(XCHAR* savepointname)
{
	if(transactionstarted)
		{
			MYSQL* conn=static_cast<SQLMYSQLCONNECTION*>(this->connection)->connection;
			if(!savepointname)
				{
					int RC= mysql_rollback(conn);
					if (RC == DBSQLMYSQL_OK)
						{
							transactionstarted=false;
							return true;
						}		
				}
			 else
				{
					DBSQLSTRING text;
					text.Format(__L("ROLLBACK TO SAVEPOINT %s"),savepointname);

					XSTRING_CREATEOEM(text,rollbacktext);
					int rc=mysql_query(conn, rollbacktext);						
					XSTRING_DELETEOEM(rollbacktext);

					if(rc != DBSQLMYSQL_OK)
						{
							Error((char*)mysql_error(conn));
							return false;
						}

					this->transactionstarted=false;

					return true;
				}
		}

	return false;
}




/*-------------------------------------------------------------------
//	SQLMYSQLDATABASE::Savepoint
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 16:43:43
//	
//	@return 			bool : 
//
//  @param				savepoint : 
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLDATABASE::Savepoint(XCHAR* savepoint)
{
	MYSQL* conn = static_cast<SQLMYSQLCONNECTION*>(this->connection)->connection;
	if(!conn) return false;

	DBSQLSTRING text;
	
	text.Format(__L("SAVEPOINT %s"),savepoint);

	XSTRING_CREATEOEM(text,savetext);
	int rc = mysql_query(conn, savetext);
	XSTRING_DELETEOEM(savetext);

	if(rc != DBSQLMYSQL_OK)
		{
			Error((char*)mysql_error(conn));
			return false;
		}

	return false;
}






/*-------------------------------------------------------------------
//	SQLMYSQLDATABASE::Savepoint
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 16:43:43
//	
//	@return 			bool : 
//
//  @param				savepoint : 
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLDATABASE::ReleaseSavepoint(XCHAR* savepoint)
{
	MYSQL* conn=static_cast<SQLMYSQLCONNECTION*>(this->connection)->connection;
	if(!conn) return false;

	DBSQLSTRING text;
	text.Format(__L("RELEASE %s"),savepoint);

	XSTRING_CREATEOEM(text,savetext);
	int rc=mysql_query(conn, savetext);
	XSTRING_DELETEOEM(savetext);

	if(rc != DBSQLMYSQL_OK)
		{
			Error((char*)mysql_error(conn));
			return false;
		}

	return false;
}




/*-------------------------------------------------------------------
//	SQLMYSQLDATABASE::CreateQuery
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 16:43:57
//	
//	@return 			DBSQLQUERY* : 
//
*/
/*-----------------------------------------------------------------*/
DBSQLQUERY* SQLMYSQLDATABASE::CreateQuery()
{
	DBSQLQUERY* query = new SQLMYSQLQUERY(this);
	return query;
}




/*-------------------------------------------------------------------
//	SQLMYSQLDATABASE::CreateConnection
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 16:44:06
//	
//	@return 			DBSQLCONNECTION* : 
//
*/
/*-----------------------------------------------------------------*/
DBSQLCONNECTION* SQLMYSQLDATABASE::CreateConnection()
{
	DBSQLCONNECTION* conn = new SQLMYSQLCONNECTION(this);
	return conn;
}



/*-------------------------------------------------------------------
//	SQLMYSQLDATABASE::GetTables
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 16:44:14
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLDATABASE::GetTables()
{
	MYSQL*			conn	=	static_cast<SQLMYSQLCONNECTION*>(this->connection)->connection;
	DBSQLQUERY* query	=	CreateQuery();

	if(!conn)		return false;
	if(!query)	return false;

	tables.DeleteContents();
	tables.DeleteAll();

	query->Set(__L("SHOW TABLES"));
	if(query->Execute())
		{	
			DBSQLRESULT* result=query->GetResult();
			if(result) //hay un result
				{	
					result->GetNumRows();										
					while(result->HasNext())
						{													
							result->ProcessRow();					
						
							DBSQLROW* row=result->GetRow();
							for(XDWORD e=0; e<result->GetNumColumns(); e++)
								{
									if(row->Get(e).GetType()==(XVARIANT_TYPE)DBSQLVARIANT_TYPE_STRING)
										{
											DBSQLSTRING* tablename=new DBSQLSTRING();
											if(tablename)
												{
													tablename->Set((XCHAR*)row->Get(0));
													this->tables.Add(tablename);								
												}
										}
								}	
						
							result->Next();
						}
				}
		}
	 else
		{
			Error(__L("Unable to obtain Tables"));
			delete(query);

			return false;
		}

	delete(query);

	return false;
}



/*-------------------------------------------------------------------
//	SQLMYSQLDATABASE::GetDatabaseEncoding
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			05/10/2015 9:47:27
//	
//	@return 			DBSQLDATABASE_ENCODING : 
//
*/
/*-----------------------------------------------------------------*/
DBSQLDATABASE_ENCODING SQLMYSQLDATABASE::GetDatabaseEncoding()
{
	MYSQL* conn = static_cast<SQLMYSQLCONNECTION*>(this->connection)->connection;
	if(!conn)	return DBSQLDATABASE_ENCODING_UNKNOWN;

	const char* name = mysql_character_set_name(conn);

	XSTRING s;
	s.Set(name);

	if (s.Compare(__L("ucs2")) == 0)  return DBSQLDATABASE_ENCODING_UNKNOWN;
	if (s.Compare(__L("utf8")) == 0)  return DBSQLDATABASE_ENCODING_UTF8;

	return DBSQLDATABASE_ENCODING_UNKNOWN;
}




/*-------------------------------------------------------------------
//	SQLMYSQLDATABASE::SetDatabaseEncoding
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			05/10/2015 9:47:39
//	
//	@return 			bool : 
//
//  @param				encodingtouse : 
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLDATABASE::SetDatabaseEncoding(DBSQLDATABASE_ENCODING encodingtouse)
{
	MYSQL* conn = static_cast<SQLMYSQLCONNECTION*>(this->connection)->connection;

	switch(encodingtouse)
		{
			case DBSQLDATABASE_ENCODING_BIG5							:
			case DBSQLDATABASE_ENCODING_EUC_CN							:
			case DBSQLDATABASE_ENCODING_EUC_JP							:
			case DBSQLDATABASE_ENCODING_ISO_8859_5					:	 
			case DBSQLDATABASE_ENCODING_ISO_8859_6					: 
			case DBSQLDATABASE_ENCODING_ISO_8859_7					:	 
			case DBSQLDATABASE_ENCODING_ISO_8859_8					: 
			case DBSQLDATABASE_ENCODING_JOHAB								:
			case DBSQLDATABASE_ENCODING_EUC_JIS_2004				:
			case DBSQLDATABASE_ENCODING_EUC_KR							:
			case DBSQLDATABASE_ENCODING_EUC_TW							:
			case DBSQLDATABASE_ENCODING_GB18030							:
			case DBSQLDATABASE_ENCODING_GBK									:	
			case DBSQLDATABASE_ENCODING_KOI8R								:
			case DBSQLDATABASE_ENCODING_KOI8U								:
			case DBSQLDATABASE_ENCODING_LATIN1							:
			case DBSQLDATABASE_ENCODING_LATIN2							:
			case DBSQLDATABASE_ENCODING_LATIN3							:
			case DBSQLDATABASE_ENCODING_LATIN4							:
			case DBSQLDATABASE_ENCODING_LATIN5							:
			case DBSQLDATABASE_ENCODING_LATIN6							:
			case DBSQLDATABASE_ENCODING_LATIN7							:
			case DBSQLDATABASE_ENCODING_LATIN8							:
			case DBSQLDATABASE_ENCODING_LATIN9							:
			case DBSQLDATABASE_ENCODING_LATIN10							:
			case DBSQLDATABASE_ENCODING_MULE_INTERNAL				:
			case DBSQLDATABASE_ENCODING_SJIS								:
			case DBSQLDATABASE_ENCODING_SHIFT_JIS_2004			:
			case DBSQLDATABASE_ENCODING_SQL_ASCII						:
			case DBSQLDATABASE_ENCODING_UHC									:	break;

			case DBSQLDATABASE_ENCODING_UTF8								:	{ int rc = mysql_set_character_set(conn,"utf8"); 
																													if (rc == 0) return true;																																																						
																												}
																												break;
			case DBSQLDATABASE_ENCODING_WIN866							:
			case DBSQLDATABASE_ENCODING_WIN874							:
			case DBSQLDATABASE_ENCODING_WIN1250							:
			case DBSQLDATABASE_ENCODING_WIN1251							:
			case DBSQLDATABASE_ENCODING_WIN1252							:
			case DBSQLDATABASE_ENCODING_WIN1253							:
			case DBSQLDATABASE_ENCODING_WIN1254							:
			case DBSQLDATABASE_ENCODING_WIN1255							:
			case DBSQLDATABASE_ENCODING_WIN1256							:
			case DBSQLDATABASE_ENCODING_WIN1257							:
			case DBSQLDATABASE_ENCODING_WIN1258							:	break;

		}

	Error (__L("Unable to set the charset encoding"));

	return false;
}


#endif
