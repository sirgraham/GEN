//------------------------------------------------------------------------------------------
//  SCRIPTLIBBASE.CPP
//
//  Script Basic library
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

#include "ScriptLibBase.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  SCRIPTLIBBASE::AddLibraryFunctions
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
bool SCRIPTLIBBASE::AddLibraryFunctions(SCRIPT* script)
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
//  @version  09/03/2018 13:14:08
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    XVECTOR<XVARIANT*>* :
//  @param    XVARIANT* :
//
*//*-----------------------------------------------------------------*/
void Call_StrEmpty(SCRIPTLIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  XSTRING* string = (XSTRING*)params->Get(0)->GetData();
  if(string) string->Empty();
}




/*-------------------------------------------------------------------
//   Call_StrCpy
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  09/03/2018 13:14:31
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    XVECTOR<XVARIANT*>* :
//  @param    XVARIANT* :
//
*//*-----------------------------------------------------------------*/
void Call_StrCpy(SCRIPTLIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  XSTRING* string1 = (XSTRING*)params->Get(0)->GetData();
  XSTRING* string2 = (XSTRING*)params->Get(1)->GetData();

  if(string1 && string2) (*string1)=(*string2);
}




/*-------------------------------------------------------------------
//   Call_StrCat
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  09/03/2018 13:14:46
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    XVECTOR<XVARIANT*>* :
//  @param    XVARIANT* :
//
*//*-----------------------------------------------------------------*/
void Call_StrCat(SCRIPTLIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  XSTRING* string1 = (XSTRING*)params->Get(0)->GetData();
  XSTRING* string2 = (XSTRING*)params->Get(1)->GetData();

  if(string1 && string2) (*string1)+=(*string2);
}




/*-------------------------------------------------------------------
//   Call_Abs
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  09/03/2018 13:15:03
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    XVECTOR<XVARIANT*>* :
//  @param    XVARIANT* :
//
*//*-----------------------------------------------------------------*/
void Call_Abs(SCRIPTLIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  (*returnvalue) = abs((*params->Get(0)));
}




/*-------------------------------------------------------------------
//   Call_Rand
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  09/03/2018 13:15:18
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    XVECTOR<XVARIANT*>* :
//  @param    XVARIANT* :
//
*//*-----------------------------------------------------------------*/
void Call_Rand(SCRIPTLIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  (*returnvalue) = rand();
}




/*-------------------------------------------------------------------
//   Call_RandIntegerRange
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  09/03/2018 13:15:32
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    XVECTOR<XVARIANT*>* :
//  @param    XVARIANT* :
//
*//*-----------------------------------------------------------------*/
void Call_RandIntegerRange(SCRIPTLIB* library, SCRIPT* script , XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(params->GetSize()<2)
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  int min = (int)(*params->Get(0));
  int max = (int)(*params->Get(1));

  (*returnvalue) = (int)rand()%(max-min)+min;
}




/*-------------------------------------------------------------------
//   Call_Delay
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  09/03/2018 13:15:51
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    XVECTOR<XVARIANT*>* :
//  @param    XVARIANT* :
//
*//*-----------------------------------------------------------------*/
void Call_Delay(SCRIPTLIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  int miliseconds = (int)(*params->Get(0));

  xsleep->MilliSeconds(miliseconds);
}




/*-------------------------------------------------------------------
//   Call_GetChar
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  09/03/2018 13:16:07
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    XVECTOR<XVARIANT*>* :
//  @param    XVARIANT* :
//
*//*-----------------------------------------------------------------*/
void Call_GetChar(SCRIPTLIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  (*returnvalue) = getwchar();
}




/*-------------------------------------------------------------------
//   Call_PutChar
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  09/03/2018 13:16:21
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    XVECTOR<XVARIANT*>* :
//  @param    XVARIANT* :
//
*//*-----------------------------------------------------------------*/
void Call_PutChar(SCRIPTLIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  XCHAR character = (*params->Get(0));

  putwchar(character);
}




/*-------------------------------------------------------------------
//   Call_SPrintf
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  09/03/2018 13:16:37
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    XVECTOR<XVARIANT*>* :
//  @param    XVARIANT* :
//
*//*-----------------------------------------------------------------*/
void Call_SPrintf(SCRIPTLIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  XVARIANT  variantout = (*params->Get(0));
  XCHAR*    out = variantout;
  XSTRING  _out = out;

  XVARIANT  variantmask = (*params->Get(1));
  XCHAR*    mask = variantmask;

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

                        switch(mask[c])
                              {
                                case __C('c')   :
                                case __C('C')   :
                                case __C('d')   :
                                case __C('i')   :
                                case __C('o')   :
                                case __C('u')   :
                                case __C('x')   :
                                case __C('X')   : { XVARIANT variantparam = (*params->Get(paramindex));
                                                    string.Format(param,(int)variantparam);
                                                    paramindex++;
                                                    end  = true;
                                                  }
                                                  break;

                                case __C('f')   : { XVARIANT variantparam = (*params->Get(paramindex));
                                                    string.Format(param,(float)variantparam);
                                                    paramindex++;
                                                    end  = true;
                                                  }
                                                  break;

                                case __C('g')   :
                                case __C('G')   :

                                case __C('e')   :
                                case __C('E')   :

                                case __C('n')   :
                                case __C('p')   : end = true;
                                                  break;

                                case __C('s')   :
                                case __C('S')   : { XVARIANT variantparam = (*params->Get(paramindex));
                                                    paramindex++;
                                                    string.Format(param,(XCHAR*)variantparam);
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

                      }
                      break;

            default : string.Set(mask[c]);
                      c++;
                      break;
        }

      outstring += string;
    }

  _out = outstring;
}



/*-------------------------------------------------------------------
//   Call_Printf
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  09/03/2018 13:16:53
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    XVECTOR<XVARIANT*>* :
//  @param    XVARIANT* :
//
*//*-----------------------------------------------------------------*/
void Call_Printf(SCRIPTLIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  XVARIANT  variantmask = (*params->Get(0));
  XCHAR*    mask = variantmask;
  XSTRING   outstring;
  XSTRING   string;

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
                                case __C('X')   : { XVARIANT variantparam = (*params->Get(paramindex));
                                                    string.Format(param,(int)variantparam);
                                                    paramindex++;
                                                    end  = true;
                                                  }
                                                  break;

                                case __C('f')   : { XVARIANT variantparam = (*params->Get(paramindex));
                                                    string.Format(param,(float)variantparam);
                                                    paramindex++;
                                                    end  = true;
                                                  }
                                                  break;

                                case __C('g')   :
                                case __C('G')   :

                                case __C('e')   :
                                case __C('E')   :

                                case __C('n')   :
                                case __C('p')   : end = true;
                                                  break;

                                case __C('s')   :
                                case __C('S')   : { XVARIANT variantparam = (*params->Get(paramindex));
                                                    paramindex++;
                                                    string.Format(param,(XCHAR*)variantparam);
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
//   Call_GetPathScript
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  09/03/2018 13:17:25
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    XVECTOR<XVARIANT*>* :
//  @param    XVARIANT* :
//
*//*-----------------------------------------------------------------*/
void Call_GetPathScript(SCRIPTLIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

  XVARIANT  variantmask = (*params->Get(0));
  variantmask  = script->GetPath()->Get();
}





/*-------------------------------------------------------------------
//   Call_XDEBUG_PRINTCOLOR
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  09/03/2018 13:17:41
//
//  @return   void :
//
//  @param    SCRIPTLIB* :
//  @param    SCRIPT* :
//  @param    XVECTOR<XVARIANT*>* :
//  @param    XVARIANT* :
//
*//*-----------------------------------------------------------------*/
void Call_XDEBUG_PRINTCOLOR(SCRIPTLIB* library, SCRIPT* script, XVECTOR<XVARIANT*>* params, XVARIANT* returnvalue)
{
  if(!library)      return;
  if(!script)       return;
  if(!params)       return;
  if(!returnvalue)  return;

  returnvalue->Set();

  if(!params->GetSize())
    {
      script->HaveError(SCRIPTERRORMSG_INSUF_PARAMS);
      return;
    }

    // first param is the colorç
  XDWORD    color       = (*params->Get(0));
  XVARIANT  variantmask = (*params->Get(1));
  XCHAR*    mask = variantmask;
  XSTRING   outstring;
  XSTRING   string;

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
                                case __C('X')   : { XVARIANT variantparam = (*params->Get(paramindex));
                                                    string.Format(param,(int)variantparam);
                                                    paramindex++;
                                                    end  = true;
                                                  }
                                                  break;

                                case __C('f')   : { XVARIANT variantparam = (*params->Get(paramindex));
                                                    string.Format(param,(float)variantparam);
                                                    paramindex++;
                                                    end  = true;
                                                  }
                                                  break;

                                case __C('g')   :
                                case __C('G')   :

                                case __C('e')   :
                                case __C('E')   :

                                case __C('n')   :
                                case __C('p')   : end = true;
                                                  break;

                                case __C('s')   :
                                case __C('S')   : { XVARIANT variantparam = (*params->Get(paramindex));
                                                    paramindex++;
                                                    string.Format(param,(XCHAR*)variantparam);
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



