
/*------------------------------------------------------------------------------------------
//  GRPSTENCILPASS.CPP
//
//  Manages stenciled objects
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 07/11/2016 13:20:50
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPStencilPass.h"
#include "GRPElement.h"

#include "GRPShaderLibrary.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  GRPSTENCILPASS::GRPSTENCILPASS
*/
/**
//
//  Class Constructor GRPSTENCILPASS
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/11/2016 13:29:08
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPSTENCILPASS::GRPSTENCILPASS() : GRPPASS(__L("stencil"))
{
  Clear();
  this->priority=0;

  iterations=1;

  AddType   (GRPPASS_TYPE_STENCIL);
    AddType   (GRPPASS_TYPE_STENCIL_FUNCTION);
    AddType   (GRPPASS_TYPE_STENCIL_MASK);
    AddType   (GRPPASS_TYPE_STENCIL_OPERATION);

  AddType   (GRPPASS_TYPE_COLORWRITE);
  AddType   (GRPPASS_TYPE_DEPTHWRITE);

  SetClear  (GRPPASS_CLEAR_FLAG_NONE);

  this->shader=GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"BAS");

  this->OrderClone=false;
}



/*------------------------------------------------------------------
//  GRPSTENCILPASS::Start
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/11/2016 13:29:18
//
*/
/*-----------------------------------------------------------------*/
void GRPSTENCILPASS::Begin()
{
    //glEnable(GL_STENCIL_TEST);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    //glStencilFunc(GL_ALWAYS, 1, 0xFF);
    //glStencilMask(0xFF); // each bit is written to the stencil buffer as is

    this->SetStencilTestIsActive(true);

    this->SetStencilFailOperation     (GRPSTENCILOPERATION_KEEP);
    this->SetStencilDepthFailOperation(GRPSTENCILOPERATION_KEEP);
    this->SetStencilPassOperation     (GRPSTENCILOPERATION_REPLACE);
    this->SetStencilFunction          (GRPSTENCILFUNCTION_ALWAYS,0x01);
    this->SetStencilMask              (0xFF);
    this->SetClear                    (GRPPASS_CLEAR_FLAG_COLOR | GRPPASS_CLEAR_FLAG_DEPTH | GRPPASS_CLEAR_FLAG_STENCIL);

    this->lightingIsEnabled       =false;
    this->SetGlobalShaderOverride (true);

    this->SetDepthTestIsActive    (false);
    this->SetDepthWritingIsActive (false);
    this->SetColorWritingIsActive (false);


    return GRPPASS::Begin();

    passtype= GRPPASS_STENCIL;
}


/*-------------------------------------------------------------------
//  GRPSTENCILPASS::Finish
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/11/2016 17:56:00
//
*/
/*-----------------------------------------------------------------*/
void  GRPSTENCILPASS::Finish()
{
    //glEnable(GL_STENCIL_TEST);
    //glStencilFunc(GL_EQUAL, 1, 0xFF);
    //glStencilMask(0x00); // no writing now
    //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    this->SetStencilTestIsActive      (true);
    this->SetStencilFunction          (GRPSTENCILFUNCTION_EQUAL,0x01);
    this->SetStencilMask              (0x00);
    this->SetStencilFailOperation     (GRPSTENCILOPERATION_KEEP);
    this->SetStencilDepthFailOperation(GRPSTENCILOPERATION_KEEP);
    this->SetStencilPassOperation     (GRPSTENCILOPERATION_REPLACE);

    return GRPPASS::Begin();
}


/*-------------------------------------------------------------------
//  GRPSTENCILPASS::OnDraw
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/06/2017 17:53:17
//
//  @param        d :
*/
/*-----------------------------------------------------------------*/
void  GRPSTENCILPASS::OnDraw(GRPELEMENT* d)
{
    if (tag)
      GRPOPENGLCTRL::STENCIL::SetStencilFunction( GRPSTENCILFUNCTION_ALWAYS,(XDWORD)tag);
    else
      GRPOPENGLCTRL::STENCIL::SetStencilFunction( GRPSTENCILFUNCTION_ALWAYS,0x01);
}