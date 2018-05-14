/*------------------------------------------------------------------------------------------
//  UIFILEBROWSER.H
*/
/**
// \file
//
//  UI File Browser to select files
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 27/01/2017 12:33:37
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIFILEBROWSER_H_
#define _UIFILEBROWSER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class UIMANAGER;
class XSTRING;

class UIFILEBROWSER
{
public:

                                        UIFILEBROWSER                       (UIMANAGER* uimanager);
  virtual                              ~UIFILEBROWSER                       ();

  bool                                  SelectFile                          (XSTRING* str);

protected:
  UIMANAGER*                            uimanager;
private:
  void Clean()
  {
    uimanager = NULL;
  }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

