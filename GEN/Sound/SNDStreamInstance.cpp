
/*------------------------------------------------------------------------------------------
//	SNDSTREAMINSTANCE.CPP
//	
//	implements an streaming instance
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 01/12/2016 17:15:13
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "SNDStreamInstance.h"
#include "SNDStreamElement.h"
#include "SNDFile.h"	
#include "SNDCodec.h"
#include "MEDIAFrameReader.h"

#include "XDebug.h"
#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	SNDSTREAMINSTANCE::SNDSTREAMINSTANCE
*/	
/**	
//	
//	Class Constructor SNDSTREAMINSTANCE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2016 17:30:23
//	
//  @param				sndsystem : 
//  @param				source : 
//  @param				element : 
*/
/*-----------------------------------------------------------------*/
SNDSTREAMINSTANCE::SNDSTREAMINSTANCE(SNDFACTORY* sndsystem, SNDSOURCE* source, SNDELEMENT* element) : SNDINSTANCE(sndsystem, source, element)
{
	Clean();

	isplaying = true;
}




/*-------------------------------------------------------------------
//	SNDSTREAMINSTANCE::~SNDSTREAMINSTANCE
*/	
/**	
//	
//	 Class Destructor SNDSTREAMINSTANCE
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2016 17:30:34
//	
*/
/*-----------------------------------------------------------------*/
SNDSTREAMINSTANCE::~SNDSTREAMINSTANCE()
{
	Clean();
}




/*-------------------------------------------------------------------
//	SNDSTREAMINSTANCE::Update
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			01/12/2016 17:30:41
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SNDSTREAMINSTANCE::Update()
{
	// buffer in here or in the constructor
	// in here, and don't start playing until the minimum amount has been buffered
	// also, need to check if framereader is EOS, as in that case we need to stopSND
	if(element)
		{
			if(element->GetFile())
				{
					if(element->GetFile()->GetFrameReader()->IsEndOfStream())
						{
							// need to comunicate that we stopped(but we didn't stop sounding!)
							// need to detect when both the stream finished reading and when it finished playing!
							isplaying = source->IsPLaying();
						}

					if(buffered < 2.0f)
						{
							SNDFILE* file = element->GetFile();

							MEDIAFRAMEREADER* framereader = file->GetFrameReader();
							SNDCODEC* codec = (SNDCODEC*)file->GetCodec();

							MEDIAPACKET packet;
							framereader->ReadFrame(&packet);
							XBUFFER output(false);
							codec->DecodePacket(&output, &packet);
							buffered += (float)packet.duration;

							// queue the buffer in the source
							SNDSTREAMELEMENT* streamelement = (SNDSTREAMELEMENT*)element;
							streamelement->AddData(output.GetSize(), output.Get());
						}
					else
						{
							SNDSTREAMELEMENT* streamelement = (SNDSTREAMELEMENT*)element;

							if(!hasbuffered)
								{
									// tell play to the source(we just finished queuing the basic data)
									streamelement->Play();
									isplaying = true;
								}

							hasbuffered = true;

							SNDFILE* file = element->GetFile();
							MEDIAFRAMEREADER* framereader = file->GetFrameReader();
							SNDCODEC* codec = (SNDCODEC*)file->GetCodec();

							MEDIAPACKET packet;
							framereader->ReadFrame(&packet);
							XBUFFER output(false);
							codec->DecodePacket(&output, &packet);
							buffered += (float)packet.duration;

							// queue the buffer in the source
							
							streamelement->AddData(output.GetSize(), output.Get());
						}
				}
			else
				{
					//if(element->duration
					if(element->GetDuration() > 0)
						{
							isplaying = source->IsPLaying();
						}
				}
		}

	return true;
}
