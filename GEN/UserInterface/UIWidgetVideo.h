/*------------------------------------------------------------------------------------------
//	UIWIDGETVIDEO.H
*/	
/**	
// \file 
//   
//  reproduces a video
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 27/10/2016 11:57:40
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETVIDEO_H_
#define _UIWIDGETVIDEO_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetForm.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGETVIDEO : public UIWIDGETFORM
{
public:

																								UIWIDGETVIDEO						(XCHAR* name, UIMANAGER* uimanager);
	virtual																			 ~UIWIDGETVIDEO						();

	void																					SetType									()																							{ this->widgettype = UI_WIDGET_TYPE_VIDEO; }

	void																					SetVideo								(XCHAR* video);
	void																					Play										();
	void																					Stop										();

protected:

	XSTRING video;

private:
	void Clean()
	{
	}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

