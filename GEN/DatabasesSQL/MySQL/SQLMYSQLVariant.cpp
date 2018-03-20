
/*------------------------------------------------------------------------------------------
//	SQLMYSQLVARIANT.CPP
//	
//	mysql variant concrete class
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 16/09/2015 10:33:27
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#if defined(DBSQL_ACTIVE) && defined(DBSQLMYSQL_ACTIVE)	

/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "SQLMYSQLVariant.h"
	
#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	DBSQLVARIANT& SQLMYSQLVARIANT::operator =(DBSQLDATETIME datetime)
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/09/2015 10:35:16
//	
//	@return 			DBSQLVARIANT& : 
//
//  @param				datetime : 
*/
/*-----------------------------------------------------------------*/
DBSQLVARIANT& SQLMYSQLVARIANT::operator =(DBSQLDATETIME& datetime)
{								
	type = (XVARIANT_TYPE)DBSQLVARIANT_TYPE_DATETIME;
	size = 0;

	mysqltime.day						= datetime.GetDay();
	mysqltime.month					= datetime.GetMonth();
	mysqltime.year					= datetime.GetYear();
	mysqltime.hour					= datetime.GetHours();
	mysqltime.minute				= datetime.GetMinutes();
	mysqltime.second				= datetime.GetSeconds();
	mysqltime.second_part		= 0;
	mysqltime.neg						= 0;
	mysqltime.time_type			= MYSQL_TIMESTAMP_DATETIME;
	
	return (*this);
}



/*-------------------------------------------------------------------
//	SQLMYSQLVARIANT::operator DBSQLDATETIME()
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/09/2015 10:35:22
//	
//	@return 			SQLMYSQLVARIANT::operator : 
//
*/
/*-----------------------------------------------------------------*/
SQLMYSQLVARIANT::operator DBSQLDATETIME()
{
	DBSQLDATETIME	time;
	
	time.SetDay(this->mysqltime.day);
	time.SetMonth(this->mysqltime.month);
	time.SetYear(this->mysqltime.year);

	time.SetHours(this->mysqltime.hour);
	time.SetMinutes(this->mysqltime.minute);
	time.SetSeconds(this->mysqltime.second);

	return	time;
}



/*-------------------------------------------------------------------
//	SQLMYSQLVARIANT::GetSize
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/09/2015 10:42:47
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD SQLMYSQLVARIANT::GetSize()
{
	if (type == (XVARIANT_TYPE)DBSQLVARIANT_TYPE_DATETIME) return 0;

	return DBSQLVARIANT::GetSize();
}



/*-------------------------------------------------------------------
//	SQLMYSQLVARIANT::GetData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/09/2015 10:42:37
//	
//	@return 			void* : 
//
*/
/*-----------------------------------------------------------------*/
void*	SQLMYSQLVARIANT::GetData	()
{
	return (void*)&mysqltime;
}


#endif
