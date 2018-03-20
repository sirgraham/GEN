/*------------------------------------------------------------------------------------------
//	UIWIDGETEXCLUSIVECONTAINERDRAWABLE.H
*/	
/**	
// \file 
//   
//  drawable class for exclusive container
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 14/07/2016 12:23:34
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETEXCLUSIVECONTAINERDRAWABLE_H_
#define _UIWIDGETEXCLUSIVECONTAINERDRAWABLE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetExclusiveContainer.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPSCENE;

class UIWIDGETEXCLUSIVECONTAINERDRAWABLE : public UIWIDGETEXCLUSIVECONTAINER
{
public:

	UIWIDGETEXCLUSIVECONTAINERDRAWABLE(XCHAR* name, UIMANAGER* uimanager, GRPSCENE* scene);
	virtual ~UIWIDGETEXCLUSIVECONTAINERDRAWABLE();

protected:

private:
	void Clean()
	{
	}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

