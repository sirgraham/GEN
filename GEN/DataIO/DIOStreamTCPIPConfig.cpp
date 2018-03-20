//------------------------------------------------------------------------------------------
//	DIOSTREAMTCPIP.CPP
//	
//	Data IO Stream TCP/IP Config class
//	
// 
//	@author	 Abraham J. Velez
//	@version 12/3/2003  
//	
//	GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include "XFactory.h"

#include "DIOFactory.h"

#include "DIOStreamTCPIPConfig.h"

#include "XMemory.h"
	
//---- GENERAL VARIABLE --------------------------------------------------------------------
	
	
//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//	::DIOSTREAMTCPIPCONFIG
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			29/05/2014 17:30:01
//	
//	@return 			DIOSTREAMTCPIPCONFIG : 
//
 
*/
/*-----------------------------------------------------------------*/
DIOSTREAMTCPIPCONFIG ::DIOSTREAMTCPIPCONFIG () : DIOSTREAMCONFIG()
{
	Clean();

	type  = DIOSTREAMTYPE_TCPIP;

	remoteURL = diofactory->CreateURL();

	xmutexmultiserver = xfactory->Create_Mutex();





}


//-------------------------------------------------------------------
//  DIOSTREAMTCPIPCONFIG ::~DIOSTREAMTCPIPCONFIG 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			20/11/2003 10:15:01
//	
//	@return 			
//	*/
//-------------------------------------------------------------------
DIOSTREAMTCPIPCONFIG ::~DIOSTREAMTCPIPCONFIG ()
{
	if(xmutexmultiserver) xfactory->Delete_Mutex(xmutexmultiserver);

	if(diofactory && remoteURL) diofactory->DeleteURL(remoteURL);

	Clean();	
}



/*-------------------------------------------------------------------
//  DIOSTREAMTCPIPCONFIG ::GetLocalIP
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/05/2013 10:16:04
//	
//	@return 			DIOIP* : 
//	*/
/*-----------------------------------------------------------------*/
DIOIP* DIOSTREAMTCPIPCONFIG ::GetLocalIP()
{
	return &localIP;
}



/*-------------------------------------------------------------------
//  DIOSTREAMTCPIPCONFIG ::GetRemoteURL
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/05/2013 10:21:54
//	
//	@return 			DIOURL* : 
//	*/
/*-----------------------------------------------------------------*/
DIOURL* DIOSTREAMTCPIPCONFIG ::GetRemoteURL()
{
	return remoteURL;
}


/*-------------------------------------------------------------------
//  DIOSTREAMTCPIPCONFIG ::GetRemotePort
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/05/2013 9:54:32
//	
//	@return 			int : 
//	*/
/*-----------------------------------------------------------------*/
int DIOSTREAMTCPIPCONFIG ::GetRemotePort()
{
	return remoteport;
}




/*-------------------------------------------------------------------
//  DIOSTREAMTCPIPCONFIG ::SetRemotePort
*/ 
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			26/05/2013 9:54:52
//	
//	@return 			bool : 
//	@param				port : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMTCPIPCONFIG ::SetRemotePort(int port)
{
	this->remoteport = port;

	return true;
}




/*-------------------------------------------------------------------
//	DIOSTREAMTCPIPCONFIG::GetToString
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			24/02/2016 9:19:10
//	
//	@return 			bool : 
//
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMTCPIPCONFIG::GetToString(XSTRING* string)
{
	if(!string)			return false;
	if(!remoteURL)	return false;
	
	string->Format(__L("%s:%d"), remoteURL->Get(), remoteport);
	
	return true;
}





/*-------------------------------------------------------------------
//	DIOSTREAMTCPIPCONFIG::SetFromString
*/	
/**	
//	
//	
//	
//	@author				Abraham J. Velez
//	@version			24/02/2016 9:19:43
//	
//	@return 			bool : 
//
//  @param				string : 
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMTCPIPCONFIG::SetFromString(XCHAR* string)
{
	if(!string) return false;

	DIOURL  _string;
	DIOURL  urlall;
	XSTRING url;
	XSTRING urladd;
	XSTRING login;
	XSTRING password;
	int     endurlaccess;
	int     port = 0;

	remoteURL->Empty();
	
	_string = string;
		
	if(_string.HaveHTTPID()) _string.DeleteHTTPID();

	endurlaccess = _string.Find(__L("/"), true);
	if(endurlaccess == XSTRING_NOTFOUND) endurlaccess = _string.Find(__L("\\"), true);
	if(endurlaccess == XSTRING_NOTFOUND) endurlaccess = _string.GetSize();

	_string.Copy(endurlaccess+1, urladd);
	_string.Copy(0, endurlaccess, urlall);
	urlall.Slash_Delete();

	int havepassword = urlall.Find(__L("@"), true);
	int haveport		 = urlall.Find(__L(":"), true, (havepassword == XSTRING_NOTFOUND)?0:havepassword);

	login.AdjustSize(_MAXSTR*10);
	password.AdjustSize(_MAXSTR*10);
	url.AdjustSize(_MAXSTR*10);
	
	if(haveport== XSTRING_NOTFOUND)
		{
			if(havepassword == XSTRING_NOTFOUND)	
							urlall.UnFormat(__L("%s"), url.Get());
				 else urlall.UnFormat(__L("%s:%s@%s"), login.Get(), password.Get(), url.Get());
		}
	 else
	  {
			if(havepassword == XSTRING_NOTFOUND)	
							urlall.UnFormat(__L("%s:%d"), url.Get(), &port);
				 else urlall.UnFormat(__L("%s:%s@%s:%d"), login.Get(), password.Get(), url.Get(), &port);
		}
		
	login.AdjustSize();
	password.AdjustSize();
	url.AdjustSize();
	
	if(remoteURL) 
		{
			if(!login.IsEmpty() || !password.IsEmpty())  remoteURL->AddFormat(__L("%s:%s@"), login.Get(), password.Get());
				
			remoteURL->Add(url);
			if(urladd.Get()[0])
				{
					remoteURL->Slash_Add();
					remoteURL->Add(urladd);
				}
		}

	if(port) SetRemotePort(port);
	
	return true;
}

