
/*------------------------------------------------------------------------------------------
//  UIWIDGETTAB.CPP
//
//  tab widget
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 27/06/2016 12:42:14
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIWidgetTab.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  UIWIDGETTAB::UIWIDGETTAB
*/
/**
//
//  Class Constructor UIWIDGETTAB
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/06/2016 13:02:54
//
//  @param        name :
//  @param        uimanagerarg :
*/
/*-----------------------------------------------------------------*/
UIWIDGETTAB::UIWIDGETTAB(XCHAR* name, UIMANAGER * uimanagerarg) : UIWIDGETCONTAINER(name, uimanagerarg)
{
  Clean();

  widgettype = UI_WIDGET_TYPE_TAB;
}




/*-------------------------------------------------------------------
//  UIWIDGETTAB::~UIWIDGETTAB
*/
/**
//
//   Class Destructor UIWIDGETTAB
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/06/2016 13:02:46
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETTAB::~UIWIDGETTAB()
{
  Clean();
}




/*-------------------------------------------------------------------
//  UIWIDGETTAB::InternalDraw
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/06/2016 13:02:40
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETTAB::InternalDraw(XDWORD& widgetlevel)
{
  return true;
}




/*-------------------------------------------------------------------
//  UIWIDGETTAB::InternalUpdate
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/06/2016 13:02:33
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETTAB::InternalUpdate(XDWORD widgetlevel)
{
  return true;
}




/*-------------------------------------------------------------------
//  UIWIDGETTAB::UnRegisterOwnEvents
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/06/2016 13:02:26
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETTAB::UnRegisterOwnEvents()
{
}



/*-------------------------------------------------------------------
//  UIWIDGETTAB::RegisterOwnEvents
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/06/2016 13:02:19
//
*/
/*-----------------------------------------------------------------*/
void UIWIDGETTAB::RegisterOwnEvents()
{
}




/*-------------------------------------------------------------------
//  UIWIDGETTAB::HandleInternalEvent
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/06/2016 13:02:13
//
//  @return       bool :
//
//  @param        event :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETTAB::HandleInternalEvent(XEVENT* event)
{
  return true;
}



/*-------------------------------------------------------------------
//  UIWIDGETTAB::SetActive
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/06/2016 13:02:06
//
//  @param        active :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETTAB::SetActive(bool active)
{
  UIWIDGETCONTAINER::SetActive(active);

  XDWORD size = tabs.GetSize();

  for(XDWORD i = 0; i < size; i++)
    {
      XVECTOR<UIWIDGET*>* tab = tabs.FastGet(i);
      XDWORD innersize = tab->GetSize();

      bool localactive = active && (activetab == i);

      for(XDWORD j = 0; j < innersize; j++)
        {
          tab->FastGet(j)->SetActive(localactive);
        }
    }
}




/*-------------------------------------------------------------------
//  UIWIDGETTAB::SetActiveTab
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/06/2016 13:01:59
//
//  @return       bool :
//
//  @param        index :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETTAB::SetActiveTab(XDWORD index)
{
  if(activetab >= tabs.GetSize())
    {
      return false;
    }

  activetab = index;

  // set rest of tabs as inactive;
  XDWORD size = tabs.GetSize();

  for(XDWORD i = 0; i < size; i++)
    {
      XVECTOR<UIWIDGET*>* tab = tabs.FastGet(i);
      XDWORD innersize = tab->GetSize();

      if(i != index)
        {
          for(XDWORD j = 0; j < innersize; j++)
            {
              tab->FastGet(j)->SetActive(false);
            }
        }
      else
        {
          for(XDWORD j = 0; j < innersize; j++)
            {
              tab->FastGet(j)->SetActive(true);
            }
        }
    }

  return true;
}



/*-------------------------------------------------------------------
//  UIWIDGETTAB::AddTab
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/06/2016 13:01:51
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETTAB::AddTab()
{
  XVECTOR<UIWIDGET*>* newtab = new XVECTOR<UIWIDGET*>();
  if(!newtab) return false;

  return tabs.Add(newtab);
}




/*-------------------------------------------------------------------
//  UIWIDGETTAB::RemoveTab
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/06/2016 13:01:43
//
//  @return       bool :
//
//  @param        index :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETTAB::RemoveTab(XDWORD index)
{
  // what to do with children widgets here?

  return tabs.DeleteIndex(index);
}



/*-------------------------------------------------------------------
//  UIWIDGETTAB::GetNumberOfTabs
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/06/2016 13:01:36
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD UIWIDGETTAB::GetNumberOfTabs()
{
  return tabs.GetSize();
}




/*-------------------------------------------------------------------
//  UIWIDGETTAB::GetTab
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      27/06/2016 13:01:29
//
//  @return       XVECTOR<UIWIDGET*>* :
//
//  @param        index :
*/
/*-----------------------------------------------------------------*/
XVECTOR<UIWIDGET*>* UIWIDGETTAB::GetTab(XDWORD index)
{
  return tabs.Get(index);
}
