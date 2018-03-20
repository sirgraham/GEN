/*------------------------------------------------------------------------------------------
//	GRPSUBJECT.H
*/	
/**	
// \class 
//   
//  subject for graphic notifications
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 03/11/2015 12:47:16
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPSUBJECT_H_
#define _GRPSUBJECT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"
#include "GRPObserver.h"
#include "GRPNamed.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPSUBJECT : virtual public GRPNAMED
{
public:
													GRPSUBJECT			();
	virtual									~GRPSUBJECT			();
						bool					AddObserver			(GRPOBSERVER* observer);
						bool					RemoveObserver	(GRPOBSERVER* observer);
						bool					Hide						();

	virtual		int						Publish					(GRPMSGTYPE msg);
	virtual		int						Publish					(GRPMSGTYPE msg, GRPSUBJECT* subject);

	virtual		XDWORD				GetSubjectType	()=0;

	XVECTOR<GRPOBSERVER*>*	GetObservers		() {	return &this->Observers;}

protected:

	XVECTOR<GRPOBSERVER*>		Observers;
	XDWORD									nObservers;

private:

	virtual void Clean()
	{
		nObservers=0;
		Observers.SetIsMulti(false);
	}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

