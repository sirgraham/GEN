/*------------------------------------------------------------------------------------------
//  SNDSTREAMELEMENT.H
*/
/**
// \file
//
//  sound streaming
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 20/07/2016 10:04:05
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _SNDSTREAMELEMENT_H_
#define _SNDSTREAMELEMENT_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "SNDElement.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class SNDFILE;

class SNDSTREAMELEMENT : public SNDELEMENT
{
public:

                            SNDSTREAMELEMENT                          () : SNDELEMENT()                             { Clean(); streamer = true;                       }
  virtual                  ~SNDSTREAMELEMENT                          ()                                            { Clean();                                        }

  void                      SetAttributes                             (XWORD channels, XWORD freq)                  { this->channels = channels; this->freq = freq;   }

  virtual void              Play                                      ()                                            {                                                 }
  virtual void              AddData                                   (XDWORD size, void* data) = 0;

  void                      SetFile                                   (SNDFILE* file)                               { this->file = file;                              }
  SNDFILE*                  GetFile                                   ()                                            { return file;                                    }

protected:

  XWORD                     channels;
  XWORD                     freq;
  //SNDFILE*                  file;

private:
  void                      Clean                                     ()
                            {
                              channels  = 0;
                              freq      = 0;
                              file      = NULL;
                            }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

