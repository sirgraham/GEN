/*------------------------------------------------------------------------------------------
//	GRPFACTORY.CPP
//	
//	sdsdffds
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 31/10/2013 12:14:32
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPFileBitmap.h"	
#include "GRPFactory.h"

#include "XMemory.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
GRPFACTORY* grpfactory = NULL;
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	GRPFACTORY::CreateBitmap
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/04/2014 16:19:33
//	
//	@return 			GRPBITMAP* : 
//
//  @param				width : 
//  @param				height : 
//  @param				mode : 
*/
/*-----------------------------------------------------------------*/
GRPBITMAP* GRPFACTORY::CreateBitmap(int width, int height, GRPPROPERTYMODE mode)
{
	GRPBITMAP* bitmap = NULL;

	if(mode == GRPPROPERTYMODE_XX_UNKNOWN) return NULL;
			
	switch(mode)
		{									
			case GRPPROPERTYMODE_08_INDEX			: break;			
			case GRPPROPERTYMODE_16_RGBA_4444	: break;
			case GRPPROPERTYMODE_16_RGBA_5551	: bitmap =  (GRPBITMAP*)new GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_RGB555>(width, height, mode);		break;		
			case GRPPROPERTYMODE_16_RGB_565		: bitmap =  (GRPBITMAP*)new GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_RGB565>(width, height, mode);		break;		
			case GRPPROPERTYMODE_16_SRGB_565	: break;
 			case GRPPROPERTYMODE_24_RGB_888		: bitmap =  (GRPBITMAP*)new GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_RGB24> (width, height, mode);		break;
			case GRPPROPERTYMODE_24_BGR_888		: bitmap =  (GRPBITMAP*)new GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_BGR24> (width, height, mode);		break;
			case GRPPROPERTYMODE_32_RGBA_8888	: bitmap =  (GRPBITMAP*)new GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_RGBA32>(width, height, mode);		break;
			case GRPPROPERTYMODE_32_BGRA_8888	: bitmap =  (GRPBITMAP*)new GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_BGRA32>(width, height, mode);		break;
			default: break;
		}

	return bitmap;																					
}




/*-------------------------------------------------------------------
//	GRPFACTORY::DeleteBitmap
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			14/04/2014 16:19:48
//	
//	@return 			bool : 
//
//  @param				bitmap : 
*/
/*-----------------------------------------------------------------*/
bool GRPFACTORY::DeleteBitmap(GRPBITMAP* bitmap)
{
	if(!bitmap) return false;

	switch(bitmap->GetMode())
		{									
			case GRPPROPERTYMODE_08_INDEX			: break;
			
			case GRPPROPERTYMODE_16_RGBA_4444	: break;

			case GRPPROPERTYMODE_16_RGBA_5551	: {	GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_RGB555>* _class =  (GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_RGB555>*)bitmap;
																						delete _class;
																						return true;
																					}
																					break;		

			case GRPPROPERTYMODE_16_RGB_565		: {	GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_RGB565>* _class =  (GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_RGB565>*)bitmap;
																						delete _class;
																						return true;
																					}
																					break;		

			case GRPPROPERTYMODE_16_SRGB_565	: break;

 			case GRPPROPERTYMODE_24_RGB_888		: {	GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_RGB24>* _class =  (GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_RGB24>*)bitmap;
																						delete _class;
																						return true;
																					}
																					break;

			case GRPPROPERTYMODE_24_BGR_888		: {	GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_BGR24>* _class =  (GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_BGR24>*)bitmap;
																						delete _class;
																						return true;
																					}
																					break;

			case GRPPROPERTYMODE_32_RGBA_8888	: {	GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_RGBA32>* _class =  (GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_RGBA32>*)bitmap;
																						delete _class;
																						return true;
																					}
																					break;

			case GRPPROPERTYMODE_32_BGRA_8888	: {	GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_BGRA32>* _class =  (GRPBUFFERBITMAPPIXELFORMAT<GRPBUFFERPIXELFORMAT_BGRA32>*)bitmap;
																						delete _class;
																						return true;
																					}
																					break;
			default: break;
		}

	return false;
}


XDWORD GRPFACTORY::GetTextureFormat(GRPTEXTURE_FORMAT format)
{
	return textureformatmap.Get(format);
}


