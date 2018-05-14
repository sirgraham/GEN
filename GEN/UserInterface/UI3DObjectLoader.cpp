
/*------------------------------------------------------------------------------------------
//  UI3DOBJECTLOADER.CPP
//
//  load 3D object for the UI
//
//  Author            : Imanol Celaya Ruiz de Alegria
//  Date Of Creation  : 13/02/2017 17:59:31
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "UI3DObjectLoader.h"

#include "GRPFile3D.h"
#include "GRPObject.h"
#include "GRPShaderLibrary.h"

#include "XPath.h"
#include "XDebug.h"
#include "XMemory.h"
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

//DIEGO: esto esta obsoleto

GRPOBJECT* UI3DOBJECTLOADER::Load(XCHAR* path)
{
  //GRPOBJECT*                LoadObject                    (GRPOBJECT* obj,XCHAR* path,XDWORD flags=GRPFILE3D_INCLUDE_ALL, XDWORD verboseLevel=GRPFILE3D_VERBOSE_LEVEL_WARNINGS | GRPFILE3D_VERBOSE_LEVEL_ERRORS);
  XDWORD loadflags = GRPFILE3D_INCLUDE_ALL;
  XDWORD verbose = GRPFILE3D_VERBOSE_LEVEL_WARNINGS | GRPFILE3D_VERBOSE_LEVEL_ERRORS;

  XPATH xpath;
  GRPFILE3D* grpfile3d  = NULL;

  xpath = path;

  GRPOBJECT* obj = new GRPOBJECT();

  grpfile3d = new GRPFILE3D();
  if(!grpfile3d) return NULL;

  grpfile3d->SetObject(obj);
  grpfile3d->SetVerboseLevel(verbose);
  grpfile3d->SetLoadFlags(loadflags);

  XDEBUG_PRINTCOLOR(0,__L("Starting Load"));
  GRPOBJECT* object = grpfile3d->Load(xpath.Get());
  OPENGLCHECKERROR(__L("LoadObj"));
  if(!object)
    {
      delete grpfile3d;
      grpfile3d=NULL;
      return NULL;
    }

  OPENGLCHECKERROR(__L("SetProgram"));

  object->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"DOT"));

  for(XDWORD e=0;e<object->GetElements()->GetSize();e++)
    {
      if(object->GetElements()->Get(e)->GetNode()->GetMaterial()==NULL || object->GetElements()->Get(e)->GetNode()->GetMaterial()->GetLayers()->GetSize()<=0)
        {
          object->GetElements()->Get(e)->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"DDF"));
          OPENGLCHECKERROR(__L("SetDiffuse"));
        }

      if(object->GetElements()->Get(e)->GetSkin() != NULL)    object->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SKN"));
      if(object->GetElements()->Get(e)->GetDeformer()!= NULL) object->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"MPD"));

      if (object->GetElements()->Get(e)->GetNode()->GetMaterial())
      if (object->GetElements()->Get(e)->GetNode()->GetMaterial()->GetLayers()>0)
      {
      if(object->GetElements()->Get(e)->GetSkin() != NULL)    object->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"SKN"));
      if(object->GetElements()->Get(e)->GetDeformer()!= NULL) object->SetProgram(GRPSHADERLIBRARY::Get()->GetProgramByKey((char*)"MPH"));
      }
    }

  delete grpfile3d;

  return object;
}
