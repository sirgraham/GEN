/*------------------------------------------------------------------------------------------
//	UIWIDGETTOASTDRAWABLE.H
*/	
/**	
// \file 
//   
//  toast drawable
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 11/07/2016 16:03:51
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETTOASTDRAWABLE_H_
#define _UIWIDGETTOASTDRAWABLE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetToast.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPSCENE;
class GRPOBJECT;
class GRPELEMENT;

class UIWIDGETTOASTDRAWABLE : public UIWIDGETTOAST
{
public:

																UIWIDGETTOASTDRAWABLE																			(XCHAR* name, UIMANAGER * uimanager, GRPSCENE* scene);
	virtual											 ~UIWIDGETTOASTDRAWABLE																			();

	virtual				bool						InternalDraw																							(XDWORD& widgetlevel);
	virtual				bool						InternalUpdate																						(XDWORD widgetlevel);

	virtual				void						SetActive																									(bool active);

protected:

	GRPOBJECT*										obj;
	GRPELEMENT*										element;

private:
	void													Clean																											()
																{
																	obj			= NULL;
																	element = NULL;
																}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

