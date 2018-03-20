/*------------------------------------------------------------------------------------------
//	GRPPARTICLEPOOL.H
*/	
/**	
// \class 
//   
//  Manages Particle Clusters
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 15/10/2015 11:03:33
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPPARTICLEPOOL_H_
#define _GRPPARTICLEPOOL_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XVector.h"
#include "GRPSprite.h"
#include "GRPElement.h"
#include "GRPParticle.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/


class GRPPARTICLEPOOL : public GRPELEMENT
{
protected:

	XVECTOR<GRPPARTICLE*>			Particles;
	XDWORD										Size;
	GLFLOAT*									Positions;
	GLFLOAT*									Matrices;
	GLFLOAT*									Color;
	int												PoolSize;
	GRPSPRITE*								Sprite;
	GRPMATERIAL*							Material;

public:

														GRPPARTICLEPOOL					();
														~GRPPARTICLEPOOL				();

	bool											Draw										(	GRPMATRIX& viewmatrix,
																											GRPMATRIX* viewprojection,
																											GRPMATRIX* projection);

	void											LoadMatricesToGPU				(	GRPMATRIX* viewmatrix,	
																											GRPMATRIX* viewprojection,	
																											GRPMATRIX* projection,	
																											GRPSHADERPROGRAM* extshaderlink);

	void											ComputeMatrices					(	GRPMATRIX* viewmatrix,	
																											GRPMATRIX* viewprojection,	
																											GRPMATRIX* projection);

	bool											Init										(	XDWORD n=0);
	void											Step										();
	GRPPARTICLE*							Add											();	
	bool											CreateMesh							();
	bool											CreateTriangleStripMesh	();
	bool											CreateTriangleMesh			();
	
	XVECTOR<GRPPARTICLE*>*		GetPool									();	
	void											SetSprite								(	GRPSPRITE* sprte);
	XDWORD										GetSize									();
	bool											SetMaterial							(	GRPMATERIAL* m	);

private:

	void					Clean()
	{
			Positions	=NULL;
			Color			=NULL;			 
			Sprite		=NULL;
			PoolSize	=0;
	}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

