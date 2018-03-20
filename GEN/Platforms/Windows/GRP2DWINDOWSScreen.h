/*------------------------------------------------------------------------------------------
//	GRP2DWINDOWSSCREEN.H
*/	
/**	
// \file 
//   
//  2D screen
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 22/07/2016 16:11:32
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRP2DWINDOWSSCREEN_H_
#define _GRP2DWINDOWSSCREEN_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "GRPScreen.h"
#include <Windows.h>	

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
	
	
/*---- CLASS -----------------------------------------------------------------------------*/
class GRPBITMAP;

class GRP2DWINDOWSSCREEN : public GRPSCREEN
{
public:

																GRP2DWINDOWSSCREEN						();
	virtual											 ~GRP2DWINDOWSSCREEN						();

	virtual bool									Create												(void* handle = NULL)					= 0;				
	virtual bool									Update												()														= 0;	
	virtual bool									Delete												()														= 0;

	GRPBITMAP*										GetBitmap											()																						{ return this->bitmap; }

protected:

	GRPBITMAP*										bitmap;
	WNDCLASSEX										wndclass;
	XSTRING												classname;
	XSTRING												title;

	HINSTANCE											hinstance;
	HWND													hwnd;
	HDC														hdc;
	HBITMAP												hbmp;
	HBITMAP												ohbmp;
	BITMAPINFO										hinfo;	

private:

	void													Clean													()
																{
																	bitmap = NULL;
																}

};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

