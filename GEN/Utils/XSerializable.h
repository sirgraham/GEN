/*------------------------------------------------------------------------------------------
//  XSERIALIZABLE.H
*/
/**
// \file
//
//  base class for serializable data types
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 04/02/2016 10:23:38
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _XSERIALIZABLE_H_
#define _XSERIALIZABLE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBuffer.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class XSERIALIZABLE
{
  public:
                                    XSERIALIZABLE                         ()                              {  }
    virtual                        ~XSERIALIZABLE                         ()                              {  }

    virtual bool                    DeSerialize                           (XBUFFER* buffer) const = 0;
    virtual bool                    Serialize                             (XBUFFER* buffer)       = 0;

  protected:

  private:

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

