/*------------------------------------------------------------------------------------------
//  UIWIDGETCREATORHELPER.H
*/
/**
// \file
//
//  class with helper functions to create complex widgets
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 22/06/2016 11:03:18
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETCREATORHELPER_H_
#define _UIWIDGETCREATORHELPER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XBase.h"
#include "XString.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

class UIMANAGER;
class UIWIDGET;

class UIWIDGETCREATORHELPER
{
public:
                        UIWIDGETCREATORHELPER                           (UIMANAGER* uimanager)                            { this->uimanager = uimanager;  }
  virtual              ~UIWIDGETCREATORHELPER                           ()                                                {                               }

  UIWIDGET*                   CreateButton                              (XCHAR* text, XCHAR* background, XCHAR* hover, XCHAR* click);
  UIWIDGET*                   CreateMessageDialog                       (XCHAR* text, XCHAR* backimage, XCHAR* background, XCHAR* hover, XCHAR* click);
  UIWIDGET*                   CreateToast                               (XCHAR* text, XCHAR* image);
  UIWIDGET*                   CreateScrollBar                           (XCHAR* name, XCHAR* upnormal, XCHAR* uphover, XCHAR* uppressed, XCHAR* downnormal, XCHAR* downhover, XCHAR* downpressed, XCHAR* background, XCHAR* movenormal, XCHAR* movehover, XCHAR* movepressed);
  UIWIDGET*                   CreateScrollBar                           (XCHAR* name, UIWIDGET* upbutton, UIWIDGET* downbutton, UIWIDGET* background, UIWIDGET* movebutton);

protected:

  UIMANAGER* uimanager;
private:
  void                  Clean                                           ()
                        {
                          uimanager = NULL;
                        }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

