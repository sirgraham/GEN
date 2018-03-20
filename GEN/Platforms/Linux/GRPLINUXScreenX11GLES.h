/*------------------------------------------------------------------------------------------
//	GRPLINUXSCREENX11GLES.H
*/	
/**	
// \file 
//   
//  Opens an OpenGL ES context via EGL on X11
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 02/02/2016 16:00:05
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPLINUXSCREENX11GLES_H_
#define _GRPLINUXSCREENX11GLES_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPLINUXScreenX11.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPLINUXSCREENX11GLES : public GRPLINUXSCREENX11
{
public:

																				GRPLINUXSCREENX11GLES( );
	virtual															 ~GRPLINUXSCREENX11GLES();

	bool																	Create											(void* handle = NULL);
	bool																	Update											();

protected:

	EGLDisplay														display;
	EGLSurface														surface;
	EGLContext														context;

private:
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

