
/*------------------------------------------------------------------------------------------
//  UIWIDGETTEXTDRAWABLE.CPP
//
//  draws a textbox
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 30/10/2015 10:43:43
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UIWidgetTextDrawable.h"
#include "UIWidgetContainer.h"
#include "GRPScene.h"
#include "GRPFontManager.h"
#include "GRPRenderer.h"
#include "GRPObjectBuilder.h"
#include "GRPFont.h"
#include "GRPMaterialLibrary.h"
#include "GRPTextBox.h"
#include "GRPViewPort.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  UIWIDGETTEXTDRAWABLE::UIWIDGETTEXTDRAWABLE
*/
/**
//
//  Class Constructor UIWIDGETTEXTDRAWABLE
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/02/2016 10:32:43
//
//  @param        name :
//  @param        uimanagerarg :
//  @param        scene :
*/
/*-----------------------------------------------------------------*/
UIWIDGETTEXTDRAWABLE::UIWIDGETTEXTDRAWABLE (XCHAR* name, UIMANAGER * uimanagerarg, GRPSCENE* scene) : UIWIDGETTEXT(name, uimanagerarg)
{
  Clean();

  this->scene = scene;

  textbox = new GRPTEXTBOX(scene);
  textbox->SetNode(scene->CreateNode());


  GRPFONTMANAGER* fontmanager = GRPOBJECTBUILDER::Get()->GetFontManager();
  font = fontmanager->GetFont(__L("UI"));

  textbox->SetFont(font);
  textbox->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"BAS"));

  GRPMATERIAL* materialproc = new GRPMATERIAL();
  materialproc->GetAttributes()->SetDiffuse(GRPCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
  materialproc->GetAttributes()->isupdated = true;
  textbox->GetNode()->SetMaterial(materialproc);
  textbox->SetMaterial(materialproc);
  GRPMATERIALLIBRARY::Get()->Add(materialproc);

  this->scene->Add(textbox);
  SetText(__L(""));
  textbox->SetAlign(GRPTEXTBOX_ALIGNLEFT);
  textbox->SetContent(GetText()->Get());
  textbox->MergeElements();
}




/*-------------------------------------------------------------------
//  UIWIDGETTEXTDRAWABLE::~UIWIDGETTEXTDRAWABLE
*/
/**
//
//   Class Destructor UIWIDGETTEXTDRAWABLE
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/07/2016 16:56:18
//
*/
/*-----------------------------------------------------------------*/
UIWIDGETTEXTDRAWABLE::~UIWIDGETTEXTDRAWABLE()
{
  scene->GetObjects()->Delete(textbox);

  delete textbox;
}



/*-------------------------------------------------------------------
//  UIWIDGETTEXTDRAWABLE::InternalDraw
*/
/**
//
//  draws the text
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/02/2016 10:32:35
//
//  @return       bool :
//
//  @param        widgetlevel :
*/
/*-----------------------------------------------------------------*/
bool UIWIDGETTEXTDRAWABLE::InternalDraw(XDWORD& widgetlevel)
{
  if(textchanged)
    {
      textbox->SetContent(GetText()->Get());
      textbox->MergeElements();
      textbox->GetElements()->FastGet(0)->UpdateBoundingBox();
      scene->Add(textbox);

      textchanged = false;
      modified = true;
    }

  if(textbox->GetElements()->GetSize())
    {
      if(modified)
        {
          float maxpointx = textbox->GetElements()->FastGet(0)->GetBoundingBox()->maxpoint.x;
          float minpointx = textbox->GetElements()->FastGet(0)->GetBoundingBox()->minpoint.x;

          float maxpointy = textbox->GetElements()->FastGet(0)->GetBoundingBox()->maxpoint.y;
          float minpointy = textbox->GetElements()->FastGet(0)->GetBoundingBox()->minpoint.y;

          GRPPOINT nmax = scene->GetViewport()->GetInverseNormalizeFromViewPort(maxpointx, maxpointy);
          GRPPOINT nmin = scene->GetViewport()->GetInverseNormalizeFromViewPort(minpointx, minpointy);

          float nscalex = nmax.x - nmin.x;
          float nscaley = nmin.y - nmax.y;

          // probably not
          float gscalex = gpt1x - gpt0x;
          float gscaley = gpt1y - gpt0y;

          float scalex = gpt1x - gpt0x;
          float scaley = gpt1y - gpt0y;

          if(nscalex > scalex)
            {
              // adjust in horizontal, which is not this, as this way we arre adjusting the size we were passed instead of our size
              float ratiox = scalex/nscalex;
              nscalex *= ratiox;
              nscaley *= ratiox;
            }

          if(nscaley > scaley)
            {
              // adjust in vertical
              float ratioy = scaley/nscaley;
              nscalex *= ratioy;
              nscaley *= ratioy;
            }

          scalex = nscalex;

          GRPPOINT pt = scene->GetViewport()->GetViewPortFromInverseNormalized(gpt0x, gpt0y, 0.0f);

          float x = pt.x;
          float y = pt.y;

          float normalizationscale = maxpointx - minpointx;

          float verticalsize = maxpointy - minpointy;
          verticalsize /= normalizationscale;


          float haligment = 0.0f;
          float valigment = 0.0f;

          // now we need to align if we have wiggle room
          if(horizontalaligment == UIWIDGET_ALIGNFLAGS_CENTER)
            {
              float diff = gscalex - nscalex;
              haligment = diff/2.0f;
            }
          else if(horizontalaligment == UIWIDGET_ALIGNFLAGS_RIGHT)
            {
              float diff = gscalex - nscalex;
              haligment = diff;
            }

          if(verticalaligment == UIWIDGET_ALIGNFLAGS_CENTER)
            {
              float diff = gscaley - nscaley;
              valigment = diff/2.0f;
            }
          else if(verticalaligment == UIWIDGET_ALIGNFLAGS_TOP)
            {
              float diff = gscaley - nscaley;
              valigment = diff;
            }

          GRPPOINT realpt = scene->GetViewport()->GetViewPortFromInverseNormalized(gpt0x + haligment, gpt1y - valigment, 0.0f);

          textbox->GetNode()->SetPosition(realpt.x - 2.0f*minpointx*scalex/normalizationscale, realpt.y, widgetlevel + 0.1f);

          // set appropiate scale
          textbox->GetNode()->SetScale(2.0f*scalex/normalizationscale, 2.0f*scalex/normalizationscale, 1.0f);

          textbox->GetNode()->Update();
          textbox->GetNode()->UpdateWorldMatrix();

          this->SetActive(isactive);
        }

      textbox->GetElements()->Get(0)->Publish(GRPMSG_ORDER);
    }

  return true;
}





/*-------------------------------------------------------------------
//  UIWIDGETTEXTDRAWABLE::SetActive
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/06/2016 13:35:19
//
//  @param        active :
*/
/*-----------------------------------------------------------------*/
void UIWIDGETTEXTDRAWABLE::SetActive(bool active)
{
  textbox->SetVisible(active);

  UIWIDGETTEXT::SetActive(active);
}
