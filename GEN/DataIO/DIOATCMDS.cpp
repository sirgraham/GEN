//------------------------------------------------------------------------------------------
//	DIOATCMDS.CPP
//	
//	ATCommand Standard Class 
//
//	Author						: Abraham J. Velez
//	Date Of Creation	: 13/05/2002
//	Last Mofificacion	:	
//
//	GEN  Copyright (C).  All right reserved.		 			
//------------------------------------------------------------------------------------------
	

//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XSleep.h"
#include "XFSMachine.h"
#include "XBuffer.h"
#include "XDebug.h"
#include "XThreadCollected.h"

#include "DIOStream.h"
#include "DIOStreamUARTConfig.h"
#include "DIOStreamUART.h"

#include "DIOATCMDS.h"

#include "XMemory.h"

	
//---- GENERAL VARIABLE -------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOATCMD::DIOATCMD
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/03/2011 11:27:15
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
DIOATCMD::DIOATCMD()
{
	Clean();
}


/*-------------------------------------------------------------------
//  DIOATCMD::~DIOATCMD
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/03/2011 11:27:19
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
DIOATCMD::~DIOATCMD()
{
	Clean();
}
	

/*-------------------------------------------------------------------
//  DIOATCMD::GetType
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/03/2011 12:06:16
//	
//	@return				XDWORD : 
//	*/
/*-----------------------------------------------------------------*/
XDWORD DIOATCMD::GetType()
{
	return type;
}


/*-------------------------------------------------------------------
//  DIOATCMD::GetCommand
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/03/2011 11:43:56
//	
//	@return				XCHAR* : 
//	*/
/*-----------------------------------------------------------------*/
XCHAR* DIOATCMD::GetCommand()
{
	return command.Get();
}
		


/*-------------------------------------------------------------------
//  DIOATCMD::GetCommandString
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/08/2012 13:44:04
//	
//	@return 			XSTRING* : 
//	*/
/*-----------------------------------------------------------------*/
XSTRING* DIOATCMD::GetCommandString()
{
	return &command;
}




/*-------------------------------------------------------------------
//  DIOATCMD::Set
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/08/2012 13:00:19
//	
//	@return 			bool : 
//	@param				type : 
//  @param				command : 
//  @param				isunsolicited :  
*/
/*-----------------------------------------------------------------*/
bool DIOATCMD::Set(XDWORD type, XCHAR* command,bool isunsolicited)
{
	this->type					= type;	
	this->command				= command;
	this->isunsolicited = isunsolicited;

	return true;
}


/*-------------------------------------------------------------------
//  DIOATCMD::IsUnSolicited
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/08/2012 13:35:00
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOATCMD::IsUnSolicited()
{
	return isunsolicited;	
}





/*-------------------------------------------------------------------
//  DIOATCMD::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/03/2011 11:27:23
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
void DIOATCMD::Clean()
{
	type					= DIOATCMD_ERROR_NONE;
	isunsolicited = false;
}




//-------------------------------------------------------------------
//  DIOATCMDS::DIOATCMDS
/**
//
//	
//	@author				Abraham J. Velez
//	@version			17/04/2002 13:34:54
//	
//	@return 			void :

//	@param				diostream :
*/
//-------------------------------------------------------------------
DIOATCMDS::DIOATCMDS(DIOSTREAM* diostream) : XFSMACHINE(0)
{
  Clean();
	
	this->diostream = diostream;
	
	xtimer									= xfactory->CreateTimer();
	mutexreadwriteproccess	= xfactory->Create_Mutex(); 
	threadconnexion					= CREATEXTHREAD(XTHREADGROUPID_DIOATCMS, __L("DIOATCMDS::DIOATCMDS"),ThreadConnexion,(void*)this);
	
  AddCommand(DIOATCMD_TYPE_BASIC			, __L(""));	
	AddCommand(DIOATCMD_TYPE_CHARACTER	, __L("S"));	
	AddCommand(DIOATCMD_TYPE_RESET			, __L("Z"));	
	AddCommand(DIOATCMD_TYPE_ECHO				, __L("E"));	
	AddCommand(DIOATCMD_TYPE_VERBOSE		, __L("V"));	
	AddCommand(DIOATCMD_TYPE_BAUDRATE		, __L("+IPR"));

}



//-------------------------------------------------------------------
//  DIOATCMDS::~DIOATCMDS
/**
//
//
//	@author				Abraham J. Velez
//	@version			02/12/2001 21:53:15
//
//	@return
//	*/
//-------------------------------------------------------------------
DIOATCMDS::~DIOATCMDS()
{  	
	End();

	if(threadconnexion)
		{
			DELETEXTHREAD(XTHREADGROUPID_DIOATCMS, threadconnexion);
			threadconnexion = NULL;
		}

	if(mutexreadwriteproccess)
		{
			xfactory->Delete_Mutex(mutexreadwriteproccess);
			mutexreadwriteproccess = NULL;
		}

	if(xtimer)
		{
			xfactory->DeleteTimer(xtimer);
			xtimer = NULL;
		}

	DeleteAllSendCommand();
	DeleteAllAnswers();
	DeleteAllCommand();

  Clean();
}



/*-------------------------------------------------------------------
//  DIOATCMDS::GetDIOStream
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/07/2012 11:06:58
//	
//	@return 			DIOSTREAM* : 
//	*/
/*-----------------------------------------------------------------*/
DIOSTREAM* DIOATCMDS::GetDIOStream()
{
	return diostream;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::GetXTimer
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/07/2012 11:09:27
//	
//	@return 			XTIMER* : 
//	*/
/*-----------------------------------------------------------------*/
XTIMER* DIOATCMDS::GetXTimer()
{
	return xtimer;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::Ini
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/08/2012 13:39:17
//	
//	@return 			bool : 
//	@param				echomode : 
//  @param				verbosemode : 
//  @param				minimumbaurate : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::Ini(int timeout, bool echomode, bool verbosemode, int minimumbaurate)
{
  if(!diostream)	return false;
	
	diostream->ResetXBuffers();

	if(threadconnexion) threadconnexion->Ini();

	/*
	DIOSTREAMUARTCONFIG* diostreamcfg = (DIOSTREAMUARTCONFIG*)diostream->GetConfig();
	if(!diostreamcfg) return 0;	

	if(!diostreamcfg->GetBaudRate()) 
		{
			if(!CalculeBaudRate(timeout/2)) return false;
		}
	 else
		{			
			if(!diostream->Open()) return false;			
		}
	*/

	if(!diostream->Open()) return false;		

	if(!diostream->WaitToConnected(timeout)) return false;

	if(SetVerboseMode(verbosemode,timeout)==DIOATCMD_ERROR_NONE)
		{							
			if(SetEchoMode(echomode,timeout)==DIOATCMD_ERROR_NONE)
				{   		
					if(GetCharacter(DIOATCMD_CODECHARACTER_ESCAPE		, escapechar	, timeout) != DIOATCMD_ERROR_NONE) return false;
					if(GetCharacter(DIOATCMD_CODECHARACTER_ENDLINE	, endlinechar	, timeout) != DIOATCMD_ERROR_NONE) return false;
					if(GetCharacter(DIOATCMD_CODECHARACTER_ANSWER		, answerchar	, timeout) != DIOATCMD_ERROR_NONE) return false;
					if(GetCharacter(DIOATCMD_CODECHARACTER_DEL			, delchar			, timeout) != DIOATCMD_ERROR_NONE) return false;

					// ----------------------------------------
					// Adjust minumum baud rate.
					//
					if(diostream->GetConfig()->GetType()==DIOSTREAMTYPE_UART)
						{
							int baudrate = 0;
							if(GetBaudRate(baudrate)==DIOATCMD_ERROR_NONE)
								{
									if(baudrate<minimumbaurate)
										{
											if(SetBaudRate(minimumbaurate)!= DIOATCMD_ERROR_NONE) return false;
										}
								}
						}


					DeleteAllAnswers();

			    return AdditionalInitialization(timeout);
				}
		}

	return false;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::Basic
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/07/2012 11:37:55
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				timeout : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::Basic(int timeout)
{
	return MakeSimpleCommand(DIOATCMD_TYPE_BASIC,NULL,timeout);
}



/*-------------------------------------------------------------------
//  DIOATCMDS::GetCharacter
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/08/2012 10:48:27
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				code : 
//  @param				character : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::GetCharacter(DIOATCMD_CODECHARACTER code,XBYTE& character,int timeout)
{
	XSTRING param;

	param.Format(__L("%d?"),code);

	DIOATCMD_ERROR error = SendCommand(DIOATCMD_TYPE_CHARACTER,param.Get());
	if(error!=DIOATCMD_ERROR_NONE) return error;

	error = WaitToProccessAnswer(timeout);
	if(error!=DIOATCMD_ERROR_NONE) return error;

	character = 0;

	XSTRING* answer = GetLastAnswer(); 
	if(answer)
		{
			character = (XBYTE)answer->ConvertToInt();
			if(!character) error = DIOATCMD_ERROR_UNKNOWN;
			
			DeleteLastAnswer();
		}

	return error;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::Reset
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/07/2012 11:38:27
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				timeout : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::Reset(int timeout)
{	
	return MakeSimpleCommand(DIOATCMD_TYPE_RESET,NULL,timeout);	
}



/*-------------------------------------------------------------------
//  DIOATCMDS::SetEchoMode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/07/2012 18:05:17
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				echomode : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::SetEchoMode(bool echomode, int timeout) 
{
	DIOATCMD_ERROR error = SendCommand(DIOATCMD_TYPE_ECHO,echomode?__L("1"):__L("0"));
	if(error!=DIOATCMD_ERROR_NONE) return error;

	this->echomode = echomode;

	return WaitToProccessAnswer(timeout);
}



/*-------------------------------------------------------------------
//  DIOATCMDS::SetVerboseMode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/07/2012 18:12:32
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				 : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::SetVerboseMode(bool verbosemode  , int timeout) 
{
	DIOATCMD_ERROR error = SendCommand(DIOATCMD_TYPE_VERBOSE, verbosemode?__L("1"):__L("0"));
	if(error!=DIOATCMD_ERROR_NONE) return error;
	
	this->verbosemode = verbosemode;

	return WaitToProccessAnswer(timeout);
}




/*-------------------------------------------------------------------
//  DIOATCMDS::GetBaudRate
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/08/2012 13:01:43
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				baudrate : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::GetBaudRate(int& baudrate, int timeout) 
{
	DIOATCMD_ERROR error = MakeSimpleCommand(DIOATCMD_TYPE_BAUDRATE, __L("?"), timeout);
	if(error!=DIOATCMD_ERROR_NONE) return error;

	int index = SearchInAnswers(DIOATCMD_TYPE_BAUDRATE,timeout);
	if(index!=DIOATCMDS_ANSWERNOTFOUND)
		{
			XSTRING* answer = GetAnswer(index);
			if(answer)
				{
					XSTRING  mask;											
					
					mask = GetCommandString(DIOATCMD_TYPE_BAUDRATE);
					mask += __L(":%d");					
					answer->UnFormat(mask.Get(),&baudrate);	
				}					
			
			DeleteAnswer(index);
		}

	return error;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::SetBaudRate
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/08/2012 13:06:32
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				baudrate : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::SetBaudRate(int baudrate, int timeout) 
{
	DIOATCMD_ERROR error;
	XSTRING				 param;

	if(diostream->GetConfig()->GetType()!=DIOSTREAMTYPE_UART) return DIOATCMD_ERROR_INVALIDCMD;

	param.Format(__L("=%d"),baudrate);

	error = SendCommand(DIOATCMD_TYPE_BAUDRATE,param.Get());
	if(error!=DIOATCMD_ERROR_NONE) return error;

	error =  WaitToProccessAnswer(timeout);
	if(error!=DIOATCMD_ERROR_NONE) return error;

	DIOSTREAMUARTCONFIG* diostreamcfg = (DIOSTREAMUARTCONFIG*)diostream->GetConfig();
	if(diostreamcfg)
		{
			if(!diostream->Close()) return DIOATCMD_ERROR_INVALIDCMD;	
			
			diostreamcfg->SetBaudRate(baudrate);		
			
			if(!diostream->Open())  return DIOATCMD_ERROR_INVALIDCMD;			
		}

	return error;
}




//-------------------------------------------------------------------
//  DIOATCMDS::End
/**
//
//
//	@author				Abraham J. Velez
//	@version			02/12/2001 15:59:02
//
//	@return 			void :
//	*/
//-------------------------------------------------------------------
void DIOATCMDS::End()
{
	if(threadconnexion) threadconnexion->End();

  if(diostream) diostream->Close();
}



/*-------------------------------------------------------------------
//  DIOATCMDS::AdditionalInitialization
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/08/2012 9:24:59
//	
//	@return 			bool : 
//	@param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::AdditionalInitialization(int timeout)
{
	return true;
}


/*-------------------------------------------------------------------
//  DIOATCMDS::UnsolicitedCommandAddtionalSize
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/08/2012 8:16:02
//	
//	@return 			bool : 
//	@param				type : 
//  @param				param : 
//  @param				typesize : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::UnsolicitedCommandAddtionalSize(XDWORD type, XSTRING& param, DIOATCMD_TYPEADDITIONALSIZE& typesize, int& size)
{
	return false;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::UnsolicitedCommands
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/08/2012 11:29:22
//	
//	@return 			bool : 
//	@param				type : 
//  @param				param : 
//  @param				additionalparamint : 
*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::UnsolicitedCommand(XDWORD type, XSTRING& param, XBUFFER& additionalparamint)
{
	return false;
}




/*-------------------------------------------------------------------
//  DIOATCMDS::CalculeBaudRateStreamUART
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/06/2013 9:49:25
//	
//	@return 			int : 
//	@param				timeout : 
*/
/*-----------------------------------------------------------------*/
int DIOATCMDS::CalculeBaudRateStreamUART(int timeout)
{
	int baudrates[] = { 115200, 9600, 57600, 38400, 19200, 4800, 2400 , 1200 };
	int baudrate    = 0;
	
	DIOSTREAMUARTCONFIG* diostreamcfg = (DIOSTREAMUARTCONFIG*)diostream->GetConfig();
	if(!diostreamcfg) return 0;	

	if(diostreamcfg->GetType() != DIOSTREAMTYPE_UART) return 0;

	for(int c=0;c<sizeof(baudrates)/sizeof(int);c++)
		{	
			diostreamcfg->SetBaudRate(baudrates[c]);

			//XDEBUG_PRINT(__L("Open connexion: %d bauds"),baudrates[c]);

			diostream->ResetXBuffers();

			diostream->Close();			
			if(!diostream->Open())  return 0;

			if(diostream->WaitToConnected(timeout))
				{
					if(threadconnexion) threadconnexion->Ini();

					echomode = false;
					if(Basic(timeout)==DIOATCMD_ERROR_NONE) baudrate = baudrates[c];
					
					echomode = true;
					if(Basic(timeout)==DIOATCMD_ERROR_NONE)  baudrate = baudrates[c];
					
					if(threadconnexion) threadconnexion->End();

					if(baudrate) break;

				}
			 
		}

	diostream->Close();	

	return baudrate;
}




/*-------------------------------------------------------------------
//  DIOATCMDS::AddCommand
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/08/2012 13:00:55
//	
//	@return 			bool : 
//	@param				type : 
//  @param				command : 
//  @param				isunsolicited : 
//  @param				nmorelines : 
*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::AddCommand(XDWORD type, XCHAR* command,bool isunsolicited)
{
	DIOATCMD* ATcommand = GetCommand(type);
	if(ATcommand) return false;

	ATcommand = new DIOATCMD();
	if(!ATcommand) return false;

	ATcommand->Set(type,command,isunsolicited);	

	ATcmds.Add(ATcommand);

	return true;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::GetCommand
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/03/2011 11:57:19
//	
//	@return				DIOATCMD* : 
//	@param				type : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD* DIOATCMDS::GetCommand(XDWORD type)
{
	DIOATCMD* ATcommand;

	for(int c=0;c<(int)ATcmds.GetSize();c++)
		{
			 ATcommand = (DIOATCMD*)ATcmds.Get(c);
			 if(ATcommand)
				 {
					 if(ATcommand->GetType() == type) return ATcommand;
				 }
		}
	
	return NULL;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::GetCommandString
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/07/2012 17:10:11
//	
//	@return 			XCHAR* : 
//	@param				type : 
*/
/*-----------------------------------------------------------------*/
XCHAR* DIOATCMDS::GetCommandString(XDWORD type)
{
	DIOATCMD* command = GetCommand(type);
	if(!command) return NULL;

	return command->GetCommand();
}



/*-------------------------------------------------------------------
//  DIOATCMDS::DeleteAllCommand
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/03/2011 11:35:30
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::DeleteAllCommand()
{
	if(ATcmds.IsEmpty()) return false;
	
	ATcmds.DeleteContents();

  ATcmds.DeleteAll();
	
	return true;
}


/*-------------------------------------------------------------------
//  DIOATCMDS::SendCommand
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/07/2012 10:14:53
//	
//	@return 			bool : 
//	@param				type : 
//  @param				nparams : 
//  @param				... : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::SendCommand(XDWORD type,XCHAR* param)
{
	DIOATCMD* ATcommand = GetCommand(type);
	if(!ATcommand) return DIOATCMD_ERROR_UNKNOWNCMD;

	XSTRING command( __L("AT"));
	command += ATcommand->GetCommand();

	if(param) command += param;

	if(echomode) AddSendCommand(&command);

	command.Add(endlinechar);

	if(mutexreadwriteproccess) mutexreadwriteproccess->Lock();
												
	XSTRING_CREATEOEM(command, charOEM)	
	bool status = diostream->WriteStr(charOEM);
	XSTRING_DELETEOEM(charOEM)	

	if(status) status = diostream->WaitToFlushOutXBuffer(10);

	
	if(mutexreadwriteproccess) mutexreadwriteproccess->UnLock();

	if(!status) return DIOATCMD_ERROR_WRITECMD;

	//XDEBUG_PRINT(__L("> %s"),command.Get());
	
	return DIOATCMD_ERROR_NONE;
}




/*-------------------------------------------------------------------
//  DIOATCMDS::SendCommand
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/07/2012 14:26:09
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				type : 
//  @param				param : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::SendCommand(XDWORD type,XSTRING& param)
{
	return DIOATCMDS::SendCommand(type,param.Get());
}



/*-------------------------------------------------------------------
//  DIOATCMDS::SendParam
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/07/2012 14:22:20
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				param : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::SendParam(XCHAR* param)
{	
	XSTRING _param;
	
	_param = param;

	if(_param.IsEmpty()) return DIOATCMD_ERROR_NONE;

	if(echomode) AddSendCommand(&_param);

	if(mutexreadwriteproccess) mutexreadwriteproccess->Lock();

	XSTRING_CREATEOEM(_param, charOEM)	
	bool status = diostream->WriteStr(charOEM);
	XSTRING_DELETEOEM(charOEM)	

	if(mutexreadwriteproccess) mutexreadwriteproccess->UnLock();

	if(!status) return DIOATCMD_ERROR_WRITECMD;

	//XDEBUG_PRINT(__L("> %s"),_param.Get());
	
	return DIOATCMD_ERROR_NONE;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::SendParam
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/07/2012 10:24:59
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				xbuffer : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::SendParam(XBUFFER& xbuffer)
{
	if(!xbuffer.GetSize())  return DIOATCMD_ERROR_NONE;

	if(echomode) AddSendCommand(xbuffer);

	if(mutexreadwriteproccess) mutexreadwriteproccess->Lock();

	XDWORD status = diostream->Write(xbuffer);
	
	if(mutexreadwriteproccess) mutexreadwriteproccess->UnLock();

	if(!status) return DIOATCMD_ERROR_WRITECMD;

	
	/*
	#ifdef XDEBUG
	XSTRING string;

	for(int c=0;c<(int)xbuffer.GetSize();c++)
		{
			string.Add((XBYTE)xbuffer.GetByte(c));
		}

	XDEBUG_PRINT(__L("1< %s"),string.Get());
	#endif
	*/
	
	return DIOATCMD_ERROR_NONE;
}




/*-------------------------------------------------------------------
//  DIOATCMDS::SendParam
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/07/2012 14:27:02
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				param : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::SendParam(XSTRING& param)
{
	return DIOATCMDS::SendParam(param.Get());
}



/*-------------------------------------------------------------------
//  DIOATCMDS::AddSendCommand
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/08/2012 14:55:17
//	
//	@return 			bool : 
//	@param				sendcommand : 
*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::AddSendCommand(XSTRING* sendcommand)
{
	if(!sendcommand)					 return false;
	if(sendcommand->IsEmpty()) return false;

	XSTRING* sc = new XSTRING();
	if(!sc) return false;

	for(XDWORD c=0;c<sendcommand->GetSize();c++)
		{
			XCHAR character = sendcommand->Get()[c];
			if((character!=endlinechar) && (character!=answerchar))  sc->Add(character);									
		}				

	sendcommands.Add(sc);

	return true;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::AddSendCommand
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/08/2012 8:24:59
//	
//	@return 			bool : 
//	@param				xbuffer : 
*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::AddSendCommand(XBUFFER& xbuffer)
{
  XSTRING param;

	param.Set(xbuffer.Get(),xbuffer.GetSize());

	return AddSendCommand(&param);
}



/*-------------------------------------------------------------------
//  DIOATCMDS::GetSendCommand
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/08/2012 14:53:38
//	
//	@return 			XSTRING* : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
XSTRING* DIOATCMDS::GetSendCommand(int index)
{
	XSTRING* sendcommand = (XSTRING*)sendcommands.Get(index);
	if(!sendcommand) return NULL;

	return sendcommand;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::SearchSendCommand
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/08/2012 15:01:56
//	
//	@return 			int : 
//	@param				sendcommand : 
*/
/*-----------------------------------------------------------------*/
int DIOATCMDS::SearchSendCommand(XSTRING* sendcommand)
{
	if(!sendcommand) return DIOATCMDS_COMMANDNOTFOUND;

	for(int c=0;c<(int)sendcommands.GetSize();c++)
		{
			XSTRING* sc = (XSTRING*)sendcommands.Get(c);
			if(sc)
				{
					if(!sc->Compare((*sendcommand))) return c;					
				}
		}

	return DIOATCMDS_COMMANDNOTFOUND;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::DeleteSendCommand
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/08/2012 14:54:56
//	
//	@return 			bool : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::DeleteSendCommand(int index)
{
	XSTRING* sendcommand = (XSTRING*)sendcommands.Get(index);
	if(!sendcommand) return false;

	delete sendcommand;
	sendcommands.DeleteIndex(index);

	return true;
}




/*-------------------------------------------------------------------
//  DIOATCMDS::DeleteAllSendCommand
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			03/08/2012 14:50:54
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::DeleteAllSendCommand()
{
	if(sendcommands.IsEmpty()) return false;
	
	sendcommands.DeleteContents();

  sendcommands.DeleteAll();
	
	return true;
}




/*-------------------------------------------------------------------
//  DIOATCMDS::ReadAnswerUntilCRLF
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/07/2012 8:44:22
//	
//	@return 			bool : 
//	@param				answer : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::ReadAnswer(XSTRING& answer,int timeout)
{
	if(!diostream) return false;
	if(!xtimer)		 return false;
	
	XBUFFER* bufferread = diostream->GetInXBuffer();
	XDWORD	 index			= 0;
	XBYTE		 data;
	bool		 status			= true;

	if(!bufferread) return false;
	
	answer.Empty();

	xtimer->Reset();

	do{ if(bufferread->GetSize()>index)
				{			
					if(bufferread->Get(data,index))
						{
							if(data==endlinechar)
								{
									index++;

									if(bufferread->GetSize()>index)
										{	
											if(bufferread->Get(data,index))
												{
													if(data==answerchar) index++;
												}
										}

									break;

								} else index++;
						}
				}

  		if((int)xtimer->GetMeasureSeconds()>=timeout)
				{
					status = false;
					break; 

				} else xsleep->MilliSeconds(10);     		
	
		} while(1);

	if(status)
		{
			for(XDWORD c=0;c<index;c++)
				{
					XBYTE byte;
					int   br;

					br = diostream->Read((XBYTE*)&byte,1);
					if(br)
						{
							if((byte!=endlinechar) && (byte!=answerchar))  answer.Add(byte);					
						}
				}

			/*
			#ifdef XDEBUG
			if(!answer.IsEmpty()) XDEBUG_PRINT(__L("2< %s"),answer.Get());
			#endif
			*/
			
		}

	return status;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::ReadAnswerWithOutEndChar
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/08/2012 12:03:57
//	
//	@return 			bool : 
//	@param				answer : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::ReadAnswerWithOutEndChar(XSTRING& answer, int timeout)
{
	if(!diostream)			 return false;
	if(!xtimer)					 return false;
	if(answer.IsEmpty()) return false;
	
	XBUFFER* bufferread = diostream->GetInXBuffer();
	XBYTE		 data;
	int			 index			= 0;
	bool		 status			= true;
	int			 c;

	if(!bufferread) return false;

	xtimer->Reset();

	do{ if(bufferread->GetSize())
				{
					if(bufferread->GetSize()>=answer.GetSize()) 
						{
							for(c=0;c<(int)bufferread->GetSize();c++)
								{
									if(bufferread->Get(data,c))
										{
											if(data==answer[c]) index++; else break;	
											
										}
								}
							
							if(index == answer.GetSize()) break;
						}
				}

			if((int)xtimer->GetMeasureSeconds()>=timeout)
				{
					status = false;
					break; 

				} else xsleep->MilliSeconds(10);     		
	
		} while(1);

	if(!status)										return false;
	if(index != answer.GetSize()) return false;

	for(c=0;c<index;c++)
		{
			XBYTE byte;
			int   br;

			br = diostream->Read((XBYTE*)&byte,1);
			if(!br)	status = false;						
		}
	
	/*
	#ifdef XDEBUG
	if(!answer.IsEmpty()) XDEBUG_PRINT(__L("3< %s"),answer.Get());
	#endif
	*/
	
	return status;
}




/*-------------------------------------------------------------------
//  DIOATCMDS::ReadAnswerWithOutEndChar
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			24/08/2012 12:04:04
//	
//	@return 			bool : 
//	@param				answer : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::ReadAnswerWithOutEndChar(XCHAR* answer, int timeout)
{ 
	XSTRING _answer;

	_answer = answer;

	return ReadAnswerWithOutEndChar(_answer,timeout);	
}




/*-------------------------------------------------------------------
//  DIOATCMDS::GetAnswer
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/07/2012 10:53:32
//	
//	@return 			XSTRING* : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
XSTRING* DIOATCMDS::GetAnswer(int index)
{
	XSTRING* answer = (XSTRING*)answers.Get(index);
	if(!answer) return NULL;

	return answer;
}


/*-------------------------------------------------------------------
//  DIOATCMDS::GetLastAnswer
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/07/2012 12:55:37
//	
//	@return 			XSTRING* : 
//	*/
/*-----------------------------------------------------------------*/
XSTRING* DIOATCMDS::GetLastAnswer()
{
	XSTRING* answer = (XSTRING*)answers.GetLast();
	return answer;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::DeleteAnswer
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/07/2012 10:53:38
//	
//	@return 			bool : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::DeleteAnswer(int index)
{
	XSTRING* answer = (XSTRING*)answers.Get(index);
	if(!answer) return false;

	delete answer;
	answers.DeleteIndex(index);

	return true;
}


/*-------------------------------------------------------------------
//  DIOATCMDS::DeleteLastAnswer
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/07/2012 13:03:14
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::DeleteLastAnswer()
{
	XSTRING* answer = (XSTRING*)answers.GetLast();
	if(!answer) return false;	

	int index = answers.GetSize();
	if(!index) return false;

	index--;
	
	delete answer;
	answers.DeleteIndex(index);

	return true;

}


/*-------------------------------------------------------------------
//  DIOATCMDS::DeleteAllAnswers
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/07/2012 10:48:13
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::DeleteAllAnswers()
{
	if(answers.IsEmpty()) return false;

	answers.DeleteContents();

  answers.DeleteAll();
	
	return true;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::GetAnswers
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/07/2012 12:58:53
//	
//	@return 			XVECTOR* : 
//	*/
/*-----------------------------------------------------------------*/
XVECTOR<XSTRING*>* DIOATCMDS::GetAnswers()
{
	return &answers;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::SearchInAnswers
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/07/2012 18:19:34
//	
//	@return 			int : 
//	@param				string : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
int DIOATCMDS::SearchInAnswers(XCHAR* string, int timeout)
{
	if(timeout) xtimer->Reset();

	while(1)
		{
			for(int c=0;c<(int)answers.GetSize();c++)
				{	
					XSTRING* answer = (XSTRING*)answers.Get(c);
					if(answer)
						{
							if(answer->Find(string,true)!=XSTRING_NOTFOUND) return c;					
						}
				}

			if(timeout)
				{
					if((int)xtimer->GetMeasureSeconds()>=timeout)
						{
							return -1;

						} else xsleep->MilliSeconds(10);   

				} else break;

		}
	
	return DIOATCMDS_ANSWERNOTFOUND;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::SearchInAnswers
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/07/2012 17:08:08
//	
//	@return 			int : 
//	@param				typecommand : 
*/
/*-----------------------------------------------------------------*/
int DIOATCMDS::SearchInAnswers(XDWORD typecommand,int timeout)
{
	DIOATCMD* command = GetCommand(typecommand);
	if(!command) return DIOATCMDS_ANSWERNOTFOUND;

	XSTRING string;

	string = command->GetCommand();
	string += __L(":");

	return SearchInAnswers(string.Get(),timeout);
}



/*-------------------------------------------------------------------
//  DIOATCMDS::WaitToProccessAnswer
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/07/2012 11:19:25
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				timeout : 
//  @param				index : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::WaitToProccessAnswer(int timeout, int* index)
{
	if(!xtimer) return DIOATCMD_ERROR_UNKNOWN;

	xtimer->Reset();

	do{ for(int c=0;c<(int)answers.GetSize();c++)
				{	
					XSTRING* answer = (XSTRING*)answers.Get(c);
					if(answer)
						{
							if((!answer->Compare(__L("OK")))  || (!answer->Compare(__L("0"))))		 
								{
									if(index) (*index) = c;
									DeleteAnswer(c);
									return DIOATCMD_ERROR_NONE;
								}

							if((!answer->Compare(__L("ERROR")))  || (!answer->Compare(__L("1"))))		 
								{
									if(index) (*index) = c;
									DeleteAnswer(c);
									return DIOATCMD_ERROR_INVALIDCMD;
								}
						}					
				} 

			if((int)xtimer->GetMeasureSeconds()>=timeout)
				{
					return DIOATCMD_ERROR_TIMEOUT;

				} else xsleep->MilliSeconds(50);     			

		} while(1);

	return DIOATCMD_ERROR_UNKNOWN;
}



/*-------------------------------------------------------------------
//  DIOATCMDS::WaitToOneAnswerMore
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/08/2012 12:32:16
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				timeout : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::WaitToOneAnswerMore(int timeout)
{
	if(!xtimer) return DIOATCMD_ERROR_UNKNOWN;

	int actualnanswer = (int)answers.GetSize();

	xtimer->Reset();

	do{ if((int)xtimer->GetMeasureSeconds()>=timeout)
				{
					return DIOATCMD_ERROR_TIMEOUT;

				} else xsleep->MilliSeconds(50);     			

		} while(actualnanswer == (int)answers.GetSize());

	return DIOATCMD_ERROR_NONE;
}





/*-------------------------------------------------------------------
//  DIOATCMDS::MakeSimpleCommand
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/07/2012 11:55:07
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				type : 
//  @param				param : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::MakeSimpleCommand(XDWORD type,XCHAR* param,int timeout)
{
	DIOATCMD_ERROR error = SendCommand(type,param);
	if(error!=DIOATCMD_ERROR_NONE) return error;

	return WaitToProccessAnswer(timeout);
}




/*-------------------------------------------------------------------
//  DIOATCMDS::MakeCommandWithSimpleAnswer
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/07/2012 12:33:07
//	
//	@return 			DIOATCMD_ERROR : 
//	@param				type : 
//  @param				answer : 
//  @param				param : 
//  @param				timeout : 
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDS::MakeCommandWithSimpleAnswer(XDWORD type,XSTRING& answer,XCHAR* param,int timeout)
{
	DIOATCMD_ERROR error = SendCommand(type);
	if(error!=DIOATCMD_ERROR_NONE) return error;

	int index=0;

	error = WaitToProccessAnswer(timeout,&index);
	if(error!=DIOATCMD_ERROR_NONE) return error;

	XSTRING* _answer = GetAnswer(index-1);
	if(_answer) 
		{
			answer = (*_answer);
			DeleteAnswer(index-1);
		}

	return DIOATCMD_ERROR_NONE;
}




//-------------------------------------------------------------------
//  DIOATCMDS::Clean
/**
//
//
//	@author				Abraham J. Velez
//	@version			02/12/2001 21:50:13
//
//	@return 			void :
//	*/
//-------------------------------------------------------------------
void DIOATCMDS::Clean()
{	
	diostream								= NULL;	
	xtimer									= NULL;

	mutexreadwriteproccess	= NULL;
	threadconnexion					= NULL;

	echomode								= false;
	verbosemode							= false;

	escapechar							=  3; 
	endlinechar							= 13;
	answerchar							= 10;
	delchar									=  8;
}




/*-------------------------------------------------------------------
//  DIOATCMDS::ThreadConnexion
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			27/07/2012 9:52:03
//	
//	@return 			void : 
//	@param				data : 
*/
/*-----------------------------------------------------------------*/
void DIOATCMDS::ThreadConnexion(void* data)
{  
	DIOATCMDS* dioatcmds = (DIOATCMDS*)data;
	if(!dioatcmds) return;
	
	DIOSTREAM* diostream = dioatcmds->GetDIOStream();
	if(!diostream) return;

	if(diostream->GetConnectStatus()!=DIOSTREAMSTATUS_CONNECTED) return;
	
	XBUFFER* bufferread = diostream->GetInXBuffer();
	if(bufferread)
		{
			if(bufferread->GetSize())
				{
					if(dioatcmds->mutexreadwriteproccess) dioatcmds->mutexreadwriteproccess->Lock();

					XSTRING answer;

					if(dioatcmds->ReadAnswer(answer))
						{
							if(!answer.IsEmpty())
								{									
									if(dioatcmds->echomode)
										{
											int index = dioatcmds->SearchSendCommand(&answer);
											if(index!=DIOATCMDS_COMMANDNOTFOUND)
												{
													dioatcmds->DeleteSendCommand(index);
													if(dioatcmds->mutexreadwriteproccess) dioatcmds->mutexreadwriteproccess->UnLock();
													return;
											  }							
										}

									if(!dioatcmds->CheckUnsolicitedAnswer(answer))
										{									
											XSTRING* answerkeep = new XSTRING();
											if(answerkeep)
												{
													(*answerkeep) = answer;
													dioatcmds->answers.Add(answerkeep);
												}
										}
								}
						}

					if(dioatcmds->mutexreadwriteproccess) dioatcmds->mutexreadwriteproccess->UnLock();
				}
		}
}



/*-------------------------------------------------------------------
//  DIOATCMDS::CheckUnsolicitedAnswer
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/08/2012 12:57:37
//	
//	@return 			bool : 
//	@param				answer : 
*/
/*-----------------------------------------------------------------*/
bool DIOATCMDS::CheckUnsolicitedAnswer(XSTRING& answer)
{
	if(answer.IsEmpty())  return false;
	if(ATcmds.IsEmpty())   return false;

	for(int c=0;c<(int)ATcmds.GetSize();c++)
		{
			DIOATCMD* atcmd = (DIOATCMD*)ATcmds.Get(c);
			if(atcmd)
				{
					if(atcmd->IsUnSolicited())
						{
							int index = answer.Find(atcmd->GetCommand(),false);
							if(index!=XSTRING_NOTFOUND)
								{	
									XSTRING											 param;								
									XBUFFER											 additionalparam;																		
									DIOATCMD_TYPEADDITIONALSIZE  typesize = DIOATCMD_TYPEADDITIONALSIZE_NONE;
									int													 sizedata = 0;

									answer.Copy(index+atcmd->GetCommandString()->GetSize()+1,param);

									if(UnsolicitedCommandAddtionalSize(atcmd->GetType(),param,typesize,sizedata))
										{
											switch(typesize)
												{
													case DIOATCMD_TYPEADDITIONALSIZE_NONE		: break;	

													case DIOATCMD_TYPEADDITIONALSIZE_BYTES	: additionalparam.Resize(sizedata);
																																		diostream->Read((XBYTE*)additionalparam.Get(),sizedata);
																																		break;
												
													case DIOATCMD_TYPEADDITIONALSIZE_LINES	: { XSTRING line;
																																			int			d = 0;

																																			while(d<sizedata)
																																				{
																																					if(ReadAnswer(line)) 
																																						{
																																							if(!line.IsEmpty()) 
																																								{
																																									XSTRING_CREATEOEM(line, charOEM)	
																																									additionalparam.Add((XBYTE*)charOEM, (XDWORD)line.GetSize());
																																									XSTRING_DELETEOEM(charOEM)	

																																									d++;
																																								}
																																						}

																																				}																																			
																																		}
																																		break;
												}

											return UnsolicitedCommand(atcmd->GetType(),param,additionalparam);					

										} else return false;
								}
						}
				}
		}

	return false;
}


