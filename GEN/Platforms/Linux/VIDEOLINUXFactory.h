/*------------------------------------------------------------------------------------------
//	VIDEOLINUXFACTORY.H
*/	
/**	
// \file 
//   
//  linux video factory
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 11/04/2016 14:06:28
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _VIDEOLINUXFACTORY_H_
#define _VIDEOLINUXFACTORY_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "VIDEOFactory.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class VIDEOLINUXFACTORY : public VIDEOFACTORY
{
public:

																		VIDEOLINUXFACTORY																(VIDEOPLAYER* videoplayer);
	virtual													 ~VIDEOLINUXFACTORY																();

	virtual VIDEOCODEC*								CreateH264Codec																	();

protected:

private:

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

