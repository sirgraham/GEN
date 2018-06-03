/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XApplication.h
*
* @class       XAPPLICATION
* @brief       Application Base class
* @ingroup     UTILS
*
* @author      Abraham J. Velez 
* @date        25/05/2018 18:02:50
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

#ifndef _XAPPLICATION_H_
#define _XAPPLICATION_H_

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>

#include "XString.h"
#include "XDebugTrace.h"
#include "XObserver.h"
#include "XSubject.h"

/*---- DEFINES & ENUMS  ----------------------------------------------------------------------------------------------*/

enum XAPPLICATIONEXITTYPE
{
  XAPPLICATIONEXITTYPE_NONE             = 0 ,
  XAPPLICATIONEXITTYPE_APPERROR             ,
  XAPPLICATIONEXITTYPE_APPEND               ,  //append en ingles significa añadir, igual seria mejor simplemente END o APP_END
  XAPPLICATIONEXITTYPE_BYUSER               ,
  XAPPLICATIONEXITTYPE_SOSHUTDOWN           ,
  XAPPLICATIONEXITTYPE_INVALIDLICENSE       ,
  XAPPLICATIONEXITTYPE_EXPIREDLICENSE       ,
  XAPPLICATIONEXITTYPE_NOTVCC               ,
  XAPPLICATIONEXITTYPE_UPDATE
};

/*---- CLASS ---------------------------------------------------------------------------------------------------------*/

class XSYSTEM;
class XTIMER;
class MAIN;


class XAPPLICATION : public XOBSERVER
{
  public:
                                  XAPPLICATION                  ();
    virtual                      ~XAPPLICATION                  ();

    static XAPPLICATION*          Create                        ();

    virtual bool                  IniApplication                (XVECTOR<XSTRING*>* execparams = NULL);
    bool                          UpdateApplication             ();
    bool                          EndApplication                ();

    virtual bool                  Ini                           ();
    virtual bool                  FirstUpdate                   ();
    virtual bool                  Update                        ();
    virtual bool                  LastUpdate                    ();
    virtual bool                  End                           ();

    XVECTOR<XSTRING*>*            GetExecParams                 ();

    XSTRING*                      GetApplicationName            ();
    void                          SetApplicationName            (XCHAR* name);
    XSTRING*                      GetApplicationID              ();
    
    MAIN*                         GetMain                       ();
    void                          SetMain                       (MAIN* main);

    XTIMER*                       GetTimerGlobal                ();


    XAPPLICATIONEXITTYPE          GetExitStatus                 ();
    void                          SetExitStatus                 (XAPPLICATIONEXITTYPE exitstatus);
                            
    virtual void                  HandleEvent                   (XEVENT* xevent);
     

  protected:

    XVECTOR<XSTRING*>*            execparams;
    MAIN*                         main;
    XTIMER*                       xtimerglobal;
    XAPPLICATIONEXITTYPE          exitstatus;


    XSTRING                       applicationname;
    XSTRING                       applicationID;

  private:

    void                          Clean                         ();
         

};


/*---- INLINE FUNCTIONS ----------------------------------------------------------------------------------------------*/

#endif



