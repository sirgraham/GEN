/*------------------------------------------------------------------------------------------
//	GRPLINUXSCREENDISPMAN.H
*/	
/**	
// \class 
//   
//  Linux Dispman Raspberry graphics screen class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 18/03/2014 17:46:05
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#ifndef _GRPLINUXSCREENDISPMAN_H_
#define _GRPLINUXSCREENDISPMAN_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPOpenGL.h"
#include "GRPScreen.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;

class GRPLINUXSCREENDISPMAN : public GRPSCREEN
{
	public:	
																					GRPLINUXSCREENDISPMAN				();
		virtual															 ~GRPLINUXSCREENDISPMAN				();

		bool																	Create											(void* handle = NULL);
		bool																	Update											();
		bool																	Delete											();


		EGL_DISPMANX_WINDOW_T*								GetDispManHandle();

	private:
		 
		void																	Clean												();
		
		bool																	Init												();		

	//XBYTE*																Buffer;
					

		EGL_DISPMANX_WINDOW_T									handle;
		DISPMANX_ELEMENT_HANDLE_T							dispman_element;
		DISPMANX_DISPLAY_HANDLE_T							dispman_display;
		DISPMANX_UPDATE_HANDLE_T							dispman_update;
		VC_RECT_T															dst_rect;
		VC_RECT_T															src_rect;


		//int																		programID;
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif



