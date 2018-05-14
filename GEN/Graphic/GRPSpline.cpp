
/*------------------------------------------------------------------------------------------
//  GRPSPLINE.CPP
//
//  GRP Spline class
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 23/06/2014 13:47:27
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"
#include "XDebug.h"

#include "GRPSpline.h"
#include "GRPVector.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  GRPSPLINE::GRPSPLINE
*/
/**
//
//  Class Constructor GRPSPLINE
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/06/2014 13:53:04
//
*/
/*-----------------------------------------------------------------*/
GRPSPLINE::GRPSPLINE()
{
  nsegments = 1;
  origin    = NULL;
}



/*-------------------------------------------------------------------
//  GRPSPLINE::~GRPSPLINE
*/
/**
//
//   Class Destructor GRPSPLINE
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/06/2014 13:53:39
//
*/
/*-----------------------------------------------------------------*/
GRPSPLINE::~GRPSPLINE()
{
  points.DeleteContents();
  points.DeleteAll();
}

// Define Infinite (Using INT__MAX caused overflow problems)
#define INF 10000


// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
bool GRPSPLINE::onSegment(GRPPOINT* p, GRPPOINT* q, GRPPOINT* r)
{
    if (q->x <= __MAX(p->x, r->x) && q->x >= __MIN(p->x, r->x) &&
            q->y <= __MAX(p->y, r->y) && q->y >= __MIN(p->y, r->y))
        return true;
    return false;
}
bool GRPSPLINE::onSegment(GRPANCHORPOINT* p, GRPANCHORPOINT* q, GRPANCHORPOINT* r)
{
    if (q->value.x <= __MAX(p->value.x, r->value.x) && q->value.x >= __MIN(p->value.x, r->value.x) &&
            q->value.y <= __MAX(p->value.y, r->value.y) && q->value.y >= __MIN(p->value.y, r->value.y))
        return true;
    return false;
}
// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int GRPSPLINE::orientation(GRPPOINT* p, GRPPOINT* q, GRPPOINT* r)
{
    GLFLOAT val = (q->y - p->y) * (r->x - q->x) -
              (q->x - p->x) * (r->y - q->y);

    if (val == 0)
      return 0;  // colinear
    return (val > EPSILON)? 1: 2; // clock or counterclock wise
}
int GRPSPLINE::orientation(GRPANCHORPOINT* p, GRPANCHORPOINT* q, GRPANCHORPOINT* r)
{
    GLFLOAT val = (q->value.y - p->value.y) * (r->value.x - q->value.x) -
              (q->value.x - p->value.x) * (r->value.y - q->value.y);

    if (val == 0)
      return 0;  // colinear
    return (val > EPSILON)? 1: 2; // clock or counterclock wise
}
// The function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
bool GRPSPLINE::doIntersect(GRPPOINT* p1, GRPPOINT* q1, GRPPOINT* p2, GRPPOINT* q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

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
bool GRPSPLINE::doIntersect(GRPANCHORPOINT* p1, GRPANCHORPOINT* q1, GRPANCHORPOINT* p2, GRPANCHORPOINT* q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

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

// Returns true if the point p lies inside the polygon[] with n vertices
bool GRPSPLINE::isInside(XVECTOR<GRPPOINT*>* polygon, GRPPOINT* p)
{
  XDWORD n=polygon->GetSize();

    // There must be at least 3 vertices in polygon[]
    if (n < 3)  return false;

    // Create a point for line segment from p to infinite
    GRPPOINT extreme(INF, p->y);

    // Count intersections of the above line with sides of polygon
    int count = 0, i = 0;
    do
    {
        int next = (i+1)%n;

        // Check if the line segment from 'p' to 'extreme' intersects
        // with the line segment from 'polygon[i]' to 'polygon[next]'
        if (doIntersect(polygon->Get(i), polygon->Get(next), p, &extreme))
        {
            // If the point 'p' is colinear with line segment 'i-next',
            // then check if it lies on segment. If it lies, return true,
            // otherwise false
            if (orientation(polygon->Get(i), p, polygon->Get(next)) == 0)
            {
               if (onSegment(polygon->Get(i), p, polygon->Get(next)))
                 return true;
               else
                 count--;
            }
            count++;
        }
        i = next;
    } while (i != 0);

    // Return true if count is odd, false otherwise
    return count&1;  // Same as (count%2 == 1)
}

// Returns true if the point p lies inside the polygon[] with n vertices
bool GRPSPLINE::isInside(GRPANCHORPOINT* p)
{
  return isInside(this,p);
}

bool GRPSPLINE::isInside(GRPSPLINE* polygon,  GRPANCHORPOINT* p)
{
XDWORD i, j, c = 0;
XDWORD n=polygon->points.GetSize();
float testx=p->value.x;
float testy=p->value.y;

  for (i = 0, j = n-1; i < n; j = i++)
  {
    float x0=polygon->points.Get(i)->value.x;
    float y0=polygon->points.Get(i)->value.y;

    float x1=polygon->points.Get(j)->value.x;
    float y1=polygon->points.Get(j)->value.y;

    if (y1==y0)
      continue;

    if ( ((y0>testy) != (y1>testy)) &&
          (testx < ((x1-x0) * ((testy-y0)/(y1-y0)) + x0)) )
       c = !c;
  }

  return (c==1);
}
/*-------------------------------------------------------------------
//  GRPSPLINE::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/06/2014 13:54:35
//
//  @return       bool :
//
//  @param        spline :
*/
/*-----------------------------------------------------------------*/
bool GRPSPLINE::Add (GRPSPLINE* spline)
{
  for(XDWORD e=0;e<spline->points.GetSize();e++)
    {
      GRPANCHORPOINT* anchor      = spline->points.Get(e);
      GRPANCHORPOINT* anchorcopy  = new GRPANCHORPOINT(anchor->type, anchor->value);
      if(anchorcopy == NULL) return false;

      points.Add(anchorcopy);
    }

  return true;
}



/*-------------------------------------------------------------------
//  GRPSPLINE::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/06/2014 13:55:26
//
//  @return       bool :
//
//  @param        type :
//  @param        point :
*/
/*-----------------------------------------------------------------*/
bool GRPSPLINE::Add(GRPANCHORPOINTTYPE type, GRPPOINT point)
{
  GRPANCHORPOINT* anchor = new GRPANCHORPOINT(type, point);
  if(anchor == NULL) return false;

  points.Add(anchor);

  return true;
}

/*-------------------------------------------------------------------
//  GRPSPLINE::IsReversed
*/
/**
//
//  Detects if a spline is counterclockwise
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/09/2014 12:46:34
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  GRPSPLINE::IsReversed ()
{
  float area=0.0f;

  for (XDWORD i=0;i<this->points.GetSize();i++)
  {
    GRPANCHORPOINT* a=this->points.Get(i);
    GRPANCHORPOINT* b=this->points.Get((i+1)%this->points.GetSize());
    area+=(a->value.x*b->value.y);
  }
    for (XDWORD i=0;i<this->points.GetSize();i++)
  {
    GRPANCHORPOINT* a=this->points.Get(i);
    GRPANCHORPOINT* b=this->points.Get((i+1)%this->points.GetSize());
    area-=(a->value.y*b->value.x);
  }
return area>EPSILON;
}

/*-------------------------------------------------------------------
//  GRPSPLINE::Reverse
*/
/**
//
//  Reverses spline in vertex order (CCW or CW)*
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/09/2014 13:18:39
//
//  @return       bool :
//
// *Note: this is NOT a mirroring, only vertex order is changed
*/
/*-----------------------------------------------------------------*/

bool  GRPSPLINE::Reverse                      ()
{
  XVECTOR<GRPANCHORPOINT*> reversedpoints;
  XDWORD np=this->points.GetSize();
  for (XDWORD i=0;i<np;i++)
  {
    reversedpoints.Insert(0,this->points.FastGet(i));
  }
  this->points.DeleteAll();
  for (XDWORD i=0;i<np;i++)
  {
    this->points.Add(reversedpoints.FastGet(i));
  }
  return true;
}
/*-------------------------------------------------------------------
//  GRPSPLINE::operator+
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/06/2014 13:55:52
//
//  @return       GRPSPLINE& :
//
//  @param        other :
*/
/*-----------------------------------------------------------------*/
GRPSPLINE& GRPSPLINE::operator+(GRPSPLINE other)
{
  Add(&other);
  return (*this);
}




/*-------------------------------------------------------------------
//  GRPSPLINE::CalculateBoundingBox
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/06/2014 14:00:58
//
//  @return       GRPBB* :
//
*/
/*-----------------------------------------------------------------*/
GRPBB* GRPSPLINE::CalculateBoundingBox()
{
  boundingbox.minpoint.x = points.FastGet(0)->value.x;
  boundingbox.minpoint.y = points.FastGet(0)->value.y;
  boundingbox.maxpoint.x = points.FastGet(0)->value.x;
  boundingbox.maxpoint.y = points.FastGet(0)->value.y;
  boundingbox.maxpoint.z = points.FastGet(0)->value.z;
  boundingbox.maxpoint.z = points.FastGet(0)->value.z;

  XDWORD np=points.GetSize();
  for(XDWORD i=0; i<np; i++)
    {
      GRPPOINT point = points.FastGet(i)->value;

      if(point.x < boundingbox.minpoint.x)  boundingbox.minpoint.x = point.x;
      if(point.y < boundingbox.minpoint.y)  boundingbox.minpoint.y  = point.y;
      if(point.x > boundingbox.maxpoint.x)  boundingbox.maxpoint.x = point.x;
      if(point.y > boundingbox.maxpoint.y)  boundingbox.maxpoint.y  = point.y;

      if(point.z > boundingbox.maxpoint.z)  boundingbox.maxpoint.z = point.z;
      if(point.z < boundingbox.minpoint.y)  boundingbox.minpoint.z  = point.z;

    }

  return &boundingbox;
}



/*-------------------------------------------------------------------
//  GRPSPLINE::CalculateSegments
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2014 12:37:02
//
*/
/*-----------------------------------------------------------------*/
void    GRPSPLINE::CalculateSegments  ()
    {
      float total=0.0f;
      this->percents.DeleteAll();

      percents.Add(0.0f);

      for (XDWORD e=1;e<points.GetSize();e++)
      {
        GRPVECTOR v(points.FastGet(e)->value.x-points.FastGet(e-1)->value.x,
                    points.FastGet(e)->value.y-points.FastGet(e-1)->value.y,
                    points.FastGet(e)->value.z-points.FastGet(e-1)->value.z);

        float l=v.GetLengthf();
        total+=l;
        percents.Add(l);
      }

      for (XDWORD e=0;e<points.GetSize();e++)
      {
        percents.Set(e,percents.FastGet(e)/total);
      }
    }