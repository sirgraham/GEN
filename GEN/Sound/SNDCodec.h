/*------------------------------------------------------------------------------------------
//  SNDCODEC.H
*/
/**
// \file
//
//  basic sound codec
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 29/11/2016 13:02:04
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _SNDCODEC_H_
#define _SNDCODEC_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "MEDIACodec.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
struct MEDIAPACKET;

class SNDCODEC : public MEDIACODEC
{
public:

                        SNDCODEC                        ()
                        {
                          Clean();
                        }

  virtual              ~SNDCODEC                        ()
                        {
                          Clean();
                        }

  virtual bool          DecodePacket                    (XBUFFER* output, MEDIAPACKET* packet)                      { return true; };
  virtual bool          DecodeAll                       (XBUFFER* output)                                           { return true; };

protected:

private:
  void                  Clean                           ()
                        {

                        }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

