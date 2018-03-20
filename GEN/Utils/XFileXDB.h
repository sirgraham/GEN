//------------------------------------------------------------------------------------------
//	XFILEXDB.H
//	
/**	
// \class 
//   
//  Dialog Sentence Class
//   
//	@author	 Abraham J. Velez
//	@version 25/05/2004 18:34:57
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _XFILEXDB_H_
#define _XFILEXDB_H_
		
//---- INCLUDES ----------------------------------------------------------------------------

#include "XMap.h"
#include "XFileHash.h"
#include "XFileID.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define XFILEXDB_TYPE				  (XFILE_TYPE_GENERIC | XFILE_FEATURES_HASH)
#define XFILEXDB_VERSION			0x0100
#define XFILEXDB_IDSTRING 	  __L("[XDB File]")
	
//---- CLASS -------------------------------------------------------------------------------


class XFILEXDB : public XFILEHASH
{
	public:
															XFILEXDB								(HASH* hash);
															XFILEXDB								(HASH* hash, XPATH& xpath);
		virtual									 ~XFILEXDB								();

		bool											SetPath									(XPATH& xpath);
		
		bool											Ini											();
		bool											End											();

		XMAP<XDWORD, XDWORD>*			GetIndexMap							();

		bool											OpenFile								();

		int												GetNumberRecords				();
		XBUFFER*									GetRecord								(XDWORD ID);
		XBUFFER*									GetRecordIndex					(XDWORD index);
		XDWORD										GetPosition							(XDWORD ID);

		bool											CloseFile								();


	protected:
		
		HASH*											hash;

		XPATH											xpath;
		bool											isopen;
		
		int												nrecords;	
		XMAP<XDWORD, XDWORD>			indexmap;

	private:

		void											Clean										();
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

