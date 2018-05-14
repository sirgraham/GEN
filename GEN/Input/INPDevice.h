//------------------------------------------------------------------------------------------
//  INPDEVICE.H
//
/**
// \class
//
//  Input Device Class
//
//  @author  Abraham J. Velez
//  @version 10/02/2003
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _INPDEVICE_H_
#define _INPDEVICE_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XString.h"

#include "INPButton.h"
#include "INPCursor.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum INPDEVICE_TYPE
{
  INPDEVICE_TYPE_NONE           = 0 ,

  INPDEVICE_TYPE_KEYBOARD           ,
  INPDEVICE_TYPE_MOUSE              ,
  INPDEVICE_TYPE_JOSTICK            ,
  INPDEVICE_TYPE_TOUCHSCREEN        ,
  INPDEVICE_TYPE_WIIMOTE            ,
};


#define INPDEVICE_SIZEID  32

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTIMER;
class INPBUTTON;
class INPCURSOR;


class INPDEVICE
{
  public:
                                    INPDEVICE           ();
    virtual                        ~INPDEVICE           ();


    virtual bool                    SetScreen           (void* screenpointer) = 0;
    virtual bool                    AddPosition         (int index, float x, float y, bool ispressed){ return true;}

    bool                            IsCreated           ();
    bool                            IsEnabled           ();

    void                            SetEnabled          (bool ON);

    INPDEVICE_TYPE                  GetType             ();
    void                            SetType             (INPDEVICE_TYPE type);

    int                             GetNButtons         ();
    INPBUTTON*                      GetButton           (INPBUTTON_ID ID);
    INPBUTTON*                      GetButton           (int index);
    INPBUTTON_STATE                 GetButtonState      (INPBUTTON_ID ID);
    INPBUTTON*                      GetButtonByCode     (XWORD code);
    INPBUTTON*                      IsPressButton       ();

    int                             GetNCursors         ();
    INPCURSOR*                      GetCursor           (INPCURSOR_ID ID);
    INPCURSOR*                      GetCursor           (int index);
    INPCURSOR*                      IsChangeCursor      ();

    virtual XVECTOR<INPBUTTON*>*    GetButtons          ();
    virtual XVECTOR<INPCURSOR*>*    GetCursors          ();

    virtual bool                    Release             ();
    virtual bool                    Update              ();

  protected:

    bool                            created;
    bool                            enabled;

    INPDEVICE_TYPE                  type;

  private:



    void                            Clean               ();
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

