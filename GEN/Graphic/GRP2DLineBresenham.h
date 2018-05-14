/*------------------------------------------------------------------------------------------
//  GRP2DLINEBRESENHAM.H
*/
/**
// \class
//
//  Graphics 2 Line Bresenham Class
//
//  @author  Abraham J. Velez
//  @version 07/08/2013 12:32:28
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRP2DLINEBRESENHAM_H_
#define _GRP2DLINEBRESENHAM_H_



/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdlib.h> // for abs, better to check it

#include "XBase.h"
#include "GRP2DLineDDA.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


class GRP2DLINEBRESENHAMINTERPOLATOR
{
  public:

    enum
    {
      SUBPIXEL_SHIFT = 8,
      SUBPIXEL_SIZE  = 1 << SUBPIXEL_SHIFT,
      SUBPIXEL_MASK  = SUBPIXEL_SIZE - 1
    };


                                GRP2DLINEBRESENHAMINTERPOLATOR    (int x1, int y1, int x2, int y2) : x1_lr(LineLR(x1))                                      ,
                                                                                                   y1_lr(LineLR(y1))                                      ,
                                                                                                   x2_lr(LineLR(x2))                                      ,
                                                                                                   y2_lr(LineLR(y2))                                      ,
                                                                                                   ver(abs(x2_lr - x1_lr) < abs(y2_lr - y1_lr))           ,
                                                                                                   size(ver ? abs(y2_lr - y1_lr) : abs(x2_lr - x1_lr))    ,
                                                                                                   inc(ver ? ((y2 > y1) ? 1 : -1) : ((x2 > x1) ? 1 : -1)) ,
                                                                                                   interpolator(ver ? x1 : y1, ver ? x2 : y2, size)
                                {

                                }

    virtual                    ~GRP2DLINEBRESENHAMINTERPOLATOR    ()
                                {
                                  Clean();
                                }

    static int                  LineLR                          (int v)       { return (v >> SUBPIXEL_SHIFT); }

    bool                        IsVer                           () const      { return ver;   }
    XDWORD                      GetSize                         () const      { return size;  }
    int                         GetInc                          () const      { return inc;   }


     void                       HStep                           ()
                                {
                                  ++interpolator;
                                  x1_lr += inc;
                                }


     void                       VStep                           ()
                                {
                                    ++interpolator;
                                    y1_lr += inc;
                                }


     int                        X1                              () const      { return x1_lr;                     }
     int                        Y1                              () const      { return y1_lr;                     }
     int                        X2                              () const      { return LineLR(interpolator.Y());  }
     int                        Y2                              () const      { return LineLR(interpolator.Y());  }
     int                        X2HR                            () const      { return interpolator.Y();          }
     int                        Y2HR                            () const      { return interpolator.Y();          }

  private:

    void                        Clean                           ()
                                {
                                  x1_lr   = 0;
                                  y1_lr   = 0;
                                  x2_lr   = 0;
                                  y2_lr   = 0;
                                  ver     = false;
                                  size    = 0;
                                  inc     = 0;
                                }

    int                         x1_lr;
    int                         y1_lr;
    int                         x2_lr;
    int                         y2_lr;
    bool                        ver;
    XDWORD                      size;
    int                         inc;

    GRP2DLINEDDA2INTERPOLATOR   interpolator;

};





/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/



#endif

