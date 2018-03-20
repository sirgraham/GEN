//------------------------------------------------------------------------------------------
//	DIOSTREAMUDP.CPP
//	
//	Data IO Stream UDP class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"
#include "XBuffer.h"
#include "XThread.h"

#include "DIOFactory.h"
#include "DIOStreamUDPConfig.h"

#include "DIOStreamUDP.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------



/*-------------------------------------------------------------------
//  DIOSTREAMUDP::DIOSTREAMUDP
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			18/02/2013 7:40:46
//	
//	@return 			

 
 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMUDP::DIOSTREAMUDP() : DIOSTREAM()
{
	Clean();

	datagramsmutex = xfactory->Create_Mutex();
	host					 = diofactory->CreateURL();
}



//-------------------------------------------------------------------
//  DIOSTREAMUDP::~DIOSTREAMUDP
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:15:51
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
DIOSTREAMUDP::~DIOSTREAMUDP()
{ 
	DeleteAllDatagrams();

	if(diofactory) diofactory->DeleteURL(host);	
	xfactory->Delete_Mutex(datagramsmutex);
			
	Clean();
}



//-------------------------------------------------------------------
//  DIOSTREAMUDP::GetConfig
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			14/09/2004 14:32:49
//	
//	@return				DIOSTREAMCONFIG* : 
//	*/
//-------------------------------------------------------------------
DIOSTREAMCONFIG* DIOSTREAMUDP::GetConfig()
{
	return config;
}




//-------------------------------------------------------------------
//  DIOSTREAMUDP::SetConfig
/**
//
//
//	@author				Abraham J. Velez
//	@version			03/09/2001 16:58:17
//
//	@return 			bool :
//  @param				config :
*/
//-------------------------------------------------------------------
bool DIOSTREAMUDP::SetConfig(DIOSTREAMCONFIG* config)
{	
	if(!config) return false;
	this->config = (DIOSTREAMUDPCONFIG *)config;
	
	return true;
}



/*-------------------------------------------------------------------
//	DIOSTREAMUDP::ReadDatagram
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2015 12:21:17
//	
//	@return 			bool : 
//
//  @param				address : 
//  @param				port : 
//  @param				xbuffer : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDP::ReadDatagram(XSTRING& address, XWORD& port, XBUFFER& xbuffer)
{
	int index = GetFirstDatagram(false);
	if(index != DIOSTREAMUDP_NOTFOUND)
		{
			DIOSTREAMUDPDATAGRAM* datagram = GetDatagram(index);
			if(datagram)				
				{
					address = datagram->GetAddress()->Get();
					port    = datagram->GetPort();

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
//	DIOSTREAMUDP::WriteDatagram
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2015 12:22:34
//	
//	@return 			bool : 
//
//  @param				address : 
//  @param				port : 
//  @param				buffer : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDP::WriteDatagram(XSTRING& address, XWORD port, XBYTE* buffer, int size)
{
	if(address.IsEmpty())																	return false;
	if(!AddDatagram(true, address , port, buffer, size))	return false;
	if(!outbuffer->Add(buffer,size))											return false;

	return true;
}




/*-------------------------------------------------------------------
//	DIOSTREAMUDP::WriteDatagram
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2015 12:23:02
//	
//	@return 			bool : 
//
//  @param				address : 
//  @param				port : 
//  @param				xbuffer : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDP::WriteDatagram(XSTRING& address, XWORD port, XBUFFER& xbuffer)
{	
	return WriteDatagram(address, port, xbuffer.Get(), xbuffer.GetSize());
}



/*-------------------------------------------------------------------
//	DIOSTREAMUDP::ResetXBuffers
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			16/11/2014 16:00:37
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDP::ResetXBuffers()
{
	if(!DIOSTREAM::ResetXBuffers()) return false;

	return DeleteAllDatagrams();
}
		


/*-------------------------------------------------------------------
//	DIOSTREAMUDP::ResetInXBuffer
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			17/11/2014 20:46:44
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDP::ResetInXBuffer()
{		
	if(!DIOSTREAM::ResetInXBuffer()) return false;

	int index = 0;

	while(1)							
		{
			DIOSTREAMUDPDATAGRAM* datagram = (DIOSTREAMUDPDATAGRAM*)datagrams.Get(index);
			if(!datagram) break;

			if(datagram->GetAddress()->GetSize()) 
				{
					DeleteDatagram(index);

				} else index++;
		}


	return true;
}
	




/*-------------------------------------------------------------------
//	DIOSTREAMUDP::ResetOutXBuffer
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			17/11/2014 20:46:55
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDP::ResetOutXBuffer()
{	
	if(!DIOSTREAM::ResetOutXBuffer()) return false;

	int index = 0;

	while(1)							
		{
			DIOSTREAMUDPDATAGRAM* datagram = (DIOSTREAMUDPDATAGRAM*)datagrams.Get(index);
			if(!datagram) break;

			if(!datagram->GetAddress()->GetSize()) 
				{
					DeleteDatagram(index);

				} else index++;
		}

	return true;
}



/*-------------------------------------------------------------------
//  DIOSTREAMUDP::GetHost
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/04/2013 23:11:27
//	
//	@return 			DIOURL* : 
//	*/
/*-----------------------------------------------------------------*/
DIOURL* DIOSTREAMUDP::GetHost()
{
	return host;
}



/*-------------------------------------------------------------------
//  DIOSTREAMUDP::WaitToGetFirstDatagram
*/
/**
//
//
//	@author				Abraham J. Velez
//	@version			30/08/2012 14:07:34
//
//	@return 			bool :
//	@param				tosend :
//	@param				timeout :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDP::WaitToWriteDatagramsEmpty(int timeout)
{
	if (!xtimerconnexion) return false;

	int index;

	xtimerconnexion->Reset();

	while(1)
		{
			index = GetFirstDatagram(true);
			if(index == DIOSTREAMUDP_NOTFOUND)
				{
					return true;
				}

			if((int)xtimerconnexion->GetMeasureSeconds() >= timeout) break;

			Wait(DIOSTREAM_TIMEINWAITFUNCTIONS);
		}

	return false;
}





/*-------------------------------------------------------------------
//  DIOSTREAMUDP::WaitToGetFirstDatagram
*/
/**
//
//
//	@author				Abraham J. Velez
//	@version			30/08/2012 14:07:34
//
//	@return 			bool :
//	@param				tosend :
//	@param				timeout :
*/
/*-----------------------------------------------------------------*/
int DIOSTREAMUDP::WaitToGetFirstDatagram(bool tosend, int timeout)
{
	if (!xtimerconnexion) return false;

	int index;

	xtimerconnexion->Reset();

	while (1)
		{
			index = GetFirstDatagram(tosend);
			if(index != DIOSTREAMUDP_NOTFOUND)
				{
					break;
				}

			if((int)xtimerconnexion->GetMeasureSeconds() >= timeout) break;

			Wait(DIOSTREAM_TIMEINWAITFUNCTIONS);
		}

	return index;
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDP::DeleteDatagram
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/08/2012 14:01:55
//	
//	@return 			bool : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDP::DeleteDatagram(int index)
{	
	if(datagramsmutex)	datagramsmutex->Lock();

	DIOSTREAMUDPDATAGRAM* datagram = (DIOSTREAMUDPDATAGRAM*)GetDatagram(index);
	if(datagram)
		{
			datagrams.DeleteIndex(index);
			delete datagram;		
			datagram = NULL;
		}

	if(datagramsmutex)	datagramsmutex->UnLock();

	return datagram?true:false;
}



/*-------------------------------------------------------------------
//  DIOSTREAMUDP::DeleteAllDatagrams
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/08/2012 14:00:01
//	
//	@return 			bool : 
//	*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDP::DeleteAllDatagrams()
{
	if(datagrams.IsEmpty()) return false;

	if(datagramsmutex)	datagramsmutex->Lock();
	
	datagrams.DeleteContents();

  datagrams.DeleteAll();

	if(datagramsmutex)	datagramsmutex->UnLock();
	
	return true;
}



/*-------------------------------------------------------------------
//	DIOSTREAMUDP::AddDatagram
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2015 12:24:13
//	
//	@return 			bool : 
//
//  @param				istosend : 
//  @param				address : 
//  @param				port : 
//  @param				data : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDP::AddDatagram(bool istosend, XCHAR* address, XWORD port, XBYTE* data, int size)
{
	if(!address) return false;
	if(!data)		 return false;
	
	if(datagramsmutex)	datagramsmutex->Lock();

	DIOSTREAMUDPDATAGRAM* datagram = new DIOSTREAMUDPDATAGRAM();
	if(datagram) 
		{
			datagram->SetIsToSend(istosend);
			datagram->SetAddress(address);
			datagram->SetPorts(port);
			datagram->SetData(data,size);

			datagrams.Add(datagram);
		}

	if(datagramsmutex)	datagramsmutex->UnLock();

	return datagram?true:false;
}



/*-------------------------------------------------------------------
//	DIOSTREAMUDP::AddDatagram
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			08/03/2015 12:23:33
//	
//	@return 			bool : 
//
//  @param				istosend : 
//  @param				address : 
//  @param				port : 
//  @param				data : 
//  @param				size : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDP::AddDatagram(bool istosend, XSTRING& address, XWORD port, XBYTE* data,int size)
{
	return AddDatagram(istosend, address.Get(), port, data, size);
}



/*-------------------------------------------------------------------
//  DIOSTREAMUDP::GetDatagram
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/08/2012 14:08:54
//	
//	@return 			DIOSTREAMUDPDATAGRAM* : 
//	@param				index : 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMUDPDATAGRAM* DIOSTREAMUDP::GetDatagram(int index)
{
	return (DIOSTREAMUDPDATAGRAM*)datagrams.Get(index);
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDP::GetFirstDatagram
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/08/2012 14:07:34
//	
//	@return 			bool : 
//	@param				issend : 
*/
/*-----------------------------------------------------------------*/
int DIOSTREAMUDP::GetFirstDatagram(bool tosend)
{
	int index = DIOSTREAMUDP_NOTFOUND;

	if(datagramsmutex)	datagramsmutex->Lock();

	for(int c=0;c<(int)datagrams.GetSize();c++)
		{
			DIOSTREAMUDPDATAGRAM* datagram = (DIOSTREAMUDPDATAGRAM*)datagrams.Get(c);
			if(datagram)
				{
					if(datagram->IsToSend() == tosend)
						{
							index = c;
							break;						
						}					 
				}
		}

	if(datagramsmutex)	datagramsmutex->UnLock();

	return index;
}





//-------------------------------------------------------------------
//  DIOSTREAMUDP::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:16:00
//	
//	@return 			void : 
//	*/
//-------------------------------------------------------------------
void DIOSTREAMUDP::Clean()
{
	config					= NULL;		
	host						= NULL;	
	datagramsmutex	= NULL;	
}
