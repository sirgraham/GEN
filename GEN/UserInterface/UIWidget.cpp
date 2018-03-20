
/*------------------------------------------------------------------------------------------
//	UIWIDGET.CPP
//	
//	base widget class
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 08/10/2015 10:04:00
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XDebug.h"

#include "UIManager.h"
#include "UIWidget.h"
#include "UIEvent.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//	UIWIDGET::SetParent
*/	
/**	
//	
//	Sets the new parent of the widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/10/2015 14:01:22
//	
//	@return 			bool : true is suceeded
//
//  @param				p : pointer to the new parent(NULL implies root, although it's not enforced)
*/
/*-----------------------------------------------------------------*/
bool UIWIDGET::SetParent(UIWIDGET* p)
{
	parent = p;
	if(parent)
		{
			// calling the actual function that makes the work
			if(!parent->JoinChild(this)) return false;
		}


	return true;
}

/*-------------------------------------------------------------------
//	UIWIDGET::RegisterOwnEvent
*/	
/**	
//	
//	registers the spacific event
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/10/2015 9:28:38
//	
//  @param				eventtype : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGET::RegisterOwnEvent(XDWORD eventtype, XOBSERVER* observer)
{
	RegisterEvent(eventtype);

	observer->SubscribeEvent(eventtype, this);
}

/*-------------------------------------------------------------------
//	UIWIDGET::UnRegisterOwnEvent
*/	
/**	
//	
//	unregisters the specific event
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/10/2015 9:28:51
//	
//  @param				eventtype : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGET::UnRegisterOwnEvent(XDWORD eventtype, XOBSERVER* observer)
{
	DeRegisterEvent(eventtype);

	observer->UnSubscribeEvent(eventtype, this);
}

/*-------------------------------------------------------------------
//	UIWIDGET::HandleEvent
*/	
/**	
//	
//	handles the event upwards
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/10/2015 9:29:23
//	
//  @param				*event : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGET::HandleEvent(XEVENT *event)
{
	// first handle the specific widget event handler
	// if it returns false don't chainload
	if(HandleInternalEvent(event))
		{
			if(parent)
				{
					// not sure, but if we sent the event to ourselves maybe not send it upwards
					if(event->GetSubject() != this)
						{
							parent->HandleEvent(event);
						}
				}
			else
				{
					// if no parent we are the root and need to send to the manager
					// the manager will copy and transmit the event if necessary
					if(event->GetSubject() != this)
						{
							uimanager->HandleEvent(event);
						}
				}
		}
	
}





/*-------------------------------------------------------------------
//	UIWIDGET::ForceWidgetState
*/	
/**	
//	
//	forces a widget state
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			14/07/2016 12:01:03
//	
//  @param				widgetstate : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGET::ForceWidgetState(XDWORD widgetstate)
{
	this->widgetstate = widgetstate;
	this->oldwidgetstate = widgetstate;

}



/*-------------------------------------------------------------------
//	UIWIDGET::SetSelected
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/06/2016 17:05:36
//	
//  @param				selected : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGET::SetSelected(bool selected)
{
	if(selected)
		{
			widgetstate |= UIWIDGET_STATE_SELECTED;
		}
	else
		{
			widgetstate = widgetstate & ~UIWIDGET_STATE_SELECTED;
		}
}




/*-------------------------------------------------------------------
//	UIWIDGET::IsSelected
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/06/2016 17:05:29
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGET::IsSelected()
{
	return (widgetstate & UIWIDGET_STATE_SELECTED) != 0;
}





/*-------------------------------------------------------------------
//	UIWIDGET::SetPreSelected
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/06/2016 17:05:23
//	
//  @param				preselected : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGET::SetPreSelected(bool preselected)
{
	if(preselected)
		{
			widgetstate |= UIWIDGET_STATE_PRESELECTED;
		}
	else
		{
			widgetstate = widgetstate & ~UIWIDGET_STATE_PRESELECTED;
		}
}





/*-------------------------------------------------------------------
//	UIWIDGET::GetPreSelected
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/06/2016 17:05:15
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGET::GetPreSelected()
{
	return (widgetstate & UIWIDGET_STATE_PRESELECTED) != 0;
}




/*-------------------------------------------------------------------
//	UIWIDGET::AddChild
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			15/07/2016 12:47:33
//	
//	@return 			bool : 
//
//  @param				child : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGET::AddChild(UIWIDGET* child)
{
	uimanager->SetModifiedHierarchy(true);

	//uimanager->AddToAddQueue(this, child);
	return child->SetParent(this);
	return true;
}
		

