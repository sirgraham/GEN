/*------------------------------------------------------------------------------------------
//	INPANDROIDDEVICEMOUSE.CPP
//	
//	Input Device Touch Screen as mouse
//   
//	Author						: Diego martinez ruiz de gaona
//	Date Of Creation	: 19/06/2017
//	Last Mofificacion	:	
//	
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	

#include "XANDROIDSystem.h"
#include "GRPANDROIDScreen.h"
#include "INPCursor.h"

#include "INPANDROIDDeviceMouse.h"

#include "XMemory.h"
	

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  INPANDROIDDEVICEMOUSE::INPANDROIDDEVICEMOUSE
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/03/2012 12:20:23
//	
//	@return 			

//	@param				grpscreen : 
*/
/*-----------------------------------------------------------------*/
INPANDROIDDEVICEMOUSE::INPANDROIDDEVICEMOUSE( GRPANDROIDSCREEN*	grpscreen) : INPDEVICE()
{
	Clean();
		
	SetType(INPDEVICE_TYPE_MOUSE);

	created = true;

	this->SetScreen(grpscreen);

	if(CreateAllCursors())
	if(CreateAllButtons()) 
		SetEnabled(true);	
}



/*-------------------------------------------------------------------
//  INPANDROIDDEVICEMOUSE::~INPANDROIDDEVICEMOUSE
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			21/12/2011 17:26:28
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
INPANDROIDDEVICEMOUSE::~INPANDROIDDEVICEMOUSE()
{
	DeleteAllCursors();

	SetEnabled(false);
	created = false;

	Clean();
}

/*-------------------------------------------------------------------
//	 INPANDROIDDEVICEMOUSE::SetScreen
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		void* : 
//
*//*-----------------------------------------------------------------*/
bool INPANDROIDDEVICEMOUSE::SetScreen(void* screenpointer)
{
		this->grpscreen = static_cast<GRPANDROIDSCREEN*>(screenpointer);
		return true;
}

/*-------------------------------------------------------------------
//  INPANDROIDDEVICEMOUSE::GetCursors
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/12/2011 11:54:25
//	
//	@return				XVECTOR* : 
//	*/
/*-----------------------------------------------------------------*/
XVECTOR<INPCURSOR*>* INPANDROIDDEVICEMOUSE::GetCursors()
{
	return &cursors;
}




/*-------------------------------------------------------------------
//  INPANDROIDDEVICEMOUSE::Update
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			21/12/2011 17:05:21
//	
//	@return				bool : 
//	@param				void : 
*/
/*-----------------------------------------------------------------*/
bool INPANDROIDDEVICEMOUSE::Update()
{	
	if(!IsEnabled()) return false;	

	for (XDWORD e=0;e<this->buttons.GetSize();e++)
	{
		if (this->buttons.Get(e))
		if (this->buttons.Get(e)->state==INPBUTTON_STATE_RELEASED)
			this->buttons.Get(e)->state=INPBUTTON_STATE_UP;
	}
	return true;
}




/*-------------------------------------------------------------------
//  INPANDROIDDEVICEMOUSE::AddPosition
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/04/2012 1:28:39
//	
//	@return 			bool : 
//  @param				index :
//	@param				x : 
//  @param				y : 
//  @param				ispressed : 
*/
/*-----------------------------------------------------------------*/
bool INPANDROIDDEVICEMOUSE::AddPosition(int index, float x, float y, bool ispressed)
{
	if(!grpscreen) return false;

	INPCURSOR* cursor = GetCursor(index);//(INPCURSOR_ID(INPCURSOR_ID_TOUCHSCREEN1+index));
	if(!cursor) return false;

	cursor->SetIsChanged(false);
	
	if((cursor->GetX() != x) || (cursor->GetY() != y))
		{
			GRPANDROIDSCREEN* _grpscreen = (GRPANDROIDSCREEN*)grpscreen;
			
			// to place the origin coordinates top left instead of bottom left
			y = (_grpscreen->GetHeight()) - y;

			cursor->Set(x,y);						
			cursor->SetIsChanged(true);
		}
	
	cursor->AddPointToMotion(ispressed);

	return true;
}



/*-------------------------------------------------------------------
//  INPANDROIDDEVICEMOUSE::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			21/12/2011 18:27:18
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
void INPANDROIDDEVICEMOUSE::Clean()
{
	grpscreen = NULL;
}



/*-------------------------------------------------------------------
//  INPANDROIDDEVICEMOUSE::CreateAllCursors
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			22/12/2011 10:55:24
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool INPANDROIDDEVICEMOUSE::CreateAllCursors()
{
	INPCURSOR* cursor;

	for(int c=0;c<5;c++)
		{
			cursor = new INPCURSOR(); 
			if(!cursor) return false;

			cursor->SetID(INPCURSOR_ID(INPCURSOR_ID_MOUSE+c));
			cursor->SetHavePreSelect(false);		
			cursors.Add(cursor);
		}
								
	return true;																									
}																								


/*-------------------------------------------------------------------
//	INPANDROIDDEVICEMOUSE::CreateAllButtons
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/06/2017 11:14:24
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool INPANDROIDDEVICEMOUSE::CreateAllButtons()
{
	INPBUTTON::CreateButton( &buttons, 0	, INPBUTTON_ID_MOUSE_LEFT		, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, 1	, INPBUTTON_ID_MOUSE_RIGHT	, __C('\x0'));	
	INPBUTTON::CreateButton( &buttons, 2	, INPBUTTON_ID_MOUSE_MIDDLE	, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, 3	, INPBUTTON_ID_MOUSE_MIDDLE	, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, 4	, INPBUTTON_ID_MOUSE_MIDDLE	, __C('\x0'));

	return true;
}

//-------------------------------------------------------------------
//  INPANDROIDDEVICEMOUSE::DeleteAllButtons
/**
//	
//	@author        Abraham J. Velez
//	@version       18/2/2003   15:34:3
//	
//	@return        bool : 
*/
//-------------------------------------------------------------------
bool INPANDROIDDEVICEMOUSE::DeleteAllCursors()
{
	if(cursors.IsEmpty())  return false;

	cursors.DeleteContents();
	
	cursors.DeleteAll();
	
	return true;
}


