/*------------------------------------------------------------------------------------------
// GRPSmartVector.HPP
*/
/**
// \class
//
// Manages smart data vectors
//
//	@author: Diego Martinez Ruiz de Gaona 
//
//	Date of Creation : 05/03/2018 15:55:57 
//	Last Modification : 
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


#ifndef _GRPSMARTVECTOR_HPP
#define _GRPSMARTVECTOR_HPP
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPObserver.h"
#include "GRPSubject.h"
#include "XVector.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

template<class T>
class GRPSMARTVECTOR : public GRPOBSERVER, public XVECTOR<T>
{
public:
		bool Add(T subject)
		{
				if (XVECTOR<T>::Add(subject))
				{
						this->Subscribe(subject);
						return true;
				}
				return false;
		}

		bool Delete(T subject)
		{
				if (XVECTOR<T>::Delete(subject))
				{
						this->Unsubscribe(subject);
						return true;
				}
				return false;
		}

		bool DeleteIndex(int n)
		{
				T subject = this->Get(n);
				if (subject)
				if (XVECTOR<T>::DeleteIndex(n))
				{
						this->Unsubscribe(subject);
						return true;
				}
				return false;
		}

		bool Notify(GRPMSGTYPE msg, GRPSUBJECT* notifier)
		{
				if (msg == GRPMSG_DESTROYED)
						this->Delete(dynamic_cast<T>(notifier));
				return true;
		}

private:
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif