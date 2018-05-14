/*------------------------------------------------------------------------------------------
//  INPLINUXDEVICEMOUSEDIRECT.H
*/
/**
// \class
//
//  Input LINUX device mouse class
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 10/04/2014 12:54:27
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _INPLINUXDEVICEMOUSEDIRECT_H_
#define _INPLINUXDEVICEMOUSEDIRECT_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"

#include "INPCursor.h"
#include "INPDevice.h"

#include "INPLINUXDeviceID.h"
#include "GRPScreen.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;


class INPLINUXDEVICEMOUSEDIRECT : public INPDEVICE
{
  public:

                                  INPLINUXDEVICEMOUSEDIRECT   ( GRPSCREEN* grpscreen);
  virtual                        ~INPLINUXDEVICEMOUSEDIRECT   ();
  bool                            SetScreen(void* screenpointer);


  XVECTOR<INPBUTTON*>*            GetButtons                  ()              { return &buttons;    }
  XVECTOR<INPCURSOR*>*            GetCursors                  ()              { return &cursors;    }

protected:

    bool                          Update                      ();

  private:

    void                          Clean                       ()
                                  {
                                    grpscreen =  NULL;

                                    mousex    = 0;
                                    minx      = 0;
                                    maxx      = 0;

                                    mousey    = 0;
                                    miny      = 0;
                                    maxy      = 0;

                                    mousez    = 0;
                                    minz      = 0;
                                    maxz      = 0;
                                  }

    bool                          CreateAllButtons            ();
    bool                          DeleteAllButtons            ();

    bool                          CreateAllCursors            ();
    bool                          DeleteAllCursors            ();

    bool                          OpenAllDevicesID            ();
    bool                          CloseAllDevicesID           ();

    bool                          UpdateMouseData             ();

    GRPSCREEN*                    grpscreen;

    XVECTOR<INPBUTTON*>           buttons;
    XVECTOR<INPCURSOR*>           cursors;

    XVECTOR<INPLINUXDEVICEID*>    mice;

    int                           mousex;
    int                           minx;
    int                           maxx;

    int                           mousey;
    int                           miny;
    int                           maxy;

    int                           mousez;
    int                           minz;
    int                           maxz;


};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/


#endif
