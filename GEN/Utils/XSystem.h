/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XSystem.h
*
* @class       XSYSTEM
* @brief       System class
* @note        Can´t be construct Factory + singelton without depends of system. IT´S NOT A SINGLETON.
* @ingroup     UTILS
*
* @author      Abraham J. Velez 
* @date        02/06/2018 16:56:14
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

#ifndef _XSYSTEM_H_
#define _XSYSTEM_H_

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include "XLanguage.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XSYSTEM_HARDWARETYPE
{
  XSYSTEM_HARDWARETYPE_UNKNOWN           = 0  ,

  XSYSTEM_HARDWARETYPE_PC                     ,
  XSYSTEM_HARDWARETYPE_LEX3V700A              ,

  XSYSTEM_HARDWARETYPE_GUMSTIXCONNEX          ,
  XSYSTEM_HARDWARETYPE_RASPBERRYPI            ,
  XSYSTEM_HARDWARETYPE_ODROIDC1               ,
  XSYSTEM_HARDWARETYPE_ARTILA500              ,
  XSYSTEM_HARDWARETYPE_ARMGENERIC             ,

  XSYSTEM_HARDWARETYPE_MICRO_STM32FXXX        ,

  XSYSTEM_HARDWARETYPE_END
};


enum XSYSTEM_SO
{
  XSYSTEM_SO_UNKNOWN                = 0x0000  ,

  XSYSTEM_SO_WINDOWS                = 0x0100  ,
  XSYSTEM_SO_WINDOWS95                        ,
  XSYSTEM_SO_WINDOWS98                        ,
  XSYSTEM_SO_WINDOWSNT4                       ,
  XSYSTEM_SO_WINDOWS2000                      ,
  XSYSTEM_SO_WINDOWSXP                        ,
  XSYSTEM_SO_WINDOWSSERVER2003                ,
  XSYSTEM_SO_WINDOWSVISTA                     ,
  XSYSTEM_SO_WINDOWS7                         ,
  XSYSTEM_SO_WINDOWS8                         ,
  XSYSTEM_SO_WINDOWS81                        ,
  XSYSTEM_SO_WINDOWS10                        ,

  XSYSTEM_SO_LINUX                  = 0x0200  ,

  XSYSTEM_SO_LINUX_EMBEDDED         = 0x0400  ,

  XSYSTEM_SO_ANDROID                = 0x0800  ,
};


enum XSYSTEM_SHUTDOWNTYPE
{
  XSYSTEM_SHUTDOWNTYPE_UNKNOWN      = 0       ,
  XSYSTEM_SHUTDOWNTYPE_REBOOT                 ,
  XSYSTEM_SHUTDOWNTYPE_LOGOFF                 ,
  XSYSTEM_SHUTDOWNTYPE_POWEROFF               ,
};



/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XSYSTEM
{  
  public:
                                    XSYSTEM                         ();                                    
    virtual                        ~XSYSTEM                         ();
  
    static bool                     GetIsInstanced                  ();                       
    static XSYSTEM&                 GetInstance                     ();                      
    static bool                     SetInstance                     (XSYSTEM* instance);                      
    static bool                     DelInstance                     ();
                       
    virtual XSYSTEM_HARDWARETYPE    GetTypeHardware                 (int* revision = NULL);
    
    bool                            HardwareUseLittleEndian         ();

    virtual XSYSTEM_SO              GetTypeSO                       ();
    bool                            CheckTypeSO                     (XSYSTEM_SO SOtoCheck);

    virtual XLANGUAGE_CODE          GetLanguage                     ();

    virtual bool                    GetMemoryInfo                   (XDWORD& total,XDWORD& free);    
    int                             GetFreeMemoryPercent            ();

    virtual bool                    MakeCommand                     (XCHAR* command, int* returncode = NULL);
    bool                            MakeCommand                     (XSTRING& command, int* returncode = NULL);
    virtual bool                    ExecuteApplication              (XCHAR* command, XCHAR* params = NULL);    
    virtual bool                    IsApplicationRunning            (XCHAR* command, XDWORD* ID = NULL);

    virtual bool                    ShutDown                        (XSYSTEM_SHUTDOWNTYPE type);

  private:

    void                            Clean                           ();
    
    static XSYSTEM*                 instance;
};

/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/

#endif

