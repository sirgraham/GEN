
/*------------------------------------------------------------------------------------------
//	UIWIDGETSCROLLBAR.CPP
//	
//	widget scrollbar
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 23/02/2017 10:10:18
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIWidgetScrollBar.h"

#include "UIManager.h"
#include "UIInput.h"

#include "UIWidgetButton.h"
#include "UIWidgetImage.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	UIWIDGETSCROLLBAR::UIWIDGETSCROLLBAR
*/	
/**	
//	
//	Class Constructor UIWIDGETSCROLLBAR
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/02/2017 10:16:02
//	
//  @param				name : 
//  @param				uimanager : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETSCROLLBAR::UIWIDGETSCROLLBAR(XCHAR* name, UIMANAGER* uimanager) : UIWIDGETCONTAINER(name, uimanager)
{
	Clean();
	SetType();

	minimumbarsize = 25; // ?
	scrolldelta = 0.05f;
}




/*-------------------------------------------------------------------
//	UIWIDGETSCROLLBAR::UIWIDGETSCROLLBAR
*/	
/**	
//	
//	Class Constructor UIWIDGETSCROLLBAR
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/02/2017 10:16:10
//	
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETSCROLLBAR::UIWIDGETSCROLLBAR(UIWIDGETSCROLLBAR* rhs) : UIWIDGETCONTAINER(rhs)
{
	Clean();
	SetType();

	int index = rhs->childrenbyindex.Find(rhs->increase);
	if(index != NOTFOUND)
		{
			increase = (UIWIDGETBUTTON*)childrenbyindex.Get(index);
		}

	index = rhs->childrenbyindex.Find(rhs->decrease);
	if(index != NOTFOUND)
		{
			decrease = (UIWIDGETBUTTON*)childrenbyindex.Get(index);
		}

	index = rhs->childrenbyindex.Find(rhs->background);
	if(index != NOTFOUND)
		{
			background = (UIWIDGETIMAGE*)childrenbyindex.Get(index);
		}

	index = rhs->childrenbyindex.Find(rhs->movebar);
	if(index != NOTFOUND)
		{
			movebar = (UIWIDGETBUTTON*)childrenbyindex.Get(index);
		}
}




/*-------------------------------------------------------------------
//	UIWIDGETSCROLLBAR::~UIWIDGETSCROLLBAR
*/	
/**	
//	
//	 Class Destructor UIWIDGETSCROLLBAR
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/02/2017 10:16:16
//	
*/
/*-----------------------------------------------------------------*/
UIWIDGETSCROLLBAR::~UIWIDGETSCROLLBAR()
{
	Clean();
}




/*-------------------------------------------------------------------
//	UIWIDGETSCROLLBAR::SetWidgets
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/02/2017 16:07:57
//	
//  @param				increase : 
//  @param				decrease : 
//  @param				background : 
//  @param				movebar : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSCROLLBAR::SetWidgets(UIWIDGETBUTTON* increase, UIWIDGETBUTTON* decrease, UIWIDGETIMAGE* background, UIWIDGETBUTTON* movebar)
{
	SetWidgetIncrease(increase);
	SetWidgetDecrease(decrease);
	SetWidgetBackground(background);
	SetWidgetMoveBar(movebar);
}




/*-------------------------------------------------------------------
//	UIWIDGETSCROLLBAR::SetWidgetIncrease
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/02/2017 16:08:05
//	
//  @param				increase : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSCROLLBAR::SetWidgetIncrease(UIWIDGETBUTTON* increase)
{
	if(this->increase)
		{
			uimanager->AddToDeleteQueue(this->increase);
			this->increase = NULL;
		}

	this->increase = increase;
	//AddChild(increase);
	widgetschanged = true;
}



/*-------------------------------------------------------------------
//	UIWIDGETSCROLLBAR::SetWidgetDecrease
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/02/2017 16:08:11
//	
//  @param				decrease : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSCROLLBAR::SetWidgetDecrease(UIWIDGETBUTTON* decrease)
{
	if(this->decrease)
		{
			uimanager->AddToDeleteQueue(this->decrease);
			this->decrease = NULL;
		}

	this->decrease = decrease;
	//AddChild(decrease);
	widgetschanged = true;
}



/*-------------------------------------------------------------------
//	UIWIDGETSCROLLBAR::SetWidgetBackground
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/02/2017 16:08:27
//	
//  @param				background : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSCROLLBAR::SetWidgetBackground(UIWIDGETIMAGE* background)
{
	if(this->background)
		{
			uimanager->AddToDeleteQueue(this->background);
			this->background = NULL;
		}

	this->background = background;
	//AddChild(background);
	widgetschanged = true;
}




/*-------------------------------------------------------------------
//	UIWIDGETSCROLLBAR::SetWidgetMoveBar
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/02/2017 16:08:34
//	
//  @param				movebar : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSCROLLBAR::SetWidgetMoveBar(UIWIDGETBUTTON* movebar)
{
	if(this->movebar)
		{
			uimanager->AddToDeleteQueue(this->movebar);
			this->movebar = NULL;
		}

	this->movebar = movebar;
	//AddChild(movebar);
	widgetschanged = true;
}



/*-------------------------------------------------------------------
//	UIWIDGETSCROLLBAR::InternalUpdate
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/02/2017 11:39:06
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETSCROLLBAR::InternalUpdate(XDWORD widgetlevel)
{
	// to ensure correct rendering order
	if(widgetschanged)
		{
			widgetschanged = false;

			childrenbyindex.DeleteAll();

			childrenbyindex.Add(background);
			childrenbyindex.Add(increase);
			childrenbyindex.Add(decrease);
			childrenbyindex.Add(movebar);

			background->SetParent(this);
			increase->SetParent(this);
			decrease->SetParent(this);
			movebar->SetParent(this);
		}

	// need to save initial mouse/widget ofset to move the widget with the mouse!!

	deltay = 0;

	if((movebar->GetOldWidgetState() & UIWIDGET_STATE_PRESSED) || mousepressed)
		{
			// get cursor position and move the bar accordingly
			float x, y;
			uimanager->GetInput()->GetCursorPosition(x, y);

			x /= uimanager->GetRealWidth();
			y /= uimanager->GetRealHeight();

			x *= uimanager->GetVirtualWidth();
			y *= uimanager->GetVirtualHeight();

			// compute the required scrollposition to move the bar
			// just use mouse displacement to know how much we need to move?
			deltay = (int)(y - oldy);
			mousepressed = true;

			// work with the computed delta
			// need to available space computed and available over here
			// compute the delta on terms of total size and set progress
		}

	if((movebar->GetWidgetState() & UIWIDGET_STATE_PRESSED) || mousepressed)
		{
			// get cursor position and move the bar accordingly
			float x, y;
			uimanager->GetInput()->GetCursorPosition(x, y);

			x /= uimanager->GetRealWidth();
			y /= uimanager->GetRealHeight();

			x *= uimanager->GetVirtualWidth();
			y *= uimanager->GetVirtualHeight();

			oldx = (int)x;
			oldy = (int)y;
		}

	if(uimanager->GetInput()->IsReleased(INPBUTTON_ID_MOUSE_LEFT))
		{
			mousepressed = false;
		}

	return true;
}


/*-------------------------------------------------------------------
//	UIWIDGETSCROLLBAR::InternalArrangeChildren
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/02/2017 16:27:41
//	
*/
/*-----------------------------------------------------------------*/
void UIWIDGETSCROLLBAR::InternalArrangeChildren()
{
	UIWIDGETCONTAINER::InternalArrangeChildren();

	background->adjustedx				= 0;
	background->adjustedy				= 0;
	background->adjustedwidth		= adjustedwidth;
	background->adjustedheight	= adjustedheight;

	increase->adjustedx				= 0;
	increase->adjustedy				= 0;
	increase->adjustedwidth		= adjustedwidth;
	increase->adjustedheight	= adjustedwidth;

	decrease->adjustedx				= 0;
	decrease->adjustedy				= adjustedheight - adjustedwidth;
	decrease->adjustedwidth		= adjustedwidth;
	decrease->adjustedheight	= adjustedwidth;

	// adjust bar accordingly depending on the total size we are covering
	// we also need to position it correctly(duh...)
	int adjustedforbuttons = adjustedheight-2*adjustedwidth;
	float ratio = adjustedforbuttons/(float)(adjustedforbuttons+currentextrasize);

	movebar->adjustedwidth  = adjustedwidth;
	movebar->adjustedheight = (int)(adjustedforbuttons*ratio);
	if(movebar->adjustedheight < minimumbarsize)
		{
			movebar->adjustedheight =  minimumbarsize;
		}

	float moveratio = 0;

	if(currentextrasize > 0)
		{
			//moveratio = deltay/((float)currentextrasize);
			moveratio = deltay/(float)(adjustedforbuttons-movebar->adjustedheight);
		}

	float sc = GetScrollPosition();

	//if(adjustedheight)
	//	{
	//		float p = deltay/(adjustedheight-(width*2));
	//		SetScrollPosition(p);
	//	}

	SetScrollPosition(GetScrollPosition() - moveratio);
	

	int range = adjustedforbuttons - movebar->adjustedheight;
	int posinrange = (int)(range*scrollposition);

	movebar->adjustedx = 0;
	movebar->adjustedy = adjustedwidth + posinrange;

	background->isadjusted	= true;
	increase->isadjusted		= true;
	decrease->isadjusted		= true;
	movebar->isadjusted			= true;
}



/*-------------------------------------------------------------------
//	UIWIDGETSCROLLBAR::HandleInternalEvent
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			24/02/2017 15:55:26
//	
//	@return 			bool : 
//
//  @param				event : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETSCROLLBAR::HandleInternalEvent(XEVENT* event)
{
	if(event->GetEventType() == XEVENTTYPE_UI)
		{
			UIEVENT* uievent = (UIEVENT*)event;
			if(uievent->GetSender() == increase)
				{
					if(uievent->GetSender()->GetWidgetState() & UIWIDGET_STATE_TRIGGERED)
						{
							SetScrollPosition(scrollposition - scrolldelta);
						}
				}
			else if(uievent->GetSender() == decrease)
				{
					if(uievent->GetSender()->GetWidgetState() & UIWIDGET_STATE_TRIGGERED)
						{
							SetScrollPosition(scrollposition + scrolldelta);
						}
				}
		}

	return true;
}
