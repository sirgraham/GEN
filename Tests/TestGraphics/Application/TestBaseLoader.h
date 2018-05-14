/*------------------------------------------------------------------------------------------
//	TESTBASELOADER.H
*/	
/**	
// \class 
//   
//  Test Base Loader
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 2/09/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASELOADER_H_
#define _TESTBASELOADER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"

	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class SNDELEMENT;		
class	SNDFACTORY;
class GRPLOADERINTERFACE;
	
/*---- CLASS -----------------------------------------------------------------------------*/

class TESTBASELOADER : public TESTBASE
{
	public:																				
										TESTBASELOADER						(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)  { block = false; loaderInterface=NULL;} ;

	virtual bool			IniResources							();
	virtual	bool			EndResources							();
	virtual bool			Do												();     	
	virtual bool			InputManagement						();
	virtual bool			Notify										(GRPMSGTYPE msg, GRPSUBJECT* subject);

	bool block;

	GRPLOADERINTERFACE*						loaderInterface;
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif
