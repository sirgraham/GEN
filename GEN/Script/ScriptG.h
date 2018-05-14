//------------------------------------------------------------------------------------------
//  SCRIPTG.H
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

#ifndef _SCRIPTG_H_
#define _SCRIPTG_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XVector.h"
#include "XMap.h"
#include "XSubject.h"
#include "XString.h"
#include "XPath.h"
#include "XEvent.h"

#include "Script.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

class XFILETXT;

enum SCRIPTGTOKENTYPES
{
  SCRIPTGTOKENTYPES_UNDEFTT     ,
  SCRIPTGTOKENTYPES_DELIMITER   ,
  SCRIPTGTOKENTYPES_IDENTIFIER    ,
  SCRIPTGTOKENTYPES_NUMBER        ,
  SCRIPTGTOKENTYPES_KEYWORD     ,
  SCRIPTGTOKENTYPES_TEMP          ,
  SCRIPTGTOKENTYPES_STRING        ,
  SCRIPTGTOKENTYPES_BLOCK
};

enum SCRIPTGDOUBLEOPERATOR
{
  SCRIPTGDOUBLEOPERATOR_LT  = 1   ,
  SCRIPTGDOUBLEOPERATOR_LE        ,
  SCRIPTGDOUBLEOPERATOR_GT        ,
  SCRIPTGDOUBLEOPERATOR_GE        ,
  SCRIPTGDOUBLEOPERATOR_EQ        ,
  SCRIPTGDOUBLEOPERATOR_NE        ,
  SCRIPTGDOUBLEOPERATOR_LS        ,
  SCRIPTGDOUBLEOPERATOR_RS        ,
  SCRIPTGDOUBLEOPERATOR_INC     ,
  SCRIPTGDOUBLEOPERATOR_DEC
};


enum SCRIPTGTOKENIREPS
{
  SCRIPTGTOKENIREPS_UNDEFTOK      ,
  SCRIPTGTOKENIREPS_CHAR          ,
  SCRIPTGTOKENIREPS_INT           ,
  SCRIPTGTOKENIREPS_STRING        ,
  SCRIPTGTOKENIREPS_SWITCH        ,
  SCRIPTGTOKENIREPS_CASE          ,
  SCRIPTGTOKENIREPS_IF            ,
  SCRIPTGTOKENIREPS_ELSE          ,
  SCRIPTGTOKENIREPS_FOR           ,
  SCRIPTGTOKENIREPS_DO            ,
  SCRIPTGTOKENIREPS_WHILE         ,
  SCRIPTGTOKENIREPS_BREAK         ,
  SCRIPTGTOKENIREPS_RETURN        ,
  SCRIPTGTOKENIREPS_END
};



#define SCRIPTG_MAXIDLEN       31    // max ID     length
#define SCRIPTG_MAXTOKENLEN   128    // max token  length
#define SCRIPTG_NUMPARAMS      31    // max params lenght

#define SCRIPTG_EXTENSION   __L(".G")


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XTIMER;
class XTHREADCOLLECTED;
class XPUBLISHER;
class SCRIPTLIB;


class SCRIPTGCOMMAND
{
  public:
                                        SCRIPTGCOMMAND          ();
                                       ~SCRIPTGCOMMAND          ();

    XSTRING*                            GetCommand              ();
    SCRIPTGTOKENIREPS                   GetToken                ();

    bool                                Set                     (XCHAR* command,SCRIPTGTOKENIREPS token);
    bool                                Set                     (XSTRING& command,SCRIPTGTOKENIREPS token);

  private:

    void                                Clean                   ();

    XSTRING                             command;
    SCRIPTGTOKENIREPS                   token;
};





union SCRIPTGVARVALUE
{
    int                                 integer;
    XCHAR                               character;
    XDWORD                              uinteger;
    XSTRING*                            string;
};





class SCRIPTGFUNCTIONTYPE
{
  public:
                                        SCRIPTGFUNCTIONTYPE     ();
    virtual                            ~SCRIPTGFUNCTIONTYPE     ();

    XSTRING*                            GetName                 ();
    SCRIPTGTOKENIREPS                   GetReturnType           ();
    XCHAR*                              GetLocation             ();

    bool                                SetName                 (XCHAR* name);
    bool                                SetName                 (XSTRING& name);
    bool                                SetReturnType           (SCRIPTGTOKENIREPS returntype);
    bool                                SetLocation             (XCHAR* location);

  private:

    void                                Clean                   ();

    XSTRING                             name;
    SCRIPTGTOKENIREPS                   returntype;
    XCHAR*                              location;
};




class SCRIPTGVAR
{
  public:

                                        SCRIPTGVAR              ();
    virtual                            ~SCRIPTGVAR              ();

    XSTRING*                            GetName                 ();
    SCRIPTGTOKENIREPS                   GetType                 ();

    int                                 GetValueInteger         ();
    XCHAR                               GetValueCharacter       ();
    XDWORD                              GetValueUInteger        ();
    XSTRING*                            GetValueString          ();

    bool                                NotHaveReservedSize     ();
    bool                                IsArg                   ();
    bool                                IsReturnValue           ();

    bool                                Set                     (SCRIPTGVAR* var);

    bool                                SetName                 (XCHAR* name);
    bool                                SetType                 (SCRIPTGTOKENIREPS type);

    bool                                SetValueInteger         (int value);
    bool                                SetValueCharacter       (XCHAR value);
    bool                                SetValueUInteger        (XDWORD value);
    bool                                SetValueString          (XSTRING* value);
    bool                                SetValueString          ()
                                        {
                                          value.string = NULL;
                                          return true;
                                        }

    bool                                SetNotHaveReservedSize  (bool nothavereservedsize);
    bool                                SetIsArg                (bool isarg);
    bool                                SetIsReturnValue        (bool isreturnvalue);

    bool                                ConvertToXVariant       (XVARIANT& variant);
    bool                                ConvertFromXVariant     (XVARIANT& variant);

    bool                                Clear                   ();

  private:

    void                                Clean                   ();

    XSTRING                             name;
    SCRIPTGTOKENIREPS                   type;
    SCRIPTGVARVALUE                     value;
    bool                                nothavereservedsize;
    bool                                isarg;
    bool                                isreturnvalue;
};



class SCRIPTG : public SCRIPT
{
  public:
                                        SCRIPTG                 ();
    virtual                            ~SCRIPTG                 ();


    SCRIPTERRORMSG                      Run                     (int* returnval = NULL);

    bool                                AddCommand              (XCHAR* command, SCRIPTGTOKENIREPS  token);
    bool                                DeleteCommands          ();

    int                                 GetFuncParams           (SCRIPTGVAR* params);
    void                                NotFuncParams           ();

    int                                 GetReturnValueScript    ();
    int                                 SetReturnValueScript    (int returnvaluescript);

    bool                                HaveError               (SCRIPTERRORMSG errorcode);


  private:

    void                                Clean                   ()
                                        {
                                          returnvaluescript   = 0;

                                          ipprg               = NULL;

                                          iscancelexec        = false;

                                          tokentype           = SCRIPTGTOKENTYPES_UNDEFTT;
                                          tokenireps          = SCRIPTGTOKENIREPS_UNDEFTOK;

                                          returnfound         = false;
                                          breakfound          = false;

                                          int c;

                                          for(c=0;c<SCRIPTG_MAXTOKENLEN+1;c++)
                                            {
                                              currenttoken[c] = 0;
                                            }

                                          for(c=0;c<_MAXSTR;c++)
                                            {
                                              currentfunction[c] = 0;
                                            }

                                          returnvalue.SetValueInteger(0);
                                        }


    bool                                DeleteVarsExec          ();

    bool                                IsSpace                 (XCHAR character);
    bool                                IsDigit                 (XCHAR character);
    bool                                IsAlpha                 (XCHAR character);

    bool                                IsDelimiter             (XCHAR  c);
    SCRIPTGTOKENIREPS                   LookUpToken             (XCHAR* s);
    void                                PutBackToken            ();

    void                                EvalExp                 (SCRIPTGVAR& value);
    void                                EvalExp0                (SCRIPTGVAR& value);
    void                                EvalExp1                (SCRIPTGVAR& value);
    void                                EvalExp2                (SCRIPTGVAR& value);
    void                                EvalExp3                (SCRIPTGVAR& value);
    void                                EvalExp4                (SCRIPTGVAR& value);
    void                                EvalExp5                (SCRIPTGVAR& value);
    void                                Atom                    (SCRIPTGVAR& value);

    SCRIPTGVAR*                         IsVariable              (XCHAR* variblename);
    void                                AssignVariable          (XCHAR* variablename,SCRIPTGVAR& value);
    SCRIPTGVAR*                         FindVariable            (XCHAR* variablename);
    SCRIPTGTOKENIREPS                   FindVariableType        (XCHAR* variablename);

    XCHAR*                              FindFunction            (XCHAR* name);
    SCRIPTGFUNCTIONTYPE*                GetFunction             (XCHAR* name);
    void                                FindEndofBlock          ();

    void                                DeclareGlobalVariable   ();
    void                                DeclareLocalVariable    ();

    SCRIPTGTOKENTYPES                   GetToken                ();

    void                                PreScan                 ();

    void                                GetArgs                 ();
    void                                GetParams               ();

    void                                FunctionReturn          ();

    void                                Exec_IF                 ();
    void                                Exec_SWITCH             ();
    void                                Exec_WHILE              ();
    void                                Exec_DO                 ();
    void                                Exec_FOR                ();

    bool                                Interpret               ();

    void                                Call                    ();

    static void                         ThreadFunction          (void* data);

    int                                 returnvaluescript;

    XCHAR*                              ipprg;

    XVECTOR<SCRIPTGCOMMAND*>            commands;

    XVECTOR<SCRIPTLIB*>                 librarys;

    XVECTOR<SCRIPTLIBRARYFUNCTION*>     libraryfunctions;

    XVECTOR<SCRIPTGVAR*>                globalvars;
    XVECTOR<SCRIPTGFUNCTIONTYPE*>       functiontable;
    XVECTOR<SCRIPTGVAR*>                localvarsstack;
    XVECTOR<int>                        functioncallstack;
    XVECTOR<int>                        nestscopestack;

    SCRIPTGTOKENTYPES                   tokentype;
    SCRIPTGTOKENIREPS                   tokenireps;

    bool                                breakfound;
    bool                                returnfound;

    XCHAR                               currenttoken[SCRIPTG_MAXTOKENLEN+1];
    XCHAR                               currentfunction[_MAXSTR];
    SCRIPTGVAR                          returnvalue;
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

