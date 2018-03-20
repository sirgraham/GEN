//------------------------------------------------------------------------------------------
//	XBASE.H
//	
/**	
// \class 
//   
//  eXtended Base
//   
//	@author	 Abraham J. Velez
//	@version 17/11/2001
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XBASE_H_
#define _XBASE_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------
		

//---- DEFINES & ENUMS  --------------------------------------------------------------------


#ifndef XBYTE
typedef unsigned char					XBYTE;
#endif

#ifndef XWORD
typedef unsigned short				XWORD;
#endif

#ifndef XDWORD
typedef unsigned int					XDWORD;
#endif

#ifndef XQWORD
typedef unsigned long long		XQWORD;
#endif


#ifndef XBYTESIG
typedef signed char						XBYTESIG;
#endif

#ifndef XWORDSIG
typedef signed short					XWORDSIG;
#endif

#ifndef XDWORDSIG
typedef signed int						XDWORDSIG;
#endif

#ifndef XQWORDSIG
typedef signed long long			XQWORDSIG;
#endif



#ifndef POINTER
typedef void*									POINTER;
#endif

#ifndef NULL
#define NULL									0
#endif

#define NOTFOUND							-1

#define ISEQUAL								 0
#define	ISGREATER							 1
#define ISLESS                -1

#define COMMENT SLASH(/)
#define SLASH(s) /##s

#ifndef __MAX
#define __MAX(a,b)						(((a)>(b))?(a):(b))
#endif

#ifndef __MIN
#define __MIN(a,b)						(((a)<(b))?(a):(b))
#endif

#ifdef MICROCONTROLLER
#define _MAXSTR			          64
#else
#define _MAXSTR			          256
#endif
#define _MAXPATH		          _MAXSTR
#define _MAXBUFFER	          _MAXSTR

#define PI										3.141592654f

#ifndef MPI
#define MPI										1.570796327f
#endif

#define EPSILON								0.00000000001f

#define DEGREE2RADIAN(c)			((c*PI)/180.0f)//((c / 2.0f / 180.0f) * PI)
#define RADIAN2DEGREE(c)			((c * 180.0f) / PI)

#define SWAPWORD(data)        data = SwapWORD(data);
#define SWAPDWORD(data)       data = SwapDWORD(data);
#define SWAPQWORD(data)       data = SwapQWORD(data);

typedef bool (*PROGRESS_FUNCTION)(void);

#define	FRAMEWORKNAME					"GEN"
#define	FRAMEWORKVERSION			"0.1.0"


#define TRUNCATE(number)			(number - fmod(number, 0.01))


//---- CLASS -------------------------------------------------------------------------------


//---- INLINE FUNCTIONS --------------------------------------------------------------------


double Truncate       (double value); 
double Fraction				(double value);
	
XWORD		SwapWORD		  (XWORD  data);
XDWORD	SwapDWORD		  (XDWORD data);
XQWORD	SwapQWORD			(XQWORD data);

float	 RoundFloat			(float x);

XQWORD DWORDToBCD			(XDWORD dword);
XDWORD BCDToDWORD			(XQWORD bcd);


#endif

