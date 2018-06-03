//------------------------------------------------------------------------------------------
//  DIOWEBCLIENT.CPP
//
//  DIO Web Client class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 20/01/2007 21:09:36
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XTimer.h"
#include "XPath.h"
#include "XFile.h"
#include "XDebugTrace.h"

#include "DIOFactory.h"
#include "DIOURL.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"
#include "DIOWebClientXEvent.h"

#include "DIOWebClient.h"

#include "XMemory.h"


//---- GENERAL VARIABLE --------------------------------------------------------------------



//---- CLASS MEMBERS -----------------------------------------------------------------------







/*-------------------------------------------------------------------
//  DIOWEBCLIENT_HEADER::DIOWEBCLIENT_HEADER
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2013 23:06:59
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWEBCLIENT_HEADER::DIOWEBCLIENT_HEADER()
{
  Clean();

}



/*-------------------------------------------------------------------
//  DIOWEBCLIENT_HEADER::~DIOWEBCLIENT_HEADER
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2013 23:07:22
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWEBCLIENT_HEADER::~DIOWEBCLIENT_HEADER()
{
  DeleteAllLines();

  Clean();
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT_HEADER::GetMethod
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2013 23:07:32
//
//  @return       bool :
//  @param        method :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT_HEADER::GetMethod(XSTRING& method)
{
  if(!GetFormat(false, 0, &method)) return false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT_HEADER::GetResource
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2013 23:07:40
//
//  @return       bool :
//  @param        resource :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT_HEADER::GetResource(XSTRING& resource)
{
  if(!GetFormat(false, 1, &resource)) return false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT_HEADER::GetVersion
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2013 23:07:49
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOWEBCLIENT_HEADER::GetVersion()
{
  int data = 0;

  if(!GetFormat(false, 2, &data)) return false;

  return data;
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT_HEADER::GetSubVersion
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2013 23:07:59
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOWEBCLIENT_HEADER::GetSubVersion()
{
  int data = 0;

  if(!GetFormat(false, 3, &data)) return false;

  return data;
}


/*-------------------------------------------------------------------
//   DIOWEBCLIENT_HEADER::GetResultServer
*/
/**
//
//
//
//  @author   Abraham J. Velez
//  @version
//
//  @return   int :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
int DIOWEBCLIENT_HEADER::GetResultServer()
{
  int data = 0;

  if(!GetFormat(true, 2, &data)) return false;

  return data;
}



/*-------------------------------------------------------------------
//  DIOWEBCLIENT_HEADER::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      11/04/2013 23:08:03
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOWEBCLIENT_HEADER::Clean()
{


}






/*-------------------------------------------------------------------
//  DIOWEBCLIENT::DIOWEBCLIENT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      21/06/2011 16:57:01
//
//  @return


//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
DIOWEBCLIENT::DIOWEBCLIENT()
{
  Clean();

  RegisterEvent(DIOWEBCLIENTXEVENTTYPE_OPENWEB);
  RegisterEvent(DIOWEBCLIENTXEVENTTYPE_WRITEHEADER);
  RegisterEvent(DIOWEBCLIENTXEVENTTYPE_SENDPOSTDATA);
  RegisterEvent(DIOWEBCLIENTXEVENTTYPE_READHEADER);
  RegisterEvent(DIOWEBCLIENTXEVENTTYPE_READBODYBLOCK);
  RegisterEvent(DIOWEBCLIENTXEVENTTYPE_CLOSEWEB);

  if(xfactory && diofactory)
    {
      timerout        = xfactory->CreateTimer();

      diostreamcfg = new DIOSTREAMTCPIPCONFIG();
      if(diostreamcfg) diostream = (DIOSTREAMTCPIP*)diofactory->CreateStreamIO(diostreamcfg);
    }

}




//-------------------------------------------------------------------
//  DIOWEBCLIENT::~DIOWEBCLIENT
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/01/2007 21:13:27
//
//  @return
//  */
//-------------------------------------------------------------------
DIOWEBCLIENT::~DIOWEBCLIENT()
{
  DeRegisterEvent(DIOWEBCLIENTXEVENTTYPE_OPENWEB);
  DeRegisterEvent(DIOWEBCLIENTXEVENTTYPE_WRITEHEADER);
  DeRegisterEvent(DIOWEBCLIENTXEVENTTYPE_SENDPOSTDATA);
  DeRegisterEvent(DIOWEBCLIENTXEVENTTYPE_READHEADER);
  DeRegisterEvent(DIOWEBCLIENTXEVENTTYPE_READBODYBLOCK);
  DeRegisterEvent(DIOWEBCLIENTXEVENTTYPE_CLOSEWEB);

  if(diofactory)
    {
      if(diostream)
        {
          diofactory->DeleteStreamIO(diostream);
          diostream = NULL;
        }

      if(diostreamcfg)
        {
          delete diostreamcfg;
          diostreamcfg = NULL;
        }
    }

  if(timerout)
    {
      xfactory->DeleteTimer(timerout);
      timerout        = NULL;
    }

  Clean();
}





/*-------------------------------------------------------------------
//  DIOWEBCLIENT::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 12:11:30
//
//  @return       bool :
//
//  @param        url :
//  @param        webpage :
//  @param        addheader :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::Get(DIOURL& url, XBUFFER& webpage, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  return MakeOperationBuffer(url, webpage, NULL, addheader, timeout, localIP);
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 12:16:50
//
//  @return       bool :
//
//  @param        url :
//  @param        webpage :
//  @param        addheader :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::Get(XCHAR* url, XBUFFER& webpage, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  DIOURL _url;
  _url = url;

  return MakeOperationBuffer(_url, webpage, NULL, addheader, timeout, localIP);
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 12:16:12
//
//  @return       bool :
//
//  @param        url :
//  @param        pathwebpage :
//  @param        addheader :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::Get(DIOURL& url, XPATH& pathwebpage, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  XFILE* webfile  = xfactory->Create_File();
  if(!webfile) return false;

  XBUFFER webpage;
  bool    status  = false;

  if(!webfile->Open(pathwebpage, false))
    {
      if(webfile->Create(pathwebpage))
        {
          status = true;
        }

    } else status = true;

  if(status)
    {
      status = MakeOperationFile(url, *webfile, NULL, addheader, timeout, localIP);
      webfile->Close();

      if(!status)  webfile->Erase(pathwebpage);
    }

  xfactory->Delete_File(webfile);

  return status;
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 12:15:40
//
//  @return       bool :
//
//  @param        url :
//  @param        pathwebpage :
//  @param        addheader :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::Get(XCHAR* url, XPATH& pathwebpage, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  DIOURL _url;
  _url = url;

  return Get(_url, pathwebpage, addheader, timeout, localIP);
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT::Post
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 12:15:12
//
//  @return       bool :
//
//  @param        url :
//  @param        webpage :
//  @param        postdata :
//  @param        addheader :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::Post(DIOURL& url, XBUFFER& webpage, XBUFFER* postdata, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  return MakeOperationBuffer(url, webpage, postdata, addheader, timeout, localIP);
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT::Post
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 12:13:57
//
//  @return       bool :
//
//  @param        url :
//  @param        webpage :
//  @param        postdata :
//  @param        addheader :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::Post(XCHAR* url, XBUFFER& webpage, XBUFFER* postdata, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  DIOURL _url;
  _url = url;

  return MakeOperationBuffer(_url, webpage, postdata, addheader, timeout, localIP);
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT::Post
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 12:13:04
//
//  @return       bool :
//
//  @param        url :
//  @param        pathwebpage :
//  @param        postdata :
//  @param        addheader :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::Post(DIOURL& url, XPATH& pathwebpage, XBUFFER* postdata, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  XFILE* webfile  = xfactory->Create_File();
  if(!webfile) return false;

  XBUFFER webpage;
  bool    status  = false;

  if(webfile->Create(pathwebpage))
    {
      status = Post(url, webpage, postdata, addheader, timeout, localIP);

      if(status) webfile->Write(webpage.Get(), webpage.GetSize());

      webfile->Close();

      if(!status) webfile->Erase(pathwebpage);
    }

  xfactory->Delete_File(webfile);

  return status;
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT::Post
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 12:12:52
//
//  @return       bool :
//
//  @param        url :
//  @param        pathwebpage :
//  @param        postdata :
//  @param        addheader :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::Post(XCHAR* url, XPATH& pathwebpage, XBUFFER* postdata, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  DIOURL _url;
  _url = url;

  return Post(_url, pathwebpage, postdata, addheader, timeout, localIP);
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT::Post
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 12:15:12
//
//  @return       bool :
//
//  @param        url :
//  @param        webpage :
//  @param        postdata :
//  @param        addheader :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::Post(DIOURL& url, XBUFFER& webpage, XBUFFER* boundary, XFILE* file, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  return MakeOperationFile(url, webpage, boundary, file, addheader, timeout, localIP);
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT::Post
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 12:13:57
//
//  @return       bool :
//
//  @param        url :
//  @param        webpage :
//  @param        postdata :
//  @param        addheader :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::Post(XCHAR* url, XBUFFER& webpage, XBUFFER* boundary, XFILE* file, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  DIOURL _url;
  _url = url;

  return Post(_url, webpage, boundary, file, addheader, timeout, localIP);
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT::Post
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 12:13:04
//
//  @return       bool :
//
//  @param        url :
//  @param        pathwebpage :
//  @param        postdata :
//  @param        addheader :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::Post(DIOURL& url, XPATH& pathwebpage, XBUFFER* boundary, XFILE* file, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  XFILE* webfile  = xfactory->Create_File();
  if(!webfile) return false;

  XBUFFER webpage;
  bool    status  = false;

  if(webfile->Create(pathwebpage))
    {
      if(Post(url, webpage, boundary, addheader, timeout, localIP))
        {
          status = true;
        }

      webfile->Write(webpage.Get(), webpage.GetSize());

      webfile->Close();
    }

  xfactory->Delete_File(webfile);

  return status;
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT::Post
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 12:12:52
//
//  @return       bool :
//
//  @param        url :
//  @param        pathwebpage :
//  @param        postdata :
//  @param        addheader :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::Post(XCHAR* url, XPATH& pathwebpage, XBUFFER* boundary, XFILE* file, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  DIOURL _url;
  _url = url;

  return Post(_url, pathwebpage, boundary, file, addheader, timeout, localIP);
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT::MakeOperation
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/06/2014 12:09:08
//
//  @return       bool :
//
//  @param        url :
//  @param        webpage :
//  @param        postdata :
//  @param        addhead :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::MakeOperationBuffer(DIOURL& url, XBUFFER& webpage, XBUFFER* postdata, XCHAR* addhead, int timeout, XSTRING* localIP)
{
  if(!diostreamcfg)     return false;
  if(!diostream)        return false;
  if(!timerout)         return false;

  bool status = true;

  if(!url.HaveHTTPID()) url.AddHTTPID();

  XSTRING             server;
  XSTRING             login;
  XSTRING             password;
  XSTRING             loginpassword;
  XSTRING             loginpasswordbase64;
  DIOURL              resource;
  DIOWEBCLIENTXEVENT  xevent(this, XEVENTTYPE_NONE);

  diostreamcfg->SetFromString(url.Get());

  url  = diostreamcfg->GetRemoteURL()->Get();
  if(diostreamcfg->GetRemotePort()) port = diostreamcfg->GetRemotePort();

  url.GetHTTPServer(server, login, password);
  url.GetHTTPResource(resource);

  loginpassword.Format(__L("%s:%s"), login.Get(), password.Get());
  loginpassword.ConvertToBase64(loginpasswordbase64);

  if(resource.IsEmpty()) resource.Slash_Add();

  diostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);
  if(localIP)
    {
      if(!localIP->IsEmpty()) diostreamcfg->GetLocalIP()->Set(localIP->Get());
    }

  if(proxyurl.IsEmpty())
    {
      diostreamcfg->GetRemoteURL()->Set(server.Get());
      diostreamcfg->SetRemotePort(port);
    }
   else
    {
      diostreamcfg->GetRemoteURL()->Set(proxyurl.Get());
      diostreamcfg->SetRemotePort(proxyport);
    }

  //--- Connexion WEB server -------------------

  //XDEBUGTRACE_PRINTCOLOR(XDEBUG_COLORGREEN, __L("WEB client access %s:%d resource [%s]"), diostreamcfg->GetRemoteURL()->Get(), diostreamcfg->GetRemotePort(), resource.Get());

  if(!diostream->Open())
    {
      diostream->Close();
      return false;
    }

  if(!diostream->WaitToConnected(timeout))
    {
      diostream->Close();
      return false;
    }

  xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_OPENWEB);
  PostEvent(&xevent);

  //--- Send Header ----------------------------

  XSTRING sendheader;

  //resource.EncodeUnsafeChars();

  sendheader.Format(__L("%s %s HTTP/1.0\r\n") , postdata?__L("POST"):__L("GET"), resource.Get());

  sendheader += __L("Host: ");
  sendheader += server.Get();

  if(port!=DIOWEBCLIENT_DEFAULTPORT)
    {
      XSTRING stringport;

      stringport.ConvertFromInt(port);

      sendheader += __L(":");
      sendheader += stringport.Get();
    }
  sendheader += __L("\r\n");

  if(addhead) sendheader += addhead;

  if(login.GetSize() && password.GetSize())
    {
      sendheader += __L("Authorization: Basic ");
      sendheader += loginpasswordbase64;
      sendheader += __L("\r\n");
    }

  if(postdata)
    {
      XSTRING stringlenght;
      stringlenght.Format(__L("Content-Length: %d\r\n"), postdata->GetSize());

      sendheader += stringlenght;
    }

  sendheader += __L("Connection: close\r\n");

  sendheader += __L("\r\n");

  //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB Client Header %s"), sendheader.Get());

  diostream->WriteStr(sendheader);

  xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_WRITEHEADER);
  PostEvent(&xevent);

  //--- Send Post data -------------------------

  if(postdata)
    {
      diostream->Write(postdata->Get(), postdata->GetSize());

      xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_SENDPOSTDATA);
      PostEvent(&xevent);
    }

  diostream->WaitToFlushOutXBuffer(timeout);

  //--- Read Header ----------------------------

  if(!header.Read(diostream, timerout, timeout))
    {
      //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Error! leer header! "));

      diostream->Close();
      return false;
    }

  //--- Read Content ---------------------------

  if((header.GetResultServer()>=400)||(!header.GetResultServer()))
    {
      //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Error! leer header2! "));

      xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_HEADERERROR);
      PostEvent(&xevent);

      diostream->Close();
      return false;
    }

  xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_READHEADER);
  PostEvent(&xevent);

  //--- Read Content ---------------------------

  webpage.Delete();

  XBYTE* buffer = new XBYTE[DIOWEBCLIENT_MAXBUFFER];
  if(buffer)
    {
      int br;

      timerout->Reset();

      do{ memset(buffer, 0,  DIOWEBCLIENT_MAXBUFFER);

          br = diostream->Read(buffer, DIOWEBCLIENT_MAXBUFFER);
          if(br)
            {
              timerout->Reset();

              webpage.Add(buffer, br);

              //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Block read %d [size: %d]"), br, webpage.GetSize());

              xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_READBODYBLOCK);
              xevent.SetDownloadSize(br);
              PostEvent(&xevent);
            }
           else
            {
              if(timerout->GetMeasureSeconds()>(XDWORD)timeout)
                {
                  //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Error! time out! "));
                  status = false;
                  break;
                }

              if(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED)
                {
                  if(!diostream->GetInXBuffer()->GetSize())
                    {
                      //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Connexion close! "));
                      break;
                    }
                }
            }


        } while(1);

      delete [] buffer;
    }

  //--- Close ----------------------------------

  diostream->Close();

  xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_CLOSEWEB);
  PostEvent(&xevent);


  return status;
}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT::MakeOperationFile
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      23/09/2016 16:17:42
//
//  @return       bool :
//
//  @param        url :
//  @param        webpage :
//  @param        postdata :
//  @param        addhead :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::MakeOperationFile(DIOURL& url, XFILE& webpage, XBUFFER* postdata, XCHAR* addhead, int timeout, XSTRING* localIP)
{
  if(!diostreamcfg)     return false;
  if(!diostream)        return false;

  if(!timerout)         return false;

  bool status = true;

  if(!url.HaveHTTPID()) url.AddHTTPID();

  XSTRING             server;
  XSTRING             login;
  XSTRING             password;
  XSTRING             loginpassword;
  XSTRING             loginpasswordbase64;
  DIOURL              resource;
  DIOWEBCLIENTXEVENT  xevent(this, XEVENTTYPE_NONE);

  diostreamcfg->SetFromString(url.Get());

  url  = diostreamcfg->GetRemoteURL()->Get();
  if(diostreamcfg->GetRemotePort()) port = diostreamcfg->GetRemotePort();

  url.GetHTTPServer(server, login, password);
  url.GetHTTPResource(resource);

  //XDEBUGTRACE_PRINTCOLOR(1, __L("WEB Download %s %s [%d]"), server.Get(), resource.Get(), port);

  loginpassword.Format(__L("%s:%s"), login.Get(), password.Get());
  loginpassword.ConvertToBase64(loginpasswordbase64);

  if(resource.IsEmpty()) resource.Slash_Add();

  diostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);
  if(localIP)
    {
      if(!localIP->IsEmpty()) diostreamcfg->GetLocalIP()->Set(localIP->Get());
    }

  if(proxyurl.IsEmpty())
    {
      diostreamcfg->GetRemoteURL()->Set(server.Get());
      diostreamcfg->SetRemotePort(port);
    }
   else
    {
      diostreamcfg->GetRemoteURL()->Set(proxyurl.Get());
      diostreamcfg->SetRemotePort(proxyport);
    }

  //--- Connexion WEB server -------------------

  if(!diostream->Open())
    {
      diostream->Close();
      return false;
    }

  if(!diostream->WaitToConnected(timeout))
    {
      diostream->Close();
      return false;
    }

  xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_OPENWEB);
  PostEvent(&xevent);

  //--- Send Header ----------------------------

  XSTRING sendheader;

  //resource.EncodeUnsafeChars();

  resource.Replace(__L(" "),__L("%20"));
  sendheader.Format(__L("%s %s HTTP/1.0\r\n") , postdata?__L("POST"):__L("GET"), resource.Get());

  sendheader += __L("Host: ");
  sendheader += server.Get();

  if(port!=DIOWEBCLIENT_DEFAULTPORT)
    {
      XSTRING stringport;

      stringport.ConvertFromInt(port);

      sendheader += __L(":");
      sendheader += stringport.Get();
    }
  sendheader += __L("\r\n");

  if(addhead) sendheader += addhead;

  if(login.GetSize() && password.GetSize())
    {
      sendheader += __L("Authorization: Basic ");
      sendheader += loginpasswordbase64;
      sendheader += __L("\r\n");
    }

  if(postdata)
    {
      XSTRING stringlenght;
      stringlenght.Format(__L("Content-Length: %d\r\n"), postdata->GetSize());

      sendheader += stringlenght;
    }

  sendheader += __L("Connection: close\r\n");

  sendheader += __L("\r\n");

  //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB Client Header %s"), sendheader.Get());

  diostream->WriteStr(sendheader);

  xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_WRITEHEADER);
  PostEvent(&xevent);

  //--- Send Post data -------------------------

  if(postdata)
    {
      diostream->Write(postdata->Get(), postdata->GetSize());

      xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_SENDPOSTDATA);
      PostEvent(&xevent);
    }

  diostream->WaitToFlushOutXBuffer(timeout);

  //--- Read Header ----------------------------

  if(!header.Read(diostream, timerout, timeout))
    {
      //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Error! leer header! "));

      diostream->Close();
      return false;
    }

  //--- Read Content ---------------------------

  if((header.GetResultServer()>=400)||(!header.GetResultServer()))
    {
      //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Error! leer header2! "));

      xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_HEADERERROR);
      PostEvent(&xevent);

      diostream->Close();
      return false;
    }

  xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_READHEADER);
  PostEvent(&xevent);

  //--- Read Content ---------------------------

  //webpage.Delete();

  XBYTE* buffer = new XBYTE[DIOWEBCLIENT_MAXBUFFER];
  if(buffer)
    {
      int br;

      timerout->Reset();

      do{ memset(buffer, 0,  DIOWEBCLIENT_MAXBUFFER);

          br = diostream->Read(buffer, DIOWEBCLIENT_MAXBUFFER);
          if(br)
            {
              timerout->Reset();

              //webpage.Add(buffer, br);
              webpage.Write(buffer, br);

              //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Block read %d [size: %d]"), br, webpage.GetSize());

              xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_READBODYBLOCK);
              xevent.SetDownloadSize(br);
              PostEvent(&xevent);
            }
           else
            {
              if(timerout->GetMeasureSeconds()>(XDWORD)timeout)
                {
                  //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Error! time out! "));
                  status = false;
                  break;
                }

              if(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED)
                {
                  if(!diostream->GetInXBuffer()->GetSize())
                    {
                      //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Connexion close! "));
                      break;
                    }
                }
            }


        } while(1);

      delete [] buffer;
    }

  //--- Close ----------------------------------

  diostream->Close();

  xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_CLOSEWEB);
  PostEvent(&xevent);


  return status;

}




/*-------------------------------------------------------------------
//  DIOWEBCLIENT::MakeOperationFile
*/
/**
//
//
//
//  @author       Imanol Celaya Ruiz de Alegria
//  @version      26/10/2016 12:48:07
//
//  @return       bool :
//
//  @param        url :
//  @param        webpage :
//  @param        postdata :
//  @param        addheader :
//  @param        timeout :
//  @param        localIP :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBCLIENT::MakeOperationFile(DIOURL& url, XBUFFER& webpage, XBUFFER* boundary, XFILE* file, XCHAR* addheader, int timeout, XSTRING* localIP)
{
  if(!diostreamcfg)     return false;
  if(!diostream)        return false;

  if(!timerout)         return false;

  bool status = true;

  if(!url.HaveHTTPID()) url.AddHTTPID();

  XSTRING             server;
  XSTRING             login;
  XSTRING             password;
  XSTRING             loginpassword;
  XSTRING             loginpasswordbase64;
  DIOURL              resource;
  DIOWEBCLIENTXEVENT  xevent(this, XEVENTTYPE_NONE);


  url.GetHTTPServer(server, login, password);
  url.GetHTTPResource(resource);

  loginpassword.Format(__L("%s:%s"), login.Get(), password.Get());
  loginpassword.ConvertToBase64(loginpasswordbase64);

  if(resource.IsEmpty()) resource.Slash_Add();

  diostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);
  if(localIP)
    {
      if(!localIP->IsEmpty()) diostreamcfg->GetLocalIP()->Set(localIP->Get());
    }

  if(proxyurl.IsEmpty())
    {
      diostreamcfg->GetRemoteURL()->Set(server.Get());
      diostreamcfg->SetRemotePort(port);
    }
   else
    {
      diostreamcfg->GetRemoteURL()->Set(proxyurl.Get());
      diostreamcfg->SetRemotePort(proxyport);
    }

  //--- Connexion WEB server -------------------

  if(!diostream->Open())
    {
      diostream->Close();
      return false;
    }

  if(!diostream->WaitToConnected(timeout))
    {
      diostream->Close();
      return false;
    }

  xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_OPENWEB);
  PostEvent(&xevent);

  //--- Send Header ----------------------------

  XSTRING sendheader;

  //resource.EncodeUnsafeChars();


  sendheader.Format(__L("%s %s HTTP/1.0\r\n") , file?__L("POST"):__L("GET"), resource.Get());

  sendheader += __L("Host: ");
  sendheader += server.Get();

  if(port!=DIOWEBCLIENT_DEFAULTPORT)
    {
      XSTRING stringport;

      stringport.ConvertFromInt(port);

      sendheader += __L(":");
      sendheader += stringport.Get();
    }
  sendheader += __L("\r\n");

  if(addheader) sendheader += addheader;

  XSTRING toaddheader;
  XSTRING bound;
  bound.Set(*boundary);
  toaddheader.Format(__L("Content-Type: multipart/form-data; boundary=------------------------%s\n"), bound.Get());

  sendheader += toaddheader;

  if(login.GetSize() && password.GetSize())
    {
      sendheader += __L("Authorization: Basic ");
      sendheader += loginpasswordbase64;
      sendheader += __L("\r\n");
    }

  // need to compute the total length of the body in here, before sending the http header


  XSTRING top;
  XSTRING fileline;
  fileline.Format(__L("Content-Disposition: form-data; name=\"upload\"; filename=\"%s\"\r\n"), file->GetPathNameFile());
  XSTRING boundtop;
  boundtop.Format(__L("--------------------------%s\r\n"), bound.Get());
  top += boundtop;
  top.Add(fileline.Get());
  top.Add("Content-Type: application/octet-stream\r\n");
  top.Add("\r\n");

  XSTRING bottom;
  bottom.Add("\r\n");
  XSTRING boundbottom;
  boundbottom.Format(__L("--------------------------%s--\r\n"), bound.Get());
  bottom += boundbottom;

  XBUFFER topbuffer(false);
  XBUFFER bottombuffer(false);

  XSTRING_CREATEOEM(top, charstr0)
  topbuffer.Add((XBYTE*)charstr0, top.GetSize());
  XSTRING_DELETEOEM(charstr0)

  XSTRING_CREATEOEM(bottom, charstr1)
  bottombuffer.Add((XBYTE*)charstr1, bottom.GetSize());
  XSTRING_DELETEOEM(charstr1)

  if(file)
    {
      XSTRING stringlenght;
      stringlenght.Format(__L("Content-Length: %d\r\n"), file->GetSize() + topbuffer.GetSize() + bottombuffer.GetSize()); // !! WRONG !!

      sendheader += stringlenght;
    }

  sendheader += __L("Connection: close\r\n");

  sendheader += __L("\r\n");

  //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB Client Header %s"), sendheader.Get());

  diostream->WriteStr(sendheader);

  xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_WRITEHEADER);
  PostEvent(&xevent);

  //--- Send Post data -------------------------

  // need a loop to send the file in chunks
  XBYTE* localbuffer = new XBYTE[1024*1024];
  XDWORD totalread = 0;
  if(file)
    {

      diostream->Write(topbuffer);
      //diostream->Write(file->Get(), file->GetSize());
      // now send the file
      while(totalread < file->GetSize())
        {
          XDWORD size = 1024*1024;

          file->Read(localbuffer, &size);

          totalread += size;

          diostream->Write(localbuffer, size);
          diostream->WaitToFlushOutXBuffer(timeout); // maybe
        }

      diostream->Write(bottombuffer);

      xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_SENDPOSTDATA);
      PostEvent(&xevent);
    }

  delete [] localbuffer;

  diostream->WaitToFlushOutXBuffer(timeout);

  //--- Read Header ----------------------------

  if(!header.Read(diostream, timerout, timeout))
    {
      //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Error! leer header! "));

      diostream->Close();
      return false;
    }

  //--- Read Content ---------------------------

  if((header.GetResultServer()>=400)||(!header.GetResultServer()))
    {
      //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Error! leer header2! "));

      xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_HEADERERROR);
      PostEvent(&xevent);

      diostream->Close();
      return false;
    }

  xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_READHEADER);
  PostEvent(&xevent);

  //--- Read Content ---------------------------

  webpage.Delete();

  XBYTE* buffer = new XBYTE[DIOWEBCLIENT_MAXBUFFER];
  if(buffer)
    {
      int br;

      timerout->Reset();

      do{ memset(buffer, 0,  DIOWEBCLIENT_MAXBUFFER);

          br = diostream->Read(buffer, DIOWEBCLIENT_MAXBUFFER);
          if(br)
            {
              timerout->Reset();

              webpage.Add(buffer, br);

              //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Block read %d [size: %d]"), br, webpage.GetSize());

              xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_READBODYBLOCK);
              xevent.SetDownloadSize(br);
              PostEvent(&xevent);
            }
           else
            {
              if(timerout->GetMeasureSeconds()>(XDWORD)timeout)
                {
                  //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Error! time out! "));
                  status = false;
                  break;
                }

              if(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED)
                {
                  if(!diostream->GetInXBuffer()->GetSize())
                    {
                      //XDEBUGTRACE_PRINTCOLOR(2, __L("WEB_CLIENT: Connexion close! "));
                      break;
                    }
                }
            }


        } while(1);

      delete [] buffer;
    }

  //--- Close ----------------------------------

  diostream->Close();

  xevent.SetEventType(DIOWEBCLIENTXEVENTTYPE_CLOSEWEB);
  PostEvent(&xevent);

  return status;
}

