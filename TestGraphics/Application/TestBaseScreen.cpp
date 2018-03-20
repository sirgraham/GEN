/*------------------------------------------------------------------------------------------
//	TESTBASESCREEN.H
*/	
/**	
// \class 
//   
//  Test Base Minimun screen
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 2/09/2016 16:16:37
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPScene.h"
#include "TestBaseScreen.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	TESTBASESCREEN::TESTBASESCREEN
*/	
/**	
//	
//	Class Constructor TESTBASESCREEN
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/09/2016 17:09:42
//	
//  @param				xapplication : 
*/
/*-----------------------------------------------------------------*/
TESTBASESCREEN::TESTBASESCREEN(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)  
{

} ;

/*-------------------------------------------------------------------
//	TESTBASESCREEN::IniResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/09/2016 17:09:49
//	
//	@return 			ool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASESCREEN::IniResources()
{
	PathManagement(__L("screen"));

	xapplication->GetMainScene()->GetPass()->SetClearColor(GRPCOLOR(0.2f,0.3f,0.4f));
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASESCREEN::EndResources
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/09/2016 17:09:56
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASESCREEN::EndResources()
{

	return true;
}

/*-------------------------------------------------------------------
//	TESTBASESCREEN::Do
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/09/2016 17:10:01
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASESCREEN::Do()
{
	static double i=0;
	i+=0.1f;
	
	double r=(sin(i));
	double g=(cos(i));
	double b=(sin(i)*cos(i));
	xapplication->GetMainScene()->GetPass()->SetClearColor(GRPCOLOR(r,g,b));
	xapplication->GetMainScene()->GetPass()->Clear();
	return true;
}

/*-------------------------------------------------------------------
//	TESTBASESCREEN::InputManagement
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/09/2016 17:10:08
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASESCREEN::InputManagement()
{	
	return TESTBASE::InputManagement();
}

/*-------------------------------------------------------------------
//	TESTBASESCREEN::Notify
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/09/2016 17:10:23
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool TESTBASESCREEN::Notify()
{
	return true;
}


	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

