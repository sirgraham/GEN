//------------------------------------------------------------------------------------------
//  GRPPROPERTIES.H
//
/**
// \class
//
//  Graphics Propertys Class
//
//  @author  Diego Martinez Ruiz de Gaona
//  @version 28/06/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _GRPPROPERTIES_H_
#define _GRPPROPERTIES_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"

#include "XMemory.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum GRPPROPERTYMODE
{
  GRPPROPERTYMODE_XX_UNKNOWN          = 0 ,
  GRPPROPERTYMODE_08_INDEX              ,
  GRPPROPERTYMODE_16_RGBA_4444          ,
  GRPPROPERTYMODE_16_RGBA_5551          ,
  GRPPROPERTYMODE_16_RGB_565            ,
  GRPPROPERTYMODE_16_SRGB_565           ,
  GRPPROPERTYMODE_24_RGB_888            ,
  GRPPROPERTYMODE_24_BGR_888            ,
  GRPPROPERTYMODE_32_RGBA_8888          ,
  GRPPROPERTYMODE_32_BGRA_8888          ,
};


//---- CLASS -------------------------------------------------------------------------------

class GRPPROPERTIES
{
  public:

                            GRPPROPERTIES               ()                                                { Default();                                }
    virtual                ~GRPPROPERTIES               ()                                                { Clean();                                  }

    GRPPROPERTYMODE         GetMode                     ()                                                { return mode;                              }
    void                    SetMode                     (GRPPROPERTYMODE mode)                            { this->mode = mode;                        }

    void                    SetPosition                 (XDWORD x, XDWORD y)                              { positionx=x;positiony=y;                  }

    XDWORD                  GetWidth                    ()                                                { return width;                             }
    void                    SetWidth                    (XDWORD width)                                    { this->width = width;                      }

    XDWORD                  GetHeight                   ()                                                { return height;                            }
    void                    SetHeight                   (XDWORD height)                                   { this->height = height;                    }

    XDWORD                  GetStride                   ()                                                { return stride;                            }
    void                    SetStride                   (XDWORD stride)                                   { this->stride = stride;                    }

    bool                    IsBufferInverse             ()                                                { return isbufferinverse;                   }
    void                    SetIsBufferInverse          (bool isbufferinverse)                            { this->isbufferinverse = isbufferinverse;  }

    XBYTE                   GetBitsperPixel             ()
                            {
                              XBYTE bitsppixel = 0;

                              switch(mode)
                                {
                                  case GRPPROPERTYMODE_XX_UNKNOWN   : break;
                                  case GRPPROPERTYMODE_08_INDEX     : bitsppixel =  8;  break;

                                  case GRPPROPERTYMODE_16_RGBA_4444 :
                                  case GRPPROPERTYMODE_16_RGB_565   :
                                  case GRPPROPERTYMODE_16_SRGB_565  :
                                  case GRPPROPERTYMODE_16_RGBA_5551 : bitsppixel = 16;  break;

                                  case GRPPROPERTYMODE_24_RGB_888   :
                                  case GRPPROPERTYMODE_24_BGR_888   : bitsppixel = 24;  break;

                                  case GRPPROPERTYMODE_32_RGBA_8888 :
                                  case GRPPROPERTYMODE_32_BGRA_8888 : bitsppixel = 32;  break;

                                                          default   : bitsppixel =  0;  break;
                                }

                              return bitsppixel;
                            }


    XBYTE                   GetBytesperPixel            ()                                                { return (GetBitsperPixel()/8);     }

    void                    CopyPropertysFrom           (GRPPROPERTIES* properties)
                            {
                              if(!properties) return;

                              mode            = properties->mode;
                              width           = properties->width;
                              height          = properties->height;
                              stride          = properties->stride;
                              isbufferinverse = properties->isbufferinverse;
                            }

    void                    CopyPropertysTo             (GRPPROPERTIES* properties)
                            {
                              if(!properties) return;

                              properties->mode              = mode;
                              properties->width           = width;
                              properties->height            = height;
                              properties->stride            = stride;
                              properties->isbufferinverse = isbufferinverse;
                            }


    int                     IsEqualPropertysSizeTo     (GRPPROPERTIES* properties)
                            {
                              if(!properties) return ISLESS;

                              int size[2];

                              size[0] = width * height;
                              size[1] = properties->width * properties->height;

                              if(size[0] > size[1])   return ISGREATER;
                              if(size[0] < size[1])   return ISLESS;

                              return ISEQUAL;
                            }


    void                    Default                     ()
                            {
                              mode            = GRPPROPERTYMODE_24_BGR_888;
                              width           = 640;
                              height          = 480;
                              stride          = 0;
                              isbufferinverse = false;
                            }

  protected:

    GRPPROPERTYMODE         mode;
    XDWORD                  positionx;
    XDWORD                  positiony;
    XDWORD                  width;
    XDWORD                  height;
    XDWORD                  stride;
    bool                    isbufferinverse;

  private:

    void                    Clean                       ()
                            {
                              mode            = GRPPROPERTYMODE_XX_UNKNOWN;
                              positionx       = 0;
                              positiony       = 0;
                              width           = 0;
                              height          = 0;
                              stride          = 0;
                              isbufferinverse = false;
                            }
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

