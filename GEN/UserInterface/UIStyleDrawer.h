/*------------------------------------------------------------------------------------------
//  UISTYLEDRAWER.H
*/
/**
// \file
//
//  draws a UI
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 08/08/2016 17:09:03
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UISTYLEDRAWER_H_
#define _UISTYLEDRAWER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "GRPPass.h"

#include "UIStyle.h"

#include "XList.h"

#include "XBase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

// if I make this inherit from GRPPASS I can insert the UI elements into the GRP architecture

class GRPSCENE;
class UIGRPOBJECTBUILDER;

struct UISCISSOR
{
  bool operator==(const UISCISSOR& rhs)
  {
    return (x0 == rhs.x0) && (y0 == rhs.y0) && (x1 == rhs.x1) && (y1 == rhs.y1) && (constrain == rhs.constrain);
  }

  int x0;
  int y0;
  int x1;
  int y1;
  bool constrain;
};

struct UIZOOM
{
  bool operator==(const UIZOOM& rhs)
  {
    return (zoom == rhs.zoom) && (zoomx == rhs.zoomx) && (zoomy == rhs.zoomy);
  }

  float zoom;
  float zoomx;
  float zoomy;
};

class UISTYLEDRAWER : public UISTYLE, public GRPPASS
{
public:

                                      UISTYLEDRAWER                                 (GRPSCENE* uiscene, XCHAR* name);
  virtual                            ~UISTYLEDRAWER                                 ();

  void                                ShowDebug                                     (bool showdebug)                                                { this->showdebug = showdebug;  }
  bool                                GetShowDebug                                  ()                                                              { return  showdebug;            }

  // implement pass draw and walk the widget tree

  bool                                Draw                                          (GRPPASS*);
  bool                                Draw                                          ();
  bool                                Draw                                          (UIWIDGETCONTAINER* widget);
  bool                                PostDraw                                      (UIWIDGETCONTAINER* widget);

  UIGRPOBJECTBUILDER*                 GetObjectBuilder                              ()                                                              { return objectbuilder;         }



protected:

  GRPELEMENT*                         BuildDebugLine                                (GRPPOINT p0, GRPPOINT p1);
  void                                DeleteDebugLine                               (GRPELEMENT* element);

  void                                PushScissor                                   (int & x0, int &y0, int &x1, int &y1, bool constrain = true);
  void                                PopScissor                                    ();

  void                                PushZoom                                      (float zoom, float zoomx, float zoomy);
  void                                PopZoom                                       ();

  GRPSCENE*                           uiscene;
  UIGRPOBJECTBUILDER*                 objectbuilder;

  XLIST<UISCISSOR>                    scissorstack;
  XLIST<UIZOOM>                       zoomstack;

  bool                                showdebug;

private:
  void                                RecursiveDraw                                 (UIWIDGETCONTAINER* widget);
  void                                RecursiveDebugDraw                            (UIWIDGETCONTAINER* widget);
  void                                DrawDebugLine                                 (GRPPOINT p0, GRPPOINT p1);

  void                                ApplyZoom                                     (float& pt0x, float& pt0y, float& pt1x, float& pt1y, float& scalex, float& scaley);

  void                                Clean                                         ()
                                      {
                                        uiscene       = NULL;
                                        objectbuilder = NULL;
                                        showdebug     = false;
                                      }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

