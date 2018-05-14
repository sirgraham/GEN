/*------------------------------------------------------------------------------------------
// GRPWIDGETPASS.CPP
*/
/**
// \class
//
// Manages interface pass
//
//  @author: Diego Martinez Ruiz de Gaona
//
//  Date of Creation : 12/12/2017 11:23:46
//  Last Modification :
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPWidgetPass.h"
#include "GRPWidgetPage.h"
#include "GRPScene.h"
#include "GRPScreen.h"
#include "GRPWidgetPass.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  GRPWIDGETPASS::GRPWIDGETPASS
*/
/**
//
//  Class Constructor GRPWIDGETPASS
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/08/2017 13:30:14
//
*/
/*-----------------------------------------------------------------*/
GRPWIDGETPASS::GRPWIDGETPASS() : GRPPASS(__L("UI"))
{

    this->active_page               = 0;
    this->type                      = GRPPASS_TYPE_DEPTHTEST  |
                                      GRPPASS_TYPE_DEPTHFUNC  |
                                      GRPPASS_TYPE_CULLFACE   |
                                      GRPPASS_TYPE_ALPHABLEND;

    this->AlphaBlendingIsActive     = true;
    this->AlphaBlendingMode         = GRPBLENDINGMODE_ALPHABLEND;

    this->DepthTestIsActive         = false;
    this->DepthFunction             = GRPDEPTHFUNC_LESS_OR_EQUAL;

    this->BackFaceCullingIsActive   = false;
    this->hasToClear                = GRPPASS_CLEAR_FLAG_ALL;

    this->clearcolor                = GRPCOLOR::WHITE;

}

/*-------------------------------------------------------------------
//  GRPWIDGETPASS::~GRPWIDGETPASS
*/
/**
//
//   Class Destructor GRPWIDGETPASS
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      04/09/2017 10:28:41
//
*/
/*-----------------------------------------------------------------*/
GRPWIDGETPASS::~GRPWIDGETPASS()
{
    pages.DeleteContentsInstanced();
    pages.DeleteAll();
}

/*-------------------------------------------------------------------
//  GRPWIDGETPASS::GetPage
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/08/2017 13:31:02
//
//  @return       GRPWIDGETPAGE* :
//
*/
/*-----------------------------------------------------------------*/
GRPWIDGETPAGE* GRPWIDGETPASS::GetPage()
{
    return pages.Get(active_page);
}

/*-------------------------------------------------------------------
//   GRPWIDGETPASS::SetCurrentPage
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  22/02/2018 13:53:56
//  @return   void :
//
//  @param    XDWORD :
//
*//*-----------------------------------------------------------------*/
void GRPWIDGETPASS::SetCurrentPage(XDWORD page)
{
    if (page < this->pages.GetSize())
        this->active_page = page;
}



/*-------------------------------------------------------------------
//  GRPWIDGETPASS::DrawRenderOrder
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      18/08/2017 12:55:00
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGETPASS::DrawRenderOrder()
{
    GRPWIDGETPAGE* page = this->GetPage();
    if (page)
    {

        page->Update();

        page->root->Align();

        for (XDWORD e = 0; e < this->GetScene()->GetNodes()->GetSize(); e++)
        {
            this->GetScene()->GetNodes()->Get(e)->Update();
            this->GetScene()->GetNodes()->Get(e)->UpdateWorldMatrix();
        }

        if (page->root)
            page->root->Draw(viewmatrix, &viewprojection, projectionmatrix);
    }
    return true;
}

/*-------------------------------------------------------------------
//   GRPWIDGETPASS::SetScale
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  01/02/2018 11:51:11
//  @return   void :
//
//  @param    float :
//
*//*-----------------------------------------------------------------*/
void GRPWIDGETPASS::SetScale(float sc)
{
    this->width = float(this->holderscene->GetScreen()->GetWidth());
    this->height = float(this->holderscene->GetScreen()->GetHeight());
    this->scale = sc;
    viewport->SetWindowSize(width, height);
    viewport->SelectProjectionPixels(sc);
}

/*-------------------------------------------------------------------
//   GRPWIDGETPASS::GetScale
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  14/02/2018 13:18:38
//  @return   float :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
float GRPWIDGETPASS::GetScale()
{
    return this->scale;
}

/*-------------------------------------------------------------------
//   GRPWIDGETPASS::GetScreen
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  07/02/2018 10:29:48
//  @return   GRPSCREEN* :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
GRPSCREEN* GRPWIDGETPASS::GetScreen()
{
    return screen;
}

/*-------------------------------------------------------------------
//   GRPWIDGETPASS::GetPages
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  14/02/2018 14:09:59
//  @return   XVECTOR<GRPWIDGETPAGE*>* :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
XVECTOR<GRPWIDGETPAGE*>* GRPWIDGETPASS::GetPages()
{
    return &this->pages;
}

/*-------------------------------------------------------------------
//  GRPWIDGETPASS::AddPage
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/08/2017 14:40:24
//
//  @return       GRPWIDGETPAGE* :
//
*/
/*-----------------------------------------------------------------*/
GRPWIDGETPAGE* GRPWIDGETPASS::AddPage(XCHAR* toload)
{
    GRPWIDGETPAGE*  page = new GRPWIDGETPAGEHTML(toload);
                    pages.Add(page);
                    page->ownerpass = this;
    return    page;
}

/*-------------------------------------------------------------------
//  GRPWIDGETPASS::OnAdded
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      17/08/2017 14:31:38
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPWIDGETPASS::OnAdded()
{

    if (!camera)
    CreateCamera();

    if (!viewport)
    CreateViewport();

    screen = this->holderscene->GetScreen();

    GRPVIEWPORT*    view = viewport;
                    view->SetRealScreen(screen);

                    SetScale(1.0f);

    GRPCAMERA*      cam = camera;
                    cam->SetViewport(view);
                    cam->SetPosition(0.0, 0.0, 1.0);

    for (XDWORD e=0;e<this->pages.GetSize();e++)
        if (!this->pages.Get(e)->Load())
            return false;




    return true;
}

bool GRPWIDGETPASS::Add(INPDEVICE* device)
{
    return true;
}



