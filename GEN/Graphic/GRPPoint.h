/*------------------------------------------------------------------------------------------
//  GRPPOINT.H
*/
/**
// \class
//
//  Graphic 3D point for Vector Graphics (Not a vector point)
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 02/06/2014 10:25:13
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPPOINT_H_
#define _GRPPOINT_H_



#if (defined(LINUX) || defined(ANDROID)) && !defined(__clang__)
#pragma GCC diagnostic error "-Wuninitialized"
//#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#endif

#pragma warning (disable : 4172)

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPOpenGL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define GRPPOINT_AXIS_X 0
#define GRPPOINT_AXIS_Y 1
#define GRPPOINT_AXIS_Z 2

/*---- CLASS -----------------------------------------------------------------------------*/



class GRPPOINT
{
  public:

                      GRPPOINT                () : x(0.0f), y(0.0f), z(0.0f), w(0.0f), index(0)                             {   }
    virtual          ~GRPPOINT                ()                              {   }

                      GRPPOINT                (GRPPOINT* p)
                      {
                        this->x     = p->x;
                        this->y     = p->y;
                        this->z     = p->z;
                        this->index = p->index;
                      }

                      GRPPOINT                (GLFLOAT xx, GLFLOAT yy = 0.0f, GLFLOAT zz = 0.0f,XDWORD ii=0) : x(xx),y(yy),z(zz),index(ii) {};
                      /*

                      GRPPOINT                (GLFLOAT x, GLFLOAT y = 0.0f, GLFLOAT z = 0.0f,XDWORD i=0)
                      {
                        this->x     = x;
                        this->y     = y;
                        this->z     = z;
                        this->index = i;
                      }
                      */

    GRPPOINT          operator *              (double scalar)
                      {
                        GRPPOINT tmp;

                        tmp.x = this->x * float(scalar);
                        tmp.y = this->y * float(scalar);
                        tmp.z = this->z * float(scalar);
                        return tmp;
                      }

    GRPPOINT          operator *              (GLFLOAT scalar)
                      {
                        GRPPOINT tmp;

                        tmp.x = this->x * scalar;
                        tmp.y = this->y * scalar;
                        tmp.z = this->z * scalar;
                        return tmp;
                      }

    GRPPOINT          operator /              (GLFLOAT scalar)
                      {
                        GRPPOINT tmp;

                        tmp.x = this->x / scalar;
                        tmp.y = this->y / scalar;
                        tmp.z = this->z / scalar;
                        return tmp;
                      }
    GRPPOINT          operator +=             (GRPPOINT point)
                      {
                        (*this).x += point.x;
                        (*this).y += point.y;
                        (*this).z += point.z;

                        return (*this);
                      }

    GRPPOINT          operator -=             (GRPPOINT point)
                      {
                        (*this).x -= point.x;
                        (*this).y -= point.y;
                        (*this).z -= point.z;

                        return (*this);
                      }

    GRPPOINT          operator +              (GRPPOINT point)
                      {
                        GRPPOINT tmp;

                        tmp.x = this->x + point.x;
                        tmp.y = this->y + point.y;
                        tmp.z = this->z + point.z;
                        return tmp;
                      }

    GRPPOINT          operator -              (GRPPOINT point)
                      {
                        GRPPOINT tmp;

                        tmp.x = this->x - point.x;
                        tmp.y = this->y - point.y;
                        tmp.z = this->z - point.z;

                        return tmp;
                      }

    static float      Lerp                    (float v0,  float v1, float t)          { return (1-t)*v0 + t*v1;                                     } // Precise method which guarantees v = v1 when t = 1.
    static float      Interpolate             (float t,   float a,  float b)          { return  Lerp(a, b, t);                                      }

    static GRPPOINT   Interpolate             (float t, GRPPOINT* a, GRPPOINT*b)
                      {
                        static GRPPOINT p;

                        p.x = Lerp(a->x, b->x,t);
                        p.y = Lerp(a->y, b->y,t);
                        p.z = Lerp(a->z, b->z,t);

                        return p;
                      }

    static const GRPPOINT Interpolate       (const GRPPOINT& a, const GRPPOINT& b,float t)
    {
      /*
      GRPPOINT  result;
                //result.x=a.x+((b.x-a.x)*t);
                //result.y=a.y+((b.y-a.y)*t);
                result.x=t*b.x + (1-t)*a.x;
                result.y=t*b.y + (1-t)*a.y;
      return    result;
      */
      return GRPPOINT(t*b.x + (1-t)*a.x,t*b.y + (1-t)*a.y);
    }

    void              Clean                   ()
                      {
                        index = 0;
                        x     = 0.0f;
                        y     = 0.0f;
                        z     = 0.0f;
                        w     = 0.0f;

                      }

    XDWORD            index;
    GLFLOAT           x;
    GLFLOAT           y;
    GLFLOAT           z;
    GLFLOAT           w;
};




/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

