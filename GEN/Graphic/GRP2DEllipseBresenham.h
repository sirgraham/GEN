/*------------------------------------------------------------------------------------------
//  GRP2DELLIPSEBRESENHAM.H
*/
/**
// \class
//
//  Graphics Ellipse Bresenham Class
//
//  @author  Abraham J. Velez
//  @version 07/08/2013 16:54:50
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRP2DELLIPSEBRESENHAM_H_
#define _GRP2DELLIPSEBRESENHAM_H_


/*---- INCLUDES --------------------------------------------------------------------------*/


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


class GRP2DELLIPSEBRESENHAM
{
  public:

                            GRP2DELLIPSEBRESENHAM           (int rx, int ry)
                            {
                              Clean();

                              rx2     = (rx * rx);
                              ry2     = (ry * ry);
                              tworx2  = (rx2 << 1);
                              twory2  = (ry2 << 1);

                              incy    = (-ry * tworx2);
                            }

    int                     Dx                              () const      { return dx; }
    int                     Dy                              () const      { return dy; }


    virtual                ~GRP2DELLIPSEBRESENHAM           ()
                            {
                              Clean();
                            }

    void                    operator++                      ()
                            {
                              int mx;
                              int my;
                              int mxy;
                              int min_m;
                              int fx;
                              int fy;
                              int fxy;

                              mx = fx = curf + incx + ry2;
                              if(mx < 0) mx = -mx;

                              my = fy = curf + incy + rx2;
                              if(my < 0) my = -my;

                              mxy = fxy = curf + incx + ry2 + incy + rx2;
                              if(mxy < 0) mxy = -mxy;

                              min_m = mx;
                              bool flag = true;

                              if(min_m > my)
                                {
                                  min_m = my;
                                  flag = false;
                                }

                              dx = dy = 0;

                              if(min_m > mxy)
                                {
                                  incx += twory2;
                                  incy += tworx2;
                                  curf  = fxy;
                                  dx    = 1;
                                  dy    = 1;

                                  return;
                                }

                              if(flag)
                                {
                                  incx += twory2;
                                  curf  = fx;
                                  dx    = 1;

                                  return;
                                }

                              incy += tworx2;
                              curf  = fy;
                              dy    = 1;
                            }



  private:

    void                    Clean                           ()
                            {
                              rx2     = 0;
                              ry2     = 0;
                              tworx2  = 0;
                              twory2  = 0;
                              dx      = 0;
                              dy      = 0;
                              incx    = 0;
                              incy    = 0;
                              curf    = 0;
                            }

    int                     rx2;
    int                     ry2;
    int                     tworx2;
    int                     twory2;
    int                     dx;
    int                     dy;
    int                     incx;
    int                     incy;
    int                     curf;
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

