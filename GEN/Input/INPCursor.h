/*------------------------------------------------------------------------------------------
//  INPCURSOR.H
*/
/**
// \class
//
//  Input Cursor Class
//
//  @author  Abraham J. Velez
//  @version 21/12/2011 12:03:09
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#ifndef _INPCURSOR_H_
#define _INPCURSOR_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XVector.h"
#include "XString.h"
#include "XRect.h"

#include "INPPath.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum INPCURSOR_MOTIONDIR
{
  INPCURSOR_MOTIONDIR_UNKNOWN   = 0 ,

  INPCURSOR_MOTIONDIR_UP            ,
  INPCURSOR_MOTIONDIR_DOWN          ,
  INPCURSOR_MOTIONDIR_RIGHT         ,
  INPCURSOR_MOTIONDIR_LEFT          ,

  INPCURSOR_MOTIONDIR_RIGHTUP       ,
  INPCURSOR_MOTIONDIR_LEFTUP        ,
  INPCURSOR_MOTIONDIR_RIGHTDOWN     ,
  INPCURSOR_MOTIONDIR_LEFTDOWN      ,
};


enum INPCURSOR_ID
{
  INPCURSOR_ID_NONE             = 0 ,

  INPCURSOR_ID_MOUSE                ,
  INPCURSOR_ID_TOUCHSCREEN1         ,
  INPCURSOR_ID_TOUCHSCREEN2         ,
  INPCURSOR_ID_TOUCHSCREEN3         ,
  INPCURSOR_ID_TOUCHSCREEN4         ,
};



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTIMER;
class INPBUTTON;
class INPCURSOR;


class INPCURSORMOTION : public INPPATH
{
  public:
                            INPCURSORMOTION           ();
    virtual                ~INPCURSORMOTION           ();

    bool                    IsInCurse                 ();
    bool                    SetIsInCurse              (bool isincurse);

    bool                    IsReadyToTest             ();

    INPCURSOR_MOTIONDIR     GetDirection              (bool indetail);

    XQWORD                  GetTimeElapsed            ();
    bool                    SetTimeElapsed            (XQWORD timeelapsed);

    bool                    Reset                     ();

  private:

    void                    Clean                     ();

    bool                    isincurse;
    XQWORD                  timeelapsed;
};




class INPCURSOR : public INPPATHPOINT
{
  public:
                            INPCURSOR                 ();
    virtual                ~INPCURSOR                 ();

    INPCURSOR_ID            GetID                     ();
    void                    SetID                     (INPCURSOR_ID ID);

    XTIMER*                 GetTimer                  ();

    bool                    HavePreSelect             ();
    bool                    SetHavePreSelect          (bool havepreselect);

    bool                    IsChanged                 ();
    bool                    SetIsChanged              (bool ischanged);

    bool                    IsPositionInRect          (int x = 0, int y = 0, int width = 0,int height = 0);

    INPCURSORMOTION*        GetMotion                 ();
    bool                    AddPointToMotion          (bool ispressed);


    void                    Set                       (float x = 0, float y = 0, float z = 0);
    void                    SetX                      (float x = 0);
    void                    SetY                      (float y = 0);
    void                    SetZ                      (float z = 0);

  private:

    void                    Clean                     ();

    XTIMER*                 timer;
    INPCURSOR_ID            ID;
    bool                    havepreselect;
    bool                    ischanged;

    XRECT                   source;
    XRECT                   destination;

    INPCURSORMOTION         motion;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

