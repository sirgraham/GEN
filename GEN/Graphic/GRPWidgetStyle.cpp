/*------------------------------------------------------------------------------------------
// GRPWIDGETSTYLE.CPP
*/
/**
// \class
//
// Manages widget styles
//
//	@author: Diego Martinez Ruiz de Gaona 
//
//	Date of Creation : 15/11/2017 18:12:52 
//	Last Modification : 
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPFont.h"
#include "GRPWidgetStyle.h"
#include "GRPWidget.h"
#include "GRPMaterialLibrary.h"
#include "GRPWidgetPass.h"
#include "GRPWidgetPage.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	GRPWIDGETSTYLECLASS::ApplyTo
*/
/**
//
//
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/08/2017 13:36:37
//
//	@return 			bool :
//
//  @param				GRPWIDGET* :
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGETSTYLECLASS::ApplyTo(GRPWIDGET* w)
{
		if (this->styles.GetSize() == 0)
				return true;

		//----------------------------- display
		if (this->styles.Find(GRPWIDGETSTYLETYPE_DISPLAY) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_DISPLAY);
				
				if (s->value.Compare(__L("block")) == 0)				
						w->Set(GRPWIDGETSTYLETYPE_DISPLAY, GRPWIDGETSTYLE_DISPLAY_TYPE_BLOCK);										
				else
				if (s->value.Compare(__L("inline")) == 0)				
								w->Set(GRPWIDGETSTYLETYPE_DISPLAY, GRPWIDGETSTYLE_DISPLAY_TYPE_INLINE);				
				else
				if (s->value.Compare(__L("hidden")) == 0)
						w->Set(GRPWIDGETSTYLETYPE_DISPLAY, GRPWIDGETSTYLE_DISPLAY_TYPE_HIDDEN);
				else
				if (s->value.Compare(__L("initial")) == 0)
						w->Set(GRPWIDGETSTYLETYPE_DISPLAY, GRPWIDGETSTYLE_DISPLAY_TYPE_INITIAL);
				else
				if (s->value.Compare(__L("inline-block")) == 0)				
						w->Set(GRPWIDGETSTYLETYPE_DISPLAY, GRPWIDGETSTYLE_DISPLAY_TYPE_INLINEBLOCK);														
				else
				if (s->value.Compare(__L("list-item")) == 0)
						w->Set(GRPWIDGETSTYLETYPE_DISPLAY, GRPWIDGETSTYLE_DISPLAY_TYPE_LIST_ITEM);
				else
				if (s->value.Compare(__L("none")) == 0)				
						w->Set(GRPWIDGETSTYLETYPE_DISPLAY, GRPWIDGETSTYLE_DISPLAY_TYPE_HIDDEN);												
				else
				if (s->value.Compare(__L("table")) == 0)
						w->Set(GRPWIDGETSTYLETYPE_DISPLAY, GRPWIDGETSTYLE_DISPLAY_TYPE_TABLE);
				else
				if (s->value.Compare(__L("table-row")) == 0)
						w->Set(GRPWIDGETSTYLETYPE_DISPLAY, GRPWIDGETSTYLE_DISPLAY_TYPE_TABLE_ROW);
				else
				if (s->value.Compare(__L("table-cell")) == 0)
						w->Set(GRPWIDGETSTYLETYPE_DISPLAY, GRPWIDGETSTYLE_DISPLAY_TYPE_TABLE_CELL);
		}

		//---------------------------- visibility
		if (this->styles.Find(GRPWIDGETSTYLETYPE_VISIBILITY) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_VISIBILITY);

				if (s->value.Compare(__L("visible")) == 0)
						w->Set(GRPWIDGETSTYLETYPE_VISIBILITY, GRPWIDGETSTYLE_VISIBILITY_VISIBLE);
				else
				if (s->value.Compare(__L("hidden")) == 0)
						w->Set(GRPWIDGETSTYLETYPE_VISIBILITY, GRPWIDGETSTYLE_VISIBILITY_HIDDEN);
				else
				if (s->value.Compare(__L("inherit")) == 0)
						w->Set(GRPWIDGETSTYLETYPE_VISIBILITY, GRPWIDGETSTYLE_VISIBILITY_INHERIT);
				else
				if (s->value.Compare(__L("collapse")) == 0)
						w->Set(GRPWIDGETSTYLETYPE_VISIBILITY, GRPWIDGETSTYLE_VISIBILITY_COLLAPSE);
				else
				if (s->value.Compare(__L("initial")) == 0)
						w->Set(GRPWIDGETSTYLETYPE_VISIBILITY, GRPWIDGETSTYLE_VISIBILITY_INITIAL);
		}

		//---------------------------- float
		if (this->styles.Find(GRPWIDGETSTYLETYPE_FLOAT) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_FLOAT);
			
				if (s->value.Compare(__L("left")) == 0)
						w->Set(GRPWIDGETSTYLETYPE_FLOAT, GRPWIDGETSTYLE_FLOAT_TYPE_LEFT);						
				else
				if (s->value.Compare(__L("right")) == 0)
						w->Set(GRPWIDGETSTYLETYPE_FLOAT, GRPWIDGETSTYLE_FLOAT_TYPE_RIGHT);
				else
				if (s->value.Compare(__L("none")) == 0)
						w->Set(GRPWIDGETSTYLETYPE_FLOAT, GRPWIDGETSTYLE_FLOAT_TYPE_NONE);		
				else
				if (s->value.Compare(__L("both")) == 0)				
						w->Set(GRPWIDGETSTYLETYPE_FLOAT, GRPWIDGETSTYLE_FLOAT_TYPE_BOTH);				
				else
						w->Set(GRPWIDGETSTYLETYPE_FLOAT, GRPWIDGETSTYLE_FLOAT_TYPE_INHERIT);
		}

		//---------------------------- width
		if (this->styles.Find(GRPWIDGETSTYLETYPE_WIDTH) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_WIDTH);
				w->Set(GRPWIDGETSTYLETYPE_WIDTH, s->value.ConvertToInt(), s->valuetype);
				float width=(float)w->Get(GRPWIDGETSTYLETYPE_WIDTH);
				if (width != w->boundingbox.width)
				{
						w->boundingbox.width = width;
						w->ismodified |= GRPWIDGETSECTION_FLAG_BACKGROUND;
						w->ismodified |= GRPWIDGETSECTION_FLAG_CONTENT;
				}

		}		

		//----------------------------- clear

		if (this->styles.Find(GRPWIDGETSTYLETYPE_CLEAR) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_CLEAR);
								
				if (s->value.Compare(__L("left"))  == 0) { w->Set(GRPWIDGETSTYLETYPE_CLEAR, GRPWIDGETSTYLE_FLOAT_TYPE_LEFT); }else
				if (s->value.Compare(__L("right")) == 0) { w->Set(GRPWIDGETSTYLETYPE_CLEAR, GRPWIDGETSTYLE_FLOAT_TYPE_RIGHT);}else
				if (s->value.Compare(__L("none"))  == 0) { w->Set(GRPWIDGETSTYLETYPE_CLEAR, GRPWIDGETSTYLE_FLOAT_TYPE_NONE); }else
				if (s->value.Compare(__L("both"))  == 0) { w->Set(GRPWIDGETSTYLETYPE_CLEAR, GRPWIDGETSTYLE_FLOAT_TYPE_BOTH); }
		}


		//---------------------------- position
		if (this->styles.Find(GRPWIDGETSTYLETYPE_POSITION) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_POSITION);
				if (s->value.Compare(__L("absolute")) == 0)
				{
						w->node->Dettach();
						w->node->SetParent(w->root->node);
						w->Set(GRPWIDGETSTYLETYPE_POSITION, GRPWIDGETSTYLE_POSITION_TYPE_ABSOLUTE);
				}
				else
				if (s->value.Compare(__L("relative")) == 0)				
						w->Set(GRPWIDGETSTYLETYPE_POSITION, GRPWIDGETSTYLE_POSITION_TYPE_RELATIVE);						
				else
				if (s->value.Compare(__L("inherit")) == 0)				
						w->Set(GRPWIDGETSTYLETYPE_POSITION, GRPWIDGETSTYLE_POSITION_TYPE_INHERIT);						
				
				if (w->parent)
						w->parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;

		}

		//---------------------------- left
		if (this->styles.Find(GRPWIDGETSTYLETYPE_LEFT) != NOTFOUND)
		{			
						GRPWIDGETSTYLE* s		= this->styles.Get(GRPWIDGETSTYLETYPE_LEFT);
						XDWORD x						= s->value.ConvertToInt();
						XDWORD old= w->Get(GRPWIDGETSTYLETYPE_LEFT);
						if (x != old)
						{
								w->Set(GRPWIDGETSTYLETYPE_LEFT, x, s->valuetype);
								if (w->parent)
										w->parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
						}
		}

		//---------------------------- top
		if (this->styles.Find(GRPWIDGETSTYLETYPE_TOP) != NOTFOUND)
		{				
						GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_TOP);
						XDWORD y = s->value.ConvertToInt();
						XDWORD old = w->Get(GRPWIDGETSTYLETYPE_TOP);
						if (y != old)
						{
								w->Set(GRPWIDGETSTYLETYPE_TOP, y, s->valuetype);
								if (w->parent)
										w->parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
						}
		}

		//---------- padding
		if (this->styles.Find(GRPWIDGETSTYLETYPE_PADDING) != NOTFOUND)
		{
				WIDGETSTRING translate;
				XVECTOR<WIDGETSTRING*>explode;
				GRPWIDGETSTYLE* style = this->styles.Get(GRPWIDGETSTYLETYPE_PADDING);
				style->value.Explode(__C(' '), &explode);

				if (explode.GetSize() == 1)
				{
						float value = 0;
						GRPWIDGETSTYLEVALUETYPE type = GRPWIDGETSTYLEVALUETYPE_UNSET;
						style->ParseValue(explode.Get(0), value, type);
						w->Set(GRPWIDGETSTYLETYPE_PADDING,				(XDWORD)value, type);
						w->Set(GRPWIDGETSTYLETYPE_PADDING_TOP,    (XDWORD)value, type);
						w->Set(GRPWIDGETSTYLETYPE_PADDING_LEFT,   (XDWORD)value, type);
						w->Set(GRPWIDGETSTYLETYPE_PADDING_RIGHT,  (XDWORD)value, type);
						w->Set(GRPWIDGETSTYLETYPE_PADDING_BOTTOM, (XDWORD)value, type); 						
				}
				else
				{
						for (XDWORD e=0;e<explode.GetSize();e++)
						{
								float value = 0;
								GRPWIDGETSTYLEVALUETYPE type = GRPWIDGETSTYLEVALUETYPE_UNSET;
								style->ParseValue(explode.Get(e), value, type);
								w->Set((GRPWIDGETSTYLETYPE)(GRPWIDGETSTYLETYPE_PADDING_TOP+e), (XDWORD)value, type);
						}
				}
				explode.DeleteContents();
				explode.DeleteAll();
		}
		//---------------------------- padding top
		if (this->styles.Find(GRPWIDGETSTYLETYPE_PADDING_TOP) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_PADDING_TOP);
				GLFLOAT y = 0;
				switch (s->valuetype)
				{
				case GRPWIDGETSTYLEVALUETYPE_ABSOLUTE:		y = s->value.ConvertToFloat();															 break;
				case GRPWIDGETSTYLEVALUETYPE_RELATIVE:		y = (w->boundingbox.height / 100.0f)*(s->value.ConvertToFloat()); break;
				}
				w->boundingbox.toppadding = y;
		}
		//---------------------------- padding bottom
		if (this->styles.Find(GRPWIDGETSTYLETYPE_PADDING_BOTTOM) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_PADDING_BOTTOM);
				GLFLOAT y = 0;
				switch (s->valuetype)
				{
				case GRPWIDGETSTYLEVALUETYPE_ABSOLUTE:		y = s->value.ConvertToFloat();															 break;
				case GRPWIDGETSTYLEVALUETYPE_RELATIVE:		y = (w->boundingbox.height / 100.0f)*(s->value.ConvertToFloat()); break;
				}
				w->boundingbox.bottompadding = y;
		}
		//---------------------------- padding bottom
		if (this->styles.Find(GRPWIDGETSTYLETYPE_PADDING_LEFT) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_PADDING_LEFT);
				GLFLOAT x = 0;
				switch (s->valuetype)
				{
				case GRPWIDGETSTYLEVALUETYPE_ABSOLUTE:		x = s->value.ConvertToFloat();															 break;
				case GRPWIDGETSTYLEVALUETYPE_RELATIVE:		x = (w->boundingbox.width / 100.0f)*(s->value.ConvertToFloat()); break;
				}
				w->boundingbox.leftpadding = x;
		}

		//---------------------------- padding bottom
		if (this->styles.Find(GRPWIDGETSTYLETYPE_PADDING_RIGHT) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_PADDING_RIGHT);
				GLFLOAT x = 0;
				switch (s->valuetype)
				{
				case GRPWIDGETSTYLEVALUETYPE_ABSOLUTE:		x = s->value.ConvertToFloat();															 break;
				case GRPWIDGETSTYLEVALUETYPE_RELATIVE:		x = (w->boundingbox.width / 100.0f)*(s->value.ConvertToFloat()); break;
				}
				w->boundingbox.rightpadding = x;
		}

		//---------------------------- content
		if (this->styles.Find(GRPWIDGETSTYLETYPE_CONTENT) != NOTFOUND)
		{
				XFSTRING url;
				url.AdjustSize(500);
				XFSTRING value;
				value = this->styles.Get(GRPWIDGETSTYLETYPE_CONTENT)->value;
				int n=value.scan(__L("url('%[^']ls')"), url.Get());

				//if (n == 1)
				{
						XPATH path = *GRPMATERIALLIBRARY::Get()->GetResourceLocation();
						path.Add(url.Get());
						if (GRPMATERIALLIBRARY::Get()->Load(path.Get(), url.Get()))
						{
								GRPMATERIAL* material = GRPMATERIALLIBRARY::Get()->Select(url.Get());
								if (material)
								{
										if (!w->materials.Set(GRPWIDGETSECTION_CONTENT, material))
												w->materials.Add(GRPWIDGETSECTION_CONTENT, material);

										if (!w->Has(GRPWIDGETSTYLETYPE_WIDTH))
												w->boundingbox.width = material->GetLayer(0)->GetWidth();
										if (!w->Has(GRPWIDGETSTYLETYPE_HEIGHT))
												w->boundingbox.height = material->GetLayer(0)->GetHeight();
										w->texturex = w->boundingbox.width;
										w->texturey = w->boundingbox.height;
										w->ismodified |= GRPWIDGETSECTION_FLAG_CONTENT;
								}
						}
						else
						{
								XDEBUG_PRINTCOLOR(4, __L("Failed loading content image %s"), path.Get());
						}
				}
			/*
			else
				{
						XDEBUG_PRINTCOLOR(4, __L("Failed parsing content image"));
				}
				*/
		}

		//---------------------------- height
		if (this->styles.Find(GRPWIDGETSTYLETYPE_HEIGHT) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_HEIGHT);				
				w->Set(GRPWIDGETSTYLETYPE_HEIGHT, s->value.ConvertToInt(), s->valuetype);
				float height = (float)w->Get(GRPWIDGETSTYLETYPE_HEIGHT);
				if (height != w->boundingbox.height)
				{
						w->boundingbox.height = height;
						w->ismodified |= GRPWIDGETSECTION_FLAG_BACKGROUND;
						w->ismodified |= GRPWIDGETSECTION_FLAG_CONTENT;
				}
		}			


		//---------------------------- background color
		if (this->styles.Find(GRPWIDGETSTYLETYPE_BACKGROUND_COLOR) != NOTFOUND)
		{				
				WIDGETSTRING color = this->styles.Get(GRPWIDGETSTYLETYPE_BACKGROUND_COLOR)->value;
				GRPCOLOR* c=ParseColor(color);
				if (c)
				{						
						delete(w->colors.Get(GRPWIDGETSECTION_BACKGROUND));
						if (!w->colors.Set(GRPWIDGETSECTION_BACKGROUND, c))
								if (!w->colors.Add(GRPWIDGETSECTION_BACKGROUND, c))
										delete(c);

						w->ismodified |= GRPWIDGETSECTION_FLAG_BACKGROUND_COLOR;
				}
		}

		//---------------------------- font color
		if (this->styles.Find(GRPWIDGETSTYLETYPE_FONT_COLOR) != NOTFOUND)
		{
				WIDGETSTRING color = this->styles.Get(GRPWIDGETSTYLETYPE_FONT_COLOR)->value;
				GRPCOLOR* c = ParseColor(color);				
				if (c)
				{
						if (w->colors.Get(GRPWIDGETSECTION_TEXT))
						{
								GRPCOLOR* oldcolor = w->colors.Get(GRPWIDGETSECTION_TEXT);
								if (!(*oldcolor==*c))
										w->ismodified |= GRPWIDGETSECTION_FLAG_COLOR;
						}
						else
								w->ismodified |= GRPWIDGETSECTION_FLAG_COLOR;
						
								delete(w->colors.Get(GRPWIDGETSECTION_TEXT));
								if (!w->colors.Set(GRPWIDGETSECTION_TEXT, c))
										w->colors.Add(GRPWIDGETSECTION_TEXT, c);														
				}
		}

		//---------------------------- font size
		if (this->styles.Find(GRPWIDGETSTYLETYPE_FONT_SIZE) != NOTFOUND)
		{
				XDWORD size = 0;
							 size = (XDWORD)this->styles.Get(GRPWIDGETSTYLETYPE_FONT_SIZE)->value.ConvertToInt();
							 if (size != w->Get(GRPWIDGETSTYLETYPE_FONT_SIZE))
							 {
									 w->ismodified |= GRPWIDGETSECTION_FLAG_TEXT;
									 w->Set(GRPWIDGETSTYLETYPE_FONT_SIZE, (XDWORD)size);
							 }				
		}

		//---------------------------- font face
		if (this->styles.Find(GRPWIDGETSTYLETYPE_FONT_FACE) != NOTFOUND)
		{				
						WIDGETSTRING s= this->styles.Get(GRPWIDGETSTYLETYPE_FONT_FACE)->value.Get();
						if (s.Compare(w->fontfamily) != 0)
						{
								  w->fontfamily = s;
									w->ismodified |= GRPWIDGETSECTION_FLAG_TEXT;
						}
		}

		//---------------------------- font style
		if (this->styles.Find(GRPWIDGETSTYLETYPE_FONT_STYLE) != NOTFOUND)
		{
				XDWORD style = w->Get(GRPWIDGETSTYLETYPE_FONT_STYLE);

				if (this->styles.Get(GRPWIDGETSTYLETYPE_FONT_STYLE)->value.Compare(__L("italic")) == 0)				
						style |= GRPFONT_STYLE_ITALIC;

				if (this->styles.Get(GRPWIDGETSTYLETYPE_FONT_STYLE)->value.Compare(__L("normal")) == 0)
						style &= ~GRPFONT_STYLE_ITALIC;

				if (this->styles.Get(GRPWIDGETSTYLETYPE_FONT_STYLE)->value.Compare(__L("regular")) == 0)
						style = GRPFONT_STYLE_REGULAR;

				w->Set(GRPWIDGETSTYLETYPE_FONT_STYLE, style);
				w->ismodified |= GRPWIDGETSECTION_FLAG_TEXT;
		}		

		//----------------------------- font weight
		if (this->styles.Find(GRPWIDGETSTYLETYPE_FONT_WEIGHT) != NOTFOUND)
		{
				XDWORD style = w->Get(GRPWIDGETSTYLETYPE_FONT_STYLE);

				if (this->styles.Get(GRPWIDGETSTYLETYPE_FONT_WEIGHT)->value.Compare(__L("bold")) == 0)
						style |= GRPFONT_STYLE_BOLD;
				if (this->styles.Get(GRPWIDGETSTYLETYPE_FONT_WEIGHT)->value.Compare(__L("bolder")) == 0)
						style |= GRPFONT_STYLE_BOLD;

				if (this->styles.Get(GRPWIDGETSTYLETYPE_FONT_WEIGHT)->value.Compare(__L("normal")) == 0)
						style &= ~GRPFONT_STYLE_BOLD;

				w->Set(GRPWIDGETSTYLETYPE_FONT_STYLE, style);
				w->ismodified |= GRPWIDGETSECTION_FLAG_TEXT;
		}

		//---------------------------- line-height
		if (this->styles.Find(GRPWIDGETSTYLETYPE_FONT_LINE_HEIGHT) != NOTFOUND)
		{
				XDWORD size = 0;
							 size = (XDWORD)this->styles.Get(GRPWIDGETSTYLETYPE_FONT_LINE_HEIGHT)->value.ConvertToInt();
				w->Set(GRPWIDGETSTYLETYPE_FONT_LINE_HEIGHT, (XDWORD)size);								
				w->ismodified |= GRPWIDGETSECTION_FLAG_TEXT;
		}


		//---------------------------- font align

		if (this->styles.Find(GRPWIDGETSTYLETYPE_TEXT_ALIGN) != NOTFOUND)
		{
		GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_TEXT_ALIGN);

		if (s->value.Compare(__L("left"))		== 0)	{ w->Set(GRPWIDGETSTYLETYPE_TEXT_ALIGN, GRPWIDGETSTYLETYPE_TEXT_ALIGN_LEFT); }
		else
		if (s->value.Compare(__L("right"))	== 0) { w->Set(GRPWIDGETSTYLETYPE_TEXT_ALIGN, GRPWIDGETSTYLETYPE_TEXT_ALIGN_RIGHT); }
		else
		if (s->value.Compare(__L("justify"))== 0) { w->Set(GRPWIDGETSTYLETYPE_TEXT_ALIGN, GRPWIDGETSTYLETYPE_TEXT_ALIGN_JUSTIFY); }
		else
		if (s->value.Compare(__L("center")) == 0) { w->Set(GRPWIDGETSTYLETYPE_TEXT_ALIGN, GRPWIDGETSTYLETYPE_TEXT_ALIGN_CENTER); }
		w->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
		}

		//---------------------------- background image
		if (this->styles.Find(GRPWIDGETSTYLETYPE_BACKGROUND_IMAGE) != NOTFOUND)
		{
				WIDGETSTRING url;
				url.AdjustSize(500);
				WIDGETSTRING value;
				value = this->styles.Get(GRPWIDGETSTYLETYPE_BACKGROUND_IMAGE)->value;
				value.UnFormat(__L("url('%[^']ls')"), url.Get());

				XPATH path = *GRPMATERIALLIBRARY::Get()->GetResourceLocation();
				path.Add(url.Get());
				if (GRPMATERIALLIBRARY::Get()->Load(path.Get(), url.Get()))
				{
						GRPMATERIAL* material = GRPMATERIALLIBRARY::Get()->Select(url.Get());
						if (material)
						{
								if (!w->materials.Set(GRPWIDGETSECTION_BACKGROUND, material))
										w->materials.Add(GRPWIDGETSECTION_BACKGROUND, material);
								w->texturex = material->GetLayer(0)->GetWidth();
								w->texturey = material->GetLayer(0)->GetHeight();																
								w->ismodified |= GRPWIDGETSECTION_FLAG_BACKGROUND_COLOR;
						}
				}
				else
				{
						XDEBUG_PRINTCOLOR(4, __L("Loading texture %s failed!"), path.Get());
				}
		}

		//----------------------------- background offset
		if (this->styles.Find(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION) != NOTFOUND)
		{
				if (w->materials.Find(GRPWIDGETSECTION_BACKGROUND) != NOTFOUND)
				{
						float width = w->materials.Get(GRPWIDGETSECTION_BACKGROUND)->GetLayer(0)->GetWidth();
						float height = w->materials.Get(GRPWIDGETSECTION_BACKGROUND)->GetLayer(0)->GetHeight();

						WIDGETSTRING size;

						float value = 0;
						GRPWIDGETSTYLEVALUETYPE type = GRPWIDGETSTYLEVALUETYPE_UNSET;
						XVECTOR<XFSTRING*>values;

						GRPWIDGETSTYLE* style = this->styles.Get(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION);						
						style->value.Explode(__C(' '), &values);

						style->ParseValue(values.Get(0), value, type);
						w->Set((GRPWIDGETSTYLETYPE)(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_X), (XDWORD)value, type);
						w->texture_offset_x = (float)value;

						style->ParseValue(values.Get(1), value, type);
						w->Set((GRPWIDGETSTYLETYPE)(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_Y), (XDWORD)value, type);											
						w->texture_offset_y = (float)value;

						if (type == GRPWIDGETSTYLEVALUETYPE_RELATIVE)
						{
								w->texture_offset_x *= width / 100.0f;
								w->texture_offset_y *= height / 100.0f;
						}

						w->Set(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_X, (XDWORD)w->texture_offset_x);
						w->Set(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_Y, (XDWORD)w->texture_offset_y);
						w->ismodified |= GRPWIDGETSECTION_FLAG_BACKGROUND;
				}
		}
		//---------------------------- background position x
		if (this->styles.Find(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_X) != NOTFOUND)
		{
				if (w->materials.Find(GRPWIDGETSECTION_BACKGROUND) != NOTFOUND)
				{
						float width = w->materials.Get(GRPWIDGETSECTION_BACKGROUND)->GetLayer(0)->GetWidth();
						float height = w->materials.Get(GRPWIDGETSECTION_BACKGROUND)->GetLayer(0)->GetHeight();

						WIDGETSTRING size;
						XDWORD x;
						this->styles.Get(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_X)->value.scan(__L("%d"), &x);

						w->texture_offset_x = (float)x;						

						if (this->styles.Get(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_X)->valuetype == GRPWIDGETSTYLEVALUETYPE_RELATIVE)
						{
								w->texture_offset_x *= width / 100.0f;								
						}

						w->Set(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_X, (XDWORD)w->texture_offset_x);						
						w->ismodified |= GRPWIDGETSECTION_FLAG_BACKGROUND;
				}
		}

		//----------------------------- background offset y		
		if (this->styles.Find(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_Y) != NOTFOUND)
		{
						WIDGETSTRING size;
						XDWORD y;
						this->styles.Get(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_Y)->value.scan(__L("%d"), &y);
						
						w->texture_offset_y = (float)y;

								if (w->materials.Find(GRPWIDGETSECTION_BACKGROUND) != NOTFOUND)
								{
												float width = w->materials.Get(GRPWIDGETSECTION_BACKGROUND)->GetLayer(0)->GetWidth();
												float height = w->materials.Get(GRPWIDGETSECTION_BACKGROUND)->GetLayer(0)->GetHeight();

										if (this->styles.Get(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_Y)->valuetype == GRPWIDGETSTYLEVALUETYPE_RELATIVE)
												w->texture_offset_y *= height / 100.0f;					
								}

						w->Set(GRPWIDGETSTYLETYPE_BACKGROUND_POSITION_Y, (XDWORD)w->texture_offset_y);
						w->ismodified |= GRPWIDGETSECTION_FLAG_BACKGROUND;

		}

		//---------------------------- background size
		if (this->styles.Find(GRPWIDGETSTYLETYPE_BACKGROUND_SIZE) != NOTFOUND)
		{
				if (w->materials.Find(GRPWIDGETSECTION_BACKGROUND) != NOTFOUND)
				{
						WIDGETSTRING size;

						float value = 0;
						GRPWIDGETSTYLEVALUETYPE type = GRPWIDGETSTYLEVALUETYPE_UNSET;
						XVECTOR<XFSTRING*>values;

						GRPWIDGETSTYLE* style = this->styles.Get(GRPWIDGETSTYLETYPE_BACKGROUND_SIZE);
						style->value.Explode(__C(' '), &values);

						style->ParseValue(values.Get(0), value, type);
						w->Set((GRPWIDGETSTYLETYPE)(GRPWIDGETSTYLETYPE_BACKGROUND_SIZE_X), (XDWORD)value, type);
						w->texturex = (float)value;

						style->ParseValue(values.Get(1), value, type);
						w->Set((GRPWIDGETSTYLETYPE)(GRPWIDGETSTYLETYPE_BACKGROUND_SIZE_Y), (XDWORD)value, type);
						w->texturey = (float)value;
						
						w->Set(GRPWIDGETSTYLETYPE_BACKGROUND_SIZE_X, (XDWORD)w->texturex);
						w->Set(GRPWIDGETSTYLETYPE_BACKGROUND_SIZE_Y, (XDWORD)w->texturey);


						float width = w->materials.Get(GRPWIDGETSECTION_BACKGROUND)->GetLayer(0)->GetWidth();
						float height = w->materials.Get(GRPWIDGETSECTION_BACKGROUND)->GetLayer(0)->GetHeight();
						
						if (this->styles.Get(GRPWIDGETSTYLETYPE_BACKGROUND_SIZE)->valuetype == GRPWIDGETSTYLEVALUETYPE_RELATIVE)
								{
										w->texturex *= width / 100.0f;
										w->texturey *= height / 100.0f;
								}
						w->ismodified |= GRPWIDGETSECTION_FLAG_BACKGROUND;
				}
		}

		//---------------------------- GRPWIDGETSTYLETYPE_OPACITY
		if (this->styles.Find(GRPWIDGETSTYLETYPE_OPACITY) != NOTFOUND)
		{
				WIDGETSTRING alpha = this->styles.Get(GRPWIDGETSTYLETYPE_OPACITY)->value;
				w->ismodified |= GRPWIDGETSECTION_FLAG_BACKGROUND;
				w->ismodified |= GRPWIDGETSECTION_FLAG_CONTENT;
		}

		//---------------------------- GRPWIDGETSTYLETYPE_MARGIN
		if (this->styles.Find(GRPWIDGETSTYLETYPE_MARGIN) != NOTFOUND)
		{
				WIDGETSTRING translate;
				XVECTOR<WIDGETSTRING*>explode;
				GRPWIDGETSTYLE* style = this->styles.Get(GRPWIDGETSTYLETYPE_MARGIN);
												style->value.Explode(__C(' '), &explode);

				delete(this->styles.Get(GRPWIDGETSTYLETYPE_MARGIN));
				this->styles.Delete(GRPWIDGETSTYLETYPE_MARGIN);

				if (explode.GetSize() == 1)
				{
						float x = explode.Get(0)->ConvertToFloat();
						w->Set(GRPWIDGETSTYLETYPE_MARGIN_TOP,		(XDWORD)x);
						w->boundingbox.topmargin = x;
						w->Set(GRPWIDGETSTYLETYPE_MARGIN_RIGHT, (XDWORD)x);
						w->boundingbox.rightmargin = x;
						w->Set(GRPWIDGETSTYLETYPE_MARGIN_BOTTOM,(XDWORD)x);
						w->boundingbox.bottommargin = x;
						w->Set(GRPWIDGETSTYLETYPE_MARGIN_LEFT, (XDWORD)x);
						w->boundingbox.leftmargin = x;					
				}
				else if (explode.GetSize() == 4)
				{
						float x = 0;
						x = explode.Get(0)->ConvertToFloat();
						w->Set(GRPWIDGETSTYLETYPE_MARGIN_TOP, (XDWORD)x);
						w->boundingbox.topmargin = x;
						x = explode.Get(1)->ConvertToFloat();
						w->Set(GRPWIDGETSTYLETYPE_MARGIN_RIGHT, (XDWORD)x);
						w->boundingbox.rightmargin = x;
						x = explode.Get(2)->ConvertToFloat();
						w->Set(GRPWIDGETSTYLETYPE_MARGIN_BOTTOM, (XDWORD)x);
						w->boundingbox.bottommargin = x;
						x = explode.Get(3)->ConvertToFloat();
						w->Set(GRPWIDGETSTYLETYPE_MARGIN_LEFT, (XDWORD)x);
						w->boundingbox.leftmargin = x;
						
				}				

				explode.DeleteContents();
				explode.DeleteAll();
				w->parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
		}

		//---------------------------- margin-top
		if (this->styles.Find(GRPWIDGETSTYLETYPE_MARGIN_TOP) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_MARGIN_TOP);				
				GLFLOAT y = 0;
				switch (s->valuetype)
				{
				case GRPWIDGETSTYLEVALUETYPE_ABSOLUTE:		y = s->value.ConvertToFloat();															 break;
				case GRPWIDGETSTYLEVALUETYPE_RELATIVE:		y = (w->parent->boundingbox.height / 100.0f)*s->value.ConvertToFloat(); break;
				}
				w->boundingbox.topmargin = y;
				if (w->parent)
				w->parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
		}
		//---------------------------- margin-bottom
		if (this->styles.Find(GRPWIDGETSTYLETYPE_MARGIN_BOTTOM) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_MARGIN_BOTTOM);
				GLFLOAT y = 0;
				switch (s->valuetype)
				{
				case GRPWIDGETSTYLEVALUETYPE_ABSOLUTE:		y = s->value.ConvertToFloat();															 break;
				case GRPWIDGETSTYLEVALUETYPE_RELATIVE:		y = (w->parent->boundingbox.height / 100.0f)*s->value.ConvertToFloat(); break;
				}
				w->boundingbox.bottommargin = y;
				if (w->parent)
				w->parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
		}

		//---------------------------- margin-left
		if (this->styles.Find(GRPWIDGETSTYLETYPE_MARGIN_LEFT) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_MARGIN_LEFT);
				const GRPVECTOR* v = w->node->GetPosition();

				GLFLOAT x = 0;
				switch (s->valuetype)
				{
				case GRPWIDGETSTYLEVALUETYPE_ABSOLUTE:		x = s->value.ConvertToFloat();															 break;
				case GRPWIDGETSTYLEVALUETYPE_RELATIVE:		x = (w->parent->boundingbox.width / 100.0f)*s->value.ConvertToFloat(); break;
				}
				w->boundingbox.leftmargin = x;
				if (w->parent)
				w->parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
		}

		//---------------------------- margin-right
		if (this->styles.Find(GRPWIDGETSTYLETYPE_MARGIN_RIGHT) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_MARGIN_RIGHT);
				const GRPVECTOR* v = w->node->GetPosition();

				GLFLOAT x = 0;
				switch (s->valuetype)
				{
						case GRPWIDGETSTYLEVALUETYPE_ABSOLUTE:		x = s->value.ConvertToFloat();																	break;
						case GRPWIDGETSTYLEVALUETYPE_RELATIVE:		x = (w->parent->boundingbox.width / 100.0f)*s->value.ConvertToFloat();	break;
				}
				w->boundingbox.rightmargin = x;
				if (w->parent)
				w->parent->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
		}

		//--------------------- vertical align

		if (this->styles.Find(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN);				

				if (s->value.Compare(__L("baseline"))		== 0) { w->Set(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN, GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_BASELINE); }
				else
				if (s->value.Compare(__L("sub"))				== 0) { w->Set(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN, GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_SUB); }
				else
				if (s->value.Compare(__L("super"))			== 0) { w->Set(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN, GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_SUPER); }
				else
				if (s->value.Compare(__L("top"))				== 0) { w->Set(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN, GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_TOP); }
				else
				if (s->value.Compare(__L("text-top"))		== 0) { w->Set(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN, GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_TEXT_TOP); }
				else
				if (s->value.Compare(__L("middle"))			== 0) { w->Set(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN, GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_MIDDLE); }
				else
				if (s->value.Compare(__L("bottom"))			== 0) { w->Set(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN, GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_BOTTOM); }
				else
				if (s->value.Compare(__L("text-bottom"))== 0) { w->Set(GRPWIDGETSTYLETYPE_VERTICAL_ALIGN, GRPWIDGETSTYLETYPE_VERTICAL_ALIGN_TEXT_BOTTOM); }

				w->ismodified |= GRPWIDGETSECTION_FLAG_FLOW;
		}

		//---------------- Overflow
		if (this->styles.Find(GRPWIDGETSTYLETYPE_OVERFLOW) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_OVERFLOW);

				if (s->value.Compare(__L("hidden"))   == 0) { w->Set(GRPWIDGETSTYLETYPE_OVERFLOW, GRPWIDGETSTYLE_OVERFLOW_TYPE_HIDDEN); }
				else
				if (s->value.Compare(__L("scroll")) == 0) { w->Set(GRPWIDGETSTYLETYPE_OVERFLOW, GRPWIDGETSTYLE_OVERFLOW_TYPE_SCROLL); }
				else
				if (s->value.Compare(__L("visible")) == 0) { w->Set(GRPWIDGETSTYLETYPE_OVERFLOW, GRPWIDGETSTYLE_OVERFLOW_TYPE_VISIBLE); }
				else
				if (s->value.Compare(__L("inherit")) == 0) { w->Set(GRPWIDGETSTYLETYPE_OVERFLOW, GRPWIDGETSTYLE_OVERFLOW_TYPE_INHERIT); }
		}
		else
				w->Set(GRPWIDGETSTYLETYPE_OVERFLOW, GRPWIDGETSTYLE_OVERFLOW_TYPE_VISIBLE);

		
		//-----------------------------FONT_DECORATION

		if (this->styles.Find(GRPWIDGETSTYLETYPE_FONT_DECORATION)!=NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_FONT_DECORATION);
				if (s->value.Compare(__L("underline")) == 0) { w->Set(GRPWIDGETSTYLETYPE_FONT_DECORATION, GRPWIDGETSTYLE_FONT_DECORATION_UNDERLINE); }
				else
				if (s->value.Compare(__L("stroke")) == 0)		 { w->Set(GRPWIDGETSTYLETYPE_FONT_DECORATION, GRPWIDGETSTYLE_FONT_DECORATION_STROKE); }
				else
				if (s->value.Compare(__L("line-through")) == 0) { w->Set(GRPWIDGETSTYLETYPE_FONT_DECORATION, GRPWIDGETSTYLE_FONT_DECORATION_STROKE); }
				else
				if (s->value.Compare(__L("overline")) == 0) { w->Set(GRPWIDGETSTYLETYPE_FONT_DECORATION, GRPWIDGETSTYLE_FONT_DECORATION_OVERLINE); }
				else
				if (s->value.Compare(__L("inherit")) == 0) { w->Set(GRPWIDGETSTYLETYPE_FONT_DECORATION, 0); }
				else
				if (s->value.Compare(__L("none")) == 0) { w->Set(GRPWIDGETSTYLETYPE_FONT_DECORATION, GRPWIDGETSTYLE_FONT_DECORATION_NONE); }				
		}

		//---------------------------- OUTLINE		
		if (this->styles.Find(GRPWIDGETSTYLETYPE_OUTLINE) != NOTFOUND)
		{
				w->Set(GRPWIDGETSTYLETYPE_OUTLINE, 1);
				
				GRPWIDGETSTYLE* style = this->styles.Get(GRPWIDGETSTYLETYPE_OUTLINE);
				w->ismodified |= GRPWIDGETSECTION_FLAG_OUTLINE;

				XVECTOR<WIDGETSTRING*> values;
				style->value.Explode(__C(' '), &values);

				float value = 0;
				GRPWIDGETSTYLEVALUETYPE type = GRPWIDGETSTYLEVALUETYPE_UNSET;

				//----- border width
				style->ParseValue(values.Get(0), value, type);
				w->Set((GRPWIDGETSTYLETYPE)(GRPWIDGETSTYLETYPE_OUTLINE_WIDTH), (XDWORD)value, type);

				//----- border style
				WIDGETSTRING* borderstyle = values.Get(1);
				if (borderstyle->Compare(__L("hidden")) == 0) { w->Set(GRPWIDGETSTYLETYPE_OUTLINE_STYLE, GRPWIDGETSTYLE_BORDER_STYLE_TYPE_HIDDEN); }
				else
				if (borderstyle->Compare(__L("solid")) == 0) { w->Set(GRPWIDGETSTYLETYPE_OUTLINE_STYLE, GRPWIDGETSTYLE_BORDER_STYLE_TYPE_SOLID); }
				else
				if (borderstyle->Compare(__L("inherit")) == 0) { w->Set(GRPWIDGETSTYLETYPE_OUTLINE_STYLE, GRPWIDGETSTYLE_BORDER_STYLE_TYPE_INHERIT); }
				else
				if (borderstyle->Compare(__L("none")) == 0) { w->Set(GRPWIDGETSTYLETYPE_OUTLINE_STYLE, GRPWIDGETSTYLE_BORDER_STYLE_TYPE_NONE); }

				//----- border color
				WIDGETSTRING color = *values.Get(2);
				GRPCOLOR* c = ParseColor(color);
				if (c)
				{
						if (w->colors.Get(GRPWIDGETSECTION_OUTLINE))
						{
								GRPCOLOR* oldcolor = w->colors.Get(GRPWIDGETSECTION_OUTLINE);
								if (!(*oldcolor == *c))
										w->ismodified |= GRPWIDGETSECTION_FLAG_OUTLINE_COLOR;
						}
						else
								w->ismodified |= GRPWIDGETSECTION_FLAG_OUTLINE_COLOR;

						delete(w->colors.Get(GRPWIDGETSECTION_OUTLINE));
						if (!w->colors.Set(GRPWIDGETSECTION_OUTLINE, c))
								w->colors.Add(GRPWIDGETSECTION_OUTLINE, c);
				}

				values.DeleteContents();
				values.DeleteAll();

				if (this->styles.Find(GRPWIDGETSTYLETYPE_OUTLINE_OFFSET) != NOTFOUND)
				{
						GRPWIDGETSTYLE* style = this->styles.Get(GRPWIDGETSTYLETYPE_OUTLINE_OFFSET);
						w->ismodified |= GRPWIDGETSECTION_FLAG_OUTLINE;

						float value = 0;
						GRPWIDGETSTYLEVALUETYPE type = GRPWIDGETSTYLEVALUETYPE_UNSET;

						//----- border offset
						style->ParseValue(&style->value, value, type);
						w->Set((GRPWIDGETSTYLETYPE)(GRPWIDGETSTYLETYPE_OUTLINE_OFFSET), (XDWORD)value, type);

				}
		}

		

		//---------------------------- BORDER
		if (this->styles.Find(GRPWIDGETSTYLETYPE_BORDER)!=NOTFOUND)
		{
				w->Set(GRPWIDGETSTYLETYPE_BORDER_TOP,    1);
				w->Set(GRPWIDGETSTYLETYPE_BORDER_BOTTOM, 1);
				w->Set(GRPWIDGETSTYLETYPE_BORDER_LEFT,   1);
				w->Set(GRPWIDGETSTYLETYPE_BORDER_RIGHT,  1);
	
				GRPWIDGETSTYLE* style = this->styles.Get(GRPWIDGETSTYLETYPE_BORDER);
			
				XVECTOR<WIDGETSTRING*> values;
				style->value.Explode(__C(' '), &values);

						float value = 0;
						GRPWIDGETSTYLEVALUETYPE type = GRPWIDGETSTYLEVALUETYPE_UNSET;
						
						//----- border width
						style->ParseValue(values.Get(0), value, type);
						w->Set((GRPWIDGETSTYLETYPE)(GRPWIDGETSTYLETYPE_BORDER_WIDTH), (XDWORD)value, type);
						
						//----- border style
						WIDGETSTRING* borderstyle = values.Get(1);
						if (borderstyle->Compare(__L("hidden")) == 0) { w->Set(GRPWIDGETSTYLETYPE_BORDER_STYLE, GRPWIDGETSTYLE_BORDER_STYLE_TYPE_HIDDEN); }
						else
						if (borderstyle->Compare(__L("solid")) == 0) { w->Set(GRPWIDGETSTYLETYPE_BORDER_STYLE, GRPWIDGETSTYLE_BORDER_STYLE_TYPE_SOLID); }
						else
						if (borderstyle->Compare(__L("inherit")) == 0) { w->Set(GRPWIDGETSTYLETYPE_BORDER_STYLE, GRPWIDGETSTYLE_BORDER_STYLE_TYPE_INHERIT); }
						else
						if (borderstyle->Compare(__L("none")) == 0) { w->Set(GRPWIDGETSTYLETYPE_BORDER_STYLE, GRPWIDGETSTYLE_BORDER_STYLE_TYPE_NONE); }												

						//----- border color
						WIDGETSTRING color = *values.Get(2);
						GRPCOLOR* c = ParseColor(color);
						if (c)
						{
								if (w->colors.Get(GRPWIDGETSECTION_BORDER))
								{
										GRPCOLOR* oldcolor = w->colors.Get(GRPWIDGETSECTION_BORDER);
										if (!(*oldcolor == *c))
												w->ismodified |= GRPWIDGETSECTION_FLAG_BORDER_COLOR;
								}
								else
										w->ismodified |= GRPWIDGETSECTION_FLAG_BORDER_COLOR;

								delete(w->colors.Get(GRPWIDGETSECTION_BORDER));
								if (!w->colors.Set(GRPWIDGETSECTION_BORDER, c))
										w->colors.Add(GRPWIDGETSECTION_BORDER, c);
						}
						values.DeleteContents();
						values.DeleteAll();
				
		}

		if (this->styles.Find(GRPWIDGETSTYLETYPE_BORDER_RADIUS) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_BORDER_RADIUS);
				int value = s->value.ConvertToInt();
				w->Set(GRPWIDGETSTYLETYPE_BORDER_RADIUS, value);
		}

		if (this->styles.Find(GRPWIDGETSTYLETYPE_BORDER_WIDTH) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_BORDER_WIDTH);
				int value = s->value.ConvertToInt();

				w->boundingbox.topborder = (float)value;
				w->boundingbox.bottomborder = (float)value;
				w->boundingbox.leftborder = (float)value;
				w->boundingbox.rightborder = (float)value;
				w->Set(GRPWIDGETSTYLETYPE_BORDER_WIDTH, value);
				w->ismodified |= GRPWIDGETSECTION_FLAG_BORDER;

				w->Set(GRPWIDGETSTYLETYPE_BORDER_TOP, 1);
				w->Set(GRPWIDGETSTYLETYPE_BORDER_BOTTOM, 1);
				w->Set(GRPWIDGETSTYLETYPE_BORDER_LEFT, 1);
				w->Set(GRPWIDGETSTYLETYPE_BORDER_RIGHT, 1);
		}

		if (this->styles.Find(GRPWIDGETSTYLETYPE_BORDER_TOP_WIDTH) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_BORDER_TOP_WIDTH);
				int value = s->value.ConvertToInt();

				w->boundingbox.topborder = (float)value;				
				w->Set(GRPWIDGETSTYLETYPE_BORDER_WIDTH, value);
				w->ismodified |= GRPWIDGETSECTION_FLAG_BORDER;

				w->Set(GRPWIDGETSTYLETYPE_BORDER_TOP, 1);				
		}
		if (this->styles.Find(GRPWIDGETSTYLETYPE_BORDER_LEFT_WIDTH) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_BORDER_LEFT_WIDTH);
				int value = s->value.ConvertToInt();

				w->boundingbox.leftborder = (float)value;
				w->Set(GRPWIDGETSTYLETYPE_BORDER_WIDTH, value);
				w->ismodified |= GRPWIDGETSECTION_FLAG_BORDER;

				w->Set(GRPWIDGETSTYLETYPE_BORDER_LEFT, 1);
		}

		if (this->styles.Find(GRPWIDGETSTYLETYPE_BORDER_BOTTOM_WIDTH) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_BORDER_BOTTOM_WIDTH);
				int value = s->value.ConvertToInt();

				w->boundingbox.bottomborder = (float)value;
				w->Set(GRPWIDGETSTYLETYPE_BORDER_WIDTH, value);
				w->ismodified |= GRPWIDGETSECTION_FLAG_BORDER;

				w->Set(GRPWIDGETSTYLETYPE_BORDER_BOTTOM, 1);
		}

		if (this->styles.Find(GRPWIDGETSTYLETYPE_BORDER_RIGHT_WIDTH) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_BORDER_RIGHT_WIDTH);
				int value = s->value.ConvertToInt();

				w->boundingbox.rightborder = (float)value;
				w->Set(GRPWIDGETSTYLETYPE_BORDER_WIDTH, value);
				w->ismodified |= GRPWIDGETSECTION_FLAG_BORDER;

				w->Set(GRPWIDGETSTYLETYPE_BORDER_RIGHT, 1);
		}

		if (this->styles.Find(GRPWIDGETSTYLETYPE_BORDER_STYLE) != NOTFOUND)
		{
				GRPWIDGETSTYLE* s = this->styles.Get(GRPWIDGETSTYLETYPE_BORDER_STYLE);
				if (s->value.Compare(__L("hidden")) == 0) { w->Set(GRPWIDGETSTYLETYPE_BORDER_STYLE, GRPWIDGETSTYLE_BORDER_STYLE_TYPE_HIDDEN); }
				else
				if (s->value.Compare(__L("solid")) == 0) { w->Set(GRPWIDGETSTYLETYPE_BORDER_STYLE, GRPWIDGETSTYLE_BORDER_STYLE_TYPE_SOLID); }
				else				
				if (s->value.Compare(__L("inherit")) == 0) { w->Set(GRPWIDGETSTYLETYPE_BORDER_STYLE, GRPWIDGETSTYLE_BORDER_STYLE_TYPE_INHERIT); }
				else
				if (s->value.Compare(__L("none")) == 0) { w->Set(GRPWIDGETSTYLETYPE_BORDER_STYLE, GRPWIDGETSTYLE_BORDER_STYLE_TYPE_NONE); }
				w->ismodified |= GRPWIDGETSECTION_FLAG_BORDER;
		}

		if (this->styles.Find(GRPWIDGETSTYLETYPE_BORDER_COLOR) != NOTFOUND)
		{
				WIDGETSTRING color = this->styles.Get(GRPWIDGETSTYLETYPE_BORDER_COLOR)->value;
				GRPCOLOR* c = ParseColor(color);
				if (c)
				{
						if (w->colors.Get(GRPWIDGETSECTION_BORDER))
						{
								GRPCOLOR* oldcolor = w->colors.Get(GRPWIDGETSECTION_BORDER);
								if (!(*oldcolor == *c))
										w->ismodified |= GRPWIDGETSECTION_FLAG_BORDER_COLOR;
						}
						else
								w->ismodified |= GRPWIDGETSECTION_FLAG_BORDER_COLOR;

						delete(w->colors.Get(GRPWIDGETSECTION_BORDER));
						if (!w->colors.Set(GRPWIDGETSECTION_BORDER, c))
								w->colors.Add(GRPWIDGETSECTION_BORDER, c);
				}
		}

		//---------------------------- WORD SPACING
		if (this->styles.Find(GRPWIDGETSTYLETYPE_WORD_SPACING)!=NOTFOUND)
		{
				GRPWIDGETSTYLE* style=this->styles.Get(GRPWIDGETSTYLETYPE_WORD_SPACING);
				WIDGETSTRING string = style->value;

				//Defines normal space between words(0.25em).This is default

				float value=0;
				GRPWIDGETSTYLEVALUETYPE type = GRPWIDGETSTYLEVALUETYPE_UNSET;
				style->ParseValue(&string, value, type);				
				w->Set(GRPWIDGETSTYLETYPE_WORD_SPACING, (XDWORD)value,type);
		}		

		//---------------------------- TEXT-INDENT		
		if (this->styles.Find(GRPWIDGETSTYLETYPE_FONT_INDENT)!=NOTFOUND)
		{
				WIDGETSTRING value = this->styles.Get(GRPWIDGETSTYLETYPE_FONT_INDENT)->value;
				w->Set(GRPWIDGETSTYLETYPE_FONT_INDENT, (XDWORD)value.ConvertToInt());
		}


		//---------------------------- WHITE SPACE	
		if (this->styles.Find(GRPWIDGETSTYLETYPE_WHITESPACE) != NOTFOUND)
		{
				WIDGETSTRING value = this->styles.Get(GRPWIDGETSTYLETYPE_WHITESPACE)->value;
				if (value.Compare(__L("normal"))		== 0) { w->Set(GRPWIDGETSTYLETYPE_WHITESPACE, GRPWIDGETSTYLE_WHITESPACE_NORMAL);	}	else
				if (value.Compare(__L("nowrap"))		== 0) { w->Set(GRPWIDGETSTYLETYPE_WHITESPACE, GRPWIDGETSTYLE_WHITESPACE_NOWRAP);	}	else
				if (value.Compare(__L("pre"))				== 0)	{ w->Set(GRPWIDGETSTYLETYPE_WHITESPACE, GRPWIDGETSTYLE_WHITESPACE_PRE);			}	else
				if (value.Compare(__L("pre-line"))	== 0) { w->Set(GRPWIDGETSTYLETYPE_WHITESPACE, GRPWIDGETSTYLE_WHITESPACE_PRELINE); }	else
				if (value.Compare(__L("pre-wrap"))	== 0) { w->Set(GRPWIDGETSTYLETYPE_WHITESPACE, GRPWIDGETSTYLE_WHITESPACE_PREWRAP); }	else
				if (value.Compare(__L("initial"))		== 0) { w->Set(GRPWIDGETSTYLETYPE_WHITESPACE, GRPWIDGETSTYLE_WHITESPACE_INITIAL); }	else
				if (value.Compare(__L("inherit"))		== 0) { w->Set(GRPWIDGETSTYLETYPE_WHITESPACE, GRPWIDGETSTYLE_WHITESPACE_INHERIT); }	
		}

		///-------------------------------- shadow
		if (this->styles.Find(GRPWIDGETSTYLETYPE_BOXSHADOW) != NOTFOUND)
		{
				GRPWIDGETSTYLE* style = this->styles.Get(GRPWIDGETSTYLETYPE_BOXSHADOW);
				WIDGETSTRING value = style->value;
				XVECTOR<WIDGETSTRING*> values;
				value.Explode(__C(' '), &values);		
				if (values.GetSize() < 2)
						return true;

				w->Set(GRPWIDGETSTYLETYPE_BOXSHADOW, 1);

				w->ismodified |= GRPWIDGETSECTION_FLAG_BOXSHADOW;

				for (XDWORD e = 0; e < 2; e++)
				{						
						float value = 0;
						GRPWIDGETSTYLEVALUETYPE type = GRPWIDGETSTYLEVALUETYPE_UNSET;
						style->ParseValue(values.Get(e), value, type);
						w->Set((GRPWIDGETSTYLETYPE)(GRPWIDGETSTYLETYPE_BOXSHADOW_POSITIONX + e), (XDWORD)value, type);
				}

				for (XDWORD e = 2; e < values.GetSize(); e++)
				{
						WIDGETSTRING *s = values.Get(e);
						GRPCOLOR* c = ParseColor(*s);
						if (c)
						{
								delete(w->colors.Get(GRPWIDGETSECTION_BOXSHADOW));
								if (!w->colors.Set(GRPWIDGETSECTION_BOXSHADOW, c))
										if (!w->colors.Add(GRPWIDGETSECTION_BOXSHADOW, c))
												delete(c);
								w->ismodified |= GRPWIDGETSECTION_FLAG_BOXSHADOW_COLOR;
						}																
						else
						{
								float value = 0;
								GRPWIDGETSTYLEVALUETYPE type = GRPWIDGETSTYLEVALUETYPE_UNSET;
								style->ParseValue(values.Get(e), value, type);
								w->Set(GRPWIDGETSTYLETYPE(GRPWIDGETSTYLETYPE_BOXSHADOW_POSITIONX + e), (XDWORD)value, type);
						}
				}
				values.DeleteContents();
				values.DeleteAll();
		}
		///-------------------------------- text shadow
		if (this->styles.Find(GRPWIDGETSTYLETYPE_TEXTSHADOW) != NOTFOUND)
		{
				GRPWIDGETSTYLE* style = this->styles.Get(GRPWIDGETSTYLETYPE_TEXTSHADOW);
				WIDGETSTRING value = style->value;
				XVECTOR<WIDGETSTRING*> values;
				value.Explode(__C(' '), &values);
				if (values.GetSize() < 2)
						return true;

				w->ismodified |= GRPWIDGETSECTION_FLAG_TEXTSHADOW;
				
				for (XDWORD e = 0; e < 2; e++)
				{
						float value = 0;
						GRPWIDGETSTYLEVALUETYPE type = GRPWIDGETSTYLEVALUETYPE_UNSET;
						style->ParseValue(values.Get(e), value, type);
						w->Set((GRPWIDGETSTYLETYPE)(GRPWIDGETSTYLETYPE_TEXTSHADOW_POSITIONX + e), (XDWORD)value,type);
				}

						int e = 2;
						if (values.GetSize() >= 3)
						{
								WIDGETSTRING *s = values.Get(e);
								GRPCOLOR* c = ParseColor(*s);
								if (c)
								{
										delete(w->colors.Get(GRPWIDGETSECTION_TEXTSHADOW));
										if (!w->colors.Set(GRPWIDGETSECTION_TEXTSHADOW, c))
												if (!w->colors.Add(GRPWIDGETSECTION_TEXTSHADOW, c))
														delete(c);

										w->ismodified |= GRPWIDGETSECTION_FLAG_TEXTSHADOW_COLOR;
								}
						}
						values.DeleteContents();
						values.DeleteAll();
		}


		///-------------------------------- table border spacing
		if (this->styles.Find(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING) != NOTFOUND)
		{
				GRPWIDGETSTYLE* style = styles.Get(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING);
				XFSTRING value = style->value;
				XVECTOR<WIDGETSTRING*> values;
				value.Explode(__C(' '), &values);

				if (values.GetSize() == 1)
				{
						float value = 0;
						GRPWIDGETSTYLEVALUETYPE type = GRPWIDGETSTYLEVALUETYPE_UNSET;
						style->ParseValue(values.Get(0), value, type);
						w->Set(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_X, (XDWORD)value, type);
						w->Set(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_Y, (XDWORD)value, type);
				}
				else
				if (values.GetSize() >= 2)
				{
						float value = 0;
						GRPWIDGETSTYLEVALUETYPE type = GRPWIDGETSTYLEVALUETYPE_UNSET;
						style->ParseValue(values.Get(0), value, type);
						w->Set(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_X, (XDWORD)value, type);
						style->ParseValue(values.Get(1), value, type);
						w->Set(GRPWIDGETSTYLETYPE_TABLE_BORDER_SPACING_Y, (XDWORD)value, type);
				}

				values.DeleteContents();
				values.DeleteAll();
		}


		
		//--------------------- table colspan
		if (this->styles.Find(GRPWIDGETSTYLETYPE_TABLE_COLSPAN) != NOTFOUND)
		{
				GRPWIDGETSTYLE* style = styles.Get(GRPWIDGETSTYLETYPE_TABLE_COLSPAN);
				XFSTRING value = style->value;				
				int cols = value.ConvertToInt();
				w->Set(GRPWIDGETSTYLETYPE_TABLE_COLSPAN, (cols!=0 ? cols-1 : 0));
		}
		if (this->styles.Find(GRPWIDGETSTYLETYPE_TABLE_ROWSPAN) != NOTFOUND)
		{
				GRPWIDGETSTYLE* style = styles.Get(GRPWIDGETSTYLETYPE_TABLE_ROWSPAN);
				XFSTRING value = style->value;
				int rows = value.ConvertToInt();
				w->Set(GRPWIDGETSTYLETYPE_TABLE_ROWSPAN, (rows != 0 ? rows - 1 : 0));
		}


		return true;
}


/*-------------------------------------------------------------------
//	 GRPWIDGETSTYLECLASS::ParseColor
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	13/12/2017 13:56:40
//	@return		GRPCOLOR : 
//
//	@param		XCHAR* : 
//
*//*-----------------------------------------------------------------*/
GRPCOLOR* GRPWIDGETSTYLECLASS::ParseColor(WIDGETSTRING &color)
{
		int r, g, b;
		float alpha;
		GRPCOLOR* c=NULL;
		
		if (color.Get()[0] == __C('#'))
		{
				int result = color.scan(__L("#%02x%02x%02x"), &r, &g, &b);
				if (result == 3)									
						c = new GRPCOLOR(float(r) / 255, float(g) / 255, float(b) / 255, 1.0f);		
				else
				{
						int result = color.scan(__L("#%1x%1x%1x"), &r, &g, &b);
						if (result==3)
								c = new GRPCOLOR(float(r) / 16, float(g) / 16, float(b) / 16, 1.0f);
				}

		}
		else
		{
				int pos = color.FindCharacter(__C('('));
				if (pos != -1)
				{
						WIDGETSTRING type;
						color.Copy(0, pos, type);
						if (type.Compare(__L("rgba")) == 0)
						{
								int result= color.scan(__L("rgba(%d,%d,%d,%f)"), &r, &g, &b, &alpha);
								if (result ==4)
										c = new GRPCOLOR(float(r) / 255, float(g) / 255, float(b) / 255, float(alpha));										
						}
						else
								if (type.Compare(__L("rgb")) == 0)
								{
										int result = color.scan(__L("rgba(%d,%d,%d)"), &r, &g, &b);
										if (result ==3)
												c = new GRPCOLOR(float(r) / 255, float(g) / 255, float(b) / 255);
								}
				}
				else
				{				
						c = new GRPCOLOR();
						*c=GRPSTYLEPROCESSORCSS::GetPalette()->palette[color.Get()];
				}
		}
		return c;
}

/*-------------------------------------------------------------------
//	 GRPWIDGETSTYLECLASS::AppliesTo
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	08/02/2018 13:05:34
//	@return		bool : 
//
//	@param		GRPWIDGET* : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETSTYLECLASS::AppliesTo(GRPWIDGET* widget)
{
		if (this->selector.targetstate == GRPWIDGETSTATE_NONE)
				return true;

		if (this->selector.targetstate != widget->state)
				return false;

		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGETSTYLECLASS::Selects
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	08/02/2018 13:42:54
//	@return		bool : 
//
//	@param		GRPWIDGET* : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETSTYLECLASS::Selects(GRPWIDGET* widget)
{
		GRPWIDGET* w = widget;

		switch (this->selector.targettype)
		{
		case GRPWIDGETTARGETTYPE_TAG:
				if (w->tag.Compare(this->name) != 0)
						return false;
				break;
		case GRPWIDGETTARGETTYPE_CLASS:
				{
				bool found = false;
				for (XDWORD e = 0; e < w->classes.GetSize(); e++)
						if (w->classes.Get(e)->selector.name.Compare(name) == 0)
								found = true;
				if (!found)
						return false;
				}
				break;
		case GRPWIDGETTARGETTYPE_NAME:
				if (w->name.Compare(this->name) != 0)
							return false;
				break;
		case GRPWIDGETTARGETTYPE_ALL:				
						return true;
		case GRPWIDGETTARGETTYPE_RULE:
						return false;
		}
		
		w = widget->parent;
		for (XDWORD e = 0; e < this->dependencies.GetSize() && w; e++)
		{
				GRPWIDGETSTYLECLASSSELECTOR* selector=this->dependencies.Get(e);

				switch (this->selector.targettype)
				{
				case GRPWIDGETTARGETTYPE_TAG:
						if (w->tag.Compare(selector->name) == 0)
								return true;
						break;
				case GRPWIDGETTARGETTYPE_CLASS:
						for (XDWORD e = 0; e<widget->classes.GetSize(); e++)
								if (widget->classes.Get(e)->selector.name.Compare(name) == 0)
										return true;
						break;
				case GRPWIDGETTARGETTYPE_NAME:
						if (widget->name.Compare(this->name) == 0)
								return true;
						break;
				case GRPWIDGETTARGETTYPE_ALL:
						return true;
				case GRPWIDGETTARGETTYPE_RULE:
						return false;
				}

				w = w->parent;
		}
		
		return true;
}

/*-------------------------------------------------------------------
//	 GRPWIDGETSTYLE::ParseValue
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	22/02/2018 13:12:18
//	@return		bool : 
//
//	@param		WIDGETSTRING : 
//	@param		int& : 
//	@param		GRPWIDGETSTYLEVALUETYPE& : 
//
*//*-----------------------------------------------------------------*/
bool GRPWIDGETSTYLE::ParseValue(WIDGETSTRING*text, float& value, GRPWIDGETSTYLEVALUETYPE& type)
{		
		type = GRPWIDGETSTYLEVALUETYPE_ABSOLUTE;

		if (text->Character_IsAlpha(text->Get()[0]))
		{
				type = GRPWIDGETSTYLEVALUETYPE_SPECIAL;
				return false;
		}

		if (text->Find(__L("em"), true) != NOTFOUND)
		{
				text->DeleteCharacters(text->GetSize() - 2, 2);
				value = text->ConvertToFloat() * 16;
		}
		else
				if (text->Find(__L("px"), true) != NOTFOUND)
				{
						text->DeleteCharacters(text->GetSize() - 2, 2);
						value = text->ConvertToFloat();
				}
				else
				{
						if (text->Find(__L("%"), true) != NOTFOUND)
						{
								text->DeleteCharacters(text->GetSize() - 1, 1);
								type = GRPWIDGETSTYLEVALUETYPE_RELATIVE;
						}
						value = text->ConvertToFloat();
				}

		return true;
}

/*-------------------------------------------------------------------
//	GRPWIDGETSTYLECLASS::GRPWIDGETSTYLECLASS
*/
/**
//
//	Class Constructor GRPWIDGETSTYLECLASS
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/09/2017 18:27:35
//
*/
/*-----------------------------------------------------------------*/
GRPWIDGETSTYLECLASS::GRPWIDGETSTYLECLASS()
{
		Clean();
}

/*-------------------------------------------------------------------
//	GRPWIDGETSTYLECLASS::~GRPWIDGETSTYLECLASS
*/
/**
//
//	 Class Destructor GRPWIDGETSTYLECLASS
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/09/2017 11:00:56
//
*/
/*-----------------------------------------------------------------*/
GRPWIDGETSTYLECLASS::~GRPWIDGETSTYLECLASS()
{
		styles.DeleteElementContents();
		styles.DeleteAll();
		dependencies.DeleteContents();
		dependencies.DeleteAll();
}


/*-------------------------------------------------------------------
//	GRPSTYLESHEET::~GRPSTYLESHEET
*/
/**
//
//	 Class Destructor GRPSTYLESHEET
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/09/2017 10:45:45
//
*/
/*-----------------------------------------------------------------*/
GRPSTYLESHEET::~GRPSTYLESHEET()
{
		this->styles.DeleteContents();
		this->styles.DeleteAll();
}


/*-------------------------------------------------------------------
//	GRPSTYLESHEET::Add
*/
/**
//
//
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/08/2017 11:33:29
//
//  @param				c :
*/
/*-----------------------------------------------------------------*/
bool GRPSTYLESHEET::Add(GRPWIDGETSTYLECLASS* c)
{
		return this->styles.Add(c);
}

