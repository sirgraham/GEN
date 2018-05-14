/*------------------------------------------------------------------------------------------
//  UIWIDGETZOOMCONTAINER.H
*/
/**
// \file
//
//  makes zoom on widgets
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 08/03/2017 10:10:06
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETZOOMCONTAINER_H_
#define _UIWIDGETZOOMCONTAINER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "UIWidgetContainer.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGETZOOMCONTAINER : public UIWIDGETCONTAINER
{
public:

                                      UIWIDGETZOOMCONTAINER                                 (XCHAR* name, UIMANAGER* uimanager);
                                      UIWIDGETZOOMCONTAINER                                 (UIWIDGETZOOMCONTAINER* rhs);
  virtual                            ~UIWIDGETZOOMCONTAINER                                 ();

  virtual UIWIDGET*                   Clone                                                 ()
                                      {
                                        return new UIWIDGETZOOMCONTAINER(this);
                                      }

  void                                SetType                                               ()                                    { widgettype = UI_WIDGET_TYPE_ZOOM;         }

  virtual       UIWIDGET*             GetHoveredWidget                                      (bool force = true, float zoom = 1, float x = 0, float y = 0);

  void                                SetZoom                                               (float zoom)                          { this->zoom = zoom;                        }
  float                               GetZoom                                               ()                                    { return zoom;                              }

  void                                SetZoomPosition                                       (int zoomx, int zoomy)                { this->zoomx = zoomx; this->zoomy = zoomy; }
  int                                 GetZoomX                                              ()                                    { return zoomx;                             }
  int                                 GetZoomY                                              ()                                    { return zoomy;                             }

  float                               gptzx;
  float                               gptzy;

protected:

  virtual bool                        InternalUpdate                                        (XDWORD widgetlevel);

  float                               zoom;
  int                                 zoomx;
  int                                 zoomy;

private:
  void                                Clean                                                 ()
                                      {
                                        zoom  = 1;
                                        zoomx = 0;
                                        zoomy = 0;
                                      }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

