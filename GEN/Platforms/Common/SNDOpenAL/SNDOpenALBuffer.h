/*------------------------------------------------------------------------------------------
//	SNDOPENALBUFFER.H
*/	
/**	
// \file 
//   
//  wraps an openal buffer
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 04/11/2015 17:28:55
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _OPENALBUFFER_H_
#define _OPENALBUFFER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"

#include <AL/al.h>
#include <AL/alc.h>
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class SNDOPENALBUFFER
{
	public:

								SNDOPENALBUFFER							() 
								{
									Clean();
								}

		virtual		 ~SNDOPENALBUFFER							()
								{
									Clean();
								}

		void				Generate										();
		void				Destroy											();

		void				Upload											(XWORD channels, void* data, XDWORD size, XWORD freq);

		ALuint			GetHandle										()																														{ return buffer; }

		ALuint			buffer;

	protected:

	private:

		void				Clean												()
								{
									buffer = 0;
								}
	
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

