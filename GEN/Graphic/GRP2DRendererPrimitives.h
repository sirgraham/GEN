/*------------------------------------------------------------------------------------------
//	GRP2DRENDERERPRIMITIVES.H
*/	
/**	
// \file 
//   
//  renders primitives onto a bitmap
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 21/07/2016 12:28:23
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRP2DRENDERERPRIMITIVES_H_
#define _GRP2DRENDERERPRIMITIVES_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPBITMAP;
class GRPBUFFERCOLOR;

class GRP2DRENDERERPRIMITIVES
{
public:

														GRP2DRENDERERPRIMITIVES													();
	virtual									 ~GRP2DRENDERERPRIMITIVES													();


	void											Rectangle																				(int x1, int y1, int x2, int y2, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color);
	void											RectangleSolid																	(int x1, int y1, int x2, int y2, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color);
	void											RectangleOutlined																(int x1, int y1, int x2, int y2, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color, const GRPBUFFERCOLOR* outlinecolor);


	void											HLine																						(int x1, int y1, int y2, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color, bool last = false);
	void											VLine																						(int x1, int y1, int x2, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color, bool last = false);
	void											Line																						(int x1, int y1, int x2, int y2, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color, bool last=false);

	void											Ellipse																					(int x, int y, int rx, int ry, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color);
	void											EllipseSolid																		(int x, int y, int rx, int ry, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color);
	void											EllipseOutlined																	(int x, int y, int rx, int ry, GRPBITMAP* bitmap, const GRPBUFFERCOLOR* color, const GRPBUFFERCOLOR* outlinecolor);

protected:

	

private:
	void											Clean																														()
														{
														}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

