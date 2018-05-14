
/*------------------------------------------------------------------------------------------
//  GRPFILE3D.CPP
//
//  Loads generic 3D files
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 06/05/2015 10:08:40
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPObject.h"
#include "GRPScene.h"
#include "GRPFile3D.h"
#include "GRPFile3DOBJ.h"
#include "GRPFile3DFBX.h"
#include "GRPFile3DXML.h"
#include "GRPFile3DGGF.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  GRPFILE3D::GRPFILE3D
*/
/**
//
//  Class Constructor GRPFILE3D
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2016 10:23:49
//
*/
/*-----------------------------------------------------------------*/
GRPFILE3D::GRPFILE3D()
{
  Clean();

  chronometer=xfactory->CreateTimer();

  VerboseLevel=GRPFILE3D_VERBOSE_LEVEL_ERRORS;
}

/*-------------------------------------------------------------------
//  GRPFILE3D::~GRPFILE3D
*/
/**
//
//   Class Destructor GRPFILE3D
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2016 10:24:01
//
*/
/*-----------------------------------------------------------------*/
GRPFILE3D::~GRPFILE3D()
{



  if (chronometer)
  {
    xfactory->DeleteTimer(chronometer);
    chronometer=NULL;
  }

  if (file3dfactory)
  {
      file3dfactory->Unsubscribe();
      file3dfactory->Publish(GRPMSG_DESTROYED);
      file3dfactory->Hide();
      delete(file3dfactory);
      file3dfactory = NULL;
  }

};

/*-------------------------------------------------------------------
//  GRPFILE3D::SetVerboseLevel
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2016 10:24:20
//
//  @param        lvl :
*/
/*-----------------------------------------------------------------*/
void  GRPFILE3D::SetVerboseLevel(int lvl)
{
  this->VerboseLevel  =lvl;
};

/*-------------------------------------------------------------------
//  GRPFILE3D::SetNextStackID
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      29/11/2016 13:46:08
//
//  @param        nextStackId :
*/
/*-----------------------------------------------------------------*/
void  GRPFILE3D::SetNextStackID(XDWORD nextStackId)
{
  this->NextStackID=nextStackId;
}

/*-------------------------------------------------------------------
//  GRPFILE3D::SetLoadFlags
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2016 10:24:30
//
//  @param        lvl :
*/
/*-----------------------------------------------------------------*/
void  GRPFILE3D::SetLoadFlags(XDWORD lvl)
{
  this->loadFlags     =lvl;
};

/*-------------------------------------------------------------------
//  GRPFILE3D::SetFilename
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      30/11/2016 10:34:49
//
//  @param        file :
*/
/*-----------------------------------------------------------------*/
void  GRPFILE3D::SetFilename(XCHAR* file)
{
  this->filename.Set(file);
}
void  GRPFILE3D::SetFile(XCHAR* file)
{
  this->file.Set(file);
}

/*-------------------------------------------------------------------
//  GRPFILE3D::SetObject
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      28/11/2016 10:24:39
//
//  @param        obj :
*/
/*-----------------------------------------------------------------*/
void  GRPFILE3D::SetObject(GRPOBJECT* obj)
{
  this->object=obj;
}

void GRPFILE3D::SetScene(GRPSCENE*scene)
{
  this->scene=scene;
}

void GRPFILE3D::Abort()
{

}

bool GRPFILE3D::Notify(GRPMSGTYPE msg, GRPSUBJECT* subject)
{
    if (msg == GRPMSG_DESTROYED)
        this->Unsubscribe();

    Publish(msg, subject);
    return true;
}

float GRPFILE3D::GetPercent()
{
    float p = percent;
    for (XDWORD e = 0; e < this->tasks.GetSize(); e++)
    {
        p += tasks.Get(e);
    }
    if (this->tasks.GetSize())
        p /= this->tasks.GetSize();
    return p;
}

/*-------------------------------------------------------------------
//  GRPFILE3D::Load
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      02/03/2016 14:01:27
//
//  @return       GRPOBJECT* :
//
//  @param        file :
*/
/*-----------------------------------------------------------------*/
GRPOBJECT*  GRPFILE3D::Load(XCHAR* file)
  {
  filename.Set(file);

  if (file3dfactory!=NULL)
    delete(file3dfactory);

  file3dfactory=NULL;


      if (filename.Find(__L(".obj"),true)!=NOTFOUND) //-------------------------------- WAVEFRONT .obj
      {
        file3dfactory=new GRPFILE3DOBJ();
      }
      else
      if (filename.Find(__L(".fbx"),true)!=NOTFOUND) //--------------------------------Autodesk FILMBOX . fbx
      {
        file3dfactory=new GRPFILE3DFBX();
      }
      else
      if (filename.Find(__L(".xml"),true)!=NOTFOUND) //--------------------------------XML definition
      {
        file3dfactory=new GRPFILE3DXML();
      }
      else
      if (filename.Find(__L(".ggf"),true)!=NOTFOUND) //--------------------------------GGF
      {
        file3dfactory=new GRPFILE3DGGF();
        file3dfactory->Init();
      }
      else                                          //-------------------------------- Unknown
      {
        XDEBUG_PRINTCOLOR(0,__L("GRPFILE3D::Load : Error, unknown format!"));
        return NULL;
      }

      bool owned=false;

      if (object==NULL)
      {
        object=new GRPOBJECT();
        owned=true;
      }

      this->Subscribe(file3dfactory);

      file3dfactory->SetFilename      (file);
      XPATH xpath;      xpath=file; xpath.GetNamefileExt(this->file);
      file3dfactory->SetFile(this->file.Get());
      file3dfactory->SetObject        (object);
      file3dfactory->SetScene         (scene);
      file3dfactory->SetLoadFlags     (this->loadFlags);
      file3dfactory->SetVerboseLevel  (this->VerboseLevel);
      file3dfactory->SetNextStackID   (this->NextStackID);



      GRPOBJECT* obj=file3dfactory->Load(filename.Get());
      if (obj==NULL && owned) //la carga ha fallado y el object lo hemos creado nosotros, lo borramos
      {
        delete(object);
      }

    if (VerboseLevel & GRPFILE3D_VERBOSE_LEVEL_STATUS)
    {
      FBX_DEBUG(FBX_NOTIFY,__L("Loading Complete"));
    }
  return obj;
}


