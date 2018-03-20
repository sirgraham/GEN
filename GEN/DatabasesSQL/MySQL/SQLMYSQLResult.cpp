
/*------------------------------------------------------------------------------------------
//	MYSQLRESULT.CPP
//	
//	MYSQL result concrete class
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 19/08/2015 17:48:08
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
#if defined(DBSQL_ACTIVE) && defined(DBSQLMYSQL_ACTIVE)

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "mysql.h"	

#include "SQLMYSQLQuery.h"
#include "SQLMYSQLResult.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	SQLMYSQLRESULT::~SQLMYSQLRESULT
*/	
/**	
//	
//	 Class Destructor MYSQLRESULT
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/09/2015 16:14:37
//	
*/
/*-----------------------------------------------------------------*/
SQLMYSQLRESULT::~SQLMYSQLRESULT	() 
{	
	XQWORD ncols = this->GetNumColumns();

	if(bindresults)
		{
			int e=0;
			while(e < bind_results_size)
				{							
					if(bindresults[e].buffer)
						{
							switch(bindresults[e].buffer_type)				
								{
									case MYSQL_TYPE_FLOAT				:	delete((float*)bindresults[e].buffer);
																								break;

									case MYSQL_TYPE_LONG				:
									case MYSQL_TYPE_SHORT				:	delete((int*)bindresults[e].buffer);
																								break;

									case MYSQL_TYPE_VAR_STRING	:
									case MYSQL_TYPE_STRING			:
									case MYSQL_TYPE_VARCHAR			:	
																								delete((char*)bindresults[e].buffer);
																								break;

									case MYSQL_TYPE_DATETIME		:	delete((MYSQL_TIME*)bindresults[e].buffer);
																								break;
								}
						}
	
					bindresults[e].buffer = NULL;
					e++;
					
				}

			delete[] bindresults;			
			bindresults = NULL;
			
		}
	
	row.Clear();		
	
	Clean(); 	
}




/*-------------------------------------------------------------------
//	MYSQLRESULT::GetNumRows
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 17:50:36
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORD SQLMYSQLRESULT::GetNumRows()
{
	return mysql_stmt_num_rows(query->stmt);	
}



/*-------------------------------------------------------------------
//	MYSQLRESULT::GetNumColumns
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 17:50:40
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORD SQLMYSQLRESULT::GetNumColumns()
{
	return mysql_stmt_field_count(query->stmt);	
}



/*-------------------------------------------------------------------
//	MYSQLRESULT::Seek
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 17:50:56
//	
//	@return 			bool : 
//
//  @param				index : 
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLRESULT::Seek(int index)
{
	mysql_stmt_data_seek(query->stmt, index);

	return true;
}



/*-------------------------------------------------------------------
//	MYSQLRESULT::First
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 17:51:01
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLRESULT::First()
{
	return false;
}



/*-------------------------------------------------------------------
//	MYSQLRESULT::Last
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 17:51:06
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLRESULT::Last()
{
	return false;
}



/*-------------------------------------------------------------------
//	MYSQLRESULT::Next
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 17:51:10
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLRESULT::Next()
{	
	int RC = mysql_stmt_fetch(query->stmt);
	
	currentrow++;
	hasnext = RC;

	return (RC==DBSQLMYSQL_OK);
}



/*-------------------------------------------------------------------
//	MYSQLRESULT::ProcessRow
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 17:51:16
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLRESULT::ProcessRow()
{
	row.Clear();

		if (GetNumRows()==0) 
		return true;

	MYSQL_RES* ares = mysql_stmt_result_metadata(query->stmt);
	XQWORD		 ncol = GetNumColumns();

	for(XQWORD e=0; e<ncol; e++)
		{
			DBSQLSTRING* columnname = new DBSQLSTRING();
			if(!columnname)	return false;		
	
			MYSQL_FIELD* field = &ares->fields[e];
			columnname->Set(field->name);

			DBSQLVARIANT* variant = new DBSQLVARIANT();
			if(!variant)
				{
					delete(columnname);
					return false;
				}
		
			DBSQLSTRING columnvalue;

			if(!bindresults[e].buffer) continue;

			switch(bindresults[e].buffer_type)
				{	
					case MYSQL_TYPE_LONG			: { long value = *(long*)bindresults[e].buffer;											
																				(*variant) = (int)value;
																			}
																			break;

					case MYSQL_TYPE_SHORT			: { int value  = *(int*)bindresults[e].buffer;											
																				(*variant) = value;
																			}
																			break;

					case MYSQL_TYPE_DOUBLE		: 
					case MYSQL_TYPE_FLOAT			: { float value = *(float*)bindresults[e].buffer;											
																				(*variant) = value;
																			}
																			break;

					case MYSQL_TYPE_TIME			:
					case MYSQL_TYPE_DATE			: break;

					case MYSQL_TYPE_DATETIME	: {	MYSQL_TIME* mytime=(MYSQL_TIME*)bindresults[e].buffer;
											
																				DBSQLDATETIME time;

																				time.SetDay(mytime->day);
																				time.SetMonth(mytime->month);
																				time.SetYear(mytime->year);

																				time.SetHours(mytime->hour);
																				time.SetMinutes(mytime->minute);
																				time.SetSeconds(mytime->second);

																				(*variant) = time;										
																			}											
																			break;

				case MYSQL_TYPE_VAR_STRING	:
				case MYSQL_TYPE_VARCHAR			: {	XWORD* value = (XWORD*)bindresults[e].buffer;											 											
																				int		 size  = bindresults[e].buffer_length;
																																
																				XSTRING	received;
																				received.ConvertFromUTF8((XBYTE*)value,size);
																				(*variant) = received; 																				
																			}
																			break;
			}					
		header.Add(columnname);
		row.Add(variant);
	}

	return false;
}





/*-------------------------------------------------------------------
//	MYSQLRESULT::Abort
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 17:51:22
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLRESULT::Abort()
{
	return 0;
}

/*-------------------------------------------------------------------
//	MYSQLRESULT::HasNext
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 17:51:26
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLMYSQLRESULT::HasNext()
{	
	return (hasnext != MYSQL_NO_DATA);
}



#endif
