//------------------------------------------------------------------------------------------
//	GRPFILEBITMAPPNG.H
//	
/**	
// \class 
//   
//  Graphic File Bitmap PNG class
//   
//	@author	 Diego Martinez Ruiz de Gaona
//	@version 19/11/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _GRPFILEBITMAPPNG_H_
#define _GRPFILEBITMAPPNG_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "png.h"

#include "XBase.h"
#include "XFile.h"	
#include "XPath.h"
#include "XMap.h"

#include "GRPProperties.h"
#include "GRPBufferColorRGB.h"	
#include "GRPFileBitmap.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define GRPFILEBITMAPPNG_HEADERSIZE 8

//---- CLASS -------------------------------------------------------------------------------

class GRPFACTORY;
class GRPBITMAP;


class GRPFILEBITMAPPNG : public GRPFILEBITMAPBASE
{
	public:
																GRPFILEBITMAPPNG					( );
		virtual										 ~GRPFILEBITMAPPNG					();
		
		GRPBITMAP*									CreateBitmapFromFile			(XPATH& xpath);
		bool												CreateFileFromBitmap			(XPATH& xpath, GRPBITMAP* bitmap);
		
	private:
		
		void												Clean											();

		static void									ReadData									(png_structp png, png_bytep data, png_size_t length);
		static void									WriteData									(png_structp png, png_bytep data, png_size_t length);
};

	
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------
	
#endif

