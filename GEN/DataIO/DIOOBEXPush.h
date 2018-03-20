//------------------------------------------------------------------------------------------
//	DIOOBEXPUSH.H
//	
/**	
// \class 
//   
//  Send Method
//   
//	@author	 Abraham J. Velez
//	@version 14/09/2004 11:59:59
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _DIOOBEXPUSH_H_
#define _DIOOBEXPUSH_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include "XBase.h"
#include "XString.h"
#include "XPath.h"
#include "XFSMachine.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum DIOOBEXPUSHERROR
{	
	DIOOBEXPUSHERROR_NONE									= 0 ,
	DIOOBEXPUSHERROR_NOTCONNEXION							,
	DIOOBEXPUSHERROR_REFUSEDCONNEXION					,
	DIOOBEXPUSHERROR_SENDING								  ,
};


enum DIOOBEXPUSHXFSMEVENTS
{
	DIOOBEXPUSH_XFSMEVENT_NONE						= 0 ,
	DIOOBEXPUSH_XFSMEVENT_CONNECT							,	
	DIOOBEXPUSH_XFSMEVENT_INIPUSH							,	
	DIOOBEXPUSH_XFSMEVENT_SENDHEADPUSH				,
	DIOOBEXPUSH_XFSMEVENT_RECVHEADPUSH				,
	DIOOBEXPUSH_XFSMEVENT_SENDBLOCKPUSH				,
	DIOOBEXPUSH_XFSMEVENT_RECVBLOCKPUSH				,
	DIOOBEXPUSH_XFSMEVENT_ENDPUSH							,	
	DIOOBEXPUSH_XFSMEVENT_DISCONNECT					,

	DIOOBEXPUSH_LASTEVENT
};


enum DIOOBEXPUSHXFSMSTATES
{
	DIOOBEXPUSH_XFSMSTATE_NONE						= 0 ,
	DIOOBEXPUSH_XFSMSTATE_CONNECT							,
	DIOOBEXPUSH_XFSMSTATE_INIPUSH							,
	DIOOBEXPUSH_XFSMSTATE_SENDHEADPUSH				,
	DIOOBEXPUSH_XFSMSTATE_RECVHEADPUSH				,
	DIOOBEXPUSH_XFSMSTATE_SENDBLOCKPUSH				,
	DIOOBEXPUSH_XFSMSTATE_RECVBLOCKPUSH				,
	DIOOBEXPUSH_XFSMSTATE_ENDPUSH							,
	DIOOBEXPUSH_XFSMSTATE_DISCONNECT					,

	DIOOBEXPUSH_LASTSTATE
};


#define DIOOBEXPUSH_MAXBUFFER				65534	  // Minimo evitar problemas en linux 38400 
#define DIOOBEXPUSH_MAXTIMEOUTASK		30		  

#define DIOOBEXPUSH_MAXANSWER				32
	
//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XBUFFER;
class XTHREADCOLLECTED;
class XTIMER;
class DIOSTREAM;
class BTIDDEVICE;


class DIOOBEXPUSHFILE
{
	public:
																		DIOOBEXPUSHFILE										();
		virtual												 ~DIOOBEXPUSHFILE										();
		
		XDWORD													GetID															();
		XPATH*													GetXPath													();
		XSTRING*											  GetNameSend												();		
		bool														IsSend														();
		XBYTE														GetSendPercent										();
		XQWORD													GetSendTimeElapsed								();
		DIOOBEXPUSHERROR							  GetError													();

		bool														SetID															(XDWORD ID);
		bool														SetIsSend													(bool issend);	
		bool														SetSendPercent										(XBYTE percent);
		bool														SetSendTimeElapsed								(XQWORD sendtimeelapsed);
		bool														SetError													(DIOOBEXPUSHERROR error);

	private:

		void														Clean															();		

		XDWORD													ID;
		XPATH														xpath;
		XSTRING													namesend;
		bool														sendis;
		XBYTE														sendpercent;
		XQWORD													sendtimeelapsed;
		DIOOBEXPUSHERROR							  error;
		
};



class DIOOBEXPUSH : public XFSMACHINE
{
	public:	
																		DIOOBEXPUSH												(DIOSTREAM* diostream, bool inlittleendian);
		virtual												 ~DIOOBEXPUSH												();
	
		bool														Ini																(int timeout);

		bool														IsSending													();
		bool														SetIsSending											(bool issending);
		    
		bool														SendFile													(XPATH& xpath,XSTRING& namesend,XDWORD ID = 0,int asktimeout = DIOOBEXPUSH_MAXTIMEOUTASK);
	  bool														SendFiles													(int asktimeout = DIOOBEXPUSH_MAXTIMEOUTASK);

		XTIMER*													GetXTimer													();
		XQWORD													GetAskTimeElapsed									();

		bool														SendFileList_AddFile							(XPATH& xpath,XSTRING& namesend,XDWORD ID = 0);
		DIOOBEXPUSHFILE*								SendFileList_GetFile							(int index);
		DIOOBEXPUSHFILE*								SendFileList_GetActualFile				();
		bool														SendFileList_DeleteAll						();		
		int															SendFileList_GetIndex							();
		int															SendFileList_GetSize							();
							 
		bool														End																();

		DIOSTREAM*											GetDIOStream											();
		
		XPATH*													GetFileNameSend										();

	  XDWORD													GenerateFileID										(XPATH& xpath);

	private:

		void														Clean															();		

		bool														CheckSending											();
		bool														ReadAnswer												(XBUFFER& xbuffer);
		static void											ThreadSend												(void* data);		
		
		DIOSTREAM*											diostream;
		bool														inlittleendian;

		XTIMER*													xtimer;
		int															asktimeout;
		XQWORD													asktimeelapsed;
				
		XVECTOR<DIOOBEXPUSHFILE*> 			sendfilelist;
		int															indexfilelist;		

		XPATH														xpath;
		XFILE*													xfile;

		XBUFFER*												sendbuffer;
		
		bool														issending;
		
		int															sendblocksize;
		bool														sendisstart;
				
		int															filesize;
		int 														readfilesize;
		int 														readfilesizeblock;
		
		bool														haveerror;		
		
		XTHREADCOLLECTED*								xthreadobex;

};


	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	

#endif