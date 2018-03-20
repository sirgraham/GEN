/*------------------------------------------------------------------------------------------
//	GRPTEXTUREEXT.H
*/	
/**	
// \file 
//   
//  Texture Extented
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 19/04/2016 12:16:20
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPTEXTUREEXT_H_
#define _GRPTEXTUREEXT_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
#include "GRPTexture.h"
#include "GRPTextureFormats.h"
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
enum GRPTEXTURE_FILTERING
{
	GRPTEXTURE_FILTERING_UNKNOWN = 0										,
	GRPTEXTURE_FILTERING_LINEAR													,
	GRPTEXTURE_FILTERING_NEAREST
};

	
/*---- CLASS -----------------------------------------------------------------------------*/

class GRPTEXTUREEXT : public GRPTEXTURE
{
public:

																							GRPTEXTUREEXT																						();
	virtual																		 ~GRPTEXTUREEXT																						();

	virtual void																Create																									();
	virtual void																Create																									(XBUFFER* xbuffer, XDWORD width, XDWORD height, GRPTEXTURE_FORMAT format = GRPTEXTURE_FORMAT_RGB, GRPTEXTURE_FILTERING filtering = GRPTEXTURE_FILTERING_LINEAR);
	virtual void																Resize																									(XDWORD width, XDWORD height);
	virtual void																SetFormat																								(GRPTEXTURE_FORMAT format);
	virtual void																LoadToGPU																								(XBUFFER* xbuffer);
	virtual void																ReadFromGPU																							(XBUFFER* xbuffer);

	virtual void																Bind																										(XDWORD unit = 0);

	virtual void																SetFiltering																						(GRPTEXTURE_FILTERING filtering);
	virtual GRPTEXTUREHANDLE										GetHandle																								();

protected:
	GRPTEXTURE_FORMAT														format;
	GRPTEXTURE_FILTERING												filtering;

	XDWORD width;
	XDWORD height;

private:
	void																				Clean																										()
																							{
																								width		= 0;
																								height	= 0;
																							}
};
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

