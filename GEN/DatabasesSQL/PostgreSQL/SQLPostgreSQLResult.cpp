/*------------------------------------------------------------------------------------------
//	SQLPOSTGRESQLRESULT.CPP
//	
//	SQLPOSTGRESQL concrete result
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 11/08/2015 16:52:19
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#if defined(DBSQL_ACTIVE) && defined(DBSQLPOSTGRESQL_ACTIVE)
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdlib.h>     /* strtoll */
#include <time.h>

#include "XDebug.h"
#include "XSystem.h"

#include "SQLPostgreSQLResult.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	SQLPOSTGRESQLRESULT::~SQLPOSTGRESQLRESULT
*/	
/**	
//	
//	 Class Destructor SQLPOSTGRESQLRESULT
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/08/2015 17:53:56
//	
*/
/*-----------------------------------------------------------------*/
SQLPOSTGRESQLRESULT::~SQLPOSTGRESQLRESULT	() 
{ 
	

	row.Clear();
	header.DeleteContents();	
	header.DeleteAll();

	delete(system);
	system = NULL;

	PQclear(this->resultset);
	Clean();
}



/*-------------------------------------------------------------------
//	SQLPOSTGRESQLRESULT::GetNumRows
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/08/2015 17:35:53
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORD SQLPOSTGRESQLRESULT::GetNumRows()
{		
	int res=PQntuples(this->resultset);
	return res;
}



/*-------------------------------------------------------------------
//	SQLPOSTGRESQLRESULT::GetNumColumns
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/08/2015 17:35:58
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XQWORD SQLPOSTGRESQLRESULT::GetNumColumns()
{
	return PQnfields(resultset);	
}



/*-------------------------------------------------------------------
//	SQLPOSTGRESQLRESULT::Seek
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/08/2015 16:54:33
//	
//	@return 			bool : 
//
//  @param				i : 
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLRESULT::Seek(int index)	 
{	
	return false;
}



/*-------------------------------------------------------------------
//	SQLPOSTGRESQLRESULT::First
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/08/2015 16:54:39
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLRESULT::First	()			 
{ 
	if(!initialized)
		{
			Next();
			GetRow();

			initialized = false;
			currentrow  = 0;

			return true;
		}	

	return false;
}



/*-------------------------------------------------------------------
//	SQLPOSTGRESQLRESULT::Last
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/08/2015 16:54:44
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLRESULT::Last()			 
{ 
	return false;
}



/*-------------------------------------------------------------------
//	SQLPOSTGRESQLRESULT::Next
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			11/08/2015 16:54:53
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLRESULT::Next()			 
{ 
	currentrow++;
	return true;	
}




/*-------------------------------------------------------------------
//	SQLPOSTGRESQLRESULT::ProcessRow
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 10:06:55
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLRESULT::ProcessRow()
{
	row.Clear();
	header.DeleteContents();	
	header.DeleteAll();	
				
	//--------------------- headers
	int ncol = (int)GetNumColumns();

	char * testcmdstatus=PQcmdStatus(resultset);
	if (GetNumRows()==0) 
		return true;

	for(int e=0; e<ncol; e++)
		{
			DBSQLSTRING* head=new DBSQLSTRING();
			if(!head)
				{
					DBSQLERROR*	error=new DBSQLERROR(DBSQLERROR_TYPE_MEMORY_ERROR);	
					if(!error) return false;

					error->description.Set(__L("not enought memory for result"));
					query->database->GetErrorList()->Add(error);									

					return true;														 
				}

			char* name = PQfname(resultset, e);					    
			head->Set(name);
			header.Add(head);			
	 }

	//----------------- row
for(int e=0; e<ncol; e++)
		{
			DBSQLVARIANT* variant=new DBSQLVARIANT();
			if(!variant)
				{
					DBSQLERROR*	error = new DBSQLERROR(DBSQLERROR_TYPE_MEMORY_ERROR);	
					if(!error) return false;

					error->description.Set(__L("not enought memory for result"));
					query->database->GetErrorList()->Add(error);									

					return true;														 
				}

			Oid		type		 = PQftype		(this->resultset, e);
			int		isBinary = PQfformat	(this->resultset, e);
			int   isNull	 = PQgetisnull(this->resultset,	(int)currentrow,e);
		
						
			if (isNull!=1)
			switch(type)
				{
					case BOOLOID							:				
					case BYTEAOID							:
					case CHAROID							:	break;

					case NAMEOID							:	{ char* data = PQgetvalue(resultset, (int)currentrow,e);				
																				(*variant) = data; 
																				XCHAR* value= ((XSTRING*)(variant->GetData()))->Get();
																				(*variant) = data; 
																			}
																			break;

					case INT8OID							: { 
																				XQWORD number  = 0;		

																				if (!isNull)
																				{
																					XBYTE* data    = (XBYTE*)PQgetvalue(resultset, (int)currentrow,e);																								
																					memcpy(&number, (void*)data, sizeof(XQWORD));
																					if(is_le) SWAPQWORD(number);						
																				}

																				(*variant) = number; 
																			}
																			break;

					case INT2VECTOROID				: break;

					case INT2OID							:	{ 
																				short integer	=	0;

																				if (!isNull)
																				{
																					XBYTE* data		= (XBYTE*)PQgetvalue(resultset, (int)currentrow, e);																								
																					memcpy(&integer,(void*)data,sizeof(integer));
																					if(is_le) SWAPWORD(integer);						
																				}

																				(*variant) = (int)integer; 
																			}

					case INT4OID							: { 
																				int		integer	=	0;

																				if (!isNull)
																				{
																					XBYTE* data		= (XBYTE*)PQgetvalue(resultset, (int)currentrow, e);																																									
																					memcpy(&integer,(void*)data,sizeof(integer));
																					if(is_le) SWAPDWORD(integer);						
																				}
						
																				(*variant)=integer; 
																			}
																			break;

					case REGPROCOID						: break;

					case TEXTOID							: { 
																				if (!isNull)
																				{
																				char* data = PQgetvalue(resultset, (int)currentrow, e);				
																				(*variant)  = data; 
																				}
																				else
																					(*variant)  = __L(""); 
																			}
																			break;

					case OIDOID								:	 
					case TIDOID								:
					case XIDOID								:
					case CIDOID								:
					case OIDVECTOROID					: break;
				
					case JSONOID							: { 
																				if (!isNull)
																				{
																				char* data = PQgetvalue(resultset, (int)currentrow, e);				
																				(*variant)  = data; 
																				}
																				else
																					(*variant)  = __L(""); 
																			}
																			break;

					case XMLOID								:
					case PGNODETREEOID				:
					case PGDDLCOMMANDOID			:
					case POINTOID							:
					case LSEGOID							:
					case PATHOID							:
					case BOXOID								:
					case POLYGONOID						:
					case LINEOID							:	break;

					case FLOAT4OID						: { 
																				double	dat=0.0;
																				if (!isNull)
																				{
																				char*		data = PQgetvalue(resultset, (int)currentrow, e);																													
																				if(data) memcpy(&dat,(void*)data,sizeof(dat));
																				XDWORD dword;
																				if(data) memcpy(&dword,(void*)data,sizeof(dword));
																				if(is_le) SWAPDWORD(dword);
																				memcpy(&dat,(void*)&dword,sizeof(double));
																				}

																				(*variant) = (float)dat; 
																			}
																			break;

					case FLOAT8OID						: { 
																				double dat = 0.0;
																				if (!isNull)
																				{
																				char* data = PQgetvalue(resultset, (int)currentrow,e);																								
																				memcpy(&dat, (void*)data, sizeof(dat));
																				XQWORD dword;
																				memcpy(&dword, (void*)data, sizeof(dword));
																				if(is_le) SWAPQWORD(dword);
																				memcpy(&dat,(void*)&dword,sizeof(double));
																				}

																				(*variant) = (float)dat;
																			}
																			break;

					case ABSTIMEOID						:
					case RELTIMEOID						:
					case TINTERVALOID					:
					case UNKNOWNOID						: 
					case CIRCLEOID						:
					case CASHOID							:
					case MACADDROID						:
					case INETOID							: 
					case CIDROID							:
					case INT2ARRAYOID					:
					case INT4ARRAYOID					:
					case TEXTARRAYOID					:
					case OIDARRAYOID					:
					case FLOAT4ARRAYOID				:
					case ACLITEMOID						: break;

					case CSTRINGARRAYOID			: { char* data = PQgetvalue(resultset, (int)currentrow, e);				
																				(*variant) = data; 
																			}
																			break;

					case BPCHAROID						: break;

					case VARCHAROID						: { char* data				= PQgetvalue	(this->resultset, (int)this->currentrow, e);																				
																				int		length			= PQgetlength(this->resultset,(int)this->currentrow, e);

																				XSTRING	received;
																				received.ConvertFromUTF8((XBYTE*)data,length);
																				(*variant) = received; 
																			}
																			break;


					case TIMEOID							: 
																			{
																				XBYTE* data = (XBYTE*)PQgetvalue(resultset, (int)currentrow, e);
																																																											
																				DBSQLDATETIME datetime;

																				if (!isNull)
																				{
																				XQWORD qword;
																			
																				memcpy(&qword,(void*)data,sizeof(qword));
																				if(is_le) 
																					SWAPQWORD(qword);								

																				qword/=1000000;

																									datetime.SetDay		(1);
																									datetime.SetMonth	(1);
																									datetime.SetYear		(2000);											

																									datetime.AddSeconds(qword);

																									datetime.SetDay		(0);
																									datetime.SetMonth	(0);
																									datetime.SetYear		(0);
																				}
																				(*variant) = datetime;
																				variant->SetType(XVARIANT_TYPE_TIME);
																			}
																			break;
					case DATEOID							:																				
																			{ 
																				XBYTE* data = (XBYTE*)PQgetvalue(resultset, (int)currentrow, e);
																																																											
																				DBSQLDATETIME datetime;

																				if (!isNull)
																				{
																				XDWORD dword;
																			
																				memcpy(&dword,(void*)data,sizeof(dword));
																				if(is_le) 
																					SWAPDWORD(dword);											
																																						
																									datetime.SetDay		(1);
																									datetime.SetMonth	(1);
																									datetime.SetYear		(2000);
																									datetime.AddDays		(dword);
																				}
																				(*variant) = datetime;
																				variant->SetType(XVARIANT_TYPE_DATE);
																			}
																			break;

					case DATETIME							:
					case TIMESTAMPOID					: { XBYTE* data = (XBYTE*)PQgetvalue(resultset, (int)currentrow, e);
																																																											
																				DBSQLDATETIME datetime;

																				if (!isNull)
																				{
																				XQWORD qword;																			
																				memcpy(&qword,(void*)data,sizeof(qword));
																				if(is_le) SWAPQWORD(qword);											
																																							
																				qword/=1000000;

																				XDATETIME start;																			
																									start.SetDay		(1);
																									start.SetMonth	(1);
																									start.SetYear		(2000);

																				XQWORD secstoposix=start.GetSeconsFromDate(false);																			
																				datetime.SetDateFromSeconds(secstoposix+qword,false);																																						
																				}
																				(*variant) = datetime;
																			}
																			break;
																		
					case INTERVALOID					:
					case TIMETZOID						:
					case BITOID								:
					case VARBITOID						:
					case NUMERICOID						:
					case REFCURSOROID					:
					case REGPROCEDUREOID			:
					case REGOPEROID						: 
					case REGOPERATOROID				:
					case REGCLASSOID					:
					case REGTYPEOID						:
					case REGROLEOID						:
					case REGNAMESPACEOID			:
					case REGTYPEARRAYOID			:
					case UUIDOID							:
					case LSNOID								:
					case TSVECTOROID					:
					case GTSVECTOROID					:
					case TSQUERYOID						:
					case REGCONFIGOID					:
					case REGDICTIONARYOID			:
					case JSONBOID							:
					case INT4RANGEOID					:
					case RECORDOID						:
					case RECORDARRAYOID				:
					case CSTRINGOID						:
					case ANYOID								:
					case ANYARRAYOID					:
					case VOIDOID							: 
					case TRIGGEROID						: 
					case EVTTRIGGEROID				:	 
					case LANGUAGE_HANDLEROID	:
					case INTERNALOID					:	 
					case OPAQUEOID						:
					case ANYELEMENTOID				:
					case ANYNONARRAYOID				:
					case ANYENUMOID						:	 
					case FDW_HANDLEROID				:
					case TSM_HANDLEROID				: 
					case ANYRANGEOID					:	break;			
				}
			this->row.Add(variant);	
		}

	return true;
}


/*-------------------------------------------------------------------
//	SQLPOSTGRESQLRESULT::Abort
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 10:56:46
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLRESULT::Abort()
{
	return false;
}

/*-------------------------------------------------------------------
//	SQLPOSTGRESQLRESULT::HasNext
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/08/2015 10:56:51
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SQLPOSTGRESQLRESULT::HasNext()
{	
	return (((int)this->currentrow)<PQntuples(this->resultset));
}


#endif
