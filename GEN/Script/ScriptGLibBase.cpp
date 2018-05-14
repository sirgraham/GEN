//------------------------------------------------------------------------------------------
//  SCRIPTLIBBASEBASE.CPP
//
//  Script with Basic library
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 19/07/2007 20:10:28
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "XFactory.h"
#include "XSleep.h"
#include "XTimer.h"
#include "XDebug.h"

#include "ScriptG.h"
#include "ScriptGLibBase.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  SCRIPTGLIBBASE::AddLibraryFunctions
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/09/2011 16:56:44
//
//  @return       bool :
//  @param        script :
*/
/*-----------------------------------------------------------------*/
bool SCRIPTGLIBBASE::AddLibraryFunctions(SCRIPT* script)
{
  if(!script) return false;

  this->script = script;

  script->AddLibraryFunction(this, __L("StrEmpty")            , Call_StrEmpty);

  script->AddLibraryFunction(this, __L("StrCpy")              , Call_StrCpy);
  script->AddLibraryFunction(this, __L("StrCat")              , Call_StrCat);

  script->AddLibraryFunction(this, __L("Abs")                 , Call_Abs);
  script->AddLibraryFunction(this, __L("Rand")                , Call_Rand);

  script->AddLibraryFunction(this, __L("RandIntegerRange")    , Call_RandIntegerRange);

  script->AddLibraryFunction(this, __L("Delay")               , Call_Delay);

  script->AddLibraryFunction(this, __L("GetChar")             , Call_GetChar);
  script->AddLibraryFunction(this, __L("PutChar")             , Call_PutChar);

  script->AddLibraryFunction(this, __L("SPrintf")             , Call_SPrintf);
  script->AddLibraryFunction(this, __L("Printf")              , Call_Printf);

  script->AddLibraryFunction(this, __L("GetPathExec")         , Call_GetPathExec);
  script->AddLibraryFunction(this, __L("GetPathScript")       , Call_GetPathScript);

  script->AddLibraryFunction(this, __L("XDEBUG_PRINTCOLOR")   , Call_XDEBUG_PRINTCOLOR);

  return true;
}




/*-------------------------------------------------------------------
//   Call_StrEmpty
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 17:15:57
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
void Call_StrEmpty(SCRIPTLIB* library, SCRIPT* script, void* returnvalue)
{
  if(!script) return;

  SCRIPTGVAR* _returnvalue = (SCRIPTGVAR*)returnvalue;
  if(_returnvalue) _returnvalue->SetType(SCRIPTGTOKENIREPS_UNDEFTOK);


  SCRIPTGVAR params[SCRIPTG_NUMPARAMS];
  memset(params,0,SCRIPTG_NUMPARAMS*sizeof(SCRIPTGVAR));

  int nparams = ((SCRIPTG*)script)->GetFuncParams(params);
  if(!nparams)
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  XSTRING* string = params[0].GetValueString();
  if(string) string->Empty();
}



/*-------------------------------------------------------------------
//   Call_StrCpy
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 17:16:31
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
void Call_StrCpy(SCRIPTLIB* library, SCRIPT* script, void* returnvalue)
{
  if(!script) return;

  SCRIPTGVAR* _returnvalue = (SCRIPTGVAR*)returnvalue;
  if(_returnvalue) _returnvalue->SetType(SCRIPTGTOKENIREPS_UNDEFTOK);

  SCRIPTGVAR params[SCRIPTG_NUMPARAMS];
  memset(params,0,SCRIPTG_NUMPARAMS*sizeof(SCRIPTGVAR));

  int nparams = ((SCRIPTG*)script)->GetFuncParams(params);
  if(nparams!=2)
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  XSTRING* string1 = params[0].GetValueString();
  XSTRING* string2 = params[1].GetValueString();

  if(string1 && string2) (*string1)=(*string2);
}



/*-------------------------------------------------------------------
//   Call_StrCat
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 17:16:48
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
void Call_StrCat(SCRIPTLIB* library, SCRIPT* script, void* returnvalue)
{
  if(!script) return;

  SCRIPTGVAR* _returnvalue = (SCRIPTGVAR*)returnvalue;
  if(_returnvalue) _returnvalue->SetType(SCRIPTGTOKENIREPS_UNDEFTOK);

  SCRIPTGVAR params[SCRIPTG_NUMPARAMS];
  memset(params,0,SCRIPTG_NUMPARAMS*sizeof(SCRIPTGVAR));

  int nparams = ((SCRIPTG*)script)->GetFuncParams(params);
  if(nparams!=2)
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  XSTRING* string1 = params[0].GetValueString();
  XSTRING* string2 = params[1].GetValueString();

  if(string1 && string2) (*string1)+=(*string2);
}




/*-------------------------------------------------------------------
//   Call_Abs
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 17:17:01
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
void Call_Abs(SCRIPTLIB* library, SCRIPT* script, void* returnvalue)
{
  if(!script) return;

  SCRIPTGVAR* _returnvalue = (SCRIPTGVAR*)returnvalue;
  if(_returnvalue)
    {
      _returnvalue->SetType(SCRIPTGTOKENIREPS_INT);
      _returnvalue->SetValueInteger(0);
    }

  SCRIPTGVAR params[SCRIPTG_NUMPARAMS];
  memset(params,0,SCRIPTG_NUMPARAMS*sizeof(SCRIPTGVAR));

  int nparams = ((SCRIPTG*)script)->GetFuncParams(params);
  if(!nparams)
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  if(_returnvalue) _returnvalue->SetValueInteger(abs(params[0].GetValueInteger()));
}




/*-------------------------------------------------------------------
//   Call_Rand
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 17:17:14
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
void Call_Rand(SCRIPTLIB* library, SCRIPT* script, void* returnvalue)
{
  if(!script) return;

  SCRIPTGVAR* _returnvalue = (SCRIPTGVAR*)returnvalue;
  if(_returnvalue)
    {
      _returnvalue->SetType(SCRIPTGTOKENIREPS_INT);
      _returnvalue->SetValueInteger(0);
    }

  ((SCRIPTG*)script)->NotFuncParams();

  if(_returnvalue) _returnvalue->SetValueInteger(rand());
}




/*-------------------------------------------------------------------
//   Call_RandIntegerRange
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 17:17:25
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
void Call_RandIntegerRange(SCRIPTLIB* library, SCRIPT* script , void* returnvalue)
{
  if(!script) return;

  SCRIPTGVAR* _returnvalue = (SCRIPTGVAR*)returnvalue;
  if(_returnvalue)
    {
      _returnvalue->SetType(SCRIPTGTOKENIREPS_INT);
      _returnvalue->SetValueInteger(0);
    }

  SCRIPTGVAR params[SCRIPTG_NUMPARAMS];
  memset(params,0,SCRIPTG_NUMPARAMS*sizeof(SCRIPTGVAR));
  int nparams = ((SCRIPTG*)script)->GetFuncParams(params);

  if(!nparams)
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  int min = (int)params[0].GetValueInteger();
  int max = (int)params[1].GetValueInteger();

  int random = rand()%(max-min)+min;

  if(_returnvalue) _returnvalue->SetValueInteger(random);
}




/*-------------------------------------------------------------------
//   Call_Delay
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 17:17:36
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
void Call_Delay(SCRIPTLIB* library, SCRIPT* script, void* returnvalue)
{
  if(!script) return;

  SCRIPTGVAR* _returnvalue = (SCRIPTGVAR*)returnvalue;
  if(_returnvalue) _returnvalue->SetType(SCRIPTGTOKENIREPS_UNDEFTOK);

  SCRIPTGVAR params[SCRIPTG_NUMPARAMS];
  memset(params,0,SCRIPTG_NUMPARAMS*sizeof(SCRIPTGVAR));

  int nparams = ((SCRIPTG*)script)->GetFuncParams(params);
  if(!nparams)
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  int miliseconds = (int)params[0].GetValueInteger();

  xsleep->MilliSeconds(miliseconds);
}




/*-------------------------------------------------------------------
//   Call_GetChar
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 17:18:00
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
void Call_GetChar(SCRIPTLIB* library, SCRIPT* script, void* returnvalue)
{
  if(!script) return;

  SCRIPTGVAR* _returnvalue = (SCRIPTGVAR*)returnvalue;
  if(_returnvalue)
    {
      _returnvalue->SetType(SCRIPTGTOKENIREPS_CHAR);
      _returnvalue->SetValueCharacter(0);
    }

  ((SCRIPTG*)script)->NotFuncParams();

  if(_returnvalue) _returnvalue->SetValueCharacter(getwchar());
}




/*-------------------------------------------------------------------
//   Call_PutChar
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 17:18:10
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
void Call_PutChar(SCRIPTLIB* library, SCRIPT* script, void* returnvalue)
{
  if(!script) return;

  SCRIPTGVAR* _returnvalue = (SCRIPTGVAR*)returnvalue;
  if(_returnvalue) _returnvalue->SetType(SCRIPTGTOKENIREPS_UNDEFTOK);

  SCRIPTGVAR params[SCRIPTG_NUMPARAMS];
  memset(params,0,SCRIPTG_NUMPARAMS*sizeof(SCRIPTGVAR));

  int nparams = ((SCRIPTG*)script)->GetFuncParams(params);
  if(!nparams)
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  putwchar((XCHAR)params[0].GetValueCharacter());
}




/*-------------------------------------------------------------------
//   Call_SPrintf
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 17:18:20
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
void Call_SPrintf(SCRIPTLIB* library, SCRIPT* script, void* returnvalue)
{
  if(!script) return;

  SCRIPTGVAR* _returnvalue = (SCRIPTGVAR*)returnvalue;
  if(_returnvalue) _returnvalue->SetType(SCRIPTGTOKENIREPS_UNDEFTOK);

  SCRIPTGVAR params[SCRIPTG_NUMPARAMS];
  memset(params,0,SCRIPTG_NUMPARAMS*sizeof(SCRIPTGVAR));

  int nparams = ((SCRIPTG*)script)->GetFuncParams(params);
  if(!nparams)
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  XSTRING* _out = params[0].GetValueString();
  if(!_out) return;

  XSTRING* _mask = params[1].GetValueString();
  if(!_mask) return;

  XCHAR* mask = _mask->Get();

  XSTRING outstring;
  XSTRING string;

  int paramindex = 2;
  int c          = 0;

  while(mask[c])
    {
      switch(mask[c])
        {
          case '%' : {
                        #define MAXTEMPOSTR 32

                        XCHAR param[MAXTEMPOSTR];

                        int  nparam = 1;
                        bool end    = false;

                        memset(param,0,MAXTEMPOSTR*sizeof(XCHAR));
                        param[0] = '%';

                        c++;

                        do{ string.Empty();

                            param[nparam] = mask[c];
                            nparam++;

                            switch(mask[c])
                              {
                                case __C('c')   :
                                case __C('C')   :
                                case __C('d')   :
                                case __C('i')   :
                                case __C('o')   :
                                case __C('u')   :
                                case __C('x')   :
                                case __C('X')   : string.Format(param,(int)params[paramindex].GetValueInteger());
                                                  paramindex++;
                                                  end  = true;
                                                  break;

                                case __C('f')   : string.Format(param,(float)params[paramindex].GetValueUInteger());
                                                  paramindex++;
                                                  end  = true;
                                                  break;

                                case __C('g')   :
                                case __C('G')   :

                                case __C('e')   :
                                case __C('E')   :

                                case __C('n')   :
                                case __C('p')   : end = true;
                                                  break;

                                case __C('s')   :
                                case __C('S')   : { XSTRING* strvalue = params[paramindex].GetValueString();
                                                    paramindex++;
                                                    if(strvalue) string.Format(param,(XCHAR*)strvalue->Get());
                                                    end = true;
                                                  }
                                                  break;

                                case __C('%')   : string = __L("%");
                                                  end = true;
                                                  break;

                                case __C('\0')  : end = true;
                                                  break;

                                    default     : break;
                              }

                            c++;

                          } while(!end);
                      }
                      break;

            default : string.Set(mask[c]);
                      c++;
                      break;
        }

      outstring += string;
    }

  (*_out) = outstring;
}



/*-------------------------------------------------------------------
//   Call_Printf
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 17:18:30
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
void Call_Printf(SCRIPTLIB* library, SCRIPT* script, void* returnvalue)
{
  if(!script) return;

  SCRIPTGVAR* _returnvalue = (SCRIPTGVAR*)returnvalue;
  if(_returnvalue) _returnvalue->SetType(SCRIPTGTOKENIREPS_UNDEFTOK);

  SCRIPTGVAR params[SCRIPTG_NUMPARAMS];
  memset(params,0,SCRIPTG_NUMPARAMS*sizeof(SCRIPTGVAR));

  int nparams = ((SCRIPTG*)script)->GetFuncParams(params);
  if(!nparams)
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  XSTRING* _mask = params[0].GetValueString();
  if(!_mask) return;

  XCHAR*  mask = _mask->Get();
  XSTRING outstring;
  XSTRING string;

  int paramindex = 1;
  int c          = 0;

  while(mask[c])
    {
      switch(mask[c])
        {
          case '%' : {
                        #define MAXTEMPOSTR 32

                        XCHAR param[MAXTEMPOSTR];

                        int  nparam = 1;
                        bool end    = false;

                        memset(param,0,MAXTEMPOSTR*sizeof(XCHAR));
                        param[0] = '%';

                        c++;

                        do{ string.Empty();

                            param[nparam] = mask[c];
                            nparam++;

                            switch(mask[c])
                              {
                                case __C('c')   :
                                case __C('C')   :
                                case __C('d')   :
                                case __C('i')   :
                                case __C('o')   :
                                case __C('u')   :
                                case __C('x')   :
                                case __C('X')   : string.Format(param,(int)params[paramindex].GetValueInteger());
                                                  paramindex++;
                                                  end  = true;
                                                  break;

                                case __C('f')   : string.Format(param,(float)params[paramindex].GetValueUInteger());
                                                  paramindex++;
                                                  end  = true;
                                                  break;

                                case __C('g')   :
                                case __C('G')   :

                                case __C('e')   :
                                case __C('E')   :

                                case __C('n')   :
                                case __C('p')   : end = true;
                                                  break;

                                case __C('s')   :
                                case __C('S')   : { XSTRING* strvalue = params[paramindex].GetValueString();
                                                    paramindex++;
                                                    if(strvalue) string.Format(param,(XCHAR*)strvalue->Get());
                                                    end = true;
                                                  }
                                                  break;

                                case __C('%')   : string = __L("%");
                                                  end = true;
                                                  break;

                                case __C('\0')  : end = true;
                                                  break;

                                      default   : break;
                              }

                            c++;

                          } while(!end);
                      }
                      break;

            default : string.Set(mask[c]);
                      c++;
                      break;
        }

      outstring += string;
    }


  wprintf((XCHAR *)outstring.Get());

}




/*-------------------------------------------------------------------
//   Call_GetPathExec
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 17:18:41
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
void Call_GetPathExec(SCRIPTLIB* library, SCRIPT* script, void* returnvalue)
{
  if(!script) return;

  SCRIPTGVAR* _returnvalue = (SCRIPTGVAR*)returnvalue;
  if(_returnvalue) _returnvalue->SetType(SCRIPTGTOKENIREPS_UNDEFTOK);

  SCRIPTGVAR params[SCRIPTG_NUMPARAMS];
  memset(params,0,SCRIPTG_NUMPARAMS*sizeof(SCRIPTGVAR));

  int nparams = ((SCRIPTG*)script)->GetFuncParams(params);
  if(!nparams)
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  XSTRING* path = params[0].GetValueString();

  script->GetPath()->GetPath((*path));
}



/*-------------------------------------------------------------------
//   Call_GetPathScript
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 17:18:50
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
void Call_GetPathScript(SCRIPTLIB* library, SCRIPT* script, void* returnvalue)
{
  if(!script) return;

  SCRIPTGVAR* _returnvalue = (SCRIPTGVAR*)returnvalue;
  if(_returnvalue) _returnvalue->SetType(SCRIPTGTOKENIREPS_UNDEFTOK);

  SCRIPTGVAR params[SCRIPTG_NUMPARAMS];
  memset(params,0,SCRIPTG_NUMPARAMS*sizeof(SCRIPTGVAR));

  int nparams = ((SCRIPTG*)script)->GetFuncParams(params);
  if(!nparams)
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  XSTRING* path = params[0].GetValueString();

  (*path)  = script->GetPath()->Get();
}





/*-------------------------------------------------------------------
//   Call_XDEBUG_PRINTCOLOR
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  06/03/2018 17:19:06
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    void* :
//
*//*-----------------------------------------------------------------*/
void Call_XDEBUG_PRINTCOLOR(SCRIPTLIB* library, SCRIPT* script , void* returnvalue)
{
  if(!script) return;

  SCRIPTGVAR* _returnvalue = (SCRIPTGVAR*)returnvalue;
  if(_returnvalue) _returnvalue->SetType(SCRIPTGTOKENIREPS_UNDEFTOK);

  SCRIPTGVAR params[SCRIPTG_NUMPARAMS];
  memset(params,0,SCRIPTG_NUMPARAMS*sizeof(SCRIPTGVAR));

  int nparams = ((SCRIPTG*)script)->GetFuncParams(params);

  if(!nparams)
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  // first param is the colorç
  XDWORD color    = params[0].GetValueInteger();

  //second is the print mask
  XSTRING* _mask = params[1].GetValueString();
  if(!_mask) return;

  XCHAR*  mask = _mask->Get();
  XSTRING outstring;
  XSTRING string;

  int paramindex = 2;
  int c          = 0;

  while(mask[c])
    {
      switch(mask[c])
        {
          case '%' : {
                        #define MAXTEMPOSTR 32

                        XCHAR param[MAXTEMPOSTR];

                        int  nparam = 1;
                        bool end    = false;

                        memset(param,0,MAXTEMPOSTR*sizeof(XCHAR));
                        param[0] = '%';

                        c++;

                        do{ string.Empty();

                            param[nparam] = mask[c];
                            nparam++;

                            switch(mask[c])
                              {
                                case __C('c')   :
                                case __C('C')   :
                                case __C('d')   :
                                case __C('i')   :
                                case __C('o')   :
                                case __C('u')   :
                                case __C('x')   :
                                case __C('X')   : string.Format(param,(int)params[paramindex].GetValueInteger());
                                                  paramindex++;
                                                  end  = true;
                                                  break;

                                case __C('f')   : string.Format(param,(float)params[paramindex].GetValueUInteger());
                                                  paramindex++;
                                                  end  = true;
                                                  break;

                                case __C('g')   :
                                case __C('G')   :

                                case __C('e')   :
                                case __C('E')   :

                                case __C('n')   :
                                case __C('p')   : end = true;
                                                  break;

                                case __C('s')   :
                                case __C('S')   : { XSTRING* strvalue = params[paramindex].GetValueString();
                                                    paramindex++;
                                                    if(strvalue) string.Format(param,(XCHAR*)strvalue->Get());
                                                    end = true;
                                                  }
                                                  break;

                                case __C('%')   : string = __L("%");
                                                  end = true;
                                                  break;

                                case __C('\0')  : end = true;
                                                  break;

                                      default   : break;
                              }

                            c++;

                          } while(!end);
                      }
                      break;

            default : string.Set(mask[c]);
                      c++;
                      break;
        }

      outstring += string;
    }


  XDEBUG_PRINTCOLOR(color, outstring.Get());
}



