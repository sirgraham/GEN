
/*------------------------------------------------------------------------------------------
//	GRPNODEMANAGER.CPP
//	
//	Holds the collection of nodes
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 28/10/2015 17:39:31
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPNodeManager.h"
#include "GRPNode.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
		
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

GRPNODEMANAGER* GRPNODEMANAGER::Instance=NULL;

/*-------------------------------------------------------------------
//	GRPNODEMANAGER::CreateNode
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			28/10/2015 17:44:37
//	
//	@return 			GRPNODE* : 
//
*/
/*-----------------------------------------------------------------*/
GRPNODE*	GRPNODEMANAGER::CreateNode()
{
	GRPNODE*	node	=new GRPNODE();
						
						GRPNODEMANAGER::Get()->GetPool()->Add(node);					

	return node;
}

/*-------------------------------------------------------------------
//	GRPNODEMANAGER::DestroyNode
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			29/10/2015 10:05:24
//	
//	@return 			bool : 
//
//  @param				node : 
*/
/*-----------------------------------------------------------------*/
bool	GRPNODEMANAGER::DestroyNode			(GRPNODE* node)
{
	int index=this->GetPool()->Find(node);
		if (index!=NOTFOUND)
		{
			delete	(node);
			this->GetPool()->DeleteIndex(index);		
		}
	return true;
}

/*-------------------------------------------------------------------
//	GRPNODEMANAGER::DestroyAll
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			29/10/2015 10:16:25
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool	GRPNODEMANAGER::DestroyAll			()
{
	int n=NodePool.GetSize();
	for (int e=0;e<n;e++)		
		delete(NodePool.FastGet(e));

	NodePool.DeleteAll();

	return true;
}

/*-------------------------------------------------------------------
//	GRPNODEMANAGER::GetPool
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			29/10/2015 10:02:58
//	
//	@return 			XVECTOR<GRPNODE*>* : 
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<GRPNODE*>*	GRPNODEMANAGER::GetPool()
{
	return &this->NodePool;
}
