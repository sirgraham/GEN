
/*------------------------------------------------------------------------------------------
//	GRPPARTICLEPOOL.CPP
//	
//	 Manages Particle Clusters
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 15/10/2015 13:57:54
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPParticlePool.h"
#include "GRPShader.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::GRPPARTICLEPOOL
*/	
/**	
//	
//	Class Constructor GRPPARTICLEPOOL
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/10/2015 11:58:16
//	
*/
/*-----------------------------------------------------------------*/
GRPPARTICLEPOOL::GRPPARTICLEPOOL()																		
{
	Clean();								
	GRPNAMED::classtype = GRPNAMED_TYPE_GRPPARTICLEPOOL;
}

/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::~GRPPARTICLEPOOL
*/	
/**	
//	
//	 Class Destructor GRPPARTICLEPOOL
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/10/2015 11:58:20
//	
*/
/*-----------------------------------------------------------------*/
GRPPARTICLEPOOL::~GRPPARTICLEPOOL()			
{
	Particles.DeleteContents();
	Particles.DeleteAll			();
	delete[] Matrices;
}

/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::Init
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/10/2015 11:11:02
//	
//	@return 			bool : 
//
//  @param				n : 
*/
/*-----------------------------------------------------------------*/
bool GRPPARTICLEPOOL::Init			(XDWORD n)
{
	for (XDWORD e=0;e<n;e++)
	{
		if (!Add()) return false;
	}

	this->CreateMesh();
	
	return true;
}

/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::Step
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/10/2015 11:11:06
//	
*/
/*-----------------------------------------------------------------*/
void GRPPARTICLEPOOL::Step						()
{

}

/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::Add
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/10/2015 11:32:15
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
GRPPARTICLE* GRPPARTICLEPOOL::Add							()
{
		GRPPARTICLE* particle=new GRPPARTICLE(this->PoolSize);
		if (particle==NULL)									return NULL;
		if (!this->Particles.Add(particle)) 
		{
				delete(particle);
				return NULL;		
		}
		this->PoolSize=this->Particles.GetSize();
		return particle;
}

/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::CreateMesh
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/10/2015 16:23:16
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/

bool GRPPARTICLEPOOL::CreateMesh			()
{
	return CreateTriangleMesh();
}

/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::CreateTriangleStripMesh
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/10/2015 12:16:16
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPPARTICLEPOOL::CreateTriangleStripMesh			()
{
	XDWORD n=this->Particles.GetSize();

	int nvertex=4*n;
	int nindex=6*n;
	this->SetNVertex(nvertex);

	if (this->GetVertexData()!=NULL)	delete[] this->GetVertexData();
	if (this->GetIndexData()!=NULL)		delete[] this->GetIndexData();

	this->SetVertexData(new GRPVERTEX[nvertex]);
	
	if (this->GetVertexData()==NULL)
	{
		XDEBUG_PRINTCOLOR(4,__L("GRPELEMENT::LoadModel Not enought memory"));	
		return false;
	}

	this->SetIndexData(new XWORD[nindex]);
	if (this->GetIndexData()==NULL)
	{
		XDEBUG_PRINTCOLOR(4,__L("GRPELEMENT::LoadModel Not enought memory"));	
		return false;
	}
	
	this->SetNindexData(nindex);
	this->SetNvertexData(nvertex);

	XDWORD e=0,i=0,p=0;

	GRPVERTEX*	vertexdata=this->GetVertexData	();
	XWORD*			indexdata	=this->GetIndexData		();

for (p=0;p<n;p++)
{
		indexdata	[i++]=p*4+0;

		vertexdata[e].position					[0]=-0.5f;
		vertexdata[e].position					[1]=+0.5f;
		vertexdata[e].position					[2]=+0.0;

		vertexdata[e].normal						[0]=+0.0;
		vertexdata[e].normal						[1]=+0.0;
		vertexdata[e].normal						[2]=-1.0;

		vertexdata[e].texturecoordinates[0]=0.0;
		vertexdata[e].texturecoordinates[1]=1.0;

//		vertexdata[e].instanceID=(float)p;

		e++;
		indexdata	[i++]=p*4+1;

		vertexdata[e].position					[0]=-0.5f;
		vertexdata[e].position					[1]=-0.5f;
		vertexdata[e].position					[2]=+0.0;

		vertexdata[e].normal						[0]=+0.0;
		vertexdata[e].normal						[1]=+0.0;
		vertexdata[e].normal						[2]=-1.0;

		vertexdata[e].texturecoordinates[0]=0.0;
		vertexdata[e].texturecoordinates[1]=0.0;
	
//		vertexdata[e].instanceID=(float)p;

		e++;

		indexdata	[i++]=p*4+2;

		vertexdata[e].position					[0]=+0.5f;
		vertexdata[e].position					[1]=+0.5f;
		vertexdata[e].position					[2]=+0.0;

		vertexdata[e].normal						[0]=+0.0;
		vertexdata[e].normal						[1]=+0.0;
		vertexdata[e].normal						[2]=-1.0;

		vertexdata[e].texturecoordinates[0]=1.0;
		vertexdata[e].texturecoordinates[1]=1.0;

//		vertexdata[e].instanceID=(float)p;

		e++;
		
		indexdata	[i++]=p*4+3;

		vertexdata[e].position					[0]=+0.5f;
		vertexdata[e].position					[1]=-0.5f;
		vertexdata[e].position					[2]=+0.0;

		vertexdata[e].normal						[0]=+0.0;
		vertexdata[e].normal						[1]=+0.0;
		vertexdata[e].normal						[2]=-1.0;

		vertexdata[e].texturecoordinates[0]=1.0;
		vertexdata[e].texturecoordinates[1]=0.0;

//		vertexdata[e].instanceID=(float)p;

		e++;
	
		//0, 1, 2, 3, 3, 0, 0, 1, 2, 3		
	/*
.A B C
 C B D
.C D D - deg
 D D E - deg
.D E E - deg
 E E F - deg
.E F G
 G F H

 A, B, C, D, 
                              D, E,
                              E, F, G, H

 */
		indexdata	[i++]=p*4+3; //d
		indexdata	[i++]=p*4+0; //d
		//indexdata	[i++]=p*4+3; //d
		//indexdata	[i++]=p*4+0; //d

		//-----quad till there

}

	Matrices=new GLFLOAT[n*4*4];
	if (Matrices==NULL)
		return false;

	for (XDWORD e=0;e<n;e++)
	{
		GRPMATRIX m(1);	m.BuildTranslationMatrix(this->Particles.FastGet(e)->position.vector[0],this->Particles.FastGet(e)->position.vector[1],this->Particles.FastGet(e)->position.vector[2]);
		GRPMATRIX s(1);	s.BuildScaleMatrix(this->Particles.FastGet(e)->size,this->Particles.FastGet(e)->size,this->Particles.FastGet(e)->size);
		GRPMATRIX ms;		ms.GetMultiplicationMatrix(s,m);

		ms.CopyTo(&Matrices[e*4*4]);
	}

	this->SetRenderMode(GRPRENDERMODE_TRIANGLE_STRIP);
	this->SetIsLoaded(false);

	return true;	
}


/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::CreateTriangleMesh
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/10/2015 12:16:09
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool GRPPARTICLEPOOL::CreateTriangleMesh			()
{
	XDWORD nParticles=this->Particles.GetSize();

	XDWORD vertexPerParticle=4;
	XDWORD indexPerParticle=6;

	int nvertex=vertexPerParticle*nParticles;
	int nindex=indexPerParticle*nParticles;
	this->SetNVertex(nvertex);

	if (this->GetVertexData()!=NULL)	delete[] this->GetVertexData();
	if (this->GetIndexData()!=NULL)		delete[] this->GetIndexData();

	this->SetVertexData(new GRPVERTEX[nvertex]);
	
	if (this->GetVertexData()==NULL)
	{
		XDEBUG_PRINTCOLOR(4,__L("GRPELEMENT::LoadModel Not enought memory"));	
		return false;
	}

	this->SetIndexData(new XWORD[nindex]);
	if (this->GetIndexData()==NULL)
	{
		XDEBUG_PRINTCOLOR(4,__L("GRPELEMENT::LoadModel Not enought memory"));	
		return false;
	}
	
	this->SetNindexData(nindex);
	this->SetNvertexData(nvertex);

	XDWORD e=0,i=0,p=0;

	GRPVERTEX*	vertexdata=this->GetVertexData	();
	XWORD*			indexdata	=this->GetIndexData		();

	XDWORD index=0;

for (p=0;p<nParticles;p++)
{
		indexdata	[i++]=index+0;

		vertexdata[e].position					[0]=-0.5f;
		vertexdata[e].position					[1]=+0.5f;
		vertexdata[e].position					[2]=+0.0;

		vertexdata[e].normal						[0]=+0.0;
		vertexdata[e].normal						[1]=+0.0;
		vertexdata[e].normal						[2]=-1.0;

		vertexdata[e].texturecoordinates[0]=0.0;
		vertexdata[e].texturecoordinates[1]=1.0;

//		vertexdata[e].instanceID=(float)p;

		e++;
		indexdata	[i++]=index+1;

		vertexdata[e].position					[0]=-0.5f;
		vertexdata[e].position					[1]=-0.5f;
		vertexdata[e].position					[2]=+0.0;

		vertexdata[e].normal						[0]=+0.0;
		vertexdata[e].normal						[1]=+0.0;
		vertexdata[e].normal						[2]=-1.0;

		vertexdata[e].texturecoordinates[0]=0.0;
		vertexdata[e].texturecoordinates[1]=0.0;
	
//		vertexdata[e].instanceID=(float)p;

		e++;

		indexdata	[i++]=index+2;

		vertexdata[e].position					[0]=+0.5f;
		vertexdata[e].position					[1]=+0.5f;
		vertexdata[e].position					[2]=+0.0;

		vertexdata[e].normal						[0]=+0.0;
		vertexdata[e].normal						[1]=+0.0;
		vertexdata[e].normal						[2]=-1.0;

		vertexdata[e].texturecoordinates[0]=1.0;
		vertexdata[e].texturecoordinates[1]=1.0;

//		vertexdata[e].instanceID=(float)p;

		e++;
	
		indexdata	[i++]=index+3;

		vertexdata[e].position					[0]=+0.5f;
		vertexdata[e].position					[1]=-0.5f;
		vertexdata[e].position					[2]=+0.0;

		vertexdata[e].normal						[0]=+0.0;
		vertexdata[e].normal						[1]=+0.0;
		vertexdata[e].normal						[2]=-1.0;

		vertexdata[e].texturecoordinates[0]=1.0;
		vertexdata[e].texturecoordinates[1]=0.0;

//		vertexdata[e].instanceID=(float)p;

		e++;

		indexdata	[i++]=index+2;
		indexdata	[i++]=index+1;

		index+=vertexPerParticle;
}

	Matrices=new GLFLOAT[nParticles*4*4];
	if (Matrices==NULL)
		return false;

	for (XDWORD e=0;e<nParticles;e++)
	{
		GRPMATRIX m(1);	m.BuildTranslationMatrix(this->Particles.FastGet(e)->position.vector[0],this->Particles.FastGet(e)->position.vector[1],this->Particles.FastGet(e)->position.vector[2]);
		GRPMATRIX s(1);	s.BuildScaleMatrix(this->Particles.FastGet(e)->size,this->Particles.FastGet(e)->size,this->Particles.FastGet(e)->size);
		GRPMATRIX ms;		ms.GetMultiplicationMatrix(s,m);

		ms.CopyTo(&Matrices[e*4*4]);
	}

	this->SetRenderMode(GRPRENDERMODE_TRIANGLES);
	this->SetIsLoaded(false);

	return true;	
}


/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::Draw
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/10/2015 15:56:22
//	
//	@return 			bool : 
//
//  @param				viewmatrix : 
//  @param				viewprojection : 
//  @param				projection : 
//  @param				extshaderlink : 
*/
/*-----------------------------------------------------------------*/
bool GRPPARTICLEPOOL::Draw(GRPMATRIX& viewmatrix,	GRPMATRIX* viewprojection,	GRPMATRIX* projection)
{
	GRPSHADER::UseShader(this->program);
	return GRPELEMENT::Draw(viewmatrix,viewprojection,projection);
}


/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::LoadMatricesToGPU
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/10/2015 17:46:45
//	
//	@return 			bool : 
//
//  @param				viewmatrix : 
//  @param				viewprojection : 
//  @param				projection : 
//  @param				extshaderlink : 
*/
/*-----------------------------------------------------------------*/
void GRPPARTICLEPOOL::LoadMatricesToGPU	(GRPMATRIX* viewmatrix,	GRPMATRIX* viewprojection,	GRPMATRIX* projection,GRPSHADERPROGRAM* link)
{
if (this->renderflags & GRPELEMENTRENDERFLAGS_LOADMATRIXTOGPU)
	{	
		if (link->ModelMatrixLoc							!=-1)					
																												glUniformMatrix4fv(link->ModelMatrixLoc,								(GLsizei)1, GL_FALSE, &(modelmatrix.matrix[0][0]));		
		OPENGLCHECKERROR(__L("ModelMatrixLoc"));

		if (link->ViewMatrixLoc								!=-1)					
																												glUniformMatrix4fv(link->ViewMatrixLoc,									(GLsizei)1, GL_FALSE, &(viewmatrix->matrix[0][0]));																				
		OPENGLCHECKERROR(__L("ViewMatrixLoc"));

		if (link->ProjectionMatrixLoc					!=-1)					
																												glUniformMatrix4fv(link->ProjectionMatrixLoc,						(GLsizei)1, GL_FALSE,	&(projection->matrix[0][0]));																				
		OPENGLCHECKERROR(__L("ProjectionMatrixLoc"));

		if (link->ModelViewMatrixLoc					!=-1)					
																												glUniformMatrix4fv(link->ModelViewMatrixLoc,						(GLsizei)1, GL_FALSE, &(modelviewmatrix.matrix[0][0]));																				
		OPENGLCHECKERROR(__L("ModelViewMatrixLoc"));

		if (link->ModelViewProjectionMatrixLoc!=-1)			
		{						
			glUniformMatrix4fv(link->ModelViewProjectionMatrixLoc,	(GLsizei)PoolSize, GL_FALSE, Matrices);				
		}
		OPENGLCHECKERROR(__L("ModelViewProjectionMatrixLoc"));
	}

}

/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::ComputeMatrices
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/10/2015 11:59:08
//	
//  @param				viewmatrix : 
//  @param				viewprojection : 
//  @param				projection : 
*/
/*-----------------------------------------------------------------*/
void GRPPARTICLEPOOL::ComputeMatrices(GRPMATRIX* viewmatrix,	GRPMATRIX* viewprojection,	GRPMATRIX* projection)
{		

				modelmatrix=node->GetWorldMatrix();		
				GRPMATRIX t,m,s;

				for (int e=0;e<PoolSize;e++)
				{
					GRPPARTICLE* p=this->Particles.FastGet(e);
					t.BuildTranslationMatrix(p->position.vector[0],p->position.vector[1],p->position.vector[2]);
					s.BuildScaleMatrix(p->size,p->size,p->size);
					m.GetMultiplicationMatrix(t,s);
					m.GetMultiplicationMatrix(&m,projection);
					m.GetMultiplicationMatrix(&m,viewmatrix);
					m.GetMultiplicationMatrix(m,modelmatrix);					
					
					m.CopyTo(&Matrices[e*4*4]);
				}								
}


/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::GetPool
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/10/2015 12:00:45
//	
//	@return 			XVECTOR<GRPPARTICLE*>* : 
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPPARTICLE*>* GRPPARTICLEPOOL::GetPool	()																	
{ 
	return &Particles;							
}	

/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::SetSprite
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/10/2015 12:01:02
//	
//  @param				sprte : 
*/
/*-----------------------------------------------------------------*/
void GRPPARTICLEPOOL::SetSprite(GRPSPRITE* sprte)									
{ 
	this->Sprite=sprte;							
}

/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::GetSize
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/10/2015 12:01:07
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPPARTICLEPOOL::GetSize()																	
{ 
	return PoolSize;								
}

/*-------------------------------------------------------------------
//	GRPPARTICLEPOOL::SetMaterial
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			22/10/2015 12:01:13
//	
//	@return 			bool : 
//
//  @param				m : 
*/
/*-----------------------------------------------------------------*/
bool GRPPARTICLEPOOL::SetMaterial	(GRPMATERIAL* m)										
{ 
	this->Material=m; 
	return true;	
}
