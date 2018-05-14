//------------------------------------------------------------------------------------------
//  DIOSTREAMICMPCONFIG.H
//
/**
// \class
//
//  Data IO Stream ICMP Config class
//
//  @author  Abraham J. Velez
//  @version 23/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOSTREAMICMPCONFIG_H_
#define _DIOSTREAMICMPCONFIG_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOIP.h"
#include "DIOURL.h"

#include "DIOStreamConfig.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XSTRING;
class DIOFACTORY;


class DIOSTREAMICMPCONFIG : public DIOSTREAMCONFIG
{
  public:
                            DIOSTREAMICMPCONFIG         ();
    virtual                ~DIOSTREAMICMPCONFIG         ();

    DIOIP*                  GetLocalIP                  ();
    DIOURL*                 GetRemoteURL                ();

  protected:

    DIOIP                   localIP;
    DIOURL*                 remoteURL;

  private:



    void                    Clean                       ();
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif
