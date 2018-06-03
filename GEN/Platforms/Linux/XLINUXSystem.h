/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XLINUXSystem.h
*
* @class       XLINUXSYSTEM
* @brief       Linux System class
* @ingroup     PLATFORM_LINUX
*
* @author      Abraham J. Velez 
* @date        03/06/2018 13:32:25
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

#ifndef _XLINUXSYSTEM_H_
#define _XLINUXSYSTEM_H_

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XSystem.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XLINUXSYSTEM : public XSYSTEM
{
  public:
                            XLINUXSYSTEM                ();
    virtual                ~XLINUXSYSTEM                ();

    XSYSTEM_HARDWARETYPE    GetTypeHardware             (int* revision = NULL);

    XSYSTEM_SO              GetTypeSO                   ();

    XLANGUAGE_CODE          GetLanguage                 ();

    bool                    GetMemoryInfo               (XDWORD& total,XDWORD& free);

    bool                    MakeCommand                 (XCHAR* command, int* returncode = NULL);

    bool                    ExecuteApplication          (XCHAR* command, XCHAR* params = NULL);

    bool                    IsApplicationRunning        (XCHAR* command, XDWORD* ID = NULL);

    bool                    ShutDown                    (XSYSTEM_SHUTDOWNTYPE type);

  private:

    void                    Clean                       ();

};


/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/

#endif

