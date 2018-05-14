/*------------------------------------------------------------------------------------------
//	TESTCALIBRATION.CPP
//	
//	Test Base Render Object
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 30/05/2016 16:19:29
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
#include "GRPCamera.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPVisualDebugger.h"

#include "GRPObjectBuilder.h"
#include "GRPFont.h"
#include "GRPFontManager.h"

#include "TestCalibration.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	TESTCALIBRATION::TESTCALIBRATION
*/	
/**	
//	
//	Class Constructor TESTBASERENDEROBJECT
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/02/2016 16:27:24
//	
//  @param				inpmanager : 
//  @param				screen : 
//  @param				scene : 
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTCALIBRATION::TESTCALIBRATION(GRPAPPLICATION* xapplication)  :  TESTBASE(xapplication) 
{
	Clean();

	timer = xfactory->CreateTimer();
	delta = 0;
}




/*-------------------------------------------------------------------
//	TESTCALIBRATION::~TESTCALIBRATION
*/	
/**	
//	
//	 Class Destructor TESTBASERENDEROBJECT
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/02/2016 16:27:19
//	
*/
/*-----------------------------------------------------------------*/
TESTCALIBRATION::~TESTCALIBRATION()
{
	if(timer) xfactory->DeleteTimer(timer);

	Clean();
}




/*-------------------------------------------------------------------
//	TESTCALIBRATION::IniResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			30/04/2015 13:48:43
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTCALIBRATION::IniResources()
{		
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE*  scene  = xapplication->GetMainScene();
	XPATH			 xpath;

	PathManagement(__L("calibrate"));


			
	pca9685 = new DIOI2CPWMCONTROLERPCA9685();
	if(!pca9685) return false;
	
	if(pca9685->Ini(__L("/dev/i2c-1"), 0x40, 10))
		{
			XDEBUG_PRINTCOLOR(0,__L(" Ini device ..."), 1, true, true);	
		
			if(!pca9685->SetPWMFrequency(60)) 
				{
					XDEBUG_PRINTCOLOR(4,__L(" Error PWM frecuency \n"));						
				}
						
			pca9685->SetPWM(0, 0, ((1200)/255)); 
			pca9685->SetPWM(1, 0, ((1200)/255));
			pca9685->SetPWM(2, 0, ((1200)/255));
					
		}		

	InitConsole(0.9f);

	return true;
}


/*-------------------------------------------------------------------
//	TESTCALIBRATION::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 10:45:54
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTCALIBRATION::EndResources()
{
	return true;
}

/*-------------------------------------------------------------------
//	TESTCALIBRATION::Do
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			30/04/2015 13:48:47
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTCALIBRATION::Do()
{
	bool update=false;

	speed=1;

	if(keyboard->GetButton(INPBUTTON_ID_SHIFT_LEFT))
	if(keyboard->GetButton(INPBUTTON_ID_SHIFT_LEFT)->IsPressed()) 	
		speed=10;

	if(keyboard->GetButton(INPBUTTON_ID_ALT_LEFT))
	if(keyboard->GetButton(INPBUTTON_ID_ALT_LEFT)->IsPressed()) 	
		speed=50;
	
	
	if(keyboard->GetButton(INPBUTTON_ID_1))
	if(keyboard->GetButton(INPBUTTON_ID_1)->IsPressed()) 	
	{
		r+=speed;update=true;
	}
	if(keyboard->GetButton(INPBUTTON_ID_Q))
	if(keyboard->GetButton(INPBUTTON_ID_Q)->IsPressed()) 	
	{
		r-=speed;update=true;
	}

	if(keyboard->GetButton(INPBUTTON_ID_2))
	if(keyboard->GetButton(INPBUTTON_ID_2)->IsPressed()) 	
	{
		g+=speed;update=true;
	}
	if(keyboard->GetButton(INPBUTTON_ID_W))
	if(keyboard->GetButton(INPBUTTON_ID_W)->IsPressed()) 	
	{
		g-=speed;update=true;
	}

	if(keyboard->GetButton(INPBUTTON_ID_3))
	if(keyboard->GetButton(INPBUTTON_ID_3)->IsPressed()) 	
	{
		b+=speed;update=true;
	}
	if(keyboard->GetButton(INPBUTTON_ID_E))
	if(keyboard->GetButton(INPBUTTON_ID_E)->IsPressed()) 	
	{
		b-=speed;update=true;
	}


	if(keyboard->GetButton(INPBUTTON_ID_4))
	if(keyboard->GetButton(INPBUTTON_ID_4)->IsPressed()) 	
	{
		a+=speed;update=true;
	}
	if(keyboard->GetButton(INPBUTTON_ID_R))
	if(keyboard->GetButton(INPBUTTON_ID_R)->IsPressed()) 	
	{
		a-=speed;update=true;
	}

	if(keyboard->GetButton(INPBUTTON_ID_A))
	if(keyboard->GetButton(INPBUTTON_ID_A)->IsPressed()) 	
	{
		r=100.0f;
		g=0.0f;
		b=0.0f;
		a=100.0f;update=true;
	}
	if(keyboard->GetButton(INPBUTTON_ID_S))
	if(keyboard->GetButton(INPBUTTON_ID_S)->IsPressed()) 	
	{
		r=0.0f;
		g=100.0f;
		b=0.0f;
		a=100.0f;update=true;
	}
	
	if(keyboard->GetButton(INPBUTTON_ID_T))
	if(keyboard->GetButton(INPBUTTON_ID_T)->IsPressed()) 	
	{
		brate++;update=true;
	}
	if(keyboard->GetButton(INPBUTTON_ID_5))
	if(keyboard->GetButton(INPBUTTON_ID_5)->IsPressed()) 	
	{
		brate--;update=true;
	}


	if(keyboard->GetButton(INPBUTTON_ID_D))
	if(keyboard->GetButton(INPBUTTON_ID_D)->IsPressed()) 	
	{
		r=0.0f;
		g=0.0f;
		b=100.0f;
		a=100.0f;update=true;
	}

	if(keyboard->GetButton(INPBUTTON_ID_B))
		if(keyboard->GetButton(INPBUTTON_ID_B)->GetState()==INPBUTTON_STATE_RELEASED) 	
	{
		blinking=!blinking;update=true;
	}

		
	if (r<0.0f) r=0.0f;
	if (g<0.0f) g=0.0f;
	if (b<0.0f) b=0.0f;
	if (a<0.0f) a=0.0f;
	if (r>100.0f) r=100.0f;
	if (g>100.0f) g=100.0f;
	if (b>100.0f) b=100.0f;
	if (a>100.0f) a=100.0f;
	if (brate<=1) brate=1;

	console->Clear();
	XSTRING rs;
	rs.Format(__L("Red %d "),int(r));
	console->Print(XDEBUG_COLORRED,rs.Get());
	XSTRING gs;
	gs.Format(__L("Green %d "),int(g));
	console->Print(XDEBUG_COLORGREEN,gs.Get());
	XSTRING bs;
	bs.Format(__L("Blue %d "),int(b));
	console->Print(XDEBUG_COLORBLUE,bs.Get());
	XSTRING as;
	as.Format(__L("Alpha %d "),int(a));
	console->Print(XDEBUG_COLORBLACK,as.Get());
	XSTRING blink;
	blink.Format(__L("Blinking %s rate %d"),(blinking? __L("On") : __L("Off")),brate);
	console->Print(XDEBUG_COLORBLACK,blink.Get());	

	
	static int cycle=0;

	if (blinking)
	cycle++;


	if (cycle>brate)
	{			
			if (cycle>=(brate*2))
			{
					cycle=0;					
			}		

			if(pca9685)
			{
			pca9685->SetPWM(0, 0, 0); 
			pca9685->SetPWM(1, 0, 0); 
			pca9685->SetPWM(2, 0, 0);	
			console->Update();
			return true;
			}
			
						
	}
	

	XSTRING s;
	s.Format(__L("Result"));
	console->Print(GRPCOLOR((r*(a/100))/100,(g*(a/100))/100,(b*(a/100))/100,a/100),s.Get());
	console->Update();
	
	if (cycle<=brate)
	if(pca9685)
		{
			color.Set((float)r/100.0f,(float)g/100.0f,(float)b/100.0f,(float)a/100.0f);

			pca9685->SetPWM(0, 0, (XDWORD)((color.GetRed()*		color.GetAlpha()*4095))); 
			pca9685->SetPWM(1, 0, (XDWORD)((color.GetGreen()*	color.GetAlpha()*4095))); 
			pca9685->SetPWM(2, 0, (XDWORD)((color.GetBlue()*	color.GetAlpha()*4095)));			
			
		}
		
	
	
	return true;
}



/*-------------------------------------------------------------------
//	TESTCALIBRATION::Notify
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			15/01/2016 16:21:20
//	
//	@return 			bool : 
//
//  @param				msg : 
//  @param				notifier : 
*/
/*-----------------------------------------------------------------*/
bool TESTCALIBRATION::Notify(GRPMSGTYPE msg, GRPSUBJECT* notifier)
{
	XQWORD delta = timer->GetMeasureMicroSeconds();
	timer->Reset();

	//XDEBUG_PRINTCOLOR(4,__L("Delta %llu"), delta);

	GRPANIMATIONSTACKMILESTONE * n = (GRPANIMATIONSTACKMILESTONE*)notifier;

	n->disabled = false;

	return false;
}
