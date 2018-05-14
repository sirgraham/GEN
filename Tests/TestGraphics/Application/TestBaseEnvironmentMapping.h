/*------------------------------------------------------------------------------------------
//	TESTBASEENVIRONMENTMAPPING.H
*/	
/**	
// \class 
//   
//  Test environment mapping
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 03/07/2017 12:19:19
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASEENVIRONMENTMAPPING_H_
#define _TESTBASEENVIRONMENTMAPPING_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"	

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPRENDERTARGET;
class GRPVIEWPORT;
class GRPENVMAPPASS;

/*---- CLASS -----------------------------------------------------------------------------*/
const GLuint ENV_WIDTH = 1024, ENV_HEIGHT = 1024;

class TESTBASEENVIRONMENTMAPPING : public TESTBASE								
{																								
	public:																				
											TESTBASEENVIRONMENTMAPPING(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)  { target=NULL; fullviewport=NULL;}  
		virtual bool			IniResources							();    
		virtual	bool			EndResources							();
		virtual bool			Do												();     
		virtual bool			Notify										(GRPMSGTYPE msg,GRPSUBJECT* notifier);	

		GRPVIEWPORT*			fullviewport;
		GRPRENDERTARGET*	target;		
		GRPPASS*					envpass;
};
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

