/*------------------------------------------------------------------------------------------
//  GRPSHAPE.H
*/
/**
// \class
//
//  Classes for Vector Graphics
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 02/06/2014 10:21:57
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPSHAPE_H_
#define _GRPSHAPE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPSpline.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


class GRPSHAPE
{
  public:
                             ~GRPSHAPE                    ()
                              {
                                contours.DeleteContents   ();
                                contours.DeleteAll();
                              }
    bool                      Add                         (GRPANCHORPOINTTYPE type, GRPPOINT point)
                              {
                                if (contours.GetSize()==0) //no hay un contorno base, creamos uno vacio
                                {
                                  GRPSPLINE* newSpline=new GRPSPLINE();
                                  if (newSpline)
                                    this->AddContour(newSpline);
                                  else
                                    return false;
                                }

                                return this->contours.GetLast()->Add(type,point);
                              }

    bool                      AddContour                  (GRPSPLINE* spline)
                              {
                                contours.Add(spline);
                                return true;
                              }

    GRPSHAPE&                 operator+                   (GRPSPLINE other)
                              {
                                AddContour(&other);
                                return (*this);
                              }

    GRPSHAPE*                 CreateCopy  ()
    {
      GRPSHAPE* shape=new GRPSHAPE();

      if (shape)
      for (int c=0;c<(int)this->contours.GetSize();c++)
      {
        GRPSPLINE* spline=new GRPSPLINE();
        if (spline)
        {
          shape->AddContour(spline);
          for (int e=0;e<(int)this->contours.Get(c)->points.GetSize();e++)
            spline->Add(this->contours.Get(c)->points.Get(e)->type,this->contours.Get(c)->points.Get(e)->value);
        }
      }
      return shape;
    }

    bool              Setup                               ()
    {
      if (!this->contours.IsEmpty())
      if (this->contours.Get(0)->IsReversed())
        this->Reverse();

      return true;
    }

    void              Reverse()
    {
        for (XDWORD e=0;e<this->contours.GetSize();e++)
          this->contours.FastGet(e)->Reverse();
    }
    void              Print()
    {
        for (XDWORD e=0;e<this->contours.GetSize();e++)
        {
          XDEBUG_PRINTCOLOR(0,__L("Contour: %d"),e);
          this->contours.Get(e)->Print();
        }

    }

    void                      Clear()
    {
      contours.DeleteContents   ();
      contours.DeleteAll();
    }

    GRPBB*                  GetBoundingBox              ()            { return &boundingbox;                      }

    XVECTOR<GRPSPLINE*>       contours;
    GRPBB                   boundingbox;
};

#endif