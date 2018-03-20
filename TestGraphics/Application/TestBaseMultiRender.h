/*------------------------------------------------------------------------------------------
//	TESTBASEMULTIRENDER.H
*/	
/**	
// \class 
//   
//  Test multi viewport render
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 02/06/2016 12:19:19
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASEMULTIRENDER_H_
#define _TESTBASEMULTIRENDER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"	

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPRENDERTARGET;
class GRPVIEWPORT;

/*---- CLASS -----------------------------------------------------------------------------*/


class TESTBASEMULTIRENDER : public TESTBASE								
{																								
	public:																				
											TESTBASEMULTIRENDER				(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)  { }  
		virtual bool			IniResources							();    
		virtual	bool			EndResources							();
		virtual bool			Do												();     
		virtual bool			Notify										(GRPMSGTYPE msg,GRPSUBJECT* notifier);	

		GRPVIEWPORT*			fullviewport;
		GRPRENDERTARGET*	target;

		void Clean()
		{
				fullviewport = NULL;
				target = NULL;
		}
};
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

