/*------------------------------------------------------------------------------------------
//	GRPVECTOR.H
*/	
/**	
// \class 
//   
//  Graphics Generic Vector class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 13/05/2014 12:00:23
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPVECTOR_H_
#define _GRPVECTOR_H_
	

	
#if (defined(LINUX) || defined(ANDROID)) && !defined(__clang__)
//#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#else
#pragma warning (disable : 4172)
#endif
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPPoint.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	

/*---- CLASS -----------------------------------------------------------------------------*/


class GRPVECTOR
{	
	public:		

																GRPVECTOR								();
																GRPVECTOR								(GRPPOINT point) 
																{
																	vector[0] = point.x;
																	vector[1] = point.y;
																	vector[2] = point.z;
																	vector[3] = 1.0;
																}

																GRPVECTOR								(GLFLOAT x, GLFLOAT y, GLFLOAT z);
																GRPVECTOR								(GLFLOAT v[3])
																{
																	vector[0] = v[0];
																	vector[1] = v[1];
																	vector[2] = v[2];
																	vector[3] = 1.0;
																}
																GRPVECTOR								(GLFLOAT x, GLFLOAT y, GLFLOAT z, GLFLOAT w);

																	
		const static bool						LineIntersects					(GRPPOINT* ao,	GRPPOINT* af,	GRPPOINT* bo,	GRPPOINT* bf, GRPPOINT* result);
					static int						Orientation							(GRPPOINT* p,		GRPPOINT* q,	GRPPOINT* r);
					static bool						onSegment								(GRPPOINT *p,		GRPPOINT *q,	GRPPOINT *r);
					static bool						doIntersect							(GRPPOINT* p1,	GRPPOINT* q1, GRPPOINT* p2, GRPPOINT* q2);
		const static float					AngleBetween						(GRPVECTOR* a,	GRPVECTOR* b);

		void												Rotate									(GLFLOAT angle, GLFLOAT axisx,GLFLOAT axisy,GLFLOAT axisz);

		GLFLOAT											GetLengthf							();
		GLFLOAT											GetLength2f							();
	
		void												Normalize								();

		void												GetNormalizedCopy				(GRPVECTOR* target);
	
		void												CrossProduct						(const GLFLOAT	a[3],	const GLFLOAT b[3]);
		void												CrossProduct						(GRPVECTOR*			a,		GRPVECTOR*		b);
		const static	float					DotProduct							(GRPVECTOR*			a,		GRPVECTOR*		b);
		const static	float					DotProduct							(GRPVECTOR&			a,		GRPVECTOR&		b);
		const static	float					InnerProduct						(GRPVECTOR&			a,		GRPVECTOR&		b);

		GRPVECTOR&									operator =							(GRPPOINT point)
																{
																	this->vector[0] = point.x;
																	this->vector[1] = point.y;
																	this->vector[2] = point.z;
																	return (*this);
																}

		operator GRPPOINT	()
		{
			return GRPPOINT(this->vector[0],this->vector[1],this->vector[2]);
		}

					GRPVECTOR							operator +							(GRPPOINT);
				  GRPVECTOR							operator *							(GLFLOAT Scalar);
					GRPVECTOR							operator /=							(GLFLOAT Scalar);
					GRPVECTOR							operator /							(GLFLOAT scalar);
					GRPVECTOR							operator +							(GLFLOAT scalar);
					GRPVECTOR							operator -							(GLFLOAT scalar);
					GRPVECTOR							operator -							(GRPPOINT);

					GRPVECTOR							operator =							(GLFLOAT b[3]);
					GRPVECTOR							operator +							(GLFLOAT b[3]);
					GRPVECTOR							operator -							(GLFLOAT b[3]);
	
					GRPVECTOR							operator =							(GRPVECTOR b);
					GRPVECTOR							operator +							(GRPVECTOR b);
					GRPVECTOR							operator -							(GRPVECTOR b);

					void									operator +=							(GRPVECTOR );
					void									operator -=							(GRPVECTOR);

					GRPVECTOR							operator *							(GRPVECTOR&);

		GLFLOAT											vector[4];

		static GRPVECTOR						Up;

	private:

		void												Clean										()
																{
																	vector[0]  = 0;
																	vector[1]  = 0;
																	vector[2]  = 0;
																	vector[3]  = 0;
																}
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

