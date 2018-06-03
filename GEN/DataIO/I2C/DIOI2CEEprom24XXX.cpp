/*------------------------------------------------------------------------------------------
//  DIOI2CEEPROM24XXX.CPP
//
//  Data IO I2C EEprom 24XXX class.
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 03/05/2014 16:21:57
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XFactory.h"
#include "XBuffer.h"
#include "XDebugTrace.h"

#include "DIOFactory.h"
#include "DIOStreamI2CConfig.h"
#include "DIOStreamI2C.h"

#include "DIOI2CEEprom24XXX.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOI2CEEPROM24XXX::DIOI2CEEPROM24XXX
*/
/**
//
//  Class Constructor DIOI2CEEPROM24XXX
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:37:54
//

//  @param        xpublisher :
*/
/*-----------------------------------------------------------------*/
DIOI2CEEPROM24XXX::DIOI2CEEPROM24XXX() : DIODEVICEI2C()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOI2CEEPROM24XXX::~DIOI2CEEPROM24XXX
*/
/**
//
//   Class Destructor DIOI2CEEPROM24XXX
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:25:15
//
*/
/*-----------------------------------------------------------------*/
DIOI2CEEPROM24XXX::~DIOI2CEEPROM24XXX()
{
  End();

  Clean();
}






/*-------------------------------------------------------------------
//  DIOI2CEEPROM24XXX::Read
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/05/2014 9:47:21
//
//  @return       bool :
//
//  @param        address :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool DIOI2CEEPROM24XXX::Read(XDWORD address, XBYTE* buffer, XDWORD size)
{
  if(!diostream)        return false;
  if(!IsInitialized())  return false;

  if(diostreamcfg->GetRemoteAddress() ==  DIOSTREAMI2CCONFIG_ADDRESSINVALID) return false;

  if((address+size)>maxsize) return false;

  bool status = true;

  for(XDWORD c=0; c<size; c++)
    {
      xtimerout->Reset();

      while(1)
        {
          status = ReadBYTE(address+c, buffer[c]);
          if(!status)
            {
              if(xtimerout->GetMeasureSeconds()>(XDWORD)timeout) break;

            } else break;
        }
    }

  return status;
}




/*-------------------------------------------------------------------
//  DIOI2CEEPROM24XXX::Read
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      19/05/2014 0:04:44
//
//  @return       bool :
//
//  @param        address :
//  @param        size :
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
bool DIOI2CEEPROM24XXX::Read(XDWORD address, XDWORD size, XBUFFER& xbuffer)
{
  xbuffer.Delete();
  xbuffer.Resize(size);

  return Read(address, xbuffer.Get(), size);
}




/*-------------------------------------------------------------------
//  DIOI2CEEPROM24XXX::Write
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/05/2014 9:47:38
//
//  @return       bool :
//
//  @param        address :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool DIOI2CEEPROM24XXX::Write(XDWORD address, XBYTE* buffer, XDWORD size)
{
  if(!diostream)        return false;
  if(!IsInitialized())  return false;

  if(diostreamcfg->GetRemoteAddress() ==  DIOSTREAMI2CCONFIG_ADDRESSINVALID) return false;

  if((address +size)>maxsize) return false;

  bool status = true;

  for(XDWORD c=0; c<size; c++)
    {
      status = WriteBYTE(address+c, buffer[c]);
      if(!status) break;
    }

  return status;
}




/*-------------------------------------------------------------------
//  DIOI2CEEPROM24XXX::Write
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/05/2014 10:11:05
//
//  @return       bool :
//
//  @param        address :
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
bool DIOI2CEEPROM24XXX::Write(XDWORD address, XBUFFER& xbuffer)
{
  return Write(address, xbuffer.Get(), xbuffer.GetSize());
}



/*-------------------------------------------------------------------
//  DIOI2CEEPROM24XXX::End
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      03/05/2014 16:29:31
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2CEEPROM24XXX::End()
{
  if(!DIODEVICE::End()) return false;

  if(diostream)
    {
      diostream->Close();
      diofactory->DeleteStreamIO(diostream);
      diostream = NULL;
    }

  return true;
}



/*-------------------------------------------------------------------
//  DIOI2CEEPROM24XXX::IniDevice
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      09/09/2016 14:07:23
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOI2CEEPROM24XXX::IniDevice()
{
  int deviceaddressbase = diostreamcfg->GetRemoteAddress();

  switch(type)
    {
      case DIOI2CEEPROM24XXXTYPE_UNKNOWN  : return false;

      case DIOI2CEEPROM24XXXTYPE_08       : npages            = 8;
                                            maxsizepage       = 128;
                                            maxsizechunk      = 16;
                                            maxsize           = npages*maxsizepage;
                                            deviceaddressbase = DIOI2CEEPROM2408_FIXEDADDRESSBASE;
                                            break;

      case DIOI2CEEPROM24XXXTYPE_16       : npages            = 8;
                                            maxsizepage       = 256;
                                            maxsizechunk      = 16;
                                            maxsize           = npages*maxsizepage;
                                            deviceaddressbase = DIOI2CEEPROM2416_FIXEDADDRESSBASE;
                                            break;

      case DIOI2CEEPROM24XXXTYPE_65       : npages            = 1;
                                            maxsizepage       = 8192;
                                            maxsizechunk      = 16;
                                            maxsize           = 8192;
                                            break;

      case DIOI2CEEPROM24XXXTYPE_515      : npages            = 1;
                                            maxsizepage       = 65535;
                                            maxsizechunk      = 16;
                                            maxsize           = 65536;
                                            break;
    }

  diostreamcfg->SetRemoteAddress(deviceaddressbase);

  diostream = (DIOSTREAMI2C*)diofactory->CreateStreamIO( diostreamcfg);
  if(!diostream)  return false;

  if(!diostream->Open())  return false;

  if(!diostream->WaitToConnected(timeout)) return false;

  return DIODEVICE::Ini();
}




/*-------------------------------------------------------------------
//  DIOI2CEEPROM24XXX::ReadBYTE
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/05/2014 23:48:54
//
//  @return       bool :
//
//  @param        address :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool DIOI2CEEPROM24XXX::ReadBYTE(XDWORD address, XBYTE& data)
{
  if(!diostream)        return false;
  if(!IsInitialized())  return false;

  XBYTE   remotebasebackup = (XBYTE)diostreamcfg->GetRemoteAddress();
  XBYTE   remotebase       = remotebasebackup;
  XBUFFER xbuffer;
  bool    status;

  switch(type)
    {
      case DIOI2CEEPROM24XXXTYPE_UNKNOWN  :
                                  default : return false;

      case DIOI2CEEPROM24XXXTYPE_08       :
      case DIOI2CEEPROM24XXXTYPE_16       : { if((address)>=maxsize) return false;

                                              XBYTE page =  (XBYTE)(address / maxsizepage);
                                              remotebase+=(XBYTE)page;

                                              XBYTE offset = (address % maxsizepage);

                                              xbuffer.Add((XBYTE)offset);

                                              //XDEBUGTRACE_PRINTCOLOR(1, __L("%d, %d"), page, offset);
                                            }
                                            break;

      case DIOI2CEEPROM24XXXTYPE_65       :
      case DIOI2CEEPROM24XXXTYPE_515      : if((address)>=maxsize) return false;
                                            xbuffer.Add((XWORD)address);
                                            break;
    }

  diostreamcfg->SetRemoteAddress(remotebase);
  if(diostream->Write(xbuffer)) status = diostream->WaitToFlushOutXBuffer(timeout);

  status = diostream->WaitToFilledReadingBuffer(1, timeout);
  if(status) status = diostream->Read(&data, 1)?true:false;

  //XDEBUGTRACE_PRINTCOLOR(1, __L("%02X, %02X"), xbuffer.GetByte(0), data);

  diostreamcfg->SetRemoteAddress(remotebasebackup);

  return status;
}





/*-------------------------------------------------------------------
//  DIOI2CEEPROM24XXX::WriteBYTE
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      18/05/2014 23:47:48
//
//  @return       bool :
//
//  @param        address :
//  @param        data :
*/
/*-----------------------------------------------------------------*/
bool DIOI2CEEPROM24XXX::WriteBYTE(XDWORD address, XBYTE data)
{
  if(!diostream)        return false;
  if(!IsInitialized())  return false;

  XBYTE   remotebasebackup = (XBYTE)diostreamcfg->GetRemoteAddress();
  XBYTE   remotebase       = remotebasebackup;
  XBUFFER xbuffer;
  bool    status           = false;

  switch(type)
    {
      case DIOI2CEEPROM24XXXTYPE_UNKNOWN  :
                                  default : return false;

      case DIOI2CEEPROM24XXXTYPE_08       :
      case DIOI2CEEPROM24XXXTYPE_16       : { if((address)>=maxsize) return false;

                                              XBYTE page =  (XBYTE)(address / maxsizepage);
                                              remotebase+=(XBYTE)page;

                                              XBYTE offset = (address % maxsizepage);

                                              xbuffer.Add((XBYTE)offset);
                                            }
                                            break;

      case DIOI2CEEPROM24XXXTYPE_65       :
      case DIOI2CEEPROM24XXXTYPE_515      : if((address)>=maxsize) return false;
                                            xbuffer.Add((XWORD)address);
                                            break;
    }


  diostreamcfg->SetRemoteAddress(remotebase);

  xbuffer.Add((XBYTE)data);

  status = diostream->Write(xbuffer)==xbuffer.GetSize()?true:false;
  if(status) status = diostream->WaitToFlushOutXBuffer(timeout);

  diostreamcfg->SetRemoteAddress(remotebasebackup);

  return status;
}
