/*------------------------------------------------------------------------------------------
//	GRPDRAWABLE.H
*/	
/**	
// \class 
//   
//  Interface for drawable entities
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 27/07/2016 9:25:51
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPDRAWABLE_H_
#define _GRPDRAWABLE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPNamed.h"

	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPMATRIX;
class GRPPASS;
class GRPSHADERPROGRAM;
class GRPELEMENT;

#define GRPRENDERLIST XLIST<GRPELEMENT*>* 

/*---- CLASS -----------------------------------------------------------------------------*/
class GRPDRAWABLE 
{
public : 

		virtual GRPSHADERPROGRAM*				GetProgram					()=0;
		virtual void										SetProgram					(GRPSHADERPROGRAM*)=0;

		virtual bool										Draw								(const GRPMATRIX*  viewmatrix, const GRPMATRIX*  viewprojection, const GRPMATRIX*  projection) = 0;
		
		/*
		virtual bool										Draw								()=0;
		virtual bool										Draw								(GRPPASS* pass)=0;
		virtual bool										Draw								(const GRPMATRIX&	view,	const GRPMATRIX*	viewprojection,	const GRPMATRIX*	projection)=0;		
			*/			
		int															GetRenderPriority		()																																							{ return renderpriority;									}
		void														SetRenderPriority		(int renderpriority)																														{ this->renderpriority = renderpriority;	}

protected:
		int															renderpriority;
};

	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

