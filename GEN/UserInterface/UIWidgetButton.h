/*------------------------------------------------------------------------------------------
//	UIWIDGETPUSHBUTTON.H
*/	
/**	
// \file 
//   
//  class that implements a push button
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 05/10/2015 10:35:14
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETPUSHBUTTON_H_
#define _UIWIDGETPUSHBUTTON_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetForm.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class UIFACTORY;

class UIWIDGETBUTTON : public UIWIDGETFORM
{
	friend class UIFACTORY;

	public:
																									UIWIDGETBUTTON							(XCHAR* name, UIMANAGER * uimanagerargin);
																									UIWIDGETBUTTON							(UIWIDGETBUTTON* rhs);
		virtual																			 ~UIWIDGETBUTTON							()																													{ Clean();										}

		virtual				UIWIDGET*												Clone												();

		virtual				void														SetActive										(bool active);

		virtual				void														SetStateWidgets							(UIWIDGET* widgetnormal = NULL, UIWIDGET* widgethover = NULL, UIWIDGET* widgetpressed = NULL);

		virtual				UIWIDGET*												GetWidgetNormal							()																													{ return this->widgetnormal;	}
		virtual				UIWIDGET*												GetWidgetHover							()																													{ return this->widgethover;		}
		virtual				UIWIDGET*												GetWidgetPressed						()																													{ return this->widgetpressed; }

		virtual				bool														SubstituteNormal						(UIWIDGET* widgetnormal);
		virtual				bool														SubstituteHover							(UIWIDGET* widgethover);
		virtual				bool														SubstitutePressed						(UIWIDGET* widgetpressed);

		virtual				bool														CheckCursorOver							(float zoom = 1, int x = 0, int y = 0);

	protected:

		virtual				bool														InternalDraw								(XDWORD& widgetlevel);
		virtual				bool														InternalUpdate							(XDWORD widgetlevel);
		virtual				void														UnRegisterOwnEvents					();
		virtual				void														RegisterOwnEvents						();

		virtual				bool														HandleInternalEvent					(XEVENT* event);

									UIWIDGET*												widgetnormal;
									UIWIDGET*												widgethover;
									UIWIDGET*												widgetpressed;
									UIWIDGET*												activewidget;

	private:
									void														Clean												()
																									{
																										issticky			= false;
																										widgetnormal	= NULL;
																										widgethover		= NULL;
																										widgetpressed = NULL;
																										activewidget	= NULL;
																									}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

