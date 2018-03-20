/*------------------------------------------------------------------------------------------
//	UIWIDGET3DMODEL.H
*/	
/**	
// \file 
//   
//  Shows a 3D model in the UI
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 13/02/2017 17:10:51
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGET3DMODEL_H_
#define _UIWIDGET3DMODEL_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetForm.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGET3DMODEL : public UIWIDGETFORM
{
public:

																UIWIDGET3DMODEL																		(XCHAR* name, UIMANAGER* uimanager);
																UIWIDGET3DMODEL																		(UIWIDGET3DMODEL* rhs);
	virtual											 ~UIWIDGET3DMODEL																		();

	void													Set3DModel																				(XCHAR* modelname)									{ this->modelname.Set(modelname);							}
	XSTRING*											Get3DModel																				()																	{ return &modelname;													}

	UIWIDGET*											Clone																							()
																{
																	return new UIWIDGET3DMODEL(this);
																}

	void													SetType																						()																	{ this->widgettype = UI_WIDGET_TYPE_3DMODEL; }

protected:
	XSTRING												modelname;
private:
	void													Clean																							()
																{
																}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

