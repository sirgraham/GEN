
/*------------------------------------------------------------------------------------------
//	TESTBASEINPUT.CPP
//	
//	Test Base Input
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 28/03/2017 12:20
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XApplication.h"

#include "GRPFile3DFBX.h"
#include "GRPFile3DOBJ.h"
#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPObjectBuilder.h"
#include "GRPFont.h"
#include "GRPFontManager.h"
#include "GRPConsole.h"
#include "GRPRenderer.h"
#include "GRPViewport.h"
#include "GRPCamera.h"
#include "GRPVisualDebugger.h"

#include "TestInput.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//	TESTBASEINPUT::TESTBASEINPUT
*/	
/**	
//	
//	Class Constructor TESTBASEINPUT
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/03/2017 12:20
//	
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTBASEINPUT::TESTBASEINPUT(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)
{
	Clean();
	
}




/*-------------------------------------------------------------------
//	TESTBASEINPUT::~TESTBASEINPUT
*/	
/**	
//	
//	 Class Destructor TESTBASEINPUT
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/03/2017 12:20
//	
*/
/*-----------------------------------------------------------------*/
TESTBASEINPUT::~TESTBASEINPUT()
{
	Clean();
}



/*-------------------------------------------------------------------
//	TESTBASEINPUT::IniResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/03/2017 12:20
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEINPUT::IniResources()
{
 	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("console"));

	XPATH xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	InitConsole();

	for (XDWORD e=0;e<mouse->GetCursors()->GetSize();e++)
	{
		XSTRING s;
		s.Format(__L("Mouse cursor[%d]"),mouse->GetCursors()->Get(e)->GetID());
		console->Print(0,s.Get());
	}
	
	return true;
}


/*-------------------------------------------------------------------
//	TESTBASEINPUT::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/03/2017 12:20
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEINPUT::EndResources()
{
	
	return false;
}

/*-------------------------------------------------------------------
//	TESTBASEINPUT::Do
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/03/2017 12:20
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEINPUT::Do()
{
	if (console)
		console->Update();

	XDWORD g = gesture.CheckSequence();

	if (g)
	{
			XDEBUG_PRINTCOLOR(0, __L("Seq: %d"), g);
	}

	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEINPUT::InputManagement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/03/2017 12:21:42
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEINPUT::InputManagement()
{
	static float dragx=0.0f;
	static float dragy=0.0f;
	
	if (!keyboard)
			{
				keyboard = INPMANAGER::GetInstance()->GetDevice(INPDEVICE_TYPE_KEYBOARD);
			}

	if(!keyboard) return false;

	if (!mouse)
			{
				mouse = INPMANAGER::GetInstance()->GetDevice(INPDEVICE_TYPE_MOUSE);
			}

		if(keyboard->GetButton(INPBUTTON_ID_ESCAPE))
		if(keyboard->GetButton(INPBUTTON_ID_ESCAPE)->GetState()==INPBUTTON_STATE_RELEASED) 			
			xapplication->SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER);

		if(keyboard->GetButton(INPBUTTON_ID_F1))												
		if(keyboard->GetButton(INPBUTTON_ID_F1)->GetState()==INPBUTTON_STATE_RELEASED) 			
		{
			if (console)
			{
			console->Clear();
			console->Print(GRPCOLOR::GRAY,__L("test"));
			}
		}
		
	if (mouse)
	{
		if (!mouse->GetCursors()->Get(0)->GetX() && !mouse->GetCursors()->Get(0)->GetY())
			return TESTBASE::InputManagement();
					
			for (XDWORD e=0;e<mouse->GetButtons()->GetSize();e++)
			{
				INPBUTTON* button=mouse->GetButtons()->Get(e);
				if (button)		
				{
						XSTRING s;
						s.Set(__L("test"));
						float x=0.0f;
						float y=0.0f;
					
						if (mouse->GetCursors()->GetSize()>e)
						{
							x=mouse->GetCursors()->Get(e)->GetX();
							y=mouse->GetCursors()->Get(e)->GetY();
						}
						else
						{
							x=mouse->GetCursors()->Get(0)->GetX();
							y=mouse->GetCursors()->Get(0)->GetY();
						}											
						
						switch(button->state)
						{
						case INPBUTTON_STATE_UP: break;
						case INPBUTTON_STATE_PRESSED: 									
							s.Format(__L("Mouse PRESS %d [%f,%f]"),e,x,y);																									
							if (console)	console->Print(e,s.Get());
						break;
						case INPBUTTON_STATE_HOLD:														
							s.Format(__L("Mouse HOLD %d [%f,%f]"),e,x,y);							
							if (console)	console->Print(e,s.Get());
						break;
						case INPBUTTON_STATE_RELEASED:				
							s.Format(__L("Mouse RELEASED %d [%f,%f]"),e,x,y);							
							if (console)	console->Clear();
							if (console)	console->Print(e,s.Get());
							
						break;
						}
						
						
				}
			}
		
	}	
//*/

		if (keyboard)
		{
			XSTRING s;			
			for (XDWORD e=0;e<keyboard->GetButtons()->GetSize();e++)
			{			
				INPBUTTON* button=keyboard->GetButtons()->Get(e);
				
				switch(button->state)
				{
				case INPBUTTON_STATE_UP: break;
				case INPBUTTON_STATE_PRESSED: 
					s.Format(__L("INPBUTTON_STATE_PRESSED %d %c"),button->GetKeyCode(),button->GetSymbol());	
					if (console)			
					console->Print(0,s.Get());
					break;
				case INPBUTTON_STATE_HOLD:
					s.Format(__L("INPBUTTON_STATE_HOLD %d %c"),button->GetKeyCode(),button->GetSymbol());	
					if (console)
					console->Print(0,s.Get());
					break;
				case INPBUTTON_STATE_RELEASED:
					if (console)
					console->Clear();
					s.Format(__L("INPBUTTON_STATE_RELEASED %d %c"),button->GetKeyCode(),button->GetSymbol());	
					if (console)
					console->Print(0,s.Get());
					break;
				}
			}
		}
//		*/
		return true;
}
