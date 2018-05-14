/*------------------------------------------------------------------------------------------
// TESTCULLING.CPP
*/
/**
// \class
//
// 
//
//	@author: Diego Martinez Ruiz de Gaona 
//
//	Date of Creation : 07/11/2017 17:32:53 
//	Last Modification : 
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "TestCulling.h"
#include "GRPApplication.h"
#include "GRPMaterialLibrary.h"
#include "GRPLight.h"
#include "GRPFrustum.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

TESTCULLING::TESTCULLING(GRPAPPLICATION* xapplication) : TESTBASE(xapplication)
{
		scene = NULL;
		obj = NULL;
}

bool TESTCULLING::IniResources()
{
		if (!xapplication)										 return NULL;
		if (!xapplication->GetMainScene())		 return NULL;

		scene = xapplication->GetMainScene();
		XPATH			 xpath;

		PathManagement(__L("culling"));

		culling_disabled = true;

		//----------------------------------- material default location		
		xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L(""));
		GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);
		
	

		//-------------------------------- load model	

		xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L(""));
		GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

		xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("culling.fbx"));

				 obj = LoadObject(obj, xpath.Get(), GRPFILE3D_INCLUDE_ALL, GRPFILE3D_VERBOSE_LEVEL_LOW, 0);
		if (!obj){ XDEBUG_PRINTCOLOR(4, __L("Loading FBX Failed!")); return false; }

		scene->Add(obj);

		obj->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Texture")));


		GRPPLANE planetest;
		planetest.Set(GRPVECTOR(0.0, 0.0, 0.0),
									GRPVECTOR(1.0, 0.0, 0.0),
									GRPVECTOR(0.0, 0.5, -1.0));

		GRPVECTOR test(0.0, 1.0, -0.5);
		GLFLOAT f=planetest.distance(test);

		GRPFRUSTUM frutest;
		frutest.setCamInternals(90.0f, 1.33f, 0.1f, 10000.0f);
		frutest.setCamDef(GRPVECTOR(0.0, 0.0, 0.0), GRPVECTOR(0.0, 0.0, -1.0), GRPVECTOR(0.0, 1.0, 0.0));
		
		int is = frutest.pointInFrustum(GRPVECTOR(0.0, 0.0, -0.5));
			if (GRPFRUSTUM_INSIDE == is)
			{
				static int a = 0;
				a++;
			}
			 is = frutest.pointInFrustum(GRPVECTOR(0.0, 0.0, +0.5));
			if (GRPFRUSTUM_INSIDE == is)
			{
				static int a = 0;
				a++;
			}

		//scene->Add(GRPPASSMANAGER::Get()->Create(__L("debug")));
		/*
		if (obj)
		if (obj->GetLogic())
				obj->GetLogic()->GetControllers()->Get(0)->SetVerbose(true);

				obj->PlayAll();
				*/
		return true;
}

/*-------------------------------------------------------------------
//	 TESTCULLING::EndResources
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	11/12/2017 13:06:58
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool TESTCULLING::EndResources()
{
		return true;
}

/*-------------------------------------------------------------------
//	 TESTCULLING::Do
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	11/12/2017 13:07:03
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool TESTCULLING::Do()
{
/*
		if (!culling_disabled)
		{
				for (XDWORD e = 0; e < this->scene->GetPass()->GetRenderOrder()->GetSize(); e++)
						this->scene->GetPass()->GetRenderOrder()->Get(e)->SetIsCulled(false);
				return true;
		}

		for (XDWORD e = 0; e < this->scene->GetPass()->GetRenderOrder()->GetSize(); e++)
		{
				GRPELEMENT* element = this->scene->GetPass()->GetRenderOrder()->Get(e);
				GRPMATRIX mvp				= element->modelviewprojectionmatrix;
				GRPVECTOR position = *element->GetNode()->GetPosition();//element->GetNode()->GetWorldPosition();
				GRPFRUSTUM* frustum = this->scene->GetCamera()->GetFrustum();

				frustum->Update(this->scene->GetCamera());
				element->GetBoundingBox()->Update();
				float radius=element->GetBoundingBox()->radius;

				int isInside = frustum->objectVisible(mvp, &position,0.0f);
				if (isInside == GRPFRUSTUM::OUTSIDE)
				{
						element->SetIsCulled(true);
				}
				else
				{
						element->SetIsCulled(false);
				}

		}
		*/

	this->scene->GetCamera()->SetViewport(scene->GetViewport());

	for (XDWORD e = 0; e < this->scene->GetPass()->GetRenderOrder()->GetSize(); e++)
	{
		GRPELEMENT* element = this->scene->GetPass()->GetRenderOrder()->Get(e);
		GRPMATRIX		mvp			= element->modelviewprojectionmatrix;
		GRPVECTOR		position= *element->GetNode()->GetPosition();//element->GetNode()->GetWorldPosition();
		GRPFRUSTUM* frustum = this->scene->GetCamera()->GetFrustum();

		frustum->Update(this->scene->GetCamera());
		element->GetBoundingBox()->Update();
		float radius = element->GetBoundingBox()->radius;

		//int isInside = frustum->objectVisible(mvp, &position, 0.0f);
		int isInside = frustum->pointInFrustum(element->GetNode()->GetWorldPosition()); 
		if (isInside == GRPFRUSTUM_OUTSIDE)
		{
			element->SetIsCulled(true);
		}
		else
		{
			element->SetIsCulled(false);
		}

	}
		return true;
}


bool TESTCULLING::InputManagement()
{
		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_1))
						if (keyboard->GetButton(INPBUTTON_ID_1)->GetState() == INPBUTTON_STATE_PRESSED)
								this->culling_disabled = true;

		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_2))
						if (keyboard->GetButton(INPBUTTON_ID_2)->GetState() == INPBUTTON_STATE_PRESSED)
								this->culling_disabled = false;

		return TESTBASE::InputManagement();
}

bool TESTCULLING::Notify(GRPMSGTYPE,GRPSUBJECT*)
{
		return false;
}