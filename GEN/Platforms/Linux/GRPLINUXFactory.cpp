//------------------------------------------------------------------------------------------
//	GRPLINUXFACTORY.CPP
//	
//	LINUX graphics platform factory class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 08/08/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XFactoryMacros.h"
#include "XString.h"
#include "XDebug.h"

#include "GRPOpengl.h"

#ifdef HW_PC
#include "GRPLINUXScreenX11.h"
#include "GRPLINUXX11GLContext.h"
#endif

#ifdef HW_ODROIDC1
//#include "GRPLINUXScreenX11.h"
//#include "GRPLINUXScreenX11GLES.h"
//#include "GRPLINUXX11GLESContext.h"
#include "GRPLINUXScreenFrameBufferGLES.h"
#include "GRPLINUXFrameBuffer.h"
//#include "GRPLINUXFrameBufferContext.h"
#endif

#ifdef HW_RASPBERRYPI
#include "GRPLINUXScreenDispman.h"	
#include "GRPLINUXDispmanContext.h"
#endif
	
#include "GRPLINUXFactory.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------

	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//	GRPLINUXFACTORY::CreateScreen
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			21/05/2014 16:07:20
//	
//	@return 			GRPSCREEN* : 
//
 
 
*/
/*-----------------------------------------------------------------*/
GRPSCREEN* GRPLINUXFACTORY::CreateScreen()
{
	GRPSCREEN* screen = NULL;	

	#ifdef HW_PC
	screen =  (GRPSCREEN*)new GRPLINUXSCREENX11;
	#endif

	#ifdef HW_ODROIDC1
	//screen =  (GRPSCREEN*)new GRPLINUXSCREENX11GLES;
	//screen =  (GRPSCREEN*)new GRPLINUXSCREENX11;
	screen =  (GRPSCREEN*)new GRPLINUXSCREENFRAMEBUFFER;
	#endif

	#ifdef HW_RASPBERRYPI
	screen =  (GRPSCREEN*)new GRPLINUXSCREENDISPMAN;
	#endif

	if(screen)
		{
			screen->SetDPI(dpi);
		}

	return screen;	
}




/*-------------------------------------------------------------------
//  GRPLINUXFACTORY::DeleteScreen
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			17/10/2013 16:35:35
//	
//	@return 			bool : 
//	@param				screen : 
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXFACTORY::DeleteScreen(GRPSCREEN* screen)
{	
	#ifdef HW_PC
	GRPLINUXSCREENX11* _class =  (GRPLINUXSCREENX11*)screen;
	delete _class;
	#endif

	#ifdef HW_RASPBERRYPI
	GRPLINUXSCREENDISPMAN* _class =  (GRPLINUXSCREENDISPMAN*)screen;
	delete _class;																																																
	#endif

	return true;


}






/*-------------------------------------------------------------------
//	GRPLINUXFACTORY::CreateContext
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/04/2016 12:44:38
//	
//	@return 			GRPCONTEXT* : 
//
*/
/*-----------------------------------------------------------------*/
GRPCONTEXT* GRPLINUXFACTORY::CreateContext()
{
	GRPCONTEXT* context = NULL;

	#ifdef HW_PC
	context =  new GRPLINUXX11GLCONTEXT;
	#endif

	#ifdef HW_RASPBERRYPI
	context =  new GRPLINUXDISPMANCONTEXT;
	#endif

	#ifdef HW_ODROIDC1
	context = new GRPLINUXFRAMEBUFFERCONTEXT;
	//context = new GRPLINUXX11GLESCONTEXT;
	#endif

	return context;
}






/*-------------------------------------------------------------------
//	GRPLINUXFACTORY::DeleteContext
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			05/04/2016 12:44:32
//	
//	@return 			bool : 
//
//  @param				context : 
*/
/*-----------------------------------------------------------------*/
bool GRPLINUXFACTORY::DeleteContext(GRPCONTEXT* context)
{
	delete context;
}


void GRPLINUXFACTORY::GenerateTextureMap()
{
#ifdef HW_PC
	textureformatmap.Add(GRPTEXTURE_FORMAT_R					,	GL_RED				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RG					,	GL_RG					);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB				,	GL_RGB				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA				,	GL_RGBA				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_R8					, GL_R8					);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RG8				, GL_RG8				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB8				, GL_RGB8				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA8			, GL_RGBA8			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_R16F				, GL_R16F				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RG16F			, GL_RG16F			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB16F			, GL_RGB16F			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA16F		, GL_RGBA16F		);
  textureformatmap.Add(GRPTEXTURE_FORMAT_R32F				, GL_R32F				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RG32F			, GL_RG32F			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB32F			, GL_RGB32F			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA32F		, GL_RGBA32F		);
  textureformatmap.Add(GRPTEXTURE_FORMAT_R8I				, GL_R8I				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RG8I				, GL_RG8I				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB8I			, GL_RGB8I			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA8I			, GL_RGBA8I			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_R16I				, GL_R16I				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RG16I			, GL_RG16I			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB16I			, GL_RGB16I			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA16I		, GL_RGBA16I		);
  textureformatmap.Add(GRPTEXTURE_FORMAT_R32I				, GL_R32I				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RG32I			, GL_RG32I			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB32I			, GL_RGB32I			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA32I		, GL_RGBA32I		);
  textureformatmap.Add(GRPTEXTURE_FORMAT_R8UI				, GL_R8UI				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RG8UI			, GL_RG8UI			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB8UI			, GL_RGB8UI			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA8UI		, GL_RGBA8UI		);
  textureformatmap.Add(GRPTEXTURE_FORMAT_R16UI			, GL_R16UI			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RG16UI			, GL_RG16UI			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB16UI		, GL_RGB16UI		);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA16UI		, GL_RGBA16UI		);
  textureformatmap.Add(GRPTEXTURE_FORMAT_R32UI			, GL_R32UI			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RG32UI			, GL_RG32UI			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB32UI		, GL_RGB32UI		);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA32UI		, GL_RGBA32UI		);

  // Gamma correct
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB				, GL_SRGB				);
	textureformatmap.Add(GRPTEXTURE_FORMAT_SRGBA			, GL_SRGB_ALPHA	);

  // Compressed formats
  // S3TC
	textureformatmap.Add(GRPTEXTURE_FORMAT_RGB_S3TC_DXT1			, GL_COMPRESSED_RGB_S3TC_DXT1_EXT					);
	textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB_S3TC_DXT1			, GL_COMPRESSED_SRGB_S3TC_DXT1_EXT				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_S3TC_DXT1			, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGBA_S3TC_DXT1		, GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT	);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_S3TC_DXT3			, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGBA_S3TC_DXT3		, GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT	);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_S3TC_DXT5			, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGBA_S3TC_DXT5		, GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT	);

	// ETC2
	textureformatmap.Add(GRPTEXTURE_FORMAT_RGB8_ETC2												, GL_COMPRESSED_RGB8_ETC2											);
	textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ETC2												, GL_COMPRESSED_SRGB8_ETC2										);
	textureformatmap.Add(GRPTEXTURE_FORMAT_RGB8_PUNCHTHROUGH_ALPHA1_ETC2		, GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2	);
	textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2		, GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2);
	textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA8_ETC2_EAC										, GL_COMPRESSED_RGBA8_ETC2_EAC								);
	textureformatmap.Add(GRPTEXTURE_FORMAT_SRGBA8_ETC2_EAC									, GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC					);

	//ASTC
	textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_ASTC_4x4								, GL_COMPRESSED_RGBA_ASTC_4x4_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_ASTC_5x4								, GL_COMPRESSED_RGBA_ASTC_5x4_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_ASTC_5x5								, GL_COMPRESSED_RGBA_ASTC_5x5_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_ASTC_6x5								, GL_COMPRESSED_RGBA_ASTC_6x5_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_ASTC_6x6								, GL_COMPRESSED_RGBA_ASTC_6x6_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_ASTC_8x5								, GL_COMPRESSED_RGBA_ASTC_8x5_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_ASTC_8x6								, GL_COMPRESSED_RGBA_ASTC_8x6_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_ASTC_8x8								, GL_COMPRESSED_RGBA_ASTC_8x8_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_ASTC_10x5								, GL_COMPRESSED_RGBA_ASTC_10x5_KHR );
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_ASTC_10x6								, GL_COMPRESSED_RGBA_ASTC_10x6_KHR );
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_ASTC_10x8								, GL_COMPRESSED_RGBA_ASTC_10x8_KHR );
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_ASTC_10x10							, GL_COMPRESSED_RGBA_ASTC_10x10_KHR);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_ASTC_12x10							, GL_COMPRESSED_RGBA_ASTC_12x10_KHR);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_ASTC_12x12							, GL_COMPRESSED_RGBA_ASTC_12x12_KHR);

  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_4x4				, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_5x4				, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_5x5				, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_6x5				, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_6x6				, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_8x5				, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_8x6				, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_8x8				, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_10x5				, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR );
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_10x6				, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR );
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_10x8				, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR );
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_10x10			, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR);
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_12x10			, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR);
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_12x12			, GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR);

 // BPTC																																																						 
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA_BPTC_UNORM							, GL_COMPRESSED_RGBA_BPTC_UNORM				 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_SRGBA_BPTC_UNORM							, GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB_BPTC_SIGNED_FLOAT				, GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT	 );
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB_BPTC_UNSIGNED_FLOAT			, GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT);

  // "Special" formats
  textureformatmap.Add(GRPTEXTURE_FORMAT_DEPTH						, GL_DEPTH_COMPONENT		);
  textureformatmap.Add(GRPTEXTURE_FORMAT_DEPTH16					, GL_DEPTH_COMPONENT16	);
  textureformatmap.Add(GRPTEXTURE_FORMAT_DEPTH24					, GL_DEPTH_COMPONENT24	);
	textureformatmap.Add(GRPTEXTURE_FORMAT_DEPTH32F					, GL_DEPTH_COMPONENT32F	);
	textureformatmap.Add(GRPTEXTURE_FORMAT_DEPTH_STENCIL		, GL_DEPTH_STENCIL			);
#endif


/* ----------------------------------------------------------------------------------------------------------------------------------- */



#ifdef HW_RASPBERRYPI
	textureformatmap.Add(GRPTEXTURE_FORMAT_R					,	GL_LUMINANCE	);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RG					,	GL_LUMINANCE_ALPHA					);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB				,	GL_RGB				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA				,	GL_RGBA				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_R8					, GL_LUMINANCE					);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RG8				, GL_LUMINANCE_ALPHA				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB8				, GL_RGB				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA8			, GL_RGBA			);

  textureformatmap.Add(GRPTEXTURE_FORMAT_R8UI				, GL_LUMINANCE				);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RG8UI			, GL_LUMINANCE_ALPHA			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGB8UI			, GL_RGB			);
  textureformatmap.Add(GRPTEXTURE_FORMAT_RGBA8UI		, GL_RGBA		);

  // "Special" formats, to check on rpi
  //textureformatmap.Add(GRPTEXTURE_FORMAT_DEPTH						, GL_DEPTH_COMPONENT		);
  //textureformatmap.Add(GRPTEXTURE_FORMAT_DEPTH16					, GL_DEPTH_COMPONENT16	);
  //textureformatmap.Add(GRPTEXTURE_FORMAT_DEPTH24					, GL_DEPTH_COMPONENT24	);
	//textureformatmap.Add(GRPTEXTURE_FORMAT_DEPTH32F					, GL_DEPTH_COMPONENT32F	);
	//textureformatmap.Add(GRPTEXTURE_FORMAT_DEPTH_STENCIL		, GL_DEPTH_STENCIL			);
#endif

}


