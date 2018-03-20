//------------------------------------------------------------------------------------------
//	XDIR.H
//	
/**	
// \class 
//   
//  File Directory Class
//   
//	@author	 Abraham J. Velez
//	@version 29/10/2004 10:14:00
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _XDIR_H_
#define _XDIR_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include "XBase.h"
#include "XString.h"
#include "XPath.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum XDIRELEMENTTYPE
{
	XDIRELEMENTTYPE_NONE	= 0 ,
	XDIRELEMENTTYPE_FILE			,
	XDIRELEMENTTYPE_DIR				,
};
	
//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;

class XDIRELEMENT
{
	public:
														XDIRELEMENT							()																{ Clean();                                 } 
		virtual								 ~XDIRELEMENT							()
														{
															DeleteFindFileData();
															Clean();
														}
			

		XDIRELEMENTTYPE					GetType									()																{ return type;                             }	
		void										SetType									(XDIRELEMENTTYPE type)						{ this->type = type;											 }

		XPATH*									GetPathSearch						()																{ return &pathsearch;											 }
		XSTRING*								GetPatternSearch				()																{ return &patternsearch;									 }								
		XPATH*									GetNameFile							()																{ return &namefile;												 }
		
		void*                 	GetHandle								()																{ return handle;                           }
		void	                 	SetHandle								(void* handle)										{ this->handle = handle;                   } 

		void*                 	GetFindFileData					()																{ return findfiledata;                     }
		void	                 	SetFindFileData					(void* findfiledata)							{ this->findfiledata = findfiledata;       }

		bool										DeleteFindFileData			()
														{
															if(!findfiledata) return false;

															delete [] (XBYTE*)findfiledata;

															findfiledata = NULL;

															return true;
														}



	private:
		
		void										Clean										()
														{
															type = XDIRELEMENTTYPE_NONE;	

														  handle       = (void*)-1;
 															findfiledata = NULL;
														}

		XDIRELEMENTTYPE					type;	
		
		XPATH										pathsearch;
		XSTRING									patternsearch;
		XPATH										namefile;

    void*        						handle;
		void*                 	findfiledata;
};



class XDIR
{
	public:
														XDIR										();
		virtual								 ~XDIR										();

		bool										Exist										(XPATH& xpath);
		bool										Make										(XPATH& xpath);
		bool										ChangeTo								(XPATH& xpath);
		bool										Delete									(XPATH& xpath,bool all);	

		virtual bool						Exist										(XCHAR* path)	= 0;
		virtual bool						Make										(XCHAR* path)	= 0;
		virtual bool						ChangeTo								(XCHAR* path)	= 0;
		virtual bool						Delete									(XCHAR* path,bool all)	= 0;	

		bool										FirstSearch							(XPATH& xpath,XSTRING& patternsearch,XDIRELEMENT* searchelement);				
		bool										FirstSearch							(XPATH& xpath,XCHAR* patternsearch,XDIRELEMENT* searchelement);
		bool										FirstSearch							(XCHAR* xpath,XSTRING& patternsearch,XDIRELEMENT* searchelement);		

		virtual bool						FirstSearch							(XCHAR* xpath,XCHAR* patternsearch,XDIRELEMENT* searchelement) = 0;

		virtual bool						NextSearch							(XDIRELEMENT* searchelement)															 = 0;

		bool										Copy										(XPATH& xpathsource,XPATH& xpathtarget);
		bool										Copy										(XCHAR* xpathsource,XPATH& xpathtarget);
		bool										Copy										(XPATH& xpathsource,XCHAR* xpathtarget);
		bool										Copy										(XCHAR* xpathsource,XCHAR* xpathtarget);

	private:
	
		void										Clean										()
														{
															
														}	
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

