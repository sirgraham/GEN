/*------------------------------------------------------------------------------------------
//	TESTDIVS.H
*/
/**
// \class
//
//  Manages space divisions
//
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 16/08/2017 13:46:39
//	Last Modification	:
*/
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _TESTCULLING_H_
#define _TESTCULLING_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "TestBase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPSCENE;
class GRPOBJECT;

/*---- CLASS -----------------------------------------------------------------------------*/


class TESTCULLING : public TESTBASE
{
public:

												TESTCULLING(GRPAPPLICATION* xapplication);
		virtual bool				IniResources();
		virtual	bool				EndResources();
		virtual bool				InputManagement();
		virtual bool				Do();


		virtual bool				Notify(GRPMSGTYPE msg, GRPSUBJECT* notifier);

protected:

		GRPSCENE* scene;
		GRPOBJECT* obj;

		bool culling_disabled;

private:

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

