/*------------------------------------------------------------------------------------------
//	WINDOWDRAWER.H
*/	
/**	
// \file 
//   
//  draws an individual window
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 21/10/2015 17:47:51
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _WINDOWDRAWER_H_
#define _WINDOWDRAWER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XVector.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPELEMENT;

class WINDOWDRAWER
{
public:

	WINDOWDRAWER() {}
	virtual ~WINDOWDRAWER() {}

protected:

private:

	XVECTOR<GRPELEMENT*> paintelement;

	void Clean()
	{
		paintelement.DeleteAll();
	}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

