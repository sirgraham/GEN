/*------------------------------------------------------------------------------------------
//	UIWIDGETTEXT.H
*/	
/**	
// \file 
//   
//  UIWIDGETTEXT
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 05/10/2015 9:56:11
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETTEXT_H_
#define _UIWIDGETTEXT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetForm.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class UIFACTORY;

class UIWIDGETTEXT : public UIWIDGETFORM
{
	friend class UIFACTORY;

	public:
																									UIWIDGETTEXT						(XCHAR* name, UIMANAGER * uimanagerarg) : UIWIDGETFORM(name, uimanagerarg)
																									{
																										Clean();
																										widgettype = UI_WIDGET_TYPE_TEXT;
																									}

																									UIWIDGETTEXT						(UIWIDGETTEXT* rhs) : UIWIDGETFORM(rhs)
																									{
																										Clean();

																										textchanged		= rhs->textchanged;
																										bold					= rhs->bold;
																										italic				= rhs->italic;

																										R							= rhs->R;
																										G							= rhs->G;
																										B							= rhs->B;
																										text					= rhs->text;
																									}

		virtual																			 ~UIWIDGETTEXT						()																	{ Clean();																}

		virtual				UIWIDGET*												Clone										()
																									{
																										return new UIWIDGETTEXT(this);
																									}

		virtual				void														SetType									()																	{ this->widgettype = UI_WIDGET_TYPE_TEXT;	}




									bool														SetText									(XCHAR * texttoset);
									XSTRING*												GetText									();

									void														SetBold									(bool bold)													{ this->bold = bold;											}
									bool														GetBold									()																	{ return this->bold;											}
									void														SetItalic								(bool italic)												{ this->italic = italic;									}
									bool														GetItalic								()																	{ return this->italic;										}

									void														SetColor								(XBYTE R, XBYTE G, XBYTE B)					{ this->R = R; this->G = G; this->B = B;	}
									XBYTE														GetColorR								()																	{ return this->R;													}
									XBYTE														GetColorG								()																	{ return this->G;													}
									XBYTE														GetColorB								()																	{ return this->B;													}

	protected:

		virtual				bool														InternalDraw						(XDWORD& widgetlevel);
		virtual				bool														InternalUpdate					(XDWORD widgetlevel);

									bool														textchanged;
									bool														bold;
									bool														italic;

									XBYTE														R;
									XBYTE														G;
									XBYTE														B;

	private:
									XSTRING													text;
									void														Clean										()
																									{
																										textchanged			= false;
																										bold						= false;
																										italic					= false;
																										R								= 0;
																										G								= 0;
																										B								= 0;
																									}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

