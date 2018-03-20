//------------------------------------------------------------------------------------------
//	INPANDROIDDEVICEKEYBOARD.H
//	
/**	
// \class 
//   
//  Android Input Device Keyboard
//   
//	@author	 Abraham J. Velez
//	@version 11/02/2003
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _INPANDROIDDEVICEKEYBOARD_H_
#define _INPANDROIDDEVICEKEYBOARD_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XVector.h"

#include "INPButton.h"
#include "INPDevice.h"	
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------


#define KEYCODE_SOFT_LEFT        		 1
#define KEYCODE_SOFT_RIGHT       		 2
#define KEYCODE_HOME             		 3
#define KEYCODE_BACK             		 4
#define KEYCODE_CALL             		 5
#define KEYCODE_ENDCALL          		 6

#define KEYCODE_0                		 7
#define KEYCODE_1                		 8
#define KEYCODE_2                		 9
#define KEYCODE_3                		 10
#define KEYCODE_4                		 11
#define KEYCODE_5                		 12
#define KEYCODE_6                		 13
#define KEYCODE_7                		 14
#define KEYCODE_8                		 15
#define KEYCODE_9                		 16

#define KEYCODE_STAR             		 17
#define KEYCODE_POUND            		 18

#define KEYCODE_DPAD_UP          		 19
#define KEYCODE_DPAD_DOWN        		 20
#define KEYCODE_DPAD_LEFT        		 21
#define KEYCODE_DPAD_RIGHT       		 22
#define KEYCODE_DPAD_CENTER      		 23

#define KEYCODE_VOLUME_UP        		 24
#define KEYCODE_VOLUME_DOWN      		 25

#define KEYCODE_POWER            		 26
#define KEYCODE_CAMERA           		 27
#define KEYCODE_CLEAR            		 28

#define KEYCODE_A                		 29
#define KEYCODE_B                		 30
#define KEYCODE_C                		 31
#define KEYCODE_D                		 32
#define KEYCODE_E                		 33
#define KEYCODE_F                		 34
#define KEYCODE_G                		 35
#define KEYCODE_H                		 36
#define KEYCODE_I                		 37
#define KEYCODE_J                		 38
#define KEYCODE_K                		 39
#define KEYCODE_L                		 40
#define KEYCODE_M                		 41
#define KEYCODE_N                		 42
#define KEYCODE_O                		 43
#define KEYCODE_P                		 44
#define KEYCODE_Q                		 45
#define KEYCODE_R                		 46
#define KEYCODE_S                		 47
#define KEYCODE_T                		 48
#define KEYCODE_U                		 49
#define KEYCODE_V                		 50
#define KEYCODE_W                		 51
#define KEYCODE_X                		 52
#define KEYCODE_Y                		 53
#define KEYCODE_Z                		 54

#define KEYCODE_COMMA            		 55
#define KEYCODE_PERIOD           		 56
#define KEYCODE_ALT_LEFT         		 57
#define KEYCODE_ALT_RIGHT        		 58
#define KEYCODE_SHIFT_LEFT       		 59
#define KEYCODE_SHIFT_RIGHT      		 60
#define KEYCODE_TAB              		 61
#define KEYCODE_SPACE            		 62
#define KEYCODE_SYM              		 63
#define KEYCODE_EXPLORER         		 64
#define KEYCODE_ENVELOPE         		 65
#define KEYCODE_ENTER            		 66
#define KEYCODE_DEL              		 67
#define KEYCODE_GRAVE            		 68
#define KEYCODE_MINUS            		 69
#define KEYCODE_EQUALS           		 70
#define KEYCODE_LEFT_BRACKET     		 71
#define KEYCODE_RIGHT_BRACKET    		 72
#define KEYCODE_BACKSLASH        		 73
#define KEYCODE_SEMICOLON        		 74
#define KEYCODE_APOSTROPHE       		 75
#define KEYCODE_SLASH            		 76
#define KEYCODE_AT               		 77
#define KEYCODE_NUM              		 78
#define KEYCODE_HEADSETHOOK      		 79
#define KEYCODE_FOCUS            		 80   // *Camera* focus
#define KEYCODE_PLUS             		 81
#define KEYCODE_MENU             		 82
#define KEYCODE_NOTIFICATION     		 83
#define KEYCODE_SEARCH           		 84
#define KEYCODE_MEDIA_PLAY_PAUSE 		 85
#define KEYCODE_MEDIA_STOP       		 86
#define KEYCODE_MEDIA_NEXT       		 87
#define KEYCODE_MEDIA_PREVIOUS   		 88
#define KEYCODE_MEDIA_REWIND     		 89
#define KEYCODE_MEDIA_FAST_FORWARD   90
#define KEYCODE_MUTE            		 91



//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;

class INPANDROIDDEVICEKEYBOARD : public INPDEVICE
{
	public:
												  INPANDROIDDEVICEKEYBOARD			();
		virtual							 ~INPANDROIDDEVICEKEYBOARD			();

		bool									SetScreen											(void* screenpointer);



		XVECTOR<INPBUTTON*>*	GetButtons										();

		bool									Release												();		
		bool									Update												();	

	private:

		void									Clean													();

		bool									CreateAllButtons							();
		bool									DeleteAllButtons							();
	
		XVECTOR<INPBUTTON*>		buttons;			
};
	
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

