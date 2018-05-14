
/*------------------------------------------------------------------------------------------
//  GRPREFLECTIONPASS.CPP
//
//  Reflection pass
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 09/08/2016 11:41:49
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "grpreflectionpass.h"
#include "GRPScene.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  GRPREFLECTIONPASS::GRPREFLECTIONPASS
*/
/**
//
//  Class Constructor GRPREFLECTIONPASS
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/08/2016 9:35:30
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPREFLECTIONPASS::GRPREFLECTIONPASS() : GRPPASS(__L("reflection"))
{
  Clear();
  this->priority=0;

  Reflectors    =   new XORDEREDLIST<GRPELEMENT*, GRPELEMENT::COMPARATOR>();
  Reflectables  =   new XORDEREDLIST<GRPELEMENT*, GRPELEMENT::COMPARATOR>();

  iterations=3;

  this->OrderClone=false;

  flipmatrix.BuildScaleMatrix(+1.0f,-1.0f,+1.0f);
  passtype      = GRPPASS_REFLECTION;
}

/*-------------------------------------------------------------------
//  GRPREFLECTIONPASS::~GRPREFLECTIONPASS
*/
/**
//
//   Class Destructor GRPREFLECTIONPASS
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      11/08/2016 14:53:38
//
*/
/*-----------------------------------------------------------------*/
GRPREFLECTIONPASS::~GRPREFLECTIONPASS ()
{
  DestroyRenderOrder();
  delete(this->Reflectors);
  delete(this->Reflectables);
}

/*-------------------------------------------------------------------
//  GRPREFLECTIONPASS::Begin
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/08/2016 11:44:42
//
*/
/*-----------------------------------------------------------------*/
void GRPREFLECTIONPASS::Begin()
{
  switch (currentiteration)
  {
  case 0:
          AddType                       (GRPPASS_TYPE_STENCIL);
          AddType                       (GRPPASS_TYPE_COLORWRITE);
          AddType                       (GRPPASS_TYPE_DEPTHTEST);
          AddType                       (GRPPASS_TYPE_DEPTHWRITE);
          AddType                       (GRPPASS_TYPE_DEPTHFUNC);
          AddType                       (GRPPASS_TYPE_CULLFACE);
          AddType                       (GRPPASS_TYPE_ALPHABLEND);

          SetDepthTestIsActive          (true);
          SetDepthFunction              (GRPDEPTHFUNC_LESS);
          SetDepthWritingIsActive       (false);
          SetColorWritingIsActive       (true);
          SetStencilFunction            (GRPSTENCILFUNCTION_ALWAYS,0x01);
          SetStencilMask                (0xFF);

          SetStencilFailOperation       (GRPSTENCILOPERATION_KEEP);
          SetStencilDepthFailOperation  (GRPSTENCILOPERATION_KEEP);
          SetStencilPassOperation       (GRPSTENCILOPERATION_REPLACE);

          SetStencilTestIsActive        (true);
          SetStencilValue               (0x01);

          BackFaceCullingIsActive       =true;
          BackFaceCullingMode           =GRPCULLMODE_BACK;

          AlphaBlendingIsActive         =false;

          SetClear                      (GRPPASS_CLEAR_FLAG_STENCIL);
          this->SetRenderOrder          (this->Reflectors);

  break;
  case 1:
          this->SetRenderOrder          (NULL);
          SetStencilTestIsActive        (false);
          SetDepthWritingIsActive       (true);
          SetClear                      (GRPPASS_CLEAR_FLAG_DEPTH);
          break;
  case 2:
          //SetBlending                   (true,GRPBLENDINGMODE_ADDITIVE);

          SetStencilTestIsActive        (true);
          SetColorWritingIsActive       (true);
          SetDepthTestIsActive          (true);
          SetDepthFunction              (GRPDEPTHFUNC_LESS);

          SetStencilFunction            (GRPSTENCILFUNCTION_EQUAL,0x01);
          SetStencilFailOperation       (GRPSTENCILOPERATION_KEEP);
          SetStencilDepthFailOperation  (GRPSTENCILOPERATION_KEEP);
          SetStencilPassOperation       (GRPSTENCILOPERATION_REPLACE);
          SetStencilMask                (0x00);
          SetStencilValue               (0x01);

          BackFaceCullingIsActive       =true;
          BackFaceCullingMode           =GRPCULLMODE_FRONT;

          SetClear                      (GRPPASS_CLEAR_FLAG_DEPTH);
          this->SetRenderOrder(this->Reflectables);

  break;
    case 3:

          //flipmatrixIsActive= false;
          //SetBlending                   (true,GRPBLENDINGMODE_ADDITIVE);

          SetStencilTestIsActive        (false);
          SetColorWritingIsActive       (true);
          SetDepthTestIsActive          (false);
          SetDepthWritingIsActive       (true);
          SetDepthFunction              (GRPDEPTHFUNC_LESS);

          SetClear                      (GRPPASS_CLEAR_FLAG_NONE);
          this->SetRenderOrder          (this->Reflectors);

  break;
  }

  GRPPASS::Begin();

}


void GRPREFLECTIONPASS::Start()
{
  switch(currentiteration)
  {
  case 0: flipmatrixIsActive= true; break;
  case 1: flipmatrixIsActive= true; break;
  case 2: flipmatrixIsActive= true; break;
  case 3: flipmatrixIsActive= true; break;
  case 4: flipmatrixIsActive= true; break;
  }
}

/*-------------------------------------------------------------------
//  GRPREFLECTIONPASS::Draw
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/08/2016 11:44:47
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPREFLECTIONPASS::Draw()
{
  return GRPPASS::Draw();
}

/*-------------------------------------------------------------------
//  GRPREFLECTIONPASS::End
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/08/2016 11:44:54
//
*/
/*-----------------------------------------------------------------*/
void GRPREFLECTIONPASS::End()
{
  GRPPASS::End();

}

/*-------------------------------------------------------------------
//  GRPREFLECTIONPASS::GetReflectionCasters
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/08/2016 11:45:09
//
*/
/*-----------------------------------------------------------------*/
void GRPREFLECTIONPASS::GetReflectionCasters()
{
    if (holderscene)
    for (XDWORD e=0;e<this->holderscene->GetPass()->GetRenderOrder()->GetSize();e++)
    {
      GRPELEMENT* element=static_cast<GRPELEMENT*>(this->holderscene->GetPass()->GetRenderOrder()->Get(e));
      if (element) //es un element con mesh
        if (element->GetCastsReflections())
        {
          this->Reflectors->Add(element);

        }
        else
        {
          this->Reflectables->Add(element);
        }
    }
}