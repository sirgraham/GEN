/*------------------------------------------------------------------------------------------
//	AIANIMATIONOVERLAY.H
*/	
/**	
// \class 
//   
//  Manages animation overlays
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 01/12/2016 13:25:12
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _AIANIMATIONOVERLAY_H_
#define _AIANIMATIONOVERLAY_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPSubject.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/


namespace AI
{
	namespace ANIMATION
	{
		class OVERLAY : public GRPSUBJECT
		{
		public:
														OVERLAY									(XDWORD id,XDWORD status);			
				XVECTOR<XDWORD>*		GetOverlayables					() { return &this->Overlayables;							}

				XVECTOR<XDWORD>			Overlayables;
								XDWORD			id;
								XDWORD			status;
		};

	};
};
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

