//------------------------------------------------------------------------------------------
//  DIOIEC60870_5.H
//
/**
// \class
//
//  IEC 60870 5 client protocol class
//
//  @author  Abraham J. Velez
//  @version 13/05/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOIEC60870_5_H_
#define _DIOIEC60870_5_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFSMachine.h"
#include "XEvent.h"
#include "XSubject.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOIEC60870_5XEVENTTYPE
{
  DIOIEC60870_5XEVENTTYPE_UNKNOWN                   = XEVENTTYPE_DIOIEC60870_5  ,
  DIOIEC60870_5XEVENTTYPE_C_AC_NA_2_INISESSION                                  ,
  DIOIEC60870_5XEVENTTYPE_C_CI_NU_2_READVALUE                                   ,
  DIOIEC60870_5XEVENTTYPE_C_TR_AA_READVALUES                                    ,
  DIOIEC60870_5XEVENTTYPE_C_FS_NA_2_ENDSESSION
};

#define DIOIEC60870_5_CONNECTTIMEOUT                            5        // Seconds
#define DIOIEC60870_5_READTIMEOUT                               15       // Seconds

#define DIOIEC60870_5_SIZEMSGFIX                                6        // Bytes

#define DIOIEC60870_5_SIZEMAXTIMELABELTYPEA                     5        // Bytes
#define DIOIEC60870_5_SIZEMAXTIMELABELTYPEB                     7        // Bytes

#define DIOIEC60870_5_MARKSTARTFIX                              0x10
#define DIOIEC60870_5_MARKSTARTVAR                              0x68
#define DIOIEC60870_5_MARKEND                                   0x16
#define DIOIEC60870_5_MAXSIZE                                   256

#define DIOIEC60870_5_CONTROLDIRPRIMARY                         0x40
#define DIOIEC60870_5_CONTROLFCB                                0x20
#define DIOIEC60870_5_CONTROLFCV                                0x10

#define DIOIEC60870_5_MASTERFUNCRESETREMOTELINK                 0x00
#define DIOIEC60870_5_MASTERFUNCRESETUSERPROCCESS               0x01
#define DIOIEC60870_5_MASTERFUNCUSERDATA                        0x03
#define DIOIEC60870_5_MASTERFUNCREQUESTSTATUSLINK               0x09
#define DIOIEC60870_5_MASTERFUNCREQUESTUSERDATACLASS1           0x0A
#define DIOIEC60870_5_MASTERFUNCREQUESTUSERDATACLASS2           0x0B

#define DIOIEC60870_5_SLAVEFUNCCONFIRMACK                       0x00
#define DIOIEC60870_5_SLAVEFUNCNOCONFIRMACK                     0x01
#define DIOIEC60870_5_SLAVEFUNCSENDNOREPLY                      0x06
#define DIOIEC60870_5_SLAVEFUNCRESPONDDATAUSER                  0x08
#define DIOIEC60870_5_SLAVEFUNCRESPONDNOTDATAUSER               0x09
#define DIOIEC60870_5_SLAVEFUNCRESPONDSTATUSLINK                0x0B

#define DIOIEC60870_5_ASDUID_C_AC_NA_2_SESSIONINI               0xB7  // 183
#define DIOIEC60870_5_ASDUID_C_FS_NA_2_SESSIONEND               0xBB  // 187
#define DIOIEC60870_5_ASDUID_C_CI_NU_2                          0x7B  // 123
#define DIOIEC60870_5_ASDUID_C_TR_AA                            0xA2  // 162

#define DIOIEC60870_5_ASDUCAUSETRANS_INIT                       4
#define DIOIEC60870_5_ASDUCAUSETRANS_REQUESTED                  5
#define DIOIEC60870_5_ASDUCAUSETRANS_ACTIVATION                 6
#define DIOIEC60870_5_ASDUCAUSETRANS_CONFIRMACTIVATION          7
#define DIOIEC60870_5_ASDUCAUSETRANS_DISACTIVATION              8
#define DIOIEC60870_5_ASDUCAUSETRANS_CONFIRMDISACTIVATION       9
#define DIOIEC60870_5_ASDUCAUSETRANS_ENDACTIVATION              10
#define DIOIEC60870_5_ASDUCAUSETRANS_DATANOTAVAILABLE           13
#define DIOIEC60870_5_ASDUCAUSETRANS_TYPEUNKNOWN                14
#define DIOIEC60870_5_ASDUCAUSETRANS_REGISTERNUMBERUNKNOWN      15
#define DIOIEC60870_5_ASDUCAUSETRANS_ADDRESSUNKNOWN             16
#define DIOIEC60870_5_ASDUCAUSETRANS_OBJECTINFOUNKNOWN          17
#define DIOIEC60870_5_ASDUCAUSETRANS_INTEGRPEROIDNOTAVAILABLE   18


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XDATETIME;
class XTIMER;
class XPUBLISHER;
class DIOSTREAM;


class DIOIEC60870_5XEVENT : public XEVENT
{
  public:
                              DIOIEC60870_5XEVENT                 (XSUBJECT* subject, XDWORD type = DIOIEC60870_5XEVENTTYPE_UNKNOWN, XDWORD family = XEVENTTYPE_DIOIEC60870_5);
    virtual                  ~DIOIEC60870_5XEVENT                 ();

  private:

    void                      Clean                               ();
};



class DIO_C_CI_NU_2_RESULT
{
  public:
                              DIO_C_CI_NU_2_RESULT                ();
    virtual                  ~DIO_C_CI_NU_2_RESULT                ();

    XDATETIME*                GetDateTime                         ();

    XVECTOR<XDWORD>*          GetIndexValues                      ();
    XVECTOR<XDWORD>*          GetValues                           ();

  private:

    void                      Clean                               ();

    XDATETIME*                xdatetime;
    XVECTOR<XDWORD>           indexvalues;
    XVECTOR<XDWORD>           values;
};



class DIO_C_TR_AA_RESULT
{
  public:
                              DIO_C_TR_AA_RESULT                  ();
    virtual                  ~DIO_C_TR_AA_RESULT                  ();

    XDATETIME*                GetDateTime                         ();

    int                       GetActiveImport                     ();
    bool                      SetActiveImport                     (int activeimport);
    int                       GetActiveExport                     ();
    bool                      SetActiveExport                     (int activeexport);
    int                       GetReactiveQ                        (int index);
    bool                      SetReactiveQ                        (int index, int reactiveQ);

    int                       GetTotalActivePower                 ();
    bool                      SetTotalActivePower                 (int totalactivepower);
    int                       GetTotalReactivePower               ();
    bool                      SetTotalReactivePower               (int totalreactivepower);
    float                     GetTotalPowerFactor                 ();
    bool                      SetTotalPowerFactor                 (float totalpowerfactor);

    int                       GetActivePowerPhase                 (int phase);
    bool                      SetActivePowerPhase                 (int phase, int activepowerphase);
    int                       GetReactivePowerPhase               (int phase);
    bool                      SetReactivePowerPhase               (int phase, int reactivepowerphase);
    float                     GetPowerFactorPhase                 (int phase);
    bool                      SetPowerFactorPhase                 (int phase, float powerfactorphase);

    float                     GetVoltagePhase                     (int phase);
    float                     SetVoltagePhase                     (int phase, float voltagephase);
    float                     GetCurrentPhase                     (int phase);
    float                     SetCurrentPhase                     (int phase, float currentphase);

  private:

    void                      Clean                               ();

    XDATETIME*                xdatetime;

    int                       activeimport;
    int                       activeexport;
    int                       reactiveQ[4];

    int                       totalactivepower;
    int                       totalreactivepower;
    float                     totalpowerfactor;

    int                       activepowerphase[3];
    int                       reactivepowerphase[3];
    float                     powerfactorphase[3];

    float                     voltagephase[3];
    float                     currentphase[3];
};




class DIOIEC60870_5  : public XSUBJECT
{
  public:
                              DIOIEC60870_5                       (DIOSTREAM* diostream);
    virtual                  ~DIOIEC60870_5                       ();

    bool                      Connect                             (XWORD addressfield, XBYTE addressPM, XDWORD keyPM, bool inlittleendian, bool havelongaddress = true, int timeout = DIOIEC60870_5_CONNECTTIMEOUT);

    bool                      CMD_ResetRemoteLink                 (int timeout = DIOIEC60870_5_READTIMEOUT);
    bool                      CMD_RequestStatusLink               (int timeout = DIOIEC60870_5_READTIMEOUT);

    bool                      CMD_RequestUserDataClass1           (XBUFFER* asdu, int timeout = DIOIEC60870_5_READTIMEOUT);
    bool                      CMD_RequestUserDataClass2           (XBUFFER* asdu, int timeout = DIOIEC60870_5_READTIMEOUT);

    bool                      CMD_C_AC_NA_2_IniSession            (int timeout = DIOIEC60870_5_READTIMEOUT);
    bool                      CMD_C_FS_NA_2_EndSession            (int timeout = DIOIEC60870_5_READTIMEOUT);

    bool                      CMD_C_CI_NU_2_Init                  (int TM,XDATETIME& timestart, XDATETIME& timeend, int timeout = DIOIEC60870_5_READTIMEOUT);
    bool                      CMD_C_CI_NU_2_Read                  (DIO_C_CI_NU_2_RESULT* result, int timeout = DIOIEC60870_5_READTIMEOUT);
    bool                      CMD_C_CI_NU_2_ReadAllValues         (int TM,XDATETIME* timestart, XDATETIME* timeend, XVECTOR<DIO_C_CI_NU_2_RESULT*>* results, int timeout = DIOIEC60870_5_READTIMEOUT);
    bool                      CMD_C_CI_NU_2_DeleteResults         (XVECTOR<DIO_C_CI_NU_2_RESULT*>* results);

    bool                      CMD_C_TR_AA_ReadValues              (DIO_C_TR_AA_RESULT* result, int timeout = DIOIEC60870_5_READTIMEOUT);

    bool                      SetTimeLabelTypeA                   (XDATETIME& time, bool rate, bool VI, bool summer, XBYTE* result);
    bool                      SetTimeLabelTypeB                   (XDATETIME& time, bool rate, bool VI, bool summer, XBYTE* result);

    bool                      GetTimeLabelTypeA                   (XBYTE* result, bool& rate, bool& VI, bool& summer, XDATETIME& time);
    bool                      GetTimeLabelTypeB                   (XBYTE* result, bool& rate, bool& VI, bool& summer, XDATETIME& time);

    bool                      CancelOperations                    ();

    void                      Disconnect                          ();

  private:

    void                      Clean                               ()
                              {
                                diostream         = NULL;
                                xtimer            = NULL;

                                addressfield      = 0;
                                addressPM         = 0;
                                keyPM             = 0;

                                FCBstate          = true;

                                inlittleendian    = true;
                                havelongaddress   = true;

                                canceloperations  = false;
                              }

    bool                      IsValidFrameCount                   (XBYTE functioncode);

    bool                      SendMsgVar                          (XBYTE functioncode, XBUFFER* dataASDU);
    bool                      SendMsgFix                          (XBYTE functioncode);

    bool                      WaitToReadMsg                       (int size,int timeout =  DIOIEC60870_5_READTIMEOUT);
    bool                      ReadMsg                             (XBYTE& retfunctioncode, bool& retisfixmsg, XBUFFER* retASDU, int timeout =  DIOIEC60870_5_READTIMEOUT);

    XBYTE                     CalculateCheckSum                   (XBYTE* buffer, int sizebuffer);
    XBYTE                     CalculateCheckSum                   (XBUFFER& xbuffer);

    int                       GetIntFromBuffer                    (XBYTE* buffer, int size);

    DIOSTREAM*                diostream;
    XTIMER*                   xtimer;

    XWORD                     addressfield;
    XBYTE                     addressPM;
    XDWORD                    keyPM;

    bool                      FCBstate;
    bool                      inlittleendian;
    bool                      havelongaddress;

    bool                      canceloperations;
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------



#endif

