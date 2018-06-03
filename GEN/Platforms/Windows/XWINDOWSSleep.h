/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XWINDOWSSleep.h
*
* @class       XWINDOWSSLEEP
* @brief       Windows Sleep class
* @ingroup     PLATFORM_WINDOWS
*
* @author      Abraham J. Velez 
* @date        03/06/2018 18:42:43
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

#ifndef _XWINDOWSSLEEP_H_
#define _XWINDOWSSLEEP_H_

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <windows.h>
#include <time.h>

#include "XBase.h"
#include "XSleep.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XWINDOWSSLEEP : public XSLEEP
{
  public:

                                XWINDOWSSLEEP                     ();
    virtual                    ~XWINDOWSSLEEP                     ();

    void                        Seconds                           (int seconds);
    void                        MilliSeconds                      (int milliseconds);
    void                        MicroSeconds                      (int microseconds);

    void                        Clean                             ()
                                {
                                  PCfrequencymilliseconds = 0.0;
                                  PCfrequencymicroseconds = 0.0;
                                }

  private:

    LARGE_INTEGER               frequency;
    double                      PCfrequencymilliseconds;
    double                      PCfrequencymicroseconds;
};



/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/

#endif


