//------------------------------------------------------------------------------------------
//	DIOATCMDGSM.H
//	
/**	
// \class 
//   
//  ATCommand for GSM Modem
//
//	@author	 Abraham J. Velez
//	@version 13/05/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------

#ifndef _DIOATCMDGSM_H_
#define _DIOATCMDGSM_H_
	

//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOATCMDS.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum 
{
	DIOATCMD_TYPE_GSMGETMANUFACTURED					= DIOATCMD_TYPE_GSM ,
	DIOATCMD_TYPE_GSMGETMODEL  																		,
	DIOATCMD_TYPE_GSMGETSERIAL																		,
	DIOATCMD_TYPE_GSMGETVERSION																		,
	DIOATCMD_TYPE_GSMGETSIGNALQUALITY															,
	DIOATCMD_TYPE_GSMGETSERVICES																	,

	DIOATCMD_TYPE_GSMGETIMSI																			,
	DIOATCMD_TYPE_GSMGETSMSSUPPORT																,
	DIOATCMD_TYPE_GSMGETSMSFORMAT																	,
	DIOATCMD_TYPE_GSMSETSMSFORMAT																	,
	DIOATCMD_TYPE_GSMGETSMSCENTER																	,
	DIOATCMD_TYPE_GSMSETSMSCENTER																	,
	DIOATCMD_TYPE_GSMSENDSMS																			,			
};

#define DIOATCMDGSM_SMSSUPPORT_MOVILETERMINATED			0x01
#define DIOATCMDGSM_SMSSUPPORT_MOVILEORIGINATED			0x02
#define DIOATCMDGSM_SMSSUPPORT_BROADCAST						0x04

#define DIOATCMDGSM_SMSFORMAT_PDU										0x01
#define DIOATCMDGSM_SMSFORMAT_TXT										0x02


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class DIOSTREAM;



class DIOATCMDGSM_ALPHABET
{
	public:
		
		virtual XWORD						Traslate											(bool from,XWORD data) = 0;
};



class DIOATCMDGSM_ALPHABET_8859_1 : public DIOATCMDGSM_ALPHABET
{
	public:

														DIOATCMDGSM_ALPHABET_8859_1		();
		virtual								 ~DIOATCMDGSM_ALPHABET_8859_1		();
    
		virtual XWORD						Traslate											(bool from,XWORD data);

	private:
		
		void										Clean													();
		bool										CreateTable										();

		XBYTE						        table[138][2];
};



class DIOATCMDGSM  : public DIOATCMDS
{
  public:
		                        DIOATCMDGSM										( DIOSTREAM* diostream);
		virtual								 ~DIOATCMDGSM										();

		DIOATCMD_ERROR					GetManufactured								(XSTRING& manufactured, int timeout = DIOATCMDS_TIMEOUT);	
		DIOATCMD_ERROR					GetModel											(XSTRING& model, int timeout = DIOATCMDS_TIMEOUT);
		DIOATCMD_ERROR					GetIMEI												(XSTRING& IMEI, int timeout = DIOATCMDS_TIMEOUT);
		DIOATCMD_ERROR					GetVersion										(XSTRING& version, int timeout = DIOATCMDS_TIMEOUT);
		DIOATCMD_ERROR					GetSignalQuality							(int& RSSI, int& BER, int timeout = DIOATCMDS_TIMEOUT);	
		DIOATCMD_ERROR					IsAvailableServiceGSM					(bool& available, int timeout = DIOATCMDS_TIMEOUT);				
		DIOATCMD_ERROR					GetIMSI												(XSTRING& IMSI, int timeout = DIOATCMDS_TIMEOUT);		

		DIOATCMD_ERROR					GetSMSSupport									(XBYTE& support, int timeout = DIOATCMDS_TIMEOUT);	
		DIOATCMD_ERROR					GetSMSFormat									(XBYTE& format, bool support = false, int timeout = DIOATCMDS_TIMEOUT);
		DIOATCMD_ERROR					SetSMSFormat									(XBYTE format, int timeout = DIOATCMDS_TIMEOUT);
		DIOATCMD_ERROR					GetSMSCenter									(XSTRING& SMScenter, int timeout = DIOATCMDS_TIMEOUT);
		DIOATCMD_ERROR					SetSMSCenter									(XCHAR* smscenter, int timeout = DIOATCMDS_TIMEOUT);
		DIOATCMD_ERROR					SendSMS												(XCHAR* number, XCHAR* message,int* mrID = NULL, int timeout = DIOATCMDS_TIMEOUT);
    
  private:

		void										Clean													();				
		bool										CodecPDUFormat								(XCHAR* number,XCHAR* message,bool inoctets,int validperiod,XSTRING& length, XSTRING& result);
		
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------



#endif

