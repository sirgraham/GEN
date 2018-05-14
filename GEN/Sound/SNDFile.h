/*------------------------------------------------------------------------------------------
//  SNDFILE.H
*/
/**
// \file
//
//  serve as a base class for all sound files
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 05/11/2015 9:13:40
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _SNDFILE_H_
#define _SNDFILE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"
#include "XFile.h"
#include "XPath.h"
#include "XString.h"

#include "MEDIAFile.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class SNDFILE : public MEDIAFILE
{
  public:

                                    SNDFILE                               ()
                                    {
                                      Clean();

                                    }

    virtual                        ~SNDFILE                               ()
                                    {
                                      Clean();
                                      delete xbuffer;
                                      delete xbufferdecodeddata;
                                    }

    virtual bool                    LoadFile                              (XCHAR* path, XCHAR* name, bool streaming)      { return false;               }
    virtual bool                    LoadFile                              (XPATH& xpath, XCHAR* name, bool streaming)     { return false;               }

    bool                            WriteRaw                              (XCHAR* path, XCHAR* name);
    bool                            WriteRaw                              (XPATH& xpath, XCHAR* name);

    XBUFFER*                        GetData                               ()                                              { return xbufferdecodeddata;  }
    XWORD                           GetChannels                           ()                                              { return channels;            }
    XDWORD                          GetSampleRate                         ()                                              { return samplerate;          }
    XDWORD                          GetSamplesNumber                      ()                                              { return samples;             }

    XSTRING*                        GetName                               ()                                              { return &name;               }
    XDWORD                          samples;

    virtual bool                    Reset                                 ()                                              { return true;                }

    float                           GetDuration                           ()                                              { return duration;            }

  protected:

    XBUFFER*                        xbuffer;
    XBUFFER*                        xbufferdecodeddata;

    float                           duration;

    XWORD                           channels;
    XDWORD                          samplerate;

    bool                            streaming;
    XSTRING                         name;

  private:

    void                            Clean                                 ()
                                    {
                                      xbuffer             = 0;
                                      xbufferdecodeddata  = 0;
                                      channels            = 0;
                                      samplerate          = 0;
                                      samples             = 0;
                                    }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

