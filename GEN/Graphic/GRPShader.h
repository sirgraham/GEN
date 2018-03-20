/*------------------------------------------------------------------------------------------
//	GRPLINUXSCREENDISPMANSHADER.H
*/	
/**	
// \class 
//   
//  Shader and Fragment wrapper
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 21/03/2014 13:13:18
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPSHADER_H_
#define _GRPSHADER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XString.h"	
#include "XMap.h"
#include "XPath.h"

#include "GRPVertex.h"
#include "GRPOpenGL.h"
#include "GRPNamed.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPELEMENT;
class GRPSHADERPROGRAM;

class GRPSHADER
{
protected:

		GRPSHADERPROGRAMTYPE										shadertype;
		GLUINT																	ID;	
		XSTRING																	source;		
		char**																	Description;

public:

		static GLUINT														CurrentShader;
		static GRPSHADERPROGRAM*								CurrentProgram;
		static GLUINT														LastShader;
		static GLUINT														MaxLights;

		static GLUINT														UseShader									(GRPSHADERPROGRAM* s);
		static GLUINT														UseShader									(GLUINT s);
		static GLUINT														UseTmpShader							(GLUINT s);
		static GLUINT														PopShader									();

																						GRPSHADER									(GRPSHADERPROGRAMTYPE type, XCHAR* source);	
		virtual															   ~GRPSHADER									();

		GLUINT																	GetID											();
		GRPSHADERPROGRAMTYPE										GetType										()																		{ return shadertype;				}

		bool																		Set												(GRPSHADERPROGRAMTYPE shadertype, XSTRING& shader);		
		bool																		Set												(GRPSHADERPROGRAMTYPE shadertype, XCHAR* shader);

		
		bool																		Compile										();

		bool																		IsLoaded;

	protected:

		void																		Clean											()
																						{
																							ID				 = 0;
																							shadertype = GRPSHADERPROGRAMTYPE_NONE;
																							Description= NULL;
																							IsLoaded	 = false;
																							source.Empty();
																						}	



		
};




class GRPSHADERPROGRAM : public GRPNAMED
{
	public:

		GRPNAMED_TYPE														GetType										() { return GRP_NAMED_TYPE_SHADER; } 
																						GRPSHADERPROGRAM					();	
		virtual																 ~GRPSHADERPROGRAM					();

		GLUINT																	GetID											();

		void																		SetIndex									(XDWORD index);
		XDWORD																	GetIndex									();

		bool																		AddShader									(GRPSHADER* shader);

		bool																		Create										(GRPSHADER* vertexshader, GRPSHADER* fragmentshader, char Description[]);
		bool																		Compile										();
		bool																		Link											();
		bool																		SetAttributes							(GRPELEMENT*);
		//void																		SetName										(char* name)	{ this->name.Set(name);		}
		//XSTRING*																GetName										()						{ return &this->name;}
		bool																		SetKey										(char* name);


		bool																		Set(const char* name, float value);
		bool																		Set(const char* name, GLUINT value);

		GLINT																		iLightDataMatrix;
		GLINT																		iLightColorMatrix;	
		GLINT																		iLightSpaceMatrix;

		GLINT 																	attributeVertex;
		GLINT 																	attributeNormal;
		GLINT 																	attributeTextCoord;
		GLINT 																	attributeSampler;
		GLINT 																	attributeSampler1;
		GLINT 																	attributeSampler2;
		GLINT 																	attributeSampler3;
		GLINT 																	attributeSampler4;
		GLINT 																	attributeSampler5;
		GLINT 																	attributeSampler6;
		GLINT 																	attributeSampler7;
		GLINT 																	attributeSampler8;
		GLINT 																	attributeSampler9;
		GLINT 																	attributeSampler10;
		GLINT 																	attributeSampler11;
		GLINT 																	attributeSampler12;
		

		GLINT																		itextureSize0;
		GLINT																		itextureSize1;
		GLINT																		itextureSize2;
		GLINT																		itextureSize3;
		GLINT																		itextureSize4;
		GLINT																		itextureSize5;
		GLINT																		itextureSize6;
		GLINT																		itextureSize7;
		GLINT																		itextureSize8;
		GLINT																		itextureSize9;
		GLINT																		itextureSize10;
		GLINT																		itextureSize11;
		GLINT																		itextureSize12;

		

		GLINT 																	attributeBinormal;
		GLINT 																	attributeTangent;

		GLINT 																	attributeBoneIndices;
		GLINT 																	attributeBoneWeights;

		GLINT 																	attributePositionMorph1;
		GLINT 																	attributePositionMorph2;
		GLINT 																	attributePositionMorph3;
		GLINT 																	attributePositionMorph4;
		GLINT 																	attributePositionMorph5;
		GLINT 																	attributePositionMorph6;
		GLINT 																	attributePositionMorph7;
		GLINT 																	attributePositionMorph8;
		GLINT 																	attributePositionMorph9;
		GLINT 																	attributePositionMorph10;
		GLINT 																	attributePositionMorph11;
		GLINT 																	attributePositionMorph12;		
/*
		GLINT 																	attributeNormalMorph1;
		GLINT 																	attributeNormalMorph2;
		GLINT 																	attributeNormalMorph3;
		GLINT 																	attributeNormalMorph4;
		GLINT 																	attributeNormalMorph5;
		GLINT 																	attributeNormalMorph6;
		GLINT 																	attributeNormalMorph7;
		GLINT 																	attributeNormalMorph8;
		*/
		GLINT																		attributeInstanceID;						

		GLINT 																	ModelMatrixLoc;
		GLINT 																	ViewMatrixLoc;
		GLINT 																	invViewMatrixLoc;
		GLINT 																	ProjectionMatrixLoc;
		GLINT 																	invProjectionMatrixLoc;


		GLINT 																	NormalMatrixLoc;

		GLINT																		TextureMatrixLoc;

		GLINT 																	ModelViewMatrixLoc;
		GLINT 																	ModelViewProjectionMatrixLoc;	
				
		GLINT 																	iMaterialMatrix;
		
		GLINT																		iGlobalTimeHandle;
		GLINT																		iResolutionHandle;

		GLINT 																	iBoneMatrices;
		GLINT 																	iBonePoseMatrices;
		GLINT																		iBoneRootMatrix;

		GLINT																		iChannelMorph;
		GLINT																		iChannelMorph2;
		GLINT																		iChannelMorph3;

		char																		key[4];
		char*																		Description;
		bool																		IsLoaded;

protected:

		XDWORD																	index;
		GLUINT																	id;	

	private:

		void																		Clean()
																						{
																							index													=0;
																							id														=0;	
																							IsLoaded											=false;
																							Description										=NULL;

																							iLightDataMatrix							=-1;
																							iLightColorMatrix							=-1;
																							iLightSpaceMatrix							=-1;

																							attributeVertex								=-1;
																							attributeNormal								=-1;
																							attributeTextCoord						=-1;
																							attributeSampler							=-1;
																							attributeSampler1							=-1;
																							attributeSampler2							=-1;
																							attributeSampler3							=-1;
																							attributeSampler4							=-1;
																							attributeSampler5							=-1;
																							attributeSampler6							=-1;
																							attributeSampler7							=-1;
																							attributeSampler8							=-1;
																							attributeSampler9							=-1;
																							attributeSampler10						=-1;
																							attributeSampler11						=-1;
																							attributeSampler12						=-1;

																							itextureSize0									=-1;
																							itextureSize1									=-1;
																							itextureSize2									=-1;
																							itextureSize3									=-1;
																							itextureSize4									=-1;
																							itextureSize5									=-1;
																							itextureSize6									=-1;
																							itextureSize7									=-1;
																							itextureSize8									=-1;
																							itextureSize9									=-1;
																							itextureSize10								=-1;
																							itextureSize11								=-1;
																							itextureSize12								=-1;

																							ModelViewProjectionMatrixLoc	=-1;																							
																							ModelMatrixLoc								=-1;
																							ViewMatrixLoc									=-1;
																							invViewMatrixLoc							=-1;
																																														
																							ProjectionMatrixLoc						=-1;
																							invProjectionMatrixLoc			  =-1;

																							NormalMatrixLoc								=-1;
																							ModelViewMatrixLoc						=-1;
					
																							iMaterialMatrix								=-1;

																							iGlobalTimeHandle							=-1;
																							iResolutionHandle							=-1;		

																							attributeBinormal							=-1;
																							attributeTangent							=-1;
																							attributeBoneIndices					=-1;
																							attributeBoneWeights					=-1;

																							attributeInstanceID						=-1;

																							iBoneMatrices									=-1;
																							iBonePoseMatrices							=-1;

																							iChannelMorph									=-1;
																							iChannelMorph2								=-1;
																							iChannelMorph3								=-1;

																							attributePositionMorph1				=-1;
																							attributePositionMorph2				=-1;
																							attributePositionMorph3				=-1;
																							attributePositionMorph4				=-1;
																							attributePositionMorph5				=-1;
																							attributePositionMorph6				=-1;
																							attributePositionMorph7				=-1;
																							attributePositionMorph8				=-1;
																							attributePositionMorph9				=-1;
																							attributePositionMorph10			=-1;
																							attributePositionMorph11			=-1;
																							attributePositionMorph12			=-1;

																							iBoneRootMatrix								=-1;

																							vertexshader=NULL;
																							fragmentshader=NULL;
																							
																							key[0]='-';
																							key[1]='-';
																							key[2]='-';
																							key[3]='\0';

																							name.Set(__L("unnasigned"));
																						}	


		GRPSHADER*															vertexshader;
		GRPSHADER*															fragmentshader;
		
		XMAP<GRPSHADERPROGRAMTYPE,GRPSHADER*>		shaders;
		
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif







