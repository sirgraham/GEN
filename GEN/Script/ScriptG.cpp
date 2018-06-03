//------------------------------------------------------------------------------------------
//  SCRIPT.CPP
//
//  Script Interpreter (Mini CPP interpreter)
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 17/07/2007 17:46:23
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <ctype.h>

#include "XPath.h"
#include "XFactory.h"
#include "XFileTXT.h"
#include "XThread.h"
#include "XDebugTrace.h"
#include "XThreadCollected.h"
#include "XVariant.h"

#include "ScriptLib.h"
#include "ScriptG.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  SCRIPTGCOMMAND::SCRIPTGCOMMAND
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 13:06:40
//
//  @return
//  */
/*-----------------------------------------------------------------*/
SCRIPTGCOMMAND::SCRIPTGCOMMAND()
{
  Clean();
}



/*-------------------------------------------------------------------
//  SCRIPTGCOMMAND::~SCRIPTGCOMMAND
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 13:06:48
//
//  @return
//  */
/*-----------------------------------------------------------------*/
SCRIPTGCOMMAND::~SCRIPTGCOMMAND()
{
  Clean();
}


/*-------------------------------------------------------------------
//  SCRIPTGCOMMAND::GetCommand
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 15:57:58
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* SCRIPTGCOMMAND::GetCommand()
{
  return &command;
}




/*-------------------------------------------------------------------
//  SCRIPTGCOMMAND::GetToken
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 15:58:14
//
//  @return       SCRIPTGTOKENIREPS :
//  */
/*-----------------------------------------------------------------*/
SCRIPTGTOKENIREPS SCRIPTGCOMMAND::GetToken()
{
  return token;
}



/*-------------------------------------------------------------------
//  SCRIPTGCOMMAND::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 13:06:59
//
//  @return       bool :
//  @param        command :
//  @param        SCRIPTGTOKENIREPS token :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGCOMMAND::Set(XCHAR* command, SCRIPTGTOKENIREPS  token)
{
  if(!command) return false;

  this->command = command;
  this->token   = token;

  return true;
}



/*-------------------------------------------------------------------
//  SCRIPTGCOMMAND::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 22:41:14
//
//  @return       bool :
//  @param        command :
//  @param        token :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGCOMMAND::Set(XSTRING& command,SCRIPTGTOKENIREPS token)
{
  return Set(command.Get(),token);
}



/*-------------------------------------------------------------------
//  SCRIPTGCOMMAND::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 13:07:05
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void SCRIPTGCOMMAND::Clean()
{
  command.Empty();
  token = SCRIPTGTOKENIREPS_UNDEFTOK;
}







//-------------------------------------------------------------------
//  SCRIPTGVAR::SCRIPTGVAR
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/07/2007 13:48:43
//
//  @return
//  */
//-------------------------------------------------------------------
SCRIPTGVAR::SCRIPTGVAR()
{
  Clean();
}


//-------------------------------------------------------------------
//  SCRIPTGVAR::~SCRIPTGVAR
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/07/2007 13:49:33
//
//  @return
//  */
//-------------------------------------------------------------------
SCRIPTGVAR::~SCRIPTGVAR()
{
  if(type == SCRIPTGTOKENIREPS_STRING)
    {
      if(!NotHaveReservedSize())
         {
           XSTRING* string = value.string;
           delete string;
         }
    }

  Clean();
}


/*-------------------------------------------------------------------
//  SCRIPTGVAR::GetName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 17:59:31
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* SCRIPTGVAR::GetName()
{
  return &name;
}



/*-------------------------------------------------------------------
//  SCRIPTGVAR::GetType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 18:00:43
//
//  @return       SCRIPTGTOKENIREPS :
//  */
/*-----------------------------------------------------------------*/
SCRIPTGTOKENIREPS SCRIPTGVAR::GetType()
{
  return type;
}




/*-------------------------------------------------------------------
//  SCRIPTGVAR::GetValueInteger
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/09/2011 17:37:43
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int SCRIPTGVAR::GetValueInteger()
{
  return value.integer;
}


/*-------------------------------------------------------------------
//  SCRIPTGVAR::GetValueCharacter
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/09/2011 17:37:49
//
//  @return       XCHAR :
//  */
/*-----------------------------------------------------------------*/
XCHAR SCRIPTGVAR::GetValueCharacter()
{
  return value.character;
}



/*-------------------------------------------------------------------
//  SCRIPTGVAR::GetValueUInteger
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/09/2011 17:38:05
//
//  @return       XDWORD :
//  */
/*-----------------------------------------------------------------*/
XDWORD SCRIPTGVAR::GetValueUInteger()
{
  return value.uinteger;
}



/*-------------------------------------------------------------------
//  SCRIPTGVAR::GetValueString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/09/2011 17:38:30
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* SCRIPTGVAR::GetValueString()
{
  return value.string;
}



/*-------------------------------------------------------------------
//  SCRIPTGVAR::IsCopy
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 18:00:55
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool SCRIPTGVAR::NotHaveReservedSize()
{
  return nothavereservedsize;
}



/*-------------------------------------------------------------------
//  SCRIPTGVAR::IsArg
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/09/2011 17:22:20
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool SCRIPTGVAR::IsArg()
{
  return isarg;
}


/*-------------------------------------------------------------------
//  SCRIPTGVAR::IsReturnValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/09/2011 0:03:20
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool SCRIPTGVAR::IsReturnValue()
{
  return isreturnvalue;
}




/*-------------------------------------------------------------------
//  SCRIPTGVAR::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/09/2011 11:35:33
//
//  @return       bool :
//  @param        var :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGVAR::Set(SCRIPTGVAR* var)
{
  name                = var->name;
  type                = var->type;
  value               = var->value;
  nothavereservedsize = false;
  isarg               = var->isarg;
  isreturnvalue       = var->isreturnvalue;

  return true;
}



/*-------------------------------------------------------------------
//  SCRIPTGVAR::SetName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 18:04:47
//
//  @return       bool :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGVAR::SetName(XCHAR* name)
{
  this->name = name;

  return true;
}


/*-------------------------------------------------------------------
//  SCRIPTGVAR::SetType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 18:04:51
//
//  @return       bool :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGVAR::SetType(SCRIPTGTOKENIREPS type)
{
  this->type = type;

  return true;
}



/*-------------------------------------------------------------------
//  SCRIPTGVAR::SetValueInteger
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/09/2011 17:45:16
//
//  @return       bool :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGVAR::SetValueInteger(int value)
{
  this->value.integer = value;

  return true;
}



/*-------------------------------------------------------------------
//  SCRIPTGVAR::SetValueCharacter
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/09/2011 17:45:24
//
//  @return       bool :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGVAR::SetValueCharacter(XCHAR value)
{
  this->value.character = value;

  return true;
}



/*-------------------------------------------------------------------
//  SCRIPTGVAR::SetValueUInteger
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/09/2011 17:45:29
//
//  @return       bool :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGVAR::SetValueUInteger(XDWORD value)
{
  this->value.uinteger = value;

  return true;
}



/*-------------------------------------------------------------------
//  SCRIPTGVAR::SetValueString
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/09/2011 17:45:33
//
//  @return       bool :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGVAR::SetValueString(XSTRING* value)
{
  this->value.string = value;

  return true;
}



/*-------------------------------------------------------------------
//  SCRIPTGVAR::SetNotHaveReservedSize
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 18:05:13
//
//  @return       bool :
//  @param        iscopy :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGVAR::SetNotHaveReservedSize(bool nothavereservedsize)
{
  this->nothavereservedsize = nothavereservedsize;

  return true;
}



/*-------------------------------------------------------------------
//  SCRIPTGVAR::SetIsArg
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/09/2011 17:23:26
//
//  @return       bool :
//  @param        isarg :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGVAR::SetIsArg(bool isarg)
{
  this->isarg = isarg;

  return true;
}



/*-------------------------------------------------------------------
//  SCRIPTGVAR::SetIsReturnValue
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/09/2011 0:02:22
//
//  @return       bool :
//  @param        isreturnvalue :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGVAR::SetIsReturnValue(bool isreturnvalue)
{
  this->isreturnvalue = isreturnvalue;

  return true;
}



/*-------------------------------------------------------------------
//   SCRIPTGVAR::ConvertToXVariant
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  09/03/2018 10:07:37
//
//  @return   bool :
//
//  @param    XVARIANT& :
//
*//*-----------------------------------------------------------------*/
bool SCRIPTGVAR::ConvertToXVariant(XVARIANT& variant)
{
  switch(type)
    {
      case SCRIPTGTOKENIREPS_CHAR     : { XSTRING str;

                                          str.Format(__L("%c"), GetValueCharacter());
                                          variant = (XCHAR)str.Get()[0];
                                        }
                                        break;

      case SCRIPTGTOKENIREPS_INT      : variant = GetValueInteger();        break;
      case SCRIPTGTOKENIREPS_STRING   : variant = GetValueString()->Get();  break;

                              default : variant.Set();
                                        return false;
    }

  return true;
}





/*-------------------------------------------------------------------
//   SCRIPTGVAR::ConvertFromXVariant
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  09/03/2018 10:07:49
//
//  @return   bool :
//
//  @param    XVARIANT& :
//
*//*-----------------------------------------------------------------*/
bool SCRIPTGVAR::ConvertFromXVariant(XVARIANT& variant)
{
  Clear();

  switch(variant.GetType())
    {
      case XVARIANT_TYPE_NULL           : break;

      case XVARIANT_TYPE_SERIALIZABLE   : return false;

      case XVARIANT_TYPE_INTEGER        : SetType(SCRIPTGTOKENIREPS_INT);
                                          SetValueInteger(variant);
                                          break;

      case XVARIANT_TYPE_CHAR           : { XSTRING str;
                                            XCHAR   character = 0x000;

                                            str = variant;

                                            if(!str.IsEmpty())
                                              {
                                                character = str.Get()[0];
                                                SetType(SCRIPTGTOKENIREPS_CHAR);
                                                SetValueCharacter(character);
                                              }
                                          }
                                          break;

      case XVARIANT_TYPE_FLOAT          : return false;

      case XVARIANT_TYPE_STRING         : { XSTRING string;

                                            string = variant;
                                            SetType(SCRIPTGTOKENIREPS_STRING);
                                            SetValueString(&string);
                                          }
                                          break;

      case XVARIANT_TYPE_DATE           :
      case XVARIANT_TYPE_TIME           :
      case XVARIANT_TYPE_DATETIME       : return false;
    }

  return true;
}





/*-------------------------------------------------------------------
//  SCRIPTGVAR::Clear
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/09/2011 11:14:39
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool SCRIPTGVAR::Clear()
{
  name.Empty();

  type                = SCRIPTGTOKENIREPS_UNDEFTOK;
  value.integer       = 0;
  nothavereservedsize = false;
  isarg               = false;
  isreturnvalue       = false;

  return true;
}



//-------------------------------------------------------------------
//  SCRIPTGVAR::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/07/2007 13:49:53
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTGVAR::Clean()
{
  name.Empty();

  type                = SCRIPTGTOKENIREPS_UNDEFTOK;
  value.integer       = 0;
  nothavereservedsize = false;
  isarg               = false;
  isreturnvalue       = false;
}



//-------------------------------------------------------------------
//  SCRIPTGFUNCTIONTYPE::SCRIPTGFUNCTIONTYPE
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/07/2007 13:48:43
//
//  @return
//  */
//-------------------------------------------------------------------
SCRIPTGFUNCTIONTYPE::SCRIPTGFUNCTIONTYPE()
{
  Clean();
}


//-------------------------------------------------------------------
//  SCRIPTGFUNCTIONTYPE::~SCRIPTGFUNCTIONTYPE
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/07/2007 13:49:33
//
//  @return
//  */
//-------------------------------------------------------------------
SCRIPTGFUNCTIONTYPE::~SCRIPTGFUNCTIONTYPE()
{
  Clean();
}


/*-------------------------------------------------------------------
//  SCRIPTGFUNCTIONTYPE::GetName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 18:11:26
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* SCRIPTGFUNCTIONTYPE::GetName()
{
  return &name;
}



/*-------------------------------------------------------------------
//  SCRIPTGFUNCTIONTYPE::GetReturnType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 18:11:29
//
//  @return       SCRIPTGTOKENIREPS :
//  */
/*-----------------------------------------------------------------*/
SCRIPTGTOKENIREPS SCRIPTGFUNCTIONTYPE::GetReturnType()
{
  return returntype;
}



/*-------------------------------------------------------------------
//  SCRIPTGFUNCTIONTYPE::GetLocation
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 18:11:36
//
//  @return       XCHAR* :
//  */
/*-----------------------------------------------------------------*/
XCHAR* SCRIPTGFUNCTIONTYPE::GetLocation()
{
  return location;
}



/*-------------------------------------------------------------------
//  SCRIPTGFUNCTIONTYPE::SetName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 18:11:43
//
//  @return       bool :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGFUNCTIONTYPE::SetName(XCHAR* name)
{
  this->name = name;

  return false;
}



/*-------------------------------------------------------------------
//  SCRIPTGFUNCTIONTYPE::SetName
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 22:47:51
//
//  @return       bool :
//  @param        name :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGFUNCTIONTYPE::SetName(XSTRING& name)
{
  return SetName(name.Get());
}




/*-------------------------------------------------------------------
//  SCRIPTGFUNCTIONTYPE::SetReturnType
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 18:11:48
//
//  @return       bool :
//  @param        returntype :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGFUNCTIONTYPE::SetReturnType(SCRIPTGTOKENIREPS returntype)
{
  this->returntype = returntype;

  return true;
}



/*-------------------------------------------------------------------
//  SCRIPTGFUNCTIONTYPE::SetLocation
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 18:11:53
//
//  @return       bool :
//  @param        location :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGFUNCTIONTYPE::SetLocation(XCHAR* location)
{
  this->location = location;

  return true;
}



//-------------------------------------------------------------------
//  SCRIPTGFUNCTIONTYPE::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/07/2007 13:49:53
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTGFUNCTIONTYPE::Clean()
{
  name.Empty();                              // name

  returntype  = SCRIPTGTOKENIREPS_UNDEFTOK;      // return type
  location    = NULL;
}




/*-------------------------------------------------------------------
//  SCRIPTG::SCRIPTG
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/09/2011 16:34:18
//
//  @return

//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
SCRIPTG::SCRIPTG()
{
  Clean();

  functioncallstack.SetIsMulti(true);

  DeleteVarsExec();

  AddCommand(__L("if")      , SCRIPTGTOKENIREPS_IF);
  AddCommand(__L("else")    , SCRIPTGTOKENIREPS_ELSE);
  AddCommand(__L("for")     , SCRIPTGTOKENIREPS_FOR);
  AddCommand(__L("do")      , SCRIPTGTOKENIREPS_DO);
  AddCommand(__L("while")   , SCRIPTGTOKENIREPS_WHILE);
  AddCommand(__L("char")    , SCRIPTGTOKENIREPS_CHAR);
  AddCommand(__L("int")     , SCRIPTGTOKENIREPS_INT);
  AddCommand(__L("string")  , SCRIPTGTOKENIREPS_STRING);
  AddCommand(__L("return")  , SCRIPTGTOKENIREPS_RETURN);
  AddCommand(__L("switch")  , SCRIPTGTOKENIREPS_SWITCH);
  AddCommand(__L("break")   , SCRIPTGTOKENIREPS_BREAK);
  AddCommand(__L("case")    , SCRIPTGTOKENIREPS_CASE);
  AddCommand(__L("")        , SCRIPTGTOKENIREPS_END);
}



//-------------------------------------------------------------------
//  SCRIPTG::~SCRIPTG
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/07/2007 18:21:48
//
//  @return       virtual :
//  */
//-------------------------------------------------------------------
SCRIPTG::~SCRIPTG()
{
  DeleteCommands();

  DeleteLibraryFuncions();

  librarys.DeleteAll();

  DeleteVarsExec();

  Clean();
}



/*-------------------------------------------------------------------
//  SCRIPTG::Run
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/12/2009 08:46:25 a.m.
//
//  @return       SCRIPTERRORMSG :
//  @param        returnval :
*/
/*-----------------------------------------------------------------*/
SCRIPTERRORMSG SCRIPTG::Run(int* returnval)
{
  if(script.IsEmpty()) return SCRIPTERRORMSG_INTERNALERROR;

  isrunwiththread = false;
  if(thread) isrunwiththread = thread->IsRunning();

  errorscript  = SCRIPTERRORMSG_NONE;
  iscancelexec = false;
  breakfound   = false;
  ipprg        = script.Get();

  PreScan();

  if(errorscript==SCRIPTERRORMSG_NONE)
    {
      ipprg = FindFunction(__L("main"));    // Find program starting point.
      if(ipprg)
        {
          ipprg--;
          XSTRING::Set(currenttoken,__L("main"));

          Call();

        } else errorscript = SCRIPTERRORMSG_FUNC_UNDEF;
    }

  if(errorscript==SCRIPTERRORMSG_NONE)
    {
      if(returnval)
        {
          (*returnval) = returnvalue.GetValueInteger();
        }
    }


  DeleteVarsExec();

  return errorscript;
}





/*-------------------------------------------------------------------
//  SCRIPTG::AddCommand
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 16:14:44
//
//  @return       bool :
//  @param        command :
//  @param        SCRIPTGTOKENIREPS token :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTG::AddCommand(XCHAR* command,SCRIPTGTOKENIREPS token)
{
  SCRIPTGCOMMAND* _command = new SCRIPTGCOMMAND();
  if(!_command) return false;

  if(!_command->Set(command,token)) return false;

  commands.Add(_command);

  return true;
}



/*-------------------------------------------------------------------
//  SCRIPTG::DeleteCommands
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 16:14:49
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool SCRIPTG::DeleteCommands()
{
  if(commands.IsEmpty()) return false;

  commands.DeleteContents();
  commands.DeleteAll();

  return true;
}




//-------------------------------------------------------------------
//  SCRIPTG::GetFuncParams
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/07/2007 20:45:40
//
//  @return   int
//  @param    params:
//  */
//-------------------------------------------------------------------
int SCRIPTG::GetFuncParams(SCRIPTGVAR* params)
{
  int count = 0;

  GetToken();

  if(currenttoken[0] != __C('('))
    {
      HaveError(SCRIPTERRORMSG_PAREN_EXPECTED);
      return count;
    }

  do{ SCRIPTGVAR* param = new SCRIPTGVAR();
      if(param)
        {
          EvalExp((*param));

          params[count].Set(param);

          if(param->IsReturnValue())
                 params[count].SetNotHaveReservedSize(false);
            else params[count].SetNotHaveReservedSize(true);

          GetToken();

          count++;

          param->SetNotHaveReservedSize(true);

          delete param;
        }

    } while(currenttoken[0] == __C(','));

  return count;
}



//-------------------------------------------------------------------
//  SCRIPTG::NotFuncParams
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 21:06:38
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTG::NotFuncParams()
{
  GetToken();
  if(currenttoken[0] != __C('('))
    {
      HaveError(SCRIPTERRORMSG_PAREN_EXPECTED);
      return;
    }

  GetToken();
  if(currenttoken[0] != __C(')'))
    {
      HaveError(SCRIPTERRORMSG_PAREN_EXPECTED);
      return;
    }
}


/*-------------------------------------------------------------------
//  SCRIPTG::GetReturnValueScript
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 17:02:05
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int SCRIPTG::GetReturnValueScript()
{
  return returnvaluescript;
}



/*-------------------------------------------------------------------
//  SCRIPTG::SetReturnValueScript
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2011 17:18:22
//
//  @return       int :
//  @param        returnvaluescript :
*/
/*-----------------------------------------------------------------*/
int SCRIPTG::SetReturnValueScript(int returnvaluescript)
{
  this->returnvaluescript = returnvaluescript;

  return true;
}




/*-------------------------------------------------------------------
//  SCRIPTG::HaveError
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/01/2010 03:51:34 p.m.
//
//  @return       void :
//  @param        errorscript :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTG::HaveError(SCRIPTERRORMSG errorcode)
{
  if(errorscript==SCRIPTERRORMSG_NONE)
    {
      static XCHAR* errorstr[]= { __L("None")                     ,
                                  __L("Internal error")           ,
                                  __L("Syntax error")             ,
                                  __L("No expression present")      ,
                                  __L("Not a variable")           ,
                                  __L("Duplicate variable name")    ,
                                  __L("Duplicate function name")    ,
                                  __L("Semicolon expected")       ,
                                  __L("Unbalanced braces")          ,
                                  __L("Function undefined")       ,
                                  __L("Type specifier expected")    ,
                                  __L("Return without call")        ,
                                  __L("Parentheses expected")     ,
                                  __L("While expected")           ,
                                  __L("Closing quote expected")   ,
                                  __L("Division by zero")         ,
                                  __L("{ expected (control statements must use blocks)"),
                                  __L("Colon expected")           ,
                                  __L("Insufficient parameters")   ,
                                  __L("Break by user")
                                };

      SCRIPTXEVENT xevent(this,(errorcode==SCRIPTERRORMSG_USERBREAK)?SCRIPTXEVENTTYPE_BREAK:SCRIPTXEVENTTYPE_ERROR);


      XDWORD sizecharnow = (int)(ipprg - script.Get());
      XDWORD sizechar    = 0;
      int nline       = 0;

      for(int c=0;c<xfiletxt->GetNLines();c++)
        {
          sizechar += (xfiletxt->GetLine(c)->GetSize()+2);

          if(sizecharnow<=sizechar)
            {
              nline = c+1;
              break;
            }
        }

      xevent.SetError(errorcode);
      xevent.GetErrorText()->Set(errorstr[errorcode]);
      xevent.GetCurrentToken()->Set(currenttoken);
      xevent.SetNLine(nline);

     XDEBUGTRACE_PRINTCOLOR(4, __L("Script [%s] ERROR %d: %s, line %d \"%s\"") , namescript.Get(), errorcode, errorstr[errorcode], nline, currenttoken);

      PostEvent(&xevent);
      errorscript  = errorcode;
      iscancelexec = true;
    }

  return (errorscript==SCRIPTERRORMSG_NONE)?false:true;
}




//-------------------------------------------------------------------
//  SCRIPTG::DeleteVarsExec
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 16:17:14
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool SCRIPTG::DeleteVarsExec()
{
  if(!globalvars.IsEmpty())
    {
      globalvars.DeleteContents();
      globalvars.DeleteAll();
    }

  if(!functiontable.IsEmpty())
    {
      functiontable.DeleteContents();
      functiontable.DeleteAll();
    }

  if(!localvarsstack.IsEmpty())
    {
      localvarsstack.DeleteContents();
      localvarsstack.DeleteAll();
    }

  if(!functioncallstack.IsEmpty()) functioncallstack.DeleteAll();
  if(!nestscopestack.IsEmpty())    nestscopestack.DeleteAll();

  return true;
}



/*-------------------------------------------------------------------
//  SCRIPTG::IsSpace
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/05/2011 22:14:41
//
//  @return       bool :
//  @param        XCHARacter :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTG::IsSpace(XCHAR character)
{
  if(character == __C(' '))   return true;
  if(character == __C('\x09')) return true;

  return false;
}



/*-------------------------------------------------------------------
//  SCRIPTG::IsDigit
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/05/2011 22:15:43
//
//  @return       bool :
//  @param        character :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTG::IsDigit(XCHAR character)
{
  if((character>=__C('0'))&&(character <= __C('9'))) return true;

  return false;
}



/*-------------------------------------------------------------------
//  SCRIPTG::IsAlpha
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/05/2011 22:14:47
//
//  @return       bool :
//  @param        XCHARacter :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTG::IsAlpha(XCHAR character)
{
  if((character>='a')&&(character<='z')) return true;
  if((character>='A')&&(character<='Z')) return true;

  return false;
}



//-------------------------------------------------------------------
//  SCRIPTG::IsDelimiter
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/07/2007 22:08:51
//
//  @return       bool :
//  @param        c :
*/
//-------------------------------------------------------------------
bool SCRIPTG::IsDelimiter(XCHAR c)
{
  if(c==0) return true;

  XSTRING delimiter(__L("\x09\r !:;,+-<>'/*%^=()"));

  if(delimiter.FindCharacter(c)!=XSTRING_NOTFOUND) return true;

  return false;
}


//-------------------------------------------------------------------
//  SCRIPTG::LookUpToken
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/07/2007 22:11:32
//
//  @return       SCRIPTGTOKENIREPS :
//  @param        *s :
*/
//-------------------------------------------------------------------
SCRIPTGTOKENIREPS SCRIPTG::LookUpToken(XCHAR* s)
{
  for(int c=0;c<(int)commands.GetSize();c++)
    {
      SCRIPTGCOMMAND* command = (SCRIPTGCOMMAND*)commands.Get(c);
      if(command)
        {
          if(!command->GetCommand()->Compare(s)) return command->GetToken();
        }
    }

  return SCRIPTGTOKENIREPS_UNDEFTOK;
}




//-------------------------------------------------------------------
//  SCRIPTG::PutBackToken
/**
//  Return a token to input stream.
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 9:20:18
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTG::PutBackToken()
{
  XCHAR* token;

  token = currenttoken;

  for(; *token; token++)
    {
      ipprg--;
    }
}



//-------------------------------------------------------------------
//  SCRIPTG::EvalExp
/**
//  Entry point into parser.
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 16:55:09
//
//  @return       void :
//  @param        value :
*/
//-------------------------------------------------------------------
void SCRIPTG::EvalExp(SCRIPTGVAR& value)
{
  GetToken();

  if(!currenttoken[0])
    {
      HaveError(SCRIPTERRORMSG_NO_EXP);
      return;
    }


  if((tokentype == SCRIPTGTOKENTYPES_IDENTIFIER))
    {
      XSTRING* string = new XSTRING();
      if(string)
        {
          (*string) = currenttoken;

          value.SetType(SCRIPTGTOKENIREPS_STRING);
          value.SetValueString(string);

          value.SetIsReturnValue(true);
        }
    }


  if(currenttoken[0] == __C(';'))
    {
      value.Clear();
      return;
    }

  EvalExp0(value);

  PutBackToken();
}



//-------------------------------------------------------------------
//  EvalExp0
/**
//  Process an assignment expression.
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 16:57:17
//
//  @return       void :
//  @param        value :
*/
//-------------------------------------------------------------------
void SCRIPTG::EvalExp0(SCRIPTGVAR& value)
{
  XSTRING      tempttoken;
  SCRIPTGTOKENTYPES temptokentype;

  if(tokentype == SCRIPTGTOKENTYPES_IDENTIFIER)
    {
       if(IsVariable(currenttoken))
         {
           tempttoken    = currenttoken;
           temptokentype = tokentype;

           GetToken();

           if(currenttoken[0] == __C('='))
             {
               GetToken();

               if(tokentype==SCRIPTGTOKENTYPES_STRING)
                {
                  XSTRING* string = new XSTRING();
                  if(string)
                    {
                      (*string) = currenttoken;

                      value.SetType(SCRIPTGTOKENIREPS_STRING);
                      value.SetValueString(string);

                      AssignVariable(tempttoken.Get(), value);

                      delete string;

                      GetToken();
                    }
                }
               else
                {
                  EvalExp0(value);
                  AssignVariable(tempttoken.Get(), value);
                }

               return;
             }
            else
             {
               PutBackToken();

               XSTRING::Set(currenttoken,tempttoken.Get());
               tokentype    = temptokentype;
             }
          }
    }

  EvalExp1(value);
}


//-------------------------------------------------------------------
//  SCRIPTG::EvalExp1
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 19:09:50
//
//  @return       void :
//  @param        value :
*/
//-------------------------------------------------------------------
void SCRIPTG::EvalExp1(SCRIPTGVAR& value)
{
  XCHAR   operation;
  XSTRING relops;

  relops.Format(__L("%c%c%c%c%c%c"), SCRIPTGDOUBLEOPERATOR_LT, SCRIPTGDOUBLEOPERATOR_LE, SCRIPTGDOUBLEOPERATOR_GT
                                   , SCRIPTGDOUBLEOPERATOR_GE, SCRIPTGDOUBLEOPERATOR_EQ, SCRIPTGDOUBLEOPERATOR_NE);
  EvalExp2(value);

  operation = currenttoken[0];

  if(relops.FindCharacter(operation)!=XSTRING_NOTFOUND)
    {
      GetToken();

      if(tokentype == SCRIPTGTOKENTYPES_STRING)
        {
          XSTRING* strpvalue = new XSTRING();
          if(strpvalue)
            {
              XSTRING* strvalue = value.GetValueString();

              SCRIPTGVAR pvalue;

              pvalue.SetValueString(strpvalue);

              EvalExp2(pvalue);

              switch(operation)
                {
                  case SCRIPTGDOUBLEOPERATOR_LT:  value.SetValueInteger((strvalue->GetSize() <  strpvalue->GetSize())); break;
                  case SCRIPTGDOUBLEOPERATOR_LE:  value.SetValueInteger((strvalue->GetSize() <= strpvalue->GetSize())); break;
                  case SCRIPTGDOUBLEOPERATOR_GT:  value.SetValueInteger((strvalue->GetSize() >  strpvalue->GetSize())); break;
                  case SCRIPTGDOUBLEOPERATOR_GE:  value.SetValueInteger((strvalue->GetSize() >= strpvalue->GetSize())); break;
                  case SCRIPTGDOUBLEOPERATOR_EQ:  value.SetValueInteger(!strvalue->Compare((*strpvalue)));              break;
                  case SCRIPTGDOUBLEOPERATOR_NE:  value.SetValueInteger(strvalue->Compare((*strpvalue)));               break;
                }

              delete strpvalue;
            }
        }
       else
        {
          SCRIPTGVAR partialvalue;

          EvalExp2(partialvalue);

          switch(operation)
            {
              case SCRIPTGDOUBLEOPERATOR_LT:  value.SetValueInteger((value.GetValueInteger() <  partialvalue.GetValueInteger())); break;
              case SCRIPTGDOUBLEOPERATOR_LE:  value.SetValueInteger((value.GetValueInteger() <= partialvalue.GetValueInteger())); break;
              case SCRIPTGDOUBLEOPERATOR_GT:  value.SetValueInteger((value.GetValueInteger() >  partialvalue.GetValueInteger())); break;
              case SCRIPTGDOUBLEOPERATOR_GE:  value.SetValueInteger((value.GetValueInteger() >= partialvalue.GetValueInteger())); break;
              case SCRIPTGDOUBLEOPERATOR_EQ:  value.SetValueInteger((value.GetValueInteger() == partialvalue.GetValueInteger())); break;
              case SCRIPTGDOUBLEOPERATOR_NE:  value.SetValueInteger((value.GetValueInteger() != partialvalue.GetValueInteger())); break;
            }
        }
    }
}



//-------------------------------------------------------------------
//  SCRIPTG::EvalExp2
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 19:15:05
//
//  @return       void :
//  @param        value :
*/
//-------------------------------------------------------------------
void SCRIPTG::EvalExp2(SCRIPTGVAR& value)
{
  XCHAR      operation;
  XSTRING okops;

  okops.Format(__L("(%c%c-+"), SCRIPTGDOUBLEOPERATOR_INC, SCRIPTGDOUBLEOPERATOR_DEC);

  EvalExp3(value);

  while(((operation = currenttoken[0]) == __C('+')) || (operation == __C('-')))
    {
      GetToken();

      if((tokentype == SCRIPTGTOKENTYPES_DELIMITER) && (okops.FindCharacter(currenttoken[0]) == XSTRING_NOTFOUND))
        {
          HaveError(SCRIPTERRORMSG_SYNTAX);
          return;
        }

      if(tokentype == SCRIPTGTOKENTYPES_STRING)
        {
          HaveError(SCRIPTERRORMSG_SYNTAX);
          return;
        }

      if(tokentype == SCRIPTGTOKENTYPES_IDENTIFIER)
        {
          SCRIPTGVAR* var = IsVariable(currenttoken);
          if(var)
            {
              if(var->GetType()==SCRIPTGTOKENIREPS_STRING)
                {
                  HaveError(SCRIPTERRORMSG_SYNTAX);
                  return;
                }
            }
        }

      SCRIPTGVAR partialvalue;

      EvalExp3(partialvalue);

      switch(operation)
        {
          case __C('-'):    value.SetValueInteger((value.GetValueInteger() - partialvalue.GetValueInteger()));    break;
          case __C('+'):    value.SetValueInteger((value.GetValueInteger() + partialvalue.GetValueInteger()));    break;
        }
    }
}



//-------------------------------------------------------------------
//  SCRIPTG::EvalExp3
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 19:25:05
//
//  @return       void :
//  @param        value :
*/
//-------------------------------------------------------------------
void SCRIPTG::EvalExp3(SCRIPTGVAR& value)
{
  XCHAR         operation;
  XSTRING   okops;
  SCRIPTGVAR  partialvalue;

  okops.Format(__L("(%c%c-+"), SCRIPTGDOUBLEOPERATOR_INC, SCRIPTGDOUBLEOPERATOR_DEC);

  EvalExp4(value);

  while( ((operation = currenttoken[0]) == __C('*')) || (operation == __C('/')) || (operation == __C('%')))
    {
      GetToken();

      if((tokentype == SCRIPTGTOKENTYPES_DELIMITER) && (okops.FindCharacter(currenttoken[0]) == XSTRING_NOTFOUND))
        {
          HaveError(SCRIPTERRORMSG_SYNTAX);
          return;
        }

      EvalExp4(partialvalue);

      switch(operation)
        {
          // mul, div, or modulus
          case __C('*'): value.SetValueInteger(value.GetValueInteger() * partialvalue.GetValueInteger());
                         break;

          case __C('/'): if(partialvalue.GetValueInteger() == 0)
                           {
                             HaveError(SCRIPTERRORMSG_DIV_BY_ZERO);
                             return;
                           }
                         value.SetValueInteger((value.GetValueInteger() / partialvalue.GetValueInteger()));
                         break;

          case __C('%'): { int t = (value.GetValueInteger() / partialvalue.GetValueInteger());
                           value.SetValueInteger(value .GetValueInteger() - (t * partialvalue.GetValueInteger()));
                         }
                         break;
        }
    }
}


//-------------------------------------------------------------------
//  SCRIPTG::EvalExp4
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 19:25:23
//
//  @return       void :
//  @param        value :
*/
//-------------------------------------------------------------------
void SCRIPTG::EvalExp4(SCRIPTGVAR& value)
{
  XCHAR operation;
  XCHAR temptoken;

  operation = __C('\0');

  if(tokentype != SCRIPTGTOKENTYPES_STRING)
    {
      if((currenttoken[0] == __C('+'))  ||
         (currenttoken[0] == __C('-'))  ||
         (currenttoken[0] == SCRIPTGDOUBLEOPERATOR_INC) ||
         (currenttoken[0] == SCRIPTGDOUBLEOPERATOR_DEC))
        {
          temptoken = currenttoken[0];
          operation = currenttoken[0];

          GetToken();

          SCRIPTGVAR* var = FindVariable(currenttoken);
          if(var)
            {
              if(temptoken == SCRIPTGDOUBLEOPERATOR_INC) var->SetValueInteger(var->GetValueInteger()+1);
              if(temptoken == SCRIPTGDOUBLEOPERATOR_DEC) var->SetValueInteger(var->GetValueInteger()-1);

              AssignVariable(currenttoken,(*var));
            }
        }
    }

  EvalExp5(value);

  if(operation == __C('-'))  value.SetValueInteger(-value.GetValueInteger());
}




//-------------------------------------------------------------------
//  SCRIPTG::EvalExp5
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 19:32:22
//
//  @return       void :
//  @param        value :
*/
//-------------------------------------------------------------------
void SCRIPTG::EvalExp5(SCRIPTGVAR& value)
{
  if(currenttoken[0] == __C('('))
    {
      GetToken();

      EvalExp0(value);

      if(currenttoken[0] != __C(')'))
        {
          HaveError(SCRIPTERRORMSG_PAREN_EXPECTED);
          return;
        }

      GetToken();

    } else Atom(value);
}




//-------------------------------------------------------------------
//  SCRIPTG::Atom
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 19:38:45
//
//  @return       void :
//  @param        value :
*/
//-------------------------------------------------------------------
void SCRIPTG::Atom(SCRIPTGVAR& value)
{
  switch(tokentype)
    {
      case SCRIPTGTOKENTYPES_IDENTIFIER : { SCRIPTLIBRARYFUNCTION* function = GetLibraryFunction(currenttoken);
                                            if(function)
                                              {
                                                if(function->GetFunctionLibrary())
                                                  {
                                                    //(*function->GetFunctionLibrary())(function->GetLibrary(), this, (void*)(&value));

                                                    XVECTOR<XVARIANT*>  funcparams;
                                                    XVARIANT            funcreturnvalue;
                                                    SCRIPTGVAR          params[SCRIPTG_NUMPARAMS];

                                                    memset(params,0,SCRIPTG_NUMPARAMS*sizeof(SCRIPTGVAR));

                                                    int nparams = GetFuncParams(params);
                                                    if(nparams)
                                                      {
                                                        for(int c=0; c<nparams; c++)
                                                          {
                                                            XVARIANT* variant = new XVARIANT();
                                                            if(variant)
                                                              {
                                                                params[c].ConvertToXVariant((*variant));
                                                                funcparams.Add(variant);
                                                              }
                                                          }
                                                      }

                                                    (*function->GetFunctionLibrary())(function->GetLibrary(), this, &funcparams, &funcreturnvalue);

                                                    value.ConvertFromXVariant(funcreturnvalue);
                                                    value.SetIsReturnValue(true);

                                                    funcparams.DeleteContents();
                                                    funcparams.DeleteAll();
                                                  }
                                                 else
                                                  {
                                                    HaveError(SCRIPTERRORMSG_FUNC_UNDEF);
                                                    return;
                                                  }
                                              }
                                            else
                                              {
                                                if(FindFunction(currenttoken))
                                                  {
                                                    XCHAR tempcurrentfunc[_MAXSTR];

                                                    XSTRING::Set(tempcurrentfunc,currentfunction);

                                                    Call();

                                                    value.Set(&returnvalue);
                                                    value.SetIsReturnValue(true);

                                                    XSTRING::Set(currentfunction,tempcurrentfunc);

                                                    returnvalue.SetType(SCRIPTGTOKENIREPS_UNDEFTOK);
                                                    returnvalue.SetValueInteger(0);


                                                  }
                                                else
                                                  {
                                                    XCHAR tempcurrenttoken[_MAXSTR];

                                                    SCRIPTGVAR* var = FindVariable(currenttoken);
                                                    if(var)
                                                      {
                                                        value.Set(var);

                                                        XSTRING::Set(tempcurrenttoken,currenttoken);

                                                        GetToken();

                                                        if((currenttoken[0] == SCRIPTGDOUBLEOPERATOR_INC) || (currenttoken[0] == SCRIPTGDOUBLEOPERATOR_DEC))
                                                          {
                                                            SCRIPTGVAR* var = FindVariable(tempcurrenttoken);
                                                            if(var)
                                                              {
                                                                if(currenttoken[0] == SCRIPTGDOUBLEOPERATOR_INC)
                                                                        var->SetValueInteger(var->GetValueInteger()+1);
                                                                  else  var->SetValueInteger(var->GetValueInteger()-1);

                                                                AssignVariable(tempcurrenttoken, (*var));
                                                              }

                                                          } else PutBackToken();
                                                      }
                                                  }
                                              }

                                            GetToken();
                                          }
                                          return;

      case SCRIPTGTOKENTYPES_NUMBER     : { XSTRING string(currenttoken);

                                            value.SetType(SCRIPTGTOKENIREPS_INT);
                                            value.SetValueInteger(string.ConvertToInt());
                                            GetToken();
                                          }
                                          return;

      case SCRIPTGTOKENTYPES_STRING     : { XSTRING* string;

                                            if(value.GetValueString())
                                              {
                                                (*value.GetValueString()) = currenttoken;
                                              }
                                             else
                                              {
                                                string = new XSTRING();

                                                if(string) (*string) = currenttoken;
                                                value.SetValueString(string);

                                                SCRIPTGVAR* vt = new SCRIPTGVAR();
                                                if(vt)
                                                  {
                                                    vt->SetType(SCRIPTGTOKENIREPS_STRING);
                                                    vt->SetValueString(string);

                                                    localvarsstack.Add(vt);
                                                  }
                                              }

                                            GetToken();
                                          }
                                          return;

      case SCRIPTGTOKENTYPES_DELIMITER  : if(currenttoken[0] == __C('\''))
                                            {
                                              value.SetValueCharacter((XCHAR)(*ipprg));
                                              ipprg++;

                                              if((*ipprg)!=__C('\''))
                                                {
                                                  HaveError(SCRIPTERRORMSG_QUOTE_EXPECTED);
                                                  return;
                                                }

                                              ipprg++;

                                              GetToken();

                                              return;
                                            }

                                          if(currenttoken[0] == __C(')'))
                                            {
                                            }
                                           else HaveError(SCRIPTERRORMSG_SYNTAX);

                                          return;

                        default         : HaveError(SCRIPTERRORMSG_SYNTAX);
                                          return;

    }
}



//-------------------------------------------------------------------
//  SCRIPTG::IsVariable
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 19:01:22
//
//  @return       SCRIPTGVAR* :
//  @param        variblename :
*/
//-------------------------------------------------------------------
SCRIPTGVAR* SCRIPTG::IsVariable(XCHAR* variablename)
{
  if(!localvarsstack.IsEmpty())
    {
      for(int c=((int)localvarsstack.GetSize()-1); c>= functioncallstack.GetLast(); c--)
        {
          SCRIPTGVAR* var = localvarsstack.Get(c);
          if(var)
            {
              if(!var->GetName()->Compare(variablename)) return var;
            }
        }
    }


  for(XDWORD c=0; c<(int)globalvars.GetSize(); c++)
    {
      SCRIPTGVAR* var = (SCRIPTGVAR*)globalvars.Get(c);
      if(var)
        {
          if(!var->GetName()->Compare(variablename)) return var;
        }
    }

  return NULL;
}




//-------------------------------------------------------------------
//  SCRIPTG::AssignVariable
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 19:55:31
//
//  @return       void :
//  @param        variablename :
//  @param        value :
*/
//-------------------------------------------------------------------
void SCRIPTG::AssignVariable(XCHAR* variablename, SCRIPTGVAR& value)
{
  if(!localvarsstack.IsEmpty())
    {
      for(int c=localvarsstack.GetSize()-1; c>=functioncallstack.GetLast(); c--)
        {
          SCRIPTGVAR* var = (SCRIPTGVAR*)localvarsstack.Get(c);
          if(var)
            {
              if(!var->GetName()->Compare(variablename))
                {
                  if(var->GetType() == SCRIPTGTOKENIREPS_CHAR)
                    {
                      var->SetValueCharacter(value.GetValueCharacter());
                    }
                   else
                    {
                      if(var->GetType() == SCRIPTGTOKENIREPS_INT)
                        {
                          var->SetValueInteger(value.GetValueInteger());
                        }
                       else
                        {
                          if(var->GetType() == SCRIPTGTOKENIREPS_STRING)
                            {
                              XSTRING* string1 = var->GetValueString();
                              XSTRING* string2 = value.GetValueString();

                              if(string1 && string2 && (string1!=string2))
                                {
                                  (*string1) = string2->Get();
                                }
                            }
                        }
                    }

                  return;
                }
            }
        }
    }

  for(XDWORD c=0;c<globalvars.GetSize();c++)
    {
      SCRIPTGVAR* var = (SCRIPTGVAR*)globalvars.Get(c);
      if(var)
        {
          if(!var->GetName()->Compare(variablename))
            {
              if(var->GetType() == SCRIPTGTOKENIREPS_CHAR)
                {
                  var->SetValueCharacter(value.GetValueCharacter());
                }
               else
                {
                  if(var->GetType() == SCRIPTGTOKENIREPS_INT)
                    {
                      var->SetValueInteger(value.GetValueInteger());
                    }
                   else
                    {
                      if(var->GetType() == SCRIPTGTOKENIREPS_STRING)
                        {
                          XSTRING* string1 = var->GetValueString();
                          XSTRING* string2 = value.GetValueString();

                          if(string1 && string2 && (string1!=string2))
                            {
                              (*string1) = string2->Get();
                            }
                        }
                    }
                }

              return;
            }
        }
    }

  HaveError(SCRIPTERRORMSG_NOT_VAR);
}



//-------------------------------------------------------------------
//  SCRIPTG::FindVariable
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 19:59:04
//
//  @return       int :
//  @param        variablename :
*/
//-------------------------------------------------------------------
SCRIPTGVAR* SCRIPTG::FindVariable(XCHAR* variablename)
{
  if(!localvarsstack.IsEmpty())
    {
      for(int c=localvarsstack.GetSize()-1; c>=functioncallstack.GetLast(); c--)
        {
          SCRIPTGVAR* var = (SCRIPTGVAR*)localvarsstack.Get(c);
          if(var)
            {
              if(!var->GetName()->Compare(variablename)) return var;
            }
        }
    }

  for(int c=0; c<(int)globalvars.GetSize(); c++)
    {
      SCRIPTGVAR* var = (SCRIPTGVAR*)globalvars.Get(c);
      if(var)
        {
          if(!var->GetName()->Compare(variablename)) return var;
        }
    }

  HaveError(SCRIPTERRORMSG_NOT_VAR);

  return NULL;
}



//-------------------------------------------------------------------
//  SCRIPTG::FindVariableType
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/07/2007 17:43:06
//
//  @return       SCRIPTGTOKENIREPS :
//  @param        variablename :
*/
//-------------------------------------------------------------------
SCRIPTGTOKENIREPS SCRIPTG::FindVariableType(XCHAR* variablename)
{
  if(!localvarsstack.IsEmpty())
    {
      for(int c=localvarsstack.GetSize()-1; c>=functioncallstack.GetLast(); c--)
        {
          SCRIPTGVAR* var = (SCRIPTGVAR*)localvarsstack.Get(c);
          if(var)
            {
              if(!var->GetName()->Compare(variablename))  return var->GetType();
            }
        }
    }

  for(int c=0; c<(int)globalvars.GetSize(); c++)
    {
      SCRIPTGVAR* var = (SCRIPTGVAR*)globalvars.Get(c);
      if(var)
        {
          if(!var->GetName()->Compare(variablename)) return var->GetType();
        }
    }

  return SCRIPTGTOKENIREPS_UNDEFTOK;
}




//-------------------------------------------------------------------
//  SCRIPTG::FindFunction
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 19:01:13
//
//  @return       XCHAR* :
//  @param        name :
*/
//-------------------------------------------------------------------
XCHAR* SCRIPTG::FindFunction(XCHAR* name)
{
  for(XDWORD c=0; c<functiontable.GetSize(); c++)
    {
      SCRIPTGFUNCTIONTYPE* functiontype = (SCRIPTGFUNCTIONTYPE*)functiontable.Get(c);
      if(functiontype)
        {
          if(!functiontype->GetName()->Compare(name))
            {
              return functiontype->GetLocation();
            }
        }
    }

  return NULL;
}



//-------------------------------------------------------------------
//  SCRIPTG::GetFunction
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 19:01:13
//
//  @return       SCRIPTGFUNCTIONTYPE* :
//  @param        name :
*/
//-------------------------------------------------------------------
SCRIPTGFUNCTIONTYPE* SCRIPTG::GetFunction(XCHAR* name)
{
  for(XDWORD c=0; c<functiontable.GetSize(); c++)
    {
      SCRIPTGFUNCTIONTYPE* functiontype = (SCRIPTGFUNCTIONTYPE*)functiontable.Get(c);
      if(functiontype)
        {
          if(!functiontype->GetName()->Compare(name))
            {
              return functiontype;
            }
        }
    }

  return NULL;
}



//-------------------------------------------------------------------
//  SCRIPTG::FindEndofBlock
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/07/2007 9:55:08
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTG::FindEndofBlock()
{
  int brace;

  GetToken();

  if(currenttoken[0] != __C('{'))
    {
      HaveError(SCRIPTERRORMSG_BRACE_EXPECTED);
      return;
    }

  brace = 1;

  do{ GetToken();

      if(currenttoken[0] == __C('{'))
        {
          brace++;
        }
       else
        {
          if(currenttoken[0] == __C('}')) brace--;
        }

    } while(brace && tokenireps != SCRIPTGTOKENIREPS_END);

  if(tokenireps==SCRIPTGTOKENIREPS_END)
    {
      HaveError(SCRIPTERRORMSG_UNBAL_BRACES);
    }
}



//-------------------------------------------------------------------
//  SCRIPTG::DeclareGlobalVariable
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 9:08:48
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTG::DeclareGlobalVariable()
{
  SCRIPTGTOKENIREPS  vartype;
  SCRIPTGVAR*       vt;

  GetToken();

  vartype = tokenireps;

  do {  vt = new SCRIPTGVAR();
        if(!vt) return;

        vt->SetType(vartype);
        vt->SetValueInteger(0);

        GetToken();

        for(int c=0; c<(int)globalvars.GetSize(); c++)
          {
            SCRIPTGVAR* vttmp = (SCRIPTGVAR*)globalvars.Get(c);

            if(!vttmp->GetName()->Compare(currenttoken))
              {
                HaveError(SCRIPTERRORMSG_DUP_VAR);
                return;
              }
          }

        vt->SetName(currenttoken);

        if((vartype==SCRIPTGTOKENIREPS_STRING)&&(!vt->GetValueString()))
          {
            XSTRING* string = new XSTRING();
            if(string) vt->SetValueString(string);
          }

        globalvars.Add(vt);

        GetToken();

      } while(currenttoken[0] == __C(','));

  if(currenttoken[0] != __C(';'))  HaveError(SCRIPTERRORMSG_SEMI_EXPECTED);
}



//-------------------------------------------------------------------
//  SCRIPTG::DeclareLocalVariable
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 22:14:29
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTG::DeclareLocalVariable()
{
  SCRIPTGTOKENIREPS  vartype;
  SCRIPTGVAR*       vt;

  GetToken();

  vartype = tokenireps;


  do{   vt = new SCRIPTGVAR();
        if(!vt) return;

        vt->SetType(vartype);
        vt->SetValueInteger(0);

        GetToken();

        if(!localvarsstack.IsEmpty())
          {
            for(int c=localvarsstack.GetSize()-1; c>=nestscopestack.GetLast(); c--)
              {
                SCRIPTGVAR* var = (SCRIPTGVAR*)localvarsstack.Get(c);

                if(var)
                  {
                    if(!var->GetName()->Compare(currenttoken))
                      {
                        HaveError(SCRIPTERRORMSG_DUP_VAR);
                        return;
                      }
                  }
              }
          }

        vt->SetName(currenttoken);

        if((vartype==SCRIPTGTOKENIREPS_STRING)&&(!vt->GetValueString()))
          {
            XSTRING* string = new XSTRING();
            if(string) vt->SetValueString(string);
          }

        localvarsstack.Add(vt);

        GetToken();

    } while(currenttoken[0] == __C(','));

  if(currenttoken[0] != __C(';'))
    {
      HaveError(SCRIPTERRORMSG_SEMI_EXPECTED);
      return;
    }
}



//-------------------------------------------------------------------
//  SCRIPTG::GetToken
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 9:57:13
//
//  @return       SCRIPTGTOKENTYPES :
//  */
//-------------------------------------------------------------------
SCRIPTGTOKENTYPES SCRIPTG::GetToken()
{
  XCHAR* temptoken;

  tokentype  = SCRIPTGTOKENTYPES_UNDEFTT;
  tokenireps = SCRIPTGTOKENIREPS_UNDEFTOK;

  temptoken    = currenttoken;
  (*temptoken) = __C('\0');

  // Skip over white space.
  while(IsSpace(*ipprg) && (*ipprg)) ++ipprg;

  // Skip over newline.
  while((*ipprg) == __C('\r'))
    {
      ++ipprg;
      ++ipprg;

      // Again, skip over white space.
      while(IsSpace(*ipprg) && (*ipprg)) ++ipprg;
    }

  // Check for end of ipprgram.
  if((*ipprg) == __C('\0'))
    {
      currenttoken[0] = __C('\0');
      tokenireps            = SCRIPTGTOKENIREPS_END;

      return (tokentype = SCRIPTGTOKENTYPES_DELIMITER);
    }

  XSTRING delimiters(__L("{}"));

  // Check for block delimiters.
  if(delimiters.FindCharacter(*ipprg)!=XSTRING_NOTFOUND)
    {
      (*temptoken) = (*ipprg);
      temptoken++;

      (*temptoken) = __C('\0');
      ipprg++;

      return (tokentype = SCRIPTGTOKENTYPES_BLOCK);
    }

  // Look for comments.
  if((*ipprg) == __C('/'))
    {
      if(*(ipprg+1) == __C('*'))
        { // is a /* comment
          ipprg += 2;

          do { // find end of comment
              while((*ipprg) != __C('*')) ipprg++;
              ipprg++;
            } while ((*ipprg) != __C('/'));

          ipprg++;

          return (tokentype = SCRIPTGTOKENTYPES_DELIMITER);
        }
       else
        {
          if(*(ipprg+1) == '/')
            {
              // is a // comment
              ipprg += 2;
              // Find end of comment.
              while(((*ipprg) != __C('\r')) && ((*ipprg) != __C('\0'))) ipprg++;

              if((*ipprg) == __C('\r')) ipprg +=2;

              return (tokentype = SCRIPTGTOKENTYPES_DELIMITER);
            }
        }
    }

  XSTRING dobleops(__L("!<>=+-"));

  // Check for double-ops.
  if(dobleops.FindCharacter((*ipprg))!=XSTRING_NOTFOUND)
    {
      switch((*ipprg))
        {
          case '=': if(*(ipprg+1) == __C('='))
                      {
                        ipprg++; ipprg++;
                        (*temptoken) = SCRIPTGDOUBLEOPERATOR_EQ;
                        temptoken++;
                        (*temptoken) = SCRIPTGDOUBLEOPERATOR_EQ;
                        temptoken++;
                        (*temptoken) = __C('\0');
                      }
                    break;

          case '!': if(*(ipprg+1) == __C('='))
                      {
                        ipprg++; ipprg++;
                        (*temptoken) = SCRIPTGDOUBLEOPERATOR_NE;
                        temptoken++;
                        (*temptoken) = SCRIPTGDOUBLEOPERATOR_NE;
                        temptoken++;
                        (*temptoken) = __C('\0');
                      }
                    break;

          case '<': if(*(ipprg+1) == __C('='))
                      {
                        ipprg++; ipprg++;
                        (*temptoken) = SCRIPTGDOUBLEOPERATOR_LE;
                        temptoken++;
                        (*temptoken) = SCRIPTGDOUBLEOPERATOR_LE;
                      }
                     else
                      {
                        if(*(ipprg+1) == __C('<'))
                          {
                            ipprg++; ipprg++;
                            (*temptoken) = SCRIPTGDOUBLEOPERATOR_LS;
                            temptoken++;
                            (*temptoken) = SCRIPTGDOUBLEOPERATOR_LS;
                          }
                          else
                          {
                            ipprg++;
                            (*temptoken) = SCRIPTGDOUBLEOPERATOR_LT;
                          }
                      }

                    temptoken++;
                    *temptoken = __C('\0');
                    break;

          case '>': if(*(ipprg+1) == __C('='))
                      {
                        ipprg++; ipprg++;
                        (*temptoken) = SCRIPTGDOUBLEOPERATOR_GE;
                        temptoken++;
                        (*temptoken) = SCRIPTGDOUBLEOPERATOR_GE;
                      }
                     else
                      {
                        if(*(ipprg+1) == __C('>'))
                          {
                            ipprg++; ipprg++;
                            (*temptoken) = SCRIPTGDOUBLEOPERATOR_RS;
                            temptoken++;
                            (*temptoken) = SCRIPTGDOUBLEOPERATOR_RS;
                          }
                          else
                          {
                            ipprg++;
                            (*temptoken) = SCRIPTGDOUBLEOPERATOR_GT;
                          }
                      }
                     temptoken++;
                     (*temptoken) = __C('\0');
                    break;

          case '+': if(*(ipprg+1) == __C('+'))
                      {
                        ipprg++;
                        ipprg++;
                        (*temptoken) = SCRIPTGDOUBLEOPERATOR_INC;
                        temptoken++;
                        (*temptoken) = SCRIPTGDOUBLEOPERATOR_INC;
                        temptoken++;
                        (*temptoken) = __C('\0');
                      }
                    break;

          case '-': if(*(ipprg+1) == __C('-'))
                      {
                        ipprg++;
                        ipprg++;
                        (*temptoken) = SCRIPTGDOUBLEOPERATOR_DEC;
                        temptoken++;
                        (*temptoken) = SCRIPTGDOUBLEOPERATOR_DEC;
                        temptoken++;
                        (*temptoken) = '\0';
                      }
                    break;
        }

      if(currenttoken[0]) return(tokentype = SCRIPTGTOKENTYPES_DELIMITER);
    }

  XSTRING otherdelimiters(__L("+-*^/%=;:(),'"));

  // Check for other delimiters.
  if(otherdelimiters.FindCharacter((*ipprg))!=XSTRING_NOTFOUND)
    {
      (*temptoken) = (*ipprg);
      ipprg++;
      temptoken++;
      (*temptoken) = '\0';

      return (tokentype = SCRIPTGTOKENTYPES_DELIMITER);
    }

  // Read a quoted string.
  if((*ipprg) == __C('"'))
    {
      ipprg++;

      while(((*ipprg) != __C('"')) && ((*ipprg)!=__C('\r')) && (*ipprg))
        {
          // Check for \n escape sequence.
          if((*ipprg) == __C('\\'))
            {
              if(*(ipprg+1) == __C('n'))
                {
                  ipprg++;
                  (*temptoken++) = __C('\n');
                }
               else
                {
                  if(*(ipprg+1) == __C('r'))
                    {
                      ipprg++;
                      (*temptoken++) = __C('\r');
                    }
                   else
                    {
                      if(*(ipprg+1) == '\\')
                        {
                          ipprg++;
                          (*temptoken++) = __C('\\');
                        }
                    }
                }
            }
           else
            {
              if((temptoken - currenttoken) < SCRIPTG_MAXTOKENLEN)  (*temptoken++) = (*ipprg);
            }

          ipprg++;
        }

        if((*ipprg) == __C('\r') || (*ipprg) == 0)
          {
            HaveError(SCRIPTERRORMSG_SYNTAX);
            return tokentype;
          }

      ipprg++;
      (*temptoken) = '\0';

      return (tokentype = SCRIPTGTOKENTYPES_STRING);
    }

  // Read an integer number.
  if(IsDigit((*ipprg)))
    {
      while(!IsDelimiter((*ipprg)))
        {
          if((temptoken - currenttoken) < SCRIPTG_MAXIDLEN)  (*temptoken++) = (*ipprg);
          ipprg++;
        }
      (*temptoken) = __C('\0');

      return (tokentype = SCRIPTGTOKENTYPES_NUMBER);
    }

  // Read identifier or keyword.
  if(IsAlpha((*ipprg)))
    {
      while(!IsDelimiter((*ipprg)) && ((*ipprg)!=__C('{')) && ((*ipprg)!=__C('}')))
        {
          if((temptoken - currenttoken) < SCRIPTG_MAXIDLEN) (*temptoken++) = (*ipprg);
          ipprg++;
        }

      tokentype = SCRIPTGTOKENTYPES_TEMP;
    }

  (*temptoken) = __C('\0');

  // Determine if token is a keyword or identifier.
  if(tokentype == SCRIPTGTOKENTYPES_TEMP)
    {
      tokenireps = LookUpToken(currenttoken); // convert to internal form

      if(tokenireps)
             tokentype = SCRIPTGTOKENTYPES_KEYWORD; // is a keyword
        else tokentype = SCRIPTGTOKENTYPES_IDENTIFIER;
    }

  // Check for unidentified XCHARacter in file.
  if(tokentype == SCRIPTGTOKENTYPES_UNDEFTT)
    {
      HaveError(SCRIPTERRORMSG_SYNTAX);
    }

  return tokentype;
}





//-------------------------------------------------------------------
//  SCRIPTG::PreScan
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/07/2007 21:47:20
//
//  @return
//  */
//-------------------------------------------------------------------
void SCRIPTG::PreScan()
{
  XCHAR*                position;
  XCHAR*                tempposition;
  SCRIPTGTOKENIREPS     datatype;
  SCRIPTGFUNCTIONTYPE*  functiontype;
  XSTRING               temptoken;
  int                   brace = 0;

  position = ipprg;

  do{ while(brace)
        {
          GetToken();

          if(tokenireps == SCRIPTGTOKENIREPS_END)
            {
              HaveError(SCRIPTERRORMSG_UNBAL_BRACES);
              return;
            }

          if(currenttoken[0] == __C('{')) brace++;
          if(currenttoken[0] == __C('}')) brace--;
        }

      tempposition = ipprg;

      GetToken();


      if(tokenireps==SCRIPTGTOKENIREPS_CHAR || tokenireps==SCRIPTGTOKENIREPS_INT || tokenireps==SCRIPTGTOKENIREPS_STRING)
        {
          datatype = tokenireps;

          GetToken();

          if(tokentype == SCRIPTGTOKENTYPES_IDENTIFIER)
            {
              temptoken = currenttoken;

              GetToken();

              if(currenttoken[0] != __C('('))
                {
                  ipprg = tempposition;
                  DeclareGlobalVariable();
                }
               else
                {
                  if(currenttoken[0] == __C('('))
                    {
                      functiontype = new SCRIPTGFUNCTIONTYPE();
                      if(!functiontype) return;

                      for(int c=0;c<(int)functiontable.GetSize();c++)
                        {
                          SCRIPTGFUNCTIONTYPE* functiontype = (SCRIPTGFUNCTIONTYPE*)functiontable.Get(c);

                          if(!functiontype->GetName()->Compare(temptoken))
                            {
                              HaveError(SCRIPTERRORMSG_DUP_FUNC);
                              return;
                            }
                        }

                      functiontype->SetLocation(ipprg);
                      functiontype->SetReturnType(datatype);
                      functiontype->SetName(temptoken.Get());

                      functiontable.Add(functiontype);

                      do{ GetToken();

                        } while(currenttoken[0] != __C(')'));

                    } else PutBackToken();
                }
            }
        }
       else
        {
          if(currenttoken[0] == __C('{')) brace++;
          if(currenttoken[0] == __C('}')) brace--;
        }

    } while(tokenireps != SCRIPTGTOKENIREPS_END);

  if(brace)
    {
      HaveError(SCRIPTERRORMSG_UNBAL_BRACES);
      return;
    }

  ipprg = position;
}




//-------------------------------------------------------------------
//  SCRIPTG::GetArgs
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 16:35:21
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTG::GetArgs()
{
  SCRIPTGVAR* values[SCRIPTG_NUMPARAMS];
  SCRIPTGVAR* value = NULL;
  int        count;

  for(int c=0;c<SCRIPTG_NUMPARAMS;c++)
    {
      values[c] = NULL;
    }

  count = 0;

  GetToken();

  if(*currenttoken != '(')
    {
      HaveError(SCRIPTERRORMSG_PAREN_EXPECTED);
      return;
    }

  // Process a comma-separated list of values.
  do{ value = new SCRIPTGVAR();
      if(!value) break;

      EvalExp((*value));
      values[count] = value; // save temporarily

      GetToken();
      count++;

    } while(*currenttoken == ',');

  count--;

  // Now, push on local_var_stack in reverse order.
  for(; count>=0; count--)
    {
      value = values[count];
      if(value)
        {
          value->SetIsArg(true);

          localvarsstack.Add(value);
        }
    }
}



//-------------------------------------------------------------------
//  SCRIPTG::GetParams
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 21:49:04
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTG::GetParams()
{
  SCRIPTGVAR* var;
  int        indexstack;

  indexstack = localvarsstack.GetSize()-1;

  // Process comma-separated list of parameters.
  do{
      GetToken();

      var = (SCRIPTGVAR*)localvarsstack.Get(indexstack);

      if(currenttoken[0] != __C(')') )
        {
          if(tokenireps != SCRIPTGTOKENIREPS_INT   &&
             tokenireps != SCRIPTGTOKENIREPS_CHAR  &&
             tokenireps != SCRIPTGTOKENIREPS_STRING)
          {
            HaveError(SCRIPTERRORMSG_TYPE_EXPECTED);
            return;
          }

          var->SetType(tokenireps);

          GetToken();

          var->SetName(currenttoken);
          var->SetNotHaveReservedSize(true);

          GetToken();

          indexstack--;
        }
       else break;

    } while(currenttoken[0] == __C(','));

  if(currenttoken[0] != __C(')'))
    {
      HaveError(SCRIPTERRORMSG_PAREN_EXPECTED);
    }
}


//-------------------------------------------------------------------
//  SCRIPTG::FunctionReturn
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/07/2007 9:38:26
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTG::FunctionReturn()
{
  SCRIPTGVAR value;

  value.Clear();

  SCRIPTGFUNCTIONTYPE* function = GetFunction(currentfunction);

  EvalExp(value);

  value.SetNotHaveReservedSize(true);

  if(function)
    {
      if(function->GetReturnType() == SCRIPTGTOKENIREPS_STRING)
        {
          returnvalue.SetType(SCRIPTGTOKENIREPS_STRING);

          XSTRING* string = new XSTRING();
          if(string)
            {
              XSTRING* stringvalue = value.GetValueString();
              (*string) = (*stringvalue);

              returnvalue.SetValueString(string);
            }

          return;
        }
    }

  returnvalue.SetType(SCRIPTGTOKENIREPS_INT);
  returnvalue.SetValueInteger(value.GetValueInteger());
}



//-------------------------------------------------------------------
//  SCRIPTG::Exec_IF
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/07/2007 9:40:37
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTG::Exec_IF()
{
  SCRIPTGVAR cond;

  cond.Clear();

  EvalExp(cond);

  if(cond.GetValueInteger())
    {
      if(currenttoken[0] != __C('{'))
        {
          HaveError(SCRIPTERRORMSG_BRACE_EXPECTED);
          return;
        }

      Interpret();
    }
   else
    {
      FindEndofBlock(); // find start of next line
      GetToken();

      if(tokenireps != SCRIPTGTOKENIREPS_ELSE)
        {
          PutBackToken();
          return;
        }

      GetToken();

      if(currenttoken[0] != __C('{'))
        {
          HaveError(SCRIPTERRORMSG_BRACE_EXPECTED);
          return;
        }

      PutBackToken();

      Interpret();
    }
}


//-------------------------------------------------------------------
//  SCRIPTG::Exec_SWITCH
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/07/2007 10:25:32
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTG::Exec_SWITCH()
{
  SCRIPTGVAR sval;
  SCRIPTGVAR cval;
  int       brace;

  EvalExp(sval);

  if(currenttoken[0] != __C('{'))
    {
      HaveError(SCRIPTERRORMSG_BRACE_EXPECTED);
      return;
    }

  nestscopestack.Add(localvarsstack.GetSize());

  for(;;)
    {
      brace = 1;

      do { GetToken();

           if(currenttoken[0] == '{')
                   brace++;
              else if(currenttoken[0] == '}') brace--;

         } while(tokenireps != SCRIPTGTOKENIREPS_CASE && tokenireps != SCRIPTGTOKENIREPS_END && brace);

      if(!brace) break;

      if(tokenireps == SCRIPTGTOKENIREPS_END)
        {
          HaveError(SCRIPTERRORMSG_SYNTAX);
          return;
        }

      EvalExp(cval);

      GetToken();

      if(currenttoken[0] != __C(':'))
        {
          HaveError(SCRIPTERRORMSG_COLON_EXPECTED);
          return;
        }

      if(cval.GetValueInteger() == sval.GetValueInteger())
        {
          brace = 1;

          do{ Interpret();

              if(currenttoken[0] == __C('{'))
                    brace++;
               else if(currenttoken[0] == __C('}')) brace--;

            } while(!breakfound && tokenireps != SCRIPTGTOKENIREPS_END && brace);

          while(brace)
            {
              GetToken();

              if(currenttoken[0] == __C('{'))
                     brace++;
                else if(currenttoken[0] == __C('}')) brace--;
            }

          breakfound = false;

          break;
        }
    }
}


//-------------------------------------------------------------------
//  SCRIPTG::Exec_WHILE
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/07/2007 11:05:39
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTG::Exec_WHILE()
{
  SCRIPTGVAR cond;
  XCHAR*        templocation;

  PutBackToken();

  templocation = ipprg;

  GetToken();

  EvalExp(cond);

  if(currenttoken[0] != __C('{'))
    {
      HaveError(SCRIPTERRORMSG_BRACE_EXPECTED);
      return;
    }

  if(cond.GetValueInteger())
    {
      Interpret();
    }
   else
    {
      FindEndofBlock();
      return;
    }

  ipprg = templocation;

  if(breakfound)
    {
      do { GetToken();

         } while((currenttoken[0] != __C('{')) && (tokenireps != SCRIPTGTOKENIREPS_END));

      PutBackToken();

      breakfound = false;

      FindEndofBlock();

      return;
    }
}


//-------------------------------------------------------------------
//  SCRIPTG::Exec_DO
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/07/2007 11:10:26
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTG::Exec_DO()
{
  SCRIPTGVAR  cond;
  XCHAR*        templocation;

  PutBackToken();

  templocation = ipprg;

  GetToken();
  GetToken();

  if(currenttoken[0] != __C('{'))
    {
      HaveError(SCRIPTERRORMSG_BRACE_EXPECTED);
      return;
    }

  PutBackToken();

  Interpret();

  if(breakfound)
    {
      ipprg = templocation;

      do{ GetToken();

        } while((currenttoken[0] != __C('{')) && (tokenireps != SCRIPTGTOKENIREPS_END));

      PutBackToken();

      FindEndofBlock();

      do{ GetToken();

        } while((currenttoken[0] != __C(';')) && (tokenireps != SCRIPTGTOKENIREPS_END));

      if(tokenireps == SCRIPTGTOKENIREPS_END)
        {
          HaveError(SCRIPTERRORMSG_SYNTAX);
          return;
        }

      breakfound = false;
      return;
    }

  GetToken();

  if(tokenireps != SCRIPTGTOKENIREPS_WHILE)
    {
      HaveError(SCRIPTERRORMSG_WHILE_EXPECTED);
      return;
    }

  EvalExp(cond);

  if(cond.GetValueInteger()) ipprg = templocation;
}



//-------------------------------------------------------------------
//  SCRIPTG::Exec_FOR
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/07/2007 11:16:59
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTG::Exec_FOR()
{
  SCRIPTGVAR cond;
  XCHAR*        templocation;
  XCHAR*        templocation2;
  int           parent;

  GetToken();

  EvalExp(cond);

  if(currenttoken[0] != __C(';'))
    {
      HaveError(SCRIPTERRORMSG_SEMI_EXPECTED);
      return;
    }

  ipprg++;

  templocation = ipprg;

  for(;;)
    {

      EvalExp(cond);
      if(currenttoken[0] != __C(';'))
        {
          HaveError(SCRIPTERRORMSG_SEMI_EXPECTED);
          return;
        }

      ipprg++;
      templocation2 = ipprg;

      parent = 1;
      while(parent)
        {
          GetToken();

          if(currenttoken[0] == __C('(')) parent++;
          if(currenttoken[0] == __C(')')) parent--;
        }

      GetToken();

      if(currenttoken[0] != __C('{'))
        {
          HaveError(SCRIPTERRORMSG_BRACE_EXPECTED);
          return;
        }

      PutBackToken();

      if(cond.GetValueInteger())
        {
          Interpret();
        }
       else
        {
          FindEndofBlock();
          return;
        }

      ipprg = templocation2;

      if(breakfound)
        {

          do{ GetToken();

            } while(currenttoken[0] != '{' && tokenireps!= SCRIPTGTOKENIREPS_END);

          PutBackToken();

          breakfound = false;

          FindEndofBlock();
          return;
        }


      EvalExp(cond);

      ipprg = templocation;
    }
}


//-------------------------------------------------------------------
//  SCRIPTG::Interpret
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 21:58:25
//
//  @return       void :
//  */
//-------------------------------------------------------------------
bool SCRIPTG::Interpret()
{
  SCRIPTGVAR value;
  int       block = 0;

  do{ if(iscancelexec)
        {
          HaveError(SCRIPTERRORMSG_USERBREAK);
          return false;
        }

      if(errorscript!=SCRIPTERRORMSG_NONE) return false;

      if(breakfound)  return false;
      if(returnfound) return true;

      tokentype = GetToken();

      if(tokentype == SCRIPTGTOKENTYPES_IDENTIFIER || currenttoken[0] == SCRIPTGDOUBLEOPERATOR_INC || currenttoken[0] == SCRIPTGDOUBLEOPERATOR_DEC)
        {
          value.Clear();

          PutBackToken();

          EvalExp(value);

          /*
          if(value.IsReturnValue())
            {
              if(value.GetType() == SCRIPTGTOKENIREPS_STRING)
                {
                  delete value.GetValueString();
                }
            }
          */

          if(currenttoken[0] != ';')
            {
              HaveError(SCRIPTERRORMSG_SEMI_EXPECTED);
              return false;
            }
        }
       else
         if(tokentype==SCRIPTGTOKENTYPES_BLOCK)
          {
            if(currenttoken[0] == '{')
              {
                block = 1;
                nestscopestack.Add(localvarsstack.GetSize());
              }
             else
              {
                localvarsstack.ResizeContents(nestscopestack.GetLast());
                nestscopestack.DeleteLast();

                return false;
              }
          }
         else // is keyword
          {
            switch(tokenireps)
              {
                case SCRIPTGTOKENIREPS_CHAR   :
                case SCRIPTGTOKENIREPS_INT    :
                case SCRIPTGTOKENIREPS_STRING : PutBackToken();
                                                DeclareLocalVariable();
                                                break;

                case SCRIPTGTOKENIREPS_RETURN : FunctionReturn();
                                                returnfound = true;
                                                return true;

                case SCRIPTGTOKENIREPS_IF     : Exec_IF();
                                                break;

                case SCRIPTGTOKENIREPS_ELSE   : FindEndofBlock();
                                                break;

                case SCRIPTGTOKENIREPS_WHILE  : Exec_WHILE();
                                                break;

                case SCRIPTGTOKENIREPS_DO     : Exec_DO();
                                                break;

                case SCRIPTGTOKENIREPS_FOR    : // process a for loop
                                                Exec_FOR();
                                                break;

                case SCRIPTGTOKENIREPS_BREAK  : breakfound = true;

                                                localvarsstack.ResizeContents(nestscopestack.GetLast());
                                                nestscopestack.DeleteLast();

                                                return false;

                case SCRIPTGTOKENIREPS_SWITCH : Exec_SWITCH();
                                                break;

                case SCRIPTGTOKENIREPS_END    : break;


              }
          }

      if(returnfound) return true;

    } while (tokenireps != SCRIPTGTOKENIREPS_END && block);

  return false;
}



//-------------------------------------------------------------------
//  SCRIPTG::Call
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/07/2007 16:22:10
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void SCRIPTG::Call()
{
  XCHAR* location;
  XCHAR* templocation;
  int    localvarstacksize;

  location = FindFunction(currenttoken);
  if(!location)
    {
      HaveError(SCRIPTERRORMSG_FUNC_UNDEF);
      return;
    }
  else
    {
      XSTRING::Set(currentfunction,currenttoken);

      localvarstacksize = localvarsstack.GetSize();

      GetArgs();
      templocation = ipprg;

      functioncallstack.Add(localvarstacksize);

      ipprg = location;

      GetParams();

      returnfound = false;

      Interpret();

      returnfound = false;

      ipprg = templocation;

      if(functioncallstack.IsEmpty())
        {
          HaveError(SCRIPTERRORMSG_RET_NOCALL);
          return;
        }

      localvarsstack.ResizeContents(functioncallstack.GetLast());

      functioncallstack.DeleteLast();
    }
}



/*-------------------------------------------------------------------
//  SCRIPT_ThreadFunction
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/11/2009 10:33:45 p.m.
//
//  @return       void :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
void SCRIPTG::ThreadFunction(void* data)
{
  SCRIPTG* script = (SCRIPTG*)data;
  if(!script) return;

  int returnvaluescript;

  script->SetErrorScript(script->Run(&returnvaluescript));

  script->SetReturnValueScript(returnvaluescript);

  script->GetThread()->Run(false);
}

