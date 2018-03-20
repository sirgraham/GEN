/*------------------------------------------------------------------------------------------
//	COMPRESS_LZW.CPP
//	
//	Compress LZW (Gif compression) class
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 15/12/2009 10:41:24 p.m.
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "XBuffer.h"

#include "CompressLZW.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  COMPRESS_LZW::COMPRESS_LZW
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 10:41:34 p.m.
//	
//	@return				
//	*/
/*-----------------------------------------------------------------*/
COMPRESS_LZW::COMPRESS_LZW() 
{ 
	SetType(COMPRESSBASE_TYPE_LZW);

	m_Interval[0] = m_Interval[1] = 8; 
	m_Interval[2] = 4; 
	m_Interval[3] = 2; 
} 



/*-------------------------------------------------------------------
//  COMPRESS_LZW::~COMPRESS_LZW
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 10:41:50 p.m.
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
COMPRESS_LZW::~COMPRESS_LZW() 
{ 
	
} 


//-------------------------------------------------------------------
//  COMPRESS_LZW::Compress
/**
//
//
//	@author				Abraham J. Velez
//	@version			10/02/2004 18:05:17
//
//	@return				bool :
//	@param				source :
//  @param				size :
//  @param				xbuffer :
*/
//-------------------------------------------------------------------
bool COMPRESS_LZW::Compress(XBYTE* source,int size,XBUFFER* xbuffer)
{
	if(!source)	 return false;
	if(!size)		 return false;
	if(!xbuffer) return false;

	xbuffer->Delete();
	xbuffer->Resize(size+LZW_MAX_OVERCMP);

	XDWORD csize = LZW_Encode(source,size,xbuffer->Get()); 

	xbuffer->Resize(csize);

	return true;
}


//-------------------------------------------------------------------
//  COMPRESS_LZW::Decompress
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			10/02/2004 18:05:21
//	
//	@return				bool :
//	@param				source :
//  @param				size :
//  @param				xbuffer :
*/
//-------------------------------------------------------------------
bool COMPRESS_LZW::Decompress(XBYTE* source,int size,XBUFFER* xbuffer)
{
	if(!source)	 return false;	
	if(!xbuffer) return false;
	if(!size)		 return false;
		
	LZW_Decode(source,xbuffer->Get());

	return true;
}



/*-------------------------------------------------------------------
//  COMPRESS_LZW::LZW_Encode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 10:43:00 p.m.
//	
//	@return				XDWORD : 
//	@param				InBuffer : 
//  @param				dwLength : 
//  @param				OutBuffer : 
*/
/*-----------------------------------------------------------------*/
XDWORD COMPRESS_LZW::LZW_Encode(XBYTE* InBuffer,XDWORD dwLength,XBYTE* OutBuffer) 
{ 
	*OutBuffer++ = 8 ; 
	
	return 1 + this->LZW_GIF_Encode(InBuffer, OutBuffer, dwLength, 1, 8); 
} 


/*-------------------------------------------------------------------
//  COMPRESS_LZW::LZW_Decode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 10:44:14 p.m.
//	
//	@return				void : 
//	@param				InBuffer : 
//  @param				OutBuffer : 
*/
/*-----------------------------------------------------------------*/
void COMPRESS_LZW::LZW_Decode(XBYTE* InBuffer,XBYTE* OutBuffer) 
{ 
	this->LZW_GIF_Decode (InBuffer, OutBuffer, 0, 0, false); 
} 


/*-------------------------------------------------------------------
//  COMPRESS_LZW::LZW_GIF_Encode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 10:47:56 p.m.
//	
//	@return				XDWORD : 
//	@param				DibBuffer : 
//  @param				OutBuffer : 
//  @param				dwDibWidth : 
//  @param				dwDibHeight : 
//  @param				wColorBit : 
*/
/*-----------------------------------------------------------------*/
XDWORD COMPRESS_LZW::LZW_GIF_Encode(XBYTE* DibBuffer,XBYTE* OutBuffer,XDWORD dwDibWidth,XDWORD dwDibHeight,XWORD wColorBit)   
{       
  m_pHash = new XWORD [LZW_MAX_HASHSIZE] ;   

	if((m_pHash == NULL) || (DibBuffer == NULL) || (OutBuffer == NULL)) return 0;   
    
  m_byMinCode			= (wColorBit == 1) ? 2 : wColorBit; 
  m_LZW_CLEAR			= 1 << m_byMinCode;   
  m_LZW_END				= m_LZW_CLEAR + 1;   
  m_pOrigin				= m_pCurIn = DibBuffer;   
  m_pCurOut				= OutBuffer;   
  m_byInBit				= 8;   
  m_byOutBit			= 0; 
   
  m_dwDibWidth		= dwDibWidth;   
  m_dwDibHeight		= dwDibHeight; 
  m_dwCurrPixel		= 0;   
  m_dwCurrHeight	= 0; 
  m_dwPitch				= 4 * ((dwDibWidth * m_byMinCode + 31) / 32);   
    
  XWORD Old;   
  XBYTE Pixel; 
 
	this->Encode_InitStringTable();   
  this->Encode_WriteIndex(m_LZW_CLEAR); 
  
	Old = this->Encode_GetNextPixel();   
    
  while(m_dwCurrHeight<m_dwDibHeight)   
    {   
      Pixel = this->Encode_GetNextPixel();   

      if(this->Encode_IsInTable (Old, Pixel))   
			  {   
				  Old = m_pHash[(Old << 8) | Pixel]; 
				}
       else   
			  {               
          this->Encode_WriteIndex(Old);   
          this->Encode_AddStringToTable(Old, Pixel);   

          Old = Pixel ;   
          if(m_CurrTableIndex == LZW_MAX_TABLE_SIZE) 
					  {   
              this->Encode_WriteIndex(Pixel);   
              this->Encode_WriteIndex(m_LZW_CLEAR);   
              this->Encode_InitStringTable();   

              Old = this->Encode_GetNextPixel();   
            }   
        }   
    }   
  
	this->Encode_WriteIndex(Old);   
  this->Encode_WriteIndex(m_LZW_END);   

  delete [] m_pHash;   
  
	return (XDWORD)(m_pCurOut - OutBuffer + 1);   
}   


/*-------------------------------------------------------------------
//  COMPRESS_LZW::LZW_GIF_Decode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 10:52:05 p.m.
//	
//	@return				void : 
//	@param				InBuffer : 
//  @param				DibBuffer : 
//  @param				dwDibWidth : 
//  @param				dwDibHeight : 
//  @param				bInterlace : 
*/
/*-----------------------------------------------------------------*/
void COMPRESS_LZW::LZW_GIF_Decode(XBYTE* InBuffer,XBYTE* DibBuffer,XDWORD dwDibWidth,XDWORD dwDibHeight,bool bInterlace)
{   
  m_pStrBegin = new LZW_STRING [LZW_MAX_TABLE_SIZE + 32];   
 
	if((m_pStrBegin == NULL) || (InBuffer == NULL) || (DibBuffer == NULL)) return;   

  m_pCurIn			= InBuffer + 1;   
  m_pCurOut			= DibBuffer;   
  m_byInBit			= 0; 
  m_byMinCode		= *InBuffer; 
  m_LZW_CLEAR		= 1 << m_byMinCode; 
  m_LZW_END			= m_LZW_CLEAR + 1; 
   
  m_dwDibWidth	= dwDibWidth; 
  m_dwCurrPixel = 0;   
  m_dwPitch			= 4 * ((dwDibWidth * 8 + 31) / 32);   
   
  m_byInterval	= bInterlace ? 0 : 0xFF; 
  
	if(m_byInterval == 0)
    {   
      m_pOrigin			 = m_pCurOut;   
      m_dwDibHeight	 = dwDibHeight;
      m_dwCurrHeight = 0; 
    }   
   
  XWORD Code;
	XWORD Old; 

  this->Decode_InitStringTable();   
  
	while((Code = this->Decode_GetNextCode ()) != m_LZW_END)   
    {   
      if(Code == m_LZW_CLEAR)   
        {   
          this->Decode_InitStringTable () ;   
          while((Code = this->Decode_GetNextCode ()) == m_LZW_CLEAR) ;   
        }   
       else   
				{ 
					if(this->Decode_IsInTable (Code))   
						     this->Decode_AddStringToTable (Old, this->Decode_GetFirstChar (Code));   
            else this->Decode_AddStringToTable (Old, this->Decode_GetFirstChar (Old));   
				}
   
      this->Decode_WriteString_to8(Code);   
      Old = Code;   
    }   
  
	delete [] m_pStrBegin;   
}   




/*-------------------------------------------------------------------
//  COMPRESS_LZW::Decode_InitStringTable
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 10:56:25 p.m.
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
void COMPRESS_LZW::Decode_InitStringTable() 
{ 
	memset(m_pStrBegin, 0xFF, LZW_MAX_TABLE_SIZE * sizeof(LZW_STRING)); 

	for (XWORD i = 0 ; i < m_LZW_CLEAR ; i++) 
		m_pStrBegin[i].wSuffix = i ; 

	m_CurrTableIndex = m_LZW_END + 1; 
	m_byCurrBits		 = m_byMinCode + 1; 
} 



/*-------------------------------------------------------------------
//  COMPRESS_LZW::Decode_GetNextCode
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 10:58:12 p.m.
//	
//	@return				XWORD : 
//	*/
/*-----------------------------------------------------------------*/
XWORD COMPRESS_LZW::Decode_GetNextCode()
{ 
	register XDWORD        dwRet = 0; 
	register unsigned int	uiAdd = m_byInBit + m_byCurrBits; 
 
	if(uiAdd <= 8) 
		{
			dwRet |= * m_pCurIn; 
		}
	 else 
		{
			if(uiAdd <= 16) 
				{
					dwRet |= * (XWORD *) m_pCurIn; 
				}
			 else 
				{ 
					dwRet |= * (m_pCurIn + 2); 
					dwRet <<= 16; 
					dwRet |= * (XWORD *) m_pCurIn; 
				}
		}

	m_pCurIn += uiAdd/8; 
	m_byInBit = uiAdd%8; 

	dwRet <<= 32 - uiAdd ; 
	dwRet >>= 32 - m_byCurrBits ; 

	return (XWORD)dwRet ; 
} 


/*-------------------------------------------------------------------
//  COMPRESS_LZW::Decode_IsInTable
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 10:59:57 p.m.
//	
//	@return				bool : 
//	@param				Code : 
*/
/*-----------------------------------------------------------------*/
bool COMPRESS_LZW::Decode_IsInTable(XWORD Code) 
{ 
	return (Code < m_CurrTableIndex); 
} 


/*-------------------------------------------------------------------
//  COMPRESS_LZW::Decode_AddStringToTable
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 11:04:12 p.m.
//	
//	@return				void : 
//	@param				wPrefix : 
//  @param				wSuffix : 
*/
/*-----------------------------------------------------------------*/
void COMPRESS_LZW::Decode_AddStringToTable(XWORD wPrefix, XWORD wSuffix) 
{ 
	m_pStrBegin[m_CurrTableIndex  ].wPrefix = wPrefix ; 
	m_pStrBegin[m_CurrTableIndex++].wSuffix = wSuffix ; 

	if((m_CurrTableIndex == 0x008) || (m_CurrTableIndex == 0x010) || 
		 (m_CurrTableIndex == 0x020) || (m_CurrTableIndex == 0x040) || 
		 (m_CurrTableIndex == 0x080) || (m_CurrTableIndex == 0x100) || 
		 (m_CurrTableIndex == 0x200) || (m_CurrTableIndex == 0x400) || 
		 (m_CurrTableIndex == 0x800)) m_byCurrBits++ ; 
} 



/*-------------------------------------------------------------------
//  COMPRESS_LZW::Decode_GetFirstChar
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 11:04:37 p.m.
//	
//	@return				XBYTE : 
//	@param				Code : 
*/
/*-----------------------------------------------------------------*/
XBYTE COMPRESS_LZW::Decode_GetFirstChar(XWORD Code) 
{ 
	while (m_pStrBegin[Code].wPrefix != 0xFFFF) 
		{
			Code = m_pStrBegin[Code].wPrefix; 
		}

	return (XBYTE) m_pStrBegin[Code].wSuffix; 
} 



/*-------------------------------------------------------------------
//  COMPRESS_LZW::Decode_WriteString_to8
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 11:04:58 p.m.
//	
//	@return				void : 
//	@param				Code : 
*/
/*-----------------------------------------------------------------*/
void COMPRESS_LZW::Decode_WriteString_to8(XWORD Code)   
{   
  if(Code < m_LZW_CLEAR)   
    {   
      m_pCurOut[m_dwCurrPixel++] = (XBYTE) m_pStrBegin[Code].wSuffix;   
      
			if(m_dwCurrPixel == m_dwDibWidth) this->Decode_SwitchToFollowLine();   
    }   
   else   
    {  
			this->Decode_WriteString_to8(m_pStrBegin[Code].wPrefix);   
			this->Decode_WriteString_to8(m_pStrBegin[Code].wSuffix);   			
    }   
}   



/*-------------------------------------------------------------------
//  COMPRESS_LZW::Decode_SwitchToFollowLine
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 11:05:46 p.m.
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
void COMPRESS_LZW::Decode_SwitchToFollowLine() 
{ 
	m_dwCurrPixel = 0;

	if (m_byInterval == 0xFF) 
		{
			m_pCurOut -= m_dwPitch; 
		}
	 else 
		{ 
			m_dwCurrHeight += m_Interval[m_byInterval]; 
			
			if(m_dwCurrHeight >= m_dwDibHeight) 
				{
					if(++m_byInterval < 4)  m_dwCurrHeight = m_Interval[m_byInterval]/2;  else return; 
				}

			m_pCurOut = m_pOrigin - m_dwCurrHeight * m_dwPitch; 
		} 
} 


/*-------------------------------------------------------------------
//  COMPRESS_LZW::Encode_InitStringTable
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 11:06:07 p.m.
//	
//	@return				void : 
//	*/
/*-----------------------------------------------------------------*/
void COMPRESS_LZW::Encode_InitStringTable() 
{ 
	m_CurrTableIndex = m_LZW_END   + 1; 
	m_byCurrBits		 = m_byMinCode + 1; 

	memset(m_pHash, 0x00, LZW_MAX_HASHSIZE * sizeof(XWORD)); 
}



/*-------------------------------------------------------------------
//  COMPRESS_LZW::Encode_GetNextPixel
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 11:06:31 p.m.
//	
//	@return				XBYTE : 
//	*/
/*-----------------------------------------------------------------*/
XBYTE COMPRESS_LZW::Encode_GetNextPixel() 
{ 
	register XBYTE byRet = 0; 
	
	switch(m_byMinCode) 
		{ 
			case 8 : byRet = m_pCurIn[m_dwCurrPixel]; 
							 break; 

			case 4 : byRet = (m_dwCurrPixel % 2 == 0) ? m_pCurIn[m_dwCurrPixel / 2] >> 4 : m_pCurIn[m_dwCurrPixel / 2] & 0x0F; 
							 break; 

			case 1 : byRet = 0x01 & (m_pCurIn[m_dwCurrPixel / 8] >> (7 - (m_dwCurrPixel & 7))); 
							 break; 
		} 

	if(++m_dwCurrPixel == m_dwDibWidth) 
		{ 
			m_dwCurrPixel = 0; 

			m_dwCurrHeight++; 

			m_pCurIn = m_pOrigin - m_dwCurrHeight * m_dwPitch; 
		} 

	return byRet; 
} 


/*-------------------------------------------------------------------
//  COMPRESS_LZW::Encode_IsInTable
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 11:08:02 p.m.
//	
//	@return				bool : 
//	@param				Old : 
//  @param				Pixel : 
*/
/*-----------------------------------------------------------------*/
bool COMPRESS_LZW::Encode_IsInTable(XWORD Old, XWORD Pixel) 
{ 
	return (m_pHash[(Old << 8) | Pixel] != 0); 
} 



/*-------------------------------------------------------------------
//  COMPRESS_LZW::Encode_AddStringToTable
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 11:08:16 p.m.
//	
//	@return				void : 
//	@param				Old : 
//  @param				Pixel : 
*/
/*-----------------------------------------------------------------*/
void COMPRESS_LZW::Encode_AddStringToTable(XWORD Old, XWORD Pixel) 
{ 
	m_pHash[(Old << 8) | Pixel] = m_CurrTableIndex++; 

	if ((m_CurrTableIndex == 0x009) || (m_CurrTableIndex == 0x011) || 
			(m_CurrTableIndex == 0x021) || (m_CurrTableIndex == 0x041) || 
			(m_CurrTableIndex == 0x081) || (m_CurrTableIndex == 0x101) || 
			(m_CurrTableIndex == 0x201) || (m_CurrTableIndex == 0x401) || 
			(m_CurrTableIndex == 0x801))  m_byCurrBits++; 
} 


/*-------------------------------------------------------------------
//  COMPRESS_LZW::Encode_WriteIndex
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			15/12/2009 11:08:45 p.m.
//	
//	@return				void : 
//	@param				Index : 
*/
/*-----------------------------------------------------------------*/
void COMPRESS_LZW::Encode_WriteIndex(XDWORD Index) 
{ 
	register unsigned int	uiAdd = m_byOutBit + m_byCurrBits; 
 
	*(XDWORD*)m_pCurOut |= (Index << m_byOutBit); 

	m_byOutBit = uiAdd % 8; 
	m_pCurOut += uiAdd / 8; 
} 









