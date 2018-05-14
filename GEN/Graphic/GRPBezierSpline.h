/*------------------------------------------------------------------------------------------
//  GRPBEZIERSPLINE.H
*/
/**
// \class
//
//  Holds bezier spline data
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 16/02/2015 17:19:00
//  Last Modification :
*/
//* GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPBEZIERSPLINE_H_
#define _GRPBEZIERSPLINE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPSpline.h"
#include "GRPProfiler.h"
#include <math.h>

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


class GRPBEZIERANCHORS
{
public:

  GRPBEZIERANCHORS(GRPPOINT StA,GRPPOINT AA,GRPPOINT AB,GRPPOINT Eb) : StartA(StA),AnchorA(AA),AnchorB(AB),EndB(Eb)
  {

  };

  GRPPOINT StartA;
  GRPPOINT AnchorA;
  GRPPOINT AnchorB;
  GRPPOINT EndB;
};



class GRPBEZIERSPLINE
{
  public:

                                    GRPBEZIERSPLINE             ()                            {   }
    virtual                        ~GRPBEZIERSPLINE             ()
                                    {
                                      for(XDWORD e=0; e<points.GetSize(); e++)
                                        {
                                          points.FastGet(e)->DeleteContents();
                                          points.FastGet(e)->DeleteAll();
                                        }

                                      points.DeleteContents();
                                      points.DeleteAll();
                                    }

    void                            Debug()
    {
      XSTRING s;
      XSTRING v;
      for  (float e=0;e<1.0f;e+=0.01f)
      {
        float value=GetStep(e);
        v.ConvertFromFloat(value);
        s.Add(v);
        s.Add(__L(","));
      }
      XDEBUG_PRINTCOLOR(0,s.Get());
    }

    bool                            GetAnchor                   (XDWORD index, GRPPOINT* StartA,GRPPOINT* AnchorA, GRPPOINT* AnchorB, GRPPOINT* StartB);

    GRPSPLINE*                      ResolveCubicBezier          (GRPSPLINE* inputsp, int nsegments);
    virtual GRPPOINT                InterpolateBezier           (XVECTOR<GRPPOINT*>* points, float t);
    inline  GRPPOINT                InterpolateBezier           (float t)
    {
      //return InterpolateBezier(points.FastGet(0),t);
      if (first)
      return InterpolateBezier(first,t);

      //return GRPPOINT();
      return InterpolateBezier(t);
    }

      float InterpolateBezierAmplitude(float t)
      {
        if (!first) return 0.0f;

        float a=0.0f;
        int  len = (int)this->first->GetSize();

        for(int i=0; i<len; i++)
          {
            GRPPOINT* pointtempo  = this->first->FastGet(i);
            if (pointtempo==NULL)
            {
              XDEBUG_PRINTCOLOR(0,__L("float InterpolateBezierAmplitude(float) : pointtempo is null"));
              continue;
            }

            float  mult = 1.0;
            if ((t==0.0f && i<=0) || (t==1 && (len - 1 - i)<=0))     //  excepcion de dominio : Domain error occurs if base is 0 and exp is less than or equal to 0. Nan is returned
            {
                   mult=0.0;
                   GRPPROFILER::DomainChecks++;
            }
            else
            {
                    //mult = CombinatorialPermutation((float)(len - 1), (float)i);
                    mult  = CombinatorialPermutation((float)(len - 1), (float)i) * (float)pow(1 - t, int(len - 1 - i)) * (float)pow(t, int(i));
                    /*
                   if ((1-t)!=0 || (t==0 && int(len - 1 - i)>0))
                   {
                    mult = mult * pow(1 - t, int(len - 1 - i));
                    GRPPROFILER::DomainChecks++;
                   }

                   if (t!=0 || (t==0 && i>0))
                   {
                    mult = mult * pow(t, int(i));
                    GRPPROFILER::DomainChecks++;
                   }*/
            }

            a += pointtempo->y * mult;
          }

        return a;
      }


    float                           CombinatorialPermutation    (float a, float b);
    float                           Factorial                   (float num);

    virtual void                    Init                        (GRPSPLINE* inputsp);
    virtual float                   GetStep                     (float t);

    XVECTOR<XVECTOR<GRPPOINT*>*>*   GetPoints                   ()                    { return &points;}
    XVECTOR<GRPBEZIERANCHORS*>*     GetAnchors                  ()                    { return &Anchors;}

protected:

    XVECTOR<XVECTOR<GRPPOINT*>*>    points;
    XVECTOR<GRPBEZIERANCHORS*>      Anchors;

    XVECTOR<GRPPOINT*>*             first;

    void Clean()
    {
      first=NULL;
    }



};



class GRPBEZIERCURVE : public GRPBEZIERSPLINE
{
public:

                                    GRPBEZIERCURVE              (){   }
    virtual                        ~GRPBEZIERCURVE              ()
                                    {
                                      for(XDWORD e=0; e<points.GetSize(); e++)
                                        {
                                          points.Get(e)->DeleteContents();
                                          points.Get(e)->DeleteAll();
                                        }

                                      points.DeleteContents();
                                      points.DeleteAll();
                                    }

    GRPPOINT                        InterpolateBezier           (XVECTOR<GRPPOINT*>* points, float t);
    GRPPOINT                        InterpolateBezier           (float t);
    void                            Init                        (GRPSPLINE* inputsp);
    void                            Init                        (float p0x, float p0y, float p1x, float p1y, float p2x, float p2y,  float p3x, float p3y);

private:

    GRPPOINT Sa,ab,bE;
    GRPPOINT p0,p1,p2,p3;
    GRPPOINT Sab,abE;
    GRPPOINT Result;

};

  /*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif



/****************************************
 * Spline Class
 * By Will Perone
 * Original: 21-10-2003
 * Revised:  05-11-2003
 *
 * Dependancies: matrix4, vector2 class, STL vector, STL list
 ***************************************

#pragma once

#include <vector>
#include <list>
#include "vector2.h"
#include "matrix4.h"

#ifdef WIN32
#pragma warning(disable:4786) // STL expands to beyond 256 chars in windows
#endif


#define ARCLEN_GENERATION_ERROR  0.001f  //!< error for adaptive arclen table generation


template <class Vector>
class Spline
{
public:
    struct ArclenTableEntry
    {
        float    time;   //!< the timestamp
        float    arclen; //!< the arclength at this time (monotonically growing in the table)
        Vector   pos;    //!< the space curve position at this point
    };

    enum SplineMode { Bezier, Hermite, CatMullRom, BSpline }; //!< default mode is bezier

protected:
    SplineMode mode;   //!< the current spline mode
    matrix4    basis;  //!< the current basis matrix

    unsigned int step; //!< the stepping value for control point interpolation

    //! returns nearest index into the arclength table matching arc_val
    inline int ArcTableBisectionSearch(float arc_val) const;

public:
    std::vector<Vector>  ctrlpts; //!< the control points

    typedef std::vector<ArclenTableEntry> ArclenTable;
    ArclenTable arclentable; //!< the arclength table


    void Mode(SplineMode m); //!< set the spline mode

    SplineMode Mode() const  //!< get the current spline mode
    {  return mode;  }

    unsigned int Step() const
    {  return step;  } //!< returns the stepping value

    Spline()
    {  Mode(Bezier);  }

    ~Spline()
    { }

    //! returns the interpolated value of the spline at a time value in a particular dimension
    inline float InterpCtrlPt(float p1, float p2, float p3, float p4, float t);

    //! normalizes the arclength table so that the final arclength = 1
    void  ArclenTableNormalize(void)
    {
        for (unsigned int i= 0; i < arclentable.size(); ++i)
             arclentable[i].arclen/=arclentable.back().arclen;
    }

    //! returns the time on the space curve given a partial arclength value
    float ArcTime(float arc_val) const;

    //! builds the arclength table
    virtual void  ArclenTableBuild()= 0;

    //! returns approximated space curve point given a partial arclength value
    virtual Vector ArcPoint(float arc_val)= 0;
};


//! 1D spline class
class Spline1D: public Spline<float>
{
private:
    std::list<ArclenTableEntry>::iterator InternalBuildArclenTable(std::list<ArclenTableEntry> &templist, std::list<ArclenTableEntry>::iterator curr, unsigned int ctrlpt_offset);

public:
    void  ArclenTableBuild();
    float ArcPoint(float arc_val);
};


//! 2D spline class
class Spline2D: public Spline<vector2f>
{
private:
    std::list<ArclenTableEntry>::iterator InternalBuildArclenTable(std::list<ArclenTableEntry> &templist, std::list<ArclenTableEntry>::iterator curr, unsigned int ctrlpt_offset);

public:
    void  ArclenTableBuild();
    vector2f ArcPoint(float arc_val);
};


// basis matrices

// Bezier Spline
// B= [P(i) P(i+1) P(i+2) P(i+3)
// C0 continuous, if tangents are aligned then C1 continuous
// step = 3
static matrix4 MatBezier(-1,  3, -3, 1,
                          3, -6,  3, 0,
                         -3,  3,  0, 0,
                          1,  0,  0, 0);

// Hermite Spline
// B= [P(i) P(i+1) P'(i) P'(i+1)]
// C0 continuous
// step = 1
static matrix4 MatHermite  ( 2, -2,  1,  1,
                            -3,  3, -2, -1,
                             0,  0,  1,  0,
                             1,  0,  0,  0);

// CatMullRom Spline
// B= [P(i-1) P(i) P(i+1) P(i+2)]
// C0 continuous
// curve guaranteed to go through all control points
// each 4 consecutive control points define a curve (ex: 0123, 1234, 2345...)
// step = 1
static matrix4 MatCatMullRom(-.5f,  1.5f, -1.5f,  .5f,
                                1, -2.5f,     2, -.5f,
                             -.5f,     0,   .5f,    0,
                                0,     1,     0,    0);

// BSpline
// B= [P(i) P(i+1) P(i+2) P(i+3)
// C2 continuous
// each 4 consecutive control points define a curve (ex: 0123, 1234, 2345...)
// step = 1
static matrix4 MatBSpline(-1.f/6,  3.f/6, -3.f/6, 1.f/6,
                           3.f/6, -6.f/6,  3.f/6, 0,
                          -3.f/6,      0,  3.f/6, 0,
                           1.f/6,  4.f/6,  1.f/6, 0);



// returns the interpolated value of a control point in a particular dimension
template <class Vector>
inline float Spline<Vector>::InterpCtrlPt(float p1, float p2, float p3, float p4, float t)
{
    static float t2, t3;

    t2= t*t; // precalculations
    t3= t2*t;

    return (basis[0][0]*t3 + basis[1][0]*t2 + basis[2][0]*t + basis[3][0])*p1 +
            (basis[0][1]*t3 + basis[1][1]*t2 + basis[2][1]*t + basis[3][1])*p2 +
            (basis[0][2]*t3 + basis[1][2]*t2 + basis[2][2]*t + basis[3][2])*p3 +
            (basis[0][3]*t3 + basis[1][3]*t2 + basis[2][3]*t + basis[3][3])*p4;
   // optimally you want to do something like the following but the ++'s in C are ambiguous in the operation order and won't generate the correct ASM
   //  You'd need to write the corresponding assembly code for the following to work.
   // ptr= (float *)&basis;
  //  --ptr;
   // return t3*((p1 * *++ptr) + (p2 * *++ptr) + (p3 * *++ptr) + (p4 * *++ptr)) +
 //           t2*((p1 * *++ptr) + (p2 * *++ptr) + (p3 * *++ptr) + (p4 * *++ptr)) +
 //           t* ((p1 * *++ptr) + (p2 * *++ptr) + (p3 * *++ptr) + (p4 * *++ptr)) +
  //             ((p1 * *++ptr) + (p2 * *++ptr) + (p3 * *++ptr) + (p4 * *++ptr));
   //

}


template <class Vector>
inline int Spline<Vector>::ArcTableBisectionSearch(float arc_val) const
{
    unsigned int upper, middle, lower;
    float ascnd;

    lower= 0;
    upper= arclentable.size();
    ascnd= (arclentable.back().arclen >= arclentable.front().arclen);
    while (upper-lower > 1)
    {
        middle= (upper+lower) >> 1;
        if (arc_val >= arclentable[middle].arclen == ascnd) lower= middle;
                                                        else upper= middle;
    }
    return lower;
}


template <class Vector>
inline void Spline<Vector>::Mode(SplineMode m)
{
    mode= m;
    switch(mode)
    {
    case Bezier:     step= 3; basis= MatBezier; break;
    case Hermite:    step= 1; basis= MatHermite; break;
    case CatMullRom: step= 1; basis= MatCatMullRom; break;
    case BSpline:    step= 1; basis= MatBSpline; break;
    }
}


template <class Vector>
float Spline<Vector>::ArcTime(float arc_val) const
{
    // trivial cases
    if (arc_val==arclentable.front().arclen) return arclentable.front().time;
    else
    if (arc_val==arclentable.back().arclen) return arclentable.back().time;

    unsigned int lower= ArcTableBisectionSearch(arc_val);

    // perform a linear interpolation of 2 nearest time values
    return ( (arclentable[lower+1].arclen-arc_val)*arclentable[lower].time +
              (arc_val-arclentable[lower].arclen)*arclentable[lower+1].time ) /
              (arclentable[lower+1].time - arclentable[lower].time);
}


std::list<Spline1D::ArclenTableEntry>::iterator Spline1D::InternalBuildArclenTable(std::list<Spline1D::ArclenTableEntry> &templist,
                                                                                   std::list<Spline1D::ArclenTableEntry>::iterator curr, unsigned int ctrlpt_offset)
{
    std::list<ArclenTableEntry>::iterator next= curr;
    ArclenTableEntry  middle;


    const unsigned int size= ctrlpts.size();
    ++next;
    middle.time= (curr->time + (next)->time)/2;
    middle.pos= InterpCtrlPt(ctrlpts[ctrlpt_offset], ctrlpts[(ctrlpt_offset+1)%size],
                             ctrlpts[(ctrlpt_offset+2)%size], ctrlpts[(ctrlpt_offset+3)%size],
                             middle.time - (float)ctrlpt_offset/step);

    float a, b, c;
    a= middle.pos-curr->pos;
    b= next->pos-middle.pos;
    c= next->pos-curr->pos;

    float error= a+b-c;
    if (error > ARCLEN_GENERATION_ERROR)
    {
        // recalculate the arclengths for the rest of the table
        std::list<ArclenTableEntry>::iterator i= next;
        while (i != templist.end())
        {
            i->arclen+=error;
            ++i;
        }

        // insert the mid point into the table
        middle.arclen= curr->arclen+a;
        templist.insert(next, middle);

        // recursive subdivision
        next= InternalBuildArclenTable(templist, curr, ctrlpt_offset);
        next= InternalBuildArclenTable(templist, next, ctrlpt_offset);
    }
    return next;
}


void Spline1D::ArclenTableBuild()
{
    std::list<ArclenTableEntry> templist;
    std::list<ArclenTableEntry>::iterator ai;

    ArclenTableEntry   entry;
    unsigned int size= ctrlpts.size();


    arclentable.clear();

    // add the first entry to the arc table
    entry.time= 0;
    entry.arclen= 0;
    entry.pos= InterpCtrlPt(ctrlpts[0], ctrlpts[1], ctrlpts[2%size], ctrlpts[3%size], 0);
    templist.push_back(entry);

    // go through all the control point sets generating arclen table entries
    ai= templist.begin();

    for (unsigned int i= 0; i < ctrlpts.size()-(step-1); i+=step)
    {
        // add the end entry to the list
        entry.time= (float)(i+step)/step;
        entry.pos= InterpCtrlPt(ctrlpts[i%size], ctrlpts[(i+1)%size], ctrlpts[(i+2)%size], ctrlpts[(i+3)%size], 1);
        entry.arclen= entry.pos-ai->pos + templist.back().arclen; // beginning approximation
        templist.push_back(entry);

        // do the subdivision
        ai= InternalBuildArclenTable(templist, ai, i);
    }

    ai= templist.begin();
    while (ai!=templist.end())
    {
        ai->time/=templist.back().time;
        arclentable.push_back(*ai);
        ++ai;
    }
}


float Spline1D::ArcPoint(float arc_val)
{
    // trivial cases
    if (arc_val==arclentable.front().arclen) return arclentable.front().pos;
    else
    if (arc_val==arclentable.back().arclen) return arclentable.back().pos;

    unsigned int lower= ArcTableBisectionSearch(arc_val);

    // perform a linear interpolation of 2 nearest position values
    float pos;
    pos= ( (arclentable[lower+1].arclen-arc_val)*arclentable[lower].pos +
           (arc_val-arclentable[lower].arclen)*arclentable[lower+1].pos ) /
           (arclentable[lower+1].arclen - arclentable[lower].arclen);
    return pos;
}


std::list<Spline2D::ArclenTableEntry>::iterator Spline2D::InternalBuildArclenTable(std::list<Spline2D::ArclenTableEntry> &templist,
                                                                                   std::list<Spline2D::ArclenTableEntry>::iterator curr, unsigned int ctrlpt_offset)
{
    std::list<ArclenTableEntry>::iterator next= curr;
    ArclenTableEntry  middle;


    const unsigned int size= ctrlpts.size();
    ++next;
    middle.time= (curr->time + (next)->time)/2;
    middle.pos.x= InterpCtrlPt(ctrlpts[ctrlpt_offset].x, ctrlpts[(ctrlpt_offset+1)%size].x,
                               ctrlpts[(ctrlpt_offset+2)%size].x, ctrlpts[(ctrlpt_offset+3)%size].x,
                               middle.time - (float)ctrlpt_offset/step);
    middle.pos.y= InterpCtrlPt(ctrlpts[ctrlpt_offset].y, ctrlpts[(ctrlpt_offset+1)%size].y,
                               ctrlpts[(ctrlpt_offset+2)%size].y, ctrlpts[(ctrlpt_offset+3)%size].y,
                               middle.time - (float)ctrlpt_offset/step);

    float a, b, c;
    a= vector2f(middle.pos-curr->pos).length();
    b= vector2f(next->pos-middle.pos).length();
    c= vector2f(next->pos-curr->pos).length();

    float error= a+b-c;
    if (error > ARCLEN_GENERATION_ERROR)
    {
        // recalculate the arclengths for the rest of the table
        std::list<ArclenTableEntry>::iterator i= next;
        while (i != templist.end())
        {
            i->arclen+=error;
            ++i;
        }

        // insert the mid point into the table
        middle.arclen= curr->arclen+a;
        templist.insert(next, middle);

        // recursive subdivision
        next= InternalBuildArclenTable(templist, curr, ctrlpt_offset);
        next= InternalBuildArclenTable(templist, next, ctrlpt_offset);
    }
    return next;
}


void Spline2D::ArclenTableBuild()
{
    std::list<ArclenTableEntry> templist;
    std::list<ArclenTableEntry>::iterator ai;

    ArclenTableEntry   entry;
    unsigned int size= ctrlpts.size();


    arclentable.clear();

    // add the first entry to the arc table
    entry.time= 0;
    entry.arclen= 0;
    entry.pos.x= InterpCtrlPt(ctrlpts[0].x, ctrlpts[1].x, ctrlpts[2%size].x, ctrlpts[3%size].x, 0);
    entry.pos.y= InterpCtrlPt(ctrlpts[0].y, ctrlpts[1].y, ctrlpts[2%size].y, ctrlpts[3%size].y, 0);
    templist.push_back(entry);

    // go through all the control point sets generating arclen table entries
    ai= templist.begin();

    for (unsigned int i= 0; i < ctrlpts.size()-(step-1); i+=step)
    {
        // add the end entry to the list
        entry.time= (float)(i+step)/step;
        entry.pos.x= InterpCtrlPt(ctrlpts[i%size].x, ctrlpts[(i+1)%size].x, ctrlpts[(i+2)%size].x, ctrlpts[(i+3)%size].x, 1);
        entry.pos.y= InterpCtrlPt(ctrlpts[i%size].y, ctrlpts[(i+1)%size].y, ctrlpts[(i+2)%size].y, ctrlpts[(i+3)%size].y, 1);
        entry.arclen= vector2f(entry.pos-ai->pos).length() + templist.back().arclen; // beginning approximation
        templist.push_back(entry);

        // do the subdivision
        ai= InternalBuildArclenTable(templist, ai, i);
    }

    ai= templist.begin();
    while (ai!=templist.end())
    {
        ai->time/=templist.back().time;
        arclentable.push_back(*ai);
        ++ai;
    }
}


vector2f Spline2D::ArcPoint(float arc_val)
{
    // trivial cases
    if (arc_val==arclentable.front().arclen) return arclentable.front().pos;
    else
    if (arc_val==arclentable.back().arclen) return arclentable.back().pos;

    unsigned int lower= ArcTableBisectionSearch(arc_val);

    // perform a linear interpolation of 2 nearest position values
    vector2f pos;
    pos.x= ( (arclentable[lower+1].arclen-arc_val)*arclentable[lower].pos.x +
             (arc_val-arclentable[lower].arclen)*arclentable[lower+1].pos.x ) /
             (arclentable[lower+1].arclen - arclentable[lower].arclen);
    pos.y= ( (arclentable[lower+1].arclen-arc_val)*arclentable[lower].pos.y +
             (arc_val-arclentable[lower].arclen)*arclentable[lower+1].pos.y ) /
             (arclentable[lower+1].arclen - arclentable[lower].arclen);
    return pos;
}


Here are some usage examples
Spline2D spline;
spline.Mode(Spline2D::CatMullRom);  // set it to Cat Mull Rom mode (spline will actually go through the control points)
spline.ctrlpts.push_back(vector2f(x1, y1));  // add 3 new control points to the spline (you need at least 3 to render a spline)
spline.ctrlpts.push_back(vector2f(x2, y2));
spline.ctrlpts.push_back(vector2f(x3, y3));
spline.ctrlpts.erase(spline.ctrlpts.begin() + ctrlpt_to_erase); // erase a point from the spline
spline.ArclenTableBuild();     // build the arc length table
spline.ArclenTableNormalize(); // normalize the arc length table to be 0 - 1


// render the spline
if (spline.ctrlpts.size() >= 3) // only draw the spline if it has enough control points
{
    unsigned int i;
    float t;
    float x, y;

    // draw curves
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    unsigned int max= spline.ctrlpts.size()-(spline.Step()-1);
    unsigned int size= spline.ctrlpts.size();
    for (i= 0; i < max; i+= spline.Step())
    for (t= 0; t < 1; t+=0.01f)
    {
        x= spline.InterpCtrlPt(spline.ctrlpts[i%size].x, spline.ctrlpts[(i+1)%size].x, spline.ctrlpts[(i+2)%size].x, spline.ctrlpts[(i+3)%size].x, t);
        y= spline.InterpCtrlPt(spline.ctrlpts[i%size].y, spline.ctrlpts[(i+1)%size].y, spline.ctrlpts[(i+2)%size].y, spline.ctrlpts[(i+3)%size].y, t);
        glVertex2f(x, y);
        x= spline.InterpCtrlPt(spline.ctrlpts[i%size].x, spline.ctrlpts[(i+1)%size].x, spline.ctrlpts[(i+2)%size].x, spline.ctrlpts[(i+3)%size].x, t+0.01);
        y= spline.InterpCtrlPt(spline.ctrlpts[i%size].y, spline.ctrlpts[(i+1)%size].y, spline.ctrlpts[(i+2)%size].y, spline.ctrlpts[(i+3)%size].y, t+0.01);
        glVertex2f(x, y);
    }
    glEnd();
}



// draw generated (approximated) path from the arc table
if (spline.arclentable.size())
{
    glBegin(GL_POINTS);
    Spline2D::ArclenTable::iterator ai= spline.arclentable.begin();
    while (ai!=spline.arclentable.end())
    {
        glColor3f(0, ai->time,1); // use the color to see what direction it's going in
        glVertex2fv((GLfloat *)&ai->pos);
        ++ai;
    }
    glEnd();
}

spline.ctrlpts.clear();        // clear out all the control points

*/