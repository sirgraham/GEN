//------------------------------------------------------------------------------------------
//  DIOSTREAM.H
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

#ifndef _DIOSTREAMDEVICE_H_
#define _DIOSTREAMDEVICE_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XUUID.h"
#include "XString.h"
#include "XDebug.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOSTREAMDEVICE_TYPE
{
  DIOSTREAMDEVICE_TYPE_UNKNOWN            = 0   ,
  DIOSTREAMDEVICE_TYPE_UART                   ,
  DIOSTREAMDEVICE_TYPE_USB                    ,
  DIOSTREAMDEVICE_TYPE_IP                     ,
  DIOSTREAMDEVICE_TYPE_BLUETOOTH              ,
  DIOSTREAMDEVICE_TYPE_SPI                    ,
};

#define DIOSTREAMDEBUGPRINTINFO(console, line)      if(console) console->Printf(__L("%s\n"), line); \
                                                    XDEBUG_PRINTCOLOR(2, line)

#define DIOGUID     XUUID


//---- CLASS -------------------------------------------------------------------------------

class XCONSOLE;

class DIOSTREAMDEVICE
{
  public:
                                DIOSTREAMDEVICE                 ();
    virtual                    ~DIOSTREAMDEVICE                 ();

    int                         GetIndex                        ();
    bool                        SetIndex                        (int index);

    DIOSTREAMDEVICE_TYPE        GetType                         ();
    bool                        SetType                         (DIOSTREAMDEVICE_TYPE type);

    XSTRING*                    GetName                         ();
    XSTRING*                    GetDescription                  ();
    XSTRING*                    GetResource                     ();

    virtual bool                CopyFrom                        (DIOSTREAMDEVICE& device);
    virtual bool                CopyTo                          (DIOSTREAMDEVICE& device);

    virtual bool                DebugPrintInfo                  (XCONSOLE* xconsole);

  protected:

    int                         index;
    DIOSTREAMDEVICE_TYPE        type;
    XSTRING                     name;
    XSTRING                     description;
    XSTRING                     resource;

  private:

    void                        Clean                           ();
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif
