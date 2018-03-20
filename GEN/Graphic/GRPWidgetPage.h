/*------------------------------------------------------------------------------------------
// GRPWidgetPage.HPP
*/
/**
// \class
//
// Manages widget pages
//
//	@author: Diego Martinez Ruiz de Gaona 
//
//	Date of Creation : 12/12/2017 11:19:16 
//	Last Modification : 
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


#ifndef _GRPWIDGETPAGE_HPP
#define _GRPWIDGETPAGE_HPP

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPWidget.h"
#include "GRPWidgetPass.h"
#include "GRPStyleProcessorCSS.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/



class GRPWIDGETPAGE : public GRPOBSERVER, public GRPSUBJECT
{
public:
		GRPWIDGETPAGE(XCHAR* toload = NULL);
		GRPWIDGETPAGE(XCHAR* toload, GRPSTYLEPROCESSORCSS* processor, GRPWIDGETFACTORY* factory);
		~GRPWIDGETPAGE();

		bool											Notify(GRPMSGTYPE, GRPSUBJECT*);
		XDWORD 										GetSubjectType() { return GRPNAMED_TYPE_WIDGET; }

		bool											Update();

		bool											Add(INPDEVICE*						inpdevice);
		bool											Add(GRPWIDGET*						widget);
		bool											Add(GRPWIDGETSTYLECLASS*	classstyle);
		bool											Add(GRPSTYLESHEET*				sheet);
		
		bool											ParseStyle(XCHAR*);		

		bool											Set(GRPWIDGET*, GRPWIDGETSTATE);

		int												Hit(GLFLOAT x, GLFLOAT y, XVECTOR<GRPWIDGET*>*hits, GRPWIDGET* parent);
		GRPWIDGET*								Pick(GLFLOAT x, GLFLOAT y);
		GRPWIDGET*								SubPick(GLFLOAT x, GLFLOAT y, GRPWIDGET*);

		GRPWIDGET*								Get(XCHAR* name);
		GRPWIDGET*								FindByName(XCHAR* name);
		GRPWIDGET*								FindByName(XCHAR* name, GRPWIDGET* parent);
		GRPWIDGET*								FindByTag(XCHAR* name);
		GRPWIDGET*								FindByTag(XCHAR* name, GRPWIDGET* parent);

		GRPWIDGET*								GetByClass(XCHAR* name);
		GRPWIDGET*								FindByClass(XCHAR* name, GRPWIDGET* parent);

		GRPSCENE*									GetScene();

		virtual bool							Load() = 0;

		virtual GRPWIDGET*				Create(XCHAR* tag = NULL, XCHAR* name = NULL, XCHAR* classname = NULL, XCHAR* style = NULL, XCHAR* content = NULL);
		bool											AssignStyleToWidget(GRPWIDGET* widget);

		void											FitInterfaceToScreen();
		void											CreateProcessor();
		void											CreateWidgetFactory();

		GRPSTYLEPROCESSORCSS*			processor;
		GRPWIDGETFACTORY*					widgetfactory;

		GRPWIDGET*								root;
		GRPWIDGET*								focus;

		XVECTOR<GRPSTYLESHEET*>		stylesheets;
		GRPWIDGETPASS*						ownerpass;
		XSTRING										content;

		GRPWIDGET*								currentActiveWidget;
		GRPWIDGET*								currentHoverWidget;

		XVECTOR<INPDEVICE*>				inputdevices;


		void Clean()
		{
				root = NULL;
				focus = NULL;
				processor = NULL;
				currentActiveWidget = NULL;
				currentHoverWidget = NULL;
				widgetfactory = NULL;
		}
};

class GRPWIDGETPAGEHTML : public GRPWIDGETPAGE
{
public:		

								GRPWIDGETPAGEHTML		(XCHAR* load) : GRPWIDGETPAGE(load) {};
								GRPWIDGETPAGEHTML		(XCHAR* load, 
																		 GRPSTYLEPROCESSORCSS* processor, 
																		 GRPWIDGETFACTORY* factory) 
																				: GRPWIDGETPAGE(load, processor, factory) {};
		
		bool	ParseElement							(XFILEXMLELEMENT*			element, GRPWIDGET* father);		
		int		ReplaceHTMLEntities				(XSTRING& s);
		bool	Load											();		
		
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif