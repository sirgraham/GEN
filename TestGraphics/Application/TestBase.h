/*------------------------------------------------------------------------------------------
//	TESTBASE.H
*/	
/**	
// \class 
//   
//  Test BASE + Test Graphics functions 
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 30/04/2015 12:40:24
//	Last Modification	:	
*/	
//	GEN (C) Copyright  (All right reserved)		 
//----------------------------------------------------------------------------------------*/
	
#ifndef _TESTBASE_H_
#define _TESTBASE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include  "XBase.h"
#include  "XString.h"
#include	"INPManager.h"

#include  "GRPConsole.h"
#include	"GRPFile3D.h"
#include	"GRPObserver.h"
#include  "GRPSubject.h"

#include	"GRPApplication.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
#define CREATETESTCLASS(nameclass)		class nameclass : public TESTBASE								\
																			{																								\
																				public:																				\
																														nameclass									(GRPAPPLICATION* xapplication) :  TESTBASE(xapplication)  { }  \
																					virtual bool			IniResources							();     \
																					virtual	bool			EndResources							();			\
																					virtual bool			Do												();     \
																					virtual bool			Notify										(GRPMSGTYPE msg,GRPSUBJECT* notifier);	\
																			};
 
/*---- CLASS -----------------------------------------------------------------------------*/

class XAPPLICATION;

class TESTBASE : public GRPOBSERVER
{
	public:

		enum STATE
		{
			STATE_NONE,
			STATE_LOADING,
			STATE_SETUP,
			STATE_IDLE,
			STATE_FINISHING,
			STATE_END,
		};

															TESTBASE											(GRPAPPLICATION* xapplication);
		virtual									 ~TESTBASE											()																			{ Clean();							}

		virtual	bool							IniResources									()																			{ return false;					}
		virtual	bool							EndResources									()																			{ return false;					}

		virtual bool							PathManagement								(XCHAR* addpath);
		virtual	bool							InputManagement								();																													
		virtual	bool							Do														()																			{ return false;         }
		virtual bool							Notify												(GRPMSGTYPE msg,GRPSUBJECT* notifier)		{ return true;					}

		GRPOBJECT*								LoadObject										(GRPOBJECT* obj,XCHAR* path,XDWORD flags=GRPFILE3D_INCLUDE_ALL, XDWORD verboseLevel=GRPFILE3D_VERBOSE_LEVEL_WARNINGS | GRPFILE3D_VERBOSE_LEVEL_ERRORS,XDWORD NextStackID=0);		
		void											InitConsole										(float size=0.12f);

	protected:

		GRPCONSOLE*								console;
						
		GRPAPPLICATION*						xapplication;		
		INPDEVICE*								keyboard;
		INPDEVICE*								mouse;		

		XPATH											rootpath;
		
		STATE											status;

		bool										  lockcamera;

	private:
		
		void											Clean													()
															{			
																lockcamera		= false;
																console				= NULL;
																xapplication	= NULL;
																keyboard			= NULL;
																mouse					= NULL;
																status				= STATE_NONE;
															}		
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

