/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPERPUBLICIP.CPP
//
//  Public IP (Web Service)
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/11/2007 16:26:37
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XThread.h"
#include "DIOURL.h"

#include "DIOWebScraperPublicIP.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  DIOPUBLICIP::DIOPUBLICIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 19:00:57
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOPUBLICIP::DIOPUBLICIP()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOPUBLICIP::~DIOPUBLICIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 19:01:15
//
//  @return       virtual :
//  */
/*-----------------------------------------------------------------*/
DIOPUBLICIP::~DIOPUBLICIP()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOPUBLICIP::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 18:59:18
//
//  @return       DIOIP* :
//  */
/*-----------------------------------------------------------------*/
DIOIP* DIOPUBLICIP::Get()
{
  return &IP;
}




/*-------------------------------------------------------------------
//  DIOPUBLICIP::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 19:00:33
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOPUBLICIP::Clean()
{

}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERPUBLICIP::DIOWEBSCRAPERPUBLICIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/02/2014 17:22:24
//
//  @return       void :
*/
/*-----------------------------------------------------------------*/
DIOWEBSCRAPERPUBLICIP::DIOWEBSCRAPERPUBLICIP() : DIOWEBSCRAPER()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERPUBLICIP::~DIOWEBSCRAPERPUBLICIP
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/11/2007 16:28:53
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOWEBSCRAPERPUBLICIP::~DIOWEBSCRAPERPUBLICIP()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOWEBSCRAPERPUBLICIP::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/02/2014 12:48:01
//
//  @return       bool :
//  @param        IP :
//  @param        timeoutforurl :
//  @param        localIP :
//  @param        usecache :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERPUBLICIP::Get(DIOIP& IP, int timeoutforurl,  XSTRING* localIP, bool usecache)
{
  XSTRING publicIPID;
  bool    status = false;

  if(xmutexdo) xmutexdo->Lock();

  publicIPID = __L("public IP ID");

  if(usecache)
    {
      DIOPUBLICIP* publicIP = (DIOPUBLICIP*)cache->Get(publicIPID);
      if(publicIP)
        {
          XSTRING IPstring;

          publicIP->Get()->GetXString(IPstring);

          IP.Set(IPstring.Get());

          if(xmutexdo) xmutexdo->UnLock();

          return true;
        }
    }

  if(Load(DIOWEBSCRAPERPUBLICIP_NAMEFILE))
    {
      if(Do(DIOWEBSCRAPERPUBLICIP_NAMESERVICE, timeoutforurl,  localIP))
        {
          XSTRING stringIP;

          stringIP = GetValue(__L("IP"));

          if(!stringIP.IsEmpty())
            {
              stringIP.DeleteCharacter(0x20);
              if(!stringIP.IsEmpty())
                {
                  IP.Set(stringIP);

                  if(usecache)
                    {
                      DIOPUBLICIP* publicIP = new DIOPUBLICIP();
                      if(publicIP)
                        {
                          publicIP->Get()->Set(stringIP.Get());
                          cache->Add(publicIPID, publicIP);
                        }
                    }

                  status = true;
                }
            }
        }
    }

  if(xmutexdo) xmutexdo->UnLock();

  return status;
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERPUBLICIP::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      05/11/2007 16:28:12
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOWEBSCRAPERPUBLICIP::Clean()
{

}

