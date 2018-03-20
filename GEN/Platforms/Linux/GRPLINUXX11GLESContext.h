/*------------------------------------------------------------------------------------------
//	GRPLINUXX11GLESCONTEXT.H
*/	
/**	
// \file 
//   
//  GLES X11 context
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 08/04/2016 17:25:43
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPLINUXX11GLESCONTEXT_H_
#define _GRPLINUXX11GLESCONTEXT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "GRPContext.h"
#include "GRPOpenGL.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPLINUXX11GLESCONTEXT : public GRPCONTEXT
{
public:

																		GRPLINUXX11GLESCONTEXT										();
	virtual													 ~GRPLINUXX11GLESCONTEXT										();

	virtual bool											Create																		(void* handle = NULL);
	virtual bool											Destroy																		();
	virtual bool											IsLost																		();

	virtual bool											Update																		();

protected:
	EGLContext												context;
	EGLConfig													config;
	EGLDisplay												display;
	EGLSurface												surface;

	GRPOPENGLCTRL											openglctrl;

private:
	
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

