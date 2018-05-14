//------------------------------------------------------------------------------------------
//  XWINDOWSCONSOLE.CPP
//
//  WINDOWSxconsole class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 03/03/2004 12:28:40
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------


#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#include "XString.h"
#include "XFileTXT.h"

#include "XWINDOWSConsole.h"

#include "XMemory.h"



//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XWINDOWSCONSOLE::XWINDOWSCONSOLE
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:28:59
//
//  @return
//  */
//-------------------------------------------------------------------
XWINDOWSCONSOLE::XWINDOWSCONSOLE() : XCONSOLE()
{

}



//-------------------------------------------------------------------
//  XWINDOWSCONSOLE::~XWINDOWSCONSOLE
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2004 12:29:20
//
//  @return
//  */
//-------------------------------------------------------------------
XWINDOWSCONSOLE::~XWINDOWSCONSOLE()
{

}




/*-------------------------------------------------------------------
//  XWINDOWSCONSOLE::GetSize
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/08/2014 11:11:12
//
//  @return       bool :
//
//  @param        width :
//  @param        height :
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSCONSOLE::GetSize(int& width, int& height)
{
  HWND consolehandle = GetConsoleWindow();
  RECT rect;

  width  = 0;
  height = 0;

  if(GetWindowRect(consolehandle, &rect) == FALSE) return false;

  width  = rect.right  - rect.left;
  height = rect.bottom - rect.top;

  return true;
}




/*-------------------------------------------------------------------
//  XWINDOWSCONSOLE::SetSize
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/08/2014 11:11:41
//
//  @return       bool :
//
//  @param        width :
//  @param        height :
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSCONSOLE::SetSize(int width, int height)
{
  HWND consolehandle = GetConsoleWindow();
  RECT rect;
  RECT rectdesktop;

  if(GetWindowRect(GetDesktopWindow(), &rectdesktop) == FALSE) return false;

  if(GetWindowRect(consolehandle, &rect) == FALSE) return false;

  rect.left   = (rectdesktop.right-width)/2;
  rect.top    = (rectdesktop.bottom-height)/2;

  if(MoveWindow(consolehandle, rect.left, rect.top, width, height, TRUE) == FALSE) return false;

  return true;
}



/*-------------------------------------------------------------------
//  XWINDOWSCONSOLE::Maximize
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
bool XWINDOWSCONSOLE::Maximize()
{
  HANDLE                     hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
  HWND                       hwindow  = GetConsoleWindow();
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  if(!hconsole) return false;
  if(!hwindow)  return false;

  GetConsoleScreenBufferInfo(hconsole , &csbi);

  csbi.dwSize.X = 150;
  csbi.dwSize.Y = 60;

  SetConsoleScreenBufferSize(hconsole,csbi.dwSize);

  ShowWindow(hwindow,SW_MAXIMIZE);

  return true;
}



/*-------------------------------------------------------------------
//  XWINDOWSCONSOLE::Minimize
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
bool XWINDOWSCONSOLE::Minimize()
{
  HANDLE                     hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
  HWND                       hwindow  = GetConsoleWindow();
  CONSOLE_SCREEN_BUFFER_INFO csbi;

  if(!hconsole) return false;
  if(!hwindow)  return false;

  GetConsoleScreenBufferInfo(hconsole , &csbi);

  csbi.dwSize.X = 120;
  csbi.dwSize.Y = 50;

  SetConsoleScreenBufferSize(hconsole,csbi.dwSize);

  ShowWindow(hwindow,SW_MINIMIZE);

  return true;
}



/*-------------------------------------------------------------------
//  XWINDOWSCONSOLE::Hide
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
bool XWINDOWSCONSOLE::Hide()
{
  HWND hwindow  = GetConsoleWindow();
  if(!hwindow)  return false;

  ShowWindow(hwindow,SW_HIDE);

  return true;
}



/*-------------------------------------------------------------------
//  XWINDOWSCONSOLE::IsHide
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
bool XWINDOWSCONSOLE::IsHide()
{
  HWND hwindow  = GetConsoleWindow();
  if(!hwindow)  return false;

  return (IsWindowVisible(hwindow))?false:true;
}



/*-------------------------------------------------------------------
//  XWINDOWSCONSOLE::UnHide
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/08/2014 11:09:10
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSCONSOLE::UnHide()
{
  HWND hwindow  = GetConsoleWindow();
  if(!hwindow)  return false;

  ShowWindow(hwindow,SW_SHOW);

  return true;

}



/*-------------------------------------------------------------------
//  XWINDOWSCONSOLE::Print
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/03/2011 9:59:30
//
//  @return       bool :
//  @param        string :
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSCONSOLE::Print(XCHAR* string)
{
  if(!string)     return false;
  if(!string[0])  return false;

  wprintf((wchar_t *)string);

  return true;
}




//-------------------------------------------------------------------
//  XWINDOWSCONSOLE::Clear
/**
//
//
//  @author       Abraham J. Velez
//  @version      07/01/2007 21:06:05
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool XWINDOWSCONSOLE::Clear()
{
  COORD                      coordscreen  = { 0, 0 };                         // Home for the cursor
  DWORD                      ccharswritten;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD                      dwconsize;
  HANDLE                     hconsole    = GetStdHandle(STD_OUTPUT_HANDLE);   // Get the number of character cells in the current buffer.

  if(IsHide()) return false;

  GetConsoleScreenBufferInfo(hconsole, &csbi);

  dwconsize = csbi.dwSize.X * csbi.dwSize.Y;

  FillConsoleOutputCharacter(hconsole, (TCHAR) ' ', dwconsize, coordscreen, &ccharswritten);          // Fill the entire screen with blanks.
  GetConsoleScreenBufferInfo(hconsole, &csbi );                                                       // Get the current text attribute.

  FillConsoleOutputAttribute(hconsole, csbi.wAttributes,dwconsize, coordscreen, &ccharswritten );     // Set the buffer's attributes accordingly.
  SetConsoleCursorPosition  (hconsole, coordscreen );                                                 // Put the cursor at its home coordinates.

  return true;
}




//-------------------------------------------------------------------
//  XWINDOWSCONSOLE::KBHit
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
bool XWINDOWSCONSOLE::KBHit(void)
{
  #ifdef XVISUALC
  if(_kbhit()) return true;
  #endif

  return false;
}



/*-------------------------------------------------------------------
//  XWINDOWSCONSOLE::GetChar
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      01/12/2009 08:53:27 p.m.
//
//  @return
int :
//  */
/*-----------------------------------------------------------------*/
int XWINDOWSCONSOLE::GetChar()
{
  /*
  DWORD mode;
  unsigned int thechar[5] = { 0,0,0,0,0 };
  DWORD count;

  HANDLE hstdin = GetStdHandle(STD_INPUT_HANDLE);         //get a handle to stdin
  if(!hstdin) return 0;

  GetConsoleMode(hstdin,&mode);                           //save the current input mode...

  // Clear the mode, turn off ENABLE_ECHO_INPUT and ENABLE_LINE_INPUT
  // so that the output will not be echoed and will not pause until the end of
  // a line for input.

  SetConsoleMode(hstdin,0);

  ReadConsole(hstdin,&thechar,sizeof(thechar),&count,NULL); // Read in 1 char from the input buffer.

  SetConsoleMode(hstdin, mode);   //restore the current input mode.

  return (int)(thechar[0]&0xFF);
  */

  if(!KBHit()) return 0;

  return   _getch();
}



