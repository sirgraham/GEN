//------------------------------------------------------------------------------------------
//	XFSMACHINE.CPP
//	
//	Finite State Machine (Generic)
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 09/01/2004 11:26:02
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------
	
#include <stdarg.h>

#include "XFSMachine.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XFSMACHINESTATE::XFSMACHINESTATE
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:30:20
//	
//	@return				
//	@param				stateID : 
//  @param				ntransitions : 
*/
//-------------------------------------------------------------------
XFSMACHINESTATE::XFSMACHINESTATE(int stateID, int ntransitions)
{
	Clean();

  
	this->maxtransitions = ntransitions?ntransitions:1;
	this->stateID				 = stateID;

	inputs						   = NULL;
	outputstates			   = NULL;
	
  inputs = new int[this->maxtransitions];
	if(!inputs) return;

	outputstates = new int[this->maxtransitions];
	if(!outputstates) 
		{
		  delete[] inputs;
		  return;
		}

	for(int c=0; c<this->maxtransitions;c++)
		{
			inputs[c]			  = 0;
			outputstates[c] = 0;
		}

}



//-------------------------------------------------------------------
//  XFSMACHINESTATE::~XFSMACHINESTATE
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:30:49
//	
//	@return				
//	*/
//-------------------------------------------------------------------
XFSMACHINESTATE::~XFSMACHINESTATE()
{
	delete[] inputs;
	delete[] outputstates;

	Clean();
}


//-------------------------------------------------------------------
//  XFSMACHINESTATE::GetID
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:33:07
//	
//	@return				int : 
//	*/
//-------------------------------------------------------------------
int XFSMACHINESTATE::GetID()	
{
	return stateID;
}



//-------------------------------------------------------------------
//  XFSMACHINESTATE::AddTransition
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:33:30
//	
//	@return				bool : 
//	@param				input : 
//  @param				outputID : 
*/
//-------------------------------------------------------------------
bool XFSMACHINESTATE::AddTransition(int input,int outputID)
{
	int c;

	for(c=0;c<maxtransitions;c++)	
		if(!outputstates[c]) break;
		
	if(c<maxtransitions)
		{
			outputstates[c] = outputID;
			inputs[c]	 		  = input;

			return true;
		}

	return false;	
}


//-------------------------------------------------------------------
//  XFSMACHINESTATE::GetOutput
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:33:52
//	
//	@return				int : 
//	@param				input : 
*/
//-------------------------------------------------------------------
int XFSMACHINESTATE::GetOutput(int input)
{
	int outputID = stateID;	
	
	for(int c=0;c<maxtransitions;c++)
		{		
			if(!outputstates[c]) break;
		
			if(input==inputs[c])
				{
					outputID = outputstates[c];
					break;
				}
		}
	
	return(outputID);
}


//-------------------------------------------------------------------
//  XFSMACHINESTATE::DeleteTransition
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:34:07
//	
//	@return				bool : 
//	@param				outputID : 
*/
//-------------------------------------------------------------------
bool XFSMACHINESTATE::DeleteTransition(int outputID)
{
	int c;

	for(c=0;c<maxtransitions;c++)
		if(outputstates[c]==outputID) break;
	
	if(c>=maxtransitions) return false;

	inputs[c]			  = 0; 
	outputstates[c] = 0;

	for(;c<(maxtransitions-1);c++)
		{
			if(!outputstates[c]) break;

			inputs[c]		    = inputs[c+1];
			outputstates[c] = outputstates[c+1];
		}
	
	inputs[c]			  = 0; 
	outputstates[c] = 0;

	return true;
}



//-------------------------------------------------------------------
//  XFSMACHINESTATE::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:34:21
//	
//	@return				void : 
//	*/
//-------------------------------------------------------------------
void XFSMACHINESTATE::Clean()
{
	stateID				 = 0;
	maxtransitions = 0;

	inputs				 = NULL;
	outputstates   = NULL;
}




//-------------------------------------------------------------------
//  XFSMACHINE::XFSMACHINE
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:46:57
//	
//	@return				
//	@param				stateID : 
*/
//-------------------------------------------------------------------
XFSMACHINE::XFSMACHINE(int stateID)
{
	Clean();

	SetCurrentState(stateID); 
}


//-------------------------------------------------------------------
//  XFSMACHINE::~XFSMACHINE
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:47:18
//	
//	@return				
//	*/
//-------------------------------------------------------------------
XFSMACHINE::~XFSMACHINE()
{
	DeleteAllStates();

	Clean();
}


//-------------------------------------------------------------------
//  XFSMACHINE::GetEvent
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/01/2004 17:04:43
//	
//	@return				int : 
//	*/
//-------------------------------------------------------------------
int XFSMACHINE::GetEvent()
{ 
	return fsmevent; 
};


//-------------------------------------------------------------------
//  XFSMACHINE::SetEvent
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/01/2004 17:05:18
//	
//	@return				bool : 
//	@param				event : 
*/
//-------------------------------------------------------------------
bool XFSMACHINE::SetEvent(int event)			
{ 
	fsmevent = event; 

	return true; 
};




//-------------------------------------------------------------------
//  XFSMACHINE::GetCurrentState
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:48:04
//	
//	@return				int : 
//	*/
//-------------------------------------------------------------------
int XFSMACHINE::GetCurrentState()
{
	return currentstate;					
}


//-------------------------------------------------------------------
//  XFSMACHINE::SetCurrentState
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:48:17
//	
//	@return				void : 
//	@param				stateID : 
*/
//-------------------------------------------------------------------
void XFSMACHINE::SetCurrentState(int stateID)
{
	currentstate = stateID;				
}


//-------------------------------------------------------------------
//  XFSMACHINE::GetState
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:48:37
//	
//	@return				XFSMACHINESTATE* : 
//	@param				stateID : 
*/
//-------------------------------------------------------------------
XFSMACHINESTATE* XFSMACHINE::GetState(int stateID)
{
	XFSMACHINESTATE* state = NULL;
	if(map.IsEmpty()) return state;

	int index = map.Find(stateID);
	if(index==-1) return state;

	state = map.GetElement(index);

	return state;  
}


//-------------------------------------------------------------------
//  XFSMACHINE::AddState
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:48:56
//	
//	@return				bool : 
//	@param				state : 
*/
//-------------------------------------------------------------------
bool XFSMACHINE::AddState(XFSMACHINESTATE* state)
{
	if(GetState(state->GetID())) return false;

	map.Add(state->GetID(), state);

	return true;
}



//-------------------------------------------------------------------
//  XFSMACHINE::AddState
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/02/2004 12:50:07
//	
//	@return				bool : 
//	@param				state : 
//  @param				event : 
//  @param				tostate : 
//  @param				... : 
*/
//-------------------------------------------------------------------
bool XFSMACHINE::AddState(int state,int event,int tostate,...)
{
	XFSMACHINESTATE* _state;
	int							 narg = 0;
	va_list					 arg;

	va_start(arg, tostate);
  while(va_arg(arg, int)!=EVENTDEFEND)
   {
		 narg++;   
   }
  va_end(arg);        

	if((narg)&&(narg%2)) return false;
	
	narg+=2;

	_state = new XFSMACHINESTATE(state,(narg/2));
	if(!_state) return false;

	_state->AddTransition(event,tostate);	

	va_start(arg,tostate);

	int _event;	
	int _tostate;

	do { _event   = va_arg(arg, int);	
			 if(_event!=EVENTDEFEND)
				{
					_tostate = va_arg(arg, int);	
				  _state->AddTransition(_event,_tostate);				    
				}

		 } while(_event!=EVENTDEFEND);

	va_end(arg);

	if(!AddState(_state))		
		{ 
			delete _state;
			return false;
		}	

	return true;
}



/*-------------------------------------------------------------------
//  XFSMACHINE::AddSecuencialStates
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/11/2011 16:12:33
//	
//	@return				bool : 
//	@param				state : 
//  @param				maxsecuencialtransations : 
*/
/*-----------------------------------------------------------------*/
bool XFSMACHINE::AddSecuencialStates(int state,int maxsecuencialtransitions)
{
	XFSMACHINESTATE* _state;
	
	_state = new XFSMACHINESTATE(state,maxsecuencialtransitions);
	if(!_state) return false;

	for(int c=0;c<maxsecuencialtransitions;c++)
		{
			_state->AddTransition(c,c);	
		}

	if(!AddState(_state))
		{ 
			delete _state;
			return false;
		}	
	
	return true;
}





//-------------------------------------------------------------------
//  XFSMACHINE::DeleteState
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:49:08
//	
//	@return				bool : 
//	@param				stateID : 
*/
//-------------------------------------------------------------------
bool XFSMACHINE::DeleteState(int stateID)
{
	XFSMACHINESTATE* state = GetState(stateID);

	if(!state) return false;

	map.Delete(stateID);

	delete state;

	return true;
}


//-------------------------------------------------------------------
//  XFSMACHINE::DeleteAllStates
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/02/2004 15:28:35
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool XFSMACHINE::DeleteAllStates()
{
	if(map.IsEmpty()) return false;

	map.DeleteElementContents();	
	map.DeleteAll();

	return true;
}


//-------------------------------------------------------------------
//  XFSMACHINE::StateTransition
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:49:26
//	
//	@return				int : 
//	@param				input : 
*/
//-------------------------------------------------------------------
int XFSMACHINE::StateTransition(int input)
{
	XFSMACHINESTATE*	state = GetState(currentstate);

	if(!state)
		{
			currentstate = 0;
			return currentstate;
		}

	currentstate = state->GetOutput(input);
	
	return currentstate;
}



//-------------------------------------------------------------------
//  XFSMACHINE::CheckTransition
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/08/2004 12:26:26
//	
//	@return				bool : 
//	@param				event : 
*/
//-------------------------------------------------------------------
bool XFSMACHINE::CheckTransition(int& event)
{	
	int state = GetCurrentState(); 

	int tevent = (int)GetEvent();

	StateTransition(GetEvent());	

	SetEvent(0); 

	if(GetCurrentState()==state) return false;

	event = tevent;

	return true;
}


//-------------------------------------------------------------------
//  XFSMACHINE::CheckTransition
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/08/2004 12:28:34
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool XFSMACHINE::CheckTransition()
{	
	int state = GetCurrentState(); 

	StateTransition(GetEvent());	

	SetEvent(0); 

	if(GetCurrentState()==state) return false;

	return true;
}



//-------------------------------------------------------------------
//  XFSMACHINE::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			09/01/2004 11:49:42
//	
//	@return				void : 
//	*/
//-------------------------------------------------------------------
void XFSMACHINE::Clean()
{
	fsmevent		 = 0;
	currentstate = 0;
}



