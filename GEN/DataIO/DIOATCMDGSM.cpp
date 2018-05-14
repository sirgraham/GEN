//------------------------------------------------------------------------------------------
//  DIOATCMDGSM.CPP
//
//  ATCommand for GSM Modem
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 13/05/2002
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "XFactory.h"
#include "XFSMachine.h"

#include "DIOStream.h"

#include "DIOATCMDGSM.h"

#include "XMemory.h"


//---- GENERAL VARIABLE -------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOATCMDGSM_ALPHABET_8859_1::DIOATCMDGSM_ALPHABET_8859_1
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/03/2011 12:51:39
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOATCMDGSM_ALPHABET_8859_1::DIOATCMDGSM_ALPHABET_8859_1()
{
  Clean();
  CreateTable();
}



/*-------------------------------------------------------------------
//  DIOATCMDGSM_ALPHABET_8859_1::DIOATCMDGSM_ALPHABET_8859_1
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/03/2011 12:51:39
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOATCMDGSM_ALPHABET_8859_1::~DIOATCMDGSM_ALPHABET_8859_1()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOATCMDGSM_ALPHABET_8859_1::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/03/2011 12:53:12
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOATCMDGSM_ALPHABET_8859_1::Clean()
{
  memset(table,0, 138*2);
}



/*-------------------------------------------------------------------
//  DIOATCMDGSM_ALPHABET_8859_1::CreateTable
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/03/2011 12:53:25
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOATCMDGSM_ALPHABET_8859_1::CreateTable()
{
  char table2[138][2] = { // Normal Characters
                          { 0x00,'@'   }, { 0x10,'?'   },{ 0x20,' '   },{ 0x30,'0'   },{ 0x40,'¡'   },{ 0x50,'P'   },{ 0x60,'¿'   },{ 0x70,'p'   },
                          { 0x01,'£'   }, { 0x11,'_'   },{ 0x21,'!'   },{ 0x31,'1'   },{ 0x41,'A'   },{ 0x51,'Q'   },{ 0x61,'a'   },{ 0x71,'q'   },
                          { 0x02,'$'   }, { 0x12,'F'   },{ 0x22,'"'   },{ 0x32,'2'   },{ 0x42,'B'   },{ 0x52,'R'   },{ 0x62,'b'   },{ 0x72,'r'   },
                          { 0x03,'¥'   }, { 0x13,'G'   },{ 0x23,'#'   },{ 0x33,'3'   },{ 0x43,'C'   },{ 0x53,'S'   },{ 0x63,'c'   },{ 0x73,'s'   },
                          { 0x04,'è'   }, { 0x14,'?'   },{ 0x24,'¤'   },{ 0x34,'4'   },{ 0x44,'D'   },{ 0x54,'T'   },{ 0x64,'d'   },{ 0x74,'t'   },
                          { 0x05,'é'   }, { 0x15,'O'   },{ 0x25,'%'   },{ 0x35,'5'   },{ 0x45,'E'   },{ 0x55,'U'   },{ 0x65,'e'   },{ 0x75,'u'   },
                          { 0x06,'ú'   }, { 0x16,'?'   },{ 0x26,'&'   },{ 0x36,'6'   },{ 0x46,'F'   },{ 0x56,'V'   },{ 0x66,'f'   },{ 0x76,'v'   },
                          { 0x07,'ì'   }, { 0x17,'?'   },{ 0x27,'\\'  },{ 0x37,'7'   },{ 0x47,'G'   },{ 0x57,'W'   },{ 0x67,'g'   },{ 0x77,'w'   },
                          { 0x08,'ò'   }, { 0x18,'S'   },{ 0x28,'('   },{ 0x38,'8'   },{ 0x48,'H'   },{ 0x58,'X'   },{ 0x68,'h'   },{ 0x78,'x'   },
                          { 0x09,'Ç'   }, { 0x19,'T'   },{ 0x29,')'   },{ 0x39,'9'   },{ 0x49,'I'   },{ 0x59,'Y'   },{ 0x69,'i'   },{ 0x79,'y'   },
                          { 0x0A,'\r'  }, { 0x1A,'?'   },{ 0x2A,'*'   },{ 0x3A,':'   },{ 0x4A,'J'   },{ 0x5A,'Z'   },{ 0x6A,'j'   },{ 0x7A,'z'   },
                          { 0x0B,'Ø'   }, { 0x1B,0x1B  },{ 0x2B,'+'   },{ 0x3B,';'   },{ 0x4B,'K'   },{ 0x5B,'Ä'   },{ 0x6B,'k'   },{ 0x7B,'ä'   },
                          { 0x0C,'ø'   }, { 0x1C,'Æ'   },{ 0x2C,','   },{ 0x3C,'<'   },{ 0x4C,'L'   },{ 0x5C,'Ö'   },{ 0x6C,'l'   },{ 0x7C,'ö'   },
                          { 0x0D,'\n'  }, { 0x1D,'æ'   },{ 0x2D,'-'   },{ 0x3D,'='   },{ 0x4D,'M'   },{ 0x5D,'Ñ'   },{ 0x6D,'m'   },{ 0x7D,'ñ'   },
                          { 0x0E,'Å'   }, { 0x1E,'ß'   },{ 0x2E,'.'   },{ 0x3E,'>'   },{ 0x4E,'N'   },{ 0x5E,'Ü'   },{ 0x6E,'n'   },{ 0x7E,'ü'   },
                          { 0x0F,'å'   }, { 0x1F,'Ê'   },{ 0x2F,'/'   },{ 0x3F,'?'   },{ 0x4F,'O'   },{ 0x5F,'§'   },{ 0x6F,'o'   },{ 0x7F,'à'   },
                          // Extended Characters
                          { 0x0A,'\t'  },
                          { 0x14,'^'   },
                          { 0x28,'{'   },
                          { 0x29,'}'   },
                          { 0x2F,'\\'  },
                          { 0x3C,'['   },
                          { 0x3D,'~'   },
                          { 0x3E,']'   },
                          { 0x40,'|'   },
                          { 0x65,'€'   }
                        };

  for(int c=0;c<138;c++)
    {
      table[c][0] = table2[c][0];
      table[c][1] = table2[c][1];
    }

  return true;
}



/*-------------------------------------------------------------------
//  DIOATCMDGSM_ALPHABET_8859_1::Traslate
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/03/2011 12:58:02
//
//  @return       XWORD :
//  @param        from :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
XWORD DIOATCMDGSM_ALPHABET_8859_1::Traslate(bool from,XWORD data)
{
  XDWORD end         = (sizeof(table)/sizeof(char[2]));
  char  extended   = ((data&0xFF00)>>8);
  char  character  = (data&0x00FF);
  char  escape     = 0x1B;
  XDWORD c;

  if(from)
    {
      for(c=0;c<127;c++)
        {
          if(table[c][1]==character) return (XWORD)table[c][0];
        }

      for(c=128;c<end;c++)
        {
          if(table[c][1]==character) return (XWORD)((escape<<8)|table[c][0]);
        }
    }
   else
    {
      int first =0;

      if(extended==escape)
            first = 128;
       else end  = 127;

      for(c=first;c<end;c++)
        {
          if(table[c][0]==character) return (XWORD)table[c][1];
        }
     }

  return 0xFF00;
}




//-------------------------------------------------------------------
//  DIOATCMDGSM::DIOATCMDGSM
/**
//
//
//  @author       Abraham J. Velez
//  @version      17/04/2002 13:34:54
//
//  @return       void :

//  @param        diostream :
*/
//-------------------------------------------------------------------
DIOATCMDGSM::DIOATCMDGSM( DIOSTREAM* diostream) : DIOATCMDS(diostream)
{
  Clean();

  AddCommand(DIOATCMD_TYPE_GSMGETMANUFACTURED       , __L("+GMI"));
  AddCommand(DIOATCMD_TYPE_GSMGETMODEL              , __L("+GMM"));
  AddCommand(DIOATCMD_TYPE_GSMGETSERIAL             , __L("+GSN"));
  AddCommand(DIOATCMD_TYPE_GSMGETVERSION            , __L("+GMR"));
  AddCommand(DIOATCMD_TYPE_GSMGETSIGNALQUALITY      , __L("+CSQ"));
  AddCommand(DIOATCMD_TYPE_GSMGETSERVICES           , __L("+GCAP"));
  AddCommand(DIOATCMD_TYPE_GSMGETIMSI               , __L("+CIMI"));
  AddCommand(DIOATCMD_TYPE_GSMGETSMSSUPPORT         , __L("+CSMS"));
  AddCommand(DIOATCMD_TYPE_GSMGETSMSFORMAT          , __L("+CMGF"));
  AddCommand(DIOATCMD_TYPE_GSMSETSMSFORMAT          , __L("+CMGF"));
  AddCommand(DIOATCMD_TYPE_GSMGETSMSCENTER          , __L("+CSCA"));
  AddCommand(DIOATCMD_TYPE_GSMSETSMSCENTER          , __L("+CSCA"));
  AddCommand(DIOATCMD_TYPE_GSMSENDSMS               , __L("+CMGS"));
}



//-------------------------------------------------------------------
//  DIOATCMDGSM::~DIOATCMDGSM
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/12/2001 21:53:15
//
//  @return
//  */
//-------------------------------------------------------------------
DIOATCMDGSM::~DIOATCMDGSM()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOATCMDPEI::GetManufactured
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/07/2012 11:49:44
//
//  @return       DIOATCMD_ERROR :
//  @param        manufactured :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDGSM::GetManufactured(XSTRING& manufactured,int timeout)
{
  return MakeCommandWithSimpleAnswer(DIOATCMD_TYPE_GSMGETMANUFACTURED,manufactured,NULL,timeout);
}



/*-------------------------------------------------------------------
//  DIOATCMDGSM::GetModelID
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2011 11:49:39
//
//  @return       DIOATCMD_ERROR :
//  @param        model :
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDGSM::GetModel(XSTRING& model, int timeout)
{
  return MakeCommandWithSimpleAnswer(DIOATCMD_TYPE_GSMGETMODEL, model, NULL, timeout);
}




/*-------------------------------------------------------------------
//  DIOATCMDGSM::GetIMEI
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2011 12:10:29
//
//  @return       DIOATCMD_ERROR :
//  @param        IMEI :
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDGSM::GetIMEI(XSTRING& IMEI, int timeout)
{
  return MakeCommandWithSimpleAnswer(DIOATCMD_TYPE_GSMGETSERIAL, IMEI, NULL, timeout);
}


/*-------------------------------------------------------------------
//  DIOATCMDPEI::GetVersion
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/07/2012 12:39:40
//
//  @return       DIOATCMD_ERROR :
//  @param        version :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDGSM::GetVersion(XSTRING& version,int timeout)
{
  DIOATCMD_ERROR error;
  int            index[2];

  index[0]= GetAnswers()->GetSize()-1;
  if(index[0]<0) index[0]=0;

  error = SendCommand(DIOATCMD_TYPE_GSMGETVERSION);
  if(error!=DIOATCMD_ERROR_NONE) return error;

  error = WaitToProccessAnswer(timeout);
  if(error!=DIOATCMD_ERROR_NONE) return error;

  index[1]= GetAnswers()->GetSize();

  version.Empty();

  for(int c=index[0];c<index[1];c++)
    {
      XSTRING* string = GetAnswer(c);
      if(string)
        {
          version += (*string);
          version += __L(" ");
        }
    }

  for(int c=index[0];c<index[1];c++)
    {
      DeleteLastAnswer();
    }

  return error;
}



/*-------------------------------------------------------------------
//  DIOATCMDGSM::GetSignalQuality
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/07/2012 13:42:15
//
//  @return       DIOATCMD_ERROR :
//  @param        RSSI :
//  @param        BER :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDGSM::GetSignalQuality(int& RSSI,int& BER,int timeout)
{
  DIOATCMD_ERROR error = SendCommand(DIOATCMD_TYPE_GSMGETSIGNALQUALITY);
  if(error!=DIOATCMD_ERROR_NONE) return error;

  error = WaitToProccessAnswer(timeout);
  if(error!=DIOATCMD_ERROR_NONE) return error;

  int index = SearchInAnswers(DIOATCMD_TYPE_GSMGETSIGNALQUALITY);
  if(index!=DIOATCMDS_ANSWERNOTFOUND)
    {
      XSTRING* answer = GetAnswer(index);
      if(answer)
        {
          XSTRING  mask;

          mask  = GetCommandString(DIOATCMD_TYPE_GSMGETSIGNALQUALITY);
          mask += __L(":%d,%d");

          answer->UnFormat(mask.Get(),&RSSI,&BER);
        }

      DeleteAnswer(index);
    }

  return DIOATCMD_ERROR_NONE;
}



/*-------------------------------------------------------------------
//  DIOATCMDGSM::IsAvailableServiceGSM
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      27/07/2012 12:57:23
//
//  @return       DIOATCMD_ERROR :
//  @param        available :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDGSM::IsAvailableServiceGSM(bool& available,int timeout)
{
  DIOATCMD_ERROR error = SendCommand(DIOATCMD_TYPE_GSMGETSERVICES);
  if(error!=DIOATCMD_ERROR_NONE) return error;

  error = WaitToProccessAnswer(timeout);
  if(error!=DIOATCMD_ERROR_NONE) return error;

  available = false;

  XSTRING* answer = GetLastAnswer();
  if(answer)
    {
      if(answer->Find(__L("+CGSM"),true)!=XSTRING_NOTFOUND) available = true;
      DeleteLastAnswer();
    }

  DeleteAllAnswers();

  return DIOATCMD_ERROR_NONE;
}



/*-------------------------------------------------------------------
//  DIOATCMDGSM::GSM_GetIMSI
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2011 12:10:29
//
//  @return       DIOATCMD_ERROR :
//  @param        IMSI :
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDGSM::GetIMSI(XSTRING& IMSI, int timeout)
{
  return MakeCommandWithSimpleAnswer(DIOATCMD_TYPE_GSMGETIMSI, IMSI, NULL, timeout);
}


/*-------------------------------------------------------------------
//  DIOATCMDGSM::GetSMSSupport
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/07/2012 17:32:50
//
//  @return       DIOATCMD_ERROR :
//  @param        support :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDGSM::GetSMSSupport(XBYTE& support, int timeout)
{
  DIOATCMD_ERROR error = SendCommand(DIOATCMD_TYPE_GSMGETSMSSUPPORT,__L("?"));
  if(error!=DIOATCMD_ERROR_NONE) return error;

  error = WaitToProccessAnswer(timeout);
  if(error!=DIOATCMD_ERROR_NONE) return error;

  int index = SearchInAnswers(DIOATCMD_TYPE_GSMGETSMSSUPPORT,timeout);
  if(index!=DIOATCMDS_ANSWERNOTFOUND)
    {
      support = 0;

      XSTRING* answer = GetAnswer(index);
      if(answer)
        {
          int      service;
          int      mt = 0;
          int      mo = 0;
          int      bc = 0;
          XSTRING  mask;

          mask  = GetCommandString(DIOATCMD_TYPE_GSMGETSMSSUPPORT);
          mask += __L(": %d,%d,%d,%d");
          answer->UnFormat(mask.Get(),&service,&mt,&mo,&bc);

          support|= (service<<4);
          if(mt) support|=DIOATCMDGSM_SMSSUPPORT_MOVILETERMINATED;
          if(mo) support|=DIOATCMDGSM_SMSSUPPORT_MOVILEORIGINATED;
          if(bc) support|=DIOATCMDGSM_SMSSUPPORT_BROADCAST;
        }

      DeleteAnswer(index);
    }

  return DIOATCMD_ERROR_NONE;
}





/*-------------------------------------------------------------------
//  DIOATCMDGSM::GetSMSFormat
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/07/2012 17:34:40
//
//  @return       DIOATCMD_ERROR :
//  @param        format :
//  @param        support :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDGSM::GetSMSFormat(XBYTE& format,bool support, int timeout)
{
  DIOATCMD_ERROR error = SendCommand(DIOATCMD_TYPE_GSMGETSMSFORMAT,support?__L("=?"):__L("?"));
  if(error!=DIOATCMD_ERROR_NONE) return error;

  error = WaitToProccessAnswer(timeout);
  if(error!=DIOATCMD_ERROR_NONE) return error;

  int index = SearchInAnswers(DIOATCMD_TYPE_GSMGETSMSFORMAT,timeout);
  if(index!=DIOATCMDS_ANSWERNOTFOUND)
    {
      XSTRING* answer = GetAnswer(index);
      XSTRING  mask;

      format = 0;

      if(answer)
        {
          mask  = GetCommandString(DIOATCMD_TYPE_GSMGETSMSFORMAT);

          if(support)
            {
              mask += __L(": (%d,%d)");

              int value[2] = { -1 , -1 };

              answer->UnFormat(mask.Get(),&value[0],&value[1]);

              if(value[0]==0) format |= DIOATCMDGSM_SMSFORMAT_PDU;
              if(value[1]==1) format |= DIOATCMDGSM_SMSFORMAT_TXT;

            }
           else
            {
              mask += __L(": %d");

              int value;

              answer->UnFormat(mask.Get(),&value);

              if(value==0) format = DIOATCMDGSM_SMSFORMAT_PDU;
              if(value==1) format = DIOATCMDGSM_SMSFORMAT_TXT;
            }
        }

      DeleteAnswer(index);
    }

  return DIOATCMD_ERROR_NONE;
}



/*-------------------------------------------------------------------
//  DIOATCMDGSM::SetSMSFormat
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/07/2012 17:45:08
//
//  @return       DIOATCMD_ERROR :
//  @param        format :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDGSM::SetSMSFormat(XBYTE format, int timeout)
{
  XSTRING param;

  switch(format)
    {
      case DIOATCMDGSM_SMSFORMAT_PDU : param = __L("=0"); break;
      case DIOATCMDGSM_SMSFORMAT_TXT : param = __L("=1"); break;
    }

  DIOATCMD_ERROR error = SendCommand(DIOATCMD_TYPE_GSMSETSMSFORMAT,param.Get());
  if(error!=DIOATCMD_ERROR_NONE) return error;

  return WaitToProccessAnswer(timeout);
}



/*-------------------------------------------------------------------
//  DIOATCMDGSM::GetSMSCenter
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/03/2011 10:48:11
//
//  @return       DIOATCMD_ERROR :
//  @param        SMScenter :
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDGSM::GetSMSCenter(XSTRING& SMScenter, int timeout)
{
  DIOATCMD_ERROR error = SendCommand(DIOATCMD_TYPE_GSMGETSMSCENTER,__L("?"));
  if(error!=DIOATCMD_ERROR_NONE) return error;

  error = WaitToProccessAnswer(timeout);
  if(error!=DIOATCMD_ERROR_NONE) return error;

  int index = SearchInAnswers(DIOATCMD_TYPE_GSMGETSMSCENTER,timeout);
  if(index!=DIOATCMDS_ANSWERNOTFOUND)
    {
      XSTRING* answer = GetAnswer(index);
      if(answer)
        {
          XSTRING  mask;
          XCHAR    smscenter[64];
          int      value;

          mask  = GetCommandString(DIOATCMD_TYPE_GSMGETSMSCENTER);
          mask += __L(": \"%s\",%d");

          answer->UnFormat(mask.Get(),smscenter,&value);

          if(smscenter[0])
            {
              SMScenter = smscenter;

            } else return DIOATCMD_ERROR_INVALIDCMD;
        }

      DeleteAnswer(index);
    }

  return DIOATCMD_ERROR_NONE;
}



/*-------------------------------------------------------------------
//  DIOATCMDGSM::SetSMSCenter
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2011 19:30:31
//
//  @return       DIOATCMD_ERROR :
//  @param        smscenter :
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDGSM::SetSMSCenter(XCHAR* smscenter, int timeout)
{
  XSTRING param;

  param.Format(__L("=\"%s\""),smscenter);

  DIOATCMD_ERROR error = SendCommand(DIOATCMD_TYPE_GSMSETSMSCENTER,param.Get());
  if(error!=DIOATCMD_ERROR_NONE) return error;

  return WaitToProccessAnswer(timeout);
}



/*-------------------------------------------------------------------
//  DIOATCMDGSM::SendSMS
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/07/2012 18:52:32
//
//  @return       DIOATCMD_ERROR :
//  @param        number :
//  @param        message :
//  @param         :
//  @param        mrID :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
DIOATCMD_ERROR DIOATCMDGSM::SendSMS(XCHAR* number, XCHAR* message, int* mrID, int timeout)
{
  DIOATCMD_ERROR error;
  XBYTE          SMSformat = DIOATCMDGSM_SMSFORMAT_PDU;
  XSTRING        gsmnumber;
  XSTRING        gsmmessage;

  error=GetSMSFormat(SMSformat);
  if(error!=DIOATCMD_ERROR_NONE) return error;

  switch(SMSformat)
    {
      case DIOATCMDGSM_SMSFORMAT_PDU    : { XSTRING _number;
                                            XSTRING _message;

                                            CodecPDUFormat(number,message,false,20,_number,_message);

                                            gsmnumber.Format(__L("= %s ")  ,_number.Get());
                                            gsmmessage.Format(__L("%s\x1A"),_message.Get());
                                          }
                                          break;

      case DIOATCMDGSM_SMSFORMAT_TXT    : gsmnumber.Format(__L("=\"%s\""),number);
                                          gsmmessage.Format(__L("%s\x1A"),message);
                                          break;

                                default : return DIOATCMD_ERROR_INVALIDCMD;
    }

  error = SendCommand(DIOATCMD_TYPE_GSMSENDSMS,gsmnumber.Get());
  if(error!=DIOATCMD_ERROR_NONE) return error;

  error = DIOATCMD_ERROR_INVALIDCMD;

  if(ReadAnswerWithOutEndChar(__L("> "),timeout))
    {
      error = SendParam(gsmmessage.Get());
      if(error!=DIOATCMD_ERROR_NONE) return error;

      error = WaitToProccessAnswer(timeout);
      if(error!=DIOATCMD_ERROR_NONE) return error;

      int index = SearchInAnswers(DIOATCMD_TYPE_GSMSENDSMS,timeout);
      if(index!=DIOATCMDS_ANSWERNOTFOUND)
        {
          XSTRING* answer = GetAnswer(index);
          if(answer)
            {
              if(mrID)
                {
                  int      size = 0;
                  XSTRING  mask;

                  mask  = GetCommandString(DIOATCMD_TYPE_GSMSENDSMS);
                  mask += __L(": %d");

                  answer->UnFormat(mask.Get(),mrID);
                }

              error = DIOATCMD_ERROR_NONE;
            }

           DeleteAnswer(index);
        }
    }

  return error;
}



//-------------------------------------------------------------------
//  DIOATCMDGSM::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/12/2001 21:50:13
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOATCMDGSM::Clean()
{

}



/*-------------------------------------------------------------------
//  DIOATCMDGSM::CodecPDUFormat
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/03/2011 17:44:39
//
//  @return       bool :
//  @param        number :
//  @param        message :
//  @param        inoctets :
//  @param        validperiod :
//  @param        length :
//  @param        result :
*/
/*-----------------------------------------------------------------*/
bool DIOATCMDGSM::CodecPDUFormat(XCHAR* number,XCHAR* message,bool inoctets,int validperiod,XSTRING& length, XSTRING& result)
{
  if(!number)  return false;
  if(!message) return false;

  DIOATCMDGSM_ALPHABET_8859_1  alphabet;
  XSTRING                      string;
  XSTRING                      _number;
  XSTRING                      _message;
  int                          value;
  XBYTE*                       codec7 = new XBYTE[256];
  XBYTE*                       codec8 = new XBYTE[256];
  int                          size7  = 0;
  int                          size8  = 0;
  int                          c;

  if((!codec7) || (!codec8)) return false;

  memset(codec7 ,0 , 256);
  memset(codec8 ,0 , 256);

  result.Empty();

  _number  = number;
  _message = message;

  // No SMS Center.
  result += __L("00");
  // SMS-SUBMIT mensaje.
  result += __L("11");
  result += __L("00");

  // Size Number (National)
  string.Format(__L("%02X"),_number.GetSize());
  result += string;

  if(_number.GetSize()>9)
         result += __L("91");    // Type Number (International);
    else result += __L("81");    // Type Number (National);

  // GSM Number
  string = number;
  if(string.GetSize()%2) string += __L("F");
  string.Swab();
  result += string;
  result += __L("00");

  // Codec 7/8 Bits
  if(inoctets)
         result += __L("02");
    else result += __L("00");

  // Time Max Msg <28 days  >28 weeks
  if(validperiod>28)
          value = 168 + validperiod;
    else  value = 197 + validperiod;
  string.Format(__L("%02X"),value);
  result += string;

  // Convert message
  for(c=0;c<(int)_message.GetSize();c++)
    {
      XWORD data=alphabet.Traslate(false,(XBYTE)message[c]);
      XBYTE byte[2];

      byte[1]=(XBYTE)(data>>8);
      byte[0]=(XBYTE)(data&0x00FF);

      if(byte[1])
        {
          codec8[size8]=byte[1];
          size8++;
        }

      codec8[size8]=byte[0];
      size8++;
    }


  // Compress 7 Bits.
  if(!inoctets)
    {
      int shift=0;

      for(c=0;c<size8;c++)
        {
          XBYTE mask = (0xFF>>(7-shift));
          XBYTE data = (codec8[c+1]&mask)<<(XBYTE)(7-shift);

          if(shift!=7)
            {
              codec7[size7]=(data|(codec8[c]>>(XBYTE)shift));
              size7++;
            }

          shift++;
          if(shift>7) shift=0;
        }
    }

  string.Format(__L("%02X"),_message.GetSize());
  result += string;

  for(c=0;c<((inoctets)?size8:size7);c++)
    {
      string.Format(__L("%02X"),(inoctets)?codec8[c]:codec7[c]);
      result += string;
    }

  delete [] codec7;
  delete [] codec8;

  length.Format(__L("%d"),(result.GetSize()/2)-1);

  return true;
}

