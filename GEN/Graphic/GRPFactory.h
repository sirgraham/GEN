//------------------------------------------------------------------------------------------
//	GRPFACTORY.H
//	
/**	
// \class 
//   
//  Platform Factory
//   
//	@author	 Diego Martinez Ruiz de Gaona
//	@version 15/07/2002
*/	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
#ifndef _GRPFACTORY_H_
#define _GRPFACTORY_H_
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XPath.h"

#include "GRPScreen.h"
#include "GRPBufferColor.h"
#include "GRPBitmap.h"
#include "GRPFileBitmap.h"
#include "GRPTextureFormats.h"

#include "XBinaryMap.h"

#include "XMemory.h"
	
//---- DEFINES & ENUMS  --------------------------------------------------------------------
		

//---- CLASS -------------------------------------------------------------------------------
class GRPCONTEXT;

class GRPFACTORY
{
	public:
																																		GRPFACTORY						()																{ }; 	
		virtual						 																						 ~GRPFACTORY						()																{ };  

		virtual GRPSCREEN*																							CreateScreen 					( )	= 0;
		virtual bool																										DeleteScreen 					(GRPSCREEN* screen)														= 0;

		virtual GRPCONTEXT*																							CreateContext					( )	= 0;
		virtual bool																										DeleteContext					(GRPCONTEXT* screen)													= 0;

		GRPBITMAP*																											CreateBitmap 					(int width, int height, GRPPROPERTYMODE mode);
		bool																														DeleteBitmap					(GRPBITMAP* bitmapa);

		float																														dpi;
	
		virtual void																										GenerateTextureMap		()																						= 0;
		XDWORD																													GetTextureFormat			(GRPTEXTURE_FORMAT format);

		XBINARYMAP<GRPTEXTURE_FORMAT, XDWORD> textureformatmap;

};
	
//---- INLINE FUNCTIONS --------------------------------------------------------------------

extern GRPFACTORY* grpfactory;

#endif

