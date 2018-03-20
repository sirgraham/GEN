/*------------------------------------------------------------------------------------------
// GRPDepthPass.HPP
*/
/**
// \class
//
// Tries to create depth pass before color pass
//
//	@author: Diego Martinez Ruiz de Gaona
//
//	Date of Creation : 30/10/2017 11:55:05
//	Last Modification :
*/
/* (C) Copyright . All right reserved.
/*------------------------------------------------------------------------------------------*/
#ifndef _GRPDEPTHPASS_HPP
#define _GRPDEPTHPASS_HPP

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPPass.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPDEPTHPASS : public GRPPASS
{
public:
								 GRPDEPTHPASS		(XCHAR* name = __L("depth"));
								 ~GRPDEPTHPASS	();
		virtual bool OnAdded				();
		virtual void Begin					();
		virtual void End						();

protected:

private:

		void Clean ()
		{

		}



};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif