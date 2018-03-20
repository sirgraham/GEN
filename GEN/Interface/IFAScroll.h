//------------------------------------------------------------------------------------------
//	IFASCROLL.H
//	
/**	
// \class 
//   
//  Scroll Indicator for the Interface
//   
//	@author	 Abraham J. Velez
//	@version 08/03/2004 11:26:19
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _IFASCROLL_H_
#define _IFASCROLL_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "IFAObj.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum IFASCROLLPOS
{
	IFASCROLLPOS_NONE					= 0 ,
	IFASCROLLPOS_XSTART						,
	IFASCROLLPOS_XEND							,
	IFASCROLLPOS_YSTART						,	
	IFASCROLLPOS_YEND							,
};


#define IFASCROLL_STANDARDSIZE				7
#define IFASCROLL_BLINKFRAMESSECOND		3

//---- CLASS -------------------------------------------------------------------------------

class GRPRECT;
class IFASKINMANAGER;

class IFASCROLL : public IFAOBJ
{
	public:
												IFASCROLL						(GRPCANVAS* canvas, GRPSCREEN* screen,INPMANAGER* inpmanager,IFASKINMANAGER* skinmanager = NULL,IFAOBJTYPE type = IFAOBJTYPE_SCROLL );
		virtual						 ~IFASCROLL						();

		bool								SetRects						(GRPRECT* rect,GRPRECT* rectfull);

		int									GetSize							();
		bool								SetSize							(int size = IFASCROLL_STANDARDSIZE);

		bool								IsAvailable						();
		bool								IsHorizontalAvailable	();
		bool								IsVerticalAvailable		();

		bool								SetOrigin						();
		bool								SetEnd							();
		bool								IsAtEnd							(IFASCROLLPOS pos);

		bool								SetStep							(int stepx,int stepy,bool animation=false,int incx=0, int incy=0);
		bool								SetGoStep						(int gotox,int gotoy,bool animation=false,int incx=0, int incy=0);
		bool								MakeStep						();
		bool								IncrementStep				(int x, int y);
		bool								IsMakeStep					();

		virtual	bool				Draw								(int x, int y);	

		bool								SetAvailable					();
		
	private:		

		bool								DrawScrollSignal		(IFAOBJLOCATION location,int x,int y);

		bool								DrawScrollH					(int x,int y,int sizebar);		
		bool								DrawScrollV					(int x,int y,int sizebar);
		
		void								Clean								();		

		GRPRECT*						objrect;
		GRPRECT*						objrectfull;

		bool								blinkstatus;

		bool								horizontalON;
		bool								verticalON;

		int									size;

		int									posx;
		int								  posy;
		int									stepx;
		int									stepy;	

		bool								animation;
		int									animationincx;
		int									animationincy;
		int									animationstepx;
		int									animationstepy;
		
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

