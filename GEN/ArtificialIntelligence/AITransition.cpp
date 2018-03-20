
/*------------------------------------------------------------------------------------------
//	AITRANSITION.CPP
//	
//	Manages transitionables states
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 02/12/2016 13:47:01
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "AITransition.h"
#include "XDebug.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	AI::TRANSITION::GetTransitions
*/	
/**	
//	
//	return vector
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 13:48:10
//	
//	@return 			XVECTOR<AI::STATE*>* : 
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<XDWORD>*	AI::TRANSITION::GetTransitions()
{
	return &this->transitions;
}

XVECTOR<XDWORD>*	AI::TRANSITION::GetRejections()
{
	return &this->rejections;
}

/*-------------------------------------------------------------------
//	AI::TRANSITION::Accept
*/	
/**	
//	
//	returns if newanimation state is acceptable in current state
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 13:48:31
//	
//	@return 			bool : 
//
//  @param				newanimation : 
*/
/*-----------------------------------------------------------------*/
bool AI::TRANSITION::Accept(XDWORD newanimation)
{	
	for (XDWORD e=0;e<this->transitions.GetSize();e++)
	{
		if (newanimation==this->transitions.FastGet(e))
			return true;
	}
	return false;
}

bool AI::TRANSITION::Reject(XDWORD newanimation)
{	
	for (XDWORD e=0;e<this->rejections.GetSize();e++)
	{
		if (newanimation==this->rejections.FastGet(e))
			return true;
	}
	return false;
}