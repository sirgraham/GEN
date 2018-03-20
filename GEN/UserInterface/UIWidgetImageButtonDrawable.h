/*------------------------------------------------------------------------------------------
//	UIWIDGETIMAGEBUTTONDRAWABLE.H
*/	
/**	
// \file 
//   
//  draws a button with correct corners
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 23/06/2016 16:25:35
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _UIWIDGETIMAGEBUTTONDRAWABLE_H_
#define _UIWIDGETIMAGEBUTTONDRAWABLE_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetImageDrawable.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGETIMAGEBUTTONDRAWABLE : public UIWIDGETIMAGEDRAWABLE
{
public:

															 UIWIDGETIMAGEBUTTONDRAWABLE																(XCHAR* name, UIMANAGER* uimanager, GRPSCENE* scene);
	virtual											~UIWIDGETIMAGEBUTTONDRAWABLE																();

	virtual				bool						SetImages																									(XCHAR* tl, XCHAR* tm, XCHAR* tr, XCHAR* ml, XCHAR* mm, XCHAR* mr, XCHAR* bl, XCHAR* bm, XCHAR* br);

	virtual				bool						InternalDraw																							(XDWORD& widgetlevel);
	virtual				bool						InternalUpdate																						(XDWORD widgetlevel);

	virtual				void						SetActive																									(bool active);

protected:

	GRPELEMENT*										etl;
	GRPELEMENT*										etm;
	GRPELEMENT*										etr;
		 
	GRPELEMENT*										eml;
	GRPELEMENT*										emm;
	GRPELEMENT*										emr;
		 
	GRPELEMENT*										ebl;
	GRPELEMENT*										ebm;
	GRPELEMENT*										ebr;

	GRPTEXTURE*										ttl;
	GRPTEXTURE*										ttm;
	GRPTEXTURE*										ttr;

	GRPTEXTURE*										tml;
	GRPTEXTURE*										tmm;
	GRPTEXTURE*										tmr;

	GRPTEXTURE*										tbl;
	GRPTEXTURE*										tbm;
	GRPTEXTURE*										tbr;

	XSTRING												stl;
	XSTRING												stm;
	XSTRING												str;

	XSTRING												sml;
	XSTRING												smm;
	XSTRING												smr;

	XSTRING												sbl;
	XSTRING												sbm;
	XSTRING												sbr;

	bool													imageloaded;

private:
	void													Clean																											()
																{
																	etl						= NULL;
																	etm						= NULL;
																	etr						= NULL;
																	
																	eml						= NULL;
																	emm						= NULL;
																	emr						= NULL;
																	
																	ebl						= NULL;
																	ebm						= NULL;
																	ebr						= NULL;
																	
																	ttl						= NULL;
																	ttm						= NULL;
																	ttr						= NULL;
																	
																	tml						= NULL;
																	tmm						= NULL;
																	tmr						= NULL;
																	
																	tbl						= NULL;
																	tbm						= NULL;
																	tbr						= NULL;

																	imageloaded		= false;
																}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

