/*------------------------------------------------------------------------------------------
//	UIWIDGETSPINBOXDRAWABLE.H
*/	
/**	
// \file 
//   
//  draws a spinbox
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 02/11/2015 12:31:00
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETSPINBOXDRAWABLE_H_
#define _UIWIDGETSPINBOXDRAWABLE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetSpinBox.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPOBJECT;
class GRPELEMENT;
class GRPTEXTBOX;
class GRPFONT;

class UIWIDGETSPINBOXDRAWABLE : public UIWIDGETSPINBOX
{
public:

																					UIWIDGETSPINBOXDRAWABLE			(XCHAR* name, UIMANAGER * uimanagerargin, GRPSCENE* scene);
	virtual																 ~UIWIDGETSPINBOXDRAWABLE			()
																					{
																						Clean();
																					}

	virtual				bool											InternalDraw								(XDWORD& widgetlevel);
	virtual				bool											InternalUpdate							(XDWORD widgetlevel);

	virtual				void											SetActive										(bool active);

	virtual				void											SetBoxWidgets								(UIWIDGETCONTAINER* upwidget, UIWIDGETCONTAINER* downwidget);

protected:

								void											AdjustText									(float lpt0x, float lpt0y, float lpt1x, float lpt1y, float widgetlevel);

								UIWIDGETCONTAINER*				upwidget;
								UIWIDGETCONTAINER*				downwidget;

								GRPOBJECT*								spinbox;
								GRPELEMENT*								background;
								GRPTEXTBOX*								textdisplay;
								GRPELEMENT*								upbutton;
								GRPELEMENT*								uparrow;
								GRPELEMENT*								downbutton;
								GRPELEMENT*								downarrow;
								GRPFONT*									font;

private:
								void											Clean												()
																					{
																						upwidget		= NULL;
																						downwidget	= NULL;

																						spinbox			= NULL;
																						background	= NULL;
																						textdisplay = NULL;
																						upbutton		= NULL;
																						uparrow			= NULL;
																						downbutton	= NULL;
																						downarrow		= NULL;
																						font				= NULL;
																					}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

