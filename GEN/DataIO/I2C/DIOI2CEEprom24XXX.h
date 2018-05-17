/*------------------------------------------------------------------------------------------
//  DIOI2CEEPROM24XXX.H
*/
/**
// \class
//
//  Data IO I2C EEprom 24XXX class.
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 03/05/2014 16:12:18
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOI2CEEPROM24XXX_H_
#define _DIOI2CEEPROM24XXX_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>

#include "XBase.h"

#include "DIODeviceI2C.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum DIOI2CEEPROM24XXXTYPE
{
  DIOI2CEEPROM24XXXTYPE_UNKNOWN       = 0 ,
  DIOI2CEEPROM24XXXTYPE_08                ,
  DIOI2CEEPROM24XXXTYPE_16                ,
  DIOI2CEEPROM24XXXTYPE_65                ,
  DIOI2CEEPROM24XXXTYPE_515               ,
};


#define DIOI2CEEPROM2408_FIXEDADDRESSBASE       0x50
#define DIOI2CEEPROM2416_FIXEDADDRESSBASE       0x50


/*---- CLASS -----------------------------------------------------------------------------*/


class XFACTORY;
class DIOFACTORY;
class XPUBLISHER;
class DIOSTREAMI2CCONFIG;
class DIOSTREAMI2C;


class DIOI2CEEPROM24XXX : public DIODEVICEI2C
{
  public:

                              DIOI2CEEPROM24XXX           ();
    virtual                  ~DIOI2CEEPROM24XXX           ();

    DIOI2CEEPROM24XXXTYPE     GetType                     ()                              { return type;                                   }
    void                      SetType                     (DIOI2CEEPROM24XXXTYPE type)    { this->type = type;                             }

    XDWORD                    GetSize                     ()                              { return maxsize;                                }

    bool                      Read                        (XDWORD address, XBYTE* buffer, XDWORD size);
    bool                      Read                        (XDWORD address, XDWORD size, XBUFFER& xbuffer);

    bool                      Write                       (XDWORD address, XBYTE* buffer, XDWORD size);
    bool                      Write                       (XDWORD address, XBUFFER& xbuffer);

    bool                      End                         ();

  protected:

    DIOSTREAMI2C*             diostream;

    DIOI2CEEPROM24XXXTYPE     type;

    XDWORD                    maxsize;                        // All descriptors in Bytes.
    XWORD                     npages;
    XWORD                     maxsizepage;
    XWORD                     maxsizechunk;

  private:

    bool                      IniDevice                   ();

    void                      Clean                       ()
                              {
                                diostream     = NULL;

                                type          = DIOI2CEEPROM24XXXTYPE_UNKNOWN;
                                maxsize;
                                npages;
                                maxsizepage;
                                maxsizechunk;
                              }

    bool                      ReadBYTE                        (XDWORD address, XBYTE& data);
    bool                      WriteBYTE                       (XDWORD address, XBYTE data);
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

