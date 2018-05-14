//------------------------------------------------------------------------------------------
//  INPBUTTON.H
//
/**
// \class
//
//  Input Button Class
//
//  @author  Abraham J. Velez
//  @version 11/02/2003
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _INPBUTTON_H_
#define _INPBUTTON_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XString.h"
#include "XVector.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum INPBUTTON_ID
{
   INPBUTTON_ID_NOBUTTON        = 0 ,

   //------------------------------------------------------------------------------------------
   // Keyboard

   INPBUTTON_ID_BACK_SPACE          ,
   INPBUTTON_ID_TAB                 ,
   INPBUTTON_ID_RETURN              ,
   INPBUTTON_ID_INTRO               ,
   INPBUTTON_ID_ESCAPE              ,
   INPBUTTON_ID_CAPS_LOCK           ,

   INPBUTTON_ID_SHIFT_RIGHT         ,
   INPBUTTON_ID_SHIFT_LEFT          ,
   INPBUTTON_ID_CONTROL_RIGHT       ,
   INPBUTTON_ID_CONTROL_LEFT        ,
   INPBUTTON_ID_ALT_RIGHT           ,
   INPBUTTON_ID_ALT_LEFT            ,

   INPBUTTON_ID_MENU                ,
   INPBUTTON_ID_LWIN                ,
   INPBUTTON_ID_RWIN                ,
   INPBUTTON_ID_NN                  ,

   INPBUTTON_ID_OPEN_BRANCH         ,
   INPBUTTON_ID_CLOSE_BRANCH        ,
   INPBUTTON_ID_ARROWS              ,
   INPBUTTON_ID_COMMA               ,
   INPBUTTON_ID_MINUS               ,
   INPBUTTON_ID_POINT               ,
   INPBUTTON_ID_SLASH               ,
   INPBUTTON_ID_BACK_QUOTE          ,
   INPBUTTON_ID_OPEN_BRACKET        ,
   INPBUTTON_ID_CLOSE_BRACKET       ,
   INPBUTTON_ID_QUOTE               ,
   INPBUTTON_ID_SPACE               ,

   INPBUTTON_ID_PAGE_UP             ,
   INPBUTTON_ID_PAGE_DOWN           ,
   INPBUTTON_ID_END                 ,
   INPBUTTON_ID_HOME                ,
   INPBUTTON_ID_LEFT                ,
   INPBUTTON_ID_UP                  ,
   INPBUTTON_ID_RIGHT               ,
   INPBUTTON_ID_DOWN                ,
   INPBUTTON_ID_INSERT              ,
   INPBUTTON_ID_DELETE              ,

   INPBUTTON_ID_0                   ,
   INPBUTTON_ID_1                   ,
   INPBUTTON_ID_2                   ,
   INPBUTTON_ID_3                   ,
   INPBUTTON_ID_4                   ,
   INPBUTTON_ID_5                   ,
   INPBUTTON_ID_6                   ,
   INPBUTTON_ID_7                   ,
   INPBUTTON_ID_8                   ,
   INPBUTTON_ID_9                   ,

   INPBUTTON_ID_A                   ,
   INPBUTTON_ID_B                   ,
   INPBUTTON_ID_C                   ,
   INPBUTTON_ID_D                   ,
   INPBUTTON_ID_E                   ,
   INPBUTTON_ID_F                   ,
   INPBUTTON_ID_G                   ,
   INPBUTTON_ID_H                   ,
   INPBUTTON_ID_I                   ,
   INPBUTTON_ID_J                   ,
   INPBUTTON_ID_K                   ,
   INPBUTTON_ID_L                   ,
   INPBUTTON_ID_M                   ,
   INPBUTTON_ID_N                   ,
   INPBUTTON_ID_O                   ,
   INPBUTTON_ID_P                   ,
   INPBUTTON_ID_Q                   ,
   INPBUTTON_ID_R                   ,
   INPBUTTON_ID_S                   ,
   INPBUTTON_ID_T                   ,
   INPBUTTON_ID_U                   ,
   INPBUTTON_ID_V                   ,
   INPBUTTON_ID_W                   ,
   INPBUTTON_ID_X                   ,
   INPBUTTON_ID_Y                   ,
   INPBUTTON_ID_Z                   ,

   INPBUTTON_ID_NUMLOCK             ,
   INPBUTTON_ID_NUMPAD0             ,
   INPBUTTON_ID_NUMPAD1             ,
   INPBUTTON_ID_NUMPAD2             ,
   INPBUTTON_ID_NUMPAD3             ,
   INPBUTTON_ID_NUMPAD4             ,
   INPBUTTON_ID_NUMPAD5             ,
   INPBUTTON_ID_NUMPAD6             ,
   INPBUTTON_ID_NUMPAD7             ,
   INPBUTTON_ID_NUMPAD8             ,
   INPBUTTON_ID_NUMPAD9             ,
   INPBUTTON_ID_MULTIPLY            ,
   INPBUTTON_ID_ADD                 ,
   INPBUTTON_ID_SUBTRACT            ,
   INPBUTTON_ID_DECIMAL             ,
   INPBUTTON_ID_DIVIDE              ,

   INPBUTTON_ID_F1                  ,
   INPBUTTON_ID_F2                  ,
   INPBUTTON_ID_F3                  ,
   INPBUTTON_ID_F4                  ,
   INPBUTTON_ID_F5                  ,
   INPBUTTON_ID_F6                  ,
   INPBUTTON_ID_F7                  ,
   INPBUTTON_ID_F8                  ,
   INPBUTTON_ID_F9                  ,
   INPBUTTON_ID_F10                 ,
   INPBUTTON_ID_F11                 ,
   INPBUTTON_ID_F12                 ,

   INPBUTTON_ID_PRINTSCREEN         ,
   INPBUTTON_ID_SCROLL_LOCK         ,
   INPBUTTON_ID_PAUSE               ,


   //-------------------------------------------------------------------------------------------
   // Mouse
   INPBUTTON_ID_MOUSE_RIGHT         ,
   INPBUTTON_ID_MOUSE_LEFT          ,
   INPBUTTON_ID_MOUSE_MIDDLE        ,


   //-------------------------------------------------------------------------------------------
   // SmartPhone
   INPBUTTON_ID_SMARTPHONE_A        ,
   INPBUTTON_ID_SMARTPHONE_B        ,
   INPBUTTON_ID_SMARTPHONE_C        ,

   INPBUTTON_ID_SMARTPHONE_START    ,
   INPBUTTON_ID_SMARTPHONE_SELECT   ,

   //-------------------------------------------------------------------------------------------
   // WiiMote
   INPBUTTON_ID_WIIMOTE_A           ,
   INPBUTTON_ID_WIIMOTE_B           ,
   INPBUTTON_ID_WIIMOTE_MINUS       ,
   INPBUTTON_ID_WIIMOTE_HOME        ,
   INPBUTTON_ID_WIIMOTE_PLUS        ,
   INPBUTTON_ID_WIIMOTE_ONE         ,
   INPBUTTON_ID_WIIMOTE_TWO         ,
   INPBUTTON_ID_WIIMOTE_UP          ,
   INPBUTTON_ID_WIIMOTE_DOWN        ,
   INPBUTTON_ID_WIIMOTE_LEFT        ,
   INPBUTTON_ID_WIIMOTE_RIGHT       ,
   INPBUTTON_ID_WIIMOTE_C           ,
   INPBUTTON_ID_WIIMOTE_Z           ,

   // Touch
   INPBUTTON_ID_TOUCH               ,

   INPBUTTON_ID_LAST
};


enum INPBUTTON_STATE
{
  INPBUTTON_STATE_UNKNOWN,
  INPBUTTON_STATE_RELEASED,
  INPBUTTON_STATE_UP,
  INPBUTTON_STATE_PRESSED,
  INPBUTTON_STATE_HOLD

};

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTIMER;



class INPBUTTON
{
  public:
                      INPBUTTON             ();
    virtual          ~INPBUTTON             ();

    INPBUTTON_ID      GetID                 ();
    void              SetID                 (INPBUTTON_ID ID);

    XWORD             GetKeyCode            ();
    void              SetKeyCode            (XWORD keycode);

    XCHAR             GetSymbol             ();
    void              SetSymbol             (XCHAR symbol);

    float             GetPressure           ();
    void              SetPressure           (float p);

    bool              IsPressed             ();
    bool              IsPressedWithRelease  ();
    bool              IsRead                ();

    void              SetPressed            (bool on);

    bool              IsMarkToRelase        ();
    bool              SetMarkToRelase       (bool on);

    XQWORD            GetTimePressed        ();
    bool              SetTimePressed        (XDWORD time);
    bool              ResetTimePressed      ();

    static bool       CreateButton          ( XVECTOR<INPBUTTON*>* buttons, int code, INPBUTTON_ID ID, XCHAR symbol);
    INPBUTTON_STATE   GetState              () { isread=true;return state;}

    INPBUTTON_STATE   state;

    bool              updated;

  private:


    void              Clean                 ();

    INPBUTTON_ID      ID;
    XWORD             keycode;
    XCHAR             symbol;
    float             pressure;
    bool              pressed;
    bool              isread;
    bool              marktorelase;
    XTIMER*           xtimer;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

