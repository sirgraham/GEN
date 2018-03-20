/*------------------------------------------------------------------------------------------
// GRPViewportPass.HPP
*/
/**
// \class
//
// Manages viewport renderers
//
//	@author: Diego Martinez Ruiz de Gaona 
//
//	Date of Creation : 11/10/2017 17:35:05 
//	Last Modification : 
*/
/* (C) Copyright . All right reserved.
/*------------------------------------------------------------------------------------------*/
#ifndef _GRPVIEWPORTPASS_HPP
#define _GRPVIEWPORTPASS_HPP

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPPass.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPCAMERA;

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPVIEWPORTPASS : public GRPPASS
{
public:
		GRPVIEWPORTPASS(XCHAR* name = __L("background"));
		~GRPVIEWPORTPASS();

		virtual bool OnAdded();
		virtual void Begin();
		virtual void End();
		
protected:

private:

		void		Clean()
		{

		}



};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif