/*------------------------------------------------------------------------------------------
//  DIOWINDOWSPING.H
*/
/**
// \class
//
//  Data IO Ping class
//
//  @author  Abraham J. Velez
//  @version 30/08/2012 9:13:13
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOWINDOWSPING_H_
#define _DIOWINDOWSPING_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOPing.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/




/*---- CLASS -----------------------------------------------------------------------------*/


class DIOWINDOWSPING  : public  DIOPING
{
  public:

                                  DIOWINDOWSPING              ();
    virtual                      ~DIOWINDOWSPING              ();

    #ifdef DIOPING_USEWINDOWDLL
    bool                          Do                          (XDWORD nretries = DIOPING_DEFAULTNRETRYS, XDWORD timebetweenchecks = DIOPING_DEFAULTTIMEBETWEENCHECKS, bool exitfirstgoodreply = false);
    #endif

  private:

    void                          Clean                       ()
                                  {

                                  }

};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

