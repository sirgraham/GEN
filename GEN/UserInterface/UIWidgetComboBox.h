/*------------------------------------------------------------------------------------------
//	UIWIDGETCOMBOBOX.H
*/	
/**	
// \file 
//   
//  combo box
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 06/02/2017 11:22:57
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETCOMBOBOX_H_
#define _UIWIDGETCOMBOBOX_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetContainer.h"

#include "XVector.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class UILISTADAPTERBASE;
class UIWIDGETBUTTON;
class UIWIDGETIMAGE;

class UIWIDGETCOMBOBOX : public UIWIDGETCONTAINER
{
public:

																UIWIDGETCOMBOBOX													(XCHAR* name, UIMANAGER* uimanager);
																UIWIDGETCOMBOBOX													(UIWIDGETCOMBOBOX* rhs);
	virtual											 ~UIWIDGETCOMBOBOX													();

	virtual			UIWIDGET*					Clone()
																{
																	return new UIWIDGETCOMBOBOX(this);
																}

	virtual				void						SetType																		()																										{ this->widgettype = UI_WIDGET_TYPE_COMBOBOX;					}

	void													SetAdapter																(UILISTADAPTERBASE* adapter);
	void													ApplyAdapter															();

	virtual				void						SetActive																	(bool active);

	void													SetUpComboButton													(XCHAR* normal, XCHAR* hover, XCHAR* clicked);

	void													SetDefaultIndex														(XDWORD index)																				{ if(selectedindex == 0) { selectedindex = index; }		}
	void													SetSelectedIndex													(XDWORD index)																				{ selectedindex = index;															}
	XDWORD												GetSelectedIndex													()																										{ return selectedindex;																}

protected:

	virtual				bool						InternalDraw															(XDWORD& widgetlevel);
	virtual				bool						InternalUpdate														(XDWORD widgetlevel);
	virtual				bool						InternalPostUpdate												(XDWORD widgetlevel);
	virtual				void						UnRegisterOwnEvents												();
	virtual				void						RegisterOwnEvents													();

	virtual				bool						HandleInternalEvent												(XEVENT* event);

	virtual				void						InternalArrangeChildren										();

	XDWORD												selectedindex;
	UIWIDGETBUTTON*								combobutton;
	UIWIDGETIMAGE*								combobackground;
	UIWIDGET*											displayedwidget;
	bool													oldlockedmouse;

	XVECTOR<UIWIDGETCONTAINER*>		entries;
	UILISTADAPTERBASE*						adapter;

private:
	void													Clean																		()
																{
																	selectedindex			= 0;
																	combobutton				= NULL;
																	combobackground		= NULL;
																	displayedwidget		= NULL;
																	adapter						= NULL;
																	oldlockedmouse		= false;
																}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

