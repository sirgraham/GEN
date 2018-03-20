
/*------------------------------------------------------------------------------------------
//	GRP2DRENDERERPRIMITIVES.CPP
//	
//	render basic primitives into a bitmap
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 21/07/2016 12:46:16
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRP2DRENDERERPRIMITIVES.h"
#include "GRPBitmap.h"	
#include "GRP2DLineBresenham.h"
#include "GRP2DEllipseBresenham.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	GRP2DRENDERERPRIMITIVES::GRP2DRENDERERPRIMITIVES
*/	
/**	
//	
//	Class Constructor GRP2DRENDERERPRIMITIVES
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/07/2016 12:54:29
//	
*/
/*-----------------------------------------------------------------*/
GRP2DRENDERERPRIMITIVES::GRP2DRENDERERPRIMITIVES()
{
	Clean();
}




/*-------------------------------------------------------------------
//	GRP2DRENDERERPRIMITIVES::~GRP2DRENDERERPRIMITIVES
*/	
/**	
//	
//	 Class Destructor GRP2DRENDERERPRIMITIVES
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/07/2016 12:54:37
//	
*/
/*-----------------------------------------------------------------*/
GRP2DRENDERERPRIMITIVES::~GRP2DRENDERERPRIMITIVES()
{
	Clean();
}




/*-------------------------------------------------------------------
//	GRP2DRENDERERPRIMITIVES::Rectangle
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/07/2016 16:36:07
//	
//  @param				x1 : 
//  @param				y1 : 
//  @param				x2 : 
//  @param				y2 : 
//  @param				bitmap : 
//  @param				color : 
*/
/*-----------------------------------------------------------------*/
void GRP2DRENDERERPRIMITIVES::Rectangle(int x1, int y1, int x2, int y2, GRPBITMAP* bitmap, const  GRPBUFFERCOLOR* color)
{					
	//renderer->PutBlendHLine(x1,   y1,   x2-1 	, linecolor , COVERTYPE_FULL);
	//renderer->PutBlendVLine(x2,   y1,   y2-1	, linecolor , COVERTYPE_FULL);
	//renderer->PutBlendHLine(x1+1, y2,   x2		, linecolor , COVERTYPE_FULL);
	//renderer->PutBlendVLine(x1,   y1+1, y2	  , linecolor , COVERTYPE_FULL);

	// Need to rethink this
	//this->Line(x1		,   y1	,		x2-1, 	y1	, bitmap, color);
	//this->Line(x2		,   y1	,		x2	,   y2-1, bitmap, color);
	//this->Line(x1+1	,		y2	,		x2	,		y2	, bitmap, color);
	//this->Line(x1		,		y1+1,		x1	,		y2	, bitmap, color);	

	this->HLine(x1,   y1,   x2-1 	, bitmap, color);
	this->VLine(x2,   y1,   y2-1	, bitmap, color);
	this->HLine(x1+1, y2,   x2		, bitmap, color);
	this->VLine(x1,   y1+1, y2	  , bitmap, color);
}


/*-------------------------------------------------------------------
//	GRP2DRENDERERPRIMITIVES::RectangleSolid
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/07/2016 12:54:46
//	
//  @param				x1 : 
//  @param				y1 : 
//  @param				x2 : 
//  @param				y2 : 
//  @param				bitmap : 
//  @param				color : 
*/
/*-----------------------------------------------------------------*/
void GRP2DRENDERERPRIMITIVES::RectangleSolid(int x1, int y1, int x2, int y2, GRPBITMAP* bitmap, const  GRPBUFFERCOLOR* color)
{
	for(int j = y1; j <= y2; j++)
		{
			for(int i = x1; i <= x2; i++)
				{
					bitmap->PutPixel(i, j, color);
				}
		}
}




/*-------------------------------------------------------------------
//	GRP2DRENDERERPRIMITIVES::RectangleOutlined
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/07/2016 16:36:35
//	
//  @param				x1 : 
//  @param				y1 : 
//  @param				x2 : 
//  @param				y2 : 
//  @param				bitmap : 
//  @param				color : 
*/
/*-----------------------------------------------------------------*/
void GRP2DRENDERERPRIMITIVES::RectangleOutlined(int x1, int y1, int x2, int y2, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color, const GRPBUFFERCOLOR* outlinecolor)
{
	Rectangle(x1, y1, x2, y2, bitmap, outlinecolor);
	this->RectangleSolid(x1+1, y1+1, x2-1, y2-1, bitmap, color);
	//renderer->PutBlendBar(x1+1, y1+1, x2-1, y2-1, fillcolor, COVERTYPE_FULL);
}




/*-------------------------------------------------------------------
//	GRP2DRENDERERPRIMITIVES::HLine
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			22/07/2016 9:53:59
//	
//  @param				x1 : 
//  @param				y1 : 
//  @param				x2 : 
//  @param				bitmap : 
//  @param				color : 
//  @param				last : 
*/
/*-----------------------------------------------------------------*/
void GRP2DRENDERERPRIMITIVES::HLine(int x1, int y1, int x2, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color, bool last)
{
	int end = last ? x2 : x2+1;

	for(int i = x1; i < end; i++)
		{
			bitmap->PutPixel(i, y1, color);
		}
}





/*-------------------------------------------------------------------
//	GRP2DRENDERERPRIMITIVES::VLine
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			22/07/2016 9:53:49
//	
//  @param				x1 : 
//  @param				y1 : 
//  @param				y2 : 
//  @param				bitmap : 
//  @param				color : 
//  @param				last : 
*/
/*-----------------------------------------------------------------*/
void GRP2DRENDERERPRIMITIVES::VLine(int x1, int y1, int y2, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color, bool last)
{
	int end = last ? y2 : y2+1;

	for(int i = y1; i < end; i++)
		{
			bitmap->PutPixel(x1, i, color);
		}
}


/*-------------------------------------------------------------------
//	GRP2DRENDERERPRIMITIVES::Line
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/07/2016 16:36:46
//	
//  @param				x1 : 
//  @param				y1 : 
//  @param				x2 : 
//  @param				y2 : 
//  @param				bitmap : 
//  @param				color : 
//  @param				last : (default false)
*/
/*-----------------------------------------------------------------*/
void GRP2DRENDERERPRIMITIVES::Line(int x1, int y1, int x2, int y2, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color, bool last)
{
	GRP2DLINEBRESENHAMINTERPOLATOR li(x1, y1, x2, y2);

	XDWORD size = li.GetSize();
	if(size == 0)
		{
			if(last) bitmap->PutPixel(li.LineLR(x1), li.LineLR(y1), color);
				
			return;
		}

	if(last) ++size;

	if(li.IsVer())
		{
			do{ bitmap->PutPixel(li.X2(), li.Y1(), color);
					li.VStep();

				} while(--size);
		}
	 else
		{
			do{ bitmap->PutPixel(li.X1(), li.Y2(), color);
					li.HStep();

				}	while(--size);
		}
}




/*-------------------------------------------------------------------
//	GRP2DRENDERERPRIMITIVES::Ellipse
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			22/07/2016 9:53:38
//	
//  @param				x : 
//  @param				y : 
//  @param				rx : 
//  @param				ry : 
//  @param				bitmap : 
//  @param				color : 
*/
/*-----------------------------------------------------------------*/
void GRP2DRENDERERPRIMITIVES::Ellipse(int x, int y, int rx, int ry, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color)
{
	GRP2DELLIPSEBRESENHAM ei(rx, ry);

	int dx = 0;
	int dy = -ry;
	
	do{ dx += ei.Dx();
			dy += ei.Dy();
			
			bitmap->PutPixel(x + dx, y + dy, color);
			bitmap->PutPixel(x + dx, y - dy, color);
			bitmap->PutPixel(x - dx, y - dy, color);
			bitmap->PutPixel(x - dx, y + dy, color);
			
			++ei;

		} while(dy < 0);
}




/*-------------------------------------------------------------------
//	GRP2DRENDERERPRIMITIVES::EllipseSolid
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			22/07/2016 9:53:27
//	
//  @param				x : 
//  @param				y : 
//  @param				rx : 
//  @param				ry : 
//  @param				bitmap : 
//  @param				color : 
*/
/*-----------------------------------------------------------------*/
void GRP2DRENDERERPRIMITIVES::EllipseSolid(int x, int y, int rx, int ry, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color)
{
	GRP2DELLIPSEBRESENHAM ei(rx, ry);

	int dx	= 0;
	int dy	= -ry;
	int dy0 = dy;
	int dx0 = dx;

	do{
			dx += ei.Dx();
			dy += ei.Dy();

			if(dy != dy0)
				{
					this->HLine(x-dx0, y+dy0, x+dx0, bitmap, color);
					this->HLine(x-dx0, y-dy0, x+dx0, bitmap, color);
				}
			
			dx0 = dx;
			dy0 = dy;

			++ei;

		} while(dy < 0);

	this->HLine(x-dx0, y+dy0, x+dx0, bitmap, color);
}




/*-------------------------------------------------------------------
//	GRP2DRENDERERPRIMITIVES::EllipseOutlined
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			22/07/2016 9:53:15
//	
//  @param				x : 
//  @param				y : 
//  @param				rx : 
//  @param				ry : 
//  @param				bitmap : 
//  @param				color : 
*/
/*-----------------------------------------------------------------*/
void GRP2DRENDERERPRIMITIVES::EllipseOutlined(int x, int y, int rx, int ry, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color, const GRPBUFFERCOLOR* outlinecolor)
{
	GRP2DELLIPSEBRESENHAM ei(rx, ry);
	int dx = 0;
	int dy = -ry;

	do{ dx += ei.Dx();
			dy += ei.Dy();

			bitmap->PutPixel(x + dx, y + dy, outlinecolor);
			bitmap->PutPixel(x + dx, y - dy, outlinecolor);
			bitmap->PutPixel(x - dx, y - dy, outlinecolor);
			bitmap->PutPixel(x - dx, y + dy, outlinecolor);

			if(ei.Dy() && dx)
				{
					 this->HLine(x-dx+1, y+dy, x+dx-1, bitmap, color);
					 this->HLine(x-dx+1, y-dy, x+dx-1, bitmap, color);
				}

			++ei;

		} while(dy < 0);
}
