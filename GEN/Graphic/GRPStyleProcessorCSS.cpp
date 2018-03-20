/*------------------------------------------------------------------------------------------
// GRPSTYLEPROCESSORCSS.CPP
*/
/**
// \class
//
// Parses css styles
//
//	@author: Diego Martinez Ruiz de Gaona 
//
//	Date of Creation : 22/02/2018 14:09:56 
//	Last Modification : 
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPStyleProcessorCSS.h"
#include "GRPWidgetStyle.h"
#include "XDebug.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

GRPCOLORPALETTE* GRPSTYLEPROCESSORCSS::csscolorpalette=NULL;

/*-------------------------------------------------------------------
//	GRPSTYLEPROCESSORCSS::GRPSTYLEPROCESSORCSS
*/
/**
//
//	constructor
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 14:50:25
//	@return		 : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
GRPSTYLEPROCESSORCSS::GRPSTYLEPROCESSORCSS()
{

}

/*-------------------------------------------------------------------
//	GRPSTYLEPROCESSORCSS::~GRPSTYLEPROCESSORCSS
*/
/**
//
//	destructor
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 14:50:19
//	@return		 : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
GRPSTYLEPROCESSORCSS::~GRPSTYLEPROCESSORCSS()
{
		csstypes.entries.DeleteContents();
		csstypes.entries.DeleteAll();

		cssproperties.DeleteElementContents();
		cssproperties.DeleteAll();

		if (csscolorpalette)
		{
				delete(csscolorpalette);
				csscolorpalette = NULL;
		}
		
}


/*-------------------------------------------------------------------
//	 GRPSTYLEPROCESSORCSS::ParseStyle
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 14:10:22
//	@return		bool : 
//
//	@param		XCHAR* : 
//
*//*-----------------------------------------------------------------*/
bool GRPSTYLEPROCESSORCSS::ParseStyle(GRPSTYLESHEET* sheet, XCHAR* styledata)
{
		XSTRING content;
		content = styledata;

		if (!content.GetSize())
				return true;

		this->CleanStyle(&content);

		XVECTOR<XSTRING*> classes;

		content.Explode(__C('}'), &classes);

		XDWORD start = 0;
		for (XDWORD e = 0; e<classes.GetSize(); e++)
		{
				XSTRING classname;
				XSTRING classdata;

				classes.Get(e)->DeleteNoCharacters(__L(" "), 0, XSTRINGCONTEXT_ATFIRST);

				int end = classes.Get(e)->FindCharacter(__C('{'), start);
				if (end == -1)
						break;

				classes.Get(e)->Copy(0, end, classname);
				classname.DeleteNoCharacters(__L(" "), 0, XSTRINGCONTEXT_ATEND);

				classes.Get(e)->Copy(end + 1, classes.Get(e)->GetSize(), classdata);

				XVECTOR<XSTRING*> classnames;
				classname.Explode(__C(','), &classnames);
				for (XDWORD i = 0; i < classnames.GetSize(); i++)
				{
						GRPWIDGETSTYLECLASS* style = CreateStyle(classnames.Get(i), &classdata);
						if (!sheet->Add(style))
								delete(style);
				}
				classnames.DeleteContents();
				classnames.DeleteAll();
		}
		classes.DeleteContents();
		classes.DeleteAll();
		return true;
}

bool GRPSTYLEPROCESSORCSS::Process(GRPWIDGETSTYLECLASS* styleclass,XCHAR* stext)
{
		XSTRING stylestring;
		stylestring.Set(stext);

		XVECTOR<XSTRING*> strings;
		stylestring.Explode(__C(';'), &strings);

		for (XDWORD e = 0; e<strings.GetSize(); e++)
		{
				XVECTOR<XSTRING*> values;
				strings.Get(e)->Explode(__C(':'), &values);

				if (values.GetSize() < 2)
				{
						values.DeleteContents();
						values.DeleteAll();
						continue;
				}

				values.Get(0)->DeleteNoCharacters(__L(" "), 0, XSTRINGCONTEXT_ATFIRST);
				values.Get(0)->DeleteNoCharacters(__L(" "), 0, XSTRINGCONTEXT_ATEND);
				values.Get(1)->DeleteNoCharacters(__L(" "), 0, XSTRINGCONTEXT_ATFIRST);

				GRPWIDGETSTYLETYPE type = GRPSTYLEPROCESSORCSS::csstypes[values.Get(0)->Get()];

				if (type == GRPWIDGETSTYLETYPE_UNKNOWN)
				{
						XDEBUG_PRINTCOLOR(4, __L("GRPWIDGETSTYLE: unknown style  %s"), values.Get(0)->Get());
						values.DeleteContents();
						values.DeleteAll();
						continue;
				}

				//----

				GRPWIDGETSTYLE* style = new GRPWIDGETSTYLE();
				style->csstype = type;
				style->type.Set(values.Get(0)->Get());
				style->value.Set(values.Get(1)->Get());
				style->valuetype = GRPWIDGETSTYLEVALUETYPE_ABSOLUTE;

				if (!style->value.Character_IsAlpha(style->value.Get()[0])) //empieza por un numero
				{
						XSTRING units;
						style->value.Copy(style->value.GetSize() - 2, style->value.GetSize(), units);
						if (units.Compare(__L("px")) == 0) //esta en pixeles
								style->value.DeleteCharacters(style->value.GetSize() - 2, 2);
						else
								if (units.Compare(__L("em")) == 0) //esta en picas
								{
										style->value.DeleteCharacters(style->value.GetSize() - 2, 2);
										double em = style->value.ConvertToDouble();
										style->value.Format(__L("%d"), int(em * 16));
								}
								else
								{
										style->value.Copy(style->value.GetSize() - 1, style->value.GetSize(), units);
										if (units.Compare(__L("%")) == 0)
										{
												style->valuetype = GRPWIDGETSTYLEVALUETYPE_RELATIVE;
												style->value.DeleteCharacter(__C('%'), XSTRINGCONTEXT_ATEND);
										}
										else
												style->valuetype = GRPWIDGETSTYLEVALUETYPE_SPECIAL;
								}
				}

				//----

				XDWORD index = styleclass->styles.Find(style->csstype);
				if (index != NOTFOUND)
				{
						styleclass->styles.keys.DeleteIndex(index);
						styleclass->styles.elements.DeleteIndex(index);
				}

				if (!styleclass->styles.Add(style->csstype, style))
						delete(style);

				values.DeleteContents();
				values.DeleteAll();
		}

		strings.DeleteContents();
		strings.DeleteAll();
		return true;
}


/*-------------------------------------------------------------------
//	 GRPSTYLEPROCESSORCSS::CleanStyle
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 14:10:57
//	@return		bool : 
//
//	@param		XSTRING* : 
//
*//*-----------------------------------------------------------------*/
bool  GRPSTYLEPROCESSORCSS::CleanStyle(XSTRING* content)
{
		content->DeleteNoCharacters(__L("\r\n\t"), 0, XSTRINGCONTEXT_ALLSTRING);

		XDWORD start = 0;

		while (true)
		{
				start = content->Find(__L("/*"), true, start);
				if (start == -1) //no hay comentarios
						break;

				int end = content->Find(__L("*/"), true, start);
				if (end == -1)
						end = content->GetSize();
				else
						end += 2;

				content->DeleteCharacters(start, end - start);
				start = end;

				if (start == content->GetSize())
						break;
		}
		content->DeleteNoCharacters(__L(" "), 0, XSTRINGCONTEXT_ATFIRST);
		return true;
}

/*-------------------------------------------------------------------
//	 GRPSTYLEPROCESSORCSS::CreateStyle
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 14:10:52
//	@return		GRPWIDGETSTYLECLASS* : 
//
//	@param		XSTRING* : 
//	@param		XSTRING*data : 
//
*//*-----------------------------------------------------------------*/
GRPWIDGETSTYLECLASS*  GRPSTYLEPROCESSORCSS::CreateStyle(XSTRING* _classname, XSTRING *classdata)
{
		GRPWIDGETSTYLECLASS* style = new GRPWIDGETSTYLECLASS();

		XVECTOR<XSTRING*> classes;
		_classname->Explode(__C(' '), &classes);

		GRPWIDGETSTYLECLASSSELECTOR* selector = &style->selector;

		for (int e = classes.GetSize() - 1; e >= 0; e--)
		{
				XSTRING* classname = classes.Get(e);

				switch (classname->Get()[0])
				{
				case __C('*'): selector->targettype = GRPWIDGETTARGETTYPE_ALL;   break;
				case __C('#'): selector->targettype = GRPWIDGETTARGETTYPE_NAME;  break;
				case __C('.'): selector->targettype = GRPWIDGETTARGETTYPE_CLASS; break;
				case __C('@'): selector->targettype = GRPWIDGETTARGETTYPE_RULE; break;

				default:			 selector->targettype = GRPWIDGETTARGETTYPE_TAG;   break;
				}

				int stateseparator = classname->FindCharacter(__C(':'));
				if (stateseparator != NOTFOUND)
				{
						XSTRING state;
						classname->Copy(stateseparator + 1, classname->GetSize(), state);

						//------------- hover state
						if (state.Compare(__L("hover")) == 0)		selector->targetstate = GRPWIDGETSTATE_HOVER;						else
						if (state.Compare(__L("active")) == 0)		selector->targetstate = GRPWIDGETSTATE_ACTIVE;					else
						if (state.Compare(__L("visited")) == 0)		selector->targetstate = GRPWIDGETSTATE_VISITED;					else
						if (state.Compare(__L("disabled")) == 0)		selector->targetstate = GRPWIDGETSTATE_DISABLED;				else
						if (state.Compare(__L("focus")) == 0)		selector->targetstate = GRPWIDGETSTATE_FOCUS;
				}
				else
						stateseparator = classname->GetSize();

				if (selector->targettype == GRPWIDGETTARGETTYPE_TAG)
						classname->Copy(0, stateseparator, style->name);
				else
						classname->Copy(1, stateseparator, style->name);

				selector->name = style->name;

				classdata->DeleteNoCharacters(__L(" "), 0, XSTRINGCONTEXT_ATFIRST);
				this->Process(style,classdata->Get());

				if (e != 0)
				{
						GRPWIDGETSTYLECLASSSELECTOR* dependency = new GRPWIDGETSTYLECLASSSELECTOR();
						style->dependencies.Add(dependency);
						selector = dependency;
				}
		}

		style->selector.name = classes.GetLast()->Get();
		classes.DeleteContents();
		classes.DeleteAll();
		return style;		
}

/*-------------------------------------------------------------------
//	 GRPSTYLEPROCESSORCSS::Init
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 14:44:38
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPSTYLEPROCESSORCSS::Init(GRPSTYLESHEET* defaultsheet)
{
		const char defaultcss[] =
		{
				"html { "
				"		visibility:visible; "
				"		display:block; "
				"		color:black; "
				"		font-size:1em; "
				"		font-family: Times New Roman, Arial, Roboto; "
				"		white-space:normal;"
//				"		background-position:0px 0px;"
				"}"

				"html, address,blockquote,body,dd,div,dl,dt,fieldset,form,frame, frameset,h1,h2,h3,h4,h5,h6,noframes,ol,p,ul,center,dir,hr,menu,pre{ display: block;}"
				"li{ display: list-item; }"
				"head{ display: none; }"
				"table{ display: table; }"
				"tr{ display: table-row; }"
				"thead{ display: table-header-group; vertical-align:middle; }"
				"tbody{ display: table-row-group;    vertical-align:middle; }"
				"tfoot{ display: table-footer-group; vertical-align:middle; }"
				"col	{ display: table-column; }"
				"colgroup{ display: table-column-group; }"
				"td, th{ display: table-cell; }"
				"caption{ display: table-caption }"
				"th{ font-weight:bold; text-align:center; }"
				"caption{ text-align: center; }"
				"body{ padding: 8px; }"
				"h1{ font-size: 2em; margin: .67em 0; }"
				"h2{ font-size: 1.5em; margin: .75em 0; }"
				"h3{ font-size: 1.17em; margin: .83em 0; }"
				"h4, p,	blockquote, ul,	fieldset, form, ol, dl, dir,menu { margin: 1.12em; }"
				"h5{ font-size: .83em; margin: 1.5em 0; }"
				"h6{ font-size: .75em; margin: 1.67em 0; }"
				"h1, h2, h3, h4,h5, h6, b,strong{ font-weight: bolder; }"
				"blockquote{ margin-left: 40px; margin-right: 40px; }"
				"i, cite, em, var, address{ font-style: italic; }"
				"pre, tt, code, kbd, samp{ font-family: monospace; }"
				"pre{ white-space: pre; }"
				"button, textarea,input, select{ display: inline-block; }"
				"big{ font-size: 1.17em; }"
				"small, sub, sup{ font-size: 0.83em;}"
				"sub{ vertical-align: sub; }"
				"sup{ vertical-align: super; }"
				"table{ border-spacing: 2px; }"
				"thead,tbody,tfoot{ vertical-align: middle; }"
				"td, th, tr{ vertical-align: inherit; }"
				"s, strike, del{ text-decoration: line-through; }"
				"hr{ border: 1px inset; }"
				"ol, ul, dir,menu, dd{ margin-left: 40px; }"
				"ol{ list-style-type: decimal; }"
				//	"ol ul, ul ol,ul ul, ol ol{ margin-top: 0; margin-bottom: 0 }"
				"u, ins{ text-decoration: underline; }"
				"center{ text-align: center; }"
				//":link, :visited{ text-decoration: underline }"
				//":focus{ outline: thin dotted invert }"
		};
		if (!InitDictionary())
				return false;

		XSTRING css(defaultcss);
		this->ParseStyle(defaultsheet,css.Get());

		//---- especial
		this->ParseStyle(defaultsheet, __L("anonymous_text { display:inline;}"));

		
		return true;
}

/*-------------------------------------------------------------------
//	 GRPSTYLEPROCESSORCSS::InitDictionary
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 14:43:09
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPSTYLEPROCESSORCSS::InitDictionary()
{
		//----------- box model
		csstypes[__L("display")] = GRPWIDGETSTYLETYPE_DISPLAY;
		csstypes[__L("visibility")] = GRPWIDGETSTYLETYPE_VISIBILITY;
		csstypes[__L("clear")] = GRPWIDGETSTYLETYPE_CLEAR;
		csstypes[__L("float")] = GRPWIDGETSTYLETYPE_FLOAT;
		csstypes[__L("width")] = GRPWIDGETSTYLETYPE_WIDTH;
		csstypes[__L("height")] = GRPWIDGETSTYLETYPE_HEIGHT;
		csstypes[__L("top")] = GRPWIDGETSTYLETYPE_TOP;
		csstypes[__L("left")] = GRPWIDGETSTYLETYPE_LEFT;
		csstypes[__L("position")] = GRPWIDGETSTYLETYPE_POSITION;
		csstypes[__L("opacity")] = GRPWIDGETSTYLETYPE_OPACITY;
		csstypes[__L("background-color")] = GRPWIDGETSTYLETYPE_BACKGROUND_COLOR;
		csstypes[__L("background")] = GRPWIDGETSTYLETYPE_BACKGROUND_IMAGE;
		csstypes[__L("background-size")] = GRPWIDGETSTYLETYPE_BACKGROUND_SIZE;
		csstypes[__L("background-size-x")] = GRPWIDGETSTYLETYPE_BACKGROUND_SIZE_X;
		csstypes[__L("background-size-y")] = GRPWIDGETSTYLETYPE_BACKGROUND_SIZE_Y;
		csstypes[__L("background-position")] = GRPWIDGETSTYLETYPE_BACKGROUND_POSITION;
		csstypes[__L("background-position-x")] = GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_X;
		csstypes[__L("background-position-y")] = GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_Y;
		csstypes[__L("content")] = GRPWIDGETSTYLETYPE_CONTENT;
		csstypes[__L("margin")] = GRPWIDGETSTYLETYPE_MARGIN;
		csstypes[__L("margin-top")] = GRPWIDGETSTYLETYPE_MARGIN_TOP;
		csstypes[__L("margin-left")] = GRPWIDGETSTYLETYPE_MARGIN_LEFT;
		csstypes[__L("margin-bottom")] = GRPWIDGETSTYLETYPE_MARGIN_BOTTOM;
		csstypes[__L("margin-right")] = GRPWIDGETSTYLETYPE_MARGIN_RIGHT;
		csstypes[__L("padding")] = GRPWIDGETSTYLETYPE_PADDING;
		csstypes[__L("padding-top")] = GRPWIDGETSTYLETYPE_PADDING_TOP;
		csstypes[__L("padding-bottom")] = GRPWIDGETSTYLETYPE_PADDING_BOTTOM;
		csstypes[__L("padding-left")] = GRPWIDGETSTYLETYPE_PADDING_LEFT;
		csstypes[__L("padding-right")] = GRPWIDGETSTYLETYPE_PADDING_RIGHT;

		//--- text
		csstypes[__L("color")] = GRPWIDGETSTYLETYPE_FONT_COLOR;
		csstypes[__L("font-size")] = GRPWIDGETSTYLETYPE_FONT_SIZE;
		csstypes[__L("font-family")] = GRPWIDGETSTYLETYPE_FONT_FACE;
		csstypes[__L("font-style")] = GRPWIDGETSTYLETYPE_FONT_STYLE;
		csstypes[__L("font-weight")] = GRPWIDGETSTYLETYPE_FONT_WEIGHT;
		csstypes[__L("line-height")] = GRPWIDGETSTYLETYPE_FONT_LINE_HEIGHT;
		csstypes[__L("text-align")] = GRPWIDGETSTYLETYPE_TEXT_ALIGN;
		csstypes[__L("vertical-align")] = GRPWIDGETSTYLETYPE_VERTICAL_ALIGN;
		csstypes[__L("overflow")] = GRPWIDGETSTYLETYPE_OVERFLOW;
		csstypes[__L("white-space")] = GRPWIDGETSTYLETYPE_WHITESPACE;
		csstypes[__L("word-spacing")] = GRPWIDGETSTYLETYPE_WORD_SPACING;
		csstypes[__L("text-indent")] = GRPWIDGETSTYLETYPE_FONT_INDENT;
		csstypes[__L("text-decoration")] = GRPWIDGETSTYLETYPE_FONT_DECORATION;

		//---- border
		csstypes[__L("border")]							 = GRPWIDGETSTYLETYPE_BORDER;
		csstypes[__L("border-radius")]			 = GRPWIDGETSTYLETYPE_BORDER_RADIUS;
		csstypes[__L("border-color")]				 = GRPWIDGETSTYLETYPE_BORDER_COLOR;
		csstypes[__L("border-style")]				 = GRPWIDGETSTYLETYPE_BORDER_STYLE;
		csstypes[__L("border-width")]				 = GRPWIDGETSTYLETYPE_BORDER_WIDTH;

		csstypes[__L("border-bottom")]			 = GRPWIDGETSTYLETYPE_BORDER_BOTTOM;
		csstypes[__L("border-bottom-style")] = GRPWIDGETSTYLETYPE_BORDER_STYLE;
		csstypes[__L("border-bottom-width")] = GRPWIDGETSTYLETYPE_BORDER_BOTTOM_WIDTH;
		csstypes[__L("border-bottom-color")] = GRPWIDGETSTYLETYPE_BORDER_COLOR;

		csstypes[__L("border-top")]					 = GRPWIDGETSTYLETYPE_BORDER_TOP;
		csstypes[__L("border-top-style")]		 = GRPWIDGETSTYLETYPE_BORDER_STYLE;
		csstypes[__L("border-top-width")]		 = GRPWIDGETSTYLETYPE_BORDER_TOP_WIDTH;
		csstypes[__L("border-top-color")]		 = GRPWIDGETSTYLETYPE_BORDER_COLOR;

		csstypes[__L("border-left")]				 = GRPWIDGETSTYLETYPE_BORDER_LEFT;
		csstypes[__L("border-left-style")]   = GRPWIDGETSTYLETYPE_BORDER_STYLE;
		csstypes[__L("border-left-width")]   = GRPWIDGETSTYLETYPE_BORDER_LEFT_WIDTH;
		csstypes[__L("border-left-color")]   = GRPWIDGETSTYLETYPE_BORDER_COLOR;

		csstypes[__L("border-right")]				 = GRPWIDGETSTYLETYPE_BORDER_RIGHT;
		csstypes[__L("border-right-style")]  = GRPWIDGETSTYLETYPE_BORDER_STYLE;
		csstypes[__L("border-right-width")]  = GRPWIDGETSTYLETYPE_BORDER_RIGHT_WIDTH;
		csstypes[__L("border-right-color")]  = GRPWIDGETSTYLETYPE_BORDER_COLOR;

		//---- outline
		csstypes[__L("outline")]						 = GRPWIDGETSTYLETYPE_OUTLINE;
		csstypes[__L("outline-color")]			 = GRPWIDGETSTYLETYPE_OUTLINE_COLOR;
		csstypes[__L("outline-width")]			 = GRPWIDGETSTYLETYPE_OUTLINE_WIDTH;
		csstypes[__L("outline-style")]			 = GRPWIDGETSTYLETYPE_OUTLINE_STYLE;
		csstypes[__L("outline-offset")]			 = GRPWIDGETSTYLETYPE_OUTLINE_OFFSET;

		//--- shadow 
		csstypes[__L("box-shadow")]					 = GRPWIDGETSTYLETYPE_BOXSHADOW;
		csstypes[__L("text-shadow")]				 = GRPWIDGETSTYLETYPE_TEXTSHADOW;

		//--- table
		csstypes[__L("border-spacing")]			 = GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING;
		csstypes[__L("colspan")]						 = GRPWIDGETSTYLETYPE_TABLE_COLSPAN;
		csstypes[__L("rowspan")]						 = GRPWIDGETSTYLETYPE_TABLE_ROWSPAN;
				
		//==================================================================== FULL DEF

		cssproperties.Add(GRPWIDGETSTYLETYPE_DISPLAY,								new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_DISPLAY, false, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_VISIBILITY,						new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_VISIBILITY, true, false, GRPWIDGETSTYLETYPE_VISIBILITY));
		cssproperties.Add(GRPWIDGETSTYLETYPE_CLEAR,									new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_CLEAR, false, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_FLOAT,									new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_FLOAT, false, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_WIDTH,									new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_WIDTH, true, true, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_HEIGHT,								new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_HEIGHT, true, true, GRPWIDGETSTYLETYPE_HEIGHT));
		cssproperties.Add(GRPWIDGETSTYLETYPE_MAX_WIDTH,							new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_MAX_WIDTH, true, true, GRPWIDGETSTYLETYPE_MAX_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_MAX_HEIGHT,						new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_MAX_HEIGHT, true, true, GRPWIDGETSTYLETYPE_MAX_HEIGHT));
		cssproperties.Add(GRPWIDGETSTYLETYPE_TOP,										new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_TOP, false, true, GRPWIDGETSTYLETYPE_HEIGHT));
		cssproperties.Add(GRPWIDGETSTYLETYPE_LEFT,									new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_LEFT, false, true, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_POSITION,							new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_POSITION, false, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_OPACITY,								new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_OPACITY, false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BACKGROUND_COLOR,			new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BACKGROUND_COLOR, false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BACKGROUND_IMAGE,			new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BACKGROUND_IMAGE, false, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BACKGROUND_SIZE,				new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BACKGROUND_SIZE, false, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BACKGROUND_SIZE_X,			new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BACKGROUND_SIZE_X, false, true, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BACKGROUND_SIZE_Y,			new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BACKGROUND_SIZE_Y, false, true, GRPWIDGETSTYLETYPE_HEIGHT));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION,		new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION, false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_X, new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_X, false, true, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_Y, new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_Y, false, true, GRPWIDGETSTYLETYPE_HEIGHT));
		cssproperties.Add(GRPWIDGETSTYLETYPE_CONTENT,								new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_CONTENT, false, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_MARGIN,								new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_MARGIN, false, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_MARGIN_TOP,						new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_MARGIN_TOP, false, true, GRPWIDGETSTYLETYPE_HEIGHT));
		cssproperties.Add(GRPWIDGETSTYLETYPE_MARGIN_LEFT,						new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_MARGIN_LEFT, false, true, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_MARGIN_BOTTOM,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_MARGIN_BOTTOM, false, true, GRPWIDGETSTYLETYPE_HEIGHT));
		cssproperties.Add(GRPWIDGETSTYLETYPE_MARGIN_RIGHT,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_MARGIN_RIGHT, false, true, GRPWIDGETSTYLETYPE_WIDTH));

		cssproperties.Add(GRPWIDGETSTYLETYPE_PADDING,								new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_PADDING, false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_PADDING_TOP,						new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_PADDING_TOP, false, true, GRPWIDGETSTYLETYPE_HEIGHT));
		cssproperties.Add(GRPWIDGETSTYLETYPE_PADDING_BOTTOM,				new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_PADDING_BOTTOM, false, true, GRPWIDGETSTYLETYPE_HEIGHT));
		cssproperties.Add(GRPWIDGETSTYLETYPE_PADDING_LEFT,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_PADDING_LEFT, false, true, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_PADDING_RIGHT,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_PADDING_RIGHT, false, true, GRPWIDGETSTYLETYPE_WIDTH));

		cssproperties.Add(GRPWIDGETSTYLETYPE_FONT_COLOR,						new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_FONT_COLOR, true, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_FONT_SIZE,							new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_FONT_SIZE, true, true, GRPWIDGETSTYLETYPE_FONT_SIZE));
		cssproperties.Add(GRPWIDGETSTYLETYPE_FONT_FACE,							new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_FONT_FACE, true, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_FONT_STYLE,						new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_FONT_STYLE, true, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_FONT_WEIGHT,						new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_FONT_WEIGHT, true, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_FONT_LINE_HEIGHT,			new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_FONT_LINE_HEIGHT, true, true, GRPWIDGETSTYLETYPE_FONT_LINE_HEIGHT));

		cssproperties.Add(GRPWIDGETSTYLETYPE_TEXT_ALIGN,						new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_TEXT_ALIGN, true, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN,				new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN, true, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_OVERFLOW,							new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_OVERFLOW, false, false));

		cssproperties.Add(GRPWIDGETSTYLETYPE_WHITESPACE,						new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_WHITESPACE,						true, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_WORD_SPACING,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_WORD_SPACING,					true, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_FONT_INDENT,						new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_FONT_INDENT,					true, true, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_FONT_DECORATION,				new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_FONT_DECORATION,			false, false));

		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER,								new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER,								false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_RADIUS,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_RADIUS,				false, true, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_COLOR,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_COLOR,					false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_STYLE,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_STYLE,					false, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_WIDTH,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_WIDTH,					false, true));

		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_BOTTOM,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_BOTTOM,				false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_BOTTOM_RADIUS,	new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_BOTTOM_RADIUS, false, true, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_BOTTOM_COLOR,		new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_BOTTOM_COLOR,	false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_BOTTOM_STYLE,		new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_BOTTOM_STYLE,	false, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_BOTTOM_WIDTH,		new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_BOTTOM_WIDTH,	false, true));

		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_TOP,						new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_TOP,						false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_TOP_RADIUS,			new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_TOP_RADIUS,		false, true, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_TOP_COLOR,			new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_TOP_COLOR,			false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_TOP_STYLE,			new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_TOP_STYLE,			false, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_TOP_WIDTH,			new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_TOP_WIDTH,			false, true));

		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_LEFT,						new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_LEFT,					false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_LEFT_RADIUS,		new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_LEFT_RADIUS,		false, true, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_LEFT_COLOR,			new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_LEFT_COLOR,		false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_LEFT_STYLE,			new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_LEFT_STYLE,		false, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_LEFT_WIDTH,			new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_LEFT_WIDTH,		false, true));

		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_RIGHT,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_RIGHT,					false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_RIGHT_RADIUS,		new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_RIGHT_RADIUS,	false, true, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_RIGHT_COLOR,		new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_RIGHT_COLOR,		false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_RIGHT_STYLE,		new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_RIGHT_STYLE,		false, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BORDER_RIGHT_WIDTH,		new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BORDER_RIGHT_WIDTH,		false, true));

		cssproperties.Add(GRPWIDGETSTYLETYPE_BOXSHADOW,							new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BOXSHADOW,						false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BOXSHADOW_POSITIONX,		new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BOXSHADOW_POSITIONX,	false, true, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BOXSHADOW_POSITIONY,		new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BOXSHADOW_POSITIONY,	false, true, GRPWIDGETSTYLETYPE_HEIGHT));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BOXSHADOW_BLUR,				new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BOXSHADOW_BLUR,				false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_BOXSHADOW_SPREAD,			new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_BOXSHADOW_SPREAD,			false, true));

		cssproperties.Add(GRPWIDGETSTYLETYPE_OUTLINE,								new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_OUTLINE,							false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_OUTLINE_COLOR,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_OUTLINE_COLOR,				false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_OUTLINE_WIDTH,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_OUTLINE_WIDTH,				false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_OUTLINE_STYLE,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_OUTLINE_STYLE,				false, true));		
		cssproperties.Add(GRPWIDGETSTYLETYPE_OUTLINE_OFFSET,				new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_OUTLINE_OFFSET,				false, true));

		cssproperties.Add(GRPWIDGETSTYLETYPE_TEXTSHADOW,						new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_TEXTSHADOW,						false, true));
		cssproperties.Add(GRPWIDGETSTYLETYPE_TEXTSHADOW_POSITIONX,	new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_TEXTSHADOW_POSITIONX, false, true, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_TEXTSHADOW_POSITIONY,	new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_TEXTSHADOW_POSITIONY, false, true, GRPWIDGETSTYLETYPE_HEIGHT));
		cssproperties.Add(GRPWIDGETSTYLETYPE_TEXTSHADOW_BLUR,				new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_TEXTSHADOW_BLUR,			false, true));
		
		cssproperties.Add(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING,	new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING, true, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_X,new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING, true, false, GRPWIDGETSTYLETYPE_WIDTH));
		cssproperties.Add(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_Y,new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING, true, false, GRPWIDGETSTYLETYPE_HEIGHT));

		cssproperties.Add(GRPWIDGETSTYLETYPE_TABLE_ROWSPAN,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_TABLE_ROWSPAN,				false, false));
		cssproperties.Add(GRPWIDGETSTYLETYPE_TABLE_COLSPAN,					new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_TABLE_COLSPAN,				false, false));

		cssproperties.Add(GRPWIDGETSTYLETYPE_ISTEXT,								new GRPWIDGETSTYLE(GRPWIDGETSTYLETYPE_ISTEXT,								false, false));

		;
		return true;
}


/*-------------------------------------------------------------------
//	 GRPSTYLEPROCESSORCSS::GetPalette
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	26/02/2018 10:44:30
//	@return		GRPCOLORPALETTE* : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
GRPCOLORPALETTE*  GRPSTYLEPROCESSORCSS::GetPalette()
{
		if (!GRPSTYLEPROCESSORCSS::csscolorpalette)
				 GRPSTYLEPROCESSORCSS::csscolorpalette = new GRPCOLORPALETTE();

		return GRPSTYLEPROCESSORCSS::csscolorpalette;
}