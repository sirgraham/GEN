//------------------------------------------------------------------------------------------
//	GRPFILEBITMAPTGA.CPP
//	
//	Graphic File Bitmap TGA class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 19/11/2002
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.		 			 
//------------------------------------------------------------------------------------------

	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"

#include "GRPFactory.h"
#include "GRPBitmap.h"

#include "GRPFileBitmapTGA.h"

#include "XMemory.h"

	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  GRPFILEBITMAPTGAHEADER::GRPFILEBITMAPTGAHEADER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/11/2013 11:32:37
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
GRPFILEBITMAPTGAHEADER::GRPFILEBITMAPTGAHEADER()
{
	Clean();
}


/*-------------------------------------------------------------------
//  GRPFILEBITMAPTGAHEADER::~GRPFILEBITMAPTGAHEADER
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			04/11/2013 11:33:03
//	
//	@return 			
//	*/
/*-----------------------------------------------------------------*/
GRPFILEBITMAPTGAHEADER::~GRPFILEBITMAPTGAHEADER()
{
	Clean();	
}



//-------------------------------------------------------------------
//  GRPFILEBITMAPTGAHEADER::Load
/**
//
//
//	@author				Abraham J. Velez
//	@version			20/11/2002 10:40:28
//
//	@return 			bool :
//	@param				file :
*/
//-------------------------------------------------------------------
bool GRPFILEBITMAPTGAHEADER::Load(XFILE* file)
{
	if(!file->Read((XBYTE*)&headerinfo.IDlength					, sizeof(XBYTE)))	return false;
	if(!file->Read((XBYTE*)&headerinfo.colourmaptype		, sizeof(XBYTE)))	return false;	
	if(!file->Read((XBYTE*)&headerinfo.datatypecode			, sizeof(XBYTE)))	return false;
	if(!file->Read((XBYTE*)&headerinfo.colourmaporigin	, sizeof(XWORD)))	return false;
	if(!file->Read((XBYTE*)&headerinfo.colourmaplength	, sizeof(XWORD)))	return false;
	if(!file->Read((XBYTE*)&headerinfo.colourmapdepth		, sizeof(XBYTE)))	return false;
	if(!file->Read((XBYTE*)&headerinfo.xorigin					, sizeof(XWORD)))	return false;
	if(!file->Read((XBYTE*)&headerinfo.yorigin					, sizeof(XWORD)))	return false;
	if(!file->Read((XBYTE*)&headerinfo.width						, sizeof(XWORD)))	return false;
	if(!file->Read((XBYTE*)&headerinfo.height						, sizeof(XWORD)))	return false;
	if(!file->Read((XBYTE*)&headerinfo.bitsperpixel			, sizeof(XBYTE)))	return false;
	if(!file->Read((XBYTE*)&headerinfo.imagedescriptor	, sizeof(XBYTE)))	return false;

	// Only RGB format valid.
	if((headerinfo.datatypecode != GRPFILEBITMAPTGATYPE_UNCOMPRESS_RGB) && 
	   (headerinfo.datatypecode != GRPFILEBITMAPTGATYPE_RLECOMPRESS_RGB))  return false;
		     
	// Only 16,24,32 bits format valid.
  if((headerinfo.bitsperpixel != 16) && 
	   (headerinfo.bitsperpixel != 24) && 
		 (headerinfo.bitsperpixel != 32)) return false; 
			      
	// Only 0,1 colour map types 
  if((headerinfo.colourmaptype != 0) && (headerinfo.colourmaptype != 1)) return false; 	     

	switch(headerinfo.bitsperpixel)
		{
			case 16	: mode = GRPPROPERTYMODE_16_RGBA_5551;	break;			
			case 24	: mode = GRPPROPERTYMODE_24_RGB_888;		break;
			case 32	: mode = GRPPROPERTYMODE_32_RGBA_8888;	break;
		}	

	offsetdata += sizeof(GRPFILEBITMAPTGAHEADERINFO);
	offsetdata += headerinfo.IDlength;
  offsetdata += headerinfo.colourmaptype * headerinfo.colourmaplength;
	
	return true;
}





//-------------------------------------------------------------------
//  GRPFILEBITMAPTGAHEADER::Clean
/**
//
//	
//	@author				Abraham J. Velez
//	@version			20/11/2002 11:04:30
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void GRPFILEBITMAPTGAHEADER::Clean()
{	
	mode			 = GRPPROPERTYMODE_XX_UNKNOWN;	
	memset((XBYTE*)&headerinfo	,0	,sizeof(GRPFILEBITMAPTGAHEADERINFO));
	offsetdata = 0;
}




/*-------------------------------------------------------------------
//  GRPFILEBITMAPTGA::GRPFILEBITMAPTGA
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/11/2013 9:43:44
//	
//	@return 			

//  @param				grpfactory : 
*/
/*-----------------------------------------------------------------*/
GRPFILEBITMAPTGA::GRPFILEBITMAPTGA() : GRPFILEBITMAPBASE()
{
	Clean();		
}



//-------------------------------------------------------------------
//  GRPFILEBITMAPTGA::~GRPFILEBITMAPTGA
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			19/11/2002 18:35:37
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
GRPFILEBITMAPTGA::~GRPFILEBITMAPTGA()
{	
	Clean();
}


/*-------------------------------------------------------------------
//  GRPFILEBITMAPTGA::CreateBitmapFromFile
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			13/11/2013 9:25:35
//	
//	@return 			GRPBITMAP* : 
//	@param				xpath : 
*/
/*-----------------------------------------------------------------*/
#define USE_FAST_TGA_LOAD
#ifndef USE_FAST_TGA_LOAD
GRPBITMAP* GRPFILEBITMAPTGA::CreateBitmapFromFile(XPATH& xpath)
{
	if(!grpfactory)		return NULL;

	if(!Open(xpath))	return NULL;
	
	GRPBITMAP*				bitmap = NULL;
	GRPFILEBITMAPTGAHEADER	header;
	XBYTE*						buffer = NULL;
	bool							status = true;

	if(!ReadHeader(&header)) 
		{
			Close();
			return NULL;
		}
	
	int										width				= header.GetWidth();
	int										height			= header.GetHeight();	
	int										bytesppixel	= (header.GetBitsPerPixel()/8);
	GRPBUFFERCOLORRGBA8*	colors			= NULL;
	int										index				= 0;

	bitmap = grpfactory->CreateBitmap(width, height, header.GetMode());
	if(bitmap)
		{				
			XBYTE databuffer[5];
					
			file->SetPosition(header.GetOffsetData());

			XBUFFER filebuffer( false);
			filebuffer.Resize(height*width*bytesppixel);
			file->Read(filebuffer.Get(), height*width*bytesppixel);
			filebuffer.SetPosition(0);

			colors = new GRPBUFFERCOLORRGBA8[(height*width)];
			if(colors)
				{					 
					while(index < (height*width))
						{
							switch(header.GetType())
								{
								case GRPFILEBITMAPTGATYPE_UNCOMPRESS_RGB	:	if(filebuffer.Get(databuffer, bytesppixel))
																												{
																													CreateColor(header.GetMode(), databuffer, colors[index]);																													
																													index++;

																												} else status = false;
																						
																											break;

									case GRPFILEBITMAPTGATYPE_RLECOMPRESS_RGB	: { 
																												if(filebuffer.Get(databuffer, bytesppixel))
																													{
																														int j = databuffer[0] & 0x7F;

																														CreateColor(header.GetMode(), &databuffer[1], colors[index]);																														
																														index++;

																														if(databuffer[0] & 0x80)
																															{
																																// RLE chunk 
																																for(int c=0; c<j; c++) 
            																											{
               																											CreateColor(header.GetMode(), &databuffer[1], colors[index]);																																		
               																											index++;
            																											}
																															}
																														 else
																														  {
																																// Normal chunk 
            																										for(int c=0; c<j; c++) 
            																											{
               																											if(filebuffer.Get(databuffer, bytesppixel))
               																												{
																																				CreateColor(header.GetMode(), databuffer, colors[index]);																																				
               																													index++;
                  																											
               																												} else status = false;               				               																											
            																											}
																															}

																													} else status = false;
																											}
																											break;
										default: break;
								}

							if(!status) break;
					  }
				}
		}
			
	index = 0;

	if(status)		
		{
			for(int y=height-1; y>-1; y--)
				{							
					for(int x=0; x<width; x++)
						{
							switch(header.GetMode())
								{
									case GRPPROPERTYMODE_16_RGBA_5551	:	bitmap->PutBlendPixel(x, y, (GRPBUFFERCOLOR*)&colors[index], (int)colors[index].a);
																											break;

									case GRPPROPERTYMODE_24_RGB_888		: bitmap->PutPixel(x, y, (GRPBUFFERCOLOR*)&colors[index]);
																											break;

									case GRPPROPERTYMODE_32_RGBA_8888	: //bitmap->PutBlendPixel(x, y, (GRPBUFFERCOLOR*)&colors[index], (int)colors[index].a);
																											bitmap->PutPixel(x, y, (GRPBUFFERCOLOR*)&colors[index]);
																											break;
									default: break;
								}
							
							index++;
						}
				}
		}	
	 else
		{
			if(bitmap) 
				{
					grpfactory->DeleteBitmap(bitmap);
					bitmap = NULL;
				}
		}

	delete [] colors;

	Close();

  return bitmap;
}

#else

GRPBITMAP* GRPFILEBITMAPTGA::CreateBitmapFromFile(XPATH& xpath)
{

	if(!Open(xpath))	return NULL;	
	

	XBYTE* chunk=new XBYTE[GetPrimaryFile()->GetSize()];
	
	if (!GetPrimaryFile()->Read(chunk,GetPrimaryFile()->GetSize()))
		return NULL;
	
	Decompress(chunk,GetPrimaryFile()->GetSize());

	delete(chunk);
	Close();


	bitmap = grpfactory->CreateBitmap(width, height, (this->bitsperpixel==24 ? GRPPROPERTYMODE_24_RGB_888 : GRPPROPERTYMODE_32_RGBA_8888));
	memcpy(bitmap->GetBuffer(),this->Data,(int)this->DataSize);
	delete(this->Data);
	
	return bitmap;
}

XQWORD GRPFILEBITMAPTGA::SeekBuffer (XBYTE * Buffer,int size, unsigned int from,  int MaxSize)
{
	switch(from)
	{
	case SET: Pointer=size;	break;
	case CUR: Pointer+=size;break;
	case END: Pointer=MaxSize+size;break;
	}
	
	return 0;
}

bool GRPFILEBITMAPTGA::ReadData (XBYTE * file, void * dest, int size, int MaxSize)
{
	memcpy(dest,(unsigned char*)file+Pointer,size);
	Pointer+=size;

	if (Pointer>MaxSize)
		return 1;
	

	return 0;
}

bool GRPFILEBITMAPTGA::Decompress(XBYTE * file, XQWORD BufferSize)
{
	Pointer=0;

	bool rle = false;
    bool truecolor = false;
    XDWORD CurrentPixel = 0;
    XBYTE ch_buf1, ch_buf2;
    XBYTE buf1[1000];

	int AlphaDepth=0;
	int Mode=0;
	unsigned int nBits=0;

    XBYTE IDLength=0;
    XBYTE IDColorMapType=0;
    XBYTE IDImageType=0;

    ReadData(file, (char*)&IDLength, 1, (int)BufferSize);
    ReadData(file, (char*)&IDColorMapType, 1, (int)BufferSize);
    
    if (IDColorMapType == 1) //color map no soportado
        return 1;

    ReadData(file, (char*)&IDImageType, 1, (int)BufferSize); 

    switch (IDImageType)
    {
    case 2:
            truecolor = true;
            break;
    case 3:
            Mode = itGreyscale;
            break;
    case 10:
            rle = true;
            truecolor = true;
            break;
    case 11:
            rle = true;
            Mode = itGreyscale;
            break;
    default:
            return 1;
    }

   // SeekBuffer(file, 5, CUR, (int)BufferSize);
   // SeekBuffer(file, 4, CUR, (int)BufferSize);
			SeekBuffer(file, 9, CUR, (int)BufferSize);

	int w=0,h=0;
	nBits=0;

    ReadData(file, (char*)&w, 2, (int)BufferSize);
    ReadData(file, (char*)&h, 2, (int)BufferSize);
    ReadData(file, (char*)&nBits, 1, (int)BufferSize);


	this->width=w;
	this->height=h;

	this->nChannels=nBits>>3;
	this->bitsperpixel=nBits;

    if (! ((nBits == 8) || (nBits ==  24) ||
             (nBits == 16) || (nBits == 32)))
        return 1;

    ReadData(file, (char*)&ch_buf1, 1, (int)BufferSize);     
		imageOrigin=(IMAGE_ORIGIN)((ch_buf1>>4)&3);

    ch_buf2 = 15; //00001111;
    AlphaDepth = ch_buf1 & ch_buf2;

    if (! ((AlphaDepth == 0) || (AlphaDepth == 8)))
        return 1;

    if (truecolor)
    {
        Mode = itRGB;
        if (nBits == 32)
            Mode = itRGBA;
    }

    if (Mode == itUndefined)
        return 1;

    SeekBuffer(file, IDLength, CUR, (int)BufferSize);

		DataSize	=(width*height*nChannels);
		Data			=(XBYTE*)new XBYTE[(unsigned int)DataSize];

    if (!rle)
        ReadData(file, (char*)this->Data, (int)DataSize, (int)BufferSize);
    else
    {
				XDWORD totalpixels=this->width*this->height -1;
        while (CurrentPixel < totalpixels)
        {
            ReadData(file, (char*)&ch_buf1, 1, (int)BufferSize);
            if ((ch_buf1 & 0x80))
            {   
							  // this is an rle packet
                ch_buf2 = (XBYTE)((ch_buf1 & 0x7F) + 1);   // how many pixels are encoded using this packet

                ReadData(file, (char*)buf1, nChannels, (int)BufferSize);

                for (XDWORD i=CurrentPixel; i<CurrentPixel+ch_buf2; i++)
                    for (XDWORD j=0; j<nChannels; j++)
                        this->Data[i*nChannels+j] = buf1[j];

                CurrentPixel += ch_buf2;
            }
            else
            {   
								// this is a raw packet
                ch_buf2 = (XBYTE)((ch_buf1 & 127) + 1);
								ReadData(file, (char*)&(this->Data[CurrentPixel]), nChannels*ch_buf2, (int)BufferSize);								
                CurrentPixel += ch_buf2;
            }
        }
    }
	// m_loaded = true;
		
    //------------------------------------------ swap BGR(A) to RGB(A)
	
			XBYTE temp=0;
			XQWORD size=this->width*this->height;
		  for (XQWORD i= 0; i<size; i++)
            {
                temp = this->Data[i*(nChannels)];
                this->Data[i*(nChannels)] = this->Data[i*(nChannels)+2];
                this->Data[i*(nChannels)+2] = temp;
            }
						
		
	/*las imagenes salen con su Y invertida */
		
if (imageOrigin==TOP_LEFT || imageOrigin==TOP_RIGHT)
	{
	int row=sizeof(char)*(nBits/8)*this->width;		
	XBYTE * mt=(XBYTE*)new XBYTE[(row)];		
	for (XDWORD i= 0; i<this->height/2; i++)
            {
								const XDWORD offset=(i*row);
								const XDWORD target=((this->height-i-1)*row);

                memcpy(mt,this->Data+offset,row);
								memcpy(this->Data+offset,this->Data+target,row);
								memcpy(this->Data+target,mt,row);
            }
	delete(mt);
	}
	

return 0;
}
#endif


/*-------------------------------------------------------------------
//  GRPFILEBITMAPTGA::CreateFileFromBitmap
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/11/2013 9:31:08
//	
//	@return 			bool : 
//	@param				xpath : 
//  @param				bitmap : 
*/
/*-----------------------------------------------------------------*/


bool GRPFILEBITMAPTGA::CreateFileFromBitmap(XPATH& xpath, GRPBITMAP* bitmap)
{
	if(!grpfactory)			return false;
	if(!bitmap)					return false;

	if(!Create(xpath))	return false;
	
	GRPFILEBITMAPTGAHEADER header;

	header.GetHeaderInfo()->IDlength						= 0;
	header.GetHeaderInfo()->colourmaptype				= 0;
	header.GetHeaderInfo()->datatypecode				= GRPFILEBITMAPTGATYPE_UNCOMPRESS_RGB;
	header.GetHeaderInfo()->colourmaporigin			= 0;
	header.GetHeaderInfo()->colourmaplength			= 0;
	header.GetHeaderInfo()->colourmapdepth			= 0;
	header.GetHeaderInfo()->xorigin							= 0;
	header.GetHeaderInfo()->yorigin							= 0;
	header.GetHeaderInfo()->width								= bitmap->GetWidth();
	header.GetHeaderInfo()->height							= bitmap->GetHeight();
	header.GetHeaderInfo()->bitsperpixel				= 32;
	header.GetHeaderInfo()->imagedescriptor			= 8;

	int	 width				= bitmap->GetWidth();
	int  height				= bitmap->GetHeight();	
	int	 bytesppixel	= (header.GetBitsPerPixel()/8);
	int	 sizebmp			= (header.GetWidth()*header.GetHeight()*bytesppixel);
	int	 index				= 0;	
	bool status				= true;

	if(file->Write((XBYTE*)header.GetHeaderInfo(), sizeof(GRPFILEBITMAPTGAHEADERINFO)))	
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





//-------------------------------------------------------------------
//  GRPFILEBITMAPTGA::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2002 11:08:06
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void GRPFILEBITMAPTGA::Clean()
{
	
}




/*-------------------------------------------------------------------
//  GRPFILEBITMAPTGA::ReadHeader
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/11/2013 11:13:05
//	
//	@return 			bool : 
//	@param				header : 
*/
/*-----------------------------------------------------------------*/
bool GRPFILEBITMAPTGA::ReadHeader(GRPFILEBITMAPTGAHEADER* header)
{
	if(!header)						return false;
	if(!GetPrimaryFile()) return false;
	
	if(!header->Load(GetPrimaryFile())) return false;

	return true;
}





/*-------------------------------------------------------------------
//  GRPFILEBITMAPTGA::CreateColor
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			06/11/2013 13:10:46
//	
//	@return 			bool : 
//	@param				mode : 
//  @param				databuffer : 
//  @param				color : 
*/
/*-----------------------------------------------------------------*/
bool GRPFILEBITMAPTGA::CreateColor(GRPPROPERTYMODE mode, XBYTE* databuffer, GRPBUFFERCOLORRGBA8& color)
{
	if(!databuffer) return false;

	switch(mode)
		{										
			case GRPPROPERTYMODE_16_RGBA_5551	:	color.r =  (databuffer[1] & 0x7c) << 1;
																					color.g = ((databuffer[1] & 0x03) << 6) | ((databuffer[0] & 0xe0) >> 2);
																					color.b =  (databuffer[0] & 0x1f) << 3;
																					color.a =  (databuffer[1] & 0x80)?0:255;																																																	
																					break;
																						
			case GRPPROPERTYMODE_24_RGB_888		: color.r	= databuffer[2];
																					color.g = databuffer[1];
																					color.b	= databuffer[0];
																					color.a = 255;
																					break;

			case GRPPROPERTYMODE_32_RGBA_8888	: color.r	= databuffer[2];
																					color.g = databuffer[1];
																					color.b	= databuffer[0];
																					color.a = databuffer[3];
																					break;
			default: break;
		}

	return true;
}


