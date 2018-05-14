
/*------------------------------------------------------------------------------------------
//  GRPBLENDINGPASS.CPP
//
//  Blending Pass
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 27/07/2016 12:23:20
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPBlendingPass.h"
#include "GRPScene.h"
#include "GRPViewport.h"
#include "GRPCamera.h"
#include "GRPShaderLibrary.h"
#include "GRPProfiler.h"


/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  GRPBLENDINGPASS::GRPBLENDINGPASS
*/
/**
//
//  Class Constructor GRPSHADOWPASS
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 12:23:42
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPBLENDINGPASS::GRPBLENDINGPASS () : GRPPASS(__L("blending"))
{
  Clean();
  type                        = GRPPASS_TYPE_DEPTHWRITE | GRPPASS_TYPE_DEPTHTEST | GRPPASS_TYPE_DEPTHFUNC | GRPPASS_TYPE_ALPHABLEND;
  DepthWritingIsActive        = false;
  DepthTestIsActive           = true;
  DepthFunction               = GRPDEPTHFUNC_LESS;
  AlphaBlendingIsActive       = true;
  AlphaBlendingMode           = GRPBLENDINGMODE_ALPHABLEND;

  globalshaderoverride        = false;
  lightingIsEnabled           = false;
  priority                    = 200;
  hasToClear                  = GRPPASS_CLEAR_FLAG_NONE;
  passtype                    = GRPPASS_BLENDING;

  this->SetBlending(true,GRPBLENDINGMODE_ALPHABLEND);

}

/*-------------------------------------------------------------------
//  GRPSHADOWPASS::GetShadowCasters
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      27/07/2016 14:10:21
//
//  @return       int :
//
*/
/*-----------------------------------------------------------------*/
int GRPBLENDINGPASS::GetBlendedObjects()
{
  XSTRING s;
  XSTRING output;

  if (this->OrderClone)
      return 0;

  if (holderscene)
  {
    for (XDWORD e=0;e<this->holderscene->GetPass()->GetRenderOrder()->GetSize();e++)
    {
      GRPELEMENT* element=static_cast<GRPELEMENT*>(this->holderscene->GetPass()->GetRenderOrder()->Get(e));

      if (element) //es un element con mesh
      {
          if (element->IsBlending())
          {
              GRPMATERIAL* material = element->GetNode()->GetMaterial();

              this->RenderOrder->Add(element);
          }
      }
    }
    for (int e=this->holderscene->GetPass()->GetRenderOrder()->GetSize()-1;e>=0;e--)
    {
      GRPELEMENT* element=static_cast<GRPELEMENT*>(this->holderscene->GetPass()->GetRenderOrder()->Get(e));
      if (element) //es un element con mesh
        if (element->IsBlending())
        {
          this->holderscene->GetPass()->Remove(element);
        }
    }
  }
    return this->RenderOrder->GetSize();
}

/*-------------------------------------------------------------------
//  GRPBLENDINGPASS::Begin
*/
/**
//
//  Trigger on the start of pasw
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/06/2017 10:54:11
//
*/
/*-----------------------------------------------------------------*/
void GRPBLENDINGPASS::Begin()
{
  AlphaBlendingIsActive       = true;
  DepthWritingIsActive        = false;
  GRPPASS::Begin();
}


/*-------------------------------------------------------------------
//  GRPBLENDINGPASS::End
*/
/**
//
//  Trigger after finish pass
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/06/2017 10:53:59
//
*/
/*-----------------------------------------------------------------*/
void GRPBLENDINGPASS::End()
{
  AlphaBlendingIsActive       = false;
  DepthWritingIsActive        = true;
  GRPPASS::ApplyFlags();
}

/*-------------------------------------------------------------------
//  GRPBLENDINGPASS::OnAdded
*/
/**
//
//  Triggers when the pass is added to a scene
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/06/2017 10:53:37
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPBLENDINGPASS::OnAdded()
{
  this->GetBlendedObjects();

  for (XDWORD e=0;e<this->GetRenderOrder()->GetSize();e++)
  {
      GRPELEMENT* element=this->GetRenderOrder()->Get(e);
      element->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SDF"));

      if (element->GetNode())
        if (element->GetNode()->GetMaterial())
          if (element->GetNode()->GetMaterial()->GetLayers()->GetSize()>0)
              element->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"STX"));
  }
  return true;
}