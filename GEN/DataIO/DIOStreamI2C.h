/*------------------------------------------------------------------------------------------
//  DIOSTREAMI2C.H
*/  
/** 
// \class 
//   
//  Data IO Stream I2C class
//   
//  @author  Abraham J. Velez
//  @version 20/09/2012 14:31:53
*/  
/*  GEN  Copyright (C).  All right reserved.		
//----------------------------------------------------------------------------------------*/
  
#ifndef _DIOSTREAMI2C_H_
#define _DIOSTREAMI2C_H_
  
  
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFSMachine.h"

#include "DIOStream.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


enum  DIOSTREAMI2C_FSMEVENTS
{
	DIOSTREAMI2C_FSMEVENT_NONE								 = 0  ,
	DIOSTREAMI2C_FSMEVENT_CONNECTED									,
	DIOSTREAMI2C_FSMEVENT_WAITINGTOREAD							,	
	DIOSTREAMI2C_FSMEVENT_SENDINGDATA								,
	DIOSTREAMI2C_FSMEVENT_DISCONNECTING							,			

  DIOSTREAMI2C_LASTEVENT
};

enum  DIOSTREAMI2C_FSMSTATES
{
	DIOSTREAMI2C_FSMSTATE_NONE							  = 0  ,	

	DIOSTREAMI2C_FSMSTATE_CONNECTED								 ,
	DIOSTREAMI2C_FSMSTATE_WAITINGTOREAD						 ,
	DIOSTREAMI2C_FSMSTATE_SENDINGDATA							 ,
	DIOSTREAMI2C_FSMSTATE_DISCONNECTING						 ,				

	DIOSTREAMI2C_LASTSTATE
};



/*---- CLASS -----------------------------------------------------------------------------*/
  
class XBUFFER;
class XFACTORY;
class XTHREADCOLLECTED;
class DIOGPIO;
class DIOSTREAMI2CCONFIG;



class DIOSTREAMI2C : public DIOSTREAM , public XFSMACHINE 
{
	public:
																	DIOSTREAMI2C															();
		virtual											 ~DIOSTREAMI2C															();		

		DIOSTREAMCONFIG*							GetConfig																	();
		bool									        SetConfig																	(DIOSTREAMCONFIG* config);	
     				
		virtual bool									Open																			();
    
    virtual XDWORD							  ReadDirect										            (XBYTE* buffer, XDWORD size)  { return 0;                  }
		virtual XDWORD							  WriteDirect										            (XBYTE* buffer, XDWORD size)  { return 0;                  }

		virtual bool									Close																			();		

 protected:
		
		DIOSTREAMI2CCONFIG*						config;		
		XTHREADCOLLECTED*							threadconnexion;		
		
	private:
		
		void													Clean																			();					
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

