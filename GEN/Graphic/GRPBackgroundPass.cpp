
/*------------------------------------------------------------------------------------------
//	GRPBACKGROUNDPASS.CPP
//	
//	Holds backgrounds
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 12/09/2016 11:12:41
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "grpbackgroundpass.h"
#include "GRPObjectBuilder.h"
#include "GRPElement.h"
#include "GRPScene.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	GRPBACKGROUNDPASS::GRPBACKGROUNDPASS
*/	
/**	
//	
//	Class Constructor GRPBACKGROUNDPASS
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/06/2017 13:06:07
//	
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
GRPBACKGROUNDPASS::GRPBACKGROUNDPASS(XCHAR* name) : GRPPASS(name)
{
	Clean();

	this->passtype=GRPPASS_BACKGROUND;
	priority=999;
	plane=GRPOBJECTBUILDER::Get()->CreatePlane(GRPOBJECTBUILDER::AXIS_SCREEN);
	this->hasToClear=false;
		
	lightingIsEnabled=false;

	program = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("PostProcessing"));
}

/*-------------------------------------------------------------------
//	GRPBACKGROUNDPASS::~GRPBACKGROUNDPASS
*/	
/**	
//	
//	 Class Destructor GRPBACKGROUNDPASS
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/06/2017 13:15:52
//	
*/
/*-----------------------------------------------------------------*/
GRPBACKGROUNDPASS::~GRPBACKGROUNDPASS()
{	
		if (plane)
		{
				plane->SetNode(NULL);
				delete(plane);
				plane = NULL;
		}
		this->RenderOrder->DeleteAll();
}


/*-------------------------------------------------------------------
//	GRPBACKGROUNDPASS::Begin
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/06/2017 13:06:44
//	
*/
/*-----------------------------------------------------------------*/
void GRPBACKGROUNDPASS::Begin()
{	
	this->SetDepthWritingIsActive(false);
	this->SetDepthTestIsActive(true);
	this->DisableStencil();
	this->SetDepthFunction(GRPDEPTHFUNC_EQUAL);
	this->SetBlending(false);

	this->type|=GRPPASS_TYPE_ALPHABLEND;
	this->type|=GRPPASS_TYPE_DEPTHFUNC;
	this->type|=GRPPASS_TYPE_DEPTHTEST;

	GRPPASS::Begin();
}

/*-------------------------------------------------------------------
//	GRPBACKGROUNDPASS::End
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/06/2017 13:07:05
//	
*/
/*-----------------------------------------------------------------*/
void GRPBACKGROUNDPASS::End()
{	
	this->SetDepthWritingIsActive	(true);
	this->SetDepthTestIsActive		(true);
	this->SetDepthFunction				(GRPDEPTHFUNC_LESS);
		GRPPASS::Begin();
}

/*-------------------------------------------------------------------
//	GRPBACKGROUNDPASS::SetMaterial
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/06/2017 13:07:12
//	
//  @param				material : 
*/
/*-----------------------------------------------------------------*/
void GRPBACKGROUNDPASS::SetMaterial(GRPMATERIAL* material)
{
	this->material=material;
	if (plane)
		if (plane->GetNode())
			plane->GetNode()->SetMaterial(material);

}

/*-------------------------------------------------------------------
//	GRPBACKGROUNDPASS::SetProgram
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/06/2017 16:10:31
//	
//  @param				program : 
*/
/*-----------------------------------------------------------------*/
void GRPBACKGROUNDPASS::SetProgram (GRPSHADERPROGRAM* program)
{	
		if (plane)
				if (program)
				{
						this->program = program;
						plane->SetProgram(program);
				}
}

/*-------------------------------------------------------------------
//	GRPBACKGROUNDPASS::OnAdded
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/06/2017 13:15:10
//	
*/
/*-----------------------------------------------------------------*/
bool GRPBACKGROUNDPASS::OnAdded()
{
	plane->SetNode(this->holderscene->CreateNode());
	plane->SetProgram(program);
	
	
	plane->GetNode()->SetScale(100.0);
	plane->SetName(__L("LoaderScreenPlane"));
	plane->GetNode()->SetName(__L("LoaderScreenPlaneNode"));

	plane->GetNode()->SetMaterial(material);
	
	CreateCamera	();


	this->Add(plane);
	return true;
}
