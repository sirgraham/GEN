/*------------------------------------------------------------------------------------------
//  INPANDROIDDEVICETOUCHSCREEN.H
*/
/**
// \class
//
//  Input Device Touch Screen
//
//  @author  Abraham J. Velez
//  @version 21/12/2011 16:54:40
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _INPANDROIDDEVICETOUCHSCREEN_H_
#define _INPANDROIDDEVICETOUCHSCREEN_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XVector.h"

#include "INPCursor.h"
#include "INPDevice.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class GRPANDROIDSCREEN;

class INPANDROIDDEVICEMOUSE : public INPDEVICE
{
  public:
                                    INPANDROIDDEVICEMOUSE         (GRPANDROIDSCREEN*  grpscreen=NULL);
        virtual                    ~INPANDROIDDEVICEMOUSE         ();

virtual bool                        SetScreen                     (void* screenpointer);

        bool                        Update                        ();

        bool                        AddPosition                   (int index, float x, float y, bool ispressed);

        XVECTOR<INPCURSOR*>*        GetCursors                    ();
        XVECTOR<INPBUTTON*>*        GetButtons                    ()          {   return &buttons;    }

  private:

        void                        Clean                         ();

        bool                        CreateAllCursors              ();
        bool                        CreateAllButtons              ();
        bool                        DeleteAllCursors              ();


        GRPANDROIDSCREEN*           grpscreen;

        XVECTOR<INPCURSOR*>         cursors;
        XVECTOR<INPBUTTON*>         buttons;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------



#endif

