/*------------------------------------------------------------------------------------------
//	UIWIDGETLAYOUTFITPARENT.H
*/	
/**	
// \file 
//   
//  fits the layout in the size of its parent
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 03/08/2016 16:44:26
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETLAYOUTFITPARENT_H_
#define _UIWIDGETLAYOUTFITPARENT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetLayout.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
enum UIWIDGETLAYOUTFITPARENT_CONSTRAINS
{
	UIWIDGETLAYOUTFITPARENT_CONSTRAINS_NONE					= 0	,
	UIWIDGETLAYOUTFITPARENT_CONSTRAINS_VERTICAL					,
	UIWIDGETLAYOUTFITPARENT_CONSTRAINS_HORIZONTAL				,
	UIWIDGETLAYOUTFITPARENT_CONSTRAINS_ALL							,
};
	
/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGETLAYOUTFITPARENT : public UIWIDGETLAYOUT
{
public:

																									UIWIDGETLAYOUTFITPARENT																		(XCHAR* name, UIMANAGER* uimanager);
	virtual																				 ~UIWIDGETLAYOUTFITPARENT																		();

																									UIWIDGETLAYOUTFITPARENT																		(UIWIDGETLAYOUTFITPARENT* rhs) : UIWIDGETLAYOUT(this)
																									{
																										Clean();

																										positionconstrains = rhs->positionconstrains;
																										scaleconstrains = rhs->scaleconstrains;
																									}

	bool																						HandleInternalEvent																				(XEVENT* xevent);

	virtual				bool															AddChild																									(UIWIDGET* child);

	void																						SetPositionConstrains																			(UIWIDGETLAYOUTFITPARENT_CONSTRAINS positionconstrains);
	UIWIDGETLAYOUTFITPARENT_CONSTRAINS							GetPositionConstrains																			();

	void																						SetScaleConstrains																				(UIWIDGETLAYOUTFITPARENT_CONSTRAINS scaleconstrains);
	UIWIDGETLAYOUTFITPARENT_CONSTRAINS							GetScaleConstrains																				();

protected:

	virtual	void																		InternalArrangeChildren																		();

	UIWIDGETLAYOUTFITPARENT_CONSTRAINS							positionconstrains;
	UIWIDGETLAYOUTFITPARENT_CONSTRAINS							scaleconstrains;

private:
	void																						Clean																											()
																									{
																										positionconstrains = UIWIDGETLAYOUTFITPARENT_CONSTRAINS_ALL;
																										scaleconstrains = UIWIDGETLAYOUTFITPARENT_CONSTRAINS_ALL;
																									}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

