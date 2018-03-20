/*------------------------------------------------------------------------------------------
//	GRPBLENDINGPASS.H
*/	
/**	
// \class 
//   
//  Blending pass
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 31/08/2016 11:30:08
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPBLENDINGPASS_H_
#define _GRPBLENDINGPASS_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPPass.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPBLENDINGPASS : public GRPPASS
{
public:				
																GRPBLENDINGPASS		();																	
							int								GetBlendedObjects	();
							void							Begin							();
							void							End								();
	virtual			bool							OnAdded						();

protected:							

private:

			void		Clean()
			{

			}



};
	
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

