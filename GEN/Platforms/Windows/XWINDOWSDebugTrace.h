/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XWINDOWSDebugTrace.h
*
* @class       XWINDOWSDEBUGTRACE
* @brief       Windows debug trace class
* @ingroup     PLATFORM_WINDOWS
*
* @author      Abraham J. Velez 
* @date        31/05/2018 20:34:50
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

#ifndef _XWINDOWSDEBUGTRACE_H_
#define _XWINDOWSDEBUGTRACE_H_

#ifdef XDEBUG_TRACE

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XBase.h"
#include "XString.h"
#include "DIOURL.h"

#include "XWINDOWSFactory.h"
#include "XWINDOWSDateTime.h"

#include "XDebugTrace.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/


class XWINDOWSDEBUGTRACE : public XDEBUGTRACE
{
  public:
                        XWINDOWSDEBUGTRACE      ();
    virtual            ~XWINDOWSDEBUGTRACE      ();

    void                PrintSpecial            (XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string);
    void                PrintFile               (XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string);
    void                PrintNet                (XDEBUGTRACE_TARGET* target, XBYTE level, XCHAR* string);

    bool                GetHandleNet            (XDEBUGTRACE_TARGET* target);
    bool                CloseHandleNet          (XDEBUGTRACE_TARGET* target);

  private:

    bool                Lock                    ();
    bool                UnLock                  (); 
    
    void                Clean                   ();
                        

    XWINDOWSDATETIME    xdatetime;
    XDWORD              mutexhandle;
};


/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/


#endif

#endif


