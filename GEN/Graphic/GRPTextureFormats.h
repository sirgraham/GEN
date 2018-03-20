/*------------------------------------------------------------------------------------------
//	GRPTEXTUREFORMATS.H
*/	
/**	
// \file 
//   
//  Texture Formats Enum
//   
//	@author	 Imanol Celaya Ruiz de Alegria
//
//	Date Of Creation	: 19/04/2016 13:38:36
//	Last Modification	:	
*/	
/*	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
#ifndef _GRPTEXTUREFORMATS_H_
#define _GRPTEXTUREFORMATS_H_
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
	
/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
// The great format enum
enum GRPTEXTURE_FORMAT
{
  GRPTEXTURE_FORMAT_R = 0															,
  GRPTEXTURE_FORMAT_RG																,
  GRPTEXTURE_FORMAT_RGB																,
  GRPTEXTURE_FORMAT_RGBA															,
  GRPTEXTURE_FORMAT_R8																,
  GRPTEXTURE_FORMAT_RG8																,
  GRPTEXTURE_FORMAT_RGB8															,
  GRPTEXTURE_FORMAT_RGBA8															,
  GRPTEXTURE_FORMAT_R16F															,
  GRPTEXTURE_FORMAT_RG16F															,
  GRPTEXTURE_FORMAT_RGB16F														,
  GRPTEXTURE_FORMAT_RGBA16F														,
  GRPTEXTURE_FORMAT_R32F															,
  GRPTEXTURE_FORMAT_RG32F															,
  GRPTEXTURE_FORMAT_RGB32F														,
  GRPTEXTURE_FORMAT_RGBA32F														,
  GRPTEXTURE_FORMAT_R8I																,
  GRPTEXTURE_FORMAT_RG8I															,
  GRPTEXTURE_FORMAT_RGB8I															,
  GRPTEXTURE_FORMAT_RGBA8I														,
  GRPTEXTURE_FORMAT_R16I															,
  GRPTEXTURE_FORMAT_RG16I															,
  GRPTEXTURE_FORMAT_RGB16I														,
  GRPTEXTURE_FORMAT_RGBA16I														,
  GRPTEXTURE_FORMAT_R32I															,
  GRPTEXTURE_FORMAT_RG32I															,
  GRPTEXTURE_FORMAT_RGB32I														,
  GRPTEXTURE_FORMAT_RGBA32I														,
  GRPTEXTURE_FORMAT_R8UI															,
  GRPTEXTURE_FORMAT_RG8UI															,
  GRPTEXTURE_FORMAT_RGB8UI														,
  GRPTEXTURE_FORMAT_RGBA8UI														,
  GRPTEXTURE_FORMAT_R16UI															,
  GRPTEXTURE_FORMAT_RG16UI														,
  GRPTEXTURE_FORMAT_RGB16UI														,
  GRPTEXTURE_FORMAT_RGBA16UI													,
  GRPTEXTURE_FORMAT_R32UI															,
  GRPTEXTURE_FORMAT_RG32UI														,
  GRPTEXTURE_FORMAT_RGB32UI														,
  GRPTEXTURE_FORMAT_RGBA32UI													,

  // Gamma correct
  GRPTEXTURE_FORMAT_SRGB															,
  GRPTEXTURE_FORMAT_SRGBA															,

  // Compressed formats
  // S3TC
  GRPTEXTURE_FORMAT_RGB_S3TC_DXT1											,
  GRPTEXTURE_FORMAT_SRGB_S3TC_DXT1										,
  GRPTEXTURE_FORMAT_RGBA_S3TC_DXT1										,
  GRPTEXTURE_FORMAT_SRGBA_S3TC_DXT1										,
  GRPTEXTURE_FORMAT_RGBA_S3TC_DXT3										,
  GRPTEXTURE_FORMAT_SRGBA_S3TC_DXT3										,
  GRPTEXTURE_FORMAT_RGBA_S3TC_DXT5										,
  GRPTEXTURE_FORMAT_SRGBA_S3TC_DXT5										,

	// ETC2
	GRPTEXTURE_FORMAT_RGB8_ETC2,
	GRPTEXTURE_FORMAT_SRGB8_ETC2,
	GRPTEXTURE_FORMAT_RGB8_PUNCHTHROUGH_ALPHA1_ETC2			,
	GRPTEXTURE_FORMAT_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2		,
	GRPTEXTURE_FORMAT_RGBA8_ETC2_EAC										,
	GRPTEXTURE_FORMAT_SRGBA8_ETC2_EAC										,

	//ASTC
	GRPTEXTURE_FORMAT_RGBA_ASTC_4x4          						,
  GRPTEXTURE_FORMAT_RGBA_ASTC_5x4          						,
  GRPTEXTURE_FORMAT_RGBA_ASTC_5x5          						,
  GRPTEXTURE_FORMAT_RGBA_ASTC_6x5          						,
  GRPTEXTURE_FORMAT_RGBA_ASTC_6x6          						,
  GRPTEXTURE_FORMAT_RGBA_ASTC_8x5          						,
  GRPTEXTURE_FORMAT_RGBA_ASTC_8x6          						,
  GRPTEXTURE_FORMAT_RGBA_ASTC_8x8          						,
  GRPTEXTURE_FORMAT_RGBA_ASTC_10x5         						,
  GRPTEXTURE_FORMAT_RGBA_ASTC_10x6         						,
  GRPTEXTURE_FORMAT_RGBA_ASTC_10x8         						,
  GRPTEXTURE_FORMAT_RGBA_ASTC_10x10        						,
  GRPTEXTURE_FORMAT_RGBA_ASTC_12x10        						,
  GRPTEXTURE_FORMAT_RGBA_ASTC_12x12        						,

  GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_4x4  						,
  GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_5x4  						,
  GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_5x5  						,
  GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_6x5  						,
  GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_6x6  						,
  GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_8x5  						,
  GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_8x6  						,
  GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_8x8  						,
  GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_10x5 						,
  GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_10x6 						,
  GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_10x8 						,
  GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_10x10						,
  GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_12x10						,
  GRPTEXTURE_FORMAT_SRGB8_ALPHA8_ASTC_12x12						,

  // BPTC
  GRPTEXTURE_FORMAT_RGBA_BPTC_UNORM										,
  GRPTEXTURE_FORMAT_SRGBA_BPTC_UNORM									,
  GRPTEXTURE_FORMAT_RGB_BPTC_SIGNED_FLOAT							,
  GRPTEXTURE_FORMAT_RGB_BPTC_UNSIGNED_FLOAT						,

  // "Special" formats
  GRPTEXTURE_FORMAT_DEPTH															,
  GRPTEXTURE_FORMAT_DEPTH16														,
  GRPTEXTURE_FORMAT_DEPTH24														,
  GRPTEXTURE_FORMAT_DEPTH32F													,
  GRPTEXTURE_FORMAT_DEPTH_STENCIL											,

  GRPTEXTURE_FORMAT_END
};

	
/*---- CLASS -----------------------------------------------------------------------------*/
	
	
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/
	
#endif

