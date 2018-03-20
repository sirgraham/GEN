/*------------------------------------------------------------------------------------------
//	GRPBITMAP.H
*/	
/**	
// \class 
//   
//  Graphics Bitmap class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//	@version 31/10/2013 11:28:42
*/	
/*	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
#ifndef	_GRPBITMAP_H_
#define _GRPBITMAP_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "GRPProperties.h"
#include "GRPBuffer.h"
#include "GRPBufferColorRGB.h"
#include "GRPBufferPixelFormatRGB.h"
#include "GRPBufferPixelFormatRGBA.h"
#include "GRPBufferPixelFormatRGBPacked.h"
#include "GRPBufferRendererBase.h"
#include "GRPBufferRendererPrimitives.h"
#include "GRPBufferColor.h"
#include "GRPBufferColorRGB.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define	GRPBITMAP_INVALIDHANDLE		0xFFFFFFFFUL 

//---- CLASS -------------------------------------------------------------------------------

class GRPBITMAP : public GRPPROPERTIES
{
	public:
																													GRPBITMAP											  (int width, int height, GRPPROPERTYMODE mode)				
																													{			
																														Clean();	

																														this->width  = width;
																														this->height = height;
																														this->mode   = mode;
																													}

		virtual																							 ~GRPBITMAP											  ()						
																													{			
																														Clean();
																													}				

		bool																									IsValid													()																								{ return isvalid;				}
		XBYTE*																								GetBuffer												()																								{ return buffer;				}											
		XBYTE**																								GetBufferPointer								()																								{ return &buffer;				}
		int																										GetBufferSize										()																								{ return buffersize;		}											

		virtual GRPBUFFERCOLOR*																GetPixel												(int x, int y)																					= 0;													
		virtual void																					PutPixel												(int x, int y, const GRPBUFFERCOLOR* color)		 				  = 0;									
		virtual void																					PutBlendPixel										(int x, int y, const GRPBUFFERCOLOR* color, int alpha)  = 0;

		virtual GRPBITMAP*																		GetSubBitmap										(GRPRECTINT& rect)																{ return NULL;								}

		virtual GRPBITMAP*																		Clone														()																								{ return NULL;								}

		virtual bool																					CopyFrom												(GRPBITMAP* bitmap)																{ return false;								}

		virtual	bool																					FlipHorizontal									()																								{ return false;								}
		virtual	bool																					FlipVertical										()																								{ return false;								}
		virtual	bool																					Rotate													(float degrees)																		{ return false;								}	
		virtual	bool																					Scale														(int nwidth, int nheight)													{ return false;								}
		virtual bool																					Crop														(GRPRECTINT& rect)																{ return false;								}

		XDWORD																								GetHandle												()																								{ return handle;							}
		void																									SetHandle												(XDWORD handle)																		{ this->handle = handle;			}


	protected:

		XBYTE*																								buffer;
		int																										buffersize;		
		bool																									isvalid;
		XDWORD																								handle;

	private:

		void																									Clean													  ()
																													{
																														buffer			= NULL;
																														buffersize	= 0;
																														isvalid			= false;
																														handle			= GRPBITMAP_INVALIDHANDLE;
																													}
};




template<class BUFFERPIXELFORMAT>
class GRPBUFFERBITMAPPIXELFORMAT : public GRPBITMAP
{
	public:

		typedef BUFFERPIXELFORMAT															BUFFERPIXELFORMATTYPE;
    typedef typename BUFFERPIXELFORMATTYPE::COLORTYPE			COLORTYPE;


									 																				GRPBUFFERBITMAPPIXELFORMAT		  (int width, int height, GRPPROPERTYMODE mode) : GRPBITMAP(width, height, mode)
																													{
																														Clean();																																																																													 
																														if(CreateBuffers())  isvalid = true;
																													}


		virtual																							 ~GRPBUFFERBITMAPPIXELFORMAT		  ()
																													{
																														DeleteBuffers();
																														Clean();
																													}


		GRPBUFFERCOLOR*																				GetPixel												  (int x, int y)
																													{
																														color = renderbase->GetPixel(x, y);

																														return (GRPBUFFERCOLOR*)&color;
																													}


		void																									PutPixel												(int x, int y, const GRPBUFFERCOLOR* color)
																													{ 
																														renderbase->PutPixel(x, y, (*(COLORTYPE*)color));
																													}
		

		void																									PutBlendPixel										(int x, int y, const GRPBUFFERCOLOR* color, int alpha)
																													{ 
																														renderbase->PutBlendPixel(x, y, (*(COLORTYPE*)color), alpha);
																													}


		GRPBITMAP*																						GetSubBitmap										(GRPRECTINT& rect)																		
																													{
																														int w = (rect.x2 - rect.x1);
																														int h = (rect.y2 - rect.y1);

																														if((w <= 0) || (h <=0)) return NULL;

																														GRPBUFFERBITMAPPIXELFORMAT<BUFFERPIXELFORMAT>* bitmap = new GRPBUFFERBITMAPPIXELFORMAT<BUFFERPIXELFORMAT>(w, h, mode);
																														if(!bitmap) return NULL;

																														for(XDWORD y=0; y<(XDWORD)h; y++)
																															{
																																for(XDWORD x=0; x<(XDWORD)w; x++)
																																	{
																																		COLORTYPE* color = (COLORTYPE*)GetPixel(x+rect.x1, y+rect.y1);
																																		if(color) bitmap->PutBlendPixel(x, y, color, color->a);																															
																																	}
																															}

																														return bitmap;
																													}


		GRPBITMAP*																						Clone														()																								
																													{
																														GRPBUFFERBITMAPPIXELFORMAT<BUFFERPIXELFORMAT>* bmp = new GRPBUFFERBITMAPPIXELFORMAT<BUFFERPIXELFORMAT>(width, height, mode);
																														if(!bmp) return NULL;

																														if(!bmp->CopyBuffer(buffer))
																															{
																																delete bmp;
																																return NULL;
																															}
																												
																														return bmp;
																													}																													


		
		bool																									CopyFrom												(GRPBITMAP* bitmap)																
																													{
																														DeleteBuffers();
																														CopyPropertysFrom(bitmap);

																														if(!CreateBuffers()) return false;

																														return CopyBuffer(bitmap->GetBuffer());																																																											
																													}



		bool																									FlipHorizontal									()																								
																													{ 
																														GRPBUFFERBITMAPPIXELFORMAT<BUFFERPIXELFORMAT>* bitmap = new GRPBUFFERBITMAPPIXELFORMAT<BUFFERPIXELFORMAT>(width, height, mode);
																														if(!bitmap) return false;

																														for(XDWORD y=0; y<height; y++)
																															{
																																for(XDWORD x=0; x<width; x++)
																																	{
																																		COLORTYPE* color = (COLORTYPE*)GetPixel(x,y);
																																		if(color) bitmap->PutBlendPixel(width-x, y, color, color->a);																															
																																	}
																															}
																														
																														if(!CopyFrom(bitmap)) return false;
																														
																														delete bitmap;

																														return true;					
																													}



		bool																									FlipVertical										()																								
																													{ 
																														GRPBUFFERBITMAPPIXELFORMAT<BUFFERPIXELFORMAT>* bitmap = new GRPBUFFERBITMAPPIXELFORMAT<BUFFERPIXELFORMAT>(width, height, mode);
																														if(!bitmap) return false;

																														for(XDWORD y=0; y<height; y++)
																															{
																																for(XDWORD x=0; x<width; x++)
																																	{
																																		COLORTYPE* color = (COLORTYPE*)GetPixel(x,y);
																																		if(color) bitmap->PutBlendPixel(x, height-y, color, color->a);																															
																																	}
																															}

																														if(!CopyFrom(bitmap)) return false;
																														
																														delete bitmap;

																														return true;										
																													}

		
		
		bool																									Rotate													(float degrees)
																													{
																														float	radians	= DEGREE2RADIAN(degrees);
																														float oneminusradians = DEGREE2RADIAN((180-degrees));


																														float	cosine	= (float)cos(radians);
																														float	sine		= (float)sin(radians);
																														
																														float	oneminuscosine	= (float)cos(oneminusradians);
																														float	oneminussine		= (float)sin(oneminusradians);

																														int w = (int)(height*fabs(oneminussine) + width*fabs(cosine));
																														int h = (int)(height*fabs(cosine) + width*fabs(oneminussine));

																														int bytesppixel = 4;

																														GRPBUFFERBITMAPPIXELFORMAT<BUFFERPIXELFORMAT>* bitmap = new GRPBUFFERBITMAPPIXELFORMAT<BUFFERPIXELFORMAT>(w, h, mode);
																														if(!bitmap) return false;
																												
																														int w2 = (w/2);
																														int h2 = (h/2);
	
																														for(int y=-h2;y<h2;y++)
																															{
																																for(int x=-w2;x<w2;x++)
																																	{
																																		int sx = (int)((x*cosine) + (y*sine)) + (width/2);
																																		int sy = (int)((y*cosine) - (x*sine)) + (height/2);
					
																																		if( (sx>=0) && (sx<(int)width) && (sy>=0) && (sy<(int)height) )
																																			{
																																				COLORTYPE* color = (COLORTYPE*)GetPixel(sx,sy);
																																				if(color) bitmap->PutBlendPixel(x+w2, y+h2, color, color->a);																																					
																																			}
																																	}
																															}
																														
																														if(!CopyFrom(bitmap)) return false;
																														
																														delete bitmap;

																														return true;
																													}


	bool																										Scale														(int nwidth, int nheight)
																													{ 																																		
																														float  xstep;
																														float  ystep;
																														float  xratio;
																														float  yratio;
																														XWORD  xcount;
																														XWORD  ycount;

																														xratio = (float)width  / nwidth;
																														yratio = (float)height / nheight;

																														GRPBUFFERBITMAPPIXELFORMAT<BUFFERPIXELFORMAT>* bitmap = new GRPBUFFERBITMAPPIXELFORMAT<BUFFERPIXELFORMAT>(nwidth, nheight, mode);
																														if(!bitmap) return false;

																														for(ystep=0,ycount=0; ycount<nheight; ystep+=yratio,ycount++)
																															{ 
																																for(xstep=0,xcount=0;xcount<nwidth; xstep+=xratio,xcount++)
																																	{
																																		COLORTYPE* color = (COLORTYPE*)GetPixel((int)xstep, (int)ystep);
																																		if(color) bitmap->PutBlendPixel(xcount, ycount, color, color->a);																																		
																																	}
																															}
																														
																														if(!CopyFrom(bitmap)) return false;
																														
																														delete bitmap;

																														return true;			
																													}


		bool																									Crop(GRPRECTINT& rect)
																													{
																														GRPBITMAP* bitmap = GetSubBitmap(rect);																														
																														if(!bitmap) return false;
																														
																														if(!CopyFrom(bitmap)) return false;
																														
																														delete bitmap;

																														return true;			
																													}

	private:

		void																									Clean													  ()
																													{
																														pixelformatbuffer	= NULL;	
																														renderbase				= NULL;
																													}
		
		
		bool																									CreateBuffers									  ()
																													{
																														buffersize = width * height * GetBytesperPixel();

																														buffer = new XBYTE[buffersize];		
																														if(!buffer) return false;

																														memset(buffer, 0, buffersize);

																														grpbuffer.Set(buffer, width , height, (IsBufferInverse()?1:-1)*((int)width * GetBytesperPixel()));
																																						
																														pixelformatbuffer	= new BUFFERPIXELFORMAT(grpbuffer);	
																														if(!pixelformatbuffer) return false;

																														renderbase				= new GRPBUFFERRENDERERBASE<BUFFERPIXELFORMAT>((*pixelformatbuffer));	
																														if(!renderbase)  return false;

																														return true;
																													}


		bool																									CopyBuffer											(XBYTE* buffer)
																													{
																														if(!buffersize) return false;
																														memcpy(this->buffer,buffer, buffersize);	

																														return true;
																													}


		bool																									DeleteBuffers									  ()
																													{
																														delete [] buffer;
																														buffer = NULL;
																																						
																														delete pixelformatbuffer;
																														pixelformatbuffer = NULL;	

																														delete renderbase;
																														renderbase = NULL;

																														return true;
																													}


		GRPBUFFER																							grpbuffer;
		BUFFERPIXELFORMAT*																		pixelformatbuffer;	
		GRPBUFFERRENDERERBASE<BUFFERPIXELFORMAT>*							renderbase;
		COLORTYPE																							color;	

};


	
//---- INLINE FUNCTIONS --------------------------------------------------------------------

	
#endif

