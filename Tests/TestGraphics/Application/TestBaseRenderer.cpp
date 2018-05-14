
/*------------------------------------------------------------------------------------------
//	TESTBASERENDERER.CPP
//	
//	tests renderer
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 03/08/2016 14:38:25
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "TestBaseRenderer.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XApplication.h"

#include "GRPFile3DFBX.h"
#include "GRPFile3DOBJ.h"
#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPVisualDebugger.h"
#include "GRPViewport.h"


#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	TESTBASERENDEROBJECT::TESTBASERENDEROBJECT
*/	
/**	
//	
//	Class Constructor TESTBASERENDEROBJECT
//	
//	@author				Abraham J. Velez
//	@version			13/02/2016 16:27:24
//	
//  @param				inpmanager : 
//  @param				screen : 
//  @param				scene : 
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTBASERENDERER::TESTBASERENDERER(GRPAPPLICATION* xapplication)  :  TESTBASE(xapplication) 
{
	Clean();

	delta = 0;
}




/*-------------------------------------------------------------------
//	TESTBASERENDEROBJECT::~TESTBASERENDEROBJECT
*/	
/**	
//	
//	 Class Destructor TESTBASERENDEROBJECT
//	
//	@author				Abraham J. Velez
//	@version			13/02/2016 16:27:19
//	
*/
/*-----------------------------------------------------------------*/
TESTBASERENDERER::~TESTBASERENDERER()
{


	Clean();
}




/*-------------------------------------------------------------------
//	TESTBASERENDEROBJECT::IniResources
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
bool TESTBASERENDERER::IniResources()
{		
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

			XDEBUG_PRINTCOLOR(0,__L("Size of matrix: %d"),sizeof(GRPMATRIX));

	GRPSCENE*  scene  = xapplication->GetMainScene();
	XPATH			 xpath;

	PathManagement(__L("renderer"));

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("Base.FBX"));		
	GRPOBJECT* obj=NULL;
	obj = LoadObject(obj,xpath.Get());
	if(!obj)		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}	
	scene->Add(obj);

	timer=xfactory->CreateTimer();

	GRPMATRIX m1=scene->GetPass()->GetViewport()->Select();
	GRPMATRIX m2=scene->GetPass()->GetViewport()->Select();
	GRPMATRIX m;

	timer->Reset();

	for (int e=0;e<900000;e++)			
		m.GetMultiplicationMatrix(&m1,&m2);	
		
	XDEBUG_PRINTCOLOR(0,__L("time: %lld"),timer->GetMeasureMicroSeconds());
	timer->Reset();  //time: 42631 723067

	for (int e=0;e<900000;e++)			
		m.GetMultiplicationMatrix(m1,m2);	
		
	XDEBUG_PRINTCOLOR(0,__L("time: %lld"),timer->GetMeasureMicroSeconds());

	GRPELEMENT* element=scene->FindElementByNodeName(__L("Model::Fondo"));
	if (element)
	{
		element->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));
		element->SetRenderPriority(99);
	}
	xfactory->DeleteTimer(timer);
	timer=NULL;




	//scene->PlayAll();		


	//scene->GetVisualDebugger()->CreateScene(GRPVISUALDEBUGGER_FLAG_ALL);  
	//scene->ShowDebug(false);

	return true;
}


/*-------------------------------------------------------------------
//	TESTBASERENDEROBJECT::EndResources
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
bool TESTBASERENDERER::EndResources()
{
	xfactory->DeleteTimer(timer);
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASERENDEROBJECT::Do
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
bool TESTBASERENDERER::Do()
{	
	return true;
}



/*-------------------------------------------------------------------
//	TESTBASERENDEROBJECT::Notify
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
bool TESTBASERENDERER::Notify(GRPMSGTYPE msg, GRPSUBJECT* notifier)
{

	return false;
}
