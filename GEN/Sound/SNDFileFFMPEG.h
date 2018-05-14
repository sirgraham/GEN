/*------------------------------------------------------------------------------------------
//  SNDFILEFFMPEG.H
*/
/**
// \file
//
//  sound decoder with ffmpeg
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 21/11/2016 14:23:21
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _SNDFILEFFMPEG_H_
#define _SNDFILEFFMPEG_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "SNDFile.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class SNDCODECFFMPEG;

class SNDFILEFFMPEG : public SNDFILE
{
public:

                          SNDFILEFFMPEG                   ();
  virtual                ~SNDFILEFFMPEG                   ();

  bool                    LoadFile                        (XCHAR* path, XCHAR* name, bool streaming);
  bool                    LoadFile                        (XPATH& xpath, XCHAR* name, bool streaming);

  //float                   duration;


  bool                    Reset                           ();
protected:

  bool                    streaming;
  XSTRING                 pathtoopen;
  //SNDCODECFFMPEG*         codec;

private:
  void                    Clean                           ()
                          {
                            streaming = false;
                          }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

