//----------------------------------------------------------------------------------------
//  GRPBITMAPSEQUENCE.H
//
/**
// \class
//
//  Graphics Bitmap Sequence class
//
//  @author  Diego Martinez Ruiz de Gaona
//  @version 28/11/2003 13:35:00
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _GRPBITMAPSEQUENCE_H_
#define _GRPBITMAPSEQUENCE_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XVector.h"

#include "GRPProperties.h"
#include "GRPBitmap.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum GRPBITMAPSEQUENCESTATUS
{
  GRPBITMAPSEQUENCESTATUS_NONE     = 0,
  GRPBITMAPSEQUENCESTATUS_INIT        ,
  GRPBITMAPSEQUENCESTATUS_PLAY        ,
  GRPBITMAPSEQUENCESTATUS_STOP        ,
  GRPBITMAPSEQUENCESTATUS_PAUSE       ,
  GRPBITMAPSEQUENCESTATUS_END
};


#define GRPBITMAPSEQUENCE_INFINITE      0
#define GRPBITMAPSEQUENCE_FRAMESSECOND  12

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTIMER;
class GRPRECT;
class GRPBITMAP;

class GRPBITMAPFRAME
{
  public:

                                  GRPBITMAPFRAME                ()                        { Clean();                  }
    virtual                      ~GRPBITMAPFRAME                ()
                                  {
                                    delete bitmap;
                                    Clean();
                                  }

    GRPBITMAP*                    GetBitmap                     ()                        { return bitmap;            }
    int                           GetAdjustX                    ()                        { return adjustx;           }
    int                           GetAdjustY                    ()                        { return adjusty;           }

    void                          SetBitmap                     (GRPBITMAP* bitmap)       { this->bitmap = bitmap;    }
    void                          SetAdjustX                    (int adjustx)             { this->adjustx = adjustx;  }
    void                          SetAdjustY                    (int adjusty)             { this->adjusty = adjusty;  }

  private:

    void                          Clean                         ()
                                  {
                                    bitmap = NULL;
                                    adjustx = 0;
                                    adjusty = 0;
                                  }

    GRPBITMAP*                    bitmap;
    int                           adjustx;
    int                           adjusty;
};



class GRPBITMAPSEQUENCE
{
  public:
                                  GRPBITMAPSEQUENCE             ();
    virtual                      ~GRPBITMAPSEQUENCE             ();

    bool                          AddFrame                      (GRPBITMAP* bitmap, int adjustx = 0, int adjusty = 0);
    bool                          DelAllSequence                (bool delbitmaps = true);

    GRPBITMAPFRAME*               GetActualFrame                ();
    int                           GetActualFrameIndex           ();
    GRPBITMAPFRAME*               GetFrame                      (XWORD nframe);
    int                           GetNFrames                    ();

    int                           GetNLoops                     ();
    int                           GetNLoopsMade                 ();

    bool                          SetNLoops                     (int nloops);
    bool                          SetNLoopsMade                 (int nloopsmade);

    bool                          AdjustAllSequence             (int adjustx = 0,int adjusty = 0);

    GRPBITMAPFRAME*               Play                          (int nloops = GRPBITMAPSEQUENCE_INFINITE, bool adjusttimeauto = true, bool reverse = false);
    bool                          Stop                          ();
    bool                          Pause                         ();
    bool                          Finished                      ();

    bool                          IsPlaying                     ();
    bool                          IsEndSequence                 ();

    GRPBITMAPFRAME*               Update                        ();

    bool                          AdjustTime                    (int  framebysec);


  private:

    void                          Clean                         ();


    int                           framebysec;
    float                         timebyframe;
    XTIMER*                       timer;

    bool                          repeat;
    GRPBITMAPSEQUENCESTATUS       status;

    int                           frameindex;
    XVECTOR<GRPBITMAPFRAME*>      frames;

    int                           nloops;
    int                           nloopsmade;

    bool                          reverse;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

