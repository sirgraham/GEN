/*------------------------------------------------------------------------------------------
//	GRPBUFFERPIXELFORMATRGB.H
*/	
/**	
// \class 
//   
//  Graphics Pixel Format Buffer RGB Class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//	@version 01/08/2013 16:02:44
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPBUFFERPIXELFORMATRGB_H_
#define _GRPBUFFERPIXELFORMATRGB_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include <string.h>	
		
#include "GRPBufferColorRGB.h"
#include "GRPBuffer.h"
#include "GRPRect.h"

#include "XMemory.h"

	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

	
/*---- CLASS -----------------------------------------------------------------------------*/



template<class COLORT, class ORDER, class GAMMALUT> 
class APPLYGAMMADIRRGB
{
	public:
		
		typedef typename COLORT::VALUETYPE								VALUETYPE;

																											APPLYGAMMADIRRGB										(const GAMMALUT& gamma)
																											{
																												this->gamma = gamma; 
																											}

		void																							operator()													(VALUETYPE* p)
																											{
																												p[ORDER::R] = gamma.Dir(p[ORDER::R]);
																												p[ORDER::G] = gamma.Dir(p[ORDER::G]);
																												p[ORDER::B] = gamma.Dir(p[ORDER::B]);
																											}
	private:

		const GAMMALUT&																		gamma;
};



template<class COLORT, class ORDER, class GAMMALUT> 
class APPLYGAMMAINVRGB
{
	public:
		
		typedef typename COLORT::VALUETYPE								VALUETYPE;

																											APPLYGAMMAINVRGB										(const GAMMALUT& gamma)
																											{
																												this->gamma = gamma;
																											}

		void																							operator()													(VALUETYPE* p)
																											{
																													p[ORDER::R] = gamma.Inv(p[ORDER::R]);
																													p[ORDER::G] = gamma.Inv(p[ORDER::G]);
																													p[ORDER::B] = gamma.Inv(p[ORDER::B]);
																											}

	private:

		const GAMMALUT&																		gamma;
};




 
template<class COLORT, class ORDER> 
class BLENDERRGB
{
	public:

		typedef COLORT																		COLORTYPE;
    typedef ORDER																			ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE							VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE							CALCTYPE;

    enum BASESCALE 
		{ 
			BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT 
		};

       
    static void																				PutBlendPixel												(VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD cover = 0)
																											{
																												p[ORDER::R] += (VALUETYPE)(((cr - p[ORDER::R]) * alpha) >> BASESCALE_SHIFT);
																												p[ORDER::G] += (VALUETYPE)(((cg - p[ORDER::G]) * alpha) >> BASESCALE_SHIFT);
																												p[ORDER::B] += (VALUETYPE)(((cb - p[ORDER::B]) * alpha) >> BASESCALE_SHIFT);
																											}
};


    
template<class COLORT, class ORDER> 
class BLENDERRGBPRE
{
public:

		typedef COLORT																		COLORTYPE;
    typedef ORDER																			ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE							VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE							CALCTYPE;

    enum BASESCALE 
		{ 
			BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT 
		};

    static void																				PutBlendPixel												(VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha,	XDWORD cover)
																											{
																												alpha = COLORTYPE::BASESCALE_MASK - alpha;
																												cover = (cover + 1) << (BASESCALE_SHIFT - 8);

																												p[ORDER::R] = (VALUETYPE)((p[ORDER::R] * alpha + cr * cover) >> BASESCALE_SHIFT);
																												p[ORDER::G] = (VALUETYPE)((p[ORDER::G] * alpha + cg * cover) >> BASESCALE_SHIFT);
																												p[ORDER::B] = (VALUETYPE)((p[ORDER::B] * alpha + cb * cover) >> BASESCALE_SHIFT);
																											}

       
    static void																				PutBlendPixel												(VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha)
																											{
																												alpha = COLORTYPE::BASESCALE_MASK - alpha;

																												p[ORDER::R] = (VALUETYPE)(((p[ORDER::R] * alpha) >> BASESCALE_SHIFT) + cr);
																												p[ORDER::G] = (VALUETYPE)(((p[ORDER::G] * alpha) >> BASESCALE_SHIFT) + cg);
																												p[ORDER::B] = (VALUETYPE)(((p[ORDER::B] * alpha) >> BASESCALE_SHIFT) + cb);
																											}

};




template<class COLORT, class ORDER, class GAMMA> 
class BLENDERRGBGAMMA
{
	public:
  
		typedef COLORT																		COLORTYPE;
    typedef ORDER																			ORDERTYPE;
    typedef GAMMA																			GAMMATYPE;
    typedef typename COLORTYPE::VALUETYPE							VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE							CALCTYPE;
  
		enum BASESCALE 
		{ 
			BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT 
		};
																								
																											BLENDERRGBGAMMA											()
																											{																												
																												gamma = NULL;
																											}


		void																							SetGamma														(const GAMMATYPE& g) 
																											{ 
																												gamma = &g; 
																											}
        
    void																							PutBlendPixel												(VALUETYPE* p, XDWORD cr, XDWORD cg, XDWORD cb, XDWORD alpha, XDWORD cover=0)
																											{
																												CALCTYPE r = gamma->Dir(p[ORDER::R]);
																												CALCTYPE g = gamma->Dir(p[ORDER::G]);
																												CALCTYPE b = gamma->Dir(p[ORDER::B]);

																												p[ORDER::R] = gamma->Inv((((gamma->Dir(cr) - r) * alpha) >> BASESCALE_SHIFT) + r);
																												p[ORDER::G] = gamma->Inv((((gamma->Dir(cg) - g) * alpha) >> BASESCALE_SHIFT) + g);
																												p[ORDER::B] = gamma->Inv((((gamma->Dir(cb) - b) * alpha) >> BASESCALE_SHIFT) + b);
																											}

	private:

		const GAMMATYPE*																	gamma;
};



    
    
  
template<class BLENDER, class RENDERERBUFFER> 
class GRPBUFFERPIXELFORMATALPHABLENDRGB
{
	public:
  
		typedef RENDERERBUFFER														RENDERERBUFFERTYPE;
    typedef BLENDER																		BLENDERTYPE;
    typedef typename RENDERERBUFFERTYPE::ROWDATA			ROWDATA;
    typedef typename BLENDERTYPE::COLORTYPE						COLORTYPE;
    typedef typename BLENDERTYPE::ORDERTYPE						ORDERTYPE;
    typedef typename COLORTYPE::VALUETYPE							VALUETYPE;
    typedef typename COLORTYPE::CALCTYPE							CALCTYPE;

    enum BASESCALE 
    {
			BASESCALE_SHIFT = COLORTYPE::BASESCALE_SHIFT	,
      BASESCALE_SCALE = COLORTYPE::BASESCALE_SCALE	,
      BASESCALE_MASK  = COLORTYPE::BASESCALE_MASK		,
      
			PIXELWIDTH		  = sizeof(VALUETYPE) * 3
    };
 
																											GRPBUFFERPIXELFORMATALPHABLENDRGB		(RENDERERBUFFERTYPE& rb)
																											{
																												rendererbuffer = &rb;
																											}
        
		void																							Set																	(RENDERERBUFFERTYPE& rb) 
																											{ 
																												rendererbuffer = &rb; 
																											}
		template<class BUFFERPIXELFORMAT>
    bool																							Set																	(BUFFERPIXELFORMAT& pixelformatbuffer, int x1, int y1, int x2, int y2)
																											{
																												GRPRECTINT r(x1, y1, x2, y2);
																												
																												if(r.Clip(GRPRECTINT(0, 0, pixelformatbuffer.width()-1, pixelformatbuffer.height()-1)))
																													{
																														int stride = pixelformatbuffer.GetStride();

																														rendererbuffer->Set(pixelformatbuffer.pix_ptr(r.x1, stride < 0 ? r.y2 : r.y1), (r.x2 - r.x1) + 1, (r.y2 - r.y1) + 1, stride);
																														
																														return true;
																													}

																												return false;
																											}

        
		BLENDER&																					GetBlender													()															{ return blender;																	}

    XDWORD																						GetWidth														() const												{ return rendererbuffer->GetWidth();							}
    XDWORD																						GetHeight														() const												{ return rendererbuffer->GetHeight();							}
    int																								GetStride														() const												{ return rendererbuffer->GetStride();							}

    XBYTE*																						Row																	(int y)													{ return rendererbuffer->Row(y);									}
    const XBYTE*																			Row																	(int y) const										{ return rendererbuffer->Row(y);									}
    ROWDATA																						RowData															(int y) const										{ return rendererbuffer->RowData(y);							}

    
    XBYTE*																						GetPtr															(int x, int y)									{ return rendererbuffer->Row(y) + x * PIXELWIDTH; }
    const XBYTE*																			GetPtr															(int x, int y) const						{ return rendererbuffer->Row(y) + x * PIXELWIDTH; }


    COLORTYPE																					GetPixel														(int x, int y) const
																											{
																												VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(y) + x + x + x;
																												return COLORTYPE(p[ORDERTYPE::R], p[ORDERTYPE::G], p[ORDERTYPE::B]);
																											}

    static void																				PutPixel														(XBYTE* p, const COLORTYPE& c)	
																											{	
																												((VALUETYPE*)p)[ORDERTYPE::R] = c.r;
																												((VALUETYPE*)p)[ORDERTYPE::G] = c.g;
																												((VALUETYPE*)p)[ORDERTYPE::B] = c.b;
																											}

		void																							PutPixel														(int x, int y, const COLORTYPE& c)
																											{
																												VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, 1) + x + x + x;

																												p[ORDERTYPE::R] = c.r;
																												p[ORDERTYPE::G] = c.g;
																												p[ORDERTYPE::B] = c.b;
																											}

        
		void																							PutBlendPixel												(int x, int y, const COLORTYPE& c, XBYTE cover)
																											{
																												PutOrBlendPixel((VALUETYPE*)rendererbuffer->Row(x, y, 1) + x + x + x, c, cover);
																											}

     
		void																							PutHLine														(int x, int y, XDWORD size, const COLORTYPE& c)
																											{
																													VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, size) + x + x + x;

																													do{ p[ORDERTYPE::R] = c.r; 
																															p[ORDERTYPE::G] = c.g; 
																															p[ORDERTYPE::B] = c.b;
																															p += 3;

																													} while(--size);
																											}

       
		void																							PutVLine														(int x, int y,	XDWORD size, const COLORTYPE& c)
																											{
																												do{	VALUETYPE* p = (VALUETYPE*)
																														rendererbuffer->Row(x, y++, 1) + x + x + x;
																														
																														p[ORDERTYPE::R] = c.r; 
																														p[ORDERTYPE::G] = c.g; 
																														p[ORDERTYPE::B] = c.b;

																													} while(--size);
																											}


        
		void																							PutBlendHLine												(int x, int y,	XDWORD size,	const COLORTYPE& c, XBYTE cover)
																											{
																												if(c.a)
																													{
																														VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, size) + x + x + x;

																														CALCTYPE alpha = (CALCTYPE(c.a) * (CALCTYPE(cover) + 1)) >> 8;
																														if(alpha == BASESCALE_MASK)
																															{
																																do{	p[ORDERTYPE::R] = c.r; 
																																		p[ORDERTYPE::G] = c.g; 
																																		p[ORDERTYPE::B] = c.b;
																																		p += 3;

																																	} while(--size);
																															}
																														 else
																															{
																																do{ blender.PutBlendPixel(p, c.r, c.g, c.b, alpha, cover);
																																		p += 3;

																																	} while(--size);
																															}
																													}
																											}

      
		void																							PutBlendVLine												(int x, int y,	XDWORD size, const COLORTYPE& c, XBYTE cover)
																											{
																												if(c.a)
																													{
																														VALUETYPE* p;
																														CALCTYPE alpha = (CALCTYPE(c.a) * (cover + 1)) >> 8;
																														if(alpha == BASESCALE_MASK)
																															{
																																do{	p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + x + x + x;

																																		p[ORDERTYPE::R] = c.r; 
																																		p[ORDERTYPE::G] = c.g; 
																																		p[ORDERTYPE::B] = c.b;

																																	} while(--size);
																															}
																														 else
																															{
																																do{ p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + x + x + x;

																																		blender.PutBlendPixel(p, c.r, c.g, c.b, alpha, cover);

																																	} while(--size);
																															}
																													}
																											}


		void																							PutBlendSolidHSpan									(int x, int y, XDWORD size, const COLORTYPE& c, const XBYTE* covers)
																											{
																												if(c.a)
																													{
																														VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, size) + x + x + x;

																														do{ CALCTYPE alpha = (CALCTYPE(c.a) * (CALCTYPE(*covers) + 1)) >> 8;
																																if(alpha == BASESCALE_MASK)
																																	{
																																		p[ORDERTYPE::R] = c.r;
																																		p[ORDERTYPE::G] = c.g;
																																		p[ORDERTYPE::B] = c.b;
																																	}
																																 else
																																	{
																																		blender.PutBlendPixel(p, c.r, c.g, c.b, alpha, *covers);
																																	}

																																p += 3;
																																++covers;

																															} while(--size);
																													}
																											}

      
    void																							PutBlendSolidVSpan									(int x, int y,	XDWORD size, const COLORTYPE& c,	const XBYTE* covers)
																											{
																												if(c.a)
																													{
																															do{	VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + x + x + x;

																																	CALCTYPE alpha = (CALCTYPE(c.a) * (CALCTYPE(*covers) + 1)) >> 8;
																																	if(alpha == BASESCALE_MASK)
																																		{
																																			p[ORDERTYPE::R] = c.r;
																																			p[ORDERTYPE::G] = c.g;
																																			p[ORDERTYPE::B] = c.b;
																																		}
																																	 else
																																		{
																																			blender.blend_pix(p, c.r, c.g, c.b, alpha, *covers);
																																		}

																																	++covers;

																															} while(--size);
																													}
																											}


        
		void																							PutColorHSpan													(int x, int y,	XDWORD size, const COLORTYPE* colors)
																											{
																												VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y, size) + x + x + x;

																												do{	p[ORDERTYPE::R] = colors->r;
																														p[ORDERTYPE::G] = colors->g;
																														p[ORDERTYPE::B] = colors->b;
																													
																														++colors;
																														p += 3;

																													}	while(--size);
																											}


		void																							PutColorVSpan													(int x, int y,	XDWORD size, const COLORTYPE* colors)
																											{
																												do{	VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + x + x + x;
																												
																														p[ORDERTYPE::R] = colors->r;
																														p[ORDERTYPE::G] = colors->g;
																														p[ORDERTYPE::B] = colors->b;

																														++colors;

																													} while(--size);
																											}

        
		void																							PutBlendColorHSpan										(int x, int y,	XDWORD size, const COLORTYPE* colors, const XBYTE* covers,	XBYTE cover)
																											{
																												VALUETYPE* p = (VALUETYPE*) rendererbuffer->Row(x, y, size) + x + x + x;
																												if(covers)
																													{
																														do{	PutOrBlendPixel(p, *colors++, *covers++);
																																p += 3;

																															} while(--size);
																													}
																												 else
																													{
																														if(cover == 255)
																															{
																																do{	PutOrBlendPixel(p, *colors++);
																																		p += 3;

																																	} while(--size);
																															}
																														 else
																															{
																																do{	PutOrBlendPixel(p, *colors++, cover);
																																		p += 3;

																																	} while(--size);
																															}
																													}
																											}

        
		void																							PutBlendColorVSpan										(int x, int y, XDWORD size, const COLORTYPE* colors,const XBYTE* covers, XBYTE cover)
																											{
																												VALUETYPE* p;
																												if(covers)
																													{
																														do{	p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + x + x + x;
																																PutOrBlendPixel(p, *colors++, *covers++);

																															} while(--size);
																													}
																												 else
																													{
																														if(cover == 255)
																															{
																																do{	p = (VALUETYPE*)
																																		rendererbuffer->Row(x, y++, 1) + x + x + x;
																																		PutOrBlendPixel(p, *colors++);

																																	} while(--size);
																															}
																														 else
																															{
																																do{	p = (VALUETYPE*)rendererbuffer->Row(x, y++, 1) + x + x + x;
																																		PutOrBlendPixel(p, *colors++, cover);

																																	} while(--size);
																															}
																													}
																											}

        
		template<class FUNCTION> 
		void																							ForEachPixel													(FUNCTION f)
																											{
																												XDWORD y;
																												for(y = 0; y < GetHeight(); ++y)
																													{
																														ROWDATA r = rendererbuffer->Row(y);
																														if(r.ptr)
																															{
																																XDWORD size = r.x2 - r.x1 + 1;
																																VALUETYPE* p = (VALUETYPE*)rendererbuffer->Row(r.x1, y, size) + r.x1 * 3;

																																do{	f(p);
																																		p += 3;

																																	} while(--size);
																															}
																													}
																											}

        
		template<class GAMMALUT> 
		void																							ApplyGammaDir													(const GAMMALUT& g)
																											{
																												ForEachPixel(APPLYGAMMADIRRGB<COLORTYPE, ORDERTYPE, GAMMALUT>(g));
																											}

       
		template<class GAMMALUT> 
		void																							ApplyGammaInv													(const GAMMALUT& g)
																											{
																												ForEachPixel(APPLYGAMMAINVRGB<COLORTYPE, ORDERTYPE, GAMMALUT>(g));
																											}

        
		template<class RENDERERBUFFER2>
    void																							CopyFrom															(const RENDERERBUFFER2& from, int xdst, int ydst, int xsrc, int ysrc, XDWORD size)
																											{
																												const XBYTE* p = from.Row(ysrc);
																												if(p) memmove(rendererbuffer->Row(xdst, ydst, size) + xdst * PIXELWIDTH, p + xsrc * PIXELWIDTH, size * PIXELWIDTH);																												
																											}


        
		template<class SOURCEBUFFERPIXELFORMAT>
    void																							PutBlendFrom													(const SOURCEBUFFERPIXELFORMAT& from, int xdst, int ydst, int xsrc, int ysrc, XDWORD size, XBYTE cover)
																											{
																												typedef typename SOURCEBUFFERPIXELFORMAT::ORDERTYPE SOURCEORDER;

																												const VALUETYPE* psrc = (const VALUETYPE*)from.Row(ysrc);
																												if(psrc)
																													{
																														psrc += xsrc * 4;
																														VALUETYPE* pdst = (VALUETYPE*)rendererbuffer->Row(xdst, ydst, size) + xdst * 3;   

																														if(cover == 255)
																															{
																																do{	VALUETYPE alpha = psrc[SOURCEORDER::A];
																																		if(alpha)
																																			{
																																				if(alpha == BASESCALE_MASK)
																																					{
																																						pdst[ORDERTYPE::R] = psrc[SOURCEORDER::R];
																																						pdst[ORDERTYPE::G] = psrc[SOURCEORDER::G];
																																						pdst[ORDERTYPE::B] = psrc[SOURCEORDER::B];
																																					}
																																					else
																																					{
																																						blender.PutBlendPixel(pdst, psrc[SOURCEORDER::R], psrc[SOURCEORDER::G], psrc[SOURCEORDER::B], alpha);
																																					}
																																			}

																																		psrc += 4;
																																		pdst += 3;

																																	} while(--size);
																															}
																														 else
																															{
																																COLORTYPE color;

																																do{	color.r = psrc[SOURCEORDER::R];
																																		color.g = psrc[SOURCEORDER::G];
																																		color.b = psrc[SOURCEORDER::B];
																																		color.a = psrc[SOURCEORDER::A];
																																	
																																		PutOrBlendPixel(pdst, color, cover);

																																		psrc += 4;
																																		pdst += 3;

																																	} while(--size);
																															}
																													}
																											}

        
		template<class SOURCEBUFFERPIXELFORMAT>
    void																							PutBlendFromColor											(const SOURCEBUFFERPIXELFORMAT& from, const COLORTYPE& color, int xdst, int ydst, int xsrc, int ysrc, XDWORD size, XBYTE cover)
																											{
																												typedef typename SOURCEBUFFERPIXELFORMAT::VALUETYPE SOURCEVALUETYPE;

																												const SOURCEVALUETYPE* psrc = (SOURCEVALUETYPE*)from.Row(ysrc);
																												if(psrc)
																													{
																														VALUETYPE* pdst = (VALUETYPE*)rendererbuffer->Row(xdst, ydst, size) + xdst * 3;

																														do{ PutOrBlendPixel(pdst, color, (*psrc * cover + BASESCALE_MASK) >> BASESCALE_SHIFT);
																																++psrc;
																																pdst += 3;

																															} while(--size);
																													}
																											}

        
		template<class SOURCEBUFFERPIXELFORMAT>
    void																							PutBlendFromLut												(const SOURCEBUFFERPIXELFORMAT& from, const COLORTYPE* colorlut, int xdst, int ydst, int xsrc, int ysrc, XDWORD size, XBYTE cover)
																											{
																												typedef typename SOURCEBUFFERPIXELFORMAT::VALUETYPE SOURCEVALUETYPE;

																												const SOURCEVALUETYPE* psrc = (SOURCEVALUETYPE*)from.Row(ysrc);
																												if(psrc)
																													{
																														VALUETYPE* pdst = (VALUETYPE*)rendererbuffer->Row(xdst, ydst, size) + xdst * 3;

																														if(cover == 255)
																															{
																																do{	const COLORTYPE& color = colorlut[*psrc];
																																		blender.PutBlendPixel(pdst, color.r, color.g, color.b, color.a);

																																		++psrc;
																																		pdst += 3;

																																	} while(--size);
																															}
																														 else
																															{
																																do{ PutOrBlendPixel(pdst, colorlut[*psrc], cover);
																																		++psrc;
																																		pdst += 3;

																																	} while(--size);
																															}
																													}
																											}

		private:
		
    
			void																						PutOrBlendPixel												(VALUETYPE* p, const COLORTYPE& c, XDWORD cover)
																											{
																												if(c.a)
																													{
																														CALCTYPE alpha = (CALCTYPE(c.a) * (cover + 1)) >> 8;
																														if(alpha == BASESCALE_MASK)
																															{
																																p[ORDERTYPE::R] = c.r;
																																p[ORDERTYPE::G] = c.g;
																																p[ORDERTYPE::B] = c.b;
																															}
																														 else
																															{
																																blender.PutBlendPixel(p, c.r, c.g, c.b, alpha, cover);
																															}
																													}
																											}

        
		void																							PutOrBlendPixel												(VALUETYPE* p, const COLORTYPE& c)
																											{
																												if(c.a)
																													{
																														if(c.a == BASESCALE_MASK)
																															{
																																p[ORDERTYPE::R] = c.r;
																																p[ORDERTYPE::G] = c.g;
																																p[ORDERTYPE::B] = c.b;
																															}
																														 else
																															{
																																blender.PutBlendPixel(p, c.r, c.g, c.b, c.a);
																															}
																													}
																											}


		RENDERERBUFFERTYPE*																rendererbuffer;
    BLENDER																						blender;
};



typedef GRPBUFFERPIXELFORMATALPHABLENDRGB<BLENDERRGB<GRPBUFFERCOLORRGBA8, GRPBUFFERCOLORORDER_RGB>, GRPBUFFER>  GRPBUFFERPIXELFORMAT_RGB24;    
typedef GRPBUFFERPIXELFORMATALPHABLENDRGB<BLENDERRGB<GRPBUFFERCOLORRGBA8, GRPBUFFERCOLORORDER_BGR>, GRPBUFFER>  GRPBUFFERPIXELFORMAT_BGR24;    

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/


	
#endif



