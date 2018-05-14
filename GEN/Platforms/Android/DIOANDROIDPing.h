/*------------------------------------------------------------------------------------------
//  DIOANDROIDPING.H
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

#ifndef _DIOANDROIDPING_H_
#define _DIOANDROIDPING_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOPing.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/




/*---- CLASS -----------------------------------------------------------------------------*/


class DIOANDROIDPING  : public  DIOPING
{
  public:

                                  DIOANDROIDPING              ();
    virtual                      ~DIOANDROIDPING              ();

    bool                          Do                          (XDWORD nretries = DIOPING_DEFAULTNRETRYS, XDWORD timebetweenchecks = DIOPING_DEFAULTTIMEBETWEENCHECKS, bool exitfirstgoodreply = false);

  private:

    void                          Clean                       ()
                                  {

                                  }
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

