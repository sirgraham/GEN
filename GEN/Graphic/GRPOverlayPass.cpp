/*------------------------------------------------------------------------------------------
//  GRPOVERLAYPASS.CPP
//
//  Stencil overlay
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 13/10/2017 11:12:41
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPPass.h"
#include "GRPOverlayPass.h"
#include "GRPObjectBuilder.h"
#include "GRPMaterialLibrary.h"
#include "GRPElement.h"
#include "GRPScene.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  GRPOVERLAYPASS::GRPOVERLAYPASS
*/
/**
//
//  constructor
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return    :
//
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
GRPOVERLAYPASS::GRPOVERLAYPASS(XCHAR* name): GRPBACKGROUNDPASS(name)
{
    Clean();

    this->passtype = GRPPASS_OVERLAY;
    priority = 0;
    //this->hasToClear = GRPPASS_CLEAR_FLAG_STENCIL;
    this->hasToClear = GRPPASS_CLEAR_FLAG_NONE;

    iterations = 2;

    lightingIsEnabled = false;

    program = GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Overlay"));
}


GRPOVERLAYPASS::~GRPOVERLAYPASS()
{

}


void GRPOVERLAYPASS::Begin()
{
    switch (currentiteration)
    {
    case 0:
        this->SetDepthWritingIsActive(false);

        this->SetDepthTestIsActive(false);
        this->SetDepthFunction(GRPDEPTHFUNC_ALWAYS);

        this->SetColorWritingIsActive(true);

        this->SetBlending(true,GRPBLENDINGMODE_ALPHABLEND);

        this->type |= GRPPASS_TYPE_ALPHABLEND;
        this->type |= GRPPASS_TYPE_DEPTHFUNC;
        this->type |= GRPPASS_TYPE_DEPTHTEST;
        this->type |= GRPPASS_TYPE_COLORWRITE;

        this->hasToClear = GRPPASS_CLEAR_FLAG_NONE;

        this->plane->SetProgram(program);

        this->DisableStencil();
        this->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Mask")));
        GRPPASS::Begin();
        break;

    case 1:
        this->SetBlending(false);
        this->hasToClear = GRPPASS_CLEAR_FLAG_STENCIL;
        this->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Overlay")));
        this->SetStencilWriting();
        GRPPASS::Begin();
        break;
    }
}



/*-------------------------------------------------------------------
//   GRPOVERLAYPASS::End
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  24/10/2017 18:05:48
//  @return   void :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
void GRPOVERLAYPASS::End()
{
    switch (currentiteration)
    {
    case 0: break;
    case 1:
            this->SetDepthTestIsActive(true);
            this->SetDepthFunction(GRPDEPTHFUNC_LESS);

            this->SetStencilTesting();
            this->hasToClear = GRPPASS_CLEAR_FLAG_NONE;
            GRPPASS::Begin();
            break;
    }

}

/*-------------------------------------------------------------------
//   GRPOVERLAYPASS::OnAdded
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  24/10/2017 18:05:53
//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool GRPOVERLAYPASS::OnAdded()
{
    this->CreateViewport();

    viewport->SetProjection (GRPPROJECTIONTYPE_ORTHO);
    viewport->SetPosition   (rect.x1, rect.y1);
    viewport->SetSize       (rect.x2 - rect.x1, rect.y2 - rect.y1);
    viewport->Select        ();

    plane->SetNode          (this->holderscene->CreateNode());
    plane->SetProgram       (program);


    plane->GetNode()->SetScale(100.0);
    plane->SetName(__L("LoaderScreenPlane"));
    plane->GetNode()->SetName(__L("LoaderScreenPlaneNode"));

    if (material)
        plane->GetNode()->SetMaterial(material);
    else
        plane->GetNode()->SetMaterial(GRPMATERIALLIBRARY::Get()->FindByName(this->material_name.Get()));

    CreateCamera();

    this->Add(plane);
    return true;
}
