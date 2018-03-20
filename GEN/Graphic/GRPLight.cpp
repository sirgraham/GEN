
/*------------------------------------------------------------------------------------------
//	GRPLIGHT.CPP
//	
//	Lights
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 25/05/2015 11:13:38
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPLight.h"
#include "GRPViewport.h"
#include "GRPAnimation.h"
#include "GRPFactory.h"
#include "GRPRenderTarget.h"
#include "GRPMaterialLibrary.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	GRPLIGHT::~GRPLIGHT
*/	
/**	
//	
//	 Class Destructor GRPLIGHT
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			27/07/2016 13:31:40
//	
*/
/*-----------------------------------------------------------------*/
GRPLIGHT::~GRPLIGHT()
{
	Publish(GRPMSG_DESTROYED);
	if (this->rendertarget)
	{
		delete(this->rendertarget);
		rendertarget=NULL;
	}
}

/*-------------------------------------------------------------------
//	GRPLIGHT::BuildData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/05/2015 11:17:22
//	
*/
/*-----------------------------------------------------------------*/
void GRPLIGHT::BuildData()
{
		//--------------- lightdatas
		GLFLOAT m[4][4];

		//	GRPNODE::Update();

		m[0][0]	= GetPosition()->vector[0];
		m[0][1]	= GetPosition()->vector[1];
		m[0][2]	= GetPosition()->vector[2];
		m[0][3]	= 1.0f;
		
		m[1][0]	=	ConstantAttenuation+1.0f;
		m[1][1]	=	LinearAttenuation;
		m[1][2]	=	QuadraticAttenuation;
		m[1][3]	=	Strength; 	

		if (target)
		{
			Direction=(target->GetWorldPosition()-position);
			Direction.Normalize();
		}

		m[2][0] = this->Direction.vector[0];
		m[2][1] = this->Direction.vector[1];
		m[2][2] = this->Direction.vector[2];
		m[2][3] = 1.0;
	
		m[2][0] = this->SpotInner;
		m[2][1] = this->SpotOuter;
		m[2][2] = 1.0;
		m[2][3] = 1.0;

		LightDataMatrix.BuildMatrixFromFloatArray(&(m[0][0]));	

		//-----------------lightcolor
		m[0][0]=this->Ambient.GetRed();
		m[0][1]=this->Ambient.GetGreen();
		m[0][2]=this->Ambient.GetBlue();
		m[0][3]=1.0f;

		m[1][0]=this->Diffuse.GetRed();
		m[1][1]=this->Diffuse.GetGreen();
		m[1][2]=this->Diffuse.GetBlue();
		m[1][3]=1.0f;

		m[2][0]=this->Specular.GetRed();
		m[2][1]=this->Specular.GetGreen();
		m[2][2]=this->Specular.GetBlue();
		m[2][3]=1.0f;

		m[3][0]=0.0f;
		m[3][1]=0.0f;
		m[3][2]=0.0f;
		m[3][3]=1.0f;

		LightColorMatrix.BuildMatrixFromFloatArray(&(m[0][0]));	

		if (this->Type==GRPLIGHTTYPE_SPOTLIGHT)
		{
				if (!this->rendertarget)
					BuildShadowMap();
		}
}

/*-------------------------------------------------------------------
//	SetAttenuation
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			27/07/2015 12:04:17
//	
//  @param				constant : 
//  @param				linear : 
//  @param				quadric : 
//  @param				cubic : 
*/
/*-----------------------------------------------------------------*/
void	GRPLIGHT::SetAttenuation							(GLFLOAT constant,GLFLOAT linear,GLFLOAT quadric,GLFLOAT cubic)
{
	ConstantAttenuation		= constant;
	LinearAttenuation			= linear;
	QuadraticAttenuation	= quadric;
	CubicAttenuation			= cubic;
}

/*-------------------------------------------------------------------
//	GRPLIGHT::Update
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/05/2015 11:17:29
//	
//  @param				viewmatrix : 
*/
/*-----------------------------------------------------------------*/
void GRPLIGHT::Update(GRPMATRIX* viewmatrix)	
{						
		if (!this->worldmatrix_is_pending)	return;

		GRPVECTOR lightViewPosition=viewmatrix->TransformPoint(*GetPosition());
						
							LightDataMatrix.matrix[0][0]=lightViewPosition.vector[0];
							LightDataMatrix.matrix[0][1]=lightViewPosition.vector[1];
							LightDataMatrix.matrix[0][2]=lightViewPosition.vector[2];
							LightDataMatrix.matrix[0][3]=1.0f;

							LightDataMatrix.matrix[1][3]=(Strength/100.0f)*LightScaleFactor*Multiplier;

		if (GetTarget()!=NULL)
		{
		GRPCAMERA::Update();
		GRPCAMERA::BuildLookAtMatrix();

		GRPVECTOR lightViewTarget=viewmatrix->TransformPoint(*GetTarget()->GetPosition());

							LightDataMatrix.matrix[2][0]=lightViewTarget.vector[0];
							LightDataMatrix.matrix[2][1]=lightViewTarget.vector[1];
							LightDataMatrix.matrix[2][2]=lightViewTarget.vector[2];
							LightDataMatrix.matrix[2][3]=1.0f;
						

	
		}

		GRPMATRIX* lightprojmatrix = viewport;
		lightSpaceMatrix.GetMultiplicationMatrix(lightprojmatrix, &this->viewmatrix);



}


/*-------------------------------------------------------------------
//	GRPLIGHT::CopyFrom
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/06/2015 13:28:46
//	
//  @param				light : 
*/
/*-----------------------------------------------------------------*/
bool GRPLIGHT::CopyFrom(GRPLIGHT* light)
{
	if (light==NULL)
	{
		return false;
	}

	this->Diffuse								= light->Diffuse;
	this->Ambient								= light->Ambient;
	this->Specular							= light->Specular;
	this->DecayType							= light->DecayType;
	this->ConstantAttenuation		= light->ConstantAttenuation;
	this->LinearAttenuation			= light->LinearAttenuation;
	this->CubicAttenuation			= light->CubicAttenuation;
	this->QuadraticAttenuation	= light->QuadraticAttenuation;
	this->Strength							= light->Strength;
	this->Type									= light->Type;
	this->LightScaleFactor			= light->LightScaleFactor;
	this->Multiplier						=	light->Multiplier;
	this->bias									= light->bias;

	switch (Type)
	{
	case GRPLIGHTTYPE_POINT:			GRPCAMERA::SetType(GRPCAMERA_TYPE_FREE);			break;
	case GRPLIGHTTYPE_DIRECTIONAL:
																this->SetTarget(light->target);
																this->GetTarget()->SetParent(this);
																this->GetViewport()->SetProjection(GRPPROJECTIONTYPE_ORTHO);
	default:											GRPCAMERA::SetType(GRPCAMERA_TYPE_TARGET);		break;
	}

	this->fov					= light->fov;
	this->FocalLength = light->FocalLength;

	if (light->GetUpNode()!=NULL)
	{
		this->SetUpNode(light->GetUpNode());
		this->GetUpNode()->SetParent(this);
	}

	return true;
}

	

/*-------------------------------------------------------------------
//	GRPLIGHT::SetValue
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/06/2015 13:01:06
//	
//  @param				type : 
//  @param				subtype : 
//  @param				value : 
//  @param				base : (default e=0.0)
*/
/*-----------------------------------------------------------------*/
bool GRPLIGHT::SetValue(GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS subtype, double value,double base)
{
	bool r=true;
				switch (type)
				{	
				case GRPANIMATIONCURVETYPE_INTENSITY:				this->SetStrength((GLFLOAT)value);		this->modified=true;					break;
				case GRPANIMATIONCURVETYPE_DIFFUSECOLOR:		switch(subtype)
																										{
																											case GRPANIMATIONCURVEAXIS_X	:
																											case GRPANIMATIONCURVECOLOR_R	:	this->Diffuse.SetRed	((GLFLOAT)value);this->modified=true;	break;

																											case GRPANIMATIONCURVEAXIS_Y	:
																											case GRPANIMATIONCURVECOLOR_G	:	this->Diffuse.SetGreen((GLFLOAT)value);this->modified=true;	break;

																											case GRPANIMATIONCURVEAXIS_Z	:
																											case GRPANIMATIONCURVECOLOR_B	:	this->Diffuse.SetBlue	((GLFLOAT)value);this->modified=true;	break;		

																											case GRPANIMATIONCURVECOLOR_A	:	this->Diffuse.SetAlpha((GLFLOAT)value);this->modified=true;	break;
																											default: break;
																										}																										
																										break;
				default:																		r=GRPNODE::SetValue(type,subtype,value,base);	this->modified=true;		break;
				}

	if (this->modified==true)
		this->BuildData();

	return r;
}


/*-------------------------------------------------------------------
//	GRPLIGHT::BuildShadowMap
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/07/2016 16:00:24
//	
*/
/*-----------------------------------------------------------------*/
bool GRPLIGHT::BuildShadowMap()
{
OPENGLCHECKERROR(__L("GRPLIGHT::BuildShadowMap Enter"));
if (this->rendertarget)
{
		delete(this->rendertarget);
		rendertarget = NULL;
}


				if (viewport)
				{						
						this->viewport.SetWindowSize		(512, 512);
						this->viewport.Select						();
						this->CreateDepthBuffer					(512, 512);
						
				}
OPENGLCHECKERROR(__L("GRPLIGHT::BuildShadowMap Exit"));
return true;
}

/*-------------------------------------------------------------------
//	GRPLIGHT::SetViewport
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/07/2016 16:21:37
//	
//	@return 			bool : 
//
//  @param				viewport : 
*/
/*-----------------------------------------------------------------*/
bool GRPLIGHT::SetViewport(GRPVIEWPORT* viewport)
{
			this->viewport=*viewport;		
			
			GLFLOAT w,h;

			this->viewport.GetRealWindowSize(w,h);
			this->viewport.SetWindowSize(w*shadowquality,h*shadowquality);		
			this->viewport.Select();
			this->fov=45.0f;
			
	return true;
}