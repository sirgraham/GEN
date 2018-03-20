/*------------------------------------------------------------------------------------------
//	TESTBASEDIOFUNCTIONS.H
*/	
/**	
// \class 
//   
//  Test Base DataIO Functions
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 30/05/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASEDIOFUNCTIONS_H_
#define _TESTBASEDIOFUNCTIONS_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPCONSOLE;

class TESTBASEDIOFUNCTIONS : public TESTBASE								
{																								
	public:																				
																	TESTBASEDIOFUNCTIONS			(GRPAPPLICATION* xapplication);
																 ~TESTBASEDIOFUNCTIONS			();

		virtual bool									IniResources							(); 
		virtual bool									EndResources							(); 

		bool													Test_DIOStreamTCPIP				(bool modeserver);
		bool													Test_DIOStreamUDP					(bool modeserver);
		bool													Test_DIOPing							();
		bool													Test_DIOStreamUSB								();

		virtual bool									Do												(); 

		static void										ThreadRunFunction					(void* param);	
		
		XPATH													xpath;
		XTIMER*												xtimer;
		XTHREAD*                      xthread;
		GRPCONSOLE*										console;	
		XRAND*                        xrand;
};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

