/*------------------------------------------------------------------------------------------
//	GRPLINUXDISPMANCONTEXT.H
*/	
/**	
// \file 
//   
//  linux dispman graphics context
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 05/04/2016 12:32:10
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPLINUXDISPMANCONTEXT_H_
#define _GRPLINUXDISPMANCONTEXT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"
#include "GRPContext.h"
#include "GRPOpenGL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPLINUXDISPMANCONTEXT : public GRPCONTEXT
{
public:

																		GRPLINUXDISPMANCONTEXT										();
	virtual													 ~GRPLINUXDISPMANCONTEXT										();

	virtual bool											Create																		(void* handle = NULL);
	virtual bool											Destroy																		();
	virtual bool											IsLost																		();

	virtual bool											Update																		();

protected:

private:
	EGLDisplay												display;
	EGLSurface												surface;
		
	EGLConfig													config;
	EGLContext												context;
	GRPOPENGLCTRL											openglctrl;
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

