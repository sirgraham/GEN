/*------------------------------------------------------------------------------------------
// GRPDEPTHPASS.CPP
*/
/**
// \class
//
// Manages depth prepass
//
//	@author: Diego Martinez Ruiz de Gaona 
//
//	Date of Creation : 30/10/2017 11:57:21 
//	Last Modification : 
*/
/* (C) Copyright . All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPDepthPass.h"
#include "GRPScene.h"
#include "GRPShaderLibrary.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	GRPDEPTHPASS::GRPDEPTHPASS
*/
/**
//
//	constructor
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	30/10/2017 11:58:38
//	@return		 : 
//
//	@param		XCHAR* : 
//
*//*-----------------------------------------------------------------*/
GRPDEPTHPASS::GRPDEPTHPASS(XCHAR* name): GRPPASS(name)
{
		Clear();
		SetClone(true);
		SetClear(GRPPASS_CLEAR_FLAG_DEPTH);
		this->AddType(GRPPASS_TYPE_DEPTHFUNC);
		this->AddType(GRPPASS_TYPE_DEPTHWRITE);
		this->AddType(GRPPASS_TYPE_COLORWRITE);

		this->SetShader(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Basic")));
}

/*-------------------------------------------------------------------
//	GRPDEPTHPASS::~GRPDEPTHPASS
*/
/**
//
//	destructor
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	30/10/2017 11:58:43
//	@return		 : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
GRPDEPTHPASS::~GRPDEPTHPASS()
{

}

/*-------------------------------------------------------------------
//	 GRPDEPTHPASS::OnAdded
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	30/10/2017 11:58:47
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool GRPDEPTHPASS::OnAdded()
{		
		GRPPASS* main = holderscene->GetPass();

		main->AddType(GRPPASS_TYPE_COLORWRITE);
		main->AddType(GRPPASS_TYPE_DEPTHFUNC);
		main->AddType(GRPPASS_TYPE_DEPTHWRITE);
		main->SetDepthFunction(GRPDEPTHFUNC_EQUAL);
		main->SetColorWritingIsActive(true);
		main->SetDepthWritingIsActive(false);
		return true;
}

/*-------------------------------------------------------------------
//	 GRPDEPTHPASS::Begin
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	30/10/2017 11:58:52
//	@return		void : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
void GRPDEPTHPASS::Begin()
{
		this->SetDepthFunction			 (GRPDEPTHFUNC_LESS);
		this->SetDepthWritingIsActive(true);
		this->SetColorWritingIsActive(true);
		this->SetGlobalShaderOverride(true);
		GRPPASS::Begin();
}

/*-------------------------------------------------------------------
//	 GRPDEPTHPASS::End
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	30/10/2017 11:58:57
//	@return		void : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
void GRPDEPTHPASS::End()
{
		this->SetDepthFunction(GRPDEPTHFUNC_EQUAL);
		this->SetDepthWritingIsActive(false);
		this->SetColorWritingIsActive(true);
		this->SetGlobalShaderOverride(false);
		this->ApplyFlags();
}