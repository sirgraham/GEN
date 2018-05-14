/*------------------------------------------------------------------------------------------
//  GRPBB.H
*/
/**
// \class
//
//  Class for Axis Aligned Bounding Boxes
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 02/06/2014 10:23:33
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPBB_H_
#define _GRPBB_H_

/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>
#include "GRPPoint.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPNODE;

class GRPAABB
{
  public:

                GRPAABB                       ()                              { Clean();                }
                GRPAABB                       (const GRPAABB* rhs)
                {
                  Clean();

                  minpoint      = rhs->minpoint;
                  maxpoint      = rhs->maxpoint;

                  width         = rhs->width;
                  height        = rhs->height;
                  depth         = rhs->depth;

                  leftmargin    = rhs->leftmargin;
                  rightmargin   = rhs->rightmargin;
                  topmargin     = rhs->topmargin;
                  bottommargin  = rhs->bottommargin;

                  leftborder = rhs->leftborder;
                  rightborder = rhs->rightborder;
                  topborder = rhs->topborder;
                  bottomborder = rhs->bottomborder;

                  depthmargin   = rhs->depthmargin;
                  frontmargin   = rhs->frontmargin;

                  toppadding    = rhs->toppadding;
                  leftpadding   = rhs->leftpadding;
                  rightpadding  = rhs->rightpadding;
                  bottompadding = rhs->bottompadding;
                  frontpadding  = rhs->frontpadding;
                  depthpadding  = rhs->depthpadding;
                  kerning       = rhs->kerning;
                  blocked       = rhs->blocked;
                }

    virtual    ~GRPAABB                       ()                              { Clean();                }

    float       GetRadius                     ()                              { return radius; }

    bool        IsPointInside                 (GRPPOINT point)
                {
                  if(point.x <= maxpoint.x)
                    if(point.y <= maxpoint.y)
                      if(point.x >= minpoint.x)
                        if(point.y >= minpoint.y) return true;

                  return false;
                }

    void        Clean                         ()
                {
                  minpoint.Clean();
                  maxpoint.Clean();

                  width         = 0.0f;
                  height        = 0.0f;
                  depth         = 0.0f;

                  leftmargin    = 0.0f;
                  rightmargin   = 0.0f;
                  topmargin     = 0.0f;
                  bottommargin  = 0.0f;
                  depthmargin   = 0.0f;
                  frontmargin   = 0.0f;

                  leftborder    = 0.0f;
                  rightborder   = 0.0f;
                  topborder     = 0.0f;
                  bottomborder  = 0.0f;

                  toppadding    = 0.0f;
                  leftpadding   = 0.0f;
                  rightpadding  = 0.0f;
                  bottompadding = 0.0f;
                  frontpadding  = 0.0f;
                  depthpadding  = 0.0f;


                  bearing       = 0.0f;
                  advance       = 0.0f;
                  ascend        = 0.0f;
                  descend       = 0.0f;
                  linegap       = 0.0f;
                  lineheight    = 0.0f;
                  kerning       = 0.0f;

                  blocked       = false;
                  radius        = 0.0f;
                }

    GRPPOINT    minpoint;
    GRPPOINT    maxpoint;

    GLFLOAT     width;
    GLFLOAT     height;
    GLFLOAT     depth;

    GLFLOAT     leftmargin;
    GLFLOAT     rightmargin;
    GLFLOAT     topmargin;
    GLFLOAT     bottommargin;

    GLFLOAT     depthmargin;
    GLFLOAT     frontmargin;

    GLFLOAT     toppadding;
    GLFLOAT     leftpadding;
    GLFLOAT     rightpadding;
    GLFLOAT     bottompadding;

    GLFLOAT     leftborder;
    GLFLOAT     rightborder;
    GLFLOAT     topborder;
    GLFLOAT     bottomborder;

    GLFLOAT     frontpadding;
    GLFLOAT     depthpadding;

    GLFLOAT     bearing;
    GLFLOAT     advance;
    GLFLOAT     ascend;
    GLFLOAT     descend;
    GLFLOAT     linegap;

    GLFLOAT     lineheight;

    GLFLOAT     kerning;

    GLFLOAT     radius;

    bool        blocked;

    void        Update()
    {
      width =maxpoint.x-minpoint.x;
      height=maxpoint.y-minpoint.y;
      depth =maxpoint.z-minpoint.z;

      double maxdist = sqrt(maxpoint.x*maxpoint.x + maxpoint.y*maxpoint.y + maxpoint.z*maxpoint.z);
      double mindist = sqrt(minpoint.x*minpoint.x + minpoint.y*minpoint.y + minpoint.z*minpoint.z);
      radius = (GLFLOAT)((maxdist > mindist) ? maxdist : mindist);
    }
};


class GRPBB : public GRPAABB
{
  public:

                GRPBB                       ( float minx        =  -1.0f, float miny          =-1.0f, float maxx        = 1.0f, float maxy          = 1.0f,
                                              float margin_left =   0.0f, float margin_right  = 0.0f, float margin_top  = 0.0f, float margin_bottom = 0.0f,
                                              float padding_left=   0.0f, float padding_right = 0.0f, float padding_top = 0.0f, float padding_bottom= 0.0f)
                {
                  Clean();

                  this->minpoint.x    =minx;
                  this->minpoint.y    =miny;
                  this->maxpoint.x    =maxx;
                  this->maxpoint.y    =maxy;

                  this->leftmargin    =margin_left;
                  this->rightmargin   =margin_right;
                  this->topmargin     =margin_top;
                  this->bottommargin  =margin_bottom;

                  this->leftpadding   =padding_left;
                  this->rightpadding  =padding_right;
                  this->toppadding    =padding_top;
                  this->bottompadding =padding_bottom;
                }

                GRPBB                       (const GRPBB* rhs) : GRPAABB(rhs)
                {
                  Clean();
                }

    virtual    ~GRPBB                       ()                              { Clean();                }


    void        Clean                         ()
                {
                  node=NULL;

                  this->minpoint.x=0.0;
                  this->minpoint.y=0.0;
                  this->minpoint.z=0.0;

                  this->maxpoint.x=1.0;
                  this->maxpoint.y=1.0;
                  this->maxpoint.z=1.0;

                }

    GRPNODE*    node;

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/


#endif

