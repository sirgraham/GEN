
/*------------------------------------------------------------------------------------------
//	UIWIDGETMENU.CPP
//	
//	widget menu class
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 11/07/2016 17:52:06
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIWidgetMenu.h"

#include "UIWidgetListContainer.h"

#include "UIWidgetButton.h"
#include "UIWidgetImage.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	UIWIDGETMENU::UIWIDGETMENU
*/	
/**	
//	
//	Class Constructor UIWIDGETMENU
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/07/2016 17:54:34
//	
//  @param				name : 
//  @param				uimanager : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETMENU::UIWIDGETMENU(XCHAR* name, UIMANAGER* uimanager) : UIWIDGETFORM(name, uimanager)
{
	Clean();

	// compose the name?
	container = new UIWIDGETLISTCONTAINER(name, uimanager);
	container->SetScrollMode(UIWIDGETLISTCONTAINER_SCROLL_HORIZONTAL);

	this->AddChild(container);
}




/*-------------------------------------------------------------------
//	UIWIDGETMENU::~UIWIDGETMENU
*/	
/**	
//	
//	 Class Destructor UIWIDGETMENU
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/07/2016 17:54:40
//	
*/
/*-----------------------------------------------------------------*/
UIWIDGETMENU::~UIWIDGETMENU()
{
	// delete the lists
	XLIST<XSTRING*>::XITERATOR it;
	for(it = items.Begin(); it != items.End(); it++)
		{
			delete *it;
		}

	items.DeleteAll();
	actions.DeleteAll();

	Clean();
}




/*-------------------------------------------------------------------
//	UIWIDGETMENU::AddAction
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/07/2016 17:54:46
//	
//  @param				action : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETMENU::AddAction(XCHAR* action)
{
	XSTRING* str = new XSTRING(action);

	items.Add(str);
	actions.Add(str);

	UIWIDGETBUTTON* button = new UIWIDGETBUTTON(action, uimanager);

	// create the state widgets, careful, as they could be subtituted
	UIWIDGETIMAGE* normalimage = new UIWIDGETIMAGE(normal.Get(), uimanager);
	normalimage->SetImage(normal.Get());

	UIWIDGETIMAGE* hoverimage = new UIWIDGETIMAGE(hover.Get(), uimanager);
	hoverimage->SetImage(hover.Get());

	UIWIDGETIMAGE* clickedimage = new UIWIDGETIMAGE(clicked.Get(), uimanager);
	clickedimage->SetImage(clicked.Get());

	button->SetStateWidgets(normalimage, hoverimage, clickedimage);

	container->AddChild(button);
}




/*-------------------------------------------------------------------
//	UIWIDGETMENU::AddMenu
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/07/2016 17:54:52
//	
//  @param				widgetmenu : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETMENU::AddMenu(UIWIDGETMENU* widgetmenu)
{
	XSTRING* str = new XSTRING(widgetmenu->GetName()->Get());

	AddChild(widgetmenu); // shouldn't, as this way parent scissor will be affecting it !!
	widgetmenu->SetActive(false);

	items.Add(str);

	UIWIDGETBUTTON* button = new UIWIDGETBUTTON(widgetmenu->GetName()->Get(), uimanager);

	// create the state widgets, careful, as they could be subtituted
	UIWIDGETIMAGE* normalimage = new UIWIDGETIMAGE(normal.Get(), uimanager);
	normalimage->SetImage(normal.Get());

	UIWIDGETIMAGE* hoverimage = new UIWIDGETIMAGE(hover.Get(), uimanager);
	hoverimage->SetImage(hover.Get());

	UIWIDGETIMAGE* clickedimage = new UIWIDGETIMAGE(clicked.Get(), uimanager);
	clickedimage->SetImage(clicked.Get());

	button->SetStateWidgets(normalimage, hoverimage, clickedimage);

	container->AddChild(button);
}





/*-------------------------------------------------------------------
//	UIWIDGETMENU::SetMenuOrientation
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/09/2016 10:00:41
//	
//  @param				orientation : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETMENU::SetMenuOrientation(UIWIDGET_MENU_ORIENTATION orientation)
{
	this->orientation = orientation;

	if(orientation == UIWIDGET_MENU_ORIENTATION_HORIZONTAL)
		{
			container->SetScrollMode(UIWIDGETLISTCONTAINER_SCROLL_HORIZONTAL);
		}
	else
		{
			container->SetScrollMode(UIWIDGETLISTCONTAINER_SCROLL_VERTICAL);
		}
}




/*-------------------------------------------------------------------
//	UIWIDGETMENU::GetMenuOrientation
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/09/2016 10:00:34
//	
//	@return 			UIWIDGET_MENU_ORIENTATION : 
//
*/
/*-----------------------------------------------------------------*/
UIWIDGET_MENU_ORIENTATION UIWIDGETMENU::GetMenuOrientation()
{
	return orientation;
}




/*-------------------------------------------------------------------
//	UIWIDGETMENU::SetButtonsImages
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/09/2016 10:00:28
//	
//  @param				normal : 
//  @param				hover : 
//  @param				clicked : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETMENU::SetButtonsImages(XCHAR* normal, XCHAR* hover, XCHAR* clicked)
{
	this->normal.Set(normal);
	this->hover.Set(hover);
	this->clicked.Set(clicked);
}



/*-------------------------------------------------------------------
//	UIWIDGETMENU::HandleInternalEvent
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			12/07/2016 11:30:52
//	
//	@return 			bool : 
//
//  @param				event : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETMENU::HandleInternalEvent(XEVENT* event)
{
	// check buttons and if they are menu buttons return false
	if(event->GetEventType() == XEVENTTYPE_UI)
		{
			UIEVENT* uievent = (UIEVENT*)event;

			if(uievent->GetUIEventType() == UIXEVENTTYPE_CLICK)
				{
					// check the button is actually one of our own
					XSTRING* str = uievent->GetSender()->GetName();

					XLIST<XSTRING*>::XITERATOR it;

					for(it = actions.Begin(); it != actions.End(); it++)
						{
							if((*it)->Compare(str->Get()) == 0)
								{
									return true; // own button pass through, if menu capture and show accordingly
								}
						}

					// if we got here not on actions, so it's a submenu
					return false; // now to consider how to handle it
				}
		}


	return true;
}

