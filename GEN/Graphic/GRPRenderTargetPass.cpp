/*------------------------------------------------------------------------------------------
// GRPRENDERTARGETPASS.CPP
*/
/**
// \class
//
// Render target pass
//
//  @author: Diego Martinez Ruiz de Gaona
//
//  Date of Creation : 23/11/2017 14:55:40
//  Last Modification :
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPRenderTarget.h"
#include "GRPRenderTargetPass.h"
#include "GRPMaterialLibrary.h"
#include "GRPFactory.h"
#include "GRPScene.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  GRPRENDERTARGETPASS::GRPRENDERTARGETPASS
*/
/**
//
//  constructor
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  23/11/2017 14:57:28
//  @return    :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
GRPRENDERTARGETPASS::GRPRENDERTARGETPASS(): GRPPASS(__L("offscreen"))
{
    this->SetClear(GRPPASS_CLEAR_FLAG_ALL);
    this->SetClone(true);
}

GRPTEXTURE* GRPRENDERTARGETPASS::GetTexture()
{
    return this->texture;
}

GRPMATERIAL* GRPRENDERTARGETPASS::GetMaterial()
{
    return this->material;
}

void GRPRENDERTARGETPASS::SetViewport(GRPVIEWPORT*  viewport)
{
    this->viewport = viewport;

    GLFLOAT w, h;

    this->viewport->GetRealWindowSize(w, h);

    GRPBITMAP* bitmap = grpfactory->CreateBitmap((int)w, (int)h, GRPPROPERTYMODE_32_RGBA_8888);

    texture = new GRPTEXTURE();
    texture->Create(bitmap);
    texture->SetName(__L("RenderTarget"));

    this->material = GRPMATERIALLIBRARY::Get()->Create(texture);
}

/*-------------------------------------------------------------------
//  GRPRENDERTARGETPASS::~GRPRENDERTARGETPASS
*/
/**
//
//  destructor
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  23/11/2017 14:57:33
//  @return    :
//
//  @param    ) :
//
*//*-----------------------------------------------------------------*/
GRPRENDERTARGETPASS::~GRPRENDERTARGETPASS()
{

}

/*-------------------------------------------------------------------
//   GRPRENDERTARGETPASS::OnAdded
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  23/11/2017 14:57:37
//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool GRPRENDERTARGETPASS::OnAdded()
{
    this->viewport = this->holderscene->GetViewport();
    CreateRenderTarget();
    return true;
}

/*-------------------------------------------------------------------
//   GRPRENDERTARGETPASS::Begin
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  23/11/2017 15:59:32
//  @return   bool :
//
//  @param    ) :
//
*//*-----------------------------------------------------------------*/
void GRPRENDERTARGETPASS::Begin()
{
    this->DisableStencil();
    this->GetRenderTarget()->SelectAsTarget();
    this->Clear();
    GRPPASS::Begin();
};

void GRPRENDERTARGETPASS::End()
{
    this->GetRenderTarget()->Deselect();
    GRPPASS::End();
};