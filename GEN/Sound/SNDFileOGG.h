/*------------------------------------------------------------------------------------------
//	SNDFILEOGG.H
*/	
/**	
// \file 
//   
//  Loads and decodes an ogg file
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 03/11/2015 10:38:05
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _SNDFILEOGG_H_
#define _SNDFILEOGG_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XBase.h"
#include "XFile.h"	
#include "XPath.h"

#include "SNDFile.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

struct stb_vorbis;

class SNDFILEOGG : public SNDFILE
{
	public:

														SNDFILEOGG											() : SNDFILE()
														{
															Clean();
														}

		virtual								 ~SNDFILEOGG											()
														{
															Clean();
															if(xbuffer) delete xbuffer;
															if(xbufferdecodeddata) delete xbufferdecodeddata;
														}

		bool										LoadFile												(XCHAR* path, XCHAR* name, bool streaming);
		bool										LoadFile												(XPATH& xpath, XCHAR* name, bool streaming);

		

	protected:

	private:

		void										Clean														()
														{
															stream	= 0;
														}

		stb_vorbis*							stream;
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

