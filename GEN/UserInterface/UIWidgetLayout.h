/*------------------------------------------------------------------------------------------
//	UIWIDGETLAYOUT.H
*/	
/**	
// \file 
//   
//  base layout class
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 03/08/2016 16:08:32
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETLAYOUT_H_
#define _UIWIDGETLAYOUT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetContainer.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGETLAYOUT : public UIWIDGETCONTAINER
{
public:

																								UIWIDGETLAYOUT					(XCHAR* name, UIMANAGER * uimanagerarg) : UIWIDGETCONTAINER(name, uimanagerarg)
																								{
																									Clean();
																									SetType();
																								}

																								UIWIDGETLAYOUT(UIWIDGETLAYOUT* rhs) : UIWIDGETCONTAINER(rhs)
																								{
																									Clean();
																									SetType();
																								}

	virtual																			 ~UIWIDGETLAYOUT					()
																								{
																									Clean();
																								}

	virtual					void													SetType									()																		{ this->widgettype = UI_WIDGET_TYPE_LAYOUT; }

	//virtual				bool														AddChild								(UIWIDGET* child); // overload and get correct function

protected:

private:
	void																					Clean										()
																								{
																								}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

