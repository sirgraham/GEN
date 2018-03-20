/*------------------------------------------------------------------------------------------
//	UIWIDGETGRIDCONTAINER.H
*/	
/**	
// \file 
//   
//  grid container for widgets
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 20/02/2017 16:22:25
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETGRIDCONTAINER_H_
#define _UIWIDGETGRIDCONTAINER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetContainer.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class UILISTADAPTERBASE;

class UIWIDGETSCISSORCONTAINER;

class UIWIDGETGRIDCONTAINER : public UIWIDGETCONTAINER
{
public:

																	UIWIDGETGRIDCONTAINER												(XCHAR* name, UIMANAGER* uimanager);
																	UIWIDGETGRIDCONTAINER												(UIWIDGETGRIDCONTAINER* rhs);
	virtual												 ~UIWIDGETGRIDCONTAINER												();

	virtual UIWIDGET*								Clone																				()
																	{
																		return new UIWIDGETGRIDCONTAINER(this);
																	}

	void														SetType																			()																				{ widgettype = UI_WIDGET_TYPE_GRIDCONTAINER;	}

	void														SetAdapter																	(UILISTADAPTERBASE* listadapter)					{ this->listadapter = listadapter;						}
	void														ApplyAdapter																();

	void														SetMargin																		(int margin)															{ this->margins = margin;											}
	void														SetMarginDIP																(float margin);

	int															GetYSize																		()																				{ return ysize;																}

protected:

	virtual bool										InternalUpdate															(XDWORD widgetlevel);

	UILISTADAPTERBASE*							listadapter;
	int															margins;
	int															ysize;

	//UIWIDGETSCISSORCONTAINER*				scissor;

private:
	void														Clean																				()
																	{
																		listadapter		= NULL;
																		margins				= 0;
																		ysize					= 0;
//																		scissor				= NULL;
																	}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

