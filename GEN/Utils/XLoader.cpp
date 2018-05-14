
/*------------------------------------------------------------------------------------------
//  XLOADER.CPP
//
//  loader
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 05/09/2016 11:14:44
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XLoader.h"
#include "XTimer.h"
#include "XFactory.h"
#include "XDebug.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  XLOADERTASK::XLOADERTASK
*/
/**
//
//  Class Constructor XLOADERTASK
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2016 10:30:36
//
//  @param        pname :
*/
/*-----------------------------------------------------------------*/
XLOADERTASK::XLOADERTASK(XCHAR* pname) : name(pname),file (NULL)
{
  Clean();
}

/*-------------------------------------------------------------------
//  XLOADERTASK::~XLOADERTASK
*/
/**
//
//   Class Destructor XLOADERTASK
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2016 10:32:01
//
*/
/*-----------------------------------------------------------------*/
XLOADERTASK::~XLOADERTASK()
{
  if (xtimer)
  {
    xfactory->DeleteTimer(xtimer);
    xtimer=NULL;
  }

  if (lock)
  {
    xfactory->Delete_Mutex(lock);
    lock=NULL;
  }

  if (thread)
  {
    xfactory->DeleteThread(XTHREADGROUPID_GRPLOADER,thread);
    thread=NULL;
  }
}

/*-------------------------------------------------------------------
//  XLOADERTASK::IsFinished
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2016 10:30:59
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  XLOADERTASK::IsFinished()
{
  return isfinished;
}

/*-------------------------------------------------------------------
//  XLOADERTASK::Start
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2016 10:31:16
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  XLOADERTASK::Start()
{
  isfinished=false;
  if (xtimer==NULL)
    xtimer=xfactory->CreateTimer();

  if (xtimer==NULL) return false;

  xtimer->Reset();

  if (lock==NULL)
    lock=xfactory->Create_Mutex();

  if (lock==NULL) return false;

  if (thread==NULL)
  {
    thread=xfactory->CreateThread(XTHREADGROUPID_GRPLOADER,name.Get(),XLOADERTASK::ThreadFunction,this);
  }

  if (thread!=NULL)
  {
    thread->Ini();
    thread->Run(true);
  }
  delete(xtimer);
  xtimer = NULL;

  return true;
}


/*-------------------------------------------------------------------
//  XLOADERTASK::ThreadFunction
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2016 11:46:32
//
//  @param        v :
*/
/*-----------------------------------------------------------------*/
void XLOADERTASK::ThreadFunction  (void* v)
{
    XLOADERTASK* task=(XLOADERTASK*)v;
    task->Run();
}


/*-------------------------------------------------------------------
//  XLOADER::XLOADER
*/
/**
//
//  Class Constructor XLOADER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/09/2016 10:57:02
//
*/
/*-----------------------------------------------------------------*/
XLOADER::XLOADER()
{

}

/*-------------------------------------------------------------------
//  XLOADER::~XLOADER
*/
/**
//
//   Class Destructor XLOADER
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/09/2016 10:57:08
//
*/
/*-----------------------------------------------------------------*/
XLOADER::~XLOADER()
{
  this->tasks.DeleteContents();
  this->tasks.DeleteAll();

  this->files.DeleteContents();
  this->files.DeleteAll();

}

/*-------------------------------------------------------------------
//  XLOADER::SetName
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/09/2016 11:00:47
//
//  @param        name :
*/
/*-----------------------------------------------------------------*/
void XLOADER::SetName(XCHAR* name)
{
  this->Name=name;
}

/*-------------------------------------------------------------------
//  XLOADER::Start
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/09/2016 10:57:18
//
*/
/*-----------------------------------------------------------------*/
void XLOADER::Start()
{
  for (XDWORD e=0;e<this->files.GetSize();e++)
  {
    XLOADERTASK*  task=CreateTask(this->files.Get(e)->Get());
                  task->file=this->files.Get(e);
                  this->tasks.Add(task);
  }

  for (XDWORD e=0;e<tasks.GetSize();e++)
  {
        tasks.Get(e)->Start();
  }


}

/*-------------------------------------------------------------------
//  XLOADER::AddFile
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/09/2016 10:57:25
//
//  @param        path :
*/
/*-----------------------------------------------------------------*/
void XLOADER::AddFile(XCHAR* path)
{
  XSTRING*  string=new XSTRING();
  if (string)
  {
            string->Set(path);
            this->files.Add(string);
  }
}

/*-------------------------------------------------------------------
//  XLOADER::AddTask
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/09/2016 10:57:34
//
//  @param        task :
*/
/*-----------------------------------------------------------------*/
void XLOADER::AddTask(XLOADERTASK* task)
{
  this->tasks.Add(task);
  task->loader=this;
  Update();
}


/*-------------------------------------------------------------------
//  XLOADER::IsFinished
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/09/2016 10:57:53
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XLOADER::IsFinished()
{
  for (XDWORD e=0;e<this->tasks.GetSize();e++)
  {
    if (!this->tasks.FastGet(e)->IsFinished())
      return false;
  }
  return true;
}


/*-------------------------------------------------------------------
//  XLOADER::Update
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/09/2016 11:03:11
//
*/
/*-----------------------------------------------------------------*/
void XLOADER::Update()
{
  Total=0;
  Current=0;
  Percent=0;

  for (XDWORD e=0;e<tasks.GetSize();e++)
  {
    XLOADERTASK* task=tasks.Get(e);

    Total   +=task->total;
    Current +=task->current;

    if (task->current>=task->total)
        task->isfinished=true;
  }

  if (Total!=0.0f)
    Percent=(Current/Total)*100.0f;

}

/*-------------------------------------------------------------------
//   XLOADER::Abort
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  29/11/2017 14:03:57
//  @return   void :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
void XLOADER::Abort()
{
    this->abort = true;
    for (XDWORD i = 0; i<tasks.GetSize(); i++)
    {
        tasks.Get(i)->Abort();
    }
}


/*-------------------------------------------------------------------
//  XLOADER::HandleEvent
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/09/2016 10:58:19
//
//  @param        event :
*/
/*-----------------------------------------------------------------*/
void XLOADER::HandleEvent(XEVENT* event)
{
if(!event) return;


}


/*-------------------------------------------------------------------
//  XLOADER::GetCurrentPercent
*/
/**
//
//  Gets current total percent
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/09/2016 10:58:51
//
//  @return       float :
//
*/
/*-----------------------------------------------------------------*/
float XLOADER::GetCurrentPercent()
{
  return this->Percent;
}


/*-------------------------------------------------------------------
//  XLOADER::CreateTask
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/09/2016 11:40:32
//
//  @return       XLOADERTASK* :
//
//  @param        pname :
*/
/*-----------------------------------------------------------------*/
XLOADERTASK*  XLOADER::CreateTask(XCHAR* pname)
{
  XLOADERTASK* task=new XLOADERTASK(pname);
  task->loader=this;
  return task;
}


/*-------------------------------------------------------------------
//  XLOADERTASK::SetFile
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 12:15:11
//
//  @param        file :
*/
/*-----------------------------------------------------------------*/
void XLOADERTASK::SetFile(XSTRING* file)
{
  this->file=file;
}

/*-------------------------------------------------------------------
//  XLOADERTASK::GetFile
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 12:15:26
//
//  @return       XCHAR* :
//
*/
/*-----------------------------------------------------------------*/
XCHAR*  XLOADERTASK::GetFile()
{
  return this->file->Get();
}



/*-------------------------------------------------------------------
//  XLOADERTASK::SetTotal
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 12:18:41
//
//  @param        total :
*/
/*-----------------------------------------------------------------*/
void XLOADERTASK::SetTotal(float total)
{
  this->total=total;
}

/*-------------------------------------------------------------------
//  XLOADERTASK::GetTotal
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 12:18:52
//
//  @return       float :
//
*/
/*-----------------------------------------------------------------*/
float XLOADERTASK::GetTotal()
{
  return total;
}

/*-------------------------------------------------------------------
//  XLOADERTASK::SetCurrent
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 12:19:02
//
//  @param        current :
*/
/*-----------------------------------------------------------------*/
void XLOADERTASK::SetCurrent(float current)
{
  this->current=current;
}

/*-------------------------------------------------------------------
//  XLOADERTASK::GetCurrent
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 12:19:12
//
//  @return       float :
//
*/
/*-----------------------------------------------------------------*/
float XLOADERTASK::GetCurrent()
{
  return this->current;
}

/*-------------------------------------------------------------------
//  XLOADERTASK::SetPercent
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 12:21:48
//
//  @param        current :
*/
/*-----------------------------------------------------------------*/
void XLOADERTASK::SetPercent(float percent)
{
  this->percent=percent;
}

/*-------------------------------------------------------------------
//  XLOADERTASK::GetPercent
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 12:21:58
//
//  @return       float :
//
*/
/*-----------------------------------------------------------------*/
float XLOADERTASK::GetPercent()
{
  return this->percent;
}

/*-------------------------------------------------------------------
//  XLOADERTASK::Abort
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/03/2017 17:11:47
//
*/
/*-----------------------------------------------------------------*/
void XLOADERTASK::Abort()
{
  this->aborted=true;
  if (thread)
  if (this->thread->Run(false))
    thread->WaitToEnd(1000);
};

/*-------------------------------------------------------------------
//  XLOADERTASK::Update
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      21/09/2016 12:36:58
//
*/
/*-----------------------------------------------------------------*/
void XLOADERTASK::Update()
{
   if (total!=0.0f)
   {
     this->percent=(this->current/this->total)*100.0f;
   }
}

/*-------------------------------------------------------------------
//  XLOADERTASK::Output
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/03/2017 11:43:38
//
//  @param        priority :
//  @param        string :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
void XLOADERTASK::Output(XDWORD priority, XCHAR* mask,...)
{
  XSTRING string;

  va_list arg;

  va_start(arg, mask);

  string.FormatArg(mask, &arg);

  va_end(arg);

  Print (priority,string.Get());
}


/*-------------------------------------------------------------------
//  XLOADERTASK::Print
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/03/2017 11:48:13
//
//  @param        priority :
//  @param        text :
*/
/*-----------------------------------------------------------------*/
void XLOADERTASK::Print(XDWORD priority,XCHAR* text)
{
  if (this->loader)
    this->loader->Print(priority,text);
}

/*-------------------------------------------------------------------
//  XLOADERTASK::Wait
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/03/2017 16:50:24
//
*/
/*-----------------------------------------------------------------*/
void XLOADER::Wait()
{
    for (XDWORD e=0;e<tasks.GetSize();e++)
    {
      while (!tasks.Get(e)->IsFinished());
    }
}