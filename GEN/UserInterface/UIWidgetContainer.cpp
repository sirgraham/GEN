
/*------------------------------------------------------------------------------------------
//	UIWIDGETCONTAINER.CPP
//	
//	container class for the rest of widgets
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 08/10/2015 11:09:10
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPScene.h"
#include "GRPScreen.h"
#include "GRPViewport.h"
#include "GRP2DCollisionDetection.h"

#include "UIWidgetLayout.h"
#include "UIWidgetContainer.h"
#include "UIManager.h"
#include "UIMotion.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::UIWIDGETCONTAINER
*/
/**
//
//	constructor
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version		

//	@return		 : 
//
//	@param		XCHAR* : 
//	@param		 UIMANAGER* uimanagerarg : 
//
*//*-----------------------------------------------------------------*/
UIWIDGETCONTAINER::UIWIDGETCONTAINER(XCHAR* name, UIMANAGER * uimanagerarg): UIWIDGET(name, uimanagerarg)
{
	Clean();
	widgettype = UI_WIDGET_TYPE_CONTAINER;
	SetIsComposable(true);
	SetIsClickable(true);
	childrenbyindex.SetIsMulti(false);

	motion = new UIMOTION();
}

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::UIWIDGETCONTAINER
*/	
/**	
//	
//	Class Constructor UIWIDGETCONTAINER
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/02/2017 16:42:11
//	
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETCONTAINER::UIWIDGETCONTAINER(UIWIDGETCONTAINER* rhs) : UIWIDGET(rhs)
{
	Clean();
	

	gpt0x											= rhs->gpt0x;
	gpt0y											= rhs->gpt0y;
	gpt1x											= rhs->gpt1x;
	gpt1y											= rhs->gpt1y;

	// adjusted to arrangechildren specifications
	adjustedx									= rhs->adjustedx;
	adjustedy									= rhs->adjustedy;
	adjustedwidth							= rhs->adjustedwidth;
	adjustedheight						= rhs->adjustedheight;
	isadjusted								= rhs->isadjusted;

	//scene											= rhs->scene; // need to think of a way to remove this from here, as we shouldn't have any GRP classes on the base classes

	// clone every rhs child and add
	childrenbyindex.SetIsMulti(false);
	XDWORD size = rhs->childrenbyindex.GetSize();
	for(XDWORD i = 0; i < size; i++)
		{
			UIWIDGET* w = rhs->childrenbyindex.FastGet(i);
			UIWIDGET* c = w->Clone();
			c->SetParent(this);
			childrenbyindex.Add(c);
		}

	x													= rhs->x;
	y													= rhs->y;
	width											= rhs->width;
	height										= rhs->height;
	rotationx									= rhs->rotationx;
	rotationy									= rhs->rotationy;
	rotationz									= rhs->rotationz;

	// aligment and size conditions
	horizontalaligment				= rhs->horizontalaligment;
	verticalaligment					= rhs->verticalaligment;

	isclickable								= rhs->isclickable;
	ishoverable								= rhs->ishoverable;
	isdragable								= rhs->isdragable;

	// These coordinates are local to the widget
	iscursorover							= rhs->iscursorover;
	istriggered								= rhs->istriggered;
	ispressed									= rhs->ispressed;
	ispressedlong							= rhs->ispressedlong;
	isreleased								= rhs->isreleased;
	lastcursorposx						= rhs->lastcursorposx;
	lastcursorposy						= rhs->lastcursorposy;
	currentcursorposx					= rhs->currentcursorposx;
	currentcursorposy					= rhs->currentcursorposy;
	triggerposx								= rhs->triggerposx;
	triggerposy								= rhs->triggerposy;
	triggerlocalposx					= rhs->triggerlocalposx;
	triggerlocalposy					= rhs->triggerlocalposy;

	isownvisible							= rhs->isownvisible;

	horizontalconstraint			= rhs->horizontalconstraint;
	verticalconstraints				= rhs->verticalconstraints;
	horizontalanchor					= rhs->horizontalanchor;
	verticalanchor						= rhs->verticalanchor;

	layoutweigth							= rhs->layoutweigth;

	// clone layouts
	//attachedlayouts;
	size = rhs->attachedlayouts.GetSize();
	for(XDWORD i = 0; i < size; i++)
		{
			UIWIDGETLAYOUT* w = rhs->attachedlayouts.FastGet(i);
			//UIWIDGETLAYOUT* c = (UIWIDGETLAYOUT*)w->Clone();
			//c->SetParent(this); // maybe ?
			//attachedlayouts.Add(c);
			attachedlayouts.Add(w); // because we are not cloning the layouts, but their attachments(should really remove the parent jumping attach mechanism)
		}

	motion = new UIMOTION();
}



/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::~UIWIDGETCONTAINER
*/
/**
//
//	destructor
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version		

//	@return		 : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
UIWIDGETCONTAINER::~UIWIDGETCONTAINER()
{
	DeleteChildrenAndLink();

	delete motion;

	Clean();
}

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::AddChild
*/	
/**	
//	
//	Adds a child widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/10/2015 17:11:26
//	
//	@return 			bool : true if it was added correctly
//
//  @param				child : pointer to the widget to add
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCONTAINER::JoinChild(UIWIDGET* child)
{
	if(child)
		{
			return childrenbyindex.Add(child);
		}

	return false;
}


/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::GetChild
*/	
/**	
//	
//	Gets a child widget by index
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/10/2015 17:11:49
//	
//	@return 			UIWIDGET* : The child widget, NULL if failed
//
//  @param				childindex : 0 based index of the children(order based on insertion)
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETCONTAINER::GetChild(XWORD childindex)
{
	return childrenbyindex.Get(childindex);
}

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::GetChild
*/	
/**	
//	
//	Gets a child widget by name, in case of multiple ones, the first one
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/10/2015 17:12:17
//	
//	@return 			UIWIDGET* : The child widget, NULL if failed
//
//  @param				childname : name of the child
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETCONTAINER::GetChildByName(XSTRING* childname)
{
	for(XDWORD i = 0; i < childrenbyindex.GetSize(); i++)
		{
			if(childrenbyindex.Get(i)->GetName()->Compare(*childname) == 0)
				{
					return childrenbyindex.Get(i);
				}
		}


	return NULL;
}

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::GetChildren
*/	
/**	
//	
//	Gets all the children with the same name
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/10/2015 17:12:34
//	
//	@return 			XVECTOR<UIWIDGET*>* : pointer to a vector with all the children with the same name, must be deallocated by the caller
//
//  @param				childrenname : name of the children to get
*/
/*-----------------------------------------------------------------*/
XVECTOR<UIWIDGET*>* UIWIDGETCONTAINER::GetChildren(XSTRING* childrenname)
{
	XVECTOR<UIWIDGET*>* returnvector = new XVECTOR<UIWIDGET*>();

	for(XDWORD i = 0; i < childrenbyindex.GetSize(); i++)
		{
			if(childrenbyindex.Get(i)->GetName()->Compare(*childrenname) == 0)
				{
					returnvector->Add(childrenbyindex.Get(i));
				}
		}



	return returnvector;
}

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::GetAllChildren
*/	
/**	
//	
//	Gets all the children this widget has
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/10/2015 17:12:48
//	
//	@return 			XVECTOR<UIWIDGET*>* : Pointer to the internal vector with all the children
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<UIWIDGET*>* UIWIDGETCONTAINER::GetAllChildren()
{
	return &childrenbyindex;
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::FindChildByName
*/	
/**	
//	
//	find the first child by name
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/10/2015 14:03:10
//	
//	@return 			UIWIDGET* : 
//
//  @param				childname : 
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETCONTAINER::FindChildByName(XSTRING* childname)
{

	// if we get here, no child with that name was found  //DIEGO (here?)
	for(XDWORD i = 0; i < childrenbyindex.GetSize(); i++)
		{
			if(childrenbyindex.Get(i)->GetName()->Compare(*childname) == ISEQUAL)
				{
					return childrenbyindex.Get(i);
				}
		}


	return NULL;
}


/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::FindChildByName
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/06/2017 13:00:47
//	
//	@return 			UIWIDGET* : 
//
//  @param				childname : 
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETCONTAINER::FindChildByName(XCHAR* childname)
{
	XSTRING str(childname);
	return FindChildByName(&str);
}

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::FindChildren
*/	
/**	
//	
//	find a list of children by name
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/10/2015 14:02:53
//	
//	@return 			XVECTOR<UIWIDGET*>* : 
//
//  @param				childrenname : 
*/
/*-----------------------------------------------------------------*/
XVECTOR<UIWIDGET*>* UIWIDGETCONTAINER::FindChildren(XSTRING* childrenname)
{
	XVECTOR<UIWIDGET*>* returnvector = new XVECTOR<UIWIDGET*>();

	for(XDWORD i = 0; i < childrenbyindex.GetSize(); i++)
		{
			if(childrenbyindex.Get(i)->GetName()->Compare(*childrenname) == ISEQUAL)
				{
					returnvector->Add(childrenbyindex.Get(i));
				}
		}

	return returnvector;
}



/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::FindChildren
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/06/2017 13:02:04
//	
//	@return 			XVECTOR<UIWIDGET*>* : 
//
//  @param				childrenname : 
*/
/*-----------------------------------------------------------------*/
XVECTOR<UIWIDGET*>* UIWIDGETCONTAINER::FindChildren(XCHAR* childrenname)
{
	XSTRING str(childrenname);
	return FindChildren(&str);
}


/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::FindChildByNameRec
*/	
/**	
//	
//	find the first child by name recursively
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/10/2015 14:03:10
//	
//	@return 			UIWIDGET* : 
//
//  @param				childname : 
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETCONTAINER::FindChildByNameRec(XSTRING* childname)
{

	// direct child
	for(XDWORD i = 0; i < childrenbyindex.GetSize(); i++)
		{
			if(childrenbyindex.Get(i)->GetName()->Compare(*childname) == ISEQUAL)
				{
					return childrenbyindex.Get(i);
				}
		}

	// continue the search down the tree
	for(XDWORD i = 0; i < childrenbyindex.GetSize(); i++)
		{
			UIWIDGET * widget = childrenbyindex.Get(i)->FindChildByNameRec(childname);
			if(widget)
				{
					return widget;
				}
		}


	return NULL;
}



/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::FindChildByNameRec
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/06/2017 13:02:39
//	
//	@return 			UIWIDGET* : 
//
//  @param				childname : 
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETCONTAINER::FindChildByNameRec(XCHAR* childname)
{
	XSTRING str(childname);
	return FindChildByNameRec(&str);
}

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::FindChildrenRec
*/	
/**	
//	
//	find recursively a lsit of children by name
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/10/2015 14:02:53
//	
//	@return 			XVECTOR<UIWIDGET*>* : 
//
//  @param				childrenname : 
*/
/*-----------------------------------------------------------------*/
XVECTOR<UIWIDGET*>* UIWIDGETCONTAINER::FindChildrenRec(XSTRING* childrenname)
{
	XVECTOR<UIWIDGET*>* returnvector = new XVECTOR<UIWIDGET*>();

	// check direct children
	for(XDWORD i = 0; i < childrenbyindex.GetSize(); i++)
		{
			if(childrenbyindex.Get(i)->GetName()->Compare(*childrenname) == ISEQUAL)
				{
					returnvector->Add(childrenbyindex.Get(i));
				}
		}

	// continue search down the tree
	for(XDWORD i = 0; i < childrenbyindex.GetSize(); i++)
		{
			UIWIDGET * widget = childrenbyindex.Get(i)->GetChildByName(childrenname);
			if(widget)
				{
					returnvector->Add(widget);
				}
		}

	return returnvector;
}

	

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::FindChildrenRec
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			09/06/2017 13:03:35
//	
//	@return 			XVECTOR<UIWIDGET*>* : 
//
//  @param				childrenname : 
*/
/*-----------------------------------------------------------------*/
XVECTOR<UIWIDGET*>* UIWIDGETCONTAINER::FindChildrenRec(XCHAR* childrenname)
{
	XSTRING str(childrenname);
	return FindChildrenRec(&str);
}




/*-------------------------------------------------------------------
//	 UIWIDGETCONTAINER::IsChild
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version		

//	@return		bool : 
//
//	@param		UIWIDGET* : 
//
*//*-----------------------------------------------------------------*/
bool UIWIDGETCONTAINER::IsChild(UIWIDGET* uiwidget)
{
	XDWORD size = childrenbyindex.GetSize();
	for(XDWORD i = 0; i < size; i++)
		{
			UIWIDGET* w = childrenbyindex.FastGet(i);
			if(w == uiwidget)
				{
					return true;
				}

			if(w->IsChild(uiwidget))
				{
					return true;
				}
		}

	return false;
}



	/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::RemoveChild
*/	
/**	
//	
//	Removes a child by index, but doesn't delete the memory it
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/10/2015 17:13:03
//	
//	@return 			bool : true if succeeded, false otherwise
//
//  @param				childindex : 0 based index of the child to remove
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCONTAINER::RemoveChild(XWORD childindex)
{
	UIWIDGET * widget = childrenbyindex.Get(childindex);

	if(!widget)
		{
			return false;
		}

	return childrenbyindex.Delete(widget);
}

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::RemoveChild
*/	
/**	
//	
//	Removes a child by name, but doesn't delete the memory it
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/10/2015 17:13:14
//	
//	@return 			bool : true if succeeded, false otherwise
//
//  @param				childname : name of the child to remove(selects the first one if multiple)
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCONTAINER::RemoveChild(XSTRING* childname)
{
	for(XDWORD i = 0; i < childrenbyindex.GetSize(); i++)
		{
			if(childrenbyindex.Get(i)->GetName()->Compare(*childname) == ISEQUAL)
				{
					childrenbyindex.DeleteIndex(i);
					return true;
				}
		}

	return false;
}

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER
*/	
/**	
//	
//	Removes a child by pointer, but doesn't delete the memory it
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/10/2015 9:40:14
//	
//	@return 			bool : true if succeeded, false otherwise
//
//  @param				childpointer : pointer to the child to remove
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCONTAINER::RemoveChildPointer(UIWIDGET* childpointer)
{
	if(childrenbyindex.Delete(childpointer))
		{
			childpointer->SetParent(NULL);
			return true;
		}

	return false;
}


/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::RemoveAll
*/	
/**	
//	
//	Removes all children
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/10/2015 17:13:24
//	
//	@return 			bool : true if removed
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCONTAINER::RemoveAll()
{
	return childrenbyindex.DeleteAll();
}

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::Draw
*/	
/**	
//	
//	Recursively draws this widget and its children
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/10/2015 17:13:42
//	
//	@return 			bool : true if succeeded
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCONTAINER::Draw(XDWORD& widgetlevel)
{
	ComputeDesiredSize();

	if(!ishidden)
		{
			// Draw this widget here
			// try to draw with a special one, if fails draw with the default
			// compute scales
			
			if(!InternalDraw(widgetlevel))
				{
					//uimanager->GetUISkin()->Draw(widgetlevel, (UIWIDGETFORM*)this);
				}
		}

	widgetlevel += 1;
	// Draw children widget
	XDWORD childrennumber = childrenbyindex.GetSize();
	for(XDWORD i = 0; i < childrennumber; ++i)
		{
			childrenbyindex.FastGet(i)->modified = this->modified || childrenbyindex.FastGet(i)->modified;
			bool status = childrenbyindex.FastGet(i)->Draw(widgetlevel);
			childrenbyindex.FastGet(i)->modified = false;
			if(!status)
				{
					return false;
				}
		}

	InternalPostDraw(widgetlevel);

	return true;
}

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::Update
*/	
/**	
//	
//	Recursively updates this widget and its children
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/10/2015 17:13:56
//	
//	@return 			bool : true if succeeded
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCONTAINER::Update(XDWORD widgetlevel)
{
	isadjusted = false;

	if(!ishidden && isactive)
		{
			XDWORD childrennumber = childrenbyindex.GetSize();

			//ComputeCoords();

			// save old state, maybe run this outside the if?
			oldwidgetstate = widgetstate;

			//iscursorover = CheckCursorOver();

			iscursorover = uimanager->GetHoveredWidget() == this;

			UpdateWidgetState();

			// Update this widget here
			InternalUpdate(widgetlevel);
			

			// Update children widget
			if(childrennumber)
				{
					// update outermost children firstç
					if(childrennumber > 0)
						{
							for(XDWORD i = (childrennumber); i > 0; i--)
								{
									childrenbyindex.FastGet(i-1)->modified = this->modified || childrenbyindex.FastGet(i-1)->modified;
									if(!childrenbyindex.FastGet(i-1)->Update(widgetlevel+1))
										{
											return false;
										}
								}
						}
				}

			// maybe this better on a PostUpdate
			if(widgetstate != oldwidgetstate)
				{
					// send event with new state
					UIEVENT event(NULL, XEVENTTYPE_UI);
			
					event.SetUIEventType(UIXEVENTTYPE_CHAGESTATE);
					event.SetWidgetState(widgetstate);
					event.SetSender(this);
			
					this->HandleEvent(&event);
				}

			InternalPostUpdate(widgetlevel);
			HandleDrag();
		}
		
	return true;
}

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::InternalDraw
*/	
/**	
//	
//	Draw function for this specific widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/10/2015 9:16:07
//	
//	@return 			bool : true if succeeded
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCONTAINER::InternalDraw(XDWORD& widgetlevel)
{
	return true;
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::InternalPostDraw
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/08/2016 10:12:23
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCONTAINER::InternalPostDraw(XDWORD& widgetlevel)
{
	return true;
}


/*-------------------------------------------------------------------
//	UIWIDGETWINDOW::InternalUpdate
*/	
/**	
//	
//	Update function for this specific widget
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/10/2015 9:16:21
//	
//	@return 			bool : true if succeeded
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCONTAINER::InternalUpdate(XDWORD widgetlevel)
{
	return true;
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::InternalPostUpdate
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/08/2016 13:41:35
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCONTAINER::InternalPostUpdate(XDWORD widgetlevel)
{
	return true;
}

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::HandleInternalEvent
*/	
/**	
//	
//	this function eats the pushbutton click event
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			06/10/2015 10:34:33
//	
//	@return 			bool : 
//
//  @param				event : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCONTAINER::HandleInternalEvent(XEVENT* event)
{
	return true;
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetPosition
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/10/2016 9:36:31
//	
//  @param				x : 
//  @param				y : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetPosition(int x, int y)
{
	this->x = x;
	this->y = y;

	XDWORD layouts = attachedlayouts.GetSize();

	UIEVENT event(this, XEVENTTYPE_UI);
	event.SetUIEventType(UIXEVENTTYPE_MOVE);
	event.SetSender(this);
	for(XDWORD i = 0; i < layouts; i++)
		{
			// send a resize event
			attachedlayouts.FastGet(i)->HandleEvent(&event);
		}
	this->HandleEvent(&event);
	if(parent)
		{
			parent->HandleEvent(&event);
		}
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetPositionX
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/10/2016 9:36:25
//	
//  @param				x : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetPositionX(int x)
{
	this->x = x;

	XDWORD layouts = attachedlayouts.GetSize();

	UIEVENT event(this, XEVENTTYPE_UI);
	event.SetUIEventType(UIXEVENTTYPE_MOVE);
	event.SetSender(this);
	for(XDWORD i = 0; i < layouts; i++)
		{
			// send a resize event
			attachedlayouts.FastGet(i)->HandleEvent(&event);
		}
	this->HandleEvent(&event);
	if(parent)
		{
			parent->HandleEvent(&event);
		}
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetPositionY
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/10/2016 9:36:18
//	
//  @param				y : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetPositionY(int y)
{
	this->y = y;

	XDWORD layouts = attachedlayouts.GetSize();

	UIEVENT event(this, XEVENTTYPE_UI);
	event.SetUIEventType(UIXEVENTTYPE_MOVE);
	event.SetSender(this);
	for(XDWORD i = 0; i < layouts; i++)
		{
			// send a resize event
			attachedlayouts.FastGet(i)->HandleEvent(&event);
		}
	this->HandleEvent(&event);
	if(parent)
		{
			parent->HandleEvent(&event);
		}
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetPositionDIP
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/11/2016 10:58:39
//	
//  @param				x : 
//  @param				y : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetPositionDIP(float x, float y)
{
	float w = x*uimanager->GetDPI()/uidipunit;
	float h = y*uimanager->GetDPI()/uidipunit;

	SetPosition((int)w, (int)h);
}



/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetPositionXDIP
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/11/2016 10:58:33
//	
//  @param				x : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetPositionXDIP(float x)
{
	float w = x*uimanager->GetDPI()/uidipunit;

	SetPositionX((int)w);
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetPositionDIPY
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/11/2016 10:58:26
//	
//  @param				y : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetPositionDIPY(float y)
{
	float h = y*uimanager->GetDPI()/uidipunit;

	SetPositionY((int)h);
}


/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetSize
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/10/2016 9:36:11
//	
//  @param				width : 
//  @param				height : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetSize(XDWORD width, XDWORD height)
{
	this->width = width;
	this->height = height;

	XDWORD layouts = attachedlayouts.GetSize();

	UIEVENT event(this, XEVENTTYPE_UI);
					event.SetUIEventType(UIXEVENTTYPE_RESIZE);
					event.SetSender(this);

	for(XDWORD i = 0; i < layouts; i++)
		{
			// send a resize event
			attachedlayouts.FastGet(i)->HandleEvent(&event);
		}
	this->HandleEvent(&event);
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetWidth
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/10/2016 9:36:04
//	
//  @param				width : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetWidth(XDWORD width)
{
	this->width = width;

	XDWORD layouts = attachedlayouts.GetSize();

	UIEVENT event(this, XEVENTTYPE_UI);
	event.SetUIEventType(UIXEVENTTYPE_RESIZE);
	event.SetSender(this);
	for(XDWORD i = 0; i < layouts; i++)
		{
			// send a resize event
			attachedlayouts.FastGet(i)->HandleEvent(&event);
		}
	this->HandleEvent(&event);
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetHeight
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/10/2016 9:35:58
//	
//  @param				height : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetHeight(XDWORD height)
{
	this->height = height;

	XDWORD layouts = attachedlayouts.GetSize();

	UIEVENT event(this, XEVENTTYPE_UI);
	event.SetUIEventType(UIXEVENTTYPE_RESIZE);
	event.SetSender(this);
	for(XDWORD i = 0; i < layouts; i++)
		{
			// send a resize event
			attachedlayouts.FastGet(i)->HandleEvent(&event);
		}
	this->HandleEvent(&event);
}
 



/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetSizeDIP
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/11/2016 11:07:02
//	
//  @param				width : 
//  @param				height : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetSizeDIP(float width, float height)
{
	float w = width*uimanager->GetDPI()/uidipunit;

	float h = height*uimanager->GetDPI()/uidipunit;
			
	// we have it in pixels, now divide by screen resolution to get actual normalized coordinates
	//scene->GetViewport()->GetWindowSize(pxwidth, pxheight);

	SetSize((int)w, (int)h);
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetWidthDIP
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/11/2016 11:06:55
//	
//  @param				width : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetWidthDIP(float width)
{
	float w = width*uimanager->GetDPI()/uidipunit;

	SetWidth((int)w);
}



/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetHeightDIP
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/11/2016 11:06:41
//	
//  @param				height : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetHeightDIP(float height)
{
	float h = height*uimanager->GetDPI()/uidipunit;

	SetHeight((int)h);
}


/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetRotation
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			28/10/2016 13:16:55
//	
//  @param				x : 
//  @param				y : 
//  @param				z : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetRotation(float x, float y, float z)
{
	rotationx = x;
	rotationy = y;
	rotationz = z;
}



/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetRotationPivot
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			18/04/2017 16:36:20
//	
//  @param				x : 
//  @param				y : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetRotationPivot(int x, int y)
{
	rotationpivotx = x;
	rotationpivoty = y;
}

/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::DeleteChildrenAndLink
*/	
/**	
//	
//	performs the children deletion here instead of the destructor(called by it)
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/10/2015 16:00:53
//	
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::DeleteChildrenAndLink()
{
	// can't iterate the vector normaly as we are changing its contents
	while(childrenbyindex.GetSize())
		{
			delete childrenbyindex.Get(0);
		}

	if(parent)
		{
			parent->RemoveChildPointer(this);
		}
}




/*-------------------------------------------------------------------
//	 UIWIDGETCONTAINER::SetIsLongClickable
*/
/**
//
//	
//
//	@author		Imanol Celaya Ruiz de Alegria 
//	@version		

//	@return		void : 
//
//	@param		bool : 
//
*//*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetIsLongClickable(bool islongclickable)
{
	if(this->islongclickable == false)
		{
			triggertime = 0;
		}

	this->islongclickable = islongclickable;
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::GetWidgetLocalCoordinates
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			16/06/2016 16:18:06
//	
//  @param				iptx : 
//  @param				ipty : 
//  @param				optx : 
//  @param				opty : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::GetWidgetLocalCoordinates(float iptx, float ipty, float & optx, float & opty)
{
	//float width = gpt1x - gpt0x;
	//float height = gpt1y - gpt0y;
	//
	//float lptx = iptx - gpt0x;
	//float lpty = ipty - gpt0y;
	//
	//optx = lptx/width;
	//opty = lpty/height;
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetAnchors
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/06/2016 13:07:55
//	
//  @param				horizontalanchor : 
//  @param				verticalanchor : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetAnchors(UIWIDGET_ANCHOR horizontalanchor, UIWIDGET_ANCHOR verticalanchor)
{
	SetHorizontalAnchor(horizontalanchor);
	SetVerticalAnchor(verticalanchor);
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetHorizontalAnchor
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/06/2016 13:07:48
//	
//  @param				horizontalanchor : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetHorizontalAnchor(UIWIDGET_ANCHOR horizontalanchor)
{
	if((horizontalanchor == UIWIDGET_ANCHOR_LEFT) || (horizontalanchor == UIWIDGET_ANCHOR_RIGHT))
	{
		this->horizontalanchor = horizontalanchor;
	}
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::SetVerticalAnchor
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			23/06/2016 13:07:40
//	
//  @param				verticalanchor : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::SetVerticalAnchor(UIWIDGET_ANCHOR verticalanchor)
{
	if((verticalanchor == UIWIDGET_ANCHOR_TOP) || (verticalanchor == UIWIDGET_ANCHOR_BOTTOM))
	{
		this->verticalanchor = verticalanchor;
	}
}



/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::AttachLayout
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			03/08/2016 16:35:00
//	
//  @param				attachedlayout : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::AttachLayout(UIWIDGETLAYOUT* attachedlayout)
{
	if(attachedlayouts.GetSize())
		{
			// remove old attached layout; // or not, who knows?
			//uimanager->AddToDeleteQueue(this->attachedlayout); // is this really a good idea?, can't handle more than 1 layout this way, and we might want more of them
		}

	this->attachedlayouts.Add(attachedlayout);
	//uimanager->AddToAddQueue(this, attachedlayout);

	if(attachedlayout)
		{
			// send a resize event
			UIEVENT event(this, XEVENTTYPE_UI);
			event.SetUIEventType(UIXEVENTTYPE_RESIZE);
			event.SetSender(this);
			attachedlayout->HandleEvent(&event);
		}
}


/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::ComputeCoords
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/06/2016 13:03:15
//	
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::ComputeCoords()
{
	UIWIDGETCONTAINER* window = (UIWIDGETCONTAINER*)parent;

	int startx = adjustedx;
	int starty = adjustedy;

	if(parent)
		{
			float w, h;
			w = (float)uimanager->GetVirtualWidth();
			h = (float)uimanager->GetVirtualHeight();

			startx += (int)(window->gpt0x*w);
			starty += (int)(window->gpt0y*h);

			float nw = startx/w;
			float nh = starty/h;

			gpt0x = nw;
			gpt0y = nh;

			float ww = adjustedwidth/w;
			float hh = adjustedheight/h;

			gpt1x = gpt0x + ww;
			gpt1y = gpt0y + hh;
		}
	else
		{
			float w, h;
			
			w = (float)uimanager->GetVirtualWidth();
			h = (float)uimanager->GetVirtualHeight();
			
			float nw = startx/w;
			float nh = starty/h;

			gpt0x = nw;
			gpt0y = nh;

			float ww = adjustedwidth/w;
			float hh = adjustedheight/h;

			gpt1x = gpt0x + ww;
			gpt1y = gpt0y + hh;
		}
}



/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::UpdateCoords
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/01/2017 11:22:55
//	
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::UpdateCoords()
{
	ComputeCoords();

	for(XDWORD i = 0; i < childrenbyindex.GetSize(); i++)
		{
			childrenbyindex.FastGet(i)->UpdateCoords();
		}
}




/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::GetHoveredWidget
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/01/2017 11:23:02
//	
//	@return 			UIWIDGET* : 
//
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETCONTAINER::GetHoveredWidget(bool force, float zoom, float x, float y)
{
	bool computeme = true;
	if((uimanager->GetMouseOwner() != this) && (uimanager->GetMouseOwner() != NULL) && !force )
		{
			computeme = false;
		}

	if(uimanager->GetMouseOwner() == this)
		{
			force = true;
		}

	if(!isactive)
		{
			return NULL;
		}

	// but widgets are actually composed, so this way will return the childest(like the text on a button)
	UIWIDGET* ret = NULL;
	bool isover = false;

	iscursorontop = false;
	if(CheckCursorOver(zoom, x, y))
		{
			isover = true;
			iscursorontop = true;
		}

	if(IsClickable() && isactive && computeme)
		{
			if(isover)// && this->ReactsToMouse())
				{
					ret = this;
				}
		}

	bool doit = false;

	if(!limitchildrencollisions)
		{
			doit = true;
		}
	else
		{
			if(isover)
				{
					doit = true;
				}	
		}

	if(doit)
		{
			XDWORD listsize = childrenbyindex.GetSize();

			for(XDWORD i = 0; i < listsize; i++)
				{
					UIWIDGET* w = childrenbyindex.FastGet(i)->GetHoveredWidget(force, zoom, x, y);
					if(w)
						{
							ret = w;
						}
				}
		}

	return ret;
}


/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::CheckCursorOver
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			14/06/2016 10:29:38
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETCONTAINER::CheckCursorOver(float zoom, float zx, float zy)
{
	float x, y;
	uimanager->GetInput()->GetCursorPosition(x, y);
	
	GRPPOINT pt;
	XDWORD w, h;
	w = uimanager->GetRealWidth(); // but I don't want the virtual one, but the real one
	h = uimanager->GetRealHeight();

	pt.x = x/w;
	pt.y = y/h;

	pt.y -= 1;
	pt.y *= -1;

	lastcursorposx = currentcursorposx;
	lastcursorposy = currentcursorposy;
	
	currentcursorposx = pt.x;
	currentcursorposy = pt.y;
	
	// but gpt are centered, or take top left as origin?

	isreleased = uimanager->GetInput()->IsReleased(INPBUTTON_ID_MOUSE_LEFT);
	
	// need to apply the zoom to gpt coordinates to get cursor collision correctly

	float scalex = gpt1x - gpt0x;
	float scaley = gpt1y - gpt0y;

	float cx = gpt0x;
	float cy = gpt0y;
	float vx = cx - zx;
	float vy = cy - zy;

	vx *= zoom;
	vy *= zoom;
	cx = zx + vx;
	cy = zy + vy;

	scalex *= zoom;
	scaley *= zoom;
	// recompute corners
	float pt0x = cx;
	float pt0y = cy;
	float pt1x = cx + scalex;
	float pt1y = cy + scaley;

	if(PointRectangle(pt.x, pt.y, pt0x, pt0y, pt1x, pt1y))
		{
			istriggered		= uimanager->GetInput()->IsTriggered	(INPBUTTON_ID_MOUSE_LEFT);
			ispressed			= uimanager->GetInput()->IsPressed		(INPBUTTON_ID_MOUSE_LEFT);
			ispressedlong = uimanager->GetInput()->IsPressedLong(INPBUTTON_ID_MOUSE_LEFT);
	
			if(istriggered)
				{
					triggerposx = currentcursorposx;
					triggerposy = currentcursorposy;
					//triggerlocalposx = currentcursorposx - gpt0x;
					//triggerlocalposy = currentcursorposy - gpt0y;
					// maybe?
					triggerlocalposx = currentcursorposx - pt0x;
					triggerlocalposy = currentcursorposy - pt0y;
				}
	
			return true;
		}
	else
		{
			istriggered = false;
			ispressed = false;
			ispressedlong = false;
		}

	return false;
}


/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::ComputeDesiredSize
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			17/11/2016 11:38:52
//	
//	@return 			GRPPOINT : 
//
*/
/*-----------------------------------------------------------------*/
GRPPOINT UIWIDGETCONTAINER::ComputeDesiredSize()
{
	// own widget doesn't send own position for required size(or yes, as long as we save it to ourselves without position?)
	if(childrenbyindex.GetSize())
		{
			// if children return required size for children as needed
			XDWORD children = childrenbyindex.GetSize();
			GRPPOINT p;

			// desired width  = [px0 + w0] + [px1 + w1] + ... + [pxn + wn]
			// desired height = [py0 + h0] + [py1 + h1] + ... + [pyn + hn]
			for(XDWORD i = 0; i < children; i++)
				{
					UIWIDGETCONTAINER* wc = (UIWIDGETCONTAINER*)childrenbyindex.FastGet(i);
					GRPPOINT np = wc->ComputeDesiredSize();
					np.x += wc->GetPositionX();
					np.y += wc->GetPositionY();

					p.x = __MAX(p.x, np.x);
					p.y = __MAX(p.y, np.y);
					//p = p + np;
				}

			// now compare with own and return the biggest?
			p.x = __MAX(p.x, width);
			p.y = __MAX(p.y, height);
			desiredsize = p;
			return desiredsize;
		}
	else
		{
			// if no children return own size
			desiredsize = GRPPOINT((float)width, (float)height);
			return desiredsize;
		}
}



/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::ArrangeChildren
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			17/11/2016 17:03:35
//	
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::ArrangeChildren()
{
	// way better function to implement
	InternalArrangeChildren();

	XDWORD children = childrenbyindex.GetSize();

	for(XDWORD i = 0; i < children; i++)
		{
			childrenbyindex.FastGet(i)->ArrangeChildren();
		}
}


/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::HandleDrag
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/06/2016 13:03:05
//	
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::HandleDrag()
{
	//// using this to allow select dragging areas without interfering with long presses
	if(widgetstate & UIWIDGET_STATE_DRAGPRESSED)
		{
			// need to get delta into virtual desktop coordinates, need to adapt them to widget local coordinates(to their parent)
			if(parent)
				{
					// wrong, we need gpt to get the cumulative real position
					UIWIDGETCONTAINER* c = (UIWIDGETCONTAINER*)parent;
					// transform cursor positions taking into account paretn widget adjusted values
					float tcurrentx = currentcursorposx*uimanager->GetVirtualWidth()  - c->gpt0x*uimanager->GetVirtualWidth();
					float tcurrenty = currentcursorposy*uimanager->GetVirtualHeight() - c->gpt0y*uimanager->GetVirtualHeight();

					float ttriggertx = triggerlocalposx*uimanager->GetVirtualWidth();
					float ttriggerty = triggerlocalposy*uimanager->GetVirtualHeight();

					SetPositionX((int)(tcurrentx - ttriggertx));
					SetPositionY((int)(tcurrenty - ttriggerty));
				}
			else
				{
					SetPositionX((int)(currentcursorposx*uimanager->GetVirtualWidth()  - triggerlocalposx*uimanager->GetVirtualWidth()));
					SetPositionY((int)(currentcursorposy*uimanager->GetVirtualHeight() - triggerlocalposy*uimanager->GetVirtualHeight()));
				}
		}
}





/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::UpdateWidgetState
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			14/06/2016 11:11:12
//	
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::UpdateWidgetState()
{
	widgetstate = 0 | (widgetstate & UIWIDGET_STATE_DRAGPRESSED);

	if(uimanager->IsInMotion() == false)
	{
		if(iscursorover)
		{
			if(this->IsHoverable())
			{
				widgetstate |= UIWIDGET_STATE_HOVERED;
				if(!(oldwidgetstate & UIWIDGET_STATE_HOVERED))
				{
					UIEVENT event(this, XEVENTTYPE_UI);

					event.SetUIEventType(UIXEVENTTYPE_HOVER);
					event.SetSender(this);

					if(uimanager->IsInMotion() == false)
					{
						if(parent)
						{
							parent->HandleEvent(&event);
						}

						this->HandleEvent(&event);
					}
				}
			}

			if(istriggered)
			{
				if(this->IsClickable())
				{
					if(!(oldwidgetstate & UIWIDGET_STATE_PRESSED))
					{
						widgetstate |= UIWIDGET_STATE_TRIGGERED;
						widgetstate |= UIWIDGET_STATE_PRESSED;

						UIEVENT event(this, XEVENTTYPE_UI);

						event.SetUIEventType(UIXEVENTTYPE_TRIGGER);
						event.SetSender(this);

						if(uimanager->IsInMotion() == false)
						{
							if(parent)
							{
								parent->HandleEvent(&event);
							}

							this->HandleEvent(&event);
						}
					}
				}
			}

			if(ispressed)
			{
				if(this->IsClickable())
				{
					if((oldwidgetstate & UIWIDGET_STATE_PRESSED) || (oldwidgetstate & UIWIDGET_STATE_TRIGGERED))
					{
						widgetstate |= UIWIDGET_STATE_PRESSED;
					}

					if((widgetstate & UIWIDGET_STATE_PRESSED) && !(oldwidgetstate & UIWIDGET_STATE_PRESSED))
					{
						UIEVENT event(this, XEVENTTYPE_UI);

						event.SetUIEventType(UIXEVENTTYPE_PRESS);
						event.SetSender(this);

						if(uimanager->IsInMotion() == false)
						{
							if(parent)
							{
								parent->HandleEvent(&event);
							}

							this->HandleEvent(&event);
						}
					}
				}

				if(this->IsDragrable())
				{
					if((oldwidgetstate & UIWIDGET_STATE_DRAGPRESSED) || (oldwidgetstate & UIWIDGET_STATE_TRIGGERED))
					{
						widgetstate |= UIWIDGET_STATE_DRAGPRESSED;
					}
				}
			}
		}

		if(isreleased)
		{
			widgetstate = widgetstate & (~UIWIDGET_STATE_DRAGPRESSED);
		}

		if(issticky)
		{
			if(oldwidgetstate & UIWIDGET_STATE_PRESSED)
			{
				widgetstate |= UIWIDGET_STATE_PRESSED;
			}


			if((oldwidgetstate & UIWIDGET_STATE_PRESSED) && istriggered)
			{
				widgetstate = widgetstate & (~UIWIDGET_STATE_PRESSED);
				widgetstate |= UIWIDGET_STATE_RELEASED;

				UIEVENT event(this, XEVENTTYPE_UI);

				event.SetUIEventType(UIXEVENTTYPE_RELEASE);
				event.SetSender(this);

				if(uimanager->IsInMotion() == false)
				{
					if(parent)
					{
						parent->HandleEvent(&event);
					}

					this->HandleEvent(&event);
				}
			}
		}
		else
		{
			if((oldwidgetstate & (UIWIDGET_STATE_PRESSED | UIWIDGET_STATE_LONGPRESSED)) && !(widgetstate & (UIWIDGET_STATE_PRESSED | UIWIDGET_STATE_LONGPRESSED)))
			{
				widgetstate |= UIWIDGET_STATE_RELEASED;
			}
		}


		if((widgetstate & UIWIDGET_STATE_RELEASED) && (oldwidgetstate & UIWIDGET_STATE_PRESSED))
		{
			// double event, we made a full click and released the button
			if(uimanager->IsInMotion() == false)
			{
				UIEVENT event(this, XEVENTTYPE_UI);

				event.SetUIEventType(UIXEVENTTYPE_CLICK);
				event.SetSender(this);

				if(parent)
				{
					parent->HandleEvent(&event);
				}


				UIEVENT releaseevent(this, XEVENTTYPE_UI);

				releaseevent.SetUIEventType(UIXEVENTTYPE_RELEASE);
				releaseevent.SetSender(this);

				if(parent)
				{
					parent->HandleEvent(&releaseevent);
				}
			}
		}
	}
	else
	{

	}

	if(islongclickable)
		{
			if(widgetstate & UIWIDGET_STATE_TRIGGERED)
				{
					triggertime = uimanager->GetClockTime();
				}
			else if(widgetstate & UIWIDGET_STATE_RELEASED)
				{
					triggertime = 0;
				}

			if(triggertime != 0)
				{
					if((uimanager->GetClockTime() - triggertime) > uimanager->GetLongPressTime())
						{
							widgetstate |= UIWIDGET_STATE_LONGPRESSED;
						}
				}
		}

	if((widgetstate & UIWIDGET_STATE_LONGPRESSED) && !(oldwidgetstate & UIWIDGET_STATE_LONGPRESSED))
		{
			UIEVENT event(this, XEVENTTYPE_UI);

			event.SetUIEventType(UIXEVENTTYPE_LONGPRESS);
			event.SetSender(this);

			if(uimanager->IsInMotion() == false)
				{
					if(parent)
					{
						parent->HandleEvent(&event);
					}
				}
		}

	if(ismotionable)
		{
			// need a flag to make sure if we are on motion(maybe all of this into update?)
			float cursorx;
			float cursory;
			uimanager->GetInput()->GetCursorPosition(cursorx, cursory);

			if(istriggered)
				{
					if(iscursorontop && (IsChild(uimanager->GetHoveredWidget()) || iscursorover))
						{
							UIMOTION::UIMOTIONENTRY* entry = new UIMOTION::UIMOTIONENTRY();
							entry->type = UIMOTION_TYPE_DOWN;
							entry->x = cursorx;
							entry->y = cursory;
							entry->timestamp = uimanager->GetClockTime();
							motion->AddMotion(entry);
							// post event
							UIEVENT uievent(this, UIXEVENTTYPE_MOTION);
							uievent.SetMotion(motion);
							uievent.SetSender(this);
							this->HandleEvent(&uievent);
							if(parent)
								{
									parent->HandleEvent(&uievent);
								}
						//uimanager->PostMotionEvent(motion);
							
						}
				}
				
			if(motion->GetMotionNumber() > 0)
				{
					if(isreleased)
						{
							UIMOTION::UIMOTIONENTRY* entry = new UIMOTION::UIMOTIONENTRY();
							entry->type = UIMOTION_TYPE_UP;
							entry->x = cursorx;
							entry->y = cursory;
							entry->timestamp = uimanager->GetClockTime();
							motion->AddMotion(entry);
							// post event

							UIEVENT uievent(this, UIXEVENTTYPE_MOTION);
							uievent.SetMotion(motion);
							uievent.SetSender(this);
							this->HandleEvent(&uievent);
							if(parent)
								{
									parent->HandleEvent(&uievent);
								}

						//uimanager->PostMotionEvent(motion);
							motion->Clear();
							uimanager->SetIsInMotion(false);
						}
					else if(ispressed)
						{
							// maybe check cadence
							UIMOTION::UIMOTIONENTRY * last = motion->GetLastMotion();
							float xdiff = (float)fabs(last->x - cursorx);
							float ydiff = (float)fabs(last->y - cursory);
							if((xdiff > 10) || (ydiff > 10))
								{
									UIMOTION::UIMOTIONENTRY* entry = new UIMOTION::UIMOTIONENTRY();
									entry->type = UIMOTION_TYPE_MOVE;
									entry->x = cursorx;
									entry->y = cursory;
									entry->timestamp = uimanager->GetClockTime();
									motion->AddMotion(entry);

									UIEVENT uievent(this, UIXEVENTTYPE_MOTION);
									uievent.SetMotion(motion);
									uievent.SetSender(this);
									this->HandleEvent(&uievent);
									if(parent)
										{
											parent->HandleEvent(&uievent);
										}
									uimanager->SetIsInMotion(true);
									// Post Event
									//uimanager->PostMotionEvent(motion);
								}
						}
				}
		}
}



/*-------------------------------------------------------------------
//	UIWIDGETCONTAINER::InternalArrangeChildren
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			02/02/2017 14:19:29
//	
*/
/*-----------------------------------------------------------------*/
void UIWIDGETCONTAINER::InternalArrangeChildren()
{
	if(!isadjusted)
		{
			UIWIDGETCONTAINER* p = (UIWIDGETCONTAINER*)parent;
			adjustedx				= x;
			adjustedy				= y;
			adjustedwidth		= width;
			adjustedheight	= height;

			isadjusted = true;

			if(p)
				{
					if(horizontalconstraint == UIWIDGET_CONSTRAINTS_FILL_PARENT)
						{
							adjustedx = 0;
							adjustedwidth = p->adjustedwidth;//p->GetWidth();
						}
			
					if(verticalconstraints == UIWIDGET_CONSTRAINTS_FILL_PARENT)
						{
							adjustedy = 0;
							adjustedheight = p->adjustedheight;//p->GetHeight();
						}
				}
		}

	// but need to set children adjusted values !!
	// nothing if regular widget
}
