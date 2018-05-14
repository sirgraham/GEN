/*------------------------------------------------------------------------------------------
//	TESTBASELEDSYNC.H
*/	
/**	
// \class 
//   
//  Test Base Led Synchronize
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 30/05/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASELEDSYNC_H_
#define _TESTBASELEDSYNC_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
#include "GRPInterfaceAnimable.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
 
class DIOI2CPWMCONTROLERPCA9685;

class TESTBASELEDSYNC : public TESTBASE,public GRPINTERFACEANIMABLE
{
	public:
																		TESTBASELEDSYNC						(GRPAPPLICATION* xapplication);
																   ~TESTBASELEDSYNC						();		
																		 
		virtual bool										IniResources							();  
		virtual bool										EndResources							();  
		virtual bool										Do												();    
		virtual bool										SetValue									(GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS subtype, double value,double base=0.0);
		
	protected:

		void														Clean										()
																		{
																			red						= 0;
																			green					= 0;
																			blue					= 0;
																			intensity			= 0;
																			pca9685				= NULL;
																		}

		XBYTE														red;
		XBYTE														green;
		XBYTE														blue;
		XBYTE														intensity;
		DIOI2CPWMCONTROLERPCA9685*					pca9685;

};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

