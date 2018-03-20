/*------------------------------------------------------------------------------------------
//	GRPINTERFACEANIMABLE.H
*/	
/**	
// \class 
//   
//  Interface for Animable
//   
//	@author	 Diego Martinez Ruiz de Gaona
//
//	Date Of Creation	: 24/02/2015 16:45:06
//	Last Modification	:	
*/	
//*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPINTERFACEANIMABLE_H_
#define _GRPINTERFACEANIMABLE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "GRPNamed.h"
#include "GRPAnimation.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
enum GRPINTERFACEANIMABLE_OFFBOUND_TYPE
{
	GRPINTERFACEANIMABLE_OFFBOUND_TYPE_OK=0,
	GRPINTERFACEANIMABLE_OFFBOUND_TYPE_PRE,
	GRPINTERFACEANIMABLE_OFFBOUND_TYPE_POST
};
	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPINTERFACEANIMABLE : public virtual GRPNAMED
{
	public:
												GRPINTERFACEANIMABLE					()
												{
													GRPNAMED::classtype = GRPNAMED_TYPE_GRPINTERFACEANIMABLE;
												}

		virtual bool				SetValue											(GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS subtype, double value,double base=0.0)	=	0;
};

	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

