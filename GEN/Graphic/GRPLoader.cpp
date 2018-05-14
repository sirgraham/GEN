
/*------------------------------------------------------------------------------------------
//  GRPLOADER.CPP
//
//  Loads graphics
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 15/09/2016 11:57:18
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPLoader.h"
#include "GRPObject.h"
#include "GRPConsole.h"
#include "GRPFile3D.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  GRPLOADER::GRPLOADER
*/
/**
//
//  Class Constructor GRPLOADER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/03/2017 12:26:58
//
*/
/*-----------------------------------------------------------------*/
GRPLOADER::GRPLOADER()
{
  console=NULL;
}

GRPLOADER::~GRPLOADER()
{
    this->tasks.DeleteContents();
    this->tasks.DeleteAll();
}

/*-------------------------------------------------------------------
//  GRPLOADERTASK::GRPLOADERTASK
*/
/**
//
//  Class Constructor GRPLOADERTASK
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 10:04:51
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
GRPLOADERTASK::GRPLOADERTASK      (XCHAR* name) :
                                    XLOADERTASK (name),
                                    object      (NULL),
                                    loadflags   (GRPFILE3D_INCLUDE_ALL),
                                    verbose     (GRPFILE3D_VERBOSE_LEVEL_NONE)
{
  layerTotal=0.0;
  AdditionalLayers.SetIsMulti(true);
  grpfile3d=NULL;
};

/*-------------------------------------------------------------------
//  GRPLOADERTASK::~GRPLOADERTASK
*/
/**
//
//   Class Destructor GRPLOADERTASK
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 10:04:40
//
*/
/*-----------------------------------------------------------------*/
GRPLOADERTASK::~GRPLOADERTASK()
{
  AdditionalLayers.DeleteKeyContents();
  AdditionalLayers.DeleteAll();
  if (this->file)
  delete(this->file);

  if (grpfile3d)
    delete(grpfile3d);

  this->Hide();
}


/*-------------------------------------------------------------------
//  GRPLOADERTASK::AddLayer
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 10:04:29
//
//  @return       XDWORD :
//
//  @param        filename :
//  @param        id :
//  @param        flags :
*/
/*-----------------------------------------------------------------*/
XDWORD  GRPLOADERTASK::AddLayer   (XCHAR* filename, XDWORD id,  XDWORD flags)
{
  XSTRING*  string=new XSTRING(filename);
            AdditionalLayers.Add(string,flags);
  return 0;
}

/*-------------------------------------------------------------------
//  GRPLOADERTASK::GetSubjectType
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 10:04:18
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPLOADERTASK::GetSubjectType()
{
  return GRPNAMED_TYPE_TASK;
}


/*-------------------------------------------------------------------
//  GRPLOADERTASK::Run
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 10:04:10
//
*/
/*-----------------------------------------------------------------*/
int n=0;
void GRPLOADERTASK::Run()
{
  n++;
  XPATH      xpath;

  if (object==NULL)
    object=new GRPOBJECT();

  this->layerTotal=0.0f;

  if (grpfile3d)
    delete(grpfile3d);

  grpfile3d=NULL;

  grpfile3d = new GRPFILE3D();
  if(grpfile3d)
  {
    grpfile3d->SetObject        (object);
    grpfile3d->SetVerboseLevel  (verbose);
    grpfile3d->SetLoadFlags     (loadflags);

      if (file)
      {
        xpath=this->file->Get();
        this->Subscribe(grpfile3d);

      XDEBUG_PRINTCOLOR (0,__L("Starting Load"));
      this->object= grpfile3d->Load(xpath.Get());

      if(!object)
        {
          delete(grpfile3d);
          grpfile3d=NULL;
          Publish(GRPMSG_EXIT);
          thread->Run(false);
          return;
        }
      this->layerTotal+=this->current;
      }

    Publish(GRPMSG_END);
    delete(grpfile3d);
    grpfile3d = NULL;
  }


  this->isfinished=true;
  thread->Exit();
  thread->Run(false);
}


/*-------------------------------------------------------------------
//  GRPLOADERTASK::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      23/09/2016 17:07:28
//
//  @param        n :
*/
/*-----------------------------------------------------------------*/
void GRPLOADERTASK::Add(float n)
{
  XLOADERTASK::Add(n);
  this->Publish(GRPMSG_STEP);
}


/*-------------------------------------------------------------------
//  GRPLOADERTASK::Update
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 14:23:22
//
*/
/*-----------------------------------------------------------------*/
void GRPLOADERTASK::Update()
{
  XLOADERTASK::Update();
  if (this->AdditionalLayers.GetSize()>0)
  {
  this->percent=(this->current+this->layerTotal)/(this->AdditionalLayers.GetSize()+1);
  }
  else
  {
    if (this->total!=0.0f)
    this->percent=(this->current/this->total)*100.0f;
  }
}

/*-------------------------------------------------------------------
//  GRPLOADERTASK::SetCurrent
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 14:29:42
//
//  @param        current :
*/
/*-----------------------------------------------------------------*/
void GRPLOADERTASK::SetCurrent(float current)
{
  this->current=current;
}

/*-------------------------------------------------------------------
//   GRPLOADERTASK::Notify
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  29/11/2017 15:00:52
//  @return   bool :
//
//  @param    GRPMSGTYPE :
//  @param    GRPSUBJECT* :
//
*//*-----------------------------------------------------------------*/
bool GRPLOADERTASK::Notify(GRPMSGTYPE msg, GRPSUBJECT* subject)
{
    Publish(msg, subject);
    return true;
}

/*-------------------------------------------------------------------
//  GRPLOADER::CreateTask
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2016 12:42:44
//
//  @return       XLOADERTASK* :
//
//  @param        pname :
*/
/*-----------------------------------------------------------------*/
XLOADERTASK*  GRPLOADER::CreateTask (XCHAR* pname)
{
  GRPLOADERTASK*  task=new GRPLOADERTASK(pname);
                  task->AddObserver(this);
                  task->SetFile(new XSTRING(pname));
  return task;
}

/*-------------------------------------------------------------------
//  GRPLOADER::Print
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      02/03/2017 15:51:05
//
//  @param        color :
//  @param        text :
*/
/*-----------------------------------------------------------------*/
void  GRPLOADER::Print(XDWORD color, XCHAR* text)
{
  if (this->console)
  this->console->Print(color,text);
}


/*-------------------------------------------------------------------
//  GRPLOADER::GetSubjectType
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 10:06:14
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD GRPLOADER::GetSubjectType()
{
  return GRPNAMED_TYPE_TASKMANAGER;
}

/*-------------------------------------------------------------------
//  GRPLOADER::Notify
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2016 12:42:33
//
//  @return       bool :
//
//  @param        msg :
//  @param        subject :
*/
/*-----------------------------------------------------------------*/
bool GRPLOADER::Notify(GRPMSGTYPE msg, GRPSUBJECT* subject)
{
  switch(msg)
  {
  case GRPMSG_EXIT:
  case GRPMSG_STEP:
  case GRPMSG_END:
    if (subject->GetSubjectType()==GRPNAMED_TYPE_TASK)
    {
      Update();
      this->Publish(msg,subject);
    }
    break;
  }

  return true;
}