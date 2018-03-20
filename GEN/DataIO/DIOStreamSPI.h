/*------------------------------------------------------------------------------------------
//  DIOSTREAMSPI.H
*/  
/** 
// \class 
//   
//  Data IO Stream SPI class
//   
//  @author  Abraham J. Velez
//  @version 20/09/2012 14:31:53
*/  
/*  GEN  Copyright (C).  All right reserved.		
//----------------------------------------------------------------------------------------*/
  
#ifndef _DIOSTREAMSPI_H_
#define _DIOSTREAMSPI_H_
  
  
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFSMachine.h"

#include "DIOStream.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


enum  DIOSTREAMSPI_FSMEVENTS
{
	DIOSTREAMSPI_FSMEVENT_NONE								 = 0  ,
	DIOSTREAMSPI_FSMEVENT_CONNECTED									,
	DIOSTREAMSPI_FSMEVENT_WAITINGTOREAD							,	
	DIOSTREAMSPI_FSMEVENT_SENDINGDATA								,
	DIOSTREAMSPI_FSMEVENT_DISCONNECTING							,			

  DIOSTREAMSPI_LASTEVENT
};

enum  DIOSTREAMSPI_FSMSTATES
{
	DIOSTREAMSPI_FSMSTATE_NONE							  = 0  ,	

	DIOSTREAMSPI_FSMSTATE_CONNECTED								 ,
	DIOSTREAMSPI_FSMSTATE_WAITINGTOREAD						 ,
	DIOSTREAMSPI_FSMSTATE_SENDINGDATA							 ,
	DIOSTREAMSPI_FSMSTATE_DISCONNECTING						 ,				

	DIOSTREAMSPI_LASTSTATE
};


#define DIOSTREAMSPI_MAXSIZEBUFFER	4*1024
#define DIOSTREAMSPI_SLEEP					Sleep(timedelay)
#define DIOSTREAMSPI_SLEEPDOUBLE		Sleep(timedelaydouble)


/*---- CLASS -----------------------------------------------------------------------------*/
  
class XBUFFER;
class XFACTORY;
class XTHREADCOLLECTED;
class DIOGPIO;
class DIOSTREAMSPICONFIG;


class DIOSTREAMSPI : public DIOSTREAM , public XFSMACHINE 
{
	public:
																	DIOSTREAMSPI															();
		virtual											 ~DIOSTREAMSPI															();		

		DIOSTREAMCONFIG*							GetConfig																	();
		bool									        SetConfig																	(DIOSTREAMCONFIG* config);	
		
		virtual bool									Open																			();
		virtual bool									Close																			();

		virtual	bool									TransferBuffer														(XBYTE* bufferread, XBYTE* bufferwrite, XDWORD size);
    
		virtual void									Sleep																			(int count);

		DIOGPIO*											GetDIOGPIO																();
		bool													SetDIOGPIO																(DIOGPIO*	diogpio);

 protected:
		
		DIOSTREAMSPICONFIG*						config;

		XTIMER*												xtimer;
		
		DIOGPIO*											diogpio;

		XBYTE*												bufferread;
		
	private:
		
		void													Clean																			();	
			
		bool													TransferOneData														(XWORD writedata,XWORD& readdata);

		static void										ThreadConnexion														(void* data);	
			
		XTHREADCOLLECTED*							threadconnexion;

		int														pin_CLK;
		int														pin_MISO;
		int														pin_MOSI;
		int														pin_CS;				

		XBYTE													mode;
		XBYTE													nbitsword;	
		int 													timedelay;
		int                           timedelaydouble;
		bool                          CSline;
		bool													lbsfirst;	
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

