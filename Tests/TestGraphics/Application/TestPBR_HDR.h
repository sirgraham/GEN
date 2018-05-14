/*------------------------------------------------------------------------------------------
//	TESTPBR_HDR.H
*/
/**
// \class
//
// Tests HDR buffers
//
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 16/08/2017 13:46:39
//	Last Modification	:
*/
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _TESTPBR_HDR_H_
#define _TESTPBR_HDR_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "TestBase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/


class TESTHDR : public TESTBASE
{
public:

												TESTHDR					(GRPAPPLICATION* xapplication);
		virtual bool				IniResources		();

		virtual	bool				EndResources		();
		virtual bool				Do							();
		virtual bool				InputManagement	();


		virtual bool				Notify					(GRPMSGTYPE msg, GRPSUBJECT* notifier);

protected:		

		GRPSHADERPROGRAM* hdrprogram;
		GRPSHADERPROGRAM* defaultprogram;
		GRPPASS* hdr;
		GRPPASS* postprocess;

		float gamma;
		float exposure;
		float intensity;

private:


		void Clean()
		{
				intensity			 = 1.0;
				gamma					 = 1.0;
				exposure			 = 1.0;
				hdrprogram		 = NULL;
				defaultprogram = NULL;
		}
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

