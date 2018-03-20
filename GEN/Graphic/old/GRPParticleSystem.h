/*------------------------------------------------------------------------------------------
//	GRPPARTICLESYSTEM.H
*/	
/**	
// \class 
//   
//  Holds cluster data for sprite clouds
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 13/10/2015 16:49:31
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPPARTICLESYSTEM_H_
#define _GRPPARTICLESYSTEM_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPParticlePool.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPSCENE;

class GRPPARTICLESYSTEM : public GRPELEMENT
{
public:

																			GRPPARTICLESYSTEM		(GRPSCENE* scn) : scene(scn)
																			{
																				GRPNAMED::classtype = GRPNAMED_TYPE_GRPPARTICLESYSTEM;
																			}
	virtual															~GRPPARTICLESYSTEM	()																		{ Clean(); }

	virtual	bool												Draw								(	GRPMATRIX&				view,		
																														GRPMATRIX*				viewprojection,	
																														GRPMATRIX*				projection);

					bool												Step								();
					XVECTOR<GRPPARTICLEPOOL*>*	GetPools						();
					GRPPARTICLEPOOL*						GetPool							(int x);
					GRPPARTICLEPOOL*						CreatePool					();

protected:

					GRPSCENE*										scene;
					XVECTOR<GRPPARTICLEPOOL*>		Pools;		

private:

		void				Clean()
		{			 			 
			Pools.DeleteContents();
			Pools.DeleteAll();
			scene=NULL;
		}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

