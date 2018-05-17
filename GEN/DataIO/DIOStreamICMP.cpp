
//------------------------------------------------------------------------------------------
//  DIOSTREAMICMP.CPP
//
//  Data IO Stream ICMP class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XBuffer.h"
#include "XThread.h"

#include "DIOFactory.h"
#include "DIOStreamUDPConfig.h"

#include "DIOStreamICMP.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTREAMICMP::DIOSTREAMICMP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/02/2013 7:40:46
//
//  @return



*/
/*-----------------------------------------------------------------*/
DIOSTREAMICMP::DIOSTREAMICMP() : DIOSTREAM()
{
  Clean();

  datagramsmutex = xfactory->Create_Mutex();
  host           = diofactory->CreateURL();
}



//-------------------------------------------------------------------
//  DIOSTREAMICMP::~DIOSTREAMICMP
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:15:51
//
//  @return
//  */
//-------------------------------------------------------------------
DIOSTREAMICMP::~DIOSTREAMICMP()
{
  DeleteAllDatagrams();

  if(diofactory) diofactory->DeleteURL(host);
  xfactory->Delete_Mutex(datagramsmutex);

  Clean();
}



//-------------------------------------------------------------------
//  DIOSTREAMICMP::GetConfig
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/09/2004 14:32:49
//
//  @return       DIOSTREAMCONFIG* :
//  */
//-------------------------------------------------------------------
DIOSTREAMCONFIG* DIOSTREAMICMP::GetConfig()
{
  return (DIOSTREAMCONFIG*)config;
}




//-------------------------------------------------------------------
//  DIOSTREAMICMP::SetConfig
/**
//
//
//  @author       Abraham J. Velez
//  @version      03/09/2001 16:58:17
//
//  @return       bool :
//  @param        config :
*/
//-------------------------------------------------------------------
bool DIOSTREAMICMP::SetConfig(DIOSTREAMCONFIG* config)
{
  if(!config) return false;
  this->config = (DIOSTREAMICMPCONFIG *)config;

  return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMICMP::Read
*/
/**
//  NO VALID FUNCTION FOR THIS STREAM: USE WriteDatagram
//
//  @author       Abraham J. Velez
//  @version      25/07/2012 13:29:32
//
//  @return       XDWORD :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTREAMICMP::Read(XBYTE* buffer, XDWORD size)
{
  XDWORD esize = 0;

  int index = GetFirstDatagram(false);
  if(index != DIOSTREAMICMP_NOTFOUND)
    {
      DIOSTREAMICMPDATAGRAM* datagram = GetDatagram(index);
      if(datagram)
        {
          esize = datagram->GetData()->GetSize();
          if(esize >= size)
            {
              esize = size;

              memcpy(buffer,(XBYTE*)datagram->GetData()->Get(), size);

              nbytesread+= size;
              if(xtimernotactivity) xtimernotactivity->Reset();

              DeleteDatagram(index);

              inbuffer->Extract(buffer, 0, size);
            }
        }
    }

  return esize;
}




/*-------------------------------------------------------------------
//  DIOSTREAMICMP::Write
*/
/**
//  NO VALID FUNCTION FOR THIS STREAM: USE WriteDatagram
//
//  @author       Abraham J. Velez
//  @version      25/07/2012 13:29:23
//
//  @return       XDWORD :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
XDWORD DIOSTREAMICMP::Write(XBYTE* buffer, XDWORD size)
{
  if(!AddDatagram(true, __L(""), buffer, size)) return 0;

  if(!outbuffer->Add(buffer,size))  return 0;

  nbyteswrite+=size;
  if(xtimernotactivity) xtimernotactivity->Reset();

  return size;
}



/*-------------------------------------------------------------------
//  DIOSTREAMICMP::ReadDatagram
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/03/2015 12:21:17
//
//  @return       bool :
//
//  @param        address :
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMICMP::ReadDatagram(XSTRING& address, XBUFFER& xbuffer)
{
  int index = GetFirstDatagram(false);
  if(index != DIOSTREAMICMP_NOTFOUND)
    {
      DIOSTREAMICMPDATAGRAM* datagram = GetDatagram(index);
      if(datagram)
        {
          address = datagram->GetAddress()->Get();

          xbuffer.Delete();
          xbuffer.Add(datagram->GetData());

          nbytesread+= datagram->GetData()->GetSize();
          if(xtimernotactivity) xtimernotactivity->Reset();

          DeleteDatagram(index);

          return true;
        }
    }

  return false;
}



/*-------------------------------------------------------------------
//  DIOSTREAMICMP::WriteDatagram
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/03/2015 12:22:34
//
//  @return       bool :
//
//  @param        address :
//  @param        port :
//  @param        buffer :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMICMP::WriteDatagram(XSTRING& address, XBYTE* buffer, XDWORD size)
{
  if(address.IsEmpty()) return false;
  if(!AddDatagram(true, address, buffer, size)) return false;
  if(!outbuffer->Add(buffer,size)) return false;

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAMICMP::WriteDatagram
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/03/2015 12:23:02
//
//  @return       bool :
//
//  @param        address :
//  @param        xbuffer :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMICMP::WriteDatagram(XSTRING& address, XBUFFER& xbuffer)
{
  return WriteDatagram(address, xbuffer.Get(), xbuffer.GetSize());
}



/*-------------------------------------------------------------------
//  DIOSTREAMICMP::ResetXBuffers
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      16/11/2014 16:00:37
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMICMP::ResetXBuffers()
{
  if(!DIOSTREAM::ResetXBuffers()) return false;

  return DeleteAllDatagrams();
}



/*-------------------------------------------------------------------
//  DIOSTREAMICMP::ResetInXBuffer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2014 20:46:44
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMICMP::ResetInXBuffer()
{
  if(!DIOSTREAM::ResetInXBuffer()) return false;

  int index = 0;

  while(1)
    {
      DIOSTREAMICMPDATAGRAM* datagram = (DIOSTREAMICMPDATAGRAM*)datagrams.Get(index);
      if(!datagram) break;

      if(datagram->GetAddress()->GetSize())
        {
          DeleteDatagram(index);

        } else index++;
    }


  return true;
}





/*-------------------------------------------------------------------
//  DIOSTREAMICMP::ResetOutXBuffer
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      17/11/2014 20:46:55
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMICMP::ResetOutXBuffer()
{
  if(!DIOSTREAM::ResetOutXBuffer()) return false;

  int index = 0;

  while(1)
    {
      DIOSTREAMICMPDATAGRAM* datagram = (DIOSTREAMICMPDATAGRAM*)datagrams.Get(index);
      if(!datagram) break;

      if(!datagram->GetAddress()->GetSize())
        {
          DeleteDatagram(index);

        } else index++;
    }

  return true;
}




/*-------------------------------------------------------------------
//  DIOSTREAMICMP::DeleteAllDatagrams
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/08/2012 14:00:01
//
//  @return       bool :
//  */
/*-----------------------------------------------------------------*/
bool DIOSTREAMICMP::DeleteAllDatagrams()
{
  if(datagrams.IsEmpty()) return false;

  if(datagramsmutex)  datagramsmutex->Lock();

  datagrams.DeleteContents();

  datagrams.DeleteAll();

  if(datagramsmutex)  datagramsmutex->UnLock();

  return true;
}





/*-------------------------------------------------------------------
//  DIOSTREAMICMP::AddDatagram
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/03/2015 12:24:13
//
//  @return       bool :
//
//  @param        istosend :
//  @param        address :
//  @param        port :
//  @param        data :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMICMP::AddDatagram(bool istosend, XCHAR* address, XBYTE* data, XDWORD size)
{
  if(!address) return false;
  if(!data)    return false;

  if(datagramsmutex)  datagramsmutex->Lock();

  DIOSTREAMICMPDATAGRAM* datagram = new DIOSTREAMICMPDATAGRAM();
  if(datagram)
    {
      datagram->SetIsToSend(istosend);
      datagram->SetAddress(address);
      datagram->SetData(data,size);

      datagrams.Add(datagram);
    }

  if(datagramsmutex)  datagramsmutex->UnLock();

  return datagram?true:false;
}



/*-------------------------------------------------------------------
//  DIOSTREAMICMP::AddDatagram
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      08/03/2015 12:23:33
//
//  @return       bool :
//
//  @param        istosend :
//  @param        address :
//  @param        port :
//  @param        data :
//  @param        size :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMICMP::AddDatagram(bool istosend, XSTRING& address, XBYTE* data,XDWORD size)
{
  return AddDatagram(istosend, address.Get(), data, size);
}



/*-------------------------------------------------------------------
//  DIOSTREAMICMP::GetDatagram
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/08/2012 14:08:54
//
//  @return       DIOSTREAMICMPDATAGRAM* :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
DIOSTREAMICMPDATAGRAM* DIOSTREAMICMP::GetDatagram(int index)
{
  return (DIOSTREAMICMPDATAGRAM*)datagrams.Get(index);
}




/*-------------------------------------------------------------------
//  DIOSTREAMICMP::GetFirstDatagram
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/08/2012 14:07:34
//
//  @return       bool :
//  @param        issend :
*/
/*-----------------------------------------------------------------*/
int DIOSTREAMICMP::GetFirstDatagram(bool tosend)
{
  int index = DIOSTREAMICMP_NOTFOUND;

  if(datagramsmutex)  datagramsmutex->Lock();

  for(int c=0;c<(int)datagrams.GetSize();c++)
    {
      DIOSTREAMICMPDATAGRAM* datagram = (DIOSTREAMICMPDATAGRAM*)datagrams.Get(c);
      if(datagram)
        {
          if(datagram->IsToSend() == tosend)
            {
              index = c;
              break;
            }
        }
    }

  if(datagramsmutex)  datagramsmutex->UnLock();

  return index;
}




/*-------------------------------------------------------------------
//  DIOSTREAMICMP::DeleteDatagram
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      30/08/2012 14:01:55
//
//  @return       bool :
//  @param        index :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMICMP::DeleteDatagram(int index)
{
  if(datagramsmutex)  datagramsmutex->Lock();

  DIOSTREAMICMPDATAGRAM* datagram = (DIOSTREAMICMPDATAGRAM*)GetDatagram(index);
  if(datagram)
    {
      datagrams.DeleteIndex(index);
      delete datagram;
    }

  if(datagramsmutex)  datagramsmutex->UnLock();

  return true;
}




//-------------------------------------------------------------------
//  DIOSTREAMICMP::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:16:00
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void DIOSTREAMICMP::Clean()
{
  config          = NULL;
  host            = NULL;
  datagramsmutex  = NULL;
}
