/*------------------------------------------------------------------------------------------
//  UIFACTORY.H
*/
/**
// \file
//
//  Factory to build every UI object
//
//  @author  Imanol Celaya Ruiz de Alegria
//
//  Date Of Creation  : 05/10/2015 13:15:42
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _UIFACTORY_H_
#define _UIFACTORY_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "XVector.h"
#include "XString.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/
enum UI_WIDGET_TYPE
{
  UI_WIDGET_TYPE_UNKNOWN        = 0           ,
  UI_WIDGET_TYPE_CONTAINER                    ,
  UI_WIDGET_TYPE_FORM                         ,
  UI_WIDGET_TYPE_TEXT                         ,
  UI_WIDGET_TYPE_BUTTON                       ,
  UI_WIDGET_TYPE_PROGRESSBAR                  ,
  UI_WIDGET_TYPE_IMAGE                        ,
  UI_WIDGET_TYPE_VIDEO                        ,
  UI_WIDGET_TYPE_LIST                         ,
  UI_WIDGET_TYPE_TAB                          ,
  UI_WIDGET_TYPE_LAYOUT                       ,
  UI_WIDGET_TYPE_SCISSORCONTAINER             ,
  UI_WIDGET_TYPE_COMBOBOX                     ,
  UI_WIDGET_TYPE_3DMODEL                      ,
  UI_WIDGET_TYPE_GRIDCONTAINER                ,
  UI_WIDGET_TYPE_SCROLLCONTAINER              ,
  UI_WIDGET_TYPE_SCROLLBAR                    ,
  UI_WIDGET_TYPE_ZOOM                         ,
  UI_WIDGET_TYPE_DIAL                         ,

  UI_WIDGET_TYPE_CUSTOM         = 0xf0000000  ,
};

/*---- CLASS -----------------------------------------------------------------------------*/
class UIWIDGET;
class UIMANAGER;
class XPUBLISHER;
class XFACTORY;

class UIFACTORY
{
  friend class UIMANAGER;

    public:

                                                    UIFACTORY                 ()
                                                    {
                                                      Clean();
                                                    }
      virtual                                      ~UIFACTORY                 ()                                                     { Clean(); }
    protected:

    private:
                        void                        Clean                     ()
                                                    {

                                                    }

                        UIWIDGET*                   CreateWidget              (UI_WIDGET_TYPE type, UIMANAGER* uimanager, XCHAR* name);
                        bool                        DeleteWidget              (UIWIDGET*);
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

