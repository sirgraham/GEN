/*------------------------------------------------------------------------------------------
// UIMOTION.CPP
*/
/**
// \class
//
// implements UI Motions
//
//	@author: Imanol Celaya Ruiz de Alegria 
//
//	Date of Creation : 17/10/2017 10:52:26 
//	Last Modification : 
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIMotion.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	UIMOTION::UIMOTION
*/
/**
//
//	constructor
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version	17/10/2017 10:53:59
//	@return		 : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
UIMOTION::UIMOTION()
{
	Clean();
}



/*-------------------------------------------------------------------
//	UIMOTION::~UIMOTION
*/
/**
//
//	destructor
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version	17/10/2017 10:54:04
//	@return		 : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
UIMOTION::~UIMOTION()
{
	XDWORD size = entries.GetSize();
	for(XDWORD i = 0; i < size; i++)
		{
			delete entries.FastGet(i);
		}

	Clean();
}



/*-------------------------------------------------------------------
//	 UIMOTION::AddMotion
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version	17/10/2017 10:54:10
//	@return		bool : 
//
//	@param		UIMOTIONENTRY* : 
//
*//*-----------------------------------------------------------------*/
bool UIMOTION::AddMotion(UIMOTIONENTRY* entry)
{
	if(entry == NULL)
		{
			return false;
		}

	return entries.Add(entry);
}




/*-------------------------------------------------------------------
//	 UIMOTION::GetLastMotion
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version	17/10/2017 10:54:16
//	@return		UIMOTION::UIMOTIONENTRY* : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
UIMOTION::UIMOTIONENTRY* UIMOTION::GetLastMotion()
{
	if(entries.GetSize() == 0)
		{
			return NULL;
		}

	return entries.GetLast();
}



/*-------------------------------------------------------------------
//	 UIMOTION::GetMotionNumber
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version	17/10/2017 10:54:22
//	@return		XDWORD : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
XDWORD UIMOTION::GetMotionNumber()
{
	return entries.GetSize();
}



/*-------------------------------------------------------------------
//	 UIMOTION::GetMotion
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version	17/10/2017 10:54:28
//	@return		UIMOTION::UIMOTIONENTRY* : 
//
//	@param		XDWORD : 
//
*//*-----------------------------------------------------------------*/
UIMOTION::UIMOTIONENTRY* UIMOTION::GetMotion(XDWORD index)
{
	return entries.Get(index);
}



/*-------------------------------------------------------------------
//	 UIMOTION::Clear
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version	17/10/2017 13:24:10
//	@return		void : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
void UIMOTION::Clear()
{
	XDWORD size = entries.GetSize();
	for(XDWORD i = 0; i < size; i++)
		{
			delete entries.FastGet(i);
		}

	entries.DeleteAll();
}



/*-------------------------------------------------------------------
//	 UIMOTION::GetCurrentX
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version		

//	@return		float : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
float UIMOTION::GetCurrentX()
{
	return x;
}




/*-------------------------------------------------------------------
//	 UIMOTION::GetCurrentY
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version		

//	@return		float : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
float UIMOTION::GetCurrentY()
{
	return y;
}



/*-------------------------------------------------------------------
//	 UIMOTION::GetCurrentTimeStamp
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
XQWORD UIMOTION::GetCurrentTimeStamp()
{
	return timestamp;
}



/*-------------------------------------------------------------------
//	 UIMOTION::GetMotionDiffX
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version		

//	@return		float : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
float UIMOTION::GetMotionDiffX()
{
	if(entries.GetSize())
		{
			return x - entries.GetLast()->x;
		}

	return x;
}



/*-------------------------------------------------------------------
//	 UIMOTION::GetMotionDiffY
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version		

//	@return		float : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
float UIMOTION::GetMotionDiffY()
{
	if(entries.GetSize())
		{
			return y - entries.GetLast()->y;
		}

	return y;
}



/*-------------------------------------------------------------------
//	 UIMOTION::GetTimeStampDiffX
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
XQWORD UIMOTION::GetTimeStampDiffX()
{
	if(entries.GetSize())
		{
			return timestamp - entries.GetLast()->timestamp;
		}

	return timestamp;
}




/*-------------------------------------------------------------------
//	 UIMOTION::SetCurrentX
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version		

//	@return		void : 
//
//	@param		float : 
//
*//*-----------------------------------------------------------------*/
void UIMOTION::SetCurrentX(float x)
{
	this->x = x;
}




/*-------------------------------------------------------------------
//	 UIMOTION::SetCurrentY
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version		

//	@return		void : 
//
//	@param		float : 
//
*//*-----------------------------------------------------------------*/
void UIMOTION::SetCurrentY(float y)
{
	this->y = y;
}




/*-------------------------------------------------------------------
//	 UIMOTION::SetCurrentTimeStamp
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version		

//	@return		void : 
//
//	@param		XQWORD : 
//
*//*-----------------------------------------------------------------*/
void UIMOTION::SetCurrentTimeStamp(XQWORD timestamp)
{
	this->timestamp = timestamp;
}
