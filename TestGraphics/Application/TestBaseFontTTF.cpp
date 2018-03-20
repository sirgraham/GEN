
/*------------------------------------------------------------------------------------------
//	TESTBASEFONTTTF.CPP
//	
//	Test Base Font TTF
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
#include "GRPCamera.h"
#include "GRPMaterialLibrary.h"
#include "GRPShaderLibrary.h"
#include "GRPFont.h"
#include "GRPFontManager.h"
#include "GRPRenderer.h"
#include "GRPVisualDebugger.h"

#include "TestBaseFontTTF.h"

#include "GRPCamera.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	TESTBASEFONTTTF::IniResources
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
bool TESTBASEFONTTTF::IniResources()
{	
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("fontttf"));

	//----------------------------------- material default location
	XPATH xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	//---------------------------------- Loading fonts
	GRPFONTATTRIBUTES att;
										att.front=true;
										att.back =false;
										att.extrude=-0.02f;
										att.quality=7;
										att.unifynormals=true;
										att.reversenormals = false;

										
		xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, __L("arial.ttf"));		

		if(!GRPOBJECTBUILDER::Get()->GetFontManager()->Load(xpath.Get(),		__L("arial.ttf"),&att)) //load default
		{
			XDEBUG_PRINTCOLOR(1,__L("TTF file not loaded!"));
			return false;
		}

//--------------------------------------- load object
		
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L("test_fonts.FBX"));

	GRPOBJECT* obj=NULL;	
	if(!(obj = LoadObject(obj,xpath.Get())))		
		{
			XDEBUG_PRINTCOLOR(4,__L("Loading FBX Failed!"));
			return false;
		}
	
	scene->Add(obj);

	//----------------------------------- Cargar un material	
	/*
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("oro_512.tga"));	XDEBUG_PRINTCOLOR(1,__L("%s"),xpath.Get());

	GRPMATERIAL* material=0;

	if(!GRPMATERIALLIBRARY::Get()->Load(xpath.Get(),	__L("oro_512.tga")))
		{
			XDEBUG_PRINTCOLOR(1,__L("Error loading Texture data"));
			return false;
		}						

							textelement3=NULL;
							GRPNODE* node=scene->FindNode(__L("Model::Point003"));
							if (node)
							{
														textelement3 = GRPOBJECTBUILDER::Get()->GetFontManager()->GetFont(__L("arial.ttf"))->TextBox(scene,GRPTEXTBOX_ALIGNCENTER,__L("X"));
														textelement3->SetNode(node);
														textelement3->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"DOT"));
														textelement3->SetMaterial(GRPMATERIALLIBRARY::Get()->Select(__L("oro_512.tga")));
														textelement3->GetNode()->SetScale(100.5,100.5,100.5);
														textelement3->UpdateContent();
								scene->Add (textelement3);
							}

							
							
							obj->PlayAll();
							*/
							scene->GetVisualDebugger()->DebugSize=0.05f;
							scene->GetVisualDebugger()->CreateScene(GRPVISUALDEBUGGER_FLAG_ALL);  
							scene->ShowDebug(GRPDEBUG_FLAG_NORMAL);	

	return true;
}





/*-------------------------------------------------------------------
//	TESTBASEFONTTTF::Do
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
bool TESTBASEFONTTTF::Do()
{
	/*
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	static int x=0,i=0;
	x++;
	i++;

	//if (i%10==0)
	
	static int e=0;
	//for (int e=0;e<4;e++)
	{		
		e++;
		dynamic_cast<GRPTEXTELEMENT*>(scene->GetObjects()->Get(0)->GetElements()->Get(1))->SetValue(GRPANIMATIONCURVETYPE_USERVALUE,GRPANIMATIONCURVEAXIS_USERVALUE,double(e),0);
		if (textelement3)
		{
			textelement3->SetValue(GRPANIMATIONCURVETYPE_USERVALUE,GRPANIMATIONCURVEAXIS_USERVALUE,double(e),0);
		}
	}
	*/
	//*/
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASEFONTTTF::Notify
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 13:44:47
//	
//	@return 			bool : 
//
//  @param				msg : 
//  @param				notifier : 
*/
/*-----------------------------------------------------------------*/
bool TESTBASEFONTTTF::Notify(GRPMSGTYPE msg,GRPSUBJECT* notifier)
{
	return false;
}

/*-------------------------------------------------------------------
//	TESTBASEFONTTTF::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 13:44:47
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASEFONTTTF::EndResources()
{
	return true;
}