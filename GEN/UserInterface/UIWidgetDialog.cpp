
/*------------------------------------------------------------------------------------------
//	UIWIDGETDIALOG.CPP
//	
//	dialog class 
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 08/07/2016 11:51:39
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIWidgetDialog.h"
#include "UIManager.h"

#include "XMEmory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	UIWIDGETDIALOG::UIWIDGETDIALOG
*/	
/**	
//	
//	Class Constructor UIWIDGETDIALOG
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/07/2016 13:58:20
//	
//  @param				name : 
//  @param				uimanager : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETDIALOG::UIWIDGETDIALOG(XCHAR* name, UIMANAGER * uimanager) : UIWIDGETFORM(name, uimanager)
{
	Clean();
}





/*-------------------------------------------------------------------
//	UIWIDGETDIALOG::~UIWIDGETDIALOG
*/	
/**	
//	
//	 Class Destructor UIWIDGETDIALOG
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/07/2016 13:58:13
//	
*/
/*-----------------------------------------------------------------*/
UIWIDGETDIALOG::~UIWIDGETDIALOG()
{
	Clean();
}





/*-------------------------------------------------------------------
//	UIWIDGETDIALOG::HandleInternalEvent
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/07/2016 13:58:07
//	
//	@return 			bool : 
//
//  @param				event : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETDIALOG::HandleInternalEvent(XEVENT* event)
{
	// check event, if OK or Cancel Button pressed mark ourselves for deletion
	if(event->GetEventType() == XEVENTTYPE_UI)
		{
			UIEVENT* uievent = (UIEVENT*)event;
			if(uievent->GetUIEventType() == UIXEVENTTYPE_CLICK)
				{
					UIEVENT senduievent(this, XEVENTTYPE_UI);
					senduievent.SetSender(this);

					if(0 == uievent->GetSender()->GetName()->Compare(__L("OK")))
						{
							senduievent.SetUIEventType(UIXEVENTTYPE_DIALOG_OK);
							uimanager->AddToDeleteQueue(this);
						}
					else if(0 == uievent->GetSender()->GetName()->Compare(__L("CANCEL")))
						{
							senduievent.SetUIEventType(UIXEVENTTYPE_DIALOG_CANCEL);
							uimanager->AddToDeleteQueue(this);
						}

					// no post, need to send ti upstream
					//PostEvent(&senduievent);
					if(parent)
						{
							parent->HandleEvent(&senduievent);
						}
				}
		}

	return true;
}
