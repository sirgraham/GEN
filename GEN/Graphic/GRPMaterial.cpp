
/*------------------------------------------------------------------------------------------
//	GRPMATERIAL.CPP
//	
//	Graphics Material class
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 07/05/2014 16:33:16
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.	
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPTexture.h"
#include "GRPMaterial.h"
#include "GRPLight.h"
#include "GRPAnimation.h"
#include "GRPProfiler.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	GRPMATERIALATTRIBUTES::UseMaterialData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/04/2016 12:04:15
//	
//  @param				externalIntensity : 
//  @param				link : 
*/
/*-----------------------------------------------------------------*/
void GRPMATERIALATTRIBUTES::UseMaterialData(float externalIntensity, GLINT link)
{
					float i=GetIntensity();			
					
					if (isupdated)
					UpdateMaterialMatrix();
								
					materialMatrix.matrix[0][3]=GetTransparency();
					materialMatrix.matrix[3][1]=i*externalIntensity;
			
					glUniformMatrix4fv(link, (GLsizei)1, GL_FALSE, &(materialMatrix.matrix[0][0]));
}

void GRPMATERIALATTRIBUTES::UseMaterialData( float externalIntensity, GRPCOLOR* diffusecolor, GLINT link )
{
					float i=GetIntensity();			
					
					if (isupdated)
					UpdateMaterialMatrix();
					
					/*
					materialMatrix.matrix[0][0]=(diffusecolor->GetRed		()>0 ? diffusecolor->GetRed		() : 0.0f);
					materialMatrix.matrix[0][1]=(diffusecolor->GetGreen	()>0 ? diffusecolor->GetGreen	() : 0.0f);
					materialMatrix.matrix[0][2]=(diffusecolor->GetBlue	()>0 ? diffusecolor->GetBlue	() : 0.0f);
					materialMatrix.matrix[0][3]=(diffusecolor->GetAlpha	()>0 ? diffusecolor->GetAlpha	() : 0.0f);
					*/
					materialMatrix.matrix[0][3]=GetTransparency();
					materialMatrix.matrix[3][1]=i*externalIntensity;
			
					glUniformMatrix4fv(link, (GLsizei)1, GL_FALSE, &(materialMatrix.matrix[0][0]));
}


/*-------------------------------------------------------------------
//	GRPMATERIALATTRIBUTES::UseMaterialData
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/04/2016 12:04:15
//	
//  @param				externalIntensity : 
//  @param				link : 
*/
/*-----------------------------------------------------------------*/
void GRPMATERIALATTRIBUTES::UseMaterialData(float externalIntensity, GRPLIGHT* light,GLINT link)
{
					float i=GetIntensity();			
					
					if (isupdated)
					UpdateMaterialMatrix();
								
					materialMatrix.matrix[3][1]=i*externalIntensity;
					materialMatrix.matrix[1][0]*=light->GetSpecular().red;
					materialMatrix.matrix[1][1]*=light->GetSpecular().green;
					materialMatrix.matrix[1][2]*=light->GetSpecular().blue;
			
					glUniformMatrix4fv(link, (GLsizei)1, GL_FALSE, &(materialMatrix.matrix[0][0]));
}
/*-------------------------------------------------------------------
//	GRPMATERIALATTRIBUTES::UpdateMaterialMatrix
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/04/2016 12:04:19
//	
*/
/*-----------------------------------------------------------------*/
void GRPMATERIALATTRIBUTES::UpdateMaterialMatrix()
{

					float normalizationSpecularPhong=1.0f;
							  normalizationSpecularPhong=((this->GetShininess()+2.0f)/(2.0f*PI))*specularFactor;
				
							
					materialMatrix.matrix[0][0]=(diffuse.red		>0 ? diffuse.red		: 0.0f);
					materialMatrix.matrix[0][1]=(diffuse.green	>0 ? diffuse.green	: 0.0f);
					materialMatrix.matrix[0][2]=(diffuse.blue   >0 ? diffuse.blue	  : 0.0f);
					materialMatrix.matrix[0][3]=(diffuse.alpha	>0 ? diffuse.alpha  : 0.0f);

					materialMatrix.matrix[1][0]=specular.red*normalizationSpecularPhong;
					materialMatrix.matrix[1][1]=specular.green*normalizationSpecularPhong;
					materialMatrix.matrix[1][2]=specular.blue*normalizationSpecularPhong;
					materialMatrix.matrix[1][3]=1.0f;

					materialMatrix.matrix[2][0]=emission.red;
					materialMatrix.matrix[2][1]=emission.green;
					materialMatrix.matrix[2][2]=emission.blue;
					materialMatrix.matrix[2][3]=1.0f;

					materialMatrix.matrix[3][0]=shininess;
					materialMatrix.matrix[3][1]=intensity;
					materialMatrix.matrix[3][2]=normalizationSpecularPhong;
					materialMatrix.matrix[3][3]=bumpfactor;

					isupdated=false;
}

/*-------------------------------------------------------------------
//	GRPMATERIAL::GRPMATERIAL
*/	
/**	
//	
//	Class Constructor GRPMATERIAL
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/05/2014 17:09:48
//	
*/
/*-----------------------------------------------------------------*/
GRPMATERIAL::GRPMATERIAL()
{
	Clean();

	attributes.SetGlossiness(1.0f);
	attributes.SetShininess(1.0f);

	attributes.GetSpecular()->Set	(0.0f, 0.0f, 0.0f, 1.0f);
	attributes.GetAmbient	()->Set	(0.0f, 0.0f, 0.0f, 1.0f);	
	attributes.GetEmission()->Set	(0.0f, 0.0f, 0.0f, 0.0f);	

	GRPNAMED::classtype = GRPNAMED_TYPE_GRPMATERIAL;
}
	



/*-------------------------------------------------------------------
//	GRPMATERIAL::~GRPMATERIAL
*/	
/**	
//	
//	 Class Destructor GRPMATERIAL
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/05/2014 17:09:51
//	
*/
/*-----------------------------------------------------------------*/
GRPMATERIAL::~GRPMATERIAL()
{	
	layers.DeleteAll();
	Clean();
}




/*-------------------------------------------------------------------
//	GRPMATERIAL::Select
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/05/2014 16:41:56
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPMATERIAL::Select() 
{									
		GRPOPENGLCTRL::TEXTURE::CurrentTexture=this;				
		XDWORD nl=layers.GetSize();

		for (XDWORD e=0;e<nl;e++)
		{
			GRPTEXTURELAYERTYPE		type		=(GRPTEXTURELAYERTYPE)layers.GetKey(e);
			GRPTEXTURE*						layer		=layers.GetElement(e);						
														layer->Bind(type);			
		}

	return true;
}


/*-------------------------------------------------------------------
//	GRPMATERIAL::SetValue
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/02/2015 16:53:13
//	
//  @param				type : 
//  @param				subtype : 
//  @param				value : 
*/
/*-----------------------------------------------------------------*/
bool GRPMATERIAL::SetValue(GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS subtype, double fvalue,double base)
{

	double value=fvalue;			
	switch(type)
	{
		//---------------------------------------------------- Diffuse
	case GRPANIMATIONCURVETYPE_DIFFUSECOLOR:						switch(subtype)
			{
			case GRPANIMATIONCURVEAXIS_X:
			case GRPANIMATIONCURVECOLOR_R:this->attributes.GetDiffuse()->SetRed		(float(value));	break;
			case GRPANIMATIONCURVEAXIS_Y:
			case GRPANIMATIONCURVECOLOR_G:this->attributes.GetDiffuse()->SetGreen	(float(value));	break;
			case GRPANIMATIONCURVEAXIS_Z:
			case GRPANIMATIONCURVECOLOR_B:this->attributes.GetDiffuse()->SetBlue	(float(value));	break;			
			case GRPANIMATIONCURVECOLOR_A:this->attributes.GetDiffuse()->SetAlpha	(float(value));	break;
			default: break;
			}
			break;
	//---------------------------------------------------- Specular
	case GRPANIMATIONCURVETYPE_SPECULARCOLOR:				
			switch(subtype)
			{
			case GRPANIMATIONCURVEAXIS_X:
			case GRPANIMATIONCURVECOLOR_R:this->attributes.GetSpecular()->SetRed		(float(value));	break;
			case GRPANIMATIONCURVEAXIS_Y:
			case GRPANIMATIONCURVECOLOR_G:this->attributes.GetSpecular()->SetGreen	(float(value));	break;
			case GRPANIMATIONCURVEAXIS_Z:
			case GRPANIMATIONCURVECOLOR_B:this->attributes.GetSpecular()->SetBlue		(float(value));	break;
			case GRPANIMATIONCURVECOLOR_A:this->attributes.GetSpecular()->SetAlpha	(float(value));	break;
			default: break;
			}
			break;

	//---------------------------------------------------- Ambient		
	case GRPANIMATIONCURVETYPE_AMBIENTCOLOR:				
			switch(subtype)
			{
			case GRPANIMATIONCURVEAXIS_X:
			case GRPANIMATIONCURVECOLOR_R:this->attributes.GetAmbient()->SetRed			(float(value));	break;
			case GRPANIMATIONCURVEAXIS_Y:
			case GRPANIMATIONCURVECOLOR_G:this->attributes.GetAmbient()->SetGreen		(float(value));	break;
			case GRPANIMATIONCURVEAXIS_Z:
			case GRPANIMATIONCURVECOLOR_B:this->attributes.GetAmbient()->SetBlue		(float(value));	break;
			case GRPANIMATIONCURVECOLOR_A:this->attributes.GetAmbient()->SetAlpha		(float(value));	break;
			default: break;
			}
			break;

				//---------------------------------------------- Emmissive		
	case GRPANIMATIONCURVETYPE_EMISSIVECOLOR:				
			switch(subtype)
			{
			case GRPANIMATIONCURVEAXIS_X:
			case GRPANIMATIONCURVECOLOR_R:this->attributes.GetEmission()->SetRed			(float(value));	break;
			case GRPANIMATIONCURVEAXIS_Y:
			case GRPANIMATIONCURVECOLOR_G:this->attributes.GetEmission()->SetGreen		(float(value));	break;
			case GRPANIMATIONCURVEAXIS_Z:
			case GRPANIMATIONCURVECOLOR_B:this->attributes.GetEmission()->SetBlue			(float(value));	break;
			case GRPANIMATIONCURVECOLOR_A:this->attributes.GetEmission()->SetAlpha		(float(value));	break;
			default: break;
			}
			break;
	
	//----------------------------------------------------Diffuse Factor
	case GRPANIMATIONCURVETYPE_DIFFUSEFACTOR:				
			this->attributes.SetIntensity(float(value));		
			break;

	//--------------------------------------------- Specular Factor			
	case GRPANIMATIONCURVETYPE_SPECULARFACTOR:					
			this->attributes.SetSpecularFactor((float)value);
			break;
	
					//---- ----//
	case GRPANIMATIONCURVETYPE_TRANSPARENCYFACTOR:			//--------------------------------------------  Transparency		
			this->attributes.SetTransparency((float)value);
			break;
	case GRPANIMATIONCURVETYPE_SHININESSFACTOR:					//--------------------------------------------- Shininess Factor	
			this->attributes.SetShininessFactor((float)value);
			break;
	case GRPANIMATIONCURVETYPE_SHININESSEXPONENT:				//--------------------------------------------- Shininess Exponent				
			this->attributes.SetShininess((float)value);
			break;
	default: 
		return false;
	}

	this->attributes.isupdated=true;
	return true;
}