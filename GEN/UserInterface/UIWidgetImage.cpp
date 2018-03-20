
/*------------------------------------------------------------------------------------------
//	UIWIDGETIMAGE.CPP
//	
//	UI Image Widget
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 07/06/2016 12:44:13
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "UIWidgetImage.h"
#include "UIManager.h"	

#include "GRPFileBitmap.h"
#include "GRPBitmap.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::UIWIDGETIMAGE
*/	
/**	
//	
//	Class Constructor UIWIDGETIMAGE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/06/2016 12:48:41
//	
//  @param				name : 
//  @param				uimanagerarg : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETIMAGE::UIWIDGETIMAGE(XCHAR* name, UIMANAGER * uimanagerarg) : UIWIDGETFORM(name, uimanagerarg)
{
	Clean();

	widgettype = UI_WIDGET_TYPE_IMAGE;
}




/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::UIWIDGETIMAGE
*/	
/**	
//	
//	Class Constructor UIWIDGETIMAGE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			08/02/2017 17:46:51
//	
//  @param				rhs : 
*/
/*-----------------------------------------------------------------*/
UIWIDGETIMAGE::UIWIDGETIMAGE(UIWIDGETIMAGE* rhs) : UIWIDGETFORM(rhs)
{
	Clean();

	image					= rhs->image;
	path					= rhs->path;
	imageloaded		= rhs->imageloaded;
	pixelperfect	= rhs->pixelperfect;
	alphamargin		= rhs->alphamargin;

	if(bitmap)
		{
			bitmap				= bitmap->Clone();
		}
}



/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::~UIWIDGETIMAGE
*/	
/**	
//	
//	 Class Destructor UIWIDGETIMAGE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/06/2016 12:48:48
//	
*/
/*-----------------------------------------------------------------*/
UIWIDGETIMAGE::~UIWIDGETIMAGE()
{
	if(bitmap)
		{
			delete bitmap;
		}


	Clean();
}







/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::SetImage
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/06/2016 12:48:54
//	
//  @param				image : 
*/
/*-----------------------------------------------------------------*/
void UIWIDGETIMAGE::SetImage(XCHAR* image, XCHAR* path)
{
	if(image)
		{
			if(this->image.Compare(image) != 0)
				{
					this->image.Set(image);
					imageloaded = false; // get the bitmap for pixel perfect collisions! (consider only alpha == 0 or allow for a range?)
				}
		}
	
	if(path)
		{
			if(this->path.Compare(path) != 0)
				{
					this->path.Set(path);
					imageloaded = false;
				}
		}
}




/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::GetImage
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			10/08/2016 11:06:33
//	
//	@return 			XCHAR* : 
//
*/
/*-----------------------------------------------------------------*/
XCHAR* UIWIDGETIMAGE::GetImage()
{
	return image.Get();
}




/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::GetPath
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			28/10/2016 11:02:46
//	
//	@return 			XCHAR* : 
//
*/
/*-----------------------------------------------------------------*/
XCHAR* UIWIDGETIMAGE::GetPath()
{
	if(path.GetSize())
		{
			return path.Get();
		}

	return NULL;
}


/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::CheckCursorOver
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			04/10/2016 16:25:19
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETIMAGE::CheckCursorOver(float zoom, float zx, float zy)
{
	if(pixelperfect)
		{
			ismouseaware = true;
		}
	else
		{
			ismouseaware = false;
		}

	if(pixelperfect)
		{
			// convert coordinates to image local(so remove starting pixel as offset)
			// test the specific pixel, if alpha below a certain threshold, true;

			float x, y;
			uimanager->GetInput()->GetCursorPosition(x, y); // returns real coordinates

			// x/y are virtual pixels in a bottom left coordinate system

			// normalize coordinates to convert into real pixels I guess?
			// and probably not even turn the Y around

			XDWORD	w, h;
							w = uimanager->GetRealWidth(); // but I don't want the virtual one, but the real one
							h = uimanager->GetRealHeight();

			XDWORD	vw = uimanager->GetVirtualWidth();
			XDWORD	vh = uimanager->GetVirtualHeight();

			float fx = x / w;
			float fy = y / h;

			fy = 1.0f - fy;

			// fx yx now on [0,1]
			fx -= gpt0x;
			fy -= gpt0y;

			float ww = gpt1x - gpt0x;
			float wh = gpt1y - gpt0y;

			ww = fx / ww;
			wh = fy / wh;

			fx = fx * vw;
			fy = fy * vh;

			float wx = vw * this->gpt0x;
			float wy = vh * this->gpt0y;

			

			float inimagex = fx - wx;
			// need to change the y into a top left coordinate system before finding the relative coordinate
			float inimagey = fy - wy;

			// need to conver normalized into real image coordinates(bitmap for this)
			inimagex = ww;
			inimagey = wh;

			//XDEBUG_PRINTCOLOR(2, __L("%f  |  %f"), inimagex, inimagey);

			if((inimagex >= 0) && (inimagey >= 0))
				{
					//GRPBUFFERCOLOR* color = bitmap->GetPixel((int)inimagex,(int)inimagey);
					if(!bitmap)
						{
							return UIWIDGETCONTAINER::CheckCursorOver();
						}

					GRPPROPERTYMODE mode = bitmap->GetMode();
					inimagex *= bitmap->GetWidth();
					inimagey *= bitmap->GetHeight();

					// shit, also, need to consider different pixel formats
						switch(mode)
						{									
							case GRPPROPERTYMODE_08_INDEX			: return UIWIDGETCONTAINER::CheckCursorOver();			
							case GRPPROPERTYMODE_16_RGBA_4444	: return UIWIDGETCONTAINER::CheckCursorOver();
							case GRPPROPERTYMODE_16_RGBA_5551	: return UIWIDGETCONTAINER::CheckCursorOver();
							case GRPPROPERTYMODE_16_RGB_565		: return UIWIDGETCONTAINER::CheckCursorOver();
							case GRPPROPERTYMODE_16_SRGB_565	: return UIWIDGETCONTAINER::CheckCursorOver();
 							case GRPPROPERTYMODE_24_RGB_888		: return UIWIDGETCONTAINER::CheckCursorOver();
							case GRPPROPERTYMODE_24_BGR_888		: return UIWIDGETCONTAINER::CheckCursorOver();
							case GRPPROPERTYMODE_32_RGBA_8888	: {
																										if(UIWIDGETCONTAINER::CheckCursorOver())
																											{
																												GRPBUFFERCOLORRGBA8* color = (GRPBUFFERCOLORRGBA8*)bitmap->GetPixel((int)inimagex, (int)inimagey);
																												
																												if(color->a >= 0.5)
																													{
																														return true;
																													}
																												else
																													{
																														return false;
																													}
																											}
																									}
							case GRPPROPERTYMODE_32_BGRA_8888	: return UIWIDGETCONTAINER::CheckCursorOver();
							default: break;
						}
				}

			UIWIDGETCONTAINER::CheckCursorOver(zoom, zx, zy);
			return false;
		}

	return UIWIDGETCONTAINER::CheckCursorOver(zoom, zx, zy);
}



/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::ForceLoad
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			03/03/2017 13:56:41
//	
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETIMAGE::ForceLoad()
{
	// if image not loaded get bitmap in here?
	if(!imageloaded)
		{
			// but it's a grpbitmap, and can't avoid that grp dependency :(
			// hardcode for now :(
			GRPFILEBITMAP filebitmap;

			// no, need to compute the full path
			XPATH xpath;

			if(GetPath())
				{
					xpath = GetPath();
					xpath.Slash_Add();
					xpath += GetImage();
				}
			else
				{
					xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,GetImage());
				}

			bitmap = filebitmap.Load(xpath);
			

			// or maybe some factory kind ?

			imageloaded = true;

			if(!bitmap)
				{
					return false;
				}

			bitmapwidth = bitmap->GetWidth();
			bitmapheight = bitmap->GetHeight();
		}

	return true;
}


/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::GetBitmapWidth
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			03/03/2017 13:54:31
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD UIWIDGETIMAGE::GetBitmapWidth()
{
	return bitmapwidth;
}



/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::GetBitmapHeight
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			03/03/2017 13:54:24
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD UIWIDGETIMAGE::GetBitmapHeight()
{
	return bitmapheight;

	return 0;
}


/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::InternalDraw
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/06/2016 12:49:02
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETIMAGE::InternalDraw(XDWORD& widgetlevel)
{
	return false;
}






/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::InternalUpdate
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/06/2016 12:49:12
//	
//	@return 			bool : 
//
//  @param				widgetlevel : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETIMAGE::InternalUpdate(XDWORD widgetlevel)
{
	ForceLoad();

	return true;
}







/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::UnRegisterOwnEvents
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/06/2016 12:49:20
//	
*/
/*-----------------------------------------------------------------*/
void UIWIDGETIMAGE::UnRegisterOwnEvents()
{
}






/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::RegisterOwnEvents
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/06/2016 12:49:28
//	
*/
/*-----------------------------------------------------------------*/
void UIWIDGETIMAGE::RegisterOwnEvents()
{
}







/*-------------------------------------------------------------------
//	UIWIDGETIMAGE::HandleInternalEvent
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			07/06/2016 12:49:36
//	
//	@return 			bool : 
//
//  @param				event : 
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETIMAGE::HandleInternalEvent(XEVENT* event)
{
	return true;
}

