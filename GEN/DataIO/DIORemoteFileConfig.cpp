//------------------------------------------------------------------------------------------
//  DIOREMOTEFILECONFIG.CPP
//
//  Application CFG class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/05/2004 20:55:19
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOWebClient.h"

#include "DIORemoteFileConfig.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOREMOTEFILECONFIG::DIOREMOTEFILECONFIG
*/
/**
//
//  Class Constructor DIOREMOTEFILECONFIG
//
//  @author       Abraham J. Velez
//  @version      27/06/2016 12:45:24
//
//  @param        namefile :
//  @param        additionalURL :
*/
/*-----------------------------------------------------------------*/
DIOREMOTEFILECONFIG::DIOREMOTEFILECONFIG(XCHAR* namefile) : XFILECONFIG(namefile)
{
  Clean();

  webclient = new DIOWEBCLIENT();

}




//-------------------------------------------------------------------
//  DIOREMOTEFILECONFIG::~DIOREMOTEFILECONFIG
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/05/2004 20:56:09
//
//  @return
//  */
//-------------------------------------------------------------------
DIOREMOTEFILECONFIG::~DIOREMOTEFILECONFIG()
{
  if(webclient)    delete webclient;

  Clean();
}



//-------------------------------------------------------------------
//  DIOREMOTEFILECONFIG::Ini
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/08/2004 16:27:54
//
//  @return       bool :
//  */
//-------------------------------------------------------------------
bool DIOREMOTEFILECONFIG::Ini()
{
  if(!webclient)         return false;
  if(namefile.IsEmpty()) return false;

  Default();

  XPATH  xpathroot;
  XPATH  xpathremotefile;
  DIOURL downloadURL;
  bool   status[2];

  AddValue(XFILECONFIGVALUETYPE_STRING, DIOREMOTEFILECONFIG_SECTIONGENERAL, DIOREMOTEFILECONFIG_URLREMOTECFG, &URLremoteCFG);

  XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathroot);
  xpathfile.Set(xpathroot.Get());
  if(!xpathfile.IsEmpty()) xpathfile.Slash_Add();
  xpathfile.Add(namefile.Get());
  xpathfile.Add(XFILECONFIG_EXTENSIONFILE);

  status[0] = Load();

  if(!URLremoteCFG.IsEmpty())
    {
      downloadURL.Set(URLremoteCFG.Get());
      downloadURL.Slash_Add();
      downloadURL.Add(DIOREMOTEFILECONFIG_PREFIXNAMEFILE);
      downloadURL.Add(namefile.Get());
      downloadURL.Add(XFILECONFIG_EXTENSIONFILE);

      XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpathroot);
      xpathremotefile.Set(xpathroot.Get());
      if(!xpathremotefile.IsEmpty()) xpathremotefile.Slash_Add();
      xpathremotefile.Add(DIOREMOTEFILECONFIG_PREFIXNAMEFILE);
      xpathremotefile.Add(namefile.Get());
      xpathremotefile.Add(XFILECONFIG_EXTENSIONFILE);

      if(webclient->Get(downloadURL, xpathremotefile))
        {
          status[0] = Load(xpathremotefile);
        }
    }

  status[1] = Save();

  return (status[0] && status[1]);
}


