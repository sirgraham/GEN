//------------------------------------------------------------------------------------------
//	GRPWINDOWSFACTORY.H
//	
/**	
// \class 
//   
//  WINDOWS GRAPHICS Factory class
//   
//	@author	 Abraham J. Velez
//	@version 15/07/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _GRPWINDOWSFACTORY_H_
#define _GRPWINDOWSFACTORY_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "GRPFactory.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define GRPWINDOWS_DPI 96.0f

//---- CLASS -------------------------------------------------------------------------------
class GRPCONTEXT;

class GRPWINDOWSFACTORY : public GRPFACTORY
{
	public:
													 GRPWINDOWSFACTORY			()	{	dpi = GRPWINDOWS_DPI; }  
		virtual						 	  ~GRPWINDOWSFACTORY			()  {  }  		
				
		GRPSCREEN*						 CreateScreen 					();		
		bool									 DeleteScreen 					(GRPSCREEN* screen);							

		GRPCONTEXT*						 CreateContext 					();		
		bool									 DeleteContext 					(GRPCONTEXT* context);

		virtual void					GenerateTextureMap			();

};
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

