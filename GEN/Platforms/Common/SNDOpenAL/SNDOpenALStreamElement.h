/*------------------------------------------------------------------------------------------
//	SNDOPENALSTREAMELEMENT.H
*/	
/**	
// \file 
//   
//  openal stream elemenet
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 20/07/2016 10:45:03
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _SNDOPENALSTREAMELEMENT_H_
#define _SNDOPENALSTREAMELEMENT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "SNDStreamElement.h"
#include "SNDOpenALSource.h"
#include "XList.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class SNDOPENALBUFFER;
class SNDOPENALSOURCE;

class SNDOPENALSTREAMELEMENT : public SNDSTREAMELEMENT
{
	friend class SNDOPENAL;

public:

																		SNDOPENALSTREAMELEMENT										();
	virtual													 ~SNDOPENALSTREAMELEMENT										();

	virtual void											Play																			();
	virtual void											AddData																		(XDWORD size, void* data);

	void															SetSource																	(SNDOPENALSOURCE* source)																	{ this->source = source;	}
	SNDOPENALSOURCE*									GetSource																	()																												{ return source;					}

	virtual void											SetFile																		(SNDFILE* file);
	void															ClearBuffers();

protected:

	XLIST<SNDOPENALBUFFER*>						freebuffers;
	XLIST<SNDOPENALBUFFER*>						queuedbuffers;

	SNDOPENALSOURCE*									source;

	XDWORD														totalbuffers;

private:
	void															Clean																			()
																		{
																			totalbuffers	= 0;
																			source				= NULL;
																		}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

