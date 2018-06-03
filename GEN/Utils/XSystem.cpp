/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XSystem.cpp
*
* @class       XSYSTEM
* @brief       System class
* @note        Can´t be construct Factory + singelton without depends of system. IT´S NOT A SINGLETON.
* @ingroup     UTILS
*
* @author      Abraham J. Velez 
* @date        02/06/2018 16:46:47
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

#include "XSystem.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XSYSTEM* XSYSTEM::instance = NULL;

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSYSTEM::XSYSTEM()
* @brief      Constructor
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 16:46:10
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSYSTEM::XSYSTEM()
{
  Clean();
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSYSTEM::~XSYSTEM()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 16:46:15
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSYSTEM::~XSYSTEM()
{
  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XSYSTEM::GetIsInstanced()
* @brief      GetIsInstanced
* @note       STATIC
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 10:18:52
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XSYSTEM::GetIsInstanced() 
{
  return instance!=NULL;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSYSTEM& XSYSTEM::GetInstance()
* @brief      GetInstance
* @note       STATIC
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 10:18:58
*
* @return     XSYSTEM& : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSYSTEM& XSYSTEM::GetInstance()
{
  if(!instance) instance = new XSYSTEM();

  return (*instance);
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XSYSTEM::SetInstance(XSYSTEM* instance)
* @brief      SetInstance
* @note       STATIC
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 10:48:40
*
* @param[in]  _instance : new instance
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XSYSTEM::SetInstance(XSYSTEM* _instance)
{
  if(!_instance) return false;

  instance = _instance;

  return (instance)?true:false;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XSYSTEM::DelInstance()
* @brief      DelInstance
* @note       STATIC
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 10:19:02
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XSYSTEM::DelInstance()
{
  if(instance)
    {
      delete instance;
      instance = NULL;

      return true;
    }

  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSYSTEM_HARDWARETYPE XSYSTEM::GetTypeHardware(int* revision)
* @brief      Get Type Hardware
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 16:45:28
*
* @param[in]  revision : revision (Pi rules)
*
* @return     XSYSTEM_HARDWARETYPE : Type Hardware (enum XSYSTEM_HARDWARETYPE)
*
*---------------------------------------------------------------------------------------------------------------------*/
XSYSTEM_HARDWARETYPE  XSYSTEM::GetTypeHardware(int* revision)                          
{ 
  return XSYSTEM_HARDWARETYPE_UNKNOWN;              
}

    
/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XSYSTEM::HardwareUseLittleEndian()
* @brief      Hardware Use LittleEndian
* @note       In ARM can be changed, can be used BIG ENDIAN or LITTLE ENDIAN
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 16:48:05
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XSYSTEM::HardwareUseLittleEndian()
{
  switch(GetTypeHardware())
    {
      case XSYSTEM_HARDWARETYPE_UNKNOWN       : return false;

      case XSYSTEM_HARDWARETYPE_PC            : return true;
      case XSYSTEM_HARDWARETYPE_LEX3V700A     : return true;

      case XSYSTEM_HARDWARETYPE_ARTILA500     : return false;
      case XSYSTEM_HARDWARETYPE_GUMSTIXCONNEX : return false;

      case XSYSTEM_HARDWARETYPE_RASPBERRYPI   : return true;
      case XSYSTEM_HARDWARETYPE_ODROIDC1      : return true;
      case XSYSTEM_HARDWARETYPE_ARMGENERIC    : return true;
    }

  return false;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSYSTEM_SO XSYSTEM::GetTypeSO()
* @brief      Get Type of System Operative 
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 16:57:33
*
* @return     XSYSTEM_SO : type SO  
*
*---------------------------------------------------------------------------------------------------------------------*/
XSYSTEM_SO XSYSTEM::GetTypeSO()                                              
{ 
  return XSYSTEM_SO_UNKNOWN;                        
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XSYSTEM::CheckTypeSO(XSYSTEM_SO typeSO)
* @brief      Check Type System Operative 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 16:58:49
*
* @param[in]  typeSO : type SO to check 
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XSYSTEM::CheckTypeSO(XSYSTEM_SO typeSO)
{
  XSYSTEM_SO _typeSO = GetTypeSO();

  if((XWORD)typeSO &(XWORD)_typeSO) return true;

  return false;
}




 
/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XLANGUAGE_CODE XSYSTEM::GetLanguage()
* @brief      Get Language Code
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 17:00:58
*
* @return     XLANGUAGE_CODE : Language code (enum XLANGUAGE_CODE)
*
*---------------------------------------------------------------------------------------------------------------------*/
XLANGUAGE_CODE XSYSTEM::GetLanguage()                                              
{ 
  return XLANGUAGE_UNKNOWN;                         
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XSYSTEM::GetMemoryInfo(XDWORD& total,XDWORD& free)
* @brief      GetMemoryInfo
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 17:01:36
*
* @param[out] total : total memory in bytes.
* @param[out] free :  free  memory in bytes
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XSYSTEM::GetMemoryInfo(XDWORD& total,XDWORD& free)
{
  total = 0;
  free  = 0;

  return false;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         int XSYSTEM::GetFreeMemoryPercent()
* @brief      Get Free Memory Percent 
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 17:02:23
*
* @return     int : 
*
*---------------------------------------------------------------------------------------------------------------------*/
int XSYSTEM::GetFreeMemoryPercent()
{
  XDWORD total;
  XDWORD free;
  int    percent;

  if(!GetMemoryInfo(total,free)) return false;

  percent = ((100*free)/total);

  return percent;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XSYSTEM::MakeCommand(XCHAR* command, int* returncode)
* @brief      Make Command (command of SO)
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 17:06:52
*
* @param[in]  command : command to make
* @param[out] returncode : return code of command
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XSYSTEM::MakeCommand(XCHAR* command, int* returncode)        
{ 
  return false;                                     
}
  


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XSYSTEM::MakeCommand(XSTRING& command, int* returncode)
* @brief      Make Command (command of SO)
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 17:07:37
*
* @param[in]  command : command to make
* @param[out  returncode : return code of command
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XSYSTEM::MakeCommand(XSTRING& command, int* returncode)      
{ 
  return MakeCommand(command.Get(), returncode);    
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XSYSTEM::ExecuteApplication(XCHAR* applicationnamefile, XCHAR* params)
* @brief      Execute Application
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 17:09:21
*
* @param[in]  applicationnamefile : path to application file
* @param[in]  params : params to send to exec of this application
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XSYSTEM::ExecuteApplication(XCHAR* applicationnamefile, XCHAR* params)
{
  return false;
}


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XSYSTEM::IsApplicationRunning(XCHAR* applicationname, XDWORD* ID)
* @brief      Check if Is Application Running
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 17:10:00
*
* @param[in]  applicationname : name of application
* @param[out] ID : ID of this application of SO
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XSYSTEM::IsApplicationRunning(XCHAR* applicationname, XDWORD* ID)             
{ 
  return false;                                     
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XSYSTEM::ShutDown(XSYSTEM_SHUTDOWNTYPE type)
* @brief      ShutDown
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 11:52:37
*
* @param[in]  type : type of shutdown (enum XSYSTEM_SHUTDOWNTYPE)
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XSYSTEM::ShutDown(XSYSTEM_SHUTDOWNTYPE type)
{
  return false;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XSYSTEM::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       02/06/2018 17:08:55
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XSYSTEM::Clean()                                              
{                                                   

}
















