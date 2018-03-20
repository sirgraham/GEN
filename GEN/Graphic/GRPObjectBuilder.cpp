
/*------------------------------------------------------------------------------------------
//	GRPOBJECTBUILDER.CPP
//	
//	Creates Objects
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 27/05/2014 13:56:47
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "xfactory.h"
#include "GRPMappingProjectionCubic.h"
#include "GRPObjectBuilderAlgorithmExtrude.h"
#include "GRPObjectBuilderAlgorithmTriangulationEar.h"
#include "GRPMappingProjectionPlanar.h"
#include "GRPObjectBuilderAlgorithm.h"
#include "GRPFontManager.h"
#include "GRPShape.h"
//#include "GRPSprite.h"
#include "GRPSpline.h"
#include "GRPBezierSpline.h"
#include "GRPObjectBuilder.h"

#include "GRPObjectLibrary.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

 GRPVECTOR GRPOBJECTBUILDER::LeftVector			(-1,0,0);
 GRPVECTOR GRPOBJECTBUILDER::RightVector		(+1,0,0);
 GRPVECTOR GRPOBJECTBUILDER::UpVector				(0,+1,0);
 GRPVECTOR GRPOBJECTBUILDER::DownVector			(0,-1,0);
 GRPVECTOR GRPOBJECTBUILDER::NearVector			(0,0,-1);
 GRPVECTOR GRPOBJECTBUILDER::FarVector			(0,0,+1);	
 GRPVECTOR GRPOBJECTBUILDER::OriginVector		(0,0,+1);	
	

 GRPOBJECTBUILDER* GRPOBJECTBUILDER::Instance=NULL;

 /*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::GRPOBJECTBUILDER
*/	
/**	
//	
//	Class Constructor GRPOBJECTBUILDER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/10/2014 17:57:05
//	
*/
/*-----------------------------------------------------------------*/
GRPOBJECTBUILDER::GRPOBJECTBUILDER()								
{	
	Clean();
	Ini();
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::~GRPOBJECTBUILDER
*/	
/**	
//	
//	 Class Destructor GRPOBJECTBUILDER
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/10/2014 9:44:48
//	
*/
/*-----------------------------------------------------------------*/
GRPOBJECTBUILDER::~GRPOBJECTBUILDER()								
{						
	if (fontmanager)
		delete (fontmanager);
			fontmanager=NULL;

	references.DeleteContents();
	references.DeleteAll();

	GRPOBJECTLIBRARY::Destroy();

	delete(this->xmutex);
	this->xmutex=NULL;

	End();
}
/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::Ini
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/10/2014 17:57:09
//	
 
 
*/
/*-----------------------------------------------------------------*/
void GRPOBJECTBUILDER::Ini()
		{
			if (xmutex)
				delete(xmutex);

			xmutex=xfactory->Create_Mutex();
			CreateFontManager();			
		}

void GRPOBJECTBUILDER::End()
		{
			if (xmutex)
				delete(xmutex);
			Clean();
		}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateLine
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 13:13:52
//	
//	@return 			GRPELEMENT* : 
//
//  @param				programID : 
//  @param				o : 
//  @param				f : 
//  @param				material : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* GRPOBJECTBUILDER::CreateLineElement(GRPPOINT o, GRPPOINT f)
{
	GLFLOAT	linevertex[2*3];
	
	linevertex[0]	=	o.x;
	linevertex[1]	=	o.y;
	linevertex[2]	=	o.z;
	linevertex[3]	=	f.x;
	linevertex[4]	=	f.y;
	linevertex[5]	=	f.z;
				
	GLFLOAT linenormals[2*3];

	linenormals[0] = 0.0f;
	linenormals[1] = 0.0f;
	linenormals[2] = +1.0f;
	linenormals[3] = 0.0f;
	linenormals[4] = 0.0f;
	linenormals[5] = +1.0f;

	GLFLOAT linetexturecoords[2*3];

	linetexturecoords[0] = 0.0f;
	linetexturecoords[1] = 0.0f;
	linetexturecoords[2] = 1.0f;
	linetexturecoords[3] = 1.0f;
	linetexturecoords[4] = 0.0f;
	linetexturecoords[5] = 1.0f;
	
	return this->CreateFromBuffer(2,linevertex,linenormals,linetexturecoords);	
}




/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateArc
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 13:19:29
//	
//	@return 			GRPELEMENT* : 
//
//  @param				programID : 
//  @param				o : 
//  @param				f : 
//  @param				c : 
//  @param				material : 
//  @param				segments : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*	GRPOBJECTBUILDER::CreateArcElement(GRPPOINT o,	GRPPOINT f,	GRPPOINT c, XDWORD nsegments)
{		
	GLFLOAT* vertex					= new GLFLOAT[(nsegments+1)*3];
	GLFLOAT* normals				= new GLFLOAT[(nsegments+1)*3];
	GLFLOAT* texturecoords	= new GLFLOAT[(nsegments+1)*2];

	if((!vertex) || (!normals) || (!texturecoords)) return NULL;
	
	float		t = 0.0f;
	float		x = 0.0f;
	float		y = 0.0f;
	XDWORD	i	=	0;

	for(i= 0; i<=nsegments; i++)
		{  														
			x = ((1.0f - t) * (1.0f - t) * o.x) + (2.0f * (1.0f - t) * t * c.x) + (t * t * f.x);
			y = ((1.0f - t) * (1.0f - t) * o.y) + (2.0f * (1.0f - t) * t * c.y) + (t * t * f.y);
								
			vertex[(i*3)+0]=x;
			vertex[(i*3)+1]=y;
			vertex[(i*3)+2]=+0.0f;

			normals[(i*3)+0]=0;
			normals[(i*3)+1]=0;
			normals[(i*3)+2]=+1.0f;

			texturecoords[(i*2)+0]	=	t;
			texturecoords[(i*2)+1]	=	0.0f;

			t += 1.0f / nsegments;
		}				


	GRPELEMENT* element = CreateFromBuffer(nsegments+1, vertex, normals, texturecoords);

	delete vertex;
	delete normals;
	delete texturecoords;

	return element;
}


/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateBackground
*/	
/**	
//	
//	Creates a background quad
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			15/12/2014 12:31:33
//	
//	@return 			GRPELEMENT* : 
//
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* GRPOBJECTBUILDER::CreateBackground()
{
	GRPELEMENT* element	=	new GRPELEMENT();							
	if(!element) return NULL;

	GRPMESH* mesh = new GRPMESH();
	element->SetMesh(mesh);

	element->SetRenderPriority(GRPRENDERPRIORITY_BACKGROUND);

	int i=0;

	{
	GRPFACE*	face=new GRPFACE();

						face->Add(new GRPPOINT(-1.0,-1.0,+0.0,0));
						face->texturecoordinates[0]=new GRPPOINT(0.0f,0.0f,0.0f);						
						
						face->Add(new GRPPOINT(+1.0,-1.0,+0.0,1));
						face->texturecoordinates[1]=new GRPPOINT(1.0f,0.0f,0.0f);						
						
						face->Add(new GRPPOINT(+1.0,+1.0,+0.0,2));
						face->texturecoordinates[2]=new GRPPOINT(1.0f,1.0f,0.0f);						
						
						face->a=0;
						face->b=1;
						face->c=2;

						face->normal.vector[0]=0.0;
						face->normal.vector[1]=0.0;
						face->normal.vector[2]=-1.0;

						element->GetMesh()->GetFaces()->Add(face);
	}
	
	{
	GRPFACE*	face=new GRPFACE();

						face->Add(new GRPPOINT(-1.0,-1.0,+0.0,0));
						face->texturecoordinates[0]=new GRPPOINT(0.0f,0.0f,0.0f);						
						
						
						face->Add(new GRPPOINT(+1.0,+1.0,+0.0,2));
						face->texturecoordinates[1]=new GRPPOINT(1.0f,1.0f,0.0f);						
						

						face->Add(new GRPPOINT(-1.0,+1.0,+0.0,3));
						face->texturecoordinates[2]=new GRPPOINT(0.0f,1.0f,0.0f);						
						
						face->a=0;
						face->b=2;
						face->c=3;

						face->normal.vector[0]=0.0;
						face->normal.vector[1]=0.0;
						face->normal.vector[2]=1.0;

						element->GetMesh()->GetFaces()->Add(face);
	}
		
	element->GetMesh()->SetFaceBuffers();
	element->GetMesh()->ReassignUVs();
	
	return		element;
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreatePoint
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/10/2015 14:16:50
//	
//	@return 			GRPELEMENT* : 
//
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* GRPOBJECTBUILDER::CreatePoint()
{
	GRPELEMENT* element=new GRPELEMENT();
	
	GRPVERTEX vertex;
	
	memset(&vertex,0,sizeof(GRPVERTEX));
	
	element->GetMesh()->LoadModel	(	1,	vertex.position, vertex.normal, vertex.texturecoordinates);

	return element;
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateMasterSprite
*/	
/**	
//	
//	Creates referible master sprite
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/09/2014 12:24:34
//	
//	@return 			GRPELEMENT* : masterSprite
//
//  @param				p : programID to use on mastersprite
*/
/*-----------------------------------------------------------------*/
/*
GRPSPRITE* GRPOBJECTBUILDER::CreateMasterSprite(GRPSPRITE::TYPE type)
{
	
	GRPSPRITE* sprite=NULL;
	
	switch(type) 
	{
		case GRPSPRITE::POINT:			sprite= new GRPSPRITEPOINT(); break;
		case GRPSPRITE::QUAD:				sprite= new GRPSPRITEQUAD	();	break;
		case GRPSPRITE::STRIP:			sprite= new GRPSPRITESTRIP();	break;
		case GRPSPRITE::BILLBOARD:	sprite= new GRPBILLBOARD	();	break;
	}

	switch(type)
	{
	case GRPSPRITE::POINT:	
		{
				GRPVERTEX vertex;	
				memset(&vertex,0,sizeof(GRPVERTEX));	
				sprite->LoadModel	(	1,	vertex.position, vertex.normal, vertex.texturecoordinates);			
		}
		break;
	case GRPSPRITE::QUAD:	
			sprite=this->CreateSpritePlane	(); 			
		break;
	default: break;
	}

							sprite->UpdateBoundingBox();
							sprite->GetBoundingBox()->blocked=true;
							masterSprite=sprite;

							
	return sprite;

}
*/
/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateSprite
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			18/09/2014 13:56:42
//	
//	@return 			GRPSPRITE* : 
//
//  @param				texture : 
*/
/*-----------------------------------------------------------------*/
/*
GRPSPRITE*	GRPOBJECTBUILDER::CreateSprite	( GRPSPRITE::TYPE spritetype)
{
	GRPSPRITE*	sprite=NULL;
	
	switch(spritetype) 
	{
		case GRPSPRITE::POINT:			sprite= new GRPSPRITEPOINT(); break;
		case GRPSPRITE::QUAD:				sprite= new GRPSPRITEQUAD	();	break;
		case GRPSPRITE::STRIP:			sprite= new GRPSPRITESTRIP();	break;
		case GRPSPRITE::BILLBOARD:	sprite= new GRPBILLBOARD	();	break;
	}

	if (sprite==NULL) return NULL;
	
	if (!masterSprite)
		CreateMasterSprite(spritetype);

	sprite->Reference(this->masterSprite);
		
	return sprite;
}
*/
/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateSpritePlane
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/10/2015 16:12:04
//	
//	@return 			GRPSPRITE* : 
//
*/
/*-----------------------------------------------------------------*/
/*
GRPSPRITE*	GRPOBJECTBUILDER::CreateSpritePlane()
{
	GRPSPRITE* element=new GRPSPRITEQUAD();
	
	int nvertex=4;
	element->SetNVertex(nvertex);
	
	if (element->GetVertexData()!=NULL) delete[] element->GetVertexData();
	if (element->GetIndexData()!=NULL)	delete[] element->GetIndexData();

	element->SetVertexData(new GRPVERTEX[nvertex]);
	if (element->GetVertexData()==NULL)
	{
		XDEBUG_PRINTCOLOR(4,__L("GRPELEMENT::LoadModel Not enought memory"));	
		return NULL;
	}

	element->SetIndexData(new XWORD[nvertex]);
	if (element->GetIndexData()==NULL)
	{
		XDEBUG_PRINTCOLOR(4,__L("GRPELEMENT::LoadModel Not enought memory"));	
		return NULL;
	}

	element->SetNindexData(nvertex);
	element->SetNvertexData(nvertex);

	GRPVERTEX*	vertexdata=element->GetVertexData	();
	XWORD*			indexdata	=element->GetIndexData	();

	int e=0;



		indexdata	[e]=e;

		vertexdata[e].position					[0]=-1.0;
		vertexdata[e].position					[1]=+1.0;
		vertexdata[e].position					[2]=+0.0;

		vertexdata[e].normal						[0]=+0.0;
		vertexdata[e].normal						[1]=+0.0;
		vertexdata[e].normal						[2]=-1.0;

		vertexdata[e].texturecoordinates[0]=0.0;
		vertexdata[e].texturecoordinates[1]=1.0;

		e++;
		indexdata	[e]=e;

		vertexdata[e].position					[0]=-1.0;
		vertexdata[e].position					[1]=-1.0;
		vertexdata[e].position					[2]=+0.0;

		vertexdata[e].normal						[0]=+0.0;
		vertexdata[e].normal						[1]=+0.0;
		vertexdata[e].normal						[2]=-1.0;

		vertexdata[e].texturecoordinates[0]=0.0;
		vertexdata[e].texturecoordinates[1]=0.0;
	
		e++;

		indexdata	[e]=e;

		vertexdata[e].position					[0]=+1.0;
		vertexdata[e].position					[1]=+1.0;
		vertexdata[e].position					[2]=+0.0;

		vertexdata[e].normal						[0]=+0.0;
		vertexdata[e].normal						[1]=+0.0;
		vertexdata[e].normal						[2]=-1.0;

		vertexdata[e].texturecoordinates[0]=1.0;
		vertexdata[e].texturecoordinates[1]=1.0;

		e++;
		
		indexdata	[e]=e;

		vertexdata[e].position					[0]=+1.0;
		vertexdata[e].position					[1]=-1.0;
		vertexdata[e].position					[2]=+0.0;

		vertexdata[e].normal						[0]=+0.0;
		vertexdata[e].normal						[1]=+0.0;
		vertexdata[e].normal						[2]=-1.0;

		vertexdata[e].texturecoordinates[0]=1.0;
		vertexdata[e].texturecoordinates[1]=0.0;
	
	element->SetRenderMode(GRPRENDERMODE_TRIANGLE_STRIP);
	element->SetIsLoaded(false);

	return element;
}
*/
/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateEllipse
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 13:35:25
//	
//	@return 			GRPELEMENT* : 
//
//  @param				programID : 
//  @param				o : 
//  @param				radius : 
//  @param				relation : 
//  @param				material : 
//  @param				segments : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*	GRPOBJECTBUILDER::CreateEllipse(GRPPOINT o, GLFLOAT radius, float relation, XDWORD nsegments)
{		
	GLFLOAT* vertex					=	new GLFLOAT[(nsegments+1)*3];
	GLFLOAT* normals				=	new GLFLOAT[(nsegments+1)*3];
	GLFLOAT* texturecoords	=	new GLFLOAT[(nsegments+1)*2];
						
	if((!vertex) || (!normals) || (!texturecoords)) return NULL;
	
	float		x					= 0.0f;
	float		y					= 0.0f;
	float		z					= 0.0f;
	XDWORD	i					=	0;
	GLFLOAT angle			=	0.0f;
	GLFLOAT arcangle	=	(360.0f / nsegments);	


	for(i= 0; i<=nsegments; i++)
		{  																				
			x = (float)sin(angle*(PI/180));
			y = (float)cos(angle*(PI/180));
			z	= o.z;
															
			vertex[(i*3)+0]		= relation*radius*(x+o.x);
			vertex[(i*3)+1]		= radius*(y+o.y);
			vertex[(i*3)+2]		= z;

			normals[(i*3)+0]	=	0;
			normals[(i*3)+1]	=	0;
			
			normals[(i*3)+2]	= 1.0f;

			texturecoords[(i*2)+0] = (-x+1)/2;
			texturecoords[(i*2)+1] = (y+1)/2;

			angle += arcangle;
		}								

	GRPELEMENT* element= new GRPELEMENT();
	if(element) 
		{
			GRPMESH* mesh=new GRPMESH();
			if(!mesh->LoadModel(nsegments+1, vertex, normals, texturecoords))
				{
					delete mesh;
					delete element;
					element = NULL;
				}
			element->SetMesh(mesh);			
			element->SetRenderMode(GRPRENDERMODE_LINE_STRIP);							
		}

	
	delete vertex;
	delete normals;
	delete texturecoords;

	return element;
}


/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateCircle
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/10/2014 9:45:42
//	
//	@return 			GRPELEMENT* : 
//
//  @param				programID : 
//  @param				o : 
//  @param				radius : 
//  @param				material :  
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*		GRPOBJECTBUILDER::CreateCircle(GRPPOINT o,	GLFLOAT radius,	XDWORD nsegments)						
{
	return CreateEllipse(o , radius, 1.0f, nsegments);
}


/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateCube
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 13:40:33
//	
//	@return 			GRPELEMENT* : 
//
//  @param				programID : 
//  @param				material : 
*/
/*-----------------------------------------------------------------*/
GRPMESH* GRPOBJECTBUILDER::CreateCube()
{	
		GRPMESH* mesh= GRPOBJECTLIBRARY::Get()->GetMesh(__L("GRPOBJECTBUILDER::cube"));
		if (mesh!=NULL)
			return mesh;

	mesh=new GRPMESH();
	XVECTOR<GRPFACE*> faces;

	//------------------- top

	GRPFACE*	face1a=new GRPFACE();
						face1a->vertices[0]=new GRPPOINT(-0.5,+0.5,+0.5);
						face1a->vertices[1]=new GRPPOINT(+0.5,+0.5,+0.5);
						face1a->vertices[2]=new GRPPOINT(-0.5,+0.5,-0.5);

						face1a->normals[0]=new GRPPOINT(+0.0,+1.0,+0.0);
						face1a->normals[1]=new GRPPOINT(+0.0,+1.0,+0.0);
						face1a->normals[2]=new GRPPOINT(+0.0,+1.0,+0.0);

						face1a->texturecoordinates[0]=new GRPPOINT(+0.0,+0.0,+0.0);
						face1a->texturecoordinates[1]=new GRPPOINT(+1.0,+0.0,+0.0);
						face1a->texturecoordinates[2]=new GRPPOINT(+0.0,+1.0,+0.0);

	faces.Add(face1a);

	
	GRPFACE*	face1b=new GRPFACE();
						face1b->vertices[0]=new GRPPOINT(+0.5,+0.5,+0.5);
						face1b->vertices[1]=new GRPPOINT(+0.5,+0.5,-0.5);
						face1b->vertices[2]=new GRPPOINT(-0.5,+0.5,-0.5);

						face1b->normals[0]=new GRPPOINT(+0.0,+1.0,+0.0);
						face1b->normals[1]=new GRPPOINT(+0.0,+1.0,+0.0);
						face1b->normals[2]=new GRPPOINT(+0.0,+1.0,+0.0);
	
						face1b->texturecoordinates[0]=new GRPPOINT(+1.0,+0.0,+0.0);
						face1b->texturecoordinates[1]=new GRPPOINT(+1.0,+1.0,+0.0);
						face1b->texturecoordinates[2]=new GRPPOINT(+0.0,+1.0,+0.0);
	
	faces.Add(face1b);
	
	//------------------- back


	GRPFACE*	face2a=new GRPFACE();
						face2a->vertices[0]=new GRPPOINT(-0.5,+0.5,-0.5);
						face2a->vertices[1]=new GRPPOINT(+0.5,+0.5,-0.5);
						face2a->vertices[2]=new GRPPOINT(-0.5,-0.5,-0.5);

						face2a->normals[0]=new GRPPOINT(+0.0,+0.0,-1.0);
						face2a->normals[1]=new GRPPOINT(+0.0,+0.0,-1.0);
						face2a->normals[2]=new GRPPOINT(+0.0,+0.0,-1.0);

						face2a->texturecoordinates[0]=new GRPPOINT(+1.0,+1.0,+0.0);
						face2a->texturecoordinates[1]=new GRPPOINT(+0.0,+1.0,+0.0);
						face2a->texturecoordinates[2]=new GRPPOINT(+1.0,+0.0,+0.0);

	faces.Add(face2a);


	GRPFACE*	face2b=new GRPFACE();
						face2b->vertices[0]=new GRPPOINT(+0.5,+0.5,-0.5);
						face2b->vertices[1]=new GRPPOINT(+0.5,-0.5,-0.5);
						face2b->vertices[2]=new GRPPOINT(-0.5,-0.5,-0.5);

						face2b->normals[0]=new GRPPOINT(+0.0,+0.0,-1.0);
						face2b->normals[1]=new GRPPOINT(+0.0,+0.0,-1.0);
						face2b->normals[2]=new GRPPOINT(+0.0,+0.0,-1.0);
	
						face2b->texturecoordinates[0]=new GRPPOINT(+0.0,+1.0,+0.0);
						face2b->texturecoordinates[1]=new GRPPOINT(+0.0,+0.0,+0.0);	
						face2b->texturecoordinates[2]=new GRPPOINT(+1.0,+0.0,+0.0);

	faces.Add(face2b);

	//------ front

	GRPFACE*	face3a=new GRPFACE();
						face3a->vertices[0]=new GRPPOINT(-0.5,+0.5,+0.5);	
						face3a->vertices[1]=new GRPPOINT(-0.5,-0.5,+0.5);
						face3a->vertices[2]=new GRPPOINT(+0.5,+0.5,+0.5);

						face3a->normals[0]=new GRPPOINT(+0.0,+0.0,+1.0);
						face3a->normals[1]=new GRPPOINT(+0.0,+0.0,+1.0);
						face3a->normals[2]=new GRPPOINT(+0.0,+0.0,+1.0);

						face3a->texturecoordinates[0]=new GRPPOINT(+0.0,+1.0,+0.0);
						face3a->texturecoordinates[1]=new GRPPOINT(+0.0,+0.0,+0.0);	
						face3a->texturecoordinates[2]=new GRPPOINT(+1.0,+1.0,+0.0);

	faces.Add(face3a);


	GRPFACE*	face3b=new GRPFACE(); 
						face3b->vertices[0]=new GRPPOINT(+0.5,+0.5,+0.5);
						face3b->vertices[1]=new GRPPOINT(-0.5,-0.5,+0.5);
						face3b->vertices[2]=new GRPPOINT(+0.5,-0.5,+0.5);

						face3b->normals[0]=new GRPPOINT(+0.0,+0.0,+1.0);
						face3b->normals[1]=new GRPPOINT(+0.0,+0.0,+1.0);
						face3b->normals[2]=new GRPPOINT(+0.0,+0.0,+1.0);
	
						face3b->texturecoordinates[0]=new GRPPOINT(+1.0,+1.0,+0.0);	
						face3b->texturecoordinates[1]=new GRPPOINT(+0.0,+0.0,+0.0);
						face3b->texturecoordinates[2]=new GRPPOINT(+1.0,+0.0,+0.0);	

	faces.Add(face3b);

	//--------------------------- right

	GRPFACE*	face4a=new GRPFACE();
						face4a->vertices[0]=new GRPPOINT(+0.5,+0.5,+0.5);	
						face4a->vertices[1]=new GRPPOINT(+0.5,-0.5,-0.5);
						face4a->vertices[2]=new GRPPOINT(+0.5,+0.5,-0.5);
	
						face4a->normals[0]=new GRPPOINT(+1.0,+0.0,+0.0);
						face4a->normals[1]=new GRPPOINT(+1.0,+0.0,+0.0);
						face4a->normals[2]=new GRPPOINT(+1.0,+0.0,+0.0);

						face4a->texturecoordinates[0]=new GRPPOINT(+0.0,+1.0,+0.0);
						face4a->texturecoordinates[1]=new GRPPOINT(+1.0,+0.0,+0.0);	
						face4a->texturecoordinates[2]=new GRPPOINT(+1.0,+1.0,+0.0);

	faces.Add(face4a);

	
	GRPFACE*	face4b=new GRPFACE(); 
						face4b->vertices[0]=new GRPPOINT(+0.5,+0.5,+0.5);
						face4b->vertices[1]=new GRPPOINT(+0.5,-0.5,+0.5);
						face4b->vertices[2]=new GRPPOINT(+0.5,-0.5,-0.5);

						face4b->normals[0]=new GRPPOINT(+1.0,+0.0,+0.0);
						face4b->normals[1]=new GRPPOINT(+1.0,+0.0,+0.0);
						face4b->normals[2]=new GRPPOINT(+1.0,+0.0,+0.0);
	
						face4b->texturecoordinates[0]=new GRPPOINT(+0.0,+1.0,+0.0);	
						face4b->texturecoordinates[1]=new GRPPOINT(+0.0,+0.0,+0.0);
						face4b->texturecoordinates[2]=new GRPPOINT(+1.0,+0.0,+0.0);	

	faces.Add(face4b);
	
		//--------------------------- left

	GRPFACE*	face5a=new GRPFACE();
						face5a->vertices[0]=new GRPPOINT(-0.5,+0.5,+0.5);		
						face5a->vertices[1]=new GRPPOINT(-0.5,+0.5,-0.5);
						face5a->vertices[2]=new GRPPOINT(-0.5,-0.5,-0.5);
	
						face5a->normals[0]=new GRPPOINT(-1.0,+0.0,+0.0);
						face5a->normals[1]=new GRPPOINT(-1.0,+0.0,+0.0);
						face5a->normals[2]=new GRPPOINT(-1.0,+0.0,+0.0);

						face5a->texturecoordinates[0]=new GRPPOINT(+0.0,+1.0,+0.0);
						face5a->texturecoordinates[1]=new GRPPOINT(+1.0,+1.0,+0.0);	
						face5a->texturecoordinates[2]=new GRPPOINT(+1.0,+0.0,+0.0);

	faces.Add(face5a);

	
	GRPFACE*	face5b=new GRPFACE(); 
						face5b->vertices[0]=new GRPPOINT(-0.5,+0.5,+0.5);	
						face5b->vertices[1]=new GRPPOINT(-0.5,-0.5,-0.5);
						face5b->vertices[2]=new GRPPOINT(-0.5,-0.5,+0.5);

						face5b->normals[0]=new GRPPOINT(-1.0,+0.0,+0.0);
						face5b->normals[1]=new GRPPOINT(-1.0,+0.0,+0.0);
						face5b->normals[2]=new GRPPOINT(-1.0,+0.0,+0.0);
	
						face5b->texturecoordinates[0]=new GRPPOINT(+0.0,+1.0,+0.0);	
						face5b->texturecoordinates[1]=new GRPPOINT(+1.0,+0.0,+0.0);
						face5b->texturecoordinates[2]=new GRPPOINT(+0.0,+0.0,+0.0);	

	faces.Add(face5b);
	
	//--

	//--------------------------- bottom

	GRPFACE*	face6a=new GRPFACE();
						face6a->vertices[0]=new GRPPOINT(-0.5,-0.5,-0.5);		
						face6a->vertices[1]=new GRPPOINT(+0.5,-0.5,-0.5);
						face6a->vertices[2]=new GRPPOINT(-0.5,-0.5,+0.5);
	
						face6a->normals[0]=new GRPPOINT(+0.0,-1.0,+0.0);
						face6a->normals[1]=new GRPPOINT(+0.0,-1.0,+0.0);
						face6a->normals[2]=new GRPPOINT(+0.0,-1.0,+0.0);

						face6a->texturecoordinates[0]=new GRPPOINT(+1.0,+1.0,+0.0);
						face6a->texturecoordinates[1]=new GRPPOINT(+0.0,+1.0,+0.0);
						face6a->texturecoordinates[2]=new GRPPOINT(+1.0,+0.0,+0.0);

	faces.Add(face6a);
	
	
	GRPFACE*	face6b=new GRPFACE(); 
						face6b->vertices[0]=new GRPPOINT(+0.5,-0.5,+0.5);	
						face6b->vertices[1]=new GRPPOINT(-0.5,-0.5,+0.5);
						face6b->vertices[2]=new GRPPOINT(+0.5,-0.5,-0.5);

						face6b->normals[0]=new GRPPOINT(+0.0,-1.0,+0.0);
						face6b->normals[1]=new GRPPOINT(+0.0,-1.0,+0.0);
						face6b->normals[2]=new GRPPOINT(+0.0,-1.0,+0.0);
	
						face6b->texturecoordinates[0]=new GRPPOINT(+0.0,+0.0,+0.0);
						face6b->texturecoordinates[1]=new GRPPOINT(+1.0,+0.0,+0.0);
						face6b->texturecoordinates[2]=new GRPPOINT(+0.0,+1.0,+0.0);
	
	faces.Add(face6b);
	
	
	//--
	mesh->LoadModel(&faces);
	
	GRPOBJECTLIBRARY::Get()->IndexMesh(__L("GRPOBJECTBUILDER::cube"),mesh);

	faces.DeleteContents();
	faces.DeleteAll();
	
	return mesh;
}




/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateFromBuffer
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 13:42:00
//	
//	@return 			GRPELEMENT* : 
//
//  @param				programID : 
//  @param				nvertex : 
//  @param				vertexpositions : 
//  @param				vertexnormals : 
//  @param				vertextexturecoords : 
//  @param				material : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*	GRPOBJECTBUILDER::CreateFromBuffer(int nvertex, GLFLOAT* vertexpositions, GLFLOAT* vertexnormals, GLFLOAT* vertextexturecoords)
{
	GRPELEMENT* element	=	new GRPELEMENT();
	if(!element) return NULL;

	if(!element->GetMesh()->LoadModel(nvertex, vertexpositions, vertexnormals, vertextexturecoords))
		{
			delete element;
			element = NULL;
		}

	element->SetProgram(GRPSHADER::CurrentProgram);

	return element;
}



/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreatePlane
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/10/2015 16:22:12
//	
//	@return 			GRPELEMENT* : 
//
//  @param				axis : GRPOBJECTBUILDER::AXIS
//  @param				doublesided : (default false)
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*		GRPOBJECTBUILDER::CreatePlane		(GRPOBJECTBUILDER::AXIS axis, bool doublesided)
{	
	GRPELEMENT*	element				 =new GRPELEMENT();
					if (element==NULL) return NULL;
  GRPMESH* mesh							 =new GRPMESH();
						element->SetMesh(mesh);

	XVECTOR<GRPFACE*> faces;
	
	switch(axis)
	{
	case AXIS_Y:
						{
					GRPFACE* face5a=new GRPFACE();
					face5a->vertices[0]=new GRPPOINT(+0.0,+1.0,+1.0);		
					face5a->vertices[1]=new GRPPOINT(+0.0,+1.0,-1.0);
					face5a->vertices[2]=new GRPPOINT(+0.0,-1.0,-1.0);
	
					face5a->normals[0]=new GRPPOINT(-1.0,+0.0,+0.0);
					face5a->normals[1]=new GRPPOINT(-1.0,+0.0,+0.0);
					face5a->normals[2]=new GRPPOINT(-1.0,+0.0,+0.0);

					face5a->texturecoordinates[0]=new GRPPOINT(+1.0,+1.0,+0.0);
					face5a->texturecoordinates[1]=new GRPPOINT(+0.0,+1.0,+0.0);	
					face5a->texturecoordinates[2]=new GRPPOINT(+0.0,+0.0,+0.0);

					faces.Add(face5a);

	
					GRPFACE* face5b=new GRPFACE(); 
					face5b->vertices[0]=new GRPPOINT(+0.0,+1.0,+1.0);	
					face5b->vertices[1]=new GRPPOINT(+0.0,-1.0,-1.0);
					face5b->vertices[2]=new GRPPOINT(+0.0,-1.0,+1.0);

					face5b->normals[0]=new GRPPOINT(-1.0,+0.0,+0.0);
					face5b->normals[1]=new GRPPOINT(-1.0,+0.0,+0.0);
					face5b->normals[2]=new GRPPOINT(-1.0,+0.0,+0.0);
	
					face5b->texturecoordinates[0]=new GRPPOINT(+1.0,+1.0,+0.0);	
					face5b->texturecoordinates[1]=new GRPPOINT(+0.0,+0.0,+0.0);
					face5b->texturecoordinates[2]=new GRPPOINT(+1.0,+0.0,+0.0);	

					faces.Add(face5b);

							if (doublesided)
							{
									GRPFACE* face4a=new GRPFACE();
									face4a->vertices[0]=new GRPPOINT(+0.0,+1.0,+1.0);	
									face4a->vertices[1]=new GRPPOINT(+0.0,-1.0,-1.0);
									face4a->vertices[2]=new GRPPOINT(+0.0,+1.0,-1.0);
	
									face4a->normals[0]=new GRPPOINT(+0.0,+0.0,+0.0);
									face4a->normals[1]=new GRPPOINT(+0.0,+0.0,+0.0);
									face4a->normals[2]=new GRPPOINT(+0.0,+0.0,+0.0);

									face4a->texturecoordinates[0]=new GRPPOINT(+0.0,+1.0,+0.0);
									face4a->texturecoordinates[1]=new GRPPOINT(+1.0,+0.0,+0.0);	
									face4a->texturecoordinates[2]=new GRPPOINT(+1.0,+1.0,+0.0);

									faces.Add(face4a);

	
									GRPFACE* face4b=new GRPFACE(); 
									face4b->vertices[0]=new GRPPOINT(+0.0,+1.0,+1.0);
									face4b->vertices[1]=new GRPPOINT(+0.0,-1.0,+1.0);
									face4b->vertices[2]=new GRPPOINT(+0.0,-1.0,-1.0);

									face4b->normals[0]=new GRPPOINT(+0.0,+0.0,+0.0);
									face4b->normals[1]=new GRPPOINT(+0.0,+0.0,+0.0);
									face4b->normals[2]=new GRPPOINT(+0.0,+0.0,+0.0);
	
									face4b->texturecoordinates[0]=new GRPPOINT(+0.0,+1.0,+0.0);	
									face4b->texturecoordinates[1]=new GRPPOINT(+0.0,+0.0,+0.0);
									face4b->texturecoordinates[2]=new GRPPOINT(+1.0,+0.0,+0.0);	

									faces.Add(face4b);
							}
						}
		break;
	case AXIS_X:
						{
							GRPFACE* face3a=new GRPFACE();
							face3a->vertices[0]=new GRPPOINT(-1.0,+1.0,+0.0);	
							face3a->vertices[1]=new GRPPOINT(-1.0,-1.0,+0.0);
							face3a->vertices[2]=new GRPPOINT(+1.0,+1.0,+0.0);

							face3a->normals[0]=new GRPPOINT(+0.0,+0.0,+1.0);
							face3a->normals[1]=new GRPPOINT(+0.0,+0.0,+1.0);
							face3a->normals[2]=new GRPPOINT(+0.0,+0.0,+1.0);

							face3a->texturecoordinates[0]=new GRPPOINT(+0.0,+1.0,+0.0);
							face3a->texturecoordinates[1]=new GRPPOINT(+0.0,+0.0,+0.0);	
							face3a->texturecoordinates[2]=new GRPPOINT(+1.0,+1.0,+0.0);

							faces.Add(face3a);


							GRPFACE* face3b=new GRPFACE(); 
							face3b->vertices[0]=new GRPPOINT(+1.0,+1.0,+0.0);
							face3b->vertices[1]=new GRPPOINT(-1.0,-1.0,+0.0);
							face3b->vertices[2]=new GRPPOINT(+1.0,-1.0,+0.0);

							face3b->normals[0]=new GRPPOINT(+0.0,+0.0,+1.0);
							face3b->normals[1]=new GRPPOINT(+0.0,+0.0,+1.0);
							face3b->normals[2]=new GRPPOINT(+0.0,+0.0,+1.0);
	
							face3b->texturecoordinates[0]=new GRPPOINT(+1.0,+1.0,+0.0);	
							face3b->texturecoordinates[1]=new GRPPOINT(+0.0,+0.0,+0.0);
							face3b->texturecoordinates[2]=new GRPPOINT(+1.0,+0.0,+0.0);	

							faces.Add(face3b);

							if (doublesided)
							{							
								GRPFACE* face2a=new GRPFACE();
								face2a->vertices[0]=new GRPPOINT(-1.0,+1.0,+0.0);
								face2a->vertices[1]=new GRPPOINT(+1.0,+1.0,+0.0);
								face2a->vertices[2]=new GRPPOINT(-1.0,-1.0,+0.0);

								face2a->normals[0]=new GRPPOINT(+0.0,+0.0,-1.0);
								face2a->normals[1]=new GRPPOINT(+0.0,+0.0,-1.0);
								face2a->normals[2]=new GRPPOINT(+0.0,+0.0,-1.0);

								face2a->texturecoordinates[0]=new GRPPOINT(+1.0,+1.0,+0.0);
								face2a->texturecoordinates[1]=new GRPPOINT(+0.0,+1.0,+0.0);
								face2a->texturecoordinates[2]=new GRPPOINT(+1.0,+0.0,+0.0);

								faces.Add(face2a);


								GRPFACE* face2b=new GRPFACE();
								face2b->vertices[0]=new GRPPOINT(+1.0,+1.0,+0.0);
								face2b->vertices[1]=new GRPPOINT(+1.0,-1.0,+0.0);
								face2b->vertices[2]=new GRPPOINT(-1.0,-1.0,+0.0);

								face2b->normals[0]=new GRPPOINT(+0.0,+0.0,-1.0);
								face2b->normals[1]=new GRPPOINT(+0.0,+0.0,-1.0);
								face2b->normals[2]=new GRPPOINT(+0.0,+0.0,-1.0);
	
								face2b->texturecoordinates[0]=new GRPPOINT(+0.0,+1.0,+0.0);
								face2b->texturecoordinates[1]=new GRPPOINT(+0.0,+0.0,+0.0);	
								face2b->texturecoordinates[2]=new GRPPOINT(+1.0,+0.0,+0.0);

								faces.Add(face2b);
							}
							
						}
						break;
	case AXIS_Z:	
	default:
						{
						GRPFACE* face1=new GRPFACE();
						face1->vertices[0]=new GRPPOINT(-1.0,+0.0,+1.0);
						face1->vertices[1]=new GRPPOINT(+1.0,+0.0,+1.0);
						face1->vertices[2]=new GRPPOINT(-1.0,+0.0,-1.0);

						face1->normals[0]=new GRPPOINT(+0.0,+1.0,+0.0);
						face1->normals[1]=new GRPPOINT(+0.0,+1.0,+0.0);
						face1->normals[2]=new GRPPOINT(+0.0,+1.0,+0.0);

						face1->texturecoordinates[0]=new GRPPOINT(+0.0,+0.0,+0.0);
						face1->texturecoordinates[1]=new GRPPOINT(+1.0,+0.0,+0.0);
						face1->texturecoordinates[2]=new GRPPOINT(+0.0,+1.0,+0.0);
						face1->a=0;
						face1->b=1;
						face1->c=2;
						faces.Add(face1);

	
						GRPFACE* face2=new GRPFACE();
						face2->vertices[0]=new GRPPOINT(+1.0,+0.0,+1.0);
						face2->vertices[1]=new GRPPOINT(+1.0,+0.0,-1.0);
						face2->vertices[2]=new GRPPOINT(-1.0,+0.0,-1.0);

						face2->normals[0]=new GRPPOINT(+0.0,+1.0,+0.0);
						face2->normals[1]=new GRPPOINT(+0.0,+1.0,+0.0);
						face2->normals[2]=new GRPPOINT(+0.0,+1.0,+0.0);
	
						face2->texturecoordinates[0]=new GRPPOINT(+1.0,+0.0,+0.0);
						face2->texturecoordinates[1]=new GRPPOINT(+1.0,+1.0,+0.0);
						face2->texturecoordinates[2]=new GRPPOINT(+0.0,+1.0,+0.0);
	
						face2->a=0;
						face2->b=1;
						face2->c=2;
						faces.Add(face2);

						if (doublesided)
							{								
							GRPFACE* face6a=new GRPFACE();
							face6a->vertices[0]=new GRPPOINT(-1.0,+0.0,-1.0);		
							face6a->vertices[1]=new GRPPOINT(+1.0,+0.0,-1.0);
							face6a->vertices[2]=new GRPPOINT(-1.0,+0.0,+1.0);
	
							face6a->normals[0]=new GRPPOINT(+0.0,-1.0,+0.0);
							face6a->normals[1]=new GRPPOINT(+0.0,-1.0,+0.0);
							face6a->normals[2]=new GRPPOINT(+0.0,-1.0,+0.0);

							face6a->texturecoordinates[0]=new GRPPOINT(+1.0,+1.0,+0.0);
							face6a->texturecoordinates[1]=new GRPPOINT(+0.0,+1.0,+0.0);
							face6a->texturecoordinates[2]=new GRPPOINT(+1.0,+0.0,+0.0);

							faces.Add(face6a);
	
	
							GRPFACE* face6b=new GRPFACE(); 
							face6b->vertices[0]=new GRPPOINT(+1.0,+0.0,+1.0);	
							face6b->vertices[1]=new GRPPOINT(-1.0,+0.0,+1.0);
							face6b->vertices[2]=new GRPPOINT(+1.0,+0.0,-1.0);

							face6b->normals[0]=new GRPPOINT(+0.0,-1.0,+0.0);
							face6b->normals[1]=new GRPPOINT(+0.0,-1.0,+0.0);
							face6b->normals[2]=new GRPPOINT(+0.0,-1.0,+0.0);
	
							face6b->texturecoordinates[0]=new GRPPOINT(+0.0,+0.0,+0.0);
							face6b->texturecoordinates[1]=new GRPPOINT(+1.0,+0.0,+0.0);
							face6b->texturecoordinates[2]=new GRPPOINT(+0.0,+1.0,+0.0);
	
							faces.Add(face6b);
							}
						}
						break;
	}

	element->GetMesh()->LoadModel(&faces);

	faces.DeleteContents();
	faces.DeleteAll();

	return element;
}

	

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateBox
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/08/2017 15:58:45
//	
//	@return 			GRPELEMENT* : 
//
//  @param				ox : 
//  @param				oy : 
//  @param				fx : 
//  @param				fy : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*		GRPOBJECTBUILDER::CreateBox(GLFLOAT w, GLFLOAT h, GLFLOAT texfactorx, GLFLOAT texfactory, GLFLOAT textureoffsetx, GLFLOAT textureoffsety)
{

	GRPELEMENT*	element				 =new GRPELEMENT();
					if (element==NULL) return NULL;

  GRPMESH* mesh							 =new GRPMESH();
					 element->SetIsInstance(false);
					 element->SetMesh(mesh);

	XVECTOR<GRPFACE*> faces;


	float tyo = texfactory, tyf = 0.0f;

	GRPFACE* face3a=new GRPFACE();
							face3a->vertices[0]=new GRPPOINT(+0.0,	+h,		+0.0);	
							face3a->vertices[1]=new GRPPOINT(+w,		+0.0,	+0.0);
							face3a->vertices[2]=new GRPPOINT(+w,		+h,		+0.0);
			
							face3a->normals[0]	=new GRPPOINT(+0.0,	+0.0,	+1.0);
							face3a->normals[1]	=new GRPPOINT(+0.0,	+0.0,	+1.0);
							face3a->normals[2]	=new GRPPOINT(+0.0,	+0.0,	+1.0);

							face3a->texturecoordinates[0]=new GRPPOINT(+0.0f			 + textureoffsetx, tyo+textureoffsety,				+0.0);
							face3a->texturecoordinates[1]=new GRPPOINT(+texfactorx + textureoffsetx, tyf+textureoffsety,				+0.0);
							face3a->texturecoordinates[2]=new GRPPOINT(+texfactorx + textureoffsetx, tyo+textureoffsety,				+0.0);
							
							faces.Add(face3a);

							
	GRPFACE* face3b=new GRPFACE(); 
							face3b->vertices[0]=new GRPPOINT(+0.0,	+h,		+0.0);
							face3b->vertices[1]=new GRPPOINT(+0.0,	+0.0,	+0.0);
							face3b->vertices[2]=new GRPPOINT(+w,		+0.0,	+0.0);

							face3b->normals[0]=new GRPPOINT(+0.0,+0.0,+1.0);
							face3b->normals[1]=new GRPPOINT(+0.0,+0.0,+1.0);
							face3b->normals[2]=new GRPPOINT(+0.0,+0.0,+1.0);
	
							face3b->texturecoordinates[0]=new GRPPOINT(+0.0f						+ textureoffsetx,	tyo + textureoffsety,				+0.0);
							face3b->texturecoordinates[1]=new GRPPOINT(+0.0f						+ textureoffsetx,	tyf + textureoffsety,				+0.0);
							face3b->texturecoordinates[2]=new GRPPOINT(+texfactorx		  + textureoffsetx,	tyf + textureoffsety,				+0.0);

							faces.Add(face3b);
							
	element->GetMesh()->LoadModel(&faces);

	faces.DeleteContents();
	faces.DeleteAll();

	return element;
}

/*-------------------------------------------------------------------
//	 GRPOBJECTBUILDER::CreateBoxRounded
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	13/02/2018 13:29:35
//	@return		GRPELEMENT* : 
//
//	@param		GLFLOAT : width of the widget
//	@param		GLFLOAT : height of the widget 
//	@param		GLFLOAT : radius of the corners
//	@param		GLFLOAT : texture x factor for texture repetition
//	@param		GLFLOAT : texture y factor for texture repetition
//	@param		GLFLOAT : texture x offset for texture movement
//	@param		GLFLOAT : texture y offset for texture movement
//
*//*-----------------------------------------------------------------*/
GRPELEMENT*		GRPOBJECTBUILDER::CreateBoxRounded(GLFLOAT  w, GLFLOAT h, GLFLOAT cornerradius, GLFLOAT texturefactorx, GLFLOAT texturefactory, GLFLOAT textureoffsetx, GLFLOAT textureoffsety)
{
		if (cornerradius <= 0.0f)
				return CreateBox(w, h, texturefactorx, texturefactory, textureoffsetx, textureoffsety);

		GRPSPLINE* spline = GRPOBJECTBUILDER::Get()->CreateBoxSpline(GRPPOINT(0.0, 0.0, 0.0), GRPPOINT(w, h, 0.0), cornerradius);
		GRPSHAPE* shape = new GRPSHAPE();
		shape->AddContour(spline);
		GRPOBJECTBUILDERALGORITHM* algorithm = new GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR(GRPOBJECTBUILDER::Get());
		GRPELEMENT* element = GRPOBJECTBUILDER::Get()->TriangulizeShape(shape, algorithm);		

		GRPVERTEX* vertices = element->GetMesh()->GetVertexData();
		for (XDWORD e = 0; e < element->GetMesh()->GetNvertexData(); e++)
		{								
				GRPVERTEX* v = &(vertices[e]);
				float x =v->position[0];
				float y =v->position[1];				
				v->texturecoordinates[0] = ((x / w)*texturefactorx)+(textureoffsetx);
				v->texturecoordinates[1] = ((y / h)*texturefactory)+(textureoffsety);
		}		
		delete(algorithm);
		delete(shape);
		return element;
}

GRPELEMENT*		GRPOBJECTBUILDER::CreateBorder(GLFLOAT  w, GLFLOAT h, GLFLOAT cornerradius, GLFLOAT borderwidth, XDWORD flags, GLFLOAT texturefactorx, GLFLOAT texturefactory, GLFLOAT textureoffsetx, GLFLOAT textureoffsety)
{		
		GRPSPLINE* spline = GRPOBJECTBUILDER::Get()->CreateBoxSpline(GRPPOINT(0.0, 0.0, 0.0), GRPPOINT(w, h, 0.0), cornerradius, flags);
		GRPSHAPE* shape = new GRPSHAPE();
		shape->AddContour(spline);
		GRPOBJECTBUILDERALGORITHMSTROKE* algorithm = new GRPOBJECTBUILDERALGORITHMSTROKE(GRPOBJECTBUILDER::Get());
																		 algorithm->SetOuterWidth(0.0f);
																		 algorithm->SetInnerWidth(borderwidth);
																		 algorithm->SetJointType(GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE_MITTER);																		 

		GRPELEMENT* element = GRPOBJECTBUILDER::Get()->TriangulizeShape(shape, algorithm);

		GRPVERTEX* vertices = element->GetMesh()->GetVertexData();
		for (XDWORD e = 0; e < element->GetMesh()->GetNvertexData(); e++)
		{
				GRPVERTEX* v = &(vertices[e]);
				float x = v->position[0];
				float y = v->position[1];
				v->texturecoordinates[0] = ((x / w)*texturefactorx) + (textureoffsetx);
				v->texturecoordinates[1] = ((y / h)*texturefactory) + (textureoffsety);
		}
		delete(shape);
		delete(algorithm);
		return element;
}

GRPELEMENT*		GRPOBJECTBUILDER::CreateShadowBorder(GLFLOAT  w, GLFLOAT h, GLFLOAT cornerradius, GLFLOAT borderwidth, XDWORD flags)
{
		GRPSPLINE* spline = GRPOBJECTBUILDER::Get()->CreateBoxSpline(GRPPOINT(0.0, 0.0, 0.0), GRPPOINT(w, h, 0.0), cornerradius, flags);
		GRPSHAPE* shape = new GRPSHAPE();
		shape->AddContour(spline);
		GRPOBJECTBUILDERALGORITHMSTROKE* algorithm = new GRPOBJECTBUILDERALGORITHMSTROKE(GRPOBJECTBUILDER::Get());
		algorithm->SetOuterWidth(0.0);
		algorithm->SetInnerWidth(borderwidth);
		algorithm->SetJointType(GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE_MITTER);

		GRPELEMENT* element = GRPOBJECTBUILDER::Get()->TriangulizeShape(shape, algorithm);
		
		delete(shape);
		delete(algorithm);
		return element;
}


/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateLine
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 14:12:46
//	
//	@return 			GRPSPLINE* : 
//
//  @param				o : 
//  @param				f : 
*/
/*-----------------------------------------------------------------*/
GRPSPLINE* GRPOBJECTBUILDER::CreateLine(GRPPOINT o, GRPPOINT f)
{
	GRPSPLINE* spline = new GRPSPLINE();
	if(!spline) return NULL;

	spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x,o.y,+1.0));
	spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x,f.y,+1.0));

	return spline;
}




/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateArc
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 14:12:53
//	
//	@return 			GRPSPLINE* : 
//
//  @param				o : 
//  @param				f : 
//  @param				c : 
//  @param				segments : 
*/
/*-----------------------------------------------------------------*/

GRPSPLINE* GRPOBJECTBUILDER::CreateArc(GRPPOINT o, GRPPOINT f, GRPPOINT c, XDWORD nsegments)
{
	GRPSPLINE* spline = new GRPSPLINE();
	if(!spline) return NULL;

	spline->nsegments = nsegments;

	float		t = 0.0f;
	float		x = 0.0f;
	float		y = 0.0f;
	float		z = 0.0f;
	XDWORD	i	=	0;

	for(i= 0; i<=nsegments; i++)
		{  				
			x = ((1.0f - t) * (1.0f - t) * o.x) + (2.0f * (1.0f - t) * t * c.x) + (t * t * f.x);
			y = ((1.0f - t) * (1.0f - t) * o.y) + (2.0f * (1.0f - t) * t * c.y) + (t * t * f.y);
			z = ((1.0f - t) * (1.0f - t) * o.z) + (2.0f * (1.0f - t) * t * c.z) + (t * t * f.z);
								
			GRPPOINT point;

			point.x = x;
			point.y = y;
			point.z = z;

			spline->Add(GRPANCHORPOINTTYPE_LINE,point);

			t += 1.0f / nsegments;
		}								
					
	return spline;
}



/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateBox
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 13:52:04
//	
//	@return 			GRPSPLINE* : 
//
//  @param				o : 
//  @param				f : 
*/
/*-----------------------------------------------------------------*/
GRPSPLINE* GRPOBJECTBUILDER::CreateBoxSpline(GRPPOINT o,	GRPPOINT f,float corner, XDWORD flags)
{
		GRPSPLINE* processedspline = NULL;
	GRPSPLINE* spline = new GRPSPLINE();
	if(!spline) 
		return NULL;
	
	if (corner==0.0f)
	{
			switch (flags)
			{
			case GRPBOXSIDEFLAG_LEFT:
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, o.y, +0.0));
					break;
			case GRPBOXSIDEFLAG_BOTTOM:
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, o.y, +0.0));
					break;
			case GRPBOXSIDEFLAG_RIGHT:
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, f.y, +0.0));
					break;
			case GRPBOXSIDEFLAG_TOP:
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, f.y, +0.0));
					break;

					//---- combination

			case GRPBOXSIDEFLAG_LEFT | GRPBOXSIDEFLAG_BOTTOM:
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, o.y, +0.0));					
					break;
			case (GRPBOXSIDEFLAG_LEFT | GRPBOXSIDEFLAG_BOTTOM | GRPBOXSIDEFLAG_RIGHT):
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, f.y, +0.0));
					break;
			case GRPBOXSIDEFLAG_ALL:
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x+(f.x/2.0f), f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x + (f.x / 2.0f), f.y, +0.0));
					break;
			case (GRPBOXSIDEFLAG_BOTTOM | GRPBOXSIDEFLAG_RIGHT | GRPBOXSIDEFLAG_TOP):
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, f.y, +0.0));
					break;
			case (GRPBOXSIDEFLAG_RIGHT | GRPBOXSIDEFLAG_TOP):
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, f.y, +0.0));
					break;
			case (GRPBOXSIDEFLAG_RIGHT | GRPBOXSIDEFLAG_TOP | GRPBOXSIDEFLAG_LEFT):
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, o.y, +0.0));
					break;
			case (GRPBOXSIDEFLAG_TOP | GRPBOXSIDEFLAG_LEFT | GRPBOXSIDEFLAG_BOTTOM):					
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, o.y, +0.0));
					break;
			case (GRPBOXSIDEFLAG_BOTTOM | GRPBOXSIDEFLAG_RIGHT):
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, f.y, +0.0));
					break;
			case (GRPBOXSIDEFLAG_TOP | GRPBOXSIDEFLAG_LEFT):
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, o.y, +0.0));
					break;
			default:
					break;
			}
			processedspline = this->ProcessSplineToLines(spline);
			delete(spline);
	}
	else
	{
			float xcorner=corner;
			float ycorner=corner;
			if (xcorner > ((f.x - o.x) / 2.0))  xcorner = ((f.x - o.x) / 2.0f);
			if (ycorner > ((f.y - o.y) / 2.0))  ycorner = ((f.y - o.y) / 2.0f);
			if (xcorner > ycorner) xcorner = ycorner;
			if (ycorner > xcorner) ycorner = xcorner;

			//--- left			
			if (flags & GRPBOXSIDEFLAG_LEFT)
			{
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, f.y - ycorner, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x, o.y + ycorner, +0.0));
			}
			
					
			//---- bottom-right corner
			if ((flags & GRPBOXSIDEFLAG_LEFT) && (flags & GRPBOXSIDEFLAG_BOTTOM))
			{
					spline->Add(GRPANCHORPOINTTYPE_QUADRIC_BEZIER, GRPPOINT(f.x, o.y + ycorner, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_QUADRIC_BEZIER, GRPPOINT(f.x - xcorner, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_CONTROLPOINT, GRPPOINT(f.x, o.y, +0.0));
			}
			
			//---  bottom
			if (flags & (GRPBOXSIDEFLAG_BOTTOM))
			{
					if (!(flags & (GRPBOXSIDEFLAG_LEFT)))
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x - xcorner, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x + xcorner, o.y, +0.0));
			}
			
			//---  bottom-left corner
			if ((flags & GRPBOXSIDEFLAG_RIGHT) && (flags & GRPBOXSIDEFLAG_BOTTOM))
			{
					spline->Add(GRPANCHORPOINTTYPE_QUADRIC_BEZIER, GRPPOINT(o.x + xcorner, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_QUADRIC_BEZIER, GRPPOINT(o.x, o.y + ycorner, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_CONTROLPOINT, GRPPOINT(o.x, o.y, +0.0));
			}
				
			//---  right
			if (flags & (GRPBOXSIDEFLAG_RIGHT))
			{
					if (!(flags & GRPBOXSIDEFLAG_BOTTOM))
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, o.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x, f.y - xcorner, +0.0));
			}
			
			//---  top-left corner
			if ((flags & GRPBOXSIDEFLAG_RIGHT) && (flags & GRPBOXSIDEFLAG_TOP))
			{
					spline->Add(GRPANCHORPOINTTYPE_QUADRIC_BEZIER, GRPPOINT(o.x, f.y - ycorner, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_QUADRIC_BEZIER, GRPPOINT(o.x + xcorner, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_CONTROLPOINT, GRPPOINT(o.x, f.y, +0.0));
			}
					 
			//---  top
			if (flags & GRPBOXSIDEFLAG_TOP)
			{
					if (!(flags & GRPBOXSIDEFLAG_RIGHT))
							spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(o.x + xcorner, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_LINE, GRPPOINT(f.x - xcorner, f.y, +0.0));
			}
			
			//--- top-right corner
			if ((flags & GRPBOXSIDEFLAG_LEFT) && (flags & GRPBOXSIDEFLAG_TOP))
			{					
					spline->Add(GRPANCHORPOINTTYPE_QUADRIC_BEZIER, GRPPOINT(f.x - xcorner, f.y, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_QUADRIC_BEZIER, GRPPOINT(f.x, f.y - ycorner, +0.0));
					spline->Add(GRPANCHORPOINTTYPE_CONTROLPOINT, GRPPOINT(f.x, f.y, +0.0));
			}
			//*/
			processedspline=this->ProcessSplineToLines(spline);		
			delete(spline);
	}

	return processedspline;
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateWedgeSpline
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/12/2014 16:28:05
//	
//	@return 			GRPSPLINE* : 
//
//  @param				o : 
//  @param				radius : 
//  @param				startingAngle : 
//  @param				endingAngle : 
//  @param				nsegments : 
*/
/*-----------------------------------------------------------------*/
GRPSPLINE* GRPOBJECTBUILDER::CreateWedgeSpline												(GRPPOINT o, GLFLOAT radius, GLFLOAT startingAngle,GLFLOAT endingAngle, XDWORD nsegments)
{
	GRPSPLINE* spline = new GRPSPLINE();

						spline->Add(GRPANCHORPOINTTYPE_LINE,GRPPOINT(o.x,o.y,+0.0));
						spline->Add(GRPANCHORPOINTTYPE_LINE,GRPPOINT(o.x+radius*(float)sin(startingAngle* PI / 180),o.y+radius*(float)cos(startingAngle* PI / 180),+0.0));
						for (XDWORD e=1;e<nsegments+1;e++)
						{
							float anglePerSegment=(endingAngle-startingAngle)/nsegments;
							float angle=startingAngle+(e*anglePerSegment);
							spline->Add(GRPANCHORPOINTTYPE_LINE,GRPPOINT(o.x+radius*(float)sin(angle* PI / 180),o.y+radius*(float)cos(angle* PI / 180),+0.0));
						}
						spline->Add(GRPANCHORPOINTTYPE_LINE,GRPPOINT(o.x,o.y,+0.0));
						
	return spline;
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::TriangulizeObject
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			09/12/2014 16:28:14
//	
//	@return 			GRPELEMENT* : 
//
//  @param				shape : 
//  @param				extrude : 
*/
/*-----------------------------------------------------------------*/
GRPELEMENT*	GRPOBJECTBUILDER::TriangulizeObject											(GRPSHAPE* shape,  GLFLOAT extrude)
		{								
		if(!shape) return NULL;
		if(shape->contours.IsEmpty()) return new GRPELEMENT();
					
						
	GRPOBJECTBUILDER															builder;
	GRPOBJECTBUILDERALGORITHMTRIANGULIZATIONEAR		triangulizationearalgorimth	(&builder);
	GRPOBJECTBUILDERALGORITHMEXTRUDE							extruderalgorimth						(&builder);
	GRPOBJECTBUILDERALGORITHMSTROKE								strokeralgorimth						(&builder);
		

	GRPELEMENT*		element					=	new GRPELEMENT();
	GRPELEMENT*		front						=	NULL;
	GRPELEMENT*		back						=	NULL;
	GRPELEMENT*		extrusion				=	NULL;
	GRPELEMENT*		border					=	NULL;
		
	GRPSHAPE*			mergedglyph;
	mergedglyph		 = builder.MergeInnerSplines(shape);
	shape->Setup();
	mergedglyph->Setup			();
		

			triangulizationearalgorimth.normal	=	GRPVECTOR(0.0, 0.0, +1.0);
			triangulizationearalgorimth.z				=	0.0;

			front = builder.TriangulizeShape( mergedglyph, &triangulizationearalgorimth);
		
	if(extrude != 0.0)
		{
			extruderalgorimth.nsegments	=	1;
			extruderalgorimth.depth			=	-extrude;
			extrusion=  builder.TriangulizeShape( shape,  &extruderalgorimth);			
		}																																	

			triangulizationearalgorimth.normal	=	GRPVECTOR(0.0, 0.0, -1.0);
			triangulizationearalgorimth.z				=	-extrude;
			back = builder.TriangulizeShape( mergedglyph, &triangulizationearalgorimth);
		
/*
	if (attributes.innerborder!=0.0f || attributes.outerborder!=0.0f)
		{
			strokeralgorimth.SetCloseSpline	(true);
			strokeralgorimth.SetInnerWidth	(attributes.innerborder);
			strokeralgorimth.SetOuterWidth	(attributes.outerborder);
			strokeralgorimth.SetExtrude			(attributes.borderextrusion);
			strokeralgorimth.SetJointType		(attributes.jointtype);
			strokeralgorimth.SetPenType			(attributes.pentype);
			strokeralgorimth.SetBevel				(attributes.bevel);
			strokeralgorimth.SetMitterWidth ((attributes.innerborder+attributes.outerborder));

			border = builder.TriangulizeShape(processedglyph, &strokeralgorimth);
		}
		*/
													
	GRPMAPPINGPROJECTIONCUBIC cubic;
																														
	if(front)				element->MergeElement(front);
	if(back)				element->MergeElement(back);
	if(extrusion)   element->MergeElement(extrusion);													
	if(border)			element->MergeElement(border);				
	
	element->Finish();

	element->SetRenderMode(GRPRENDERMODE_TRIANGLES);
	cubic.ProjectMapping(element);
				
																		
	delete mergedglyph;

	if(front)			delete front;
	if(back)			delete back;
	if(extrusion) delete extrusion;
	if(border)		delete border;
							
	return element;
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateEllipseSpline
*/	
/**	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			10/09/2014 14:13:51
//	
//	@return 			GRPSPLINE* : 
//
//  @param				o : 
//  @param				radius : 
//  @param				relation : 
//  @param				2 : 
*/
/*-----------------------------------------------------------------*/
GRPSPLINE*	GRPOBJECTBUILDER::CreateEllipseSpline	(GRPPOINT o, GLFLOAT radius, float relation, XDWORD nsegments)
{
	GRPSPLINE* spline=new GRPSPLINE();
						
	float		x					= 0.0f;
	float		y					= 0.0f;
	float		z					= 0.0f;
	XDWORD	i					=	0;
	GLFLOAT angle			=	0.0f;
	if (nsegments<3) 
		nsegments=3;
	GLFLOAT arcangle	=	(360.0f / nsegments);	


	for(i= 0; i<=nsegments; i++)
		{  																				
			x = (float)sin(angle*(PI/180));
			y = (float)cos(angle*(PI/180));
			z	= o.z;
					
			GRPPOINT p;
			p.x		= relation*radius*(x+o.x);
			p.y		= radius*(y+o.y);
			p.z		= z;

			spline->Add(GRPANCHORPOINTTYPE_LINE,p);
			angle += arcangle;
		}								


	return spline;
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateCircleSpline
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/10/2014 9:47:47
//	
//	@return 			GRPSPLINE* : 
//
//  @param				o : 
//  @param				radius : 
//  @param				nsegments : 
*/
/*-----------------------------------------------------------------*/
GRPSPLINE*		GRPOBJECTBUILDER::CreateCircleSpline	(GRPPOINT o, GLFLOAT radius, XDWORD nsegments)	
{
	return CreateEllipseSpline(o , radius, 1.0f, nsegments);
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CheckPointInsideContour
*/	
/**	
//	Checks if a point is inside a Contour. The Contour MUST be processed and Convex (Curves processed to line strips and loop) 
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/06/2014 13:08:11
//	
//	@return 			bool : 
//
//  @param				testedpoint : 
//  @param				processedspline : 
*/
/*----------------------------------------------------------------*/
bool GRPOBJECTBUILDER::CheckPointInsideContour(GRPPOINT testedpoint, GRPSPLINE* processedspline)
{
	int		isinside = 0;
	XVECTOR<GRPANCHORPOINT*> collides;
	XDWORD	i;
	XDWORD  j;

	for(i=0, j = processedspline->points.GetSize()-1; i < processedspline->points.GetSize()-1; j = i++)
		{
			GRPANCHORPOINT* splinepointi = processedspline->points.Get(i);
			GRPANCHORPOINT* splinepointj = processedspline->points.Get(j);

				if ((testedpoint.y == splinepointi->value.y) && (splinepointi->value.y == splinepointj->value.y)) //es un segmento horizontal y estamos en el espacio
				{
					if ((splinepointi->value.x>splinepointj->value.x && testedpoint.x>splinepointi->value.x) || 
							(splinepointi->value.x<splinepointj->value.x && testedpoint.x<splinepointi->value.x))						
						return true;
				}
		
			bool		isbetweeny		=	 (splinepointi->value.y > testedpoint.y && splinepointj->value.y < testedpoint.y) || 
															 (splinepointi->value.y < testedpoint.y && splinepointj->value.y > testedpoint.y);

				if ((testedpoint.y == splinepointi->value.y) && (splinepointi->value.y!=splinepointj->value.y)) //check con vertice											
					{
						isinside--;
						collides.Add(splinepointi);
						isbetweeny=true;
						continue;
					}
				
			GLFLOAT m = 0.0f;
		
			if((splinepointi->value.y - splinepointj->value.y) != 0.0f)
					    m = (splinepointi->value.x - splinepointj->value.x) / (splinepointi->value.y - splinepointj->value.y);
				else  
							m = 0;		
		
			bool islefttox	= (testedpoint.x < ((m*(testedpoint.y - splinepointj->value.y)) + splinepointj->value.x));
			
			if(isbetweeny && islefttox) 
			{
				collides.Add(splinepointj);
				isinside++;							
			}
 	}

	if ((isinside%2!=0) && isinside!=0)
		return true;
	return false;
}


/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::FastCheckPointInsideContour
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/11/2014 9:33:40
//	
//	@return 			bool : 
//
//  @param				testedpoint : 
//  @param				processedspline : 
*/
/*-----------------------------------------------------------------*/
bool GRPOBJECTBUILDER::FastCheckPointInsideContour(GRPPOINT testedpoint, GRPSPLINE* processedspline)
{
	bool		isinside = false;
	XDWORD	i;
	XDWORD  j;

	 j = processedspline->points.GetSize()-2;
	 GRPANCHORPOINT* splinepointj = processedspline->points.Get(j);

	for(i=0; i < processedspline->points.GetSize()-1; i++)
		{
			GRPANCHORPOINT* splinepointi = processedspline->points.Get(i);
			
			bool		isbetweeny		=	 (splinepointi->value.y > testedpoint.y && splinepointj->value.y < testedpoint.y) || 
															 (splinepointi->value.y < testedpoint.y && splinepointj->value.y > testedpoint.y);
			GLFLOAT m = 0.0f;
		
			if((splinepointi->value.y - splinepointj->value.y) != 0.0f)
					    m = (splinepointi->value.x - splinepointj->value.x) / (splinepointi->value.y - splinepointj->value.y);
				else  
							m = 1;		
		
			bool islefttox	= testedpoint.x <= ((m*(testedpoint.y - splinepointj->value.y)) + splinepointj->value.x);
			
			if(isbetweeny && islefttox) 
				isinside = !isinside;				

			splinepointj=splinepointi;
 	}

	return isinside;
}


/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CheckPointInsideContour
*/	
/**	
//	Checks if a point is inside a Contour. The Contour MUST be rasterized and Convex (Curves processed to line strips and loop) 
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			30/05/2014 17:45:17
//	
//	@return 			bool : true if point inside contour
//
//  @param				testedpoint :					GRPPOINT to check
//  @param				contour			:		XVECTOR<GRPPOINT*> contour
*/
/*-----------------------------------------------------------------*/
bool GRPOBJECTBUILDER::CheckPointInsideContour(GRPPOINT testedpoint, XVECTOR<GRPPOINT*>* contour)
{
	bool	 isinside = false;
	XDWORD i, j;

	XDWORD cs=contour->GetSize();
	for(i = 0, j = cs-2; i < cs-1; j = i++)
		{
			GRPPOINT* splinepointi = contour->FastGet(i);
			GRPPOINT* splinepointj = contour->FastGet(j);

			bool		isbetweeny = ((splinepointi->y > testedpoint.y) != (splinepointj->y > testedpoint.y));		
			GLFLOAT m          = (splinepointi->x - splinepointj->x) / (splinepointi->y - splinepointj->y);
			bool		islefttox	 = testedpoint.x <= ((m*(testedpoint.x - splinepointj->x)) + splinepointj->x);
			
			if(isbetweeny && islefttox) isinside = !isinside;				
		}

	return isinside;
}





/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::IsInnerContour
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 16:19:05
//	
//	@return 			bool : 
//
//  @param				index : 
//  @param				shape : 
*/
/*-----------------------------------------------------------------*/
/*
int GRPOBJECTBUILDER::IsInnerContour(int index, GRPSHAPE* shape)
{
	if(!shape) return false;

	GRPSPLINE* spline = shape->contours.Get(index);
	if(!spline) return false;

	int	 hits	=	0;
	bool flag = false;

	XVECTOR<int> contours;

	for(int i=0; i<(int)spline->points.GetSize(); i++)
		{		
			hits=0;
			for(int s=0; s<(int)shape->contours.GetSize(); s++)
				{		
					if(s==index) continue;
					if (shape->contours.Get(s)->isInside(spline->points.Get(i)))
						return s;
				}
		}

	return -1;//(hits!=0);
}
*/

int GRPOBJECTBUILDER::IsInnerContour(int index, GRPSHAPE* shape)
{
	if(!shape) return false;

	GRPSPLINE* spline = shape->contours.Get(index);
	if(!spline) return false;

	int	 hits	=	0;
	bool flag = false;

	XVECTOR<int> contours;

	int cs=shape->contours.GetSize();
			for(int s=0; s<(int)cs; s++)
				{	
					if(s==index) continue;

						bool inside=false;
						for(int i=0; i<(int)spline->points.GetSize(); i++)
						{												
							if (shape->contours.Get(s)->isInside(spline->points.FastGet(i)))
								{								
									inside=true;																
									break;
								}
						}
						if (inside)
						contours.Add(s);
			}
	if (contours.GetSize()%2!=0)
		return contours.GetLast();

	return -1;//(hits!=0);
}




/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::FindNearAnchor
*/	
/**	
//	
//	Finds nearer (non trigonometric distance) anchor to point
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/06/2014 11:17:29
//	
//	@return 			GRPANCHORPOINT* : found anchor
//
//  @param				spline					: GRPSPLINE* Spline to search
//  @param				reference				: near point  
*/
/*-----------------------------------------------------------------*/
GRPANCHORPOINT*	GRPOBJECTBUILDER::FindNearerAnchor(GRPSPLINE* spline, GRPSPLINE* reference, GRPANCHORPOINT** nearerOut)
{
	float nearerDistance=0.0f;
	GRPANCHORPOINT* nearer=FindNearAnchor(spline,reference->points.FastGet(0),&nearerDistance);
	*nearerOut=nearer;

	XDWORD ps=reference->points.GetSize();
	for (XDWORD e=0;e<ps;e++)
	{
	  float f=0;	
		GRPANCHORPOINT*anchor=FindNearAnchor(spline,reference->points.FastGet(e),&f);

		if (f<=nearerDistance)
		{
			nearer=anchor;
			nearerDistance=f;
			*nearerOut=reference->points.FastGet(e);
		}
	}
	return nearer;
}

void	GRPOBJECTBUILDER::FindNearerAnchor(GRPSPLINE* spline, GRPSPLINE* reference, int* insiderIndex,int* contourIndex)
{
	float nearerDistance=0.0f;
	GRPANCHORPOINT* nearer=FindNearAnchor(spline,reference->points.FastGet(0),&nearerDistance);

	XDWORD rs=reference->points.GetSize();
	for (XDWORD e=0;e<rs;e++)
	{
	  float f=0;			
		FindNearAnchor(spline,reference->points.FastGet(e),&f,contourIndex);
		if (f<=nearerDistance )
		{			
			nearerDistance=f;							
			*insiderIndex=e;
		}
	}	
}


/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::FindNearAnchor
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/10/2014 9:50:08
//	
//	@return 			GRPANCHORPOINT* : 
//
//  @param				spline : 
//  @param				reference : 
//  @param				f : 
*/
/*-----------------------------------------------------------------*/
GRPANCHORPOINT* GRPOBJECTBUILDER::FindNearAnchor(GRPSPLINE* spline, GRPANCHORPOINT* reference,float* f)
{		
	if(!spline)		 return NULL;
	if(!reference) return NULL;

	GRPANCHORPOINT* nearerpoint = spline->points.GetLast();
	if(!nearerpoint) return NULL;

	float lesserDistance= FastGetDistanceBetweenPoints(&nearerpoint->value, &reference->value);

	XDWORD ss=spline->points.GetSize();
	for(XDWORD e=0; e<ss; e++)
		{		
			GRPANCHORPOINT* ap = spline->points.FastGet(e);

			float fdistance = FastGetDistanceBetweenPoints(&(spline->points.FastGet(e)->value), &reference->value);
			if(fdistance < lesserDistance)
				{
					lesserDistance = fdistance;
					nearerpoint=spline->points.FastGet(e);
				}
		}
	*f=lesserDistance;
	return nearerpoint;
}


void GRPOBJECTBUILDER::FindNearAnchor(GRPSPLINE* spline, GRPANCHORPOINT* reference,float* f, int* index)
{		
	*index=-1;

	if(!spline)		 return;
	if(!reference) return;

	GRPANCHORPOINT* nearerpoint = spline->points.GetLast();
	if(!nearerpoint) return;

	float lesserDistance= FastGetDistanceBetweenPoints(&nearerpoint->value, &reference->value);

	XDWORD ss=spline->points.GetSize();
	for(XDWORD e=0; e<ss; e++)
		{		
			GRPANCHORPOINT* ap = spline->points.Get(e);
			float fdistance = FastGetDistanceBetweenPoints(&(spline->points.Get(e)->value), &reference->value);
			if(fdistance <= lesserDistance)
				{
					lesserDistance = fdistance;
					nearerpoint=spline->points.Get(e);
				}
		}
	*f=lesserDistance;	
}


/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::GetBoundingVertexesFromPointArray
*/	
/**	
//	
//	Gets Max Left Right Top and Bottom from Shape
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			05/06/2014 13:48:49
//	
//  @param				vertex	: XVECTOR<GRPPOINT*>* List of GRPPOINT to check
//  @param				_top		: higher point
//  @param				_left		: leftmost point
//  @param				_bottom : lower point
//  @param				_right	: rightmost point
//  @param				_near		: less z point
//  @param				_far		: most z point
*/
/*-----------------------------------------------------------------*/
void GRPOBJECTBUILDER::GetBoundingVertexesFromPointArray(XVECTOR<GRPPOINT*>* vertex, GRPPOINT** _top, GRPPOINT** _left, GRPPOINT** _bottom, GRPPOINT** _right) //, GRPPOINT** _near,GRPPOINT** _far)
{		
	XDWORD vs=vertex->GetSize();
	if (vs==0) return;

		*_top			= vertex->FastGet(0);
		*_bottom	= *_top;
		*_left		= *_top;
		*_right		= *_top;
		//*_near		= vertex->Get(0);
		//*_far			= vertex->Get(0);
		
		
	for(XDWORD e=0; e<vs; e++)
		{
			GRPPOINT* point = vertex->FastGet(e);

					if (point->x < (*_left)->x)		*_left		= point;
					if (point->y < (*_bottom)->y)	*_bottom	=	point;
					if (point->x > (*_right)->x)	*_right		=	point;
					if (point->y > (*_top)->y	)		*_top			=	point;
				//	if (point->z > (*_near)->z)		*_near		=	point;
				//	if (point->z<(*_far)->z	)			*_far			= point;
		}		
}



	

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::GetBoundingVertexesFromSpline
*/	
/**	
//	Gets top,bottom,left,right,near and far AnchorPoints from Spline
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 17:07:27
//	
//  @param				vertex : 
//  @param				_top : 
//  @param				_left : 
//  @param				_bottom : 
//  @param				_right : 
//  @param				_near : 
//  @param				_far : 
*/
/*-----------------------------------------------------------------*/
void GRPOBJECTBUILDER::GetBoundingVertexesFromSpline(GRPSPLINE* vertex, GRPANCHORPOINT** _top, GRPANCHORPOINT** _left, GRPANCHORPOINT** _bottom, GRPANCHORPOINT** _right, GRPANCHORPOINT** _near, GRPANCHORPOINT** _far)
{		
	if(_top)		*_top			= vertex->points.FastGet(0);
	if(_bottom)	*_bottom	= vertex->points.FastGet(0);
	if(_left)		*_left		= vertex->points.FastGet(0);
	if(_right)	*_right		= vertex->points.FastGet(0);
	if(_near)		*_near		= vertex->points.FastGet(0);
	if(_far)		*_far			= vertex->points.FastGet(0);
		
	XDWORD vs=vertex->points.GetSize();
	for(XDWORD e=0; e<vs; e++)
		{
			GRPANCHORPOINT* point = vertex->points.FastGet(e);

			if(_left)		
				{
					if (point->value.x < (*_left)->value.x)				*_left		 = point;
				}

			if(_bottom)	
				{
					if(point->value.y	<	(*_bottom)->value.y)			*_bottom	= point;
				}

			if(_right)	
				{
					if (point->value.x	>	(*_right)->value.x	)		*_right		= point;
				}

			if(_top)		
				{
					if (point->value.y	>	(*_top)->value.y	)			*_top			=	point;
				}

			if(_near)		
				{
					if (point->value.z	>	(*_near)->value.z)			*_near		= point;
				}

			if(_far)		
				{					
					if (point->value.z	<	(*_far)->value.z	)			*_far			=	point;
				}
		}		
}



/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::IsPointOnLine
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/06/2014 15:58:10
//	
//	@return 			bool : 
//
//  @param				testedpoint : 
//  @param				linePointA : 
//  @param				linePointB : 
*/
/*-----------------------------------------------------------------*/
/*

bool GRPOBJECTBUILDER::IsPointOnLine(GRPPOINT* testedpoint, GRPPOINT* linePointA, GRPPOINT* linePointB) 
{
	
  float a = (linePointB->y - linePointA->y) / (linePointB->x - linePointB->x);
  float b = linePointA->y - a * linePointA->x;
  
	if(fabs(testedpoint->y - (a* testedpoint->x+b)) < EPSILON)
		{
      return true;
		}

   return false;
	

	return (fabs(testedpoint->y - (((linePointB->y - linePointA->y) / (linePointB->x - linePointB->x))* testedpoint->x+(linePointA->y - ((linePointB->y - linePointA->y) / (linePointB->x - linePointB->x)) * linePointA->x))) < EPSILON);
}
*/


/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::SignOfBaryCentricCoordenate
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/06/2014 15:58:18
//	
//	@return 			float : 
//
//  @param				testedpoint : 
//  @param				trianglesidevectorA : 
//  @param				trianglesidevectorB : 
*/
/*-----------------------------------------------------------------*/
float GRPOBJECTBUILDER::SignOfBarycentricCoordenate(GRPPOINT testedpoint, GRPPOINT trianglesidevectorA, GRPPOINT trianglesidevectorB)
{
  return (testedpoint.x - trianglesidevectorA.x) * (trianglesidevectorA.y - trianglesidevectorA.y) - (trianglesidevectorA.x - trianglesidevectorA.x) * (testedpoint.y - trianglesidevectorA.y);
}




/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CheckPointInsideTriangle
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/06/2014 15:58:27
//	
//	@return 			bool : 
//
//  @param				testedpoint : 
//  @param				trianglepointA : 
//  @param				trianglepointB : 
//  @param				trianglepointC : 
*/
/*-----------------------------------------------------------------*/
bool nIsPointOnLine(GRPPOINT*linePointA,GRPPOINT*linePointB,GRPPOINT*testedpoint) 
{
		float slope, intercept;
    float x1=linePointA->x;
		float y1=linePointA->y;
		float x2=linePointB->x;
		float y2=linePointB->y;

    float px=testedpoint->x;
		float py=testedpoint->y;

    float left, top, right, bottom; // Bounding Box For Line Segment
    float dx, dy;

    dx = x2 - x1;
		if (dx<=EPSILON)
		{
			if (((py>=y1 && py<=y2) || (py<=y1 && py>=y2)) && (px<=x2+EPSILON && px>=x1+EPSILON))		return true;
			return false;
		}
    dy = y2 - y1;
 
    slope = dy / dx;    
    intercept = y1 - slope * x1; // which is same as y2 - slope * x2
 		
    // For Bounding Box
    if(x1 < x2)
    {
        left = x1;
        right = x2;
    }
    else
    {
        left = x2;
        right = x1;
    }
    if(y1 < y2)
    {
        top = y1;
        bottom = y2;
    }
    else
    {
        top = y1;
        bottom = y2;
    } 
   	
    if( (slope * px + intercept) > (py - EPSILON) &&
        (slope * px + intercept) < (py + EPSILON))
    {
        if( px >= left && px <= right && 
            py >= top && py <= bottom )        
						return true;       
    }
    
    return false;
}


bool GRPOBJECTBUILDER::CheckPointInsideTriangle(GRPPOINT* testedpoint, GRPPOINT* trianglepointA, GRPPOINT* trianglepointB, GRPPOINT* trianglepointC)
{

	
	if ((testedpoint->x == trianglepointA->x && testedpoint->y==trianglepointA->y && testedpoint->z==trianglepointA->z) || 
			(testedpoint->x == trianglepointB->x && testedpoint->y==trianglepointB->y && testedpoint->z==trianglepointB->z) || 
			(testedpoint->x == trianglepointC->x && testedpoint->y==trianglepointC->y && testedpoint->z==trianglepointC->z))   
			return false;


	GRPPOINT *A=trianglepointA;
	GRPPOINT *B=trianglepointB;
	GRPPOINT *C=trianglepointC;
	GRPPOINT *P=testedpoint;
	
	if(nIsPointOnLine(trianglepointA, trianglepointB, testedpoint))  
		return true;
	if(nIsPointOnLine(trianglepointA, trianglepointC, testedpoint))  
		return true;
	if(nIsPointOnLine(trianglepointB, trianglepointC, testedpoint))  
		return true;
		

	// Compute vectors        
	v0.vector[0] = C->x - A->x;	
	v0.vector[1] = C->y - A->y;	
	v0.vector[2] = C->z - A->z;		
	 
	v1.vector[0] = B->x - A->x;	
	v1.vector[1] = B->y - A->y;	
	v1.vector[2] = B->z - A->z;		
	
	v2.vector[0] = P->x - A->x;	
	v2.vector[1] = P->y - A->y;	
	v2.vector[2] = P->z - A->z;		 

	if((v0.GetLength2f() <= EPSILON) || (v1.GetLength2f() <= EPSILON) || (v2.GetLength2f() <= EPSILON)) return false;

	// Compute dot products
	const float dot00 = GRPVECTOR::DotProduct(&v0, &v0);
	const float dot01 = GRPVECTOR::DotProduct(&v0, &v1);
	const float dot02 = GRPVECTOR::DotProduct(&v0, &v2);
	const float dot11 = GRPVECTOR::DotProduct(&v1, &v1);
 
	double Denom			=(dot00 * dot11) - (dot01 * dot01);
	if (Denom<EPSILON ) 
		 return false;

	float dot12 = GRPVECTOR::DotProduct(&v1, &v2);

	const double invDenom		= 1.0f / Denom;
	const double u					= ((dot11 * dot02) - (dot01 * dot12)) * invDenom;

	if (u < 0.0f) 
		return false;

	const double v					= ((dot00 * dot12) - (dot01 * dot02)) * invDenom;
	
	return ((v >= 0.0f) && ((u + v) < 1.0f));
}



/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::IsCWTriangle
*/	
/**	
//	Checks if Triangle is Clockwise
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/06/2014 12:37:39
//	 
//	@return 			bool : 
//
//  @param				a : 
//  @param				b : 
//  @param				c : 
*/
/*-----------------------------------------------------------------*/
bool GRPOBJECTBUILDER::IsClockWiseTriangle(GRPPOINT* a, GRPPOINT* b, GRPPOINT* c)
{
	GRPVECTOR ab;
	GRPVECTOR bc;

	ab.vector[0]=a->x-b->x;
	ab.vector[1]=a->y-b->y;
	ab.vector[2]=a->z-b->z;								
							
	bc.vector[0]=b->x-c->x;
	bc.vector[1]=b->y-c->y;
	bc.vector[2]=b->z-c->z;

	ab.CrossProduct(&ab,&bc);									
	GLFLOAT normal = ab.DotProduct(&ab,&NearVector);

	return (normal>=0);
}




/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::GetAdyacentPoints
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			26/06/2014 10:40:59
//	
//  @param				vertex : 
//  @param				a : 
//  @param				b : 
//  @param				c : 
*/
/*-----------------------------------------------------------------*/
void GRPOBJECTBUILDER::GetAdyacentPoints(XVECTOR<GRPPOINT*>* vertex,GRPPOINT* a,GRPPOINT** b,GRPPOINT** c,int* aindex, int *bindex, int *cindex)
{
	int ia	= vertex->Find(a);
	int n		= vertex->GetSize();

	*aindex=n;
	*bindex=(ia+1)		%n;
	*cindex=(n+ia-1)	%n;

	*b = vertex->FastGet(*bindex);	
	*c = vertex->FastGet(*cindex);
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::ProcessSplineToLines
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 15:57:07
//	
//	@return 			GRPSPLINE* : 
//
//  @param				splinetoprocess : 
//  @param				segments : 
*/
/*-----------------------------------------------------------------*/
GRPSPLINE* GRPOBJECTBUILDER::ProcessSplineToLines(GRPSPLINE* splinetoprocess, XDWORD nsegments)
{
	GRPSPLINE* splineresult = new GRPSPLINE();
	if(!splineresult) return NULL;

	splineresult->origin   = splinetoprocess->origin;

	if(nsegments)
				 splineresult->nsegments = nsegments;
	  else splineresult->nsegments = splinetoprocess->nsegments; 
			
	GLFLOAT maxX = 0.0f;
	GLFLOAT maxY = 0.0f;
	GLFLOAT maxZ = 0.0f;
		
	GLFLOAT minX = 0.0f;
	GLFLOAT minY = 0.0f;
	GLFLOAT minZ = 0.0f;

	int segments=0;
			
	XDWORD	n			 = 0;

	float		x = 0.0f;
	float		y = 0.0f;
	float		z = 0.0f;
	float		w = 1.0f;
	XDWORD	i = 0;

	XDWORD np=splinetoprocess->points.GetSize();
	for(XDWORD e=0;e<np;e++)
	{
			GRPANCHORPOINT* anchorpoint = splinetoprocess->points.Get(e);

			switch(anchorpoint->type)										
				{											
					case GRPANCHORPOINTTYPE_LINE						:	splineresult->points.Add(new GRPANCHORPOINT(GRPANCHORPOINTTYPE_LINE,*anchorpoint));											
																										break;
										
					case GRPANCHORPOINTTYPE_QUADRIC_BEZIER	:	{ GRPANCHORPOINT* pf			= splinetoprocess->points.FastGet(e+1);
																											GRPANCHORPOINT* control	=	splinetoprocess->points.FastGet(e+2);

																											if((!pf)  || (!control)) 
																												return NULL;

																											if(pf->type == GRPANCHORPOINTTYPE_QUADRIC_BEZIER && control->type == GRPANCHORPOINTTYPE_CONTROLPOINT)
																												{
																													GRPPOINT o;				o.x = anchorpoint->value.x;					o.y = anchorpoint->value.y;					o.z = anchorpoint->value.z;
																													GRPPOINT f;				f.x = pf->value.x;										f.y = pf->value.y;										f.z = pf->value.z;
																													GRPPOINT b;				b.x = control->value.x;							b.y = control->value.y;							b.z = control->value.z;

																													GRPSPLINE* beziercurve = CreateArc(o, f, b,splineresult->nsegments);
												
																													XDWORD size = beziercurve->points.GetSize();
																													for(XDWORD i=1; i<size; i++)
																														{
																															GRPANCHORPOINT* point = beziercurve->points.FastGet(i);
																															x = point->value.x;
																															y = point->value.y;
																															z = point->value.z;
																														
																															GRPANCHORPOINT* anchorpointresult = new GRPANCHORPOINT(GRPANCHORPOINTTYPE_LINE, (*point));
																																splineresult->points.Add(anchorpointresult);																					
																														}
 																													
																													delete(beziercurve);
																													beziercurve=NULL;
																													e+=2;
																												}																																						
																										}	
																										break;		
					case GRPANCHORPOINTTYPE_CUBIC_BEZIER:
																										{
																										GRPSPLINE tmp;
																										GRPANCHORPOINT* pf=NULL;

																										GRPANCHORPOINT* a=splinetoprocess->points.FastGet(e);			tmp.Add(a->type,GRPPOINT(a->value.x,a->value.y,a->value.z));
																										GRPANCHORPOINT* b=splinetoprocess->points.FastGet(e+1);		tmp.Add(b->type,GRPPOINT(b->value.x,b->value.y,b->value.z));
																										GRPANCHORPOINT* c=splinetoprocess->points.FastGet(e+2);		tmp.Add(c->type,GRPPOINT(c->value.x,c->value.y,c->value.z));
																										GRPANCHORPOINT* d=splinetoprocess->points.FastGet(e+3);		tmp.Add(d->type,GRPPOINT(d->value.x,d->value.y,d->value.z));
																										 e+=3;
																										GRPSPLINE * solution=this->ResolveCubicBezier(&tmp,splineresult->nsegments);

																										const XDWORD ns=solution->points.GetSize()-1;
																										for (XDWORD i=0;i<ns;i++)
																											splineresult->Add(solution->points.FastGet(i)->type,	solution->points.FastGet(i)->value);

																										delete(solution);
																										break;
																										}
					default: break;
				}
		}
				

	return splineresult;
}




/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::MergeInnerSplines
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 16:23:41
//	
//	@return 			GRPSHAPE* : 
//
//  @param				shape : 
*/
/*-----------------------------------------------------------------*/

GRPSHAPE* GRPOBJECTBUILDER::MergeInnerSplines(GRPSHAPE* shape)
{	
	if (shape->contours.GetSize()<=1) 
		return shape->CreateCopy();

	XVECTOR<GRPSPLINE*> contours;	
	XVECTOR<GRPSEAM*>		seams;

	//------------------------------------------- precacheamos los anclajes de las siluetas internas
	int ncontours=shape->contours.GetSize();
	for (int e=0;e<ncontours;e++)
	{
		GRPSPLINE* currentContour=shape->contours.FastGet(e);

		int Context=this->IsInnerContour(e,shape);
		
		if (Context!=-1)		
		{
	
			//----------------------- preparamos el seam
			GRPSEAM*	seam	=new GRPSEAM();
								seam->ContourIndex=Context;
								seam->Contour	=shape->contours.FastGet(Context);
								seam->InnerIndex=e;
								seam->Inner		=shape->contours.FastGet(e);

								//------- averiguamos el seam
								GRPANCHORPOINT* seamOutAnchor=NULL;
								GRPANCHORPOINT* seamInAnchor=this->FindNearerAnchor(seam->Inner,	seam->Contour,&seamOutAnchor);								
																seam->InnerAnchor=seamInAnchor;
																seam->InnerSeamIndex=seam->Inner->points.Find(seamInAnchor);
																seam->OuterAnchor=seamOutAnchor;
																seam->OutSeamIndex=seam->Contour->points.Find(seamOutAnchor);
																seam->m=(seamOutAnchor->value.x-seamInAnchor->value.x)/(seamOutAnchor->value.y-seamInAnchor->value.y);
								seams.Add(seam);							
		}
		else			
			contours.Add(currentContour);
	}

		GRPSHAPE* result=new GRPSHAPE();

	if (contours.GetSize()==0)
		return result;

	//------------------------------------------- ordenamos los seams por indice de externo
	int i=0;
	XDWORD s=0;
	int nseams=int(seams.GetSize());

				for ( i=0;i<nseams;i++) //algoritmo de burbuja
				for (int e=0;e<nseams-1;e++)
				{
					if (seams.FastGet(e)->ContourIndex > seams.FastGet(e+1)->ContourIndex)
						seams.Swap(e,e+1);
				}
				
				for ( i=0;i<nseams;i++) //algoritmo de burbuja
				for (int e=0;e<nseams-1;e++)
				{
					if (seams.FastGet(e)->ContourIndex == seams.FastGet(e+1)->ContourIndex)
					if (seams.FastGet(e)->OutSeamIndex > seams.FastGet(e+1)->OutSeamIndex)
						seams.Swap(e,e+1);					
				}

				for ( i=0;i<nseams;i++) //algoritmo de burbuja
				for (int e=0;e<nseams-1;e++)
				{
					if (seams.FastGet(e)->ContourIndex != seams.FastGet(e+1)->ContourIndex)
						continue;

					if (seams.FastGet(e)->OutSeamIndex == seams.FastGet(e+1)->OutSeamIndex && seams.FastGet(e)->OutSeamIndex > seams.FastGet(e+1)->OutSeamIndex)		
						seams.Swap(e,e+1);
					else
					if (seams.FastGet(e)->OutSeamIndex == seams.FastGet(e+1)->OutSeamIndex && seams.FastGet(e)->m > seams.FastGet(e+1)->m)
							seams.Swap(e,e+1);
				}

	//------------------------------------------- recorremos el contorno interior buscando anclajes	
	

	

	XDWORD currentContour=0;

	i=0;
		while (true)
		{
			GRPSPLINE* contour=contours.Get(currentContour);

			int stop=-1;
			GRPSEAM* currentSeam=NULL;
			
			//----------------- estamos pendientes de un corte
			if (seams.GetSize()>0 && s<seams.GetSize())
			{
				GRPSEAM* seam=seams.Get(s);
				if (seam->Contour==contour)
				{
				currentSeam=seams.Get(s);
				stop=currentSeam->OutSeamIndex;
				}				
			}
			
			//---------------- no hay corte, o no es válido, continuamos hasta el final
			if (stop==-1)
					stop=contour->points.GetSize();

			//----------------- copiamos hasta el corte/fin
				while (i<stop)
				{
					result->Add(contour->points.Get(i)->type,contour->points.Get(i)->value);
					i++;
				}

				//-------------------- hemos terminado al encontrar un corte?
				if (stop<int(contour->points.GetSize()) && currentSeam!=NULL)
				{
							//hemos parado antes de poner el punto de seam, asi que garantizamos ponerlo
							result->Add(contour->points.Get(i)->type,contour->points.Get(i)->value);
					
							//copiamos el interno
								for (XDWORD e=currentSeam->InnerSeamIndex;e<currentSeam->Inner->points.GetSize();e++)					
									result->Add(currentSeam->Inner->points.Get(e)->type,currentSeam->Inner->points.Get(e)->value); 
								for (int e=0;e<currentSeam->InnerSeamIndex;e++)					
									result->Add(currentSeam->Inner->points.Get(e)->type,currentSeam->Inner->points.Get(e)->value);

							//terminamos el interno en seam
							result->Add(currentSeam->InnerAnchor->type,currentSeam->InnerAnchor->value); 	
							
				}	
				else //-------------------- hemos terminado la figura
				{
					i=0;
			
					currentContour++; //siguiente contorno
				
					if (currentContour>=contours.GetSize())  //si no hay hemos terminado
						break;

					for (XDWORD e=0;e<result->contours.GetLast()->points.GetSize();e++)
					{
						result->contours.GetLast()->points.Get(e)->value.index=e;
					}
								
					result->AddContour(new GRPSPLINE());
					
						continue; //evitamos el avance de seam

				}
				//-------------------- avanzamos seam, lo que puede permitirnos terminar el contorno
				if (stop!=-1)
						s++;
			}

		//ajustamos los indices
		if (result->contours.GetSize()>0)
		for (XDWORD e=0;e<result->contours.Get(0)->points.GetSize();e++)
		{
			result->contours.Get(0)->points.Get(e)->value.index=e;
		}	

		seams.DeleteContents();
		seams.DeleteAll();
		return result;
}

	/*-------------------------------------------------------------------
	//	GRPOBJECTBUILDER::WeldVertexes
	*/	
	/**	
	//	
	//	
	//	
	//	@author				Diego Martinez Ruiz de Gaona
	//	@version			30/12/2014 16:21:13
	//	
	//	@return 			GRPSHAPE* : 
	//
	//  @param				shape : 
	//  @param				treshold : 
	*/
	/*-----------------------------------------------------------------*/
GRPSHAPE*	 GRPOBJECTBUILDER::WeldVertexes													(GRPSHAPE* shape,float treshold)
{

	return NULL;
}
GRPELEMENT*	 GRPOBJECTBUILDER::WeldVertexes													(GRPELEMENT* element,float treshold)
{
	XVECTOR<int> removableVertexes;

	for (XDWORD e=0;e<element->GetMesh()->GetNVertex();e++)
	{
		GRPVERTEX* a=&(element->GetMesh()->GetVertexData()[e]);
		for (XDWORD i=0;i<element->GetMesh()->GetNVertex();i++)
		{
			if (i==e) 
				continue;

			GRPVERTEX* b=&(element->GetMesh()->GetVertexData()[i]);

			GRPPOINT pa(a->position[0],a->position[1],a->position[2]);
			GRPPOINT pb(b->position[0],b->position[1],b->position[2]);
			float distance=this->FastGetDistanceBetweenPoints(&pa,&pb);
			if (distance<treshold)			
			{	
				XDEBUG_PRINTCOLOR(0,__L(" Vertex %d and %d distance %f"),e,i,distance);		
			}
		}
	}


	return NULL;
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::ProcessShapeToLines
*/	
/**	
//	Process Anchors from Shape
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 17:25:20
//	
//	@return 			GRPSHAPE* : 
//
//  @param				shape			: 
//  @param				segments	:
*/
/*-----------------------------------------------------------------*/
GRPSHAPE* GRPOBJECTBUILDER::ProcessShapeToLines(GRPSHAPE* shapetoprocess, XDWORD nsegments)
{	
	GRPSHAPE* shaperesult = new GRPSHAPE();

	for(XDWORD e=0; e<shapetoprocess->contours.GetSize(); e++)
		{
			GRPSPLINE* spline= ProcessSplineToLines(shapetoprocess->contours.Get(e), nsegments);	
			if(spline) 
				shaperesult->AddContour(spline);
		}
	
	return shaperesult;
}


/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::AddShapeAsElement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/06/2014 9:50:26
//	
//	@return 			bool : 
//
//  @param				object : 
//  @param				programID : 
//  @param				shape : 
//  @param				material : 
*/
/*-----------------------------------------------------------------*/
bool GRPOBJECTBUILDER::AddShapeAsElement(GRPOBJECT* object, GRPSHAPE* shape)	
{		
	GLFLOAT			maxX			=	0.0f;
	GLFLOAT			maxY			=	0.0f;
	GLFLOAT			maxZ			=	0.0f;
	
	GLFLOAT			minX			=	0.0f;
	GLFLOAT			minY			=	0.0f;
	GLFLOAT			minZ			=	0.0f;

	int					nsegments	=	0;

	for(XDWORD c=0;c< shape->contours.GetSize(); c++)
		{
			for(XDWORD e=0; e<shape->contours.Get(c)->points.GetSize(); e++)
				{
					GRPSPLINE*			spline				= shape->contours.Get(c);					
					GRPANCHORPOINT* point					=	shape->contours.Get(c)->points.Get(e);
					
					if(point->type!=GRPANCHORPOINTTYPE_CONTROLPOINT)
						{
							nsegments++;
					
							if(point->value.x	<	minX)		minX = point->value.x;
							if(point->value.y	<	minY)		minY = point->value.y;
							if(point->value.z	<	minZ)		minZ = point->value.z;

							if(point->value.x	>	maxX)		maxX = point->value.x;
							if(point->value.y	>	maxY)		maxY = point->value.y;
							if(point->value.z	>	maxZ)		maxZ = point->value.z;
						}
					 else
						{
							if(spline)  nsegments += spline->nsegments;
						}
				}
		}
		
	GLFLOAT* vertex					= new GLFLOAT[nsegments*3];	if (!vertex)				return false;	
	GLFLOAT* normals				= new GLFLOAT[nsegments*3];	if (!normals)				return false;
	GLFLOAT* texturecoords	= new GLFLOAT[nsegments*2];	if (!texturecoords) return false;

	
	GLFLOAT width						=	maxX-minX;
	GLFLOAT height					=	maxY-minY;
	GLFLOAT depth						=	maxZ-minZ;		
	
	XDWORD	n								=	0;
	float		x								= 0.0f;
	float		y								= 0.0f;
	float		z								= 0.0f;
	float		w								= 1.0f;
	XDWORD	i								=	0;

	for(XDWORD c=0; c<shape->contours.GetSize();c++)
		{
			for(XDWORD e=0; e<shape->contours.Get(c)->points.GetSize(); e++)
				{
					GRPANCHORPOINT* ap = shape->contours.Get(c)->points.Get(e);

					switch(ap->type)										
						{											
							case GRPANCHORPOINTTYPE_LINE						:	x = ap->value.x;
																												y = ap->value.y;
																												z = ap->value.z;
																												w = ap->value.w;

																												vertex[(n*3)+0]					=	x;
																												vertex[(n*3)+1]					=	y;
																												vertex[(n*3)+2]					=	z;

																												normals[(n*3)+0]				=	0.0f;
																												normals[(n*3)+1]				=	0.0f;
																												normals[(n*3)+2]				=	1.0f;

																												texturecoords[(n*2)+0]	=	(x-minX)/width;
																												texturecoords[(n*2)+1]	=	(y-minY)/height;		
																												n++;
																												break;
										
							case GRPANCHORPOINTTYPE_QUADRIC_BEZIER	:	{ GRPANCHORPOINT* pf			=	shape->contours.Get(c)->points.Get(e+1);
																													GRPANCHORPOINT* control	=	shape->contours.Get(c)->points.Get(e+2);

																													if(pf->type==GRPANCHORPOINTTYPE_QUADRIC_BEZIER && control->type==GRPANCHORPOINTTYPE_CONTROLPOINT)
																														{
																															GRPPOINT o;				o.x	=	ap->value.x;				o.y = ap->value.y;				o.z = ap->value.z;
																															GRPPOINT f;				f.x	=	pf->value.x;				f.y = pf->value.y;				f.z = pf->value.z;
																															GRPPOINT b;				b.x = control->value.x;		b.y	=	control->value.y;		b.z	=	control->value.z;

																															GRPSPLINE* beziercurve= CreateArc(o, f, b);
												
																															for(XDWORD i=0; i<beziercurve->points.GetSize(); i++)
																																{
																																	x = beziercurve->points.Get(i)->value.x;
																																	y = beziercurve->points.Get(i)->value.y;
																																	z = beziercurve->points.Get(i)->value.z;
																																	w = beziercurve->points.Get(i)->value.w;

																																	vertex[(n*3)+0]		= x;
																																	vertex[(n*3)+1]		= y;
																																	vertex[(n*3)+2]		= z;

																																	normals[(n*3)+0]	=	0.0f;
																																	normals[(n*3)+1]	=	0.0f;
																																	normals[(n*3)+2]	=	1.0f;

																																	texturecoords[(n*2)+0]	= width  / (x-minX);
																																	texturecoords[(n*2)+1]	=	height / (y-minY);		

																																	n++;
																																}

 																															delete(beziercurve);
																												 		}											
																														
																												}
																												break;
							default: break;
							}
					}
							GRPELEMENT* element = new GRPELEMENT();	
							if(!element)
								{
									delete(vertex);
									vertex=NULL;
									delete(normals);
									normals=NULL;
									delete(texturecoords);
									texturecoords=NULL;

									return false;
								}
							if (!element->GetMesh())
							{
									element->SetMesh(new GRPMESH());
							}
							element->GetMesh()->LoadModel(n, vertex, normals, texturecoords);
							
							if (element==NULL) 
								return false;
								
							element->SetRenderMode(GRPRENDERMODE_LINE_STRIP);
							if(	object != NULL) 							
							{
									object->GetElements()->Add(element);							
									element->SetNode(object->GetNode());
									//element->SetProgram(object->GetProgram());
							}
						
							n=0;
					
		}


	delete(vertex);
	vertex=NULL;
	delete(normals);
	normals=NULL;
	delete(texturecoords);
	texturecoords=NULL;

	return true;
}

/*-------------------------------------------------------------------
//	 GRPOBJECTBUILDER::ConvertShapeToElement
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	16/02/2018 11:43:29
//	@return		GRPELEMENT* : 
//
//	@param		GRPSHAPE* : 
//
*//*-----------------------------------------------------------------*/
GRPELEMENT* GRPOBJECTBUILDER::ConvertShapeToElement(GRPSHAPE* _shape)
{
		GRPELEMENT* element = NULL;

		GLFLOAT			maxX = 0.0f;
		GLFLOAT			maxY = 0.0f;
		GLFLOAT			maxZ = 0.0f;

		GLFLOAT			minX = 0.0f;
		GLFLOAT			minY = 0.0f;
		GLFLOAT			minZ = 0.0f;

		int					nsegments = 0;

		GRPSHAPE* shape = _shape->CreateCopy();
		
		for (XDWORD c = 0; c< shape->contours.GetSize(); c++)
		{
				for (XDWORD e = 0; e<shape->contours.Get(c)->points.GetSize(); e++)
				{
						GRPSPLINE*			spline = shape->contours.Get(c);
						GRPANCHORPOINT* point = shape->contours.Get(c)->points.Get(e);

						if (point->type != GRPANCHORPOINTTYPE_CONTROLPOINT)
						{
								nsegments++;

								if (point->value.x	<	minX)		minX = point->value.x;
								if (point->value.y	<	minY)		minY = point->value.y;
								if (point->value.z	<	minZ)		minZ = point->value.z;

								if (point->value.x	>	maxX)		maxX = point->value.x;
								if (point->value.y	>	maxY)		maxY = point->value.y;
								if (point->value.z	>	maxZ)		maxZ = point->value.z;
						}
						else
						{
								if (spline)  nsegments += spline->nsegments;
						}
				}
		}

		GLFLOAT* vertex					= new GLFLOAT[nsegments * 3];	if (!vertex)				return NULL;
		GLFLOAT* normals				= new GLFLOAT[nsegments * 3];	if (!normals)				return NULL;
		GLFLOAT* texturecoords	= new GLFLOAT[nsegments * 2];	if (!texturecoords) return NULL;


		GLFLOAT width = maxX - minX;
		GLFLOAT height = maxY - minY;
		GLFLOAT depth = maxZ - minZ;

		XDWORD	n = 0;
		float		x = 0.0f;
		float		y = 0.0f;
		float		z = 0.0f;
		float		w = 1.0f;
		XDWORD	i = 0;

		for (XDWORD c = 0; c<shape->contours.GetSize(); c++)
		{
				for (XDWORD e = 0; e<shape->contours.Get(c)->points.GetSize(); e++)
				{
						GRPANCHORPOINT* ap = shape->contours.Get(c)->points.Get(e);

						switch (ap->type)
						{
						case GRPANCHORPOINTTYPE_LINE:	x = ap->value.x;
								y = ap->value.y;
								z = ap->value.z;
								w = ap->value.w;

								vertex[(n * 3) + 0] = x;
								vertex[(n * 3) + 1] = y;
								vertex[(n * 3) + 2] = z;

								normals[(n * 3) + 0] = 0.0f;
								normals[(n * 3) + 1] = 0.0f;
								normals[(n * 3) + 2] = 1.0f;

								texturecoords[(n * 2) + 0] = (x - minX) / width;
								texturecoords[(n * 2) + 1] = (y - minY) / height;
								n++;
								break;

						case GRPANCHORPOINTTYPE_QUADRIC_BEZIER: { GRPANCHORPOINT* pf = shape->contours.Get(c)->points.Get(e + 1);
								GRPANCHORPOINT* control = shape->contours.Get(c)->points.Get(e + 2);

								if (pf->type == GRPANCHORPOINTTYPE_QUADRIC_BEZIER && control->type == GRPANCHORPOINTTYPE_CONTROLPOINT)
								{
										GRPPOINT o;				o.x = ap->value.x;				o.y = ap->value.y;				o.z = ap->value.z;
										GRPPOINT f;				f.x = pf->value.x;				f.y = pf->value.y;				f.z = pf->value.z;
										GRPPOINT b;				b.x = control->value.x;		b.y = control->value.y;		b.z = control->value.z;

										GRPSPLINE* beziercurve = CreateArc(o, f, b);

										for (XDWORD i = 0; i<beziercurve->points.GetSize(); i++)
										{
												x = beziercurve->points.Get(i)->value.x;
												y = beziercurve->points.Get(i)->value.y;
												z = beziercurve->points.Get(i)->value.z;
												w = beziercurve->points.Get(i)->value.w;

												vertex[(n * 3) + 0] = x;
												vertex[(n * 3) + 1] = y;
												vertex[(n * 3) + 2] = z;

												normals[(n * 3) + 0] = 0.0f;
												normals[(n * 3) + 1] = 0.0f;
												normals[(n * 3) + 2] = 1.0f;

												texturecoords[(n * 2) + 0] = width / (x - minX);
												texturecoords[(n * 2) + 1] = height / (y - minY);

												n++;
										}

										delete(beziercurve);
								}

						}
																										break;
						default: break;
						}
				}
				element = new GRPELEMENT();
				if (!element)
				{
						delete(vertex);
						vertex = NULL;
						delete(normals);
						normals = NULL;
						delete(texturecoords);
						texturecoords = NULL;

						return NULL;
				}
				if (!element->GetMesh())
				{
						element->SetMesh(new GRPMESH());
				}
				element->GetMesh()->LoadModel(n, vertex, normals, texturecoords);

				if (element == NULL)
						return NULL;

				element->SetRenderMode(GRPRENDERMODE_LINE_STRIP);			
				n = 0;
		}


		delete(vertex);
		vertex = NULL;
		delete(normals);
		normals = NULL;
		delete(texturecoords);
		texturecoords = NULL;

		return element;
}


/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::ResolveCubicBezier
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			16/02/2015 17:31:54
//	
//	@return 			GRPSPLINE* : 
//
//  @param				inputSp : 
//  @param				nSegments : (default egments=25)
*/
/*-----------------------------------------------------------------*/
GRPSPLINE*	GRPOBJECTBUILDER::ResolveCubicBezier										(GRPSPLINE* inputSp,					int nSegments)
{
	GRPBEZIERSPLINE spline;
	return spline.ResolveCubicBezier(inputSp,nSegments);
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::TriangulizeShape
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/06/2014 10:27:26
//	
//	@return 			GRPELEMENT* : 
//
//  @param				programID : 
//  @param				shape : 
//  @param				material : 
//  @param				algorithm : 
*/
/*-----------------------------------------------------------------*/

GRPELEMENT*	GRPOBJECTBUILDER::TriangulizeShape(GRPSHAPE* shape,  GRPOBJECTBUILDERALGORITHM* algorithm) 
{
xmutex->Lock();
		
		GRPELEMENT* element = new GRPELEMENT();
		if (element)
		element->SetMesh(new GRPMESH());
	
	if (shape->contours.GetSize()==0)
	{
		xmutex->UnLock();
		return element;
	}

	if (shape->contours.Get(0)->points.GetSize()==0) //no tiene datos el shape!
	{
		xmutex->UnLock();
		return element;
	}

	for(int i=shape->contours.GetSize()-1; i>=0; i--)
		{
			GRPSHAPE* newshape = new GRPSHAPE();				
			if(newshape)
				{
					GRPSPLINE* spline = new GRPSPLINE();
					if(spline)
						{
							newshape->AddContour(spline);

							for(XDWORD e=0; e<shape->contours.Get(i)->points.GetSize(); e++)
								{
									GRPANCHORPOINT* ap= shape->contours.Get(i)->points.Get(e);
									newshape->contours.GetLast()->Add(ap->type, ap->value);									
								}

							GRPELEMENT* newelement = algorithm->Execute(newshape);
							
							if(newelement)
								{
									newelement->GetMesh()->SetFaceBuffers();
									if (!element->MergeElement(newelement))									
											{
												xmutex->UnLock();
												return NULL;
											}	
									delete(newelement);
									newelement=NULL;
								}							
						}					
					delete(newshape);
					newshape=NULL;
				}
		}			

	//OPENGLCHECKERROR(__L("TriangulizeShape"));	
	xmutex->UnLock();
	return element;
}



/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::GetMasterSprite
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/10/2014 9:51:19
//	
//	@return 			GRPELEMENT* : 
//
*/
/*-----------------------------------------------------------------*/
/*
GRPSPRITE*	GRPOBJECTBUILDER::GetMasterSprite()
{
	if (masterSprite==0)
		CreateMasterSprite();

	return masterSprite;
	}
	*/
/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::CreateFontManager
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/10/2014 9:52:23
//	
//	@return 			GRPFONTMANAGER* : 
//
*/
/*-----------------------------------------------------------------*/
GRPFONTMANAGER* GRPOBJECTBUILDER::CreateFontManager()
{
	if (this->fontmanager==NULL)
	this->fontmanager= new GRPFONTMANAGER();
	return this->fontmanager;
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::GetFontManager
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			17/10/2014 9:52:28
//	
//	@return 			GRPFONTMANAGER* : 
//
*/
/*-----------------------------------------------------------------*/
GRPFONTMANAGER*	GRPOBJECTBUILDER::GetFontManager() 
{
	return fontmanager;
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::GetReferences
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			06/10/2015 11:39:40
//	
//	@return 			XVECTOR<GRPELEMENT*>* : 
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPELEMENT*>* GRPOBJECTBUILDER::GetReferences	()
{
	return &references; 
};


/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::Get
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/09/2016 14:28:43
//	
//	@return 			GRPOBJECTBUILDER* : 
//
*/
/*-----------------------------------------------------------------*/
GRPOBJECTBUILDER*		 GRPOBJECTBUILDER::Get()
{
	if (GRPOBJECTBUILDER::Instance==NULL)
	{
		GRPOBJECTBUILDER::Instance=new GRPOBJECTBUILDER();
	}
	return GRPOBJECTBUILDER::Instance;
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::Destroy
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			05/09/2016 14:46:57
//	
*/
/*-----------------------------------------------------------------*/
void GRPOBJECTBUILDER::Destroy()
{
	if (GRPOBJECTBUILDER::Instance)
	{
		delete GRPOBJECTBUILDER::Instance;
		GRPOBJECTBUILDER::Instance=NULL;
	}
}

/*-------------------------------------------------------------------
//	GRPOBJECTBUILDER::FastGetDistanceBetweenPoints
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			24/06/2014 16:45:35
//	
//	@return 			float : 
//
//  @param				a : 
//  @param				b : 
*/
/*-----------------------------------------------------------------*/
float GRPOBJECTBUILDER::FastGetDistanceBetweenPoints(GRPPOINT* a, GRPPOINT* b)
{
	float cat1 = a->x - b->x;
	float cat2 = a->y - b->y;

	return (cat1*cat1) + (cat2*cat2);
}
