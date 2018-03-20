/*------------------------------------------------------------------------------------------
//	SQLMYSQLCONNECTION.CPP
//	
//	MYSQL concrete connection
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 19/08/2015 16:51:25
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
#if defined(DBSQL_ACTIVE) && defined(DBSQLMYSQL_ACTIVE)

#pragma comment (lib, "mysqlclient.lib")

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XDebug.h"

#include "SQLMYSQLConnection.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
		


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	SQLMYSQLCONNECTION::Connect
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 16:52:26
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLCONNECTION::Connect()
{
	connection = mysql_init(NULL);
  if(!connection) 
	  {
		  XSTRING error;
			error.Set(mysql_error(connection));

      database->Error(error.Get());

			return false;
		}

	DBSQLSTRING databaseurl;	
	DBSQLSTRING databaseuser;
	DBSQLSTRING databasepassword;	
	DBSQLSTRING databasename;
	DBSQLSTRING databaseport;			
	DBSQLSTRING databasetimeout;	
	DBSQLSTRING reconnect;

	if(!FindOption(__L("URL"),	&databaseurl))		  		
	  {		
			database->Error(__L("DATABASE URL argument not supplied"));					
			return false;	
		}

	if(!FindOption(__L("USER"),	&databaseuser))					
		{		
			database->Error(__L("DATABASE User Name argument not supplied"));		
			return false;	
		}

	if(!FindOption(__L("PASSWORD"), &databasepassword))			
		{		
			database->Error(__L("DATABASE User Name argument not supplied"));		
			return false;	
		}
	

	

	FindOption(__L("DATABASE")	, &databasename);
	FindOption(__L("PORT")			,	&databaseport);

	XSTRING_CREATEOEM(databaseurl,url);
	XSTRING_CREATEOEM(databaseuser,user);
	XSTRING_CREATEOEM(databasepassword,pswd);
	XSTRING_CREATEOEM(databasename,name);

	unsigned int port = databaseport.ConvertToDWord();

	bool success=true;
	
	if (FindOption(__L("RECONNECT"), &reconnect))
	{
		int reconnect_opt = reconnect.ConvertToInt();
		mysql_options(connection, MYSQL_OPT_RECONNECT, &reconnect_opt);
	}
	else
	{
		int reconnect_opt = 1;
		mysql_options(connection, MYSQL_OPT_RECONNECT, &reconnect_opt);
	}

	if (FindOption(__L("TIMEOUT"), &databasetimeout))
	{
			int timeoutseconds=databasetimeout.ConvertToInt();
			mysql_options(connection,MYSQL_OPT_CONNECT_TIMEOUT ,&timeoutseconds);
	}

  if(mysql_real_connect(connection, url, user, pswd, name, port, NULL, 0) == NULL) 
		{
		  DBSQLSTRING error;

			error.Set(mysql_error(connection));
      database->Error(error.Get());
      mysql_close(connection);

			success = false;
	  }  

	XSTRING_DELETEOEM(url);
	XSTRING_DELETEOEM(user);
	XSTRING_DELETEOEM(pswd);
	XSTRING_DELETEOEM(name);	

	return success;
}



/*-------------------------------------------------------------------
//	SQLMYSQLCONNECTION::Disconnect
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 16:52:21
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLCONNECTION::Disconnect()
{
	mysql_close(connection);

	return true;
}


#endif
