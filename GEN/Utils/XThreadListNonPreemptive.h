/*------------------------------------------------------------------------------------------
//	XTHREADLISTNONPREEMPTIVE.H
*/	
/**	
// \class 
//   
//  XThread List Non Preemtive 
//   
//	@author	 Abraham J. Velez
//
//	Date Of Creation	: 12/08/2016 12:26:35
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
#ifndef _XTHREADLISTNONPREEMPTIVE_H_
#define _XTHREADLISTNONPREEMPTIVE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XBase.h"
#include "XVector.h"
#include "XThread.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/


class XTHREADLISTNONPREEMPTIVE
{
	public:		
		
		static XTHREADLISTNONPREEMPTIVE&				GetInstance											()
																						{
																							if(!instance) instance = new XTHREADLISTNONPREEMPTIVE();
																						
																							return (*instance);	
																						}						

		static bool															DelInstance											()
																						{
																							if(instance)
																								{																						
																									delete instance;
																									instance = NULL;

																									return true;
																								} 
																				
																							return false;
																						}		
			
	
		bool																		AddThread												(XTHREAD* thread)
																						{		
																							if(!thread) return false;
																							return xthreadsvector.Add(thread);
																						}

		bool																		DeleteThread										(XTHREAD* thread)
																						{	
																							if(!thread) return false;
																							return xthreadsvector.Delete(thread);
																						}
																	
		bool																		DeleteAll												()
																						{								
																							xthreadsvector.DeleteContents();
																							xthreadsvector.DeleteAll();
																																														
																							return true;
																						}

		bool																		ExecuteFunctions								()
																						{
                                              if(!xthreadsvector.GetSize()) return false;
                                              
																							for(XDWORD c=0; c<xthreadsvector.GetSize(); c++)
																								{
																									XTHREAD* xthread  = xthreadsvector.Get(c);
																									if(xthread) 
																										{
																											switch(xthread->GetStatusFunc())
																												{
																													case XTHREADSTATUS_RUN		:	xthread->GetFunction()(xthread->GetParam());																																																																																						
																																											break;		

																													case XTHREADSTATUS_EXIT		:	break;
																												}																																																					
																										}
																								}

                                              return true;
																						}

	
	private:
																						XTHREADLISTNONPREEMPTIVE				()																
																						{ 
																							Clean();																																																																																																																										
																						}

		virtual																 ~XTHREADLISTNONPREEMPTIVE				()																
																						{ 																							
																							DeleteAll();
																							
																							Clean();																																	
																						}

		void																		Clean														()		
																						{
																							
																						}

		
		static XTHREADLISTNONPREEMPTIVE*				instance;
		XVECTOR<XTHREAD*>												xthreadsvector;	
};


	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

