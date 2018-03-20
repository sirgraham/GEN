/*------------------------------------------------------------------------------------------
//	GRPBUFFERCOLORRGBA.H
*/	
/**	
// \class 
//   
//  Graphics Color RGBA Class, RGBA8 Class, RGBA16 Class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//	@version 01/08/2013 10:13:24
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPBUFFERCOLORRGBA_H_
#define _GRPBUFFERCOLORRGBA_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPBuffer.h"
#include "GRPBufferColor.h"

#include "XMemory.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	

struct GRPBUFFERCOLORORDER_RGB  { enum { R=0, G=1, B=2			}; };
struct GRPBUFFERCOLORORDER_BGR  { enum { B=0, G=1, R=2		  }; };
struct GRPBUFFERCOLORORDER_RGBA { enum { R=0, G=1, B=2, A=3 }; };
struct GRPBUFFERCOLORORDER_ARGB { enum { A=0, R=1, G=2, B=3 }; };
struct GRPBUFFERCOLORORDER_ABGR { enum { A=0, B=1, G=2, R=3 }; };
struct GRPBUFFERCOLORORDER_BGRA { enum { B=0, G=1, R=2, A=3 }; };


/*---- CLASS -----------------------------------------------------------------------------*/


class GRPBUFFERCOLORRGBA : public GRPBUFFERCOLOR
{
	public:

	  typedef double								VALUETYPE;

																	GRPBUFFERCOLORRGBA														() 
																	{
																		Clear();
																	}

        
																	GRPBUFFERCOLORRGBA														(double r, double g, double b, double a = 1.0) 
																	{
																		this->r = r;
																		this->g	= g;
																		this->b	= b;
																		this->a	= a;
																	}

        
																	GRPBUFFERCOLORRGBA														(const GRPBUFFERCOLORRGBA& c, double a) 
																	{
																		this->r = c.r;
																		this->g	= c.g;
																		this->b	= c.b;
																		this->a = a;
																	}

																	GRPBUFFERCOLORRGBA														(double wavelen, double gamma	=	1.0)
																	{
																		*this = FromWaveLength(wavelen, gamma);
																	}


		virtual											 ~GRPBUFFERCOLORRGBA														() 
																	{
																		Clear();
																	}

        
		void													Clear()
																	{
																		r = 0;
																		g = 0;
																		b = 0;
																		a = 0;
																	}

        
    const GRPBUFFERCOLORRGBA&			Transparent																		()
																	{
																		a = 0.0;
																		return *this;
																	}

    
		const GRPBUFFERCOLORRGBA&			Opacity																				(double a)
																	{
																		if(a < 0.0) a = 0.0;
																		if(a > 1.0) a = 1.0;
																
																		this->a = a;

																		return *this;
																	}


		double												Opacity() const
																	{
																		return a;
																	}


		const GRPBUFFERCOLORRGBA&			PreMultiply																		()
																	{
																		r *= a;
																		g *= a;
																		b *= a;

																		return *this;
																	}

        
    const GRPBUFFERCOLORRGBA&			PreMultiply																		(double a)
																	{
																		if(this->a <= 0.0 || a <= 0.0)
																			{
																				r = 0.0;
																				g = 0.0;
																				b = 0.0;
																				a = 0.0;
																				
																				return *this;
																			}
																
																		a /= this->a;
																		r *= a;
																		g *= a;
																		b *= a;
																		a  = a;

																		return *this;																		
																	}

        
		const GRPBUFFERCOLORRGBA&			DeMultiply																		()
																	{
																		if(a == 0)
																			{
																				r = g = b = 0;
																				return *this;
																			}

																		double _a = 1.0 / a;

																		r *= _a;
																		g *= _a;
																		b *= _a;

																		return *this;
																	}

    
		GRPBUFFERCOLORRGBA						Gradient																			(GRPBUFFERCOLORRGBA c, double k) const
																	{
																		GRPBUFFERCOLORRGBA ret;
															
																		ret.r = r + (c.r - r) * k;
																		ret.g = g + (c.g - g) * k;
																		ret.b = b + (c.b - b) * k;
																		ret.a = a + (c.a - a) * k;

																		return ret;
																	}


		static GRPBUFFERCOLORRGBA			GetNoColor																		() 															
																	{ 
																		return GRPBUFFERCOLORRGBA(0,0,0,0); 
																	}

        
		static GRPBUFFERCOLORRGBA			FromWaveLength																(double wl, double gamma = 1.0)
																	{
																		GRPBUFFERCOLORRGBA t(0.0, 0.0, 0.0);

																		if(wl >= 380.0 && wl <= 440.0)
																			{
																				t.r = -1.0 * (wl - 440.0) / (440.0 - 380.0);
																				t.b = 1.0;
																			}
																		 else 
																			if(wl >= 440.0 && wl <= 490.0)
																				{
																					t.g = (wl - 440.0) / (490.0 - 440.0);
																					t.b = 1.0;
																				}
																			 else
																				if(wl >= 490.0 && wl <= 510.0)
																					{
																						t.g = 1.0;
																						t.b = -1.0 * (wl - 510.0) / (510.0 - 490.0);
																					}
																				 else
																					if(wl >= 510.0 && wl <= 580.0)
																						{
																							t.r = (wl - 510.0) / (580.0 - 510.0);
																							t.g = 1.0;
																						}
																					 else
																						if(wl >= 580.0 && wl <= 645.0)
																							{
																								t.r = 1.0;
																								t.g = -1.0 * (wl - 645.0) / (645.0 - 580.0);
																							}
																						 else
																							if(wl >= 645.0 && wl <= 780.0)
																								{
																									t.r = 1.0;
																								}

																		double s = 1.0;

																		if(wl > 700.0)       
																			s = 0.3 + 0.7 * (780.0 - wl) / (780.0 - 700.0);
																		 else 
																			 if(wl <  420.0) s = 0.3 + 0.7 * (wl - 380.0) / (420.0 - 380.0);

																		t.r = pow(t.r * s, gamma);
																		t.g = pow(t.g * s, gamma);
																		t.b = pow(t.b * s, gamma);

																		return t;
																	}

	
		double												r;
    double												g;
    double												b;
    double												a;
		
	private:

		void													Clean																					()
																	{
																		r = 0.0;
																		g = 0.0;
																		b = 0.0;
																		a = 0.0;		
																	}

};




class GRPBUFFERCOLORRGBA8 : public GRPBUFFERCOLOR
{
	public:
  
		enum BASESCALE
    {
			BASESCALE_SHIFT = 8											,
			BASESCALE_SCALE = 1 << BASESCALE_SHIFT	,
			BASESCALE_MASK  = BASESCALE_SCALE - 1
    };
    
		typedef XBYTE									VALUETYPE;
    typedef XDWORD								CALCTYPE;
    typedef int										LONGTYPE;
		typedef GRPBUFFERCOLORRGBA8		SELFTYPE;

        
																	GRPBUFFERCOLORRGBA8														() 
																	{
																		Clean();
																	}

        
																	GRPBUFFERCOLORRGBA8														(XDWORD r, XDWORD g, XDWORD b, XDWORD a = BASESCALE_MASK)
																	{
																		this->r = VALUETYPE(r); 
																		this->g = VALUETYPE(g); 
																		this->b = VALUETYPE(b); 
																		this->a = VALUETYPE(a); 
																	}

        
																	GRPBUFFERCOLORRGBA8														(const GRPBUFFERCOLORRGBA& c, double a_) 
																	{
																		this->r = (VALUETYPE)uRound(c.r * double(BASESCALE_MASK)); 
																		this->g = (VALUETYPE)uRound(c.g * double(BASESCALE_MASK)); 
																		this->b = (VALUETYPE)uRound(c.b * double(BASESCALE_MASK)); 
																		this->a = (VALUETYPE)uRound(a	  * double(BASESCALE_MASK)); 
																	}

        
																	GRPBUFFERCOLORRGBA8														(const SELFTYPE& c, XDWORD a) 
																	{
																		this->r = c.r;
																		this->g = c.g;
																		this->b	= c.b;
																		this->a = VALUETYPE(a);
																	}

        
																	GRPBUFFERCOLORRGBA8														(const GRPBUFFERCOLORRGBA& c) 
																	{
																		this->r = (VALUETYPE)uRound(c.r * double(BASESCALE_MASK)); 
																		this->g = (VALUETYPE)uRound(c.g * double(BASESCALE_MASK));
																		this->b = (VALUETYPE)uRound(c.b * double(BASESCALE_MASK)); 
																		this->a = (VALUETYPE)uRound(c.a * double(BASESCALE_MASK));
																	}


		virtual											 ~GRPBUFFERCOLORRGBA8														() 
																	{
																		Clear();
																	}

        
		void													Clear()
																	{
																		r = g = b = a = 0;
																	}
        
        
		const SELFTYPE&								Transparent																		()
																	{
																		a = 0;
																		return *this;
																	}

        
		const SELFTYPE&								Opacity																				(double a)
																	{
																		if(a < 0.0) a = 0.0;
																		if(a > 1.0) a = 1.0;

																		this->a = (VALUETYPE)uRound(a * double(BASESCALE_MASK));
																		
																		return *this;
																	}

        
		double												Opacity() const
																	{
																		return double(a) / double(BASESCALE_MASK);
																	}

        
		const SELFTYPE&								Premultiply																		()
																	{
																		if(a == BASESCALE_MASK) return *this;
																		if(a == 0)
																			{
																				r = g = b = 0;
																				return *this;
																			}

																		r = VALUETYPE((CALCTYPE(r) * a) >> BASESCALE_SHIFT);
																		g = VALUETYPE((CALCTYPE(g) * a) >> BASESCALE_SHIFT);
																		b = VALUETYPE((CALCTYPE(b) * a) >> BASESCALE_SHIFT);
																	
																		return *this;
																	}

        
		const SELFTYPE&								Premultiply																		(XDWORD a)
																	{
																		if(this->a == BASESCALE_MASK && a >= BASESCALE_MASK) return *this;
																		if(this->a == 0 || a == 0)
																			{
																				this->r = this->g = this->b = this->a = 0;
																				return *this;
																			}

																		CALCTYPE _r = (CALCTYPE(r) * a) / this->a;
																		CALCTYPE _g = (CALCTYPE(g) * a) / this->a;
																		CALCTYPE _b = (CALCTYPE(b) * a) / this->a;
																		
																		this->r = VALUETYPE((_r > a) ? a : _r);
																		this->g = VALUETYPE((_g > a) ? a : _g);
																		this->b = VALUETYPE((_b > a) ? a : _b);
																		this->a = VALUETYPE(a);

																		return *this;
																	}

        
		const SELFTYPE&								Demultiply																			()
																	{
																		if(a == BASESCALE_MASK) return *this;
																		if(a == 0)
																			{
																				r = g = b = 0;
																				return *this;
																			}
																	
																		CALCTYPE r_ = (CALCTYPE(r) * BASESCALE_MASK) / a;
																		CALCTYPE g_ = (CALCTYPE(g) * BASESCALE_MASK) / a;
																		CALCTYPE b_ = (CALCTYPE(b) * BASESCALE_MASK) / a;

																		r = VALUETYPE((r_ > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : r_);
																		g = VALUETYPE((g_ > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : g_);
																		b = VALUETYPE((b_ > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : b_);

																		return *this;
																	}

        
		SELFTYPE											Gradient																				(const SELFTYPE& c, double k) const
																	{
																		SELFTYPE ret;
																		CALCTYPE ik = uRound(k * BASESCALE_SCALE);
																
																		ret.r = VALUETYPE(CALCTYPE(r) + (((CALCTYPE(c.r) - r) * ik) >> BASESCALE_SHIFT));
																		ret.g = VALUETYPE(CALCTYPE(g) + (((CALCTYPE(c.g) - g) * ik) >> BASESCALE_SHIFT));
																		ret.b = VALUETYPE(CALCTYPE(b) + (((CALCTYPE(c.b) - b) * ik) >> BASESCALE_SHIFT));
																		ret.a = VALUETYPE(CALCTYPE(a) + (((CALCTYPE(c.a) - a) * ik) >> BASESCALE_SHIFT));
																
																		return ret;
																	}

        
		void													Add																							(const SELFTYPE& c, XDWORD cover)
																	{
																		CALCTYPE cr, cg, cb, ca;
																		if(cover == COVERTYPE_MASK)
																			{
																				if(c.a == BASESCALE_MASK) 
																					{
																						*this = c;
																					}
																				 else
																					{
																						cr = r + c.r; r = (cr > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : cr;
																						cg = g + c.g; g = (cg > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : cg;
																						cb = b + c.b; b = (cb > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : cb;
																						ca = a + c.a; a = (ca > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : ca;
																					}
																			}
																		 else
																			{
																				cr = r + ((c.r * cover + COVERTYPE_MASK/2) >> COVERTYPE_SHIFT);
																				cg = g + ((c.g * cover + COVERTYPE_MASK/2) >> COVERTYPE_SHIFT);
																				cb = b + ((c.b * cover + COVERTYPE_MASK/2) >> COVERTYPE_SHIFT);
																				ca = a + ((c.a * cover + COVERTYPE_MASK/2) >> COVERTYPE_SHIFT);

																				r	 = (cr > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : cr;
																				g  = (cg > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : cg;
																				b  = (cb > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : cb;
																				a  = (ca > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : ca;
																			}
																	}

        
																	template<class GAMMALUT>
		void													ApplyGammaDir																		(const GAMMALUT& gamma)
																	{
																		r = gamma.Dir(r);
																		g = gamma.Dir(g);
																		b = gamma.Dir(b);
																	}

        
																	template<class GAMMALUT>
		void													ApplyGammaInv																		(const GAMMALUT& gamma)
																	{
																		r = gamma.Inv(r);
																		g = gamma.Inv(g);
																		b = gamma.Inv(b);
																	}

        
		static SELFTYPE								GetNoColor() 
																	{ 
																		return SELFTYPE(0, 0, 0, 0); 
																	}

        
		static SELFTYPE								FromWaveLength																	(double wl, double gamma = 1.0)
																	{
																		return SELFTYPE(GRPBUFFERCOLORRGBA::FromWaveLength(wl, gamma));
																	}

				
		VALUETYPE											r;
    VALUETYPE											g;
    VALUETYPE											b;
    VALUETYPE											a;

	private:

		void													Clean																						()
																	{
																		r = 0;
																		g = 0;
																		b = 0;
																		a = 0;		
																	}
};



 
class GRPBUFFERCOLORRGBA16 : public GRPBUFFERCOLOR
{
	public:
  
		enum BASESCALE
    {
			BASESCALE_SHIFT = 8											,
			BASESCALE_SCALE = 1 << BASESCALE_SHIFT	,
			BASESCALE_MASK  = BASESCALE_SCALE - 1
    };
    
		typedef XBYTE								VALUETYPE;
    typedef XDWORD							CALCTYPE;
    typedef int									LONGTYPE;
		typedef GRPBUFFERCOLORRGBA16	SELFTYPE;


																	GRPBUFFERCOLORRGBA16													() 
																	{
																		Clean();
																	}

        
																	GRPBUFFERCOLORRGBA16													(XDWORD r, XDWORD g, XDWORD b, XDWORD a = BASESCALE_MASK)
																	{
																		this->r = VALUETYPE(r); 
																		this->g = VALUETYPE(g); 
																		this->b = VALUETYPE(b); 
																		this->a = VALUETYPE(a); 
																	}

        
																	GRPBUFFERCOLORRGBA16													(const GRPBUFFERCOLORRGBA& c, double a_) 
																	{
																		this->r = (VALUETYPE)uRound(c.r * double(BASESCALE_MASK)); 
																		this->g = (VALUETYPE)uRound(c.g * double(BASESCALE_MASK)); 
																		this->b = (VALUETYPE)uRound(c.b * double(BASESCALE_MASK)); 
																		this->a = (VALUETYPE)uRound(a	  * double(BASESCALE_MASK)); 
																	}

        
																	GRPBUFFERCOLORRGBA16													(const SELFTYPE& c, XDWORD a) 
																	{
																		this->r = c.r;
																		this->g = c.g;
																		this->b	= c.b;
																		this->a = VALUETYPE(a);
																	}

        
																	GRPBUFFERCOLORRGBA16													(const GRPBUFFERCOLORRGBA& c) 
																	{
																		this->r = (VALUETYPE)uRound(c.r * double(BASESCALE_MASK)); 
																		this->g = (VALUETYPE)uRound(c.g * double(BASESCALE_MASK));
																		this->b = (VALUETYPE)uRound(c.b * double(BASESCALE_MASK)); 
																		this->a = (VALUETYPE)uRound(c.a * double(BASESCALE_MASK));
																	}															        														

															
																	GRPBUFFERCOLORRGBA16													(const GRPBUFFERCOLORRGBA8& c) 
																	{
																		this->r = VALUETYPE((VALUETYPE(c.r) << 8) | c.r);
																		this->g	= VALUETYPE((VALUETYPE(c.g) << 8) | c.g);
																		this->b	= VALUETYPE((VALUETYPE(c.b) << 8) | c.b);
																		this->a	= VALUETYPE((VALUETYPE(c.a) << 8) | c.a);
																	}

															
																	GRPBUFFERCOLORRGBA16													(const GRPBUFFERCOLORRGBA8& c, XDWORD a) 
																	{
																		this->r = VALUETYPE((VALUETYPE(c.r) << 8) | c.r); 
																		this->g = VALUETYPE((VALUETYPE(c.g) << 8) | c.g); 
																		this->b = VALUETYPE((VALUETYPE(c.b) << 8) | c.b);
																		this->a = VALUETYPE((             a << 8) | c.a); 
																	}


		virtual											 ~GRPBUFFERCOLORRGBA16													() 
																	{
																		Clear();
																	}

        
		void													Clear()
																	{
																		r = g = b = a = 0;
																	}
        
        
		const SELFTYPE&								Transparent																		()
																	{
																		a = 0;
																		return *this;
																	}

        
		const SELFTYPE&								Opacity																				(double a)
																	{
																		if(a < 0.0) a = 0.0;
																		if(a > 1.0) a = 1.0;

																		this->a = (VALUETYPE)uRound(a * double(BASESCALE_MASK));
																		
																		return *this;
																	}

        
		double											Opacity() const
																{
																	return double(a) / double(BASESCALE_MASK);
																}

        
		const SELFTYPE&							Premultiply																			()
																{
																	if(a == BASESCALE_MASK) return *this;
																	if(a == 0)
																		{
																			r = g = b = 0;
																			return *this;
																		}

																	r = VALUETYPE((CALCTYPE(r) * a) >> BASESCALE_SHIFT);
																	g = VALUETYPE((CALCTYPE(g) * a) >> BASESCALE_SHIFT);
																	b = VALUETYPE((CALCTYPE(b) * a) >> BASESCALE_SHIFT);
																	
																	return *this;
																}

        
		const SELFTYPE&							Premultiply																			(XDWORD a)
																{
																	if(this->a == BASESCALE_MASK && a >= BASESCALE_MASK) return *this;
																	if(this->a == 0 || a == 0)
																		{
																			this->r = this->g = this->b = this->a = 0;
																			return *this;
																		}

																	CALCTYPE _r = (CALCTYPE(r) * a) / this->a;
																	CALCTYPE _g = (CALCTYPE(g) * a) / this->a;
																	CALCTYPE _b = (CALCTYPE(b) * a) / this->a;
																		
																	this->r = VALUETYPE((_r > a) ? a : _r);
																	this->g = VALUETYPE((_g > a) ? a : _g);
																	this->b = VALUETYPE((_b > a) ? a : _b);
																	this->a = VALUETYPE(a);

																	return *this;
																}

        
		const SELFTYPE&							Demultiply																			()
																{
																	if(a == BASESCALE_MASK) return *this;
																	if(a == 0)
																		{
																			r = g = b = 0;
																			return *this;
																		}
																	
																	CALCTYPE r_ = (CALCTYPE(r) * BASESCALE_MASK) / a;
																	CALCTYPE g_ = (CALCTYPE(g) * BASESCALE_MASK) / a;
																	CALCTYPE b_ = (CALCTYPE(b) * BASESCALE_MASK) / a;

																	r = VALUETYPE((r_ > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : r_);
																	g = VALUETYPE((g_ > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : g_);
																	b = VALUETYPE((b_ > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : b_);

																	return *this;
																}

        
		SELFTYPE										Gradient																				(const SELFTYPE& c, double k) const
																{
																	SELFTYPE ret;
																	CALCTYPE ik = uRound(k * BASESCALE_SCALE);
																
																	ret.r = VALUETYPE(CALCTYPE(r) + (((CALCTYPE(c.r) - r) * ik) >> BASESCALE_SHIFT));
																	ret.g = VALUETYPE(CALCTYPE(g) + (((CALCTYPE(c.g) - g) * ik) >> BASESCALE_SHIFT));
																	ret.b = VALUETYPE(CALCTYPE(b) + (((CALCTYPE(c.b) - b) * ik) >> BASESCALE_SHIFT));
																	ret.a = VALUETYPE(CALCTYPE(a) + (((CALCTYPE(c.a) - a) * ik) >> BASESCALE_SHIFT));
																
																	return ret;
																}

        
		void												Add																							(const SELFTYPE& c, XDWORD cover)
																{
																	CALCTYPE cr, cg, cb, ca;
																	if(cover == COVERTYPE_MASK)
																		{
																			if(c.a == BASESCALE_MASK) 
																				{
																					*this = c;
																				}
																			 else
																				{
																					cr = r + c.r; r = (cr > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : cr;
																					cg = g + c.g; g = (cg > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : cg;
																					cb = b + c.b; b = (cb > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : cb;
																					ca = a + c.a; a = (ca > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : ca;
																				}
																		}
																	 else
																		{
																			cr = r + ((c.r * cover + COVERTYPE_MASK/2) >> COVERTYPE_SHIFT);
																			cg = g + ((c.g * cover + COVERTYPE_MASK/2) >> COVERTYPE_SHIFT);
																			cb = b + ((c.b * cover + COVERTYPE_MASK/2) >> COVERTYPE_SHIFT);
																			ca = a + ((c.a * cover + COVERTYPE_MASK/2) >> COVERTYPE_SHIFT);

																			r	 = (cr > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : cr;
																			g  = (cg > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : cg;
																			b  = (cb > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : cb;
																			a  = (ca > CALCTYPE(BASESCALE_MASK)) ? CALCTYPE(BASESCALE_MASK) : ca;
																		}
																}

        
																template<class GAMMALUT>
		void												ApplyGammaDir																		(const GAMMALUT& gamma)
																{
																	r = gamma.Dir(r);
																	g = gamma.Dir(g);
																	b = gamma.Dir(b);
																}

        
																template<class GAMMALUT>
		void												ApplyGammaInv																	(const GAMMALUT& gamma)
																{
																	r = gamma.Inv(r);
																	g = gamma.Inv(g);
																	b = gamma.Inv(b);
																}

        
		static SELFTYPE							GetNoColor																		() 
																{ 
																	return SELFTYPE(0, 0, 0, 0); 
																}

        
		static SELFTYPE							FromWaveLength																(double wl, double gamma = 1.0)
																{
																	return SELFTYPE(GRPBUFFERCOLORRGBA::FromWaveLength(wl, gamma));
																}

				
		VALUETYPE										r;
    VALUETYPE										g;
    VALUETYPE										b;
    VALUETYPE										a;

	private:

		void												Clean																					()
																{
																	r = 0;
																	g = 0;
																	b = 0;
																	a = 0;		
																}
};




	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

