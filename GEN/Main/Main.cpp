//------------------------------------------------------------------------------------------
//  MAIN.CPP
//
//  2 Main Function + Main Loop
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 10/01/2001
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

//---- INCLUDES ----------------------------------------------------------------------------

#include "XString.h"

#include "Main.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------



//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  MAIN::MAIN
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   12:27:35
//
//  @return
*/
//-------------------------------------------------------------------
MAIN::MAIN()
{
  Clean();
}


//-------------------------------------------------------------------
//  MAIN::~MAIN
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   12:27:35
//
//  @return
*/
//-------------------------------------------------------------------
MAIN::~MAIN()
{
  End();

  Clean();
}



//-------------------------------------------------------------------
//  MAIN::Update
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   11:17:1
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool MAIN::Update()
{
  return true;
}



//-------------------------------------------------------------------
//  MAIN::End
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   11:10:57
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool MAIN::End()
{
  return true;
}




/*-------------------------------------------------------------------
//  MAIN::GetExecParams
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/06/2012 10:12:38
//
//  @return       XVECTOR<XSTRING*>* :
//  */
/*-----------------------------------------------------------------*/
XVECTOR<XSTRING*>* MAIN::GetExecParams()
{
  return &execparams;
}






/*-------------------------------------------------------------------
//  MAIN::DeleteAllExecParams
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/06/2012 19:07:14
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool MAIN::DeleteAllExecParams()
{
  if(execparams.IsEmpty()) return false;

  execparams.DeleteContents();

  execparams.DeleteAll();

  return true;
}



//-------------------------------------------------------------------
//  MAIN::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/08/2003 12:09:01
//
//  @return
//  */
//-------------------------------------------------------------------
void MAIN::Clean()
{
  typesystem    = MAIN_SYSTEM_UNKNOWN;
  xlogexception = NULL;
}

