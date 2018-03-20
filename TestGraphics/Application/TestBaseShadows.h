/*------------------------------------------------------------------------------------------
//	TESTBASESHADOWS.H
*/	
/**	
// \class 
//   
//  Test shadow casting
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 02/06/2016 12:19:19
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASESHADOWS_H_
#define _TESTBASESHADOWS_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"	

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPRENDERTARGET;
class GRPVIEWPORT;

/*---- CLASS -----------------------------------------------------------------------------*/
const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

class TESTBASESHADOWS : public TESTBASE								
{																								
	public:																				
											TESTBASESHADOWS						(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)  { target=NULL; fullviewport=NULL;}  

		virtual bool			InputManagement						();
		virtual bool			IniResources							();    
		virtual	bool			EndResources							();
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

