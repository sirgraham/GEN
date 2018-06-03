/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XWINDOWSSleep.cpp
*
* @class       XWINDOWSSLEEP
* @brief       Windows sleep class
* @ingroup     PLATFORM_WINDOWS
*
* @author      Abraham J. Velez 
* @date        03/06/2018 18:43:47
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

#include <math.h>

#include "XWINDOWSSleep.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWINDOWSSLEEP::XWINDOWSSLEEP()
* @brief      Constructor
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 18:44:09
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSLEEP::XWINDOWSSLEEP()
{
  Clean();

  QueryPerformanceFrequency(&frequency);

  PCfrequencymilliseconds = (double)(frequency.QuadPart) / 1000.0;
  PCfrequencymicroseconds = (double)(frequency.QuadPart) / 1000000.0;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XWINDOWSSLEEP::~XWINDOWSSLEEP()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 18:44:31
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XWINDOWSSLEEP::~XWINDOWSSLEEP()
{
  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XWINDOWSSLEEP::Seconds(int seconds)
* @brief      Sleep n Seconds
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 18:45:44
*
* @param[in]  seconds : seconds to sleep
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSSLEEP::Seconds(int seconds)
{
  Sleep(seconds*1000);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XWINDOWSSLEEP::MilliSeconds(int milliseconds)
* @brief      Sleep n MilliSeconds
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 18:46:55
*
* @param[in]  milliseconds : milliseconds to sleep
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSSLEEP::MilliSeconds(int milliseconds)
{
  Sleep(milliseconds);
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XWINDOWSSLEEP::MicroSeconds(int microseconds)
* @brief      Sleep n MicroSeconds
* @ingroup    PLATFORM_WINDOWS
*
* @author     Abraham J. Velez 
* @date       03/06/2018 18:47:36
*
* @param[in]  microseconds : microseconds to sleep
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XWINDOWSSLEEP::MicroSeconds(int microseconds)
{
  __int64 timeellapsed;
  __int64 timestart;
  __int64 timedelta;

  QueryPerformanceFrequency((LARGE_INTEGER*)(&timedelta));

  __int64 timetowait = (__int64)((XDWORD)timedelta * ((XDWORD)microseconds / 1000000.0f));

  QueryPerformanceCounter ((LARGE_INTEGER*)(&timestart));

  timeellapsed = timestart;

  while(( timeellapsed - timestart ) < timetowait)
   {
     QueryPerformanceCounter( (LARGE_INTEGER*)(&timeellapsed ) );
   };
}



