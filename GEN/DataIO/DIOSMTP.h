/*------------------------------------------------------------------------------------------
//  DIOSMTP.H
*/
/**
// \class
//
//  Data IO SMTP (Simple Mail Transfer Protocol) class
//
//  @author  Abraham J. Velez
//
//  Date Of Creation  : 26/05/2014 16:01:01
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _DIOSMTP_H_
#define _DIOSMTP_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XFile.h"

#include "DIOURL.h"
#include "DIOEmailAddress.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define DIOSMTP_MAXNATTEMPTSCONNECT         3
#define DIOSMTP_DEFAULTTIMEOUT              3
#define DIOSMTP_DEFAULSIZELIMITATTACHMENT   (3*1024*1024)
#define DIOSMTP_ANYSIZELIMITATTACHMENT      -1
#define DIOSMTP_BOUNDARYTEXT                __L("__MESSAGE__ID__54yg6f6h6y456345")


enum DIOSSMPTCONTENTTYPE
{
  DIOSSMPTCONTENTTYPE_PLAINTTEXT          = 0 ,
  DIOSSMPTCONTENTTYPE_UTF8                    ,
};


enum DIOSMTPRECIPIENTTYPE
{
  DIOSMTPRECIPIENTTYPE_UNKNOWN            = 0 ,
  DIOSMTPRECIPIENTTYPE_TO                     ,  //  TO: header with this recipient's address
  DIOSMTPRECIPIENTTYPE_CC                     ,  //  CC: header with this recipient's address
  DIOSMTPRECIPIENTTYPE_BCC                       //  BCC: This recipient isn't mentioned in the headers at all, but is still a recipient.
};


enum DIOSMTPXPRIORITY
{
  DIOSMTPXPRIORITY_UNKNOWN                = 0 ,
  DIOSMTPXPRIORITY_HIGH                   = 2 ,
  DIOSMTPXPRIORITY_NORMAL                 = 3 ,
  DIOSMTPXPRIORITY_LOW                    = 4
};


/*---- CLASS -----------------------------------------------------------------------------*/

class XFACTORY;
class XFILETXT;
class DIOSTREAM;


class DIOSMTPRECIPIENT
{
  public:
                                  DIOSMTPRECIPIENT                ()                                        { Clean();                          }
    virtual                      ~DIOSMTPRECIPIENT                ()                                        { Clean();                          }

    DIOSMTPRECIPIENTTYPE          GetType                         ()                                        { return type;                      }
    void                          SetType                         (DIOSMTPRECIPIENTTYPE type)               { this->type = type;                }

    XSTRING*                      GetName                         ()                                        { return &name;                     }
    DIOEMAILADDRESS*              GetEmail                        ()                                        { return &email;                    }

  private:

    void                          Clean                           ()
                                  {
                                    type = DIOSMTPRECIPIENTTYPE_UNKNOWN;
                                  }

    DIOSMTPRECIPIENTTYPE          type;
    XSTRING                       name;
    DIOEMAILADDRESS               email;
};




class DIOSMTPATTACHMENT
{
  public:
                                  DIOSMTPATTACHMENT               ()
                                  {
                                    Clean();

                                  }

    virtual                      ~DIOSMTPATTACHMENT               ()                                        { Clean();                          }


    XPATH*                        GetXPath                        ()                                        { return &xpath;                    }

    bool                          FileExists                      (XDWORD* size = NULL)
                                  {
                                    XFILE* xfile = xfactory->Create_File();
                                    bool   status = false;
                                    if(xfile)
                                      {
                                        if(xfile->Open(xpath))
                                          {
                                            if(size) (*size) = xfile->GetSize(); else this->size = xfile->GetSize();
                                            xfile->Close();
                                            status = true;
                                          }
                                        xfactory->Delete_File(xfile);
                                      }

                                    return status;
                                  }

    XDWORD                        GetSize                         ()
                                  {
                                    FileExists();
                                    return size;
                                  }

    int                           GetSizeLimit                    ()                                        { return sizelimit;                 }
    void                          SetSizeLimit                    (XDWORD sizelimit = DIOSMTP_ANYSIZELIMITATTACHMENT)
                                  {
                                    this->sizelimit = sizelimit;
                                  }

  private:

    void                          Clean                           ()
                                  {
                                    size        = 0;
                                    sizelimit   = 0;
                                  }


    XPATH                         xpath;
    XDWORD                        size;
    XDWORD                        sizelimit;
};



class DIOSMTP
{
  public:

                                  DIOSMTP                         ( DIOSTREAM* diostream);
    virtual                      ~DIOSMTP                         ();

    bool                          Ini                             ();

    DIOURL*                       Server_GetURL                   ()                                        { return &serverURL;                          }

    int                           Server_GetPort                  ()                                        { return serverport;                          }
    void                          Server_SetPort                  (int port)                                { serverport = port;                          }

    XSTRING*                      Server_GetLogin                 ()                                        { return &serverlogin;                        }
    XSTRING*                      Server_GetPassword              ()                                        { return &serverpassword;                     }

    int                           Server_GetConnexionTimeout      ()                                        { return serverconnexiontimeout;              }
    void                          Server_SetConnexionTimeout      (int timeout =  DIOSMTP_DEFAULTTIMEOUT)   { serverconnexiontimeout = timeout;           }

    XSTRING*                      Server_GetConnexionLocalIP      ()                                        { return serverconnexionlocalIP;              }

    bool                          Server_IsAvailable              ();


    DIOSSMPTCONTENTTYPE           GetContentType                  ()                                        { return contenttype;                         }
    void                          SetContentType                  (DIOSSMPTCONTENTTYPE contenttype)         { this->contenttype = contenttype;            }

    XSTRING*                      GetSenderName                   ()                                        { return &sendername;                         }
    DIOEMAILADDRESS*              GetSenderEmail                  ()                                        { return &senderemail;                        }

    DIOEMAILADDRESS*              GetReplytoEmail                 ()                                        { return &replytoemail;                       }

    XVECTOR<DIOSMTPRECIPIENT*>*   GetRecipients                   ()                                        { return &recipients;                         }
    bool                          AddRecipient                    (DIOSMTPRECIPIENTTYPE type, XCHAR* name, XCHAR* email, bool check = true);
    bool                          AddRecipient                    (DIOSMTPRECIPIENTTYPE type, XSTRING& name, XSTRING& email, bool check = true);
    bool                          AddRecipient                    (DIOSMTPRECIPIENTTYPE type, XCHAR* name, DIOEMAILADDRESS& email, bool check = true);
    bool                          AddRecipient                    (DIOSMTPRECIPIENTTYPE type, XSTRING& name, DIOEMAILADDRESS& email, bool check = true);
    bool                          DelRecipient                    (DIOEMAILADDRESS& email);
    bool                          DelAllRecipients                ();

    XSTRING*                      GetXMailer                      ()                                        { return &xmailer;                            }

    XSTRING*                      GetSubject                      ()                                        { return &subject;                            }
    XFILETXT*                     GetMessage                      ()                                        { return message;                             }

    DIOSMTPXPRIORITY              GetXPriority                    ()                                        { return xpriority;                           }
    void                          SetXPriority                    (DIOSMTPXPRIORITY xpriority)              { this->xpriority = xpriority;                }

    bool                          AddAttachment                   (XCHAR* path, bool check = true, XDWORD sizelimit = DIOSMTP_DEFAULSIZELIMITATTACHMENT);
    bool                          AddAttachment                   (XPATH& xpath, bool check = true, XDWORD sizelimit = DIOSMTP_DEFAULSIZELIMITATTACHMENT);
    bool                          DelAttachment                   (XCHAR* path);
    bool                          DelAttachment                   (XPATH& xpath);
    bool                          DelAllAttachments               ();

    bool                          Send                            ();

    bool                          End                             ();

  private:

    void                          Clean                           ()
                                  {

                                    diostream                 = NULL;

                                    serverport                = 0;

                                    serverconnexiontimeout    = DIOSMTP_DEFAULTTIMEOUT;
                                    serverconnexionlocalIP    = NULL;

                                    contenttype               = DIOSSMPTCONTENTTYPE_PLAINTTEXT;

                                    message                   = NULL;

                                    xpriority                 = DIOSMTPXPRIORITY_UNKNOWN;
                                  }

    bool                          UpdateConnexionConfig           ();

    int                           GetCodeResult                   (XSTRING& answer);

    bool                          SendResponse                    (XSTRING& response, int codevalid);
    bool                          SendResponseAndWait             (XSTRING& response, int codevalid, int codecontinue);

    bool                          CreateHeader                    (XSTRING& header);


    DIOSTREAM*                    diostream;

    DIOURL                        serverURL;
    int                           serverport;

    XSTRING                       serverlogin;
    XSTRING                       serverpassword;

    int                           serverconnexiontimeout;
    XSTRING*                      serverconnexionlocalIP;

    DIOSSMPTCONTENTTYPE           contenttype;

    XSTRING                       sendername;
    DIOEMAILADDRESS               senderemail;
    DIOEMAILADDRESS               replytoemail;
    XVECTOR<DIOSMTPRECIPIENT*>    recipients;
    XSTRING                       xmailer;
    XSTRING                       subject;
    XFILETXT*                     message;
    DIOSMTPXPRIORITY              xpriority;
    XVECTOR<DIOSMTPATTACHMENT*>   attachments;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif


