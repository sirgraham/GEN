/*------------------------------------------------------------------------------------------
//	UIWIDGETIMAGE.H
*/	
/**	
// \file 
//   
//  UI Image Widget
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 07/06/2016 12:18:52
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETIMAGE_H_
#define _UIWIDGETIMAGE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetForm.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPBITMAP;

class UIWIDGETIMAGE : public UIWIDGETFORM
{
public:

																								UIWIDGETIMAGE						(XCHAR* name, UIMANAGER * uimanagerarg);
																								UIWIDGETIMAGE						(UIWIDGETIMAGE* rhs);

	virtual																			 ~UIWIDGETIMAGE						();

	virtual				UIWIDGET*												Clone()
																								{
																									return new UIWIDGETIMAGE(this);
																								}

	void																					SetType									()																							{ this->widgettype = UI_WIDGET_TYPE_IMAGE; }

	void																					SetImage								(XCHAR* image, XCHAR* path = NULL);
	XCHAR*																				GetImage								();
	XCHAR*																				GetPath									();

	void																					SetPixelPerfect					(bool pixelperfect)															{ this->pixelperfect = pixelperfect;			}
	bool																					GetPixelPerfect					()																							{ return this->pixelperfect;							}

	void																					SetAlphaMargin					(XCHAR alphamargin)															{ this->alphamargin = alphamargin;				}
	XCHAR																					GetAlphaMargin					()																							{ return this->alphamargin;								}

	virtual				bool														CheckCursorOver					(float zoom = 1, float x = 0, float y = 0);

	bool																					ForceLoad								();

	XDWORD																				GetBitmapWidth					();
	XDWORD																				GetBitmapHeight					();

protected:
	virtual				bool														InternalDraw						(XDWORD& widgetlevel);
	virtual				bool														InternalUpdate					(XDWORD widgetlevel);
	virtual				void														UnRegisterOwnEvents			();
	virtual				void														RegisterOwnEvents				();

	virtual				bool														HandleInternalEvent			(XEVENT* event);

	XSTRING																				image;
	XSTRING																				path;
	bool																					imageloaded;
	bool																					pixelperfect;
	XCHAR																					alphamargin;

	GRPBITMAP*																		bitmap;
	XDWORD																				bitmapwidth;
	XDWORD																				bitmapheight;

private:
	void																					Clean										()
																								{
																									imageloaded			= false;
																									pixelperfect		= false;
																									bitmap					= NULL;
																									bitmapwidth			= 0;
																									bitmapheight		= 0;
																									alphamargin			= 0;
																								}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

