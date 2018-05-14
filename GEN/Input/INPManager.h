//------------------------------------------------------------------------------------------
//  INPMANAGER.H
//
/**
// \class
//
//  Input Manager
//
//  @author  Abraham J. Velez
//  @version 10/02/2003
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _INPMANAGER_H_
#define _INPMANAGER_H_

//---- INCLUDES ----------------------------------------------------------------------------


#include "XBase.h"
#include "XMap.h"

#include "INPDevice.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------


class XFACTORY;

class INPMANAGER
{
  public:

    bool                                AddDevice                 (INPDEVICE* device);
    bool                                DelDevice                 (INPDEVICE* device);

    bool                                DeleteAllDevices          ();

    INPDEVICE_TYPE                      GetDeviceIDByName         (XSTRING& name);
    INPDEVICE_TYPE                      GetDeviceIDByName         (XCHAR* name);

    int                                 GetNDevices               ();
    INPDEVICE*                          GetDevice                 (int index);
    INPDEVICE*                          GetDevice                 (INPDEVICE_TYPE type);
    INPDEVICE*                          GetDevice                 (XSTRING& name);
    INPDEVICE*                          GetDevice                 (XCHAR* name);

    virtual bool                        Update                    ();

    static INPMANAGER*                  GetInstance               ();
    static void                         DelInstance               ();

  protected:



                                        INPMANAGER                ();
                                        INPMANAGER                (INPMANAGER const&);        // Don't implement
    virtual                            ~INPMANAGER                ();

    void                                operator =                (INPMANAGER const&);        // Don't implement

    static INPMANAGER*                  Instance;


  private:

    void                                Clean                     ();


    XMAP<int, INPDEVICE*>               devicemap;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

