/*------------------------------------------------------------------------------------------
//	TESTLAYERS.H
*/	
/**	
// \class 
//   
//  Test layers from files
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 28/11/2016 10:00:47
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTLAYERS_H_
#define _TESTLAYERS_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
#include "AILogic.h"
#include "SNDFactory.h"
#include "GRPLoaderInterface.h"
#include "GRPBlendingPass.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class TESTLAYERS : public TESTBASE
{
	public:																				
											TESTLAYERS								(GRPAPPLICATION* xapplication);
	virtual bool				IniResources							();
	
	virtual bool				Load											();
	virtual	bool				EndResources							();
	virtual bool				Do												();     	
	virtual bool				InputManagement						();
	virtual bool				Notify										(GRPMSGTYPE msg, GRPSUBJECT* notifier);

	GRPLOADERINTERFACE* loaderInterface;

	GRPOBJECT*					object;
	GRPSCENE*						scene;	
	GRPPASS*						blendingpass;
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

