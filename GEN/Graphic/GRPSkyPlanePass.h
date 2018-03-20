/*------------------------------------------------------------------------------------------
//	GRPSKYPLANEPASS.H
*/	
/**	
// \class 
//   
//  Manages skyplane pass
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 20/06/2017 10:56:42
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPSKYPLANEPASS_H_
#define _GRPSKYPLANEPASS_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPbackgroundPass.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPMATERIAL;
	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPSKYPLANEPASS : public GRPBACKGROUNDPASS
{
public:

								GRPSKYPLANEPASS() : GRPBACKGROUNDPASS(__L("skybox")){}
	virtual				~GRPSKYPLANEPASS() {}

	virtual bool	 OnAdded();
	virtual void	CreateCamera		(GRPCAMERA_TYPE type) { };

protected:

private:

	void Clean()
	{
		
	}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif