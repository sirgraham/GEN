 //------------------------------------------------------------------------------------------
//  DIOSTREAMBLUETOOTH.H
//
/**
// \class
//
//  Data IO Stream class
//
//  @author  Abraham J. Velez
//  @version 23/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTREAMBLUETOOTH_H_
#define _DIOSTREAMBLUETOOTH_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOStream.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XTIMER;
class DIOSTREAMBLUETOOTHCONFIG;


class DIOSTREAMBLUETOOTH : public DIOSTREAM
{
  public:
                                          DIOSTREAMBLUETOOTH                        ();
    virtual                              ~DIOSTREAMBLUETOOTH                        ();

    DIOSTREAMCONFIG*                      GetConfig                                 ();
    bool                                  SetConfig                                 (DIOSTREAMCONFIG* config);

    virtual bool                          Open                                      ()                                = 0;

    bool                                  IsAcceptConnexion                         ();
    bool                                  WaitToAcceptConnexion                     (int timeout);

    virtual bool                          Close                                     ()                                = 0;

    bool                                  IsRefusedConnexion                        ();
    bool                                  SetIsRefusedConnexion                     (bool isrefused);

  protected:

    DIOSTREAMBLUETOOTHCONFIG*             config;
    bool                                  isrefusedconnexion;

  private:

    void                                  Clean                                     ();
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
