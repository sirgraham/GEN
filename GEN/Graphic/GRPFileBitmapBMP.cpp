//------------------------------------------------------------------------------------------
//	GRPFILEBITMAPBMP.CPP
//	
//	Graphic File Bitmap BMP class
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

#include "GRPFileBitmapBMP.h"

#include "XMemory.h"

	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  GRPFILEBITMAPBMPHEADER::GRPFILEBITMAPBMPHEADER
*/ 
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/11/2013 11:32:37
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
GRPFILEBITMAPBMPHEADER::GRPFILEBITMAPBMPHEADER()
{
	Clean();
}


/*-------------------------------------------------------------------
//  GRPFILEBITMAPBMPHEADER::~GRPFILEBITMAPBMPHEADER
*/ 
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			04/11/2013 11:33:03
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
GRPFILEBITMAPBMPHEADER::~GRPFILEBITMAPBMPHEADER()
{
	Clean();	

	DeleteAllPalette();
}



//-------------------------------------------------------------------
//  GRPFILEBITMAPBMPHEADER::Load
/**
//
//
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/11/2002 10:40:28
//
//	@return 			bool :
//	@param				file :
*/
//-------------------------------------------------------------------
bool GRPFILEBITMAPBMPHEADER::Load(XFILE* file)
{
	if(!file->Read((XBYTE*)&headerinfo.type							, sizeof(XWORD)))	 		return false;
	if(!file->Read((XBYTE*)&headerinfo.size							, sizeof(XDWORD))) 		return false;
	if(!file->Read((XBYTE*)&headerinfo.reserved1				, sizeof(XWORD)))			return false;
	if(!file->Read((XBYTE*)&headerinfo.reserved2				, sizeof(XWORD)))			return false;
	if(!file->Read((XBYTE*)&headerinfo.offsetdata				, sizeof(XDWORD))) 		return false;

	if(!file->Read((XBYTE*)&headerinfo.headersize				, sizeof(XDWORD)))		return false;
	if(!file->Read((XBYTE*)&headerinfo.width						, sizeof(XDWORD)))		return false;
	if(!file->Read((XBYTE*)&headerinfo.height						, sizeof(XDWORD)))		return false;
	if(!file->Read((XBYTE*)&headerinfo.planes						, sizeof(XWORD)))			return false;
	if(!file->Read((XBYTE*)&headerinfo.bitsperpixel			, sizeof(XWORD)))			return false;
	if(!file->Read((XBYTE*)&headerinfo.compression			, sizeof(XDWORD)))		return false;
	if(!file->Read((XBYTE*)&headerinfo.sizebmp					, sizeof(XDWORD)))		return false;
	if(!file->Read((XBYTE*)&headerinfo.xpixelspermeter	, sizeof(XDWORD)))		return false;
	if(!file->Read((XBYTE*)&headerinfo.ypixelspermeter	, sizeof(XDWORD)))		return false;
	if(!file->Read((XBYTE*)&headerinfo.colorused				, sizeof(XDWORD)))		return false;
	if(!file->Read((XBYTE*)&headerinfo.colorimportant		, sizeof(XDWORD)))		return false;

	// Check: "BM" signature in the file for WINDOWS NO OS/2 valid format.
	if(headerinfo.type!=0x4D42) return false;

	switch(headerinfo.bitsperpixel)
		{
			case	8 : mode = GRPPROPERTYMODE_08_INDEX;				
								break;

			case 16	: {	XDWORD maskcolor[4];
									if(!file->Read((XBYTE*)maskcolor, 16)) return false;
				
									if((maskcolor[0] == 0x00007c00) && (maskcolor[1] == 0x000003E0) && (maskcolor[2] == 0x0000001F) && (maskcolor[3] == 0x00008000)) mode = GRPPROPERTYMODE_16_RGBA_5551; 											
									if((maskcolor[0] == 0x0000F800) && (maskcolor[1] == 0x000007E0) && (maskcolor[2] == 0x0000001F) && (maskcolor[3] == 0x00000000)) mode = GRPPROPERTYMODE_16_RGB_565;
								}
								break;

			
			case 24	: mode = GRPPROPERTYMODE_24_RGB_888;			
								break;

			case 32	: mode = GRPPROPERTYMODE_32_RGBA_8888;		
								break;
		}
	

	DeleteAllPalette();

	int ncpalette = headerinfo.colorused;

	if(ncpalette)
		{	
			for(int c=0;c<ncpalette;c++)				
				{
					XBYTE red;
					XBYTE green;
					XBYTE blue;
					XBYTE alpha;
					
					if(!file->Read(&blue	,1)) return false;
					if(!file->Read(&green ,1)) return false;
					if(!file->Read(&red		,1)) return false;
					if(!file->Read(&alpha	,1)) return false;

					GRPFILEBITMAPBMPCOLORPALETE* colorpalette = new GRPFILEBITMAPBMPCOLORPALETE(red, green, blue, alpha);
					if(colorpalette) palette.Add((XBYTE)c, colorpalette);																
				}
		}


	
	return true;
}





//-------------------------------------------------------------------
//  GRPFILEBITMAPBMPHEADER::Clean
/**
//
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/11/2002 11:04:30
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void GRPFILEBITMAPBMPHEADER::Clean()
{	
	mode = GRPPROPERTYMODE_XX_UNKNOWN;
	memset((XBYTE*)&headerinfo	,0	,sizeof(GRPFILEBITMAPBMPHEADERINFO));
}




/*-------------------------------------------------------------------
//  GRPFILEBITMAPBMP::GRPFILEBITMAPBMP
*/ 
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/11/2013 10:56:59
//	
//	@return 			

//  @param				grpfactory : 
*/
/*-----------------------------------------------------------------*/
GRPFILEBITMAPBMP::GRPFILEBITMAPBMP() : GRPFILEBITMAPBASE()
{
	Clean();		
}



//-------------------------------------------------------------------
//  GRPFILEBITMAPBMP::~GRPFILEBITMAPBMP
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/11/2002 18:35:37
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
GRPFILEBITMAPBMP::~GRPFILEBITMAPBMP()
{	
	Clean();
}



/*-------------------------------------------------------------------
//  GRPFILEBITMAPBMP::CreateBitmapFromFile
*/ 
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			13/11/2013 9:23:23
//	
//	@return 			GRPBITMAP* : 
//	@param				xpath : 
*/
/*-----------------------------------------------------------------*/
GRPBITMAP* GRPFILEBITMAPBMP::CreateBitmapFromFile(XPATH& xpath)
{
	if(!Open(xpath))	return NULL;
	
	GRPBITMAP*				bitmap = NULL;
	GRPFILEBITMAPBMPHEADER	header;
	XBYTE*						buffer = NULL;
	bool							status = true;

	if(!ReadHeader(&header)) 
		{
			Close();
			return NULL;
		}
	
	int	 width				= header.GetWidth();
	int  height				= header.GetHeight();	
	int	 bytesppixel	= (header.GetBitsPerPixel()/8);

	bitmap = grpfactory->CreateBitmap(width, height, header.GetMode());
	if(bitmap)
		{			
			int			sizebmp				= (((header.GetWidth()*bytesppixel)+3)&(~3))*header.GetHeight();
			XBYTE*	databuffer		= NULL;			
			int			index					= 0;
			
			databuffer = new XBYTE[sizebmp];
			if(databuffer)
				{
					file->SetPosition(header.GetOffsetData());
	
					if(!file->Read(databuffer, sizebmp)) return NULL;			

					for(int y=height-1; y>-1; y--)
						{							
							for(int x=0; x<width; x++)
								{
									GRPBUFFERCOLORRGBA8 color;
							
									switch(header.GetMode())
										{
											case GRPPROPERTYMODE_08_INDEX			:	header.GetColorPalette(databuffer[index],	color);
																													color.a = 255;
																													bitmap->PutPixel(x, y, (GRPBUFFERCOLOR*)&color);
																													break;

											case GRPPROPERTYMODE_16_RGBA_5551	:	{	XWORD rgb = databuffer[index+1];
																														rgb <<= 8;
																														rgb |= databuffer[index+0];

																														color.r = (rgb >> 7) & 0xF8;
																														color.g = (rgb >> 2) & 0xF8;
																														color.b = (rgb << 3) & 0xF8;
																														color.a = (rgb & 0x8000)?255:0;

																														bitmap->PutBlendPixel(x, y, (GRPBUFFERCOLOR*)&color, (int)color.a);
																													}																												
																													break;

											case GRPPROPERTYMODE_16_RGB_565		:	{	XWORD rgb = databuffer[index+1];
																														rgb <<= 8;
																														rgb |= databuffer[index+0];

																														color.r = (rgb >> 8) & 0xF8;
																														color.g = (rgb >> 3) & 0xFC;
																														color.b = (rgb << 3) & 0xF8;
																														color.a = 255;

																														bitmap->PutPixel(x, y, (GRPBUFFERCOLOR*)&color);
																													}
																													break;
											
											case GRPPROPERTYMODE_24_RGB_888		: color.r	= databuffer[index+2];
																													color.g = databuffer[index+1];
																													color.b	= databuffer[index+0];
																													color.a = 255;

																													bitmap->PutPixel(x, y, (GRPBUFFERCOLOR*)&color);
																													break;

											case GRPPROPERTYMODE_32_RGBA_8888	: color.r	= databuffer[index+2];
																													color.g = databuffer[index+1];
																													color.b	= databuffer[index+0];
																													color.a = databuffer[index+3];

																													bitmap->PutBlendPixel(x, y, (GRPBUFFERCOLOR*)&color, (int)color.a);
																													break;
											default: break;
										}
																		
									index	 += bytesppixel;
								}
						}

				}	else status = false;
						
			delete [] databuffer;

		}	else status = false;
	
	
	if(!status)
		{
			if(bitmap) 
				{
					grpfactory->DeleteBitmap(bitmap);
					bitmap = NULL;
				}
		}

	Close();

  return bitmap;
}




/*-------------------------------------------------------------------
//  GRPFILEBITMAPBMP::CreateFileFromBitmap
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
bool GRPFILEBITMAPBMP::CreateFileFromBitmap(XPATH& xpath, GRPBITMAP* bitmap)
{
	if(!grpfactory) return false;
	if(!bitmap)			return false;

	if(!Create(xpath)) return false;
	
	GRPFILEBITMAPBMPHEADER header;

	header.GetHeaderInfo()->type						= 0x4D42;  // BM Windows	 
	header.GetHeaderInfo()->size						= 0;
	header.GetHeaderInfo()->reserved1				= 0;
	header.GetHeaderInfo()->reserved2				= 0;
	header.GetHeaderInfo()->offsetdata			= sizeof(GRPFILEBITMAPBMPHEADERINFO);

	header.GetHeaderInfo()->headersize			= sizeof(GRPFILEBITMAPBMPHEADERINFO) - 14;		
	header.GetHeaderInfo()->width						= bitmap->GetWidth();
	header.GetHeaderInfo()->height					= bitmap->GetHeight();
	header.GetHeaderInfo()->planes					= 1;
	header.GetHeaderInfo()->bitsperpixel		= 32;
	header.GetHeaderInfo()->compression			= GRPFILEBITMAPBMPCOMPRESSION_NOT;
	header.GetHeaderInfo()->sizebmp					= 0;
	header.GetHeaderInfo()->xpixelspermeter = 2835;	
	header.GetHeaderInfo()->ypixelspermeter	= 2835;
	header.GetHeaderInfo()->colorused				= 0;
	header.GetHeaderInfo()->colorimportant	= 0;

	int	 width				= bitmap->GetWidth();
	int  height				= bitmap->GetHeight();	
	int	 bytesppixel	= (header.GetBitsPerPixel()/8);
	int	 sizebmp			= (((header.GetWidth()*bytesppixel)+3)&(~3))*header.GetHeight();
	int	 index				= 0;
	bool status				= true;
	
	header.GetHeaderInfo()->sizebmp	= sizebmp;
	header.GetHeaderInfo()->size			= sizebmp + sizeof(GRPFILEBITMAPBMPHEADERINFO); 

	if(file->Write((XBYTE*)header.GetHeaderInfo(), sizeof(GRPFILEBITMAPBMPHEADERINFO)))	
		{
			XBYTE* databuffer = new XBYTE[sizebmp];
			if(databuffer)
				{
					for(int y=height-1; y>-1; y--)
						{							
							for(int x=0; x<width; x++)
								{
									GRPBUFFERCOLORRGBA8* color = (GRPBUFFERCOLORRGBA8*)bitmap->GetPixel(x, y);
									if(color)
										{
											databuffer[index+2] = color->r;
											databuffer[index+1] = color->g;
											databuffer[index+0] = color->b;
											databuffer[index+3]	= color->a;
										}

									index	 += bytesppixel;
								}
						}
			
					if(!file->Write((XBYTE*)databuffer, sizebmp))	status = false;
					
					delete [] databuffer;

				} else status = false;

		} else status = false;
	
	Close();

	return status;
}




/*-------------------------------------------------------------------
//  GRPFILEBITMAPBMP::ReadHeader
*/ 
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/11/2013 11:13:05
//	
//	@return 			bool : 
//	@param				header : 
*/
/*-----------------------------------------------------------------*/
bool GRPFILEBITMAPBMP::ReadHeader(GRPFILEBITMAPBMPHEADER* header)
{
	if(!header)						return false;
	if(!GetPrimaryFile()) return false;
	
	if(!header->Load(GetPrimaryFile())) return false;

	return true;
}




//-------------------------------------------------------------------
//  GRPFILEBITMAPBMP::Clean
/**
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			20/11/2002 11:08:06
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void GRPFILEBITMAPBMP::Clean()
{
	
}



