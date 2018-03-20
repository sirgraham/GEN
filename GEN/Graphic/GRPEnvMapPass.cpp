
/*------------------------------------------------------------------------------------------
//	GRPENVIRONMENTMAPPASS.CPP
//	
//	Manage Environment Texture
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 03/07/2017 12:29:28
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPEnvMapPass.h"
#include "GRPMaterialLibrary.h"
#include "GRPRenderTarget.h"
#include "GRPCamera.h"
#include "GRPScene.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



const char GRPSHADERLIBRARY::ReflectiveVertex	[]=
	"uniform mat4 lightDataMatrix[4];\n"
	"uniform mat4 lightColorMatrix[4];\n"
	"uniform mat4 materialDataMat;\n"
	"attribute vec3 a_position;\n"
	"attribute vec3 a_normal;\n"	
	"attribute vec2 a_texCoord;\n"	
	"uniform sampler2D s_texture;												\n"	
	"uniform samplerCube textureChannel3;								\n"	

	"varying vec4 v_position;\n"
	"varying vec2 v_texCoord;\n"
	"varying vec3 v_normal;\n"

	"uniform mat4 modelMat;\n"
	"uniform mat4 invViewMat;\n"
	"uniform mat4 modelviewMat;\n"
	"uniform mat4 invmodelviewMat;\n"
	"uniform mat4 modelviewprojectionMat;\n"
					
		"varying vec4 specularColor,lambertianColor,diffuseColor; \n"
	

	"void main() {\n"

	"		vec3 normalInterp;			\n"
	"		vec3 vertPos;						\n"
	"		vec3	L;								\n"
	"   float fspecular=0.0;		\n"
	"   float flambertian=0.0;	\n"
	"   float fattenuattion=1.0;\n"

	"		float Tlambertian=0.0;\n"
	"		float Tspecular=0.0;\n"

	"    gl_Position			= modelviewprojectionMat * vec4(a_position, 1.0);         \n"
	"    v_position				= modelviewMat * vec4(a_position, 1.0);\n"
	"    v_normal					= modelviewMat * vec4(a_normal, 0.0);\n"

	//----------------- light0

	"			L									=	(v_position.xyz-lightDataMatrix[0][0].xyz).xyz;																															\n" //precalculated lightposition*viewMat
	"			float distance		= length (L);																																																	\n"
	"			fattenuattion			= (materialDataMat[3][1]*(lightDataMatrix[0][1][3])) / ((((lightDataMatrix[0][1][2]*distance)+lightDataMatrix[0][1][1])*distance)+lightDataMatrix[0][1][0]);\n"
		
	"			normalInterp			= (modelviewMat * vec4(a_normal, 0.0)).xyz;																																		\n"
	"			vec3 normal				= normalize(normalInterp);																																										\n"

	"			vec3 lightDir			= normalize(-L);																																															\n"
	"			lambertianColor		= max(dot(lightDir,normal), 0.0)*lightColorMatrix[0][1]*fattenuattion;																				\n"

	"			vec4 Emissive			= materialDataMat[2];																																													\n"
	"     vec3 reflectDir		= reflect(-lightDir, normal);																																									\n"
	"     float specAngle		= max(dot(reflectDir, normalize(-v_position.xyz)), 0.0);																											\n"
	
	"				specularColor		= (materialDataMat[1]*lightColorMatrix[0][2]*fattenuattion*pow(specAngle, materialDataMat[3][0]))+Emissive;		\n"
	"				diffuseColor		= (lambertianColor*materialDataMat[0]) + specularColor;																												\n"	
	"}";

const char GRPSHADERLIBRARY::ReflectiveFragmentTexture[]=
"uniform sampler2D s_texture;									\n"	
"uniform samplerCube textureChannel10;				\n"		
"varying vec2 v_texCoord;											\n"
"varying vec3 v_normal;												\n"
"varying vec4 v_position;											\n"
"uniform mat4 invViewMat;											\n"
	"uniform mat4 materialDataMat;							\n"
	
	"varying vec4 specularColor,lambertianColor; \n"
	"varying vec4 diffuseColor; \n"
	
"void main() {\n"
/*
"	 vec3 r=reflect((v_position.xyz), normalize(v_normal.xyz)).xyz;\n"
"				r=(invViewMat * vec4(r, 0.0)).xyz;\n"
"  vec4 material_reflection	=mix(texture2D(s_texture,v_texCoord),textureCube(textureChannel10,r.xyz),0.5);\n"
"  gl_FragColor		=material_reflection;	\n"
*/
"vec3 incident_eye = normalize(-v_position.xyz); \n"
"vec3 normal			 = normalize(-v_normal.xyz); \n"

"vec3 reflected = reflect(incident_eye,normal); \n"

"			reflected = vec3((invViewMat*vec4(reflected,0.0))); \n"

"  vec4 material_reflection	=mix(texture2D(s_texture,v_texCoord),textureCube(textureChannel10,reflected),0.5);\n"
"  gl_FragColor		=material_reflection;	\n"


"}\n";

const char GRPSHADERLIBRARY::ReflectiveFragmentDiffuse[]=
"uniform samplerCube textureChannel10;					\n"		
"varying vec3 v_normal;												\n"
"varying vec4 v_position;											\n"
"uniform mat4 invViewMat;											\n"
"uniform mat4 materialDataMat;								\n"

"varying vec4 specularColor,lambertianColor,diffuseColor; \n"

"void main() {\n"

"vec3 incident_eye = normalize(-v_position.xyz); \n"
"vec3 normal			 = normalize(-v_normal.xyz); \n"

"vec3 reflected = reflect(incident_eye,normal); \n"

"			reflected = vec3((invViewMat*vec4(reflected,0.0))); \n"

#ifdef GRP_RENDER_MODE_LAMBERT
" vec4 surface		= lambertianColor*materialDataMat[0] + specularColor;\n"
#else
"  vec4 surface		=	diffuseColor;	\n"
#endif

"  vec4 material_reflection	= mix(diffuseColor,textureCube(textureChannel10,reflected.xyz),0.5);\n"
"  gl_FragColor		=material_reflection;	\n"

"}\n";


/*-------------------------------------------------------------------
//	GRPENVIRONMENTMAPPASS::GRPENVIRONMENTMAPPASS
*/	
/**	
//	
//	Class Constructor GRPENVIRONMENTMAPPASS
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/07/2017 20:08:27
//	
//  @param				name : 
//  @param				GRPPASS(name) : 
//  @param				node(node : 
*/
/*-----------------------------------------------------------------*/
GRPENVIRONMENTMAPPASS::GRPENVIRONMENTMAPPASS	() : GRPPASS(__L("EnvironmentMapping"))	
{
	Clean();	
	type												= GRPPASS_TYPE_CULLFACE | GRPPASS_TYPE_COLORWRITE | GRPPASS_TYPE_DEPTHWRITE | GRPPASS_TYPE_DEPTHTEST;	
	BackFaceCullingIsActive			=	true;		
	DepthWritingIsActive				=	true;		
	DepthTestIsActive						=	true;
	ColorWritingIsActive				= true;
	
	lightingIsEnabled						=	true;
	this->priority							= 1;
	this->hasToClear						=	GRPPASS_CLEAR_FLAG_ALL;
	this->iterations						=	6;
	this->SetNode(node);

	this->OrderClone=false;

	this->material=GRPMATERIALLIBRARY::Get()->Create(this->name.Get());

	passtype=GRPPASS_ENVMAP;
}

/*-------------------------------------------------------------------
//	GRPENVIRONMENTMAPPASS::OnAdded
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/07/2017 20:08:22
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	GRPENVIRONMENTMAPPASS::OnAdded()
{
	
		OPENGLCHECKERROR(__L("GRPENVIRONMENTMAPPASS::OnAdded"));
	this->CreateViewport									();
	this->viewport->SetName								(__L("EnvView"));
	this->viewport->SetWindowSize					(512,512);
	this->viewport->SetPosition						(0.0,0.0);	
	this->viewport->SetSize								(1.0,1.0);
	this->viewport->SetProjection					(GRPPROJECTIONTYPE_FRUSTUM);
	this->viewport->SetProjectionFrustum	(0.01f,100000.0f);	
	this->viewport->SetFieldOfView				(90.0f);
	this->viewport->Select								();
	
	this->CreateCamera();

	if (!this->node)
	{
			this->node = this->camera;
	}
	else
			camera->LinkTo(node);


		const GRPVECTOR* v=this->node->GetPosition();
		//this->camera->SetPosition(v->vector[0],v->vector[1], v->vector[2]);
		this->camera->SetPosition(0.0,0.0,0.0);
		this->camera->fov=90.0f;
		this->camera->SetViewport(this->viewport);
		this->camera->CreateRenderBuffer(512,512);	

		this->texture=this->camera->rendertexture;
		this->material->AddLayer(GRPTEXTURELAYERTYPE_REFLEXION,texture);


	return true;
}

/*-------------------------------------------------------------------
//	GRPENVIRONMENTMAPPASS::Begin
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/07/2017 20:23:33
//	
*/
/*-----------------------------------------------------------------*/
void GRPENVIRONMENTMAPPASS::Begin()
{
	this->camera->rendertarget->SelectAsTarget(currentiteration);	
	GRPPASS::Begin();
}

/*-------------------------------------------------------------------
//	GRPENVIRONMENTMAPPASS::End
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/07/2017 20:34:33
//	
*/
/*-----------------------------------------------------------------*/
void GRPENVIRONMENTMAPPASS::End()
{
	GRPPASS::End();
	this->camera->rendertarget->Deselect();
}

/*-------------------------------------------------------------------
//	GRPENVIRONMENTMAPPASS::Start
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			03/07/2017 20:23:39
//	
*/
/*-----------------------------------------------------------------*/
void GRPENVIRONMENTMAPPASS::Start() 
{		
	//	GRPVECTOR pos=this->node->GetWorldPosition();
	//	this->camera->SetPosition(pos.vector[0],pos.vector[1],pos.vector[2]);		
	  this->viewport->Select();
		//this->camera->UsesGeometricRotation=false;

		switch(currentiteration)
		{
		case GRPTEXTURE_CUBEMAP_RIGHT:  this->camera->SetRotation(0.0f,			-90.0f,	 180.0f);		break;
		case GRPTEXTURE_CUBEMAP_LEFT:		this->camera->SetRotation(0.0f,			+90.0f,	 180.0f); break;

		case GRPTEXTURE_CUBEMAP_BOTTOM: this->camera->SetRotation(+90.0f,		+0.0f, 0.0f); break;
		case GRPTEXTURE_CUBEMAP_TOP:		this->camera->SetRotation(-90.0f,		+0.0f, 0.0f); break;

		case GRPTEXTURE_CUBEMAP_FRONT:	this->camera->SetRotation(0.0f,			0.0f,		180.0f); break;
		case GRPTEXTURE_CUBEMAP_BACK:		this->camera->SetRotation(0.0f,			180.0f, 180.0f); break;
		}
		/*
		switch(currentiteration)
		{
		case GRPTEXTURE_CUBEMAP_RIGHT:  this->camera->SetRotation(0.0f,			+0.0,	 0.0f);		break;
		case GRPTEXTURE_CUBEMAP_LEFT:		this->camera->SetRotation(0.0f,			+0.0,	 0.0f); break;

		case GRPTEXTURE_CUBEMAP_BOTTOM: this->camera->SetRotation(+0.0,		+0.0, 0.0f); break;
		case GRPTEXTURE_CUBEMAP_TOP:		this->camera->SetRotation(+0.0,		+0.0, 0.0f); break;

		case GRPTEXTURE_CUBEMAP_FRONT:	this->camera->SetRotation(0.0f,			+0.0,	0.0f); break;
		case GRPTEXTURE_CUBEMAP_BACK:		this->camera->SetRotation(0.0f,			+0.0, 0.0f); break;
		}
		*/

		
		this->camera->Update();		
		this->camera->BuildLookAt();

		
};