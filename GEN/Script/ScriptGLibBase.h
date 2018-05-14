//------------------------------------------------------------------------------------------
//  SCRIPTGLIBBASE.H
//
/**
// \class
//
//  Script Basic library
//
//  @author  Abraham J. Velez
//  @version 19/07/2007 20:06:27
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _SCRIPTGLIBBASE_H_
#define _SCRIPTGLIBBASE_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "Script.h"
#include "ScriptLib.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define SCRIPTGLIBBASE_NAME __L("ScriptGLibBase")

//---- CLASS -------------------------------------------------------------------------------

class SCRIPTGLIBBASE : public SCRIPTLIB
{
  public:
                          SCRIPTGLIBBASE            () : SCRIPTLIB(SCRIPTGLIBBASE_NAME)
                          {
                            Clean();
                          }

    virtual              ~SCRIPTGLIBBASE            ()
                          {
                            Clean();
                          }

    bool                  AddLibraryFunctions     (SCRIPT* script);

  private:

    void                  Clean                   ()
                          {

                          }
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

void    Call_StrEmpty             (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);
void    Call_StrCpy               (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);
void    Call_StrCat               (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);

void    Call_Abs                  (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);
void    Call_Rand                 (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);
void    Call_RandIntegerRange     (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);

void    Call_Delay                (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);

void    Call_GetChar              (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);
void    Call_PutChar              (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);

void    Call_SPrintf              (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);
void    Call_Printf               (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);

void    Call_GetPathExec          (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);
void    Call_GetPathScript        (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);

void    Call_XDEBUG_PRINTCOLOR    (SCRIPTLIB* library, SCRIPT* script, void* returnvalue);

#endif

