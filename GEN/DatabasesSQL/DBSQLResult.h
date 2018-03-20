/*------------------------------------------------------------------------------------------
//	DBSQLRESULT.H
*/	
/**	
// \class 
//   
//  Holds result set data
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 11/08/2015 16:23:19
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifdef DBSQL_ACTIVE

#ifndef _DBSQLRESULT_H_
#define _DBSQLRESULT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "DBSQLBase.h"
#include "DBSQLVariant.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;


class DBSQLRESULT
{
	public:

																					DBSQLRESULT					() : row()
																					{ 
																						Clean(); 
																					}

	 virtual															 ~DBSQLRESULT					()										
																					{																						
																						row.Clear();

																						header.DeleteContents();
																						header.DeleteAll();	

																						Clean();																		
																					}

	 DBSQLROW*															GetRow							()										{ return &row;																}
	 DBSQLROWHEADER*												GetHeader						()										{ return &header;															}
	 virtual XDWORD													GetCurrentRowIndex	()										{ return currentrow;													}

	 virtual XQWORD													GetNumRows					()						= 0;
	 virtual XQWORD													GetNumColumns				()						= 0;
	 virtual bool														Seek								(int index)		= 0;
   virtual bool														First								()						= 0;
   virtual bool														Last								()						= 0;
	 virtual bool														Next								()						= 0;
	 virtual bool														ProcessRow					()						= 0;
	 virtual bool														Abort								()						= 0;
	 virtual bool														HasNext							()						= 0;

	protected:

		bool																	initialized;
		XDWORD																currentrow;
		DBSQLROWHEADER												header;
		DBSQLROW															row;		

	private:

		void																	Clean								()
																					{
																						currentrow			= 0;
																						initialized			= false;
																					}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

#endif
