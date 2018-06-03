//------------------------------------------------------------------------------------------
//  XLINUXCONSOLE.CPP
//
//  LINUX system class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 03/03/2004 12:28:40
//  Lsdddast Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "XDebugTrace.h"

#include "XLINUXConsole.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XLINUXCONSOLE::XLINUXCONSOLE
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:28:59
//
//  @return
//  */
//-------------------------------------------------------------------
XLINUXCONSOLE::XLINUXCONSOLE() : XCONSOLE()
{

}



//-------------------------------------------------------------------
//  XLINUXCONSOLE::~XLINUXCONSOLE
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:29:20
//
//  @return
//  */
//-------------------------------------------------------------------
XLINUXCONSOLE::~XLINUXCONSOLE()
{

}


/*-------------------------------------------------------------------
//  XLINUXCONSOLE::Maximize
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/10/2008 10:48:13
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XLINUXCONSOLE::Maximize()
{
  setvbuf(stdout,NULL,_IONBF,0);

  return true;
}



/*-------------------------------------------------------------------
//  XLINUXCONSOLE::Minimize
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/10/2008 10:48:13
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XLINUXCONSOLE::Minimize()
{
  return false;
}



/*-------------------------------------------------------------------
//  XLINUXCONSOLE::Hide
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/02/2011 11:27:28
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XLINUXCONSOLE::Hide()
{
  return false;
}



/*-------------------------------------------------------------------
//  XLINUXCONSOLE::IsHide
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/02/2011 11:52:14
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool XLINUXCONSOLE::IsHide()
{
  return false;
}



/*-------------------------------------------------------------------
//   XLINUXCONSOLE::Print
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/02/2011 11:52:14
//
//  @return       bool :
//  @param        string :
//  */
/*-----------------------------------------------------------------*/
bool XLINUXCONSOLE::Print(XCHAR* string)
{
  if(!string)     return false;
  if(!string[0])  return false;

  wprintf((wchar_t *)string);
  fflush(stdout);


  return true;
}



//-------------------------------------------------------------------
//  XLINUXCONSOLE::Clear
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/01/2007 21:06:05
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XLINUXCONSOLE::Clear()
{
  //printf("\033[2J\033[H");
  wprintf(L"\033c");

  return true;
}



//-------------------------------------------------------------------
//  XLINUXCONSOLE::KBHit
/**
//
//
//  @author       Abraham J. Velez
//  @version      12/04/2006 11:59:47
//
//  @return       bool :
//  @param        void :
*/
//-------------------------------------------------------------------
bool XLINUXCONSOLE::KBHit(void)
{
  struct termios oldt;
  struct termios newt;
  int            ch;
  int            oldf;

  tcgetattr(STDIN_FILENO, &oldt);

  newt          = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);

  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);

  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
    {
      ungetc(ch, stdin);
      return true;
    }

  return false;
}



/*-------------------------------------------------------------------
//  XLINUXCONSOLE::GetChar
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/12/2009 08:51:46 p.m.
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int XLINUXCONSOLE::GetChar()
{
  struct termios oldt, newt;
  int ch;

  tcgetattr( STDIN_FILENO, &oldt );               /* Obtenemos atributos del terminal */
  newt = oldt;
  newt.c_lflag &= ~ICANON;
  newt.c_lflag &= ~ECHO;                          /* Eliminamos el Echo */

  tcsetattr( STDIN_FILENO, TCSANOW, &newt );      /* Definimos los nuevos atributos al terminal */

  ch = getchar();

  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );      /* Ponemos los atributos como estaban al principio */

  return ch;
}


