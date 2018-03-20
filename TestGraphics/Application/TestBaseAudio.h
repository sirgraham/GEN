/*------------------------------------------------------------------------------------------
//	TESTBASEAUDIO.H
*/	
/**	
// \class 
//   
//  Test Base Audio
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 30/05/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASEAUDIO_H_
#define _TESTBASEAUDIO_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class SNDFACTORY;
class SNDSTREAMELEMENT; 
class SNDCODEC;


class TESTBASEAUDIO : public TESTBASE, public XOBSERVER								
{																								
	public:																				
																	TESTBASEAUDIO							(GRPAPPLICATION* xapplication);
																 ~TESTBASEAUDIO							();

		virtual bool									IniResources							();   
		virtual bool									EndResources							();
		virtual bool									Do												();     
		
		void													HandleEvent								(XEVENT* xevent);

	private:	

		XPATH												xpath;
		SNDFACTORY*											soundsystem;
		SNDSTREAMELEMENT*									stream;
		SNDCODEC*											codec;
		short*												audiobuffer;

		SNDELEMENT*											beep;
		XTIMER*												timer;
};


	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

