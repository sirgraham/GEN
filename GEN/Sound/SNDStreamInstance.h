/*------------------------------------------------------------------------------------------
//  SNDSTREAMINSTANCE.H
*/
/**
// \file
//
//  stream instance
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 01/12/2016 16:20:03
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _SNDSTREAMINSTANCE_H_
#define _SNDSTREAMINSTANCE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "SNDInstance.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class SNDSTREAMINSTANCE : public SNDINSTANCE
{
public:

                        SNDSTREAMINSTANCE             (SNDFACTORY* sndsystem, SNDSOURCE* source, SNDELEMENT* element);
  virtual              ~SNDSTREAMINSTANCE             ();

  bool                  Update                        (); // function to actually perform decoding and send the buffers to the stream element

protected:
  float                 buffered;
  bool                  hasbuffered;

private:
  void                  Clean                         ()
                        {
                          buffered      = 0.0f;
                          hasbuffered   = false;
                        }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

