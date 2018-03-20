//------------------------------------------------------------------------------------------
//	XAPPLICATION.H
//	
/**	
// \class 
//   
//  Generic Application Class
//   
//	@author	 Abraham J. Velez
//	@version 09/08/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _XAPPLICATION_H_
#define _XAPPLICATION_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XString.h"
#include "XDebug.h"
#include "XObserver.h"
#include "XSubject.h"

	
//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum XAPPLICATIONEXITTYPE
{
  XAPPLICATIONEXITTYPE_NONE							=	0 ,
  XAPPLICATIONEXITTYPE_APPERROR							,
	XAPPLICATIONEXITTYPE_APPEND								,  //append en ingles significa añadir, igual seria mejor simplemente END o APP_END
	XAPPLICATIONEXITTYPE_BYUSER								,
	XAPPLICATIONEXITTYPE_SOSHUTDOWN						,
	XAPPLICATIONEXITTYPE_INVALIDLICENSE				,
	XAPPLICATIONEXITTYPE_EXPIREDLICENSE				,
	XAPPLICATIONEXITTYPE_NOTVCC								,
	XAPPLICATIONEXITTYPE_UPDATE						
};

//---- CLASS -------------------------------------------------------------------------------

class XSYSTEM;
class MAIN;
class XTIMER;


class XAPPLICATION : public XOBSERVER
{
	public:
																	XAPPLICATION									();
		virtual											 ~XAPPLICATION									();

 		static XAPPLICATION*					Create												();

		virtual bool									IniApplication								(XSYSTEM* xsystem, XVECTOR<XSTRING*>* execparams = NULL);				
		bool													UpdateApplication							();		
		bool													EndApplication								();
		
		virtual bool									Ini 													()																{	return true;														}						
		virtual bool									FirstUpdate										()																{	return true;														}				
		virtual bool									Update												()																{	return true;														}		
		virtual bool									LastUpdate										()																{	return true;														}					
		virtual bool									End														()																{	return true;														}
				
		XVECTOR<XSTRING*>*						GetExecParams									()																{ return execparams;											}		
																			      
		XSTRING*                      GetApplicationName						()																{ return &applicationname;								}		
		void													SetApplicationName						(XCHAR* name)											{ applicationname.Set(name);							}
		XSTRING*                      GetApplicationID							()																{ return &applicationID;									}		

		XSYSTEM*											GetXSystem										()																{	return xsystem;													}							
		
		MAIN*													GetMain												()																{ return main;														}
		void													SetMain												(MAIN* main)											{ this->main = main;											} 
		
		XTIMER*												GetTimerGlobal                ()																{	return xtimerglobal;										}
		
		 
		XAPPLICATIONEXITTYPE   				GetExitStatus									()																{ return exitstatus;											}
		void													SetExitStatus									(XAPPLICATIONEXITTYPE exitstatus)	
																	{ 																		
																		if(this->exitstatus!=XAPPLICATIONEXITTYPE_NONE) return;
																		this->exitstatus = exitstatus;	
																	}

		virtual void									HandleEvent										(XEVENT* xevent)
																	{

																	}
				
	protected:
			
		XVECTOR<XSTRING*>*						execparams;					
		XSYSTEM*											xsystem;
		MAIN*													main;
		XTIMER*												xtimerglobal;		
		XAPPLICATIONEXITTYPE          exitstatus;		


		XSTRING                       applicationname;
		XSTRING                       applicationID;

	private:
		
		void													Clean													()
																	{
																		execparams							= NULL;																				
																		xsystem                 = NULL;
																		main										= NULL;
																		xtimerglobal						= NULL;																		
																		exitstatus 							= XAPPLICATIONEXITTYPE_NONE;																																						
																	}
		
};

	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

