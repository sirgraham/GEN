/*------------------------------------------------------------------------------------------
//	GRPSTENCILPASS.H
*/	
/**	
// \class 
//   
//  Stencil pass
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 07/11/2016 13:17:47
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPSTENCILPASS_H_
#define _GRPSTENCILPASS_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPPass.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPSTENCILPASS : public GRPPASS
{
public:

										GRPSTENCILPASS		();
										virtual						~GRPSTENCILPASS		() {}
	
	virtual 	void		Begin							();
	virtual   void		Finish						();

	virtual		void		OnDraw						(GRPELEMENT*);

protected:
private:

};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

