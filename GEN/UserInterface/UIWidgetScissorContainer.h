/*------------------------------------------------------------------------------------------
//	UIWIDGETSCISSORCONTAINER.H
*/	
/**	
// \file 
//   
//  scissor its children
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 13/09/2016 13:20:12
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETSCISSORCONTAINER_H_
#define _UIWIDGETSCISSORCONTAINER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetContainer.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGETSCISSORCONTAINER : public UIWIDGETCONTAINER
{
public:

																													UIWIDGETSCISSORCONTAINER																		(XCHAR* name, UIMANAGER* uimanager);
																													UIWIDGETSCISSORCONTAINER																		(UIWIDGETSCISSORCONTAINER* rhs);
	virtual																								 ~UIWIDGETSCISSORCONTAINER																		();

	virtual				UIWIDGETSCISSORCONTAINER*									Clone																												()
																													{
																														return new UIWIDGETSCISSORCONTAINER(this);
																													}

protected:

	virtual				void																			InternalArrangeChildren																			();

private:
								void																			Clean																												()
																													{
																													}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

