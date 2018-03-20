/*------------------------------------------------------------------------------------------
//	GRPRENDERER.H
*/	
/**	
// \file 
//   
//  Class that encapsules the scenes
//   
//	@author	Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 16/10/2015 16:18:36
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPRENDERER_H_
#define _GRPRENDERER_H_
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPOpenGL.h"
#include "GRPColor.h"
#include "GRPSettings.h"

#include "XVector.h"
#include "XList.h"
#include "XThread.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum GRPSCENE_CLEARFLAGS_TYPE
{
	GRPSCENE_CLEARFLAGS_NONE=0,
	GRPSCENE_CLEAR_COLOR				= GL_COLOR_BUFFER_BIT,
	GRPSCENE_CLEAR_DEPTH				= GL_DEPTH_BUFFER_BIT,
	GRPSCENE_CLEAR_STENCIL			= GL_STENCIL_BUFFER_BIT,
	GRPSCENE_CLEAR_ALL					=(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)
};
	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPSCENE;
class GRPRENDERTARGET;
class GRPOBJECTBUILDER;
class GRPSCREEN;
class GRPCONTEXT;
class GRPFACTORY;
class GRPSHADERPROGRAM;
class GRPOBJECT;

class XFACTORY;

class GRPRENDERER : public GRPOBSERVER
{
public:

																									GRPRENDERER											(GRPSCREEN* mainscreen,  GRPCONTEXT* context, bool postprocess = false);
	virtual																				 ~GRPRENDERER											();

								bool															Ini															();
								void															End															();

								GRPSCENE*													GetScene												(int index);
								void															AddScene												(GRPSCENE* scene);

								void															Render													();
								void															PrepareScenes										();
								bool															SetupPostProcessing							();
								bool															Notify													(GRPMSGTYPE msg,GRPSUBJECT*);
								void															SetClearColor										(GRPCOLOR c);

								void															SetClearFlags										(GRPSCENE_CLEARFLAGS_TYPE clearflags)						{	this->ClearFlags=clearflags;											}
								GRPCOLOR													GetClearColor										()																							{ return this->ClearColor;													}
								void															Clear														()																							{	GRPOPENGLCTRL::COLOR::Clear(this->ClearFlags);		}
								void															Clear														(GLUINT v)																			{ GRPOPENGLCTRL::COLOR::Clear(v);										}
								GRPCONTEXT*												GetContext											()																							{ return this->context;															}
								GRPSCREEN*												GetMainScreen										()																							{ return this->mainscreen;													}

								XVECTOR<GRPSCENE*>*								GetScenes												() { return &this->scenes;}
															
	// to add on clean
	// ClearFlags						= GRPSCENE_CLEAR_ALL;	


protected:

								GLUINT														ClearFlags;

private:

	//----------- rendering
								GRPSCREEN*												mainscreen;
								GRPCONTEXT*												context;

								GRPCOLOR													ClearColor;

								bool															postprocess;
								bool															pingpong;
								GRPRENDERTARGET*									rendertarget0;
								GRPRENDERTARGET*									rendertarget1;

								GRPOBJECT*												PostProcessCanvas;

								XVECTOR<GRPSHADERPROGRAM*>				PostProcessingProgramStack;


								XVECTOR<GRPSCENE*>								scenes;





};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

