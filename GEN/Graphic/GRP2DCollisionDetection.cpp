
/*------------------------------------------------------------------------------------------
//  GRP2DCOLLISIONDETECTION.CPP
//
//  code to detect 2D collisions
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 02/11/2015 17:35:14
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRP2DCollisionDetection.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
bool PointRectangle(float x, float y, float x0, float y0, float x1, float y1)
{
  if(x0 > x1)
    {
      float t = x0;
      x0      = x1;
      x1      = t;
    }

  if(y0 > y1)
    {
      float t = y0;
      y0      = y1;
      y1      = t;
    }

  return (x > x0) && (x < x1) && (y > y0) && (y < y1);
}
