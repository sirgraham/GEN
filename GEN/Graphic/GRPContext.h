/*------------------------------------------------------------------------------------------
//	GRPCONTEXT.H
*/	
/**	
// \file 
//   
//  Graphics Context
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 04/04/2016 14:16:47
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPCONTEXT_H_
#define _GRPCONTEXT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPSCREEN;

class GRPCONTEXT
{
public:
																		GRPCONTEXT																();
	virtual													 ~GRPCONTEXT																();

	void															SetScreen																	(GRPSCREEN* screen);
	GRPSCREEN*												GetScreen																	();

	virtual bool											Create																		(void* handle = NULL);
	virtual bool											Destroy																		();
	virtual bool											IsLost																		();

	virtual bool											Update																		();

	void															SetMustRebuild														(bool mustrebuild);
	bool															IsMustRebuild															();

	void															SetMultiSampling													(XDWORD multisampling);
	XDWORD														GetMultisampling													();
	
protected:
	GRPSCREEN*												screen;
	bool															mustrebuild;
	XDWORD														multisampling;
private:
	void															Clean																			()
																		{
																			screen					= NULL;
																			mustrebuild			= false;
																			multisampling		= 0;
																		}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

