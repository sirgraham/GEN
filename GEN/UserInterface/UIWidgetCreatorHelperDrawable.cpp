
/*------------------------------------------------------------------------------------------
//	UIWIDGETCREATORHELPERDRAWABLE.CPP
//	
//	helper class to create drawable widgets
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 22/06/2016 11:41:59
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIWidgetCreatorHelperDrawable.h"
#include "UIWidgetButtonDrawable.h"
#include "UIWidgetImageDrawable.h"
#include "UIWidgetTextDrawable.h"
#include "UIWidgetDialogDrawable.h"

#include "UIWidgetLayoutFitParent.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	UIWIDGETCREATORHELPERDRAWABLE::UIWIDGETCREATORHELPERDRAWABLE
*/	
/**	
//	
//	Class Constructor UIWIDGETCREATORHELPERDRAWABLE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			22/06/2016 11:56:29
//	
//  @param				uimanager : 
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETCREATORHELPERDRAWABLE::UIWIDGETCREATORHELPERDRAWABLE(UIMANAGER* uimanager, GRPSCENE* scene) : UIWIDGETCREATORHELPER(uimanager)
{
	Clean();

	this->scene = scene;
}



/*-------------------------------------------------------------------
//	UIWIDGETCREATORHELPERDRAWABLE::~UIWIDGETCREATORHELPERDRAWABLE
*/	
/**	
//	
//	 Class Destructor UIWIDGETCREATORHELPERDRAWABLE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			22/06/2016 11:56:09
//	
*/
/*-----------------------------------------------------------------*/
UIWIDGETCREATORHELPERDRAWABLE::~UIWIDGETCREATORHELPERDRAWABLE()
{
	Clean();
}




/*-------------------------------------------------------------------
//	UIWIDGETCREATORHELPERDRAWABLE::CreateButton
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			22/06/2016 11:56:00
//	
//	@return 			UIWIDGET* : 
//
//  @param				text : 
//  @param				background : 
//  @param				hover : 
//  @param				click : 
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETCREATORHELPERDRAWABLE::CreateButton(XCHAR* text, XCHAR* background, XCHAR* hover, XCHAR* click)
{
	UIWIDGETBUTTONDRAWABLE* widget = new UIWIDGETBUTTONDRAWABLE(text, uimanager, scene);
	if(!widget) return NULL;
	
	UIWIDGETLAYOUTFITPARENT* fitparent = new UIWIDGETLAYOUTFITPARENT(text, uimanager);

	widget->AttachLayout(fitparent);

	UIWIDGETIMAGEDRAWABLE* imagebackground	= NULL;
	UIWIDGETIMAGEDRAWABLE* imagehover				= NULL;
	UIWIDGETIMAGEDRAWABLE* imageclick				= NULL;
	
	if(background)
		{
			imagebackground = new UIWIDGETIMAGEDRAWABLE(text, uimanager, scene);
			imagebackground->SetImage(background);
			fitparent->AddChild(imagebackground);
		}
	
	if(hover)
		{
			widget->SetISHoverable(true);
			imagehover = new UIWIDGETIMAGEDRAWABLE(text, uimanager, scene);
			imagehover->SetImage(hover);
			fitparent->AddChild(imagehover);
		}
	
	if(click)
		{
			widget->SetIsClickable(true);
			imageclick = new UIWIDGETIMAGEDRAWABLE(text, uimanager, scene);
			imageclick->SetImage(click);
			fitparent->AddChild(imageclick);
		}

	widget->SetStateWidgets(imagebackground, imagehover, imageclick);
	
	if(text)
		{
			UIWIDGETTEXTDRAWABLE* textwidget = new UIWIDGETTEXTDRAWABLE(text, uimanager, scene);
			textwidget->SetText(text);
			textwidget->SetHorizontalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
			textwidget->SetVerticalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
			fitparent->AddChild(textwidget);
		}
	

	return widget;

	return NULL;
}


UIWIDGET* UIWIDGETCREATORHELPERDRAWABLE::CreateMessageDialog(XCHAR* text, XCHAR* backimage, XCHAR* background, XCHAR* hover, XCHAR* click)
{
	//UIWIDGETDIALOGDRAWABLE* widget = new UIWIDGETDIALOGDRAWABLE(__L("DIALOG"), uimanager, scene);
	//if(!widget) return NULL;
	//
	//if(backimage)
	//	{
	//		UIWIDGETIMAGEDRAWABLE* backimagewidget = NULL;
	//		backimagewidget = new UIWIDGETIMAGEDRAWABLE(text, uimanager, scene);
	//		backimagewidget->SetImage(backimage);
	//		backimagewidget->FitParent(true);
	//		widget->AddChild(backimagewidget);
	//	}	
	//
	//if(text)
	//	{
	//		UIWIDGETTEXTDRAWABLE* textwidget = new UIWIDGETTEXTDRAWABLE(text, uimanager, scene);
	//		textwidget->SetText(text);
	//		textwidget->SetOrigin(0.10f, 0.05f);
	//		textwidget->SetEndingPoint(0.9f, 0.65f);
	//		textwidget->SetHorizontalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
	//		textwidget->SetVerticalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
	//		widget->AddChild(textwidget);
	//	}
	//
	//UIWIDGETBUTTONDRAWABLE* buttonwidget = new UIWIDGETBUTTONDRAWABLE(__L("OK"), uimanager, scene);
	//
	//UIWIDGETIMAGEDRAWABLE* imagebackground	= NULL;
	//UIWIDGETIMAGEDRAWABLE* imagehover				= NULL;
	//UIWIDGETIMAGEDRAWABLE* imageclick				= NULL;
	//
	//if(background)
	//	{
	//		imagebackground = new UIWIDGETIMAGEDRAWABLE(text, uimanager, scene);
	//		imagebackground->SetImage(background);
	//		imagebackground->FitParent(true);
	//	}
	//
	//if(hover)
	//	{
	//		widget->SetISHoverable(true);
	//		imagehover = new UIWIDGETIMAGEDRAWABLE(text, uimanager, scene);
	//		imagehover->SetImage(hover);
	//		imagehover->FitParent(true);
	//	}
	//
	//if(click)
	//	{
	//		widget->SetIsClickable(true);
	//		imageclick = new UIWIDGETIMAGEDRAWABLE(text, uimanager, scene);
	//		imageclick->SetImage(click);
	//		imageclick->FitParent(true);
	//	}
	//
	//buttonwidget->SetOrigin(0.0f, 0.70f);
	//buttonwidget->SetEndingPoint(1.0f, 1.0f);
	//buttonwidget->SetIsClickable(true);
	//buttonwidget->SetISHoverable(true);
	//buttonwidget->SetStateWidgets(imagebackground, imagehover, imageclick);
	//
	//UIWIDGETTEXTDRAWABLE* buttontextwidget = new UIWIDGETTEXTDRAWABLE(text, uimanager, scene);
	//buttontextwidget->SetText(__L("OK"));
	//buttontextwidget->SetOrigin(0.15f, 0.15f);
	//buttontextwidget->SetEndingPoint(0.85f, 0.85f);
	//buttontextwidget->SetHorizontalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
	//buttontextwidget->SetVerticalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
	//buttonwidget->AddChild(buttontextwidget);
	//
	//widget->AddChild(buttonwidget);
	//
	//return widget;

	return NULL;
}

