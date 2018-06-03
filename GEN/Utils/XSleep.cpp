/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XSleep.cpp
*
* @class       XSLEEP
* @brief       Sleep class
* @ingroup     UTILS
*
* @author      Abraham J. Velez 
* @date        03/06/2018 18:33:41
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

#include "XSleep.h"

#include "XMemory.h"


/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

XSLEEP* XSLEEP::instance = NULL;


/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/


/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSLEEP::XSLEEP()
* @brief      Constructor
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 18:34:28
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSLEEP::XSLEEP()
{
  Clean();
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSLEEP::~XSLEEP()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 18:34:38
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSLEEP::~XSLEEP()
{
  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XSLEEP::GetIsInstanced()
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
bool XSLEEP::GetIsInstanced() 
{
  return instance!=NULL;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSLEEP& XSLEEP::GetInstance()
* @brief      GetInstance
* @note       STATIC
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 10:18:58
*
* @return     XSLEEP& : 
*
*---------------------------------------------------------------------------------------------------------------------*/
XSLEEP& XSLEEP::GetInstance()
{
  if(!instance) instance = new XSLEEP();

  return (*instance);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XSLEEP::SetInstance(XSLEEP* instance)
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
bool XSLEEP::SetInstance(XSLEEP* _instance)
{
  if(!_instance) return false;

  instance = _instance;

  return (instance)?true:false;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XSLEEP::DelInstance()
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
bool XSLEEP::DelInstance()
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
* @fn         void XSLEEP::Seconds(int seconds)
* @brief      Seconds
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 18:36:34
*
* @param[in]  seconds : 
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XSLEEP::Seconds(int seconds)                           
{                   

}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XSLEEP::MilliSeconds(int milliseconds)
* @brief      MilliSeconds
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 18:36:39
*
* @param[in]  milliseconds : 
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XSLEEP::MilliSeconds(int milliseconds)                      
{ 

}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XSLEEP::MicroSeconds(int microseconds)
* @brief      MicroSeconds
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 18:36:45
*
* @param[in]  microseconds : 
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XSLEEP::MicroSeconds(int microseconds)
{ 


}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XSLEEP::NanoSeconds(int nanoseconds)
* @brief      NanoSeconds
* @note       VIRTUAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 18:36:52
*
* @param[in]  nanoseconds : 
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XSLEEP::NanoSeconds(int nanoseconds)
{

}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XSLEEP::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    UTILS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 18:37:00
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XSLEEP::Clean()
{

}