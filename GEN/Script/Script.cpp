//------------------------------------------------------------------------------------------
//  SCRIPT.CPP
//
//  Script Interpreter (Mini CPP interpreter)
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 17/07/2007 17:46:23
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <ctype.h>

#include "XPath.h"
#include "XFactory.h"
#include "XFileTXT.h"
#include "XThread.h"
#include "XDebug.h"
#include "XThreadCollected.h"

#include "ScriptLib.h"
#include "Script.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  SCRIPT::SCRIPT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/09/2011 16:34:18
//
//  @return

//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
SCRIPT::SCRIPT()
{
  Clean();

  xtimer = xfactory->CreateTimer();
  if(!xtimer) return;

  xfiletxt = new XFILETXT();
  if(!xfiletxt) return;

  RegisterEvent(SCRIPTXEVENTTYPE_ERROR);
  RegisterEvent(SCRIPTXEVENTTYPE_BREAK);

  thread = CREATEXTHREAD(XTHREADGROUPID_SCRIPT, __L("SCRIPT::SCRIPT"),ThreadFunction,(void*)this);
}



//-------------------------------------------------------------------
//  SCRIPT::~SCRIPT
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/07/2007 18:21:48
//
//  @return       virtual :
//  */
//-------------------------------------------------------------------
SCRIPT::~SCRIPT()
{
  DeRegisterEvent(SCRIPTXEVENTTYPE_ERROR);
  DeRegisterEvent(SCRIPTXEVENTTYPE_BREAK);

  if(xtimer) xfactory->DeleteTimer(xtimer);

  if(thread)
    {
      thread->End();
      DELETEXTHREAD(XTHREADGROUPID_SCRIPT, thread);
    }

  if(xfiletxt)
    {
      delete xfiletxt;
    }

  DeleteLibraryFuncions();

  librarys.DeleteAll();

  Clean();
}




//-------------------------------------------------------------------
//  SCRIPT::Load
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/07/2007 18:51:20
//
//  @return       bool :
//  @param        xpath :
*/
//-------------------------------------------------------------------
bool SCRIPT::Load(XPATH& xpath)
{
  if(!xfiletxt) return false;

  bool status = false;

  this->xpath = xpath;

  if(xfiletxt->Open(xpath))
    {
      if(xfiletxt->ReadAllFile()) status = true;

      script.Empty();

      for(int c=0;c<xfiletxt->GetNLines();c++)
        {
          script += xfiletxt->GetLine(c)->Get();
          script += __L("\r\n");
        }

      xfiletxt->Close();
    }

  return status;
}




//-------------------------------------------------------------------
//  SCRIPT::Save
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/07/2007 17:25:27
//
//  @return       bool :
//  @param        xpath :
*/
//-------------------------------------------------------------------
bool SCRIPT::Save(XPATH& xpath)
{
  if(!xfiletxt) return false;

  bool status = false;

  this->xpath = xpath;

  if(xfiletxt->Create(xpath))
    {
      if(xfiletxt->WriteAllFile()) status = true;
      xfiletxt->Close();
    }

  return status;
}




/*-------------------------------------------------------------------
//   SCRIPT::IsScript
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 11:43:48
//
//  @return   bool :
//
//  @param    XPATH& :
//  @param    XCHAR* :
//
*//*-----------------------------------------------------------------*/
bool SCRIPT::IsScript(XPATH& xpath, XCHAR* extension)
{
  if(xpath.IsEmpty()) return false;

  if(!xpath.Find(extension, true)) return true;

  return false;
}




/*-------------------------------------------------------------------
//  SCRIPT::Run
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/12/2009 08:46:25 a.m.
//
//  @return       SCRIPTERRORMSG :
//  @param        returnval :
*/
/*-----------------------------------------------------------------*/
SCRIPTERRORMSG SCRIPT::Run(int* returnval)
{
  if(script.IsEmpty()) return SCRIPTERRORMSG_INTERNALERROR;

  isrunwiththread = false;
  if(thread) isrunwiththread = thread->IsRunning();

  errorscript  = SCRIPTERRORMSG_NONE;
  iscancelexec = false;
  //breakfound   = false;

  return errorscript;
}



/*-------------------------------------------------------------------
//  SCRIPT::RunWithThread
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/11/2009 11:37:57 p.m.
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
bool SCRIPT::RunWithThread()
{
  if(!thread) return false;

  return thread->Ini();
}



/*-------------------------------------------------------------------
//  SCRIPT::IsRunWithThread
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/09/2011 0:02:10
//
//  @return       bool :
*/
/*-----------------------------------------------------------------*/
bool SCRIPT::IsRunWithThread()
{
  return isrunwiththread;
}



/*-------------------------------------------------------------------
//  SCRIPT::IsRunWithThread
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/12/2009 08:49:43 a.m.
//
//  @return       bool :
//  @param        error :
//  @param        returnvalue :
*/
/*-----------------------------------------------------------------*/
bool SCRIPT::IsRunThread(SCRIPTERRORMSG* error,int* returnvalue)
{
  if(!thread) return false;

  bool isrun = thread->IsRunning();

  if(!isrun)
    {
      if(error) (*error) = errorscript;

      if(errorscript==SCRIPTERRORMSG_NONE)
        {
          if(returnvalue) (*returnvalue) = returnvaluescript;
        }

       thread->End();
    }

  return isrun;
}



//-------------------------------------------------------------------
//  SCRIPT::CancelExecution
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/07/2007 19:19:46
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool SCRIPT::CancelExecution()
{
  if(script.IsEmpty()) return false;

  iscancelexec = true;

  return true;
}





/*-------------------------------------------------------------------
//  SCRIPT::GetLibrary
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/09/2011 19:25:04
//
//  @return       SCRIPTLIB* :
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
SCRIPTLIB* SCRIPT::GetLibrary(XCHAR* ID)
{
  if(librarys.IsEmpty()) return NULL;

  for(int c=0;c<(int)librarys.GetSize();c++)
    {
      SCRIPTLIB* scriptlib = (SCRIPTLIB*)librarys.Get(c);
      if(scriptlib)
        {
          XSTRING* IDlib = scriptlib->GetID();
          if(IDlib)
            {
              if(!IDlib->Compare(ID)) return scriptlib;
            }
        }
    }

  return NULL;
}



/*-------------------------------------------------------------------
//  SCRIPT::AddLibrary
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/09/2011 18:38:15
//
//  @return       bool :
//  @param        scriptlib :
*/
/*-----------------------------------------------------------------*/
bool SCRIPT::AddLibrary(SCRIPTLIB* scriptlib)
{
  if(!scriptlib) return false;

  librarys.Add(scriptlib);

  return scriptlib->AddLibraryFunctions(this);
}



/*-------------------------------------------------------------------
//  SCRIPT::DeleteAllLibrarys
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/09/2011 21:21:13
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool SCRIPT::DeleteAllLibrarys()
{
  if(librarys.IsEmpty()) return false;

  librarys.DeleteContents();
  librarys.DeleteAll();

  return true;
}




/*-------------------------------------------------------------------
//  SCRIPT::GetLibraryFunction
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/09/2011 17:56:59
//
//  @return       SCRIPTLIBRARYFUNCTION* :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
SCRIPTLIBRARYFUNCTION* SCRIPT::GetLibraryFunction(XCHAR* name)
{
  for(XDWORD c=0;c<libraryfunctions.GetSize(); c++)
    {
      SCRIPTLIBRARYFUNCTION* function = (SCRIPTLIBRARYFUNCTION*)libraryfunctions.Get(c);
      if(function)
        {
          if(!function->GetName()->Compare(name)) return function;
        }
    }

  return NULL;
}




/*-------------------------------------------------------------------
//  SCRIPT::AddLibraryFunction
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/09/2011 17:56:52
//
//  @return       bool :
//  @param        name :
//  @param        libfunction :
*/
/*-----------------------------------------------------------------*/
bool SCRIPT::AddLibraryFunction(SCRIPTLIB* library, XCHAR* name, SCRFUNCIONLIBRARY libfunction)
{
  SCRIPTLIBRARYFUNCTION* function = new SCRIPTLIBRARYFUNCTION(library, name, libfunction);
  if(!function) return false;

  libraryfunctions.Add(function);

  return true;
}



//-------------------------------------------------------------------
//  SCRIPT::DeleteLibraryFuncions
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/07/2007 19:19:32
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool SCRIPT::DeleteLibraryFuncions()
{
  if(libraryfunctions.IsEmpty()) return false;

  libraryfunctions.DeleteContents();
  libraryfunctions.DeleteAll();

  return true;
}




/*-------------------------------------------------------------------
//  SCRIPT::GetErrorScript
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 17:01:10
//
//  @return       SCRIPTERRORMSG :
//  */
/*-----------------------------------------------------------------*/
SCRIPTERRORMSG SCRIPT::GetErrorScript()
{
  return errorscript;
}



/*-------------------------------------------------------------------
//  SCRIPT::SetErrorScript
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 17:01:34
//
//  @return       bool :
//  @param        errorscript :
*/
/*-----------------------------------------------------------------*/
bool SCRIPT::SetErrorScript(SCRIPTERRORMSG errorscript)
{
  this->errorscript = errorscript;

  return true;
}




/*-------------------------------------------------------------------
//  SCRIPT::HaveError
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/01/2010 03:51:34 p.m.
//
//  @return       void :
//  @param        errorscript :
*/
/*-----------------------------------------------------------------*/
bool SCRIPT::HaveError(SCRIPTERRORMSG errorcode)
{

  return (errorscript==SCRIPTERRORMSG_NONE)?false:true;
}








/*-------------------------------------------------------------------
//  SCRIPT_ThreadFunction
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/11/2009 10:33:45 p.m.
//
//  @return       void :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
void SCRIPT::ThreadFunction(void* data)
{
  SCRIPT* script = (SCRIPT*)data;
  if(!script) return;

  int returnvaluescript;

  script->SetErrorScript(script->Run(&returnvaluescript));

  //script->SetReturnValueScript(returnvaluescript);

  script->GetThread()->Run(false);
}

