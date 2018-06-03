/**-------------------------------------------------------------------------------------------------------------------
*
* @file        XLINUXSystem.cpp
*
* @class       XLINUXSYSTEM
* @brief       Linux system class
* @ingroup     PLATFORM_LINUX
*
* @author      Abraham J. Velez 
* @date        03/06/2018 13:34:05
*
* @copyright   Copyright(c) 2005 - 2018 GEN Group.
*
* @cond
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files(the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and/ or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of
* the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
* THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* @endcond
*
*---------------------------------------------------------------------------------------------------------------------*/

/*---- INCLUDES ------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/wait.h>
#include <linux/reboot.h>

#include "XLINUXFactory.h"
#include "XBuffer.h"
#include "XFileTXT.h"
#include "XDebugTrace.h"
#include "XString.h"
#include "XPath.h"

#include "XLINUXSystem.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ----------------------------------------------------------------------------------------------*/

/*---- CLASS MEMBERS -------------------------------------------------------------------------------------------------*/



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XLINUXSYSTEM::XLINUXSYSTEM()
* @brief      Constructor
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:35:45
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XLINUXSYSTEM::XLINUXSYSTEM(): XSYSTEM()
{
  Clean();
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XLINUXSYSTEM::~XLINUXSYSTEM()
* @brief      Destructor
* @note       VIRTUAL
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:35:53
*
* @return     Does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
XLINUXSYSTEM::~XLINUXSYSTEM()
{
   Clean();
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSYSTEM_HARDWARETYPE XLINUXSYSTEM::GetTypeHardware(int* revision)
* @brief      Get Type Hardware
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:36:19
*
* @param[out]  revision : revision of hardware
*
* @return     XSYSTEM_HARDWARETYPE : hardware type
*
*---------------------------------------------------------------------------------------------------------------------*/
XSYSTEM_HARDWARETYPE XLINUXSYSTEM::GetTypeHardware(int* revision)
{
  #define CPUINFOREVISION __L("Revision")

  if(revision)
    {
      (*revision) = -1;

      XFILETXT* xfileTXT = new XFILETXT();
      if(xfileTXT)
        {
          XPATH xpath;

          xpath = __L("/proc/cpuinfo");

          if(xfileTXT->Open(xpath))
            {
              if(xfileTXT->ReadAllFile())
                {
                  for(int c=0; c<xfileTXT->GetNLines(); c++)
                    {
                      XSTRING* line = xfileTXT->GetLine(c);
                      if(line)
                        {
                          if(line->Find(CPUINFOREVISION, false) != XSTRING_NOTFOUND)
                            {
                              int position = line->Find(__L(":"), false);
                              if(position != XSTRING_NOTFOUND)
                                {
                                  XSTRING number;

                                  number = &line->Get()[position+1];

                                  number.UnFormat(__L(" %04x"), revision);
                                }
                            }
                        }
                    }
                }

              xfileTXT->Close();
            }

          delete xfileTXT;
        }
    }


  #ifdef HW_PC
  return XSYSTEM_HARDWARETYPE_PC;
  #endif

  #ifdef HW_LEX3V700A
  return XSYSTEM_HARDWARETYPE_LEX3V700A;
  #endif

  #ifdef HW_GUMSTIXCONNEX
  return XSYSTEM_HARDWARETYPE_GUMSTIXCONNEX;
  #endif

  #ifdef HW_RASPBERRYPI
  return XSYSTEM_HARDWARETYPE_RASPBERRYPI;
  #endif

  #ifdef HW_ODROIDC1
  return XSYSTEM_HARDWARETYPE_ODROIDC1;
  #endif

  #ifdef HW_ARTILA500
  return XSYSTEM_HARDWARETYPE_ARTILA500;
  #endif

  return XSYSTEM_HARDWARETYPE_UNKNOWN;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XSYSTEM_SO XLINUXSYSTEM::GetTypeSO()
* @brief      Get Type SO
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:37:14
*
* @return     XSYSTEM_SO : type of SO (enum XSYSTEM_SO)
*
*---------------------------------------------------------------------------------------------------------------------*/
XSYSTEM_SO XLINUXSYSTEM::GetTypeSO()
{
  #ifdef HW_PC
  return XSYSTEM_SO_LINUX;
  #endif

  #ifdef HW_LEX3V700A
  return XSYSTEM_SO_LINUX;
  #endif

  #ifdef HW_GUMSTIXCONNEX
  return XSYSTEM_SO_LINUX_EMBEDDED;
  #endif

  #ifdef HW_RASPBERRYPI
  return XSYSTEM_SO_LINUX_EMBEDDED;
  #endif

  #ifdef HW_ODROIDC1
  return XSYSTEM_SO_LINUX_EMBEDDED;
  #endif

  #ifdef HW_ARTILA500
  return XSYSTEM_SO_LINUX_EMBEDDED;
  #endif

  return XSYSTEM_SO_UNKNOWN;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         XLANGUAGE_CODE XLINUXSYSTEM::GetLanguage()
* @brief      Get Language Code
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:37:55
*
* @return     XLANGUAGE_CODE : language code
*
*---------------------------------------------------------------------------------------------------------------------*/
XLANGUAGE_CODE XLINUXSYSTEM::GetLanguage()
{
  return XLANGUAGE_UNKNOWN;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXSYSTEM::GetMemoryInfo(XDWORD& total,XDWORD& free)
* @brief      Get Memory Info
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:38:20
*
* @param[out] total : total memory
* @param[out] free : free memory
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::GetMemoryInfo(XDWORD& total,XDWORD& free)
{
  struct sysinfo meminfo;

  if(sysinfo(&meminfo) == -1) return false;

  long long totalvirtualmem = meminfo.totalram;
  //Add other values in next statement to avoid int overflow on right hand side...
  //totalvirtualmem += meminfo.totalswap;
  totalvirtualmem *= meminfo.mem_unit;

  long long virtualmemused = meminfo.totalram - meminfo.freeram;
  //Add other values in next statement to avoid int overflow on right hand side...
  virtualmemused += meminfo.totalswap - meminfo.freeswap;
  virtualmemused *= meminfo.mem_unit;

  total = (XDWORD)(totalvirtualmem / 1024);
  free  = (XDWORD)((totalvirtualmem - virtualmemused)  / 1024);

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXSYSTEM::MakeCommand(XCHAR* command, int* returncode)
* @brief      Make Command
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:39:14
*
* @param[in]  command : command to make
* @param[out] returncode : return code to command
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::MakeCommand(XCHAR* command, int* returncode)
{
  int status;

  XSTRING _command;

  _command = command;

  XSTRING_CREATEOEM(_command, charOEM)
  status = system(charOEM);
  XSTRING_DELETEOEM(charOEM)
  if(status == -1) return false;

  if(returncode) (*returncode) = status;

  return true;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXSYSTEM::ExecuteApplication(XCHAR* applicationpath, XCHAR* params)
* @brief      ExecuteApplication
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       03/06/2018 13:39:58
*
* @param[in]  applicationpath : application path + name to exec
* @param[in]  params : params to exec with application
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::ExecuteApplication(XCHAR* applicationpath, XCHAR* params)
{
  if(!applicationpath) return false;

  pid_t pID;
  bool  exist  = false;
  bool  status = false;

  XFILE* xfile = xfactory->Create_File();
  if(xfile)
    {
      exist = xfile->Open(applicationpath);
      xfile->Close();
    }

  xfactory->Delete_File(xfile);

  if(!exist) return false;

  //-----------------------------------------
  // exec attributtes

  XSTRING cmd;

  cmd  = __L("chmod 775 ");
  cmd += applicationpath;

  XSTRING_CREATEOEM(cmd, charOEM)
  status = system(charOEM);
  XSTRING_DELETEOEM(charOEM)

  //if(status == -1) return false;

  //------------------------------------------

  XSTRING _params;

  cmd     = applicationpath;

  _params.Add(cmd);
  if(params)
    {
      _params.Add(" ");
      _params.Add(params);
    }
   

  /*
  XSTRING_CREATEOEM(cmd, charOEM2)
  status = system(charOEM2);
  XSTRING_DELETEOEM(charOEM2)
  */

  //XDEBUGTRACE_PRINTCOLOR(3, __L("EXEC PARAMS: cmd [%s] params [%s]"), cmd.Get(), _params.Get());

  #define MAXNPARAMS  20

  char* param[MAXNPARAMS] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                              NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
                            };
  int   start             = 0;
  bool  endfound          = false;

  for(int c=0; c<MAXNPARAMS; c++)
    {
      XSTRING string;
      int     found = _params.Find(__L(" "), true, start);

      if(found == XSTRING_NOTFOUND)
        {
          _params.Copy(start, string);
          endfound = true;
        }
       else
        {
          _params.Copy(start, found, string);
          start = found+1;
        }

      string.CreateOEM(param[c]);

      //XDEBUGTRACE_PRINTCOLOR(3, __L("EXEC PARAMS: param%02d [%s]"), c, string.Get());

      if(endfound) break;
    }

  pID = fork();

  switch(pID)
    {
      case -1 : break;

      case  0 : { // Child
                  XSTRING_CREATEOEM(cmd, charcmd)

                  int _status = execl(charcmd , param[ 0], param[ 1], param[ 2], param[ 3], param[ 4], param[ 5], param[ 6], param[ 7], param[ 8], param[ 9]
                                              , param[10], param[11], param[12], param[13], param[14], param[15], param[16], param[17], param[18], param[19]
                                              , NULL);

                  if(_status == -1)
                    {
                     XDEBUGTRACE_PRINTCOLOR(4,__L("Error en EXEC: %d"), errno);
                    }
                  else
                   {
                     status = true;
                    }

                  XSTRING_DELETEOEM(charcmd)
                }
                break;

      default : status = true;
                break;
    }

  for(int c=0; c<MAXNPARAMS; c++)
    {
      delete [] param[c];
    }

  return status;
}





/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXSYSTEM::IsApplicationRunning(XCHAR* applicationname, XDWORD* ID)
* @brief      Is Application Running
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       03/06/2018 17:14:30
*
* @param[in]  applicationname : application name
* @param[out] ID : ID of the application in the SO
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::IsApplicationRunning(XCHAR* applicationname, XDWORD* ID)
{
  DIR*            dir;
  struct dirent*  ent;
  char*           endptr;
  char            buf[512];

  if(!(dir = opendir("/proc"))) return -1;

  while((ent = readdir(dir)) != NULL)
    {
      // if endptr is not a null character, the directory is not entirely numeric, so ignore it
      long lpid = strtol(ent->d_name, &endptr, 10);
      if(*endptr != '\0')
        {
          continue;
        }

      // try to open the cmdline file
      snprintf(buf, sizeof(buf), "/proc/%ld/status", lpid);
      FILE* fp = fopen(buf, "r");
      if(fp)
        {
          if(fgets(buf, sizeof(buf), fp) != NULL)
            {
              // check the first token in the file, the program name
              char* first = strtok(buf, " ");

              XPATH nameall;

              nameall = first;
              nameall.SetOnlyNamefile();
              nameall.DeleteCharacter(__C(' ') , XSTRINGCONTEXT_ATFIRST);
              nameall.DeleteCharacter(__C('\t'), XSTRINGCONTEXT_ATFIRST);
              nameall.DeleteCharacter(__C('\n'), XSTRINGCONTEXT_ATEND);
              nameall.DeleteCharacter(__C('\r'), XSTRINGCONTEXT_ATEND);

              if(!nameall.Compare(applicationname, false))
                {
                  if(ID) (*ID) = (XDWORD)lpid;

                  fclose(fp);
                  closedir(dir);

                  return true;

                }
            }

          fclose(fp);
        }
    }

  closedir(dir);


  return false;
}



/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         bool XLINUXSYSTEM::ShutDown(XSYSTEM_SHUTDOWNTYPE type)
* @brief      ShutDown of SO
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       03/06/2018 17:15:13
*
* @param[in]  type : type of shutdown of SO
*
* @return     bool : true if is succesful. 
*
*---------------------------------------------------------------------------------------------------------------------*/
bool XLINUXSYSTEM::ShutDown(XSYSTEM_SHUTDOWNTYPE type)
{
  switch(type)
    {
      case XSYSTEM_SHUTDOWNTYPE_REBOOT   :
                                            sync();
                                            reboot(LINUX_REBOOT_CMD_RESTART);
                                            break;

      case XSYSTEM_SHUTDOWNTYPE_LOGOFF   :  break;

      case XSYSTEM_SHUTDOWNTYPE_POWEROFF :  sync();
                                            reboot(LINUX_REBOOT_CMD_POWER_OFF);
                                            break;
    }

  return true;
}




/**-------------------------------------------------------------------------------------------------------------------
*
* @fn         void XLINUXSYSTEM::Clean()
* @brief      Clean the attributes of the class: Default initialice
* @note       INTERNAL
* @ingroup    PLATFORM_LINUX
*
* @author     Abraham J. Velez 
* @date       03/06/2018 17:28:43
*
* @return     void : does not return anything. 
*
*---------------------------------------------------------------------------------------------------------------------*/
void XLINUXSYSTEM::Clean()
{
  
}


