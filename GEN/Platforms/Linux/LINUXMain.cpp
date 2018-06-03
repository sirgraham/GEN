//------------------------------------------------------------------------------------------
//  LINUXMAIN.CPP
//
//  LINUX main
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/10/2005 16:00:46
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <utmp.h>
#include <time.h>
#include <malloc.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <fcntl.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <execinfo.h>
#include <errno.h>
#include <cxxabi.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fenv.h>

#include "XSleep.h"
#include "XPath.h"
#include "XString.h"
#include "XLog.h"
#include "XFile.h"
#include "XTimer.h"
#include "XThreadCollected.h"
#include "XDateTime.h"
#include "XLanguage.h"
#include "XApplication.h"
#include "XPublisher.h"
#include "XDebugTrace.h"

#include "XLINUXFactory.h"
#include "XLINUXSleep.h"
#include "XLINUXSystem.h"

#ifdef GRP_ACTIVE
#include "GRPLINUXFactory.h"
#include "GRPScreen.h"
#endif

#ifdef INP_ACTIVE
  #if defined(HW_PC)// || defined(HW_ODROIDC1)
  #include "INPLINUXDeviceKeyboardX11.h"
  #include "INPLINUXDeviceMouseX11.h"
  #endif
  #if defined(HW_RASPBERRYPI) || defined(HW_ODROIDC1)
  #include "INPLINUXDeviceKeyboardDirect.h"
  #include "INPLINUXDeviceMouseDirect.h"
  #endif
#include "INPManager.h"
#include "INPFactory.h"
#include "INPLINUXFactory.h"
#endif

#ifdef SND_ACTIVE
#include "SNDLinuxFactory.h"
#endif

#ifdef DIO_ACTIVE
#include "DIOLINUXFactory.h"
#ifdef DIOALERTS_ACTIVE
#include "DIOAlerts.h"
#endif
#endif


#include "LINUXMain.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------

#ifdef XDEBUG_TRACE
XLINUXDEBUGTRACE      linuxdebugtrace;
#endif
LINUXMAIN             linuxmain;
struct sigaction      signalaction;
XSTRING               allexceptiontext;

//---- INTERNAL PROTOTYPES -----------------------------------------------------------------

void                  Signal_Ini                (void);
static void           Signal_Handler            (int sig);
bool                  Signal_Printf             (bool iserror, XCHAR* title, XCHAR* mask, ...);
static inline void    Signal_PrintfStackTrace   (FILE *out = stderr, unsigned int max_frames = 63);
bool                  Signal_RunLevel           (XCHAR& previous, XCHAR& actual);


//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  LINUXMAIN::LINUXMAIN
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   12:27:35
//
//  @return
*/
//-------------------------------------------------------------------
LINUXMAIN::LINUXMAIN()
{
  Clean();
}


//-------------------------------------------------------------------
//  LINUXMAIN::~LINUXMAIN
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   12:27:35
//
//  @return
*/
//-------------------------------------------------------------------
LINUXMAIN::~LINUXMAIN()
{
  Clean();
}



/*-------------------------------------------------------------------
//  LINUXMAIN::Ini
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/03/2011 14:04:56
//
//  @return       bool :
//  @param        xpath :
//  @param        mainscreenhandle :
*/
/*-----------------------------------------------------------------*/
bool LINUXMAIN::Ini(XSTRING* xpath)
{
  #ifdef FPE_ACTIVE
  feenableexcept(FE_DIVBYZERO | FE_OVERFLOW /*| FE_UNDERFLOW | FE_INEXACT*/);
  #endif

  typesystem = MAIN_SYSTEM_LINUX;

  xfactory = new  XLINUXFACTORY();
  if(!xfactory) return false;

  XSLEEP::SetInstance(new XLINUXSLEEP());
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

  XSYSTEM::SetInstance(new XLINUXSYSTEM());
  if(!XSYSTEM::GetIsInstanced()) return false;

  xlanguage = new XLANGUAGE();
  if(!xlanguage) return false;

  #ifdef GRP_ACTIVE
  grpfactory  = new GRPLINUXFACTORY();
  if(!grpfactory) return false;
  grpfactory->dpi = 96.0f;
  #endif

  #ifdef DIO_ACTIVE
  diofactory = new DIOLINUXFACTORY();
  if(!diofactory) return false;
  #endif

  #ifdef SND_ACTIVE
  SNDFACTORY::Instance = new SNDLINUXFACTORY();
  if (!SNDFACTORY::Instance) return false;
  SNDFACTORY::Get()->IniEvents();
  #endif

  #ifdef INP_ACTIVE
  INPFACTORY::instance = new INPLINUXFACTORY();
  if (!INPFACTORY::instance) return false;
  #endif

  xapplication  = (XAPPLICATION*)XAPPLICATION::Create();
  if(!xapplication) return false;
  xapplication->SetMain(&linuxmain);
  if(!xapplication->IniApplication(GetExecParams())) return false;

  return true;
}



//-------------------------------------------------------------------
//  LINUXMAIN::Update
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   11:17:1
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool LINUXMAIN::Update()
{
  if(!xapplication) return false;

  xapplication->UpdateApplication();

  if(xapplication->GetExitStatus() !=  XAPPLICATIONEXITTYPE_NONE)
    {
      xapplication->LastUpdate();
      return false;
    }

  #ifndef GRP_ACTIVE
  usleep(5000);
  #endif

  return true;
}



//-------------------------------------------------------------------
//  LINUXMAIN::End
/**
//
//  @author        Abraham J. Velez
//  @version       12/3/2003   11:10:57
//
//  @return        bool :
*/
//-------------------------------------------------------------------
bool LINUXMAIN::End()
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
  if (SNDFACTORY::Get())
  SNDFACTORY::Destroy();
  #endif

  #ifdef DIO_ACTIVE
  if(diofactory)
    {
      delete diofactory;
      diofactory = NULL;
    }
  #endif

  if(xapplication)    delete xapplication;

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

  DeleteAllExecParams();

  XFILE_DISPLAYNOTCLOSEFILES
  XMEMORY_DISPLAYMEMORYLEAKS

  return true;
}


/*-------------------------------------------------------------------
//  LINUXMAIN::GetApplication
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/09/2012 16:33:59
//
//  @return       XAPPLICATION* :
//  */
/*-----------------------------------------------------------------*/
XAPPLICATION* LINUXMAIN::GetXApplication()
{
  return xapplication;
}




/*-------------------------------------------------------------------
//  LINUXMAIN::ActivateLikeService
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      09/07/2013 8:52:04
//
//  @return       bool :
*/
/*-----------------------------------------------------------------*/
/*
bool LINUXMAIN::ActivateLikeService()
{
  pid_t pid;
  pid_t sid;

  pid = fork();
  if(pid<0) return false;

  if(pid>0) exit(EXIT_SUCCESS);

  umask(0);

  sid = setsid();
  if(sid<0)  return false;

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  return true;
}
*/



//-------------------------------------------------------------------
//  LINUXMAIN::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/08/2003 12:09:01
//
//  @return
//  */
//-------------------------------------------------------------------
void LINUXMAIN::Clean()
{
  xsystem         = NULL;
  xapplication    = NULL;
}




#ifdef INP_ACTIVE

/*-------------------------------------------------------------------
//  LINUXMAIN::CreateInputDevices
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2012 21:27:23
//
//  @return       bool :
//  @param        inpmanager :
//  @param        grpscreen :
*/
/*-----------------------------------------------------------------*/
/*
bool LINUXMAIN::CreateInputDevices(INPMANAGER* inpmanager, GRPSCREEN* grpscreen)
{
  if(!inpmanager) return false;

  #ifdef GRP_ACTIVE
  if(grpscreen)
    {
      switch(grpscreen->GetType())
        {
          case GRPSCREENTYPE_LINUX_X11_GL           :
          case GRPSCREENTYPE_LINUX_X11_GLES         :
                                                      #if defined(HW_PC)// || defined(HW_ODROIDC1)
                                                      keyboard =  new INPLINUXDEVICEKEYBOARDX11( grpscreen);
                                                      mouse    =  new INPLINUXDEVICEMOUSEX11( grpscreen);
                                                      #endif
                                                      break;

          case GRPSCREENTYPE_LINUX_FRAMEBUFFER      :
          case GRPSCREENTYPE_LINUX_FRAMEBUFFER_GLES :
                                                      #if defined(HW_ODROIDC1)
                                                      keyboard =  new INPLINUXDEVICEKEYBOARDDIRECT();
                                                      mouse    =  new INPLINUXDEVICEMOUSEDIRECT( grpscreen);

                                                      // --- Console ECHO OFF + Clear

                                                      { struct termios termiossettings;

                                                        wprintf(L"\033c");  // Clear Console

                                                        tcgetattr(STDIN_FILENO, &termiosinitialsettings );

                                                        termiossettings =  termiosinitialsettings;
                                                        termiossettings.c_lflag &= ~(ECHO | ICANON);

                                                        tcflush(STDIN_FILENO, TCIFLUSH);

                                                        tcsetattr( STDIN_FILENO, TCSANOW, &termiossettings );
                                                      }
                                                      #endif
          case GRPSCREENTYPE_LINUX_DISPMAN        :
                                                      #if !defined(HW_PC) && !defined(HW_ODROIDC1)
                                                      keyboard =  new INPLINUXDEVICEKEYBOARDDIRECT();
                                                      mouse    =  new INPLINUXDEVICEMOUSEDIRECT( grpscreen);

                                                      // --- Console ECHO OFF + Clear

                                                      { struct termios termiossettings;

                                                        wprintf(L"\033c");  // Clear Console

                                                        tcgetattr(STDIN_FILENO, &termiosinitialsettings );

                                                        termiossettings =  termiosinitialsettings;
                                                        termiossettings.c_lflag &= ~(ECHO | ICANON);

                                                        tcflush(STDIN_FILENO, TCIFLUSH);

                                                        tcsetattr( STDIN_FILENO, TCSANOW, &termiossettings );
                                                      }
                                                      #endif
                                                      break;
        }

      if(!mouse) return false;
      inpmanager->AddDevice(mouse);

      if(!keyboard) return false;
      inpmanager->AddDevice(keyboard);
    }
  #endif

  return true;
}
*/



/*-------------------------------------------------------------------
//  LINUXMAIN::DeleteInputDevices
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/07/2012 21:58:49
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
/*
bool LINUXMAIN::DeleteInputDevices(GRPSCREEN* grpscreen)
{
  #ifdef GRP_ACTIVE
  if(grpscreen)
    {
      if(keyboard) delete keyboard;
      if(mouse)    delete mouse;

      switch(grpscreen->GetType())
        {
          case GRPSCREENTYPE_LINUX_X11_GL         : break;

          case GRPSCREENTYPE_LINUX_FRAMEBUFFER  :
          case GRPSCREENTYPE_LINUX_DISPMAN      : tcflush(STDIN_FILENO, TCIFLUSH);
                                                  tcsetattr(STDIN_FILENO, TCSANOW, &termiosinitialsettings);
                                                  break;
        }
    }
  #endif

  return true;
}
*/

#endif





//-------------------------------------------------------------------
//  main
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/10/2005 15:58:09
//
//  @return       int :
//  */
//-------------------------------------------------------------------
#ifndef XLIB
int main(int argc, char* argv[])
{
  Signal_Ini();

  XPATH* xpath;
  int    status = 0;

  xpath = new XPATH();
  if(xpath)
    {
      char sztmp[64];
      int  pathsize = 512;
      char path[pathsize];

      memset(path, 0, pathsize);

      sprintf(sztmp, "/proc/%d/exe", getpid());
      int rb = readlink(sztmp, path, pathsize);
      if(rb >= 0)
        {
          path[rb] = '\0';
          xpath->Set(path);
        }
       else xpath->Set(argv[0]);

      xpath->SetOnlyPath();

      for(int c=1;c<argc;c++)
        {
          XSTRING* param = new XSTRING();
          if(param)
            {
              (*param) = argv[c];
              linuxmain.GetExecParams()->Add(param);
            }
        }

      if(linuxmain.Ini(xpath))
        {
          while(linuxmain.Update());

        } else status = 1;

      delete xpath;

      if(!linuxmain.End()) status = 1;

    } else status = 1;

  return status;
}


#else

//-------------------------------------------------------------------

void LIB_Ini(void)
{
  XPATH xpath;

  xpath.Empty();

  linuxmain.Ini(&xpath);
}

//-------------------------------------------------------------------

void LIB_End(void)
{
  linuxmain.End();
}

#endif




/*-------------------------------------------------------------------
//  Signal_Ini
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/01/2013 22:51:25
//
//  @return       void :
//  @param        void :
*/
/*-----------------------------------------------------------------*/
void Signal_Ini(void)
{
  signalaction.sa_handler = Signal_Handler;
  sigemptyset(&signalaction.sa_mask);
  signalaction.sa_flags = 0;
  signalaction.sa_mask = signalaction.sa_mask;

  sigaddset(&signalaction.sa_mask, SIGHUP);       sigaction(SIGHUP    , &signalaction , (struct sigaction*)NULL);
  sigaddset(&signalaction.sa_mask, SIGINT);       sigaction(SIGINT    , &signalaction , (struct sigaction*)NULL);
  sigaddset(&signalaction.sa_mask, SIGQUIT);      sigaction(SIGQUIT   , &signalaction , (struct sigaction*)NULL);
  sigaddset(&signalaction.sa_mask, SIGILL);       sigaction(SIGILL    , &signalaction , (struct sigaction*)NULL);
  sigaddset(&signalaction.sa_mask, SIGBUS);       sigaction(SIGBUS    , &signalaction , (struct sigaction*)NULL);
  sigaddset(&signalaction.sa_mask, SIGFPE);       sigaction(SIGFPE    , &signalaction , (struct sigaction*)NULL);
//sigaddset(&signalaction.sa_mask, SIGKILL);      sigaction(SIGKILL   , &signalaction , (struct sigaction*)NULL);   // Not Available
  sigaddset(&signalaction.sa_mask, SIGSEGV);      sigaction(SIGSEGV   , &signalaction , (struct sigaction*)NULL);
  sigaddset(&signalaction.sa_mask, SIGTERM);      sigaction(SIGTERM   , &signalaction , (struct sigaction*)NULL);
  sigaddset(&signalaction.sa_mask, SIGSTKFLT);    sigaction(SIGSTKFLT , &signalaction , (struct sigaction*)NULL);
//sigaddset(&signalaction.sa_mask, SIGSTOP);      sigaction(SIGSTOP   , &signalaction , (struct sigaction*)NULL);   // Not Available
  sigaddset(&signalaction.sa_mask, SIGTSTP);      sigaction(SIGTSTP   , &signalaction , (struct sigaction*)NULL);
  sigaddset(&signalaction.sa_mask, SIGABRT);      sigaction(SIGABRT   , &signalaction , (struct sigaction*)NULL);

  sigaddset(&signalaction.sa_mask, SIGUSR1);      sigaction(SIGUSR1   , &signalaction , (struct sigaction*)NULL);
  sigaddset(&signalaction.sa_mask, SIGUSR2);      sigaction(SIGUSR2   , &signalaction , (struct sigaction*)NULL);
}



/*-------------------------------------------------------------------
//  void Signal_Handler
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/01/2013 22:51:36
//
//  @return       static :
//  @param        sig :
*/
/*-----------------------------------------------------------------*/
static void Signal_Handler(int sig)
{
  XSTRING signalstr;
  XSTRING description;
  XSTRING string;
  bool    iserror = false;

  allexceptiontext.Empty();

  XAPPLICATION* app = linuxmain.GetXApplication();
  if(!app) return;

  if(app->GetExitStatus()!=XAPPLICATIONEXITTYPE_NONE) return;

  switch(sig)
    {
      case SIGSEGV    : iserror     = true;
                        signalstr   = __L("SEGV");
                        description = __L("Segmentation fault.");
                        break;

      case SIGBUS     : iserror     = true;
                        signalstr   = __L("BUS");
                        description = __L("Bus fault.");
                        break;

      case SIGFPE     : iserror     = true;
                        signalstr   = __L("FPE");
                        description = __L("Erroneous arithmetic operation (such as divide by zero).");
                        break;

      case SIGABRT    : iserror     = true;
                        signalstr   = __L("ABRT");
                        description = __L("Error: Abnormal termination condition.");
                        break;

      case SIGSTKFLT  : iserror     = true;
                        signalstr   = __L("STKFLT");
                        description = __L("Error: Stack fault.");
                        break;

      case SIGILL     : iserror     = true;
                        signalstr   = __L("ILL");
                        description = __L("Error: Illegal instruction.");
                        break;


      case SIGHUP     : signalstr   = __L("HUP");
                        description = __L("Program hanged up.");
                        break;

      case SIGTERM    : signalstr   = __L("TERM");
                        description = __L("Termination request, sent to the program.");
                        break;

      case SIGINT     : signalstr   = __L("INT");
                        description = __L("External interrupt, usually initiated by the user.");
                        break;

      case SIGQUIT    : signalstr   = __L("QUIT");
                        description = __L("Terminal quit.");
                        break;

      case SIGTSTP    : signalstr   = __L("TSTP");
                        description = __L("Terminal stop signalstr.");
                        break;

      case SIGKILL    : // Not Available -> Kill (can't be caught or ignored).
                        break;

      case SIGSTOP    : // Not Available -> Stop executing(can't be caught or ignored).
                        break;
      case SIGUSR1    :
      case SIGUSR2    :   signalstr = __L("USR");
                          description = __L("User signal received.");
                          break;
    }

  Signal_Printf(iserror, __L("SIGNAL"), __L("[SIG%s] Error: %s"), signalstr.Get(), description.Get());

  if(app->GetTimerGlobal())
    {
      XSTRING string2;

      app->GetTimerGlobal()->GetMeasureString(string2, true);
      Signal_Printf(false, NULL,__L("Time working: %s."), string2.Get());
    }

  switch(sig)
    {
      case SIGSEGV    :
      case SIGBUS     :
      case SIGFPE     :
      case SIGABRT    :
      case SIGSTKFLT  :
      case SIGILL     : { Signal_PrintfStackTrace();

                          string.Format(__L("SIGNAL %s: %s"), signalstr.Get(), description.Get());

                          #ifdef DIOALERTS_ACTIVE
                          DIOALERT* alert = DIOALERTS::GetInstance().CreateAlert(DIOALERTTYPE_ERRORAPP, DIOALERTLEVEL_DANGER, string.Get(), allexceptiontext.Get());
                          if(alert)
                            {
                              DIOALERTS::GetInstance().Send(DIOALERTSSENDER_ALL, alert);
                              delete alert;
                            }
                          #endif

                          fprintf(stderr," \n");

                          app->SetExitStatus(XAPPLICATIONEXITTYPE_APPERROR);
                          app->End();

                          signal(sig, SIG_DFL);
                          kill(getpid(), sig);

                          //exit(sig);
                        }
                        break;


      case SIGINT     : app->SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER);
                        break;

      case SIGQUIT    : app->SetExitStatus(XAPPLICATIONEXITTYPE_SOSHUTDOWN);
                        break;

      case SIGHUP     :
      case SIGTERM    : { XCHAR previous;
                          XCHAR actual;

                          if(app->GetExitStatus()!=XAPPLICATIONEXITTYPE_NONE) return;

                          //XSLEEP::GetInstance().Seconds(2);

                          if(Signal_RunLevel(previous, actual))
                            {
                              //XDEBUGTRACE_PRINTCOLOR(2, __L("Run level: %c"), actual);

                              if((actual == __C('0')))
                                {
                                  app->SetExitStatus(XAPPLICATIONEXITTYPE_SOSHUTDOWN);
                                  //app->End();
                                  break;
                                }
                            }

                          app->SetExitStatus(XAPPLICATIONEXITTYPE_BYUSER);
                        }
                        break;
    }
}





/*-------------------------------------------------------------------
//  Signal_Printf
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/12/2013 12:09:42
//
//  @return       bool :
//  @param        iserror :
//  @param        title :
//  @param        mask :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool Signal_Printf(bool iserror, XCHAR* title, XCHAR* mask, ...)
{
  XSTRING outstring;

  va_list arg;

  va_start(arg, mask);

  outstring.FormatArg(mask, &arg);

  va_end(arg);

  bool do_log = false;

  if(XLOG::GetInstance().IsActive())
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
  #endif

  return true;
}



/*-------------------------------------------------------------------
//  Signal_PrintfStackTrace
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/01/2017 17:16:56
//
//  @return       static :
//
//  @param        *out :
//  @param        max_frames :
*/
/*-----------------------------------------------------------------*/
static inline void Signal_PrintfStackTrace(FILE *out, unsigned int max_frames)
{
  void*  addrlist[max_frames+1];        // storage array for stack trace address data
  XDWORD addrlen = 0;

  memset(addrlist,0, sizeof(addrlist));

  addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));  // retrieve current stack addresses

  if(addrlen == 0)
    {
      Signal_Printf(true, NULL, __L("Stack trace: Not available."));
      return;

    } else Signal_Printf(true, NULL, __L("Stack trace: (%d) calls"),addrlen);

  // resolve addresses into strings containing "filename(function+address)",
  // Actually it will be ## program address function + offset
  // this array must be free()-ed
  char** symbollist = backtrace_symbols( addrlist, addrlen );

  size_t funcnamesize = 1204;
  char   funcname[1024];

  // iterate over the returned symbol lines. skip the first, it is the
  // address of this function.
  for(XDWORD i = 0; i < addrlen; i++)
    {
      char* begin_name   = NULL;
      char* begin_offset = NULL;
      char* end_offset   = NULL;

      XSTRING   beginname;
      XSTRING   beginoffset;
      XSTRING   endoffset;
      XSTRING   symbol;

      // find parentheses and +address offset surrounding the mangled name

      for(char *p = symbollist[i]; *p; ++p)
        {
         if(*p == '(')
           {
             begin_name = p;
           }
          else
           {
             if(*p == '+')
               {
                 begin_offset = p;
               }
              else
               {
                 if ( *p == ')' && ( begin_offset || begin_name ))  end_offset = p;
               }
           }
        }

      if(begin_name && end_offset && (begin_name < end_offset))
        {
          *begin_name++   = '\0';
          *end_offset++   = '\0';

          if(begin_offset) *begin_offset++ = '\0';


          int   status = -1;
          char* ret    = NULL;
          // mangled name is now in [begin_name, begin_offset) and caller
          // offset in [begin_offset, end_offset). now apply
          // __cxa_demangle():

          #ifndef HW_PC_OLD
          abi::__cxa_demangle(begin_name, funcname, &funcnamesize, &status);
          #endif

          char* fname = begin_name;

          if(status == 0)  fname = ret;

          beginname   = fname;
          endoffset   = end_offset;
          symbol      = symbollist[i];

          if(begin_offset)
            {
              beginoffset = begin_offset;
              Signal_Printf(true, NULL, __L("%-40s (%-40s  + %-6s) %s"), symbol.Get(), beginname.Get(), beginoffset.Get(), endoffset.Get());
            }
           else
            {
              Signal_Printf(true, NULL, __L("%-40s (%-40s    %-6s) %s"), symbol.Get(), beginname.Get(), __L(""), endoffset.Get());
            }
        }
       else
        {
          symbol      = symbollist[i];
          // couldn't parse the line? print the whole line.
          Signal_Printf(true, NULL, __L("%-40s"), symbol.Get());
        }
    }

  free(symbollist);
}



/*-------------------------------------------------------------------
//  Signal_RunLevel
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/12/2013 10:34:43
//
//  @return       bool :
//  @param        previous :
//  @param        actual :
*/
/*-----------------------------------------------------------------*/
bool Signal_RunLevel(XCHAR& previous, XCHAR& actual)
{
  struct utmp* ut;
  bool         status = false;

  previous = 0;
  actual   = 0;

  setutent();

  while((ut = getutent()) != NULL)
    {
      if(ut->ut_type == RUN_LVL)
        {
          previous = (XCHAR)(ut->ut_pid / 256);
          if(!previous) previous = __C('N');

          actual   = (XCHAR)(ut->ut_pid % 256);

          status = true;
        }
    }

  endutent();

  return status;
}




