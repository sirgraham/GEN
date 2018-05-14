/*------------------------------------------------------------------------------------------
//  UI3DOBJECTLOADER.H
*/
/**
// \file
//
//  3D Object Loader for UI
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 13/02/2017 17:57:13
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UI3DOBJECTLOADER_H_
#define _UI3DOBJECTLOADER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XString.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPOBJECT;

class UI3DOBJECTLOADER
{
public:

                                        UI3DOBJECTLOADER                                    ()
                                        {
                                          Clean();
                                        }

  virtual                              ~UI3DOBJECTLOADER                                    ()
                                        {
                                          Clean();
                                        }

  GRPOBJECT*                            Load                                                (XCHAR* path);

protected:

private:
  void Clean()
  {
  }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

