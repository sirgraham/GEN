//------------------------------------------------------------------------------------------
//  DIOMODBUSELECTRICMETER.H
//
/**
// \class
//
//  DIO Mod Bus Electric Meter class
//
//  @author  Abraham J. Velez
//  @version 13/05/2002
*/
//  GEN (C) Copyright  (All right reserved)
//------------------------------------------------------------------------------------------

#ifndef _DIOMODBUSELECTRICMETER_H_
#define _DIOMODBUSELECTRICMETER_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XDateTime.h"
#include "XTimer.h"
#include "XFSMachine.h"
#include "XEvent.h"
#include "XSubject.h"

#include "DIOIEC60870_5.h"
#include "DIOModBus.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOMODBUSELECTRICMETERXEVENTTYPE
{
  DIOMODBUSELECTRICMETERXEVENTTYPE_UNKNOW                 = XEVENTTYPE_DIOMODBUSELECTRICMETER + 1 ,
  DIOMODBUSELECTRICMETERXEVENTTYPE_C_CI_NU_2_READVALUE                                            ,
  DIOMODBUSELECTRICMETERXEVENTTYPE_C_TR_AA_READVALUES                                             ,
};

#define DIOMODBUSELECTRICMETER_CONNECTTIMEOUT            5       // Seconds
#define DIOMODBUSELECTRICMETER_READTIMEOUT              15       // Seconds



enum DIOMODBUSELECTRICMETER_REGISTERTYPE
{
  DIOMODBUSELECTRICMETER_REGISTERTYPE_UNKNOWN                     = 0 ,

  DIOMODBUSELECTRICMETER_REGISTERTYPE_ACTIVEIMPORT                    ,   // Activa Importacion
  DIOMODBUSELECTRICMETER_REGISTERTYPE_ACTIVEEXPORT                    ,   // Activa Exportacion

  DIOMODBUSELECTRICMETER_REGISTERTYPE_REACTIVEQ1                      ,   // Reactiva Q1
  DIOMODBUSELECTRICMETER_REGISTERTYPE_REACTIVEQ2                      ,   // Reactiva Q2
  DIOMODBUSELECTRICMETER_REGISTERTYPE_REACTIVEQ3                      ,   // Reactiva Q3
  DIOMODBUSELECTRICMETER_REGISTERTYPE_REACTIVEQ4                      ,   // Reactiva Q4

  DIOMODBUSELECTRICMETER_REGISTERTYPE_TOTALACTIVEPOWER                ,   // Potencia Activa Total
  DIOMODBUSELECTRICMETER_REGISTERTYPE_TOTALREACTIVEPOWER              ,   // Potencia Reactiva Total
  DIOMODBUSELECTRICMETER_REGISTERTYPE_TOTALPOWERFACTOR                ,   // Factor de Potencia Total

  DIOMODBUSELECTRICMETER_REGISTERTYPE_ACTIVEPOWERPHASEI               ,   // Potencia Activa Fase I
  DIOMODBUSELECTRICMETER_REGISTERTYPE_REACTIVEPOWERPHASEI             ,   // Potencia Reactiva Fase I
  DIOMODBUSELECTRICMETER_REGISTERTYPE_PHASEPOWERFACTORI               ,   // Factor de Potencia Fase I

  DIOMODBUSELECTRICMETER_REGISTERTYPE_ACTIVEPOWERPHASEII              ,   // Potencia Activa Fase II
  DIOMODBUSELECTRICMETER_REGISTERTYPE_REACTIVEPOWERPHASEII            ,   // Potencia Reactiva Fase II
  DIOMODBUSELECTRICMETER_REGISTERTYPE_PHASEPOWERFACTORII              ,   // Factor de Potencia Fase II

  DIOMODBUSELECTRICMETER_REGISTERTYPE_ACTIVEPOWERPHASEIII             ,   // Potencia Activa Fase III
  DIOMODBUSELECTRICMETER_REGISTERTYPE_REACTIVEPOWERPHASEIII           ,   // Potencia Reactiva Fase III
  DIOMODBUSELECTRICMETER_REGISTERTYPE_PHASEPOWERFACTORIII             ,   // Factor de Potencia Fase III

  DIOMODBUSELECTRICMETER_REGISTERTYPE_VOLTAGEPHASEI                   ,   // Tension Fase I
  DIOMODBUSELECTRICMETER_REGISTERTYPE_CURRENTPHASEI                   ,   // Intensidad Fase I

  DIOMODBUSELECTRICMETER_REGISTERTYPE_VOLTAGEPHASEII                  ,   // Tension Fase II
  DIOMODBUSELECTRICMETER_REGISTERTYPE_CURRENTPHASEII                  ,   // Intensidad Fase II

  DIOMODBUSELECTRICMETER_REGISTERTYPE_VOLTAGEPHASEIII                 ,   // Tension Fase III
  DIOMODBUSELECTRICMETER_REGISTERTYPE_CURRENTPHASEIII                 ,   // Intensidad Fase III
};




//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class DIOSTREAM;


class DIOMODBUSELECTRICMETERXEVENT : public XEVENT
{
  public:
                              DIOMODBUSELECTRICMETERXEVENT        (XSUBJECT* subject,XDWORD type = DIOMODBUSELECTRICMETERXEVENTTYPE_UNKNOW);
    virtual                  ~DIOMODBUSELECTRICMETERXEVENT        ();

  private:

    void                      Clean                               ();
};



class DIOMODBUSELECTRICMETER  : public XSUBJECT
{
  public:
                              DIOMODBUSELECTRICMETER              (DIOSTREAM* diostream);
    virtual                  ~DIOMODBUSELECTRICMETER              ();

    DIOMODBUS*                GetModBusProtocol                   ();

    bool                      Connect                             (XBYTE unit, bool inlittleendian, int timeout = DIOMODBUSELECTRICMETER_CONNECTTIMEOUT);

    bool                      CMD_C_CI_NU_2_ReadAllValues         (int TM, XDATETIME* timestart, XDATETIME* timeend, XVECTOR<DIO_C_CI_NU_2_RESULT*>* results, int timeout = DIOMODBUSELECTRICMETER_READTIMEOUT);
    bool                      CMD_C_CI_NU_2_DeleteResults         (XVECTOR<DIO_C_CI_NU_2_RESULT*>* results);

    bool                      CMD_C_TR_AA_ReadValues              (DIO_C_TR_AA_RESULT* result, int timeout = DIOMODBUSELECTRICMETER_READTIMEOUT);

    bool                      CancelOperations                    ();

    void                      Disconnect                          ();

  protected:

    XMAP<XDWORD,XDWORD>       registermap;

  private:

    void                      Clean                               ();
    bool                      ReadRegister                        (int registerID, int nwords, int divisor, int timeout, float& result);
    bool                      ReadRegister                        (int registerID, int nwords, int timeout, int& result);



    XTIMER*                   xtimer;

    XBYTE                     unit;
    bool                      inlittleendian;

    int                       lastactiveenergysum;
    int                       lastreactiveenergysum;

    DIOMODBUS*                modbusprotocol;

    bool                      canceloperations;
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------



#endif

