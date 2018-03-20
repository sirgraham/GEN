
/*------------------------------------------------------------------------------------------
//	GRPPROFILER.CPP
//	
//	Profile Graphics
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 14/10/2015 10:23:27
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPProfiler.h"
#include "XDebug.h"
#include "XMemory.h"
#include "XTimer.h"
#include "XFactory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
#define GRPPROFILER_SHOWFRAMERATEEVERY			60												
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

XDWORD		GRPPROFILER::showFrameEvery					= GRPPROFILER_SHOWFRAMERATEEVERY;


XDWORD		GRPPROFILER::frames									=0;
XDWORD		GRPPROFILER::Passes									=0;
XDWORD		GRPPROFILER::Scenes									=0;
XDWORD		GRPPROFILER::framesFromLastMeasure	=0;
XDWORD		GRPPROFILER::measures								=1;	
double		GRPPROFILER::AddFrames							=0;
XDWORD		GRPPROFILER::fps										=0;
double		GRPPROFILER::fpsAvg									=0;
double		GRPPROFILER::ft											=0;

XDWORD		GRPPROFILER::nShaderChanges					=0;
XDWORD		GRPPROFILER::nBufferChanges					=0;
XDWORD		GRPPROFILER::nTextureChanges				=0;
XDWORD		GRPPROFILER::nRegisterAccess				=0;
XDWORD		GRPPROFILER::nMatrixOperations			=0;
XDWORD		GRPPROFILER::nFacesRendered					=0;
XDWORD		GRPPROFILER::nDrawCalls							=0;
XDWORD		GRPPROFILER::nNodes									=0;
XDWORD		GRPPROFILER::nElements							=0;
XDWORD		GRPPROFILER::nCulledElements			  =0;
XDWORD		GRPPROFILER::nNodesUpdates					=0;
XDWORD		GRPPROFILER::vertexmemory						=0;
XDWORD		GRPPROFILER::texturememory					=0;
XDWORD		GRPPROFILER::DomainChecks						=0;
XTIMER*		GRPPROFILER::xtimershowframerate		=NULL;
bool			GRPPROFILER::showstatistics					= true;
XDWORD		GRPPROFILER::last_measures					[GRPPROFILER::nmeasures];


/*-------------------------------------------------------------------
//	GRPPROFILER::SetShowStatistics
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			30/09/2016 10:23:06
//	
//  @param				showstatistics : 
*/
/*-----------------------------------------------------------------*/
void GRPPROFILER::SetShowStatistics(bool showstatistics)
{
	GRPPROFILER::showstatistics = showstatistics;
}


/*-------------------------------------------------------------------
//	GRPPROFILER::ShowStatistics
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			12/07/2016 16:39:32
//	
*/
/*-----------------------------------------------------------------*/
void		GRPPROFILER::ShowStatistics()
{
	static XDWORD n=0;

	if (xtimershowframerate==NULL)
	{
		xtimershowframerate = xfactory->CreateTimer();
		if(!xtimershowframerate) return;
	}

	//--------------------------------------------------------------------------------------
	int secs=xtimershowframerate->GetMeasureSeconds();
	if (secs >= int(showFrameEvery))
	{
	
	GRPPROFILER::ft											= 	(double)xtimershowframerate->GetMeasureMilliSeconds() / (double)GRPPROFILER::framesFromLastMeasure;
	GRPPROFILER::fps										= 	GRPPROFILER::framesFromLastMeasure / xtimershowframerate->GetMeasureSeconds();


	if (n<(GRPPROFILER::nmeasures-1))
	{
		last_measures[n]=GRPPROFILER::fps;
		n++;
	}
	else
	{
		for (int e=0;e<(GRPPROFILER::nmeasures-1);e++)
			last_measures[e]=last_measures[e+1];

		last_measures[(GRPPROFILER::nmeasures-1)]=GRPPROFILER::fps;
	}

	GRPPROFILER::AddFrames =0;

		for (XDWORD e=0;e<n;e++)
			GRPPROFILER::AddFrames+=last_measures[e];

		GRPPROFILER::fpsAvg=GRPPROFILER::AddFrames/double(n);

	if(showstatistics)
		{
			XDEBUG_PRINTCOLOR(1, __L("F(t) [ %f / %d ], F(r) [ %d ], F(a) [ %2.2f ] E: %04d/%04d N: %04d F: %05d S:%02d B:%02d T:%02d R:%02d M:%02d Dc:%03d P:%d")			
			,GRPPROFILER::ft
			, GRPPROFILER::framesFromLastMeasure
			,	GRPPROFILER::fps
			,	GRPPROFILER::fpsAvg
			, GRPPROFILER::nElements-GRPPROFILER::nCulledElements
			,	GRPPROFILER::nElements
			,	GRPPROFILER::nNodes
			,	GRPPROFILER::nFacesRendered
			,	GRPPROFILER::nShaderChanges
			,	GRPPROFILER::nBufferChanges
			,	GRPPROFILER::nTextureChanges
			,	GRPPROFILER::nRegisterAccess
			,	GRPPROFILER::nMatrixOperations
			,	GRPPROFILER::nDrawCalls
			,	GRPPROFILER::Passes			
			);		
		}

	GRPPROFILER::framesFromLastMeasure	=		0;
	xtimershowframerate->Reset();
	}
}

/*-------------------------------------------------------------------
//	GRPPROFILER::Clear
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/10/2015 10:26:16
//	
*/
/*-----------------------------------------------------------------*/
void		GRPPROFILER::Clear()
{
	GRPPROFILER::Passes							=0;	
	GRPPROFILER::Scenes							=0;
	GRPPROFILER::nShaderChanges			=0;
	GRPPROFILER::nBufferChanges			=0;
	GRPPROFILER::nTextureChanges		=0;
	GRPPROFILER::nRegisterAccess		=0;
	GRPPROFILER::nMatrixOperations	=0;
	GRPPROFILER::nFacesRendered			=0;
	GRPPROFILER::nDrawCalls					=0;
	GRPPROFILER::nNodes							=0;
	GRPPROFILER::nNodesUpdates			=0;
	GRPPROFILER::nElements					=0;
	GRPPROFILER::nCulledElements		=0;

	GRPPROFILER::framesFromLastMeasure++;
	GRPPROFILER::frames++;
}



/*-------------------------------------------------------------------
//	GRPPROFILER::Delete
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			05/09/2016 14:36:03
//	
*/
/*-----------------------------------------------------------------*/
void GRPPROFILER::Delete()
{
	if(xtimershowframerate)
		{
			xfactory->DeleteTimer(xtimershowframerate);
			xtimershowframerate = NULL;
		}
}
		