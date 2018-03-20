
/*------------------------------------------------------------------------------------------
//	TESTTEXTANIMATION.CPP
//	
//	text animations
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 25/10/2016 12:02:32
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "testtextanimation.h"
#include "GRPMaterialLibrary.h"
#include "GRPFontManager.h"
#include "GRPScene.h"
#include "GRPCamera.h"
#include "GRPStencilPass.h"

#include "GRPSlotNumber.h"
#include "GRPProfiler.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	TESTBASETEXTANIMATION::TESTBASETEXTANIMATION
*/	
/**	
//	
//	Class Constructor TESTBASETEXTANIMATION
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/11/2016 10:09:12
//	
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTBASETEXTANIMATION::TESTBASETEXTANIMATION(GRPAPPLICATION* xapplication) : TESTBASE(xapplication)
{
	timer=NULL;
}

/*-------------------------------------------------------------------
//	TESTBASETEXTANIMATION::IniResources
*/	
/**	
//	Initializes resources for the demo
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/11/2016 10:17:01
//	
//	@return 			bool : return success
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASETEXTANIMATION::IniResources()
{
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("textanimation"));


	timer = xfactory->CreateTimer();


	GRPMATRIX a,b,c;

	timer->Reset();

	b.LoadIdentity();
	c.LoadIdentity();

	for (int e=999;e>0;e--)
		for (int i=999;i>0;i--)
			{
				a.GetMultiplicationMatrix(&b,&c);
			}

		XDEBUG_PRINTCOLOR(0,__L("999 x 999 : %d"),timer->GetMeasureMilliSeconds());


	for (int e=0;e<999;e++)
		for (int i=0;i<999;i++)
			{
				a.GetMultiplicationMatrix2(b,c);
			}

		XDEBUG_PRINTCOLOR(0,__L("999 x 999 : %d"),timer->GetMeasureMilliSeconds());


	//----------------------------------- material default location
		XPATH xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, __L("Arial Black Italic.ttf"));

	GRPFONTATTRIBUTES att;
										att.back=false;
										att.extrude=-0.02f;
										att.quality=9;
										att.unifynormals=true;

		if(!GRPOBJECTBUILDER::Get()->GetFontManager()->Load(xpath.Get(),		__L("arialblack.ttf"),&att)) //load default
		{
			XDEBUG_PRINTCOLOR(1,__L("TTF file not loaded!"));
			return false;
		}

	xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, __L("arial.ttf"));
		if(!GRPOBJECTBUILDER::Get()->GetFontManager()->Load(xpath.Get(),		__L("arial.ttf"),&att)) //load default
		{
			XDEBUG_PRINTCOLOR(1,__L("TTF file not loaded!"));
			return false;
		}
		
		//------------------------ load base

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	xpath.Create( XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("base.FBX"));		
	GRPOBJECT* obj=NULL;
	obj = LoadObject(obj,xpath.Get());
	if(!obj)		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}	
	scene->Add(obj);

	//*/
	//----------------------------------- Cargar un material	
		
	PathManagement(__L("fontttf"));
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("oro_512.tga"));	XDEBUG_PRINTCOLOR(1,__L("%s"),xpath.Get());

	GRPMATERIAL* material=0;

	GRPMATERIAL*	rojo=GRPMATERIALLIBRARY::Get()->Create(__L("Rojo"));
								rojo->GetAttributes()->SetDiffuse(GRPCOLOR::RED);
	GRPMATERIAL*	amarillo=GRPMATERIALLIBRARY::Get()->Create(__L("Amarillo"));
								amarillo->GetAttributes()->SetDiffuse(GRPCOLOR::YELLOW);
	GRPMATERIAL*	verde=GRPMATERIALLIBRARY::Get()->Create(__L("Verde"));
								verde->GetAttributes()->SetDiffuse(GRPCOLOR::GREEN);						

	if(!GRPMATERIALLIBRARY::Get()->Load(xpath.Get(),	__L("oro_512.tga")))
		{
			XDEBUG_PRINTCOLOR(1,__L("Error loading Texture data"));
			return false;
		}	

	GRPPASS* mainpass=scene->GetPass();
					 mainpass->AddType(GRPPASS_TYPE_STENCIL);
					 mainpass->SetStencilTestIsActive(true);					 
					 mainpass->SetClear								(GRPPASS_CLEAR_FLAG_STENCIL);									
					 mainpass->SetStencilTestIsActive	(false);
					
									interpolator=new XINTERPOLATOR();
									interpolator->SetType(XINTERPOLATOR_TYPE_LIMIT);
									interpolator->SetSpeed(1.24f);
									interpolator->SetFactorType(XINTERPOLATOR_FACTOR_RELATIVE);									


	GRPSLOTNUMBER*	slot=new GRPSLOTNUMBER(1); 
									slot->Init(3,scene);
									slot->interpolator=interpolator;
									slot->GetObj()->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
									slot->GetObj()->SetMaterial(GRPMATERIALLIBRARY::Get()->Select(__L("Rojo")));									

										slots.Add(slot);
																
									GRPNODE*		node		= obj->FindNode							(__L("Model::Point_jackpot_dec"));
									GRPELEMENT* element	= obj->FindElementByNodeName(__L("Model::loc_jackpot_dec"));
																				obj->FindNode							(__L("Model::loc_jackpot_dec"))->SetVisible(false);
															element->SetVisible(false);															
									slot->GetObj()->GetNode()->SetParent(node);
									
									GRPSTENCILPASS* pass=slot->GetStencilPass();

									slot=new GRPSLOTNUMBER(2);									
									slot->Init(4,scene,__L("arialblack.ttf"),pass);	
									slot->interpolator=interpolator;
									slot->GetObj()->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
									slot->GetObj()->SetMaterial(GRPMATERIALLIBRARY::Get()->Select(__L("Rojo")));
									slot->SetMultiplicator(1000);									

									
															node		= obj->FindNode							(__L("Model::Point_jackpot"));
															element	= obj->FindElementByNodeName(__L("Model::loc_jackpot"));
																				obj->FindNode							(__L("Model::loc_jackpot"))->SetVisible(false);
															element->SetVisible(false);															
									slot->GetObj()->GetNode()->SetParent(node);
									slots.Add(slot);
									//*/								
								
									slot=new GRPSLOTNUMBER(3);
									slot->Init(3,scene,__L("arialblack.ttf"),pass);
									slot->interpolator=interpolator;
									slot->SetCurrent(0);
									slot->GetObj()->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
									slot->GetObj()->SetMaterial(GRPMATERIALLIBRARY::Get()->Select(__L("Amarillo")));
									
									
															node		=	obj->FindNode(__L("Model::Point_minijackpot_dec"));
															element	=	obj->FindElementByNodeName(__L("Model::loc_minijackpot_dec"));
																				obj->FindNode(__L("Model::loc_minijackpot_dec"))->SetVisible(false);
															element->SetVisible(false);									
									slot->GetObj()->GetNode()->SetParent(node);
									//*/
									slots.Add(slot);

									slot=new GRPSLOTNUMBER(4);
									slot->Init(3,scene,__L("arialblack.ttf"),pass);
									slot->interpolator=interpolator;
									slot->SetCurrent(0);
									slot->SetMultiplicator(1000);
									slot->GetObj()->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
									slot->GetObj()->SetMaterial(GRPMATERIALLIBRARY::Get()->Select(__L("Amarillo")));
									
									
															node		=	obj->FindNode(__L("Model::Point_minijackpot"));
															element	=	obj->FindElementByNodeName(__L("Model::loc_minijackpot"));
																				obj->FindNode(__L("Model::loc_minijackpot"))->SetVisible(false);
															element->SetVisible(false);									
									slot->GetObj()->GetNode()->SetParent(node);
									//*/
									//slot->GetObj()->GetNode()->SetPosition(0.0f,-0.3f,0.0f);
					 
									slots.Add(slot);
																				
									slot=new GRPSLOTNUMBER(5);
									slot->Init(3,scene,__L("arialblack.ttf"),pass);
									slot->interpolator=interpolator;									
									slot->GetObj()->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
									slot->GetObj()->SetMaterial(GRPMATERIALLIBRARY::Get()->Select(__L("Verde")));
									//slot->GetObj()->GetNode()->SetPosition(0.0f,-0.6f,0.0f);

															node		=	obj->FindNode(__L("Model::Point_microjackpot_dec"));
															element	=	obj->FindElementByNodeName(__L("Model::loc_microjackpot_dec"));
																				obj->FindNode(__L("Model::loc_microjackpot_dec"))->SetVisible(false);
															element->SetVisible(false);									
									slot->GetObj()->GetNode()->SetParent(node);
									slots.Add(slot);	
									

									slot=new GRPSLOTNUMBER(6);
									slot->Init(2,scene,__L("arialblack.ttf"),pass);
									slot->SetMultiplicator(1000);
									slot->interpolator=interpolator;
									slot->GetObj()->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));
									slot->GetObj()->SetMaterial(GRPMATERIALLIBRARY::Get()->Select(__L("Verde")));
									
															node		=	obj->FindNode(__L("Model::Point_microjackpot"));
															element	=	obj->FindElementByNodeName(__L("Model::loc_microjackpot"));
																				obj->FindNode(__L("Model::loc_microjackpot"))->SetVisible(false);
															element->SetVisible(false);
									//slot->GetObj()->GetNode()->SetPosition(0.0f,-0.6f,0.0f);
									slot->GetObj()->GetNode()->SetParent(node);
									slots.Add(slot);	
									

 //------------- camera aim
/*
	GRPCAMERA*	cam=scene->SelectCamera(scene->GetCamera());
							cam=xapplication->GetMainScene()->GetCamera();
							cam->SetPosition(	0.0,0.0,3.0);
							cam->modified=true;
							cam->Update();							
							*/

	/*
	scene->GetVisualDebugger()->DebugSize=0.00001f;
	scene->GetVisualDebugger()->CreateScene(GRPVISUALDEBUGGER_FLAG_BBS | GRPVISUALDEBUGGER_FLAG_LOCATORS);  
	scene->ShowDebug(false);
	*/

	for (XDWORD e=0;e<slots.GetSize();e++)
			slots.Get(e)->interpolator->SetTarget(2000);

	return true;
}

/*-------------------------------------------------------------------
//	TESTBASETEXTANIMATION::EndResources
*/	
/**	
//	Release resources
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/11/2016 10:17:28
//	
//	@return 			bool : success
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASETEXTANIMATION::EndResources()
{
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASETEXTANIMATION::Do
*/	
/**	
//	Main Loop
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/11/2016 10:17:44
//	
//	@return 			bool : success
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASETEXTANIMATION::Do()
{
	XQWORD delta=timer->GetMeasureMilliSeconds();
	for (XDWORD e=0;e<slots.GetSize();e++)
							   slots.Get(e)->Update(delta);


	this->interpolator->Update(delta);
	timer->Reset();

	return true;
}

/*-------------------------------------------------------------------
//	TESTBASETEXTANIMATION::InputManagement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/11/2016 10:18:11
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASETEXTANIMATION::InputManagement()
{	
	if(keyboard)		
	if(keyboard->GetButton(INPBUTTON_ID_F11))
	if(keyboard->GetButton(INPBUTTON_ID_F11)->IsPressedWithRelease()) 			
		for (XDWORD e=0;e<slots.GetSize();e++)
			slots.Get(e)->interpolator->SetTarget(2000);

	if(keyboard)		
	if(keyboard->GetButton(INPBUTTON_ID_F10))
	if(keyboard->GetButton(INPBUTTON_ID_F10)->IsPressedWithRelease()) 			
		for (XDWORD e=0;e<slots.GetSize();e++)
			slots.Get(e)->interpolator->SetTarget(0);

	return TESTBASE::InputManagement();	
}

/*-------------------------------------------------------------------
//	TESTBASETEXTANIMATION::Notify
*/	
/**	
//	Message notification
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/11/2016 10:20:02
//	
//	@return 			bool			: success
//
//  @param				msg				: GRPMSG message type
//  @param				notifier	: GRPSUBJECT* subject emitter
*/
/*-----------------------------------------------------------------*/
bool TESTBASETEXTANIMATION::Notify(GRPMSGTYPE msg, GRPSUBJECT* notifier)
{
	return true;
}

