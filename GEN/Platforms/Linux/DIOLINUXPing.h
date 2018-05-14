/*------------------------------------------------------------------------------------------
//  DIOLINUXPING.H
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

#ifndef _DIOLINUXPING_H_
#define _DIOLINUXPING_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "DIOPing.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


class DIOLINUXPING  : public  DIOPING
{
  public:

                                  DIOLINUXPING                ();
    virtual                      ~DIOLINUXPING                ();

    bool                          Do                          (XDWORD nretries = DIOPING_DEFAULTNRETRYS, XDWORD timebetweenchecks = DIOPING_DEFAULTTIMEBETWEENCHECKS, bool exitfirstgoodreply = false);

  private:

    void                          Clean                       ()
                                  {

                                  }
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

