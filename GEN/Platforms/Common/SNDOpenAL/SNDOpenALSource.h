/*------------------------------------------------------------------------------------------
//	SNDOPENALSOURCE.H
*/	
/**	
// \file 
//   
//  source of a sound
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 05/11/2015 10:16:28
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _SNDOPENALSOURCE_H_
#define _SNDOPENALSOURCE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdlib.h>

#include <AL/al.h>
#include <AL/alc.h>


#include "SNDSource.h"
#include "XList.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class SNDOPENALBUFFER;
class SNDOPENALELEMENT;
class SNDOPENAL;

class SNDOPENALSOURCE : public SNDSOURCE
{
	friend class SNDOPENAL;
	friend class SNDOPENALELEMENT;
	friend class SNDOPENALSTREAMELEMENT;

	public:

		void												Stop																();
		void												Pause																();
		void												UnPause															();

		void												SetLoop															(bool loop);

		bool												IsPLaying														();
		bool												IsStopped														();
		bool												IsPaused														();

		void												SetVolume														(float volume);
		float												GetVolume														();

		void												SetPitch														(float pitch);
		float												GetPitch														();

		void												SetElement													(SNDELEMENT* element)						{ this->element = element;	}
		SNDELEMENT*									GetElement													()															{ return this->element;			}

		void												SetSecondsOffset										(float seconds);
		void												SetSamplesOffset										(int samples);
			
		void												SetAquired													(bool aquired)									{ this->aquired = aquired;	}
		bool												IsAquired														()															{ return this->aquired;			}

		void												Aquire															()															{ this->aquired = true;			}
		void												Release															()															{ this->aquired = false;		}

	protected:

		ALuint											source;
		SNDOPENAL*									openalsystem;

		bool												isplaying;

		bool												aquired;

		XLIST<SNDOPENALBUFFER*>			bufferlist;

	private:
																SNDOPENALSOURCE											(SNDOPENAL* openalsystem);
		virtual										 ~SNDOPENALSOURCE											();
		
		void												Play																();
		void												Play																(SNDOPENALBUFFER* buffer);
		void												Queue																(SNDOPENALBUFFER* buffer);
		void												UnQueue															(SNDOPENALBUFFER* buffer);

		int													GetQueueLength											();
		int													GetProcessedBuffers									();


		void												Clean																()
																{
																	source				= 0;
																	element				= NULL;
																	openalsystem	= NULL;
																	isplaying			= false;
																	aquired				= false;
																}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

