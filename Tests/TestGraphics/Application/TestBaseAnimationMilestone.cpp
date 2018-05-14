
/*------------------------------------------------------------------------------------------
//	TESTBASEANIMATIONMILESTONE.CPP
//	
//	Test Base Animation Milestone
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 30/05/2016 16:19:29
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XApplication.h"
#include "INPManager.h"

#include "GRPFile3DFBX.h"
#include "GRPFile3DOBJ.h"
#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPObjectBuilder.h"
#include "GRPSpline.h"
#include "GRPRenderer.h"
#include "GRPVisualDebugger.h"
#include "GRPProfiler.h"

#include "TestBaseAnimationMilestone.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	TESTBASEANIMATIONMILESTONE::IniResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			30/05/2016 22:31:29
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEANIMATIONMILESTONE::IniResources()
{	
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("animationmilestone"));

	XPATH xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	obj=NULL;

	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("TestMilestone.FBX"));	
	if(!(obj=LoadObject(obj,xpath.Get())))		
		{
			XDEBUG_PRINTCOLOR(4,__L("\t\tLoading FBX Failed!"));
			return false;
		}

	obj->MergeAnimations();
	obj->GetAnimationControl()->SetStateStack(0,true)->Rewind();
	
		scene->Add(obj);

	int n=12;
	int e=0;

	for (e=0;e<12;e++)
	{
	GRPANIMATIONSTACKMILESTONE* subjackpotprize=new GRPANIMATIONSTACKMILESTONE();
	int start	=0;
	int end		=0;
	int frame =0;

	switch(e)
	{	
	case 0:		start=0;	end=36; break;
	case 1:		start=3;	end=39; break;
	case 2:		start=6;	end=42; break;
	case 3:		start=9;	end=45; break;
	case 4:		start=12; end=48; break;
	case 5:		start=15; end=51; break;
	case 6:		start=18; end=54; break;
	case 7:		start=21; end=57; break;
	case 8:		start=24; end=60; break;
	case 9:		start=27; end=63; break;
	case 10:	start=30; end=66; break;
	case 11:	start=33;	end=69; break;
	case 12:	start=36;	end=72; break;
	default:   break;
	}

										subjackpotprize->SetStartFrame	((float)start);
										subjackpotprize->SetEndFrame		((float)end);
										subjackpotprize->SetType				(GRPANIMATIONSTACKMILESTONE::DECCELERATE);
										subjackpotprize->SetPasses			(1);
										subjackpotprize->SetTension			(0.0f);
										subjackpotprize->Enable					(false);

										obj->GetAnimationControl()->GetStack(0)->AddMilestone(subjackpotprize);
	}
	//*/
	/*
	for (XDWORD e=0;e<12;e++)
	{

	GRPANIMATIONSTACKMILESTONE* jackpotprize=new GRPANIMATIONSTACKMILESTONE();
															jackpotprize->SetStartFrame	(1.0f+(e*2.0f));
															jackpotprize->SetEndFrame		((float)(int(3.0f+(e*2.0f))%24));
															jackpotprize->SetType				(GRPANIMATIONSTACKMILESTONE::DECCELERATE);
															jackpotprize->SetPasses			(6);
															jackpotprize->SetTension		(0.0f);
															jackpotprize->Enable				(false);

															scene->GetAnimationControl()->GetStack(0)->AddMilestone(jackpotprize);
	}
	*/
	/*
	GRPANIMATIONSTACKMILESTONE* jackpotprize=new GRPANIMATIONSTACKMILESTONE();
															jackpotprize->SetStartFrame	(18.0);
															jackpotprize->SetEndFrame		(18.0);
															jackpotprize->SetType				(GRPANIMATIONSTACKMILESTONE::DECCELERATE);
															jackpotprize->SetPasses			(1);
															jackpotprize->SetTension		(0.0f);
															jackpotprize->Enable				(false);

															scene->GetAnimationControl()->GetStack(0)->AddMilestone(jackpotprize);
	
	scene->GetVisualDebugger()->CreateScene(GRPVISUALDEBUGGER_FLAG_ALL);  scene->ShowDebug(false);	
	scene->GetAnimationControl()->GetStack(0)->GetLayer(0)->SetLoopMode					(GRPANIMATIONLOOPMODE_REPEAT);
	*/
	obj->PlayAll();	

	//scene->GetAnimationControl()->GetStack(0)->GetLayer(0)->SetFrame(18);
	//scene->GetAnimationControl()->GetStack(0)->GetLayer(0)->Stop();
	
	GRPELEMENT*		element=obj->FindElementByNodeName(__L("Model::Quesito_12"));
								GRPSHADERPROGRAM* p=GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"DIT");
								element->SetProgram(p);
	
	//obj->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"DIT"));

	XDEBUG_PRINTCOLOR(0,__L("Resources Initialized"));

	OPENGLCHECKERROR(__L("Resources Initialized"));
	return true;
}





/*-------------------------------------------------------------------
//	TESTBASEANIMATIONMILESTONE::Do
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			30/05/2016 22:31:18
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEANIMATIONMILESTONE::Do()
{
	OPENGLCHECKERROR(__L(" TESTBASEANIMATIONMILESTONE::Do()"));

	GRPSHADERPROGRAM* shader=GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"DIT");	
	static float i=0;
	i+=0.01f;

	GRPSHADER::UseShader(shader);
	GLINT loc=glGetUniformLocation(shader->GetID(),"interpolator");
	glUniform1f(loc,float(fabs(sin(i))));

	OPENGLCHECKERROR(__L(" TESTBASEANIMATIONMILESTONE::Do()"));
	return true;
}


/*-------------------------------------------------------------------
//	TESTBASEANIMATIONMILESTONE::InputManagement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/06/2016 11:07:43
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	TESTBASEANIMATIONMILESTONE::InputManagement	()
{
	if (!keyboard)
			return TESTBASE::InputManagement();
	
	if(keyboard->GetButton(INPBUTTON_ID_F1))
	if(keyboard->GetButton(INPBUTTON_ID_F1)->IsPressedWithRelease()) 
	{
		obj->GetAnimationControl()->GetStack(0)->DisableMilestones();
		obj->GetAnimationControl()->SetStateStack(0,true);		
		obj->GetAnimationControl()->GetStack(0)->Play();
		obj->GetAnimationControl()->GetStack(0)->DisableMilestones();
		return true;
	}

	if(keyboard->GetButton(INPBUTTON_ID_F2))
	if(keyboard->GetButton(INPBUTTON_ID_F2)->IsPressedWithRelease()) 
	{
		obj->GetAnimationControl()->GetStack(0)->DisableMilestones();
		obj->GetAnimationControl()->SetStateStack(0,false);		
		return true;
	}

	if(keyboard->GetButton(INPBUTTON_ID_F3))
	if(keyboard->GetButton(INPBUTTON_ID_F3)->IsPressedWithRelease()) 	
	{
		obj->GetAnimationControl()->GetStack(0)->GetMilestones()->Get(0)->Enable(true);		
		return true;
	}

	if(keyboard->GetButton(INPBUTTON_ID_F4))
	if(keyboard->GetButton(INPBUTTON_ID_F4)->IsPressedWithRelease()) 	
	{
		obj->GetAnimationControl()->GetStack(0)->GetMilestones()->Get(1)->Enable(true);		
		return true;
	}

	if(keyboard->GetButton(INPBUTTON_ID_F5))
	if(keyboard->GetButton(INPBUTTON_ID_F5)->IsPressedWithRelease()) 							
	{
		obj->GetAnimationControl()->GetStack(0)->GetMilestones()->Get(2)->Enable(true);		
		return true;
	}
	
	return TESTBASE::InputManagement();
}

/*-------------------------------------------------------------------
//	TESTBASEANIMATIONMILESTONE::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 10:27:28
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	TESTBASEANIMATIONMILESTONE::EndResources()
{
	return true;
}


