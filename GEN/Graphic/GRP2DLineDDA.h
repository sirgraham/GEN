/*------------------------------------------------------------------------------------------
//  GRP2DLINEDDA.H
*/
/**
// \class
//
//   Graphics 2 Line Digital Differential Analyzer (DDA) Classes
//
//  @author  Abraham J. Velez
//  @version 07/08/2013 12:32:28
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRP2DLINEDDA_H_
#define _GRP2DLINEDDA_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


template<int FRACTIONSHIFT, int YSHIFT = 0>
class GRP2DLINEDDAINTERPOLATOR
{
  public:
                            GRP2DLINEDDAINTERPOLATOR          ()
                            {
                              Clean();
                            }


                            GRP2DLINEDDAINTERPOLATOR          (int y1, int y2, XDWORD count)
                            {
                              Clean();

                              y   = y1;
                              inc = (((y2 - y1) << FRACTIONSHIFT) / int(count));
                            }


    virtual                ~GRP2DLINEDDAINTERPOLATOR          ()
                            {
                              Clean();
                            }

    void                    operator++                      ()                  { dy += inc;      }
    void                    operator--                      ()                  { dy -= inc;      }
    void                    operator+=                      (XDWORD n)          { dy += inc * n;  }
    void                    operator-=                      (XDWORD n)          { dy -= inc * n;  }
    int                     Y                               () const            { return y + (dy >> (FRACTIONSHIFT-YSHIFT)); }
    int                     Dy                              () const            { return dy;      }

  private:

    void                    Clean                           ()
                            {
                              y   = 0;
                              inc = 0;
                              dy  = 0;
                            }


    int                     y;
    int                     inc;
    int                     dy;
};





class GRP2DLINEDDA2INTERPOLATOR
{
  public:

    typedef int             SAVEDATATYPE;
    enum
    {
      SAVESIZE = 2
    };
                            GRP2DLINEDDA2INTERPOLATOR       ()
                            {
                              Clean();
                            }


                            GRP2DLINEDDA2INTERPOLATOR       (int y1, int y2, int count)
                            {
                              Clean();

                              cnt = (count <= 0) ? 1 : count;
                              lft = ((y2 - y1) / cnt);
                              rem = ((y2 - y1) % cnt);
                              mod = rem;
                              y   = y1;

                              if(mod <= 0)
                                {
                                  mod += count;
                                  rem += count;
                                  lft--;
                                }

                              mod -= count;
                            }


                            GRP2DLINEDDA2INTERPOLATOR       (int y1, int y2, int count, int)
                            {
                              Clean();

                              cnt = (count <= 0 )? 1 : count;
                              lft = ((y2 - y1) / cnt);
                              rem = ((y2 - y1) % cnt),
                              mod = rem;
                              y   = y1;

                              if(mod <= 0)
                                {
                                  mod += count;
                                  rem += count;
                                  lft--;
                                }

                            }


                            GRP2DLINEDDA2INTERPOLATOR       (int y, int count)
                            {
                              cnt = (count <= 0)? 1 : count;
                              lft = (y / cnt);
                              rem = (y % cnt);
                              mod = rem;
                              y   = 0;

                              if(mod <= 0)
                                {
                                  mod += count;
                                  rem += count;
                                  lft--;
                                }
                            }


    virtual                ~GRP2DLINEDDA2INTERPOLATOR       ()
                            {
                              Clean();
                            }


    void                    Save                            (SAVEDATATYPE* data) const
                            {
                              data[0] = mod;
                              data[1] = y;
                            }


    void                    Load                            (const SAVEDATATYPE* data)
                            {
                              mod = data[0];
                              y   = data[1];
                            }


    void                    operator++                      ()
                            {
                              mod += rem;
                              y   += lft;

                              if(mod > 0)
                                {
                                  mod -= cnt;
                                  y++;
                                }
                            }


    void                    operator--                      ()
                            {
                              if(mod <= rem)
                                {
                                  mod += cnt;
                                  y--;
                                }

                              mod -= rem;
                              y   -= lft;
                            }


    void                    AdjustForward                   ()            { mod -= cnt; }
    void                    AdjustBackward                  ()            { mod += cnt; }
    int                     Mod                             () const      { return mod; }
    int                     Rem                             () const      { return rem; }
    int                     Lft                             () const      { return lft; }
    int                     Y                               () const      { return y;   }

  private:

    void                    Clean                           ()
                            {
                              cnt = 0;
                              lft = 0;
                              rem = 0;
                              mod = 0;
                              y   = 0;
                            }

    int                     cnt;
    int                     lft;
    int                     rem;
    int                     mod;
    int                     y;
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/



#endif

