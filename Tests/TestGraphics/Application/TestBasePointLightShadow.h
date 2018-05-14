/*------------------------------------------------------------------------------------------
//	TESTBASEPOINTLIGHTSHADOWS.H
*/	
/**	
// \class 
//   
//  Test shadow casting from point lights
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 02/06/2016 12:19:19
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASEPOINTLIGHTSHADOWS_H_
#define _TESTBASEPOINTLIGHTSHADOWS_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"	

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPRENDERTARGET;
class GRPVIEWPORT;

/*---- CLASS -----------------------------------------------------------------------------*/
class TESTBASEPOINTLIGHTSHADOWS : public TESTBASE								
{																								
	public:																				
											TESTBASEPOINTLIGHTSHADOWS	(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)  { target=NULL; fullviewport=NULL;}  
		virtual bool			IniResources							();    
		virtual	bool			EndResources							();
		virtual	bool			InputManagement						() { return true; }	
		virtual bool			Do												();     
		virtual bool			Notify										(GRPMSGTYPE msg,GRPSUBJECT* notifier);	

		GRPVIEWPORT*			fullviewport;
		GRPRENDERTARGET*	target;

		GLuint depthMapFBO;
		GLuint depthMap;

		
		
		GLUINT						handle;
};
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif