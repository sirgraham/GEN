
/*------------------------------------------------------------------------------------------
//  DIOSMTP.CPP
//
//  Data IO SMTP (Simple Mail Transfer Protocol) class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 26/05/2014 16:05:49
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBuffer.h"
#include "XFileTXT.h"
#include "XDebugTrace.h"

#include "DIOStreamTCPIPConfig.h"
#include "DIOStreamTCPIP.h"

#include "DIOSMTP.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOSMTP::DIOSMTP
*/
/**
//
//  Class Constructor DIOSMTP
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 10:28:26
//
//  @param        diostream :
*/
/*-----------------------------------------------------------------*/
DIOSMTP::DIOSMTP(DIOSTREAM* diostream)
{
  Clean();


  this->diostream   = diostream;
}



/*-------------------------------------------------------------------
//  DIOSMTP::~DIOSMTP
*/
/**
//
//   Class Destructor DIOSMTP
//
//  @author       Abraham J. Velez
//  @version      26/05/2014 16:07:06
//
*/
/*-----------------------------------------------------------------*/
DIOSMTP::~DIOSMTP()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOSMTP::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      30/05/2014 17:15:23
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::Ini()
{
  if(!diostream)                return false;
  if(!diostream->GetConfig())   return false;

  message = new XFILETXT();
  if(!message) return false;

  this->contenttype = contenttype;
  serverconnexiontimeout  = DIOSMTP_DEFAULTTIMEOUT;

  return true;
}




/*-------------------------------------------------------------------
//  DIOSMTP::Server_IsAvailable
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      30/05/2014 17:13:21
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::Server_IsAvailable()
{
  int  attempts   = DIOSMTP_MAXNATTEMPTSCONNECT;
  bool status     = false;

  UpdateConnexionConfig();

  while(attempts)
    {
      if(diostream->Open())
        {
          status = diostream->WaitToConnected(serverconnexiontimeout);
          diostream->Close();
        }

      attempts--;
      if(status) break;
    }

  return status;
}




/*-------------------------------------------------------------------
//  DIOSMTP::AddRecipient
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 15:58:27
//
//  @return       bool :
//
//  @param        type :
//  @param        name :
//  @param        email :
//  @param        check :
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::AddRecipient(DIOSMTPRECIPIENTTYPE type, XCHAR* name, XCHAR* email, bool check)
{
  DIOEMAILADDRESS _email;

  _email = email;

  if(check)
    {
      if(!_email.IsValid()) return false;
    }

  DIOSMTPRECIPIENT* recipient = new DIOSMTPRECIPIENT();
  if(!recipient) return false;

  recipient->SetType(type);
  if(name) recipient->GetName()->Set(name);
  recipient->GetEmail()->Set(_email);

  return recipients.Add(recipient);
}




/*-------------------------------------------------------------------
//  DIOSMTP::AddRecipient
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 15:59:02
//
//  @return       bool :
//
//  @param        type :
//  @param        name :
//  @param        email :
//  @param        check :
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::AddRecipient(DIOSMTPRECIPIENTTYPE type, XSTRING& name, XSTRING& email, bool check)
{
  return AddRecipient(type, name.Get(), email.Get(), check);
}



/*-------------------------------------------------------------------
//  DIOSMTP::AddRecipient
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 15:59:57
//
//  @return       bool :
//
//  @param        type :
//  @param        name :
//  @param        email :
//  @param        check :
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::AddRecipient(DIOSMTPRECIPIENTTYPE type, XCHAR* name, DIOEMAILADDRESS& email, bool check)
{
  return AddRecipient(type, name, email.Get(), check);
}



/*-------------------------------------------------------------------
//  DIOSMTP::AddRecipient
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 16:00:28
//
//  @return       bool :
//
//  @param        type :
//  @param        name :
//  @param        email :
//  @param        check :
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::AddRecipient(DIOSMTPRECIPIENTTYPE type, XSTRING& name, DIOEMAILADDRESS& email, bool check)
{
  return AddRecipient(type, name.Get(), email.Get(), check);
}



/*-------------------------------------------------------------------
//  DIOSMTP::DelRecipient
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      27/05/2014 16:21:13
//
//  @return       bool :
//
//  @param        email :
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::DelRecipient(DIOEMAILADDRESS& email)
{
  if(recipients.IsEmpty()) return false;

  for(XDWORD c=0; c<recipients.GetSize(); c++)
    {
      DIOSMTPRECIPIENT* recipient = recipients.Get(c);
      if(recipient)
        {
          if((*recipient->GetEmail()) == email)
            {
              recipients.Delete(recipient);
              delete recipient;

              return true;
            }
        }
    }

  return false;
}



/*-------------------------------------------------------------------
//  DIOSMTP::DelAllRecipients
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      27/05/2014 13:44:50
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::DelAllRecipients()
{
  if(recipients.IsEmpty()) return false;

  recipients.DeleteContents();
  recipients.DeleteAll();

  return true;
}




/*-------------------------------------------------------------------
//  DIOSMTP::AddAttachment
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 9:09:46
//
//  @return       bool :
//
//  @param        path :
//  @param        check :
//  @param        sizelimit :
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::AddAttachment(XCHAR* path, bool check, XDWORD sizelimit)
{
  if(!path) return false;

  DIOSMTPATTACHMENT* attachment = new DIOSMTPATTACHMENT();
  if(!attachment) return false;

  attachment->GetXPath()->Set(path);

  if(check)
    {
      if(!attachment->FileExists())
        {
          delete attachment;
          return false;
        }
    }

  if(attachment->GetSize()>sizelimit)
    {
      delete attachment;
      return false;
    }

  attachments.Add(attachment);

  return true;
}



/*-------------------------------------------------------------------
//  DIOSMTP::AddAttachment
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 9:09:55
//
//  @return       bool :
//
//  @param        xpath :
//  @param        check :
//  @param        sizelimit :
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::AddAttachment(XPATH& xpath, bool check, XDWORD sizelimit)
{
  return AddAttachment(xpath.Get(), check, sizelimit);
}



/*-------------------------------------------------------------------
//  DIOSMTP::DelAttachment
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 9:03:50
//
//  @return       bool :
//
//  @param        path :
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::DelAttachment(XCHAR* path)
{
  if(attachments.IsEmpty()) return false;

  for(XDWORD c=0; c<attachments.GetSize(); c++)
    {
      DIOSMTPATTACHMENT* attachment = attachments.Get(c);
      if(attachment)
        {
          if(!attachment->GetXPath()->Compare(path))
            {
              attachments.Delete(attachment);
              delete attachment;

              return true;
            }
        }
    }

  return false;
}


/*-------------------------------------------------------------------
//  DIOSMTP::DelAttachment
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 9:03:53
//
//  @return       bool :
//
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::DelAttachment(XPATH& xpath)
{
  return DelAttachment(xpath.Get());
}




/*-------------------------------------------------------------------
//  DIOSMTP::DelAllAttachments
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 9:03:58
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::DelAllAttachments()
{
  attachments.DeleteContents();
  attachments.DeleteAll();

  return true;
}




/*-------------------------------------------------------------------
//  DIOSMTP::Send
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      27/05/2014 12:54:31
//
//  @return       bool :
//
//  @param        login :
//  @param        password :
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::Send()
{
  if(!diostream)                return false;

  if(!diostream->GetConfig())   return false;

  if(recipients.IsEmpty())      return false;

  if(!UpdateConnexionConfig())  return false;

  int  attempts = DIOSMTP_MAXNATTEMPTSCONNECT;
  bool status   = false;

  while(attempts)
    {
      if(diostream->Open()) status = diostream->WaitToConnected(serverconnexiontimeout);

      attempts--;
      if(status) break;
    }

  if(!status)
    {
      diostream->Close();
      return false;
    }

  XSTRING response;

  if(!diostream->ReadStr(response, serverconnexiontimeout))
    {
      diostream->Close();
      return false;
    }

  int code = GetCodeResult(response);
  if(code!=220)
    {
      diostream->Close();
      return false;
    }

  XSTRING domain;
  senderemail.GetDomain(domain);

  response.Format(__L("EHLO %s\r\n") ,domain.Get());
  if(!SendResponse(response, 250))
    {
      diostream->Close();
      return false;
    }

  response.Format(__L("AUTH LOGIN\r\n"));
  if(!SendResponseAndWait(response, 334, 250))
    {
      diostream->Close();
      return false;
    }

  XSTRING encoded64login;
  XSTRING encoded64password;

  serverlogin.ConvertToBase64(encoded64login);
  serverpassword.ConvertToBase64(encoded64password);

  response.Format(__L("%s\r\n"), encoded64login.Get());
  if(!SendResponse(response, 334))
    {
      diostream->Close();
      return false;
    }

  response.Format(__L("%s\r\n"), encoded64password.Get());
  if(!SendResponseAndWait(response, 235, 334))
    {
      diostream->Close();
      return false;
    }

  if(sendername.GetSize())
    {
      response.Format(__L("MAIL FROM:<%s>\r\n"), sendername.Get());
      if(!SendResponse(response, 250))
        {
          diostream->Close();
          return false;
        }
    }

  bool atleastonereceiver =false;

  for(XDWORD c=0; c<recipients.GetSize(); c++)
    {
      DIOSMTPRECIPIENT* recipient = recipients.Get(c);
      if(recipient)
        {
          response.Format(__L("RCPT TO:<%s>\r\n"), recipient->GetEmail()->Get());
          if(!SendResponse(response, 250))
            {

            } else atleastonereceiver = true;
        }
    }

  if(!atleastonereceiver)
    {
      diostream->Close();
      return false;
    }

  response.Format(__L("DATA\r\n"), encoded64password.Get());
  if(!SendResponseAndWait(response, 354, 250))
    {
      diostream->Close();
      return false;
    }

  if(!CreateHeader(response))
    {
      diostream->Close();
      return false;
    }

  diostream->WriteStr(response);
  if(!diostream->WaitToFlushOutXBuffer(serverconnexiontimeout))
    {
      diostream->Close();
      return false;
    }

  if(GetMessage()->GetNLines())
    {
      switch(contenttype)
        {
          case DIOSSMPTCONTENTTYPE_PLAINTTEXT : response.Empty();
                                                for(int c=0; c<GetMessage()->GetNLines(); c++)
                                                  {
                                                    XSTRING line;
                                                    line.Format(__L("%s\r\n"), GetMessage()->GetLine(c)->Get());
                                                    response  += line.Get();
                                                  }

                                                if(response.IsEmpty()) response  += __L(" \r\n");

                                                diostream->WriteStr(response);
                                                if(!diostream->WaitToFlushOutXBuffer(serverconnexiontimeout))
                                                  {
                                                    diostream->Close();
                                                    return false;
                                                  }
                                                break;

            case DIOSSMPTCONTENTTYPE_UTF8     : for(int c=0; c<GetMessage()->GetNLines(); c++)
                                                  {
                                                    XBUFFER xbufferline;
                                                    XSTRING line;

                                                    line  = GetMessage()->GetLine(c)->Get();
                                                    line += __L("\r\n");

                                                    line.ConvertToUTF8(xbufferline);

                                                    diostream->Write(xbufferline);
                                                    if(!diostream->WaitToFlushOutXBuffer(serverconnexiontimeout))
                                                      {
                                                        diostream->Close();
                                                        return false;
                                                      }
                                                  }
                                                break;
          }
    }
   else
    {
      response  += __L(" \r\n");

      diostream->WriteStr(response);
      if(!diostream->WaitToFlushOutXBuffer(serverconnexiontimeout))
        {
          diostream->Close();
          return false;
        }
    }

  if(!attachments.IsEmpty())
    {
      for(XDWORD c=0; c<attachments.GetSize() ;c++)
        {
          DIOSMTPATTACHMENT* attachment = attachments.Get(c);
          if(attachment)
            {
              XBUFFER datafile;
              XSTRING datafileenconded64;
              XDWORD  filesize;

              if(attachment->FileExists(&filesize))
                {
                  XSTRING filenameext;

                  attachment->GetXPath()->GetNamefileExt(filenameext);

                  response.Format(__L("--%s\r\n"), DIOSMTP_BOUNDARYTEXT);
                  response += __L("Content-Type: application/octect-stream\r\n");
                  response += __L("Content-Transfer-Encoding: base64\r\n");
                  response += __L("Content-Disposition: attachment;\r\n filename=\"");
                  response += filenameext.Get();
                  response += __L("\"\r\n");
                  response += __L("\r\n");

                  diostream->WriteStr(response);
                  if(!diostream->WaitToFlushOutXBuffer(serverconnexiontimeout))
                    {
                      diostream->Close();
                      return false;
                    }

                  XFILE* xfile = xfactory->Create_File();
                  if(xfile)
                    {
                      XDWORD br = filesize;

                      if(xfile->Open((*attachment->GetXPath())))
                        {
                          datafile.Resize(filesize);

                          if(xfile->Read(datafile.Get(), &br)) datafile.ConvertToBase64(datafileenconded64);

                          xfile->Close();
                        }

                      #define MAXSIZEPARTENCODED64 64

                      XSTRING partstring;
                      int     index    = 0;
                      int     sizepart;

                      response.Empty();

                      do{ sizepart = MAXSIZEPARTENCODED64;
                          if((index+MAXSIZEPARTENCODED64)>=(int)datafileenconded64.GetSize()) sizepart = datafileenconded64.GetSize()-index;

                          datafileenconded64.Copy(index, index+sizepart, partstring);

                          response += partstring;
                          response += __L("\r\n");

                          index += sizepart;

                        } while(sizepart == MAXSIZEPARTENCODED64);

                      diostream->WriteStr(response);
                      if(!diostream->WaitToFlushOutXBuffer(serverconnexiontimeout))
                        {
                          diostream->Close();
                          return false;
                        }
                    }
                }
            }
        }

      response.Format(__L("--%s--\r\n"), DIOSMTP_BOUNDARYTEXT);
      diostream->WriteStr(response);
      if(!diostream->WaitToFlushOutXBuffer(serverconnexiontimeout))
        {
          diostream->Close();
          return false;
        }
    }

  response = __L("\r\n.\r\n");
  if(!SendResponse(response, 250))
    {
      diostream->Close();
      return false;
    }

  response = __L("QUIT\r\n");
  if(!SendResponse(response, 221))
    {
      diostream->Close();
      return false;
    }

  diostream->Close();

  return true;
}




/*-------------------------------------------------------------------
//  DIOSMTP::End
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      26/05/2014 16:28:08
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::End()
{
  DelAllRecipients();

  DelAllAttachments();

  if(message)
    {
      delete message;
      message = NULL;
    }

  return true;
}




/*-------------------------------------------------------------------
//  DIOSSMTP::UpdateConnexionConfig
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      30/05/2014 16:25:26
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::UpdateConnexionConfig()
{
  DIOSTREAMTCPIPCONFIG* diostreamcfg = (DIOSTREAMTCPIPCONFIG*)diostream->GetConfig();

  if(!diostreamcfg) return false;

  diostreamcfg->SetMode(DIOSTREAMMODE_CLIENT);
  if(serverconnexionlocalIP)  diostreamcfg->GetLocalIP()->Set(serverconnexionlocalIP->Get());

  diostreamcfg->GetRemoteURL()->Set(serverURL.Get());
  diostreamcfg->SetRemotePort(serverport);

  return true;
}





/*-------------------------------------------------------------------
//  DIOSMTP::GetCodeResult
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 12:42:59
//
//  @return       int :
//
//  @param        answer :
*/
/*-----------------------------------------------------------------*/
int DIOSMTP::GetCodeResult(XSTRING& answer)
{
  XSTRING  codestring;
  int      code = 0;

  answer.Copy(0, 3, codestring);

  code = codestring.ConvertToInt();

  return code;
}




/*-------------------------------------------------------------------
//  DIOSMTP::SendResponse
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 13:57:49
//
//  @return       bool :
//
//  @param        response :
//  @param        codevalid :
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::SendResponse(XSTRING& response, int codevalid)
{
  int code = 0;

  diostream->WriteStr(response);
  if(!diostream->WaitToFlushOutXBuffer(serverconnexiontimeout)) return false;

  if(!diostream->ReadStr(response, serverconnexiontimeout)) return false;

  code = GetCodeResult(response);
  if(code!=codevalid) return false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOSMTP::SendResponseAndWait
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 14:13:31
//
//  @return       bool :
//
//  @param        response :
//  @param        codevalid :
//  @param        codecontinue :
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::SendResponseAndWait(XSTRING& response, int codevalid, int codecontinue)
{
  diostream->WriteStr(response);
  if(!diostream->WaitToFlushOutXBuffer(serverconnexiontimeout)) return false;

  int  code     = 0;
  bool accepted = false;
  bool status   = false;

  do{ status = diostream->ReadStr(response, serverconnexiontimeout);
      if(!status) break;

      code = GetCodeResult(response);
      if(code == codevalid)
        {
          accepted = true;
        }
       else
        {
          if(code != codecontinue)  status = false;
        }

      if(!status) break;

    } while(!accepted);

  return status;
}



/*-------------------------------------------------------------------
//  DIOSMTP::CreateHeader
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      28/05/2014 15:53:40
//
//  @return       bool :
//
//  @param        header :
*/
/*-----------------------------------------------------------------*/
bool DIOSMTP::CreateHeader(XSTRING& header)
{
  XCHAR*  month[] = { __L("Jan") , __L("Feb"), __L("Mar"), __L("Apr"), __L("May"), __L("Jun"), __L("Jul") , __L("Aug"), __L("Sep"), __L("Oct"), __L("Nov"), __L("Dec") };
  XSTRING to_line;
  XSTRING cc_line;
  XSTRING bcc_line;
  XSTRING string;

  header.Empty();

  if(!recipients.IsEmpty())
    {
      for(XDWORD c=0; c<recipients.GetSize(); c++)
        {
          DIOSMTPRECIPIENT* recipient = recipients.Get(c);
          if(recipient)
            {
              string.Format(__L("%s<%s>"), recipient->GetName()->Get(), recipient->GetEmail()->Get());

              switch(recipient->GetType())
                {
                  case DIOSMTPRECIPIENTTYPE_UNKNOWN : break;

                  case DIOSMTPRECIPIENTTYPE_TO      : if(!to_line.IsEmpty()) to_line+= __L(",");
                                                      to_line += string.Get();
                                                      break;

                  case DIOSMTPRECIPIENTTYPE_CC      : if(!cc_line.IsEmpty()) cc_line+= __L(",");
                                                      cc_line += string.Get();
                                                      break;

                  case DIOSMTPRECIPIENTTYPE_BCC     : if(!bcc_line.IsEmpty()) bcc_line+= __L(",");
                                                      bcc_line += string.Get();
                                                      break;
                }
            }
        }
    }

  string.Empty();

  /*
  XDATETIME* datetime = xfactory->CreateTime();
  if(datetime)
    {
      datetime->Read();

      string.Format(__L("Date: %d %s %d %d:%d:%d\r\n"), datetime->GetDay()
                                                      , month[datetime->GetMonth()-1]
                                                      , datetime->GetYear()
                                                      , datetime->GetHours()
                                                      , datetime->GetMinutes()
                                                      , datetime->GetSeconds());


      xfactory->DeleteTime(datetime);
    }

  if(string.IsEmpty()) return false;
  header+= string;
  */

  string.Format(__L("From: %s <%s>\r\n"),   GetSenderName()->Get(), GetSenderEmail()->Get());
  header+= string;


  if(!GetReplytoEmail()->IsEmpty())
    {
      string.Format(__L("Reply-To: %s\r\n"),  GetReplytoEmail()->Get());
      header+= string;
    }

  XSTRING defaultxmailer;

  defaultxmailer  = FRAMEWORKNAME;
  defaultxmailer += __L(" ");
  defaultxmailer += FRAMEWORKVERSION;

  string.Format(__L("X-Mailer: %s\r\n"),  GetXMailer()->IsEmpty()? defaultxmailer.Get() : GetXMailer()->Get());
  header+= string;

  switch(xpriority)
    {
      case DIOSMTPXPRIORITY_HIGH    : string = __L("X-Priority: 2 (High)\r\n");     break;
      case DIOSMTPXPRIORITY_UNKNOWN :
      case DIOSMTPXPRIORITY_NORMAL  : string = __L("X-Priority: 3 (Normal)\r\n");   break;
      case DIOSMTPXPRIORITY_LOW     : string = __L("X-Priority: 4 (Low)\r\n");      break;
    }

  if(string.IsEmpty()) return false;
  header+= string;

  if(!to_line.IsEmpty())
    {
      string.Format(__L("To: %s\r\n"),  to_line.Get());
      header+= string;
    }

  if(!cc_line.IsEmpty())
    {
      string.Format(__L("Cc: %s\r\n"),  cc_line.Get());
      header+= string;
    }

  if(!bcc_line.IsEmpty())
    {
      string.Format(__L("Bcc: %s\r\n"), bcc_line.Get());
      header+= string;
    }


  string.Empty();
  switch(contenttype)
    {
      case DIOSSMPTCONTENTTYPE_PLAINTTEXT : string.Format(__L("Subject: %s\r\n"), GetSubject()->IsEmpty()?__L(" "):GetSubject()->Get());
                                            header+= string;
                                            break;

      case DIOSSMPTCONTENTTYPE_UTF8       : { XBUFFER subjectUTF8;
                                              XSTRING subjectbase64;

                                              GetSubject()->ConvertToUTF8(subjectUTF8);

                                              subjectUTF8.ConvertToBase64(subjectbase64);

                                              string = __L("Subject: =?UTF-8?B?");
                                              string += subjectbase64;
                                              string += __L("?=\r\n");
                                            }
                                            break;
    }

  header += string;


  string = __L("MIME-Version: 1.0\r\n");

  if(attachments.GetSize())
    {
      XSTRING string2;
      string2.Format(__L("Content-Type: multipart/mixed; boundary=\"%s\"\r\n\r\n--%s\n\r"), DIOSMTP_BOUNDARYTEXT, DIOSMTP_BOUNDARYTEXT);
      string += string2;
    }

  switch(contenttype)
    {
      case DIOSSMPTCONTENTTYPE_PLAINTTEXT : string += __L("Content-type: text/plain; charset=utf-8\r\n"); break;
      case DIOSSMPTCONTENTTYPE_UTF8       : string += __L("Content-type: text/plain; charset=utf-8\r\n"); break;
    }

//string += __L("Content-Transfer-Encoding: 7bit\r\n");
  string += __L("\r\n");

  header += string;

  return true;
}



