/*------------------------------------------------------------------------------------------
//	SNDMANAGER.H
*/	
/**	
// \class 
//   
//  Manages sound libraries
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 15/06/2017 11:09:50
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _SNDMANAGER_H_
#define _SNDMANAGER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPObserver.h"
#include "GRPSubject.h"
#include "XMap.h"
#include "XPath.h"
#include "XString.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
class SNDELEMENT;

#define SNDPLAYLIST XVECTOR<SNDELEMENT*>

class SNDMANAGER
{
	public:		
		
		static void														Destroy						();
		static SNDMANAGER*										Get								();

		XMAP<XSTRING*,SNDPLAYLIST*>*					GetPlaylists			()																										{   return &playlists;						}													
		SNDPLAYLIST*													GetPlaylist				(XCHAR* name);
		void																	Add								(XPATH& xpath, XSTRING samplename, XSTRING	playlist);
		void																	Play							(XSTRING samplename, XSTRING	playlist,	float volume);
		void																	Stop							(XSTRING samplename, XSTRING	playlist);

	protected:		
																					SNDMANAGER				();
																			 	 ~SNDMANAGER				();

		SNDELEMENT*														GetSample					(XSTRING& name,				XSTRING& playlist);
		XDWORD																GetSubjectType		()																										{		return SND_MANAGER;						}
		bool																	Notify						(GRPMSGTYPE msg,			GRPSUBJECT*)										{		return true;									}
						

		static SNDMANAGER*										instance;
		XMAP<XSTRING*,SNDPLAYLIST*>						playlists;
};


	
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

