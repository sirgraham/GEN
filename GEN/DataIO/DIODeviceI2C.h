/*------------------------------------------------------------------------------------------
//  DIOI2CDEVICE.H
*/
/**
// \class
//
//  DIO I2C Device
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 24/09/2014 8:49:43
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOI2CDEVICE_H_
#define _DIOI2CDEVICE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"

#include "DIODevice.h"
#include "DIOStreamI2CConfig.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


class DIODEVICEI2C : public DIODEVICE
{
  public:

                              DIODEVICEI2C          ()
                              {
                                Clean();

                                xtimerout = xfactory->CreateTimer();
                              }


    virtual                  ~DIODEVICEI2C          ()
                              {
                                if(xtimerout) xfactory->DeleteTimer(xtimerout);

                                if(diostreamcfg) delete diostreamcfg;

                                Clean();
                              }

    using DIODEVICE::Ini;
    bool                      Ini                   (XCHAR* localdevicename, int remotedeviceaddress, int timeout)
                              {
                                if(!xtimerout) return false;

                                this->timeout = timeout;

                                diostreamcfg = CreateConfig(localdevicename, remotedeviceaddress);
                                if(!diostreamcfg)  return false;

                                return IniDevice();
                              }

    bool                      Ini                   (int port, int remotedeviceaddress, int timeout)
                              {
                                if(!xtimerout) return false;

                                this->timeout = timeout;

                                diostreamcfg = CreateConfig(port, remotedeviceaddress);
                                if(!diostreamcfg)  return false;

                                return IniDevice();
                              }

    DIOSTREAMI2CCONFIG*       CreateConfig          (XCHAR* localdevicename, int remotedeviceaddress)
                              {
                                DIOSTREAMI2CCONFIG* diostreamcfg = new DIOSTREAMI2CCONFIG();
                                if(!diostreamcfg)  return NULL;

                                diostreamcfg->GetLocalDeviceName()->Set(localdevicename);

                                diostreamcfg->SetRemoteAddress(remotedeviceaddress);

                                return diostreamcfg;
                              }

    DIOSTREAMI2CCONFIG*       CreateConfig          (int port, int remotedeviceaddress)
                              {
                                DIOSTREAMI2CCONFIG* diostreamcfg = new DIOSTREAMI2CCONFIG();
                                if(!diostreamcfg)  return NULL;

                                diostreamcfg->SetPort(port);

                                diostreamcfg->SetLocalDeviceNameByPort();

                                diostreamcfg->SetRemoteAddress(remotedeviceaddress);

                                return diostreamcfg;
                              }

    virtual bool              IniDevice            ()                                             =  0;


  protected:

    int                       timeout;
    XTIMER*                   xtimerout;
    DIOSTREAMI2CCONFIG*       diostreamcfg;

  private:

    void                      Clean                 ()
                              {
                                timeout       = 0;
                                xtimerout     = NULL;
                                diostreamcfg  = NULL;
                              }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

