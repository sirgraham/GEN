//------------------------------------------------------------------------------------------
//	IFABUTTON.CPP
//	
//	 Button class for the interface
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 07/04/2004 11:05:07
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XPath.h"
#include "XFileBMP.h"

#include "GRPCanvas.h"	
#include "GRPBitmap.h"	
#include "GRPBitmapSequence.h"	

#include "INPButton.h"	
#include "INPCursor.h"	

#include "IFAXEvent.h"	
#include "IFABitmap.h"	
#include "IFAText.h"	

#include "IFAButton.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  IFABUTTON::IFABUTTON
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/11/2004 21:19:11
//	
//	@return				
//	@param				canvas : 
//  @param				screen : 
//  @param				inpmanager : 
 
//  @param				skinmanager : 
//  @param				publisher : 
//  @param				type : 
*/
//-------------------------------------------------------------------
IFABUTTON::IFABUTTON(GRPCANVAS* canvas, GRPSCREEN* screen, INPMANAGER* inpmanager, XFACTORY* xfactory, IFASKINMANAGER* skinmanager, IFAOBJTYPE type) : IFAFORM(canvas,screen,inpmanager,xfactory,skinmanager,type)
{
	Clean();

	overspace	= 4;	

	SetSubjectPublisher(publisher);	

	RegisterEventForPolling(IFAXEVENTTYPE_SELECT);
}


//-------------------------------------------------------------------
//  IFABUTTON::~IFABUTTON
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/04/2004 11:07:01
//	
//	@return				
//	*/
//-------------------------------------------------------------------
IFABUTTON::~IFABUTTON()
{
	DeRegisterEventForPolling(IFAXEVENTTYPE_SELECT);

	End();

	Clean();
}


//-------------------------------------------------------------------
//  IFABUTTON::Ini
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/04/2004 11:08:34
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFABUTTON::Ini()
{
	text = new IFATEXT(canvas,screen,inpmanager,xfactory,skinmanager);
	if(!text) return false;
	AddObj(text);

	bitmap = new IFABITMAP(canvas,screen,inpmanager,xfactory,skinmanager);
	if(!bitmap) return false;	

	AddObj(bitmap);	

	bitmapsequence = new GRPBITMAPSEQUENCE();
	if(!bitmapsequence) return false;
	
	InitFSMachine();

	SetEvent(IFABUTTON_XFSMEVENT_ENABLED);

	return true;
}



//-------------------------------------------------------------------
//  IFABUTTON::InitFSMachine
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/04/2004 15:29:54
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFABUTTON::InitFSMachine()
{
	if(!AddState(	IFABUTTON_XFSMSTATE_NONE, 								
				 				IFABUTTON_XFSMEVENT_ENABLED		, IFABUTTON_XFSMSTATE_ENABLED		, 								
								IFABUTTON_XFSMEVENT_DISABLED	, IFABUTTON_XFSMSTATE_DISABLED	, 								
								IFABUTTON_XFSMEVENT_PRESELECT	, IFABUTTON_XFSMSTATE_PRESELECT	, 								
								IFABUTTON_XFSMEVENT_SELECT		, IFABUTTON_XFSMSTATE_SELECT		, 								
								EVENTDEFEND)) return false;

  if(!AddState(	IFABUTTON_XFSMSTATE_PRESELECT, 								
								IFABUTTON_XFSMEVENT_ENABLED		, IFABUTTON_XFSMSTATE_ENABLED		, 								
				 				IFABUTTON_XFSMEVENT_DISABLED	, IFABUTTON_XFSMSTATE_DISABLED	, 																
								IFABUTTON_XFSMEVENT_SELECT		, IFABUTTON_XFSMSTATE_SELECT		, 								
								EVENTDEFEND)) return false;

	if(!AddState(	IFABUTTON_XFSMSTATE_ENABLED, 								
				 				IFABUTTON_XFSMEVENT_DISABLED	, IFABUTTON_XFSMSTATE_DISABLED	, 								
								IFABUTTON_XFSMEVENT_PRESELECT	, IFABUTTON_XFSMSTATE_PRESELECT	, 								
								IFABUTTON_XFSMEVENT_SELECT		, IFABUTTON_XFSMSTATE_SELECT		, 								
								EVENTDEFEND)) return false;

	if(!AddState(	IFABUTTON_XFSMSTATE_SELECT, 								
				 				IFABUTTON_XFSMEVENT_ENABLED		, IFABUTTON_XFSMSTATE_ENABLED		, 																
								IFABUTTON_XFSMEVENT_DISABLED	, IFABUTTON_XFSMSTATE_DISABLED	, 																
								IFABUTTON_XFSMEVENT_PRESELECT	, IFABUTTON_XFSMSTATE_PRESELECT	, 								
								EVENTDEFEND)) return false;
	
	if(!AddState(	IFABUTTON_XFSMSTATE_DISABLED,
								IFABUTTON_XFSMEVENT_ENABLED		, IFABUTTON_XFSMSTATE_ENABLED		, 																										
								EVENTDEFEND)) return false;

	return true;
}



//-------------------------------------------------------------------
//  IFABUTTON::Draw
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/04/2004 11:08:40
//	
//	@return				bool : 
//	@param				x : 
//  @param				y : 
*/
//-------------------------------------------------------------------
bool IFABUTTON::Draw(int x, int y)
{
	if(!canvas) return false;

	rectdirect.Set(x,y,rect.GetWidth(),rect.GetHeight());

	if(features&IFAFEATURE_BACKGROUND)
		{
			GRPCOLOR* fillcolor = GetColor(GRPCOLORTYPE_BACKGROUND);
			GRPCOLOR* edgecolor = GetColor(GRPCOLORTYPE_EDGE);

			canvas->DrawBox(x,y,rect.GetWidth(),rect.GetHeight() , fillcolor->Get()
																													 , fillcolor->GetAlpha()
																													 , 1
																													 , edgecolor->Get()
																													 , edgecolor->GetAlpha()
																													 , shadow);	
		}

	return true;
}



//-------------------------------------------------------------------
//  IFABUTTON::UpdateInputState
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			14/01/2004 15:44:11
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFABUTTON::UpdateInputState()
{	
	if(!IsEnabled()) return false;
	
	if(button)
		{
			if(button->IsPressedWithRelease()) 
				{
					SetEvent(IFABUTTON_XFSMEVENT_SELECT);
					return true;
				}
		}
	
	if(cursor)
		{	
			if(cursor->HavePreSelect())
				{
					if(cursor->IsPositionInRect(GetRect()->GetX(),GetRect()->GetY(),GetRect()->GetWidth(),GetRect()->GetHeight()))
						{
							SetEvent(IFABUTTON_XFSMEVENT_PRESELECT);
						} 
					 else
					  {
							SetEvent(IFABUTTON_XFSMEVENT_ENABLED);
						}
				}

			if(cursor->GetMotion())
				{
					if(cursor->GetMotion()->IsReadyToTest())
						{
							if(cursor->GetMotion()->IsInRect(GetRect()->GetX(),GetRect()->GetY(),GetRect()->GetWidth(),GetRect()->GetHeight()))
								{
									cursor->GetMotion()->Reset();

									SetEvent(IFABUTTON_XFSMEVENT_SELECT);
									return true;
								}
						}
				}		
		}
  
	return true;
}



//-------------------------------------------------------------------
//  IFABUTTON::Update
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/04/2004 11:08:53
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFABUTTON::Update()
{
	if(active)	UpdateInputState();	

  if(GetEvent()==IFABUTTON_XFSMEVENT_NONE) // No hay nuevos Eventos
		{			
		  switch(GetCurrentState())
				{       
					case IFABUTTON_XFSMSTATE_ENABLED		: break;
					
					case IFABUTTON_XFSMSTATE_PRESELECT	: break;

					case IFABUTTON_XFSMSTATE_SELECT			: SetEvent(IFABUTTON_XFSMEVENT_PRESELECT);
																								break;

					case IFABUTTON_XFSMSTATE_DISABLED		: break; 
				}							
		}
	 else //  nuevo evento
	  {
			if(GetEvent()<IFABUTTON_LASTEVENT)
				{
					CheckTransition();

					switch(GetCurrentState())
						{         						
							case IFABUTTON_XFSMSTATE_ENABLED		: SetBitmapStatus(IFABUTTON_XFSMSTATE_ENABLED);
																										break;

							case IFABUTTON_XFSMSTATE_PRESELECT	: SetBitmapStatus(IFABUTTON_XFSMSTATE_PRESELECT);																									
																										break;

							case IFABUTTON_XFSMSTATE_SELECT			: { SetBitmapStatus(IFABUTTON_XFSMSTATE_SELECT);

																											IFAXEVENTSELECT xevent(0,this,this,this);							
																											PostEvent(&xevent, true);																									
																										}
																										break;

							case IFABUTTON_XFSMSTATE_DISABLED		:	SetBitmapStatus(IFABUTTON_XFSMSTATE_DISABLED);
																										break; 
						}		
				}
		}
	
	if(visible)	IFAFORM::Update();

	return true;
}


//-------------------------------------------------------------------
//  IFABUTTON::End
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/04/2004 11:09:11
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFABUTTON::End()
{
	DeleteAllObj();

	delete text;
	text = NULL;

	delete bitmap;	
	bitmap = NULL;

	if(bitmapsequence) 
		{
			delete bitmapsequence;
			bitmapsequence = NULL;
		}

	button = NULL;
	cursor = NULL;

	return true;
}


//-------------------------------------------------------------------
//  IFABUTTON::IsEnabled
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/04/2004 16:14:09
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFABUTTON::IsEnabled()
{
	if(GetCurrentState()==IFABUTTON_XFSMSTATE_DISABLED) return false;

	return true;
}


//-------------------------------------------------------------------
//  IFABUTTON::SetEnabled
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/04/2004 16:14:26
//	
//	@return				bool : 
//	@param				on : 
*/
//-------------------------------------------------------------------
bool IFABUTTON::SetEnabled(bool on)
{	
	if(on)
		{
			SetEvent(IFABUTTON_XFSMEVENT_ENABLED);
		}
	 else
		{
			SetEvent(IFABUTTON_XFSMEVENT_DISABLED);		
		}
	
	return true;
}



//-------------------------------------------------------------------
//  IFABUTTON::IsPreSelect
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/04/2004 17:06:16
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFABUTTON::IsPreSelect()
{
	if(GetCurrentState()==IFABUTTON_XFSMSTATE_PRESELECT) return true;	

	return false;
}



/*-------------------------------------------------------------------
//  IFABUTTON::GetBitmapSequence
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/12/2011 17:53:46
//	
//	@return				GRPBITMAPSEQUENCE* : 
//	*/
/*-----------------------------------------------------------------*/
GRPBITMAPSEQUENCE* IFABUTTON::GetBitmapSequence()
{
	return bitmapsequence;
}



/*-------------------------------------------------------------------
//  IFABUTTON::LoadBitmapSequence
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/12/2011 18:11:18
//	
//	@return				bool : 
//	@param				xpath : 
//  @param				namefile : 
*/
/*-----------------------------------------------------------------*/
bool IFABUTTON::LoadBitmapSequence(XPATH& xpath,XCHAR* namefile)
{
	XPATH		 xpathfile;
	XFILEBMP filebmp();

	xpathfile = xpath.Get();
	xpathfile.Slash_Add();
	xpathfile += namefile;
	
	if(filebmp.Open(xpathfile))
		{				
			for(int c=0;c<(IFABUTTON_LASTSTATE-1);c++)
				{	
					GRPBITMAP* bitmap = filebmp.CreateBitmapFrame((IFABUTTON_LASTSTATE-1),c);
					if(bitmap) 
						{							
							bitmap->SetMode(GRPPROPERTYMODE_24R8G8B8);
							bitmap->ConvertToMode(canvas->GetMode());

							bitmapsequence->AddFrame(bitmap);						
						}
				}

			filebmp.Close();
			
			return true;
		}
	
	return false;	
}



//-------------------------------------------------------------------
//  IFABUTTON::GetBitmap
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/05/2004 12:06:22
//	
//	@return				IFABITMAP* : 
//	*/
//-------------------------------------------------------------------
IFABITMAP* IFABUTTON::GetBitmap()
{
	return bitmap;
}




//-------------------------------------------------------------------
//  IFABUTTON::SetBitmapStatus
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/04/2004 12:45:33
//	
//	@return				bool : 
//	@param				status : 
*/
//-------------------------------------------------------------------
bool IFABUTTON::SetBitmapStatus(IFABUTTON_XFSMSTATES status)
{	
	if(!bitmapsequence) return false;
	
	GRPBITMAPFRAME* frame = bitmapsequence->GetFrame(status-1);
	if(!frame) return false;

	bitmap->SetBitmap(frame->GetBitmap());
	
	return true;
}




/*-------------------------------------------------------------------
//  IFABUTTON::GetINPButton
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/12/2011 18:36:10
//	
//	@return				INPBUTTON* : 
//	*/
/*-----------------------------------------------------------------*/
INPBUTTON* IFABUTTON::GetINPButton()
{
	return button;
}



/*-------------------------------------------------------------------
//  IFABUTTON::SetINPButton
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			21/12/2011 10:23:29
//	
//	@return				bool : 
//	@param				button : 
*/
/*-----------------------------------------------------------------*/
bool IFABUTTON::SetINPButton(INPBUTTON* button)
{
	this->button = button;

	return true;
}



/*-------------------------------------------------------------------
//  IFABUTTON::GetINPCursor
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/12/2011 20:50:53
//	
//	@return				INPCURSOR* : 
//	*/
/*-----------------------------------------------------------------*/
INPCURSOR* IFABUTTON::GetINPCursor()
{
	return cursor;
}



/*-------------------------------------------------------------------
//  IFABUTTON::SetINPCursor
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/12/2011 20:51:12
//	
//	@return				bool : 
//	@param				cursor : 
*/
/*-----------------------------------------------------------------*/
bool IFABUTTON::SetINPCursor(INPCURSOR* cursor)
{
	this->cursor = cursor;

	return true;
}



//-------------------------------------------------------------------
//  IFABUTTON::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/04/2004 11:08:23
//	
//	@return				void : 
//	*/
//-------------------------------------------------------------------
void IFABUTTON::Clean()
{	
	publisher			  = NULL;
	
	text						= NULL;
	bitmap					= NULL;
	bitmapsequence	= NULL;

	button					= NULL;	
	cursor					= NULL;
}
