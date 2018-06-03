//------------------------------------------------------------------------------------------
//  WINDOWSMAIN.CPP
//
//  WINDOWS Main class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 10/01/2001
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------



//---- INCLUDES ----------------------------------------------------------------------------

#include <winsock2.h>
#include <windows.h>
#include <float.h>

#pragma warning(push)
#pragma warning (disable : 4091)
#include <dbghelp.h>
#pragma warning(pop)

#include "XWINDOWSFactory.h"
#include "XWINDOWSSleep.h"
#include "XWINDOWSSystem.h"

#ifdef GRP_ACTIVE
#include "GRPFactory.h"
#include "GRPWINDOWSFactory.h"
#include "GRPWINDOWSScreen.h"
#endif

#ifdef INP_ACTIVE
#include "INPWINDOWSDeviceKeyboard.h"
#include "INPWINDOWSDeviceMouse.h"
#include "INPWINDOWSFactory.h"
#include "INPManager.h"
#include "INPFactory.h"
#endif

#ifdef SND_ACTIVE
#include "SNDWINDOWSFactory.h"
#endif

#ifdef DIO_ACTIVE
#include "DIOWINDOWSFactory.h"
#ifdef DIOALERTS_ACTIVE
#include "DIOAlerts.h"
#endif
#endif

#include "XFactory.h"
#include "XPath.h"
#include "XThreadCollected.h"
#include "XPathsManager.h"
#include "XDateTime.h"
#include "XTimer.h"
#include "XVector.h"
#include "XLanguage.h"
#include "XLog.h"
#include "XDebugTrace.h"
#include "XPublisher.h"
#include "XFile.h"

#include "WINDOWSMain.h"
#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------

#ifdef XDEBUG_TRACE
XWINDOWSDEBUGTRACE windowsdebugtrace;
#endif

WINDOWSMAIN        windowsmain;
XSTRING            allexceptiontext;

//---- INTERNAL PROTOTYPES -----------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  WINDOWSMAIN::WINDOWSMAIN
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   12:27:35
//
//  @return
*/
//-------------------------------------------------------------------
WINDOWSMAIN::WINDOWSMAIN()
{
  Clean();
}


//-------------------------------------------------------------------
//  WINDOWSMAIN::~WINDOWSMAIN
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   12:27:35
//
//  @return
*/
//-------------------------------------------------------------------
WINDOWSMAIN::~WINDOWSMAIN()
{
  Clean();
}



/*-------------------------------------------------------------------
//  WINDOWSMAIN::Ini
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      15/03/2012 13:45:44
//
//  @return       bool :
//  @param        xpath :
//  @param        mainscreenhandle :
//  @param        x :
//  @param        y :
*/
/*-----------------------------------------------------------------*/
bool WINDOWSMAIN::Ini(XSTRING* xpath)
{
  // need to enable FPE manually additionaly to the compiler flag
  // disabled right now as OpenAL has a FPE exception in windows, need to look into it
  // also windows timers and so on
  #ifdef FPE_ACTIVE
  _controlfp_s(NULL, 0, _EM_ZERODIVIDE/* | _EM_OVERFLOW | _EM_UNDERFLOW | _EM_INVALID*/);
  #endif

  #ifdef _CONSOLE
  if(!SetConsoleCtrlHandler((PHANDLER_ROUTINE)Exception_ConsoleHandler, TRUE)) return false;
  #endif

  #if defined(DIO_ACTIVE) || defined(DIOUDP_ACTIVE) || defined(DIOTCPIP_ACTIVE) || defined(DIOBLUETOOTH_ACTIVE)
  XWORD   versionrequested = WINSOCK_VERSION;
  WSADATA data;
  int     error;

  error = WSAStartup(versionrequested, &data);
  if(error) WSACleanup();

  if(data.wVersion != versionrequested) WSACleanup();
  #endif

  typesystem = MAIN_SYSTEM_WINDOWS;

  xfactory = (XFACTORY*)new XWINDOWSFACTORY();
  if(!xfactory) return false;

  XSLEEP::SetInstance(new XWINDOWSSLEEP());
  if(!XSLEEP::GetIsInstanced()) return false;

  if(xpath)
    {
      XPATHSMANAGER::GetInstance().GetAppExecPath()->Set(xpath->Get());
      XPATHSMANAGER::GetInstance().AddPathSection(XPATHSMANAGERSECTIONTYPE_ROOT , (*xpath));
    }

  #ifdef XDEBUG_VIRTUALCLOCKTICK
  xtimerclock = new XTIMERCLOCK();
  if(!xtimerclock) return false;
  #endif

  XSYSTEM::SetInstance(new XWINDOWSSYSTEM());
  if(!XSYSTEM::GetIsInstanced()) return false;
  
  xlanguage = new XLANGUAGE();
  if(!xlanguage) return false;

  #ifdef GRP_ACTIVE
  grpfactory =  (GRPFACTORY*)new GRPWINDOWSFACTORY();
  if(!grpfactory) return false;
  #endif

  #ifdef DIO_ACTIVE
  diofactory = (DIOFACTORY*)new DIOWINDOWSFACTORY();
  if(!diofactory) return false;
  #endif

  #ifdef SND_ACTIVE
  SNDFACTORY::Instance=new SNDWINDOWSFACTORY();
  if (!SNDFACTORY::Instance) return false;
  SNDFACTORY::Get()->IniEvents();
  #endif

  #ifdef INP_ACTIVE
  INPFACTORY::instance = new INPWINDOWSFACTORY();
  if (!INPFACTORY::instance) return false;
  #endif

  xapplication  = XAPPLICATION::Create();
  if(!xapplication) return false;

  xapplication->SetMain(&windowsmain);

  if(!xapplication->IniApplication(GetExecParams())) return false;

  return true;
}



//-------------------------------------------------------------------
//  WINDOWSMAIN::Update
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   11:17:1
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool WINDOWSMAIN::Update()
{
  if(!xapplication) return false;

  #ifdef GRP_ACTIVE
  MSG  msg;
  if(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
    {
      if(msg.message == WM_QUIT)
        {
          exittype      = (XAPPLICATIONEXITTYPE)(msg.wParam&0x0000000F);
          shutdowntype  = (XSYSTEM_SHUTDOWNTYPE)(msg.wParam>>=16);
          xapplication->SetExitStatus(exittype);
        }

      if (msg.message == WM_CLOSE)
      {
          PostQuitMessage(XAPPLICATIONEXITTYPE_BYUSER);
      }

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  #endif

  xapplication->UpdateApplication();

  if(xapplication->GetExitStatus() !=  XAPPLICATIONEXITTYPE_NONE)
    return false;

  #ifndef GRP_ACTIVE
  Sleep(5);
  #endif

  return true;
}



//-------------------------------------------------------------------
//  WINDOWSMAIN::End
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   11:10:57
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool WINDOWSMAIN::End()
{
  #ifdef INP_ACTIVE
  INPFACTORY::DelInstance();
  INPMANAGER::DelInstance();
  #endif

  #ifdef SND_ACTIVE
  if (SNDFACTORY::Get())
  SNDFACTORY::Get()->EndEvents();
  #endif

  if(xapplication) xapplication->EndApplication();

  #ifdef GRP_ACTIVE
  if(grpfactory)
    {
      delete grpfactory;
      grpfactory = NULL;
    }
  #endif

  #ifdef SND_ACTIVE
  SNDFACTORY::Destroy();
  #endif

  #ifdef DIO_ACTIVE
  if(diofactory)
    {
      delete diofactory;
      diofactory = NULL;
    }
  #endif

  if(xapplication)
    {
      delete xapplication;
      xapplication = NULL;
    }

  #ifdef XDEBUG_VIRTUALCLOCKTICK
  if(xtimerclock)
    {
      delete xtimerclock;
      xtimerclock = NULL;
    }
  #endif

  if(xlanguage)
    {
      delete xlanguage;
      xlanguage = NULL;
    }

  if(XSYSTEM::GetIsInstanced()) XSYSTEM::DelInstance();      
  
  XPUBLISHER::DelInstance();
  XPATHSMANAGER::DelInstance();
  XLOG::DelInstance();

  #ifdef DIOALERTS_ACTIVE
  DIOALERTS::DelInstance();
  #endif

  #ifdef XTHREADCOLLECTION_ACTIVE
  XTHREADSCOLLECTEDMANAGER::GetInstance().DeleteAll();
  XTHREADSCOLLECTEDMANAGER::DelInstance();
  #endif

  if(XSLEEP::GetIsInstanced()) XSLEEP::DelInstance();      

  if(xfactory)
    {
      delete xfactory;
      xfactory = NULL;
    }

  xpathexec.Empty();

  DeleteAllExecParams();

  XFILE_DISPLAYNOTCLOSEFILES
  XMEMORY_DISPLAYMEMORYLEAKS

  #if defined(DIO_ACTIVE) || defined(DIOUDP_ACTIVE) || defined(DIOTCPIP_ACTIVE) || defined(DIOBLUETOOTH_ACTIVE)
  WSACleanup();
  #endif

  #ifdef _CONSOLE
  if(!SetConsoleCtrlHandler((PHANDLER_ROUTINE)Exception_ConsoleHandler, FALSE)) return false;
  #else
  if(exittype == XAPPLICATIONEXITTYPE_SOSHUTDOWN) Exception_ShutDown(shutdowntype);
  #endif

  return true;
}



//-------------------------------------------------------------------
//  WINDOWSMAIN::GetApplication
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   11:10:57
//
//  @return        bool :
*/
//-------------------------------------------------------------------
XAPPLICATION* WINDOWSMAIN::GetXApplication()
{
  return xapplication;
}



/*-------------------------------------------------------------------
//  WINDOWSMAIN::GetParams
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      19/03/2014 11:58:35
//
//  @return       bool :
//
//  @param        nparams :
//  @param        params :
*/
/*-----------------------------------------------------------------*/
bool WINDOWSMAIN::GetParams(int nparams, char* params[])
{
  for(int c=1; c<nparams; c++)
    {
      XSTRING* param = new XSTRING();
      if(param)
        {
          (*param) = params[c];
          GetExecParams()->Add(param);
        }
    }

  return true;
}



/*-------------------------------------------------------------------
//  WINDOWSMAIN::GetParams
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      10/06/2016 16:49:25
//
//  @return       bool :
//
//  @param        commandline :
*/
/*-----------------------------------------------------------------*/
bool WINDOWSMAIN::GetParams(XSTRING& commandline)
{
  if(commandline.IsEmpty()) return false;

  int    start  = 0;
  XDWORD c      = 0;

  do{ if((commandline.Get()[c] == __C(' ')) || (commandline.GetSize() == c))
        {
          XSTRING* param = new XSTRING();
          if(param)
            {
              commandline.Copy(start, c, (*param));

              GetExecParams()->Add(param);
              start = c+1;
            }
        }

      c++;

    } while(c <= commandline.GetSize());

  return true;

}




//-------------------------------------------------------------------
//  WINDOWSMAIN::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/08/2003 12:09:01
//
//  @return
//  */
//-------------------------------------------------------------------
void WINDOWSMAIN::Clean()
{
  xsystem      = NULL;
  xapplication = NULL;
  exittype     = XAPPLICATIONEXITTYPE_NONE;
  shutdowntype = XSYSTEM_SHUTDOWNTYPE_UNKNOWN;

  isappclose   = false;
}






#ifdef XVISUALC

//-------------------------------------------------------------------
//  WINAPI WinMain
/**
//
//
//  @author       Abraham J. Velez
//  @version      10/01/2001 16:45:29
//
//  @return       int :
//  @param        hinstance :
//  @param        hprevinst :
//  @param        cmdline :
//  @param        cmdshow :
*/
//-------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hprevinst, LPSTR cmdline, int cmdshow)
{
  int     status = 0;
  int     nargs  = 0;
  LPWSTR* str    = CommandLineToArgvW(__L(""), &nargs);

  if(*str)
    {
      windowsmain.GetXPathExec()->Set((XCHAR*)(*str));
      windowsmain.GetXPathExec()->SetOnlyDriveAndPath();
    }

  windowsmain.GetParams(XSTRING((char*)cmdline));

  if(windowsmain.Ini(windowsmain.GetXPathExec()))
    {
      while(windowsmain.Update());

    } else status = 1;

  if(!windowsmain.End()) status  = 1;

  return status;
}



/*-------------------------------------------------------------------
//  main
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      22/08/2016 21:07:15
//
//  @return       int :
//
//  @param        argc :
//  @param        argv[] :
*/
/*-----------------------------------------------------------------*/
int main(int argc, char* argv[])
{
  int status = 0;

  (*windowsmain.GetXPathExec()) = argv[0];

  windowsmain.GetXPathExec()->SetOnlyDriveAndPath();

  windowsmain.GetParams(argc, argv);

  if(windowsmain.Ini(windowsmain.GetXPathExec()))
    {
      while(windowsmain.Update());

    } else status = 1;


  if(!windowsmain.End()) status = 1;

  windowsmain.SetIsAppClose(true);

  return status;
}



/*-------------------------------------------------------------------
//  DllMain
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      22/08/2016 21:07:23
//
//  @return       BOOL :
//
//  @param        hinstDLL :
//  @param        fdwReason :
//  @param        lpvReserved :
*/
/*-----------------------------------------------------------------*/
BOOL WINAPI DllMain(HINSTANCE hinstDLL,XDWORD fdwReason,LPVOID lpvReserved)
{
  switch(fdwReason)
    {
      case DLL_PROCESS_ATTACH : windowsmain.Ini();
                                break;

      case DLL_THREAD_ATTACH  : break;

      case DLL_THREAD_DETACH  : break;

      case DLL_PROCESS_DETACH : windowsmain.End();
                                break;
    }

  return true;
}

#endif




/*-------------------------------------------------------------------
//  WINDOWSMAINSTACKWALKER::OnSymInit
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2014 15:47:51
//
//  @return       void :
//  @param        searchpath :
//  @param        symoptions :
//  @param        username :
*/
/*-----------------------------------------------------------------*/
void WINDOWSMAINSTACKWALKER::OnSymInit(LPCSTR szSearchPath, DWORD symOptions, LPCSTR szUserName)
{
  Exception_Printf(true, NULL, __L("Stack trace: "));
}



/*-------------------------------------------------------------------
//  WINDOWSMAINSTACKWALKER::OnCallStackEntry
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/03/2014 15:47:59
//
//  @return       void :
//  @param        etype :
//  @param        entry :
*/
/*-----------------------------------------------------------------*/
void WINDOWSMAINSTACKWALKER::OnCallstackEntry(CallstackEntryType eType, CallstackEntry& entry)
{
  XSTRING modulename;
  XSTRING linefilename;
  XSTRING name;

  if((eType != lastEntry) && (entry.offset != 0))
    {
      if(entry.name[0] == 0)          strcpy_s(entry.name, "(function-name not available)");
      if(entry.undName[0] != 0)       strcpy_s(entry.name, entry.undName);
      if(entry.undFullName[0] != 0)   strcpy_s(entry.name, entry.undFullName);
      if(entry.lineFileName[0] == 0)
        {
          strcpy_s(entry.lineFileName, "(filename not available)");
          if (entry.moduleName[0] == 0) strcpy_s(entry.moduleName, "(module-name not available)");
        }

      modulename   = entry.moduleName;
      linefilename = entry.lineFileName;
      name         = entry.name;

      Exception_Printf(true, NULL, __L("%08X  %-48s (%6d) %-64s"), (LPVOID) entry.offset, name.Get(), entry.lineNumber, linefilename.Get());
    }
}





/*-------------------------------------------------------------------
//  ExceptionFilter
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      04/12/2013 10:05:42
//
//  @return       int :
//  @param        code :
//  @param        ep :
*/
/*-----------------------------------------------------------------*/
int Exception_Filter(XDWORD code, struct _EXCEPTION_POINTERS* ep)
{
  XSTRING exception;
  XSTRING description;
  XSTRING string;

  allexceptiontext.Empty();

  XAPPLICATION* app = windowsmain.GetXApplication();
  if(!app) return EXCEPTION_EXECUTE_HANDLER;

  switch(code)
    {
      case EXCEPTION_ACCESS_VIOLATION         : exception   = __L("ACCESS VIOLATION");
                                                description = __L("Read from or write to a virtual address for which it does not have access.");
                                                break;

      case EXCEPTION_ARRAY_BOUNDS_EXCEEDED    : exception   = __L("ARRAY BOUNDS EXCEEDED");
                                                description = __L("Access an array element that is out of bounds");
                                                break;

      case EXCEPTION_DATATYPE_MISALIGNMENT    : exception   = __L("DATATYPE MISALIGNMENT");
                                                description = __L("Read or write data that is misaligned on hardware that does not provide alignment.");
                                                break;

      case EXCEPTION_FLT_DENORMAL_OPERAND     : exception   = __L("FLT DENORMAL OPERAND]");
                                                description = __L("One of the operands in a floating point operation is denormal.");
                                                break;

      case EXCEPTION_FLT_DIVIDE_BY_ZERO       : exception   = __L("FLT DIVIDE BY ZERO");
                                                description = __L("Divide a floating point value by a floating point divisor of 0 (zero).");
                                                break;

      case EXCEPTION_FLT_INEXACT_RESULT       : exception   = __L("FLT INEXACT RESULT");
                                                description = __L("A floating point operation cannot be represented exactly as a decimal fraction.");
                                                break;

      case EXCEPTION_FLT_INVALID_OPERATION    : exception   = __L("FLT INVALID OPERATION");
                                                description = __L("A floating point exception (not defined).");
                                                break;

      case EXCEPTION_FLT_OVERFLOW             : exception   = __L("FLT OVERFLOW");
                                                description = __L("The exponent of a floating point operation is greater than the magnitude allowed by the corresponding type.");
                                                break;

      case EXCEPTION_FLT_STACK_CHECK          : exception   = __L("FLT STACK CHECK");
                                                description = __L("The stack has overflowed or underflowed, because of a floating point operation.");
                                                break;

      case EXCEPTION_FLT_UNDERFLOW            : exception   = __L("FLT UNDERFLOW");
                                                description = __L("The exponent of a floating point operation is less than the magnitude allowed by the corresponding type.");
                                                break;

      case EXCEPTION_GUARD_PAGE               : exception   = __L("GUARD PAGE");
                                                description = __L("Accessed memory allocated with the PAGE GUARD modifier.");
                                                break;

      case EXCEPTION_ILLEGAL_INSTRUCTION      : exception   = __L("ILLEGAL INSTRUCTION");
                                                description = __L("Tries to execute an invalid instruction.");
                                                break;

      case EXCEPTION_IN_PAGE_ERROR            : exception   = __L("IN PAGE ERROR");
                                                description = __L("Tries to access a page that is not present, and the system is unable to load the page.");
                                                break;

      case EXCEPTION_INT_DIVIDE_BY_ZERO       : exception   = __L("INT DIVIDE BY ZERO");
                                                description = __L("Divide an integer value by an integer divisor of 0 (zero).");
                                                break;

      case EXCEPTION_INT_OVERFLOW             : exception   = __L("INT OVERFLOW");
                                                description = __L("The result of an integer operation creates a value that is too large to be held by the destination register.");
                                                break;

      case EXCEPTION_INVALID_DISPOSITION      : exception   = __L("INVALID DISPOSITION");
                                                description = __L("An exception handler returns an invalid disposition to the exception dispatcher.");
                                                break;

      case EXCEPTION_INVALID_HANDLE           : exception   = __L("INVALID HANDLE") ;
                                                description = __L("The thread used a handle to a kernel object that was invalid (probably because it had been closed).");
                                                break;

      case EXCEPTION_NONCONTINUABLE_EXCEPTION : exception   = __L("NONCONTINUABLE EXCEPTION");
                                                description = __L("The thread attempts to continue execution after a non-continuable exception occurs.");
                                                break;

      case EXCEPTION_PRIV_INSTRUCTION         : exception   = __L("PRIV INSTRUCTION");
                                                description = __L("Execute an instruction with an operation that is not allowed in the current computer mode.");
                                                break;

      case EXCEPTION_STACK_OVERFLOW           : exception   = __L("STACK OVERFLOW");
                                                description = __L("Stack overflow");
                                                break;

      case EXCEPTION_BREAKPOINT               : //Exception_Printf(__L("[EXCEPTION_BREAKPOINT]")              , __L("A breakpoint is encountered"));
      case EXCEPTION_SINGLE_STEP              : //Exception_Printf(__L("[EXCEPTION_SINGLE_STEP]")             , __L("A trace trap or other single instruction mechanism signals that one instruction is executed."));
      case STATUS_UNWIND_CONSOLIDATE          : return EXCEPTION_CONTINUE_SEARCH;
                                                break;

                                      default : return EXCEPTION_EXECUTE_HANDLER;
    }

  Exception_Printf(true, __L("EXCEPTION"), __L("%s: %s"), exception.Get(), description.Get());

  if(app->GetTimerGlobal())
    {
      app->GetTimerGlobal()->GetMeasureString(string, true);
      Exception_Printf(false, NULL, __L("Time working: %s."), string.Get());
    }

  switch(code)
    {
      case EXCEPTION_ACCESS_VIOLATION         :
      case EXCEPTION_ARRAY_BOUNDS_EXCEEDED    :
      case EXCEPTION_DATATYPE_MISALIGNMENT    :
      case EXCEPTION_FLT_DENORMAL_OPERAND     :
      case EXCEPTION_FLT_DIVIDE_BY_ZERO       :
      case EXCEPTION_FLT_INEXACT_RESULT       :
      case EXCEPTION_FLT_INVALID_OPERATION    :
      case EXCEPTION_FLT_OVERFLOW             :
      case EXCEPTION_FLT_STACK_CHECK          :
      case EXCEPTION_FLT_UNDERFLOW            :
      case EXCEPTION_GUARD_PAGE               :
      case EXCEPTION_ILLEGAL_INSTRUCTION      :
      case EXCEPTION_IN_PAGE_ERROR            :
      case EXCEPTION_INT_DIVIDE_BY_ZERO       :
      case EXCEPTION_INT_OVERFLOW             :
      case EXCEPTION_INVALID_DISPOSITION      :
      case EXCEPTION_INVALID_HANDLE           :
      case EXCEPTION_NONCONTINUABLE_EXCEPTION :
      case EXCEPTION_PRIV_INSTRUCTION         :
      case EXCEPTION_STACK_OVERFLOW           : { WINDOWSMAINSTACKWALKER stackwalker;
                                                  stackwalker.ShowCallstack(GetCurrentThread(), ep->ContextRecord);


                                                  string.Format(__L("EXCEPTION %s"), exception.Get());

                                                  #ifdef DIOALERTS_ACTIVE
                                                  DIOALERT* alert = DIOALERTS::GetInstance().CreateAlert(DIOALERTTYPE_ERRORAPP, DIOALERTLEVEL_DANGER, string.Get(), allexceptiontext.Get());
                                                  if(alert)
                                                    {
                                                      DIOALERTS::GetInstance().Send(DIOALERTSSENDER_ALL, alert);
                                                      delete alert;
                                                    }
                                                  #endif

                                                  if(app) exit(XAPPLICATIONEXITTYPE_APPERROR);
                                                }
                                                break;
    }

  return EXCEPTION_EXECUTE_HANDLER;
}




/*-------------------------------------------------------------------
//  Exception_Printf
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/04/2015 16:24:16
//
//  @return       bool :
//
//  @param        iserror :
//  @param        title :
//  @param        mask :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool Exception_Printf(bool iserror, XCHAR* title, XCHAR* mask, ...)
{
  XSTRING outstring;

  va_list arg;

  va_start(arg, mask);

  outstring.FormatArg(mask, &arg);

  va_end(arg);

  bool do_log = false;

  if(windowsmain.GetXApplication())
    {
      XLOG::GetInstance().SetFilters(__L(""));

      if(XLOG::GetInstance().AddEntry(iserror?XLOGLEVEL_ERROR:XLOGLEVEL_WARNING, title, false, outstring.Get()))
        {
          do_log = true;
        }
    }

  allexceptiontext += outstring;
  allexceptiontext += __L("\r\n");

  #ifdef XDEBUG_TRACE
  if(!do_log)
    {
      int level = iserror?4:0;
     XDEBUGTRACE_PRINTCOLOR(level, outstring.Get());
    }
  #else
  //Exception_PrintNet(outstring.Get() , 4, windowsmain.GetXApplication()->GetXSystem()->HardwareUseLittleEndian());
  #endif

  return true;
}




/*-------------------------------------------------------------------
//  Exception_PrintfStackTrace
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/12/2013 16:08:39
//
//  @return       bool :
//  @param        context :
*/
/*-----------------------------------------------------------------*/
static bool Exception_PrintfStackTrace(const CONTEXT* context)
{
  static const int MAX_TRACES     = 60;
  static const int MAX_LENGTH     = 256;
  static const int BUFFER_LENGTH  = (sizeof(SYMBOL_INFO) + MAX_LENGTH * sizeof(wchar_t) + sizeof(ULONG64) - 1) / sizeof(ULONG64);

  SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME | SYMOPT_LOAD_LINES | SYMOPT_LOAD_ANYTHING | SYMOPT_AUTO_PUBLICS | SYMOPT_OMAP_FIND_NEAREST | SYMOPT_DEBUG);
  if (!SymInitialize(GetCurrentProcess(), "http://msdl.microsoft.com/download/symbols", TRUE )) return false;

  void* trace[MAX_TRACES];
  int   count = CaptureStackBackTrace(0, MAX_TRACES , trace, NULL);

  Exception_Printf(true, NULL, __L("Stack trace: (%d) calls :"), count);

  for(int i=0; i<count; ++i)
    {
      ULONG64         buffer[BUFFER_LENGTH];
      DWORD_PTR       frame               = reinterpret_cast<DWORD_PTR>(trace[i]);
      DWORD64         sym_displacement    = 0;
      PSYMBOL_INFO    symbol              = reinterpret_cast<PSYMBOL_INFO>(&buffer[0]);

      symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
      symbol->MaxNameLen   = MAX_LENGTH;

      BOOL            has_symbol          = SymFromAddr(GetCurrentProcess(), frame, &sym_displacement, symbol);
      DWORD           line_displacement   = 0;
      IMAGEHLP_LINE64 line                = {};

      line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
      BOOL has_line                       = SymGetLineFromAddr64(GetCurrentProcess(), frame, &line_displacement, &line);

      XSTRING symbolname;
      XSTRING filename;
      XSTRING string;
      XSTRING mask;

      symbolname = symbol->Name;

      string.Format(__L(" [%02d] - 0x%08X : %-48s "), (count-i-1), (XDWORD)symbol->Address, symbolname.Get());
      if(has_line)
        {
          filename = line.FileName;
          mask.Format(__L("line (%02d) %s "), line.LineNumber, filename.Get());
          string += mask;
        }

      Exception_Printf(true, NULL, string.Get());
    }

  return true;
}




#ifdef _CONSOLE
/*-------------------------------------------------------------------
//  Exception_ConsoleHandler
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/12/2013 16:24:17
//
//  @return       BOOL :
//  @param        fdwctrltype :
*/
/*-----------------------------------------------------------------*/
BOOL Exception_ConsoleHandler(DWORD fdwctrltype)
{
  XAPPLICATION* app = windowsmain.GetXApplication();
  if(!app) return FALSE;

  switch(fdwctrltype)
    {
      case CTRL_BREAK_EVENT     : Exception_Printf(false, __L("EVENT"), __L("Ctrl-Break event."));
                                  app->SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER);
                                  return FALSE;

      case CTRL_C_EVENT         : Exception_Printf(false, __L("EVENT"), __L("Ctrl-C event."));
                                  app->SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER);
                                  return FALSE;

      case CTRL_CLOSE_EVENT     : Exception_Printf(false, __L("EVENT"), __L("Close event."));
                                  app->SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER);
                                  FreeConsole();         // Detachxconsole
                                  //ExitThread(0);       // Prevent closing.
                                  break;

      case CTRL_LOGOFF_EVENT    : Exception_Printf(false, __L("EVENT"), __L("Ctrl-Logoff event."));
                                  app->SetExitStatus(XAPPLICATIONEXITTYPE_SOSHUTDOWN);
                                  FreeConsole();         // Detachxconsole
                                  ExitThread(0);         // Prevent closing.
                                  break;

      case CTRL_SHUTDOWN_EVENT  : Exception_Printf(false, __L("EVENT"), __L("Ctrl-Shutdown event."));
                                  app->SetExitStatus(XAPPLICATIONEXITTYPE_SOSHUTDOWN);
                                  FreeConsole();         // Detachxconsole
                                  ExitThread(0);         // Prevent closing.
                                  break;

                      default   : return FALSE;
    }


  while(!windowsmain.IsAppClose())
    {
      Sleep(500);
    }

  return FALSE;
}


#endif




/*-------------------------------------------------------------------
//  Exception_ShutDown
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/12/2013 14:04:35
//
//  @return       bool :
//  @param        type :
*/
/*-----------------------------------------------------------------*/
bool Exception_ShutDown(XSYSTEM_SHUTDOWNTYPE type)
{
  if(type == XSYSTEM_SHUTDOWNTYPE_UNKNOWN) return false;

  HANDLE            hToken;
  TOKEN_PRIVILEGES  tkp;

  // Get a token for this process.
  if(!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))  return false;

  // Get the LUID for the shutdown privilege.
  LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

  tkp.PrivilegeCount            = 1;  // one privilege to set
  tkp.Privileges[0].Attributes  = SE_PRIVILEGE_ENABLED;

  // Get the shutdown privilege for this process.
  AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
  if(GetLastError() != ERROR_SUCCESS) return false;

  switch(type)
    {
      case XSYSTEM_SHUTDOWNTYPE_REBOOT   :  if(!ExitWindowsEx(EWX_REBOOT   | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) return false;
                                            break;

      case XSYSTEM_SHUTDOWNTYPE_LOGOFF   :  if(!ExitWindowsEx(EWX_LOGOFF   | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) return false;
                                            break;

      case XSYSTEM_SHUTDOWNTYPE_POWEROFF :  if(!ExitWindowsEx(EWX_POWEROFF | EWX_FORCE, SHTDN_REASON_MAJOR_OPERATINGSYSTEM | SHTDN_REASON_MINOR_UPGRADE | SHTDN_REASON_FLAG_PLANNED)) return false;
                                            break;
    }

  return true;
}




