/*------------------------------------------------------------------------------------------
//	SQLPOSTGRESQLQUERY.H
*/	
/**	
// \class 
//   
//  SQLPOSTGRESQL query concrete class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 11/08/2015 9:50:39
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#if defined(DBSQL_ACTIVE) && defined(DBSQLPOSTGRESQL_ACTIVE)

#ifndef _SQLPOSTGRESQLQUERY_H_
#define _SQLPOSTGRESQLQUERY_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "DBSQLQuery.h"
#include "DBSQLDatabase.h"
#include "DBSQLResult.h"


#include "libpq-fe.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
enum SQLPOSTGRESQLDATA_TYPE
{ 
	BOOLOID										=   16	,
	BYTEAOID									=   17	,
	CHAROID										=   18	,	 
	NAMEOID										=   19	,
	INT8OID										=   20	, 
	INT2OID										=   21	, 
	INT2VECTOROID							=   22	, 
	INT4OID										=   23	, 
	REGPROCOID								=   24	, 
	TEXTOID										=   25	, 
	OIDOID										=   26	, 
	TIDOID										=   27	, 
	XIDOID										=   28	, 
	CIDOID										=   29	, 
	OIDVECTOROID							=   30	, 
	JSONOID										=  114	, 
	XMLOID										=  142	, 
	PGNODETREEOID							=  194	, 
	PGDDLCOMMANDOID						=   32	, 
	POINTOID									=  600	, 
	LSEGOID										=  601	, 
	PATHOID										=  602	, 
	BOXOID										=  603	, 
	POLYGONOID								=  604	, 
	LINEOID										=  628	, 
	FLOAT4OID									=  700	,
	FLOAT8OID									=  701	, 
	ABSTIMEOID								=  702	, 
	RELTIMEOID								=  703	, 
	TINTERVALOID							=  704	, 
	UNKNOWNOID								=  705	, 
	CIRCLEOID									=  718	, 
	CASHOID										=  790	,	 
	MACADDROID								=  829	, 
	INETOID										=  869	, 
	CIDROID										=	 650	, 
	INT2ARRAYOID							=	1005	, 
	INT4ARRAYOID							=	1007	, 
	TEXTARRAYOID							=	1009	, 
	OIDARRAYOID								=	1028	, 
	FLOAT4ARRAYOID						=	1021	, 
	ACLITEMOID								=	1033	, 
	CSTRINGARRAYOID						=	1263	, 
	BPCHAROID									=	1042	, 
	VARCHAROID								=	1043	, 
	DATETIME									= 1080	,
	DATEOID										=	1082	, 
	TIMEOID										=	1083	, 
	TIMESTAMPOID							=	1114	, 
	TIMESTAMPTZOID						=	1184	, 
	INTERVALOID								=	1186	, 
	TIMETZOID									=	1266	, 
	BITOID										=	1560	,	 
	VARBITOID									=	1562	, 
	NUMERICOID								=	1700	, 
	REFCURSOROID							=	1790	, 
	REGPROCEDUREOID						=	2202	, 
	REGOPEROID								=	2203	, 
	REGOPERATOROID						=	2204	, 
	REGCLASSOID								=	2205	, 
	REGTYPEOID								=	2206	, 
	REGROLEOID								=	4096	, 
	REGNAMESPACEOID						=	4089	, 
	REGTYPEARRAYOID						=	2211	, 
	UUIDOID										=	2950	, 
	LSNOID										=	3220	, 
	TSVECTOROID								=	3614	, 
	GTSVECTOROID							=	3642	, 
	TSQUERYOID								=	3615	, 
	REGCONFIGOID							=	3734	,
	REGDICTIONARYOID					=	3769	, 
	JSONBOID									=	3802	, 
	INT4RANGEOID							=	3904	, 
	RECORDOID									=	2249	, 
	RECORDARRAYOID						=	2287	, 
	CSTRINGOID								=	2275	, 
	ANYOID										=	2276	, 
	ANYARRAYOID								=	2277	, 
	VOIDOID										=	2278	, 
	TRIGGEROID								=	2279	, 
	EVTTRIGGEROID							=	3838	, 
	LANGUAGE_HANDLEROID				=	2280	, 
	INTERNALOID								=	2281	, 
	OPAQUEOID									=	2282	, 
	ANYELEMENTOID							=	2283	, 
	ANYNONARRAYOID						=	2776	, 
	ANYENUMOID								=	3500	, 
	FDW_HANDLEROID						=	3115	,
	TSM_HANDLEROID						=	3310	,	 
	ANYRANGEOID								=	3831
};

/*---- CLASS -----------------------------------------------------------------------------*/
class SQLPOSTGRESQLRESULT;


class SQLPOSTGRESQLQUERY : public DBSQLQUERY
{
	public:

		friend	class						SQLPOSTGRESQLRESULT;

														SQLPOSTGRESQLQUERY			(DBSQLDATABASE* owner);
		virtual								 ~SQLPOSTGRESQLQUERY			();
		virtual	bool						Execute									();
		virtual	bool						ClearCursor							();
	
	protected:

		virtual	DBSQLRESULT*		ConstructResult					();
		virtual	bool						BindParametersToQuery		();
		bool										BindParametersToResult	();	
		
		PGconn*									conn;

	private:										

		char**									paramvalues;
		int*										paramlengths;						
		int*										paramformats;

		void										Clean										()	
														{
							
														}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

#endif
