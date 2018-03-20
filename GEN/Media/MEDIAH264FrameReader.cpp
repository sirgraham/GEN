
/*------------------------------------------------------------------------------------------
//	MEDIAH264FRAMEREADER.CPP
//	
//	H264 Frame Reader
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 11/04/2016 10:45:04
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "MEDIAH264FrameReader.h"
#include "XFactory.h"	
#include "XDebug.h"

//// yes yes I know this seems ugly and a crazy idea, but they have conflicts with new
//#undef new
//#include "libutil/util.h"
//#include "mp4file.h"

// this header will redefine new
#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//	MEDIAH264FRAMEREADER::MEDIAH264FRAMEREADER
*/	
/**	
//	
//	Class Constructor VIDEOH264FRAMEREADER
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 11:25:47
//	
*/
/*-----------------------------------------------------------------*/
MEDIAH264FRAMEREADER::MEDIAH264FRAMEREADER()
{
	Clean();
	this->SetReadBufferSize(100*1024); // 100KB

	ThreeBytesNAL		= 0x00FFFFFFu;
	FourBytesNAL		= 0xFFFFFFFFu;
	NALUID					= 0x00000001u;
	EraseByte				= 0xFFFFFF00u;
	SWAPDWORD(ThreeBytesNAL);
	SWAPDWORD(FourBytesNAL);
	SWAPDWORD(NALUID);
	SWAPDWORD(EraseByte);
}





/*-------------------------------------------------------------------
//	MEDIAH264FRAMEREADER::~MEDIAH264FRAMEREADER
*/	
/**	
//	
//	 Class Destructor VIDEOH264FRAMEREADER
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 11:25:53
//	
*/
/*-----------------------------------------------------------------*/
MEDIAH264FRAMEREADER::~MEDIAH264FRAMEREADER()
{
	Clean();
}




/*-------------------------------------------------------------------
//	MEDIAH264FRAMEREADER::OpenFile
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 11:26:02
//	
//	@return 			bool : 
//
//  @param				filename : 
*/
/*-----------------------------------------------------------------*/
bool MEDIAH264FRAMEREADER::OpenFile(XCHAR* filename)
{
	////xfile = xfactory->Create_File();
	////if(!xfile) return false;
	////
	////if(!xfile->Open(filename))
	////	{
	////		xfactory->Delete_File(xfile);
	////		return false;
	////	}
	////
	////// set the read buffer to some nice size
	////readbuffer.Resize(readbuffersize);
	////lefttoread = 0; // make sure we will read the temprary buffer
	//
	//// lets make first an ugly test
	//char* str = NULL;
	//XSTRING xstr(filename);
	//xstr.CreateOEM(str);
	//mp4file = (mp4v2::impl::MP4File*)MP4Read(str);
	//
	//isample = 1;
	//
	//// use GetBytesProperty
	//// query avcC.numOfSequenceParameterSets
	////uint64_t spsnumber = mp4file->GetTrackIntegerProperty(1, "mdia.minf.stbl.stsd.avc1.avcC.numOfSequenceParameterSets");
	//// use avcC.sequenceEntries.sequenceParameterSetLength
	////uint64_t spssize = mp4file->GetTrackIntegerProperty(1, "mdia.minf.stbl.stsd.avc1.avcC.sequenceEntries.sequenceParameterSetLength");
	//// use avcC.sequenceEntries.sequenceParameterSetNALUnit
	//
	//// SPS/PPS need to be read at file load as they are out of normal NAL stream
	//
	//XBYTE* ptr = NULL;
	//uint32_t spss = 0;
	//
	//mp4file->GetTrackBytesProperty(1, "mdia.minf.stbl.stsd.avc1.avcC.sequenceEntries.sequenceParameterSetNALUnit", &ptr, &spss);
	//
	//// need to copy over with 4 bytes of margin to add the Annex B header
	//
	//SPS.Resize(spss + 4);
	//memcpy(SPS.GetPointer(4), ptr, spss);
	//free(ptr);
	//
	//ptr = SPS.GetPointer(0);
	//ptr[0] = 0x00;
	//ptr[1] = 0x00;
	//ptr[2] = 0x00;
	//ptr[3] = 0x01;
	//
	//// query avcC.numOfPictureParameterSets
	//// use avcC.pictureEntries.pictureParameterSetLength
	//// use avcC.pictureEntries.pictureParameterSetNALUnit
	//
	//ptr = NULL;
	//uint32_t ppss = 0;
	//
	//mp4file->GetTrackBytesProperty(1, "mdia.minf.stbl.stsd.avc1.avcC.pictureEntries.pictureParameterSetNALUnit", &ptr, &ppss);
	//
	//// need to copy over with 4 bytes of margin to add the Annex B header
	//
	//PPS.Resize(ppss + 4);
	//memcpy(PPS.GetPointer(4), ptr, ppss);
	//free(ptr);
	//
	//ptr = PPS.GetPointer(0);
	//ptr[0] = 0x00;
	//ptr[1] = 0x00;
	//ptr[2] = 0x00;
	//ptr[3] = 0x01;
	//
	//width = MP4GetTrackVideoWidth(mp4file, 1);
	//height = MP4GetTrackVideoHeight(mp4file, 1);

	return true;
}





/*-------------------------------------------------------------------
//	MEDIAH264FRAMEREADER::OpenFile
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 11:26:10
//	
//	@return 			bool : 
//
//  @param				filename : 
*/
/*-----------------------------------------------------------------*/
bool MEDIAH264FRAMEREADER::OpenFile(XSTRING* filename)
{
	return OpenFile(filename->Get());
}





/*-------------------------------------------------------------------
//	VIDEOH264FRAMEREADER::ReadFrame
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 11:26:17
//	
//	@return 			bool : 
//
//  @param				xvector : 
*/
/*-----------------------------------------------------------------*/

bool MEDIAH264FRAMEREADER::ReadFrame(XVECTOR<XBYTE> & xvector, long long* timestamp, long long* decompression)
{
	//xvector.SetAddInLimit(readbuffersize);
	//// but we need to get the SPS and the PPS before starting passing these frames!
	//
	//uint32_t numtracks = MP4GetNumberOfTracks(mp4file);
	//uint32_t numsamples = MP4GetTrackNumberOfSamples(mp4file, 1);
	//
	//uint8_t* pSample = NULL;
  //uint32_t sampleSize = 0;
	//
	//
	//
	//// if number of frames is smaller than 2 we need to pass the SPS and the PPS in that order
	//
	//if(readframes == 0)
	//{
	//	xvector.Resize(SPS.GetSize());
	//	memcpy(xvector.GetPointer(0), SPS.GetPointer(0), SPS.GetSize());
	//}
	//else if(readframes == 1)
	//{
	//	xvector.Resize(PPS.GetSize());
	//	memcpy(xvector.GetPointer(0), PPS.GetPointer(0), PPS.GetSize());
	//}
	//else
	//{
	//	uint64_t samplesizesize = mp4file->GetTrackIntegerProperty(1, "mdia.minf.stbl.stsd.avc1.avcC.lengthSizeMinusOne");
	//	samplesizesize += 1;
	//
	//	uint8_t* pSample = NULL;
	//	uint32_t sampleSize = 0;
	//
	//	if(isample <= numsamples)
	//	{
	//		MP4ReadSample(mp4file, 1, isample, &pSample, &sampleSize);
	//
	//		if(samplesizesize == 4)
	//			{
	//				pSample[0] = 0x00;
	//				pSample[1] = 0x00;
	//				pSample[2] = 0x00;
	//				pSample[3] = 0x01;
	//			}
	//		else if(samplesizesize == 3)
	//			{
	//				pSample[0] = 0x00;
	//				pSample[1] = 0x00;
	//				pSample[2] = 0x01;
	//			}
	//
	//		isample++;
	//
	//		xvector.Resize(sampleSize);
	//		memcpy(xvector.GetPointer(0), pSample, sampleSize);
	//	}
	//}
	//
	//readframes++;

	return true;
}





/*-------------------------------------------------------------------
//	VIDEOH264FRAMEREADER::ReadStream
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			13/04/2016 17:36:17
//	
//	@return 			bool : 
//
//  @param				xvector : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
bool MEDIAH264FRAMEREADER::ReadStream(XVECTOR<XBYTE> & xvector, XDWORD size)
{
	if(xvector.GetSize() < size)
		{
			xvector.Resize(size);
		}

	return xfile->Read(xvector.GetPointer(0), size);
}





bool MEDIAH264FRAMEREADER::IsEndOfStream()
{
	int pos = 0;

	if(xfile)
		{
			xfile->GetPosition(pos);
			if(pos >= xfile->GetSize())
				{
					return true;
				}
		}

	return false;
}





/*-------------------------------------------------------------------
//	MEDIAH264FRAMEREADER::ResetStream
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			14/04/2016 14:23:12
//	
*/
/*-----------------------------------------------------------------*/
void MEDIAH264FRAMEREADER::ResetStream()
{
	if(xfile) xfile->SetPosition(0);
}




/*-------------------------------------------------------------------
//	MEDIAH264FRAMEREADER::NALToRBSP
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			14/04/2016 13:10:00
//	
//	@return 			int : 
//
//  @param				NAL : 
//  @param				RBSP : 
*/
/*-----------------------------------------------------------------*/
int MEDIAH264FRAMEREADER::NALToRBSP(XVECTOR<XBYTE> & NAL, XVECTOR<XBYTE>& RBSP)
{
	int i;
	int j     = 0;
	int count = 0;
  
	for(i = 1; i < (int)NAL.GetSize(); i++)
		{ 
			// in NAL unit, 0x000000, 0x000001 or 0x000002 shall not occur at any byte-aligned position
			if(( count == 2 ) && ( NAL.FastGet(i) < 0x03)) 
				{
					return -1;
				}

			if(( count == 2 ) && ( NAL.FastGet(i) == 0x03))
				{
					// check the 4th byte after 0x000003, except when cabac_zero_word is used, in which case the last three bytes of this NAL unit must be 0x000003
					if((i < (int)NAL.GetSize() - 1) && (NAL.FastGet(i+1) > 0x03))
						{
							return -1;
						}

					// if cabac_zero_word is used, the final byte of this NAL unit(0x03) is discarded, and the last two bytes of RBSP must be 0x0000
					if(i == NAL.GetSize() - 1)
						{
							break;
						}

					i++;
					count = 0;
				}

			if( j >= (int)RBSP.GetSize() ) 
				{
					// error, not enough space
					return -1;
				}	

			RBSP.Set(j, NAL.FastGet(i));
			if(NAL.FastGet(i) == 0x00)
				{
					count++;
				}
			else
				{
					count = 0;
				}
			j++;
		}

	return j;
}


/*-------------------------------------------------------------------
//	MEDIAH264FRAMEREADER::SetReadBufferSize
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 11:26:26
//	
//  @param				readbuffersize : 
*/
/*-----------------------------------------------------------------*/
void MEDIAH264FRAMEREADER::SetReadBufferSize(XDWORD readbuffersize)
{
	this->readbuffersize = readbuffersize;
}




/*-------------------------------------------------------------------
//	MEDIAH264FRAMEREADER::GetReadBufferSize
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 11:26:33
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD MEDIAH264FRAMEREADER::GetReadBufferSize()
{
	return this->readbuffersize;
}





/*-------------------------------------------------------------------
//	MEDIAH264FRAMEREADER::FillReadBuffer
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 11:26:40
//	
*/
/*-----------------------------------------------------------------*/
void MEDIAH264FRAMEREADER::FillReadBuffer()
{
	int size = (int)readbuffersize;
	xfile->Read(readbuffer.GetPointer(0), &size);
	lefttoread = size;
}




/*-------------------------------------------------------------------
//	MEDIAH264FRAMEREADER::GetSPS
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/05/2016 11:54:14
//	
//	@return 			XVECTOR<XBYTE>* : 
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<XBYTE>* MEDIAH264FRAMEREADER::GetSPS()
{
	return &SPS;
}






/*-------------------------------------------------------------------
//	MEDIAH264FRAMEREADER::GetPPS
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/05/2016 11:54:20
//	
//	@return 			XVECTOR<XBYTE>* : 
//
*/
/*-----------------------------------------------------------------*/
XVECTOR<XBYTE>* MEDIAH264FRAMEREADER::GetPPS()
{
	return &PPS;
}

