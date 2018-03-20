/*------------------------------------------------------------------------------------------
//	GRPNODEMANAGER.H
*/	
/**	
// \class 
//   
//  Manages Nodes on scenes
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 27/10/2015 10:24:11
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPNODEMANAGER_H_
#define _GRPNODEMANAGER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XVector.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPNODE;

class GRPNODEMANAGER
{
public:

	friend class GRPNODE;

	static	GRPNODEMANAGER* Instance;
	static	GRPNODEMANAGER* Get()
	{
		if (Instance==NULL)
			Instance=new GRPNODEMANAGER();

		return Instance;
	}

	GRPNODE*						CreateNode			();
	bool								DestroyNode			(GRPNODE* node);
	bool								DestroyAll			();

protected:

											GRPNODEMANAGER	() {NodePool.SetIsMulti(false);}		
	
	XVECTOR<GRPNODE*>*	GetPool					();


	XVECTOR<GRPNODE*>		NodePool;

private:

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

