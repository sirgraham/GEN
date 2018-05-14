//------------------------------------------------------------------------------------------
//  GRPANDROIDFACTORY.CPP
//
//  ANDROID Graphics Factory Class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 08/08/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XFactoryMacros.h"
#include "XDebug.h"

#include "GRPANDROIDScreen.h"
#include "GRPANDROIDFactory.h"
#include "GRPANDROIDContext.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------




/*-------------------------------------------------------------------
//  GRPANDROIDFACTORY::CreateScreen
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/10/2013 16:01:38
//
//  @return       GRPSCREEN* :
//  @param        accesstype :
//  @param        canvasapi :
//  @param        mode :
*/
/*-----------------------------------------------------------------*/
GRPSCREEN* GRPANDROIDFACTORY::CreateScreen()
{
  GRPSCREEN* screen = (GRPSCREEN*)new GRPANDROIDSCREEN();

  if(screen)
    {
      screen->SetDPI(dpi);
    }

  return screen;
}



/*-------------------------------------------------------------------
//  GRPANDROIDFACTORY::DeleteScreen
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/10/2013 16:26:55
//
//  @return       bool :
//  @param        screen :
*/
/*-----------------------------------------------------------------*/
bool GRPANDROIDFACTORY::DeleteScreen(GRPSCREEN* screen)
{
  if(!screen) return false;

  GRPANDROIDSCREEN* _class =  (GRPANDROIDSCREEN*)screen;
  delete _class;

  return true;
}


GRPCONTEXT* GRPANDROIDFACTORY::CreateContext( )
{
  return new GRPANDROIDCONTEXT();
}

bool GRPANDROIDFACTORY::DeleteContext(GRPCONTEXT* context)
{
  delete context;
  return true;
}


void GRPANDROIDFACTORY::GenerateTextureMap()
{
}
