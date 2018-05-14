//------------------------------------------------------------------------------------------
//  DIOLINUXSTREAMIPLOCALENUMDEVICES.H
//
/**
// \class
//
//  LINUX Data IO Stream IP Local Enum Devices class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOLINUXSTREAMIPLOCALENUMDEVICES_H_
#define _DIOLINUXSTREAMIPLOCALENUMDEVICES_H_


//---- INCLUDES ----------------------------------------------------------------------------


#include "DIOStreamIPLocalEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define DIOLINUXSTREAMIPLOCALENUMDEVICES_MAXIFS   64

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;


class DIOLINUXSTREAMIPLOCALENUMDEVICES :  public DIOSTREAMIPLOCALENUMDEVICES
{
  public:
                              DIOLINUXSTREAMIPLOCALENUMDEVICES      ();
    virtual                  ~DIOLINUXSTREAMIPLOCALENUMDEVICES      ();

    bool                      Search                                ();
};




//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

