/*------------------------------------------------------------------------------------------
// TESTSHADERLOADER.CPP
*/
/**
// \class
//
// Loads shader from file
//
//	@author: No se ha podido abrir el fichero de author
//
//	Date of Creation : 23/07/2017 3:43:35 
//	Last Modification : 
*/
/* (C) Copyright . All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "TestShaderLoader.h"
#include "GRPShaderLibrary.h"
#include "GRPBackgroundPass.h"
#include "XApplication.h"
#include "XFileTXT.h"
#include "GRPScene.h"
#include "TestBase.h"
#include "GRPMaterialLibrary.h"
#include "SNDFactory.h"
#include "SNDElement.h"
#include "GRPFontManager.h"
#include "GRPFont.h"
#include "TestGraphics.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

//char fragmentshader[] =
//"void main() { gl_FragColor=vec4(1.0,0.4,0.2,1.0);}";
char * fragmentshader = NULL;

TESTBASESHADERLOADER::TESTBASESHADERLOADER(GRPAPPLICATION* xapplication) : TESTBASE(xapplication)
{
	Clean();
}

TESTBASESHADERLOADER::~TESTBASESHADERLOADER()
{

}

bool TESTBASESHADERLOADER::IniResources()
{
	XPATH xpath;
	PathManagement(__L("shaderloader"));
	

	GRPSCENE* scene = xapplication->GetMainScene();

	scene->GetViewport()->SetProjection(GRPPROJECTIONTYPE_ORTHO);
	scene->GetViewport()->SetRealScreen(xapplication->mainscreen);
	scene->GetCamera()->SetPosition	(0.0, 0.0, 100.0);
	scene->GetCamera()->Setinterestposition(0.0, 0.0, -100.0);
	scene->GetCamera()->SetRotation	(0.0, 0.0, 0.0);

	//----------------------------------- material default location
	


	//---------------------------------- Loading fonts
	GRPFONTATTRIBUTES att;
	att.front = true;
	att.back = false;
	att.extrude = 0.0f;
	att.quality = 7;
	att.unifynormals = true;


	xpath.Create(XPATHSMANAGERSECTIONTYPE_FONTS, 1, __L("arial.ttf"));
	if (!GRPOBJECTBUILDER::Get()->GetFontManager()->Load(xpath.Get(), __L("arial.ttf"), &att)) //load default
	{
		XDEBUG_PRINTCOLOR(1, __L("TTF file not loaded!"));
		return false;
	}




#ifdef SND_ACTIVE

	SNDFACTORY* soundsystem = SNDFACTORY::Get();
	if (!soundsystem)
	{
		return false;
	}

	if (!soundsystem->AddFile(__L("music.ogg")))
	{
		return false;
	}

	SNDELEMENT* sndelement = soundsystem->GetFile(__L("music.ogg"));
	if (!sndelement)
	{
		return false;
	}
	sndelement->SetLoop(true);

	soundsystem->SetMasterVolume(1.0f);
	soundsystem->PlaySound(sndelement);

#endif

	
	//------text

	tb[0]=GRPOBJECTBUILDER::Get()->GetFontManager()->GetFont(__L("arial.ttf"))->CreateTextBox(scene, __L("KiwiScene::Altar of naught"),0);
	tb[0]->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Diffuse")));
		tb[0]->GetNode()->SetPosition(-0.97, 0.48,0.0);
	tb[0]->GetNode()->SetScale(0.3, 0.3, 1.0);
	tb[0]->UpdateContent();


	scene->Add(tb[0]);

	tb[1] = GRPOBJECTBUILDER::Get()->GetFontManager()->GetFont(__L("arial.ttf"))->CreateTextBox(scene, __L("by Anarthal"), 0);
	tb[1]->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Diffuse")));
	tb[1]->GetNode()->SetPosition(-0.97, -0.48, 0.0);
	tb[1]->GetNode()->SetScale(0.3, 0.3, 1.0);
	tb[1]->UpdateContent();
	scene->Add(tb[1]);

	tb[2] = GRPOBJECTBUILDER::Get()->GetFontManager()->GetFont(__L("arial.ttf"))->CreateTextBox(scene, __L("thanks to Kulebril"), 2);
	tb[2]->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Diffuse")));
	tb[2]->GetNode()->SetPosition(+0.97, -0.48, 0.0);
	tb[2]->GetNode()->SetScale(0.3, 0.3, 1.0);
	tb[2]->UpdateContent();
	scene->Add(tb[2]);

	tb[3] = GRPOBJECTBUILDER::Get()->GetFontManager()->GetFont(__L("arial.ttf"))->CreateTextBox(scene, __L("music by Kevin MacLeod(incompetech.com) "), 2);
	tb[3]->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Diffuse")));
	tb[3]->GetNode()->SetPosition(+0.97, -0.52, 0.0);
	tb[3]->GetNode()->SetScale(0.1, 0.1, 1.0);
	tb[3]->UpdateContent();
	scene->Add(tb[3]);
	tb[4] = GRPOBJECTBUILDER::Get()->GetFontManager()->GetFont(__L("arial.ttf"))->CreateTextBox(scene, __L(" License http ://creativecommons.org/licenses/by/3.0/"), 2);
	tb[4]->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Diffuse")));
	tb[4]->GetNode()->SetPosition(+0.97, -0.55, 0.0);
	tb[4]->GetNode()->SetScale(0.1, 0.1, 1.0);
	tb[4]->UpdateContent();
	scene->Add(tb[4]);
	

	//----- shader


	GRPBACKGROUNDPASS* bgpass = new GRPBACKGROUNDPASS(__L("Backgroundpass"));
	xapplication->mainscene->Add(bgpass);

	FILE* f = fopen("./resources/shaderloader/graphics/shader.txt","rt");
	if (f)
	{
		fseek(f, 0, SEEK_END);
		long size = ftell(f);
		fseek(f, 0, SEEK_SET);		
		fragmentshader = new char[size+1];		
		if (fragmentshader)
		{
			memset(fragmentshader, 0, size);
			fread(fragmentshader, sizeof(char), size, f);
			fragmentshader[size] = '\0';
		}
	}

	GRPSHADERLIBRARY::Get()->LoadShader(
		0,
		(char*)"test",
		(char*)"tst",
		(char*)GRPSHADERLIBRARY::Get()->PostProcessing,
		(char*)fragmentshader,
		(char*)""
		);
	
	p= GRPSHADERLIBRARY::Get()->GetProgramByName(__L("tst"));	
	p->GetID();
	//bgpass->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Testr")));
	bgpass->SetProgram(p);
	

	glUniform2f(p->iResolutionHandle, xapplication->GetMainScreen()->GetWidth(), xapplication->GetMainScreen()->GetHeight());//*/
	return true;
}
bool TESTBASESHADERLOADER::EndResources()
{
	return true;
}
bool TESTBASESHADERLOADER::Do()
{
	static float iGlobalTime = 0;
	iGlobalTime += 0.01f;
	GRPSHADER::UseShader(p);
	glUniform1f(p->iGlobalTimeHandle, iGlobalTime);

	if (iGlobalTime >= (dynamic_cast<TESTGRAPHICS*>(xapplication))->mt)
		this->xapplication->SetExitStatus(XAPPLICATIONEXITTYPE_APPEND);

		for (int e = 0; e < 5; e++)
		{
			switch (e)
			{
			case 0:break;
			case 1:tb[e]->GetElements()->Get(0)->SetDefaultColor(GRPCOLOR(0.56, 0.0, 1.0, 1.0)); break;
			case 2:
			case 3:
			case 4: tb[e]->GetElements()->Get(0)->SetDefaultColor(GRPCOLOR(0.7, 0.0, 0.0, 1.0)); break;
			}
			if (iGlobalTime >= 2.90)
			tb[e]->SetVisible(false);
		}
	
	//	console->Update();
	return true;
}