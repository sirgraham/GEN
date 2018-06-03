/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XWINDOWSSharedMemory.h
*
* @class       XWINDOWSSHAREDMEMORY
* @brief       Windows Shared Memory (Exchange between applications)
* @ingroup     PLATFORM_WINDOWS
*
* @author      Abraham J. Velez 
* @date        23/05/2018 12:49:36
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

#ifndef _XWINDOWSSHAREDMEMORY_H_
#define _XWINDOWSSHAREDMEMORY_H_

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <windows.h>

#include "XSharedMemory.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/


/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class XWINDOWSSHAREDMEMORY : public XSHAREDMEMORY
{
  public:
                            XWINDOWSSHAREDMEMORY    ()    { Clean();                            }
    virtual                ~XWINDOWSSHAREDMEMORY    ()    { Clean();                            }

    XBYTE*                  Create                  (XCHAR* ID, XDWORD size);   
    XBYTE*                  Open                    (XCHAR* ID, XDWORD& size);   

    bool                    Close                   ();

  private:

    void                    Clean                   ()
                            {
    
                            }

     HANDLE                handlefile;
};

/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/

#endif


