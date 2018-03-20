//------------------------------------------------------------------------------------------
//	INPANDROIDDEVICEKEYBOARD.CPP
//	
//	Input Device Keyboard
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 11/02/2003
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"

#include "INPANDROIDDeviceKeyboard.h"

#include "XMemory.h"
	

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  INPANDROIDDEVICEKEYBOARD::INPANDROIDDEVICEKEYBOARD
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/03/2012 15:38:45
//	
//	@return 			

//  @param				grpscreen : 
*/
/*-----------------------------------------------------------------*/
INPANDROIDDEVICEKEYBOARD::INPANDROIDDEVICEKEYBOARD() : INPDEVICE()
{
	Clean();
	
	created = true;			
	
	SetType(INPDEVICE_TYPE_KEYBOARD);

	SetEnabled(CreateAllButtons());			
}



//-------------------------------------------------------------------
//  INPANDROIDDEVICEKEYBOARD::~INPANDROIDDEVICEKEYBOARD
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			11/02/2003 12:55:30
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
INPANDROIDDEVICEKEYBOARD::~INPANDROIDDEVICEKEYBOARD()
{
	DeleteAllButtons();

	SetEnabled(false);
	created = false;

	Clean();
}

/*-------------------------------------------------------------------
//	 INPANDROIDDEVICEKEYBOARD::SetScreen
*/
/**
//
//
//
//	@author		Diego Martinez Ruiz de Gaona
//	@version
//	@return		bool :
//
//	@param		void* :
//
*//*-----------------------------------------------------------------*/
bool INPANDROIDDEVICEKEYBOARD::SetScreen(void* screenpointer)
{		
		return true;
}



/*-------------------------------------------------------------------
//  INPANDROIDDEVICEKEYBOARD::GetButtons
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/03/2012 15:44:00
//	
//	@return 			XVECTOR* : 
//	*/
/*-----------------------------------------------------------------*/
XVECTOR<INPBUTTON*>* INPANDROIDDEVICEKEYBOARD::GetButtons()
{
	return &buttons;
}




/*-------------------------------------------------------------------
//  INPANDROIDDEVICEKEYBOARD::Release
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/03/2012 15:44:08
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool INPANDROIDDEVICEKEYBOARD::Release()
{
	for(XDWORD c=0;c<buttons.GetSize();c++)
		{			
			INPBUTTON* button = (INPBUTTON*)buttons.Get(c);					
			
			if(button)
				{
					button->SetPressed(false);					
					button->SetMarkToRelase(true);							
				}
		}

	return true;
}

                                                    
//-------------------------------------------------------------------
//  INPANDROIDDEVICEKEYBOARD::Update                  
/**                                                 
//	                                                
//	                                                
//	@author				Abraham J. Velez                  
//	@version			12/02/2003 11:48:00               
//	                                                
//	@return 			bool :                            
//	*/                                              
//-------------------------------------------------------------------
bool INPANDROIDDEVICEKEYBOARD::Update()
{
	if((!created)||(!enabled)) return false;

	return true;
}



/*-------------------------------------------------------------------
//  INPANDROIDDEVICEKEYBOARD::Clean
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/03/2012 15:45:52
//	
//	@return 			void : 
//	*/
/*-----------------------------------------------------------------*/
void INPANDROIDDEVICEKEYBOARD::Clean()
{
	
}



//-------------------------------------------------------------------
//  INPANDROIDDEVICEKEYBOARD::CreateAllButtons
/**
//	
//	@author        Abraham J. Velez
//	@version       18/2/2003   15:33:47
//	
//	@return        bool : 
*/
//-------------------------------------------------------------------
bool INPANDROIDDEVICEKEYBOARD::CreateAllButtons()
{	

//INPBUTTON::CreateButton( &buttons, KEYCODE_SOFT_LEFT        		,	INPBUTTON_ID_								, __C('\x0'));    
//INPBUTTON::CreateButton( &buttons, KEYCODE_SOFT_RIGHT					,	INPBUTTON_ID_								, __C('\x0'));       		
	
	INPBUTTON::CreateButton( &buttons, KEYCODE_HOME             		,	INPBUTTON_ID_HOME	 					, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_BACK             		,	INPBUTTON_ID_ESCAPE  				, __C('\x0'));
	
//INPBUTTON::CreateButton( &buttons, KEYCODE_CALL             		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_ENDCALL          		,	INPBUTTON_ID_								, __C('\x0'));

	INPBUTTON::CreateButton( &buttons, KEYCODE_0                		, INPBUTTON_ID_0		 					, __C('0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_1                		,	INPBUTTON_ID_1		 					, __C('1'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_2                		,	INPBUTTON_ID_2		 					, __C('2'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_3                		,	INPBUTTON_ID_3		 					, __C('3'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_4                		,	INPBUTTON_ID_4		 					, __C('4'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_5                		,	INPBUTTON_ID_5		 					, __C('5'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_6                		,	INPBUTTON_ID_6		 					, __C('6'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_7                		,	INPBUTTON_ID_7		 					, __C('7'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_8                		,	INPBUTTON_ID_8		 					, __C('8'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_9                		,	INPBUTTON_ID_9		 					, __C('9'));

//INPBUTTON::CreateButton( &buttons, KEYCODE_STAR             		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_POUND            		,	INPBUTTON_ID_								, __C('\x0'));

	INPBUTTON::CreateButton( &buttons, KEYCODE_DPAD_UP          		,	INPBUTTON_ID_UP		 					, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_DPAD_DOWN        		,	INPBUTTON_ID_DOWN						, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_DPAD_LEFT        		,	INPBUTTON_ID_LEFT						, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_DPAD_RIGHT       		,	INPBUTTON_ID_RIGHT		 			, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_DPAD_CENTER      		,	INPBUTTON_ID_								, __C('\x0'));

//INPBUTTON::CreateButton( &buttons, KEYCODE_VOLUME_UP        		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_VOLUME_DOWN      		,	INPBUTTON_ID_								, __C('\x0'));

//INPBUTTON::CreateButton( &buttons, KEYCODE_POWER            		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_CAMERA           		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_CLEAR            		,	INPBUTTON_ID_								, __C('\x0'));

	INPBUTTON::CreateButton( &buttons, KEYCODE_A                		,	INPBUTTON_ID_A		 					, __C('A'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_B                		,	INPBUTTON_ID_B		 					, __C('B'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_C                		,	INPBUTTON_ID_C		 					, __C('C'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_D                		,	INPBUTTON_ID_D		 					, __C('D'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_E                		,	INPBUTTON_ID_E		 					, __C('E'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_F                		,	INPBUTTON_ID_F		 					, __C('F'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_G                		,	INPBUTTON_ID_G		 					, __C('G'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_H                		,	INPBUTTON_ID_H		 					, __C('H'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_I                		,	INPBUTTON_ID_I		 					, __C('I'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_J                		,	INPBUTTON_ID_J		 					, __C('J'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_K                		,	INPBUTTON_ID_K		 					, __C('K'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_L                		,	INPBUTTON_ID_L		 					, __C('L'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_M                		,	INPBUTTON_ID_M		 					, __C('M'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_N                		,	INPBUTTON_ID_N		 					, __C('N'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_O                		,	INPBUTTON_ID_O		 					, __C('O'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_P                		,	INPBUTTON_ID_P		 					, __C('P'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_Q                		,	INPBUTTON_ID_Q		 					, __C('Q'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_R                		,	INPBUTTON_ID_R		 					, __C('R'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_S                		,	INPBUTTON_ID_S		 					, __C('S'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_T                		,	INPBUTTON_ID_T		 					, __C('T'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_U                		,	INPBUTTON_ID_U		 					, __C('U'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_V                		,	INPBUTTON_ID_V		 					, __C('V'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_W                		,	INPBUTTON_ID_W		 					, __C('W'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_X                		,	INPBUTTON_ID_X		 					, __C('X'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_Y                		,	INPBUTTON_ID_Y		 					, __C('Y'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_Z                		,	INPBUTTON_ID_Z		 					, __C('Z'));
                                                                                           					
	INPBUTTON::CreateButton( &buttons, KEYCODE_COMMA            		,	INPBUTTON_ID_COMMA	 			  ,  __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_PERIOD           		,	INPBUTTON_ID_								, __C('\x0'));
	
	INPBUTTON::CreateButton( &buttons, KEYCODE_ALT_LEFT         		,	INPBUTTON_ID_ALT_LEFT		 		, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_ALT_RIGHT        		,	INPBUTTON_ID_ALT_RIGHT		 	, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_SHIFT_LEFT       		,	INPBUTTON_ID_SHIFT_LEFT			, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_SHIFT_RIGHT      		,	INPBUTTON_ID_SHIFT_RIGHT		, __C('\x0'));
	
	INPBUTTON::CreateButton( &buttons, KEYCODE_TAB              		,	INPBUTTON_ID_TAB		 				, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_SPACE            		,	INPBUTTON_ID_SPACE				  , __C('\x0'));
	
//INPBUTTON::CreateButton( &buttons, KEYCODE_SYM              		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_EXPLORER         		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_ENVELOPE         		,	INPBUTTON_ID_								, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_ENTER            		,	INPBUTTON_ID_RETURN		 			, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_DEL              		,	INPBUTTON_ID_DELETE					, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_GRAVE            		,	INPBUTTON_ID_								, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_MINUS            		,	INPBUTTON_ID_SUBTRACT				, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_EQUALS           		,	INPBUTTON_ID_								, __C('\x0'));	
	
	INPBUTTON::CreateButton( &buttons, KEYCODE_LEFT_BRACKET  	  	,	INPBUTTON_ID_OPEN_BRACKET		, __C('['));
	INPBUTTON::CreateButton( &buttons, KEYCODE_RIGHT_BRACKET     	,	INPBUTTON_ID_CLOSE_BRACKET	, __C(']'));

//INPBUTTON::CreateButton( &buttons, KEYCODE_BACKSLASH        		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_SEMICOLON        		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_APOSTROPHE       		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_SLASH            		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_AT               		,	INPBUTTON_ID_								, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_NUM              		,	INPBUTTON_ID_NUMLOCK	 			, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_HEADSETHOOK      		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_FOCUS            		,	INPBUTTON_ID_								, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_PLUS             		,	INPBUTTON_ID_ADD						, __C('\x0'));
	INPBUTTON::CreateButton( &buttons, KEYCODE_MENU             		,	INPBUTTON_ID_MENU		 				, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_NOTIFICATION     		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_SEARCH           		,	INPBUTTON_ID_								, __C('\x0'));
	
//INPBUTTON::CreateButton( &buttons, KEYCODE_MEDIA_PLAY_PAUSE 		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_MEDIA_STOP       		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_MEDIA_NEXT       		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_MEDIA_PREVIOUS   		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_MEDIA_REWIND     		,	INPBUTTON_ID_								, __C('\x0'));
//INPBUTTON::CreateButton( &buttons, KEYCODE_MEDIA_FAST_FORWARD  ,	INPBUTTON_ID_								, __C('\x0'));
	
//INPBUTTON::CreateButton( &buttons, KEYCODE_MUTE     						,	INPBUTTON_ID_								, __C('\x0'));       		

	return true;																									
}																								



//-------------------------------------------------------------------
//  INPANDROIDDEVICEKEYBOARD::DeleteAllButtons
/**
//	
//	@author        Abraham J. Velez
//	@version       18/2/2003   15:34:3
//	
//	@return        bool : 
*/
//-------------------------------------------------------------------
bool INPANDROIDDEVICEKEYBOARD::DeleteAllButtons()
{
	if(buttons.IsEmpty())  return false;

	buttons.DeleteContents();
	
	buttons.DeleteAll();

	return true;
}


