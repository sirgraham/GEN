/*------------------------------------------------------------------------------------------
//	GRPBACKGROUNDPASS.H
*/	
/**	
// \class 
//   
//  Background
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 12/09/2016 11:09:59
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPBACKGROUNDPASS_H_
#define _GRPBACKGROUNDPASS_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPPass.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
class GRPMATERIAL;

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPBACKGROUNDPASS : public GRPPASS
{
public:																				
																GRPBACKGROUNDPASS	(XCHAR* name=__L("background"));																								
virtual													~GRPBACKGROUNDPASS();

							void							Begin();
							void							End();
							void							SetMaterial(GRPMATERIAL* material);
							GRPMATERIAL*			GetMaterial() { return material;  }

							void							SetProgram (GRPSHADERPROGRAM* program);
virtual				bool							OnAdded();

protected:							

			GRPMATERIAL*			material;
			GRPELEMENT*				plane;
			GRPSHADERPROGRAM* program;

private:

			void		Clean()
			{
				material=NULL;
			}



};	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif


