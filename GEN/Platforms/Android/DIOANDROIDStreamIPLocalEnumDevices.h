//------------------------------------------------------------------------------------------
//  DIOANDROIDSTREAMIPLOCALENUMDEVICES.H
//
/**
// \class
//
//  ANDROID Data IO Stream IP Local Enum Devices class
//
//  @author  Abraham J. Velez
//  @version 02/01/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOANDROIDSTREAMIPLOCALENUMDEVICES_H_
#define _DIOANDROIDSTREAMIPLOCALENUMDEVICES_H_


//---- INCLUDES ----------------------------------------------------------------------------


#include "DIOStreamIPLocalEnumDevices.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define DIOANDROIDSTREAMIPLOCALENUMDEVICES_MAXIFS   64

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;


class DIOANDROIDSTREAMIPLOCALENUMDEVICES :  public DIOSTREAMIPLOCALENUMDEVICES
{
  public:
                              DIOANDROIDSTREAMIPLOCALENUMDEVICES      ( );
    virtual                  ~DIOANDROIDSTREAMIPLOCALENUMDEVICES      ();

    bool                      Search                                ();
};




//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

