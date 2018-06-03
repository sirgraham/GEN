//------------------------------------------------------------------------------------------
//  DIOMODBUSELECTRICMETER.CPP
//
//  DIO Mod Bus Electric Meter class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 13/05/2002
//  Last Mofificacion :
//
//  GEN (C) Copyright  (All right reserved)
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "XDateTime.h"
#include "XTimer.h"
#include "XFactory.h"
#include "XFSMachine.h"
#include "XBuffer.h"
#include "XDebugTrace.h"

#include "DIOStream.h"

#include "DIOModbusElectricMeter.h"

#include "XMemory.h"


//---- GENERAL VARIABLE -------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETERXEVENT::DIOMODBUSELECTRICMETERXEVENT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      16/06/2011 22:04:20
//
//  @return
//  @param        subject :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
DIOMODBUSELECTRICMETERXEVENT::DIOMODBUSELECTRICMETERXEVENT(XSUBJECT* subject,XDWORD type) : XEVENT(subject,type)
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETERXEVENT::~DIOMODBUSELECTRICMETERXEVENT
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
DIOMODBUSELECTRICMETERXEVENT::~DIOMODBUSELECTRICMETERXEVENT()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETERXEVENT::Clean
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
void DIOMODBUSELECTRICMETERXEVENT::Clean()
{

}




/*-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETER::DIOMODBUSELECTRICMETER
*/
/**
//
//  Class Constructor DIOMODBUSELECTRICMETER
//
//  @author       Abraham J. Velez
//  @version      26/12/2014 16:51:54
//
//  @param        diostream :
//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
DIOMODBUSELECTRICMETER::DIOMODBUSELECTRICMETER(DIOSTREAM* diostream)
{
  Clean();

  modbusprotocol = new DIOMODBUS(diostream);
  if(modbusprotocol)
    {
      RegisterEvent(DIOMODBUSELECTRICMETERXEVENTTYPE_C_TR_AA_READVALUES);
      RegisterEvent(DIOMODBUSELECTRICMETERXEVENTTYPE_C_CI_NU_2_READVALUE);

      if(xfactory) xtimer = xfactory->CreateTimer();
    }
}



//-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETER::~DIOMODBUSELECTRICMETER
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/12/2001 21:53:15
//
//  @return
//  */
//-------------------------------------------------------------------
DIOMODBUSELECTRICMETER::~DIOMODBUSELECTRICMETER()
{
  if(xfactory) xfactory->DeleteTimer(xtimer);

  Disconnect();

  DeRegisterEvent(DIOMODBUSELECTRICMETERXEVENTTYPE_C_TR_AA_READVALUES);
  DeRegisterEvent(DIOMODBUSELECTRICMETERXEVENTTYPE_C_CI_NU_2_READVALUE);

  delete modbusprotocol;

  Clean();
}




/*-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETER::GetModBusProtocol
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/06/2013 23:03:45
//
//  @return       DIOMODBUS* :
//  */
/*-----------------------------------------------------------------*/
DIOMODBUS* DIOMODBUSELECTRICMETER::GetModBusProtocol()
{
  return modbusprotocol;
}



/*-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETER::Connect
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/06/2013 22:02:50
//
//  @return       bool :
//  @param        unit :
//  @param        inlittleendian :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOMODBUSELECTRICMETER::Connect(XBYTE unit, bool inlittleendian, int timeout)
{
  if(!modbusprotocol)                   return false;
  if(!modbusprotocol->GetDIOStream())   return false;

  if(modbusprotocol->GetDIOStream()->GetConnectStatus()!=DIOSTREAMSTATUS_DISCONNECTED) return false;

  if(!modbusprotocol->Connect(unit,timeout)) return false;

  this->unit            = unit;
  this->inlittleendian  = inlittleendian;

  canceloperations      = false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETER::CMD_C_CI_NU_2_ReadAllValues
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
bool DIOMODBUSELECTRICMETER::CMD_C_CI_NU_2_ReadAllValues(int TM, XDATETIME* timestart, XDATETIME* timeend, XVECTOR<DIO_C_CI_NU_2_RESULT*>* results, int timeout)
{
  if(!results) return false;
  if(!modbusprotocol) return false;

  XBUFFER answer;
  float   fresult = 0.0f;
  int     iresult = 0;
  bool    status;

  status = modbusprotocol->ReadHoldingRegisters(50812, 14, answer, timeout);
  if(status)
    {
      DIO_C_CI_NU_2_RESULT* result;

      result = new DIO_C_CI_NU_2_RESULT();
      if(result)
        {
          result->GetDateTime()->Read();
          result->GetDateTime()->SetSeconds(0);

          switch(TM)
            {
              case 1  : for(int c=0; c<8; c++)
                          {
                            result->GetIndexValues()->Add(c+1);
                            result->GetValues()->Add(0);
                          }

                        ReadRegister(50780, 2, timeout, iresult);
                        result->GetIndexValues()->Add(1);
                        result->GetValues()->Add(iresult);

                        ReadRegister(50782, 2, timeout, iresult);
                        result->GetIndexValues()->Add(2);
                        result->GetValues()->Add(iresult);

                        break;


              case 2  : ReadRegister(51046, 2, 100, timeout, fresult);
                        result->GetIndexValues()->Add(1);
                        result->GetValues()->Add((int)fresult);

                        for(int c=1; c<8; c++)
                          {
                            result->GetIndexValues()->Add(c+1);
                            result->GetValues()->Add(0);
                          }

                        break;

              default : status = false;
                        break;

            }


          results->Add(result);

        } else status = false;

    }

  return status;
}




/*-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETER::CMD_C_CI_NU_2_DeleteResults
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
bool DIOMODBUSELECTRICMETER::CMD_C_CI_NU_2_DeleteResults(XVECTOR<DIO_C_CI_NU_2_RESULT*>* results)
{
  if(!results) return false;

  if(results->IsEmpty()) return false;

  results->DeleteContents();
  results->DeleteAll();

  return true;
}




/*-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETER::CMD_C_TR_AA_ReadValues
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
bool DIOMODBUSELECTRICMETER::CMD_C_TR_AA_ReadValues(DIO_C_TR_AA_RESULT* result, int timeout)
{
  if(!result)         return false;
  if(!modbusprotocol) return false;

  float fresult = 0.0f;
  int   iresult = 0;

  // Totalizadores Energeticos
  ReadRegister(50780, 2, timeout, iresult);           result->SetActiveImport(iresult);                       // Activa Importacion
                                                      result->SetActiveExport(0);                             // Activa Exportacion

                                                      result->SetReactiveQ(1, 0);                             // Reactiva Q1
                                                      result->SetReactiveQ(2, 0);                             // Reactiva Q2
                                                      result->SetReactiveQ(3, 0);                             // Reactiva Q3
                                                      result->SetReactiveQ(4, 0);                             // Reactiva Q4

  // Potencias Instantaneas
  ReadRegister(50536, 2,  100, timeout, fresult);     result->SetTotalActivePower((int)fresult);              // Potencia Activa    Total
  ReadRegister(50538, 2,  100, timeout, fresult);     result->SetTotalReactivePower((int)fresult);            // Potencia Reactiva  Total
  ReadRegister(50542, 2, 1000, timeout, fresult);     result->SetTotalPowerFactor(fresult);                   // Factor de Potencia Total


  ReadRegister(50544, 2,  100, timeout, fresult);     result->SetActivePowerPhase(1, (int)(fresult));         // Potencia Activa    Fase I
  ReadRegister(50550, 2,  100, timeout, fresult);     result->SetReactivePowerPhase(1, (int)(fresult));       // Potencia Reactiva  Fase I
  ReadRegister(50562, 2, 1000, timeout, fresult);     result->SetPowerFactorPhase(1, fresult);                // Factor de Potencia Fase I

  ReadRegister(50546, 2,  100, timeout, fresult);     result->SetActivePowerPhase(2, (int)(fresult));         // Potencia Activa    Fase II
  ReadRegister(50552, 2,  100, timeout, fresult);     result->SetReactivePowerPhase(2, (int)(fresult));       // Potencia Reactiva  Fase II
  ReadRegister(50564, 2, 1000, timeout, fresult);     result->SetPowerFactorPhase(2, fresult);                // Factor de Potencia Fase II

  ReadRegister(50548, 2,  100, timeout, fresult);     result->SetActivePowerPhase(3, (int)(fresult));         // Potencia Activa    Fase III
  ReadRegister(50554, 2,  100, timeout, fresult);     result->SetReactivePowerPhase(3, (int)(fresult));       // Potencia Reactiva  Fase III
  ReadRegister(50566, 2, 1000, timeout, fresult);     result->SetPowerFactorPhase(3, fresult);                // Factor de Potencia Fase III


  // Valores Secundarios
  ReadRegister(50520, 2,  100, timeout, fresult);     result->SetVoltagePhase(1, fresult);                    // Tension    Fase I
  ReadRegister(50528, 2, 1000, timeout, fresult);     result->SetCurrentPhase(1, fresult);                    // Intensidad Fase I

  ReadRegister(50522, 2,  100, timeout, fresult);     result->SetVoltagePhase(2, fresult);                    // Tension    Fase II
  ReadRegister(50530, 2, 1000, timeout, fresult);     result->SetCurrentPhase(2, fresult);                    // Intensidad Fase II

  ReadRegister(50524, 2,  100, timeout, fresult);     result->SetVoltagePhase(3, fresult);                    // Tension    Fase III
  ReadRegister(50532, 2, 1000, timeout, fresult);     result->SetCurrentPhase(3, fresult);                    // Intensidad Fase III


  return true;
}




/*-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETER::CancelOperations
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
bool DIOMODBUSELECTRICMETER::CancelOperations()
{
  canceloperations = true;
  return true;
}



//-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETER::Disconnect
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/12/2001 15:59:02
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOMODBUSELECTRICMETER::Disconnect()
{
  if(!modbusprotocol) return;

  modbusprotocol->Disconnect();
}



//-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETER::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/12/2001 21:50:13
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOMODBUSELECTRICMETER::Clean()
{
  xtimer                  = NULL;

  unit                    = 0;
  inlittleendian          = true;

  lastactiveenergysum     = 0;
  lastreactiveenergysum   = 0;

  modbusprotocol          = NULL;

  canceloperations        = false;
}




/*-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETER::ReadRegister
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/09/2013 16:34:04
//
//  @return       bool :
//  @param        registerID :
//  @param        nwords :
//  @param        divisor :
//  @param        result :
*/
/*-----------------------------------------------------------------*/
bool DIOMODBUSELECTRICMETER::ReadRegister(int registerID, int nwords, int divisor, int timeout, float& result)
{
  XBUFFER   answer;
  XDWORD    dword = 0;
  XWORD     word  = 0;
  int       data  = 0;

  result = 0.0f;

  if(!modbusprotocol->ReadHoldingRegisters(registerID, nwords, answer, timeout)) return false;

  if(nwords == 2)
    {
      answer.Get(dword);
      data = (int)dword;
    }

  if(nwords == 1)
    {
      answer.Get(word);
      data = (int)word;
    }

  result = ((float)data / divisor);

  return true;
}




/*-------------------------------------------------------------------
//  DIOMODBUSELECTRICMETER::ReadRegister
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/09/2013 18:00:22
//
//  @return       bool :
//  @param        registerID :
//  @param        nwords :
//  @param        timeout :
//  @param        result :
*/
/*-----------------------------------------------------------------*/
bool DIOMODBUSELECTRICMETER::ReadRegister(int registerID, int nwords, int timeout, int& result)
{
  XBUFFER   answer;
  XDWORD    dword = 0;
  XWORD     word  = 0;

  result = 0;

  if(!modbusprotocol->ReadHoldingRegisters(registerID, nwords, answer, timeout)) return false;

  if(nwords == 2)
    {
      answer.Get(dword);
      result = (int)dword;
    }

  if(nwords == 1)
    {
      answer.Get(word);
      result = (int)word;
    }

  return true;
}