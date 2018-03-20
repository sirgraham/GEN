/*------------------------------------------------------------------------------------------
//	UIWIDGETEXCLUSIVECONTAINER.H
*/	
/**	
// \file 
//   
//  UI container that only allows a single widget to be pressed
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 14/07/2016 11:25:04
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETEXCLUSIVECONTAINER_H_
#define _UIWIDGETEXCLUSIVECONTAINER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetForm.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGETEXCLUSIVECONTAINER : public UIWIDGETFORM
{
public:

																										UIWIDGETEXCLUSIVECONTAINER																				(XCHAR* name, UIMANAGER* uimanager);
																										UIWIDGETEXCLUSIVECONTAINER																				(UIWIDGETEXCLUSIVECONTAINER* rhs);
	virtual																					 ~UIWIDGETEXCLUSIVECONTAINER																				();

	virtual				UIWIDGETEXCLUSIVECONTAINER*					Clone																															()
																										{
																											return new UIWIDGETEXCLUSIVECONTAINER(this);
																										}

	virtual				void																SetActive																													(bool active);
	void																							SetCurrentWidget																									(UIWIDGET* widget)														{ currentwidget = widget; }
	UIWIDGET*																					GetCurrentWidget																									()																						{ return currentwidget;		}

protected:

	virtual				bool																InternalUpdate																										(XDWORD widgetlevel);
	virtual				bool																HandleInternalEvent																								(XEVENT* event);

								UIWIDGET*														currentwidget;
private:
								void																Clean																															()
																										{
																											currentwidget = NULL;
																										}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

