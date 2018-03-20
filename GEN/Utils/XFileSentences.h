//------------------------------------------------------------------------------------------
//	XFILESEN.H
//	
/**	
// \class 
//   
//  XFile Sentences Class
//   
//	@author	 Abraham J. Velez
//	@version 25/05/2004 18:34:57
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _XFILESEN_H_
#define _XFILESEN_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XMap.h"
#include "XFileXDB.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define XFILESENTENCES_TYPE						(XFILE_TYPE_GENERIC | XFILE_FEATURES_HASH)
#define XFILESENTENCES_VERSION				0x0100
#define XFILESENTENCES_IDSTRING 			__L("[Sentences File]")

#define XFILESENTENCES_EXTENSION			__L(".sen");

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;

/*
class XSENTENCE
{
	public:
                        XSENTENCE           ();
                        
		XDWORD							ID;
		char*								str;
};

*/


class XFILESENTENCES : public XFILEXDB
{
	public:
													XFILESENTENCES						( HASH* hash, XPATH& xpath);
		virtual							 ~XFILESENTENCES						();

		bool									ConvertFileFromDBF				(XPATH& xpath);

		int										GetNumberSentences				();		
		bool									GetSentence								(XDWORD index,XSTRING& sentence);
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

