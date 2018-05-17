/*------------------------------------------------------------------------------------------
//  COMPRESSLZW.H
*/
/**
// \class
//
//  Compres LWZ (Gif compression) class
//
//  @author  Abraham J. Velez
//  @version 15/12/2009 10:29:20 p.m.
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _COMPRESSLZW_H_
#define _COMPRESSLZW_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "CompressBase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define   LZW_MAX_TABLE_SIZE    4096
#define   LZW_MAX_HASHSIZE      0x1000FF // (4096 << 8) + 0xFF

#define   LZW_MAX_OVERCMP       1024       // Añadido por si la compresion es deficiente.

typedef struct tagLZW_STRING
{
  XWORD wPrefix;
  XWORD wSuffix;

} LZW_STRING, *PLZW_STRING;


/*---- CLASS -----------------------------------------------------------------------------*/

class COMPRESS_LZW : public COMPRESSBASE
{
  public :
                           COMPRESS_LZW                       ();
    virtual               ~COMPRESS_LZW                       ();

    bool                   Compress                           (XBYTE* source,XDWORD size,XBUFFER* buffer);
    bool                   Decompress                         (XBYTE* source,XDWORD size,XBUFFER* buffer);

    XDWORD                 LZW_Encode                         (XBYTE* InBuffer, XDWORD dwLength, XBYTE* OutBuffer);
    void                   LZW_Decode                         (XBYTE* InBuffer, XBYTE* OutBuffer);

    XDWORD                 LZW_GIF_Encode                     (XBYTE* DibBuffer ,XBYTE* OutBuffer, XDWORD dwDibWidth, XDWORD dwDibHeight, XWORD wColorBit);
    void                   LZW_GIF_Decode                     (XBYTE* InBuffer  ,XBYTE* DibBuffer, XDWORD dwDibWidth, XDWORD dwDibHeight, bool bInterlace);

  private:

    void                   Decode_InitStringTable             ();
    XWORD                  Decode_GetNextCode                 ();
    bool                   Decode_IsInTable                   (XWORD Code);
    void                   Decode_AddStringToTable            (XWORD wPrefix, XWORD wSuffix);
    XBYTE                  Decode_GetFirstChar                (XWORD Code);
    void                   Decode_WriteString_to8             (XWORD Code);
    void                   Decode_SwitchToFollowLine          ();
    void                   Encode_InitStringTable             ();
    XBYTE                  Encode_GetNextPixel                ();
    bool                   Encode_IsInTable                   (XWORD Old, XWORD Pixel);
    void                   Encode_AddStringToTable            (XWORD Old, XWORD Pixel);
    void                   Encode_WriteIndex                  (XDWORD wIndex);

    LZW_STRING*            m_pStrBegin;
    XWORD*                 m_pHash;

    XWORD                  m_LZW_CLEAR;
    XWORD                  m_LZW_END;

    XBYTE                  m_byInterval;
    XBYTE                  m_Interval[7];

    XBYTE*                 m_pOrigin;
    XBYTE*                 m_pCurIn;
    XBYTE*                 m_pCurOut;
    XBYTE                  m_byInBit;
    XBYTE                  m_byOutBit;

    XBYTE                  m_byMinCode;
    XBYTE                  m_byCurrBits;
    XWORD                  m_CurrTableIndex;
    XBYTE                  m_Padding[2];

    XDWORD                 m_dwDibHeight;
    XDWORD                 m_dwDibWidth;
    XDWORD                 m_dwCurrPixel;
    XDWORD                 m_dwCurrHeight;
    XDWORD                 m_dwPitch;
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif
