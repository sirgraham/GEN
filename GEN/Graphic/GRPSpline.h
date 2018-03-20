/*------------------------------------------------------------------------------------------
//	GRPSPLINE.H
*/	
/**	
// \class 
//   
//  GRP Spline class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 23/06/2014 13:41:05
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPSPLINE_H_
#define _GRPSPLINE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"

#include "GRPPoint.h"
#include "GRPBoundingBox.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
enum GRPANCHORPOINTTYPE
{
	GRPANCHORPOINTTYPE_UNKNOWN							=	-1 ,
	GRPANCHORPOINTTYPE_LINE											 ,
	GRPANCHORPOINTTYPE_QUADRIC_BEZIER						 ,
	GRPANCHORPOINTTYPE_CUBIC_BEZIER							 ,
	GRPANCHORPOINTTYPE_CONTROLPOINT
};

	
/*---- CLASS -----------------------------------------------------------------------------*/


class GRPANCHORPOINT
{
	public:
																GRPANCHORPOINT							(GRPANCHORPOINTTYPE type) : type(type) {};
																GRPANCHORPOINT							(GRPANCHORPOINTTYPE type, GRPPOINT point)	: type(type), value(point)		{	}
															 ~GRPANCHORPOINT							()																																			{ }
																operator GRPPOINT						()																																			{ return this->value;						}


		GRPANCHORPOINTTYPE					type;
		GRPPOINT										value;	
		
};




class GRPSEGMENT
{
	public:
	
		GRPPOINT										a;
		GRPPOINT										b;
};




class GRPSPLINE
{
	public:							
																GRPSPLINE										();								
												 			 ~GRPSPLINE										();

		bool												Add													(GRPSPLINE* spline);		
		bool												Add													(GRPANCHORPOINTTYPE type, GRPPOINT point);		

		bool												IsReversed									();
		bool												Reverse											();

		GRPSPLINE&									operator+										(GRPSPLINE other);
		
		GRPBB*											CalculateBoundingBox				();
			
		GRPBB*											GetBoundingBox							()																																		{ return &boundingbox;							}

		void												CalculateSegments						();

		static bool									onSegment										(GRPPOINT* p,									GRPPOINT* q,				GRPPOINT* r);
		static bool									onSegment										(GRPANCHORPOINT* p,						GRPANCHORPOINT* q,	GRPANCHORPOINT* r);
		static int									orientation									(GRPPOINT* p,									GRPPOINT* q,				GRPPOINT* r);
		static int									orientation									(GRPANCHORPOINT* p,						GRPANCHORPOINT* q,	GRPANCHORPOINT* r);
		static bool									doIntersect									(GRPPOINT* p1,								GRPPOINT* q1,			  GRPPOINT* p2, GRPPOINT* q2);
		static bool									doIntersect									(GRPANCHORPOINT* p1,					GRPANCHORPOINT* q1, GRPANCHORPOINT* p2, GRPANCHORPOINT* q2);
		static bool									isInside										(XVECTOR<GRPPOINT*>* polygon, GRPPOINT* p);
		static bool									isInside										(GRPSPLINE* polygon,					GRPANCHORPOINT* p);
		bool												isInside										(GRPANCHORPOINT* p);

		GRPSPLINE*									CreateCopy()
																{
																	GRPSPLINE* spline=new GRPSPLINE();
																	if(spline)
																		{
																			for (int e=0;e<(int)this->points.GetSize();e++)																				
																				spline->Add(this->points.Get(e)->type,this->points.Get(e)->value);					
																		}
																	
																	return spline;
																}
		
	
		void												Print												()
																{
																	if(origin != NULL)  
																	{
																		XDEBUG_PRINTCOLOR(1,__L("   Origin: (%f,%f)"), origin->value.x, origin->value.y);
																	}

																	for(XDWORD e=0;e< points.GetSize(); e++)
																		{
																			XSTRING _type;
			
																			switch(points.Get(e)->type)
																				{
																					case GRPANCHORPOINTTYPE_LINE						:		_type.Set(__L("L"));	break;
																					case GRPANCHORPOINTTYPE_QUADRIC_BEZIER	:		_type.Set(__L("B"));	break;
																					case GRPANCHORPOINTTYPE_CONTROLPOINT		:		_type.Set(__L("C"));  break;
																											default											:		_type.Set(__L("L"));	break; // just in case?
																				}
								
																			XDEBUG_PRINTCOLOR(1,__L("    [%d] %s (%f,%f)"), e , _type.Get(), points.Get(e)->value.x, points.Get(e)->value.y);
																		}
																}	
	
		int													nsegments;
		GRPBB												boundingbox;
		GRPANCHORPOINT*							origin;			
		XVECTOR<GRPANCHORPOINT*>		points;

		XVECTOR<float>							percents;
		
};	
	


class GRPSEAM
{
	public:
		
																GRPSEAM										() :	Inner					(NULL),
																																Contour				(NULL),
																																InnerAnchor		(NULL),
																																OuterAnchor		(NULL),
																																InnerSeamIndex(-1),
																																OutSeamIndex	(-1),
																																ContourIndex	(-1),
																																InnerIndex		(-1)																																
																{
																

																}

		GRPSPLINE*									Inner;
		GRPSPLINE*									Contour;

		GRPANCHORPOINT*							InnerAnchor;
		GRPANCHORPOINT*							OuterAnchor;

		int													InnerSeamIndex;
		int													InnerIndex;
		int													OutSeamIndex;
		int													ContourIndex;

		float												m;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

