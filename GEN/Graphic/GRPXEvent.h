/*------------------------------------------------------------------------------------------
//	GRPXEVENT.H
*/	
/**	
// \class 
//   
//  Graphic events types
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 11/07/2014 17:38:39
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPXEVENT_H_
#define _GRPXEVENT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XEvent.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
// insert all events in here?
enum GRPXEVENTTYPE
{
	GRPXEVENTTYPE_UNKNOWN		   							= XEVENTTYPE_GRP						,

	GRPXEVENTTYPE_SCREEN																								,
	GRPXEVENTTYPE_SCREEN_UNKNOWN																				,
	GRPXEVENTTYPE_SCREEN_CHANGEPOSITION			             								,
	GRPXEVENTTYPE_SCREEN_CHANGESIZE					               							,
  GRPXEVENTTYPE_SCREEN_CHANGEFOCUS																		,

	GRPXEVENTTYPE_SCENE																									,
	GRPXEVENTTYPE_SCENE_UNKNOWN																					,
	GRPXEVENTTYPE_SCENE_INI									               							,
	GRPXEVENTTYPE_SCENE_END									               							,
	GRPXEVENTTYPE_SCENE_FRAMEINI																				,
	GRPXEVENTTYPE_SCENE_FRAMEEND																				,
	GRPXEVENTTYPE_SCENE_ENTITYINI																				,
	GRPXEVENTTYPE_SCENE_ENTITYEND																				,
	GRPXEVENTTYPE_SCENE_VIEWPORTCHANGED																	,
	GRPXEVENTTYPE_SCENE_INPUT																						,

	GRPXEVENTTYPE_BUFFEROPENGLRENDER																		,
	GRPXEVENTTYPE_VIEWPORT
};
	

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPSCENE;
class GRPSCREEN;
class GRPOBJECT;
class GRPVIEWPORT;

class GRPXEVENT : public XEVENT
{
public:
																		GRPXEVENT											(XSUBJECT* subject, XDWORD type, XDWORD family) : XEVENT(subject, type, family)
																		{
																			Clean();
																		}

																		GRPXEVENT											(XSUBJECT* subject, XDWORD type) : XEVENT(subject, type)
																		{
																			Clean();
																		}

																	 ~GRPXEVENT											()
																		{
																			Clean();
																		}

	int																GetID													()													{ return ID;												}
	void															SetID													(int ID)										{ this->ID = ID;										}
	
	GRPSCENE*													GetScene											()													{ return scene;											}
	void															SetScene											(GRPSCENE* scene)						{ this->scene = scene;							}

	GRPSCREEN*												GetScreen											()													{ return screen;										}
	void															SetScreen											(GRPSCREEN* screen)					{ this->screen = screen;						}

	GRPOBJECT*												GetElement										()													{ return element;										}
	void															SetElement										(GRPOBJECT* element)				{ this->element = element;					}

	GRPVIEWPORT*											GetViewport										()													{ return viewport;									}
	void															SetViewport										(GRPVIEWPORT* viewport)			{ this->viewport = viewport;			  }
		
	bool															GetReady											()													{ return ready;											}
	void															SetReady											(bool r)										{ ready =	r;												}

	int																GetPositionX									()													{ return x;													}								
	int																GetPositionY									()													{ return y;                         }		

	void															SetPosition										(int x, int y)			
																		{ 
																			this->x  = x; 
																			this->y  = y;
																		}


	int																GetWidth											()													{ return width;											}
	int																GetHeight											()													{ return height;                    }

	void															SetSize					              (int width, int height)	
																		{
																			this->width		= width;
																			this->height	= height;
																		}
		

	bool															IsFocused											()													{ return isfocused;									}
	void 															SetIsFocused									(bool isfocused)						{ this->isfocused = isfocused;			}		


protected:
	// scene
	int																ID;
	GRPSCENE*													scene;
	GRPSCREEN*												screen;
	GRPOBJECT*												element;
	GRPVIEWPORT*											viewport;			
	bool															ready;

	// screen
	int																x;
	int																y;
	int																width;
	int																height;
	bool															isfocused;	

private:
	void															Clean													()
																		{
																			ID				=	0;
																			scene			= NULL;
																			screen		= NULL;
																			element		= NULL;
																			viewport	= NULL;
																			ready			= true;
																			x					= 0;
																			y					= 0;
																			width			= 0;
																			height		= 0;
																		}
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif


