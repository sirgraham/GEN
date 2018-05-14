//------------------------------------------------------------------------------------------
//  XBUFFER.H
//
/**
// \class
//
//  eXtended Buffer
//
//  @author  Abraham J. Velez
//  @version 17/11/2001
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XBUFFER_H_
#define _XBUFFER_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XString.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum XBUFFER_PADDINGTYPE
{
  XBUFFER_PADDINGTYPE_NONE              = 0 ,
  XBUFFER_PADDINGTYPE_ZEROS                 ,
  XBUFFER_PADDINGTYPE_SPACES                ,
  XBUFFER_PADDINGTYPE_ANSIX923              ,
  XBUFFER_PADDINGTYPE_ISO10126              ,
  XBUFFER_PADDINGTYPE_PKCS7                 ,
  XBUFFER_PADDINGTYPE_PKCS5                 ,
  XBUFFER_PADDINGTYPE_ISOIEC7816_4          ,
};


#define XBUFFER_INVALIDPOSITION         -1
#define XBUFFER_ISTHREADSAFE            true


//---- CLASS -------------------------------------------------------------------------------


class XFACTORY;
class XMUTEX;

class XBUFFER
{
  public:
                             XBUFFER                        (bool threadsafe = XBUFFER_ISTHREADSAFE);
                             XBUFFER                        (XDWORD size, bool threadsafe = XBUFFER_ISTHREADSAFE);
    virtual                 ~XBUFFER                        ();

    XDWORD                   GetSize                        ();
    bool                     SetSize                        (XDWORD size);

    bool                     IsEmpty                        ()                                 { return (!GetSize())?true:false;                                }

    void                     ResetPosition                  ();
    XDWORD                   GetPosition                    ();
    bool                     SetPosition                    (XDWORD position = 0);

    bool                     IsBlocked                      ();
    bool                     SetBlocked                     (bool blocked);

    bool                     FillBuffer                     (XBYTE fillchar = 0);

    bool                     Add                            (XBYTE* pbuffer, XDWORD psize);
    bool                     Add                            (XBUFFER* buffer);
    bool                     Add                            (XBUFFER& buffer);
    bool                     Add                            (XBYTE data);
    bool                     Add                            (bool data);
    bool                     Add                            (XWORD data);
    bool                     Add                            (XDWORD data);
    bool                     Add                            (XQWORD data);
    bool                     Add                            (float data);
    bool                     Add                            (double data);
    bool                     Add                            (XSTRING& string);
    bool                     AddWithMask                    (XCHAR* mask, ...);
    bool                     AddWithMask                    (XSTRING* mask, ...);
    bool                     AddNBits                       (XWORD data);

    bool                     Insert                         (XBYTE* pbuffer, XDWORD psize, int frompos = 0);
    bool                     Insert                         (XBUFFER* buffer, int frompos = 0);
    bool                     Insert                         (XBUFFER& buffer, int frompos = 0);
    bool                     Insert                         (XBYTE data, int frompos = 0);
    bool                     Insert                         (bool  data, int frompos = 0);
    bool                     Insert                         (XWORD data, int frompos = 0);
    bool                     Insert                         (XDWORD data, int frompos = 0);
    bool                     Insert                         (XQWORD data, int frompos = 0);
    bool                     Insert                         (float data, int frompos = 0);
    bool                     Insert                         (double data, int frompos = 0);
    bool                     Insert                         (XSTRING& string, int frompos = 0);
    bool                     InsertWithMask                 (XCHAR* mask, int frompos, ...);
    bool                     InsertWithMask                 (XSTRING* mask, int frompos, ...);

    bool                     Padding_Add                    (XBYTE bitsadjust, XBUFFER_PADDINGTYPE type = XBUFFER_PADDINGTYPE_PKCS5);
    bool                     Padding_Has                    ();
    XBUFFER_PADDINGTYPE      Padding_GetType                ();
    XBYTE                    Padding_GetSize                ();
    bool                     Padding_Delete                 ();

    XDWORD                   Extract                        (XBYTE* pbuffer,XDWORD ppos, XDWORD psize);
    bool                     Extract                        (XBYTE& data, XDWORD ppos = 0);
    bool                     Extract                        (bool& data, XDWORD ppos = 0);
    bool                     Extract                        (XWORD& data, XDWORD ppos = 0);
    bool                     Extract                        (XDWORD& data, XDWORD ppos = 0);
    bool                     Extract                        (XQWORD& data, XDWORD ppos = 0);
    bool                     Extract                        (float& data, XDWORD ppos = 0);
    bool                     Extract                        (double& data, XDWORD ppos = 0);
    bool                     Extract                        (XSTRING& string, XDWORD ppos, XDWORD psize);
    bool                     ExtractWithMask                (XCHAR* mask, int frompos, ...);
    bool                     ExtractWithMask                (XSTRING* mask, int frompos, ...);

    bool                     Compare                        (XBYTE* pbuffer, XDWORD psize);
    bool                     Compare                        (XBUFFER* buffer);

    XBYTE*                   Get                            ();
    XBYTE                    GetByte                        (XDWORD index);
    bool                     Get                            (XBYTE* pbuffer,int psize, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                            (XBYTE& data, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                            (bool& data,  int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                            (XWORD& data, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                            (XDWORD& data, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                            (XQWORD& data, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                            (double& data, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                            (float& data, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     Get                            (XSTRING& data, int psize, int frompos = XBUFFER_INVALIDPOSITION);
    bool                     GetWithMask                    (XSTRING* mask, int frompos, ...);

    bool                     Set                            (XBYTE* pbuffer,int psize, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                            (XBYTE data, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                            (bool data, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                            (XWORD data, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                            (XDWORD data, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                            (XQWORD data, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                            (float data, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                            (double data, int topos = XBUFFER_INVALIDPOSITION);
    bool                     Set                            (XSTRING& data, int topos = XBUFFER_INVALIDPOSITION);
    XDWORD                   SetWithMask                    (XCHAR* mask, int topos, ...);
    XDWORD                   SetWithMask                    (XSTRING* mask, int topos, ...);
    bool                     SetNBits                       (XWORD data, int topos = XBUFFER_INVALIDPOSITION);

    bool                     Resize                         (XDWORD newsize,bool setblocked = true);

    bool                     Delete                         (bool setblocked = true);

    bool                     Empty                          ()                                 { return Delete();                                               }

    bool                     Swap                           ();

    bool                     AdjustToNBits                  (int nbits);
    XBYTE                    GetNBits                       ()                                 { return nbits;                                                  }
    XBYTE                    GetNBitsFree                   ();
    bool                     SetNBitsFree                   (XBYTE nbitsfree);

    bool                     ConvertFromBase64              (XSTRING& stringbase64);
    bool                     ConvertToBase64                (XSTRING& stringbase64);

    XDWORD                   DecodeBCD                      (XDWORD ppos,XDWORD psize);
    XQWORD                   DecodeBCDLong                  (XDWORD ppos,XDWORD psize);

    static void              SetHardwareUseLittleEndian     (bool hardwareuselittleendian)     { XBUFFER::hardwareuselittleendian = hardwareuselittleendian;    }

  private:

    void                     Clean                          ()
                             {
                                buffer                    = NULL;
                                size                      = 0;
                                sizeassign                = 0;
                                position                  = 0;

                                nbits                     = 0;
                                nbitsfree                 = 0;
                                lastnbitsfree             = 0;

                                paddingtype               = XBUFFER_PADDINGTYPE_NONE;
                                paddinghas                = false;
                                paddingbitadjust          = 0;
                                paddingsize               = 0;

                                xmutex                    = NULL;
                             }


    bool                     AddXBufferWithMask             (XBUFFER& xbuffer, XSTRING* mask, va_list& arg);
    bool                     ExtractXBufferWithMask         (XBUFFER& xbuffer, XSTRING* mask, va_list& arg);

    static bool              hardwareuselittleendian;

    XBYTE*                   buffer;
    XDWORD                   size;
    XDWORD                   sizeassign;
    XDWORD                   position;

    XBYTE                    nbits;
    XBYTE                    nbitsfree;
    XBYTE                    lastnbitsfree;

    bool                     paddinghas;
    XBUFFER_PADDINGTYPE      paddingtype;
    XBYTE                    paddingbitadjust;
    XBYTE                    paddingsize;

    XMUTEX*                  xmutex;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

