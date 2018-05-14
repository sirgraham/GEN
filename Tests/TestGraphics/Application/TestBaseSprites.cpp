
/*------------------------------------------------------------------------------------------
//	TESTBASESPRITES.CPP
//	
//	Test Base Sprites
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
#include "GRPRenderer.h"
#include "GRPVisualDebugger.h"

#include "TestBaseSprites.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

TESTBASESPRITES::TESTBASESPRITES(GRPAPPLICATION* app) : TESTBASE(app) 
{

}


/*-------------------------------------------------------------------
//	TESTBASESPRITES::IniResources
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
bool TESTBASESPRITES::IniResources()
{	
	if(!xapplication)										 return NULL;
	if(!xapplication->GetMainScene())		 return NULL;

	GRPSCENE* scene  = xapplication->GetMainScene();

	PathManagement(__L("sprites"));

	//----------------------------------- material default location
	XPATH xpath;
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,__L(""));
	GRPMATERIALLIBRARY::Get()->SetResourceLocation(xpath);

	//----------------------------------- Cargar un material		
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("particle.png"));	XDEBUG_PRINTCOLOR(1,__L("%s"),xpath.Get());

	if(!GRPMATERIALLIBRARY::Get()->Load(xpath.Get(),	__L("particle.png")))
		{
			XDEBUG_PRINTCOLOR(1,__L("Error loading Texture data"));
			return false;
		}				

	
	xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1, __L("test1.fbx"));
	if(!(obj = LoadObject(obj,xpath.Get())))		
		{
			XDEBUG_PRINTCOLOR(4,__L("\t\tLoading FBX Failed!"));
			return false;
		}
	
	GRPMATERIAL * material=GRPMATERIALLIBRARY::Get()->Select(	__L("particle.png"));
	
	//scene->renderer->GetObjectBuilder()->CreateMasterSprite();
	/*
	GRPPARTICLESYSTEM*	ps					=scene->CreateParticleSystem();
	GRPPARTICLEPOOL*		pool				=ps->CreatePool();
											pool->SetSprite(scene->renderer->GetObjectBuilder()->GetMasterSprite());
								
		XDEBUG_PRINTCOLOR(2,__L("Creating particles"));

		int nx=4;
		int ny=4;
		int nz=2;

		float z=0.0f;
		float y=0.0f;

		for ( z=-0.5f;z<0.5f;z+=1.0f/nz)
		{
			for (float y=-0.5f;y<0.5f;y+=1.0f/ny)
				for (float x=-0.5f;x<0.5f;x+=1.0f/nx)
				{	
					GRPPARTICLE* particle=pool->Add();
					particle->SetPosition(x,y,z);			
					particle->SetSize(0.1f);
				}
				XDEBUG_PRINTCOLOR(2,__L("...%d "),pool->GetSize());
		}
		pool->Init();		
		pool->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SSS"));
		pool->SetMaterial(material);
		scene->Add(ps);		

	//	*/
	//	scene->GetCamera()->SetPosition(0.0f,1.2f,10.0f);

	/*
				GRPTEXTBOX* object=scene->GetObjectBuilder()->GetFontManager()->GetFont(__L("suigeneris"))->TextBox(scene,GRPTEXTBOX_ALIGNCENTER,__L("ROJO"));
 
			 GRPMATERIAL* mat=new GRPMATERIAL();
										mat->GetAttributes()->SetDiffuse(GRPCOLOR::Red);
										mat->GetAttributes()->isupdated=true;
										
										scene->Add(mat);

       object->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"DDF"));
			 object->SetMaterial(mat);
			 object->GetNode()->SetPosition(-0.5f,-0.3f,0.0f);
 			 object->GetNode()->SetScale(4.0f,4.2f,4.0f);
       scene->Add(object);

										object=scene->GetObjectBuilder()->GetFontManager()->GetFont(__L("suigeneris"))->TextBox(scene,GRPTEXTBOX_ALIGNCENTER,__L("AZUL"));
 
										mat=new GRPMATERIAL();
										mat->GetAttributes()->SetDiffuse(GRPCOLOR::Blue);
										mat->GetAttributes()->isupdated=true;

										scene->Add(mat);

       object->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"DDF"));
			 object->SetMaterial(mat);
			 object->GetNode()->SetPosition(-0.5f,0.3f,0.0f);
			 object->GetNode()->SetScale(4.0f,4.2f,4.0f);

       scene->Add(object);
			 //GRPLIGHT* l=scene->CreateLight();			 l->SetPosition(0.0,1.0,0.0);
		
										object=scene->GetObjectBuilder()->GetFontManager()->GetFont(__L("suigeneris"))->TextBox(scene,GRPTEXTBOX_ALIGNCENTER,__L("VERDE"));
 
										mat=new GRPMATERIAL();
										mat->GetAttributes()->SetDiffuse(GRPCOLOR::Green);
										mat->GetAttributes()->isupdated=true;

										scene->Add(mat);

       object->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"DDF"));
			 object->SetMaterial(mat);
			 object->GetNode()->SetPosition(-0.5f,-0.6f,0.0f);
			 object->GetNode()->SetScale(4.0f,4.2f,4.0f);


			 //scene->GetVisualDebugger()->CreateScene(GRPVISUALDEBUGGER_FLAG_BB | GRPVISUALDEBUGGER_FLAG_LIGHTS);

 obj=object;
 */
	XDEBUG_PRINTCOLOR(0,__L("Resources Initialized"));

	//scene->PlayAll();

	return true;
}





/*-------------------------------------------------------------------
//	TESTBASESPRITES::Do
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
bool TESTBASESPRITES::Do()
{
	static float	x			 =	0;
	GRPSCENE*			scene  = xapplication->GetMainScene();

	x+=0.05f;	

//	scene->GetParticleSystems()->GetLast()->GetPool(0)->GetNode()->SetRotation(sin(x),sin(x),sin(x));

	//scene->GetObjects()->GetLast()->GetNode()->SetRotation(360.0f*sin(x),1.0,360.0f*cos(x));
	//scene->GetObjects()->GetLast()->GetNode()->SetRotation(360.0f*sin(x),1.0,360.0f*cos(x));
	//const GRPVECTOR* rot=obj->GetNode()->GetRotation();	obj->GetNode()->SetRotation(rot->vector[0]+0.1f,rot->vector[1]+0.1f,rot->vector[2]+0.1f);
	
	return true;
}




/*-------------------------------------------------------------------
//	TESTBASESPRITES::Notify
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
bool TESTBASESPRITES::Notify(GRPMSGTYPE, GRPSUBJECT*)
{
	return false;
}

/*-------------------------------------------------------------------
//	TESTBASESPRITES::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/06/2016 13:52:51
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASESPRITES::EndResources()
{
	return false;
}