/*------------------------------------------------------------------------------------------
//	GRPBUFFERPIXELFORMATRGBPACKED.H
*/	
/**	
// \class 
//   
//  Graphics Pixel Format Buffer RGB Packed Class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//	@version 01/08/2013 16:02:44
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPBUFFERPIXELFORMATRGBPACKED_H_
#define _GRPBUFFERPIXELFORMATRGBPACKED_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPBufferColorRGB.h"
#include "GRPRect.h"

#include "XMemory.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

	
/*---- CLASS -----------------------------------------------------------------------------*/

  
class BLENDERRGB555
{
	public:
  
		typedef GRPBUFFERCOLORRGBA8															COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XWORD																			PIXELTYPE;


    static void																				PutBlendPixel												(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD)
																											{
																												PIXELTYPE rgb = *p;
																		
																												CALCTYPE r = (rgb >> 7) & 0xF8;
																												CALCTYPE g = (rgb >> 2) & 0xF8;
																												CALCTYPE b = (rgb << 3) & 0xF8;

																												*p = (PIXELTYPE) (((((cr - r) * alpha + (r << 8)) >> 1)  & 0x7C00) | ((((cg - g) * alpha + (g << 8)) >> 6)  & 0x03E0) | (((cb - b) * alpha + (b << 8)) >> 11) | 0x8000);
																											}


		static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((r & 0xF8) << 7) | ((g & 0xF8) << 2) | (b >> 3) | 0x8000);
																											}

    static COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p >> 7) & 0xF8, (p >> 2) & 0xF8, (p << 3) & 0xF8);
																											}
};



class BLENDERRGB555PRE
{
	public:
 
		typedef GRPBUFFERCOLORRGBA8															COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XWORD																			PIXELTYPE;


    static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((r & 0xF8) << 7) | ((g & 0xF8) << 2) | (b >> 3) | 0x8000);
																											}


		static void																				PutBlendPixel												(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD cover)
																											{
																												alpha = COLORTYPE::BASESCALE_MASK - alpha;
																												PIXELTYPE rgb = *p;
																												
																												CALCTYPE r = (rgb >> 7) & 0xF8;
																												CALCTYPE g = (rgb >> 2) & 0xF8;
																												CALCTYPE b = (rgb << 3) & 0xF8;
																												
																												*p = (PIXELTYPE)((((r * alpha + cr * cover) >> 1)  & 0x7C00) | (((g * alpha + cg * cover) >> 6)  & 0x03E0) | ((b * alpha + cb * cover) >> 11) | 0x8000);
																											}


		static COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p >> 7) & 0xF8, (p >> 2) & 0xF8, (p << 3) & 0xF8);
																											}
};




template<class GAMMA> 
class BLENDERRGB555GAMMA
{
	public:
  
		typedef GRPBUFFERCOLORRGBA8															COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XWORD																			PIXELTYPE;
    typedef GAMMA																			GAMMATYPE;


																											BLENDERRGB555GAMMA									()																{	gamma = NULL;			}

		void																							SetGamma														(const GAMMATYPE& g)							{ gamma = &g;				}

		static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((r & 0xF8) << 7) | ((g & 0xF8) << 2) | (b >> 3) | 0x8000);
																											}

		void																							PutBlendPixel												(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD)
																											{
																												PIXELTYPE rgb = *p;
																												
																												CALCTYPE r = gamma->dir((rgb >> 7) & 0xF8);
																												CALCTYPE g = gamma->dir((rgb >> 2) & 0xF8);
																												CALCTYPE b = gamma->dir((rgb << 3) & 0xF8);
																												
																												*p = (PIXELTYPE)(((gamma->inv(((gamma->dir(cr) - r) * alpha + (r << 8)) >> 8) << 7) & 0x7C00) | ((gamma->inv(((gamma->dir(cg) - g) * alpha + (g << 8)) >> 8) << 2) & 0x03E0) | (gamma->inv(((gamma->dir(cb) - b) * alpha + (b << 8)) >> 8) >> 3) | 0x8000);
																											}
		
		static COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p >> 7) & 0xF8, (p >> 2) & 0xF8, (p << 3) & 0xF8);
																											}

		private:
			
			const GAMMA*																		gamma;

};



    
class BLENDERRGB565
{
	public:
  
		typedef GRPBUFFERCOLORRGBA8															COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XWORD																			PIXELTYPE;

		static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
																											}

		static void																				PutBlendPixel												(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD)
																											{
																												PIXELTYPE rgb = *p;

																												CALCTYPE r = (rgb >> 8) & 0xF8;
																												CALCTYPE g = (rgb >> 3) & 0xFC;
																												CALCTYPE b = (rgb << 3) & 0xF8;
																												
																												*p = (PIXELTYPE)(((((cr - r) * alpha + (r << 8))     ) & 0xF800) | ((((cg - g) * alpha + (g << 8)) >> 5) & 0x07E0) | (((cb - b) * alpha + (b << 8)) >> 11));
																											}
	
		static COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p >> 8) & 0xF8, (p >> 3) & 0xFC, (p << 3) & 0xF8);
																											}
};




class BLENDERRGB565PRE
{
	public:
  
		typedef GRPBUFFERCOLORRGBA8															COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XWORD																			PIXELTYPE;
		
		static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
																											}

    static void																				PutBlendPixel												(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD cover)
																											{
																												alpha = COLORTYPE::BASESCALE_MASK - alpha;																												
																												PIXELTYPE rgb = *p;

																												CALCTYPE r = (rgb >> 8) & 0xF8;
																												CALCTYPE g = (rgb >> 3) & 0xFC;
																												CALCTYPE b = (rgb << 3) & 0xF8;
																												
																												*p = (PIXELTYPE)((((r * alpha + cr * cover)      ) & 0xF800) | (((g * alpha + cg * cover) >> 5 ) & 0x07E0) | ((b * alpha + cb * cover) >> 11));
																											}

		static COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p >> 8) & 0xF8, (p >> 3) & 0xFC, (p << 3) & 0xF8);
																											}
};




template<class GAMMA> 
class BLENDERRGB656GAMMA
{
	public:
		
		typedef GRPBUFFERCOLORRGBA8															COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XWORD																			PIXELTYPE;
    typedef GAMMA																			GAMMATYPE;

																											BLENDERRGB656GAMMA									()																{	gamma = NULL;			}

		void																							SetGamma														(const GAMMATYPE& g)							{	gamma = &g;				}
		
		
		static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
																											}

		void																							PutBlendPixel												(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD)
																											{
																												PIXELTYPE rgb = *p;

																												CALCTYPE r = gamma->dir((rgb >> 8) & 0xF8);
																												CALCTYPE g = gamma->dir((rgb >> 3) & 0xFC);
																												CALCTYPE b = gamma->dir((rgb << 3) & 0xF8);
																												
																												*p = (PIXELTYPE)(((gamma->inv(((gamma->dir(cr) - r) * alpha + (r << 8)) >> 8) << 8) & 0xF800) | ((gamma->inv(((gamma->dir(cg) - g) * alpha + (g << 8)) >> 8) << 3) & 0x07E0) | (gamma->inv(((gamma->dir(cb) - b) * alpha + (b << 8)) >> 8) >> 3));
																											}

		static COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p >> 8) & 0xF8, (p >> 3) & 0xFC, (p << 3) & 0xF8);
																											}

		private:

			const GAMMA*																		gamma;
    
};



    
class BLENDERRGBAAA
{
	public:
		
		typedef GRPBUFFERCOLORRGBA16														COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XDWORD																		PIXELTYPE;
		
		static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((r & 0xFFC0) << 14) | ((g & 0xFFC0) << 4) | (b >> 6) | 0xC0000000);
																											}


		static void																				PutBlendPixel												(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD)
																											{
																												PIXELTYPE rgb = *p;

																												CALCTYPE r = (rgb >> 14) & 0xFFC0;
																												CALCTYPE g = (rgb >> 4)  & 0xFFC0;
																												CALCTYPE b = (rgb << 6)  & 0xFFC0;

																												*p = (PIXELTYPE)(((((cr - r) * alpha + (r << 16)) >> 2)  & 0x3FF00000) | ((((cg - g) * alpha + (g << 16)) >> 12) & 0x000FFC00) | (((cb - b) * alpha + (b << 16)) >> 22) | 0xC0000000);
																											}


    static COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p >> 14) & 0xFFC0, (p >> 4)  & 0xFFC0, (p << 6)  & 0xFFC0);
																											}
};




class BLENDERRGBAAAPRE
{
	public:
  
		typedef GRPBUFFERCOLORRGBA16														COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XDWORD																		PIXELTYPE;
		
		static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((r & 0xFFC0) << 14) | ((g & 0xFFC0) << 4) | (b >> 6) | 0xC0000000);
																											}

    static void																				PutBlendPixel												(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD cover)
																											{
																												alpha = COLORTYPE::BASESCALE_MASK - alpha;
																												cover = (cover + 1) << (COLORTYPE::BASESCALE_SHIFT - 8);
																												
																												PIXELTYPE rgb = *p;
																												
																												CALCTYPE r = (rgb >> 14) & 0xFFC0;
																												CALCTYPE g = (rgb >> 4)  & 0xFFC0;
																												CALCTYPE b = (rgb << 6)  & 0xFFC0;
																												
																												*p = (PIXELTYPE)((((r * alpha + cr * cover) >> 2)  & 0x3FF00000) | (((g * alpha + cg * cover) >> 12) & 0x000FFC00) | ((b * alpha + cb * cover) >> 22) | 0xC0000000);
																											}

		
		static COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p >> 14) & 0xFFC0, (p >> 4)  & 0xFFC0, (p << 6)  & 0xFFC0);
																											}
};



    
   
template<class GAMMA> 
class BLENDERRGBAAAGAMMA
{
	public:
		
		typedef GRPBUFFERCOLORRGBA16														COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XDWORD																		PIXELTYPE;
    typedef GAMMA																			GAMMATYPE;

																											BLENDERRGBAAAGAMMA()																									{	gamma = NULL;		}

		void																							Setgamma													(const GAMMATYPE& g)								{ gamma = &g;			}
		
		static PIXELTYPE																	PutPixel													(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((r & 0xFFC0) << 14) | ((g & 0xFFC0) << 4) | (b >> 6) | 0xC0000000);
																											}

    void																							PutBlendPixel											(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD)
																											{
																												PIXELTYPE rgb = *p;
																												
																												CALCTYPE r = gamma->dir((rgb >> 14) & 0xFFC0);
																												CALCTYPE g = gamma->dir((rgb >> 4)  & 0xFFC0);
																												CALCTYPE b = gamma->dir((rgb << 6)  & 0xFFC0);

																												*p = (PIXELTYPE)(((gamma->inv(((gamma->dir(cr) - r) * alpha + (r << 16)) >> 16) << 14) & 0x3FF00000) | ((gamma->inv(((gamma->dir(cg) - g) * alpha + (g << 16)) >> 16) << 4 ) & 0x000FFC00) | (gamma->inv(((gamma->dir(cb) - b) * alpha + (b << 16)) >> 16) >> 6 ) | 0xC0000000);
																											}

		
		static COLORTYPE																	GetColor													(PIXELTYPE p)
																											{
																												return COLORTYPE((p >> 14) & 0xFFC0, (p >> 4)  & 0xFFC0, (p << 6)  & 0xFFC0);
																											}
		private:
			
			const GAMMA*																		gamma;
};




class BLENDERBGRAAA
{
	public:
	
		typedef GRPBUFFERCOLORRGBA16														COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XDWORD																		PIXELTYPE;

		static PIXELTYPE																	PutPixel(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((b & 0xFFC0) << 14) | ((g & 0xFFC0) << 4) | (r >> 6) | 0xC0000000);
																											}

    static void																				PutBlendPixel(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD)
																											{
																												PIXELTYPE bgr = *p;

																												CALCTYPE b = (bgr >> 14) & 0xFFC0;
																												CALCTYPE g = (bgr >> 4)  & 0xFFC0;
																												CALCTYPE r = (bgr << 6)  & 0xFFC0;

																												*p = (PIXELTYPE)(((((cb - b) * alpha + (b << 16)) >> 2)  & 0x3FF00000) | ((((cg - g) * alpha + (g << 16)) >> 12) & 0x000FFC00) | (((cr - r) * alpha + (r << 16)) >> 22) | 0xC0000000);
																											}

    static COLORTYPE																	GetColor(PIXELTYPE p)
																											{
																												return COLORTYPE((p << 6)  & 0xFFC0, (p >> 4)  & 0xFFC0, (p >> 14) & 0xFFC0);
																											}
};



    
class BLENDERBGRAAAPRE
{
	public:
		
		typedef GRPBUFFERCOLORRGBA16														COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XDWORD																		PIXELTYPE;
		
		static PIXELTYPE																	PutPixel(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((b & 0xFFC0) << 14) | ((g & 0xFFC0) << 4) | (r >> 6) | 0xC0000000);
																											}

    static void																				PutBlendPixel(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD cover)
																											{
																												alpha = COLORTYPE::BASESCALE_MASK - alpha;
																												cover = (cover + 1) << (COLORTYPE::BASESCALE_SHIFT - 8);

																												PIXELTYPE bgr = *p;

																												CALCTYPE b = (bgr >> 14) & 0xFFC0;
																												CALCTYPE g = (bgr >> 4)  & 0xFFC0;
																												CALCTYPE r = (bgr << 6)  & 0xFFC0;
																												
																												*p = (PIXELTYPE)((((b * alpha + cb * cover) >> 2)  & 0x3FF00000) | (((g * alpha + cg * cover) >> 12) & 0x000FFC00) | ((r * alpha + cr * cover) >> 22) | 0xC0000000);
																											}
	
    static COLORTYPE																	GetColor(PIXELTYPE p)
																											{
																												return COLORTYPE((p << 6)  & 0xFFC0, (p >> 4)  & 0xFFC0, (p >> 14) & 0xFFC0);
																											}
};




template<class GAMMA> 
class BLENDERBGRAAAGAMMA
{
	public:
  
		typedef GRPBUFFERCOLORRGBA16														COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XDWORD																		PIXELTYPE;
    typedef GAMMA																			GAMMATYPE;

																											BLENDERBGRAAAGAMMA									()																{	gamma = NULL;				}

		void																							SetGamma														(const GAMMATYPE& g)							{ gamma = &g;					}

		static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((b & 0xFFC0) << 14) | ((g & 0xFFC0) << 4) | (r >> 6) | 0xC0000000);
																											}

    void																							PutBlendPixel												(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD)
																											{
																												PIXELTYPE bgr = *p;

																												CALCTYPE b = gamma->dir((bgr >> 14) & 0xFFC0);
																												CALCTYPE g = gamma->dir((bgr >> 4)  & 0xFFC0);
																												CALCTYPE r = gamma->dir((bgr << 6)  & 0xFFC0);

																												*p = (PIXELTYPE)(((gamma->inv(((gamma->dir(cb) - b) * alpha + (b << 16)) >> 16) << 14) & 0x3FF00000) | ((gamma->inv(((gamma->dir(cg) - g) * alpha + (g << 16)) >> 16) << 4 ) & 0x000FFC00) | (gamma->inv(((gamma->dir(cr) - r) * alpha + (r << 16)) >> 16) >> 6 ) | 0xC0000000);
																											}

		static	COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p << 6)  & 0xFFC0, (p >> 4)  & 0xFFC0, (p >> 14) & 0xFFC0);
																											}

	private:

		const GAMMA*																			gamma;
    
};




class BLENDERRGBBBA
{
	public:
  
		typedef GRPBUFFERCOLORRGBA16														COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XDWORD																		PIXELTYPE;

		static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((r & 0xFFE0) << 16) | ((g & 0xFFE0) << 5) | (b >> 6));
																											}

    static void																				PutBlendPixel												(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD)
																											{
																												PIXELTYPE rgb = *p;

																												CALCTYPE r = (rgb >> 16) & 0xFFE0;
																												CALCTYPE g = (rgb >> 5)  & 0xFFE0;
																												CALCTYPE b = (rgb << 6)  & 0xFFC0;

																												*p = (PIXELTYPE)(((((cr - r) * alpha + (r << 16))      ) & 0xFFE00000) | ((((cg - g) * alpha + (g << 16)) >> 11) & 0x001FFC00) | (((cb - b) * alpha + (b << 16)) >> 22));
																											}
		
		static COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p >> 16) & 0xFFE0, (p >> 5)  & 0xFFE0, (p << 6)  & 0xFFC0);
																											}
};




class BLENDERRGBBAPRE
{
	public:
  
		typedef GRPBUFFERCOLORRGBA16														COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XDWORD																		PIXELTYPE;

		static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((r & 0xFFE0) << 16) | ((g & 0xFFE0) << 5) | (b >> 6));
																											}

    static void																				PutBlendPixel												(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD cover)
																											{
																												alpha = COLORTYPE::BASESCALE_MASK - alpha;
																												cover = (cover + 1) << (COLORTYPE::BASESCALE_SHIFT - 8);

																												PIXELTYPE rgb = *p;

																												CALCTYPE r = (rgb >> 16) & 0xFFE0;
																												CALCTYPE g = (rgb >> 5)  & 0xFFE0;
																												CALCTYPE b = (rgb << 6)  & 0xFFC0;

																												*p = (PIXELTYPE) ((((r * alpha + cr * cover)) & 0xFFE00000) |(((g * alpha + cg * cover) >> 11) & 0x001FFC00) | ((b * alpha + cb * cover) >> 22));
																											}

	  static COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p >> 16) & 0xFFE0, (p >> 5)  & 0xFFE0, (p << 6)  & 0xFFC0);
																											}
};



    
template<class GAMMA> 
class BLENDERRGBBBAGAMMA
{
	public:
  
		typedef GRPBUFFERCOLORRGBA16														COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XDWORD																		PIXELTYPE;
    typedef GAMMA																			GAMMATYPE;

																											BLENDERRGBBBAGAMMA									()																{	gamma = NULL;						}

		void																							SetGamma														(const GAMMATYPE& g)							{ gamma = &g;							}

		static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((r & 0xFFE0) << 16) | ((g & 0xFFE0) << 5) | (b >> 6));
																											}

    void																							PutBlendPixel												(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD)
																											{
																												PIXELTYPE rgb = *p;

																												CALCTYPE r = gamma->dir((rgb >> 16) & 0xFFE0);
																												CALCTYPE g = gamma->dir((rgb >> 5)  & 0xFFE0);
																												CALCTYPE b = gamma->dir((rgb << 6)  & 0xFFC0);

																												*p = (PIXELTYPE)(((gamma->inv(((gamma->dir(cr) - r) * alpha + (r << 16)) >> 16) << 16) & 0xFFE00000) | ((gamma->inv(((gamma->dir(cg) - g) * alpha + (g << 16)) >> 16) << 5 ) & 0x001FFC00) | (gamma->inv(((gamma->dir(cb) - b) * alpha + (b << 16)) >> 16) >> 6 ));
																											}

		static COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p >> 16) & 0xFFE0, (p >> 5)  & 0xFFE0, (p << 6)  & 0xFFC0);
																											}

	private:
        
		const GAMMA*																			gamma;
  
};


   
 
class BLENDERBGRABB
{
	public:
  
		typedef GRPBUFFERCOLORRGBA16														COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XDWORD																		PIXELTYPE;		
		
		static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((b & 0xFFC0) << 16) | ((g & 0xFFE0) << 6) | (r >> 5));
																											}

    static void																				PutBlendPixel												(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD)
																											{
																												PIXELTYPE bgr = *p;
																												
																												CALCTYPE b = (bgr >> 16) & 0xFFC0;
																												CALCTYPE g = (bgr >> 6)  & 0xFFE0;
																												CALCTYPE r = (bgr << 5)  & 0xFFE0;

																												*p = (PIXELTYPE) (((((cb - b) * alpha + (b << 16))) & 0xFFC00000) | ((((cg - g) * alpha + (g << 16)) >> 10) & 0x003FF800) | (((cr - r) * alpha + (r << 16)) >> 21));
																											}

    static COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p << 5)  & 0xFFE0, (p >> 6)  & 0xFFE0, (p >> 16) & 0xFFC0);
																											}
};




class BLENDERBGRABBPRE
{
	public:
  
		typedef GRPBUFFERCOLORRGBA16														COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XDWORD																		PIXELTYPE;	
		
		static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((b & 0xFFC0) << 16) | ((g & 0xFFE0) << 6) | (r >> 5));
																											}

    static void																				PutBlendPixel												(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD cover)
																											{
																												alpha = COLORTYPE::BASESCALE_MASK - alpha;
																												cover = (cover + 1) << (COLORTYPE::BASESCALE_SHIFT - 8);

																												PIXELTYPE bgr = *p;
																												
																												CALCTYPE b = (bgr >> 16) & 0xFFC0;
																												CALCTYPE g = (bgr >> 6)  & 0xFFE0;
																												CALCTYPE r = (bgr << 5)  & 0xFFE0;

																												*p = (PIXELTYPE)((((b * alpha + cb * cover)) & 0xFFC00000) | (((g * alpha + cg * cover) >> 10) & 0x003FF800) | ((r * alpha + cr * cover) >> 21));
																											}

    static COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p << 5)  & 0xFFE0, (p >> 6)  & 0xFFE0, (p >> 16) & 0xFFC0);
																											}
};




template<class GAMMA> 
class BLENDERBGRABBGAMMA
{
	public:

		typedef GRPBUFFERCOLORRGBA16														COLORTYPE;
    typedef COLORTYPE::VALUETYPE											VALUETYPE;
    typedef COLORTYPE::CALCTYPE												CALCTYPE;
    typedef XDWORD																		PIXELTYPE;
    typedef GAMMA																			GAMMATYPE;

																											BLENDERBGRABBGAMMA									()																{ gamma = NULL;				}
        
		void																							SetGamma														(const GAMMATYPE& g)							{ gamma = &g;					}
		
		static PIXELTYPE																	PutPixel														(XDWORD r, XDWORD g, XDWORD b)
																											{
																												return (PIXELTYPE)(((b & 0xFFC0) << 16) | ((g & 0xFFE0) << 6) | (r >> 5));
																											}

    void																							PutBlendPixel(PIXELTYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD)
																											{
																												PIXELTYPE bgr = *p;
																												
																												CALCTYPE b = gamma->dir((bgr >> 16) & 0xFFC0);
																												CALCTYPE g = gamma->dir((bgr >> 6)  & 0xFFE0);
																												CALCTYPE r = gamma->dir((bgr << 5)  & 0xFFE0);

																												*p = (PIXELTYPE)(((gamma->inv(((gamma->dir(cb) - b) * alpha + (b << 16)) >> 16) << 16) & 0xFFC00000) | ((gamma->inv(((gamma->dir(cg) - g) * alpha + (g << 16)) >> 16) << 6 ) & 0x003FF800) | (gamma->inv(((gamma->dir(cr) - r) * alpha + (r << 16)) >> 16) >> 5 ));
																											}
		
		static COLORTYPE																	GetColor														(PIXELTYPE p)
																											{
																												return COLORTYPE((p << 5)  & 0xFFE0, (p >> 6)  & 0xFFE0, (p >> 16) & 0xFFC0);
																											}

	private:

		const GAMMA*																			gamma;
    
};


    
    
template<class BLENDER,  class RENDERERBUFFER> 
class GRPBUFFERPIXELFORMATALPHABLENDRGBPACKED
{
	public:
  
		typedef RENDERERBUFFER														RENDERERBUFFERTYPE;
    typedef typename RENDERERBUFFERTYPE::ROWDATA			ROWDATA;
    typedef BLENDER																		BLENDERTYPE;
    typedef typename BLENDERTYPE::COLORTYPE						COLORTYPE;
    typedef typename BLENDERTYPE::PIXELTYPE						PIXELTYPE;
    typedef int																				ORDERTYPE; // A fake one
    typedef typename COLORTYPE::VALUETYPE							VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE							CALCTYPE;
				
		enum BASESCALE 
		{
			BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT	,
			BASESCALE_SCALE = COLORTYPE::BASESCALE_SCALE	,
			BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK		,
      
			PIXELWIDTH		  = sizeof(PIXELTYPE)
		};

        
																											GRPBUFFERPIXELFORMATALPHABLENDRGBPACKED		(RENDERERBUFFERTYPE& rb)	{ rendererbuffer = &rb;			 }

    void																							Set																					(RENDERERBUFFERTYPE& rb)	{ rendererbuffer = &rb;			 }

		template<class BUFFERPIXELFORMAT>
    bool																							Set																					(BUFFERPIXELFORMAT& pixelformatbuffer, int x1, int y1, int x2, int y2)
																											{
																												GRPRECTINT r(x1, y1, x2, y2);
																												
																												if(r.Clip(GRPRECTINT(0, 0, pixelformatbuffer.GetWidth()-1, pixelformatbuffer.GetHeight()-1)))
																													{
																														int stride = pixelformatbuffer.GetStride();
																														
																														rendererbuffer->Set(pixelformatbuffer.GetPtr(r.x1, stride < 0 ? r.y2 : r.y1), (r.x2 - r.x1) + 1, (r.y2 - r.y1) + 1, stride);
																														return true;
																													}

																												return false;
																											}

		BLENDER&																					GetBlender																	()												{ return blender;																		}
    
    XDWORD																						GetWidth																		() const									{ return rendererbuffer->GetWidth();								}
    XDWORD																						GetHeight																		() const									{ return rendererbuffer->GetHeight();								}
    int																								GetStride																		() const									{ return rendererbuffer->GetStride();								}    
             
		XBYTE*																						Row																					(int y)										{ return rendererbuffer->Row(y);										}
    const XBYTE*																			Row																					(int y) const							{ return rendererbuffer->Row(y);										}
    ROWDATA																						RowData																			(int y) const							{ return rendererbuffer->RowData(y);								}
       
    XBYTE*																						GetPtr																			(int x, int y)						{ return rendererbuffer->Row(y) + x * PIXELWIDTH;		}
		const XBYTE*																			GetPtr																			(int x, int y) const			{	return rendererbuffer->Row(y) + x * PIXELWIDTH;	  }
     
		COLORTYPE																					GetPixel																		(int x, int y) const
																											{
																												return blender.GetColor(((PIXELTYPE*)rendererbuffer->Row(y))[x]);
																											}    

	  void																							PutPixel																		(XBYTE* p, const COLORTYPE& c)
																											{
																												*(PIXELTYPE*)p = blender.PutPixel(c.r, c.g, c.b);
																											}
                      
    void																							PutPixel																		(int x, int y, const COLORTYPE& c)
																											{
																												((PIXELTYPE*)rendererbuffer->Row(x, y, 1))[x] = blender.PutPixel(c.r, c.g, c.b);
																											}


		void																							PutBlendPixel																(int x, int y, const COLORTYPE& c, XBYTE cover)
																											{
																												PutOrBlendPixel((PIXELTYPE*)rendererbuffer->Row(x, y, 1) + x, c, cover);
																											}
        

		void																							PutHLine																		(int x, int y, XDWORD size, const COLORTYPE& c)
																											{
																												PIXELTYPE* p = (PIXELTYPE*)rendererbuffer->Row(x, y, size) + x;
																												PIXELTYPE  v = blender.PutPixel(c.r, c.g, c.b);
																											
																												do{ *p++ = v;

																													} while(--size);
																											}


		void																							PutVLine																		(int x, int y, XDWORD size, const COLORTYPE& c)
																											{
																												PIXELTYPE v = blender.PutPixel(c.r, c.g, c.b);
																											
																												do{ PIXELTYPE* p = (PIXELTYPE*)rendererbuffer->Row(x, y++, 1) + x;
																														*p = v;

																													} while(--size);
																											}


    void																							PutBlendHLine																(int x, int y, XDWORD size, const COLORTYPE& c, XBYTE cover)
																											{
																												if(c.a)
																													{
																														PIXELTYPE* p = (PIXELTYPE*)rendererbuffer->Row(x, y, size) + x;
																														CALCTYPE alpha = (CALCTYPE(c.a) * (cover + 1)) >> 8;
																													
																														if(alpha == BASESCALE_MASK)
																															{
																																PIXELTYPE v = blender.PutPixel(c.r, c.g, c.b);

																																do{ *p++ = v;

																																	} while(--size);
																															}
																														 else
																															{
																																do{ blender.PutBlendPixel(p, c.r, c.g, c.b, alpha, cover);
																																		++p;

																																	} while(--size);
																															}
																													}
																											}

		void																							PutBlendVLine																(int x, int y, XDWORD size, const COLORTYPE& c, XBYTE cover)
																											{
																												if(c.a)
																													{
																														CALCTYPE alpha = (CALCTYPE(c.a) * (cover + 1)) >> 8;
																														
																														if(alpha == BASESCALE_MASK)
																															{
																																PIXELTYPE v = blender.PutPixel(c.r, c.g, c.b);

																																do{ ((PIXELTYPE*)rendererbuffer->Row(x, y++, 1))[x] = v;

																																	} while(--size);
																															}
																														 else
																															{
																																do{ blender.PutBlendPixel((PIXELTYPE*)rendererbuffer->Row(x, y++, 1), c.r, c.g, c.b, alpha, cover);

																																	} while(--size);
																															}
																													}
																											}

        
		void																							PutBlendSolidHSpan													(int x, int y, XDWORD size, const COLORTYPE& c, const XBYTE* covers)
																											{
																												PIXELTYPE* p = (PIXELTYPE*)rendererbuffer->Row(x, y, size) + x;

																												do{	PutOrBlendPixel(p, c, *covers++);
																														++p;

																													} while(--size);
																											}

        
   void																								PutBlendSolidVSpan													(int x, int y, XDWORD size, const COLORTYPE& c, const XBYTE* covers)
																											{
																												do{ PutOrBlendPixel((PIXELTYPE*)rendererbuffer->Row(x, y++, 1) + x, c, *covers++);

																													} while(--size);
																											}

	 void																								PutColorHSpan																(int x, int y, XDWORD size,	const COLORTYPE* colors)
																											{
																												PIXELTYPE* p = (PIXELTYPE*)rendererbuffer->Row(x, y, size) + x;

																												do{ *p++ = blender.PutPixel(colors->r, colors->g, colors->b);
																														++colors;

																													} while(--size);
																											}

		void																							PutColorVSpan																(int x, int y, XDWORD size, const COLORTYPE* colors)
																											{
																												do{ PIXELTYPE* p = (PIXELTYPE*)rendererbuffer->Row(x, y++, 1) + x;
																														*p = blender.PutPixel(colors->r, colors->g, colors->b);
																														++colors;

																													} while(--size);
																											}

        
		void																							PutBlendColorHSpan													(int x, int y, XDWORD size, const COLORTYPE* colors, const XBYTE* covers, XBYTE cover)
																											{
																												PIXELTYPE* p = (PIXELTYPE*)rendererbuffer->Row(x, y, size) + x;

																												do{ PutOrBlendPixel(p++, *colors++, covers ? *covers++ : cover);

																													} while(--size);
																											}

        
		void																							PutBlendColorVSpan													(int x, int y, XDWORD size, const COLORTYPE* colors, const XBYTE* covers, XBYTE cover)
																											{
																												do{ PutOrBlendPixel((PIXELTYPE*)rendererbuffer->Row(x, y++, 1) + x, *colors++, covers ? *covers++ : cover);

																													} while(--size);
																											}
        
        
		template<class RENDERERBUFFER2>
    void																							CopyFrom																		(const RENDERERBUFFER2& from, int xdst, int ydst, int xsrc, int ysrc, XDWORD size)
																											{
																												const XBYTE* p = from.Row(ysrc);
																												if(p) memmove(rendererbuffer->Row(xdst, ydst, size) + xdst * PIXELWIDTH, p + xsrc * PIXELWIDTH, size * PIXELWIDTH);																												
																											}

        
		template<class SOURCEBUFFERPIXELFORMAT>
    void																							PutBlendFrom																(const SOURCEBUFFERPIXELFORMAT& from, int xdst, int ydst, int xsrc, int ysrc, XDWORD size, XBYTE cover)
																											{
																												typedef typename SOURCEBUFFERPIXELFORMAT::ORDERTYPE SOURCEORDER;

																												const VALUETYPE* psrc = (const VALUETYPE*)from.Row(ysrc);
																												
																												if(psrc)
																													{
																														psrc += xsrc * 4;
																														PIXELTYPE* pdst = (PIXELTYPE*)rendererbuffer->Row(xdst, ydst, size) + xdst;
																														
																														do{ VALUETYPE alpha = psrc[SOURCEORDER::A];
																																if(alpha)
																																	{
																																		if(alpha == BASESCALE_MASK && cover == 255)
																																			{
																																				*pdst = blender.PutPixel(psrc[SOURCEORDER::R], psrc[SOURCEORDER::G], psrc[SOURCEORDER::B]);
																																			}
																																		 else
																																			{
																																				blender.PutBlendPixel(pdst, psrc[SOURCEORDER::R], psrc[SOURCEORDER::G], psrc[SOURCEORDER::B], alpha, cover);
																																			}
																																	}

																																psrc += 4;
																																++pdst;

																															} while(--size);
																													}
																											}

        
		template<class SOURCEBUFFERPIXELFORMAT>
    void																							PutBlendFromColor														(const SOURCEBUFFERPIXELFORMAT& from, const COLORTYPE& color, int xdst, int ydst, int xsrc, int ysrc, XDWORD size, XBYTE cover)
																											{
																												typedef typename SOURCEBUFFERPIXELFORMAT::VALUETYPE src_VALUETYPE;
																												const src_VALUETYPE* psrc = (src_VALUETYPE*)from.Row(ysrc);

																												if(psrc)
																													{
																														PIXELTYPE* pdst = (PIXELTYPE*)rendererbuffer->Row(xdst, ydst, size) + xdst;

																														do{ blender.PutBlendPixel(pdst, color.r, color.g, color.b, color.a, cover);
																																++psrc;
																																++pdst;

																															} while(--size);
																													}
																											}

        
		template<class SOURCEBUFFERPIXELFORMAT>
    void																							PutBlendFromLut															(const SOURCEBUFFERPIXELFORMAT& from, const COLORTYPE* colorlut, int xdst, int ydst, int xsrc, int ysrc, XDWORD size, XBYTE cover)
																											{
																												typedef typename SOURCEBUFFERPIXELFORMAT::VALUETYPE src_VALUETYPE;
																												const src_VALUETYPE* psrc = (src_VALUETYPE*)from.Row(ysrc);

																												if(psrc)
																													{
																														PIXELTYPE* pdst = (PIXELTYPE*)rendererbuffer->Row(xdst, ydst, size) + xdst;

																														do{	const COLORTYPE& color = colorlut[*psrc];
																																blender.PutBlendPixel(pdst, color.r, color.g, color.b, color.a, cover);
																																++psrc;
																																++pdst;

																															} while(--size);
																													}
																											}


	private:
        
		void																							PutOrBlendPixel															(PIXELTYPE* p, const COLORTYPE& c, XDWORD cover)
																											{
																												if(c.a)
																													{
																														CALCTYPE alpha = (CALCTYPE(c.a) * (cover + 1)) >> 8;

																														if(alpha == BASESCALE_MASK)
																															{
																																*p = blender.PutPixel(c.r, c.g, c.b);
																															}
																														 else
																															{
																																blender.PutBlendPixel(p, c.r, c.g, c.b, alpha, cover);
																															}
																													}
																											}


		RENDERERBUFFERTYPE*																rendererbuffer;
    BLENDER																						blender;
};

   
typedef GRPBUFFERPIXELFORMATALPHABLENDRGBPACKED<BLENDERRGB555, GRPBUFFER>  GRPBUFFERPIXELFORMAT_RGB555;    
typedef GRPBUFFERPIXELFORMATALPHABLENDRGBPACKED<BLENDERRGB565, GRPBUFFER>  GRPBUFFERPIXELFORMAT_RGB565;    


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif



