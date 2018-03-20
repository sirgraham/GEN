/*------------------------------------------------------------------------------------------
//	GRPUSERDATACONTAINER.H
*/	
/**	
// \class 
//   
//  Container for user data
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 24/06/2015 13:46:27
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPUSERDATACONTAINER_H_
#define _GRPUSERDATACONTAINER_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "XString.h"
#include "XMap.h"
#include "GRPNamed.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPUSERDATACONTAINER : public GRPNAMED
{
protected:

		XMAP<XSTRING*,XSTRING*> UserData;

public:
															GRPUSERDATACONTAINER	();
															~GRPUSERDATACONTAINER	();

		XDWORD										GetSize								();
		XMAP<XSTRING*,XSTRING*>*	Get										();
		XSTRING*									GetUserDataByKey			(XCHAR* k);
		void											Add										(XCHAR* k, XCHAR* v);
		void											SetUserData						(GRPUSERDATACONTAINER*);
		void											Clear									();
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

