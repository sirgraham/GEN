/*------------------------------------------------------------------------------------------
//	GRPLINUXFRAMEBUFFERCONTEXT.H
*/	
/**	
// \file 
//   
//  linux framebuffer graphics context
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 06/04/2016 12:59:22
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPLINUXFRAMEBUFFERCONTEXT_H_
#define _GRPLINUXFRAMEBUFFERCONTEXT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"
#include "GRPContext.h"

#include "GRPOpenGL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPLINUXFRAMEBUFFERCONTEXT : public GRPCONTEXT
{
public:
																		GRPLINUXFRAMEBUFFERCONTEXT								();
	virtual													 ~GRPLINUXFRAMEBUFFERCONTEXT								();

	virtual bool											Create																		(void* handle = NULL);
	virtual bool											Destroy																		();
	virtual bool											IsLost																		();

protected:

private:
	// need to mnove this over to context on all platforms(should not be hard)
	EGLDisplay												display;
	EGLSurface												surface;

	EGLContext												context;
	GRPOPENGLCTRL											openglctrl;
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

