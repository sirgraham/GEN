//------------------------------------------------------------------------------------------
//	GRPFILEBITMAPPNG.CPP
//	
//	Graphic File Bitmap PNG class
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 19/11/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"

#include "GRPFactory.h"
#include "GRPBitmap.h"

#include "GRPFileBitmapPNG.h"

#include "XMemory.h"

	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  GRPFILEBITMAPPNG::GRPFILEBITMAPPNG
*/ 
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/11/2013 9:43:44
//	
//	@return 			

//  @param				grpfactory : 
*/
/*-----------------------------------------------------------------*/
GRPFILEBITMAPPNG::GRPFILEBITMAPPNG() : GRPFILEBITMAPBASE()
{
	Clean();		
}



//-------------------------------------------------------------------
//  GRPFILEBITMAPPNG::~GRPFILEBITMAPPNG
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/11/2002 18:35:37
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
GRPFILEBITMAPPNG::~GRPFILEBITMAPPNG()
{	
	Clean();
}



/*-------------------------------------------------------------------
//  GRPFILEBITMAPPNG::CreateBitmapFromFile
*/ 
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/11/2013 11:35:56
//	
//	@return 			GRPBITMAP* : 
//	@param				xpath : 
//  @param				propertys : 
*/
/*-----------------------------------------------------------------*/
GRPBITMAP* GRPFILEBITMAPPNG::CreateBitmapFromFile(XPATH& xpath)
{
	if(!grpfactory)		return NULL;
	
	if(!Open(xpath))	return NULL;

	GRPBITMAP*		bitmap			= NULL;
	png_bytep*		databuffer	= NULL;
	XBYTE					header[8];
	png_structp		png;
  png_infop			info;
	png_uint_32		width				= 0;
	png_uint_32		height			= 0;
	png_byte			colortype		= 0;
	int						bytesppixel = 0;
	int						rowbytes		= 0;
	int						npasses			= 0;

	if(file->Read(header, GRPFILEBITMAPPNG_HEADERSIZE))
		{	
			if(!png_sig_cmp(header, 0, 8))
				{
				  png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
					if(png)
						{
							info = png_create_info_struct(png);
							if(info)
								{
									if(!setjmp(png_jmpbuf(png)))
										{
											png_set_read_fn(png	, (void *)this, GRPFILEBITMAPPNG::ReadData);

										  png_set_sig_bytes(png, 8);

											png_read_info(png, info);

											width				 = png_get_image_width(png, info);
											height			 = png_get_image_height(png, info);
											colortype		 = png_get_color_type(png, info);
							        bytesppixel	 = png_get_channels(png, info);
											rowbytes		 = png_get_rowbytes(png, info);
											npasses			 = png_set_interlace_handling(png);
											
											png_read_update_info(png, info);
        
											if(!setjmp(png_jmpbuf(png)))
												{                
													databuffer = (png_bytep*)new XBYTE[sizeof(png_bytep) * height];
													for(int y=0; y<(int)height; y++)
														{
															databuffer[y] = (png_byte*)new XBYTE[rowbytes];															
														}
										
													png_read_image(png, databuffer);
													png_destroy_read_struct(&png, &info, NULL);

													GRPPROPERTYMODE mode = GRPPROPERTYMODE_XX_UNKNOWN;
													
													switch(colortype)
														{
															case PNG_COLOR_TYPE_RGB					: mode = GRPPROPERTYMODE_24_RGB_888;
															case PNG_COLOR_TYPE_RGB_ALPHA		: mode = GRPPROPERTYMODE_32_RGBA_8888;
														}
											
													bitmap = grpfactory->CreateBitmap(width, height, mode);
													if(bitmap)
														{																		
															int	index	= 0;

															for(int y=0; y<(int)height; y++)
																{
																	index = 0;

																	for(int x=0; x<(int)width; x++)
																		{
																			GRPBUFFERCOLORRGBA8 color;
							
																			color.r	= (XBYTE)(*(XBYTE*)(databuffer[y] + index + 0));
																			color.g = (XBYTE)(*(XBYTE*)(databuffer[y] + index + 1));
																			color.b	= (XBYTE)(*(XBYTE*)(databuffer[y] + index + 2));

																			if(colortype == PNG_COLOR_TYPE_RGB)
																				{
																					color.a = 255;
																				}
																			else
																				{
																					color.a = (XBYTE)(*(XBYTE*)(databuffer[y] + index + 3));
																				}

																			bitmap->PutPixel(x, y, (GRPBUFFERCOLOR*)&color);
																												
																			index	 += bytesppixel;
																		}
																}

														}
													
													for(int y=0; y<(int)height; y++)
														{
															delete [] databuffer[y];
														}

													delete [] databuffer;														
												}
										}							
								}
						}
				}
		}

	Close();

	return bitmap;
}





/*-------------------------------------------------------------------
//  GRPFILEBITMAPPNG::CreateFileFromBitmap
*/ 
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			05/11/2013 9:31:08
//	
//	@return 			bool : 
//	@param				xpath : 
//  @param				bitmap : 
*/
/*-----------------------------------------------------------------*/
bool GRPFILEBITMAPPNG::CreateFileFromBitmap(XPATH& xpath, GRPBITMAP* bitmap)
{
	if(!grpfactory) return false;
	if(!bitmap)			return false;

	if(!Create(xpath)) return false;

	png_structp		png;
  png_infop			info;
	png_bytep*		databuffer	= NULL;
	png_uint_32		width				= bitmap->GetWidth();
	png_uint_32		height			= bitmap->GetHeight();
	png_byte			bitdepth		= 8;
	int						bytesppixel = 4;
	int						rowbytes		= 0;
	bool					status			= true;

  png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png)
		{
			info = png_create_info_struct(png);
      if(info)
				{
					if(!setjmp(png_jmpbuf(png)))
						{
							png_set_write_fn(png	, (void *)this, GRPFILEBITMAPPNG::WriteData, NULL);

							if(!setjmp(png_jmpbuf(png)))
								{
									rowbytes = width * 4;

									databuffer = (png_bytep*)new XBYTE[sizeof(png_bytep) * height];
									for(int y=0; y<(int)height; y++)
										{
											databuffer[y] = (png_byte*)new XBYTE[rowbytes];															
										}

									png_set_IHDR(png, info, width											, 
																					height										,
																					bitdepth									, 
																					PNG_COLOR_TYPE_RGB_ALPHA	, 
																					PNG_INTERLACE_NONE				,
																					PNG_COMPRESSION_TYPE_BASE	, 
																					PNG_FILTER_TYPE_BASE);

									png_write_info(png, info);
									
									if(!setjmp(png_jmpbuf(png)))
										{				
											int	index	= 0;

											for(int y=0; y<(int)height; y++)
												{							
													index	= 0;

													for(int x=0; x<(int)width; x++)
														{
															GRPBUFFERCOLORRGBA8* color = (GRPBUFFERCOLORRGBA8*)bitmap->GetPixel(x, y);
															if(color)
																{
																	XBYTE* ptr = (XBYTE*)(databuffer[y] + index);

																	ptr[0] = color->r;
																	ptr[1] = color->g;
																	ptr[2] = color->b;
																	ptr[3] = color->a;																	
																}

															index	 += bytesppixel;
														}
												}

											png_write_image(png, databuffer);
        
											if(!setjmp(png_jmpbuf(png)))
												{
												  png_write_end(png, NULL);
												}
										}
										
									for(int y=0; y<(int)height; y++)
										{
											delete [] databuffer[y];
										}

									delete [] databuffer;			

								}
						}
				}
		}

	Close();

	return status;
}




//-------------------------------------------------------------------
//  GRPFILEBITMAPPNG::Clean
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/11/2002 11:08:06
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void GRPFILEBITMAPPNG::Clean()
{
	
}



/*-------------------------------------------------------------------
//  GRPFILEBITMAPPNG::ReadData
*/ 
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/11/2013 11:43:56
//	
//	@return 			void : 
//	@param				png : 
//  @param				data : 
//  @param				length : 
*/
/*-----------------------------------------------------------------*/
void GRPFILEBITMAPPNG::ReadData(png_structp png, png_bytep data, png_size_t length)
{
	GRPFILEBITMAPPNG* xfilepng = (GRPFILEBITMAPPNG*)png_get_io_ptr(png);
	if(!xfilepng) return;

	xfilepng->GetPrimaryFile()->Read((XBYTE*)data, (int)length);
}



/*-------------------------------------------------------------------
//  GRPFILEBITMAPPNG::WriteData
*/ 
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			07/11/2013 16:08:00
//	
//	@return 			void : 
//	@param				png : 
//  @param				data : 
//  @param				length : 
*/
/*-----------------------------------------------------------------*/
void GRPFILEBITMAPPNG::WriteData(png_structp png, png_bytep data, png_size_t length)
{
	GRPFILEBITMAPPNG* xfilepng = (GRPFILEBITMAPPNG*)png_get_io_ptr(png);
	if(!xfilepng) return;

	xfilepng->GetPrimaryFile()->Write((XBYTE*)data, (int)length);
}
