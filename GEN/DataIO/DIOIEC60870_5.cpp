//------------------------------------------------------------------------------------------
//  DIOIEC60870_5.CPP
//
//  IEC 60870 5 client protocol class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 13/05/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#include "XFactory.h"
#include "XSleep.h"
#include "XDateTime.h"
#include "XTimer.h"
#include "XFSMachine.h"
#include "XBuffer.h"
#include "XDebug.h"

#include "DIOStream.h"

#include "DIOIEC60870_5.h"

#include "XMemory.h"


//---- GENERAL VARIABLE -------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOIEC60870_5XEVENT::DIOIEC60870_5XEVENT
*/
/**
//
//  Class Constructor DIOIEC60870_5XEVENT
//
//  @author       Abraham J. Velez
//  @version      10/07/2015 17:32:25
//
//  @param        subject :
//  @param        type :
//  @param        family :
*/
/*-----------------------------------------------------------------*/
DIOIEC60870_5XEVENT::DIOIEC60870_5XEVENT(XSUBJECT* subject, XDWORD type, XDWORD family) : XEVENT(subject, type, family)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5XEVENT::~DIOIEC60870_5XEVENT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/06/2011 22:05:33
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOIEC60870_5XEVENT::~DIOIEC60870_5XEVENT()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5XEVENT::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/06/2011 22:07:35
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOIEC60870_5XEVENT::Clean()
{

}






/*-------------------------------------------------------------------
//  DIO_C_CI_NU_2_RESULT::DIO_C_CI_NU_2_RESULT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/06/2012 18:24:30
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIO_C_CI_NU_2_RESULT::DIO_C_CI_NU_2_RESULT()
{
  Clean();

  xdatetime = xfactory->CreateDateTime();
}



/*-------------------------------------------------------------------
//  DIO_C_CI_NU_2_RESULT::~DIO_C_CI_NU_2_RESULT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/06/2012 18:24:49
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIO_C_CI_NU_2_RESULT::~DIO_C_CI_NU_2_RESULT()
{
  xfactory->DeleteDateTime(xdatetime);

  Clean();
}



/*-------------------------------------------------------------------
//  DIO_C_CI_NU_2_RESULT::GetDateTime
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/09/2014 10:21:59
//
//  @return       XDATETIME* :
//
*/
/*-----------------------------------------------------------------*/
XDATETIME* DIO_C_CI_NU_2_RESULT::GetDateTime()
{
  return xdatetime;
}



/*-------------------------------------------------------------------
//  DIO_C_CI_NU_2_RESULT::GetIndexValues
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/06/2012 18:32:58
//
//  @return       XVECTOR<XDWORD>* :
//  */
/*-----------------------------------------------------------------*/
XVECTOR<XDWORD>* DIO_C_CI_NU_2_RESULT::GetIndexValues()
{
  return &indexvalues;
}



/*-------------------------------------------------------------------
//  DIO_C_CI_NU_2_RESULT::GetValues
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/06/2012 18:33:05
//
//  @return       XVECTOR<XDWORD>* :
//  */
/*-----------------------------------------------------------------*/
XVECTOR<XDWORD>* DIO_C_CI_NU_2_RESULT::GetValues()
{
  return &values;
}


/*-------------------------------------------------------------------
//  DIO_C_CI_NU_2_RESULT::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/06/2012 18:28:36
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIO_C_CI_NU_2_RESULT::Clean()
{
  xdatetime   = NULL;
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::DIO_C_TR_AA_RESULT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 18:40:21
//
//  @return

*/
/*-----------------------------------------------------------------*/
DIO_C_TR_AA_RESULT::DIO_C_TR_AA_RESULT()
{
  Clean();

  xdatetime = xfactory->CreateDateTime();
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::~DIO_C_TR_AA_RESULT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 18:40:42
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIO_C_TR_AA_RESULT::~DIO_C_TR_AA_RESULT()
{
  xfactory->DeleteDateTime(xdatetime);

  Clean();
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::GetDateTime
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/09/2014 10:22:32
//
//  @return       XDATETIME* :
//
*/
/*-----------------------------------------------------------------*/
XDATETIME* DIO_C_TR_AA_RESULT::GetDateTime()
{
  return xdatetime;
}


/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::GetActiveImport
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 19:41:37
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIO_C_TR_AA_RESULT::GetActiveImport()
{
  return activeimport;
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::SetActiveImport
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 19:41:53
//
//  @return       bool :
//  @param        activeimport :
*/
/*-----------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetActiveImport(int activeimport)
{
  this->activeimport = activeimport;
  return true;
}


/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::GetActiveExport
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 19:53:12
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIO_C_TR_AA_RESULT::GetActiveExport()
{
  return activeexport;
}


/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::SetActiveExport
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 19:53:53
//
//  @return       bool :
//  @param        activeexport :
*/
/*-----------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetActiveExport(int activeexport)
{
  this->activeexport = activeexport;
  return true;
}


/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::GetReactiveQ
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 19:54:22
//
//  @return       int :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
int DIO_C_TR_AA_RESULT::GetReactiveQ(int index)
{
  if((index<1)||(index>4))  return -1;

  return reactiveQ[index-1];
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::SetReactiveQ
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:21:12
//
//  @return       bool :
//  @param        index :
//  @param        reactiveQ :
*/
/*-----------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetReactiveQ(int index,int reactiveQ)
{
  if((index<1)||(index>4))  return false;
  this->reactiveQ[index-1] = reactiveQ;
  return true;
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::GetTotalActivePower
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:23:28
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIO_C_TR_AA_RESULT::GetTotalActivePower()
{
  return totalactivepower;
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::SetTotalActivePower
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:23:33
//
//  @return       bool :
//  @param        totalactivepower :
*/
/*-----------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetTotalActivePower(int totalactivepower)
{
  this->totalactivepower = totalactivepower;
  return true;
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::GetTotalReactivePower
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:23:37
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIO_C_TR_AA_RESULT::GetTotalReactivePower()
{
  return totalreactivepower;
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::SetTotalReactivePower
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:23:43
//
//  @return       bool :
//  @param        totalreactivepower :
*/
/*-----------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetTotalReactivePower(int totalreactivepower)
{
  this->totalreactivepower = totalreactivepower;
  return true;
}


/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::GetTotalPowerFactor
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:23:51
//
//  @return       float :
//  */
/*-----------------------------------------------------------------*/
float DIO_C_TR_AA_RESULT::GetTotalPowerFactor()
{
  return totalpowerfactor;
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::SetTotalPowerFactor
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:24:13
//
//  @return       bool :
//  @param        totalpowerfactor :
*/
/*-----------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetTotalPowerFactor(float totalpowerfactor)
{
  this->totalpowerfactor = totalpowerfactor;
  return true;
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::GetActivePowerPhase
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:25:55
//
//  @return       int :
//  @param        phase :
*/
/*-----------------------------------------------------------------*/
int DIO_C_TR_AA_RESULT::GetActivePowerPhase(int phase)
{
  if((phase<1)||(phase>3)) return -1;
  return activepowerphase[phase-1];
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::SetActivePowerPhase
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:27:37
//
//  @return       bool :
//  @param        phase :
//  @param        activepowerphase :
*/
/*-----------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetActivePowerPhase(int phase, int activepowerphase)
{
  if((phase<1)||(phase>3)) return false;
  this->activepowerphase[phase-1] = activepowerphase;
  return true;
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::GetReactivePowerPhase
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:28:55
//
//  @return       int :
//  @param        phase :
*/
/*-----------------------------------------------------------------*/
int DIO_C_TR_AA_RESULT::GetReactivePowerPhase(int phase)
{
  if((phase<1)||(phase>3)) return -1;
  return reactivepowerphase[phase-1];
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::SetReactivePowerPhase
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:29:28
//
//  @return       bool :
//  @param        phase :
//  @param        reactivepowerphase :
*/
/*-----------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetReactivePowerPhase(int phase, int reactivepowerphase)
{
  if((phase<1)||(phase>3)) return false;
  this->reactivepowerphase[phase-1] = reactivepowerphase;
  return true;
}


/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::GetPowerFactorPhase
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:31:36
//
//  @return       float :
//  @param        phase :
*/
/*-----------------------------------------------------------------*/
float DIO_C_TR_AA_RESULT::GetPowerFactorPhase(int phase)
{
  if((phase<1)||(phase>3)) return -1;
  return powerfactorphase[phase-1];
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::SetPowerFactorPhase
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:31:43
//
//  @return       bool :
//  @param        phase :
//  @param        powerfactorphase :
*/
/*-----------------------------------------------------------------*/
bool DIO_C_TR_AA_RESULT::SetPowerFactorPhase(int phase, float powerfactorphase)
{
  if((phase<1)||(phase>3)) return false;
  this->powerfactorphase[phase-1] = powerfactorphase;
  return true;
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::GetVoltagePhase
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:33:08
//
//  @return       float :
//  @param        phase :
*/
/*-----------------------------------------------------------------*/
float DIO_C_TR_AA_RESULT::GetVoltagePhase(int phase)
{
  if((phase<1)||(phase>3)) return -1;
  return voltagephase[phase-1];
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::SetVoltagePhase
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:33:12
//
//  @return       float :
//  @param        phase :
//  @param        voltagephase :
*/
/*-----------------------------------------------------------------*/
float DIO_C_TR_AA_RESULT::SetVoltagePhase(int phase, float voltagephase)
{
  if((phase<1)||(phase>3)) return false;
  this->voltagephase[phase-1] = voltagephase;
  return true;
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::GetCurrentPhase
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:33:16
//
//  @return       float :
//  @param        phase :
*/
/*-----------------------------------------------------------------*/
float DIO_C_TR_AA_RESULT::GetCurrentPhase(int phase)
{
  if((phase<1)||(phase>3)) return -1;
  return currentphase[phase-1];
}



/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::SetCurrentPhase
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:33:23
//
//  @return       float :
//  @param        phase :
//  @param        currentphase :
*/
/*-----------------------------------------------------------------*/
float DIO_C_TR_AA_RESULT::SetCurrentPhase(int phase, float currentphase)
{
  if((phase<1)||(phase>3)) return false;
  this->currentphase[phase-1] = currentphase;
  return true;
}




/*-------------------------------------------------------------------
//  DIO_C_TR_AA_RESULT::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 19:37:40
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIO_C_TR_AA_RESULT::Clean()
{
  xdatetime    = NULL;

  activeimport = 0;
  activeexport = 0;

  int c;

  for(c=0;c<4;c++)
    {
      reactiveQ[c] = 0;
    }

  totalactivepower    = 0;
  totalreactivepower  = 0;
  totalpowerfactor    = 0.0f;

  for(c=0;c<3;c++)
    {
      activepowerphase[c]   = 0;
      reactivepowerphase[c] = 0;
      powerfactorphase[c]   = 0.0f;

      voltagephase[c]       = 0;
      currentphase[c]       = 0;
    }
}





//-------------------------------------------------------------------
//  DIOIEC60870_5::DIOIEC60870_5
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/04/2002 13:34:54
//
//  @return       void :

//  @param        diostream :
*/
//-------------------------------------------------------------------
DIOIEC60870_5::DIOIEC60870_5(DIOSTREAM* diostream)
{
  Clean();

  this->diostream   = diostream;

  RegisterEvent(DIOIEC60870_5XEVENTTYPE_C_AC_NA_2_INISESSION);
  RegisterEvent(DIOIEC60870_5XEVENTTYPE_C_TR_AA_READVALUES);
  RegisterEvent(DIOIEC60870_5XEVENTTYPE_C_CI_NU_2_READVALUE);
  RegisterEvent(DIOIEC60870_5XEVENTTYPE_C_FS_NA_2_ENDSESSION);

  xtimer = xfactory->CreateTimer();
}



//-------------------------------------------------------------------
//  DIOIEC60870_5::~DIOIEC60870_5
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/12/2001 21:53:15
//
//  @return
//  */
//-------------------------------------------------------------------
DIOIEC60870_5::~DIOIEC60870_5()
{
  xfactory->DeleteTimer(xtimer);

  Disconnect();

  DeRegisterEvent(DIOIEC60870_5XEVENTTYPE_C_AC_NA_2_INISESSION);
  DeRegisterEvent(DIOIEC60870_5XEVENTTYPE_C_TR_AA_READVALUES);
  DeRegisterEvent(DIOIEC60870_5XEVENTTYPE_C_CI_NU_2_READVALUE);
  DeRegisterEvent(DIOIEC60870_5XEVENTTYPE_C_FS_NA_2_ENDSESSION);

  Clean();
}




/*-------------------------------------------------------------------
//  DIOIEC60870_5::Connect
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/12/2012 21:11:50
//
//  @return       bool :
//  @param        addresfield :
//  @param        addressPM :
//  @param        keyPM :
//  @param        inlittleendian :
//  @param        havelongaddress :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::Connect(XWORD addressfield,XBYTE addressPM, XDWORD keyPM, bool inlittleendian, bool havelongaddress, int timeout)
{
  if(!diostream)                                                  return false;
  //if(diostream->GetConnectStatus()!=DIOSTREAMSTATUS_DISCONNECTED) return false;

  #ifdef XDEBUG

  switch(diostream->GetConfig()->GetType())
    {
      case DIOSTREAMTYPE_UART  :  XDEBUG_PRINTCOLOR(1,__L("IEC60870_5 Connexion to UART: "));
                                  break;

      case DIOSTREAMTYPE_TCPIP :  XDEBUG_PRINTCOLOR(1,__L("IEC60870_5 Connexion to TCP/IP: "));
                                  break;

                       default :  break;
    }




  #endif


  if(!diostream->Open())  return false;

  if(!diostream->WaitToConnected(timeout)) return false;

  XDEBUG_PRINTCOLOR(1,__L("IEC60870_5 Connected physical layer."));

  this->addressfield      = addressfield;
  this->addressPM         = addressPM;
  this->keyPM             = keyPM;
  this->inlittleendian    = inlittleendian;
  this->havelongaddress   = havelongaddress;

  XDEBUG_PRINTCOLOR(1,__L("IEC60870_5 Meter Address Field: %d, Address PM: %d, Key PM: %d"), addressfield, addressPM, keyPM);


  canceloperations = false;

  return true;
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5::CMD_ResetRemoteLink
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/05/2012 10:49:50
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_ResetRemoteLink(int timeout)
{
  XBYTE retfunctioncode;
  bool retisfixmsg;

  if(!SendMsgFix(DIOIEC60870_5_MASTERFUNCRESETREMOTELINK))    return false;
  if(!ReadMsg(retfunctioncode,retisfixmsg,NULL,timeout))      return false;

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCCONFIRMACK)    return true;
  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCNOCONFIRMACK)  return false;

  return false;
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5::CMD_RequestStatusLink
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/05/2012 10:58:54
//
//  @return       bool :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_RequestStatusLink(int timeout)
{
  XBYTE retfunctioncode;
  bool retisfixmsg;

  if(!SendMsgFix(DIOIEC60870_5_MASTERFUNCREQUESTSTATUSLINK))    return false;
  if(!ReadMsg(retfunctioncode,retisfixmsg,NULL,timeout))        return false;

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCRESPONDSTATUSLINK) return true;

  return false;
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5::CMD_RequestUserDataClass1
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2012 10:10:09
//
//  @return       bool :
//  @param        asdu :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_RequestUserDataClass1(XBUFFER* asdu,int timeout)
{
  XBYTE retfunctioncode;
  bool retisfixmsg;

  if(!SendMsgFix(DIOIEC60870_5_MASTERFUNCREQUESTUSERDATACLASS1))  return false;
  if(!ReadMsg(retfunctioncode,retisfixmsg,asdu,timeout))          return false;

  if(retisfixmsg)
    {
      if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCRESPONDDATAUSER)     return true;
      if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCRESPONDNOTDATAUSER)  return false;

    } else return true;

  return false;
}




/*-------------------------------------------------------------------
//  DIOIEC60870_5::CMD_RequestUserDataClass2
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2012 10:09:24
//
//  @return       bool :
//  @param        asdu :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_RequestUserDataClass2(XBUFFER* asdu,int timeout)
{
  if(!asdu) return false;

  XBYTE retfunctioncode;
  bool retisfixmsg;

  if(!SendMsgFix(DIOIEC60870_5_MASTERFUNCREQUESTUSERDATACLASS2))  return false;
  if(!ReadMsg(retfunctioncode,retisfixmsg,asdu,timeout))          return false;

  if(retisfixmsg)
    {
      if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCRESPONDDATAUSER)     return true;
      if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCRESPONDNOTDATAUSER)  return false;

    }
   else
    {
      if(asdu->GetByte(2) == DIOIEC60870_5_ASDUCAUSETRANS_CONFIRMACTIVATION) return true;
      if(asdu->GetByte(2) == DIOIEC60870_5_ASDUCAUSETRANS_REQUESTED)         return true;
    }

  return false;
}






/*-------------------------------------------------------------------
//  DIOIEC60870_5::CMD_C_AC_NA_2_IniSession
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 12:32:41
//
//  @return       bool :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_C_AC_NA_2_IniSession(int timeout)
{
  XBYTE retfunctioncode;
  bool  retisfixmsg;

  if(!CMD_RequestStatusLink(timeout)) return false;
  if(!CMD_ResetRemoteLink(timeout))   return false;
  if(!CMD_RequestStatusLink(timeout)) return false;

  XBUFFER asdu;

  bool haveuserdataclass1 = CMD_RequestUserDataClass1(&asdu,timeout);

  asdu.Delete();

  asdu.Add((XBYTE)DIOIEC60870_5_ASDUID_C_AC_NA_2_SESSIONINI); // Identificator
  asdu.Add((XBYTE)1);                                         // Object Number
  asdu.Add((XBYTE)DIOIEC60870_5_ASDUCAUSETRANS_ACTIVATION);   // Transmision Cause
  asdu.Add((XBYTE)addressPM);                                 // Meter Point Address
  asdu.Add((XBYTE)00);                                        // Register Address  = 0
  asdu.Add((XBYTE)00);                                        // Register Address  = 0

  XDWORD _keyPM = keyPM;

  if(inlittleendian) SWAPDWORD(_keyPM);
  asdu.Add((XDWORD)_keyPM);                                   // Meter Point Key

  if(!SendMsgVar(DIOIEC60870_5_MASTERFUNCUSERDATA,&asdu))   return false;
  if(!ReadMsg(retfunctioncode,retisfixmsg,NULL,timeout))    return false;

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCCONFIRMACK)
    {
      XBUFFER retasdu;

      if(CMD_RequestUserDataClass2(&retasdu,timeout))
        {
          XBYTE* buffer = retasdu.Get();
          if(!buffer) return false;

          if(buffer[0]!=DIOIEC60870_5_ASDUID_C_AC_NA_2_SESSIONINI)      return false;
          if(buffer[2]!=DIOIEC60870_5_ASDUCAUSETRANS_CONFIRMACTIVATION) return false;

          DIOIEC60870_5XEVENT xevent(this,DIOIEC60870_5XEVENTTYPE_C_AC_NA_2_INISESSION);
          PostEvent(&xevent);

          return true;
        }

      return false;
    }

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCNOCONFIRMACK)  return false;

  return false;
}




/*-------------------------------------------------------------------
//  DIOIEC60870_5::CMD_C_FS_NA_2_EndSession
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 12:32:31
//
//  @return       bool :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_C_FS_NA_2_EndSession(int timeout)
{
  XBYTE retfunctioncode;
  bool retisfixmsg;

  XBUFFER asdu;

  asdu.Add((XBYTE)DIOIEC60870_5_ASDUID_C_FS_NA_2_SESSIONEND);  // Identificator
  asdu.Add((XBYTE)0);                                          // Object Number
  asdu.Add((XBYTE)DIOIEC60870_5_ASDUCAUSETRANS_ACTIVATION);    // Transmision Cause
  asdu.Add((XBYTE)addressPM);                                  // Meter Point Address
  asdu.Add((XWORD)0x0000);                                     // Register Address  = 0

  if(!SendMsgVar(DIOIEC60870_5_MASTERFUNCUSERDATA,&asdu))   return false;
  if(!ReadMsg(retfunctioncode,retisfixmsg,NULL,timeout))    return false;

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCCONFIRMACK)
    {
      XBUFFER retasdu;

      if(CMD_RequestUserDataClass2(&retasdu,timeout))
        {
          XBYTE* buffer = retasdu.Get();
          if(!buffer) return false;

          if(buffer[0]!=DIOIEC60870_5_ASDUID_C_FS_NA_2_SESSIONEND)      return false;
          if(buffer[2]!=DIOIEC60870_5_ASDUCAUSETRANS_CONFIRMACTIVATION) return false;

          DIOIEC60870_5XEVENT xevent(this, DIOIEC60870_5XEVENTTYPE_C_FS_NA_2_ENDSESSION);
          PostEvent(&xevent);

          return true;
        }

      return false;
    }

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCNOCONFIRMACK)  return false;

  return false;
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5::CMD_C_CI_NU_2_Init
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/10/2012 21:17:24
//
//  @return       bool :
//  @param        TM :
//  @param        timestart :
//  @param        timeend :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_C_CI_NU_2_Init(int TM,XDATETIME& timestart,XDATETIME& timeend,int timeout)
{
  XBYTE retfunctioncode;
  bool retisfixmsg;

  XBUFFER asdu;

  asdu.Add((XBYTE)DIOIEC60870_5_ASDUID_C_CI_NU_2);              // Identificator
  asdu.Add((XBYTE)01);                                          // Object Number
  asdu.Add((XBYTE)DIOIEC60870_5_ASDUCAUSETRANS_ACTIVATION);     // Transmision Cause
  asdu.Add((XBYTE)addressPM);                                   // Meter Point Address
  asdu.Add((XBYTE)0x00);                                        // Register Address
  asdu.Add((XBYTE)((TM==1)?0x0B:0x0C));                         // Register Address
  asdu.Add((XBYTE)0x01);                                        // Address of Total First
  asdu.Add((XBYTE)0x08);                                        // Address of Total Last

  XBYTE resulttime[DIOIEC60870_5_SIZEMAXTIMELABELTYPEA];

  SetTimeLabelTypeA(timestart,false,false,false,resulttime);    asdu.Add(resulttime,DIOIEC60870_5_SIZEMAXTIMELABELTYPEA);
  SetTimeLabelTypeA(timeend  ,false,false,false,resulttime);    asdu.Add(resulttime,DIOIEC60870_5_SIZEMAXTIMELABELTYPEA);

  if(!SendMsgVar(DIOIEC60870_5_MASTERFUNCUSERDATA,&asdu))   return false;
  if(!ReadMsg(retfunctioncode,retisfixmsg,NULL,timeout))    return false;

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCCONFIRMACK)
    {
      XBUFFER retasdu;
      if(!CMD_RequestUserDataClass2(&retasdu,timeout)) return false;

      return true;
    }

  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCNOCONFIRMACK)  return false;

  return false;
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5::CMD_C_CI_NU_2_Read
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 11:22:22
//
//  @return       bool :
//  @param        result :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_C_CI_NU_2_Read(DIO_C_CI_NU_2_RESULT* result,int timeout)
{
  if(!result) return false;

  XBUFFER retasdu;

  if(!CMD_RequestUserDataClass2(&retasdu,timeout)) return false;

  DIOIEC60870_5XEVENT xevent(this,DIOIEC60870_5XEVENTTYPE_C_CI_NU_2_READVALUE);
  PostEvent(&xevent);

  XBYTE* buffer = retasdu.Get();
  if(!buffer) return false;

  bool rate   = false;
  bool VI     = false;
  bool summer = false;

  GetTimeLabelTypeA(&buffer[54],rate,VI,summer,(*result->GetDateTime()));

  for(int c=6;c<(int)retasdu.GetSize()-8;c+=6)
    {
      XDWORD value = 0;

      for(int d=0;d<4;d++)
        {
          value |= (XDWORD)buffer[c+1+(3-d)];
          if(d<3) value<<=8;
        }

      result->GetIndexValues()->Add(buffer[c]);
      result->GetValues()->Add(value);
    }

  return true;
}




/*-------------------------------------------------------------------
//  DIOIEC60870_5::CMD_C_CI_NU_2_ReadAllValues
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/12/2012 21:24:12
//
//  @return       bool :
//  @param        TM :
//  @param        timestart :
//  @param        timeend :
//  @param        results :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_C_CI_NU_2_ReadAllValues(int TM, XDATETIME* timestart, XDATETIME* timeend, XVECTOR<DIO_C_CI_NU_2_RESULT*>* results, int timeout)
{
  if(!timestart) return false;
  if(!timeend)   return false;
  if(!results)   return false;

  bool status = true;

  if(CMD_C_AC_NA_2_IniSession(timeout))
    {
      if(CMD_C_CI_NU_2_Init(TM,(*timestart),(*timeend),timeout))
        {
          DIO_C_CI_NU_2_RESULT* result;
          bool                  status2 = true;

          do{ result = new DIO_C_CI_NU_2_RESULT();
              if(result)
                {
                  status2 = CMD_C_CI_NU_2_Read(result, timeout);
                  if(status2)
                    {
                      results->Add(result);
                    }
                   else delete result;

                } else status2 = false;

            } while(status2);

        } else status = false;

      if(!CMD_C_FS_NA_2_EndSession(timeout)) status = false;

    } else status = false;

  return status;
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5::CMD_C_CI_NU_2_DeleteResults
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 11:22:05
//
//  @return       bool :
//  @param        results :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_C_CI_NU_2_DeleteResults(XVECTOR<DIO_C_CI_NU_2_RESULT*>* results)
{
  if(!results) return false;

  if(results->IsEmpty()) return false;

  results->DeleteContents();
  results->DeleteAll();

  return true;
}




/*-------------------------------------------------------------------
//  DIOIEC60870_5::CMD_C_TR_AA_ReadValues
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:35:09
//
//  @return       bool :
//  @param        result :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::CMD_C_TR_AA_ReadValues(DIO_C_TR_AA_RESULT* result,int timeout)
{
  if(!result) return false;

  bool status = true;

  if(CMD_C_AC_NA_2_IniSession(timeout))
    {
      XBYTE retfunctioncode;
      bool retisfixmsg;

      XBUFFER asdu;

      asdu.Add((XBYTE)DIOIEC60870_5_ASDUID_C_TR_AA);                // Identificator
      asdu.Add((XBYTE)0x03);                                        // Object Number
      asdu.Add((XBYTE)DIOIEC60870_5_ASDUCAUSETRANS_REQUESTED);      // Transmision Cause
      asdu.Add((XBYTE)addressPM);                                   // Meter Point Address
      asdu.Add((XBYTE)0x00);                                        // Register Address
      asdu.Add((XBYTE)0x00);                                        // Register Address
      asdu.Add((XBYTE)0xC0);                                        // ????
      asdu.Add((XBYTE)0xC1);                                        // ????
      asdu.Add((XBYTE)0xC2);                                        // ????

      if(SendMsgVar(DIOIEC60870_5_MASTERFUNCUSERDATA,&asdu))
        {
          if(ReadMsg(retfunctioncode,retisfixmsg,NULL,timeout))
            {
              if(retfunctioncode!=DIOIEC60870_5_SLAVEFUNCNOCONFIRMACK)
                {
                  if(retfunctioncode==DIOIEC60870_5_SLAVEFUNCCONFIRMACK)
                    {
                      float data        = 0.0f;
                      int   index       = 7;
                      XBYTE  datasigned = 0;
                      int   c;

                      XBUFFER retasdu;
                      if(CMD_RequestUserDataClass2(&retasdu,timeout))
                        {
                          XBYTE* buffer = retasdu.Get();
                          if(buffer)
                            {
                              result->SetActiveImport(GetIntFromBuffer(&buffer[index],4));  index+=4;
                              result->SetActiveExport(GetIntFromBuffer(&buffer[index],4));  index+=4;

                              result->SetReactiveQ(1,GetIntFromBuffer(&buffer[index],4));   index+=4;
                              result->SetReactiveQ(2,GetIntFromBuffer(&buffer[index],4));   index+=4;
                              result->SetReactiveQ(3,GetIntFromBuffer(&buffer[index],4));   index+=4;
                              result->SetReactiveQ(4,GetIntFromBuffer(&buffer[index],4));   index+=4;

                              index+=6;

                              result->SetTotalActivePower  (GetIntFromBuffer(&buffer[index],3));      index+=3;
                              result->SetTotalReactivePower(GetIntFromBuffer(&buffer[index],3));      index+=3;

                              data  = (float)(GetIntFromBuffer(&buffer[index],2)&0x03FF);
                              data /= 1000.0f;
                              result->SetTotalPowerFactor(data);

                              datasigned = buffer[index+1];
                              if(datasigned&0x04) result->SetTotalActivePower  (-result->GetTotalActivePower());
                              if(datasigned&0x08) result->SetTotalReactivePower(-result->GetTotalReactivePower());

                              index+=2;

                              for(c=0;c<3;c++)
                                {
                                  result->SetActivePowerPhase(c+1   , GetIntFromBuffer(&buffer[index],3));      index+=3;
                                  result->SetReactivePowerPhase(c+1 , GetIntFromBuffer(&buffer[index],3));      index+=3;

                                  data  = (float)(GetIntFromBuffer(&buffer[index],2)&0x03FF);
                                  data /= 1000.0f;
                                  result->SetPowerFactorPhase(c+1   , data);

                                  datasigned = buffer[index+1];
                                  if(datasigned&0x04) result->SetActivePowerPhase  (c+1,-result->GetActivePowerPhase(c+1));
                                  if(datasigned&0x08) result->SetReactivePowerPhase(c+1,-result->GetReactivePowerPhase(c+1));

                                  index+=2;
                                }

                              index+=6;

                              for(c=0;c<3;c++)
                                {
                                  data = 0.0f;

                                  data = (float)GetIntFromBuffer(&buffer[index],3);
                                  data/=10.0f; index+=3;

                                  result->SetCurrentPhase(c+1 , data);

                                  data = (float)GetIntFromBuffer(&buffer[index],4);
                                  data/=10.0f; index+=4;

                                  result->SetVoltagePhase(c+1 , data);
                                }

                              bool rate   = false;
                              bool VI     = false;
                              bool summer = false;

                              GetTimeLabelTypeA(&buffer[index],rate,VI,summer,(*result->GetDateTime()));

                            } else status = false;

                        } else status = false;

                    } else status = false;

                  DIOIEC60870_5XEVENT xevent(this,DIOIEC60870_5XEVENTTYPE_C_TR_AA_READVALUES);
                  PostEvent(&xevent);

                  if(!CMD_C_FS_NA_2_EndSession(timeout)) status = false;

                } else status = false;

            } else status = false;

        } else status = false;

    } else status = false;

  return status;
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5::SetTimeLabelTypeA
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/06/2012 17:26:34
//
//  @return       bool :
//  @param        time :
//  @param        rateON :
//  @param        VI :
//  @param        ETI :
//  @param        PTI :
//  @param        result :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::SetTimeLabelTypeA(XDATETIME& time, bool rate, bool VI, bool summer,XBYTE* result)
{
  memset(result,0,DIOIEC60870_5_SIZEMAXTIMELABELTYPEA);

  result[0] = time.GetMinutes() & 0x00FF;       // add valid flag and genuine flag
  result[1] = time.GetHours()   & 0x00FF;       // add summer flag
  result[2] = ((time.GetDayOfWeek()%7)&0x03)<<5 | (time.GetDay() & 0x1F);
  result[3] = time.GetMonth()   & 0x0F;
  result[4] = time.GetYear() - 2000;

  return true;
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5::SetTimeLabelTypeB
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/06/2012 17:01:54
//
//  @return       bool :
//  @param        time :
//  @param        rateON :
//  @param        VI :
//  @param        ETI :
//  @param        PTI :
//  @param        result :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::SetTimeLabelTypeB(XDATETIME& time, bool rate, bool VI, bool summer,XBYTE* result)
{
  memset(result,0,DIOIEC60870_5_SIZEMAXTIMELABELTYPEB);

  XDWORD m = time.GetMilliSeconds() + 1000 * time.GetSeconds();

  result[0] = (m & 0xFF);
  result[1] = (m & 0xFF00)>>8;
  result[2] = time.GetMinutes() & 0x00FF;       // add valid flag and genuine flag
  result[3] = time.GetHours()   & 0x00FF;       // add summer flag
  result[4] = ((time.GetDayOfWeek()%7)&0x03)<<5 | (time.GetDay() & 0x1F);
  result[5] = time.GetMonth()   & 0x0F;
  result[6] = time.GetYear() - 2000;

  return true;
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5::GetTimeLabelTypeA
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/06/2012 17:28:00
//
//  @return       bool :
//  @param        result :
//  @param        rate :
//  @param        VI :
//  @param        summer :
//  @param        time :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::GetTimeLabelTypeA(XBYTE* result, bool& rate, bool& VI,bool& summer,XDATETIME& time)
{
  time.SetToZero();

  rate = (result[0] & 0x40)?true:false;
  VI   = (result[0] & 0x80)?true:false;

  time.SetMinutes(result[0] & 0x3F);
  time.SetHours(result[1] & 0x1F);

  summer = (result[1] & 0x80)?true:false;

  time.SetDay(result[2] & 0x1F);

  //XBYTE dayofweek = (result[2] & 0xE0 ) >> 5;
  //if(dayofweek) dayofweek = (dayofweek + 1)%7;
  //time.SetDayOfWeek((XDATETIME_DAYWEEK)dayofweek);

  time.SetMonth(result[3] & 0x0F);

  time.SetYear(2000 + (result[4] & 0x7F));

  return true;
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5::GetTimeLabelTypeB
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/06/2012 17:28:07
//
//  @return       bool :
//  @param        result :
//  @param        rate :
//  @param        VI :
//  @param        summer :
//  @param        time :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::GetTimeLabelTypeB(XBYTE* result, bool& rate, bool& VI,bool& summer,XDATETIME& time)
{
  time.SetToZero();

  XDWORD m = result[0] | result[1]<<8;

  time.SetSeconds(m/1000);
  time.SetMilliSeconds(m - (time.GetSeconds()*1000));

  rate = (result[2] & 0x40)?true:false;
  VI   = (result[2] & 0x80)?true:false;

  time.SetMinutes(result[2] & 0x3F);
  time.SetHours(result[3] & 0x1F);

  summer = (result[3] & 0x80)?true:false;

  time.SetDay(result[4] & 0x1F);

  //XBYTE dayofweek = (result[4] & 0xE0 ) >> 5;
  //if(dayofweek) dayofweek = (dayofweek + 1)%7;
  //time.SetDayOfWeek((XDATETIME_DAYWEEK)dayofweek);

  time.SetMonth(result[5] & 0x0F);

  time.SetYear(2000 + (result[6] & 0x7F));

  return true;
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5::CancelOperations
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/02/2013 18:41:53
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::CancelOperations()
{
  canceloperations = true;
  return true;
}



//-------------------------------------------------------------------
//  DIOIEC60870_5::Disconnect
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/12/2001 15:59:02
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOIEC60870_5::Disconnect()
{
  if(diostream)
    {
      diostream->Disconnect();
      diostream->Close();

      diostream = NULL;
    }
}





/*-------------------------------------------------------------------
//  DIOIEC60870_5::IsValidFrameCount
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/05/2012 16:34:49
//
//  @return       bool :
//  @param        functioncode :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::IsValidFrameCount(XBYTE functioncode)
{
  bool isFCVon = false;

  switch(functioncode)
    {
      case DIOIEC60870_5_MASTERFUNCRESETREMOTELINK        : isFCVon = false;  break;
      case DIOIEC60870_5_MASTERFUNCRESETUSERPROCCESS      : isFCVon = false;  break;
      case DIOIEC60870_5_MASTERFUNCUSERDATA               : isFCVon = true;   break;
      case DIOIEC60870_5_MASTERFUNCREQUESTSTATUSLINK      : isFCVon = false;  break;
      case DIOIEC60870_5_MASTERFUNCREQUESTUSERDATACLASS1  : isFCVon = true;   break;
      case DIOIEC60870_5_MASTERFUNCREQUESTUSERDATACLASS2  : isFCVon = true;   break;
    }

  return isFCVon;
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5::SendMsgVar
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/05/2012 16:53:55
//
//  @return       bool :
//  @param        functioncode :
//  @param        dataASDU :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::SendMsgVar(XBYTE functioncode, XBUFFER* dataASDU)
{
  if(!diostream) return false;
  if(diostream->GetConnectStatus()!=DIOSTREAMSTATUS_CONNECTED) return false;
  if(!xtimer)    return false;

  if(!dataASDU)  return false;

  XBUFFER xbuffer;
  XBYTE   controlfield = 0;
  XBYTE   checksum     = 0;

  xbuffer.Add((XBYTE)DIOIEC60870_5_MARKSTARTVAR);
  xbuffer.Add((XBYTE)0);
  xbuffer.Add((XBYTE)0);
  xbuffer.Add((XBYTE)DIOIEC60870_5_MARKSTARTVAR);

  controlfield  = 0x00;
  controlfield |= DIOIEC60870_5_CONTROLDIRPRIMARY;

  if(IsValidFrameCount(functioncode))
    {
      controlfield |= FCBstate?DIOIEC60870_5_CONTROLFCB:0x00;
      controlfield |= DIOIEC60870_5_CONTROLFCV;

      FCBstate = !FCBstate;
    }

  controlfield |= (functioncode&0x0F);
  xbuffer.Add(controlfield);

  if(!havelongaddress)
    {
      xbuffer.Add((XBYTE)addressfield);
    }
   else
    {
      XWORD _addressfield = addressfield;

      if(inlittleendian) SWAPWORD(_addressfield);
      xbuffer.Add((XWORD)_addressfield);
    }

  xbuffer.Add(dataASDU);

  xbuffer.Set((XBYTE)(dataASDU->GetSize()+3), 1);
  xbuffer.Set((XBYTE)(dataASDU->GetSize()+3), 2);

  xbuffer.Add((XBYTE)0);
  xbuffer.Add((XBYTE)DIOIEC60870_5_MARKEND);

  xbuffer.Set(CalculateCheckSum(xbuffer), (xbuffer.GetSize()-2));

  if(!diostream->Write(xbuffer.Get(),xbuffer.GetSize())) return false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOIEC60870_5::SendMsgFix
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      25/05/2012 18:10:42
//
//  @return       bool :
//  @param        functioncode :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::SendMsgFix(XBYTE functioncode)
{
  if(!diostream) return false;
  if(diostream->GetConnectStatus()!=DIOSTREAMSTATUS_CONNECTED) return false;
  if(!xtimer)    return false;

  XBUFFER  xbuffer;
  XBYTE    controlfield = 0;
  XBYTE    checksum     = 0;

  xbuffer.Add((XBYTE)DIOIEC60870_5_MARKSTARTFIX);

  controlfield  = 0x00;
  controlfield |= DIOIEC60870_5_CONTROLDIRPRIMARY;

  if(IsValidFrameCount(functioncode))
    {
      controlfield |= FCBstate?DIOIEC60870_5_CONTROLFCB:0x00;
      controlfield |= DIOIEC60870_5_CONTROLFCV;

      FCBstate = !FCBstate;
    }

  controlfield |= (functioncode&0x0F);
  xbuffer.Add(controlfield);

  if(!havelongaddress)
    {
      xbuffer.Add((XBYTE)addressfield);
    }
   else
    {
      XWORD _addressfield = addressfield;

      if(inlittleendian) SWAPWORD(_addressfield);
      xbuffer.Add((XWORD)_addressfield);
    }

  xbuffer.Add((XBYTE)0); // Temporal CheckSUM

  xbuffer.Add((XBYTE)DIOIEC60870_5_MARKEND);

  xbuffer.Get()[4] = CalculateCheckSum(xbuffer);

  if(!diostream->Write(xbuffer.Get(),xbuffer.GetSize())) return false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOIEC60870_5::WaitToReadMsg
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/05/2012 21:42:24
//
//  @return       bool :
//  @param        size :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::WaitToReadMsg(int size,int timeout)
{
  int actualsize = 0;

  if(!xtimer) return false;
  xtimer->Reset();

  do{
      if(canceloperations)                                         return false;

      if(!diostream)                                               return false;
      if(diostream->GetConnectStatus()!=DIOSTREAMSTATUS_CONNECTED) return false;

      actualsize = diostream->GetInXBuffer()->GetSize();
      if((int)xtimer->GetMeasureSeconds() >= timeout) return false;

      xsleep->MilliSeconds(10);

      if(actualsize>=size) return true;

    } while(!actualsize);

  if(!actualsize) return false;

  return true;
}





/*-------------------------------------------------------------------
//  DIOIEC60870_5::ReadMsg
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/05/2012 17:02:24
//
//  @return       bool :
//  @param        retfunctioncode :
//  @param        retisfixmsg :
//  @param        dataASDU :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOIEC60870_5::ReadMsg(XBYTE& retfunctioncode,bool& retisfixmsg,XBUFFER* retASDU,int timeout)
{
  if(!diostream) return false;
  if(diostream->GetConnectStatus()!=DIOSTREAMSTATUS_CONNECTED) return false;
  if(!xtimer)    return false;

  bool status  = false;

  XBUFFER xbuffer((XDWORD)DIOIEC60870_5_MAXSIZE, true);

  if(!WaitToReadMsg(DIOIEC60870_5_MAXSIZE,timeout))
    {
      return false;
    }

  int indexbufferread = 0;

  xtimer->Reset();

  while(1)
    {
      if(canceloperations) return false;

      if((int)xtimer->GetMeasureSeconds()>=timeout) return false;

      if((indexbufferread+DIOIEC60870_5_SIZEMSGFIX)>(int)diostream->GetInXBuffer()->GetSize()) break;

      XBYTE* buffer = &(diostream->GetInXBuffer()->Get()[indexbufferread]);

      switch(buffer[0])
        {
          case DIOIEC60870_5_MARKSTARTFIX : { XBYTE checksum = CalculateCheckSum(buffer,DIOIEC60870_5_SIZEMSGFIX);
                                              if(checksum!=buffer[4])  break;

                                              XWORD _addressfield;

                                              _addressfield   = buffer[2];
                                              _addressfield <<= 8;
                                              _addressfield  |= buffer[3];

                                              if(inlittleendian) SWAPWORD(_addressfield);

                                              if(_addressfield != addressfield)
                                                {
                                                  indexbufferread += DIOIEC60870_5_SIZEMSGFIX;
                                                  continue;
                                                }

                                              XBYTE controlfield = buffer[1];
                                              if(controlfield&DIOIEC60870_5_CONTROLDIRPRIMARY)  break;

                                              retfunctioncode   = (controlfield&0x0f);
                                              retisfixmsg       = true;
                                              diostream->GetInXBuffer()->Extract(NULL,indexbufferread,DIOIEC60870_5_SIZEMSGFIX);
                                            }

                                            status = true;
                                            break;

          case DIOIEC60870_5_MARKSTARTVAR : { XBYTE sizeasdu  = buffer[1];
                                              XBYTE checksum = CalculateCheckSum(buffer,sizeasdu+4+2);
                                              if(checksum!=buffer[sizeasdu+4])  break;

                                              XWORD _addressfield;

                                              _addressfield   = buffer[5];
                                              _addressfield <<= 8;
                                              _addressfield  |= buffer[6];

                                              if(inlittleendian) SWAPWORD(_addressfield);

                                              if(_addressfield != addressfield)
                                                {
                                                  indexbufferread += DIOIEC60870_5_SIZEMSGFIX;
                                                  continue;
                                                }

                                              XBYTE controlfield = buffer[4];
                                              if(controlfield&DIOIEC60870_5_CONTROLDIRPRIMARY) break;

                                              retfunctioncode   = (controlfield&0x0f);
                                              retisfixmsg       = false;

                                              if(retASDU) retASDU->Add(&buffer[7],sizeasdu-3);

                                              diostream->GetInXBuffer()->Extract(NULL,indexbufferread,sizeasdu+4+2);
                                            }

                                            status = true;
                                            break;
        }
    }

  return status;
}




/*-------------------------------------------------------------------
//  DIOIEC60870_5::CalculateCheckSum
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/05/2012 22:44:24
//
//  @return       XBYTE :
//  @param        buffer :
//  @param        sizebuffer :
*/
/*-----------------------------------------------------------------*/
XBYTE DIOIEC60870_5::CalculateCheckSum(XBYTE* buffer,int sizebuffer)
{
  if(!buffer)       return 0;

  XBYTE csum    = 0;
  int  start  = 0;
  int  size   = 0;

  switch(buffer[0])
    {
      case DIOIEC60870_5_MARKSTARTFIX : if(buffer[4]==DIOIEC60870_5_MARKEND)
                                          {
                                            start = 1;
                                            size  = 2;
                                          }

                                        if(buffer[5]==DIOIEC60870_5_MARKEND)
                                          {
                                            start = 1;
                                            size  = 3;
                                          }
                                        break;

      case DIOIEC60870_5_MARKSTARTVAR : if(buffer[3]==DIOIEC60870_5_MARKSTARTVAR)
                                          {
                                            start = 4;
                                            size = sizebuffer-start-2;
                                          }
                                        break;
    }


  for(int c=0;c<size;c++)
    {
      csum += buffer[start+c];
    }

  return csum;
}



/*-------------------------------------------------------------------
//  DIOIEC60870_5::CalculateCheckSum
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/05/2012 22:19:13
//
//  @return       XBYTE :
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
XBYTE DIOIEC60870_5::CalculateCheckSum(XBUFFER& xbuffer)
{
  return CalculateCheckSum(xbuffer.Get(),xbuffer.GetSize());
}




/*-------------------------------------------------------------------
//  DIOIEC60870_5::GetIntFromBuffer
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/06/2012 20:39:27
//
//  @return       int :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
int DIOIEC60870_5::GetIntFromBuffer(XBYTE* buffer,int size)
{
  XDWORD value = 0;

  for(int d=0;d<size;d++)
    {
      value |= (XDWORD)buffer[((size-1)-d)];
      if(d<(size-1)) value<<=8;
    }

  return (int)value;
}