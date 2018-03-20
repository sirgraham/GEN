//------------------------------------------------------------------------------------------
//	DIOANDROIDSTREAMUDP.H
//
/**
// \class
//
//  ANDROID Data IO Stream UDP class
//
//	@author	 Abraham J. Velez
//	@version 02/01/2002
*/
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _DIOANDROIDSTREAMUDP_H_
#define _DIOANDROIDSTREAMUDP_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XFSMachine.h"
#include "XANDROIDThread.h"

#include "DIOStreamUDP.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum DIOANDROIDUDPFSMEVENTS
{
	DIOANDROIDUDPFSMEVENT_NONE							= 0 ,

	DIOANDROIDUDPFSMEVENT_GETTINGCONNEXION			,
	DIOANDROIDUDPFSMEVENT_CONNECTED							,
	DIOANDROIDUDPFSMEVENT_WAITINGTOREAD					,
	DIOANDROIDUDPFSMEVENT_SENDINGDATA						,
	DIOANDROIDUDPFSMEVENT_DISCONNECTING					,		

	DIOANDROIDUDP_LASTEVENT

};


enum DIOANDROIDUDPFSMSTATES
{
	DIOANDROIDUDPFSMSTATE_NONE							= 0 ,	

	DIOANDROIDUDPFSMSTATE_GETTINGCONNEXION			,
	DIOANDROIDUDPFSMSTATE_CONNECTED							,
	DIOANDROIDUDPFSMSTATE_WAITINGTOREAD					,
	DIOANDROIDUDPFSMSTATE_SENDINGDATA						,
	DIOANDROIDUDPFSMSTATE_DISCONNECTING					,		

	DIOANDROIDUDP_LASTSTATE
};


//---- CLASS -------------------------------------------------------------------------------

class THREAD;

class DIOANDROIDSTREAMUDP : public DIOSTREAMUDP , public XFSMACHINE
{
	public:
															DIOANDROIDSTREAMUDP											( );
		virtual									 ~DIOANDROIDSTREAMUDP											();

		bool											Open																	();
		bool											Disconnect														();
		bool											Close																	();

	protected:
		
		int 											IsReadyConnect												(int sock);
		    
	private:

		void											Clean																	();
		static void								ThreadRunFunction											(void* thread);
		
		XTHREAD*									threadconnexion;
		int 											handle; 		
		XSTRING										remoteaddress;
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif




