/*------------------------------------------------------------------------------------------
//  VIDEOCODEC.H
*/
/**
// \file
//
//  base class for video codecs
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 11/04/2016 12:21:57
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _VIDEOCODEC_H_
#define _VIDEOCODEC_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "VIDEOPlayer.h"
#include "MEDIACodec.h"
#include "XVector.h"
#include "XFile.h"
#include "XFactory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class MEDIAFRAMEREADER;

class VIDEOCODEC : public MEDIACODEC
{
public:

                                    VIDEOCODEC                                      (VIDEOPLAYER* videoplayer)                                  { Clean(); this->videoplayer = videoplayer;   }
  virtual                          ~VIDEOCODEC                                      ()                                                          { Clean();                                    }

  virtual bool                      SetFile                                         (XCHAR* filename)
                                    {
                                      //xfile = xfactory->Create_File();
                                      //if(!xfile) return false;
                                      //
                                      //if(!xfile->Open(filename))
                                      //{
                                      //  xfactory->Delete_File(xfile);
                                      //}
                                      //
                                      return true;
                                    }


  virtual bool                      Ini                                             ()                                                          { return true;                                }
  virtual bool                      End                                             ()                                                          { return true;                                }

  virtual bool                      SetFile                                         (XSTRING* filename)                                         { return this->SetFile(filename->Get());      }

  virtual void                      SetTarget                                       (VIDEOPLAYER_TARGET target)                                 { this->target = target;                      }

  virtual XDWORD                    GetWidth                                        ()                                                          { return width;                               }
  virtual XDWORD                    GetHeight                                       ()                                                          { return height;                              }
  virtual float                     GetFPS                                          ()                                                          { return fps;                                 }

  virtual MEDIAFRAMEREADER*         GetFrameReader                                  () = 0;

  virtual void                      Decode                                          ()                                                          {                                             }
  virtual void                      Encode                                          ()                                                          {                                             }

  virtual bool                      IsPlaying                                       ()                                                          { return this->isplaying;                     }

  virtual void                      Update                                          ()                                                          {                                             }

  bool                              eos;

protected:
  virtual bool                      FillInfo                                        (XVECTOR<XBYTE> & frame)                                    { return true;                                }
  virtual bool                      SendFrame                                       (XVECTOR<XBYTE> & frame)                                    { return true;                                }

  bool                              initialized;
  //bool                              isplaying;

  VIDEOPLAYER_TARGET                target;

  XDWORD                            width;
  XDWORD                            height;
  float                             fps;

  VIDEOPLAYER*                      videoplayer;
private:
  void                              Clean()
                                    {
                                      initialized   = false;
                                      target        = VIDEOPLAYER_TARGET_NONE;
                                      width         = 0;
                                      height        = 0;
                                      fps           = 0.0f;
                                      videoplayer   = NULL;
                                      eos           = false;
                                    }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

