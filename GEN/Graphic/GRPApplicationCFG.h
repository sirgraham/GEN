/*------------------------------------------------------------------------------------------
// GRPApplicationCFG.HPP
*/
/**
// \class
//
// Application basic graphic and sound settings
//
//	@author: Diego Martinez Ruiz de Gaona 
//
//	Date of Creation : 23/10/2017 17:35:31 
//	Last Modification : 
*/
/*	GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


#ifndef _GRPAPPLICATIONCFG_HPP
#define _GRPAPPLICATIONCFG_HPP

/*---- INCLUDES --------------------------------------------------------------------------*/


#include "DIORemoteFileConfig.h"
#include "XDebug.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#ifdef DIO_ACTIVE 
#define GRPAPPLICATIONCONFIGBASE DIOREMOTEFILECONFIG
#else
#define GRPAPPLICATIONCONFIGBASE XFILECONFIG
#endif


/*---- CLASS -----------------------------------------------------------------------------*/

class GRPAPPLICATIONCONFIG : public GRPAPPLICATIONCONFIGBASE
{
	public:

																		GRPAPPLICATIONCONFIG										(XCHAR* namefile) : GRPAPPLICATIONCONFIGBASE(namefile)		
																		{
																			XSTRING section;
																			XSTRING key;

																			Clean();

																			//-----------------------------------------------------
																			// GENERAL	
																			for(int c=0; c<XDEBUG_MAXNTARGETS; c++)
																				{
																					key.Format(__L("%s%02d"), __L("xdebugtarget"), c + 1);
																					AddValue(XFILECONFIGVALUETYPE_STRING, __L("general"), key.Get(), &xdebugtarget[c]);
																				}

																			//-----------------------------------------------------
																			// SCREEN																			
																			AddValue(XFILECONFIGVALUETYPE_BOOLEAN	, __L("screen")	, __L("isfull")					, &screenisfull);
																			AddValue(XFILECONFIGVALUETYPE_INT			, __L("screen")	, __L("positionx")			, &screenpositionx);
																			AddValue(XFILECONFIGVALUETYPE_INT			, __L("screen")	, __L("positiony")			, &screenpositiony);
																			AddValue(XFILECONFIGVALUETYPE_INT			, __L("screen")	, __L("resolutionx")		, &screenresolutionx);
																			AddValue(XFILECONFIGVALUETYPE_INT			, __L("screen")	, __L("resolutiony")		, &screenresolutiony);

																			//-----------------------------------------------------
																			// SOUND	
																			AddValue(XFILECONFIGVALUETYPE_BOOLEAN	, __L("sound")	, __L("isactive")				, &soundisactive);
																			AddValue(XFILECONFIGVALUETYPE_INT			, __L("sound")	, __L("generalvolume")	, &soundgeneralvolume);

																			
																			Ini();
																		}

		virtual												 ~GRPAPPLICATIONCONFIG											()
																		{
																			End();
																			Clean();
																		}

		XSTRING*                        GetXDebugTarget														(int index)
																		{
																			if(index<0)											return NULL;
																			if(index >= XDEBUG_MAXNTARGETS) return NULL;

																			return &xdebugtarget[index];
																		}
		
		bool														IsFullScreen															()														{ return screenisfull;																}
		void														GetResolution															(XDWORD* rx, XDWORD* ry)			
																		{ 
																			if(rx) *rx = screenresolutionx;	
																			if(ry)	*ry = screenresolutiony;	
																		}
		void														GetScreenPosition													(XDWORD* px, XDWORD *py)			
																		{ 
																			if(px) *px = screenpositionx;		
																			if(py) *py = screenpositiony;		
																		}

		//sound -----------------------------------------------

		bool														Sound_IsActive														()														{ return soundisactive;																}
		int															Sound_GetGeneralVolume										()														{ return soundgeneralvolume;													}


		virtual bool										Default																		()
																		{
																				xdebugtarget[0].Format(__L("%d,%s"), XDEBUGCTRLTYPE_NET, XDEBUG_DEFAULT_NETAIM1);
																				xdebugtarget[1].Format(__L("%d,%s"), XDEBUGCTRLTYPE_NET, XDEBUG_DEFAULT_NETAIM2);
																				xdebugtarget[2].Format(__L("%d,%s"), XDEBUGCTRLTYPE_NET, XDEBUG_DEFAULT_NETAIM3);
																				xdebugtarget[3].Format(__L("%d,%s"), XDEBUGCTRLTYPE_NET, XDEBUG_DEFAULT_NETAIM4);
																				xdebugtarget[4].Format(__L("%d,%s"), XDEBUGCTRLTYPE_NET, XDEBUG_DEFAULT_NETAIM5);

																				screenisfull				= true;
																				screenresolutionx		= 1280;
																				screenresolutiony		= 720;
																				screenpositionx			= 0;
																				screenpositiony			= 0;

																				soundisactive			 	= true;
																				soundgeneralvolume	= 100;     // percent

																				return true;
																		}

	private:

		void														Clean																			()
																		{
																			screenisfull				= false;
																			screenresolutionx		= 0;
																			screenresolutiony		= 0;

																			soundisactive				= true;
																			soundgeneralvolume	= 100;

																		}

		
		XSTRING                         xdebugtarget[XDEBUG_MAXNTARGETS];

		bool														screenisfull;
		XDWORD													screenresolutionx;
		XDWORD													screenresolutiony;
		XDWORD													screenpositionx;
		XDWORD													screenpositiony;

		bool														soundisactive;
		int                             soundgeneralvolume;

};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif