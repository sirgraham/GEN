/*------------------------------------------------------------------------------------------
//	UIINPUT.H
*/	
/**	
// \file 
//   
//  serve as a middle step between Gen input and controls
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 07/10/2015 12:24:45
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIINPUT_H_
#define _UIINPUT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XVector.h"
#include "INPButton.h"
#include "INPCursor.h"
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class INPMANAGER;
class INPDEVICE;
class UIMOTION;
class UIMANAGER;

class UIINPUT
{
public:

																						UIINPUT												(INPMANAGER* inpmanagerin, UIMANAGER* uimanager);
	virtual																	 ~UIINPUT												()																	{ Clean(); }

								void												Update												();
								bool												IsPressed											(INPBUTTON_ID ID);
								bool												IsTriggered										(INPBUTTON_ID ID);
								bool												IsReleased										(INPBUTTON_ID ID);
								bool												IsPressedLong									(INPBUTTON_ID ID);

								void												GetCursorPosition							(float & x, float & y);

								UIMOTION*										GetMotion											()																	{ return motion; }
protected:

private:
								INPMANAGER*									inpmanager;
								XVECTOR<int>								indexcodes;
								XVECTOR<bool>								currentbuttons;
								XVECTOR<bool>								previousbuttons;

								INPDEVICE*									inpdevicekeyboard;
								INPDEVICE*									inpdevicemouse;
								INPDEVICE*									inpdevicetouchscreen;
								float												mousex;
								float												mousey;

								bool												istouchpressed;
								bool												isoldtouchpressed;

								UIMOTION*										motion;
								UIMANAGER*									uimanager;

								void												Clean													()
																						{
																							inpmanager						= NULL;
																							inpdevicekeyboard			= NULL;
																							inpdevicemouse				= NULL;
																							inpdevicetouchscreen	= NULL;
																							istouchpressed				= false;
																							isoldtouchpressed			= false;
																							motion								= NULL;
																							uimanager							= NULL;
																						}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

