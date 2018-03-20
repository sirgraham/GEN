/*------------------------------------------------------------------------------------------
//	TESTBASEGGF.H
*/	
/**	
// \class 
//   
//  Test Base GGF (Gen Graphics Format)
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 30/05/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASEGGF_H_
#define _TESTBASEGGF_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
#include "AILogic.h"
#include "SNDFactory.h"
#include "GRPLoaderInterface.h"
#include "GRPBlendingPass.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPFILE3DGGF;

class TESTBASEGGF : public TESTBASE
{
	public:																				
											TESTBASEGGF								(GRPAPPLICATION* xapplication);
	virtual bool				IniResources							();
		
	virtual	bool				EndResources							();
	virtual bool				Do												();     	
	virtual bool				InputManagement						();
	virtual bool				Notify										(GRPMSGTYPE msg, GRPSUBJECT* notifier);

	virtual bool				Save											();
	virtual bool				Load											();

	GRPFILE3D*					file3d;
	GRPLOADERINTERFACE* loaderInterface;

	GRPOBJECT*					object;
	GRPSCENE*						scene;	
	GRPBLENDINGPASS*		blendingpass;
	XPATH xpath;
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

