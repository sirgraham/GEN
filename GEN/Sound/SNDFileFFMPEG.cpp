
/*------------------------------------------------------------------------------------------
//	SNDFILEFFMPEG.CPP
//	
//	implements ffmpeg audio decoding
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 21/11/2016 16:53:40
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "SNDFileFFMPEG.h"
#include "SNDCodecFFMPEG.h"

#include "MEDIAFrameReader.h"
#include "MEDIAFrameReaderLibAV.h"

#include "XFactory.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//	SNDFILEFFMPEG::SNDFILEFFMPEG
*/	
/**	
//	
//	Class Constructor SNDFILEFFMPEG
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/11/2016 16:57:13
//	
*/
/*-----------------------------------------------------------------*/
SNDFILEFFMPEG::SNDFILEFFMPEG()
{
	Clean();
}



/*-------------------------------------------------------------------
//	SNDFILEFFMPEG::~SNDFILEFFMPEG
*/	
/**	
//	
//	 Class Destructor SNDFILEFFMPEG
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/11/2016 16:57:18
//	
*/
/*-----------------------------------------------------------------*/
SNDFILEFFMPEG::~SNDFILEFFMPEG()
{
	if(xbuffer) delete xbuffer;
	if(xbufferdecodeddata) delete xbufferdecodeddata;
	if(codec) delete codec;
//	if(framereader) delete framereader;
	Clean();
}




/*-------------------------------------------------------------------
//	SNDFILEFFMPEG::LoadFile
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/11/2016 16:57:25
//	
//	@return 			bool : 
//
//  @param				path : 
//  @param				name : 
//  @param				streaming : 
*/
/*-----------------------------------------------------------------*/
bool SNDFILEFFMPEG::LoadFile(XCHAR* path, XCHAR* name, bool streaming)
{
	this->name.Set(name);
	XFILE* xfile = NULL;

  bool		status = false; 

	xfile = xfactory->Create_File();
	if(!xfile) 
		{
			return false; 
		}
  	
	if(!xfile->Open(path))		
		{
			xfactory->Delete_File(xfile);
			xfile = NULL;

			return false;
		}

  xfactory->Delete_File(xfile);

	pathtoopen.Set(path);

	codec = new SNDCODECFFMPEG();
//	framereader = new MEDIAFRAMEREADERLIBAV();

	//framereader->SetSoundEnabled(true);

	//framereader->OpenFile(path);

	XBUFFER decodeddata(false);

	codec->SetFile(path);
	codec->Ini();
	
	if(!streaming)
		{
			xbufferdecodeddata = new XBUFFER(false);
			if(!xbufferdecodeddata)
				{
					return false;
				}

			MEDIAPACKET packet;

			XDWORD realsize = 0;
			XDWORD position = 0;
			//XDWORD toresize = 10000;
			XDWORD toresize = 10000000;
			decodeddata.Resize(toresize);

			while(!codec->GetFrameReader()->IsEOF())
				{
					codec->GetFrameReader()->ReadFrame(&packet);

					XBUFFER newdata(false);

					codec->DecodePacket(&newdata, &packet);

					//xbuffer->Add(newdata);
					if(realsize + newdata.GetSize() > decodeddata.GetSize())
						{
							decodeddata.Resize(decodeddata.GetSize() + toresize, false);
						}

					memcpy(decodeddata.Get() + position, newdata.Get(), newdata.GetSize());
					realsize += newdata.GetSize();
					position += newdata.GetSize();
				}

			decodeddata.Resize(realsize, false);
			xbufferdecodeddata->Add(decodeddata);

			duration =   ((SNDCODECFFMPEG*)codec)->GetDuration();
			channels =   ((SNDCODECFFMPEG*)codec)->GetChannels();
			samplerate = ((SNDCODECFFMPEG*)codec)->GetFrequency();
			samples =    ((SNDCODECFFMPEG*)codec)->GetSamples();

			delete codec;
			codec					= NULL;
		}
	else
		{
			duration =   ((SNDCODECFFMPEG*)codec)->GetDuration();
			channels =   ((SNDCODECFFMPEG*)codec)->GetChannels();
			samplerate = ((SNDCODECFFMPEG*)codec)->GetFrequency();
			samples =    ((SNDCODECFFMPEG*)codec)->GetSamples();
		}

	

	return true;
}



/*-------------------------------------------------------------------
//	SNDFILEFFMPEG::LoadFile
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			21/11/2016 16:57:31
//	
//	@return 			bool : 
//
//  @param				xpath : 
//  @param				name : 
//  @param				streaming : 
*/
/*-----------------------------------------------------------------*/
bool SNDFILEFFMPEG::LoadFile(XPATH& xpath, XCHAR* name, bool streaming)
{
	return LoadFile(xpath.Get(), name, streaming);
}



/*-------------------------------------------------------------------
//	SNDFILEFFMPEG::Reset
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			20/01/2017 12:04:59
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool SNDFILEFFMPEG::Reset()
{
	if(codec)
		{
			delete codec;
		}

	codec = new SNDCODECFFMPEG();

	codec->SetFile(pathtoopen.Get());
	codec->Ini();

	return true;
}
