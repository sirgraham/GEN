
/*------------------------------------------------------------------------------------------
//  DIOALERTS.CPP
//
//  Data IO Alerts
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 16/01/2015 10:31:58
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#ifdef DIOALERTS_ACTIVE

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XDateTime.h"
#include "XFileTXT.h"
#include "XRand.h"
#include "XThreadCollected.h"

#include "HashCRC32.h"

#include "DIOURL.h"
#include "DIOIP.h"
#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamUDPConfig.h"
#include "DIOStreamUDPAcknowledge.h"
#include "DIOSMTP.h"
#include "DIOATCMDGSM.h"
#include "DIOWebClient.h"

#include "DIOAlerts.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

DIOALERTS* DIOALERTS::instance = NULL;

/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOALERT::CalculateID
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/01/2015 12:43:30
//
//  @return       XDWORD :
//
//  @param        withdatetime :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOALERT::CalculateID(bool withdatetime)
{
  if(!xdatetime)  return false;

  HASHCRC32* hashcrc32 = new HASHCRC32();
  if(!hashcrc32) return 0;

  XSTRING IDstring;
  XSTRING levelstring;
  XDWORD  ID = 0;
  bool    status;

  if(withdatetime) xdatetime->GetDateTimeToString(XDATETIME_FORMAT_STANDARD, IDstring);

  levelstring.Format(__L("%s %08X %d"), applicationID.Get(), type, level);
  IDstring += levelstring;

  XRAND* xrand = xfactory->CreateRand();
  if(xrand)
    {
      levelstring.Format(__L(" %d "), xrand->Between(1000, 10000000) * xrand->Between(1000, 10000000));
      xfactory->DeleteRand(xrand);
    }

  IDstring += __L(" ");   IDstring += origin;
  IDstring += __L(" ");   IDstring += title;
  IDstring += __L(" ");   IDstring += message;
  IDstring += levelstring;

  XSTRING_CREATEOEM(IDstring, charstr)
  status = hashcrc32->Do((XBYTE*)charstr, IDstring.GetSize());
  XSTRING_DELETEOEM(charstr)

  if(status) ID = hashcrc32->GetResultCRC32(); else ID = 0;

  delete hashcrc32;

  return ID;
}





/*-------------------------------------------------------------------
//  DIOALERTS::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/01/2015 13:14:37
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOALERTS::Ini()
{
  End();

  //-------------------------

  SMTPdiostreamcfg = new DIOSTREAMTCPIPCONFIG();
  if(!SMTPdiostreamcfg)  return false;

  SMTPdiostream = (DIOSTREAM*)diofactory->CreateStreamIO((DIOSTREAMCONFIG*)SMTPdiostreamcfg);
  if(!SMTPdiostream)  return false;

  SMTP = new DIOSMTP( SMTPdiostream);
  if(!SMTP) return false;

  if(!SMTP->Ini()) return false;

  //-------------------------

  WEBdiowebclient = new DIOWEBCLIENT();
  if(!WEBdiowebclient) return false;

  //-------------------------

  UDPdiostreamcfg = new DIOSTREAMUDPCONFIG();
  if(!UDPdiostreamcfg)  return false;

  UDPdiostream = new DIOSTREAMUDPACKNOWLEDGE((DIOSTREAMUDPCONFIG*)UDPdiostreamcfg);
  if(!UDPdiostream)  return false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOALERTS::Create
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/01/2015 12:29:44
//
//  @return       DIOALERT* :
//
//  @param        applicationID :
//  @param        type :
//  @param        level :
//  @param        origin :
//  @param        title :
//  @param        message :
*/
/*-----------------------------------------------------------------*/
DIOALERT* DIOALERTS::CreateAlert(XCHAR* applicationID, XDWORD type, DIOALERTLEVEL level, XCHAR* origin, XCHAR* title, XCHAR* message)
{
  if(level == DIOALERTLEVEL_UNKNOWN) return NULL;

  if(!origin)   return NULL;
  if(!title)    return NULL;
  if(!message)  return NULL;

  DIOALERT* alert = new DIOALERT();
  if(alert)
    {
      if(alert->GetDateTime()) alert->GetDateTime()->Read();

      alert->GetApplicationID()->Set(applicationID);
      alert->SetApplicationVersion(applicationversion, applicationsubversion, applicationsubversionerr);
      alert->SetType(type);
      alert->SetLevel(level);
      alert->GetOrigin()->Set(origin);
      alert->GetTitle()->Set(title);
      alert->Get_Message()->Set(message);

      XDWORD ID = alert->CalculateID(true);
      if(!ID)
        {
          delete alert;
          return NULL;
        }

      alert->SetID(ID);
    }

  return alert;
}




/*-------------------------------------------------------------------
//  DIOALERTS::Sender_SMTPConfig
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/01/2015 17:51:45
//
//  @return       bool :
//
//  @param        URL :
//  @param        port :
//  @param        login :
//  @param        password :
//  @param        senderemail :
//  @param        nrecipients :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool DIOALERTS::Sender_SMTPConfig(XCHAR* URL, int port, XCHAR* login, XCHAR* password, XCHAR* senderemail, int nrecipients,...)
{
  if(!SMTP) return false;

  SMTPsenderisactive = false;

  SMTP->Server_GetURL()->Set(URL);
  SMTP->Server_SetPort(port);
  SMTP->Server_GetLogin()->Set(login);
  SMTP->Server_GetPassword()->Set(password);
  SMTP->GetSenderEmail()->Set(senderemail);
  SMTP->SetContentType(DIOSSMPTCONTENTTYPE_UTF8);

  va_list  arg;

  va_start(arg, nrecipients);

  int nr = nrecipients;

  while(nr)
    {
      XCHAR* recipient = (XCHAR*)va_arg(arg, XCHAR*);
      if(!recipient) break;

      DIOSMTPRECIPIENTTYPE  type        = DIOSMTPRECIPIENTTYPE_UNKNOWN;
      int                   test        = 0;
      XSTRING               recipientstring;
      XSTRING               name;
      XSTRING               email;

      recipientstring = recipient;

      name.AdjustSize(_MAXSTR);
      email.AdjustSize(_MAXSTR);

      recipientstring.UnFormat(__L("%d,%s,%s,%d"), &type, name.Get(), email.Get(), &test);

      name.AdjustSize();
      email.AdjustSize();

      if((type == DIOSMTPRECIPIENTTYPE_TO) || (type == DIOSMTPRECIPIENTTYPE_CC) || (type == DIOSMTPRECIPIENTTYPE_BCC))
        {
          SMTP->AddRecipient(type, name , email, test?true:false);
        }

      nr--;
    }

  va_end(arg);

  SMTPsenderisactive = true;

  return SMTPsenderisactive;
}





/*-------------------------------------------------------------------
//  DIOALERTS::Sender_SMTPSend
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/01/2015 17:07:35
//
//  @return       bool :
//
//  @param        alert :
*/
/*-----------------------------------------------------------------*/
bool DIOALERTS::Sender_SMTPSend(DIOALERT* alert)
{
  if(!SMTP)   return false;
  if(!alert)  return false;

  SMTP->GetMessage()->DeleteAllLines();

  XSTRING appIDstring;
  XSTRING appverstring;

  SMTP->GetSenderName()->Set(alert->GetApplicationID()->Get());

  if(applicationversion || applicationsubversion  || applicationsubversionerr) appverstring.Format(__L(" %d.%d.%d"), applicationversion, applicationsubversion, applicationsubversionerr);

  appIDstring = alert->GetApplicationID()->Get();
  if(!appverstring.IsEmpty()) appIDstring += appverstring.Get();

  appIDstring += __L(": ");
  appIDstring += alert->GetTitle()->Get();

  SMTP->GetSubject()->Set(appIDstring);

  XSTRING body;

  body += alert->GetOrigin()->Get();
  body += __L("\r\n\r\n");
  body += alert->Get_Message()->Get();

  SMTP->GetMessage()->AddLine(body);

  SMTP->SetXPriority(DIOSMTPXPRIORITY_HIGH);

  return SMTP->Send();
}




/*-------------------------------------------------------------------
//  DIOALERTS::Sender_SMSConfig
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      20/01/2015 8:59:45
//
//  @return       bool :
//
//  @param        diostream :
//  @param        nrecipients :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool DIOALERTS::Sender_SMSConfig(DIOSTREAM* diostream, int nrecipients, ...)
{
  if(!diostream) return false;

  SMSsenderisactive = false;
  SMSdiostream      = diostream;

  SMSrecipients.DeleteContents();
  SMSrecipients.DeleteAll();

  va_list  arg;

  va_start(arg, nrecipients);

  int nr = nrecipients;

  while(nr)
    {
      XSTRING* recipient = new XSTRING();
      if(recipient)
        {
          (*recipient) = (XCHAR*)va_arg(arg, XCHAR*);
          SMSrecipients.Add(recipient);
        }

      nr--;
    }

  va_end(arg);

  if(SMSrecipients.GetSize()) SMSsenderisactive = true;

  return SMSsenderisactive;
}





/*-------------------------------------------------------------------
//  DIOALERTS::Sender_SMSSend
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/01/2015 19:48:34
//
//  @return       bool :
//
//  @param        alert :
*/
/*-----------------------------------------------------------------*/
bool DIOALERTS::Sender_SMSSend(DIOALERT* alert)
{
  if(!alert) return false;

  bool status = false;

  DIOATCMDGSM* SMSdioatcmdgsm = new DIOATCMDGSM( SMSdiostream);
  if(SMSdioatcmdgsm)
    {
      if(SMSdioatcmdgsm->Ini(30, false, false))
        {
          XSTRING smstext;

          if(alert->GetApplicationID()->GetSize())
            {
              XSTRING appverstring;

              if(applicationversion || applicationsubversion  || applicationsubversionerr) appverstring.Format(__L(" %d.%d.%d"), applicationversion, applicationsubversion, applicationsubversionerr);

              smstext += __L("<");
              smstext += alert->GetApplicationID()->Get();

              if(!appverstring.IsEmpty()) smstext += appverstring.Get();

              smstext += __L("> ");
            }


          if(alert->GetTitle()->GetSize())
            {
              smstext += alert->GetTitle()->Get();
              smstext += __L(" ");
            }


          if(alert->GetOrigin()->GetSize())
            {
              smstext += __L("(");
              smstext += alert->GetOrigin()->Get();
              smstext += __L(")");
            }

          if(alert->Get_Message()->GetSize())
            {
              smstext += __L(":");
              smstext += alert->Get_Message()->Get();
            }

          if(smstext.GetSize()>=160) smstext.AdjustSize(160);

          status = true;

          for(int c=0; c<(int)SMSrecipients.GetSize(); c++)
            {
              if(SMSdioatcmdgsm->SendSMS(SMSrecipients.Get(c)->Get(), smstext.Get()) != DIOATCMD_ERROR_NONE)  status = false;
            }

          SMSdioatcmdgsm->End();
        }

      delete SMSdioatcmdgsm;
    }

  return status;
}



/*-------------------------------------------------------------------
//  DIOALERTS::Sender_WEBConfig
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/07/2016 12:39:44
//
//  @return       bool :
//
//  @param        command :
//  @param        isuseget :
//  @param        nrecipients :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool DIOALERTS::Sender_WEBConfig(XCHAR* command, bool isuseget, int nrecipients, ...)
{
  if(!WEBdiowebclient) return false;

  bool serveractive = false;

  WEBcommand  = command;
  WEBisuseget = isuseget;

  WEBsenderisactive = false;

  WEBrecipients.DeleteContents();
  WEBrecipients.DeleteAll();

  va_list  arg;
  bool     status = false;

  va_start(arg, nrecipients);

  int nr = nrecipients;

  while(nr)
    {
      DIOURL* recipient = diofactory->CreateURL();
      if(recipient)
        {
          (*recipient) = (XCHAR*)va_arg(arg, XCHAR*);

          if(recipient->HaveHTTPID()) recipient->DeleteHTTPID();

          WEBrecipients.Add(recipient);
        }

      nr--;
    }

  va_end(arg);

  if(WEBrecipients.GetSize())
    {
      WEBsenderisactive = true;

      XBUFFER webpage;

      for(int c=0; c<nrecipients; c++)
        {
          XSTRING url;

          url = WEBrecipients.Get(c)->Get();
          url.AddFormat(__L("/?%s") , WEBcommand.Get());

          status = WEBdiowebclient->Get(url.Get(), webpage);
          if(!status)
            {
              break;
            }
           else serveractive = true;
        }
    }

  return serveractive;
}




/*-------------------------------------------------------------------
//  DIOALERTS::Sender_WEBSend
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      16/07/2016 19:58:18
//
//  @return       bool :
//
//  @param        alert :
*/
/*-----------------------------------------------------------------*/
bool DIOALERTS::Sender_WEBSend(DIOALERT* alert)
{
  if(WEBrecipients.IsEmpty()) return false;

  DIOURL    urlall;
  XBUFFER   webpage;
  bool      status  = true;
  XSTRING   applicationIDstring;
  XSTRING   applicationverstring;

  if(applicationversion || applicationsubversion  || applicationsubversionerr) applicationverstring.Format(__L(" %d.%d.%d"), applicationversion, applicationsubversion, applicationsubversionerr);

  applicationIDstring = alert->GetApplicationID()->Get();
  if(!applicationverstring.IsEmpty()) applicationIDstring += applicationverstring.Get();

  for(int c=0; c<(int)WEBrecipients.GetSize(); c++)
    {
      DIOURL    part;
      XBUFFER   webpage;

      urlall = WEBrecipients.Get(c)->Get();

      if(WEBisuseget)
        {
          urlall.Slash_Delete();

          urlall.AddFormat(__L("/?%s") , WEBcommand.Get());

          urlall.AddFormat(__L("&%s="), DIOALERTS_QSPARAM_APPLICATIONID);  part = applicationIDstring.Get();            part.EncodeUnsafeChars();    urlall.Add(part);
          urlall.AddFormat(__L("&%s="), DIOALERTS_QSPARAM_TYPE);           part.Format(__L("%d"), alert->GetType());    part.EncodeUnsafeChars();    urlall.Add(part);
          urlall.AddFormat(__L("&%s="), DIOALERTS_QSPARAM_LEVEL);          part.Format(__L("%d"), alert->GetLevel());   part.EncodeUnsafeChars();    urlall.Add(part);
          urlall.AddFormat(__L("&%s="), DIOALERTS_QSPARAM_TITTLE);         part = alert->GetTitle()->Get();             part.EncodeUnsafeChars();    urlall.Add(part);
          urlall.AddFormat(__L("&%s="), DIOALERTS_QSPARAM_ORIGIN);         part = alert->GetOrigin()->Get();            part.EncodeUnsafeChars();    urlall.Add(part);
          urlall.AddFormat(__L("&%s="), DIOALERTS_QSPARAM_MESSAGE);        part = alert->Get_Message()->Get();          part.EncodeUnsafeChars();    urlall.Add(part);
        }
       else
        {
          XSTRING contents;

          contents.AddFormat(__L("\n%s"), WEBcommand.Get());
          contents.Add(__L("\n%s=") , DIOALERTS_QSPARAM_APPLICATIONID);   contents.Add(applicationIDstring.Get());
          contents.Add(__L("\n%s=") , DIOALERTS_QSPARAM_TITTLE);          contents.Add(alert->GetTitle()->Get());
          contents.Add(__L("\n%s=") , DIOALERTS_QSPARAM_ORIGIN);          contents.Add(alert->GetOrigin()->Get());
          contents.Add(__L("\n%s=") , DIOALERTS_QSPARAM_MESSAGE);         contents.Add(alert->Get_Message()->Get());

          webpage.Add(contents);
        }

      /*
      if(!WEBdiowebclient->Get(urlall.Get(), webpage))
        {
          status = false;
        }
      */
    }

  return status;
}




/*-------------------------------------------------------------------
//  DIOALERTS::Sender_UDPConfig
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/01/2015 9:43:53
//
//  @return       bool :
//
//  @param        port :
//  @param        nrecipients :
//  @param        ... :
*/
/*-----------------------------------------------------------------*/
bool DIOALERTS::Sender_UDPConfig(int port, int nrecipients, ...)
{
  if(!port) return false;

  UDPsenderisactive = false;

  UDPrecipients.DeleteContents();
  UDPrecipients.DeleteAll();

  DIOURL* URL = diofactory->CreateURL();
  if(!URL) return false;

  va_list  arg;

  va_start(arg, nrecipients);

  int nr = nrecipients;

  while(nr)
    {
      (*URL) = (XCHAR*)va_arg(arg, XCHAR*);

      if(!URL->IsEmpty())
        {
          DIOIP* IP = new DIOIP();
          if(IP)
            {
              URL->ResolveURL((*IP));
              UDPrecipients.Add(IP);
            }
        }

      nr--;
    }

  va_end(arg);

  if(URL) diofactory->DeleteURL(URL);

  if(UDPrecipients.GetSize())
    {
      UDPsenderisactive = true;
      UDPdiostreamcfg->SetRemotePort(port);
    }

  return UDPsenderisactive;
}




/*-------------------------------------------------------------------
//  DIOALERTS::Sender_UDPSend
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/01/2015 9:24:23
//
//  @return       bool :
//
//  @param        alert :
*/
/*-----------------------------------------------------------------*/
bool DIOALERTS::Sender_UDPSend(DIOALERT* alert)
{
  if(!alert)            return false;
  if(!UDPdiostreamcfg)  return false;
  if(!UDPdiostream)     return false;

  XBUFFER     xbuffer;
  HASHCRC32   hashcrc32;
  XDWORD      CRC32;

  xbuffer.Delete();

  // Date time
  xbuffer.Add((XDWORD)alert->GetDateTime()->GetDay());                                                              // Day
  xbuffer.Add((XDWORD)alert->GetDateTime()->GetMonth());                                                            // Month
  xbuffer.Add((XDWORD)alert->GetDateTime()->GetYear());                                                             // Year
  xbuffer.Add((XDWORD)alert->GetDateTime()->GetHours());                                                            // Hours
  xbuffer.Add((XDWORD)alert->GetDateTime()->GetMinutes());                                                          // Minutes
  xbuffer.Add((XDWORD)alert->GetDateTime()->GetSeconds());                                                          // Seconds

  xbuffer.Add((XDWORD)alert->GetApplicationID()->GetSize());    xbuffer.Add((*alert->GetApplicationID()));          // Aplication String
  xbuffer.Add((XDWORD)applicationversion);                                                                          // Aplicatcion version
  xbuffer.Add((XDWORD)applicationsubversion);                                                                       // Aplication  subversion
  xbuffer.Add((XDWORD)applicationsubversionerr);                                                                    // Application subversion err

  xbuffer.Add((XDWORD)alert->GetID());                                                                              // ID
  xbuffer.Add((XDWORD)alert->GetType());                                                                            // Type
  xbuffer.Add((XDWORD)alert->GetLevel());                                                                           // Level
  xbuffer.Add((XDWORD)alert->GetOrigin()->GetSize());           xbuffer.Add((*alert->GetOrigin()));                 // Origin
  xbuffer.Add((XDWORD)alert->GetTitle()->GetSize());            xbuffer.Add((*alert->GetTitle()));                  // Title
  xbuffer.Add((XDWORD)alert->Get_Message()->GetSize());         xbuffer.Add((*alert->Get_Message()));               // Message

  hashcrc32.ResetResult();
  hashcrc32.Do(xbuffer);

  CRC32 = hashcrc32.GetResultCRC32();

  xbuffer.Add((XDWORD)CRC32);

  bool status;
  int  nsent = 0;

  for(int c=0; c<(int)UDPrecipients.GetSize(); c++)
    {
      DIOIP* ip = UDPrecipients.Get(c);
      if(ip)
        {
          XSTRING IPstring;

          ip->GetXString(IPstring);

          UDPdiostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);
          UDPdiostreamcfg->GetRemoteURL()->Set(IPstring.Get());
          UDPdiostreamcfg->SetIsUsedDatagrams(true);

          status = UDPdiostream->Open();
          if(status)
            {
              status = UDPdiostream->Write(xbuffer.Get(), xbuffer.GetSize())?true:false;
              if(status) nsent++;
              UDPdiostream->Close();
            }
        }
    }

  return (UDPrecipients.GetSize() == nsent)?true:false;
}






/*-------------------------------------------------------------------
//  DIOALERTS::Send
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/01/2015 2:13:19
//
//  @return       bool :
//
//  @param        sender :
//  @param        alert :
*/
/*-----------------------------------------------------------------*/
int DIOALERTS::Send(DIOALERTSENDER sender, DIOALERT* alert)
{
  int statussend = 0;

  if((sender & DIOALERTSSENDER_SMPT) && (SMTPsenderisactive)) statussend |= Sender_SMTPSend(alert)? DIOALERTSSENDER_SMPT: 0;
  if((sender & DIOALERTSSENDER_SMS)  && (SMSsenderisactive))  statussend |= Sender_SMSSend(alert) ? DIOALERTSSENDER_SMS : 0;
  if((sender & DIOALERTSSENDER_WEB)  && (WEBsenderisactive))  statussend |= Sender_WEBSend(alert) ? DIOALERTSSENDER_WEB : 0;
  if((sender & DIOALERTSSENDER_UDP)  && (UDPsenderisactive))  statussend |= Sender_UDPSend(alert) ? DIOALERTSSENDER_UDP : 0;

  return statussend;
}




/*-------------------------------------------------------------------
//  DIOALERTS::End
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/01/2015 13:15:31
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOALERTS::End()
{
  //------------------------------------------

  if(SMTP)
    {
      SMTP->End();
      delete SMTP;

      SMTP = NULL;
    }

  if(SMTPdiostream)
    {
      SMTPdiostream->Close();
      diofactory->DeleteStreamIO(SMTPdiostream);

      SMTPdiostream = NULL;
    }

  if(SMTPdiostreamcfg)
    {
      delete SMTPdiostreamcfg;

      SMTPdiostreamcfg = NULL;
    }

  SMTPsenderisactive = false;

  //------------------------------------------

  SMSrecipients.DeleteContents();
  SMSrecipients.DeleteAll();

  SMSsenderisactive = false;

  //------------------------------------------

  WEBrecipients.DeleteContents();
  WEBrecipients.DeleteAll();

  if(WEBdiowebclient)
    {
      delete WEBdiowebclient;
      WEBdiowebclient = NULL;
    }

  WEBsenderisactive = false;

  //------------------------------------------

  UDPrecipients.DeleteContents();
  UDPrecipients.DeleteAll();

  if(UDPdiostream)
    {
      UDPdiostream->Close();
      delete UDPdiostream;
      UDPdiostream = NULL;
    }

  if(UDPdiostreamcfg)
    {
      delete UDPdiostreamcfg;
      UDPdiostreamcfg = NULL;
    }

  UDPsenderisactive = false;

  return true;
}





/*-------------------------------------------------------------------
//  DIOALERTSUDPSERVER::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/05/2015 23:58:47
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOALERTSUDPSERVER::Ini(XCHAR* URL, int port)
{
  bool status = false;

  UDPdiostreamcfg = new DIOSTREAMUDPCONFIG();
  if(!UDPdiostreamcfg)  return false;

  UDPdiostreamcfg->SetMode(DIOSTREAMMODE_SERVER);
  UDPdiostreamcfg->GetRemoteURL()->Set(URL);
  UDPdiostreamcfg->SetRemotePort(port);
  UDPdiostreamcfg->SetIsUsedDatagrams(true);

  UDPdiostream = new DIOSTREAMUDPACKNOWLEDGE(UDPdiostreamcfg);
  if(UDPdiostream)
    {
      status = UDPdiostream->Open();
      if(status)
        {
          xmutexalert = xfactory->Create_Mutex();
          if(xmutexalert)
            {
              threadread = CREATEXTHREAD(XTHREADGROUPID_DIOALERTS, __L("DIOALERTSUDPSERVER::Ini"), ThreadReadFunction, (void*)this);
              if(threadread)  status = threadread->Ini();
            }
        }
    }

  return status;
}





/*-------------------------------------------------------------------
//  DIOALERTSUDPSERVER::End
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/05/2015 23:58:53
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOALERTSUDPSERVER::End()
{
  if(threadread)
    {
      threadread->End();

      DELETEXTHREAD(XTHREADGROUPID_DIOALERTS, threadread);
      threadread = NULL;
    }

  DeleteAllAlerts();

  if(UDPdiostream)
    {
      UDPdiostream->Close();
      delete UDPdiostream;
      UDPdiostream = NULL;
    }

  if(UDPdiostreamcfg)
    {
      delete UDPdiostreamcfg;
      UDPdiostreamcfg = NULL;
    }

  if(xmutexalert)
    {
      xfactory->Delete_Mutex(xmutexalert);
      xmutexalert = NULL;
    }

  return true;
}




/*-------------------------------------------------------------------
//  DIOALERTSUDPSERVER::GetNAlerts
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/05/2015 10:19:28
//
//  @return       int :
//
*/
/*-----------------------------------------------------------------*/
int DIOALERTSUDPSERVER::GetNAlerts()
{
  if(xmutexalert) xmutexalert->Lock();

  int nalerts = alerts.GetSize();

  if(xmutexalert) xmutexalert->UnLock();

  return nalerts;
}




/*-------------------------------------------------------------------
//  DIOALERTSUDPSERVER::GetAlertByIndex
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/05/2015 9:51:29
//
//  @return       DIOALERT* :
//
//  @param        index :
*/
/*-----------------------------------------------------------------*/
DIOALERT* DIOALERTSUDPSERVER::GetAlertByIndex(int index)
{
  if(xmutexalert) xmutexalert->Lock();

  DIOALERT* alert = alerts.Get(index);

  if(xmutexalert) xmutexalert->UnLock();

  return alert;
}




/*-------------------------------------------------------------------
//  DIOALERTSUDPSERVER::GetAlertByID
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/05/2015 9:49:20
//
//  @return       bool :
//
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
DIOALERT* DIOALERTSUDPSERVER::GetAlertByID(XDWORD ID)
{
  DIOALERT* alert = NULL;

  if(xmutexalert) xmutexalert->Lock();

  for(int c=0; c<(int)alerts.GetSize(); c++)
    {
      alert  = alerts.Get(c);
      if(alert)
        {
          if(alert->GetID() == ID)  break;
        }
    }

  if(xmutexalert) xmutexalert->UnLock();

  return alert;
}




/*-------------------------------------------------------------------
//  DIOALERTSUDPSERVER::DeleteAlertByIndex
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/05/2015 9:50:13
//
//  @return       bool :
//
//  @param        index :
*/
/*-----------------------------------------------------------------*/
bool DIOALERTSUDPSERVER::DeleteAlertByIndex(int index)
{
  DIOALERT* alert = alerts.Get(index);
  if(alert)
    {
      if(xmutexalert) xmutexalert->Lock();

      alerts.Delete(alert);
      delete alert;

      if(xmutexalert) xmutexalert->UnLock();

      return true;
    }

  return false;
}




/*-------------------------------------------------------------------
//  DIOALERTSUDPSERVER::DeleteAlertByID
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      15/05/2015 9:50:17
//
//  @return       bool :
//
//  @param        ID :
*/
/*-----------------------------------------------------------------*/
bool DIOALERTSUDPSERVER::DeleteAlertByID(XDWORD ID)
{
  DIOALERT* alert  = NULL;
  bool      status = false;

  if(xmutexalert) xmutexalert->Lock();

  for(int c=0; c<(int)alerts.GetSize(); c++)
    {
      alert  = alerts.Get(c);
      if(alert)
        {
          if(alert->GetID() == ID)
            {
              alerts.Delete(alert);
              delete alert;

              status = true;
            }
        }
    }

  if(xmutexalert) xmutexalert->UnLock();

  return status;
}




/*-------------------------------------------------------------------
//  DIOALERTSUDPSERVER::DeleteAllAlerts
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      14/05/2015 19:45:06
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOALERTSUDPSERVER::DeleteAllAlerts()
{
  if(alerts.IsEmpty()) return false;

  if(xmutexalert) xmutexalert->Lock();

  alerts.DeleteContents();
  alerts.DeleteAll();

  if(xmutexalert) xmutexalert->UnLock();

  return true;
}




/*-------------------------------------------------------------------
//  DIOALERTSUDPSERVER::ReceivedEvents
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      13/05/2015 0:23:15
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOALERTSUDPSERVER::ReceivedEvents()
{
  if(!UDPdiostream) return false;

  XBUFFER  data;
  XSTRING  address;
  XWORD    port;
  bool     status = false;

  if(UDPdiostream->ReadDatagram(address, port, data))
    {
      HASHCRC32   hashcrc32;
      XDWORD      CRC32;
      DIOALERT*   alert  = NULL;

      if(data.GetSize())
        {
          alert = new DIOALERT();
          if(alert)
            {
              XSTRING string;
              XDWORD  dataxdword;
              XDWORD  sizestring;
              XDWORD  applicationversion;
              XDWORD  applicationsubversion;
              XDWORD  applicationsubversionerr;

              data.Extract(CRC32, (data.GetSize()-sizeof(XDWORD)));

              hashcrc32.ResetResult();
              hashcrc32.Do(data);

              if(CRC32 == hashcrc32.GetResultCRC32())
                {
                  // Date time
                  data.Extract(dataxdword);   alert->GetDateTime()->SetDay(dataxdword);                                           // Day
                  data.Extract(dataxdword);   alert->GetDateTime()->SetMonth(dataxdword);                                         // Month
                  data.Extract(dataxdword);   alert->GetDateTime()->SetYear(dataxdword);                                          // Year
                  data.Extract(dataxdword);   alert->GetDateTime()->SetHours(dataxdword);                                         // Hours
                  data.Extract(dataxdword);   alert->GetDateTime()->SetMinutes(dataxdword);                                       // Minutes
                  data.Extract(dataxdword);   alert->GetDateTime()->SetSeconds(dataxdword);                                       // Seconds

                  data.Extract(sizestring);  data.Extract(string, 0, sizestring);   alert->GetApplicationID()->Set(string);       // Aplication String
                  data.Extract(applicationversion);
                  data.Extract(applicationsubversion);
                  data.Extract(applicationsubversionerr);

                  alert->SetApplicationVersion(applicationversion, applicationsubversion, applicationsubversionerr);

                  data.Extract(dataxdword);                                         alert->SetID(dataxdword);                     // ID
                  data.Extract(dataxdword);                                         alert->SetType(dataxdword);                   // Type
                  data.Extract(dataxdword);                                         alert->SetLevel((DIOALERTLEVEL)dataxdword);   // Level
                  data.Extract(sizestring);  data.Extract(string, 0, sizestring);   alert->GetOrigin()->Set(string);              // Origin
                  data.Extract(sizestring);  data.Extract(string, 0, sizestring);   alert->GetTitle()->Set(string);               // Title
                  data.Extract(sizestring);  data.Extract(string, 0, sizestring);   alert->Get_Message()->Set(string);            // Message

                  status = alerts.Add(alert);
                }

              if(!status) delete alert;
            }
        }
    }

  return status;
 }





/*-------------------------------------------------------------------
//  DIOALERTSUDPSERVER::ThreadReadFunction
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      13/05/2015 0:40:47
//
//  @param        param :
*/
/*-----------------------------------------------------------------*/
void DIOALERTSUDPSERVER::ThreadReadFunction(void* param)
{
  DIOALERTSUDPSERVER* alertUDPserver = ( DIOALERTSUDPSERVER*)param;
  if(!alertUDPserver) return;

  alertUDPserver->ReceivedEvents();
}



#endif
