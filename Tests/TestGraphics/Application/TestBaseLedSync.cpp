
/*------------------------------------------------------------------------------------------
//	TESTBASEFONTTTF.CPP
//	
//	Test Base Led Synchronize
//   
//	Author						: Abraham J. Velez
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
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPFont.h"
#include "GRPFontManager.h"
#include "GRPRenderer.h"
#include "GRPVisualDebugger.h"

#include "DIOI2CPWMControlerPCA9685.h"

#include "TestBaseLedSync.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	TESTBASELEDSYNC::TESTBASELEDSYNC
*/	
/**	
//	
//	Class Constructor TESTBASELEDSYNC
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/02/2016 12:37:12
//	
//  @param				inpmanager : 
//  @param				screen : 
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
TESTBASELEDSYNC::TESTBASELEDSYNC(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)
{
	XDEBUG_PRINTCOLOR(0,__L("INIT LEDS"));
	pca9685=NULL;

	red				= 0;
	green			= 0;
	blue			= 0;
	intensity = 1;
}




/*-------------------------------------------------------------------
//	TESTBASELEDSYNC::~TESTBASELEDSYNC
*/	
/**	
//	
//	 Class Destructor TESTBASELEDSYNC
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/02/2016 12:37:04
//	
*/
/*-----------------------------------------------------------------*/
TESTBASELEDSYNC::~TESTBASELEDSYNC									()
{
	if(pca9685)
		{
			pca9685->SetPWM(0, 0, 0); 
			pca9685->SetPWM(1, 0, 0);
			pca9685->SetPWM(2, 0, 0);

			pca9685->End();

			delete pca9685;
		}
}




/*-------------------------------------------------------------------
//	TESTBASEFBX::IniResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/02/2016 9:54:32
//	
*/
/*-----------------------------------------------------------------*/
bool TESTBASELEDSYNC::IniResources()
{		
	if(!xapplication)										 return false;
	if(!xapplication->GetMainScene())		 return false;

	GRPSCENE*  scene   = xapplication->GetMainScene();

	PathManagement(__L("ledsync"));

	XPATH xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);
	
	GRPOBJECT* obj=NULL;

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("ledcontrol.FBX"));
	if(!(obj=LoadObject(obj,xpath.Get())))		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}

	scene->Add (obj);
		
	pca9685 = new DIOI2CPWMCONTROLERPCA9685;
	if(!pca9685) return false;
	
	if(pca9685->Ini(__L("/dev/i2c-1"), 0x40, 10))
		{
			XDEBUG_PRINTCOLOR(0,__L(" Ini device ..."), 1, true, true);	
		
			if(!pca9685->SetPWMFrequency(60)) 
				{
					XDEBUG_PRINTCOLOR(4,__L(" Error PWM frecuency \n"));						
				}
						
			pca9685->SetPWM(0, 0, ((red*1200)/255)); 
			pca9685->SetPWM(1, 0, ((green*1200)/255));
			pca9685->SetPWM(2, 0, ((blue*1200)/255));
					
		}		
	
	GRPANIMATIONNODE* node;
	
	node = obj->GetAnimationControl()->GetStack(0)->FindAnimationNode(__L("Material::LedContour2"),GRPANIMATIONCURVETYPE_DIFFUSECOLOR,GRPANIMATIONCURVEAXIS_X);	
	if(node)  node->AttachTo(__L("DiffuseColor"),this);


	node = obj->GetAnimationControl()->GetStack(0)->FindAnimationNode(__L("Model::LedContour2"),GRPANIMATIONCURVETYPE_DIFFUSEFACTOR,GRPANIMATIONCURVECOLOR_DIFFUSEFACTOR);
	if(node) node->AttachTo(__L("DiffuseFactor"),this);

	//obj->PlayAll();	

	red		=255;
	green	=128;
	blue	=64;

	


	return true;
}

/*-------------------------------------------------------------------
//	TESTBASELEDSYNC::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 10:34:46
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASELEDSYNC::EndResources()
{
	return false;
}

/*-------------------------------------------------------------------
//	TESTBASELEDSYNC::IniResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/02/2016 9:54:32
//	
*/
/*-----------------------------------------------------------------*/
bool TESTBASELEDSYNC::Do()
{
	red		=255;
	green	=128;
	blue	=64;
	XDEBUG_PRINTCOLOR(0,__L("Setting color: %d %d %d"),red,green,blue);
	
	if(pca9685)
		{
			pca9685->SetPWM(0, 0, ((red*	intensity*1200)/255)); 
			pca9685->SetPWM(1, 0, ((green*intensity*1200)/255));
			pca9685->SetPWM(2, 0, ((blue*	intensity*1200)/255));			
		}

	return true;
}

/*-------------------------------------------------------------------
//	TESTBASELEDSYNC::SetValue
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/06/2016 16:26:50
//	
//	@return 			bool : 
//
//  @param				type : 
//  @param				subtype : 
//  @param				value : 
//  @param				base : 
*/
/*-----------------------------------------------------------------*/
bool TESTBASELEDSYNC::SetValue(GRPANIMATIONCURVETYPE type, GRPANIMATIONCURVEAXIS subtype, double value,double base)
{
	switch (type)
		{
			case	GRPANIMATIONCURVETYPE_DIFFUSECOLOR:	{ switch(subtype)
																										{
																											case GRPANIMATIONCURVEAXIS_X: this->red		=int(value*float(255));
																																										break;

																											case GRPANIMATIONCURVEAXIS_Y: this->green	=int(value*float(255));
																																										break;

																											case GRPANIMATIONCURVEAXIS_Z:	this->blue	=int(value*float(255));
																																										break;

																																					default : break;
																										}				
																								}
																								break;

			case GRPANIMATIONCURVETYPE_DIFFUSEFACTOR:	this->intensity	=(XBYTE)(value*float(255));
																								break;				
		}
																				
	return true;
}