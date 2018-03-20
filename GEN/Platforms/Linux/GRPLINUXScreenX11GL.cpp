
/*------------------------------------------------------------------------------------------
//	GRPLINUXSCREENX11GL.CPP
//	
//	OPENGL context under X11
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 02/02/2016 14:29:30
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPLINUXScreenX11.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	GRPLINUXSCREENX11GL::GRPLINUXSCREENX11GL
*/	
/**	
//	
//	Class Constructor GRPLINUXSCREENX11GL
//	
//	@author				Abraham J. Velez
//	@version			28/02/2016 17:03:53
//	
*/
/*-----------------------------------------------------------------*/
GRPLINUXSCREENX11GL::GRPLINUXSCREENX11GL() : GRPLINUXSCREENX11()
{
	type = GRPSCREENTYPE_LINUX_X11_GL;
}



/*-------------------------------------------------------------------
//	GRPLINUXSCREENX11GL::~GRPLINUXSCREENX11GL
*/	
/**	
//	
//	 Class Destructor GRPLINUXSCREENX11GL
//	
//	@author				Abraham J. Velez
//	@version			28/02/2016 17:03:59
//	
*/
/*-----------------------------------------------------------------*/
GRPLINUXSCREENX11GL::~GRPLINUXSCREENX11GL()
{

}



/*-------------------------------------------------------------------
//	GRPLINUXSCREENX11::Create
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/04/2014 17:52:21
//	
//	@return 			bool : 
//
//  @param				handle : 
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENX11GL::Create(void* handle)
{
	// #Imanol : need to make this correct for the non gl versions
	//if(!OpenDisplay()) return false;
      
	return true;
}




/*-------------------------------------------------------------------
//  GRPLINUXSCREENX11::Update
*/ 
/**
//	Updates the window
//	
//	@author				Diego Martinez
//	@version			13/03/2014 13:29:07
//	
//  @pre					display and window are valid handles
//	@return 			bool : success
//	
/*-----------------------------------------------------------------*/
bool GRPLINUXSCREENX11GL::Update()
{		
	if(!display) return false;
	if(!window)	 return false;
				
	//glFlush();
	glXSwapBuffers(display, window);

	return true;
}

