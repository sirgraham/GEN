
/*------------------------------------------------------------------------------------------
//	TESTBASESPLINE.CPP
//	
//	Test Base Spline
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
#include "GRPObjectBuilder.h"
#include "GRPShape.h"
#include "GRPSpline.h"
#include "GRPRenderer.h"
#include "GRPVisualDebugger.h"
#include "GRPObjectBuilderAlgorithmStroke.h"
#include "GRPObjectBuilderAlgorithmTriangulationEar.h"

#include "GRPFontManager.h"

#include "TestBaseSpline.h"


#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


XDWORD flags = GRPBOXSIDEFLAG_ALL;

/*-------------------------------------------------------------------
//	TESTBASESPLINE::IniResources
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			30/05/2016 22:31:29
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASESPLINE::IniResources()
{	
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("spline"));

	GRPOBJECTBUILDER::Get()->GetFontManager()->Init();	
	GRPFONT* font = GRPOBJECTBUILDER::Get()->GetFontManager()->GetFontByFamily(__L("Arial"), GRPFONT_STYLE_REGULAR);
	/*
	GRPSPLINE*	spline = new GRPSPLINE();
	if(!spline) return false;
							
	spline->Add(GRPANCHORPOINTTYPE_CUBIC_BEZIER, GRPPOINT(0.0,0.0,0.0));
	spline->Add(GRPANCHORPOINTTYPE_CONTROLPOINT, GRPPOINT(0.0,1.0,0.0));
	spline->Add(GRPANCHORPOINTTYPE_CONTROLPOINT, GRPPOINT(0.0,1.0,0.0));
	spline->Add(GRPANCHORPOINTTYPE_CUBIC_BEZIER, GRPPOINT(1.0,1.0,0.0));

	GRPSPLINE* psp =GRPOBJECTBUILDER::Get()->ProcessSplineToLines(spline,50);
	if(!psp) return false;
	
	XVECTOR<GRPPOINT*> v;

	for(XDWORD e=0; e<psp->points.GetSize(); e++)
		{
			v.Add(&psp->points.Get(e)->value);
		}

	GRPOBJECT* obj = new GRPOBJECT();
	if(obj)
		{
			GRPELEMENT* element = scene->visualdebugger->ConstructSpline(&v);	
			if(element)
				{
					obj->AddElement(element);
					obj->SetNode(scene->CreateNode());
					element->SetNode(obj->GetNode());
					obj->GetNode()->SetScale(25.0f);
					scene->Add(obj);

					obj->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"BAS"));
				}
		}
		*/
	if (!font)
		return false;

	GRPOPENGLCTRL::CULLING::SetBackFaceCulling(false);
	
	scene->GetCamera()->SetPosition(0.0, 0.0, 1.0);
	scene->GetCamera()->Setinterestposition(0.0, 0.0, 0.0);

	
	GRPNODE* node = scene->CreateNode();
	GRPSPLINE* spline = GRPOBJECTBUILDER::Get()->CreateBoxSpline(
																						GRPPOINT(0.0, 0.0, 0.0), 
																						GRPPOINT(1.0, 1.0, 0.0),
																						0.1f,
																						flags);
	GRPSHAPE* shape = new GRPSHAPE();
	shape->AddContour(spline);

/*
						GRPOBJECTBUILDERALGORITHM* algorithm = new GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR(GRPOBJECTBUILDER::Get());
						
						
	GRPELEMENT* element = GRPOBJECTBUILDER::Get()->TriangulizeShape(shape, algorithm);
	

	element->SetDefaultColor(GRPCOLOR::RED);
	element->SetNode(node);
	element->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("2DShadow")));
	scene->Add(element);


	node->SetPosition(-0.2f, -0.2f, 0.0f);
	node->SetScale(0.5);
*/
	/*
	GRPOBJECTBUILDERALGORITHMSTROKE*	strokealgorithm = new GRPOBJECTBUILDERALGORITHMSTROKE(GRPOBJECTBUILDER::Get());
																		strokealgorithm->SetInnerWidth(0.1f);
																		strokealgorithm->SetOuterWidth(0.1f);
																		strokealgorithm->SetJointType(GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE_MITTER);
	GRPELEMENT* borderelement = GRPOBJECTBUILDER::Get()->TriangulizeShape(shape, strokealgorithm);
	borderelement->SetNode(node);
	borderelement->SetDefaultColor(GRPCOLOR::WHITE);
	GRPSHADERPROGRAM* program = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("2DShadow"));
	GRPSHADER::UseShader(program);
	borderelement->SetProgram(program);
	program->Set("gradient_factor", 0.8f);
	borderelement->SetBlending(true);
	GRPOPENGLCTRL::BLENDING::SetAlphaBlending(true);
	scene->Add(borderelement);
	*/

	//----- font test
	
	scene->GetCamera()->SetPosition(0.0, 0.0, 10.0);
	GRPSHAPE* y = font->GetGlyphShape(__C('y'));	
	GRPOBJECT* obj = new GRPOBJECT();
	if (obj)
	{
					obj->SetNode(scene->CreateNode());
					GRPOBJECTBUILDER::Get()->AddShapeAsElement(obj, y);					
					glClearColor(1.0, 1.0, 1.0, 1.0);
					glLineWidth(1.0);
					obj->GetNode()->SetScale(30.0);
					obj->GetNode()->SetPosition(-2.0,0.0,0.0);					
	}

	obj->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Black")));
	scene->Add(obj);

	return true;
}





/*-------------------------------------------------------------------
//	TESTBASESPLINE::Do
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			30/05/2016 22:31:18
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASESPLINE::Do()
{
		static int e = 0;
		static int frame = 0;

		//frame++;
		if (frame > 60)
		{
				frame = 0;
				flags = e++;
				if (e > GRPBOXSIDEFLAG_ALL)
						e = 0;

				XDEBUG_PRINTCOLOR(0, __L("Flags: %x"), flags);
				EndResources();
				IniResources();
		}
		/*
		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_1))
						if (keyboard->GetButton(INPBUTTON_ID_1)->GetState() == INPBUTTON_STATE_RELEASED)
						{

							  flags = GRPBOXSIDEFLAG_TOP;
								EndResources();
								IniResources();								
						}
		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_2))
						if (keyboard->GetButton(INPBUTTON_ID_2)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								flags = GRPBOXSIDEFLAG_LEFT;
								EndResources();
								IniResources();
						}
		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_3))
						if (keyboard->GetButton(INPBUTTON_ID_3)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								flags = GRPBOXSIDEFLAG_BOTTOM;
								EndResources();
								IniResources();
						}
		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_4))
						if (keyboard->GetButton(INPBUTTON_ID_4)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								flags = GRPBOXSIDEFLAG_RIGHT;
								EndResources();
								IniResources();
						}
		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_5))
						if (keyboard->GetButton(INPBUTTON_ID_5)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								flags = GRPBOXSIDEFLAG_LEFT | GRPBOXSIDEFLAG_BOTTOM;
								EndResources();
								IniResources();
						}
		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_6))
						if (keyboard->GetButton(INPBUTTON_ID_6)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								flags = GRPBOXSIDEFLAG_BOTTOM | GRPBOXSIDEFLAG_RIGHT;
								EndResources();
								IniResources();
						}
		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_7))
						if (keyboard->GetButton(INPBUTTON_ID_7)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								flags = GRPBOXSIDEFLAG_RIGHT | GRPBOXSIDEFLAG_TOP;
								EndResources();
								IniResources();
						}
		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_8))
						if (keyboard->GetButton(INPBUTTON_ID_8)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								flags = GRPBOXSIDEFLAG_LEFT | GRPBOXSIDEFLAG_TOP;
								EndResources();
								IniResources();
						}
		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_9))
						if (keyboard->GetButton(INPBUTTON_ID_9)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								flags = GRPBOXSIDEFLAG_LEFT | GRPBOXSIDEFLAG_TOP | GRPBOXSIDEFLAG_BOTTOM;
								EndResources();
								IniResources();
						}
		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_0))
						if (keyboard->GetButton(INPBUTTON_ID_0)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								flags = GRPBOXSIDEFLAG_RIGHT | GRPBOXSIDEFLAG_TOP | GRPBOXSIDEFLAG_BOTTOM;
								EndResources();
								IniResources();
						}
		if (keyboard)
				if (keyboard->GetButton(INPBUTTON_ID_BACK_SPACE))
						if (keyboard->GetButton(INPBUTTON_ID_BACK_SPACE)->GetState() == INPBUTTON_STATE_RELEASED)
						{
								flags = GRPBOXSIDEFLAG_RIGHT | GRPBOXSIDEFLAG_TOP | GRPBOXSIDEFLAG_LEFT;
								EndResources();
								IniResources();
						}
						*/
	return true;
}


/*-------------------------------------------------------------------
//	TESTBASESPLINE::Notify
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 13:50:50
//	
//	@return 			bool : 
//
//  @param				GRPMSGTYPE : 
//  @param				GRPSUBJECT* : 
*/
/*-----------------------------------------------------------------*/
bool TESTBASESPLINE::Notify(GRPMSGTYPE, GRPSUBJECT*)
{
	return false;
}


/*-------------------------------------------------------------------
//	TESTBASESPLINE::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 13:53:33
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASESPLINE::EndResources()
{  
		this->xapplication->GetMainScene()->GetPass()->ClearAllObjects();
		return true;
}