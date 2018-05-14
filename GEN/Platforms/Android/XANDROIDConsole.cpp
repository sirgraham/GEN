//------------------------------------------------------------------------------------------
//  XANDROIDCONSOLE.CPP
//
//  ANDROID system class
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

#include "XANDROIDConsole.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XANDROIDCONSOLE::XANDROIDCONSOLE
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:28:59
//
//  @return
//  */
//-------------------------------------------------------------------
XANDROIDCONSOLE::XANDROIDCONSOLE() : XCONSOLE()
{

}



//-------------------------------------------------------------------
//  XANDROIDCONSOLE::~XANDROIDCONSOLE
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:29:20
//
//  @return
//  */
//-------------------------------------------------------------------
XANDROIDCONSOLE::~XANDROIDCONSOLE()
{

}


/*-------------------------------------------------------------------
//  XANDROIDCONSOLE::Maximize
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
bool XANDROIDCONSOLE::Maximize()
{
  setvbuf(stdout,NULL,_IONBF,0);

  return true;
}



/*-------------------------------------------------------------------
//  XANDROIDCONSOLE::Minimize
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
bool XANDROIDCONSOLE::Minimize()
{
  return false;
}



/*-------------------------------------------------------------------
//  XANDROIDCONSOLE::Hide
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
bool XANDROIDCONSOLE::Hide()
{
  return false;
}



/*-------------------------------------------------------------------
//  XANDROIDCONSOLE::IsHide
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
bool XANDROIDCONSOLE::IsHide()
{
  return false;
}



/*-------------------------------------------------------------------
//   XANDROIDCONSOLE::Print
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
bool XANDROIDCONSOLE::Print(XCHAR* string)
{
  if(!string)     return false;
  if(!string[0])  return false;

  wprintf((wchar_t *)string);
  fflush(stdout);


  return true;
}



//-------------------------------------------------------------------
//  XANDROIDCONSOLE::Clear
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/01/2007 21:06:05
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XANDROIDCONSOLE::Clear()
{
  //printf("\033[2J\033[H");
  wprintf(L"\033c");

  return true;
}



//-------------------------------------------------------------------
//  XANDROIDCONSOLE::KBHit
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
bool XANDROIDCONSOLE::KBHit(void)
{
  struct timeval tv;
  fd_set         read_fd;

  tv.tv_sec  = 0;
  tv.tv_usec = 100;

  FD_ZERO(&read_fd);
  FD_SET(0,&read_fd);

  if(select(1, &read_fd, NULL, NULL, &tv) == -1) return false;
  if(FD_ISSET(0,&read_fd))
    {
      return true;
    }

  return false;
}



/*-------------------------------------------------------------------
//  XANDROIDCONSOLE::GetChar
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
int XANDROIDCONSOLE::GetChar()
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


