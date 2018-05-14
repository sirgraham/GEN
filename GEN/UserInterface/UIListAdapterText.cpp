
/*------------------------------------------------------------------------------------------
//  UILISTADAPTERTEXT.CPP
//
//  UI DropBox
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 06/02/2017 17:28:25
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIListAdapterText.h"

#include "UIWidgetImage.h"
#include "UIWidgetText.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  UILISTADAPTERTEXT::UILISTADAPTERTEXT
*/
/**
//
//  Class Constructor UILISTADAPTERTEXT
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 17:45:07
//
//  @param        uimanager :
*/
/*-----------------------------------------------------------------*/
UILISTADAPTERTEXT::UILISTADAPTERTEXT(UIMANAGER* uimanager) : UILISTADAPTER<XSTRING*>(uimanager)
{
  Clean();
}




/*-------------------------------------------------------------------
//  UILISTADAPTERTEXT::~UILISTADAPTERTEXT
*/
/**
//
//   Class Destructor UILISTADAPTERTEXT
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 17:45:12
//
*/
/*-----------------------------------------------------------------*/
UILISTADAPTERTEXT::~UILISTADAPTERTEXT()
{
  Clean();
}




/*-------------------------------------------------------------------
//  UILISTADAPTERTEXT::GetWidget
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      06/02/2017 17:45:19
//
//  @return       UIWIDGET* :
//
//  @param        position :
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UILISTADAPTERTEXT::GetWidget(XDWORD position)
{
  XSTRING* str = this->GetItem(position);

  UIWIDGETIMAGE* background = new UIWIDGETIMAGE(__L("TextImage"), uimanager);
  background->SetImage(__L("background_move.png"));
  background->SetHeight(size);
  background->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);

  UIWIDGETTEXT* text = new UIWIDGETTEXT(__L("TextText"), uimanager);
  text->SetText(str->Get());
  text->SetHorizontalAligment(UIWIDGET_ALIGNFLAGS_LEFT);
  //text->SetVerticalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
  text->SetVerticalAligment(UIWIDGET_ALIGNFLAGS_TOP);
  text->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
  text->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);

  text->SetColor(0, 0, 0);

  background->AddChild(text);

  return background;
}
