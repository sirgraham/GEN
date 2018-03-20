/*------------------------------------------------------------------------------------------
//	GRPWIDGET.H
*/	
/**	
// \class 
//   
//  Manages widget styles
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 17/08/2017 12:05:11
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPWIDGET_H_
#define _GRPWIDGET_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPPass.h"

#include "GRPTextBox.h"
#include "GRPColor.h"
#include "GRPTexture.h"

#include "GRPWidgetStyle.h"

#include "GRPDrawable.h"

#include "XVector.h"
#include "XMap.h"
#include "XFString.h"

	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class XFILEXMLELEMENT;
class GRPWIDGET;
class GRPWIDGETPAGE;
class GRPWIDGETRENDERER;
class GRPWIDGETALIGNER;

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPWIDGETFACTORY
{
public:
		virtual GRPWIDGET* Create(XCHAR* tag = NULL);
};

class GRPWIDGET : public GRPDRAWABLE, public GRPSUBJECT, public GRPOBSERVER
{
friend class GRPWIDGETRENDERER;
friend class GRPWIDGETALIGNER;

public:
																							GRPWIDGET				();
																							~GRPWIDGET			();

					XDWORD															GetSubjectType	() { return GRPNAMED_TYPE_WIDGET; }

	virtual void																SetTag					(XCHAR* tag);	
	virtual void																SetClassType		(XCHAR* classtype);

					bool																Manage					(GRPWIDGETSTATE, float x, float y);
					bool																Hit							(float x, float y);
					WIDGETSTRING												Print						();
					int																	GetDepth				();

	virtual bool																ApplyStyles			();
	
					void																Init									();
	virtual GRPWIDGET*													Create								();

	virtual bool																CreateSection					(GRPWIDGETSECTION);
	virtual bool																AssignColor						(GRPWIDGETSECTION);
	

	virtual bool																CreateBackground			();
	virtual bool																CreateShadow					();
	virtual bool																CreateBorder					();
	virtual bool																CreateOutline					();
	virtual bool																CreateContent					();
	virtual bool																CreateText						();	
	virtual bool																CreateTextShadow			();

	virtual bool																AssignTextColor				();
	virtual bool																AssignBackgroundColor	();
	virtual bool																AssignBorderColor			();
	virtual bool																AssignContentColor		();
	virtual bool																AssignShadowColor			();
	virtual bool																AssignTextShadowColor ();
	virtual bool																AssignOutlineColor		();

				  GRPFONT*														FindSuitableFont			();

					bool																SplitTextIntoWidgets	();

					float																GetWidth				();
					float																GetHeight				();

					float																GetClientWidth	();
					float																GetClientHeight	();
					
					bool																Add							(GRPWIDGET* child);
					bool																SetParent				(GRPWIDGET* parent);

					bool																NotifyWidth			(GLFLOAT			w);
					bool																NotifyHeight		(GLFLOAT			h);
	virtual bool																Notify					(GRPMSGTYPE		msg, GRPSUBJECT*			notifier);

					bool																Has							(GRPWIDGETSTYLETYPE t);

					GRPWIDGETSTYLECLASS*								GetStyle				() { return &this->style; }					

					bool																Normalize				();
					bool																Align						();					

					bool																AlignBlock			();
					bool																AlignInline			(); 
					bool																AlignTable			();
					bool																AlignTableRow		();
					bool																AlignList				();

					bool																Draw						(const GRPMATRIX*  viewmatrix,	const GRPMATRIX*  viewprojection,	const GRPMATRIX*  projection);
					bool																SelfDraw				(const GRPMATRIX*  viewmatrix,	const GRPMATRIX*  viewprojection,	const GRPMATRIX*  projection);
					bool																ChildDraw				(const GRPMATRIX*  viewmatrix,	const GRPMATRIX*  viewprojection,	const GRPMATRIX*  projection);

					XDWORD														  Get							(GRPWIDGETSTYLETYPE);
					XDWORD														  GetValueType		(GRPWIDGETSTYLETYPE);
					void															  Set							(GRPWIDGETSTYLETYPE, XDWORD,				XDWORD valuetype=GRPWIDGETSTYLEVALUETYPE_ABSOLUTE);
				//	void															  Set							(GRPWIDGETSTYLETYPE, XSTRING*value);

					void																SetName					(XCHAR* name);
					virtual bool												OnAdded					();

					GRPWIDGETSTATE											GetState				() { return state; }
					void																SetState				(GRPWIDGETSTATE state);
					bool																HasState				(GRPWIDGETSTATE state);

					GRPCOLOR*														GetColor				(GRPWIDGETSECTION);
					GRPMATERIAL*												GetMaterial			(GRPWIDGETSECTION);

					GRPSHADERPROGRAM*										GetProgram			() { return NULL; }
					void																SetProgram			(GRPSHADERPROGRAM*) {}
					

					bool																SetValue				(XCHAR* string);
					XCHAR*															GetValue				();

					//-------------- properties

					WIDGETSTRING												tag;
					WIDGETSTRING												classtype;

					XMAP<XDWORD,GRPELEMENT*>						elements;					
					XVECTOR<GRPWIDGETSTYLECLASS*>				classes;

					XMAP<XDWORD, GRPWIDGETSTYLECLASS*>	states;
					GRPWIDGETSTATE											state;

					GRPWIDGETSTYLECLASS									style;

					GRPSCENE*														scene;

					XVECTOR<GRPWIDGET*>									widgets;	
					XMAP<GRPWIDGETSECTION,GRPNODE*>			nodes;
					
					GRPWIDGET*													parent;
					GRPWIDGET*													context;

					GRPWIDGET*												  root;

					GRPNODE*														node;

					GRPAABB															boundingbox;

					XMAP<GRPWIDGETSECTION,GRPMATERIAL*>	materials;
					XMAP<GRPWIDGETSECTION,GRPCOLOR*>		colors;
	
					WIDGETSTRING												content;
					
					XDWORD															ismodified;

					bool																isvisible;

					float																texturex;
					float																texturey;

					float																texture_offset_x;
					float																texture_offset_y;

					float																texture_width;
					float																texture_height;			

					WIDGETSTRING												text;

					GRPVECTOR													  position; 
					GRPVECTOR													  rotation;
					GRPVECTOR													  scale;
					
					WIDGETSTRING												fontfamily;

					GRPWIDGETALIGNER*										aligner;

					XMAP<GRPWIDGETSTYLETYPE, XDWORD>		map;
					XMAP<GRPWIDGETSTYLETYPE, XDWORD>		mapvaluetype;

					WIDGETSTRING												value;
					GRPWIDGETPAGE*											page;

					GRPWIDGETSTYLE_OVERFLOW_TYPE				overflow;

					XVECTOR<int>												colwidth;
					XVECTOR<int>												rowheight;

					
					XDICTIONARY<XSTRING>								attributes;

					int																	orderindex;


					float															 minimun_required_width;
					float															 minimun_required_height;
private:

			void Clean()
			{		

				map.elements.SetIsMulti(true);
				this->Set(GRPWIDGETSTYLETYPE_FLOAT,							GRPWIDGETSTYLE_FLOAT_TYPE_NONE);
				this->Set(GRPWIDGETSTYLETYPE_POSITION,					GRPWIDGETSTYLE_POSITION_TYPE_RELATIVE);
				this->Set(GRPWIDGETSTYLETYPE_DISPLAY,						GRPWIDGETSTYLE_DISPLAY_TYPE_INLINE);
				this->Set(GRPWIDGETSTYLETYPE_CLEAR,							GRPWIDGETSTYLE_CLEAR_TYPE_INHERIT);
				this->Set(GRPWIDGETSTYLETYPE_VISIBILITY,				GRPWIDGETSTYLE_CLEAR_TYPE_INHERIT);
				this->Set(GRPWIDGETSTYLETYPE_MAX_WIDTH,					GRPWIDGETSTYLE_CLEAR_TYPE_INHERIT);
				this->Set(GRPWIDGETSTYLETYPE_MAX_HEIGHT,				GRPWIDGETSTYLE_CLEAR_TYPE_INHERIT);
				this->Set(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN,		GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_BASELINE);
				this->Set(GRPWIDGETSTYLETYPE_FONT_DECORATION,		GRPWIDGETSTYLE_CLEAR_TYPE_INHERIT);

				name.Set(__L("undefined"));
				texturex								= 1.0f;
				texturey								= 1.0f;
				texture_offset_x				= 0.0f;
				texture_offset_y				= 0.0f;
				node										= NULL;
				parent									= NULL;
				ismodified							= 0;	
				fontfamily							= __L("");				
				root										= NULL;
				scene										= NULL;
				aligner									= NULL;
				page										= NULL;
				state										= GRPWIDGETSTATE_NONE;
				context									= NULL;
				orderindex							= 0;
				minimun_required_width  = 0;
				minimun_required_height = 0;
			}
};

class GRPWIDGETRENDERER { public:		virtual bool OnDraw(GRPWIDGET* widget) { return true; }};

class GRPWIDGETALIGNER																			
{
public:		
		virtual bool Align					(GRPWIDGET*)=0; 
		virtual bool OnNotifyHeight	(float h) { return true; };
		virtual bool OnNotifyWidth	(float w) { return true; };
};

class GRPWIDGETALIGNBLOCK				: public GRPWIDGETALIGNER {	public:		virtual bool Align(GRPWIDGET*);   };
class GRPWIDGETALIGNINLINE			: public GRPWIDGETALIGNER { public:		virtual bool Align(GRPWIDGET*);   };
class GRPWIDGETALIGNTABLE				: public GRPWIDGETALIGNER { public:		virtual bool Align(GRPWIDGET*);   };
class GRPWIDGETALIGNTABLEROW		: public GRPWIDGETALIGNER { public:		virtual bool Align(GRPWIDGET*);		};
class GRPWIDGETALIGNLIST				: public GRPWIDGETALIGNER {	public:		virtual bool Align(GRPWIDGET*);	  };

class GRPWIDGETBEHAVIOR
{
public:
	GRPWIDGETBEHAVIORTYPE						type;

	enum GRPWIDGETEVENTTYPE
	{
			GRPWIDGETEVENTTYPE_UNKNOWN=0,
			GRPWIDGETEVENTTYPE_ACTIVE,
			GRPWIDGETEVENTTYPE_INACTIVE,
			GRPWIDGETEVENTTYPE_HOVER,
			GRPWIDGETEVENTTYPE_OUT,
			GRPWIDGETEVENTTYPE_FOCUS,
			GRPWIDGETEVENTTYPE_BLUR,
			GRPWIDGETEVENTTYPE_HOLD,
			GRPWIDGETEVENTTYPE_RELEASE,
			GRPWIDGETEVENTTYPE_DRAG,
			GRPWIDGETEVENTTYPE_DRAG_ENTER,
			GRPWIDGETEVENTTYPE_DRAG_LEAVE,
			GRPWIDGETEVENTTYPE_DRAG_OVER,
			GRPWIDGETEVENTTYPE_DRAG_START,
			GRPWIDGETEVENTTYPE_DROP,
			GRPWIDGETEVENTTYPE_PINCH_IN,
			GRPWIDGETEVENTTYPE_PINCH_OUT,
			GRPWIDGETEVENTTYPE_PINCH_ROTATE,		
			GRPWIDGETEVENTTYPE_KEY,
			GRPWIDGETEVENTTYPE_ONCHANGE,
			GRPWIDGETEVENTTYPE_CREATE,
			GRPWIDGETEVENTTYPE_DESTROY,
			GRPWIDGETEVENTTYPE_DRAW,
			GRPWIDGETEVENTTYPE_ADDED,
			GRPWIDGETEVENTTYPE_REMOVE,
			GRPWIDGETEVENTTYPE_RELOAD,			
			GRPWIDGETEVENTTYPE_RESET,
			GRPWIDGETEVENTTYPE_ALIGN,
			GRPWIDGETEVENTTYPE_CLICK,
			GRPWIDGETEVENTTYPE_DOUBLE_CLICK,
			GRPWIDGETEVENTTYPE_WHEEL_UP,
			GRPWIDGETEVENTTYPE_WHEEL_DOWN,
			GRPWIDGETEVENTTYPE_SCROLL,
			GRPWIDGETEVENTTYPE_ONCOPY,
			GRPWIDGETEVENTTYPE_ONCUT,
			GRPWIDGETEVENTTYPE_ONPASTE,
	};

	bool														On								(GRPWIDGETEVENTTYPE){ return true; }

	//----- clicks
	bool														OnClick() { return true; }
	bool														OnDoubleClick() { return true; }

	//----- state changes
	bool														OnActive					()									{ return true; }	
	bool														OnInactive				()									{ return true; }
	bool														OnHover						()									{ return true; }
	bool														OnOut							()									{ return true; }
	bool														OnFocus						()									{ return true; }
	bool														OnBlur						()									{ return true; }
	bool														OnHold						()									{ return true; }
	bool														OnRelease					()									{ return true; }

	//----- drag and drop
	bool														OnDrag						()									{ return true; }
	bool														OnDragOver				()									{ return true; }
	bool														OnDragEnter				()									{ return true; }
	bool														OnDragExit				()									{ return true; }
	bool														OnDrop						()									{ return true; }	

	//----- scrolling
	bool														OnScroll					()									{ return true; }

	//----- zoom pinch
	bool														OnPinchIn					()									{ return true; }
	bool														OnPinchRotate			()									{ return true; }
	bool														OnPinchOut				()									{ return true; }

	//----- keyboard
	bool														OnKey							()									{ return true; }
	bool														OnKeyUp						()									{ return true; }
	bool														OnKeyDown					()									{ return true; }
	bool														OnChange					()									{ return true; }

	//----- widget relation
	bool														OnCreate					()									{ return true; }
	bool														OnDestroy					()									{ return true; }
	bool														OnDraw						()									{ return true; }
	bool														OnWidgetAdded			()									{ return true; }
	bool														OnWidgetRemoved		()									{ return true; }
	bool														OnAlign						()									{ return true; }

	//----- page event	
	bool														OnLoad						()									{ return true; }
	
	//----- wheel delta
	bool														OnWheelUp					()									{ return true; }
	bool														OnWheelDown				()									{ return true; }

	//-----copypasta
	bool														OnCopy						()									{ return true; }
	bool														OnCut							()									{ return true; }
	bool														OnPaste						()									{ return true; }


};

	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

