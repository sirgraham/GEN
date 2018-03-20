
/*------------------------------------------------------------------------------------------
//	GRPSHADERDOT.CPP
//	
//	Shader Dot3
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 09/03/2015 11:40:10
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPShaderLibrary.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

const char GRPSHADERLIBRARY::VertexSimple[]=
	"attribute	vec3 a_position;\n"
	"varying		vec4 v_position;\n"	
	"uniform		mat4 modelviewprojectionMat;\n"

	"void main(){\n"
	"		 v_position					= modelviewprojectionMat * vec4(a_position, 1.0);					\n"
	"    gl_Position				= v_position;																						  \n"
	"}";

const char GRPSHADERLIBRARY::Vertex[]=
	"attribute vec3 a_position;\n"
	"attribute vec3 a_normal;\n"	
	"attribute vec2 a_texCoord;\n"	

	"varying vec4 v_position;\n"
	"varying vec2 v_texCoord;\n"
	"varying vec3 v_normal;\n"

	"uniform mat4 modelviewMat;\n"
	"uniform mat4 modelviewprojectionMat;\n"

	"void main(){\n"
	"		 v_position					= modelviewprojectionMat * vec4(a_position, 1.0);					\n"
	"		 v_normal						= (modelviewMat * vec4(a_normal, 0.0)).xyz;								\n"
	"    v_texCoord					= a_texCoord;																							\n"
	"    gl_Position				= v_position;																						  \n"
	"}";

const char GRPSHADERLIBRARY::VertexTextureOperation[] =
"attribute vec3 a_position;\n"
"attribute vec3 a_normal;\n"
"attribute vec2 a_texCoord;\n"

"varying vec4 v_position;\n"
"varying vec2 v_texCoord;\n"
"varying vec3 v_normal;\n"

"uniform mat4 textureMat;\n"
"uniform mat4 modelviewMat;\n"
"uniform mat4 modelviewprojectionMat;\n"

"void main(){\n"
"		 v_position					= modelviewprojectionMat * vec4(a_position, 1.0);					\n"
"		 v_normal						= (modelviewMat * vec4(a_normal, 0.0)).xyz;								\n"
"    v_texCoord					= (textureMat*vec4(a_texCoord,1.0,1.0)).xy;										\n"
"    gl_Position				= v_position;																						  \n"
"}";

const char GRPSHADERLIBRARY::VertexShaderDOT[]=
	"attribute vec3 a_position;\n"
	"attribute vec3 a_normal;\n"
	"attribute vec2 a_texCoord;\n"	

	"uniform mat4 modelviewMat;\n"	
	"uniform mat4 modelviewprojectionMat;\n"
	
	"uniform mat4 lightDataMatrix[4];\n"
	"uniform mat4 lightColorMatrix[4];\n"
	"uniform mat4 materialDataMat;\n"

	//------------- varying

	"varying vec4 v_position;\n"
	"varying vec3 v_normal;\n"
	"varying vec2 v_texCoord;\n"

	"varying vec4 specularColor,lambertianColor; \n"

	"uniform sampler2D s_texture;								\n"	
	"uniform sampler2D textureChannel1;				\n"

	"varying vec4 diffuseColor; \n"
	
	"void main(){\n"

	"		vec3 normalInterp;			\n"
	"		vec3 vertPos;						\n"
	"		vec3	L;								\n"
	"   float fspecular=0.0;		\n"
	"   float flambertian=0.0;	\n"
	"   float fattenuattion=1.0;\n"

	"		float Tlambertian=0.0;\n"
	"		float Tspecular=0.0;\n"

	"    gl_Position		= modelviewprojectionMat * vec4(a_position, 1.0);         \n"
	"    v_texCoord			= a_texCoord;\n"
	
	"    v_position	= modelviewMat * vec4(a_position, 1.0);\n"

	//----------------- light0

	"			L									=	(v_position.xyz-lightDataMatrix[0][0].xyz).xyz;																\n" //precalculated lightposition*viewMat
	"			float distance		= length (L);																																		\n"
	"			fattenuattion			= (materialDataMat[3][1]*(lightDataMatrix[0][1][3])) / ((((lightDataMatrix[0][1][2]*distance)+lightDataMatrix[0][1][1])*distance)+lightDataMatrix[0][1][0]);\n"
	
	//"			normalInterp			= mat3(modelviewMat) * vec4(a_normal, 0.0).xyz;																	\n"
	"			normalInterp			= (modelviewMat * vec4(a_normal, 0.0)).xyz;																						\n"
	"			vec3 normal				= normalize(normalInterp);																											\n"
	//"			v_normal	= normal;\n"
	"			vec3 lightDir			= normalize(-L);																																\n"
	"			lambertianColor		= max(dot(lightDir,normal), 0.0)*lightColorMatrix[0][1]*fattenuattion;					\n"

	"			vec4 Emissive			= materialDataMat[2];																													 \n"
	"     vec3 reflectDir		= reflect(-lightDir, normal);																										\n"
	"     float specAngle		= max(dot(reflectDir, normalize(-v_position.xyz)), 0.0);												\n"
	"				specularColor		= (materialDataMat[1]*lightColorMatrix[0][2]*fattenuattion*pow(specAngle, materialDataMat[3][0]))+Emissive;		\n"
#ifndef GRP_RENDER_MODE_LAMBERT
	"				diffuseColor	=  (lambertianColor*materialDataMat[0]) + specularColor; \n"
#endif
		//----------------- final

	"}";


const char GRPSHADERLIBRARY::VertexShaderDOTDiff[] =
"attribute vec3 a_position;\n"
"attribute vec3 a_normal;\n"

"uniform mat4 modelviewMat;\n"
"uniform mat4 modelviewprojectionMat;\n"

"uniform mat4 lightDataMatrix[4];\n"
"uniform mat4 lightColorMatrix[4];\n"
"uniform mat4 materialDataMat;\n"

//------------- varying

"varying vec4 v_position;\n"
"varying vec3 v_normal;\n"

	"varying vec4 specularColor;\n"
	"varying vec4 lambertianColor; \n"
	"varying vec4 diffuseColor; \n"
	

	"void main(){\n"

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

	//----------------- light0

	"			L									=	(v_position.xyz-lightDataMatrix[0][0].xyz).xyz;																															\n" //precalculated lightposition*viewMat
	"			float distance		= length (L);																																																	\n"
	"			fattenuattion			= (materialDataMat[3][1]*(lightDataMatrix[0][1][3])) / ((((lightDataMatrix[0][1][2]*distance)+lightDataMatrix[0][1][1])*distance)+lightDataMatrix[0][1][0]);\n"
		
	"			normalInterp			= (modelviewMat * vec4(a_normal, 0.0)).xyz;																																		\n"
	"			vec3 normal				= normalize(normalInterp);																																										\n"

	"			vec3 lightDir			= normalize(-L);																																															\n"
	"			lambertianColor		= max(dot(lightDir,normal), 0.0)*lightColorMatrix[0][1]*fattenuattion*materialDataMat[0];											\n"	

	"			vec4 Emissive			= materialDataMat[2];																																													\n"
	"     vec3 reflectDir		= reflect(-lightDir, normal);																																									\n"
	"     float specAngle		= max(dot(reflectDir, normalize(-v_position.xyz)), 0.0);																											\n"
	
	"				specularColor		= (materialDataMat[1]*lightColorMatrix[0][2]*fattenuattion*pow(specAngle, materialDataMat[3][0]))+Emissive;		\n"
#ifndef GRP_RENDER_MODE_LAMBERT
	"				diffuseColor		= lambertianColor + specularColor;																												\n"
#endif

		//----------------- final

	"}";

const char GRPSHADERLIBRARY::FragmentShaderDOTTexture[]=
"uniform sampler2D s_texture;	\n"	
"varying vec2 v_texCoord;\n"
"varying vec4 diffuseColor;\n"
#ifdef GRP_RENDER_MODE_LAMBERT
"varying vec4 lambertianColor; \n"
"varying vec4 specularColor;\n"
#endif

"void main() {\n"
#ifdef GRP_RENDER_MODE_LAMBERT
"  gl_FragColor		=	(lambertianColor*texture2D(s_texture,v_texCoord)) + specularColor;\n"
#else
"  gl_FragColor		= diffuseColor*texture2D(s_texture,v_texCoord);\n"
#endif

"}\n";

const char GRPSHADERLIBRARY::FragmentShaderDOTTextureSpecular[]=
"uniform sampler2D s_texture;	\n"	
"uniform sampler2D textureChannel1;	\n"	
"varying vec2 v_texCoord;\n"
"varying vec4 specularColor,lambertianColor; \n"

"void main() {\n"
"  gl_FragColor		=	(lambertianColor*texture2D(s_texture,v_texCoord)) + specularColor*texture2D(textureChannel1,v_texCoord);	\n"

"}\n";

const char GRPSHADERLIBRARY::FragmentShaderDOTTextureInterpolated[]=
"uniform sampler2D s_texture;								\n"	
"uniform sampler2D textureChannel1;				\n"	


"uniform float interpolator;\n"

"varying vec2 v_texCoord;\n"
"varying vec4 specularColor,lambertianColor; \n"

"void main() {\n"
"  gl_FragColor		=	(lambertianColor*mix(texture2D(s_texture,v_texCoord),texture2D(textureChannel1,v_texCoord),interpolator)) + specularColor;	\n"

"}\n";


const char GRPSHADERLIBRARY::FragmentShaderDOTDiffuse[] =
"varying vec4 diffuseColor;\n"
#ifdef GRP_RENDER_MODE_LAMBERT
"varying vec4 lambertianColor,specularColor; \n"
"uniform mat4 materialDataMat;\n"
#endif

"void main() {\n"
#ifdef GRP_RENDER_MODE_LAMBERT
" gl_FragColor		= lambertianColor + specularColor;\n"
#else
"  gl_FragColor		=	diffuseColor;	\n"
#endif

"}\n";

const char GRPSHADERLIBRARY::DiffuseShader[]=					
"uniform mat4 materialDataMat;	\n"
"void main()	{\n"
	"	 gl_FragColor	=		materialDataMat[0];	\n"
	"}\n";




const char GRPSHADERLIBRARY::TextureShader[]=
"uniform sampler2D s_texture;	\n"	
"varying vec2 v_texCoord;\n"

"void main() {\n"		
"		 gl_FragColor		=		texture2D(s_texture,v_texCoord); \n"
"}\n";	

//------------------------------------------ PERFRAGMENT DOT3

const char GRPSHADERLIBRARY::VertexShaderDOTPerFragment[]=
	"attribute vec3 a_position;\n"	
	"attribute vec3 a_normal;\n"
	"attribute vec3 a_binormal;\n"
	"attribute vec3 a_tangent;\n"
	"attribute vec2 a_texCoord;\n"	

	"uniform mat4 modelMat;\n"
	"uniform mat4 modelviewMat;\n"	
	"uniform mat4 modelviewprojectionMat;\n"
	
	"uniform mat4 materialDataMat;\n"
	"uniform mat4 lightDataMatrix[4];\n"

	"varying vec4 v_position;\n"
	"varying vec3 v_normal;\n"
#ifdef GRPVERTEX_EXTENDED
	"varying vec3 v_binormal;\n"
	"varying vec3 v_tangent;\n"
#endif
	"varying vec2 v_texCoord;\n"

	
	"uniform sampler2D s_texture;	\n"	

	"void main(){\n"
	"    gl_Position		= modelviewprojectionMat * vec4(a_position, 1.0);         \n"

	"    v_texCoord			= a_texCoord;\n"	
	"    v_normal				= mat3(modelviewMat) * vec4(a_normal, 0.0).xyz;\n"
#ifdef GRPVERTEX_EXTENDED
	"    v_binormal			= mat3(modelMat) * vec4(a_binormal, 0.0).xyz;\n"
	"    v_tangent			= mat3(modelMat) * vec4(a_tangent, 0.0).xyz;\n"
#endif
	"    v_position			= modelviewMat * vec4(a_position, 1.0);\n"
	
	"}";

	const char GRPSHADERLIBRARY::FragmentShaderDOTPerFragment[] =
			/*
				"uniform mat4 modelviewMat;\n"
				"uniform mat4 modelviewprojectionMat;\n"

				"uniform mat4 lightDataMatrix[4];\n"
				"uniform mat4 lightColorMatrix[4];\n"
				"uniform mat4 materialDataMat;\n"

				//------------- varying

				"varying vec4 v_position;\n"
				"varying vec3 v_normal;\n"
				"varying vec2 v_texCoord;\n"

				"uniform sampler2D s_texture;					\n"


				"void main(){\n"
				"		vec3	L;								\n"
				"   float fattenuattion=1.0;\n"
				"		vec4 specularColor,lambertianColor; \n"

				//----------------- light0

				"			L									=	(v_position.xyz-lightDataMatrix[0][0].xyz).xyz;																\n" //precalculated lightposition*viewMat
				"			float distance		= length (L);																																		\n"
				"			fattenuattion			= (materialDataMat[3][1]*(lightDataMatrix[0][1][3])) / ((((lightDataMatrix[0][1][2]*distance)+lightDataMatrix[0][1][1])*distance)+lightDataMatrix[0][1][0]);\n"

				"			vec3 normal				= normalize(v_normal);		\n"
				"			vec3 lightDir			= normalize(-L);																																\n"
				"			lambertianColor		= max(dot(lightDir,normal), 0.0)*lightColorMatrix[0][1]*fattenuattion*materialDataMat[0];											\n"

				"     vec3 reflectDir		= reflect(-lightDir, normal);																										\n"
				"     float specAngle		= max(dot(reflectDir, normalize(-v_position.xyz)), 0.0);												\n"
				"				specularColor		= (lightColorMatrix[0][2]*fattenuattion*pow(specAngle, materialDataMat[3][0]));		\n"



				"  gl_FragColor		=	lambertianColor*texture2D(s_texture,v_texCoord) + specularColor;	\n"
			"}\n";
			*/
			"uniform mat4 modelviewMat;\n"
			"uniform mat4 modelviewprojectionMat;\n"

			"uniform mat4 lightDataMatrix[4];\n"
			"uniform mat4 lightColorMatrix[4];\n"
			"uniform mat4 materialDataMat;\n"

			//------------- varying

			"varying vec4 v_position;\n"
			"varying vec3 v_normal;\n"
			"varying vec2 v_texCoord;\n"

			"vec4 specularColor,lambertianColor; \n"

			"uniform sampler2D s_texture;								\n"
			"uniform sampler2D textureChannel1;				\n"

			"varying vec4 diffuseColor; \n"

			"void main(){\n"

			"		vec3 normalInterp;			\n"
			"		vec3 vertPos;						\n"
			"		vec3	L;								\n"
			"   float fspecular=0.0;		\n"
			"   float flambertian=0.0;	\n"
			"   float fattenuattion=1.0;\n"

			"		float Tlambertian=0.0;\n"
			"		float Tspecular=0.0;\n"

			//----------------- light0

			"			L									=	(v_position.xyz-lightDataMatrix[0][0].xyz).xyz;																\n" //precalculated lightposition*viewMat
			"			float distance		= length (L);																																		\n"
			"			fattenuattion			= (materialDataMat[3][1]*(lightDataMatrix[0][1][3])) / ((((lightDataMatrix[0][1][2]*distance)+lightDataMatrix[0][1][1])*distance)+lightDataMatrix[0][1][0]);\n"

			//"			normalInterp			= mat3(modelviewMat) * vec4(a_normal, 0.0).xyz;																	\n"
			"			normalInterp			= v_normal.xyz;																						\n"
			"			vec3 normal				= normalize(normalInterp);																											\n"
			//"			v_normal	= normal;\n"
			"			vec3 lightDir			= normalize(-L);																																\n"
			"			lambertianColor		= max(dot(lightDir,normal), 0.0)*lightColorMatrix[0][1]*fattenuattion*materialDataMat[0];					\n"

			"			vec4 Emissive			= materialDataMat[2];																													 \n"
			"     vec3 reflectDir		= reflect(-lightDir, normal);																										\n"
			"     float specAngle		= max(dot(reflectDir, normalize(-v_position.xyz)), 0.0);												\n"
			"				specularColor		= (materialDataMat[1]*lightColorMatrix[0][2]*fattenuattion*pow(specAngle, materialDataMat[3][0]))+Emissive;		\n"
#ifndef GRP_RENDER_MODE_LAMBERT
			"				gl_FragColor	=  (lambertianColor*materialDataMat[0]) + specularColor; \n"
#endif
			"				gl_FragColor		= lambertianColor*texture2D(s_texture,v_texCoord) + specularColor;;\n"

//----------------- final

"}";



const char GRPSHADERLIBRARY::VertexShaderNormalMapping[]=
	"attribute vec3 a_position;\n"	
	"attribute vec3 a_normal;\n"
	"attribute vec3 a_binormal;\n"
	"attribute vec3 a_tangent;\n"
	"attribute vec2 a_texCoord;\n"	

	"uniform mat4 modelMat;\n"	
	"uniform mat4 modelviewMat;\n"	
	"uniform mat4 modelviewprojectionMat;\n"
	
	"uniform mat4 materialDataMat;\n"
	"uniform mat4 lightDataMatrix[4];\n"

	"varying vec4 v_position;\n"
	"varying vec3 v_normal;\n"
	"varying vec3 v_binormal;\n"
	"varying vec3 v_tangent;\n"
	"varying vec2 v_texCoord;\n"

	//"varying mat3 TBN;\n"
	
	"uniform sampler2D s_texture;	\n"	

	"void main(){\n"
	"    gl_Position		= modelviewprojectionMat * vec4(a_position, 1.0);         \n"

	"    v_texCoord			= a_texCoord;\n"	
	//"    v_normal				= mat3(modelviewMat) * vec4(a_normal, 0.0).xyz;\n"
	//"    v_binormal			= mat3(modelviewMat) * vec4(a_binormal, 0.0).xyz;\n"
	//"    v_tangent			= mat3(modelviewMat) * vec4(a_tangent, 0.0).xyz;\n"
	"    v_normal				= normalize(mat3(modelMat) * vec4(a_normal, 0.0).xyz);\n"
	"    v_binormal			= normalize(mat3(modelMat) * vec4(a_binormal, 0.0).xyz);\n"
	"    v_tangent			= normalize(mat3(modelMat) * vec4(a_tangent, 0.0).xyz);\n"
	
	"    v_position			= modelviewMat * vec4(a_position, 1.0);\n"
	
	"}";

const char GRPSHADERLIBRARY::FragmentShaderNormalMapping[] =

"uniform mat4 modelviewMat;\n"
"uniform mat4 modelviewprojectionMat;\n"

"uniform mat4 lightDataMatrix[4];\n"
"uniform mat4 lightColorMatrix[4];\n"
"uniform mat4 materialDataMat;\n"

//------------- varying

"varying vec4 v_position;\n"
"varying vec3 v_normal;\n"
"varying vec3 v_binormal;\n"
"varying vec3 v_tangent;\n"
"varying vec2 v_texCoord;\n"

"mat3 TBN;\n"

"uniform sampler2D textureChannel1;		\n" //diffuse channel	
"uniform sampler2D textureChannel2;		\n" //specular map
"uniform sampler2D textureChannel3;		\n" //normal mapping
"uniform sampler2D textureChannel4;		\n"
"uniform sampler2D textureChannel5;		\n"
"uniform sampler2D textureChannel6;	\n"



"void main(){\n"

"		vec3 normalInterp;			\n"
"		vec3 vertPos;						\n"
"		vec3	L;								\n"
"   float fspecular=0.0;		\n"
"   float flambertian=0.0;	\n"
"   float fattenuattion=1.0;\n"
"		vec4 specularColor,lambertianColor; \n"
"		float Tlambertian=0.0;\n"
"		float Tspecular=0.0;\n"

//----------------- light0
"		 TBN = mat3(v_tangent, v_binormal, v_normal);\n"

"			L									=	(v_position.xyz-lightDataMatrix[0][0].xyz).xyz;																\n" //precalculated lightposition*viewMat
"			float distance		= length (L);																																		\n"
"			fattenuattion			= (materialDataMat[3][1]*(lightDataMatrix[0][1][3])) / ((((lightDataMatrix[0][1][2]*distance)+lightDataMatrix[0][1][1])*distance)+lightDataMatrix[0][1][0]);\n"

"			vec3 normal				= texture2D(textureChannel3,v_texCoord).xyz;\n"
"			normal = normalize(normal * 2.0 - 1.0)*materialDataMat[3][3];    \n"
"			normal = normalize(TBN * normal); \n"

"			vec3 lightDir			= normalize(-L);																																\n"
"			lambertianColor		= max(dot(lightDir,normal), 0.0)*lightColorMatrix[0][1]*fattenuattion;					\n"

"			vec4 Emissive			= materialDataMat[2];																													\n"
"     vec3 reflectDir		= reflect(-lightDir, normal);																										\n"
"     float specAngle		= max(dot(reflectDir, normalize(-v_position.xyz)), 0.0);												\n"
"				specularColor		= (materialDataMat[1]*lightColorMatrix[0][2]*fattenuattion*pow(specAngle, materialDataMat[3][0]))+Emissive;		\n"
	
	"  gl_FragColor		=	lambertianColor*texture2D(textureChannel1,v_texCoord) + specularColor;	\n"

"}\n";


const char GRPSHADERLIBRARY::DiffuseFragmentShaderDOTPerFragment[]=

"varying vec4 v_position;\n"
"varying vec3 v_normal;\n"
"varying vec2 v_texCoord;\n"
								
"uniform mat4 lightDataMatrix[4];\n"
"uniform mat4 materialDataMat;\n"

"#define LIGHT lightDataMatrix[0]\n"

"#define LIGHT0 lightDataMatrix[0] \n"
"#define LIGHT1 lightDataMatrix[1] \n"
"#define LIGHT2 lightDataMatrix[2] \n"
"#define LIGHT3 lightDataMatrix[3] \n"

"void main() {\n"

	//----Light0
	"      vec4 lightPosition  = LIGHT0[0]; \n"
	"      vec4 decay					 = LIGHT0[1]; \n"

	"				vec3 L							=	(v_position-lightPosition).xyz;\n" //precalculated lightposition*viewMat
	"				float distance			= length (L);\n"
	"				float attenuattion	= (materialDataMat[3][1]*decay[3]) / (1.0+decay[0]+(decay[1]*distance)+decay[2]*(distance*distance));\n"
	

	"				vec3 normal					= normalize(v_normal);\n"
	"				vec3 lightDir				= normalize(-L);\n"

	"      vec3 reflectDir			= reflect(-lightDir, normal);\n"
	"      float specAngle			= max(dot(reflectDir, normalize(-v_position.xyz)), 0.0);\n"
	"      float specular				= pow(specAngle, materialDataMat[3][0]);\n"

"					lightDir						= normalize(-L-v_position.xyz );\n"
"					float lambertian		= max(dot(lightDir,normal), 0.0);\n"

"		vec3	surfaceColor	= vec3(materialDataMat[0][0],materialDataMat[0][1],materialDataMat[0][2]);	\n"
"   vec4  totalColor    = vec4(	(surfaceColor*lambertian)+specular	,	1.0); \n"
"					gl_FragColor	=	totalColor;							\n"
"}\n";

