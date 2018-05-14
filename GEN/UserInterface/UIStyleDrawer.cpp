
/*------------------------------------------------------------------------------------------
//  UISTYLEDRAWER.CPP
//
//  draws the UI
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 08/08/2016 17:13:07
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"

#include "UIStyleDrawer.h"

#include "UIManager.h"
#include "UIWidgetContainer.h"
#include "UIWidgetImage.h"
#include "UIWidgetListContainer.h"
#include "UIWidgetText.h"
#include "UIWidgetScissorContainer.h"
#include "UIWidgetVideo.h"
#include "UIWidget3DModel.h"
#include "UIWidgetZoomContainer.h"

#include "UIGRPObjectBuilder.h"

#include "GRPElement.h"
#include "GRPMaterialLibrary.h"
#include "GRPScene.h"
#include "GRPViewPort.h"
#include "GRPCamera.h"
#include "GRPTextBox.h"
#include "GRPFont.h"
#include "GRPFileTTF.h"
#include "GRPFontManager.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/




/*-------------------------------------------------------------------
//  UISTYLEDRAWER::UISTYLEDRAWER
*/
/**
//
//  Class Constructor UISTYLEDRAWER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/08/2016 17:17:17
//
*/
/*-----------------------------------------------------------------*/
UISTYLEDRAWER::UISTYLEDRAWER(GRPSCENE* uiscene, XCHAR* name) : UISTYLE(), GRPPASS(name)
{
  Clean();

  this->SetScene(uiscene);
  this->AddType(GRPPASS_TYPE_ALPHABLEND);
  this->uiscene = uiscene;
  objectbuilder = new UIGRPOBJECTBUILDER(uiscene);

  this->SetName(__L("UIPass"));
}




/*-------------------------------------------------------------------
//  UISTYLEDRAWER::~UISTYLEDRAWER
*/
/**
//
//   Class Destructor UISTYLEDRAWER
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/08/2016 17:17:24
//
*/
/*-----------------------------------------------------------------*/
UISTYLEDRAWER::~UISTYLEDRAWER()
{
  delete objectbuilder;

  Clean();
}



/*-------------------------------------------------------------------
//  UISTYLEDRAWER::Draw
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/08/2016 12:30:24
//
//  @return       bool :
//
//  @param        GRPPASS* :
*/
/*-----------------------------------------------------------------*/
bool UISTYLEDRAWER::Draw(GRPPASS*)
{
  return this->Draw();
}



/*-------------------------------------------------------------------
//  UISTYLEDRAWER::Draw
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/08/2016 12:30:17
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool UISTYLEDRAWER::Draw()
{
  if(!uimanager) return false;

  this->DepthTestIsActive = false;
  this->AlphaBlendingIsActive = true;
  this->AlphaBlendingMode = GRPBLENDINGMODE_ALPHABLEND;

  this->Begin();

  UIWIDGETCONTAINER* widget = (UIWIDGETCONTAINER*)uimanager->GetRootWidget();

  if(widget)
    {
      if(widget->GetIsActive())
        {
          this->RecursiveDraw(widget);
        }
      else
        {
          //XDEBUG_PRINTCOLOR(2, __L("INACTIVE"));
        }
    }

  if(uimanager->GetDrawMouse())
    {
      RecursiveDraw(uimanager->GetMouseWidget());
    }

  if(showdebug)
    {
      if(widget)
        {
          if(widget->GetIsActive())
            {
              RecursiveDebugDraw(widget);
            }
        }
    }


  this->End();

  return true;
}

float rot = 0;

/*-------------------------------------------------------------------
//  UISTYLEDRAWER::Draw
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/08/2016 17:17:31
//
//  @return       bool :
//
//  @param        widget :
*/
/*-----------------------------------------------------------------*/
bool UISTYLEDRAWER::Draw(UIWIDGETCONTAINER* widget)
{
  if(!widget) return false;
  // render methodology, take a widget and get the corresponding GRPELEMENT from the object builder
  // this in turn will give out a cached one if available or a new one if none is present(think all the properties it has)
  // then use it to render the widget on the appropiate way, widgets should try to decompose everything into images or buttons(the button will end up having an image)?

  switch(widget->GetType())
  {
    case UI_WIDGET_TYPE_FORM:
      break;
    case UI_WIDGET_TYPE_TEXT: // try to cache text?
      {
        // the big elephant in the room
        UIWIDGETTEXT* textwidget = (UIWIDGETTEXT*)widget;


        UIGRPOBJECTBUILDER::TEXTPROPERTIES properties;
        *properties.string = *textwidget->GetText();
        properties.bold = textwidget->GetBold();
        properties.italic = textwidget->GetItalic();

        GRPTEXTBOX* textbox = objectbuilder->GetText(properties);
        if(!textbox) return false;

        // need to move this font management inside object builder as outside it won't work
        GRPFONTMANAGER* fontmanager = GRPOBJECTBUILDER::Get()->GetFontManager();


        GRPNODE* node = textbox->GetNode();

        float maxpointx = textbox->GetElements()->FastGet(0)->GetBoundingBox()->maxpoint.x;
        float minpointx = textbox->GetElements()->FastGet(0)->GetBoundingBox()->minpoint.x;

        float maxpointy = textbox->GetElements()->FastGet(0)->GetBoundingBox()->maxpoint.y;
        float minpointy = textbox->GetElements()->FastGet(0)->GetBoundingBox()->minpoint.y;

        GRPFONT* font = textbox->GetFont();
        GRPFILETTF* ttf = font->GetTTF();

        float advance, leftbearing, ascend, descend, linegap;
        ttf->GetGlyphMetrics(32, &advance, &leftbearing, &ascend, &descend, &linegap);

        float typoascend, typodescend, typolinegap;
        ttf->GetFontOS2Metrics(&typoascend, &typodescend, &typolinegap);

        float ntypoascend = typoascend*TTFGLOBALSCALE;
        float ntypodescend = typodescend*TTFGLOBALSCALE;
        float ntypolinegap = typolinegap*TTFGLOBALSCALE;
        float fdescend = descend;
        float ndesced = descend*TTFGLOBALSCALE;

        float height = (float)(fabs(maxpointy) + fabs(minpointy));
        float flineheight = font->GetLineHeight();
        float ttflineheight = flineheight*TTFGLOBALSCALE;
        float globalscale = TTFGLOBALSCALE;
        //float ratio = uimanager->GetRealHeight()/flineheight;
        float ratio = uimanager->GetVirtualHeight()/flineheight;
        float ttfratio = uimanager->GetRealHeight()/ttflineheight;




        float test = flineheight*ratio;
        //ttf->GetLineDescend(&descend);


        //minpointy = maxpointy - flineheight;

        // use line height for vertical size?

        GRPPOINT nmax = uiscene->GetViewport()->GetInverseNormalizeFromViewPort(maxpointx, maxpointy);
        GRPPOINT nmin = uiscene->GetViewport()->GetInverseNormalizeFromViewPort(minpointx, minpointy);

        GRPPOINT realscaley = uiscene->GetViewport()->GetInverseNormalizeFromViewPort(0.0f, flineheight);

        float width = maxpointx - minpointx;
        float nscalex = nmax.x - nmin.x;
        float nscaley = nmin.y - nmax.y;
        float aspect = nscalex/nscaley;

        float scalex = textwidget->gpt1x - textwidget->gpt0x;
        float scaley = textwidget->gpt1y - textwidget->gpt0y;

        //nscaley = flineheight;
        float maxy = maxpointy*TTFGLOBALSCALE;
        float miny = minpointy*TTFGLOBALSCALE;
        float origscaley = nscaley;
        nscaley = (float)(fabs(typoascend) + fabs(typodescend));

        ApplyZoom(textwidget->gpt0x, textwidget->gpt0y, textwidget->gpt1x, textwidget->gpt1y, scalex, scaley);

        nscalex *= uimanager->GetRealWidth();
        nscaley *= uimanager->GetRealHeight();
        //nscaley*=TTFGLOBALSCALE;

        // probably not
        float gscalex = textwidget->gpt1x - textwidget->gpt0x;
        float gscaley = textwidget->gpt1y - textwidget->gpt0y;

        float rscaley = textwidget->gpt1y - textwidget->gpt0y;

        scalex *= uimanager->GetRealWidth(); // need to take from viewport
        scaley *= uimanager->GetRealHeight();

        gscalex *= uimanager->GetRealWidth(); // need to take from viewport
        gscaley *= uimanager->GetRealHeight();


        // removed to evoid using horizontal for text size and control better the text height
        //if(nscalex > scalex)
        //  {
        //    // adjust in horizontal, which is not this, as this way we arre adjusting the size we were passed instead of our size
        //    float ratiox = scalex/nscalex;
        //    nscalex *= ratiox;
        //    nscaley *= ratiox;
        //  }

        if(nscaley > scaley)
          {
            // adjust in vertical
            float ratioy = scaley/nscaley;
            nscalex *= ratioy;
            nscaley *= ratioy;
          }

        scalex = nscalex;

        //GRPPOINT pt = uiscene->GetViewport()->GetViewPortFromInverseNormalized(textwidget->gpt0x, textwidget->gpt0y, 0.0f);
        //
        //pt.x *= uimanager->GetRealWidth();
        //pt.y *= uimanager->GetRealWidth();
        //
        //float x = pt.x;
        //float y = pt.y;

        float normalizationscale = maxpointx - minpointx;

        //float verticalsize = maxpointy - minpointy;
        //verticalsize /= normalizationscale;


        float haligment = 0.0f;
        float valigment = 0.0f;

        // now we need to align if we have wiggle room
        float sc = (float)(uimanager->GetRealHeight()/(fabs(typoascend) + fabs(typodescend)));

        width = width*sc*rscaley;

        if(textwidget->GetHorizontalAligment() == UIWIDGET_ALIGNFLAGS_CENTER)
          {
            float diff = gscalex - width;
            haligment = diff/2.0f;
          }
        else if(textwidget->GetHorizontalAligment() == UIWIDGET_ALIGNFLAGS_RIGHT)
          {
            float diff = gscalex - width;
            haligment = diff;
          }

        if(textwidget->GetVerticalAligment() == UIWIDGET_ALIGNFLAGS_CENTER)
          {
            float diff = gscaley - nscaley;
            valigment = diff/2.0f;
          }
        else if(textwidget->GetVerticalAligment() == UIWIDGET_ALIGNFLAGS_TOP)
          {
            float diff = gscaley - nscaley;
            valigment = diff;
          }

        // scale needed to work on real screen coordinates

        float vsc = (float)(uimanager->GetVirtualHeight()/(fabs(typoascend) + fabs(typodescend)));

        GRPPOINT realpt = uiscene->GetViewport()->GetViewPortFromInverseNormalized(textwidget->gpt0x/* + haligment/uimanager->GetRealWidth()*/, textwidget->gpt1y/* - valigment/uimanager->GetRealWidth()*/, 0.0f);

        realpt.x *= uimanager->GetRealWidth();
        realpt.y *= uimanager->GetRealWidth();

        GRPPOINT vertical = uiscene->GetViewport()->GetViewPortFromInverseNormalized(0.0f, flineheight, 0.0f);

        float v = typodescend*sc;

        float rh = (float)uimanager->GetRealHeight();
        //haligment = 0.0f;
        //textbox->GetNode()->SetPosition(realpt.x - 2.0f*minpointx*scalex/normalizationscale, realpt.y - (-14)/*(typodescend*2.0f*scalex/normalizationscale)*/, 0.0f); // fdescend needs to have actual scale applied
        //textbox->GetNode()->SetPosition(realpt.x + 2.0f*haligment - 2.0f*minpointx*sc*rscaley/*2.0f*minpointx*scalex/normalizationscale*/, realpt.y - (-2.0f*fabs(typodescend)*rscaley*sc)/*(typodescend*2.0f*scalex/normalizationscale)*/, 0.0f); // fdescend needs to have actual scale applied
        textbox->GetNode()->SetPosition(realpt.x + 2.0f*haligment, realpt.y - (float)(-2.0f*fabs(typodescend)*rscaley*sc), 0.0f); // fdescend needs to have actual scale applied
        //textbox->GetNode()->SetPosition(-1280, realpt.y - (-2.0f*fabs(typodescend)*rscaley*sc)/*(typodescend*2.0f*scalex/normalizationscale)*/, 0.0f); // fdescend needs to have actual scale applied



        // set appropiate scale
        //textbox->GetNode()->SetScale(2.0f*scalex/normalizationscale, 2.0f*scalex/normalizationscale, 1.0f);
        //textbox->GetNode()->SetScale(nscaley, nscaley, 1.0f);
        //textbox->GetNode()->SetScale(1920/flineheight, 1920/flineheight, 1.0f);
        //textbox->GetNode()->SetScale(720/(fabs(ntypoascend) + fabs(ntypodescend)), 720/(fabs(ntypoascend) + fabs(ntypodescend)), 1.0f);
        //textbox->GetNode()->SetScale((fabs(ntypoascend) + fabs(ntypodescend)), (fabs(ntypoascend) + fabs(ntypodescend)), 1.0f);

        textbox->GetNode()->SetScale(2.0f*sc*rscaley, 2.0f*sc*rscaley, 1.0f);
        //textbox->GetNode()->SetScale(ratio, ratio, 1.0f);

        float ypp = typoascend*sc;
        float yp = maxpointy*sc;

        bool update = true;
        textbox->GetNode()->Update();
        textbox->GetNode()->UpdateWorldMatrix(&update);


        // compute matrixes
        GRPCAMERA*      camera = GetScene()->GetCamera();
        camera->BuildLookAtMatrix();

        this->viewmatrix = &camera->viewmatrix;

        //this->projectionmatrix  = *viewport;//*(holderscene->GetViewports()->Get(holderscene->GetCurrentViewport()));
        this->projectionmatrix = *GetScene()->GetViewport();//*(holderscene->GetViewports()->Get(holderscene->GetCurrentViewport()));
        this->viewprojection.GetMultiplicationMatrix(this->viewmatrix,this->projectionmatrix);  //get view*projection


        // set the textbox color in here manually each time?
        XDWORD nelements = textbox->GetElements()->GetSize();

        for(XDWORD i = 0; i < nelements; i++)
          {
            GRPELEMENT* element = textbox->GetElements()->FastGet(i);

            bool update = true;
            element->GetNode()->SetScale(1.0f, 1.0f, 1.0f);
            element->GetNode()->Update();
            element->GetNode()->UpdateWorldMatrix(&update);
            element->ComputeMatrices(viewmatrix,&viewprojection,projectionmatrix);
            GRPSHADER::UseShader(element->GetProgram());
            GRPCOLOR p(textwidget->GetColorR(), textwidget->GetColorG(), textwidget->GetColorB());
            element->GetNode()->GetMaterial()->GetAttributes()->SetDiffuse(p);
            element->GetNode()->GetMaterial()->GetAttributes()->isupdated = true;
            element->SetBlending(true);
            element->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
            element->Draw(&camera->viewmatrix,&viewprojection,projectionmatrix);
          }
      }
      break;
    case UI_WIDGET_TYPE_BUTTON: // no real drawing, just its contents?
      break;
    case UI_WIDGET_TYPE_PROGRESSBAR:
      // need to handle both images
      // maybe also good use for scissor?
      // yes, but need to draw two image widgets and apply the scissor in the middle somehow(middle container?)
      break;
    case UI_WIDGET_TYPE_IMAGE:
      {
        UIWIDGETIMAGE* imagewidget = (UIWIDGETIMAGE*)widget;
        UIGRPOBJECTBUILDER::PROPERTIES prop;
        prop.cornerradious = 0;

        GRPELEMENT* element = objectbuilder->GetElement(prop);
        if(!element) return false;

        // set the node properties and the corresponding image(where to save/load the texture?), texture manager should save them by name

        // try to get the material, if none exists create it and get it then
        GRPMATERIAL* material = GRPMATERIALLIBRARY::Get()->FindByName(imagewidget->GetImage());
        if(!material)
          {
            XPATH xpath;
            if(imagewidget->GetPath())
              {
                xpath = imagewidget->GetPath();
                xpath.Slash_Add();
                xpath += imagewidget->GetImage();
              }
            else
              {
                xpath.Create(XPATHSMANAGERSECTIONTYPE_GRAPHICS, 1 ,imagewidget->GetImage());
              }

            GRPMATERIALLIBRARY::Get()->Load(xpath.Get(), imagewidget->GetImage());
            // aaaand, there is no easy war of changing a texture wrap types
            //material = GRPMATERIALLIBRARY::Get()->FindByName(imagewidget->GetImage());
            //GRPTEXTURE* texture = material->GetLayer(GRPMATERIAL_LAYER_DIFFUSECOLOR);
          }

        material = GRPMATERIALLIBRARY::Get()->FindByName(imagewidget->GetImage());
        element->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));
        element->GetNode()->SetMaterial(material);

        // draw the element(setup node size, position, etc...)
        float scalex = imagewidget->gpt1x - imagewidget->gpt0x;
        float scaley = imagewidget->gpt1y - imagewidget->gpt0y;

        scalex *= uimanager->GetRealWidth(); // need to take from viewport
        scaley *= uimanager->GetRealWidth();

        ApplyZoom(imagewidget->gpt0x, imagewidget->gpt0y, imagewidget->gpt1x, imagewidget->gpt1y, scalex, scaley);

        // need to reconsider this viewport
        // careful, as takes for granted a -1.0 to 1.0 viewport
        GRPPOINT pt = GetScene()->GetViewport()->GetViewPortFromInverseNormalized(imagewidget->gpt0x, imagewidget->gpt0y, 0.0f);
        scaley /= GetScene()->GetViewport()->GetAspectRatio();

        pt.x *= uimanager->GetRealWidth();
        pt.y *= uimanager->GetRealWidth();

        float x = pt.x + scalex;
        float y = pt.y - scaley;

        bool update = true;

        if(imagewidget->GetRotationZ() != 0.0f)
          {
            // something fails here
            float vx = x - pt.x;
            float vy = y - pt.y;

            vx = scalex;
            vy = scaley;

            float px = -(((imagewidget->GetRotationPivotX())/(float)imagewidget->GetWidth())*scalex);
            float py = -(((imagewidget->GetRotationPivotY())/(float)imagewidget->GetHeight())*scaley);

            vx = scalex + px*2;
            vy = scaley + py*2;

            // compute new position
            float fx = (float)(vx*cos(DEGREE2RADIAN(imagewidget->GetRotationZ())) - vy*sin(DEGREE2RADIAN(imagewidget->GetRotationZ())));
            float fy = (float)(vx*sin(DEGREE2RADIAN(imagewidget->GetRotationZ())) + vy*cos(DEGREE2RADIAN(imagewidget->GetRotationZ())));

            x = pt.x + fx;
            y = pt.y - fy;

            // px and x are on two different coordinates system
            x -= px*2;
            y += py*2;
          }

        //imageobject->GetNode()->SetPosition(x, y, widgetlevel + 0.1f);
        //imageobject->GetNode()->SetScale(scalex, scaley, 1.0f);

        // no real need to widgetlevel, as draw will be performen hierarchically
        element->GetNode()->SetPosition(x, y, 0);
        element->GetNode()->SetScale(scalex, scaley, 1.0f);
        element->GetNode()->SetRotation(-imagewidget->GetRotationX(), -imagewidget->GetRotationY(), -imagewidget->GetRotationZ());
        element->GetNode()->Update();
        element->GetNode()->UpdateWorldMatrix(&update);

        // compute matrixes
        GRPCAMERA* camera = GetScene()->GetCamera();
        camera->BuildLookAtMatrix();

        this->viewmatrix = &camera->viewmatrix;

        //this->projectionmatrix  = *viewport;//*(holderscene->GetViewports()->Get(holderscene->GetCurrentViewport()));
        this->projectionmatrix = *GetScene()->GetViewport();//*(holderscene->GetViewports()->Get(holderscene->GetCurrentViewport()));
        this->viewprojection.GetMultiplicationMatrix(this->viewmatrix,this->projectionmatrix);  //get view*projection

        element->ComputeMatrices(viewmatrix,&viewprojection,projectionmatrix);
        GRPSHADER::UseShader(element->GetProgram());
        element->SetBlending(true);
        element->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
        element->Draw(&camera->viewmatrix,&viewprojection,projectionmatrix);
      }
      break;
    case UI_WIDGET_TYPE_LIST:
      {
        // need to compoensate for the mismatch between virtual pixels and real screen pixels
        // WARNING: BUT WHAT ABOUT PARENT OFFSET ?? !! THE SAME IN SCISSOR

        // need to take the gpt coordinates and comopute screen pixels

        UIWIDGETLISTCONTAINER* container = (UIWIDGETLISTCONTAINER*) widget;
        // normalized top left
        float ux0 = container->gpt0x;
        float uy0 = container->gpt0y;
        float ux1 = container->gpt1x;
        float uy1 = container->gpt1y;

        // pixel bottom left
        uy0 = 1.0f - uy0;
        uy1 = 1.0f - uy1;

        XDWORD w = uimanager->GetRealWidth();
        XDWORD h = uimanager->GetRealHeight();


        int x0 = (int)(ux0*w); // but ux is float, need to have it on "pixels"
        int y0 = (int)(uy0*h);

        int x1 = (int)(ux1*w);
        int y1 = (int)(uy1*h);

        PushScissor(x0, y1, x1, y0);
      }
      break;
    case UI_WIDGET_TYPE_VIDEO:
      {
        UIWIDGETVIDEO* videowidget = (UIWIDGETVIDEO*)widget;
        UIGRPOBJECTBUILDER::PROPERTIES prop;
        prop.cornerradious = 0;

        GRPELEMENT* element = objectbuilder->GetElement(prop);
        if(!element) return false;

        // set the node properties and the corresponding image(where to save/load the texture?), texture manager should save them by name

        // try to get the material, if none exists create it and get it then
        GRPMATERIAL* material = GRPMATERIALLIBRARY::Get()->FindByName(__L("VideoRender")); // should be already created

        element->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"VID"));
        element->GetNode()->SetMaterial(material);

        // draw the element(setup node size, position, etc...)
        float scalex = videowidget->gpt1x - videowidget->gpt0x;
        float scaley = videowidget->gpt1y - videowidget->gpt0y;

        scalex *= uimanager->GetRealWidth(); // need to take from viewport
        scaley *= uimanager->GetRealWidth();

        // need to reconsider this viewport
        GRPPOINT pt = GetScene()->GetViewport()->GetViewPortFromInverseNormalized(videowidget->gpt0x, videowidget->gpt0y, 0.0f);
        scaley /= GetScene()->GetViewport()->GetAspectRatio();

        pt.x *= uimanager->GetRealWidth(); // need to take from viewport
        pt.y *= uimanager->GetRealWidth();

        float x = pt.x + scalex;
        float y = pt.y - scaley;

        bool update = true;

        if(videowidget->GetRotationZ() != 0.0f)
          {
            // something fails here
            float vx = x - pt.x;
            float vy = y - pt.y;

            vx = scalex;
            vy = scaley;

            // compute new position
            float fx = (float)(vx*cos(DEGREE2RADIAN(videowidget->GetRotationZ())) - vy*sin(DEGREE2RADIAN(videowidget->GetRotationZ())));
            float fy = (float)(vx*sin(DEGREE2RADIAN(videowidget->GetRotationZ())) + vy*cos(DEGREE2RADIAN(videowidget->GetRotationZ())));

            x = pt.x + fx;
            y = pt.y - fy;

            ////x = pt.x + fx;
            ////y = pt.y + fy;
            //x = pt.x + vx;
            //y = pt.y + vy;
          }

        //imageobject->GetNode()->SetPosition(x, y, widgetlevel + 0.1f);
        //imageobject->GetNode()->SetScale(scalex, scaley, 1.0f);

        // no real need to widgetlevel, as draw will be performen hierarchically
        element->GetNode()->SetPosition(x, y, 0);
        element->GetNode()->SetScale(scalex, scaley, 1.0f);
        element->GetNode()->SetRotation(-videowidget->GetRotationX(), -videowidget->GetRotationY(), -videowidget->GetRotationZ());
        element->GetNode()->Update();
        element->GetNode()->UpdateWorldMatrix(&update);

        // compute matrixes
        GRPCAMERA* camera = GetScene()->GetCamera();
        camera->BuildLookAtMatrix();

        this->viewmatrix = &camera->viewmatrix;

        //this->projectionmatrix  = *viewport;//*(holderscene->GetViewports()->Get(holderscene->GetCurrentViewport()));
        this->projectionmatrix = *GetScene()->GetViewport();//*(holderscene->GetViewports()->Get(holderscene->GetCurrentViewport()));
        this->viewprojection.GetMultiplicationMatrix(this->viewmatrix,this->projectionmatrix);  //get view*projection

        element->ComputeMatrices(viewmatrix,&viewprojection,projectionmatrix);
        GRPSHADER::UseShader(element->GetProgram());
        element->SetBlending(true);
        element->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
        element->Draw(&camera->viewmatrix,&viewprojection,projectionmatrix);
      }
      break;
    case UI_WIDGET_TYPE_TAB:
      break;
    case UI_WIDGET_TYPE_LAYOUT:
      break;
    case UI_WIDGET_TYPE_SCISSORCONTAINER:
      {
        UIWIDGETSCISSORCONTAINER* scissorcontainer = (UIWIDGETSCISSORCONTAINER*)widget;

        // normalized top left
        float ux0 = scissorcontainer->gpt0x;
        float uy0 = scissorcontainer->gpt0y;
        float ux1 = scissorcontainer->gpt1x;
        float uy1 = scissorcontainer->gpt1y;

        // pixel bottom left
        uy0 = 1.0f - uy0;
        uy1 = 1.0f - uy1;

        XDWORD w = uimanager->GetRealWidth();
        XDWORD h = uimanager->GetRealHeight();


        int x0 = (int)(ux0*w); // but ux is float, need to have it on "pixels"
        int y0 = (int)(uy0*h);

        int x1 = (int)(ux1*w);
        int y1 = (int)(uy1*h);

        PushScissor(x0, y1, x1, y0);
      }
      break;
    case UI_WIDGET_TYPE_ZOOM:
      {
        UIWIDGETZOOMCONTAINER* z = (UIWIDGETZOOMCONTAINER*)widget;

        PushZoom(z->GetZoom(), z->gptzx, z->gptzy); // but these are not global
      }
      break;
    case UI_WIDGET_TYPE_3DMODEL:
      {
        UIWIDGET3DMODEL* modelwidget = (UIWIDGET3DMODEL*)widget;

        UIGRPOBJECTBUILDER::OBJECTENTRY* entry = objectbuilder->Get3DObject(modelwidget->Get3DModel()->Get());

        GRPOBJECT* obj = entry->obj;
        if(!obj)
          {
            return false;
          }

        GRPBB* bb = entry->bb;
        if(!bb)
          {
            return false;
          }

        // compute the BB from own info
        GRPELEMENT* e = obj->GetElements()->Get(0);
        XDWORD size = e->GetMesh()->GetNvertexData();
        float maxw = 0.0f;
        float maxh = 0.0f;
        if(size)
          {
            GRPVERTEX* vertex = e->GetMesh()->GetVertexData();
            float maxx = vertex[0].position[0];
            float maxy = vertex[0].position[1];
            float maxz = vertex[0].position[2];
            float minx = vertex[0].position[0];
            float miny = vertex[0].position[1];
            float minz = vertex[0].position[2];
            for(XDWORD i = 0; i < size; i++)
              {
                GRPVERTEX v = vertex[i];
                maxx = __MAX(maxx, v.position[0]);
                maxy = __MAX(maxy, v.position[1]);
                maxz = __MAX(maxz, v.position[2]);
                minx = __MIN(minx, v.position[0]);
                miny = __MIN(miny, v.position[1]);
                minz = __MIN(minz, v.position[2]);
              }
            maxw = maxx - minx;
            maxh = maxy - miny;
          }

        //bb->width = maxw;
        //bb->height = maxh;

        // draw the element(setup node size, position, etc...)
        float scalex = modelwidget->gpt1x - modelwidget->gpt0x;
        float scaley = modelwidget->gpt1y - modelwidget->gpt0y;

        ApplyZoom(modelwidget->gpt0x, modelwidget->gpt0y, modelwidget->gpt1x, modelwidget->gpt1y, scalex, scaley);

        //float bbheight = maxw;
        //float sc = uimanager->GetRealHeight()/bbheight;
        //float rscaley = modelwidget->gpt1y - modelwidget->gpt0y;

        // not really ok, how are nodes going to affect it?
        //bb = e->GetBoundingBox();

        //--------------------------------------------------------------------------------------------
        // must check if the material has layers, if it does we must use the texture shader("STX")
        obj->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));



        scalex *= uimanager->GetRealWidth(); // need to take from viewport
        scaley *= uimanager->GetRealWidth();

        // need to reconsider this viewport
        // careful, as takes for granted a -1.0 to 1.0 viewport
        GRPPOINT pt = GetScene()->GetViewport()->GetViewPortFromInverseNormalized(modelwidget->gpt0x, modelwidget->gpt0y, 0.0f);
        scaley /= GetScene()->GetViewport()->GetAspectRatio();

        pt.x *= uimanager->GetRealWidth();
        pt.y *= uimanager->GetRealWidth();

        float x = pt.x + scalex;
        float y = pt.y - scaley;

        bool update = true;

        //if(modelwidget->GetRotationZ() != 0.0f)
        //  {
        //    // something fails here
        //    float vx = x - pt.x;
        //    float vy = y - pt.y;
        //
        //    vx = scalex;
        //    vy = scaley;
        //
        //    // compute new position
        //    float fx = vx*cos(DEGREE2RADIAN(modelwidget->GetRotationZ())) - vy*sin(DEGREE2RADIAN(modelwidget->GetRotationZ()));
        //    float fy = vx*sin(DEGREE2RADIAN(modelwidget->GetRotationZ())) + vy*cos(DEGREE2RADIAN(modelwidget->GetRotationZ()));
        //
        //    x = pt.x + fx;
        //    y = pt.y - fy;
        //  }

        if(modelwidget->GetRotationZ() != 0.0f)
          {
            // something fails here
            float vx = x - pt.x;
            float vy = y - pt.y;

            vx = scalex;
            vy = scaley;

            float px = -(((modelwidget->GetRotationPivotX())/(float)modelwidget->GetWidth())*scalex);
            float py = -(((modelwidget->GetRotationPivotY())/(float)modelwidget->GetHeight())*scaley);

            vx = scalex + px*2;
            vy = scaley + py*2;

            // compute new position
            float fx = (float)(vx*cos(DEGREE2RADIAN(modelwidget->GetRotationZ())) - vy*sin(DEGREE2RADIAN(modelwidget->GetRotationZ())));
            float fy = (float)(vx*sin(DEGREE2RADIAN(modelwidget->GetRotationZ())) + vy*cos(DEGREE2RADIAN(modelwidget->GetRotationZ())));

            x = pt.x + fx;
            y = pt.y - fy;

            // px and x are on two different coordinates system
            x -= px*2;
            y += py*2;
          }

        if(bb)
          {
            float widgetaspect = modelwidget->adjustedwidth/(float)modelwidget->adjustedheight;
            float modelaspect = bb->width/bb->height;

            // here we compute the actual value of maxsize
            float maxsize = bb->width;
            if(fabsf(modelaspect-widgetaspect) > EPSILON)
              {
                if(modelaspect > widgetaspect)
                  {
                    maxsize = bb->width;
                  }
                else
                  {
                    maxsize = bb->height;
                  }
              }
            else
              {
              }

            float oldmaxsize = maxsize;
            //float yoffset = 0.0; // need to compute offsets for when the model is not really centered
            float yoffset = 0.0f;
            //if(bb->minpoint.y  < 0)
            //  {
            //    yoffset = bb->minpoint.y + bb->height/2.0f;
            //  }
            //else
            //  {
            //    yoffset = bb->minpoint.y - bb->height/2.0f;
            //  }
            //yoffset /= oldmaxsize;
            //
            //yoffset *= scaley;

            if(bb->height > bb->width) // guess, which is incorrect, we should be seeing which dimension doesn't fit on the widget area and fit it from there
              {
                maxsize /= uimanager->GetRealHeight();
                yoffset /= uimanager->GetRealWidth();
              }
            else
              {
                maxsize /= uimanager->GetRealWidth();
                yoffset /= uimanager->GetRealHeight();
              }

            // this yoffset doesn't seem to be properly computed, but the idea seems valid
            obj->GetNode()->SetPosition(x + (scalex*maxsize)/2, y + (scaley*(maxsize-(yoffset/2)))/2, 0);

            //obj->GetNode()->SetScale(scalex*(1/oldmaxsize)*2, scalex*(1/oldmaxsize)*2, maxsize);
            obj->GetNode()->SetScale(scalex*(1/oldmaxsize)*2, scalex*(1/oldmaxsize)*2, maxsize);


            //obj->GetNode()->SetScale(2.0f*sc*rscaley, 2.0f*sc*rscaley, maxsize);
            //obj->GetNode()->SetScale(2.0f*sc, 2.0f*sc, maxsize);

            obj->GetNode()->SetRotation(0, 0, modelwidget->GetRotationZ()); // XY is the ground plane, z UP(towards camera)
            obj->GetNode()->Update();
            obj->GetNode()->UpdateWorldMatrix(&update);

            // compute matrixes
            GRPCAMERA* camera = GetScene()->GetCamera();
            camera->BuildLookAtMatrix();

            this->viewmatrix = &camera->viewmatrix;

            this->projectionmatrix = *GetScene()->GetViewport();
            this->viewprojection.GetMultiplicationMatrix(this->viewmatrix,this->projectionmatrix);

            XDWORD size = obj->GetElements()->GetSize();
            // yay and here iterate over all object elements?
            for(XDWORD i = 0; i < size; i++)
              {
                GRPELEMENT* element = obj->GetElements()->FastGet(i);
                element->UpdateBoundingBox();

                element->GetNode()->Update();
                bool update = true;
                element->GetNode()->UpdateWorldMatrix(&update);
                element->ComputeMatrices(viewmatrix,&viewprojection,projectionmatrix);
                GRPSHADER::UseShader(element->GetProgram());
                element->SetBlending(true);
                element->SetBlendingMode(GRPBLENDINGMODE_ALPHABLEND);
                element->Draw(&camera->viewmatrix,&viewprojection,projectionmatrix);
              }
          }
      }
      break;
    default:
      return false;
  }

  return true;
}




/*-------------------------------------------------------------------
//  UISTYLEDRAWER::PostDraw
*/
/**
//
//  to execute after all the children have been drawn
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/08/2016 12:54:05
//
//  @return       bool :
//
//  @param        widget :
*/
/*-----------------------------------------------------------------*/
bool UISTYLEDRAWER::PostDraw(UIWIDGETCONTAINER* widget)
{
  switch(widget->GetType())
    {
      case UI_WIDGET_TYPE_LIST:
        {
          PopScissor();
        }
      break;
      case UI_WIDGET_TYPE_SCISSORCONTAINER:
        {
          PopScissor();
        }
      break;
      case UI_WIDGET_TYPE_ZOOM:
        {
          PopZoom();
        }
      break;
    }

  return true;
}




/*-------------------------------------------------------------------
//  BuildDebugLine
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/06/2017 16:57:01
//
//  @return       GRPELEMENT* :
//
//  @param        p0 :
//  @param        p1 :
*/
/*-----------------------------------------------------------------*/
GRPELEMENT* UISTYLEDRAWER::BuildDebugLine(GRPPOINT p0, GRPPOINT p1)
{
  GRPELEMENT* element = new GRPELEMENT();
  GRPMESH* mesh = new GRPMESH();

  element->SetMesh(mesh);

  int n = 4;
  element->GetMesh()->indexdata   = new XWORD[n];
  if (element->GetMesh()->indexdata==NULL)
    {
      delete element;
      return NULL;
    }

  element->GetMesh()->nindexdata  = n;

  element->GetMesh()->vertexdata  = new GRPVERTEX[n];
  if (element->GetMesh()->indexdata==NULL)
    {
      delete element->GetMesh()->indexdata;
      delete element;
      return NULL;
    }

  element->GetMesh()->nvertexdata = n;
  memset(element->GetMesh()->vertexdata,0,sizeof(GRPVERTEX)*n);

  if (element->GetMesh()->indexdata==NULL || element->GetMesh()->vertexdata==NULL)
    {
      XDEBUG_PRINTCOLOR(4,__L("Not enought memory"));
      return NULL;
    }

  GRPVERTEX* vdata = element->GetMesh()->GetVertexData();
  memset(vdata,0,sizeof(GRPVERTEX)*n);

  element->GetMesh()->vertexdata[0].position[0]=p0.x;
  element->GetMesh()->vertexdata[0].position[1]=p0.y;
  element->GetMesh()->vertexdata[0].position[2]=p0.z;
  element->GetMesh()->indexdata[0]=0;

  element->GetMesh()->vertexdata[1].position[0]=p1.x;
  element->GetMesh()->vertexdata[1].position[1]=p1.y;
  element->GetMesh()->vertexdata[1].position[2]=p1.z;
  element->GetMesh()->indexdata[1]=1;

  element->SetDefaultColor(GRPCOLOR(255, 0, 233));

  element->SetRenderMode(GRPRENDERMODE_LINES);
  element->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"PNK"));

  return element;
}




/*-------------------------------------------------------------------
//  UISTYLEDRAWER::DeleteDebugLine
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/06/2017 16:58:58
//
//  @param        element :
*/
/*-----------------------------------------------------------------*/
void UISTYLEDRAWER::DeleteDebugLine(GRPELEMENT* element)
{
  delete element; // enough? not sure
}


/*-------------------------------------------------------------------
//  UISTYLEDRAWER::PushScissor
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/09/2016 11:14:26
//
//  @param        x0 :
//  @param        y0 :
//  @param        x1 :
//  @param        y1 :
*/
/*-----------------------------------------------------------------*/
void UISTYLEDRAWER::PushScissor(int& x0, int& y0, int& x1, int& y1, bool constrain)
{
  if(scissorstack.GetSize())
    {
      if(constrain)
        {
          UISCISSOR parentscissor = scissorstack.GetLast();
          x0 = __MAX(x0, parentscissor.x0);
          y0 = __MAX(y0, parentscissor.y0);
          x1 = __MIN(x1, parentscissor.x1);
          y1 = __MIN(y1, parentscissor.y1);
        }
    }

  GRPOPENGLCTRL::SCISSOR::SetScissor(true, x0, y0, x1, y1);
  // push to the stack
  UISCISSOR scissor;
  scissor.x0 = x0;
  scissor.y0 = y0;
  scissor.x1 = x1;
  scissor.y1 = y1;
  scissor.constrain = constrain;

  scissorstack.Add(scissor);
}




/*-------------------------------------------------------------------
//  UISTYLEDRAWER::PopScissor
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      13/09/2016 11:14:33
//
*/
/*-----------------------------------------------------------------*/
void UISTYLEDRAWER::PopScissor()
{
  scissorstack.DeleteLast();

  // if any element left in the scissor list, pop the value(separate common function?)
  if(scissorstack.GetSize())
    {
      UISCISSOR scissor = scissorstack.GetLast();
      GRPOPENGLCTRL::SCISSOR::SetScissor(true, scissor.x0, scissor.y0, scissor.x1, scissor.y1);
    }
  else
    {
      GRPOPENGLCTRL::SCISSOR::SetScissor(false, 0, 0, 0, 0);
    }
}



/*-------------------------------------------------------------------
//  UISTYLEDRAWER::PushZoom
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/03/2017 10:36:23
//
//  @param        zoom :
//  @param        zoomx :
//  @param        zoomy :
*/
/*-----------------------------------------------------------------*/
void UISTYLEDRAWER::PushZoom(float zoom, float zoomx, float zoomy)
{
  UIZOOM z;
  z.zoom  = zoom;
  z.zoomx = zoomx;
  z.zoomy = zoomy;

  zoomstack.Add(z);
}



/*-------------------------------------------------------------------
//  UISTYLEDRAWER::PopZoom
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/03/2017 10:36:30
//
*/
/*-----------------------------------------------------------------*/
void UISTYLEDRAWER::PopZoom()
{
  zoomstack.DeleteLast();
}


/*-------------------------------------------------------------------
//  UISTYLEDRAWER::RecursiveDraw
*/
/**
//
//  recursive draw function
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      10/08/2016 12:54:18
//
//  @param        widget :
*/
/*-----------------------------------------------------------------*/
void UISTYLEDRAWER::RecursiveDraw(UIWIDGETCONTAINER* widget)
{
  Draw(widget);

  XVECTOR<UIWIDGET*>* children = widget->GetAllChildren();
  XDWORD size = children->GetSize();

  for(XDWORD i = 0; i < size; i++)
    {
      UIWIDGETCONTAINER* w = (UIWIDGETCONTAINER*)children->FastGet(i);
      if(w->GetIsActive())
      {
        this->RecursiveDraw(w);
      }
    }

  PostDraw(widget);
}



/*-------------------------------------------------------------------
//  UISTYLEDRAWER::RecursiveDebugDraw
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      14/06/2017 11:01:16
//
//  @param        widget :
*/
/*-----------------------------------------------------------------*/
void UISTYLEDRAWER::RecursiveDebugDraw(UIWIDGETCONTAINER* widget)
{
  // render required debug lines in here if required
  GRPCAMERA* camera = GetScene()->GetCamera();
  camera->BuildLookAtMatrix();

  this->viewmatrix = &camera->viewmatrix;

  this->projectionmatrix = *GetScene()->GetViewport();
  this->viewprojection.GetMultiplicationMatrix(this->viewmatrix,this->projectionmatrix);



  // render the 4 sides of the bounding box

  GRPPOINT desiredsize = widget->GetDesiredSize();

  //float ux = widget->GetPositionX()/(float)uimanager->GetVirtualWidth();
  //float uy = widget->GetPositionY()/(float)uimanager->GetVirtualHeight();
  float ux = widget->gpt0x;
  float uy = widget->gpt0y;
  float uw = desiredsize.x/(float)uimanager->GetVirtualWidth();
  float uh = desiredsize.y/(float)uimanager->GetVirtualHeight();

  ux = ux - 0.5f;
  uy = -uy + 0.5f;

  ux *= 2.0f;
  uy *= 2.0f;
  uw *= 2.0f;
  uh *= 2.0f;

  GRPPOINT p0, p1;
  // upper horizontal
  p0.x = ux*uimanager->GetRealWidth();
  p0.y = uy*uimanager->GetRealHeight();
  p1.x = (ux+uw)*uimanager->GetRealWidth();
  p1.y = (uy)*uimanager->GetRealHeight();
  DrawDebugLine(p0, p1);

  // lower horizontal
  p0.x = ux*uimanager->GetRealWidth();
  p0.y = (uy-uh)*uimanager->GetRealHeight();
  p1.x = (ux+uw)*uimanager->GetRealWidth();
  p1.y = (uy-uh)*uimanager->GetRealHeight();
  DrawDebugLine(p0, p1);

  // left vertical
  p0.x = ux*uimanager->GetRealWidth();
  p0.y = uy*uimanager->GetRealHeight();
  p1.x = (ux)*uimanager->GetRealWidth();
  p1.y = (uy-uh)*uimanager->GetRealHeight();
  DrawDebugLine(p0, p1);

  // right vertical
  p0.x = (ux+uw)*uimanager->GetRealWidth();
  p0.y = (uy-uh)*uimanager->GetRealHeight();
  p1.x = (ux+uw)*uimanager->GetRealWidth();
  p1.y = (uy)*uimanager->GetRealHeight();
  DrawDebugLine(p0, p1);

  //// diagonal
  //p0.x = (ux)*uimanager->GetRealWidth();
  //p0.y = (uy)*uimanager->GetRealHeight();
  //p1.x = (ux+uw)*uimanager->GetRealWidth();
  //p1.y = (uy-uh)*uimanager->GetRealHeight();
  //DrawDebugLine(p0, p1);


  // if we were a model widget, render the bounding box
  if(widget->GetType() == UI_WIDGET_TYPE_3DMODEL)
    {
      UIWIDGET3DMODEL* modelwidget = (UIWIDGET3DMODEL*)widget;

      UIGRPOBJECTBUILDER::OBJECTENTRY* entry = objectbuilder->Get3DObject(modelwidget->Get3DModel()->Get());

      GRPOBJECT* obj = entry->obj;
      GRPBB* bb = entry->bb;
      if(obj && bb)
        {
          // compute the BB from own info
          GRPELEMENT* e = obj->GetElements()->Get(0);
          XDWORD size = e->GetMesh()->GetNvertexData();
          float maxw = 0.0f;
          float maxh = 0.0f;
          float maxx = 0.0f;
          float maxy = 0.0f;
          float maxz = 0.0f;
          float minx = 0.0f;
          float miny = 0.0f;
          float minz = 0.0f;
          if(size)
            {
              GRPVERTEX* vertex = e->GetMesh()->GetVertexData();
              maxx = vertex[0].position[0];
              maxy = vertex[0].position[1];
              maxz = vertex[0].position[2];
              minx = vertex[0].position[0];
              miny = vertex[0].position[1];
              minz = vertex[0].position[2];
              for(XDWORD i = 0; i < size; i++)
                {
                  GRPVERTEX v = vertex[i];
                  maxx = __MAX(maxx, v.position[0]);
                  maxy = __MAX(maxy, v.position[1]);
                  maxz = __MAX(maxz, v.position[2]);
                  minx = __MIN(minx, v.position[0]);
                  miny = __MIN(miny, v.position[1]);
                  minz = __MIN(minz, v.position[2]);
                }
              maxw = maxx - minx;
              maxh = maxy - miny;

              // draw out the box on here
              // need to convert into gpt coordinates !!
              // this means applying the same transformations as the actual model

            }

          bb->width = maxw;
          bb->height = maxh;
      }
    }

  XVECTOR<UIWIDGET*>* children = widget->GetAllChildren();
  XDWORD size = children->GetSize();

  for(XDWORD i = 0; i < size; i++)
    {
      UIWIDGETCONTAINER* w = (UIWIDGETCONTAINER*)children->FastGet(i);
      if(w->GetIsActive())
      {
        this->RecursiveDebugDraw(w);
      }
    }
}



/*-------------------------------------------------------------------
//  UISTYLEDRAWER::DrawDebugLine
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      14/06/2017 10:56:33
//
//  @param        p0 :
//  @param        p1 :
*/
/*-----------------------------------------------------------------*/
void UISTYLEDRAWER::DrawDebugLine(GRPPOINT p0, GRPPOINT p1)
{
  GRPELEMENT* element = BuildDebugLine(p0, p1);
  element->SetNode(GetScene()->CreateNode());
  element->GetNode()->Update();
  element->GetNode()->UpdateWorldMatrix();
  GRPSHADER::UseShader(element->GetProgram());
  element->ComputeMatrices(viewmatrix,&viewprojection,projectionmatrix);
  element->Draw(&camera->viewmatrix,&viewprojection,projectionmatrix);
  delete element->GetNode();
  element->SetNode(NULL);
  DeleteDebugLine(element);
}


/*-------------------------------------------------------------------
//  UISTYLEDRAWER::ApplyZoom
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      08/03/2017 11:41:44
//
//  @param        pt0x :
//  @param        pt0y :
//  @param        pt1x :
//  @param        pt1y :
//  @param        scalex :
//  @param        scaley :
*/
/*-----------------------------------------------------------------*/
void UISTYLEDRAWER::ApplyZoom(float& pt0x, float& pt0y, float& pt1x, float& pt1y, float& scalex, float& scaley)
{
  if(zoomstack.GetSize())
    {
      UIZOOM z = zoomstack.GetLast(); // shouldn't just egt the last one, but some kind of "apply" to get the final zoom level

      // zoom: zoom value
      // zoomx: zoom offsetz
      // zoomy: zoom offsety

      // move object by according zoom
      float cx = pt0x;
      float cy = pt0y;
      float vx = cx - z.zoomx;
      float vy = cy - z.zoomy;

      vx *= z.zoom;
      vy *= z.zoom;
      cx = z.zoomx + vx;
      cy = z.zoomy + vy;

      scalex *= z.zoom;
      scaley *= z.zoom;
      // recompute corners
      pt0x = cx;
      pt0y = cy;
      pt1x = cx + scalex;
      pt1y = cy + scaley;
    }
}
