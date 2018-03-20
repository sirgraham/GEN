/*------------------------------------------------------------------------------------------
//	UIWIDGETLISTCONTAINER.H
*/	
/**	
// \file 
//   
//  linear widget container
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 27/06/2016 17:34:01
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETLISTCONTAINER_H_
#define _UIWIDGETLISTCONTAINER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetContainer.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
enum UIWIDGETLISTCONTAINER_SCROLL
{
	UIWIDGETLISTCONTAINER_SCROLL_VERTICAL = 0	,
	UIWIDGETLISTCONTAINER_SCROLL_HORIZONTAL		,
};
	
/*---- CLASS -----------------------------------------------------------------------------*/

class UILISTADAPTERBASE;

class UIWIDGETLISTCONTAINER : public UIWIDGETCONTAINER
{
public:

																					UIWIDGETLISTCONTAINER													(XCHAR* name, UIMANAGER* uimanager);
																					UIWIDGETLISTCONTAINER													(UIWIDGETLISTCONTAINER* rhs);
	virtual																 ~UIWIDGETLISTCONTAINER													();

	virtual				void											SetType																				()																										{ this->widgettype = UI_WIDGET_TYPE_LIST; }

	virtual				UIWIDGETLISTCONTAINER*		Clone																					()
																					{
																						return new UIWIDGETLISTCONTAINER(this);
																					}

	UIWIDGETLISTCONTAINER_SCROLL						GetScrollMode																	();
	void																		SetScrollMode																	(UIWIDGETLISTCONTAINER_SCROLL scrollmode);

	XDWORD																	GetScrolling																	();
	void																		SetScrolling																	(XDWORD scroll);

	void																		SetPxMargin																		(XDWORD pxmargin);
	XDWORD																	GetPxMargin																		();
	void																		SetDIPMargin																	(float dipmargin);
	float																		GetDIPMargin																	();

	virtual				void											SetActive																			(bool active);

	virtual void														SetAdapter																		(UILISTADAPTERBASE* adapter);
	virtual void														ApplyAdapter																	();

	virtual	GRPPOINT												ComputeDesiredSize														();

protected:

	virtual				bool											InternalDraw																	(XDWORD& widgetlevel);
	virtual				bool											InternalUpdate																(XDWORD widgetlevel);
	virtual				bool											InternalPostUpdate														(XDWORD widgetlevel);
	virtual				void											UnRegisterOwnEvents														();
	virtual				void											RegisterOwnEvents															();

	virtual				bool											HandleInternalEvent														(XEVENT* event);

	UIWIDGETLISTCONTAINER_SCROLL						scrollmode;
	XDWORD																	scroll;

	bool																		usepxmargin;
	bool																		usedipmargin;

	XDWORD																	pxmargin;
	float																		dipmargin;

	UILISTADAPTERBASE*											adapter;

	//float																		oldgpt0x;
	//float																		oldgpt0y;
	//float																		oldgpt1x;
	//float																		oldgpt1y;

private:
	void																		Clean																															()
																					{
																						scrollmode		= UIWIDGETLISTCONTAINER_SCROLL_VERTICAL;
																						scroll				= 0;
																						usepxmargin		= false;
																						usedipmargin	= false;

																						pxmargin			= 0;
																						dipmargin			= 0.0f;
																					}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

