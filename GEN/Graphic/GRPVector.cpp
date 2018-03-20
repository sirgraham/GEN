/*------------------------------------------------------------------------------------------
//	GRPVECTOR.CPP
//	
//	Graphics Generic vector class 
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 13/05/2014 12:02:16
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#ifdef LINUX
//#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#else
#pragma warning (disable : 4172)
#endif


#include <math.h>

#include "XBase.h"

#include "GRPOpenGL.h"
#include "GRPVector.h"
#include "GRPMatrix.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

GRPVECTOR						GRPVECTOR::Up		(0.0f,1.0f,0.0);


/*-------------------------------------------------------------------
//	GRPVECTOR::GRPVECTOR
*/	
/**	
//	
//	Class Constructor GRPVECTOR
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:44:30
//	
*/
/*-----------------------------------------------------------------*/
GRPVECTOR::GRPVECTOR()																	
{ 
		vector[0]  = 0;
		vector[1]  = 0;
		vector[2]  = 0;
		vector[3]  = 0;
}



/*-------------------------------------------------------------------
//	GRPVECTOR::GRPVECTOR
*/	
/**	
//	
//	Class Constructor GRPVECTOR
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:44:36
//	
//  @param				x : vector values
//  @param				y : vector values
//  @param				z : vector values
*/
/*-----------------------------------------------------------------*/
GRPVECTOR::GRPVECTOR(GLFLOAT x, GLFLOAT y, GLFLOAT z)
{ 
	vector[0]=x;
	vector[1]=y; 
	vector[2]=z; 
}

GRPVECTOR::GRPVECTOR(GLFLOAT x, GLFLOAT y, GLFLOAT z, GLFLOAT w)
{
		vector[0] = x;
		vector[1] = y;
		vector[2] = z;
		vector[3] = w;
}
/*-------------------------------------------------------------------
//	LineIntersects
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			27/11/2014 10:28:14
//	
//	@return 			const : 
//
//  @param				ao : 
//  @param				af : 
//  @param				bo : 
//  @param				bf : 
//  @param				result : 
*/
/*-----------------------------------------------------------------*/
// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool GRPVECTOR::onSegment(GRPPOINT *p, GRPPOINT *q, GRPPOINT *r)
{
    if (q->x <= __MAX(p->x, r->x) && q->x >=__MIN(p->x, r->x) &&
        q->y <= __MAX(p->y, r->y) && q->y >=__MIN(p->y, r->y))
       return true;
 
    return false;
}
// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int GRPVECTOR::Orientation(GRPPOINT* p, GRPPOINT* q, GRPPOINT* r)
{
    // See 10th slides from following link for derivation of the formula
    // http://www.dcs.gla.ac.uk/~pat/52233/slides/Geometry1x1.pdf
    float val = ((q->y - p->y) * (r->x - q->x) -
              (q->x - p->x) * (r->y - q->y));
 
    if (val == 0) return 0;  // colinear
 
    return (val > 0)? 1: 2; // clock or counterclock wise
}
bool GRPVECTOR::doIntersect(GRPPOINT* p1, GRPPOINT* q1, GRPPOINT* p2, GRPPOINT* q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = Orientation(p1, q1, p2);
    int o2 = Orientation(p1, q1, q2);
    int o3 = Orientation(p2, q2, p1);
    int o4 = Orientation(p2, q2, q1);
 
    // General case
    if (o1 != o2 && o3 != o4)
        return true;
 
    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
 
    // p1, q1 and p2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
 
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
 
     // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
 
    return false; // Doesn't fall in any of the above cases
}
const bool	GRPVECTOR::LineIntersects	(GRPPOINT* p1,GRPPOINT* q1,GRPPOINT* p2,GRPPOINT* q2, GRPPOINT* result)
{	 

    float x1=p1->x;
		float y1=p1->y;
		float x2=q1->x;
		float y2=q1->y; 
    
		float x3=p2->x;
		float y3=p2->y;
		float x4=q2->x;
		float y4=q2->y;    

    float d = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
    if (d == 0) 
		{
				//they are paralel , impossible colision
				return false;
		}
    
    float xi = ((x3-x4)*(x1*y2-y1*x2)-(x1-x2)*(x3*y4-y3*x4))/d;
    float yi = ((y3-y4)*(x1*y2-y1*x2)-(y1-y2)*(x3*y4-y3*x4))/d;
    
		result->x=xi;
		result->y=yi;
		
		if (GRPVECTOR::onSegment(p1,result,q1))
					return true; //the colision is on segment

		//the colision lies out of segment, but is a proyected valid collision
    return false;
}

/*-------------------------------------------------------------------
//	GRPVECTOR::AngleBetween
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			27/11/2014 10:28:08
//	
//	@return 			const : 
//
//  @param				a : 
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
const float	GRPVECTOR::AngleBetween						(GRPVECTOR* a,GRPVECTOR* b)
{
			float xdiff = b->vector[0] - a->vector[0]; 
			float ydiff = b->vector[1] - a->vector[1]; 

			float angleAB= (float)atan2(ydiff, xdiff) * (180 / PI);

			if (angleAB<0) 
				return 360.0f+angleAB;

	return angleAB;

}
/*-------------------------------------------------------------------
//	GRPVECTOR::Rotate
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/11/2014 11:51:59
//	
//  @param				angle : 
//  @param				axisx : 
//  @param				axisy : 
//  @param				axisz : 
*/
/*-----------------------------------------------------------------*/
void	GRPVECTOR::Rotate									(GLFLOAT angle, GLFLOAT axisx,GLFLOAT axisy,GLFLOAT axisz)
{
	GRPMATRIX m;						
						m.BuildRotationMatrix(angle,axisx,axisy,axisz);
	GRPVECTOR v=m.TransformPoint(*this);
						*this=v;
}


/*-------------------------------------------------------------------
//	GRPVECTOR::GetLengthf
*/	
/**	
//	
//	Gets modular length
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:45:01
//	
//	@return 			 GLFloat : length
//
*/
/*-----------------------------------------------------------------*/
GLFLOAT GRPVECTOR::GetLengthf()
{
  return sqrtf(	vector[0] * vector[0] +
								vector[1] * vector[1] +
								vector[2] * vector[2]);
}




/*-------------------------------------------------------------------
//	GRPVECTOR::GetLength2f
*/	
/**	
//	
//	gets the squared length, useful when you want to check if length is zero as you avoid the sqrt
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			16/05/2016 16:38:35
//	
//	@return 			GLFLOAT : 
//
*/
/*-----------------------------------------------------------------*/
GLFLOAT GRPVECTOR::GetLength2f()
{
	return	vector[0] * vector[0] +
					vector[1] * vector[1] +
					vector[2] * vector[2];
}


/*-------------------------------------------------------------------
//	GRPVECTOR::Normalize
*/	
/**	
//	
//	Normalize vector  ( set length equal 0)
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:45:52
//	
*/
/*-----------------------------------------------------------------*/
void GRPVECTOR::Normalize()
{
	GLFLOAT const l = GetLengthf();
	if (l!=0)
	{
		vector[0] = vector[0] / l;
		vector[1] = vector[1] / l;
		vector[2] = vector[2] / l;
	}
}




/*-------------------------------------------------------------------
//	GRPVECTOR::GetNormalizedCopy
*/	
/**	
//	
//	Copy normalized vector
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:46:06
//	
//
//  @param				Target : copy destination
*/
/*-----------------------------------------------------------------*/
void GRPVECTOR::GetNormalizedCopy(GRPVECTOR* Target)
{
  GLFLOAT const l = GetLengthf();

	Target->vector[0] = vector[0] / l;
	Target->vector[1] = vector[1] / l;
	Target->vector[2] = vector[2] / l;
}



/*-------------------------------------------------------------------
//	GRPVECTOR::CrossProduct
*/	
/**	
//	
//	Process Cross Product, return it on first param
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:46:30
//		
//  @param				a[3] : first operand
//  @param				b[3] : second operand
*/
/*-----------------------------------------------------------------*/
void GRPVECTOR::CrossProduct(const GLFLOAT a[3], const GLFLOAT b[3])
{
	GLFLOAT t[3];

	t[0] = a[1]*b[2] - a[2]*b[1];
	t[1] = a[2]*b[0] - a[0]*b[2];
	t[2] = a[0]*b[1] - a[1]*b[0];

	vector[0] = t[0];
	vector[1] = t[1];
	vector[2] = t[2];
}




/*-------------------------------------------------------------------
//	GRPVECTOR::CrossProduct
*/	
/**	
//	
//	CorssProduct Overload for GRPVECTORS
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/05/2014 12:47:36
//	
//
//  @param				a : GRPVECTOR operand 
//  @param				b : GRPVECTOR operand  
*/
/*-----------------------------------------------------------------*/
void GRPVECTOR::CrossProduct(GRPVECTOR* a, GRPVECTOR* b)
{
	GLFLOAT t[3];

	t[0] = a->vector[1]*b->vector[2] - a->vector[2]*b->vector[1];
	t[1] = a->vector[2]*b->vector[0] - a->vector[0]*b->vector[2];
	t[2] = a->vector[0]*b->vector[1] - a->vector[1]*b->vector[0];

	vector[0] = t[0];
	vector[1] = t[1];
	vector[2] = t[2];
}



/*-------------------------------------------------------------------
//	GRPVECTOR::DotProduct
*/	
/**	
//	
//	Computes Dot Product for 2 vectors and returns normalized float
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/06/2014 16:09:39
//	
//	@return 			float : normalized dotproduct (normalized angle)
//
//  @param				a : GRPVECTOR
//  @param				b : GRPVECTOR
*/
/*-----------------------------------------------------------------*/
const GLFLOAT GRPVECTOR::DotProduct(GRPVECTOR* a, GRPVECTOR* b)
{
	return (a->vector[0]*b->vector[0]) + (a->vector[1]*b->vector[1]) + (a->vector[2]*b->vector[2]);
}

const GLFLOAT GRPVECTOR::DotProduct(GRPVECTOR& a, GRPVECTOR& b)
{
		return (a.vector[0] * b.vector[0]) + (a.vector[1] * b.vector[1]) + (a.vector[2] * b.vector[2]);
}



/*-------------------------------------------------------------------
//	GRPVECTOR::operator=
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/05/2014 12:17:35
//	
//	@return 			GRPVECTOR& : 
//
//  @param				b[3] : 
*/
/*-----------------------------------------------------------------*/
GRPVECTOR  GRPVECTOR::operator=(GLFLOAT b[3])
{
	vector[0] = b[0];
	vector[1] = b[1];
	vector[2] = b[2];

	return *this;
}




/*-------------------------------------------------------------------
//	GRPVECTOR::operator=
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/05/2014 12:17:42
//	
//	@return 			GRPVECTOR& : 
//
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
GRPVECTOR  GRPVECTOR::operator=(GRPVECTOR b)
{
	vector[0] = b.vector[0];
	vector[1] = b.vector[1];
	vector[2] = b.vector[2];
	vector[3] = b.vector[3];
	return *this;
}



/*-------------------------------------------------------------------
//	GRPVECTOR::operator+
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/05/2014 12:17:54
//	
//	@return 			GRPVECTOR& : 
//
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
GRPVECTOR  GRPVECTOR::operator+(GRPVECTOR b)
{
	vector[0] = vector[0] + b.vector[0];
	vector[1] = vector[1] + b.vector[1];
	vector[2] = vector[2] + b.vector[2];
	
	return *this;
}


void GRPVECTOR::operator += (GRPVECTOR b)
{
		*this=*this+b;
}
void GRPVECTOR::operator -= (GRPVECTOR b)
{
		*this=*this-b;
}

GRPVECTOR GRPVECTOR::operator *	(GRPVECTOR& v)
{
		GRPVECTOR res;
		
		res.vector[0] = (this->vector[1] * v.vector[2]) - (this->vector[2] * v.vector[1]);
		res.vector[1] = (this->vector[2] * v.vector[0]) - (this->vector[0] * v.vector[2]);
		res.vector[2] = (this->vector[0] * v.vector[1]) - (this->vector[1] * v.vector[0]);

		return (res);
}

/*-------------------------------------------------------------------
//	GRPVECTOR::operator-
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/05/2014 12:18:05
//	
//	@return 			GRPVECTOR& : 
//
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
 GRPVECTOR  GRPVECTOR::operator-(GRPVECTOR b)
{
	vector[0] = float(vector[0]) - float(b.vector[0]);
	vector[1] = float(vector[1]) - float(b.vector[1]);
	vector[2] = float(vector[2]) - float(b.vector[2]);

	return *this;
}
	

/*-------------------------------------------------------------------
//	GRPVECTOR::operator*
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/12/2014 10:55:27
//	
//	@return 			const : 
//
//  @param				scalar : 
*/
/*-----------------------------------------------------------------*/
GRPVECTOR		GRPVECTOR::operator*		(GLFLOAT scalar)
{
	vector[0]*=scalar;
	vector[1]*=scalar;
	vector[2]*=scalar;
	return *this;
}

/*-------------------------------------------------------------------
//	GRPVECTOR::operator/=
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/09/2016 16:36:11
//	
//	@return 			const : 
//
//  @param				scalar : 
*/
/*-----------------------------------------------------------------*/
 GRPVECTOR		GRPVECTOR::operator/=		(GLFLOAT scalar)
{
	vector[0]/=scalar;
	vector[1]/=scalar;
	vector[2]/=scalar;
	return *this;
}

GRPVECTOR GRPVECTOR::operator + (GLFLOAT scalar)
{
		vector[0] = vector[0] + scalar;
		vector[1] = vector[1] + scalar;
		vector[2] = vector[2] + scalar;
		return *this;
}

GRPVECTOR GRPVECTOR::operator - (GLFLOAT scalar)
{
		vector[0] = vector[0] - scalar;
		vector[1] = vector[1] - scalar;
		vector[2] = vector[2] - scalar;
		return *this;
}


/*-------------------------------------------------------------------
//	GRPVECTOR::operator+
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/05/2014 12:18:14
//	
//	@return 			GRPVECTOR& : 
//
//  @param				b[3] : 
*/
/*-----------------------------------------------------------------*/
 GRPVECTOR GRPVECTOR::operator+(GLFLOAT b[3])
{
	vector[0] = vector[0] + b[0];
	vector[1] = vector[1] + b[1];
	vector[2] = vector[2] + b[2];

	return *this;
}



/*-------------------------------------------------------------------
//	GRPVECTOR::operator-
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/05/2014 12:18:20
//	
//	@return 			GRPVECTOR& : 
//
//  @param				b[3] : 
*/
/*-----------------------------------------------------------------*/
 GRPVECTOR GRPVECTOR::operator-(GLFLOAT b[3])
{
	vector[0] = vector[0] - b[0];
	vector[1] = vector[1] - b[1];
	vector[2] = vector[2] - b[2];

	return *this;
}
