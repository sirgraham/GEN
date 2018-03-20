
/*------------------------------------------------------------------------------------------
//	SNDFACTORY.CPP
//	
//	sndsystem
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 22/01/2016 11:44:16
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XDebug.h"	
#include "SNDInstance.h"

#include "SNDFactory.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

SNDFACTORY*												SNDFACTORY::Instance=NULL;
bool															SNDFACTORY::SoundIsActive = true;

/*-------------------------------------------------------------------
//	SNDFACTORY::Update
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			02/05/2016 6:51:56
//	
*/
/*-----------------------------------------------------------------*/
void SNDFACTORY::Update()
{
	ImpUpdate();

	XLIST<SNDINSTANCE*>::XITERATOR it;

	for(it = todeleteinstances.Begin(); it != todeleteinstances.End(); it++)
		{
			delete *it;
		}

	todeleteinstances.DeleteAll();

	if(instances.GetSize() > 100)
		{
			XDEBUG_PRINTCOLOR(4, __L("WARNING: too many SNDINSTANCES"));
		}
}
