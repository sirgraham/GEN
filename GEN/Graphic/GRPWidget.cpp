
/*------------------------------------------------------------------------------------------
//	GRPWIDGET.CPP
//	
//	Manages widget styles
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 17/08/2017 12:50:34
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPMaterialLibrary.h"
#include "GRPFontManager.h"
#include "GRPObjectBuilder.h"
#include "GRPShaderLibrary.h"
#include "GRPWidget.h"
#include "GRPScreen.h"
#include "GRPScene.h"
#include "GRPWidgetPage.h"
#include "GRPProfiler.h"
#include "XPair.h"
#include "XFileXML.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	 GRPWIDGETFACTORY::Create
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 16:27:43
//	@return		GRPWIDGET* : 
//
//	@param		XCHAR* : 
//	@param		XCHAR* : 
//	@param		XCHAR* : 
//	@param		XCHAR* : 
//	@param		XCHAR* : 
//
*//*-----------------------------------------------------------------*/
GRPWIDGET* GRPWIDGETFACTORY::Create(XCHAR* tag)
{		
		GRPWIDGET*	widget = new GRPWIDGET();		
								widget->SetTag(tag);
		return widget;
}



/*-------------------------------------------------------------------
//	GRPWIDGET::Create
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/08/2017 10:50:57
//	
//	@return 			GRPWIDGET* : 
//
*/
/*-----------------------------------------------------------------*/
GRPWIDGET* GRPWIDGET::Create()
{
	ismodified = 0;

	Init									();	//----------- FreshStart	

	ApplyStyles						();	//------------ styles		
	CreateText						();	//------------ font	
	AssignTextColor				();
	CreateBackground			();	//------------ background						
	AssignBackgroundColor	();
	CreateContent					();	//------------ Image
	CreateBorder					();	//------------ border
	AssignBorderColor			();

	CreateTextShadow			();
	AssignTextShadowColor ();
	
	if (content.GetSize())
			text = content;
			

	return this;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::CreateSection
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 16:12:13
//	@return		bool : 
//
//	@param		GRPWIDGETSECTION : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::CreateSection(GRPWIDGETSECTION section)
{
		switch (section)
		{
				case GRPWIDGETSECTION_BACKGROUND: return CreateBackground		();
				case GRPWIDGETSECTION_BORDER:		  return CreateBorder				();
				case GRPWIDGETSECTION_OUTLINE:		return CreateOutline			();
				case GRPWIDGETSECTION_BOXSHADOW:	return CreateShadow				();
				case GRPWIDGETSECTION_CONTENT:		return CreateContent			();		
				case GRPWIDGETSECTION_TEXT:				return CreateText					();
				case GRPWIDGETSECTION_TEXTSHADOW:	return CreateTextShadow		();
		}
		return false;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::AssignColor
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 16:16:41
//	@return		bool : 
//
//	@param		GRPWIDGETSECTION : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::AssignColor(GRPWIDGETSECTION section)
{
		switch (section)
		{
		case GRPWIDGETSECTION_BACKGROUND: return AssignBackgroundColor();
		case GRPWIDGETSECTION_BORDER:		  return AssignBorderColor();
		case GRPWIDGETSECTION_OUTLINE:		return AssignOutlineColor();
		case GRPWIDGETSECTION_BOXSHADOW:	return AssignShadowColor();
		case GRPWIDGETSECTION_CONTENT:		return AssignContentColor();
		case GRPWIDGETSECTION_TEXT:				return AssignTextColor();
		case GRPWIDGETSECTION_TEXTSHADOW:	return AssignTextShadowColor();
		}
		return false;
}
/*-------------------------------------------------------------------
//	GRPWIDGET::GRPWIDGET
*/	
/**	
//	
//	Class Constructor GRPWIDGET
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2017 13:28:36
//	
*/
/*-----------------------------------------------------------------*/
GRPWIDGET::GRPWIDGET()
{
	Clean();
}

/*-------------------------------------------------------------------
//	GRPWIDGET::~GRPWIDGET
*/	
/**	
//	
//	 Class Destructor GRPWIDGET
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/09/2017 10:31:10
//	
*/
/*-----------------------------------------------------------------*/
GRPWIDGET::~GRPWIDGET()
{

	this->colors.DeleteElementContents();
	this->colors.DeleteAll();
	this->materials.DeleteAll();
	
	this->elements.DeleteElementContents();
	this->elements.DeleteAll();

	this->classes.DeleteAll();
	
	this->widgets.DeleteContentsInstanced();
	this->widgets.DeleteAll();

	this->mapvaluetype.DeleteAll();	

	if (this->aligner)
	{
			delete(this->aligner);
			this->aligner = NULL;
	}


	this->nodes.DeleteElementContents();
	this->nodes.DeleteAll();

	delete(this->node);

	this->Publish(GRPMSG_DESTROYED);
}


/*-------------------------------------------------------------------
//	 GRPWIDGET::SetTag
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 16:42:01
//	@return		void : 
//
//	@param		XCHAR* : 
//
*//*-----------------------------------------------------------------*/
void GRPWIDGET::SetTag(XCHAR* tag)
{
		this->tag = tag;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::SetClassType
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 16:43:20
//	@return		void : 
//
//	@param		XCHAR* : 
//
*//*-----------------------------------------------------------------*/
void GRPWIDGET::SetClassType(XCHAR* classtype)
{
		this->classtype = classtype;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::Print
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/01/2018 14:01:49
//	@return		WIDGETSTRING : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
WIDGETSTRING	GRPWIDGET::Print()
{
		WIDGETSTRING text;

		XDWORD d = (XDWORD)GetDepth();

		text.Format(__L("%010x"),(void*)this);

		for (XDWORD i = 0; i < d; i++)
				text.Add(__L(" > "));

		for (XDWORD e = 0; e < this->classes.GetSize(); e++)
		{
				text.Add(this->classes.Get(e)->name.Get());
				text.Add(__L(" "));
		}

		return text;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::Manage
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	06/02/2018 17:24:32
//	@return		bool : 
//
//	@param		GRPWIDGETSTATE : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::Manage(GRPWIDGETSTATE state,float x, float y)
{
		if (state == GRPWIDGETSTATE_NONE)		
				this->SetState(state);						
		else
		if (this->Hit(x, y))		
				this->SetState(state);				
		
		for (XDWORD e = 0; e < this->widgets.GetSize(); e++)
		{
				this->widgets.Get(e)->Manage(state, x, y);
		}
		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::Hit
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	06/02/2018 17:27:52
//	@return		bool : 
//
//	@param		float : 
//	@param		float : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::Hit(float x, float y)
{			
				GRPNODE* node = this->node;

				GRPVECTOR localposition;
				localposition = node->GetWorldPosition();
				localposition.vector[0] = x - localposition.vector[0];
				localposition.vector[1] = y - localposition.vector[1];
				localposition.vector[2] = 0;

				if (localposition.vector[0]>0 && localposition.vector[0]<boundingbox.width)
				if (localposition.vector[1]>0 && localposition.vector[1]<boundingbox.height)
				{
						return true;
				}

				return false;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::GetDepth
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/01/2018 14:01:39
//	@return		int : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
int	GRPWIDGET::GetDepth()
{
		int depth = 0;
		GRPWIDGET* p = this;
		while (p->parent != NULL)
		{
				p = p->parent;
				depth++;
		}
		return depth;
}

/*-------------------------------------------------------------------
//	GRPWIDGET::SetParent
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2017 16:09:27
//	
//	@return 			bool : 
//
//  @param				parent : 
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGET::SetParent(GRPWIDGET* parent)
{
	if (parent)
	{
			if (this->parent)
					this->parent->widgets.Delete(this);

			this->parent = parent;

			if (this->node)
					this->node->SetParent(parent->node);

			parent->widgets.Add(this);

			this->context = parent;
			this->root		= parent->root;
			
			OnAdded();
		
	}

	return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::NotifyWidth
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	04/01/2018 14:15:05
//	@return		void : 
//
//	@param		GLFLOAT : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::NotifyWidth(GLFLOAT ww)
{
		float w = ww;

		if (w > (float)Get(GRPWIDGETSTYLETYPE_MAX_WIDTH))
				w = (float)Get(GRPWIDGETSTYLETYPE_MAX_WIDTH);

		if (w > this->boundingbox.width)
		{				
				
				this->boundingbox.width = w;
				this->ismodified |= GRPWIDGETSECTION_FLAG_BACKGROUND;
				this->ismodified |= GRPWIDGETSECTION_FLAG_CONTENT;
				this->ismodified |= GRPWIDGETSECTION_FLAG_BORDER;
				this->ismodified |= GRPWIDGETSECTION_FLAG_BOXSHADOW;

				if (this->parent)
				{						
						this->parent->NotifyWidth(w);						
						this->parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
				}				
				return true;
		}

		return false;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::NotifyHeight
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	23/01/2018 11:47:28
//	@return		bool : 
//
//	@param		GLFLOAT : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::NotifyHeight(GLFLOAT hh)
{
		XDWORD h = (XDWORD)hh;
		if (h > (XDWORD)Get(GRPWIDGETSTYLETYPE_MAX_HEIGHT))
				h = (XDWORD)Get(GRPWIDGETSTYLETYPE_MAX_HEIGHT);

		if (h > this->boundingbox.height)
		{

				this->boundingbox.height = float(h);
				this->Set(GRPWIDGETSTYLETYPE_HEIGHT, (XDWORD)h);
				this->ismodified |= GRPWIDGETSECTION_FLAG_BACKGROUND;
				this->ismodified |= GRPWIDGETSECTION_FLAG_CONTENT;
				this->ismodified |= GRPWIDGETSECTION_FLAG_BORDER;
				this->ismodified |= GRPWIDGETSECTION_FLAG_BOXSHADOW;

				if (this->parent)
				{
						if (h > this->parent->boundingbox.height)
						{
								this->parent->NotifyHeight(float(h));
								this->parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
						}
				}
				return true;
		}
		return false;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::Notify
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	07/02/2018 10:42:01
//	@return		bool : 
//
//	@param		GRPMSGTYPE : 
//	@param		GRPSUBJECT* : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::Notify(GRPMSGTYPE	msg, GRPSUBJECT* notifier)
{
		switch (msg)
		{
		case UI_MOUSE_MOVE:  this->SetState(GRPWIDGETSTATE_HOVER);  break;
		case UI_MOUSE_CLICK: 
		{				
				if (this->GetState() != GRPWIDGETSTATE_ACTIVE)
				{
						this->SetState(GRPWIDGETSTATE_ACTIVE);
						this->Publish(UI_WIDGET_ACTIVE, this);
				}
				break;
		}
		case GRPMSG_DESTROYED:
		{
			GRPWIDGET* widget = dynamic_cast<GRPWIDGET*>(notifier);
			if (!widget) return false;
			this->widgets.DeleteIndex(widgets.Find(widget));
		}
		break;
		}

		
		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::Has
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	13/12/2017 12:24:40
//	@return		bool : 
//
//	@param		GRPWIDGETSTYLETYPE : 
//
*//*-----------------------------------------------------------------*/
bool	GRPWIDGET::Has(GRPWIDGETSTYLETYPE t)
{
		for (XDWORD c = 0; c < this->classes.GetSize(); c++)
				if (classes.Get(c)->styles.Find(t) != -1)
						if (classes.Get(c)->AppliesTo(this))
								return true;

		if (this->style.styles.Find(t) != -1)
				return true;
		
		return false;
}

/*-------------------------------------------------------------------
//	GRPWIDGET::ApplyStyles
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2017 14:24:13
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGET::ApplyStyles()
{
	for (XDWORD c = 0; c < this->classes.GetSize(); c++)
	{
			GRPWIDGETSTYLECLASS* wclass = classes.Get(c);			
			if (wclass->AppliesTo(this))
					wclass->ApplyTo(this);			
	}

	this->style.ApplyTo(this);	

	this->ismodified &= ~GRPWIDGETSECTION_FLAG_STYLE;
	return true;
}


/*-------------------------------------------------------------------
//	 GRPWIDGET::Init
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	11/12/2017 16:19:37
//	@return		void : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
void GRPWIDGET::Init()
{
	elements.DeleteElementContents		();
	elements.DeleteAll								();	

	colors.DeleteElementContents			();
	colors.DeleteAll									();

	texturex=1.0f;
	texturey=1.0f;
}

/*-------------------------------------------------------------------
//	GRPWIDGET::CreateBorder
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/09/2017 16:11:35
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGET::CreateBorder()
{
		XDWORD flags = 0;

		if (!this->Get(GRPWIDGETSTYLETYPE_ISTEXT))
		{
				if (this->Get(GRPWIDGETSTYLETYPE_BORDER_STYLE) == GRPWIDGETSTYLE_BORDER_STYLE_TYPE_NONE)
				{
						ismodified &= ~GRPWIDGETSECTION_FLAG_BORDER;
						return true;
				}
		}
		else
		{
				switch (this->Get(GRPWIDGETSTYLETYPE_FONT_DECORATION))
				{
				case GRPWIDGETSTYLE_FONT_DECORATION_NONE:	return true; break;
				case GRPWIDGETSTYLE_FONT_DECORATION_UNDERLINE:						
						flags |= GRPBOXSIDEFLAG_TOP;						
						break;
				case GRPWIDGETSTYLE_FONT_DECORATION_OVERLINE:
						flags |= GRPBOXSIDEFLAG_BOTTOM;
						break;
				}
				float h = this->boundingbox.height;
				this->Set(GRPWIDGETSTYLETYPE_BORDER_WIDTH, XDWORD(h / 10.0f));
		}
				
		if (this->Get(GRPWIDGETSTYLETYPE_BORDER_BOTTOM))
		{
				boundingbox.bottomborder =(float) this->Get(GRPWIDGETSTYLETYPE_BORDER_WIDTH);
				flags |= GRPBOXSIDEFLAG_TOP;
		}
		if (this->Get(GRPWIDGETSTYLETYPE_BORDER_TOP))
		{
				boundingbox.topborder = (float)this->Get(GRPWIDGETSTYLETYPE_BORDER_WIDTH);
				flags |= GRPBOXSIDEFLAG_BOTTOM;
		}
		if (this->Get(GRPWIDGETSTYLETYPE_BORDER_LEFT))
		{
				boundingbox.rightborder = (float)this->Get(GRPWIDGETSTYLETYPE_BORDER_WIDTH);
				flags |= GRPBOXSIDEFLAG_RIGHT;
		}
		if (this->Get(GRPWIDGETSTYLETYPE_BORDER_RIGHT))
		{
				boundingbox.leftborder = (float)this->Get(GRPWIDGETSTYLETYPE_BORDER_WIDTH);
				flags |= GRPBOXSIDEFLAG_LEFT;
		}

		if (flags == 0)
				return true;

		XDEBUG_PRINTCOLOR(4, __L("Creating border for %s:%s"), this->name.Get(), this->tag.Get());


		if (this->elements.Get(GRPWIDGETSECTION_BORDER) != NULL)
		{
				GRPELEMENT* e = this->elements.Get(GRPWIDGETSECTION_BORDER);					
				delete(e);
				this->elements.Delete(GRPWIDGETSECTION_BORDER);
		}

		GRPCOLOR* color = this->GetColor(GRPWIDGETSECTION_BORDER);
		if (!color)
						  color = this->GetColor(GRPWIDGETSECTION_TEXT);
		if (!color)
		{
				ismodified &= ~GRPWIDGETSECTION_FLAG_BORDER;
				return true;
		}

		float w = boundingbox.width;
		float h = boundingbox.height;

		float borderradius	= (float)this->Get(GRPWIDGETSTYLETYPE_BORDER_RADIUS);
		float borderwidth		= (float)this->Get(GRPWIDGETSTYLETYPE_BORDER_WIDTH);
		
		GRPELEMENT* e = GRPOBJECTBUILDER::Get()->CreateBorder		 (	w,
																																h,
																																borderradius,
																																borderwidth,
																																flags,
																																w / texturex,
																																-h / texturey,
																																(texture_offset_x / texturex),
																																(texture_offset_y / texturey));
		GRPNODE* node = nodes.Get(GRPWIDGETSECTION_BORDER);
		if (!node)
		{
				node = scene->CreateNode();
				nodes.Add(GRPWIDGETSECTION_BORDER, node);
				node->SetParent(this->node);
		}

		e->SetNode(node);

  this->elements.Add(GRPWIDGETSECTION_BORDER,e);
	ismodified &= ~GRPWIDGETSECTION_FLAG_BORDER;

	ismodified |= GRPWIDGETSECTION_FLAG_BORDER_COLOR;

	if (parent)
			parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
	return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::CreateOutline
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 15:52:38
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::CreateOutline()
{
		XDWORD flags = 0;

		if (this->Get(GRPWIDGETSTYLETYPE_ISTEXT))
		{			
				ismodified &= ~GRPWIDGETSECTION_FLAG_OUTLINE; 
				ismodified &= ~GRPWIDGETSECTION_FLAG_OUTLINE_COLOR;
				return true;
		}			

		if (!this->Get(GRPWIDGETSTYLETYPE_OUTLINE))
		{
				ismodified &= ~GRPWIDGETSECTION_FLAG_OUTLINE;
				ismodified &= ~GRPWIDGETSECTION_FLAG_OUTLINE_COLOR;
				return true;
		}

		if (!this->Get(GRPWIDGETSTYLETYPE_OUTLINE_STYLE))
		{
				ismodified &= ~GRPWIDGETSECTION_FLAG_OUTLINE;
				ismodified &= ~GRPWIDGETSECTION_FLAG_OUTLINE_COLOR;
				return true;
		}
		
				flags |= GRPBOXSIDEFLAG_TOP;
				flags |= GRPBOXSIDEFLAG_BOTTOM;
				flags |= GRPBOXSIDEFLAG_RIGHT;		
				flags |= GRPBOXSIDEFLAG_LEFT;
				
		if (this->elements.Get(GRPWIDGETSECTION_OUTLINE) != NULL)
		{
				GRPELEMENT* e = this->elements.Get(GRPWIDGETSECTION_OUTLINE);
				delete(e);
				this->elements.Delete(GRPWIDGETSECTION_OUTLINE);
		}

		GRPCOLOR* color = this->GetColor(GRPWIDGETSECTION_OUTLINE);
		if (!color)
				color = this->GetColor(GRPWIDGETSECTION_TEXT);
		if (!color)
		{
				ismodified &= ~GRPWIDGETSECTION_FLAG_OUTLINE;
				return true;
		}

		float w = boundingbox.width	+	boundingbox.leftborder+	boundingbox.rightborder;
		float h = boundingbox.height+ boundingbox.topborder + boundingbox.bottomborder;

		float borderradius = 0.0f;
		float borderwidth = (float)this->Get(GRPWIDGETSTYLETYPE_OUTLINE_WIDTH);

		float x = -boundingbox.leftborder;
		float y = -boundingbox.topborder;

		w+= this->Get(GRPWIDGETSTYLETYPE_OUTLINE_OFFSET)*2.0f;
		h+= this->Get(GRPWIDGETSTYLETYPE_OUTLINE_OFFSET)*2.0f;
		x-= this->Get(GRPWIDGETSTYLETYPE_OUTLINE_OFFSET);
		y-= this->Get(GRPWIDGETSTYLETYPE_OUTLINE_OFFSET);

		GRPELEMENT* e = GRPOBJECTBUILDER::Get()->CreateBorder(w,
				h,
				borderradius,
				borderwidth,
				flags,
				w / texturex,
				-h / texturey,
				(texture_offset_x / texturex),
				(texture_offset_y / texturey));

		GRPNODE* node = nodes.Get(GRPWIDGETSECTION_OUTLINE);
		if (!node)
		{
				node = scene->CreateNode();
				nodes.Add(GRPWIDGETSECTION_OUTLINE, node);
				node->SetParent(this->node);
		}

		e->SetNode(node);
		node->SetPosition(x, y, 0.0f);

		this->elements.Add(GRPWIDGETSECTION_OUTLINE,e);
		ismodified &= ~GRPWIDGETSECTION_FLAG_OUTLINE;

		ismodified |= GRPWIDGETSECTION_FLAG_OUTLINE_COLOR;

		if (parent)
				parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
		return true;

}
/*-------------------------------------------------------------------
//	 GRPWIDGET::CreateBackground
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	13/02/2018 14:38:09
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::CreateBackground()
{
		if (this->elements.Get(GRPWIDGETSECTION_BACKGROUND) != NULL)
		{
				GRPELEMENT* e = this->elements.Get(GRPWIDGETSECTION_BACKGROUND);				
				e->SetNode(NULL);
				delete(e);
				this->elements.Delete(GRPWIDGETSECTION_BACKGROUND);
		}

		if (this->colors.Find(GRPWIDGETSECTION_BACKGROUND) == -1 &&
				this->materials.Find(GRPWIDGETSECTION_BACKGROUND) == -1)
		{
				ismodified &= ~GRPWIDGETSECTION_FLAG_BACKGROUND;
				return true;
		}

		//float w = this->GetWidth();
		float w = boundingbox.width + boundingbox.leftpadding + boundingbox.rightpadding;
		float h = boundingbox.height + boundingbox.toppadding + boundingbox.bottompadding;

		float borderradius = (float)this->Get(GRPWIDGETSTYLETYPE_BORDER_RADIUS);
		GRPELEMENT* e = GRPOBJECTBUILDER::Get()->CreateBoxRounded(w,
				h,
				borderradius,
				w / texturex,
				-h / texturey,
				(texture_offset_x / texturex),
				(texture_offset_y / texturey));

		GRPNODE* node=nodes.Get(GRPWIDGETSECTION_BACKGROUND);
		if (!node)
		{
				node = scene->CreateNode();
				nodes.Add(GRPWIDGETSECTION_BACKGROUND, node);
				node->SetParent(this->node);
		}

		e->SetNode(node);
		this->elements.Add(GRPWIDGETSECTION_BACKGROUND, e);
		ismodified &= ~GRPWIDGETSECTION_FLAG_BACKGROUND;

		ismodified |= GRPWIDGETSECTION_FLAG_BACKGROUND_COLOR;

		if (parent)
				parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;

		ismodified |= GRPWIDGETSECTION_FLAG_BORDER;
		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::CreateShadow
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	19/02/2018 14:45:44
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::CreateShadow()
{
		if (this->elements.Get(GRPWIDGETSECTION_BOXSHADOW) != NULL)
		{
				GRPELEMENT* e = this->elements.Get(GRPWIDGETSECTION_BOXSHADOW);
				e->SetNode(NULL);
				delete(e);
				this->elements.Delete(GRPWIDGETSECTION_BOXSHADOW);
		}

		if (!this->Get(GRPWIDGETSTYLETYPE_BOXSHADOW))
		{
				ismodified &= ~GRPWIDGETSECTION_FLAG_BOXSHADOW;
				ismodified &= ~GRPWIDGETSECTION_FLAG_BOXSHADOW_COLOR;
				return true;
		}
		
		GRPELEMENT* box=this->elements.Get(GRPWIDGETSECTION_BACKGROUND);	
		GRPNODE* node = nodes.Get(GRPWIDGETSECTION_BOXSHADOW);
		if (!node)
		{
				node = scene->CreateNode();
				nodes.Add(GRPWIDGETSECTION_BOXSHADOW, node);
				node->SetParent(this->node);
		}

		float spread				= (float)this->Get(GRPWIDGETSTYLETYPE_BOXSHADOW_SPREAD);
		float blur					= (float)this->Get(GRPWIDGETSTYLETYPE_BOXSHADOW_BLUR);
		float borderwidth		= (float)blur;
		float borderradius = (float)0.2f;

		float w = this->boundingbox.width ;
		float h = this->boundingbox.height;
		
		float x = ((float)this->Get(GRPWIDGETSTYLETYPE_BOXSHADOW_POSITIONX));
		float y = ((float)this->Get(GRPWIDGETSTYLETYPE_BOXSHADOW_POSITIONY));
		
		
		GRPELEMENT* e = GRPOBJECTBUILDER::Get()->CreateShadowBorder(
				w,
				h,
				borderradius,
				borderwidth,
				GRPBOXSIDEFLAG_ALL);

		e->SetNode(node);
		e->GetNode()->SetPosition(x, y, 0.0);

		int find = this->elements.Find(GRPWIDGETSECTION_BACKGROUND);
		
		this->elements.Insert(find,GRPWIDGETSECTION_BOXSHADOW, e);
		ismodified &= ~GRPWIDGETSECTION_FLAG_BOXSHADOW;

		ismodified |= GRPWIDGETSECTION_FLAG_BOXSHADOW_COLOR;		
		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::CreateContent
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	19/01/2018 14:27:47
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::CreateContent()
{
		if (this->elements.Get(GRPWIDGETSECTION_CONTENT) != NULL)
		{
				GRPELEMENT* e = this->elements.Get(GRPWIDGETSECTION_CONTENT);
				e->SetNode(NULL);
				e->SetMesh(NULL);
				this->elements.Delete(GRPWIDGETSECTION_CONTENT);				
				delete(e);
		}

		if (this->materials.Find(GRPWIDGETSECTION_CONTENT) == -1)
		{
				ismodified &= ~GRPWIDGETSECTION_FLAG_CONTENT;
				return true;
		}

		XDEBUG_PRINTCOLOR(4, __L("Creating content for %s:%s"), this->name.Get(), this->tag.Get());

		float borderradius = (float)this->Get(GRPWIDGETSTYLETYPE_BORDER_RADIUS);
		GRPELEMENT* e = GRPOBJECTBUILDER::Get()->CreateBoxRounded(boundingbox.width,
				boundingbox.height,
				borderradius,
				boundingbox.width / texturex,
				-boundingbox.height / texturey,
				(texture_offset_x / texturex),
				(texture_offset_y / texturey));
		/*
		GRPELEMENT* e = GRPOBJECTBUILDER::Get()->CreateBox( boundingbox.width,
																												boundingbox.height,
																												boundingbox.width / texturex,
																												boundingbox.height / texturey);
																												*/
		GRPNODE* node = nodes.Get(GRPWIDGETSECTION_CONTENT);
		if (!node)
		{
				node = scene->CreateNode();
				nodes.Add(GRPWIDGETSECTION_CONTENT, node);
				node->SetParent(this->node);
		}
		e->SetNode(node);
		e->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Diffuse")));
		
		if (materials.Find(GRPWIDGETSECTION_CONTENT) != NOTFOUND)
		{
				e->GetNode()->SetMaterial(materials.Get(GRPWIDGETSECTION_CONTENT));
				e->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Texture")));
				e->GetNode()->GetBaseMaterial()->SetTransparency(1.0f);
		}

		if (this->style.styles.Find(GRPWIDGETSTYLETYPE_OPACITY) != NOTFOUND)
		{
				WIDGETSTRING alpha = this->style.styles.Get(GRPWIDGETSTYLETYPE_OPACITY)->value;
				e->SetBlending(true);
				e->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
				if (e->GetNode()->GetMaterial())
						e->GetNode()->GetMaterial()->GetAttributes()->SetTransparency(alpha.ConvertToFloat());
				e->GetNode()->GetBaseMaterial()->SetTransparency(alpha.ConvertToFloat());
		}

		e->SetBlending(true);

		if (!this->elements.Set(GRPWIDGETSECTION_CONTENT, e))
					this->elements.Add(GRPWIDGETSECTION_CONTENT,e);

		ismodified &= ~GRPWIDGETSECTION_FLAG_CONTENT;

		if (e->parent)
				parent->Align();

		if (parent)
				parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;

		return true;
}


/*-------------------------------------------------------------------
//	GRPWIDGET::CreateText
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/09/2017 18:53:27
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGET::CreateText() 
{ 
		
		if (!this->Get(GRPWIDGETSTYLETYPE_ISTEXT))
		if (ismodified & GRPWIDGETSECTION_FLAG_TEXT_STYLE)
		for (XDWORD e = 0; e < this->widgets.GetSize(); e++)
		{
				if (this->widgets.Get(e)->Get(GRPWIDGETSTYLETYPE_ISTEXT))				
						this->widgets.Get(e)->ismodified |= GRPWIDGETSECTION_FLAG_STYLE;										
		}

		this->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
			
		if (!this->text.GetSize() && !this->content.GetSize())
		{
				ismodified &=  ~GRPWIDGETSECTION_FLAG_TEXT_STYLE;
				ismodified &=  ~GRPWIDGETSECTION_FLAG_TEXT;
				return true;
		}

		
		//-------- has multiple entities of text
		if (!this->Get(GRPWIDGETSTYLETYPE_ISTEXT))
		if (this->text.GetSize())		
		{
						for (int e = this->widgets.GetSize()-1; e>=0; e--)
								if (this->widgets.Get(e)->Get(GRPWIDGETSTYLETYPE_ISTEXT))
								{
										GRPWIDGET* w = this->widgets.Get(e);
										this->widgets.DeleteIndex(e);
										delete(w);
								}						
						
						SplitTextIntoWidgets();
						text = __L("");						
						ismodified &= ~GRPWIDGETSECTION_FLAG_TEXT;

						this->node->Update();
						this->node->UpdateWorldMatrix();
						return true;						
		}
		
		if (!this->Get(GRPWIDGETSTYLETYPE_ISTEXT))
		{
				ismodified &= ~GRPWIDGETSECTION_FLAG_TEXT_STYLE;
				ismodified &= ~GRPWIDGETSECTION_FLAG_TEXT;
				return true;
		}

		content.DeleteNoCharacters(__L(" "),0,XSTRINGCONTEXT_ATFIRST);

		XCHAR* word = this->content.Get();

		if (this->elements.Get(GRPWIDGETSECTION_TEXT))
		{
				GRPTEXTELEMENT* element= dynamic_cast<GRPTEXTELEMENT*>(this->elements.Get(GRPWIDGETSECTION_TEXT));
				delete(element);
				this->elements.Delete(GRPWIDGETSECTION_TEXT);
		}

	//------------

		XDWORD fontsize = (XDWORD)this->Get(GRPWIDGETSTYLETYPE_FONT_SIZE);
		XDWORD fontstyle = (XDWORD)this->Get(GRPWIDGETSTYLETYPE_FONT_STYLE);

	GRPFONT* font = NULL;
					 font = this->FindSuitableFont();
					 if (!font)
							return false;

		//------------

	GLFLOAT scale =  216.0f/64.0f;
					scale *= fontsize;

	GLFLOAT lineheight		= font->GetLineHeight();	
	GLFLOAT	linegap				= font->linegap;
	GLFLOAT	baseline			= font->baseline;
	GLFLOAT descend				= font->descend;
	GLFLOAT ascend				= font->ascend;
	

	GRPNODE* node = nodes.Get(GRPWIDGETSECTION_TEXT);
	if (!node)
	{
			node = scene->CreateNode();
			nodes.Add(GRPWIDGETSECTION_TEXT, node);
			node->SetParent(this->node);
	}	

	float space = font->spaceadvance;

	XDWORD alignment = 0;
		
	GRPTEXTELEMENT* e=font->CreateTextElement	(node, word, 0);
									e->GetNode()->SetPosition	(0.0, ascend*scale, 0.0);
									e->GetNode()->SetScale		(scale, -scale, 1.0);		
									e->CollapseTransforms			();
									e->UpdateBoundingBox			();																			
									e->GetBoundingBox					()->height = lineheight*scale;									
									
	//------------ styles
	
	if (!this->Has(GRPWIDGETSTYLETYPE_WIDTH))
	{
			float w = e->GetBoundingBox()->width;
			if (w > this->boundingbox.width)	this->boundingbox.width = w;	
	}

	this->boundingbox.rightmargin = (float)int(space*scale);
	this->boundingbox.rightmargin += (float)this->Get(GRPWIDGETSTYLETYPE_WORD_SPACING);

	this->boundingbox.leftmargin = (float)this->Get(GRPWIDGETSTYLETYPE_MARGIN_LEFT);

	if (!this->Has(GRPWIDGETSTYLETYPE_HEIGHT))
	{
			float h = (float)e->GetBoundingBox()->height 
								+e->GetBoundingBox()->height*(this->boundingbox.toppadding +	this->boundingbox.bottompadding);											
			this->boundingbox.height = (GLFLOAT)fabs(h);	
	}

	//------------------------------ choose color

		GRPCOLOR* color = GetColor(GRPWIDGETSECTION_TEXT);
		if (color)
		{
		e->SetDefaultColor(*color);
		e->GetNode()->GetBaseMaterial()->SetDiffuse(*color);
		}									
								
		switch (this->Get(GRPWIDGETSTYLETYPE_FONT_DECORATION))
		{
		case GRPWIDGETSTYLE_FONT_DECORATION_UNDERLINE:
				this->ismodified |= GRPWIDGETSECTION_FLAG_BORDER;
				break;
		}
		
		e->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Diffuse")));										

	this->elements.Add(GRPWIDGETSECTION_TEXT,e);	

	if (this->parent)
			this->parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;

	return true;
}


/*-------------------------------------------------------------------
//	 GRPWIDGET::CreateTextShadow
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	19/02/2018 14:52:17
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::CreateTextShadow()
{
		ismodified &= ~GRPWIDGETSECTION_FLAG_TEXTSHADOW;
		if (this->elements.Get(GRPWIDGETSECTION_TEXTSHADOW) != NULL)
		{
				GRPELEMENT* e = this->elements.Get(GRPWIDGETSECTION_TEXTSHADOW);
				e->SetNode(NULL);
				delete(e);
				this->elements.Delete(GRPWIDGETSECTION_TEXTSHADOW);
		}

		GRPELEMENT* text = this->elements.Get(GRPWIDGETSECTION_TEXT);
		if (!text)
		{
				for (XDWORD e = 0; e < this->widgets.GetSize(); e++)
				{
						if (this->widgets.Get(e)->Get(GRPWIDGETSTYLETYPE_ISTEXT))
						{
								this->widgets.Get(e)->CreateTextShadow();
						}
				}
				return false;
		}

		if (!this->parent->Get(GRPWIDGETSTYLETYPE_TEXTSHADOW_POSITIONX))
				return true;

		GRPNODE* node = nodes.Get(GRPWIDGETSECTION_TEXTSHADOW);
		if (!node)
		{
				node = scene->CreateNode();
				nodes.Add(GRPWIDGETSECTION_TEXTSHADOW, node);
				node->SetParent(this->node);
		}
		
		float x = ((float)this->parent->Get(GRPWIDGETSTYLETYPE_TEXTSHADOW_POSITIONX));
		float y = ((float)this->parent->Get(GRPWIDGETSTYLETYPE_TEXTSHADOW_POSITIONY));


		GRPELEMENT* e = new GRPELEMENT();
								e->SetMesh(text->GetMesh());
								e->SetIsInstance(true);
								e->SetNode(node);
								e->GetNode()->SetPosition(x, y, 0.0);

		int find = this->elements.Find(GRPWIDGETSECTION_TEXT);

		this->elements.Insert(find, GRPWIDGETSECTION_TEXTSHADOW, e);
		

		ismodified |= GRPWIDGETSECTION_FLAG_TEXTSHADOW_COLOR;
		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::AssignTextColor
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	09/02/2018 12:27:06
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::AssignTextColor()
{
		GRPCOLOR* color = GetColor(GRPWIDGETSECTION_TEXT);
		if (color)
		for (XDWORD i = 0; i < this->widgets.GetSize(); i++)
		{
				GRPWIDGET* e = widgets.Get(i);
				if (e)
				if (e->Get(GRPWIDGETSTYLETYPE_ISTEXT))
				if (e->elements.Get(GRPWIDGETSECTION_TEXT))
				{
						e->elements.Get(GRPWIDGETSECTION_TEXT)->SetDefaultColor(*color);						
						e->elements.Get(GRPWIDGETSECTION_TEXT)->GetNode()->GetBaseMaterial()->SetDiffuse(*color);
				}
				e->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Diffuse")));			
		}
		ismodified &= ~GRPWIDGETSECTION_FLAG_COLOR;

		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::AssignBackgroundColor
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	09/02/2018 13:33:04
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::AssignBackgroundColor()
{
		GRPELEMENT* e = this->elements.Get(GRPWIDGETSECTION_BACKGROUND);
		if (!e) return true;
		if (colors.Find(GRPWIDGETSECTION_BACKGROUND) != NOTFOUND)
		{
				GRPCOLOR bg = *colors.Get(GRPWIDGETSECTION_BACKGROUND);
				e->SetDefaultColor(bg);
				e->GetNode()->GetBaseMaterial()->SetDiffuse(bg);
				e->GetNode()->GetBaseMaterial()->SetTransparency(bg.alpha);
		}
		e->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Diffuse")));

		if (materials.Find(GRPWIDGETSECTION_BACKGROUND) != NOTFOUND)
		{
				e->GetNode()->SetMaterial(materials.Get(GRPWIDGETSECTION_BACKGROUND));
				e->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Texture")));
		}

		if (this->style.styles.Find(GRPWIDGETSTYLETYPE_OPACITY) != NOTFOUND)
		{
				WIDGETSTRING alpha = this->style.styles.Get(GRPWIDGETSTYLETYPE_OPACITY)->value;
				e->SetBlending(true);
				e->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
				if (e->GetNode()->GetMaterial())
						e->GetNode()->GetMaterial()->GetAttributes()->SetTransparency(alpha.ConvertToFloat());
				e->GetNode()->GetBaseMaterial()->SetTransparency(alpha.ConvertToFloat());
		}

		e->SetBlending(true);
		ismodified &= ~GRPWIDGETSECTION_FLAG_BACKGROUND_COLOR;
		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::AssignBorderColor
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	13/02/2018 14:58:16
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::AssignBorderColor()
{
		GRPELEMENT* e = this->elements.Get(GRPWIDGETSECTION_BORDER);
		if (!e) 
				return true;

		if (GetColor(GRPWIDGETSECTION_BORDER))
		{
				GRPCOLOR bg = *GetColor(GRPWIDGETSECTION_BORDER);
				e->SetDefaultColor(bg);
				e->GetNode()->GetBaseMaterial()->SetDiffuse(bg);
				e->GetNode()->GetBaseMaterial()->SetTransparency(bg.alpha);
		}
		else
		if (GetColor(GRPWIDGETSECTION_TEXT))
		{
				GRPCOLOR bg = *GetColor(GRPWIDGETSECTION_TEXT);
				e->SetDefaultColor(bg);
				e->GetNode()->GetBaseMaterial()->SetDiffuse(bg);
				e->GetNode()->GetBaseMaterial()->SetTransparency(bg.alpha);
		}
		e->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Diffuse")));

		if (materials.Find(GRPWIDGETSECTION_BORDER) != NOTFOUND)
		{
				e->GetNode()->SetMaterial(materials.Get(GRPWIDGETSECTION_BORDER));
				e->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Texture")));
		}

		if (this->style.styles.Find(GRPWIDGETSTYLETYPE_OPACITY) != NOTFOUND)
		{
				WIDGETSTRING alpha = this->style.styles.Get(GRPWIDGETSTYLETYPE_OPACITY)->value;
				e->SetBlending(true);
				e->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
				if (e->GetNode()->GetMaterial())
						e->GetNode()->GetMaterial()->GetAttributes()->SetTransparency(alpha.ConvertToFloat());
				e->GetNode()->GetBaseMaterial()->SetTransparency(alpha.ConvertToFloat());
		}

		e->SetBlending(true);
		ismodified &= ~GRPWIDGETSECTION_FLAG_BORDER_COLOR;
		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::AssignContentColor
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 16:18:13
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::AssignContentColor()
{
		return true;
}


/*-------------------------------------------------------------------
//	 GRPWIDGET::AssignShadowColor
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	19/02/2018 14:55:52
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::AssignShadowColor()
{
		ismodified &= ~GRPWIDGETSECTION_FLAG_TEXTSHADOW_COLOR;

		GRPELEMENT* e = this->elements.Get(GRPWIDGETSECTION_BOXSHADOW);
		if (!e)
				return true;

		if (GetColor(GRPWIDGETSECTION_BOXSHADOW))
		{
				GRPCOLOR bg = *GetColor(GRPWIDGETSECTION_BOXSHADOW);
				e->SetDefaultColor(bg);
				e->GetNode()->GetBaseMaterial()->SetDiffuse(bg);
				e->GetNode()->GetBaseMaterial()->SetTransparency(bg.alpha);
		}
		else
				if (GetColor(GRPWIDGETSECTION_TEXT))
				{
						GRPCOLOR bg = *GetColor(GRPWIDGETSECTION_TEXT);
						e->SetDefaultColor(bg);
						e->GetNode()->GetBaseMaterial()->SetDiffuse(bg);
						e->GetNode()->GetBaseMaterial()->SetTransparency(bg.alpha);
				}
		GRPSHADERPROGRAM* shader = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("2DShadow"));
		e->SetProgram(shader);						
		float factor = (float)this->Get(GRPWIDGETSTYLETYPE_BOXSHADOW_SPREAD);
					factor = (2.0f*factor/(float)this->Get(GRPWIDGETSTYLETYPE_BOXSHADOW_BLUR))-1.0f;
		GRPSHADER::UseShader(shader);
		shader->Set("gradient_factor", factor);

		e->SetBlending(true);
		ismodified &= ~GRPWIDGETSECTION_FLAG_BOXSHADOW_COLOR;
		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::AssignTextShadowColor
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	20/02/2018 11:38:58
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::AssignTextShadowColor()
{
		ismodified &= ~GRPWIDGETSECTION_FLAG_TEXTSHADOW_COLOR;

		GRPELEMENT* e = this->elements.Get(GRPWIDGETSECTION_TEXTSHADOW);
		if (!e)
				return true;

		if (GetColor(GRPWIDGETSECTION_TEXTSHADOW))
		{
				GRPCOLOR bg = *GetColor(GRPWIDGETSECTION_TEXTSHADOW);
				e->SetDefaultColor(bg);
				e->GetNode()->GetBaseMaterial()->SetDiffuse(bg);
				e->GetNode()->GetBaseMaterial()->SetTransparency(bg.alpha);
		}
		else
				if (GetColor(GRPWIDGETSECTION_TEXT))
				{
						GRPCOLOR bg = *GetColor(GRPWIDGETSECTION_TEXT);
						e->SetDefaultColor(bg);
						e->GetNode()->GetBaseMaterial()->SetDiffuse(bg);
						e->GetNode()->GetBaseMaterial()->SetTransparency(bg.alpha/2.0f);
				}
		GRPSHADERPROGRAM* shader = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Diffuse"));
		e->SetProgram(shader);		
		e->SetBlending(true);
		
		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::AssignOutlineColor
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 15:56:47
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::AssignOutlineColor()
{
		GRPELEMENT* e = this->elements.Get(GRPWIDGETSECTION_OUTLINE);
		if (!e)
		{
				ismodified &= ~GRPWIDGETSECTION_FLAG_OUTLINE_COLOR;
				return true;
		}

		if (GetColor(GRPWIDGETSECTION_OUTLINE))
		{
				GRPCOLOR bg = *GetColor(GRPWIDGETSECTION_OUTLINE);
				e->SetDefaultColor(bg);
				e->GetNode()->GetBaseMaterial()->SetDiffuse(bg);
				e->GetNode()->GetBaseMaterial()->SetTransparency(bg.alpha);
		}
		else
				if (GetColor(GRPWIDGETSECTION_TEXT))
				{
						GRPCOLOR bg = *GetColor(GRPWIDGETSECTION_TEXT);
						e->SetDefaultColor(bg);
						e->GetNode()->GetBaseMaterial()->SetDiffuse(bg);
						e->GetNode()->GetBaseMaterial()->SetTransparency(bg.alpha / 2.0f);
				}
		GRPSHADERPROGRAM* shader = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Diffuse"));
		e->SetProgram(shader);
		e->SetBlending(true);
		ismodified &= ~GRPWIDGETSECTION_FLAG_OUTLINE_COLOR;
		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::FindSuitableFont
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	30/01/2018 11:00:22
//	@return		GRPFONT* : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
GRPFONT* GRPWIDGET::FindSuitableFont()
{
		GRPFONT* font = NULL;

		WIDGETSTRING fontname;
		int start = 0;

		GRPFONTATTRIBUTES		att;
												att.front = true;
												att.quality = 4;
												att.extrude = 0.0f;

		XDWORD fontstyle = this->Get(GRPWIDGETSTYLETYPE_FONT_STYLE);		
		WIDGETSTRING family = this->fontfamily;

		GRPWIDGET* w = this;
		while (!family.GetSize())
		{
				if (w->parent)
				{
						family = w->parent->fontfamily;
						w = w->parent;
				}
				else
						break;
		}

		if (!family.GetSize())
				return NULL;
		
		XVECTOR<WIDGETSTRING*> families;
		family.Explode(__C(','), &families);


		while (font == NULL)
		{
				if (start >= (int)family.GetSize())						
				{
								XDEBUG_PRINTCOLOR(4, __L("Unable to resolve font-families: %s"), family.Get());
								font = GRPOBJECTBUILDER::Get()->GetFontManager()->GetFonts()->GetElement(0);
								if (!font)
								{
										families.DeleteContents();
										families.DeleteAll();
										return NULL;
								}
				}

				int end = family.FindCharacter(__C(','), start);
				if (end == -1)
						end = family.GetSize();

				family.Copy(start, end, fontname);
				start = end + 1;

				GRPFONT_STYLE style = (GRPFONT_STYLE)fontstyle;
				font = GRPOBJECTBUILDER::Get()->GetFontManager()->GetFontByFamily(fontname.Get(), style);				

		}
		


		if (font) //tenemos esa familia					
						font->SetAttributes(&att);

		families.DeleteContents();
		families.DeleteAll();

		return font;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::SplitTextIntoWidgets
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	19/02/2018 11:15:08
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::SplitTextIntoWidgets()
{		
				
		XVECTOR<WIDGETSTRING*> words;
		if (Get(GRPWIDGETSTYLETYPE_WHITESPACE) == GRPWIDGETSTYLE_WHITESPACE_NORMAL)
				text.Explode(__C(' '), &words);
		else
				words.Add(new WIDGETSTRING(text));

		for (XDWORD e = 0; e < words.GetSize(); e++)
		{
		GRPWIDGET*	word = this->page->Create(__L("anonymous_text"),words.Get(e)->Get(), __L("anonymous_text"), __L(""), words.Get(e)->Get());
								word->Set(GRPWIDGETSTYLETYPE_ISTEXT, 1);

								if (e == 0)
										if (this->Get(GRPWIDGETSTYLETYPE_FONT_INDENT))
												word->Set(GRPWIDGETSTYLETYPE_MARGIN_LEFT,this->Get(GRPWIDGETSTYLETYPE_FONT_INDENT));

								word->ismodified |= GRPWIDGETSECTION_FLAG_TEXT;
								word->ismodified |= GRPWIDGETSECTION_FLAG_TEXT_STYLE;
		this->Add(word);
		word->aligner = new GRPWIDGETALIGNINLINE();
		word->node->Update();
		word->node->UpdateWorldMatrix();
		
		}
		words.DeleteContents();
		words.DeleteAll();	
		this->Normalize();	
		return true;
}


/*-------------------------------------------------------------------
//	GRPWIDGET::Add
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2017 14:24:22
//	
//	@return 			bool : 
//
//  @param				child : 
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGET::Add(GRPWIDGET* child)
{
	if (child->parent)
		child->parent->widgets.Delete(child);

	child->parent=this;

	if (child->node)
	child->node->SetParent(this->node);
		
	this->widgets.Add(child);

	child->root = this->root;
	child->Create();
	
	if (parent)
  parent->ismodified|= GRPWIDGETSECTION_FLAG_FLOW;
								
return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::Normalize
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	19/01/2018 14:34:02
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::Normalize()
{
		int nInline=0;
		int nBlock =0;
		GRPWIDGETSTYLE_DISPLAY_TYPE owndisplay= (GRPWIDGETSTYLE_DISPLAY_TYPE)this->Get(GRPWIDGETSTYLETYPE_DISPLAY);

		//------------------- table

		if (owndisplay == GRPWIDGETSTYLE_DISPLAY_TYPE_TABLE)
		{
				if (this->aligner)
						delete(this->aligner);
				this->aligner = new GRPWIDGETALIGNTABLE();

				for (XDWORD e = 0; e < this->widgets.GetSize(); e++)
						this->widgets.Get(e)->Normalize();

				return true;
		}

		//------------------- rows
		if (owndisplay == GRPWIDGETSTYLE_DISPLAY_TYPE_TABLE_ROW)
		{
				if (this->aligner)
						delete(this->aligner);
				this->aligner = new GRPWIDGETALIGNTABLEROW();

				for (XDWORD e = 0; e < this->widgets.GetSize(); e++)
						this->widgets.Get(e)->Normalize();

				return true;
		}

		//------------------- rows
		if (owndisplay == GRPWIDGETSTYLE_DISPLAY_TYPE_TABLE_CELL)
		{
				if (this->aligner)
						delete(this->aligner);
				this->aligner = new GRPWIDGETALIGNINLINE();

				for (XDWORD e = 0; e < this->widgets.GetSize(); e++)
						this->widgets.Get(e)->Normalize();

				return true;
		}
		//------------------- others
		
				for (XDWORD e = 0; e < this->widgets.GetSize(); e++)
				{
						GRPWIDGET*									widget = this->widgets.Get(e);
						GRPWIDGETSTYLE_DISPLAY_TYPE display = (GRPWIDGETSTYLE_DISPLAY_TYPE)widget->Get(GRPWIDGETSTYLETYPE_DISPLAY);
						switch (display)
						{
						case GRPWIDGETSTYLE_DISPLAY_TYPE_HIDDEN: break;
						case GRPWIDGETSTYLE_DISPLAY_TYPE_INITIAL:
						case GRPWIDGETSTYLE_DISPLAY_TYPE_STATIC:
						case GRPWIDGETSTYLE_DISPLAY_TYPE_INLINE:
						case GRPWIDGETSTYLE_DISPLAY_TYPE_INLINEBLOCK: nInline++; break;

						case GRPWIDGETSTYLE_DISPLAY_TYPE_LIST_ITEM:
						case GRPWIDGETSTYLE_DISPLAY_TYPE_BLOCK: nBlock++; break;
						default:
								break;
						}
				}

				if (nBlock == 0)
				{
						if (this->aligner)
								delete(this->aligner);
						this->aligner = new GRPWIDGETALIGNINLINE();
				}
				else
				{
						if (this->aligner)
								delete(this->aligner);
						this->aligner = new GRPWIDGETALIGNBLOCK();

						XVECTOR<GRPWIDGET*> block;

						if (nInline)
								for (XDWORD e = 0; e < this->widgets.GetSize(); e++)
								{
										GRPWIDGET* child = this->widgets.Get(e);
										int position = e;
										block.DeleteAll();

										while (child->Get(GRPWIDGETSTYLETYPE_DISPLAY) != GRPWIDGETSTYLE_DISPLAY_TYPE_BLOCK)
										{
												block.Add(this->widgets.Get(e));
												e++;
												if (e >= this->widgets.GetSize())
														break;
												child = this->widgets.Get(e);
										}

										if (block.GetSize())
										{
												GRPWIDGET* anonymous_holder = new GRPWIDGET();
												anonymous_holder->page = this->page;
												anonymous_holder->node = this->node;

												anonymous_holder->Set(GRPWIDGETSTYLETYPE_DISPLAY, GRPWIDGETSTYLE_DISPLAY_TYPE_BLOCK);
												anonymous_holder->Set(GRPWIDGETSTYLETYPE_FONT_DECORATION, GRPWIDGET_STYLE_INHERIT);

												anonymous_holder->SetName(__L("anonymousholder"));
												this->widgets.Insert(position, anonymous_holder);

												anonymous_holder->node = scene->CreateNode();
												anonymous_holder->parent = this;
												anonymous_holder->node->SetParent(this->node);
												anonymous_holder->root = this->root;

												for (XDWORD i = 0; i < block.GetSize(); i++)
														anonymous_holder->Add(block.Get(i));

												block.DeleteAll();
										}
								}
				}
		
		//cascade
		for (XDWORD e = 0; e < this->widgets.GetSize(); e++)		
				this->widgets.Get(e)->Normalize();
		

		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::Align
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	18/01/2018 16:54:54
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::Align()
{
		do
		{				
				for (XDWORD e = 0; e < this->widgets.GetSize(); e++)
				{
						GRPWIDGET* widget = this->widgets.Get(e);
						widget->Align();
				}
				
				if (ismodified & GRPWIDGETSECTION_FLAG_STYLE)						ApplyStyles					 ();

 				if (ismodified & GRPWIDGETSECTION_FLAG_TEXT_CHANGED)		CreateSection		(GRPWIDGETSECTION_TEXT);
				if (ismodified & GRPWIDGETSECTION_FLAG_COLOR)						AssignColor			(GRPWIDGETSECTION_TEXT);

				if (ismodified & GRPWIDGETSECTION_FLAG_BACKGROUND)			CreateSection		(GRPWIDGETSECTION_BACKGROUND);
				if (ismodified & GRPWIDGETSECTION_FLAG_BACKGROUND_COLOR)AssignColor			(GRPWIDGETSECTION_BACKGROUND);

				if (ismodified & GRPWIDGETSECTION_FLAG_BOXSHADOW)				CreateSection		(GRPWIDGETSECTION_BOXSHADOW);
				if (ismodified & GRPWIDGETSECTION_FLAG_BOXSHADOW_COLOR)	AssignColor			(GRPWIDGETSECTION_BOXSHADOW);;
				
				if (ismodified & GRPWIDGETSECTION_FLAG_BORDER)					CreateSection		(GRPWIDGETSECTION_BORDER);
				if (ismodified & GRPWIDGETSECTION_FLAG_BORDER_COLOR)		AssignColor			(GRPWIDGETSECTION_BORDER);

				if (ismodified & GRPWIDGETSECTION_FLAG_OUTLINE)					CreateSection		(GRPWIDGETSECTION_OUTLINE);
				if (ismodified & GRPWIDGETSECTION_FLAG_OUTLINE_COLOR)		AssignColor			(GRPWIDGETSECTION_OUTLINE);

				if (ismodified & GRPWIDGETSECTION_FLAG_CONTENT)					CreateSection		(GRPWIDGETSECTION_CONTENT);
				if (ismodified & GRPWIDGETSECTION_FLAG_CONTENT_COLOR)		AssignColor			(GRPWIDGETSECTION_CONTENT);

				if (ismodified & GRPWIDGETSECTION_FLAG_TEXTSHADOW)			CreateSection		(GRPWIDGETSECTION_TEXTSHADOW);
				if (ismodified & GRPWIDGETSECTION_FLAG_TEXTSHADOW_COLOR)AssignColor			(GRPWIDGETSECTION_TEXTSHADOW);
										
				if (ismodified & GRPWIDGETSECTION_FLAG_FLOW)
						this->aligner->Align(this);
			
		} while (ismodified);		
		return true;

}

/*-------------------------------------------------------------------
//	 GRPWIDGETRENDERERBLOCK::Align
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	19/01/2018 14:59:52
//	@return		bool : 
//
//	@param		GRPWIDGET* : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETALIGNBLOCK::Align(GRPWIDGET* w)
{		
		return w->AlignBlock();			
}


/*-------------------------------------------------------------------
//	 GRPWIDGETALIGNINLINE::Align
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/01/2018 12:22:31
//	@return		bool : 
//
//	@param		GRPWIDGET* : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETALIGNINLINE::Align(GRPWIDGET* w)
{
		return w->AlignInline();		
}

/*-------------------------------------------------------------------
//	 GRPWIDGETALIGNTABLE::Align
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	02/02/2018 10:34:11
//	@return		bool : 
//
//	@param		GRPWIDGET* : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETALIGNTABLE::Align(GRPWIDGET* w)
{
		return w->AlignTable();
}


/*-------------------------------------------------------------------
//	 GRPWIDGETALIGNTABLEROW::Align
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	06/03/2018 16:22:53
//	@return		bool : 
//
//	@param		GRPWIDGET* : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETALIGNTABLEROW::Align(GRPWIDGET* w)
{
		w->ismodified &= ~GRPWIDGETSECTION_FLAG_FLOW;
		return true;
}
/*-------------------------------------------------------------------
//	 GRPWIDGETRENDERERLIST::Align
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	02/02/2018 10:39:16
//	@return		bool : 
//
//	@param		GRPWIDGET* : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETALIGNLIST::Align(GRPWIDGET* w)
{
		return w->AlignTable();
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::GetHeight
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	19/01/2018 13:13:04
//	@return		XDWORD : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
float GRPWIDGET::GetHeight()
{
		return float(
				this->boundingbox.height + 
				this->boundingbox.topmargin + 
				this->boundingbox.bottommargin+
				this->boundingbox.bottomborder +
				this->boundingbox.topborder
				);
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::GetWidth
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	19/01/2018 13:12:59
//	@return		XDWORD : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
float GRPWIDGET::GetWidth()
{
		return float(
						this->boundingbox.width + 
						this->boundingbox.leftmargin + 
						this->boundingbox.rightmargin + 
						this->boundingbox.rightborder + 
						this->boundingbox.leftborder
				);
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::GetClientHeight
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	19/01/2018 13:09:46
//	@return		XDWORD : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
float GRPWIDGET::GetClientHeight()
{
		return float(this->boundingbox.height - this->boundingbox.toppadding - this->boundingbox.bottompadding);
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::GetClientWidth
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	19/01/2018 13:09:52
//	@return		XDWORD : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
float GRPWIDGET::GetClientWidth()
{		
		return float(this->boundingbox.width - this->boundingbox.leftpadding - this->boundingbox.rightpadding);
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::AlignBlock
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	19/01/2018 13:09:58
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::AlignBlock()
{
//---------------- Order elements acording to float

ismodified &=~GRPWIDGETSECTION_FLAG_FLOW;

GRPPOINT		Caret;
						Caret.x += this->boundingbox.leftpadding;
						Caret.y += this->boundingbox.toppadding;
						Caret.z = 0.0;

XDWORD			maxwidth = (XDWORD)GetClientWidth();
XDWORD			baseline = 0;
int					x = 0,
						y = 0,
						z = 0;

XDWORD			processedwidgets = 0;

bool				finished = false;
float				maxwidgetwidth = 0;

		for (XDWORD i = 0; i <widgets.GetSize(); i++)
		{
				GRPWIDGET* child = widgets.Get(i);

				if (child->GetWidth() > maxwidgetwidth)
						maxwidgetwidth = child->GetWidth();

				x= (int)this->boundingbox.leftpadding;

						//------------- is visible   //should be just removed from the render queue
				switch (child->Get(GRPWIDGETSTYLETYPE_DISPLAY))
				{
						case GRPWIDGETSTYLE_DISPLAY_TYPE_HIDDEN: continue;
				}				

				y = 0;

				switch (child->Get(GRPWIDGETSTYLETYPE_FLOAT))
				{
						case GRPWIDGETSTYLE_FLOAT_TYPE_RIGHT:
										x = int(parent->GetClientWidth()-child->position.vector[0]-child->GetWidth());
										break;

						default:
						case GRPWIDGETSTYLE_FLOAT_TYPE_LEFT:
										x = int(Caret.x + child->boundingbox.leftmargin);
										break;
				}
						x += XDWORD(child->boundingbox.leftborder);												
						y += XDWORD(child->boundingbox.topborder);				
				
				float z = 0.0f;
				if (child->Get(GRPWIDGETSTYLETYPE_POSITION) != GRPWIDGETSTYLE_POSITION_TYPE_STATIC)
				{
						if (child->Has(GRPWIDGETSTYLETYPE_LEFT))
								x += child->Get(GRPWIDGETSTYLETYPE_LEFT);//(child->position.vector[0]);			
						
						if (child->Has(GRPWIDGETSTYLETYPE_TOP))
								y += child->Get(GRPWIDGETSTYLETYPE_TOP);//XDWORD(child->position.vector[1]);								
				}

				if (child->Get(GRPWIDGETSTYLETYPE_POSITION) != GRPWIDGETSTYLE_POSITION_TYPE_ABSOLUTE)
				{
						y += XDWORD(Caret.y);
						Caret.y += child->GetHeight();
						y += XDWORD(child->boundingbox.topmargin - child->boundingbox.bottommargin);
				}

				

				child->node->SetPosition(float(x), float(y), z);
				child->node->Update();	
				child->node->SetVisible(child->Get(GRPWIDGETSTYLETYPE_VISIBILITY) != GRPWIDGETSTYLE_VISIBILITY_HIDDEN);

				baseline = 0;				
		}

		//flex
		if (Caret.y > this->GetHeight())
		{
				if (this->NotifyHeight(Caret.y))
				this->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
		}

		//flex
		if (maxwidgetwidth > this->GetWidth())
		{
						if (NotifyWidth(maxwidgetwidth))
						this->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
		}
	

return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::AlignInline
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	19/01/2018 15:18:50
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::AlignInline()
{

		ismodified &= ~GRPWIDGETSECTION_FLAG_FLOW;

		GRPPOINT		Caret;
								Caret.x						+= this->boundingbox.leftpadding;
								Caret.y						+= this->boundingbox.toppadding;

		XDWORD			maxwidth = 0;
		XDWORD			baseline					= 0;
		XDWORD			x									= 0, 
								y									= 0;

		int					start							= 0;
		int					end								= this->widgets.GetSize();

		bool				finished					= false;		
		XDWORD			processedwidgets	= 0;
		
		GLFLOAT widgetwidth = 0;

		maxwidth = Get(GRPWIDGETSTYLETYPE_WIDTH);
		maxwidth -= (XDWORD)this->boundingbox.leftpadding;
		maxwidth -= (XDWORD)this->boundingbox.rightpadding;
		
		while (processedwidgets < this->widgets.GetSize())
		{
				baseline		= 0;
				end					= this->widgets.GetSize()-1;
				x						= 0;

				GLFLOAT lineheightmargin = 0;

				for (int e = start; e <= end; e++)
				{
						GRPWIDGET*	child		= widgets.Get(e);
						XDWORD			h				= (XDWORD)child->GetHeight();

						XDWORD lineheight = child->Get(GRPWIDGETSTYLETYPE_FONT_LINE_HEIGHT);
						if (lineheight > h)
						{
								lineheightmargin = (GLFLOAT)(lineheight - h);
								h = lineheight;								
								lineheightmargin /= 2;
						}

						if (child->Get(GRPWIDGETSTYLETYPE_POSITION) == GRPWIDGETSTYLE_POSITION_TYPE_ABSOLUTE)
								continue;

								XDWORD w = XDWORD(child->GetWidth() + child->position.vector[0]);							

								widgetwidth+=w;								

								if (widgetwidth > maxwidth && x != 0)
								{
										end = e - 1;
										if (end < start)
												end = start;
										break;
								}
								x += w;
				
						if (h > baseline)
								baseline = (XDWORD)h;
				}
				
				float excess = (GLFLOAT)(maxwidth - x);
				float indent	= 0;
				float justify = 0;										
								
				switch (Get(GRPWIDGETSTYLETYPE_TEXT_ALIGN))
				{
				case GRPWIDGETSTYLETYPE_TEXT_ALIGN_RIGHT	:  indent = excess;								  	break;
				case GRPWIDGETSTYLETYPE_TEXT_ALIGN_CENTER	:  indent = excess / 2.0f;						break;
				case GRPWIDGETSTYLETYPE_TEXT_ALIGN_LEFT		:																			break;
				case GRPWIDGETSTYLETYPE_TEXT_ALIGN_JUSTIFY:  
						if ((end-start)>2)
								justify= excess / float(end - start);
						break;
				}				

				widgetwidth = 0;

				//--
				Caret.y += baseline;



				for (int e = start; e <= end; e++)
				{
						GRPWIDGET* child = widgets.Get(e);		

						float h = float(child->GetHeight());
						float w = float(child->GetWidth() + child->position.vector[0]);

						int istext = child->Get(GRPWIDGETSTYLETYPE_ISTEXT);	
						if (istext)						
								if (e == start) 
										Caret.x += indent;							
						

						float aligny = 0;

						switch (child->Get(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN))
						{						
						case GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_MIDDLE:	aligny = Caret.y + (h - baseline)/2.0f;	break;
						case GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_TOP:			aligny = Caret.y- baseline +h;					break;
						case GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_BOTTOM:
						case GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_BASELINE:aligny = Caret.y-h;											break;
						}						
						float x = child->boundingbox.leftborder + child->boundingbox.leftmargin;
						float y = child->boundingbox.topborder + child->boundingbox.topmargin;

						if (child->Get(GRPWIDGETSTYLETYPE_POSITION) == GRPWIDGETSTYLE_POSITION_TYPE_RELATIVE)
						{
								x += float(Caret.x);
								y += float(aligny
										- child->position.vector[1]																		
										- lineheightmargin
										);
						}

						if (child->Get(GRPWIDGETSTYLETYPE_POSITION) != GRPWIDGETSTYLE_POSITION_TYPE_STATIC)
						{
								if (child->Has(GRPWIDGETSTYLETYPE_LEFT))
										x += child->Get(GRPWIDGETSTYLETYPE_LEFT);//(child->position.vector[0]);			

								if (child->Has(GRPWIDGETSTYLETYPE_TOP))
										y += child->Get(GRPWIDGETSTYLETYPE_TOP);//XDWORD(child->position.vector[1]);								
						}

						if (child->Get(GRPWIDGETSTYLETYPE_POSITION) != GRPWIDGETSTYLE_POSITION_TYPE_ABSOLUTE)
						{						
								Caret.x += child->GetWidth();
								if (end<int(widgets.GetSize() - 1)) //last line is not justified
										Caret.x += istext*justify;
						}

						child->node->SetPosition((float)x, (float)y, 0.0f);
						child->node->Update();
						child->node->SetVisible(child->Get(GRPWIDGETSTYLETYPE_VISIBILITY)!= GRPWIDGETSTYLE_VISIBILITY_HIDDEN);
					
						processedwidgets++;
				}
				start = end + 1;
				if (start >= (int)widgets.GetSize())
						break;

				Caret.x = this->boundingbox.leftpadding;			
		}
		

		if ((Caret.y-this->boundingbox.bottompadding) > this->GetHeight())
		{
				if (!this->Has(GRPWIDGETSTYLETYPE_HEIGHT))
				{											
						if (NotifyHeight(Caret.y - this->boundingbox.bottompadding))
						this->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
				}
		}

		if ((Caret.x - this->boundingbox.rightpadding) > this->GetWidth())
		{
				if (!this->Has(GRPWIDGETSTYLETYPE_WIDTH))
				{
						if (NotifyWidth(Caret.x - this->boundingbox.rightpadding))
						this->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
				}
		}

		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::AlignTable
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	02/02/2018 10:23:53
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::AlignTable()
{
		//---------------- Order elements acording to float

		ismodified &= ~GRPWIDGETSECTION_FLAG_FLOW;

		GRPPOINT		Caret;
		Caret.x += this->boundingbox.leftpadding;
		Caret.y += this->boundingbox.toppadding;
		Caret.z = 0.0;

		XDWORD			maxwidth  = (XDWORD)GetClientWidth();
		XDWORD			maxheight = (XDWORD)GetClientHeight();
		XDWORD			baseline  = 0;
		int					x = 0 , y = 0, z = 0;

		int					height = 0;
		int					nwidgets = widgets.GetSize();

		colwidth.DeleteAll();		
		rowheight.DeleteAll();
		
		//------ get col widths
		for (XDWORD rows = 0; rows < widgets.GetSize(); rows++) //only table rows should be inside a table
		{
				GRPWIDGET* row = widgets.Get(rows);
				if (rowheight.GetSize() <= rows)				
						rowheight.Add(-1);


				if (row->Has(GRPWIDGETSTYLETYPE_HEIGHT))
				{
						XDWORD height= (XDWORD)row->GetHeight();
						int fixed_height = rowheight.Get(rows);
						if (int(height) > fixed_height)
								rowheight.Set(rows, height);
				}

				for (XDWORD cols = 0,nrealcols=0; cols < row->widgets.GetSize(); cols++)
				{						
						GRPWIDGET* col = row->widgets.Get(cols);
						nrealcols += col->Get(GRPWIDGETSTYLETYPE_TABLE_COLSPAN)+1;

						while (colwidth.GetSize() < nrealcols)
								colwidth.Add(-1);

						if (col->Has(GRPWIDGETSTYLETYPE_WIDTH))
						{
								XDWORD width = (XDWORD)col->GetWidth();								
								int fixed_width = colwidth.Get(cols);
								if (int(width) > fixed_width)
										colwidth.Set(cols, width);
						}						
				}
		}

		//---------- now we have the fixed widths , fill with rest of widths
		int used_width = this->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_X);;
		int nfixedcols = colwidth.GetSize();

		for (XDWORD e = 0; e < colwidth.GetSize(); e++)
		{
				if (colwidth.Get(e) != -1)
				{
					used_width += colwidth.Get(e);
					nfixedcols--;
				}
				used_width+= this->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_X);
		}

		if (used_width > this->GetClientWidth())
		{
				return NotifyWidth(float(used_width));
		}
		else		
		if (nfixedcols)
		{
				int rest_width = int(this->GetClientWidth()) - used_width;
				int perColWidth = rest_width / nfixedcols;
				for (XDWORD e = 0; e < colwidth.GetSize(); e++)				
						if (colwidth.Get(e) == -1)
								colwidth.Set(e, perColWidth);
				
		}

		//---------- assign the height spacing
		height = this->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_Y);
		for (XDWORD i = 0; i < widgets.GetSize(); i++)
		{
				GRPWIDGET* w = widgets.Get(i);
				if (w->Has(GRPWIDGETSTYLETYPE_HEIGHT))
				{
						height += w->Get(GRPWIDGETSTYLETYPE_HEIGHT);
						nwidgets--;
				}
				height += w->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_Y);
		}
		if (nwidgets)
		{
				XDWORD height_per_widget = (maxheight - height) / nwidgets;
				for (XDWORD i = 0; i < widgets.GetSize(); i++)
				{
						GRPWIDGET* w = widgets.Get(i);
						float newh = float(height_per_widget);
						if (!w->Has(GRPWIDGETSTYLETYPE_HEIGHT))
								w->NotifyHeight((float)newh);
				}
		}

		//---------- now we have the fixed height , fill with rest of height
		int used_height = this->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_Y);;
		int nfixedrows = rowheight.GetSize();

		for (XDWORD e = 0; e < rowheight.GetSize(); e++)
		{
				if (rowheight.Get(e) != -1)
				{
						used_height += rowheight.Get(e);
						nfixedrows--;
				}
				used_height += this->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_Y);
		}

		if (used_height > this->GetClientHeight())
		{
				return NotifyHeight(float(used_height));
		}
		else
				if (nfixedrows)
				{
						int rest_height = int(this->GetClientHeight()) - used_height;
						int perRowWidth = rest_height / nfixedrows;
						for (XDWORD e = 0; e < rowheight.GetSize(); e++)
								if (rowheight.Get(e) == -1)
										rowheight.Set(e, perRowWidth);

				}

		//------------------ detect span margins

				XMAP<XPAIR<XDWORD, XDWORD>, bool> spancells;

				for (XDWORD y = 0; y < rowheight.GetSize(); y++)
				{
						GRPWIDGET* row = this->widgets.Get(y);
						XDWORD advance = 0;
						for (XDWORD x = 0; x < colwidth.GetSize(); x++)
						{
								XDWORD i = 0;
								while (spancells.Get(XPAIR<XDWORD, XDWORD>(x + i, y)))
										i++;

								spancells.Add(XPAIR<XDWORD,XDWORD>(x + i, y), false);
								GRPWIDGET* col = row->widgets.Get(x-advance);
								if (col)
								{

										int rowspan = col->Get(GRPWIDGETSTYLETYPE_TABLE_ROWSPAN);
										for (int e = 1; e <= rowspan && e < (int)rowheight.GetSize(); e++)
												spancells.Add(XPAIR<XDWORD, XDWORD>(x + i, y + e), true);

										int colspan = col->Get(GRPWIDGETSTYLETYPE_TABLE_COLSPAN);
										x += colspan;
										advance += colspan;
								}
								else
										break;
						}							
				}

		//=============================iterate rows

		XDWORD			processedwidgets = 0;

		bool				finished = false;
		float				maxwidgetwidth = 0;

		y = this->Get(GRPWIDGETSTYLETYPE_PADDING_TOP);
		for (XDWORD _row = 0; _row <widgets.GetSize(); _row++) //a table only contains rows
		{
				GRPWIDGET* row = widgets.Get(_row);

				if (row->GetWidth() > maxwidgetwidth)
						maxwidgetwidth = row->GetWidth();				

				x = this->Get(GRPWIDGETSTYLETYPE_PADDING_LEFT);
				x += row->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_X);
				y += row->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_Y);

				row->node->SetPosition(float(x), float(y), float(z));
	
				x = 0;
			
				float rowx = 0;
				float rowy = 0;


				//we calculate the x and y of the child rows				
				for (XDWORD col = 0; col < row->widgets.GetSize(); col++)
				{
						
						GRPWIDGET* cell = row->widgets.Get(col);
						
						int i = 0;
						while (spancells.Get(XPAIR<XDWORD, XDWORD>(col + i, _row)))
						{
								rowx += colwidth.Get(col + i);
								rowx += row->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_X);
								i++;
						}

						float cellwidth = float(colwidth.Get(col + i));
						int colspan = cell->Get(GRPWIDGETSTYLETYPE_TABLE_COLSPAN)+1;
						for (int e=1; (col + e + i)<(int)colwidth.GetSize() && e<colspan;e++)
								  cellwidth  += float(colwidth.Get(col+e+i)) + row->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_X);

						float cellheight = float(rowheight.Get(_row));
						int rowspan = cell->Get(GRPWIDGETSTYLETYPE_TABLE_ROWSPAN)+1;
						for (int e = 1; e<(int)rowheight.GetSize() && e<rowspan; e++)
								cellheight += float(rowheight.Get(_row + e)) + row->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_Y);

						cell->NotifyWidth(cellwidth);
						cell->NotifyHeight(cellheight);						
						
						cell->node->SetPosition(float(rowx), float(rowy), float(z));
						cell->node->Update();						
						cell->ismodified&= ~GRPWIDGETSECTION_FLAG_FLOW;
						rowx += int(cellwidth) + row->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_X);
				}				

				y += rowheight.Get(_row);
				baseline = 0;
		}

		//flex
		if (Caret.y > this->GetHeight())
		{
				if (this->NotifyHeight(Caret.y))
						this->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
		}

		//flex
		if (maxwidgetwidth > this->GetWidth())
		{
				if (NotifyWidth(maxwidgetwidth))
						this->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
		}
		
		spancells.DeleteAll();
		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::AlignTableRow
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	06/03/2018 16:19:02
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::AlignTableRow()
{

		ismodified &= ~GRPWIDGETSECTION_FLAG_FLOW;

		GRPPOINT		Caret;
		Caret.x += this->boundingbox.leftpadding;
		Caret.y += this->boundingbox.toppadding;

		XDWORD			maxwidth  = 0;	
		XDWORD			maxheight = 0;
		XDWORD			baseline  = 0;
		XDWORD			x = 0,y   = 0;

		maxwidth = Get(GRPWIDGETSTYLETYPE_WIDTH);
		maxwidth -= (XDWORD)this->boundingbox.leftpadding;
		maxwidth -= (XDWORD)this->boundingbox.rightpadding;
		
		maxheight = Get(GRPWIDGETSTYLETYPE_HEIGHT);

		int					width = 0;
		int					nwidgets = widgets.GetSize();


		XDWORD rowindex = parent->widgets.Find(this);
		XDWORD colindex = 0;

		for (XDWORD i = 0; i < widgets.GetSize(); i++)
		{
				GRPWIDGET* w = widgets.Get(i);
								
				int colspan = w->Get(GRPWIDGETSTYLETYPE_TABLE_COLSPAN)+1;
								
				width = parent->colwidth.Get(colindex);
				for (int e = 1; e < colspan && (colindex +e)<parent->colwidth.GetSize(); e++)
						width += parent->colwidth.Get(colindex + e) + parent->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_X);

				colindex += colspan;

				w->NotifyWidth((float)width);
				
				if (!w->Has(GRPWIDGETSTYLETYPE_HEIGHT))
				{
						float h = float(maxheight);						
						h -= w->GetHeight();															
						w->NotifyHeight(w->boundingbox.height + h);
				}
				
		}

		int					start = 0;
		int					end = this->widgets.GetSize();

		bool				finished = false;
		XDWORD			processedwidgets = 0;

		GLFLOAT widgetwidth = 0;


		while (processedwidgets < this->widgets.GetSize())
		{
				baseline = 0;
				end = this->widgets.GetSize() - 1;
				x = 0;

				GLFLOAT lineheightmargin = 0;

				for (int e = start; e <= end; e++)
				{
						GRPWIDGET*	child = widgets.Get(e);
						XDWORD			h = (XDWORD)child->GetHeight();

						XDWORD lineheight = child->Get(GRPWIDGETSTYLETYPE_FONT_LINE_HEIGHT);
						if (lineheight > h)
						{
								lineheightmargin = (GLFLOAT)(lineheight - h);
								h = lineheight;
								lineheightmargin /= 2;
						}

						if (child->Get(GRPWIDGETSTYLETYPE_POSITION) == GRPWIDGETSTYLE_POSITION_TYPE_ABSOLUTE)
								continue;

						XDWORD w = XDWORD(child->GetWidth() + child->position.vector[0]);

						widgetwidth += w;

						if (widgetwidth > maxwidth && x != 0)
						{
								end = e - 1;
								if (end < start)
										end = start;
								break;
						}
						x += w;

						if (h > baseline)
								baseline = (XDWORD)h;
				}

				float excess = (GLFLOAT)(maxwidth - x);
				float indent = 0;
				float justify = 0;

				switch (Get(GRPWIDGETSTYLETYPE_TEXT_ALIGN))
				{
				case GRPWIDGETSTYLETYPE_TEXT_ALIGN_RIGHT:  indent = excess;								  	break;
				case GRPWIDGETSTYLETYPE_TEXT_ALIGN_CENTER:  indent = excess / 2.0f;						break;
				case GRPWIDGETSTYLETYPE_TEXT_ALIGN_LEFT:																			break;
				case GRPWIDGETSTYLETYPE_TEXT_ALIGN_JUSTIFY:
						if ((end - start)>2)
								justify = excess / float(end - start);
						break;
				}

				widgetwidth = 0;

				//--
				Caret.y += baseline;



				for (int e = start; e <= end; e++)
				{
						GRPWIDGET* child = widgets.Get(e);

						float h = float(child->GetHeight());
						float w = float(child->GetWidth() + child->position.vector[0]);

						int istext = child->Get(GRPWIDGETSTYLETYPE_ISTEXT);
						if (istext)
								if (e == start)
										Caret.x += indent;


						float aligny = 0;

						switch (child->Get(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN))
						{
						case GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_MIDDLE:	aligny = Caret.y + (h - baseline) / 2.0f;	break;
						case GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_TOP:			aligny = Caret.y - baseline + h;					break;
						case GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_BOTTOM:
						case GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_BASELINE:aligny = Caret.y - h;											break;
						}
						float x = child->boundingbox.leftborder + child->boundingbox.leftmargin;
						float y = child->boundingbox.topborder + child->boundingbox.topmargin;

						if (child->Get(GRPWIDGETSTYLETYPE_POSITION) == GRPWIDGETSTYLE_POSITION_TYPE_RELATIVE)
						{
								x += float(Caret.x);
								y += float(aligny
										- child->position.vector[1]
										- lineheightmargin
										);
						}

						if (child->Get(GRPWIDGETSTYLETYPE_POSITION) != GRPWIDGETSTYLE_POSITION_TYPE_STATIC)
						{
								if (child->Has(GRPWIDGETSTYLETYPE_LEFT))
										x += child->Get(GRPWIDGETSTYLETYPE_LEFT);//(child->position.vector[0]);			

								if (child->Has(GRPWIDGETSTYLETYPE_TOP))
										y += child->Get(GRPWIDGETSTYLETYPE_TOP);//XDWORD(child->position.vector[1]);								
						}

						if (child->Get(GRPWIDGETSTYLETYPE_POSITION) != GRPWIDGETSTYLE_POSITION_TYPE_ABSOLUTE)
						{
								Caret.x += child->GetWidth();
								if (end<int(widgets.GetSize() - 1)) //last line is not justified
										Caret.x += istext*justify;
						}

					  x+= child->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_X);
						child->node->SetPosition((float)x, (float)y, 0.0f);
						child->node->Update();
						child->node->SetVisible(child->Get(GRPWIDGETSTYLETYPE_VISIBILITY) != GRPWIDGETSTYLE_VISIBILITY_HIDDEN);

						Caret.x += child->Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_X);
						processedwidgets++;
				}
				start = end + 1;
				if (start >= (int)widgets.GetSize())
						break;

				Caret.x = this->boundingbox.leftpadding;
		}


		if ((Caret.y - this->boundingbox.bottompadding) > this->GetHeight())
		{
				if (!this->Has(GRPWIDGETSTYLETYPE_HEIGHT))
				{
						if (NotifyHeight(Caret.y - this->boundingbox.bottompadding))
								this->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
				}
		}

		if ((Caret.x - this->boundingbox.rightpadding) > this->GetWidth())
		{
				if (!this->Has(GRPWIDGETSTYLETYPE_WIDTH))
				{
						if (NotifyWidth(Caret.x - this->boundingbox.rightpadding))
								this->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
				}
		}

		return true;
}


/*-------------------------------------------------------------------
//	 GRPWIDGET::AlignList
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	02/02/2018 10:39:04
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::AlignList()
{
		return true;
}

/*-------------------------------------------------------------------
//	GRPWIDGET::Draw
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2017 14:26:29
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGET::Draw(const GRPMATRIX*  viewmatrix,	const GRPMATRIX*  viewprojection,	const GRPMATRIX*  projection)
{
	
	
	this->node->Update();
	this->node->UpdateWorldMatrix();
	
	if (this->Get(GRPWIDGETSTYLETYPE_DISPLAY)  == GRPWIDGETSTYLE_DISPLAY_TYPE_BLOCK ||
			this->Get(GRPWIDGETSTYLETYPE_DISPLAY)  == GRPWIDGETSTYLE_DISPLAY_TYPE_INLINEBLOCK)
	if (this->Get(GRPWIDGETSTYLETYPE_OVERFLOW) == GRPWIDGETSTYLE_OVERFLOW_TYPE_HIDDEN)
	{
			int xo = (int)this->node->GetWorldPosition().vector[0];
			int yo = (int)this->node->GetWorldPosition().vector[1];
			int xd = (int) this->GetWidth();
			int yd = (int) this->GetHeight();
			int h = int(this->root->page->ownerpass->GetScreen()->GetHeight());
			GRPOPENGLCTRL::SCISSOR::SetScissor(true, xo, h-yd, xd, h-yo);
	}
	

	SelfDraw	(viewmatrix,viewprojection,projection);
	ChildDraw	(viewmatrix,viewprojection,projection);

	
	if (!this->Get(GRPWIDGETSTYLETYPE_ISTEXT))
	if (this->Get(GRPWIDGETSTYLETYPE_OVERFLOW) == GRPWIDGETSTYLE_OVERFLOW_TYPE_HIDDEN)
 			GRPOPENGLCTRL::SCISSOR::SetScissor(false);
			
	switch (GetState())
	{
	case GRPWIDGETSTATE_ACTIVE: 
	case GRPWIDGETSTATE_HOVER : SetState(GRPWIDGETSTATE_NONE); break;
	default:
	case GRPWIDGETSTATE_NONE: break;
	}
	
	return true;
}

/*-------------------------------------------------------------------
//	GRPWIDGET::SelfDraw
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2017 14:26:33
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGET::SelfDraw(const GRPMATRIX*  viewmatrix,	const GRPMATRIX*  viewprojection,	const GRPMATRIX*  projection)
{		

	for (XDWORD e=0;e<elements.GetSize();e++)
	{
		GRPELEMENT* element = elements.GetElement(e);
		GRPSHADER::UseShader(element->GetProgram());

								element->ComputeMatrices(viewmatrix,viewprojection,projection);
								element->Draw						(viewmatrix,viewprojection,projection);

								if (this->Get(GRPWIDGETSTYLETYPE_ISTEXT))
								{
										if (this->node->GetPosition()->vector[0]==0.0f)
										if (this->node->GetPosition()->vector[1] == 0.0f)
										XDEBUG_PRINTCOLOR(3, __L("%s %f,%f,%f"), this->classtype.Get(), this->node->GetPosition()->vector[0], this->node->GetPosition()->vector[1], this->node->GetPosition()->vector[2]);
								}
	}


	GRPPROFILER::nElements += (int)elements.GetSize();

	return true;
}

/*-------------------------------------------------------------------
//	GRPWIDGET::ChildDraw
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2017 14:26:39
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGET::ChildDraw(const GRPMATRIX*  viewmatrix,	const GRPMATRIX*  viewprojection,	const GRPMATRIX*  projection)
{
	for (XDWORD e=0;e<this->widgets.GetSize();e++)
	{
		widgets.Get(e)->Draw(viewmatrix,viewprojection,projection);
	}
	return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::Get
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	18/01/2018 16:52:52
//	@return		XDWORD : 
//
//	@param		XDWORD : 
//
*//*-----------------------------------------------------------------*/
XDWORD GRPWIDGET::Get(GRPWIDGETSTYLETYPE key)
{
		XDWORD value = this->map.Get(key);

		//--------------------- otherwise, inherit
		if (parent)
		{
				if (value == GRPWIDGET_STYLE_INHERIT)
				{
						  GRPWIDGETSTYLE* def = this->page->processor->GetProperties()->Get(key);
							if (def && def->inherited)							
									return this->parent->Get(key);							
							else
							{
									if (!def)
									{
											XDEBUG_PRINTCOLOR(4, __L("unknown property %d "), key);
									}
									return value;
							}
				}
				else
						if (this->mapvaluetype.Get(key) == GRPWIDGETSTYLEVALUETYPE_RELATIVE)
						{
								GRPWIDGETSTYLE* def = this->page->processor->GetProperties()->Get(key);
								if (def)
								if (def->parentrelationproperty!=GRPWIDGETSTYLETYPE_UNKNOWN)																		
									return XDWORD((float(value) / 100.0f)*this->parent->Get(def->parentrelationproperty));															
						}
		}
				
		return value;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::GetValueType
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	23/02/2018 14:41:34
//	@return		XDWORD : 
//
//	@param		GRPWIDGETSTYLETYPE : 
//
*//*-----------------------------------------------------------------*/
XDWORD GRPWIDGET::GetValueType(GRPWIDGETSTYLETYPE key)
{
		XDWORD value = this->mapvaluetype.Get(key);

		if (parent)
				if (value == GRPWIDGET_STYLE_INHERIT)
						return this->parent->GetValueType(key);

		return value;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::Set
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	18/01/2018 16:59:18
//	@return		void : 
//
//	@param		XDWORD : 
//	@param		XDWORD : 
//
*//*-----------------------------------------------------------------*/
void GRPWIDGET::Set(GRPWIDGETSTYLETYPE key, XDWORD value, XDWORD valuetype)
{		
		if (!this->map.Set(key,value))		
				if (this->map.Add(key, value))
				{
						if (!this->mapvaluetype.Set(key, valuetype))
								this->mapvaluetype.Add(key, valuetype);
				}
}

/*-------------------------------------------------------------------
//	GRPWIDGET::SetName
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/08/2017 10:41:37
//	
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
void GRPWIDGET::SetName(XCHAR* name)
{
	this->name.Set(name);
}

/*-------------------------------------------------------------------
//	GRPWIDGET::OnAdded
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2017 16:16:02
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGET::OnAdded()
{	
	this->boundingbox=this->parent->boundingbox;
	return ApplyStyles();
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::SetState
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	05/02/2018 13:23:17
//	@return		void : 
//
//	@param		GRPWIDGETSTATE : 
//
*//*-----------------------------------------------------------------*/
void GRPWIDGET::SetState(GRPWIDGETSTATE state)
{		
		if (this->state!=state)
				if (HasState(state))
				{
						if (this->parent)
								this->parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
						this->ismodified |= GRPWIDGETSECTION_FLAG_STYLE;
						this->state = state;
				}		
}

bool GRPWIDGET::HasState(GRPWIDGETSTATE state)
{
		for (XDWORD c = 0; c < this->classes.GetSize(); c++)							
				if (classes.Get(c)->selector.targetstate==state)
										return true;

		return false;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::GetColor
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	30/01/2018 13:02:41
//	@return		GRPCOLOR* : 
//
//	@param		GRPWIDGETSECTION : 
//
*//*-----------------------------------------------------------------*/
GRPCOLOR* GRPWIDGET::GetColor(GRPWIDGETSECTION section)
{
		GRPCOLOR* color = colors.Get(section);
		if (color == NULL)
				if (parent)
						color=parent->GetColor(section);

		return color;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::SetValue
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	13/02/2018 10:16:16
//	@return		bool : 
//
//	@param		XCHAR* : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGET::SetValue(XCHAR* string)
{
		this->content = string;
		this->text = content;
		this->CreateText();
		this->Align();
		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGET::GetValue
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	20/02/2018 14:55:33
//	@return		XCHAR* : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
XCHAR* GRPWIDGET::GetValue()
{
		return this->content.Get();
}


