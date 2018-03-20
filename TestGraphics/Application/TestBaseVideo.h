/*------------------------------------------------------------------------------------------
//	TESTBASEVIDEO.H
*/	
/**	
// \file 
//   
//  video test file
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 22/06/2016 10:07:59
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASEVIDEO_H_
#define _TESTBASEVIDEO_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "TestBase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

#ifndef SND_ACTIVE
class SNDFACTORY;
#endif

class VIDEOPLAYER;

class TESTBASEVIDEO : public TESTBASE//, public XOBSERVER
{
public:

																TESTBASEVIDEO										(GRPAPPLICATION* xapplication);
	virtual											 ~TESTBASEVIDEO										();

	virtual bool									IniResources										();
	virtual	bool									EndResources										();
	virtual bool									Do															();

protected:
	VIDEOPLAYER* videoplayer;
private:
	void Clean()
	{
		videoplayer = NULL;
	}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

