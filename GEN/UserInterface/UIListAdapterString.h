/*------------------------------------------------------------------------------------------
//	UILISTADAPTERSTRING.H
*/	
/**	
// \file 
//   
//  string specialization for UI
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 13/10/2016 16:05:21
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UILISTADAPTERSTRING_H_
#define _UILISTADAPTERSTRING_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIListAdapter.h"
#include "XString.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
template <>
class UILISTADAPTER<XSTRING>
{
public:

	virtual UIWIDGET* GetWidget(XDWORD position);

protected:

private:

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

