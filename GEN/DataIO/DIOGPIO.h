/*------------------------------------------------------------------------------------------
//	DIOGPIO.H
*/	
/**	
// \class 
//   
//  DIO port functions
//   
//	@author	 Abraham J. Velez
//	@version 25/07/2009 07:13:30 p.m.
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _DIOGPIO_H_
#define _DIOGPIO_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"
#include "XSystem.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum DIOGPIO_PINSGROUP
{
	DIOGPIO_PINSGROUP_A    = 0 ,
	DIOGPIO_PINSGROUP_B				 ,
	DIOGPIO_PINSGROUP_C				 ,
	DIOGPIO_PINSGROUP_D				 ,
	DIOGPIO_PINSGROUP_E				 ,
  DIOGPIO_PINSGROUP_F				 ,
};


#define DIOGPIO_MAXNPINSGROUP		    6
#define DIOGPIO_MAXPINS					    64
#define DIOGPIO_PINNOTUSE						0x80000000
#define DIOGPIO_PINISPWM						0x40000000

#define DIOGPIO_PINMODE_NONE				0x00
#define DIOGPIO_PINMODE_INPUT				0x01
#define DIOGPIO_PINMODE_OUPUT				0x02
#define DIOGPIO_PINMODE_PWM					0x04

	
/*---- CLASS -----------------------------------------------------------------------------*/
	
class XFACTORY;
class XPATH;

class DIOGPIO  
{
	public:
															DIOGPIO							()																																								{ Clean();													}
		virtual	  			  			 ~DIOGPIO							()																																								{ Clean();													}
		
		virtual bool							Ini									(XPATH* xpath = NULL)																															{ return false;									 	  } 
				
		virtual bool							SetMode							(XBYTE pin, bool isinput, DIOGPIO_PINSGROUP block = DIOGPIO_PINSGROUP_A)					{ return false;											} 	
		virtual bool							SetMode							(XBYTE pin, XBYTE mode, DIOGPIO_PINSGROUP block = DIOGPIO_PINSGROUP_A)						{ return false;											}															
		virtual bool							Get									(XBYTE pin, DIOGPIO_PINSGROUP block = DIOGPIO_PINSGROUP_A)												{ return false;											} 
		virtual bool							Set									(XBYTE pin, bool active, DIOGPIO_PINSGROUP block = DIOGPIO_PINSGROUP_A)						{ return false;											} 


		float											GetPWMFrequency			(float frequency)																																	{ return PWMfrequency;							}
		bool											SetPWMFrequency			(float frequency)																																	
                              { 
                                this->PWMfrequency = frequency;	
                                return true;
                              }		
		bool											SetPWM							(XBYTE pin, XWORD on, XWORD off, DIOGPIO_PINSGROUP block = DIOGPIO_PINSGROUP_A)		{ return false;											}	
		
		virtual bool							End									()																																								{ return false;											} 

	protected:
	
		XDWORD										GPIOvalue[DIOGPIO_MAXNPINSGROUP][DIOGPIO_MAXPINS];
		float											PWMfrequency;

	private:

		void											Clean								()
															{
																for(int d=0; d<DIOGPIO_MAXNPINSGROUP; d++)
																	{
																		for(int c=0; c<DIOGPIO_MAXPINS; c++)
																				{
																					GPIOvalue[d][c] = 0;
																				}
																	}

																PWMfrequency = 0.0f;

															}
};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

