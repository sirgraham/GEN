/*------------------------------------------------------------------------------------------
//	UIWIDGETPROGRESSBAR.H
*/	
/**	
// \file 
//   
//  implements a progress bar
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 02/11/2015 9:21:50
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETPROGRESSBAR_H_
#define _UIWIDGETPROGRESSBAR_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetForm.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class UIWIDGETSCISSORCONTAINER;
class UIWIDGETLAYOUTFITPARENT;
class UIWIDGETIMAGE;

enum UIWIDGETPROGRESSBAR_DIRECTION
{
	UIWIDGETPROGRESSBAR_DIRECTION_HORIZONTAL = 0	,
	UIWIDGETPROGRESSBAR_DIRECTION_VERTICAL				,
};

class UIWIDGETPROGRESSBAR : public UIWIDGETFORM
{
public:

																												UIWIDGETPROGRESSBAR										(XCHAR* name, UIMANAGER * uimanagerargin);
																												UIWIDGETPROGRESSBAR										(UIWIDGETPROGRESSBAR* rhs);
	virtual																							 ~UIWIDGETPROGRESSBAR										();

	virtual				UIWIDGETPROGRESSBAR*										Clone																	()
																												{
																													return new UIWIDGETPROGRESSBAR(this);
																												}

								void																		SetType																()																{ this->widgettype = UI_WIDGET_TYPE_PROGRESSBAR; }

								float																		GetProgress														();
								void																		SetProgress														(float progress);
								float																		IncrementProgress											(float increment);

								void																		SetBackgroundImage										(XCHAR* background);
								void																		SetProgressBarImage										(XCHAR* progressbar);

								void																		SetProgressBarDirection								(UIWIDGETPROGRESSBAR_DIRECTION direction);

protected:
	virtual				bool																		InternalDraw													(XDWORD& widgetlevel);
	virtual				bool																		InternalUpdate												(XDWORD widgetlevel);
	virtual				void																		UnRegisterOwnEvents										();
	virtual				void																		RegisterOwnEvents											();

	virtual				void																		InternalArrangeChildren								();

	virtual				bool																		HandleInternalEvent										(XEVENT* event);

								void																		ProgressChange												();

								float																		progress;
								bool																		progresschanged;

								UIWIDGETSCISSORCONTAINER*								scissor;
								//UIWIDGETLAYOUTFITPARENT*								fitparentprogress;
								UIWIDGETIMAGE*													progressbar;

								UIWIDGETIMAGE*													background;

								UIWIDGETPROGRESSBAR_DIRECTION						direction;

private:
								void																		Clean													()
																												{
																													progress							= 0;
																													progresschanged				= true;

																													scissor								= NULL;
																													//fitparentprogress			= NULL;
																													progressbar						= NULL;
																													
																													background						= NULL;
																													direction							= UIWIDGETPROGRESSBAR_DIRECTION_HORIZONTAL;
																												}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

