/*------------------------------------------------------------------------------------------
//  UIWIDGETWINDOW.H
*/
/**
// \file
//
//  Base Window class for the rest of the controls
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 02/10/2015 16:43:55
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIWIDGETWINDOW_H_
#define _UIWIDGETWINDOW_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XVector.h"
#include "XMap.h"
#include "XString.h"

#include "UIWidgetContainer.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/



/*---- CLASS -----------------------------------------------------------------------------*/
class XSTRING;
class UIFACTORY;

class UIWIDGETFORM: public UIWIDGETCONTAINER
{
  friend class UIFACTORY;

  public:
                                                  UIWIDGETFORM            (XCHAR* name, UIMANAGER * uimanagerarg) : UIWIDGETCONTAINER(name, uimanagerarg)
                                                  {
                                                    Clean();
                                                    widgettype = UI_WIDGET_TYPE_FORM;
                                                  }

                                                  UIWIDGETFORM            (UIWIDGETFORM* rhs) : UIWIDGETCONTAINER(rhs)
                                                  {
                                                    Clean();

                                                    title = rhs->title;
                                                  }

    virtual                                      ~UIWIDGETFORM            ()
                                                  {
                                                    Clean();
                                                  }

    virtual       UIWIDGET*                       Clone                   ()
                                                  {
                                                    return new UIWIDGETFORM(this);
                                                  }

    virtual void                                  SetType                 ()                        { this->widgettype = UI_WIDGET_TYPE_FORM; }

    // Event Handling
    virtual       bool                            HandleInternalEvent     (XEVENT* event);

    virtual       void                            SetActive               (bool active);

                  void                            SetTitle                (XSTRING* title)          { this->title.Set(title->Get());          }
                  void                            SetTitle                (XCHAR* title)            { this->title.Set(title);                 }
                  XSTRING*                        GetTitle                ()                        { return &title;                          }

  protected:
    virtual       bool                            InternalDraw            (XDWORD& widgetlevel);
    virtual       bool                            InternalUpdate          (XDWORD widgetlevel);


                  XSTRING                         title;

  private:

                  void                            Clean                   ()
                                                  {
                                                  }



};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

