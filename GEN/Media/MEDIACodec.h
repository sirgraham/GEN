/*------------------------------------------------------------------------------------------
//	MEDIACODEC.H
*/	
/**	
// \file 
//   
//  base codec class
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 29/11/2016 12:40:49
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _MEDIACODEC_H_
#define _MEDIACODEC_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"
#include "XString.h"	

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class MEDIAFRAMEREADER;
struct MEDIAPACKET;
class XBUFFER;

class MEDIACODEC
{
public:

																		MEDIACODEC																			()
																		{
																			Clean();
																		}

	virtual													 ~MEDIACODEC																			()
																		{
																			Clean();
																		}

	virtual MEDIAFRAMEREADER*					GetFrameReader																	() = 0;

	virtual bool											SetFile																					(XCHAR* filename)
																		{
																			//xfile = xfactory->Create_File();
																			//if(!xfile) return false;
																			//
																			//if(!xfile->Open(filename))
																			//{
																			//	xfactory->Delete_File(xfile);
																			//}
																			//
																			return true;
																		}


	virtual bool											Ini																							()																													{ return true;																}
	virtual bool											End																							()																													{ return true;																}

	virtual bool											SetFile																					(XSTRING* filename)																					{ return this->SetFile(filename->Get());			}

	virtual void											Decode																					()																													{																							}
	virtual bool											DecodePacket																		(XBUFFER* xbuffer, MEDIAPACKET* packet)											{	return true;																}
	virtual void											Encode																					()																													{																							}

	virtual bool											IsPlaying																				()																													{ return this->isplaying;											}

	virtual void											Update																					()																													{																							}

	bool															eos;

protected:
	MEDIAFRAMEREADER *								framereader;
	bool															isplaying;
private:
	void															Clean																						()
																		{
																			framereader		= NULL;
																			isplaying			= false;
																		}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

