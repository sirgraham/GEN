//------------------------------------------------------------------------------------------
//	TESTGRAPHICS.H
//	
/**	
// \class 
//   
//  Generic Demo Engine
//   
//	@author	 Diego Martinez Ruiz de Gaona
//	@version 09/08/2002
*/	
//	GEN (C) Copyright  (All right reserved)		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _TESTGRAPHICS_H_
#define _TESTGRAPHICS_H_
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "GRPApplication.h"
#include "XApplication.h"
#include "XFSMachine.h"

	
//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum TESTGRAPHICSTEST
{
	TESTGRAPHICSTEST_RENDEROBJECT										= 0 ,
	TESTGRAPHICSTEST_FONTTTF														,
	TESTGRAPHICSTEST_MERGEANIMATIONLAYERS								,	
	TESTGRAPHICSTEST_SPLINE															,	
	TESTGRAPHICSTEST_SPRITES														,
	TESTGRAPHICSTEST_MILESTONE													,	
	TESTGRAPHICSTEST_CONSOLE													  ,
	TESTGRAPHICSTEST_OBJMERGING													,
	TESTGRAPHICSTEST_AUDIO															,	
	TESTGRAPHICSTEST_UI																	,	
	TESTGRAPHICSTEST_GGF																,	
	TESTGRAPHICSTEST_DIOFUNCTIONS												,	
	TESTGRAPHICSTEST_LEDSYNC														,	
	TESTGRAPHICSTEST_FACERIG														,	
	TESTGRAPHICSTEST_MULTIRENDER												,
	TESTGRAPHICSTEST_BLENDING														,	
	TESTGRAPHICSTEST_PHY																,	
	TESTGRAPHICSTEST_SHADOWS														,
	TESTGRAPHICSTEST_POINTLIGHTSHADOWS									,
	TESTGRAPHICSTEST_VIDEO															,
	TESTGRAPHICSTEST_CUBEMAP														,
	TESTGRAPHICSTEST_PASSES															,
	TESTGRAPHICSTEST_RENDERER														,
	TESTGRAPHICSTEST_REFLECTION													,
	TESTGRAPHICSTEST_LOADER															,
	TESTGRAPHICSTEST_OBJECTBUILDER											,
	TESTGRAPHICSTEST_SCREEN															,
	TESTGRAPHICSTEST_TEXTANIMATION											,
	TESTGRAPHICSTEST_LAYERS															,
	TESTGRAPHICSTEST_INPUT															,
	TESTGRAPHICSTEST_CALIBRATE_LED											,
	TESTGRAPHICSTEST_ENVMAP															,										
	TESTGRAPHICSTEST_SHADERLOADER												,
	TESTGRAPHICSTEST_DIVS																,
	TESTGRAPHICSTEST_CULLING														,
	TESTGRAPHICSTEST_HDR																,
	TESTGRAPHICSTEST_TEXTURE
};

#define TESTGRAPHICS_VERSION												0
#define TESTGRAPHICS_SUBVERSION											2
#define TESTGRAPHICS_SUBVERSIONERR									0

#define TESTGRAPHICS_NAMEAPP			 									__L("TestGraphics")
#define TESTGRAPHICS_YEAROFCREATION									2015

#define TESTGRAPHICS_CFGNAMEFILE	 									__L("testgraphics")
#define TESTGRAPHICS_LOGNAMEFILE	 									TESTGRAPHICS_CFGNAMEFILE

#define TESTGRAPHICS_LOGSECTIONIDGENERIC					 __L("Generic")


//---- CLASS -------------------------------------------------------------------------------

class TESTBASE;
class TESTGRAPHICS : public GRPAPPLICATION
{
	public:	
																			  TESTGRAPHICS									() { Clean(); }		

		bool																OnConfig											();
		bool																OnInit												();
		bool																OnUpdate											();
		bool																OnEnd													();	
			
	private:

		TESTBASE*														testbase;
		int																	testtoexec;

		void																Clean													()
																				{																					
																					testbase						= NULL;	
																					testtoexec					= -1;																					
																				}		
		
		
};
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
		


#endif

