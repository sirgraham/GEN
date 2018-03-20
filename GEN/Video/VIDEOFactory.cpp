
/*------------------------------------------------------------------------------------------
//	VIDEOFACTORY.CPP
//	
//	VIDEO factory
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 11/04/2016 14:02:31
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "VIDEOFactory.h"
#include "VIDEOCodec.h"
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/
VIDEOFACTORY::VIDEOFACTORY(VIDEOPLAYER* videoplayer)
{
	Clean();

	this->videoplayer = videoplayer;
}

VIDEOFACTORY::~VIDEOFACTORY()
{
	Clean();
}

void VIDEOFACTORY::DeleteCodec(VIDEOCODEC* codec)
{
	delete codec;
}
