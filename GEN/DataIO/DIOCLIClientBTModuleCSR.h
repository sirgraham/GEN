//------------------------------------------------------------------------------------------
//  DIOCLICLIENT.H
//
/**
// \class
//
//  ATCOMMAND
//
//  @author  Abraham J. Velez
//  @version 13/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOCLICLIENTBTMODULECSR_H_
#define _DIOCLICLIENTBTMODULECSR_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOCLIClient.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOCLICLIENT_TYPECOMMAND
{
  DIOCLICLIENT_TYPECOMMAND_UNKNOWN            = 0 ,

  DIOCLICLIENT_TYPECOMMAND_VERSION              ,
  DIOCLICLIENT_TYPECOMMAND_RSSI                 ,
  DIOCLICLIENT_TYPECOMMAND_GETCFGIOD            ,
  DIOCLICLIENT_TYPECOMMAND_SETCFGIOD            ,
  DIOCLICLIENT_TYPECOMMAND_GETIOD               ,
  DIOCLICLIENT_TYPECOMMAND_SETIOD               ,
  DIOCLICLIENT_TYPECOMMAND_GETIOA1              ,
  DIOCLICLIENT_TYPECOMMAND_ISBATTERYCHARGING    ,
  DIOCLICLIENT_TYPECOMMAND_BATTERYCHARGE        ,
  DIOCLICLIENT_TYPECOMMAND_I2CMEMREAD           ,
  DIOCLICLIENT_TYPECOMMAND_I2CMEMWRITE          ,
  DIOCLICLIENT_TYPECOMMAND_I2CADC               ,
  DIOCLICLIENT_TYPECOMMAND_I2CDAC               ,

  DIOCLICLIENT_TYPECOMMAND_LAST
};


//---- CLASS -------------------------------------------------------------------------------



class DIOCLICLIENTBTMODULECSR  : public DIOCLICLIENT
{
  public:
                                DIOCLICLIENTBTMODULECSR       (DIOSTREAM* diostream);
    virtual                    ~DIOCLICLIENTBTMODULECSR       ();

    DIOCLICLIENT_ERROR          Version                       (int& version,int& subversion);

    DIOCLICLIENT_ERROR          RSSI                          (int& RSSI);

    DIOCLICLIENT_ERROR          GetCFGIOD                     (int portio,bool& isoutput);
    DIOCLICLIENT_ERROR          SetCFGIOD                     (int portio,bool isoutput);
    DIOCLICLIENT_ERROR          GetIOD                        (int portio,bool& isactive);
    DIOCLICLIENT_ERROR          SetIOD                        (int portio,bool isactive);
    DIOCLICLIENT_ERROR          GetIOA1                       (XBYTE& value);

    DIOCLICLIENT_ERROR          IsBatteryCharging             (bool& ischarging);
    DIOCLICLIENT_ERROR          BatteryCharge                 (int& mVolts);

    DIOCLICLIENT_ERROR          I2CMemRead                    (int memID,int index,XBYTE& data);
    DIOCLICLIENT_ERROR          I2CMemWrite                   (int memID,int index,XBYTE data);

    DIOCLICLIENT_ERROR          I2CADC                        (int address,int port,XBYTE& data);
    DIOCLICLIENT_ERROR          I2CDAC                        (int address,XBYTE data);

  private:

    void                        Clean                         ();
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------



#endif

