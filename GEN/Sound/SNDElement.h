//------------------------------------------------------------------------------------------
//	SNDELEMENT.H
//	
/**	
// \class 
//   
//  Sound Base Class
//   
//	@author	 Abraham J. Velez
//	@version 29/04/2004 15:25:41
*/	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
#ifndef _SNDELEMENT_H_
#define _SNDELEMENT_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XString.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum SNDELEMENTTYPE
{
	SNDELEMENT_UNKNOWN		= 0 ,
	SNDELEMENT_FILE					,
	SNDELEMENT_NOTE				  ,	
};

enum SNDELEMENTSTATUS
{
	SNDELEMENT_STOP			= 0 ,
	SNDELEMENT_PLAY				  ,		
	SNDELEMENT_PAUSE				,
};

	
//---- CLASS -------------------------------------------------------------------------------

class SNDFILE;

class SNDELEMENT
{
	public:
															SNDELEMENT						();
		virtual									 ~SNDELEMENT						();		

		XSTRING*									GetNameFile						();

		virtual void							SetLoop								(bool loop)											{ this->loop = loop;					}
		virtual bool							GetLoop								()															{return loop;									}

		virtual void							SetVolume							(float volume)									{ this->volume = volume;			}
		virtual float							GetVolume							()															{ return volume;							}

		virtual void							SetPitch							(float pitch)										{ this->pitch = pitch;				}
		virtual float							GetPitch							()															{ return pitch;								}

		void											SetDuration						(float duration)								{	this->duration = duration;	}
		float											GetDuration						()															{ return this->duration;			}

		void											SetSamples						(int samples)										{ this->samples = samples;		}
		int												GetSamples						()															{	return this->samples;				}

		bool											IsStreamer						()															{ return streamer;						}

		virtual void							SetFile								(SNDFILE* file)									{ this->file = file;					}
		SNDFILE*									GetFile								()															{ return this->file;					}

	protected:
		
		XSTRING										namefile;
		SNDFILE*									file;

		bool											loop;
		float											volume;
		float											pitch;

		float											duration;
		int												samples;
		bool											streamer;

	private:

		void											Clean									()
															{
																file			= NULL;
																loop			= false;
																volume		= 0.0f;
																pitch			= 0.0f;
																duration	= 0.0f;
																samples		= 0;
																streamer	= false;
															}
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

