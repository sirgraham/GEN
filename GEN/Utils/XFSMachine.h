//------------------------------------------------------------------------------------------
//	XFSMACHINE.H
//	
/**	
// \class 
//   
//  Finite State Machine (Generic Machine)
//   
//	@author	 Abraham J. Velez
//	@version 09/01/2004 11:25:25
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _XFSMACHINE_H_
#define _XFSMACHINE_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XMap.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


#define EVENTDEFEND  -1

//---- CLASS -------------------------------------------------------------------------------
	
class XFSMACHINESTATE
{
	public:		
																		XFSMACHINESTATE								(int stateID, int ntransitions);
		virtual												 ~XFSMACHINESTATE								();

		int															GetID													();	
		bool														AddTransition									(int input,int outputID);
		int															GetOutput											(int input);
		bool														DeleteTransition							(int outputID);

	private:

		void														Clean													();

		int															stateID;
		int															maxtransitions;
		int*														inputs;
		int*														outputstates;
};


class XFSMACHINE
{
	public:
																		XFSMACHINE										(int stateID);
		virtual												 ~XFSMACHINE										();

		int															GetEvent											();
		bool														SetEvent											(int event);

		int															GetCurrentState								(); 
		void														SetCurrentState								(int stateID);

		XFSMACHINESTATE*								GetState											(int stateID);
		bool														AddState											(XFSMACHINESTATE* state);	
		bool														AddSecuencialStates						(int state,int maxsecuencialtransitions);
		bool														AddState											(int state,int event,int tostate,...);
		bool														DeleteState										(int stateID);	
		bool														DeleteAllStates								();

		int															StateTransition								(int input);	
		bool														CheckTransition								(int& event);
		bool														CheckTransition								();

	private:

		void														Clean													();

		int															fsmevent;		
		int															currentstate;
		XMAP<int, XFSMACHINESTATE*>			map;		
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif


