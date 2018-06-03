/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XApplication.cpp
*
* @class       XAPPLICATION
* @brief       Application Base class
* @ingroup     UTILS
*
* @author      Abraham J. Velez 
* @date        25/05/2018 18:04:36
*
* @copyright   Copyright(c) 2005 - 2018 GEN Group.
*
* @cond
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* @endcond
*
*---------------------------------------------------------------------------------------------------------------------*/

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>

#include "XFactory.h"
#include "XSleep.h"
#include "XTimer.h"
#include "XPath.h"
#include "XDir.h"
#include "XPublisher.h"
#include "XScheduler.h"
#include "XSystem.h"
#include "XDebugTrace.h"

#include "Main.h"

#include "XApplication.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XAPPLICATION::XAPPLICATION()
* @brief      Constructor
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:05:10
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XAPPLICATION::XAPPLICATION()
{
  Clean();
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XAPPLICATION::~XAPPLICATION()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:10:27
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XAPPLICATION::~XAPPLICATION()
{
  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XAPPLICATION::IniApplication(XSYSTEM* xsystem, XVECTOR<XSTRING*>* execparams)
* @brief      Ini Applicatio function
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:05:43
*
* @param[in]  xsystem : instance of xsystem of this SO.
* @param[in]  execparams : Params of command line (params of exec application)
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XAPPLICATION::IniApplication(XVECTOR<XSTRING*>* execparams)
{  
  #ifdef MICROCONTROLLER
  XDEBUG_ADDTARGET(XDEBUGTRACETYPE_SPECIAL, XDEBUG_DEFAULT_SPECIALAIM);
  #endif

  this->execparams  = execparams;

  xtimerglobal = xfactory->CreateTimer();
  if(!xtimerglobal) return false;

  xtimerglobal->Reset();

  if(!Ini()) return false;

  #if defined(WINDOWS) || defined(LINUX) || defined(MICROCONTROLLER)
  return FirstUpdate();
  #else
  return true;
  #endif
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XAPPLICATION::UpdateApplication()
* @brief      Update application callback
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:08:53
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XAPPLICATION::UpdateApplication()
{
  if(!Update())
    {
      SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER);

      return false;
    }

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XAPPLICATION::EndApplication()
* @brief      End application function
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       25/05/2018 18:10:22
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XAPPLICATION::EndApplication()
{
  bool status;

  status = LastUpdate();

  if(status) status = End();

  XPUBLISHER::GetInstance().DeRegisterAllEvents();

  if(xtimerglobal)
    {
      xfactory->DeleteTimer(xtimerglobal);
      xtimerglobal = NULL;
    }

  return status;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XAPPLICATION::Ini() 
* @brief      Ini callback for this class to use for the application class real
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:44:57
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XAPPLICATION::Ini()  
{ 
  return true;                            
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XAPPLICATION::FirstUpdate()
* @brief      First Update callback for this class to use for the application class real
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:45:18
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XAPPLICATION::FirstUpdate()
{ 
  return true;                            
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XAPPLICATION::Update() 
* @brief      Update callback for this class to use for the application class real
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:46:01
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XAPPLICATION::Update() 
{ 
  return true;                            
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XAPPLICATION::LastUpdate()
* @brief      Last Update callback for this class to use for the application class real
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:46:19
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XAPPLICATION::LastUpdate()
{ 
  return true;                            
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XAPPLICATION::End()
* @brief      End callback for this class to use for the application class real
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:46:29
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XAPPLICATION::End()
{ 
  return true;                            
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XVECTOR<XSTRING*>* XAPPLICATION::GetExecParams() 
* @brief      Obtain the parameters of the application in command line (all on strings)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:49:22
*
* @return     XVECTOR<XSTRING*>* : Vector of strings with the params
*
*---------------------------------------------------------------------------------------------------------------------*/
XVECTOR<XSTRING*>*  XAPPLICATION::GetExecParams() 
{ 
  return execparams;                      
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XAPPLICATION::GetApplicationName()
* @brief      Get Application name
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:50:19
*
* @return     XSTRING* : String pointer with the name of the application
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XAPPLICATION::GetApplicationName()
{ 
  return &applicationname;                
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XAPPLICATION::SetApplicationName(XCHAR* name)
* @brief      Set name of the application
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:51:05
*
* @param[in]  name : name of the application
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XAPPLICATION::SetApplicationName(XCHAR* name)
{ 
  applicationname.Set(name);              
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSTRING* XAPPLICATION::GetApplicationID()
* @brief      Get Application ID
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:51:44
*
* @return     XSTRING* : String with the ID of the aplication
*
*---------------------------------------------------------------------------------------------------------------------*/
XSTRING* XAPPLICATION::GetApplicationID()
{ 
  return &applicationID;                  
}

    

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         MAIN* XAPPLICATION::GetMain() 
* @brief      Get Main class for the application
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:53:48
*
* @return     MAIN* : 
*
*---------------------------------------------------------------------------------------------------------------------*/
MAIN* XAPPLICATION::GetMain() 
{ 
  return main;                            
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XAPPLICATION::SetMain(MAIN* main)
* @brief      Set Main class for the application
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:56:02
*
* @param[in]  main : 
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XAPPLICATION::SetMain(MAIN* main)
{ 
  this->main = main;                      
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XTIMER* XAPPLICATION::GetTimerGlobal() 
* @brief      Get timer global for the application
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:41:17
*
* @return     XTIMER* : XTIMER class global
*
*---------------------------------------------------------------------------------------------------------------------*/
XTIMER* XAPPLICATION::GetTimerGlobal() 
{ 
  return xtimerglobal;                    
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XAPPLICATIONEXITTYPE XAPPLICATION::GetExitStatus() 
* @brief      Ge the exit status for the application
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:40:31
*
* @return     XAPPLICATIONEXITTYPE : status of the exit
*
*---------------------------------------------------------------------------------------------------------------------*/
XAPPLICATIONEXITTYPE XAPPLICATION::GetExitStatus()  
{ 
  return exitstatus;                      
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XAPPLICATION::SetExitStatus(XAPPLICATIONEXITTYPE exitstatus)
* @brief      Set the exit status for the application
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:39:47
*
* @param[in]  exitstatus : status of the exit (XAPPLICATIONEXITTYPE enum)
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XAPPLICATION::SetExitStatus(XAPPLICATIONEXITTYPE exitstatus)
{
  if(this->exitstatus!=XAPPLICATIONEXITTYPE_NONE) return;

  this->exitstatus = exitstatus;
}

  

/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XAPPLICATION::HandleEvent(XEVENT* xevent)
* @brief      Handle of event from observer
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 10:39:00
*
* @param[in]  xevent : event class of the observer
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XAPPLICATION::HandleEvent(XEVENT* xevent)
{

}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XAPPLICATION::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       29/05/2018 13:10:43
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XAPPLICATION::Clean()
{
  execparams        = NULL;
  main              = NULL;
  xtimerglobal      = NULL;
  exitstatus        = XAPPLICATIONEXITTYPE_NONE;
}






