/*------------------------------------------------------------------------------------------
//  DIOBUSPIRATE.H
*/
/**
// \class
//
//  DIO Interface to Bus Pirate 4.0
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 05/02/2015 10:56:44
//  Last Modification :
*/
/*  GEN (C) Copyright  (All right reserved).
//----------------------------------------------------------------------------------------*/

#ifndef _DIOBUSPIRATE_H_
#define _DIOBUSPIRATE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


enum DIOBUSPIRATETYPEPROCOTOL
{
  DIOBUSPIRATETYPEPROCOTOL_UNKNOWN      = 0 ,
  DIOBUSPIRATETYPEPROCOTOL_HIZ              ,
  DIOBUSPIRATETYPEPROCOTOL_1WIRE            ,
  DIOBUSPIRATETYPEPROCOTOL_UART             ,
  DIOBUSPIRATETYPEPROCOTOL_I2C              ,
  DIOBUSPIRATETYPEPROCOTOL_SPI              ,
  DIOBUSPIRATETYPEPROCOTOL_2WIRE            ,
  DIOBUSPIRATETYPEPROCOTOL_3WIRE            ,
  DIOBUSPIRATETYPEPROCOTOL_KEYB             ,
  DIOBUSPIRATETYPEPROCOTOL_LCD              ,
  DIOBUSPIRATETYPEPROCOTOL_PIC              ,
  DIOBUSPIRATETYPEPROCOTOL_DIO              ,
};


enum DIOBUSPIRATESPEEDSNIFFER
{
  DIOBUSPIRATESPEEDSNIFFER_UNKNOWN      = 0 ,
  DIOBUSPIRATESPEEDSNIFFER_5KHZ             ,
  DIOBUSPIRATESPEEDSNIFFER_50KHZ            ,
  DIOBUSPIRATESPEEDSNIFFER_100KHZ           ,
  DIOBUSPIRATESPEEDSNIFFER_400KHZ           ,
};


enum DIOBUSPIRATESPEEDNOSNIFFER
{
  DIOBUSPIRATESPEEDNOSNIFFER_UNKNOWN    = 0 ,
  DIOBUSPIRATESPEEDNOSNIFFER_100KHZ         ,
  DIOBUSPIRATESPEEDNOSNIFFER_400KHZ         ,
  DIOBUSPIRATESPEEDNOSNIFFER_1MHZ           ,
};


#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_HIZ    __L("HiZ")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_1WIRE  __L("1-WIRE")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_UART   __L("UART")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_I2C    __L("I2C")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_SPI    __L("SPI")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_2WIRE  __L("2-WIRE")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_3WIRE  __L("3-WIRE")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_KEYB   __L("3-WIRE")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_LCD    __L("LCD")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_PIC    __L("PIC")
#define DIOBUSPIRATE_COMMANDMENU_PROTOCOL_DIO    __L("DIO")

#define DIOBUSPIRATE_DEFAULTTIMEOUT  5

/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XPUBLISHER;
class XTIMER;
class DIOFACTORY;
class DIOSTREAMUARTCONFIG;


class DIOBUSPIRATE
{
  public:
                                DIOBUSPIRATE            (DIOSTREAM* diostream);
    virtual                    ~DIOBUSPIRATE            ();

    bool                        Ini                     (int timeout = DIOBUSPIRATE_DEFAULTTIMEOUT);

    DIOBUSPIRATETYPEPROCOTOL    GetActualProtocol       (int timeout = DIOBUSPIRATE_DEFAULTTIMEOUT);
    bool                        SetMode                 (DIOBUSPIRATETYPEPROCOTOL protocol, bool issoftware, int speed, bool issniffer, int timeout = DIOBUSPIRATE_DEFAULTTIMEOUT);

    bool                        ReadI2CSnifferCommand   (XBUFFER& in, XBUFFER& out);

    bool                        End                     ();


  private:

    void                        Clean                   ()
                                {
                                  diostream   = NULL;

                                  xtimerout   = NULL;
                                }

    bool                        ReadPromptStatus        (XSTRING& status, int timeout = DIOBUSPIRATE_DEFAULTTIMEOUT);

    bool                        WriteCommand            (XCHAR* command, int timeout = DIOBUSPIRATE_DEFAULTTIMEOUT);
    bool                        WriteCommand            (XSTRING& command, int timeout = DIOBUSPIRATE_DEFAULTTIMEOUT)    { return WriteCommand(command.Get(), timeout);           }


    DIOSTREAM*                  diostream;

    XTIMER*                     xtimerout;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

