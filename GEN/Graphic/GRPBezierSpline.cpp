
/*------------------------------------------------------------------------------------------
//	GRPBEZIERSPLINE.CPP
//	
//	Holds bezier spline data
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 16/02/2015 17:19:34
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>	

#include "GRPBezierSpline.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::ResolveBezier
*/	
/**	
//	
//	Resolves cubic bezier anchors
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/12/2014 10:17:13
//	
//	@return 			GRPSPLINE* : 
//
//  @param				inputsp : 
//  @param				nsegments : 
*/
/*-----------------------------------------------------------------*/
GRPSPLINE* GRPBEZIERSPLINE::ResolveCubicBezier(GRPSPLINE* inputsp, int nsegments)
{
	GRPSPLINE* result = new GRPSPLINE();
	if(!result) return NULL;

	XVECTOR<XVECTOR<GRPPOINT*>* > points;
	char													endpointcount = 0;
	int														endpoint			=	0;
	float												deltaT				=  1.0f/(float)fabs(float(nsegments)>1.0f ? float(nsegments-1) : (float)1);

	// Prepare anchors and endpoints on XVECTORs
	const XDWORD ni=inputsp->points.GetSize();
	for(XDWORD i=0; i<ni ; i++)
		{
			GRPANCHORPOINT* anchor = 	inputsp->points.FastGet(i);
			GRPPOINT*				point	 =	new GRPPOINT(anchor->value.x, anchor->value.y, anchor->value.z);

			if((endpointcount == 0) && (anchor->type == GRPANCHORPOINTTYPE_CUBIC_BEZIER))  //is an endpoint and the first one, we create his xvector
				{
					points.Add(new XVECTOR<GRPPOINT*>());
					points.GetLast()->Add(point);
					
					endpointcount++;
					continue;
				}

			points.GetLast()->Add(point);  //add point to current vector (current endpoint pair)

			if((endpointcount != 0) && (anchor->type == GRPANCHORPOINTTYPE_CUBIC_BEZIER) &&  (i != inputsp->points.GetSize() - 1))  //add endpoints
				{
					points.Add(new XVECTOR<GRPPOINT*>());
					points.GetLast()->Add(point);
					endpointcount++;
				}
		}
	
	// Resolve
	const XDWORD np=points.GetSize();
	for(XDWORD i=0; i<np; i++)
		{
			for(float t=0.0f; t<=1.0f; t+=deltaT)
				{
					GRPPOINT point = InterpolateBezier(points.FastGet(i), t);
					
					result->Add(GRPANCHORPOINTTYPE_LINE,GRPPOINT(point.x, point.y, point.z));
				}
		}

	// Cleaning 
	for(XDWORD e=0; e<np;	e++)		
		{
			points.FastGet(e)->DeleteContents();
			points.FastGet(e)->DeleteAll();
		}

	points.DeleteContents();
	points.DeleteAll();

	// Return resulting line spline
	return result;
}




/*-------------------------------------------------------------------
//	GRPBEZIERSPLINE::InterpolateBezier
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/02/2015 9:10:58
//	
//	@return 			GRPPOINT : 
//
//  @param				points : 
//  @param				t : 
*/
/*-----------------------------------------------------------------*/
GRPPOINT GRPBEZIERSPLINE::InterpolateBezier(XVECTOR<GRPPOINT*>* points, float t)
{
	if (!points) return GRPPOINT();

	GRPPOINT	point;
						point.z=0;

	int	 len = (int)points->GetSize();

	for(int i=0; i<len; i++)
		{
			GRPPOINT* pointtempo	= points->Get(i);
			if (!pointtempo) continue;

			float	 mult	= 1.0;/*
			if ((t==0.0f && i<=0) || (t==1 && (len - 1 - i)<=0))		 //  excepcion de dominio : Domain error occurs if base is 0 and exp is less than or equal to 0. Nan is returned
						 mult=0.0;
			else*/
						 mult	= CombinatorialPermutation((float)(len - 1), (float)i) * (float)pow(1 - t, int(len - 1 - i)) * (float)pow(t, int(i));

			point.x += pointtempo->x * mult;
			point.y += pointtempo->y * mult;
		}

	return point;
}

/*-------------------------------------------------------------------
//	GRPBEZIERSPLINE::InterpolateBezier
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/02/2015 9:10:58
//	
//	@return 			GRPPOINT : 
//
//  @param				points : 
//  @param				t : 
*/
/*-----------------------------------------------------------------*/
/*
float GRPBEZIERSPLINE::InterpolateBezierAmplitude(float t)
{
	float a=0.0f;	
	int	 len = (int)this->first->GetSize();

	for(int i=0; i<len; i++)
		{
			GRPPOINT* pointtempo	= this->first->FastGet(i);
			
			float	 mult	= 1.0;
			if ((t==0.0f && i<=0) || (t==1 && (len - 1 - i)<=0))		 //  excepcion de dominio : Domain error occurs if base is 0 and exp is less than or equal to 0. Nan is returned
						 mult=0.0;
			else
						 mult = CombinatorialPermutation((float)(len - 1), (float)i) * pow(1 - t, int(len - 1 - i)) * pow(t, int(i));
			
			a += pointtempo->y * mult;
		}

	return a;
}
*/


/*-------------------------------------------------------------------
//	GRPBEZIERSPLINE::Factorial
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/02/2015 9:10:52
//	
//	@return 			double : 
//
//  @param				num : 
*/
/*-----------------------------------------------------------------*/
float GRPBEZIERSPLINE::Factorial(float num)
{
	if(num <= 1) return 1;
	
	float fac = 1;
	
	for(float i=1; i<=num; i++)
		{
			fac *= i;
		}

	return fac;
}




/*-------------------------------------------------------------------
//	GRPBEZIERSPLINE::CombinatorialPermutation
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/02/2015 9:10:46
//	
//	@return 			double : 
//
//  @param				a : 
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
float GRPBEZIERSPLINE::CombinatorialPermutation(float a, float b)
{
	float f=(Factorial(b) * Factorial(a - b));
	if (f!=0.0f)
	return Factorial(a) / f;

	return 0.0f;
}




/*-------------------------------------------------------------------
//	GRPBEZIERSPLINE::Init
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/02/2015 9:10:40
//	
//  @param				inputsp : 
*/
/*-----------------------------------------------------------------*/
void GRPBEZIERSPLINE::Init(GRPSPLINE* inputsp)
{
	char endpointcount = 0;
	int	 endpoint			 = 0;

	// Prepare anchors and endpoints on XVECTORs
	for(XDWORD i=0; i<inputsp->points.GetSize(); i++)
		{
			GRPANCHORPOINT* anchor	= inputsp->points.Get(i);
			GRPPOINT*				point		=	new GRPPOINT(anchor->value.x,anchor->value.y,anchor->value.z);

			//XDEBUG_PRINTCOLOR(0,__L("BS: %f , %f , %f , %f"),point->x,point->y,point->z);

			if(!point) 
				break;

			if((endpointcount == 0) && (anchor->type == GRPANCHORPOINTTYPE_CUBIC_BEZIER)) //is an endpoint and the first one, we create his xvector
				{
					points.Add(new XVECTOR<GRPPOINT*>());
					points.GetLast()->Add(point);
					endpointcount++;

					continue;
				}

			points.GetLast()->Add(point);  //add point to current vector (current endpoint pair)

			if((endpointcount != 0) && (anchor->type == GRPANCHORPOINTTYPE_CUBIC_BEZIER) && (i != inputsp->points.GetSize() - 1))  //add endpoints
				{	
					XVECTOR<GRPPOINT*>* vectorpoints = new XVECTOR<GRPPOINT*>();
					if(vectorpoints)
						{
							points.Add(vectorpoints);
							points.GetLast()->Add(point);
					
							endpointcount++;
						}
				}
		}	

	first=this->points.Get(0);
}
	



/*-------------------------------------------------------------------
//	GRPBEZIERSPLINE::GetStep
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/02/2015 13:04:16
//	
//	@return 			double : 
//
//  @param				t : 
*/
/*-----------------------------------------------------------------*/
float GRPBEZIERSPLINE::GetStep(float t)
{
	return this->InterpolateBezier(t).y;	


	/*
	float a=0.0f;	
	int	 len = (int)this->first->GetSize();

	for(int i=0; i<len; i++)
		{			
			float	 mult		 = CombinatorialPermutation((float)(len - 1), (float)i) * pow(1 - t, int(len - 1 - i)) * pow(t, int(i));			
			a							+= this->first->FastGet(i)->y * mult;
		}	
		return a;
		*/

}



/*-------------------------------------------------------------------
//	GRPBEZIERCURVE::Init
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/06/2015 14:16:51
//	
//  @param				inputsp : 
*/
/*-----------------------------------------------------------------*/
void GRPBEZIERCURVE::Init(GRPSPLINE* inputsp)
{		
	if (inputsp->points.GetSize()<4) return;	

	if (!points.Add(new XVECTOR<GRPPOINT*>()))
		return;

	XDWORD npoints=inputsp->points.GetSize();
	for(XDWORD i=0; i<npoints; i++)
	{
			GRPANCHORPOINT* anchor	= inputsp->points.FastGet(i);
			GRPPOINT*				point		=	new GRPPOINT(anchor->value.x,anchor->value.y,anchor->value.z);
			this->points.FastGet(0)->Add(point);
	}

	p0=this->points.FastGet(0)->FastGet(0);
	p1=this->points.FastGet(0)->FastGet(1);
	p2=this->points.FastGet(0)->FastGet(2);
	p3=this->points.FastGet(0)->FastGet(3);

	first=points.FastGet(0);
}

/*
GRPPOINT&	GRPBEZIERCURVE::Interpolate									(GRPPOINT& a,GRPPOINT& b,double t)
{
	GRPPOINT result;

	result.x=a.x+((b.x-a.x)*(float)t);
	result.y=a.y+((b.y-a.y)*(float)t);	

	return result;
}^
*/

GRPPOINT	GRPBEZIERCURVE::InterpolateBezier						(XVECTOR<GRPPOINT*>* points, float t)
{
	//if (!points)               return GRPPOINT();
  if (points->GetSize()<4)   return GRPPOINT();

	p0=*points->FastGet(0);
	p1=*points->FastGet(1);
	p2=*points->FastGet(2);
	p3=*points->FastGet(3);

	Sa=GRPPOINT::Interpolate(p0,p1,t);
	ab=GRPPOINT::Interpolate(p1,p2,t);
	bE=GRPPOINT::Interpolate(p2,p3,t);

	Sab=GRPPOINT::Interpolate(Sa,ab,t);
	abE=GRPPOINT::Interpolate(ab,bE,t);

	Result=GRPPOINT::Interpolate(Sab,abE,t);

	return Result;
}
/*
GRPPOINT	GRPBEZIERCURVE::InterpolateBezier						(float t)
{
	Sa=GRPPOINT::Interpolate(p0,p1,t);
	ab=GRPPOINT::Interpolate(p1,p2,t);
	bE=GRPPOINT::Interpolate(p2,p3,t);

	Sab=GRPPOINT::Interpolate(Sa,ab,t);
	abE=GRPPOINT::Interpolate(ab,bE,t);

	Result=GRPPOINT::Interpolate(Sab,abE,t);

	return Result;
}
*/

GRPPOINT	GRPBEZIERCURVE::InterpolateBezier						(float t)
{
	ab=GRPPOINT::Interpolate(p1,p2,t);
	return GRPPOINT::Interpolate(GRPPOINT::Interpolate(GRPPOINT::Interpolate(p0,p1,t),ab,t),GRPPOINT::Interpolate(ab,GRPPOINT::Interpolate(p2,p3,t),t),t);
}
