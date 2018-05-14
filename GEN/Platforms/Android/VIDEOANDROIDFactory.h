/*------------------------------------------------------------------------------------------
//  VIDEOANDROIDFACTORY.H
*/
/**
// \file
//
//  android video codecs factory
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 14/04/2016 16:42:46
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _VIDEOANDROIDFACTORY_H_
#define _VIDEOANDROIDFACTORY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "VIDEOFactory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class VIDEOANDROIDFACTORY : public VIDEOFACTORY
{
public:

                                    VIDEOANDROIDFACTORY(VIDEOPLAYER* videoplayer);
  virtual                          ~VIDEOANDROIDFACTORY();

  virtual VIDEOCODEC*               CreateH264Codec                                 ();

protected:

private:

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

