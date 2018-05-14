/*------------------------------------------------------------------------------------------
// GRPVIEWPORTPASS.CPP
*/
/**
// \class
//
// Manages viewport renderers
//
//  @author: Diego Martinez Ruiz de Gaona
//
//  Date of Creation : 11/10/2017 17:46:26
//  Last Modification :
*/
/* (C) Copyright . All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPViewportPass.h"
#include "GRPScene.h"
#include "GRPViewport.h"
#include "GRPCamera.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//  GRPVIEWPORTPASS::GRPVIEWPORTPASS
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
GRPVIEWPORTPASS::GRPVIEWPORTPASS(XCHAR* name): GRPPASS(name)
{
    Clean();
    this->mustClone = true;
    hasToClear = GRPPASS_CLEAR_FLAG_DEPTH;
}

/*-------------------------------------------------------------------
//  GRPVIEWPORTPASS::~GRPVIEWPORTPASS
*/
/**
//
//  destructor
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return    :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
GRPVIEWPORTPASS::~GRPVIEWPORTPASS()
{

}

/*-------------------------------------------------------------------
//   GRPVIEWPORTPASS::OnAdded
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  16/10/2017 13:23:22
//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool GRPVIEWPORTPASS::OnAdded()
{
    this->CreateViewport    ();                                     OPENGLCHECKERROR(__L("GRPPASS::CreateViewport"));

    viewport->SetProjection (GRPPROJECTIONTYPE_FRUSTUM);            OPENGLCHECKERROR(__L("GRPPASS::SetProjection"));
    viewport->SetPosition   (rect.x1,rect.y1);                      OPENGLCHECKERROR(__L("GRPPASS::SetPosition"));
    viewport->SetSize       (rect.x2- rect.x1, rect.y2-rect.y1);    OPENGLCHECKERROR(__L("GRPPASS::SetSize"));
    //viewport->Select        ();                                     OPENGLCHECKERROR(__L("GRPPASS::Select"));

    GLFLOAT w, h;
    viewport->GetRealWindowSize(w, h);

//    this->AddType           (GRPPASS_TYPE_SCISSOR);
//    this->SetScissor        (true, int(w*rect.x1), int(h*rect.y1), int(w*rect.x2), int(h*rect.y2));

    if (!this->camera)
        this->SetCamera     (holderscene->FindCamera(this->camera_name.Get()));

    return true;
}
void GRPVIEWPORTPASS::Begin()
{
    GLFLOAT w, h;
    viewport->GetRealWindowSize(w, h);
    //this->SetScissor(true, int(w*rect.x1), int(h*rect.y1), int(w*rect.x2), int(h*rect.y2));
    GRPPASS::Begin();
    OPENGLCHECKERROR(__L("GRPVIEWPORTPASS::Begin"));
}
void GRPVIEWPORTPASS::End()
{
    this->SetScissor(false);
    GRPPASS::ApplyFlags();
    GRPPASS::End();
}