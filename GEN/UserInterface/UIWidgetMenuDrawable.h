/*------------------------------------------------------------------------------------------
//	UIWIDGETMENUDRAWABLE.H
*/	
/**	
// \file 
//   
//  widget menu drawable
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 12/07/2016 9:26:13
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETMENUDRAWABLE_H_
#define _UIWIDGETMENUDRAWABLE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetMenu.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGETLISTCONTAINERDRAWABLE;

class UIWIDGETMENUDRAWABLE : public UIWIDGETMENU
{
public:

																												UIWIDGETMENUDRAWABLE									(XCHAR* name, UIMANAGER* uimanager, GRPSCENE* scene);
	virtual																							 ~UIWIDGETMENUDRAWABLE									();

	virtual				bool																		InternalDraw													(XDWORD& widgetlevel);
	virtual				bool																		InternalUpdate												(XDWORD widgetlevel);

	virtual				void																		SetActive															(bool active);

								void																		SetMenuOrientation										(UIWIDGET_MENU_ORIENTATION orientation);
								UIWIDGET_MENU_ORIENTATION								GetMenuOrientation										();
								void																		SetButtonsImages											(XCHAR* normal, XCHAR* hover, XCHAR* clicked);

	virtual				bool																		HandleInternalEvent										(XEVENT* event);


protected:
	UIWIDGETLISTCONTAINERDRAWABLE*												container;


	UIWIDGET_MENU_ORIENTATION															orientation;
	XSTRING																								normal;
	XSTRING																								hover;
	XSTRING																								clicked;

	XDWORD																								addedelemenets;

private:
	void																									Clean												()
																												{
																													container				= NULL;
																													orientation			= UIWIDGET_MENU_ORIENTATION_HORIZONTAL;
																													addedelemenets	= 0;
																												}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

