
//------------------------------------------------------------------------------------------
//	TESTGRAPHICS.CPP
//	
//	Generic Test Graphics
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 28/06/2010
//	Last Mofificacion	:	
//	
//	GEN (C) Copyright  (All right reserved)		 			 
//------------------------------------------------------------------------------------------	


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XPath.h"
#include "XRand.h"
#include "XFactory.h"
#include "XSystem.h"
#include "XTimer.h"
#include "XDir.h"
#include "XDebug.h"
#include "XLog.h"

#include "XApplication.h"
#include "XPublisher.h"
#include "XDebug.h"

#include "INPDevice.h"
#include "INPFactory.h"
#include "INPManager.h"

#include "GRPOpenGL.h"
#include "GRPFactory.h"
#include "GRPScreen.h"
#include "GRPConsole.h"
#include "GRPObjectBuilder.h"
#include "GRPFont.h"
#include "GRPFontManager.h"
#include "GRPBitmap.h"
#include "GRPFileBitmapBMP.h"
#include "GRPFileBitmapJPG.h"
#include "GRPBufferPixelFormatRGBPacked.h"
#include "GRPBufferOpenglRender.h"
#include "GRPScene.h"
#include "GRPContext.h"
#include "GRPRenderer.h"
#include "GRPCamera.h"
#include "GRPProfiler.h"
#include "GRPVisualDebugger.h"
#include "GRPRenderer.h"
#include "GRPViewport.h"

#include "SNDAnimationListener.h"
#include "SNDManager.h"

#include "TestBase.h"
#include "TestBaseRenderObject.h"
#include "TestBaseFontTTF.h"
#include "TestBaseMergeAnimationLayers.h"
#include "TestBaseSpline.h"
#include "TestBaseSprites.h"
#include "TestBaseAnimationMilestone.h"
#include "TestBaseConsole.h"
#include "TestBaseOBJMerging.h"
#include "TestBaseAudio.h"
#include "TestBaseUI.h"
#include "TestBaseGGF.h"
#include "TestBaseDIOFunctions.h"
#include "TestBaseLedSync.h"
#include "TestBaseFacerig.h"
#include "TestBaseMultiRender.h"
#include "TestBasePhy.h"
#include "TestBaseBlending.h"
#include "TestGraphics.h"
#include "TestBaseShadows.h"
#include "TestBaseVideo.h"
#include "TestBaseCubeMap.h"
#include "TestBasePasses.h"
#include "TestBaseRenderer.h"
#include "TestBaseReflection.h"
#include "TestBaseLoader.h"
#include "TestBaseObjectBuilder.h"
#include "TestBaseScreen.h"
#include "TestCalibration.h"
#include "TestTextAnimation.h"
#include "TestLayers.h"
#include "TestInput.h"
#include "TestBaseEnvironmentMapping.h"
#include "TestDivs.h"
#include "TestCulling.h"
#include "TestPBR_HDR.h"
#include "TestTexture.h"

#include "XMemory.h"


	
//---- GENERAL VARIABLE --------------------------------------------------------------------

	
//---- CLASS MEMBERS -----------------------------------------------------------------------

//-------------------------------------------------------------------
//  XAPPLICATION::Create
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/08/2002 11:02:38
//	
//	@return 			XAPPLICATIONBASE* : 
//	*/
//-------------------------------------------------------------------
XAPPLICATION* XAPPLICATION::Create()
{
	TESTGRAPHICS* testgraphics;
	testgraphics = new TESTGRAPHICS();

	return testgraphics;
}



//-------------------------------------------------------------------
//  TESTGRAPHICS::Ini
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/08/2002 13:17:31
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool TESTGRAPHICS::OnConfig()
{
		SetVersion				(TESTGRAPHICS_VERSION, TESTGRAPHICS_SUBVERSION, TESTGRAPHICS_SUBVERSIONERR);
		SetApplicationName(TESTGRAPHICS_NAMEAPP);

		positionx				= 0;
		positiony				= 0;
		resolutionx			= 640;
		resolutiony			= 480;
		isfullscreen		= false;		

		configurationfile = TESTGRAPHICS_CFGNAMEFILE;
		return true;
}
 

/*-------------------------------------------------------------------
//	TESTGRAPHICS::Update
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			05/09/2016 13:51:50
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTGRAPHICS::OnUpdate()
{
	if (this->testbase)
	{
	this->testbase->InputManagement();
	return this->testbase->Do();
	}
	return true;
}


//-------------------------------------------------------------------
//  TESTGRAPHICS::FirstUpdate
/**
//	
//	@author        Diego Martinez Ruiz de Gaona
//	@version       20/2/2003   16:26:12
//	
//	@return        bool : 
**/
//-------------------------------------------------------------------
bool TESTGRAPHICS::OnInit()
{	
	GRPPROFILER::showFrameEvery = 1;

	if (testtoexec==-1)
	{
		testtoexec = TESTGRAPHICSTEST_DIVS;
	}  
	testbase = NULL;
	switch(testtoexec)
		{
			case TESTGRAPHICSTEST_INPUT										: testbase  = (TESTBASE*)new TESTBASEINPUT								(this);		break; 
			case TESTGRAPHICSTEST_RENDEROBJECT						: testbase  = (TESTBASE*)new TESTBASERENDEROBJECT					(this);		break; 
			case TESTGRAPHICSTEST_FONTTTF									:	testbase  = (TESTBASE*)new TESTBASEFONTTTF							(this);		break; 
			case TESTGRAPHICSTEST_MERGEANIMATIONLAYERS		: testbase  = (TESTBASE*)new TESTBASEMERGEANIMATIONLAYERS	(this);		break; 
			case TESTGRAPHICSTEST_SPLINE									: testbase  = (TESTBASE*)new TESTBASESPLINE								(this);		break; 
			case TESTGRAPHICSTEST_MILESTONE								: testbase  = (TESTBASE*)new TESTBASEANIMATIONMILESTONE		(this);		break; 
			case TESTGRAPHICSTEST_OBJMERGING							: testbase  = (TESTBASE*)new TESTBASEOBJMERGING						(this);		break; 			
			case TESTGRAPHICSTEST_FACERIG									: testbase  = (TESTBASE*)new TESTBASEFACERIG							(this);		break; //ok , bad graphics	
			case TESTGRAPHICSTEST_MULTIRENDER							: testbase  = (TESTBASE*)new TESTBASEMULTIRENDER					(this);		break; 
			case TESTGRAPHICSTEST_BLENDING								: testbase  = (TESTBASE*)new TESTBASEBLENDING							(this);		break; 			
			case TESTGRAPHICSTEST_SHADOWS									: testbase  = (TESTBASE*)new TESTBASESHADOWS							(this);		break; 			
			case TESTGRAPHICSTEST_CUBEMAP									: testbase  = (TESTBASE*)new TESTBASECUBEMAP							(this);		break; 
			case TESTGRAPHICSTEST_PASSES									: testbase  = (TESTBASE*)new TESTBASEPASSES								(this);		break; 
			case TESTGRAPHICSTEST_REFLECTION							:	testbase  = (TESTBASE*)new TESTBASEREFLECTION						(this);		break; 
			case TESTGRAPHICSTEST_OBJECTBUILDER						: testbase	= (TESTBASE*)new TESTBASEOBJECTBUILDER				(this);   break; 
			case TESTGRAPHICSTEST_SCREEN									: testbase  = (TESTBASE*)new TESTBASESCREEN								(this);   break; 
			case TESTGRAPHICSTEST_CONSOLE									: testbase  = (TESTBASE*)new TESTBASECONSOLE							(this);		break; 
			case TESTGRAPHICSTEST_LAYERS									: testbase	= (TESTBASE*)new TESTLAYERS										(this);		break; //ok , bad graphics			
			case TESTGRAPHICSTEST_GGF											: testbase  = (TESTBASE*)new TESTBASEGGF									(this);		break; 
			case TESTGRAPHICSTEST_AUDIO										: testbase	= (TESTBASE*)new TESTBASEAUDIO								(this);		break; 						
			case TESTGRAPHICSTEST_UI											: testbase  = (TESTBASE*)new TESTBASEUI										(this);		break; 
			case TESTGRAPHICSTEST_DIVS										: testbase  = (TESTBASE*)new TESTDIVS											(this);		break; 
			case TESTGRAPHICSTEST_CULLING									:	testbase	= (TESTBASE*)new TESTCULLING									(this);		break; 
			case TESTGRAPHICSTEST_HDR											:	testbase	= (TESTBASE*)new TESTHDR											(this);		break; 
			case TESTGRAPHICSTEST_TEXTURE									:	testbase  = (TESTBASE*)new TESTTEXTURE(this);		break;


					//---- fails

					/* the scale on the phy world is wrong*/
			case TESTGRAPHICSTEST_PHY											:	testbase  = (TESTBASE*)new TESTBASEPHY									(this);		break; //scale fails

				 /* crashes when error on load, wich does not on production apps*/
			case TESTGRAPHICSTEST_LOADER									:	testbase  = (TESTBASE*)new TESTBASELOADER								(this);		break; //fail

				/* crashes on android*/
			case TESTGRAPHICSTEST_VIDEO										: testbase  = (TESTBASE*)new TESTBASEVIDEO								(this);		break; //	crash android				

				/* always crashing*/
			case TESTGRAPHICSTEST_DIOFUNCTIONS						: testbase  = (TESTBASE*)new TESTBASEDIOFUNCTIONS					(this);		break; //	crash 				

				/* the effect is not working */
			case TESTGRAPHICSTEST_TEXTANIMATION						: testbase  = (TESTBASE*)new TESTBASETEXTANIMATION				(this);		break; //  fails

				/* the environment mapping only holds depth space data */
			case TESTGRAPHICSTEST_ENVMAP									: testbase  = (TESTBASE*)new TESTBASEENVIRONMENTMAPPING		(this);		break; //fail
			 
		}

	if(!testbase)										return false;
	if(!testbase->IniResources())		return false;

	return true;
}


//-------------------------------------------------------------------
//  TESTGRAPHICS::End
/**
//
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/08/2002 13:19:16
//	
//	@return 			bool : 
//	*/
//-------------------------------------------------------------------
bool TESTGRAPHICS::OnEnd()
{	
	if (testbase)
	{
			testbase->EndResources();
			delete(testbase);
	}
	return true;
}



