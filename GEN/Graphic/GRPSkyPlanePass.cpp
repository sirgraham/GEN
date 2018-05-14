
/*------------------------------------------------------------------------------------------
//  GRPSKYPLANEPASS.CPP
//
//  Manages skybox on pass
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 20/06/2017 10:59:56
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPSkyPlanePass.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

#include "GRPElement.h"
#include "GRPShaderLibrary.h"

/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


bool  GRPSKYPLANEPASS::OnAdded()
{
  GRPBACKGROUNDPASS::OnAdded();
  plane->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByName(__L("Skybox")));
  return true;
}