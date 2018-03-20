/*------------------------------------------------------------------------------------------
//	TESTBASEOBJECTBUILDER.H
*/	
/**	
// \class 
//   
//  Test Base Object Builder
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 2/09/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASEOBJECTBUILDER_H_
#define _TESTBASEOBJECTBUILDER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPObserver.h"
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/


class TESTBASEOBJECTBUILDER : public TESTBASE
{
public:
		TESTBASEOBJECTBUILDER(GRPAPPLICATION* xapplication);
		virtual bool			IniResources();
		virtual	bool			EndResources();
		virtual bool			Do();
		virtual bool			InputManagement();
		virtual bool			Notify(GRPMSGTYPE msg, GRPSUBJECT* notifier);

		XVECTOR<GRPELEMENT*> elements;

		GRPSCENE* scene;
		bool block;
		float scale;
};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif
