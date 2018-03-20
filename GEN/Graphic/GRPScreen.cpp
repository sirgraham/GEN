
/*------------------------------------------------------------------------------------------
//	GRPSCREEN.CPP
//	
//	screen info
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 29/10/2015 17:22:04
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPScreen.h"
#include "GRPViewport.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

GRPSCREEN::GRPSCREEN()
{
		Clean();

		RegisterEvent(GRPXEVENTTYPE_SCREEN_CHANGEPOSITION);
		RegisterEvent(GRPXEVENTTYPE_SCREEN_CHANGESIZE);
		RegisterEvent(GRPXEVENTTYPE_SCREEN_CHANGEFOCUS);
}

GRPSCREEN::~GRPSCREEN()
{

		DeRegisterEvent(GRPXEVENTTYPE_SCREEN_CHANGEFOCUS);
		DeRegisterEvent(GRPXEVENTTYPE_SCREEN_CHANGESIZE);
		DeRegisterEvent(GRPXEVENTTYPE_SCREEN_CHANGEPOSITION);

		Clean();
}

GRPSCREENTYPE GRPSCREEN::GetType() 
{ 
		return type; 
}


bool GRPSCREEN::IsValid() 
{
		return isvalid; 
}

bool GRPSCREEN::IsActive() 
{ 
		return isactive; 
}

bool GRPSCREEN::SetPropertys(int width, int height, int stride, GRPPROPERTYMODE mode)
{
		SetWidth(width);
		SetHeight(height);
		SetStride(stride);
		SetMode(mode);
		return true;
}

bool GRPSCREEN::IsFullScreen() 
{ 
		return isfullscreen; 
}

void GRPSCREEN::SetIsFullScreen(bool isfullscreen) 
{ 
		this->isfullscreen = isfullscreen; 
}

bool GRPSCREEN::Resize(int width, int height)
{
		return true;
}

bool GRPSCREEN::UpdateSize(int width, int height)
{
		this->width = width;
		this->height = height;

		UpdateViewports(width, height);

		GRPXEVENT grpevent(this, GRPXEVENTTYPE_SCREEN_CHANGESIZE);
		grpevent.SetSize(width, height);

		PostEvent(&grpevent);

		return true;
}

void* GRPSCREEN::GetHandle() 
{ 
		return handle; 
}

GRPVIEWPORT* GRPSCREEN::GetViewport(int index) 
{ 
		return (viewports.Get(index)); 
}

XVECTOR<GRPVIEWPORT*>* GRPSCREEN::GetViewports() 
{ 
		return &viewports; 
}

void GRPSCREEN::SetDPI(float dpi) 
{ 
		this->dpi = dpi; 
}

float GRPSCREEN::GetDPI() 
{ 
		return this->dpi; 
}


/*-------------------------------------------------------------------
//	 GRPSCREEN::AddViewport
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		GRPVIEWPORT* : 
//
*//*-----------------------------------------------------------------*/
bool GRPSCREEN::AddViewport(GRPVIEWPORT* viewport)
{
	viewport->SetWindowSize((GLFLOAT)this->GetWidth(),(GLFLOAT)this->GetHeight());																		
	return this->viewports.Add(viewport);
}

/*-------------------------------------------------------------------
//	 GRPSCREEN::UseViewport
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		int : 
//	@param		GRPVIEWPORT& : 
//
*//*-----------------------------------------------------------------*/
bool GRPSCREEN::UseViewport(int n, GRPVIEWPORT& viewport)
{
	if(!viewports.Get(n))  return false;

	viewports.Get(n)->Select(); 
	viewport = (*viewports.Get(n));

	return true;
}


/*-------------------------------------------------------------------
//	 GRPSCREEN::UpdateViewports
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		int : 
//	@param		int : 
//
*//*-----------------------------------------------------------------*/
bool GRPSCREEN::UpdateViewports(int width, int height)
{   
	if(viewports.IsEmpty()) return false;
	
	for(XDWORD c=0; c<viewports.GetSize(); c++)
		{
			viewports.Get(c)->SetWindowSize((GLFLOAT)width, (GLFLOAT)height);		
			viewports.Get(c)->Select(); 		
		}

	return true;
}

