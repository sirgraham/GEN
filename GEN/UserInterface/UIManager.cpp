
/*------------------------------------------------------------------------------------------
//	UIMANAGER.CPP
//	
//	manages the widget tree and events
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 08/10/2015 10:02:40
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include <math.h>

#include "XDebug.h"
#include "XSubject.h"
#include "XEvent.h"
#include "XFactory.h"
#include "XTimer.h"
#include "INPManager.h"
#include "INPDevice.h"
#include "INPButton.h"

#include "UIManager.h"
#include "UIWidget.h"
#include "UIEvent.h"
#include "UIWidgetImage.h"

#include "UIMotion.h"

#include "UIStyle.h"
#include "UIInput.h"
#include "UI3DObjectLoader.h"

#include "GRPXEvent.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	UIMANAGER::UIMANAGER
*/	
/**	
//	
//	Class Constructor UIMANAGER
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/10/2015 15:41:32
//	
//  @param				xfactoryin : 
*/
/*-----------------------------------------------------------------*/
UIMANAGER::UIMANAGER(INPMANAGER* inpmanagerin)
{
	Clean();
	
	longpresstime = 500;

	uiclock = xfactory->CreateTimer(); // to generate timed UI events plus time measurement(only need relative time, not date)
	uiclock->Reset();

	inpmanager = inpmanagerin;

	todelete.SetIsMulti(false);
	//toadd.SetIsMulti(false);

	/*
	inpdevicekeyboard = inpmanager->GetDevice(INPDEVICE_TYPE_KEYBOARD);
	inpdevicemouse		= inpmanager->GetDevice(INPDEVICE_TYPE_MOUSE);*/

	//widgetpublisher = new XPUBLISHER();
	
	//this->SetObserverPublisher(widgetpublisher);

	// need a different publisher for outside world
	
	// TODO change how the manager handles events
	this->RegisterEvent(XEVENTTYPE_UI);

	uifactory = new UIFACTORY();
	if(!uifactory)
		{
			return;
		}	


	uiinput = new UIINPUT(inpmanager, this);
	if(!uiinput)
		{
			return;
		}

	ui3dobjectloader = new UI3DOBJECTLOADER();

	mouse = new UIWIDGETIMAGE(__L("Mouse"), this);
	mouse->SetImage(__L("cursor.png"));
	mouse->SetSize(271, 393);

	motion = new UIMOTION();

	// need to setup default skin
}



/*-------------------------------------------------------------------
//	UIMANAGER::~UIMANAGER
*/	
/**	
//	
//	 Class Destructor UIMANAGER
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/03/2017 10:33:32
//	
*/
/*-----------------------------------------------------------------*/
UIMANAGER::~UIMANAGER()
{
	// clean up their elements
	DeRegisterAllEvents();
	if(mouse)
		{
			delete mouse;
		}
	delete ui3dobjectloader;
	delete root;
	delete uifactory;
	delete uiinput;

	delete motion;

	xfactory->DeleteTimer(uiclock);

	Clean();
}

/*-------------------------------------------------------------------
//	UIMANAGER::CreateWidget
*/	
/**	
//	
//	Creates a widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/10/2015 12:58:18
//	
//	@return 			UIWIDGET* : Created widget, NULL if failed
//
//  @param				type : type of widget to create
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIMANAGER::CreateWidget(UI_WIDGET_TYPE type, XCHAR * name)
{
	UIWIDGET * widget = 0;

	// create widget here
	widget = uifactory->CreateWidget(type, this, name);
	if(!widget)
		{
			return NULL;
		}

	return widget;
}

/*-------------------------------------------------------------------
//	UIMANAGER::GetRootWidget
*/	
/**	
//	
//	returns a pointer to the root widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/10/2015 12:58:34
//	
//	@return 			UIWIDGET* : root widget that has been setup
//
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIMANAGER::GetRootWidget()
{
	return root;
}

/*-------------------------------------------------------------------
//	UIMANAGER::GetRootWidget
*/	
/**	
//	
//	adds a new root widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/10/2015 13:41:38
//	
//  @param				rootwidget : widget to be the root
*/
/*-----------------------------------------------------------------*/
void UIMANAGER::SetRootWidget(UIWIDGET * rootwidget)
{
	root = rootwidget;
	root->SetParent(0);
}


/*-------------------------------------------------------------------
//	UIMANAGER::RegisterOwnEvent
*/	
/**	
//	
//	Registers event to send outside
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/10/2015 12:12:50
//	
//  @param				eventtype : event type
*/
/*-----------------------------------------------------------------*/
void UIMANAGER::RegisterOwnEvent(XDWORD eventtype)
{

}

/*-------------------------------------------------------------------
//	UIMANAGER::UnRegisterOwnEvent
*/	
/**	
//	
//	registers events to send outside
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/10/2015 12:13:02
//	
//  @param				eventtype : event type
*/
/*-----------------------------------------------------------------*/
void UIMANAGER::UnRegisterOwnEvent(XDWORD eventtype)
{

}

/*-------------------------------------------------------------------
//	UIMANAGER::HandleEvent
*/	
/**	
//	
//	handles events
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/10/2015 12:13:16
//	
//  @param				*event : event to handle
*/
/*-----------------------------------------------------------------*/
void UIMANAGER::HandleEvent(XEVENT *event)
{
	//XDEBUG_PRINTCOLOR(1,__L("Received event by UIManager"));

	
	// post the event if anyone is interested
	if(event->GetEventType() == XEVENTTYPE_UI)
		{
			UIEVENT * uievent = (UIEVENT*)event;
			uievent->SetSubject(this);

			if(PostEvent(uievent))
				{
					
				}
		}
	else if(event->GetEventType() == GRPXEVENTTYPE_SCREEN_CHANGESIZE)
		{
			if(GetRootWidget())
				{
					GetRootWidget()->modified = true;
				}
		}
}

/*-------------------------------------------------------------------
//	UIMANAGER::Update
*/	
/**	
//	
//	Updated it's own state if needed, and updates the widget tree
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/10/2015 14:07:45
//	
*/
/*-----------------------------------------------------------------*/
void UIMANAGER::Update()
{
	XDWORD deletesize = todelete.GetSize();
	for(XDWORD i = 0; i < deletesize; i++)
		{
			UIWIDGET* w = todelete.FastGet(i);
			if(w)
				{
					UIWIDGET* p = w->GetParent();
					if(p)
						{
							p->RemoveChildPointer(w);
						}
				}

			delete w;
		}

	todelete.DeleteAll();

	if(!disableinput)
		{
			uiinput->Update();
			// handle motion events in here?
			float x, y;
			uiinput->GetCursorPosition(x, y);
			bool istriggered = uiinput->IsTriggered	(INPBUTTON_ID_MOUSE_LEFT);
			bool ispressed = uiinput->IsPressed		(INPBUTTON_ID_MOUSE_LEFT);
			bool isreleased = uiinput->IsReleased(INPBUTTON_ID_MOUSE_LEFT);

			//// need a flag to make sure if we are on motion(maybe all of this into update?)
			//if(istriggered)
			//	{
			//		UIMOTION::UIMOTIONENTRY* entry = new UIMOTION::UIMOTIONENTRY();
			//		entry->type = UIMOTION_TYPE_DOWN;
			//		entry->x = x;
			//		entry->y = y;
			//		motion->AddMotion(entry);
			//		// post event
			//		UIEVENT uievent(this, UIXEVENTTYPE_MOTION);
			//		uievent.SetMotion(motion);
			//		PostEvent(&uievent);
			//	}
			//else if(isreleased)
			//	{
			//		UIMOTION::UIMOTIONENTRY* entry = new UIMOTION::UIMOTIONENTRY();
			//		entry->type = UIMOTION_TYPE_UP;
			//		entry->x = x;
			//		entry->y = y;
			//		motion->AddMotion(entry);
			//		// post event
			//		UIEVENT uievent(this, UIXEVENTTYPE_MOTION);
			//		uievent.SetMotion(motion);
			//		PostEvent(&uievent);
			//	}
			//else if(ispressed)
			//	{
			//		// maybe check cadence
			//		UIMOTION::UIMOTIONENTRY * last = motion->GetLastMotion();
			//		float xdiff = fabs(last->x - x);
			//		float ydiff = fabs(last->y - y);
			//		if((xdiff > 10) || (ydiff > 10))
			//			{
			//				XDEBUG_PRINTCOLOR(2, __L("%f  |  %f"), xdiff, ydiff);
			//				UIMOTION::UIMOTIONENTRY* entry = new UIMOTION::UIMOTIONENTRY();
			//				entry->type = UIMOTION_TYPE_MOVE;
			//				entry->x = x;
			//				entry->y = y;
			//				motion->AddMotion(entry);
			//
			//				// Post Event
			//				UIEVENT uievent(this, UIXEVENTTYPE_MOTION);
			//				uievent.SetMotion(motion);
			//				PostEvent(&uievent);
			//			}
			//	}
		}

	if(GetRootWidget())
		{
			if(modifiedhierarchy)
				{
					GetRootWidget()->modified = true;
					modifiedhierarchy = false;
				}

			// is compute desired size necessary now? could be
			GetRootWidget()->ComputeDesiredSize(); // should call recursively
			GetRootWidget()->ArrangeChildren(); // also recursive call

			GetRootWidget()->UpdateCoords();
			// if a widget aquired the mouse don't try to apply mouse state until released(but need to not exclude children!!)

			hoveredwidget = GetRootWidget()->GetHoveredWidget(false);
			GetRootWidget()->Update(0);
		}

	XDWORD addsize = toadd.GetSize();
	for(XDWORD i = 0; i < addsize; i++)
		{
			// child is the second
			XPAIR<UIWIDGET*, UIWIDGET*> pair = toadd.FastGet(i);

			UIWIDGET* parent = pair.left;
			UIWIDGET* child = pair.right;

			if(child && parent)
				{
					child->SetParent(parent);
					parent->modified = true;
				}
		}

	toadd.DeleteAll();
}

/*-------------------------------------------------------------------
//	UIMANAGER::Draw
*/	
/**	
//	
//	performs the necessary operations to draw the widget tree
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/10/2015 14:08:00
//	
*/
/*-----------------------------------------------------------------*/
void UIMANAGER::Draw()
{
	XDWORD widgetlevel = 0;

	if(GetRootWidget())
		{
			if(modifiedhierarchy)
				{
					GetRootWidget()->modified = true;
					modifiedhierarchy = false;
				}

			GetRootWidget()->Draw(widgetlevel);
			//GetRootWidget()->modified = false;
		}

	// must draw the mosue if enabled
	if(drawmouse)
		{
			if(mouse)
				{
					// set mouse position
					float x, y;
					GetInput()->GetCursorPosition(x, y);

					GRPPOINT pt;

					pt.x = x/GetRealWidth();
					pt.y = y/GetRealHeight();

					pt.x *= GetVirtualWidth();
					pt.y *= GetVirtualHeight();

					pt.y = GetVirtualHeight() - pt.y;

					mouse->SetPosition((int)pt.x, (int)pt.y);
					mouse->SetSize(64, 64);

					XDWORD i = 0;
					mouse->Update(i);
					mouse->UpdateCoords();
					mouse->ArrangeChildren();

					// draw
					
					mouse->Draw(i);
				}
		}
}



/*-------------------------------------------------------------------
//	UIMANAGER::SetUIStyle
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/08/2016 11:36:18
//	
//  @param				uistyle : 
*/
/*-----------------------------------------------------------------*/
void UIMANAGER::SetUIStyle(UISTYLE* uistyle)
{
	this->uistyle = uistyle;
	this->uistyle->SetUIManager(this);
}




/*-------------------------------------------------------------------
//	UIMANAGER::GetUIStyle
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/08/2016 11:36:24
//	
//	@return 			UISTYLE* : 
//
*/
/*-----------------------------------------------------------------*/
UISTYLE* UIMANAGER::GetUIStyle()
{
	return this->uistyle;
}




/*-------------------------------------------------------------------
//	UIMANAGER::AquireMouse
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/02/2017 10:32:22
//	
//	@return 			bool : 
//
//  @param				widget : 
*/
/*-----------------------------------------------------------------*/
bool UIMANAGER::AquireMouse(UIWIDGET* widget)
{
	if(mouseowner == NULL)
		{
			mouseowner = widget;
			return true;
		}

	return false;
}




/*-------------------------------------------------------------------
//	UIMANAGER::ReleaseMouse
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/02/2017 10:32:29
//	
//	@return 			bool : 
//
//  @param				widget : 
*/
/*-----------------------------------------------------------------*/
bool UIMANAGER::ReleaseMouse(UIWIDGET* widget)
{
	if(mouseowner == widget)
		{
			mouseowner = NULL;
			return true;
		}

	return false;
}




/*-------------------------------------------------------------------
//	UIMANAGER::GetMouseOwner
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/02/2017 10:32:35
//	
//	@return 			UIWIDGET* : 
//
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIMANAGER::GetMouseOwner()
{
	return mouseowner;
}




/*-------------------------------------------------------------------
//	UIMANAGER::AquireKeyboard
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/02/2017 10:32:42
//	
//	@return 			bool : 
//
//  @param				widget : 
*/
/*-----------------------------------------------------------------*/
bool UIMANAGER::AquireKeyboard(UIWIDGET* widget)
{
	if(keyboardowner == NULL)
		{
			keyboardowner = widget;
			return true;
		}

	return false;
}




/*-------------------------------------------------------------------
//	UIMANAGER::ReleaseKeyboard
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/02/2017 10:32:48
//	
//	@return 			bool : 
//
//  @param				widget : 
*/
/*-----------------------------------------------------------------*/
bool UIMANAGER::ReleaseKeyboard(UIWIDGET* widget)
{
	if(keyboardowner == widget)
		{
			keyboardowner = NULL;
			return true;
		}

	return false;
}




/*-------------------------------------------------------------------
//	UIMANAGER::GetKeyboardOwner
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/02/2017 10:32:55
//	
//	@return 			UIWIDGET* : 
//
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIMANAGER::GetKeyboardOwner()
{
	return keyboardowner;
}



/*-------------------------------------------------------------------
//	 UIMANAGER::GetClockTime
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version		

//	@return		XQWORD : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
XQWORD UIMANAGER::GetClockTime()
{
	return uiclock->GetMeasureMilliSeconds();
}

