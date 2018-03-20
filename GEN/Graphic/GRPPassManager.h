/*------------------------------------------------------------------------------------------
//	GRPPASSMANAGER.H
*/	
/**	
// \class 
//   
//  Manages multiple passes and his buffer relationships
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 28/07/2016 10:30:44
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPPASSMANAGER_H_
#define _GRPPASSMANAGER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XString.h"	
#include "XVector.h"
#include "GRPObserver.h"

	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPPASS;	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPPASSMANAGER : public GRPOBSERVER
{
public:
	

	static			GRPPASSMANAGER*			Get							();
							GRPPASS*						Create					(XCHAR* passtype=NULL,XDWORD priority=101);
	static			void								Destroy					();
							GRPPASS*						GetPass					(XCHAR*);

protected:

	static			GRPPASSMANAGER*			Instance;

																	GRPPASSMANAGER	();
	virtual													~GRPPASSMANAGER	();

							XVECTOR	<GRPPASS*>	Passes;		
							bool								Notify					(GRPMSGTYPE msg,GRPSUBJECT* subject);

							


private:

	void Clean()
	{

	}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

