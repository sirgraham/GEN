/*------------------------------------------------------------------------------------------
//	DBSQLDATABASE.CPP
//	
//	Generic Abstract Database Checks
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 10/08/2015 17:19:21
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifdef DBSQL_ACTIVE
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DBSQLQuery.h"	
#include "DBSQLDatabase.h"
#include "XFactory.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	DBSQLDATABASE::DBSQLDATABASE
*/	
/**	
//	
//	Class Constructor DBSQLDATABASE
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/12/2015 17:42:02
//	
 
*/
/*-----------------------------------------------------------------*/
DBSQLDATABASE::DBSQLDATABASE() 
{
	Clean(); 		
		
	mutex	=	xfactory->Create_Mutex();

	if(mutex) threadsafe=true;
}



/*-------------------------------------------------------------------
//	DBSQLDATABASE::~DBSQLDATABASE
*/	
/**	
//	
//	 Class Destructor DBSQLDATABASE
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 12:55:43
//	
*/
/*-----------------------------------------------------------------*/
DBSQLDATABASE::~DBSQLDATABASE()
{
	if(mutex) xfactory->Delete_Mutex(mutex);

	Clean(); 

	tables.DeleteContents			();
	tables.DeleteAll					();

	errorstack.DeleteContents	();
	errorstack.DeleteAll			();
}



/*-------------------------------------------------------------------
//	DBSQLDATABASE::CreateVariant
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/10/2015 14:27:35
//	
//	@return 			DBSQLVARIANT* : 
//
*/
/*-----------------------------------------------------------------*/
DBSQLVARIANT* DBSQLDATABASE::CreateVariant()
{
	return new DBSQLVARIANT();			 												
}



/*-------------------------------------------------------------------
//	DBSQLDATABASE::OnConnection
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			05/10/2015 10:27:44
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	DBSQLDATABASE::OnConnection()	
{
	

	SetDatabaseEncoding(DBSQLDATABASE_ENCODING_UTF8);			
	GetDatabaseEncoding();		

	return true;
}

/*-------------------------------------------------------------------
//	DBSQLDATABASE::OnDisconnection
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			05/10/2015 10:28:54
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	DBSQLDATABASE::OnDisconnection()
{ 
	return true;																						
}




/*-------------------------------------------------------------------
//	DBSQLDATABASE::Open
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/08/2015 17:45:18
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DBSQLDATABASE::Open()
{
	if(connection) 
		{
			bool status=connection->Connect();

			if(status) OnConnection();
			
			return status;
		}
		
	return false;
}




/*-------------------------------------------------------------------
//	DBSQLDATABASE::Close
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 12:57:38
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DBSQLDATABASE::Close()
{
	if(connection) 
		{
			bool status = connection->Disconnect();
			if(status) OnDisconnection();

			return status;
		}

	return false;
};



/*-------------------------------------------------------------------
//	DBSQLDATABASE::GetLastError
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/10/2015 14:23:54
//	
//	@return 			XCHAR* : 
//
*/
/*-----------------------------------------------------------------*/
XCHAR* DBSQLDATABASE::GetLastError()																	
{
	return (this->errorstack.GetSize()>0 ? this->errorstack.GetLast()->description.Get() : __L(""));	
}



/*-------------------------------------------------------------------
//	DBSQLDATABASE::Execute
*/	
/**	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			29/09/2015 13:22:42
//	
//	@return 			bool : 
//
//  @param				constructedquery : 
*/
/*-----------------------------------------------------------------*/
bool DBSQLDATABASE::Execute(DBSQLQUERY* constructedquery)
{


	if(!constructedquery->IsWellConstructedQuery())
		{
			Error(__L("Query is NOT well constructed"));			
			return false;
		}		

	return ExecuteQuery(constructedquery);
}




/*-------------------------------------------------------------------
//	DBSQLDATABASE::Error
*/	
/**	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 12:38:14
//	
//  @param				s : 
*/
/*-----------------------------------------------------------------*/
void	DBSQLDATABASE::Error(XCHAR* errorstring)
{
	DBSQLERROR*	error=new DBSQLERROR(DBSQLERROR_TYPE_STATEMENT_ERROR);	
	if(!error) return;

	error->description.Set(errorstring);
	GetErrorList()->Add(error);

	if(IsTransactionStarted())	Rollback();
}




/*-------------------------------------------------------------------
//	DBSQLDATABASE::Error
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 12:38:19
//	
//  @param				s : 
*/
/*-----------------------------------------------------------------*/
void DBSQLDATABASE::Error(char* errorstring)
{
	XSTRING string;

	string.Set(errorstring);

	Error(string.Get());
}


/*-------------------------------------------------------------------
//	DBSQLDATABASE::ClearPreviousErrors
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/12/2015 14:53:51
//	
*/
/*-----------------------------------------------------------------*/
void DBSQLDATABASE::ClearPreviousErrors	()
{
	this->errorstack.DeleteContents();
	this->errorstack.DeleteAll();
}


/*-------------------------------------------------------------------
//	DBSQLDATABASE::Lock
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/12/2015 17:44:51
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DBSQLDATABASE::Lock()
{	
	if (threadsafe)
		if (this->mutex)
			return this->mutex->Lock();

	return true;
}

/*-------------------------------------------------------------------
//	DBSQLDATABASE::Unlock
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/12/2015 17:44:55
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DBSQLDATABASE::Unlock()
{
	if (threadsafe)
		if (this->mutex)
			return this->mutex->UnLock();

	return true;
}


/*-------------------------------------------------------------------
//	DBSQLDATABASE::ExecuteQuery
*/	
/**	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/08/2015 10:54:20
//	
//	@return 			bool : 
//
//  @param				wellconstructedquery : 
*/
/*-----------------------------------------------------------------*/
bool DBSQLDATABASE::ExecuteQuery(DBSQLQUERY* wellconstructedquery)
{
	//Lock();
	if(!connection)
		{
			DBSQLERROR* error=new DBSQLERROR(DBSQLERROR_TYPE_CONNECTION_ERROR);	
			error->description.Set(__L("DATABASE not connected"));
		
			GetErrorList()->Add(error);
		
			return false;	
		}

	bool result=wellconstructedquery->Execute();	
	//Unlock();
	return result;
}



#endif
