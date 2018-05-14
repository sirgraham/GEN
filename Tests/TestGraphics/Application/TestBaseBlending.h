/*------------------------------------------------------------------------------------------
//	TESTBLENDING.H
*/	
/**	
// \class 
//   
//  Tests blending on the engine
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 08/06/2016 11:06:34
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBLENDING_H_
#define _TESTBLENDING_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	

#include "TestBase.h"		

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPRENDERTARGET;
class GRPVIEWPORT;	
class GRPPASS;
	
/*---- CLASS -----------------------------------------------------------------------------*/

class TESTBASEBLENDING : public TESTBASE								
{																								
	public:																				
											TESTBASEBLENDING					(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)  { }  
		virtual bool			IniResources							();    
		virtual	bool			EndResources							();
		virtual bool			Do												();     
		virtual bool			Notify										(GRPMSGTYPE msg,GRPSUBJECT* notifier);	

		GRPPASS*					blending;
};
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif


