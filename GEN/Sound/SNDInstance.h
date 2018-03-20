/*------------------------------------------------------------------------------------------
//	SNDINSTANCE.H
*/	
/**	
// \file 
//   
//  instance of a playing sound
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 22/01/2016 10:34:13
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _SNDINSTANCE_H_
#define _SNDINSTANCE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"
#include "XObserver.h"
	
#include "SNDFactory.h"
#include "SNDSource.h"
#include "SNDElement.h"
#include "SNDEvent.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class SNDSOURCE;
class SNDELEMENT;
class SNDFACTORY;

class SNDINSTANCE : public XOBSERVER
{
	public:
													SNDINSTANCE																(SNDFACTORY* sndsystem, SNDSOURCE* source, SNDELEMENT* element)
													{
														Clean();

														this->sndsystem = sndsystem;
														this->source		= source;
														this->element		= element;
														this->managed		= true; // if managed it will mark itself for deletion
														this->isplaying = true;
													}

		virtual							 ~SNDINSTANCE																()
													{
														Clean();
													}


		bool									IsPlaying																	()
													{
														return isplaying;
													}

		bool									IsPaused																	()
													{
														return ispaused;
													}

		// these functions are only valid if IsPlaying returns true
		bool									Stop																			()
													{
														if(isplaying)
															{
																source->Stop();
															}
														return true;
													}

		bool									Pause																			()
													{
														if(ispaused && isplaying)
															{
																source->Pause();
															}
														return true;
													}

		bool									UnPause																		()
													{
														if(!ispaused && isplaying)
															{
																source->UnPause();
															}
														return true;
													}

		float									GetVolume																	()
													{
														if(isplaying)
															{
																return source->GetVolume();
															}

														return 0.0f;
													}

		void									SetVolume																	(float v)
													{
														if(isplaying)
															{
																source->SetVolume(v);
															}
													}

		float									GetPitch																	()
													{
														if(isplaying)
															{
																return source->GetPitch();
															}

														return 0.0f;
													}

		void									SetPitch																	(float p)
													{
														if(isplaying)
															{
																source->SetPitch(p);
															}
													}

		//bool									Notify																		(GRPMSGTYPE msg,GRPSUBJECT* notifier)
		void									HandleEvent																(XEVENT* xevent)
													{
														if(xevent->GetEventType() == XEVENTTYPE_SOUND)
															{
																SNDEVENT* sndevent = (SNDEVENT*)xevent;

																if(SNDEVENT_TYPE_PLAY != sndevent->GetSNDType())
																	{
																		//isplaying = source->IsPLaying();
																		isplaying = false;
																		ispaused = source->IsPaused();

																		if(!isplaying && managed)
																			{
																				//source->SetInstance(NULL); // this breaks element instance and my logic, need to find new ways to handle it
																				sndsystem->MarkInstanceForDeletion(this);
																			}
														
																	}
															}
													}

		bool									GetManaged																()
													{
														return managed;
													}

		void									SetManaged																(bool managed)
													{
														this->managed = managed;
													}

		SNDSOURCE*						GetSource																	()
													{
														return source;
													}

		SNDELEMENT*						GetElement																	()
													{
														return element;
													}


	protected:

		SNDFACTORY*						sndsystem;
		SNDSOURCE*						source;
		SNDELEMENT*						element;

		bool									isplaying;
		bool									ispaused;

		bool									managed;

	private:
		void									Clean																			()
													{
														source			= NULL;
														element			= NULL;
														isplaying		= false;
														ispaused		= false;
														managed			= false;
													}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

