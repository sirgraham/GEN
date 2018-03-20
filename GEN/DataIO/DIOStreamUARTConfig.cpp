//------------------------------------------------------------------------------------------
//	DIOSTREAMUARTCONFIG.CPP
//	
//	Data IO Stream UART Config class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include "DIOStreamUARTConfig.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	

/*-------------------------------------------------------------------
//	DIOSTREAMUARTCONFIG::DIOSTREAMUARTCONFIG
*/	
/**	
//	
//	Class Constructor DIOSTREAMUARTCONFIG
//	
//	@author				Abraham J. Velez
//	@version			29/05/2014 17:29:06
//	
 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMUARTCONFIG::DIOSTREAMUARTCONFIG() : DIOSTREAMCONFIG()
{
	Clean();

	type = DIOSTREAMTYPE_UART;
}



//-------------------------------------------------------------------
//  DIOSTREAMUARTCONFIG::~DIOSTREAMUARTCONFIG
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:18:59
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
DIOSTREAMUARTCONFIG::~DIOSTREAMUARTCONFIG()
{
	Clean();
}



/*-------------------------------------------------------------------
//	DIOSTREAMUARTCONFIG::GetToString
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			24/02/2016 9:19:10
//	
//	@return 			bool : 
//
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUARTCONFIG::GetToString(XSTRING* string)
{
	if(!string) return false;
	
	XSTRING xstringtmp;

	string->Empty();

	if(!GetLocalDeviceName()->IsEmpty())
		{
			string->Add(GetLocalDeviceName()->Get());
			string->Add(__L(","));
		}
	 else
		{
			if(!GetPort()) return false;
			string->Format(__L("%d,"), GetPort());
		}

	if(!GetBaudRate()) return false;
	xstringtmp.Format(__L("%d,"), GetBaudRate()); 
	string->Add(xstringtmp);

	switch(GetDataBits())
		{
			case DIOSTREAMUARTDATABIT_5				: string->Add(__L("5,"));			break;
			case DIOSTREAMUARTDATABIT_6				: string->Add(__L("6,"));			break;
			case DIOSTREAMUARTDATABIT_7				: string->Add(__L("7,"));			break;
			case DIOSTREAMUARTDATABIT_8				: string->Add(__L("8,"));			break;
												default					: return false;
		}

	switch(GetParity())
		{
			case DIOSTREAMUARTPARITY_NONE			: string->Add(__L("N,"));		break;
			case DIOSTREAMUARTPARITY_ODD			: string->Add(__L("O,"));		break;
			case DIOSTREAMUARTPARITY_EVEN			:	string->Add(__L("E,"));		break;
			case DIOSTREAMUARTPARITY_MARK			:	string->Add(__L("M,"));		break;
			case DIOSTREAMUARTPARITY_SPACE		:	string->Add(__L("S,"));		break;
														 default		: return false;
		}

	switch(GetFlowControl())
		{
			case DIOSTREAMUARTFLOWCONTROL_NONE : string->Add(DIOSTREAMUART_FLOWCONTROL_STR_NONE);		break;
			case DIOSTREAMUARTFLOWCONTROL_HARD : string->Add(DIOSTREAMUART_FLOWCONTROL_STR_SOFT);		break; 
			case DIOSTREAMUARTFLOWCONTROL_SOFT : string->Add(DIOSTREAMUART_FLOWCONTROL_STR_HARD);		break;
															  default	 : return false;
		}

	return true;
}





/*-------------------------------------------------------------------
//	DIOSTREAMUARTCONFIG::SetFromString
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			24/02/2016 9:19:43
//	
//	@return 			bool : 
//
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUARTCONFIG::SetFromString(XCHAR* string)
{
	if(!string) return false;

	XSTRING   _string  = string;
	XSTRING		devicename;
	int				baudrate;
	int				databits;
	XCHAR			parity;
	XSTRING		stopbits;
	XSTRING   flowcontrol;

	devicename.AdjustSize(_MAXSTR);
	stopbits.AdjustSize(_MAXSTR);
	flowcontrol.AdjustSize(_MAXSTR);

	_string.UnFormat(__L("%s,%d,%d,%c,%s,%s"), devicename.Get(), &baudrate, &databits, &parity, stopbits.Get(), flowcontrol.Get());

	devicename.AdjustSize();
	stopbits.AdjustSize();
	flowcontrol.AdjustSize();


	if(devicename.HaveOnlyNumbers()) 
		{
			SetPort(devicename.ConvertToInt());		
			if(!GetPort()) return false;
		} 
	 else GetLocalDeviceName()->Set(devicename);

	if(!baudrate) return false;
	SetBaudRate(baudrate);

	switch(databits)
		{
			case 5		: SetDataBits(DIOSTREAMUARTDATABIT_5);	break;
			case 6		: SetDataBits(DIOSTREAMUARTDATABIT_6);  break;
			case 7		: SetDataBits(DIOSTREAMUARTDATABIT_7);  break;
			case 8		: SetDataBits(DIOSTREAMUARTDATABIT_8);  break;
				default	: return false;
		}

	switch(parity)
		{
			case 'N'	: SetParity(DIOSTREAMUARTPARITY_NONE);  break;
			case 'O'	: SetParity(DIOSTREAMUARTPARITY_ODD);		break;
			case 'E'	: SetParity(DIOSTREAMUARTPARITY_EVEN);  break;
			case 'M'	: SetParity(DIOSTREAMUARTPARITY_MARK);  break;
			case 'S'	: SetParity(DIOSTREAMUARTPARITY_SPACE); break;
			 default	: return false;
		}
	
	if(!stopbits.Compare(__L("1")))
		{
			SetStopBits(DIOSTREAMUARTSTOPBITS_ONE);
		}
	 else
		{
			if(!stopbits.Compare(__L("1.5")))
				{
					SetStopBits(DIOSTREAMUARTSTOPBITS_ONEANDAHALF);
				}	
			 else
				{
					if(!stopbits.Compare(__L("2")))
						{
							SetStopBits(DIOSTREAMUARTSTOPBITS_TWO);

						} else return false;
				}	
		}

	if(!flowcontrol.Compare(DIOSTREAMUART_FLOWCONTROL_STR_NONE, true))
		{
			SetFlowControl(DIOSTREAMUARTFLOWCONTROL_NONE);
		}
	 else
		{
			if(!stopbits.Compare(DIOSTREAMUART_FLOWCONTROL_STR_SOFT) || !stopbits.Compare(__L("SOFT")))
				{
					SetFlowControl(DIOSTREAMUARTFLOWCONTROL_SOFT);
				}	
			 else
				{
					if(!stopbits.Compare(DIOSTREAMUART_FLOWCONTROL_STR_HARD) || !stopbits.Compare(__L("HARD")))
						{
							SetFlowControl(DIOSTREAMUARTFLOWCONTROL_HARD);

						} else return false;
				}	
		}

	return true;
}


