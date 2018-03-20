
/*------------------------------------------------------------------------------------------
//	GRPSKYBOXPASS.CPP
//	
//	Manages skybox on pass
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 20/06/2017 10:59:56
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "grpskyboxpass.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
#include "GRPElement.h"
#include "GRPShaderLibrary.h"
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

const char GRPSHADERLIBRARY::SkyboxVertex[] =
{
														"attribute vec3 a_position;			\n"
														"varying vec4 v_position;				\n"

														"uniform mat4 invProjectionMat;\n"
														"uniform mat4 invViewMat;\n"

														"varying vec3 cubeCoords; \n"

														"void main()										\n"
														"{															\n"

														"		vec4 v = invViewMat*invProjectionMat*vec4(vec3(a_position.x,a_position.y,1.0),1.0); \n"
														"		cubeCoords	= -v.xyz; \n"

														"   v_position	= vec4(a_position.xy,1.0,1.0);						\n"
														"   gl_Position = v_position.xyww;		\n"
														"}															\n"
};

const char GRPSHADERLIBRARY::SkyboxFragment[] =
{
														"uniform samplerCube s_texture;								\n"
														"varying vec3 cubeCoords;\n"

														"void main()\n"
														"{\n"
														"    gl_FragColor = textureCube(s_texture, normalize(cubeCoords.xyz));\n"
														"}\n"
};

/*-------------------------------------------------------------------
//	 GRPSKYBOXPASS::OnAdded
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		bool : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
bool	GRPSKYBOXPASS::OnAdded()
{
	GRPBACKGROUNDPASS::OnAdded();
	plane->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Skybox")));
	return true;
}