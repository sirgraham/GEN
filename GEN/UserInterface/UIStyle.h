/*------------------------------------------------------------------------------------------
//	UISTYLE.H
*/	
/**	
// \file 
//   
//  basic UI drawer
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 08/08/2016 11:41:08
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UISTYLE_H_
#define _UISTYLE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGETCONTAINER;
class UIMANAGER;

class UISTYLE
{
public:

																							UISTYLE																				()
																							{
																								Clean();
																							}

	virtual																		 ~UISTYLE																				()
																							{
																								Clean();
																							}

	virtual bool																Draw																					(UIWIDGETCONTAINER* widget) = 0;

	void																				SetUIManager																	(UIMANAGER* uimanager)													{ this->uimanager = uimanager;	}
	UIMANAGER*																	GetUIManager																	()																							{ return this->uimanager;				}

protected:
	UIMANAGER*																	uimanager;
private:
	void																				Clean																					()
																							{
																								uimanager = NULL;
																							}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

