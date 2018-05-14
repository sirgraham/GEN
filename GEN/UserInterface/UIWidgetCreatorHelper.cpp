
/*------------------------------------------------------------------------------------------
//  UIWIDGETCREATORHELPER.CPP
//
//  helper class to create complex widgets
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 10/08/2016 16:26:56
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIWidgetCreatorHelper.h"

#include "UIWidgetButton.h"
#include "UIWidgetImage.h"
//#include "UIWidgetLayoutFitParent.h"
#include "UIWidgetText.h"
#include "UIWidgetToast.h"
#include "UIWidgetScrollBar.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  UIWIDGETCREATORHELPER::CreateButton
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      04/10/2016 9:39:22
//
//  @return       UIWIDGET* :
//
//  @param        text :
//  @param        background :
//  @param        hover :
//  @param        click :
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETCREATORHELPER::CreateButton(XCHAR* text, XCHAR* background, XCHAR* hover, XCHAR* click)
{
  UIWIDGETBUTTON* widget = new UIWIDGETBUTTON(text, uimanager);
  if(!widget) return NULL;

  //UIWIDGETLAYOUTFITPARENT* fitparent = new UIWIDGETLAYOUTFITPARENT(text, uimanager);

  //widget->AttachLayout(fitparent);
  //widget->AddChild(fitparent);

  UIWIDGETIMAGE* imagebackground  = NULL;
  UIWIDGETIMAGE* imagehover       = NULL;
  UIWIDGETIMAGE* imageclick       = NULL;

  if(background)
    {
      imagebackground = new UIWIDGETIMAGE(text, uimanager);
      imagebackground->SetImage(background);
      imagebackground->SetPixelPerfect(true);
      imagebackground->SetIsClickable(false);
      imagebackground->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
      imagebackground->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
      widget->AddChild(imagebackground);
    }

  if(hover)
    {
      widget->SetISHoverable(true);
      imagehover = new UIWIDGETIMAGE(text, uimanager);
      imagehover->SetImage(hover);
      imagehover->SetPixelPerfect(true);
      imagehover->SetIsClickable(false);
      imagehover->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
      imagehover->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
      widget->AddChild(imagehover);
    }

  if(click)
    {
      widget->SetIsClickable(true);
      imageclick = new UIWIDGETIMAGE(text, uimanager);
      imageclick->SetImage(click);
      imageclick->SetPixelPerfect(true);
      imageclick->SetIsClickable(false);
      imageclick->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
      imageclick->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
      widget->AddChild(imageclick);
    }

  widget->SetStateWidgets(imagebackground, imagehover, imageclick);

  if(text)
    {
      UIWIDGETTEXT* textwidget = new UIWIDGETTEXT(__L("Text"), uimanager);
      textwidget->SetText(text);
      textwidget->SetIsClickable(false);
      textwidget->SetHorizontalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
      textwidget->SetVerticalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
      textwidget->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
      textwidget->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
      textwidget->SetColor(0, 0, 0);
      textwidget->SetBold(false);
      textwidget->SetItalic(false);
      widget->AddChild(textwidget);
    }


  return widget;
}




/*-------------------------------------------------------------------
//  UIWIDGETCREATORHELPER::CreateMessageDialog
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      04/10/2016 9:39:12
//
//  @return       UIWIDGET* :
//
//  @param        text :
//  @param        backimage :
//  @param        background :
//  @param        hover :
//  @param        click :
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETCREATORHELPER::CreateMessageDialog(XCHAR* text, XCHAR* backimage, XCHAR* background, XCHAR* hover, XCHAR* click)
{
  return NULL;
}




/*-------------------------------------------------------------------
//  UIWIDGETCREATORHELPER::CreateToat
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      04/10/2016 9:39:04
//
//  @return       UIWIDGET* :
//
//  @param        text :
//  @param        image :
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETCREATORHELPER::CreateToast(XCHAR* text, XCHAR* image)
{
  UIWIDGETTOAST* widget = new UIWIDGETTOAST(text, uimanager);

  UIWIDGETIMAGE* imagewidget = new UIWIDGETIMAGE(text, uimanager);

  imagewidget->SetImage(image);
  imagewidget->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
  imagewidget->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
  imagewidget->SetIsClickable(false);

  UIWIDGETTEXT* textwidget = new UIWIDGETTEXT(text, uimanager);

  textwidget->SetText(text);
  textwidget->SetHorizontalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);
  textwidget->SetVerticalConstraints(UIWIDGET_CONSTRAINTS_FILL_PARENT);

  textwidget->SetVerticalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
  textwidget->SetHorizontalAligment(UIWIDGET_ALIGNFLAGS_CENTER);
  textwidget->SetIsClickable(false);

  widget->AddChild(imagewidget);
  widget->AddChild(textwidget);

  return widget;
}


/*-------------------------------------------------------------------
//  UIWIDGETCREATORHELPER::CreateScrollBar
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      24/02/2017 11:01:16
//
//  @return       UIWIDGET* :
//
//  @param        name :
//  @param        upnormal :
//  @param        uphover :
//  @param        uppressed :
//  @param        downnormal :
//  @param        downhover :
//  @param        downpressed :
//  @param        background :
//  @param        movenormal :
//  @param        movehover :
//  @param        movepressed :
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETCREATORHELPER::CreateScrollBar(XCHAR* name, XCHAR* upnormal, XCHAR* uphover, XCHAR* uppressed, XCHAR* downnormal, XCHAR* downhover, XCHAR* downpressed, XCHAR* background, XCHAR* movenormal, XCHAR* movehover, XCHAR* movepressed)
{
  UIWIDGETBUTTON* upbtn = (UIWIDGETBUTTON*)CreateButton(__L(""), upnormal, uphover, uppressed);
  UIWIDGETBUTTON* downbtn = (UIWIDGETBUTTON*)CreateButton(__L(""), downnormal, downhover, downpressed);
  UIWIDGETIMAGE* backgroundimg = new UIWIDGETIMAGE(__L("Background"), uimanager);
  backgroundimg->SetImage(background);
  UIWIDGETBUTTON* movebtn = (UIWIDGETBUTTON*)CreateButton(__L(""), movenormal, movehover, movepressed);

  return CreateScrollBar(name, upbtn, downbtn, backgroundimg, movebtn);
}



/*-------------------------------------------------------------------
//  UIWIDGETCREATORHELPER::CreateScrollBar
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      24/02/2017 11:01:32
//
//  @return       UIWIDGET* :
//
//  @param        upbutton :
//  @param        downbutton :
//  @param        background :
//  @param        movebutton :
*/
/*-----------------------------------------------------------------*/
UIWIDGET* UIWIDGETCREATORHELPER::CreateScrollBar(XCHAR* name, UIWIDGET* upbutton, UIWIDGET* downbutton, UIWIDGET* background, UIWIDGET* movebutton)
{
  UIWIDGETSCROLLBAR* scrollbar = new UIWIDGETSCROLLBAR(name, uimanager);
  scrollbar->SetWidgets((UIWIDGETBUTTON*)upbutton, (UIWIDGETBUTTON*)downbutton, (UIWIDGETIMAGE*)background, (UIWIDGETBUTTON*)movebutton);

  return scrollbar;
}
