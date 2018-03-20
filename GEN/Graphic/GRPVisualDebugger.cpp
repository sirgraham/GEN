
/*------------------------------------------------------------------------------------------
//	GRPVISUALDEBUGGER.CPP
//	
//	Creates visual debug for normals, objects, aabs, locators..
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 08/06/2015 10:18:22
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPVisualDebugger.h"
#include "GRPElement.h"
#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPShaderLibrary.h"
#include "GRPLight.h"
#include "GRPObjectBuilder.h"
#include "GRPSkinning.h"
#include "GRPCamera.h"
#include "GRPAnimation.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::GRPVISUALDEBUGGER
*/	
/**	
//	
//	Class Constructor GRPVISUALDEBUGGER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/06/2015 10:39:16
//	
//  @param				scene : 
*/
/*-----------------------------------------------------------------*/
GRPVISUALDEBUGGER::GRPVISUALDEBUGGER	(GRPSCENE*	scene)
{
	Clean();
	this->scene=scene;
}

/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::Create
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/06/2015 17:25:18
//	
*/
/*-----------------------------------------------------------------*/
void	GRPVISUALDEBUGGER::CreateScene(XDWORD flags)
{
	XDWORD nObjects	=scene->GetObjects()->GetSize(); //object number increases as we add debug objects
	XDWORD nNodes		=scene->GetNodes	()->GetSize(); //node number increases as we add debug objects

	//GRPVISUALDEBUGGER_FLAG_BB | GRPVISUALDEBUGGER_FLAG_CAMERAS | GRPVISUALDEBUGGER_FLAG_LIGHTS | GRPVISUALDEBUGGER_FLAG_DEFORMERS | GRPVISUALDEBUGGER_FLAG_NORMALS | GRPVISUALDEBUGGER_FLAG_ANIMATION | GRPVISUALDEBUGGER_FLAG_AABBS | GRPVISUALDEBUGGER_FLAG_LOCATORS


	for (XDWORD e=0;e<nObjects;e++)
	{
		if (flags & GRPVISUALDEBUGGER_FLAG_BBS)
		{
		this->CreateBoundingBox	(scene->GetObjects()->Get(e));
		}

		if (flags & GRPVISUALDEBUGGER_FLAG_DEFORMERS)
		for (XDWORD i=0;i<scene->GetObjects()->Get(e)->GetElements()->GetSize();i++)
		{
			if (scene->GetObjects()->Get(e)->GetElements()->Get(i)->GetSkin()!=NULL)			
			{
		//	XDEBUG_PRINTCOLOR(0,__L("Creating Skinning Debug Object: %s"),scene->GetObjects()->Get(e)->GetName()->Get());		
 			this->CreateSkinDebug(scene->GetObjects()->Get(e)->GetElements()->Get(i))	;	
			}
		}
	if (flags & GRPVISUALDEBUGGER_FLAG_NORMALS)
		this->CreateNormal			(scene->GetObjects()->Get(e));
	}

	if (flags & GRPVISUALDEBUGGER_FLAG_LIGHTS)
	for (XDWORD e=0;e<scene->GetLights()->GetSize();e++)
	{
		//XDEBUG_PRINTCOLOR(0,__L("Creating Light Debug : %s"),scene->GetLights()->Get(e)->GetName()->Get());		 			
		this->CreateLight(scene->GetLights()->Get(e));
	}

	if (flags & GRPVISUALDEBUGGER_FLAG_CAMERAS)
	for (XDWORD e=0;e<scene->GetCameras()->GetSize();e++)
	{
		//XDEBUG_PRINTCOLOR(0,__L("Creating Camera Debug : %s"),scene->GetCameras()->Get(e)->GetName()->Get());		 					
		this->CreateCamera(scene->GetCameras()->Get(e));
	}

	if (flags & GRPVISUALDEBUGGER_FLAG_LOCATORS)	
	for (XDWORD e=0;e<nNodes;e++)
	{
		//XDEBUG_PRINTCOLOR(0,__L("Creating Locator Debug : %s"),scene->GetNodes()->Get(e)->GetName()->Get());		 						
		this->CreateLocator(scene->GetNodes()->Get(e));
	}
	/*
	if (flags & GRPVISUALDEBUGGER_FLAG_ANIMATION)	
	if (scene->GetAnimationControl()!=NULL)
		if (scene->GetAnimationControl()->GetStacks()->GetSize()>0)
			if (scene->GetAnimationControl()->GetStacks()->GetLast()->GetLayers()->GetSize()>0)
				if (scene->GetAnimationControl()->GetStacks()->GetLast()->GetLayers()->Get((int)scene->GetAnimationControl()->GetStacks()->GetLast()->GetCurrentLayer())->GetCurves()->GetSize()>0)
			{
				//XDEBUG_PRINTCOLOR(0,__L("Creating Animation Controller Debug"));		 								
				CreateAnimation(scene->GetAnimationControl(),flags);
			}
	//*/
	
	//XDEBUG_PRINTCOLOR(0,__L("[Debug Created]"));		 								
	
	scene->ShowDebug(GRPDEBUG_FLAG_NORMAL);

}

/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::CreateBoundingBox
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/06/2015 17:16:39
//	
//	@return 			GRPOBJECT* : 
//
//  @param				obj : 
*/
/*-----------------------------------------------------------------*/
GRPOBJECT*	GRPVISUALDEBUGGER::CreateBoundingBox		(GRPOBJECT*			obj)
{
	XDWORD nElements=obj->GetElements()->GetSize();

	GRPOBJECT* debugObject=new GRPOBJECT();

	for (XDWORD i=0;i<nElements;i++)
	 {				
				GRPELEMENT* element=obj->GetElements()->Get(i);

				GRPELEMENT* normalViewer=CreateBoundingBoxElement	(element); 	
										if (normalViewer==NULL)
										{ 
											delete(debugObject);
											return NULL;											
										}
										normalViewer->SetNode									(element->GetNode());																		
										normalViewer->GetNode									()->modified=true;

				debugObject->AddElement									(normalViewer);																								
				if (!element->isinstance)
				normalViewer->SetProgram								(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"BAS"));	
				else
				normalViewer->SetProgram								(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"YLW"));	
	}
		
		debugObject->SetNode									(scene->CreateNode());	
		
		if (obj->GetNode())
		{
		XSTRING s;s.Format(__L("Debug Object::BoundingBox::%s"),obj->GetNode()->GetName()->Get());
		debugObject->GetNode()->SetName				(s.Get());
		
		debugObject->GetNode()->LinkTo				(obj->GetNode());
		debugObject->GetNode()->modified=true;
		}
		scene->Add(debugObject);
		scene->GetDebugObjects()->Add(debugObject);
	return debugObject;
}

/*-------------------------------------------------------------------
//	GRPSCENE::CreateNormalDebugOn
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/03/2015 10:56:43
//	
//  @param				obj : 
*/
/*-----------------------------------------------------------------*/
GRPOBJECT* GRPVISUALDEBUGGER::CreateNormal(GRPOBJECT* obj)
{
	XDWORD nElements=obj->GetElements()->GetSize();

	GRPOBJECT* debugObject=new GRPOBJECT();

	for (XDWORD i=0;i<nElements;i++)
	 {				
				GRPELEMENT* element=obj->GetElements()->Get(i);
				if (element->IsInstance()) continue;

				GRPELEMENT* normalViewer=CreateNormalViewerElement(element); 
				if (normalViewer==NULL)
										{ 
											delete(debugObject);
											return NULL;											
										}
				
																	normalViewer->SetNode									(element->GetNode());																															
																	normalViewer->GetNode()->modified=true;
																	debugObject->AddElement								(normalViewer);																			
																																			
	}
		debugObject->SetNode									(scene->CreateNode());	

		if (obj->GetNode())
		{
		XSTRING s;s.Format(__L("Debug Object::NormalViewer::%s"),obj->GetNode()->GetName()->Get());
		debugObject->GetNode()->SetName				(s.Get());

		debugObject->GetNode()->LinkTo				(obj->GetNode());
		debugObject->SetProgram								(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"BAS"));	
		debugObject->GetNode()->modified=true;
		}
		scene->Add(debugObject);
		scene->GetDebugObjects()->Add(debugObject);
	return debugObject;
}


/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::CreateDebugLocator
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/03/2015 12:01:48
//	
//	@return 			GRPOBJECT* : 
//
//  @param				node : 
*/
/*-----------------------------------------------------------------*/
GRPOBJECT* GRPVISUALDEBUGGER::CreateLocator(GRPNODE* node)
{
	GRPOBJECT* debugObject=new GRPOBJECT();


				GRPELEMENT* normalViewer=CreatePointLocator(); 
				if (normalViewer==NULL)
										{ 
											delete(debugObject);
											return NULL;											
										}
			
																	normalViewer->SetNode(node);																													
																	normalViewer->GetNode()->modified=true;
																	debugObject->AddElement								(normalViewer);																			
																																			
	
		debugObject->SetNode									(scene->CreateNode());	
		debugObject->GetNode()->LinkTo				(node);

		XSTRING s;s.Format(__L("DebugObject::Locator::%s"),node->GetName()->Get());
		debugObject->GetNode()->SetName				(s.Get());

		debugObject->SetProgram								(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"GRN"));		
		debugObject->GetNode()->modified=true;

		scene->Add(debugObject);
		scene->GetDebugObjects()->Add(debugObject);

	return debugObject;
}

/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::CreateDebugLocator
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/03/2015 12:01:48
//	
//	@return 			GRPOBJECT* : 
//
//  @param				node : 
*/
/*-----------------------------------------------------------------*/
GRPOBJECT* GRPVISUALDEBUGGER::CreateLight(GRPLIGHT* node)
{
	GRPOBJECT* debugObject=new GRPOBJECT();


				GRPELEMENT* normalViewer=CreateLightLocator(); 			
				if (normalViewer==NULL)
										{ 
											delete(debugObject);
											return NULL;											
										}
										normalViewer->SetNode(node);																													
										normalViewer->GetNode()->modified=true;

				debugObject->AddElement(normalViewer);																			
																																			
	
		debugObject->SetNode									(scene->CreateNode());	
		debugObject->GetNode()->LinkTo				(node);	
		
		XSTRING s;s.Format(__L("Debug Object::LightLocator::%s"),node->GetName()->Get());
		debugObject->GetNode()->SetName(s.Get());

		debugObject->SetProgram	(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"YLW"));	
		debugObject->GetNode		()->modified=true;

		scene->Add(debugObject);
		scene->GetDebugObjects()->Add(debugObject);

	return debugObject;
}

/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::CreateCamera
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/06/2015 17:53:09
//	
//	@return 			GRPOBJECT* : 
//
//  @param				camera : 
*/
/*-----------------------------------------------------------------*/
GRPOBJECT*	GRPVISUALDEBUGGER::CreateCamera				(GRPCAMERA*			camera)
{
	GRPOBJECT* debugObject=new GRPOBJECT();


				GRPELEMENT* EyeViewer=CreateBoxLocator(); 
				XSTRING s;s.Format(__L("Debug Object::Camera::%s"),camera->GetName()->Get());

																	EyeViewer->SetNode(camera);																													
																	EyeViewer->GetNode()->modified=true;
																	debugObject->AddElement								(EyeViewer);																			
							
				if (camera->target)
				{
				GRPPOINT a=camera->GetWorldPosition();
				GRPPOINT b=camera->target->GetWorldPosition();
				GRPELEMENT* LineOfSight=CreateLine(GRPPOINT(0.0,0.0,0.0),GRPPOINT(0.0,0.0,-GRPVECTOR(b-a).GetLengthf()));
				
			  GRPELEMENT* TargetViewer=CreateBoxLocator(); 
										if (TargetViewer==NULL)
												{ 
													delete(debugObject);
													delete(EyeViewer);
													return NULL;											
												}
									s.Format(__L("Debug Object::Camera::Target::%s"),camera->GetName()->Get());


					if (camera->upnode)
					{
						GRPELEMENT* UpVect=CreateBoxLocator(); 
											if (UpVect==NULL)
												{ 
													delete(TargetViewer);
													delete(debugObject);
													delete(EyeViewer);
													return NULL;											
												}
						GRPELEMENT* UpVectSight=CreateLine(GRPPOINT(0.0,0.0,0.0),GRPPOINT(0.0,camera->upnode->GetPosition()->vector[1],0.0));
										if (UpVectSight==NULL)
												{ 
													delete(UpVect);
													delete(TargetViewer);
													delete(debugObject);
													delete(EyeViewer);
													return NULL;											
												}
							UpVect->SetNode(camera);
							UpVectSight->SetNode(camera);

							debugObject->AddElement								(UpVect);	
							debugObject->AddElement								(UpVectSight);	
					}
					else
					{
						GRPELEMENT* UpVectSight=CreateLine(GRPPOINT(camera->up.vector[0]*(float)DebugSize,camera->up.vector[1]*(float)DebugSize,camera->up.vector[2]*(float)DebugSize),GRPPOINT(camera->up.vector[0]*5.0f,camera->up.vector[1]*5.0f,camera->up.vector[2]*5.0f));
											if (UpVectSight==NULL)
												{ 
													delete(TargetViewer);
													delete(debugObject);
													delete(EyeViewer);
													return NULL;											
												}
						UpVectSight->SetNode										(camera);
						debugObject->AddElement									(UpVectSight);	
					}

																	TargetViewer->SetNode									(camera->target);																													
																	TargetViewer->GetNode									()->modified=true;
																	debugObject->AddElement								(TargetViewer);		
																	LineOfSight->SetNode									(camera);
																	debugObject->AddElement								(LineOfSight);		


				}
	
		debugObject->SetNode									(scene->CreateNode());	
		
		EyeViewer->GetNode()->GetBaseMaterial()->SetDiffuse(GRPCOLOR(0.0f,0.0f,1.0f,0.0f));
		s.Format(__L("Debug Object::Camera::%s"),camera->GetName()->Get());
		debugObject->GetNode()->SetName(s.Get());
		debugObject->GetNode()->LinkTo				(camera);
		debugObject->SetProgram								(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"BLU"));	
		debugObject->GetNode()->modified=true;

		scene->Add(debugObject);
		scene->GetDebugObjects()->Add(debugObject);

	return debugObject;
}


/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::CreateSkinDebug
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/06/2015 18:14:45
//	
//	@return 			GRPOBJECT* : 
//
//  @param				obj : 
*/
/*-----------------------------------------------------------------*/
GRPOBJECT*	GRPVISUALDEBUGGER::CreateSkinDebug	(GRPELEMENT*	obj)
{
	GRPSKIN* skin=obj->GetSkin();
	if (skin==NULL) return NULL;
	
	GRPOBJECT* debugObject=new GRPOBJECT();
												if (debugObject==NULL)
												{ 												
													return NULL;											
												}
	debugObject->SetNode									(scene->CreateNode());	
		

			for(XDWORD e=0;e<skin->GetClusters()->GetSize();e++)			
				for (XDWORD l=0;l<skin->GetClusters()->Get(e)->GetLimbs()->GetSize();l++)
				{
				GRPSKINLIMB* Limb=skin->GetClusters()->Get(e)->GetLimbs()->Get(l);
												
				GRPPOINT a(0.0,0.0,0.0);
				GRPPOINT b;
				if (Limb->GetParent()!=NULL)
				{						
						b=GRPPOINT(15.0,0.0,0.0);
				
				GRPELEMENT* LimbElement=CreateLine(&a,&b);
										if (LimbElement==NULL)
												{ 												
													return NULL;											
												}
				if (Limb->GetParent()!=NULL)
						LimbElement->SetNode(Limb->GetParent());
				else
						LimbElement->SetNode(debugObject->GetNode());
				debugObject->AddElement(LimbElement);
				}

				}
								XSTRING s;

		s.Format(__L("Debug Object::Skin::%s"),obj->GetSkin()->GetName()->Get());
		debugObject->GetNode()->SetName(s.Get());
		debugObject->GetNode()->LinkTo				(obj->GetNode());
		debugObject->SetProgram								(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"PNK"));	
		debugObject->GetNode()->modified=true;

		scene->Add(debugObject);
		scene->GetDebugObjects()->Add(debugObject);
			//*/
	return debugObject;
}

/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::CreateAnimation
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/06/2015 10:48:14
//	
//	@return 			GRPOBJECT* : 
//
//  @param				animation : 
*/
/*-----------------------------------------------------------------*/
GRPOBJECT*	GRPVISUALDEBUGGER::CreateAnimation				(GRPANIMATIONCONTROL*	animationcontrol,XDWORD flags)
{
	if (animationcontrol->GetStacks()->GetSize()<=0) return NULL;

	GRPOBJECT* debugObject=new GRPOBJECT();
				if (debugObject==NULL)
					{ 												
						return NULL;											
					}

	//-------------------------------- Ejes

	XVECTOR<GRPPOINT*> Points;
	
	GRPPOINT* p=0;
						p=new GRPPOINT(+0.0,		-100.0,		+0.0);
						if (p==NULL) 
						{
								Points.DeleteContents();
								return NULL;
						}
	Points.Add(p);
						p=new GRPPOINT(+0.0,		+100.0,		+0.0);
						if (p==NULL) 
						{
								Points.DeleteContents();
								return NULL;
						}
	Points.Add(p);
						p=new GRPPOINT(+0.0,		+0.0,		+0.0);
						if (p==NULL) 
						{
								Points.DeleteContents();
								return NULL;
						}
	Points.Add(p);
						p=new GRPPOINT(+100.0,	+0.0,		+0.0);
						if (p==NULL) 
						{
								Points.DeleteContents();
								return NULL;
						}
	Points.Add(p);
	
	GRPELEMENT* Dummy=ConstructDummy(&Points,1.0f);
							if (Dummy==NULL)
							{
								Points.DeleteContents();
								return NULL;
							}

	Points.DeleteContents();
	Points.DeleteAll();
	
	GRPNODE* node=scene->CreateNode();
						if (node==NULL)
						{
							delete(Dummy);
							return NULL;
						}

	debugObject->SetNode(node);
	debugObject->AddElement(Dummy);
	Dummy->SetNode(debugObject->GetNode());

	Dummy->SetProgram					(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"BAS"));	

	//-------------------------------- ten divisor background
	
	for (GLFLOAT f=-100.0f;f<100.0f;f+=10.0f)
	{
		if (f!=0.0)
		{
		GRPPOINT* p=new GRPPOINT(+0.0,					f,		+0.0);
						if (p==NULL)
						{
							delete(Dummy);
							return NULL;
						}
		Points.Add(p);
							p=new GRPPOINT(+100.0,				f,		+0.0);
							if (p==NULL)
						{
							delete(Dummy);
							return NULL;
						}
		Points.Add(p);
		}

		if (f>0.0f) 
		{
		GRPPOINT* p=new GRPPOINT(f,			-100.0f,		+0.0);
						if (p==NULL)
						{
							delete(Dummy);
							return NULL;
						}
		Points.Add(p);
							p=new GRPPOINT(f,			+100.0f,    +0.0);
						if (p==NULL)
						{
							delete(Dummy);
							return NULL;
						}
		Points.Add(p);
		}
	}

	
	Dummy=ConstructDummy(&Points,1.0f);

	Points.DeleteContents();
	Points.DeleteAll();
	
	debugObject->AddElement(Dummy);
	Dummy->SetNode(debugObject->GetNode());

	Dummy->SetProgram					(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"GRY"));	
		

		//--------------------------------animation curves 
	/*
	GRPANIMATIONCURVE* curve=animationcontrol->GetStacks()->GetElement(s)->->GetLayers()->GetLast()->GetCurves()->Get(0);
	curve->Update(0);
	this->AnimationDebugSizeMin=curve->target;
	this->AnimationDebugSizeMax=curve->target;
	
	for (XDWORD n=0;n<animationcontrol->GetStacks()->GetLast()->GetLayers()->GetLast()->GetAnimationNodes()->GetSize();n++)			
	{
		GRPANIMATIONCURVETYPE type=animationcontrol->GetStacks()->GetLast()->GetLayers()->GetLast()->GetAnimationNodes()->Get(n)->GetType();

		switch(type)
		{
			case GRPANIMATIONCURVETYPE_UNKNOWN:			continue;

			case GRPANIMATIONCURVETYPE_TRANSLATION: if (!(flags & GRPVISUALDEBUGGER_FLAG_ANIMATION_TRANSLATION))	continue; break;
			case GRPANIMATIONCURVETYPE_ROTATION:		if (!(flags & GRPVISUALDEBUGGER_FLAG_ANIMATION_ROTATION))			continue; break; 
			case GRPANIMATIONCURVETYPE_SCALE:				if (!(flags & GRPVISUALDEBUGGER_FLAG_ANIMATION_SCALE))				continue; break;
			case GRPANIMATIONCURVETYPE_VISIBILITY:	if (!(flags & GRPVISUALDEBUGGER_FLAG_ANIMATION_VISIBILITY))		continue; break;
			
			case GRPANIMATIONCURVETYPE_DIFFUSECOLOR:
			case GRPANIMATIONCURVETYPE_DIFFUSEFACTOR:
			case GRPANIMATIONCURVETYPE_SPECULARCOLOR:
			case GRPANIMATIONCURVETYPE_EMISSIVECOLOR:
			case GRPANIMATIONCURVETYPE_AMBIENTCOLOR:
			case GRPANIMATIONCURVETYPE_SHININESSEXPONENT:
			case GRPANIMATIONCURVETYPE_SPECULARFACTOR:
			case GRPANIMATIONCURVETYPE_TRANSPARENCYFACTOR:
			case GRPANIMATIONCURVETYPE_FOV:
			case GRPANIMATIONCURVETYPE_FIELDOFVIEWX:
			case GRPANIMATIONCURVETYPE_FIELDOFVIEWY:
			case GRPANIMATIONCURVETYPE_INTENSITY:
			case GRPANIMATIONCURVETYPE_ROLL:
			case GRPANIMATIONCURVETYPE_PITCH:
			case GRPANIMATIONCURVETYPE_YAW:
			case GRPANIMATIONCURVETYPE_DEFORMPERCENT: 
			default:																	if (!(flags & GRPVISUALDEBUGGER_FLAG_ANIMATION_OTHERS))			continue; break;
		}

		for (XDWORD c=0;c<animationcontrol->GetStacks()->GetLast()->GetLayers()->GetLast()->GetAnimationNodes()->Get(n)->GetCurveConnections()->GetSize();c++)	
		{
		GRPANIMATIONCURVE*	curve					=animationcontrol->GetStacks()->GetLast()->GetLayers()->GetLast()->GetAnimationNodes()->Get(n)->GetCurveConnections()->GetElement(c);		
		GRPELEMENT*					CurveValues		=this->CreateAnimationCurve(curve,animationcontrol);
												CurveValues->SetNode(debugObject->GetNode());
												debugObject->AddElement(CurveValues);				

								XDEBUG_PRINTCOLOR(0,__L("\tCreating Debug Curve:%s"),curve->GetName()->Get());					
		}
	}
			
	//----------------------- reescale and add to scene

		double Scale=(AnimationDebugSizeMax-AnimationDebugSizeMin);

		Dummy->GetNode()->SetScale			(1.00f,		0.25f,		1.0f		);
		Dummy->GetNode()->SetPosition		(-50.0f,	0.0f,		-100.0f	);
	
		debugObject->GetNode()->modified=true;
		debugObject->GetNode()->LinkTo	(scene->GetCamera());

		scene->Add(debugObject);
		scene->GetDebugObjects()->Add(debugObject);
		*/
	return debugObject;
}


/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::CreateAnimationCurve
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/06/2015 10:27:39
//	
//	@return 			GRPELEMENT* : 
//
//  @param				curve : 
//  @param				control : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*	GRPVISUALDEBUGGER::CreateAnimationCurve				(GRPANIMATIONCURVE*	curve,GRPANIMATIONCONTROL* control)
{	
	//------------------------ create points

	XVECTOR<GRPPOINT*> Points;

	XQWORD endFrame		=control->GetStacks()->Get(0)->GetMaxTime();
	XQWORD stFrame		=control->GetStacks()->Get(0)->GetMinFrame();	

	XQWORD framecount	=control->GetTotalFrames();

	XQWORD t=stFrame;
	double inc=double(endFrame-stFrame)/framecount;	

	double Increment=0.5f;
	for (double n=0;n<framecount;n+=Increment)
	{
		curve->Update(t);
		double value=curve->target;
		GRPPOINT* p=new GRPPOINT((GLFLOAT)n,(GLFLOAT)value,0.0); if (p==NULL) return NULL;												
		Points.Add(p);

		curve->Update(t+(XQWORD)(inc*Increment));
		value=curve->target;
								p=new GRPPOINT((GLFLOAT)(n+Increment),(GLFLOAT)value,0.0);if (p==NULL) return NULL;
		Points.Add(p);
		t+=(XQWORD)(inc*Increment);

		if (value>this->AnimationDebugSizeMax)			AnimationDebugSizeMax=value;
		if (value<this->AnimationDebugSizeMin)			AnimationDebugSizeMin=value;
	}
	
	GRPELEMENT* CurveValues=ConstructDummy(&Points,1.0f);

	Points.DeleteContents();
	Points.DeleteAll();

	//------------------------ choose color
	
	if (curve->GetName()->Find(__L("d|X"),true)!=NOTFOUND)
	CurveValues->SetProgram		(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"RED"));	
	else
	if (curve->GetName()->Find(__L("d|Y"),true)!=NOTFOUND)
	CurveValues->SetProgram		(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"GRN"));	
	else
	if (curve->GetName()->Find(__L("d|Z"),true)!=NOTFOUND)
	CurveValues->SetProgram		(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"BLU"));	
	else
	if (curve->GetName()->Find(__L("d|Visibility"),true)!=NOTFOUND)
	CurveValues->SetProgram		(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"BAS"));	
	else
		CurveValues->SetProgram		(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"GRY"));	

	return CurveValues;
}

/*-------------------------------------------------------------------
//	GRPELEMENT::CreateNormalViewerElement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/03/2015 14:27:02
//	
//	@return 			GRPELEMENT* : 
//
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* GRPVISUALDEBUGGER::CreateNormalViewerElement(GRPELEMENT* element)
{
	if (!element->GetMesh())
		return NULL;

	XVECTOR<GRPPOINT*> Points;

	//GLFLOAT Sc=0.5f;
	GLFLOAT Sc=(float)this->DebugSize;
	/*
	for (XDWORD e=0;e<element->GetFaces()->GetSize();e++)
			{
				GRPFACE* f=element->GetFaces()->Get(e);
				for (XDWORD v=0;v<3;v++)
				{
					GRPVERTEX* vdata=element->GetVertexData();

					GRPPOINT* vertex=(f->vertices[v]);				
					GRPPOINT* p=new GRPPOINT(vertex->x,vertex->y,vertex->z);if (p==NULL) return NULL;Points.Add(p);

					GRPPOINT* normal=(f->normals[v]);
										p=new GRPPOINT(vertex->x+f->normals[v]->x*Sc,	vertex->y+f->normals[v]->y*Sc,vertex->z+f->normals[v]->z*Sc);if (p==NULL) return NULL; Points.Add(p);
				}
			}	
			*/

	GRPVERTEX* vdata=element->GetMesh()->GetVertexData();
	XDWORD n=element->GetMesh()->GetNVertex();
	for (XDWORD e=0;e<n;e++)
	{			
					GRPVERTEX* vertex=&vdata[e];	
					GRPPOINT* p=new GRPPOINT(vertex->position[0],vertex->position[1],vertex->position[2]);
										if (p==NULL) return NULL;
										Points.Add(p);
										p=new GRPPOINT(vertex->position[0]+(vertex->normal[0]*Sc),vertex->position[1]+(vertex->normal[1]*Sc),vertex->position[2]+(vertex->normal[2]*Sc));
										if (p==NULL) return NULL; 
										Points.Add(p);
	}
	
	GRPELEMENT* Dummy=NULL;
	if (Points.GetSize()>=2)
		Dummy=ConstructDummy(&Points,1.0f);
	else
		XDEBUG_PRINTCOLOR(4,__L("El elemento %s tiene menos de 3 vertices"),element->GetName()->Get()); 


	Points.DeleteContents();
	Points.DeleteAll();
	return Dummy;	
}



/*-------------------------------------------------------------------
//	GRPELEMENT::CreatePointLocator
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/03/2015 14:27:02
//	
//	@return 			GRPELEMENT* : 
//
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* GRPVISUALDEBUGGER::CreatePointLocator()
{
	
	XVECTOR<GRPPOINT*> Points;
	
	GRPPOINT*		p=new GRPPOINT(-1.0f*(float)DebugSize,	+0.0,										+0.0); 										if (p==NULL) { Points.DeleteContents(); Points.DeleteAll(); return NULL;}; Points.Add(p);
							p=new GRPPOINT(+1.0f*(float)DebugSize,	+0.0,										+0.0); 										if (p==NULL) { Points.DeleteContents(); Points.DeleteAll(); return NULL;}; Points.Add(p);
							p=new GRPPOINT(+0.0,										-1.0f*(float)DebugSize,	+0.0); 										if (p==NULL) { Points.DeleteContents(); Points.DeleteAll(); return NULL;}; Points.Add(p);
							p=new GRPPOINT(+0.0,										+1.0f*(float)DebugSize,	+0.0);										if (p==NULL) { Points.DeleteContents(); Points.DeleteAll(); return NULL;}; Points.Add(p);
							p=new GRPPOINT(+0.0,										+0.0,										-1.0f*(float)DebugSize);	if (p==NULL) { Points.DeleteContents(); Points.DeleteAll(); return NULL;}; Points.Add(p);
							p=new GRPPOINT(+0.0,										+0.0,										+1.0f*(float)DebugSize);	if (p==NULL) { Points.DeleteContents(); Points.DeleteAll(); return NULL;}; Points.Add(p);
	
	GRPELEMENT* Dummy=ConstructDummy(&Points,1.0f);

	Points.DeleteContents();
	Points.DeleteAll();
	return Dummy;	
}



/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::CreateLightLocator
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/06/2015 16:28:48
//	
//	@return 			GRPELEMENT* : 
//
//  @param				node : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* GRPVISUALDEBUGGER::CreateLightLocator()
{
	XVECTOR<GRPPOINT*> Points;

	GRPPOINT*		p=new GRPPOINT(+0.0,		+1.5,		+0.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);
							p=new GRPPOINT(-1.0,		+0.0,		+1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);
							p=new GRPPOINT(+0.0,		+1.5,		+0.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);	
							p=new GRPPOINT(+1.0,		+0.0,		+1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);	
							p=new GRPPOINT(+0.0,		+1.5,		+0.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);
							p=new GRPPOINT(+1.0,		+0.0,		-1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);
							p=new GRPPOINT(+0.0,		+1.5,		+0.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);
							p=new GRPPOINT(-1.0,		+0.0,		-1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);
							p=new GRPPOINT(-1.0,		+0.0,		-1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);		
							p=new GRPPOINT(+1.0,		+0.0,		-1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);			
							p=new GRPPOINT(+1.0,		+0.0,		-1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);		
							p=new GRPPOINT(+1.0,		+0.0,		+1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);		
							p=new GRPPOINT(+1.0,		+0.0,		+1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);		
							p=new GRPPOINT(-1.0,		+0.0,		+1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);		
							p=new GRPPOINT(-1.0,		+0.0,		+1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);		
							p=new GRPPOINT(-1.0,		+0.0,		-1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);		
							p=new GRPPOINT(+0.0,		-1.5,		+0.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);
							p=new GRPPOINT(-1.0,		+0.0,		+1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);
							p=new GRPPOINT(+0.0,		-1.5,		+0.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);
							p=new GRPPOINT(+1.0,		+0.0,		+1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);		
							p=new GRPPOINT(+0.0,		-1.5,		+0.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);
							p=new GRPPOINT(+1.0,		+0.0,		-1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);		
							p=new GRPPOINT(+0.0,		-1.5,		+0.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);
							p=new GRPPOINT(-1.0,		+0.0,		-1.0);if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);		

	GRPELEMENT* Dummy=ConstructDummy(&Points,0.3f);

	Points.DeleteContents();
	Points.DeleteAll();


	return Dummy;	
}


/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::CreateLimbLocator
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/06/2015 16:28:48
//	
//	@return 			GRPELEMENT* : 
//
//  @param				node : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* GRPVISUALDEBUGGER::CreateLimbLocator(GRPSKINLIMB* limb)
{
		if (limb==NULL)
				return NULL;

	XVECTOR<GRPPOINT*> Points;
	
	Points.Add(new GRPPOINT(+0.0,		+0.0,		+0.0));
		if (limb->GetParent())
		{
			GRPPOINT* p=new GRPPOINT(limb->GetParent()->GetWorldPosition());if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);			
		}
		else
		{
			GRPPOINT* p=new GRPPOINT(+0.0,		+0.0,		+0.0);								if (p==NULL){ Points.DeleteContents(); return NULL; }	Points.Add(p);			
		}

	GRPELEMENT* Dummy=ConstructDummy(&Points,limb->Size);
							
	Points.DeleteContents();
	Points.DeleteAll();							

	return Dummy;	
}

/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::CreateBoundingBoxElement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/06/2015 17:47:58
//	
//	@return 			GRPELEMENT* : 
//
//  @param				obj : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* GRPVISUALDEBUGGER::CreateBoundingBoxElement(GRPELEMENT* obj)
{
	XVECTOR<GRPPOINT*> Points;
	if (!obj->GetMesh()) 
		return NULL;

	obj->UpdateBoundingBox();
	GRPBB* aabb=obj->GetBoundingBox();
	
	/*
	Points.Add(new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->maxpoint.z));
	Points.Add(new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->maxpoint.z));

	Points.Add(new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->maxpoint.z));
	Points.Add(new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->maxpoint.z));
	
	Points.Add(new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->minpoint.z));
	Points.Add(new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->minpoint.z));

	Points.Add(new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->minpoint.z));
	Points.Add(new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->minpoint.z));

	Points.Add(new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->maxpoint.z));
	Points.Add(new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->maxpoint.z));

	Points.Add(new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->maxpoint.z));
	Points.Add(new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->maxpoint.z));

	Points.Add(new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->minpoint.z));
	Points.Add(new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->minpoint.z));

	Points.Add(new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->minpoint.z));
	Points.Add(new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->minpoint.z));

	Points.Add(new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->minpoint.z));
	Points.Add(new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->maxpoint.z));

	Points.Add(new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->minpoint.z));
	Points.Add(new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->maxpoint.z));

	Points.Add(new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->minpoint.z));
	Points.Add(new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->maxpoint.z));

	Points.Add(new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->minpoint.z));
	Points.Add(new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->maxpoint.z));
	*/
	GRPPOINT* p=NULL;
						p=new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
	
						p=new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

	GRPELEMENT* Dummy=ConstructDummy(&Points,1.0f);

	Points.DeleteContents();
	Points.DeleteAll();

	return Dummy;	
}

/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::CreateBoundingBoxElement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/06/2015 17:47:58
//	
//	@return 			GRPELEMENT* : 
//
//  @param				obj : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* GRPVISUALDEBUGGER::CreateBoxLocator()
{
	XVECTOR<GRPPOINT*> Points;

	GRPPOINT*		p=NULL;
	
							p=new GRPPOINT(-1.0f,-1.0f,+1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
							p=new GRPPOINT(-1.0f,+1.0f,+1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

							p=new GRPPOINT(+1.0f,-1.0f,+1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
							p=new GRPPOINT(+1.0f,+1.0f,+1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
	
							p=new GRPPOINT(+1.0f,-1.0f,-1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
							p=new GRPPOINT(+1.0f,+1.0f,-1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

							p=new GRPPOINT(-1.0f,-1.0f,-1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
							p=new GRPPOINT(-1.0f,+1.0f,-1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

							p=new GRPPOINT(+1.0f,+1.0f,+1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
							p=new GRPPOINT(-1.0f,+1.0f,+1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

							p=new GRPPOINT(+1.0f,-1.0f,+1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
							p=new GRPPOINT(-1.0f,-1.0f,+1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

							p=new GRPPOINT(+1.0f,+1.0f,-1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
							p=new GRPPOINT(-1.0f,+1.0f,-1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

							p=new GRPPOINT(+1.0f,-1.0f,-1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
							p=new GRPPOINT(-1.0f,-1.0f,-1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

							p=new GRPPOINT(+1.0f,-1.0f,-1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
							p=new GRPPOINT(+1.0f,-1.0f,+1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

							p=new GRPPOINT(-1.0f,-1.0f,-1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
							p=new GRPPOINT(-1.0f,-1.0f,+1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

							p=new GRPPOINT(-1.0f,+1.0f,-1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
							p=new GRPPOINT(-1.0f,+1.0f,+1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

							p=new GRPPOINT(+1.0f,+1.0f,-1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
							p=new GRPPOINT(+1.0f,+1.0f,+1.0f); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

	GRPELEMENT* Dummy=ConstructDummy(&Points,1.0f);

	Points.DeleteContents();
	Points.DeleteAll();


	return Dummy;	
}

/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::CreateLine
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/06/2015 10:15:06
//	
//	@return 			GRPELEMENT* : 
//
//  @param				node : 
//  @param				node2 : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*	 GRPVISUALDEBUGGER::CreateLine(GRPPOINT node,GRPPOINT node2)
{
	XVECTOR<GRPPOINT*>	Points;
											Points.Add(&node);
											Points.Add(&node2);

	GRPELEMENT* Dummy=ConstructDummy(&Points,1.0f);

	Points.DeleteAll();

	return Dummy;	
}
/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::ConstructDummy
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			08/06/2015 16:51:10
//	
//	@return 			GRPELEMENT* : 
//
//  @param				Points : 
//  @param				Scale : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*  GRPVISUALDEBUGGER::ConstructDummy(XVECTOR<GRPPOINT*>* Points,GLFLOAT Scale)
{
	GRPELEMENT* Dummy	=	new GRPELEMENT();
	GRPMESH*		mesh	= new GRPMESH();
							Dummy->SetMesh(mesh);

	XDWORD n=Points->GetSize()*2;

						Dummy->GetMesh()->indexdata		= new XWORD[n];
						if (Dummy->GetMesh()->indexdata==NULL)
						{
							delete(Dummy);
							return NULL;
						}

						Dummy->GetMesh()->nindexdata	= n;						

						Dummy->GetMesh()->vertexdata	= new GRPVERTEX[n];
						if (Dummy->GetMesh()->indexdata==NULL)
						{
							delete(Dummy->GetMesh()->indexdata);
							delete(Dummy);
							return NULL;
						}

						Dummy->GetMesh()->nvertexdata = n;
						memset(Dummy->GetMesh()->vertexdata,0,sizeof(GRPVERTEX)*n);
	
	if (Dummy->GetMesh()->indexdata==NULL || Dummy->GetMesh()->vertexdata==NULL) 
	{
		XDEBUG_PRINTCOLOR(4,__L("Not enought memory"));
		return NULL;
	}

	XDWORD i									=	0;
	GRPVERTEX* vdata					=	Dummy->GetMesh()->GetVertexData();					
	memset(vdata,0,sizeof(GRPVERTEX)*n);
				
	int nPoints=Points->GetSize()-1;
	
	if (nPoints>=2) 
	for (int e=0;e<nPoints;e+=2)
	{
						Dummy->GetMesh()->vertexdata[i].position[0]=Points->Get(e)->x*Scale;
						Dummy->GetMesh()->vertexdata[i].position[1]=Points->Get(e)->y*Scale;
						Dummy->GetMesh()->vertexdata[i].position[2]=Points->Get(e)->z*Scale;							
						Dummy->GetMesh()->indexdata[i]=i;
						i++;
						Dummy->GetMesh()->vertexdata[i].position[0]=Points->Get(e+1)->x*Scale;
						Dummy->GetMesh()->vertexdata[i].position[1]=Points->Get(e+1)->y*Scale;
						Dummy->GetMesh()->vertexdata[i].position[2]=Points->Get(e+1)->z*Scale;							
						Dummy->GetMesh()->indexdata[i]=i;
						i++;
	}


			Dummy->SetDefaultColor(GRPCOLOR(1.0f,1.0f,1.0f,1.0f));

			Dummy->SetRenderMode(GRPRENDERMODE_LINES);			
			Dummy->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"BAS"));	

return Dummy;
}

/*-------------------------------------------------------------------
//	GRPVISUALDEBUGGER::ConstructSpline
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/11/2015 11:05:40
//	
//	@return 			GRPELEMENT* : 
//
//  @param				Points : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*  GRPVISUALDEBUGGER::ConstructSpline(XVECTOR<GRPPOINT*>* Points)
{
	GRPELEMENT* Dummy	=	new GRPELEMENT();
	GRPMESH*		mesh	= new GRPMESH();
	Dummy->SetMesh(mesh);

	XDWORD n=Points->GetSize()*2;

						Dummy->GetMesh()->indexdata		= new XWORD[n];
						if (Dummy->GetMesh()->indexdata==NULL)
						{
							delete(Dummy);
							return NULL;
						}

						Dummy->GetMesh()->nindexdata	= n;						

						Dummy->GetMesh()->vertexdata	= new GRPVERTEX[n];
						if (Dummy->GetMesh()->indexdata==NULL)
						{
							delete(Dummy->GetMesh()->indexdata);
							delete(Dummy);
							return NULL;
						}

						Dummy->GetMesh()->nvertexdata = n;
						memset(Dummy->GetMesh()->vertexdata,0,sizeof(GRPVERTEX)*n);
	
	if (Dummy->GetMesh()->indexdata==NULL || Dummy->GetMesh()->vertexdata==NULL) 
	{
		XDEBUG_PRINTCOLOR(4,__L("Not enought memory"));
		return NULL;
	}

	XDWORD i									=	0;
	GRPVERTEX* vdata					=	Dummy->GetMesh()->GetVertexData();					
	memset(vdata,0,sizeof(GRPVERTEX)*n);
				
	XDWORD np=Points->GetSize()-2;
	for (XDWORD e=0;e<np;e++)
	{
						Dummy->GetMesh()->vertexdata[i].position[0]=Points->Get(e)->x;
						Dummy->GetMesh()->vertexdata[i].position[1]=Points->Get(e)->y;
						Dummy->GetMesh()->vertexdata[i].position[2]=Points->Get(e)->z;							
						Dummy->GetMesh()->indexdata[i]=i;
						i++;
						Dummy->GetMesh()->vertexdata[i].position[0]=Points->Get(e+1)->x;
						Dummy->GetMesh()->vertexdata[i].position[1]=Points->Get(e+1)->y;
						Dummy->GetMesh()->vertexdata[i].position[2]=Points->Get(e+1)->z;							
						Dummy->GetMesh()->indexdata[i]=i;
						i++;
	}

			Dummy->SetDefaultColor(GRPCOLOR(1.0f,1.0f,1.0f,1.0f));

			Dummy->SetRenderMode(GRPRENDERMODE_LINES);			
			Dummy->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"BAS"));

return Dummy;
}




/*-------------------------------------------------------------------
//	GRPDEBUGGERPASS::GRPDEBUGGERPASS
*/
/**
//
//	constructor
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		 : 
//
//	@param		) : 
//
*//*-----------------------------------------------------------------*/
GRPDEBUGGERPASS::GRPDEBUGGERPASS() : GRPPASS(__L("DebuggerPass"))
{
		Clean();
}

/*-------------------------------------------------------------------
//	
GRPDEBUGGERPASS::~GRPDEBUGGERPASS
*/
/**
//
//	destructor
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		 : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/

GRPDEBUGGERPASS::~GRPDEBUGGERPASS()
{
		this->RenderOrder->DeleteContents();
		this->RenderOrder->DeleteAll();
}

/*-------------------------------------------------------------------
//	 GRPDEBUGGERPASS::Start
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		void : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
void GRPDEBUGGERPASS::Start()
{
		switch (this->currentiteration)
		{
		case 0: GRPOPENGLCTRL::BLENDING::SetAlphaBlending(false); 
						GRPOPENGLCTRL::DEPTH::SetDepthFunc(GRPDEPTHFUNC_LESS); break;

		case 1: GRPOPENGLCTRL::BLENDING::SetAlphaBlending(true,GRPBLENDINGMODE_MULTIPLICATIVE);
						GRPOPENGLCTRL::DEPTH::SetDepthFunc(GRPDEPTHFUNC_GREATER); break;
		}
}

/*-------------------------------------------------------------------
//	 GRPDEBUGGERPASS::Finish
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		void : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
void GRPDEBUGGERPASS::Finish()
{
		GRPOPENGLCTRL::BLENDING::SetAlphaBlending(false);
		GRPOPENGLCTRL::DEPTH::SetDepthFunc(GRPDEPTHFUNC_LESS);
}

/*-------------------------------------------------------------------
//	 GRPDEBUGGERPASS::OnAdded
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPDEBUGGERPASS::OnAdded()
{		
		this->iterations = 2;
		XDWORD nnodes = this->holderscene->GetNodes()->GetSize();

		if (DebugFlags & GRPVISUALDEBUGGER_FLAG_BBS)
		for (XDWORD e = 0; e < this->holderscene->GetPass()->GetRenderOrder()->GetSize(); e++)					
				CreateBoundingBox		(this->holderscene->GetPass()->GetRenderOrder()->Get(e));
		
		if (DebugFlags & GRPVISUALDEBUGGER_FLAG_NORMALS)
		for (XDWORD e = 0; e < this->holderscene->GetPass()->GetRenderOrder()->GetSize(); e++)
				CreateNormals				(this->holderscene->GetPass()->GetRenderOrder()->Get(e), DebugSize);		
		
		if (DebugFlags & GRPVISUALDEBUGGER_FLAG_LIGHTS)
		for (XDWORD e = 0; e < this->holderscene->GetLights()->GetSize(); e++)		
				CreateLightLocator(this->holderscene->GetLights()->Get(e), DebugSize);

		if (DebugFlags & GRPVISUALDEBUGGER_FLAG_LOCATORS)
		for (XDWORD e = 0; e <nnodes; e++)
				CreateLocator			(this->holderscene->GetNodes()->Get(e), DebugSize);

		this->active = false;
		return true;
}

/*-------------------------------------------------------------------
//	 GRPDEBUGGERPASS::ConstructDummy
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		GRPELEMENT* : 
//
//	@param		XVECTOR<GRPPOINT*>* : 
//	@param		float : 
//
*//*-----------------------------------------------------------------*/
GRPELEMENT* GRPDEBUGGERPASS::ConstructDummy(XVECTOR<GRPPOINT*>* Points,float Scale)
{
		GRPELEMENT* Dummy = new GRPELEMENT();
		GRPMESH*		mesh = new GRPMESH();
		Dummy->SetMesh(mesh);

		XDWORD n = Points->GetSize() * 2;

		Dummy->GetMesh()->indexdata = new XWORD[n];
		if (Dummy->GetMesh()->indexdata == NULL)
		{
				delete(Dummy);
				return NULL;
		}

		Dummy->GetMesh()->nindexdata = n;

		Dummy->GetMesh()->vertexdata = new GRPVERTEX[n];
		if (Dummy->GetMesh()->indexdata == NULL)
		{
				delete(Dummy->GetMesh()->indexdata);
				delete(Dummy);
				return NULL;
		}

		Dummy->GetMesh()->nvertexdata = n;
		memset(Dummy->GetMesh()->vertexdata, 0, sizeof(GRPVERTEX)*n);

		if (Dummy->GetMesh()->indexdata == NULL || Dummy->GetMesh()->vertexdata == NULL)
		{
				XDEBUG_PRINTCOLOR(4, __L("Not enought memory"));
				return NULL;
		}

		XDWORD i = 0;
		GRPVERTEX* vdata = Dummy->GetMesh()->GetVertexData();
		memset(vdata, 0, sizeof(GRPVERTEX)*n);

		int nPoints = Points->GetSize() - 1;

		if (nPoints >= 2)
				for (int e = 0; e<nPoints; e += 2)
				{
						Dummy->GetMesh()->vertexdata[i].position[0] = Points->Get(e)->x*Scale;
						Dummy->GetMesh()->vertexdata[i].position[1] = Points->Get(e)->y*Scale;
						Dummy->GetMesh()->vertexdata[i].position[2] = Points->Get(e)->z*Scale;
						Dummy->GetMesh()->indexdata[i] = i;
						i++;
						Dummy->GetMesh()->vertexdata[i].position[0] = Points->Get(e + 1)->x*Scale;
						Dummy->GetMesh()->vertexdata[i].position[1] = Points->Get(e + 1)->y*Scale;
						Dummy->GetMesh()->vertexdata[i].position[2] = Points->Get(e + 1)->z*Scale;
						Dummy->GetMesh()->indexdata[i] = i;
						i++;
				}


		Dummy->SetDefaultColor(GRPCOLOR(1.0f, 1.0f, 1.0f, 0.1f));

		Dummy->SetRenderMode(GRPRENDERMODE_LINES);
		Dummy->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));

		return Dummy;
}

/*-------------------------------------------------------------------
//	 GRPDEBUGGERPASS::SetValue
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	13/10/2017 13:17:25
//	@return		bool : 
//
//	@param		XDWORD : 
//	@param		double : 
//
*//*-----------------------------------------------------------------*/
bool GRPDEBUGGERPASS::SetValue(XDWORD type, double value)
{
		switch (type)
		{
		case GRPDEBUGGERPASS::DEBUG_SIZE: this->DebugSize = (float)value; return true;;
		}
		return GRPPASS::SetValue(type,value);
}

/*-------------------------------------------------------------------
//	 GRPDEBUGGERPASS::SetValue
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	13/10/2017 13:17:15
//	@return		bool : 
//
//	@param		XDWORD : 
//	@param		XDWORD : 
//
*//*-----------------------------------------------------------------*/
bool GRPDEBUGGERPASS::SetValue(XDWORD type, XDWORD value)
{
		switch (type)
		{
		case GRPDEBUGGERPASS::DEBUG_FLAGS: this->DebugFlags = value; return true;
		}
		return GRPPASS::SetValue(type, value);
}


/*-------------------------------------------------------------------
//	 GRPDEBUGGERPASS::CreateLightLocator
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		GRPLIGHT* : 
//
*//*-----------------------------------------------------------------*/
bool GRPDEBUGGERPASS::CreateLightLocator(GRPLIGHT* light, float DebugSize)
{
		XVECTOR<GRPPOINT*> Points;

		GRPPOINT*		p = new GRPPOINT(+0.0, +1.5, +0.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(-1.0, +0.0, +1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+0.0, +1.5, +0.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+1.0, +0.0, +1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+0.0, +1.5, +0.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+1.0, +0.0, -1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+0.0, +1.5, +0.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(-1.0, +0.0, -1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(-1.0, +0.0, -1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+1.0, +0.0, -1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+1.0, +0.0, -1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+1.0, +0.0, +1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+1.0, +0.0, +1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(-1.0, +0.0, +1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(-1.0, +0.0, +1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(-1.0, +0.0, -1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+0.0, -1.5, +0.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(-1.0, +0.0, +1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+0.0, -1.5, +0.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+1.0, +0.0, +1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+0.0, -1.5, +0.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+1.0, +0.0, -1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(+0.0, -1.5, +0.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);
		p = new GRPPOINT(-1.0, +0.0, -1.0); if (p == NULL) { Points.DeleteContents(); return NULL; }	Points.Add(p);

		GRPELEMENT* Dummy = ConstructDummy(&Points, DebugSize);

		Points.DeleteContents();
		Points.DeleteAll();		

		this->RenderOrder->Add(Dummy);
		GRPNODE* node = holderscene->CreateNode();
		Dummy->SetDefaultColor(GRPCOLOR::YELLOW);
		Dummy->SetNode(node);

		Dummy->GetNode()->GetBaseMaterial()->SetDiffuse(GRPCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		Dummy->GetNode()->SetParent(light);

		return true;
}

/*-------------------------------------------------------------------
//	 GRPDEBUGGERPASS::CreateBoundingBox
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		GRPELEMENT* : 
//
*//*-----------------------------------------------------------------*/
bool GRPDEBUGGERPASS::CreateBoundingBox(GRPELEMENT* element, float DebugSize)
{
	XVECTOR<GRPPOINT*> Points;
		if (!element->GetMesh())
				return NULL;
		

		element->UpdateBoundingBox();

	GRPAABB*	aabb = element->GetBoundingBox();
						aabb->Update();

	GRPPOINT* p=NULL;
						p=new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
	
						p=new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->maxpoint.x,aabb->minpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->minpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->minpoint.x,aabb->maxpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

						p=new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->minpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);
						p=new GRPPOINT(aabb->maxpoint.x,aabb->maxpoint.y,aabb->maxpoint.z); if (p==NULL) { Points.DeleteContents(); return NULL; } Points.Add(p);

	GRPELEMENT* Dummy=ConstructDummy(&Points,1.0f);

	Points.DeleteContents();
	Points.DeleteAll();

	this->RenderOrder->Add(Dummy);
  
	GRPNODE* dummynode=holderscene->CreateNode();
	Dummy->SetDefaultColor(GRPCOLOR(1.0f, 1.0f, 1.0f, 0.1f));
	Dummy->SetNode(dummynode);
	dummynode->SetName(__L("Debug::Dummy::BoundingBox::"));
	dummynode->GetName()->Add(element->GetNode()->GetName()->Get());
	Dummy->GetNode()->SetParent(element->GetNode());
	return true;
}

/*-------------------------------------------------------------------
//	 GRPDEBUGGERPASS::CreateNormals
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	13/10/2017 13:02:49
//	@return		bool : 
//
//	@param		GRPELEMENT* : 
//	@param		float : 
//
*//*-----------------------------------------------------------------*/
bool GRPDEBUGGERPASS::CreateNormals(GRPELEMENT* element, float DebugSize)
{
		{
				if (!element->GetMesh())
						return NULL;

				XVECTOR<GRPPOINT*> Points;
			
				GLFLOAT Sc = (float)this->DebugSize;
				GRPVERTEX* vdata = element->GetMesh()->GetVertexData();
				XDWORD n = element->GetMesh()->GetNVertex();
				for (XDWORD e = 0; e<n; e++)
				{
						GRPVERTEX* vertex = &vdata[e];
						GRPPOINT* p = new GRPPOINT(vertex->position[0], vertex->position[1], vertex->position[2]);
						if (p == NULL) return NULL;
						Points.Add(p);
						p = new GRPPOINT(vertex->position[0] + (vertex->normal[0] * Sc), vertex->position[1] + (vertex->normal[1] * Sc), vertex->position[2] + (vertex->normal[2] * Sc));
						if (p == NULL) return NULL;
						Points.Add(p);
				}

				GRPELEMENT* Dummy = NULL;
				if (Points.GetSize() >= 2)
						Dummy = ConstructDummy(&Points, 1.0f);
				else
						XDEBUG_PRINTCOLOR(4, __L("El elemento %s tiene menos de 3 vertices"), element->GetName()->Get());


				Points.DeleteContents();
				Points.DeleteAll();

				this->RenderOrder->Add(Dummy);
				GRPNODE* node = holderscene->CreateNode();
								 node->SetName(__L("Debug::Normals::")); node->GetName()->Add(element->GetName()->Get());

				Dummy->SetDefaultColor(GRPCOLOR::WHITE);
				Dummy->SetNode(node);

				Dummy->GetNode()->GetBaseMaterial()->SetDiffuse(GRPCOLOR::WHITE);
				Dummy->GetNode()->SetParent(element->GetNode());			
		}
		return true;
}

/*-------------------------------------------------------------------
//	 GRPDEBUGGERPASS::CreateLocator
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		GRPNODE* : 
//	@param		float : 
//
*//*-----------------------------------------------------------------*/
bool GRPDEBUGGERPASS::CreateLocator(GRPNODE* element,float DebugSize)
{

		XVECTOR<GRPPOINT*> Points;
		float a = DebugSize;

		GRPPOINT*		p = new GRPPOINT(-a,		+0.0,		+0.0); 	Points.Add(p);
		if (p)			p = new GRPPOINT(+a,		+0.0,		+0.0); 	Points.Add(p);
		if (p)			p = new GRPPOINT(+0.0,  -a,			+0.0); 	Points.Add(p);
		if (p)			p = new GRPPOINT(+0.0,  +a,			+0.0);	Points.Add(p);
		if (p)			p = new GRPPOINT(+0.0,  +0.0,		-a);		Points.Add(p);
		if (p)			p = new GRPPOINT(+0.0,  +0.0,		+a);		Points.Add(p);
		if (!p) return false;

		GRPELEMENT* Dummy = ConstructDummy(&Points, 1.0f);

		Points.DeleteContents();
		Points.DeleteAll();

		this->RenderOrder->Add(Dummy);
		GRPNODE* node = holderscene->CreateNode();
		XDEBUG_PRINTCOLOR(3, __L("Created debug node: %x"), node);


		Dummy->SetDefaultColor(GRPCOLOR::BLUE);
		Dummy->SetNode(node);
		node->SetName(__L("Debug::Locator::"));node->GetName()->Add(element->GetName()->Get());

		Dummy->GetNode()->GetBaseMaterial()->SetDiffuse(GRPCOLOR::BLUE);
		Dummy->GetNode()->SetParent(element);

		return true;
}