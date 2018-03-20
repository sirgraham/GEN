
/*------------------------------------------------------------------------------------------
//	GRPDEFORMER.CPP
//	
//	Morpher class
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 14/07/2015 12:54:53
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPDeformer.h"
#include "GRPElement.h"
#include "GRPShaderLibrary.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


const char GRPSHADERLIBRARY::Morph	[]=
        "attribute vec3 a_position;\n"
        "attribute vec3 a_normal;\n"
        "attribute vec2 a_texCoord;\n"

				"attribute vec3 a_positionMorph1;\n"				
				"attribute vec3 a_positionMorph2;\n"
				"attribute vec3 a_positionMorph3;\n"				
				"attribute vec3 a_positionMorph4;\n"

#ifndef HW_RASPBERRYPI				
				"attribute vec3 a_positionMorph5;\n"				
				"attribute vec3 a_positionMorph6;\n"				
				"attribute vec3 a_positionMorph7;\n"
				"attribute vec3 a_positionMorph8;\n"
				"attribute vec3 a_positionMorph9;\n"
				"attribute vec3 a_positionMorph10;\n"
				"attribute vec3 a_positionMorph11;\n"
				"attribute vec3 a_positionMorph12;\n"
				
				//"attribute vec3 a_normalMorph1;\n"
				//"attribute vec3 a_normalMorph2;\n"
				//"attribute vec3 a_normalMorph3;\n"
				//"attribute vec3 a_normalMorph4;\n"
				//"attribute vec3 a_normalMorph5;\n"
				//"attribute vec3 a_normalMorph6;\n"
				//"attribute vec3 a_normalMorph7;\n"
				//"attribute vec3 a_normalMorph8;\n"								
				//"attribute vec3 a_normalMorph9;\n"				
				//"attribute vec3 a_normalMorph10;\n"				
				//"attribute vec3 a_normalMorph11;\n"				
				//"attribute vec3 a_normalMorph12;\n"

#endif

				"uniform vec4 channelsMorph; \n"

#ifndef HW_RASPBERRYPI
				"uniform vec4 channelsMorph2; \n"
				"uniform vec4 channelsMorph3; \n"
#endif

        "uniform mat4 modelviewMat;\n"
        "uniform mat4 modelviewprojectionMat;\n"

				"uniform mat4 lightDataMatrix	[4];		\n"
				"uniform mat4 lightColorMatrix[4];		\n"
				"uniform mat4 materialDataMat;				\n"

				"uniform sampler2D s_texture;        \n"
				
        //------------- varying

        "varying vec4 v_position;\n"
        "varying vec2 v_texCoord;\n"				
        "varying vec4 diffuseColor; \n"

        //"uniform sampler2D textureChannel1;	\n"	

        "vec3        L; \n"


        "void main(){\n"
					"   vec4 lambertianColor;\n"
					"		vec4 specularColor;\n"
					"		vec3 normalInterp;\n"

					"		vec3	L;								\n"
					"   float fspecular=0.0;		\n"
					"   float flambertian=0.0;	\n"
					"   float fattenuattion=1.0;\n"

        //----------------- morphing
				/*
							vec3 chan3				=	(a_positionMorph9*channelsMorph3.x)+(a_positionMorph10*channelsMorph3.y)+(a_positionMorph11*channelsMorph3.z)+(a_positionMorph12*channelsMorph3.a);\n"
				*/
				"			vec3	position    = a_position;\n"
				"						position	 +=	(a_positionMorph1*channelsMorph.x) +(a_positionMorph2*channelsMorph.y)  +(a_positionMorph3*channelsMorph.z)  +(a_positionMorph4*channelsMorph.a);\n"
#if HW_PC
				"						position	 +=	(a_positionMorph5*channelsMorph2.x)+(a_positionMorph6*channelsMorph2.y) +(a_positionMorph7*channelsMorph2.z) +(a_positionMorph8*channelsMorph2.a);\n"				
				"						position	 +=	(a_positionMorph9*channelsMorph3.x)+(a_positionMorph10*channelsMorph3.y) +(a_positionMorph11*channelsMorph3.z) +(a_positionMorph12*channelsMorph3.a);\n"				
#endif
			//	"						position	 +=	(a_positionMorph5*channelsMorph2.x);\n"				
			
				"					gl_Position		=		modelviewprojectionMat  * vec4(position,1.0);         \n"
        "					v_texCoord		=		a_texCoord;\n"
        //"					v_position		=		modelviewMat * position;\n"
				"					v_position		=		modelviewMat * vec4(position,1.0);\n"

        //----------------- light0 
	"			L									=	(v_position.xyz-lightDataMatrix[0][0].xyz).xyz;																\n" //precalculated lightposition*viewMat
	"			float distance		= length (L);																																		\n"
	"			fattenuattion			= (materialDataMat[3][1]*(lightDataMatrix[0][1][3])) / ((((lightDataMatrix[0][1][2]*distance)+lightDataMatrix[0][1][1])*distance)+lightDataMatrix[0][1][0]);\n"

	"			normalInterp			= (modelviewMat * vec4(a_normal, 0.0)).xyz;																						\n"
	"			vec3 normal				= normalize(normalInterp);																											\n"

	"			vec3 lightDir			= normalize(-L);																																\n"
	"			lambertianColor		= max(dot(lightDir,normal), 0.0)*lightColorMatrix[0][1]*fattenuattion;					\n"

	
	"			vec4 Emissive			= materialDataMat[2];																												  	\n"
	"     vec3 reflectDir		= reflect(-lightDir, normal);																										\n"
	"     float specAngle		= max(dot(reflectDir, normalize(-v_position.xyz)), 0.0);												\n"
	"				specularColor		= (materialDataMat[1]*lightColorMatrix[0][2]*fattenuattion*pow(specAngle, materialDataMat[3][0]))+Emissive;		\n"
	"				diffuseColor	  = (lambertianColor*materialDataMat[0]) + specularColor; \n"
        "}";


/*-------------------------------------------------------------------
//	GRPSUBDEFORMER::GRPSUBDEFORMER
*/	
/**	
//	
//	Class Constructor GRPSUBDEFORMER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/07/2015 12:58:47
//	
*/
/*-----------------------------------------------------------------*/
GRPSUBDEFORMER::GRPSUBDEFORMER()
{
	Clean();
	GRPNAMED::classtype = GRPNAMED_TYPE_GRPSUBDEFORMER;
}

/*-------------------------------------------------------------------
//	GRPSUBDEFORMER::~GRPSUBDEFORMER
*/	
/**	
//	
//	 Class Destructor GRPSUBDEFORMER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/07/2015 12:58:58
//	
*/
/*-----------------------------------------------------------------*/
GRPSUBDEFORMER::~GRPSUBDEFORMER()
{
	if (this->shape!=NULL)
		delete(this->shape);
}

/*-------------------------------------------------------------------
//	GRPSUBDEFORMER::SetValue
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/07/2015 12:59:06
//	
//	@return 			bool : 
//
//  @param				type : 
//  @param				subtype : 
//  @param				value : 
//  @param				base : 
*/
/*-----------------------------------------------------------------*/
bool GRPSUBDEFORMER::SetValue(GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS subtype, double value,double base)	
{
	switch(subtype)
	{
	case GRPANIMATIONCURVEAXIS_DEFORMPERCENT:	this->Factor=(float)value; 	return true;
	default: break;
	}

	return false;
}

/*-------------------------------------------------------------------
//	GRPSUBDEFORMER::Init
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/07/2015 13:21:40
//	
//	@return 			bool : 
//
//  @param				element : 
//  @param				channel : 
*/
/*-----------------------------------------------------------------*/
bool GRPSUBDEFORMER::Init			(GRPELEMENT* element,					XDWORD channel)
{	
#ifdef GRPDEFORMERS_ACTIVE

	if (shape==NULL) 
		return false;
	
	if (element==NULL)
		return false;

	if (element->GetMesh()==NULL)
		return false;

	XDWORD nv					=element->GetMesh()->GetNindexData();
	XWORD* indices		=element->GetMesh()->GetIndexData();

	XVECTOR<XDWORD> faceindexes;

	 for (XDWORD e=0;e<element->GetMesh()->GetFaces()->GetSize();e++)
        {
					GRPFACE*	face=element->GetMesh()->GetFaces()->Get(e);

				            faceindexes.Add(	face->a	);
					          faceindexes.Add(	face->b	);
						        faceindexes.Add(	face->c	);
        }

        for (XDWORD e=0;e<faceindexes.GetSize();e++)
        {
					long long i=faceindexes.Get(e);
					if (this->shape->indices.Find(i)!=NOTFOUND)
					{
						GRPVERTEX* vertex=&(element->GetMesh()->GetVertexData()[e]);
						GRPVERTEX* morphvertex=this->shape->vertices.Get(this->shape->indices.Find(i));	

								switch(channel)
								{
								case 0:	
												vertex->positionMorph1[0]=morphvertex->position[0];
												vertex->positionMorph1[1]=morphvertex->position[1];
												vertex->positionMorph1[2]=morphvertex->position[2];
											//	vertex->normalMorph1[0]=morphvertex->normal[0];
											//	vertex->normalMorph1[1]=morphvertex->normal[1];
											//	vertex->normalMorph1[2]=morphvertex->normal[2];
												break;
								case 1:	
												vertex->positionMorph2[0]=morphvertex->position[0];
												vertex->positionMorph2[1]=morphvertex->position[1];
												vertex->positionMorph2[2]=morphvertex->position[2];
											//	vertex->normalMorph2[0]=morphvertex->normal[0];
											//	vertex->normalMorph2[1]=morphvertex->normal[1];
											//	vertex->normalMorph2[2]=morphvertex->normal[2];
												break;
								case 2:	
												vertex->positionMorph3[0]=morphvertex->position[0];
												vertex->positionMorph3[1]=morphvertex->position[1];
												vertex->positionMorph3[2]=morphvertex->position[2];
											//	vertex->normalMorph3[0]=morphvertex->normal[0];
											//	vertex->normalMorph3[1]=morphvertex->normal[1];
											//	vertex->normalMorph3[2]=morphvertex->normal[2];
												break;
								case 3:	
												vertex->positionMorph4[0]=morphvertex->position[0];
												vertex->positionMorph4[1]=morphvertex->position[1];
												vertex->positionMorph4[2]=morphvertex->position[2];
											//	vertex->normalMorph4[0]=morphvertex->normal[0];
											//	vertex->normalMorph4[1]=morphvertex->normal[1];
											//	vertex->normalMorph4[2]=morphvertex->normal[2];
												break;


#ifndef LIMIT_MORPH_CHANNELS
								case 4:	
												vertex->positionMorph5[0]=morphvertex->position[0];
												vertex->positionMorph5[1]=morphvertex->position[1];
												vertex->positionMorph5[2]=morphvertex->position[2];
											//	vertex->normalMorph5[0]=morphvertex->normal[0];
											//	vertex->normalMorph5[1]=morphvertex->normal[1];
											//	vertex->normalMorph5[2]=morphvertex->normal[2];
												break;
								case 5:	
												vertex->positionMorph6[0]=morphvertex->position[0];
												vertex->positionMorph6[1]=morphvertex->position[1];
												vertex->positionMorph6[2]=morphvertex->position[2];
										//		vertex->normalMorph6[0]=morphvertex->normal[0];
										//		vertex->normalMorph6[1]=morphvertex->normal[1];
										//		vertex->normalMorph6[2]=morphvertex->normal[2];
												break;
								case 6:	
												vertex->positionMorph7[0]=morphvertex->position[0];
												vertex->positionMorph7[1]=morphvertex->position[1];
												vertex->positionMorph7[2]=morphvertex->position[2];
										//		vertex->normalMorph7[0]=morphvertex->normal[0];
										//		vertex->normalMorph7[1]=morphvertex->normal[1];
										//		vertex->normalMorph7[2]=morphvertex->normal[2];
												break;
								case 7:	
												vertex->positionMorph8[0]=morphvertex->position[0];
												vertex->positionMorph8[1]=morphvertex->position[1];
												vertex->positionMorph8[2]=morphvertex->position[2];
										//		vertex->normalMorph8[0]=morphvertex->normal[0];
										//		vertex->normalMorph8[1]=morphvertex->normal[1];
										//		vertex->normalMorph8[2]=morphvertex->normal[2];
												break;


								case 8:	
												vertex->positionMorph9[0]=morphvertex->position[0];
												vertex->positionMorph9[1]=morphvertex->position[1];
												vertex->positionMorph9[2]=morphvertex->position[2];
										//		vertex->normalMorph8[0]=morphvertex->normal[0];
										//		vertex->normalMorph8[1]=morphvertex->normal[1];
										//		vertex->normalMorph8[2]=morphvertex->normal[2];
												break;								
								case 9:	
												vertex->positionMorph10[0]=morphvertex->position[0];
												vertex->positionMorph10[1]=morphvertex->position[1];
												vertex->positionMorph10[2]=morphvertex->position[2];
										//		vertex->normalMorph8[0]=morphvertex->normal[0];
										//		vertex->normalMorph8[1]=morphvertex->normal[1];
										//		vertex->normalMorph8[2]=morphvertex->normal[2];
												break;	
								case 10:	
												vertex->positionMorph11[0]=morphvertex->position[0];
												vertex->positionMorph11[1]=morphvertex->position[1];
												vertex->positionMorph11[2]=morphvertex->position[2];
										//		vertex->normalMorph8[0]=morphvertex->normal[0];
										//		vertex->normalMorph8[1]=morphvertex->normal[1];
										//		vertex->normalMorph8[2]=morphvertex->normal[2];
												break;	
								case 11:	
												vertex->positionMorph12[0]=morphvertex->position[0];
												vertex->positionMorph12[1]=morphvertex->position[1];
												vertex->positionMorph12[2]=morphvertex->position[2];
										//		vertex->normalMorph8[0]=morphvertex->normal[0];
										//		vertex->normalMorph8[1]=morphvertex->normal[1];
										//		vertex->normalMorph8[2]=morphvertex->normal[2];
												break;	
#endif
								default:
												return false;
								}
					}			
			}

			if (element->GetMesh()->vertexlayers.Find((GRPVERTEX_CHANNEL)(channel+GRPVERTEX_CHANNEL_MORPH_POSITION1))==-1)
				element->GetMesh()->vertexlayers.Add((GRPVERTEX_CHANNEL)(channel+GRPVERTEX_CHANNEL_MORPH_POSITION1));
			element->GetMesh()->numberoflayers=element->GetMesh()->vertexlayers.GetSize();

#endif

	return true;
}

/*-------------------------------------------------------------------
//	bool GRPDEFORMER::Init()
*/	
/**	
//	
//	 Class Destructor GRPSUBDEFORMER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/07/2015 12:58:58
//	
*/
/*-----------------------------------------------------------------*/
bool GRPDEFORMER::Init()
{
	#ifdef GRPDEFORMERS_ACTIVE

	if (target==NULL) 
		return false;

	for (XDWORD e=0;e<this->subdeformers.GetSize();e++)	
		this->subdeformers.elements.Get(e)->Init(this->target,e);
	
	if (this->target->GetNode())
	{
		GRPSHADERPROGRAM* p=NULL;
		if (this->target->GetNode()->GetMaterial())
		{
		if (this->target->GetNode()->GetMaterial()->GetLayers()->GetSize()>0)
				p=GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"MPH");		
		else
			p=GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"MPD");		
		}
		else
			p=GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"MPD");		
		
		this->target->SetProgram(p);						
	}
	
	
	#endif

	return true;
}

/*-------------------------------------------------------------------
//	GRPDEFORMER::Update
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/07/2015 14:22:24
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPDEFORMER::Update()
{
	#ifdef GRPDEFORMERS_ACTIVE

		GLFLOAT blends [4]={0.0f,0.0f,0.0f,0.0f};
		GLFLOAT blends2[4]={0.0f,0.0f,0.0f,0.0f};
		GLFLOAT blends3[4]={0.0f,0.0f,0.0f,0.0f};
		GLFLOAT blends4[4]={0.0f,0.0f,0.0f,0.0f};

			for (XDWORD e=0;e<this->subdeformers.GetSize() && e<4;e++)			
				blends[e]=this->subdeformers.elements.Get(e)->GetFactor()/100.0f;			
				if (target->GetShader()->iChannelMorph!=-1)				glUniform4fv(target->GetShader()->iChannelMorph,	(GLsizei)1,blends);	
				
			for (XDWORD i=4,e=0;i<this->subdeformers.GetSize() && e<4;e++,i++)			
				blends2[e]=this->subdeformers.elements.Get(i)->GetFactor()/100.0f;			
				if (target->GetShader()->iChannelMorph2!=-1)			glUniform4fv(target->GetShader()->iChannelMorph2, (GLsizei)1,blends2);	
			
			for (XDWORD i=8,e=0;i<this->subdeformers.GetSize() && e<4;e++,i++)			
				blends3[e]=this->subdeformers.elements.Get(i)->GetFactor()/100.0f;			
				if (target->GetShader()->iChannelMorph3!=-1)			glUniform4fv(target->GetShader()->iChannelMorph3, (GLsizei)1,blends3);	
		
	#endif
	return true;
}