/*------------------------------------------------------------------------------------------
//	DIOLINUXGPIOPC.CPP
//	
//	LINUX DIO port data PC class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 25/07/2009 07:14:17 p.m.
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	

/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>	
#include <stdint.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/io.h>

#include "XDebug.h"

#include "DIOLINUXGPIOPC.h"

#include "XMemory.h"


/*-------------------------------------------------------------------
//  DIOLINUXGPIOPC::DIOLINUXGPIOPC
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/07/2009 07:18:20 p.m.
//	
//	@return				
*/
/*-----------------------------------------------------------------*/
DIOLINUXGPIOPC::DIOLINUXGPIOPC() : DIOGPIO()
{
	Clean();
}


/*-------------------------------------------------------------------
//  DIOLINUXGPIOPC::~DIOLINUXGPIOPC
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/07/2009 07:18:44 p.m.
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
DIOLINUXGPIOPC::~DIOLINUXGPIOPC()
{
	Clean();
}



/*-------------------------------------------------------------------
//	DIOLINUXGPIOPC::Ini
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			22/10/2015 14:25:06
//	
//	@return 			bool : 
//
//  @param				xpath : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOPC::Ini(XPATH* xpath)
{	
  /* 0*/ GPIOvalue[DIOGPIO_PINSGROUP_A][0] = 0;
	/* 1*/ GPIOvalue[DIOGPIO_PINSGROUP_A][1] = 1;
	/* 2*/ GPIOvalue[DIOGPIO_PINSGROUP_A][2] = 2;
	/* 3*/ GPIOvalue[DIOGPIO_PINSGROUP_A][3] = 3;
	/* 4*/ GPIOvalue[DIOGPIO_PINSGROUP_A][4] = 4;
	/* 5*/ GPIOvalue[DIOGPIO_PINSGROUP_A][5] = 5;
	/* 6*/ GPIOvalue[DIOGPIO_PINSGROUP_A][6] = 6;
	/* 7*/ GPIOvalue[DIOGPIO_PINSGROUP_A][7] = 7;
	
	return true;			
}




/*-------------------------------------------------------------------
//	DIOLINUXGPIOPC::SetMode
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/09/2016 16:22:26
//	
//	@return 			bool : 
//
//  @param				pin : 
//  @param				isinput : 
//  @param				group : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOPC::SetMode(XBYTE pin,bool isinput, DIOGPIO_PINSGROUP group)
{
  return true;  
}



/*-------------------------------------------------------------------
//	DIOLINUXGPIOPC::Get
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/09/2016 16:22:47
//	
//	@return 			bool : 
//
//  @param				pin : 
//  @param				group : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOPC::Get(XBYTE pin, DIOGPIO_PINSGROUP group)
{
  XBYTE data = 0;

	PC_Get(PCPARALLEL_PORT,data);	
	
	return (data&~(0x01<<GPIOvalue[DIOGPIO_PINSGROUP_A][pin]))?true:false;
}



/*-------------------------------------------------------------------
//	DIOLINUXGPIOPC::Set
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			23/09/2016 16:23:17
//	
//	@return 			bool : 
//
//  @param				pin : 
//  @param				active : 
//  @param				group : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOPC::Set(XBYTE pin, bool active, DIOGPIO_PINSGROUP group)
{
  XBYTE data = 0;
									
	PC_Get(PCPARALLEL_PORT,data);
	PC_Set(PCPARALLEL_PORT,active?(data|(0x01<<GPIOvalue[DIOGPIO_PINSGROUP_A][pin])):(data&~(0x01<<GPIOvalue[DIOGPIO_PINSGROUP_A][pin])));
	
	return true;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOPC::End
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/07/2009 08:24:53 p.m.
//	
//	@return				bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOPC::End()
{	
  return true;  
}






/*-------------------------------------------------------------------
//  DIOLINUXGPIOPC::PC_Get
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/08/2009 12:42:40 a.m.
//	
//	@return				bool : 
//	@param				nport : 
//  @param				data : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOPC::PC_Get(XDWORD nport,XBYTE& data)
{
	XDWORD val = 0;
	
	if(ioperm(nport,1,1)) return false;		 
	
	val = inb(nport);		
	
	if(ioperm(nport,1,0)) return false;
	
	data = (XBYTE)(val & 0x0FF);

	return true;
}



/*-------------------------------------------------------------------
//  DIOLINUXGPIOPC::PC_Set
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/08/2009 12:42:47 a.m.
//	
//	@return				bool : 
//	@param				nport : 
//  @param				data : 
*/
/*-----------------------------------------------------------------*/
bool DIOLINUXGPIOPC::PC_Set(XDWORD nport,XBYTE data)
{	
	if(ioperm(nport,1,1)) return false;
	
	outb(data, nport);	
	
	if(ioperm(nport,1,0)) return false;

	return true;
}


