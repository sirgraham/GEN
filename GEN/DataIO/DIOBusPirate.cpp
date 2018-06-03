
/*------------------------------------------------------------------------------------------
//  DIOBUSPIRATE.CPP
//
//  ksdfksdlñf
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/02/2015 11:13:36
//  Last Modification :
//
/*  GEN (C) Copyright  (All right reserved).
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XSleep.h"
#include "XBuffer.h"
#include "XDebugTrace.h"

#include "DIOFactory.h"
#include "DIOStreamConfig.h"
#include "DIOStream.h"

#include "DIOBusPirate.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOBUSPIRATE::DIOBUSPIRATE
*/
/**
//
//  Class Constructor DIOBUSPIRATE
//
//  @author       Abraham J. Velez
//  @version      05/02/2015 17:03:49
//


//  @param        config :
*/
/*-----------------------------------------------------------------*/
DIOBUSPIRATE::DIOBUSPIRATE(DIOSTREAM* diostream)
{
  Clean();

  ;
  this->diostream     = diostream;

  xtimerout = xfactory->CreateTimer();
}



/*-------------------------------------------------------------------
//  DIOBUSPIRATE::~DIOBUSPIRATE
*/
/**
//
//   Class Destructor DIOBUSPIRATE
//
//  @author       Abraham J. Velez
//  @version      05/02/2015 17:02:11
//
*/
/*-----------------------------------------------------------------*/
DIOBUSPIRATE::~DIOBUSPIRATE()
{
  End();

  if(xtimerout) xfactory->DeleteTimer(xtimerout);

  Clean();
}



/*-------------------------------------------------------------------
//  DIOBUSPIRATE::Ini
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/02/2015 16:59:14
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOBUSPIRATE::Ini(int timeout)
{
  if(!diostream)                           return false;

  if(!diostream->Open())                   return false;
  if(!diostream->WaitToConnected(timeout)) return false;

  if(!WriteCommand(__L(""), timeout))      return false;
  if(!WriteCommand(__L("m"), timeout))     return false;
  if(!WriteCommand(__L("1") , timeout))    return false;

  XSLEEP::GetInstance().Seconds(1);
  diostream->GetInXBuffer()->Empty();

  return (GetActualProtocol() == DIOBUSPIRATETYPEPROCOTOL_HIZ);
}



/*-------------------------------------------------------------------
//  DIOBUSPIRATE::GetActualProtocol
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/02/2015 11:59:47
//
//  @return       DIOBUSPIRATETYPEPROCOTOL :
//
*/
/*-----------------------------------------------------------------*/
DIOBUSPIRATETYPEPROCOTOL DIOBUSPIRATE::GetActualProtocol(int timeout)
{
  if(!diostream)                                                 return DIOBUSPIRATETYPEPROCOTOL_UNKNOWN;
  if(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED) return DIOBUSPIRATETYPEPROCOTOL_UNKNOWN;

  XSTRING promptstatus;

  WriteCommand(__L(""), timeout);
  ReadPromptStatus(promptstatus, timeout);

  if(promptstatus.Find(DIOBUSPIRATE_COMMANDMENU_PROTOCOL_HIZ, false) != XSTRING_NOTFOUND)        return DIOBUSPIRATETYPEPROCOTOL_HIZ;
    else if(promptstatus.Find(DIOBUSPIRATE_COMMANDMENU_PROTOCOL_1WIRE, false) != XSTRING_NOTFOUND)  return DIOBUSPIRATETYPEPROCOTOL_1WIRE;
      else if(promptstatus.Find(DIOBUSPIRATE_COMMANDMENU_PROTOCOL_UART, false) != XSTRING_NOTFOUND) return DIOBUSPIRATETYPEPROCOTOL_UART;
        else if(promptstatus.Find(DIOBUSPIRATE_COMMANDMENU_PROTOCOL_I2C, false) != XSTRING_NOTFOUND)    return DIOBUSPIRATETYPEPROCOTOL_I2C;
          else if(promptstatus.Find(DIOBUSPIRATE_COMMANDMENU_PROTOCOL_SPI, false) != XSTRING_NOTFOUND)    return DIOBUSPIRATETYPEPROCOTOL_SPI;
            else if(promptstatus.Find(DIOBUSPIRATE_COMMANDMENU_PROTOCOL_2WIRE, false) != XSTRING_NOTFOUND)  return DIOBUSPIRATETYPEPROCOTOL_2WIRE;
              else if(promptstatus.Find(DIOBUSPIRATE_COMMANDMENU_PROTOCOL_3WIRE, false) != XSTRING_NOTFOUND)  return DIOBUSPIRATETYPEPROCOTOL_3WIRE;

  return DIOBUSPIRATETYPEPROCOTOL_UNKNOWN;
}




/*-------------------------------------------------------------------
//  DIOBUSPIRATE::SetMode
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/02/2015 15:53:22
//
//  @return       bool :
//
//  @param        protocol :
//  @param        issoftware :
//  @param        speed :
//  @param        issniffer :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOBUSPIRATE::SetMode(DIOBUSPIRATETYPEPROCOTOL protocol, bool issoftware, int speed, bool issniffer, int timeout)
{
  if(!xtimerout)                                                 return false;
  if(!diostream)                                                 return false;
  if(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED) return false;

  XSTRING option;
  bool    status = false;

  switch(protocol)
    {
      case DIOBUSPIRATETYPEPROCOTOL_I2C:  if(WriteCommand(__L("m"), timeout))
                                            {
                                              option.Format(__L("%d"), protocol);
                                              if(WriteCommand(option, timeout))
                                                {
                                                  option.Format(__L("%d"), issoftware?1:2);
                                                  if(WriteCommand(option, timeout))
                                                    {
                                                      option.Format(__L("%d"), speed);
                                                      if(WriteCommand(option, timeout))
                                                        {
                                                          if(issniffer)
                                                            {
                                                              option.Format(__L("(2)"), speed);
                                                              status = WriteCommand(option, timeout);

                                                            } else status = true;
                                                        }
                                                     }
                                                  }
                                              }

                                            XSLEEP::GetInstance().Seconds(1);
                                            diostream->GetInXBuffer()->Empty();
                                            break;

    }

  return status;
}



/*-------------------------------------------------------------------
//  DIOBUSPIRATE::ReadI2CSnifferCommand
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/02/2015 11:40:35
//
//  @return       bool :
//
//  @param        in :
//  @param        out :
*/
/*-----------------------------------------------------------------*/
bool DIOBUSPIRATE::ReadI2CSnifferCommand(XBUFFER& in, XBUFFER& out)
{
  XBUFFER xbuffer;
  bool    foundend    = false;
  int     stringsize  = 0;

  diostream->GetInXBuffer()->SetBlocked(true);
  for(int c=0;c<(int)diostream->GetInXBuffer()->GetSize();c++)
    {
      if(diostream->GetInXBuffer()->GetByte(c) == ']')
        {
          stringsize++;
          foundend = true;
          break;

        } else stringsize++;
    }
  diostream->GetInXBuffer()->SetBlocked(false);

  if(!foundend) return false;

  XSTRING string;

  XBYTE* buffer = new XBYTE[stringsize+1];
  if(!buffer) return false;

  memset(buffer, 0, stringsize+1);

  int br = diostream->Read((XBYTE*)buffer, stringsize);
  if(br != stringsize) return false;

  string.Empty();
  string = (char*)buffer;

  delete [] buffer;

 XDEBUGTRACE_PRINTCOLOR(1, __L("%s"), string.Get());

  return true;
}





/*-------------------------------------------------------------------
//  DIOBUSPIRATE::End
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      05/02/2015 16:59:17
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOBUSPIRATE::End()
{
  if(diostream) diostream->Close();

  return true;
}




/*-------------------------------------------------------------------
//  DIOBUSPIRATE::ReadPromptStatus
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/02/2015 10:49:59
//
//  @return       bool :
//
//  @param        promptstatus :
*/
/*-----------------------------------------------------------------*/
bool DIOBUSPIRATE::ReadPromptStatus(XSTRING& promptstatus, int timeout)
{
  if(!diostream)                                                 return false;
  if(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED) return false;

  promptstatus.Empty();

  if(!xtimerout) return false;

  xtimerout->Reset();

  while(1)
    {
      if(diostream->WaitToFilledReadingBuffer(1, 1))
        {
          XBYTE data;
          if(diostream->Read(&data, 1))
            {
              if(data != '>')
                {
                  promptstatus.Add(data);
                }
               else return true;

              xtimerout->Reset();
            }
        }

      if((int)xtimerout->GetMeasureSeconds() >= timeout) break;
    }

  return false;

}



/*-------------------------------------------------------------------
//  DIOBUSPIRATE::WriteCommand
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      06/02/2015 11:10:46
//
//  @return       bool :
//
//  @param        command :
//  @param        timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOBUSPIRATE::WriteCommand(XCHAR* command, int timeout)
{
  if(!diostream)                                                 return false;
  if(diostream->GetConnectStatus() != DIOSTREAMSTATUS_CONNECTED) return false;

  XSTRING _command;
  bool    status;

  _command  = command;
  _command += __L("\r");

  XSTRING_CREATEOEM(_command, charvar)
  status = diostream->Write((XBYTE*)charvar, _command.GetSize())?true:false;
  if(status)  status = diostream->WaitToFlushOutXBuffer(timeout);
  XSTRING_DELETEOEM(charvar)

  return status;
}

