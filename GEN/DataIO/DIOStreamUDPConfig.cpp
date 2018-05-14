//------------------------------------------------------------------------------------------
//  DIOSTREAMUDPCONFIG.CPP
//
//  Data IO Stream UDP Config class
//
//
//  @author  Abraham J. Velez
//  @version 12/3/2003
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include "DIOFactory.h"
#include "DIOStreamUDPConfig.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  DIOSTREAMUDPCONFIG::DIOSTREAMUDPCONFIG
*/
/**
//
//  Class Constructor DIOSTREAMUDPCONFIG
//
//  @author       Abraham J. Velez
//  @version      29/05/2014 17:29:35
//

*/
/*-----------------------------------------------------------------*/
DIOSTREAMUDPCONFIG::DIOSTREAMUDPCONFIG() : DIOSTREAMCONFIG()
{
  Clean();

  type = DIOSTREAMTYPE_UDP;

  if(diofactory) remoteURL = diofactory->CreateURL();

}


//-------------------------------------------------------------------
//  DIOSTREAMUDPCONFIG::~DIOSTREAMUDPCONFIG
/**
//
//
//  @author       Abraham J. Velez
//  @version      20/11/2003 10:15:01
//
//  @return
//  */
//-------------------------------------------------------------------
DIOSTREAMUDPCONFIG::~DIOSTREAMUDPCONFIG()
{
  if(diofactory && remoteURL) diofactory->DeleteURL(remoteURL);

  Clean();
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDPCONFIG ::GetLocalIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/05/2013 10:16:04
//
//  @return       DIOIP* :
//  */
/*-----------------------------------------------------------------*/
DIOIP* DIOSTREAMUDPCONFIG ::GetLocalIP()
{
  return &localIP;
}



/*-------------------------------------------------------------------
//  DIOSTREAMUDPCONFIG ::GetRemoteURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/05/2013 10:21:54
//
//  @return       DIOURL* :
//  */
/*-----------------------------------------------------------------*/
DIOURL* DIOSTREAMUDPCONFIG ::GetRemoteURL()
{
  if(diofactory && !remoteURL) remoteURL = diofactory->CreateURL();

  return remoteURL;
}


/*-------------------------------------------------------------------
//  DIOSTREAMUDPCONFIG ::GetRemotePort
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/05/2013 9:54:32
//
//  @return       int :
//  */
/*-----------------------------------------------------------------*/
int DIOSTREAMUDPCONFIG ::GetRemotePort()
{
  return remoteport;
}




/*-------------------------------------------------------------------
//  DIOSTREAMUDPCONFIG::SetRemotePort
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      26/05/2013 9:54:52
//
//  @return       bool :
//  @param        port :
*/
/*-----------------------------------------------------------------*/
bool DIOSTREAMUDPCONFIG::SetRemotePort(int port)
{
  this->remoteport = port;

  return true;
}




