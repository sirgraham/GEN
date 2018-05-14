/*------------------------------------------------------------------------------------------
//	TESTBASESHADERLOADER.H
*/
/**
// \class
//
//  Test shader from file
//
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 30/05/2016 16:16:37
//	Last Modification	:
*/
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _TESTBASESHADERLOADER_H_
#define _TESTBASESHADERLOADER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "TestBase.h"
#include "GRPBackgroundPass.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class TESTBASESHADERLOADER : public TESTBASE
{
public:
	TESTBASESHADERLOADER(GRPAPPLICATION* xapplication);
	~TESTBASESHADERLOADER();

	virtual bool									IniResources();
	virtual	bool									EndResources();
	virtual bool									Do();

private:

	void                          Clean()
	{
		pass = NULL;
	}
	GRPTEXTBOX* tb[5];
	GRPBACKGROUNDPASS* pass;
	GRPSHADERPROGRAM* p;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

