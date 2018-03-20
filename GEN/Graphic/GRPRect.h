/*------------------------------------------------------------------------------------------
//	GRPRECT.H
*/	
/**	
// \class 
//   
//  Graphics Rect Class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//	@version 04/08/2013 16:46:46
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPRECT_H_
#define _GRPRECT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XMemory.h"	

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
	

template<class T> 
class GRPRECTBASE
{
	public:
  
		typedef GRPRECTBASE<T>		SELFTYPE;   
		
															GRPRECTBASE												() 
															{
																Clean();
															}


															GRPRECTBASE												(T x1, T y1, T x2, T y2)
															{
																Clean();

																Set(x1, y1, x2, y2);
															}


		virtual									 ~GRPRECTBASE												()
															{
																Clean();
															}


		void											Set																(T x1, T y1, T x2, T y2)
															{
																this->x1	= x1;
																this->y1	= y1;
																this->x2	= x2;
																this->y2	= y2;
															}

		const SELFTYPE&						Normalize													()
															{
																T t;
																if(x1 > x2) { t = x1; x1 = x2; x2 = t; }
																if(y1 > y2) { t = y1; y1 = y2; y2 = t; }

																return *this;
															}


		bool											Clip															(const SELFTYPE& r)
															{
																if(x2 > r.x2) x2 = r.x2;
																if(y2 > r.y2) y2 = r.y2;
																if(x1 < r.x1) x1 = r.x1;
																if(y1 < r.y1) y1 = r.y1;

																return x1 <= x2 && y1 <= y2;
															}
		

		bool											IsValid() const
															{
																return x1 <= x2 && y1 <= y2;
															}

		bool											IsHit															(T x, T y) const
															{
																return (x >= x1 && x <= x2 && y >= y1 && y <= y2);
															}


		int												IsEqual														(GRPRECTBASE* rect)
															{
																if(!rect) return ISLESS;

																int size[2];
															
																size[0]  = (x2 - x1) * (y2 - y1);																
																size[1]  = (rect->x2 - rect->x1) * (rect->y2 - rect->y1);
																																																										
																if(size[0] > size[1])   return ISGREATER;
																if(size[0] < size[1]) 	return ISLESS;

																return ISEQUAL;
															}


    T													x1;
    T													y1;
    T													x2;
    T													y2;

  
	private:

		void											Clean															()
															{
																x1 = 0;
    														y1 = 0;
    														x2 = 0;
																y2 = 0;		
															}       
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

    
template<class R> 
inline R IntersectRectangles(const R& r1, const R& r2)
{
	R r = r1;

  if(r.x2 > r2.x2) r.x2 = r2.x2; 
  if(r.y2 > r2.y2) r.y2 = r2.y2;
  if(r.x1 < r2.x1) r.x1 = r2.x1;
  if(r.y1 < r2.y1) r.y1 = r2.y1;

  return r;
}



template<class R> 
inline R UniteRectangles(const R& r1, const R& r2)
{
	R r = r1;
  
	if(r.x2 < r2.x2) r.x2 = r2.x2;
  if(r.y2 < r2.y2) r.y2 = r2.y2;
  if(r.x1 > r2.x1) r.x1 = r2.x1;
  if(r.y1 > r2.y1) r.y1 = r2.y1;
  
	return r;
}


typedef GRPRECTBASE<int>		GRPRECTINT;  
typedef GRPRECTBASE<float>	GRPRECTFLOAT;  
typedef GRPRECTBASE<double>	GRPRECTDOUBLE; 

	
#endif

