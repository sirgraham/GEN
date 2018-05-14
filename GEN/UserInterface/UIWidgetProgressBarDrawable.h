/*------------------------------------------------------------------------------------------
//  UIWIDGETPROGRESSBARDRAWABLE.H
*/
/**
// \file
//
//  draws a progress bar
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 02/11/2015 9:35:15
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETPROGRESSBARDRAWABLE_H_
#define _UIWIDGETPROGRESSBARDRAWABLE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetProgressBar.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPOBJECT;
class GRPELEMENT;
class GRPTEXTURE;

class UIWIDGETPROGRESSBARDRAWABLE : public UIWIDGETPROGRESSBAR
{
public:

                                UIWIDGETPROGRESSBARDRAWABLE       (XCHAR* name, UIMANAGER * uimanagerargin, GRPSCENE* scene);
  virtual                      ~UIWIDGETPROGRESSBARDRAWABLE       ()
                                {
                                  Clean();
                                }

  virtual       bool            InternalDraw                      (XDWORD& widgetlevel);
  virtual       bool            InternalUpdate                    (XDWORD widgetlevel);

  virtual       void            SetProgressImages                 (XCHAR* background, XCHAR* foreground);

protected:

                GRPOBJECT*      obj;
                GRPELEMENT*     background;
                GRPELEMENT*     bar;

                bool            imagesloaded;
                GRPTEXTURE*     backgroundimage;
                GRPTEXTURE*     foregroundimage;

                XSTRING         backgroundimagename;
                XSTRING         foregroundimagename;

private:
                void            Clean                             ()
                                {
                                  obj                 = NULL;
                                  background          = NULL;
                                  bar                 = NULL;
                                  imagesloaded        = true;
                                  backgroundimage     = NULL;
                                  foregroundimage     = NULL;
                                }

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

