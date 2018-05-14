/*------------------------------------------------------------------------------------------
//  SNDOPENALELEMENT.H
*/
/**
// \file
//
//  openal implementation of the element
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 05/11/2015 10:31:09
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _SNDOPENALELEMENT_H_
#define _SNDOPENALELEMENT_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"

#include "SNDElement.h"
#include "SNDFileOGG.h"
#include "SNDOpenALBuffer.h"
#include "SNDOpenALSource.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class XPATH;
class SNDFILEOGG;
class SNDOPENALBUFFER;
class SNDOPENALSOURCE;
class XFACTORY;

class SNDOPENALELEMENT : public SNDELEMENT
{
  friend class SNDOPENAL;

  public:

                                  SNDOPENALELEMENT                      (XPATH* xpath)
                                  {
                                    Clean();


                                    buffer  = new SNDOPENALBUFFER();
                                    buffer->Generate();
                                    volume  = 1.0f;
                                    pitch   = 1.0f;
                                    sources.SetIsMulti(false);
                                  }

    virtual                      ~SNDOPENALELEMENT                      ()
                                  {
                                    if(buffer)
                                      {
                                        buffer->Destroy();
                                        delete buffer;
                                      }
                                    Clean();
                                  }

    void                          AddSource                             (SNDOPENALSOURCE* source)
                                  {
                                    sources.Add(source);
                                  }

    void                          SetSource                             (SNDOPENALSOURCE* source)
                                  {
                                    if(source != NULL)
                                      {
                                        sources.Add(source);
                                        lastsource = source;
                                      }
                                    else
                                      {
                                        XDWORD size = sources.GetSize();
                                        for(XDWORD i = 0; i < size; i++)
                                          {
                                            if(sources.FastGet(i) == source)
                                              {
                                                sources.DeleteIndex(i);
                                                return;
                                              }
                                          }
                                      }
                                  }

    SNDOPENALSOURCE*              GetSource                             ()
                                  {
                                    SNDOPENALSOURCE* ret = NULL;

                                    XDWORD size = sources.GetSize();
                                    for(XDWORD i = 0; i < size; i++)
                                      {
                                        if(sources.FastGet(i)->GetElement() == this)
                                          {
                                            return sources.FastGet(i);
                                          }
                                      }

                                    return ret;
                                  }

    void                          SetLoop                               (bool loop);
    bool                          GetLoop                               ()                              { return this->loop;      }


    SNDOPENALBUFFER*              GetBuffer                             ()                              { return this->buffer;    }

    void                          SetVolume                             (float volume);
    float                         GetVolume                             ()                              { return volume;          }

    void                          SetPitch                              (float pitch);
    float                         GetPitch                              ()                              { return pitch;           }
    virtual void                  SetFile                               (SNDFILE* file);

  protected:


    SNDOPENALBUFFER*              buffer;

    XVECTOR<SNDOPENALSOURCE*>     sources;
    SNDOPENALSOURCE*              lastsource;

    XPATH*                        xpath;
    XSTRING                       name;

  private:

    void                          Queue                                 ();

    void                          Clean                                 ()
                                  {
                                    buffer    = NULL;
                                    loop      = false;
                                    volume    = 0.0f;
                                    pitch     = 0.0f;
                                  }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

