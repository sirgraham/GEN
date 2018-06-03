/*------------------------------------------------------------------------------------------
// DIOSSHREVERSE.CPP
*/
/**
// \class
//
//  DIO SSH reverse connexions (only ssh/sshpass avaible [linux])
//
//  @author: Abraham J. Velez
//
//  Date of Creation : 27/03/2018 7:24:08
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XThreadCollected.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOWebClient.h"


#include "DIOSSHReverse.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------

DIOSSHREVERSE* DIOSSHREVERSE::instance = NULL;

//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//   DIOSSHREVERSE::DownloadCFG
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  27/03/2018 7:38:29
//
//  @return   bool :
//
//  @param    XCHAR* :
//  @param    XSTRING& :
//  @param    XSTRING& :
//
*//*-----------------------------------------------------------------*/
bool DIOSSHREVERSE::DownloadCFG(XCHAR* URL, XSTRING& publicIP, XSTRING& localIP)
{
  if(!URL)   return false;

  XBUFFER xbuffer;
  DIOURL  _URL;
  bool    status  = false;
  bool    result  = false;

  DIOWEBCLIENT* webclient = new DIOWEBCLIENT();
  if(!webclient)  return false;

  _URL.Set(URL);
  _URL.Slash_Add();
  _URL.Add(DIOSSHREVERSE_CFGREMOTENAMEFILE);

  status = webclient->Get(_URL, xbuffer, NULL, 3);
  if(status)
    {
      XFILEINI* fileini = new XFILEINI();
      if(fileini)
        {
          status = fileini->AddBufferLines(XFILETXTFORMATCHAR_ASCII, xbuffer);
          if(status)
            {
              status = fileini->ConvertFromLines();
              if(status)
                {
                  int c = 1;
                  while(1)
                    {
                      XSTRING section;
                      XSTRING value;

                      section.Format(__L("ENTRY%02d"), c);

                      if(!fileini->SelectSection(section)) break;

                      status = fileini->ReadValue(__L("isactive"), value);
                      if(status)
                        {
                          if(!value.Compare(__L("yes"), true))
                            {
                              DIOURL* urlorigin = diofactory->CreateURL();
                              if(urlorigin)
                                {
                                  XSTRING _publicIP;
                                  XSTRING _localremoteIP;

                                  status = fileini->ReadValue(__L("urlorigin"), (*urlorigin));
                                  if(status)
                                    {
                                      bool validaccess = false;

                                      urlorigin->ResolveURL(_publicIP);

                                      if(!publicIP.Compare(_publicIP))
                                        {
                                          validaccess = true;

                                          status = fileini->ReadValue(__L("localremoteip"), _localremoteIP);
                                          if(status)
                                            {
                                              if(_localremoteIP.GetSize())
                                                {
                                                  if(_localremoteIP.Compare(localIP)) validaccess = false;
                                                }
                                            }
                                        }

                                      if(validaccess)
                                        {
                                          localIP   = __L("localhost");

                                          status = fileini->ReadValue(__L("urltarget"), value);
                                          if(status) GetURLTarget()->Set(value);

                                          status = fileini->ReadValue(__L("localip"), value);
                                          if(status)  GetLocalIP()->Set(value);

                                          status = fileini->ReadValue(__L("login"), value);
                                          if(status) GetLogin()->Set(value);

                                          status = fileini->ReadValue(__L("password"), value);
                                          if(status) GetPassword()->Set(value);

                                          status = fileini->ReadValue(__L("port"), value);
                                          if(status) SetPort(value.ConvertToInt());

                                          result = true;
                                        }
                                    }

                                  diofactory->DeleteURL(urlorigin);
                                }
                            }
                        }

                      c++;
                    }
                }
            }

          delete fileini;
        }
    }

  delete webclient;

  return result;
}





/*-------------------------------------------------------------------
//   DIOSSHREVERSE::Activate
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  14/04/2018 17:43:48
//
//  @return   bool :
//
//  @param    withscreen :
//
*//*-----------------------------------------------------------------*/
bool DIOSSHREVERSE::Activate()
{
  XSTRING command;
  bool    status     = false;
  int     returncode = 0;

  //command.AddFormat(__L("%s -p %s ssh -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no -o ExitOnForwardFailure=yes -N -R %d:%s:%d %s@%s &"), DIOSSHREVERSE_DEFAULTAPPLICATION, password.Get(), port, localIP.Get() ,DIOSSHREVERSE_DEFAULTPORTSSH, login.Get(), URLtarget.Get());
  command.AddFormat(__L("%s -p %s autossh -M 0 -o \"ServerAliveInterval 30\" -o \"ServerAliveCountMax 3\" -o UserKnownHostsFile=/dev/null -o StrictHostKeyChecking=no -o ExitOnForwardFailure=yes -N -R %d:%s:%d %s@%s &"), DIOSSHREVERSE_DEFAULTAPPLICATION, password.Get(), port, localIP.Get() ,DIOSSHREVERSE_DEFAULTPORTSSH, login.Get(), URLtarget.Get());
  status = XSYSTEM::GetInstance().MakeCommand(command.Get(), &returncode);

  XTIMER* xtimerout =  xfactory->CreateTimer();
  if(xtimerout)
    {
      while(1)
        {
          if(IsRunning())
            {
              status = true;
              break;
            }

          XSLEEP::GetInstance().MilliSeconds(500);

          if(xtimerout->GetMeasureSeconds()>5) break;
        }
    }

  XLOG::GetInstance().AddEntry((status?XLOGLEVEL_INFO:XLOGLEVEL_ERROR), DIOSSHREVERSE_LOGSECTIONID, false, __L("Activate service return code [%d]"), returncode);

  return status;
}




/*-------------------------------------------------------------------
//   DIOSSHREVERSE::DeActivate
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  27/03/2018 7:41:33
//
//  @return   bool :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
bool DIOSSHREVERSE::DeActivate()
{
  XSTRING command;
  bool    status     = false;
  int     returncode = 0;

  command.Format(__L("killall -9 %s > /dev/null"), DIOSSHREVERSE_DEFAULTAPPLICATION);
  status = XSYSTEM::GetInstance().MakeCommand(command.Get(), &returncode);

  command.Format(__L("killall -9 %s > /dev/null"), DIOSSHREVERSE_DEFAULTAPPLICATION2);
  status = XSYSTEM::GetInstance().MakeCommand(command.Get(), &returncode);
    
  XLOG::GetInstance().AddEntry((status?XLOGLEVEL_INFO:XLOGLEVEL_ERROR), DIOSSHREVERSE_LOGSECTIONID, false, __L("Deactivate service return code [%d]"), returncode);

  status = true;
  if(returncode) status = false;

  return status;
}



/*-------------------------------------------------------------------
//   DIOSSHREVERSE::IsRunning
*/
/**
//
//
//  @author   Abraham J. Velez
//  @version  27/03/2018 7:45:02
//
//  @return   bool :
//
//  @param    ) :
//
*//*-----------------------------------------------------------------*/
bool DIOSSHREVERSE::IsRunning()
{
  
  bool status = false;

  //status = xsystem->IsApplicationRunning(DIOSSHREVERSE_DEFAULTAPPLICATION);
  //if(status)
    {
      //status = false;

      XSTRING command;
      XSTRING publicIPtarget;
      int     returncode = 0;

      DIOURL* URLpublic = diofactory->CreateURL();
      if(URLpublic)
        {
          URLpublic->Set(URLtarget.Get());
          URLpublic->ResolveURL(publicIPtarget);

          diofactory->DeleteURL(URLpublic);
        }

      if(!publicIPtarget.IsEmpty())
        {
          XPATH   xpath;
          XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
          xpath.Slash_Add();
          xpath.Add(__L("backscreen"));

          command.Format(__L("netstat -napt > %s"), xpath.Get());
          if(XSYSTEM::GetInstance().MakeCommand(command.Get(), &returncode))
            {
              XFILETXT* xfiletxt = new XFILETXT();
              if(xfiletxt)
                {
                  if(xfiletxt->Open(xpath, true))
                    {
                      if(xfiletxt->ReadAllFile())
                        {
                          for(int c=0; c<xfiletxt->GetNLines(); c++)
                            {
                              if(xfiletxt->GetLine(c))
                                {
                                  XSTRING* line = xfiletxt->GetLine(c);
                                  if(line)
                                    {
                                      XSTRING application;

                                      application = __L("/ssh ");

                                      if((line->Find(publicIPtarget.Get() , false, 0) != XSTRING_NOTFOUND) &&
                                         (line->Find(__L("ESTABLISHED")   , false, 0) != XSTRING_NOTFOUND) &&
                                         (line->Find(application.Get()    , false, 0) != XSTRING_NOTFOUND))
                                        {
                                          status = true;
                                          break;
                                        }
                                    }
                                }
                            }
                        }

                      xfiletxt->Close();
                    }

                  xfiletxt->GetPrimaryFile()->Erase(xpath);

                  delete xfiletxt;
                }
            }
        }
    }

  XLOG::GetInstance().AddEntry(XLOGLEVEL_INFO, DIOSSHREVERSE_LOGSECTIONID, false, __L("Check is active: %s"), status?__L("yes"):__L("no"));

  return status;
}


