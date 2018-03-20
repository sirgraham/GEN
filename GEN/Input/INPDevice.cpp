//------------------------------------------------------------------------------------------
//	INPDEVICE.CPP
//	
//	Input Device Class
//  
//	Author						: Abraham J. Velez
//	Date Of Creation	: 11/02/2003
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XVector.h"

#include "INPButton.h"	
#include "INPCursor.h"	
#include "INPDevice.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  INPDEVICE::INPDEVICE
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/03/2004 16:31:12
//	
//	@return				

*/
//-------------------------------------------------------------------
INPDEVICE::INPDEVICE()
{
	Clean();

	
}



//-------------------------------------------------------------------
//  INPDEVICE::~INPDEVICE
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/02/2003 18:06:35
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
INPDEVICE::~INPDEVICE()
{
	Clean();
}
	


//-------------------------------------------------------------------
//  INPDEVICE::IsCreated
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/02/2003 16:58:37
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool INPDEVICE::IsCreated()
{
	return created;
}


//-------------------------------------------------------------------
//  INPDEVICE::IsEnabled
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/02/2003 17:46:13
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool INPDEVICE::IsEnabled()
{
	return enabled;
}



//-------------------------------------------------------------------
//  INPDEVICE::SetEnabled
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/02/2003 17:32:56
//	
//	@return 			void : 
//	@param				ON : 
*/
//-------------------------------------------------------------------
void INPDEVICE::SetEnabled(bool ON)
{
	enabled  = ON;
}



//-------------------------------------------------------------------
//  INPDEVICE::GetType
/**
//	
//	@author        Abraham J. Velez
//	@version       20/2/2003   10:20:30
//	
//	@return        INPDEVICE_TYPE : 
*/
//-------------------------------------------------------------------
INPDEVICE_TYPE INPDEVICE::GetType()
{
	return type;
}


//-------------------------------------------------------------------
//  INPDEVICE::SetType
/**
//	
//	@author        Abraham J. Velez
//	@version       20/2/2003   10:20:50
//	
//	@return        void : 
//	@param	       type : 
*/
//-------------------------------------------------------------------
void INPDEVICE::SetType(INPDEVICE_TYPE type)
{
	this->type = type;
}


//-------------------------------------------------------------------
//  INPDEVICE::GetNButtons
/**
//	
//	@author        Abraham J. Velez
//	@version       18/2/2003   15:27:3
//	
//	@return        int : 
*/
//-------------------------------------------------------------------
int INPDEVICE::GetNButtons()
{
	XVECTOR<INPBUTTON*>* buttons = GetButtons();
	if(!buttons) return 0;

	return buttons->GetSize();
}



//-------------------------------------------------------------------
//  INPDEVICE::GetButton
/**
//	
//	@author        Abraham J. Velez
//	@version       18/2/2003   15:20:59
//	
//	@return        INPBUTTON* : 
//	@param	       ID : 
*/
//-------------------------------------------------------------------
INPBUTTON* INPDEVICE::GetButton(INPBUTTON_ID ID)
{
	XVECTOR<INPBUTTON*>* buttons = GetButtons();
	if(!buttons) return NULL;
	
	XDWORD size=buttons->GetSize();
	for(XDWORD c=0;c<size;c++)
		{
			INPBUTTON*	button = (INPBUTTON*)buttons->FastGet(c);
			if(button)
					if(button->GetID() == ID) 
							return button;								
		}

	return NULL;
}

/*-------------------------------------------------------------------
//	 INPDEVICE::GetButtonState
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	02/01/2018 12:49:26
//	@return		INPBUTTON_STATE : 
//
//	@param		INPBUTTON_ID : 
//
*//*-----------------------------------------------------------------*/
INPBUTTON_STATE INPDEVICE::GetButtonState(INPBUTTON_ID ID)
{
		XVECTOR<INPBUTTON*>* buttons = GetButtons();
		if (!buttons) return INPBUTTON_STATE_UNKNOWN;

		XDWORD size = buttons->GetSize();
		for (XDWORD c = 0; c<size; c++)
		{
				INPBUTTON*	button = (INPBUTTON*)buttons->FastGet(c);
				if (button)
						if (button->GetID() == ID)
								return button->GetState();
		}

		return INPBUTTON_STATE_UNKNOWN;
}




/*-------------------------------------------------------------------
//  INPDEVICE::GetButtonByCode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/04/2012 17:01:48
//	
//	@return 			INPBUTTON* : 
//	@param				code : 
*/
/*-----------------------------------------------------------------*/
INPBUTTON* INPDEVICE::GetButtonByCode(XWORD code)
{
	XVECTOR<INPBUTTON*>* buttons = GetButtons();
	if(!buttons) return NULL;

	if(buttons->IsEmpty()) return NULL;

	for(XDWORD c=0;c<buttons->GetSize();c++)
		{
			INPBUTTON*	button = (INPBUTTON*)buttons->Get(c);
			if(button)
				{	
					if(button->GetKeyCode() == code) return button;								
				}
		}

	return NULL;
}




/*-------------------------------------------------------------------
//  INPDEVICE::GetButton
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/11/2009 06:06:28 p.m.
//	
//	@return				INPBUTTON* : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
INPBUTTON* INPDEVICE::GetButton(int index)
{	
	XVECTOR<INPBUTTON*>* buttons = GetButtons();
	if(!buttons) return NULL;

	if(buttons->IsEmpty())					return NULL;
	if(index>=(int)buttons->GetSize()) return NULL;
	if(index<0)											return NULL;

	return (INPBUTTON*)buttons->Get(index);	
}



/*-------------------------------------------------------------------
//  INPDEVICE::IsPressButton
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/11/2009 06:17:00 p.m.
//	
//	@return				INPBUTTON* : 
//	*/
/*-----------------------------------------------------------------*/
INPBUTTON* INPDEVICE::IsPressButton()
{
	XVECTOR<INPBUTTON*>* buttons = GetButtons();
	if(!buttons) return NULL;

	for(int c=0;c<(int)buttons->GetSize();c++)
		{
			INPBUTTON* button = GetButton(c);
			if(button)
				{
					if(button->IsPressed()) return button;
				}
		}

	return NULL;
}



/*-------------------------------------------------------------------
//  INPDEVICE::GetNCursors
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/12/2011 11:18:07
//	
//	@return				int : 
//	*/
/*-----------------------------------------------------------------*/
int INPDEVICE::GetNCursors()
{
	XVECTOR<INPCURSOR*>* cursors = GetCursors();
	if(!cursors) return 0;

	return cursors->GetSize();
}




/*-------------------------------------------------------------------
//  INPDEVICE::GetCursor
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/12/2011 11:18:28
//	
//	@return				INPCURSOR* : 
//	@param				ID : 
*/
/*-----------------------------------------------------------------*/
INPCURSOR* INPDEVICE::GetCursor(INPCURSOR_ID ID)
{
	XVECTOR<INPCURSOR*>* cursors = GetCursors();
	if(!cursors) return NULL;

	if(cursors->IsEmpty()) return NULL;

	for(XDWORD c=0;c<cursors->GetSize();c++)
		{
			INPCURSOR* cursor = (INPCURSOR*)cursors->Get(c);
			if(cursor)
				{	
					if(cursor->GetID() == ID) return cursor;		
				}
		}

	return NULL;	
}




/*-------------------------------------------------------------------
//  INPDEVICE::GetCursor
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/12/2011 11:21:49
//	
//	@return				INPCURSOR* : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
INPCURSOR* INPDEVICE::GetCursor(int index)
{
	XVECTOR<INPCURSOR*>* cursors = GetCursors();
	if(!cursors) return NULL;

	if(cursors->IsEmpty())					return NULL;
	if(index>=(int)cursors->GetSize()) return NULL;
	if(index<0)											return NULL;

	return (INPCURSOR*)cursors->Get(index);	
}




/*-------------------------------------------------------------------
//  INPDEVICE::IsChangeCursor
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/12/2011 11:21:56
//	
//	@return				INPCURSOR* : 
//	*/
/*-----------------------------------------------------------------*/
INPCURSOR* INPDEVICE::IsChangeCursor()
{
	XVECTOR<INPCURSOR*>* cursors = GetCursors();
	if(!cursors) return NULL;

	for(int c=0;c<(int)cursors->GetSize();c++)
		{
			INPCURSOR* cursor = GetCursor(c);
			if(cursor)
				{
					if(cursor->IsChanged()) return cursor;
				}
		}

	return NULL;
}




/*-------------------------------------------------------------------
//  INPDEVICE::GetButtons
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/12/2011 9:52:44
//	
//	@return				XVECTOR* : 
//	*/
/*-----------------------------------------------------------------*/
XVECTOR<INPBUTTON*>* INPDEVICE::GetButtons()
{
	return NULL;
}



/*-------------------------------------------------------------------
//  XVECTOR* INPDEVICE::GetCursorss
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/12/2011 9:53:03
//	
//	@return				virtual : 
//	*/
/*-----------------------------------------------------------------*/
XVECTOR<INPCURSOR*>* INPDEVICE::GetCursors()
{
	return NULL;
}



//-------------------------------------------------------------------
//  INPDEVICE::Relase
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/08/2004 12:04:40
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool INPDEVICE::Release()
{	
	return false;
}


//-------------------------------------------------------------------
//  INPDEVICE::Update
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/02/2003 11:36:33
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool INPDEVICE::Update()
{	
	return true;
}




//-------------------------------------------------------------------
//  INPDEVICE::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/02/2003 18:07:37
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void INPDEVICE::Clean()
{
	created		= false;	
	enabled		= false;

	type			=	INPDEVICE_TYPE_NONE;
}
