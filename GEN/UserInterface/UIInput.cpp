
/*------------------------------------------------------------------------------------------
//	UIINPUT.CPP
//	
//	updates the buttons state for the ui
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 08/10/2015 10:01:22
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include <math.h>

#include "INPManager.h"
#include "INPDevice.h"
#include "INPCursor.h"

#include "UIInput.h"
#include "UIManager.h"
#include "UIMotion.h"
#include "UIEvent.h"

#include "XDebug.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	UIINPUT::UIINPUT
*/	
/**	
//	
//	Class Constructor UIINPUT
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/10/2015 9:59:14
//	
//  @param				inpmanagerin : 
*/
/*-----------------------------------------------------------------*/
UIINPUT::UIINPUT(INPMANAGER* inpmanagerin, UIMANAGER* uimanager)
{
	Clean();

	this->inpmanager = inpmanagerin;
	this->uimanager = uimanager;

	// actually hardcoded, should allow for different input interfaces , ...no, si ya
	inpdevicekeyboard			= inpmanager->GetDevice(INPDEVICE_TYPE_KEYBOARD);
	inpdevicemouse				= inpmanager->GetDevice(INPDEVICE_TYPE_MOUSE);
	inpdevicetouchscreen	= inpmanager->GetDevice(INPDEVICE_TYPE_TOUCHSCREEN);

	mousex = 0;
	mousey = 0;

	for(XDWORD i = INPBUTTON_ID_NOBUTTON; i < INPBUTTON_ID_LAST; i++)
		{
			currentbuttons.Add(false);
			previousbuttons.Add(false);
		}

	motion = new UIMOTION();
}

/*-------------------------------------------------------------------
//	UIINPUT::Update
*/	
/**	
//	
//	Updated the vectors with button pressed information
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/10/2015 9:59:53
//	
*/
/*-----------------------------------------------------------------*/
void UIINPUT::Update()
{
	// substitute old values
	for(XDWORD i = 0; i < currentbuttons.GetSize(); i++)
		{
			previousbuttons.Set(i, currentbuttons.Get(i));
			currentbuttons.Set(i, false);
		}
	
	// fill in this frame input
	
	for(int i = INPBUTTON_ID_NOBUTTON; i < INPBUTTON_ID_LAST; i++)
		{
			INPBUTTON * button = NULL;

			if(inpdevicekeyboard)
				{
					button = inpdevicekeyboard->GetButton((INPBUTTON_ID)i);
				}
		
			// fallback to control both keybiard and mouse
			if(button)
				{
					bool pressed = button->IsPressed();
					currentbuttons.Set(i, button->IsPressed());
				}
			else
				{
					if(inpdevicemouse)
						{
							button = inpdevicemouse->GetButton((INPBUTTON_ID)i);
							if(button)
								{
									bool pressed = button->IsPressed();
									currentbuttons.Set(i, button->IsPressed());
								}	
						}
				}
			}
	
	if(inpdevicemouse)
		{
			mousex = inpdevicemouse->GetCursor(INPCURSOR_ID_MOUSE)->GetX();
			mousey = inpdevicemouse->GetCursor(INPCURSOR_ID_MOUSE)->GetY();			
		}

	//// motions should go in here
	//// handle motion events in here?
	//float x, y;
	//GetCursorPosition(x, y);
	//bool istriggered = IsTriggered	(INPBUTTON_ID_MOUSE_LEFT);
	//bool ispressed = IsPressed		(INPBUTTON_ID_MOUSE_LEFT);
	//bool isreleased = IsReleased(INPBUTTON_ID_MOUSE_LEFT);
	//
	//motion->SetCurrentX(x);
	//motion->SetCurrentY(y);
	//motion->SetCurrentTimeStamp(uimanager->GetClockTime());
	//
	//// need a flag to make sure if we are on motion(maybe all of this into update?)
	//if(istriggered)
	//	{
	//		UIMOTION::UIMOTIONENTRY* entry = new UIMOTION::UIMOTIONENTRY();
	//		entry->type = UIMOTION_TYPE_DOWN;
	//		entry->x = x;
	//		entry->y = y;
	//		entry->timestamp = uimanager->GetClockTime();
	//		motion->AddMotion(entry);
	//		// post event
	//		uimanager->PostMotionEvent(motion);
	//	}
	//else if(isreleased)
	//	{
	//		UIMOTION::UIMOTIONENTRY* entry = new UIMOTION::UIMOTIONENTRY();
	//		entry->type = UIMOTION_TYPE_UP;
	//		entry->x = x;
	//		entry->y = y;
	//		entry->timestamp = uimanager->GetClockTime();
	//		motion->AddMotion(entry);
	//		// post event
	//		uimanager->PostMotionEvent(motion);
	//		motion->Clear();
	//	}
	//else if(ispressed)
	//	{
	//		// maybe check cadence
	//		UIMOTION::UIMOTIONENTRY * last = motion->GetLastMotion();
	//		float xdiff = (float)fabs(last->x - x);
	//		float ydiff = (float)fabs(last->y - y);
	//		if((xdiff > 10) || (ydiff > 10))
	//			{
	//				UIMOTION::UIMOTIONENTRY* entry = new UIMOTION::UIMOTIONENTRY();
	//				entry->type = UIMOTION_TYPE_MOVE;
	//				entry->x = x;
	//				entry->y = y;
	//				entry->timestamp = uimanager->GetClockTime();
	//				motion->AddMotion(entry);
	//
	//				// Post Event
	//				uimanager->PostMotionEvent(motion);
	//			}
	//	}
}

/*-------------------------------------------------------------------
//	UIINPUT::IsPressed
*/	
/**	
//	
//	true if the button is pressed
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/10/2015 10:00:11
//	
//	@return 			bool : 
//
//  @param				ID : 
*/
/*-----------------------------------------------------------------*/
bool UIINPUT::IsPressed(INPBUTTON_ID ID)
{
	// use Find to get what index an ID is in
	return currentbuttons.Get(ID);
}

/*-------------------------------------------------------------------
//	UIINPUT::IsTriggered
*/	
/**	
//	
//	true if the button has just been triggered
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/10/2015 10:00:23
//	
//	@return 			bool : 
//
//  @param				ID : 
*/
/*-----------------------------------------------------------------*/
bool UIINPUT::IsTriggered(INPBUTTON_ID ID)
{
	return currentbuttons.Get(ID) && (!previousbuttons.Get(ID));
}

/*-------------------------------------------------------------------
//	UIINPUT::IsReleased
*/	
/**	
//	
//	true if the button has just been released
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/10/2015 10:00:37
//	
//	@return 			bool : 
//
//  @param				ID : 
*/
/*-----------------------------------------------------------------*/
bool UIINPUT::IsReleased(INPBUTTON_ID ID)
{
	return (!currentbuttons.Get(ID)) && previousbuttons.Get(ID);
}

/*-------------------------------------------------------------------
//	UIINPUT::IsPressedLong
*/	
/**	
//	
//	true if the button has been pressed for more than 300ms
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/10/2015 10:00:56
//	
//	@return 			bool : 
//
//  @param				ID : 
*/
/*-----------------------------------------------------------------*/
bool UIINPUT::IsPressedLong(INPBUTTON_ID ID)
{
	// if the button has been pressed for two frames it might have been pressed for a while
	if(currentbuttons.Get(ID) && previousbuttons.Get(ID))
		{
			INPBUTTON * button = inpdevicekeyboard->GetButton(ID);

			if(button)
				{
					// check if it has been pressed for more than 300ms
					// might be redundant
					if(button->GetTimePressed() > 300)
						{
							return true;
						}
				}
			else
				{
					if(inpdevicemouse)
					{
						button = inpdevicemouse->GetButton(ID);

						if(button->GetTimePressed() > 300)
							{
								return true;
							}
					}
				}
		}

	return false;
}



/*-------------------------------------------------------------------
//	UIINPUT::GetCursorPosition
*/	
/**	
//	
//	returns the current position of the cursor
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/10/2015 14:01:14
//	
//  @param				x : 
//  @param				y : 
*/
/*-----------------------------------------------------------------*/
void UIINPUT::GetCursorPosition(float & x, float & y)
{
	x = mousex;
	y = mousey;
}
