/*------------------------------------------------------------------------------------------
//  VIDEOCODECH264.H
*/
/**
// \file
//
//  H264 VideoCodec
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 11/04/2016 12:32:22
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _VIDEOCODECH264_H_
#define _VIDEOCODECH264_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "VIDEOCodec.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class MEDIAH264FRAMEREADER;
class MEDIAFRAMEREADERLIBAV;
class XBUFFERBITS;

class VIDEOCODECH264 : public VIDEOCODEC
{
public:

                                    VIDEOCODECH264(VIDEOPLAYER* videoplayer);
  virtual                          ~VIDEOCODECH264();

  virtual bool                      Ini                                             ()                                                          { return false;                           }
  virtual bool                      End                                             ()                                                          { return false;                           }

  virtual bool                      SetFile                                         (XCHAR* filename);

  virtual MEDIAFRAMEREADER*         GetFrameReader                                  ();

  virtual void                      Decode                                          ()                                                          {                                         }
  virtual void                      Encode                                          ()                                                          {                                         }

  virtual void                      Update                                          ()                                                          {                                         }

protected:
  virtual bool                      FillInfo                                        (XVECTOR<XBYTE> & frame);
  virtual bool                      SendFrame                                       (XVECTOR<XBYTE> & frame)                                    {return true;                             }

  void                              ReadVUI                                         (XBUFFERBITS& bits);

  //MEDIAFRAMEREADER*                 framereader;
  XVECTOR<XBYTE>                    frame;

  XVECTOR<XBYTE>                    SPS;
  XVECTOR<XBYTE>                    PPS;


private:
  void                              Clean                                           ()
                                    {
                                      //framereader = NULL;
                                    }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

