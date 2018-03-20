
/*------------------------------------------------------------------------------------------
//	SNDSOURCE.CPP
//	
//	implements a sound source
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 15/12/2016 17:37:34
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "SNDSource.h"
#include "SNDInstance.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

bool SNDSOURCE::IsInstancePlaying()
{
	if(sndinstance)
		{
			return sndinstance->IsPlaying();
		}

	return false;
}
