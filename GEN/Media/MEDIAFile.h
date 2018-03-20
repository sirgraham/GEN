/*------------------------------------------------------------------------------------------
//	MEDIAFILE.H
*/	
/**	
// \file 
//   
//  base file for media
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 29/11/2016 13:41:31
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _MEDIAFILE_H_
#define _MEDIAFILE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"
#include "MEDIACodec.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class MEDIACODEC;
class MEDIAFRAMEREADER;
class XBUFFER;

class MEDIAFILE
{
public:

												MEDIAFILE												()
												{
													Clean();
												}

	virtual							 ~MEDIAFILE												()
												{
													Clean();
												}

	MEDIACODEC*						GetCodec												()																								{ return codec;										}
	MEDIAFRAMEREADER*			GetFrameReader									()																								{ return codec->GetFrameReader(); }

	virtual bool					DecodePacket										(MEDIACODEC* packet, XBUFFER* output);
	virtual bool					DecodeAll												(XBUFFER* output);

protected:
	MEDIACODEC*						codec;
	//MEDIAFRAMEREADER*			framereader;
private:
	void									Clean														()
												{
													codec					= NULL;
		//											framereader		= NULL;
												}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

