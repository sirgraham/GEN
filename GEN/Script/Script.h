//------------------------------------------------------------------------------------------
//  SCRIPT.H
//
/**
// \class
//
//  Script Interpreter (Mini CPP interpreter)
//
//  @author  Abraham J. Velez
//  @version 17/07/2007 17:44:28
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _SCRIPT_H_
#define _SCRIPT_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XVector.h"
#include "XMap.h"
#include "XSubject.h"
#include "XString.h"
#include "XPath.h"
#include "XEvent.h"
#include "XVariant.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


enum SCRIPTERRORMSG
{
  SCRIPTERRORMSG_NONE           ,   //00 "None"
  SCRIPTERRORMSG_INTERNALERROR  ,   //01 "Internal error"
  SCRIPTERRORMSG_SYNTAX         ,   //02 "Syntax error"
  SCRIPTERRORMSG_NO_EXP         ,   //03 "No expression present"
  SCRIPTERRORMSG_NOT_VAR        ,   //04 "Not a variable"
  SCRIPTERRORMSG_DUP_VAR        ,   //05 "Duplicate variable name"
  SCRIPTERRORMSG_DUP_FUNC       ,   //06 "Duplicate function name"
  SCRIPTERRORMSG_SEMI_EXPECTED  ,   //07 "Semicolon expected"
  SCRIPTERRORMSG_UNBAL_BRACES   ,   //08 "Unbalanced braces"
  SCRIPTERRORMSG_FUNC_UNDEF     ,   //09 "Function undefined"
  SCRIPTERRORMSG_TYPE_EXPECTED  ,   //10 "Type specifier expected"
  SCRIPTERRORMSG_RET_NOCALL     ,   //11 "Return without call"
  SCRIPTERRORMSG_PAREN_EXPECTED ,   //12 "Parentheses expected"
  SCRIPTERRORMSG_WHILE_EXPECTED ,   //13 "While expected"
  SCRIPTERRORMSG_QUOTE_EXPECTED ,   //14 "Closing quote expected"
  SCRIPTERRORMSG_DIV_BY_ZERO    ,   //15 "Division by zero"
  SCRIPTERRORMSG_BRACE_EXPECTED ,   //16 "{ expected (control statements must use blocks)"
  SCRIPTERRORMSG_COLON_EXPECTED ,   //17 "Colon expected"
  SCRIPTERRORMSG_INSUF_PARAMS   ,   //18 "Insufficient parameters"
  SCRIPTERRORMSG_USERBREAK          //19 "Break by user"
};


enum SCRIPTXEVENTTYPE
{
  SCRIPTXEVENTTYPE_UNKNOWN      = XEVENTTYPE_SCRIPT ,
  SCRIPTXEVENTTYPE_ERROR                            ,
  SCRIPTXEVENTTYPE_BREAK                            ,
};




class SCRIPT;
class SCRIPTLIB;

//typedef void (*SCRFUNCIONLIBRARY) (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);


typedef void (*SCRFUNCIONLIBRARY) (SCRIPTLIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue);

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTIMER;
class XTHREADCOLLECTED;
class XPUBLISHER;
class XFILETXT;
class SCRIPTVAR;



class SCRIPTXEVENT : public XEVENT
{
  public:
                                        SCRIPTXEVENT            (XSUBJECT* subject, XDWORD type = SCRIPTXEVENTTYPE_UNKNOWN, XDWORD family = XEVENTTYPE_SCRIPT) : XEVENT(subject, type, family)
                                        {
                                          Clean();
                                        }

    virtual                            ~SCRIPTXEVENT            ()
                                        {
                                          Clean();
                                        }

    XSTRING*                            GetNameScript           ()                                    { return &namescript;                           }

    SCRIPTERRORMSG                      GetError                ()                                    { return error;                                 }
    void                                SetError                (SCRIPTERRORMSG error)                { this->error = error;                          }

    XSTRING*                            GetErrorText            ()                                    { return &errortext;                            }

    int                                 GetNLine                ()                                    { return nline;                                 }
    void                                SetNLine                (int nline)                           { this->nline = nline;                          }

    XSTRING*                            GetCurrentToken         ()                                    { return &currenttoken;                         }

  private:

    void                                Clean                   ()
                                        {
                                          error = SCRIPTERRORMSG_NONE;
                                        }

    XSTRING                             namescript;
    SCRIPTERRORMSG                      error;
    XSTRING                             errortext;
    XSTRING                             currenttoken;
    int                                 nline;
};




class SCRIPTLIBRARYFUNCTION
{
  public:
                                        SCRIPTLIBRARYFUNCTION   (SCRIPTLIB* library, XCHAR* name, SCRFUNCIONLIBRARY functionlibrary)
                                        {
                                          Clean();

                                          this->library         = library;
                                          this->name            = name;
                                          this->functionlibrary = functionlibrary;
                                        }


                                        SCRIPTLIBRARYFUNCTION   (SCRIPTLIB* library, XSTRING& name, SCRFUNCIONLIBRARY functionlibrary)
                                        {
                                          Clean();

                                          this->library         = library;
                                          this->name            = name;
                                          this->functionlibrary = functionlibrary;
                                        }

    virtual                            ~SCRIPTLIBRARYFUNCTION   ()
                                        {
                                          Clean();
                                        }

    SCRIPTLIB*                          GetLibrary              ()                                    { return library;                             }
    bool                                SetLibrary              (SCRIPTLIB* library)
                                        {
                                          if(!library) return false;
                                          this->library = library;
                                          return true;
                                        }

    XSTRING*                            GetName                 ()                                    { return &name;                               }

    SCRFUNCIONLIBRARY                   GetFunctionLibrary      ()                                    { return functionlibrary;                     }
    bool                                SetFunctionLibrary      (SCRFUNCIONLIBRARY functionlibrary)
                                        {
                                          this->functionlibrary = functionlibrary;
                                          return true;
                                        }

  private:

    void                                Clean                   ()
                                        {
                                          name.Empty();
                                          functionlibrary = NULL;
                                        }

    SCRIPTLIB*                          library;
    XSTRING                             name;
    SCRFUNCIONLIBRARY                   functionlibrary;
};



class SCRIPT : public XSUBJECT
{
  public:
                                        SCRIPT                  ();
    virtual                            ~SCRIPT                  ();

    virtual bool                        Load                    (XPATH& xpath);
    virtual bool                        Save                    (XPATH& xpath);

    static bool                         IsScript                (XPATH& xpath, XCHAR* extension);

    XSTRING*                            GetNameScript           ()                                    { return &namescript;                     }
    XSTRING*                            GetScript               ()                                    { return &script;                         }
    XPATH*                              GetPath                 ()                                    { return &xpath;                          }

    virtual SCRIPTERRORMSG              Run                     (int* returnval = NULL);

    bool                                RunWithThread           ();
    bool                                IsRunWithThread         ();
    bool                                IsRunThread             (SCRIPTERRORMSG* error = NULL,int* returnvalue = NULL);

    bool                                CancelExecution         ();

    XTIMER*                             GetTimer                ()                                    { return xtimer;                          }
    XTHREADCOLLECTED*                   GetThread               ()                                    { return thread;                          }

    SCRIPTLIB*                          GetLibrary              (XCHAR* ID);
    bool                                AddLibrary              (SCRIPTLIB* scriptlib);
    bool                                DeleteAllLibrarys       ();

    SCRIPTLIBRARYFUNCTION*              GetLibraryFunction      (XCHAR* name) ;
    bool                                AddLibraryFunction      (SCRIPTLIB* library, XCHAR* name, SCRFUNCIONLIBRARY ptrfunction);
    bool                                DeleteLibraryFuncions   ();


    SCRIPTERRORMSG                      GetErrorScript          ();
    bool                                SetErrorScript          (SCRIPTERRORMSG errorscript);

    virtual bool                        HaveError               (SCRIPTERRORMSG errorcode);

  protected:

    XPATH                               xpath;
    XFILETXT*                           xfiletxt;
    XSTRING                             namescript;
    XSTRING                             script;

    XTIMER*                             xtimer;
    SCRIPTERRORMSG                      errorscript;

    XTHREADCOLLECTED*                   thread;
    bool                                isrunwiththread;

    int                                 returnvaluescript;

    bool                                iscancelexec;

    XVECTOR<SCRIPTLIB*>                 librarys;
    XVECTOR<SCRIPTLIBRARYFUNCTION*>     libraryfunctions;

  private:

    void                                Clean                   ()
                                        {
                                          xpath.Empty();
                                          xfiletxt            = NULL;
                                          namescript          = __L("N/A");
                                          script.Empty();

                                          xtimer              = NULL;
                                          errorscript         = SCRIPTERRORMSG_NONE;

                                          thread              = NULL;
                                          isrunwiththread     = false;

                                          returnvaluescript   = 0;

                                          iscancelexec        = false;
                                        }


    static void                         ThreadFunction          (void* data);



};

//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

