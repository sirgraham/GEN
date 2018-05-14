/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPERMACMANUFACTURER.CPP
//
//  Get Manufactured from MAC (Web Service)
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/11/2007 16:26:37
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XThread.h"
#include "DIOWebScraperMACManufacturer.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOMACMANUFACTUREDRESULT::DIOMACMANUFACTUREDRESULT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 19:47:38
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOMACMANUFACTUREDRESULT::DIOMACMANUFACTUREDRESULT()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOMACMANUFACTUREDRESULT::~DIOMACMANUFACTUREDRESULT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 19:47:57
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOMACMANUFACTUREDRESULT::~DIOMACMANUFACTUREDRESULT()
{
  Clean();
}


/*-------------------------------------------------------------------
//  DIOMACMANUFACTUREDRESULT::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 19:48:19
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* DIOMACMANUFACTUREDRESULT::Get()
{
  return &manufactured;
}



/*-------------------------------------------------------------------
//  DIOMACMANUFACTUREDRESULT::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      23/06/2013 19:48:36
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOMACMANUFACTUREDRESULT::Clean()
{

}





/*-------------------------------------------------------------------
//  DIOWEBSCRAPERMACMANUFACTURER::DIOWEBSCRAPERMACMANUFACTURER
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/02/2014 17:23:17
//
//  @return       void :
*/
/*-----------------------------------------------------------------*/
DIOWEBSCRAPERMACMANUFACTURER::DIOWEBSCRAPERMACMANUFACTURER() : DIOWEBSCRAPER()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERMACMANUFACTURER::~DIOWEBSCRAPERMACMANUFACTURER
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
DIOWEBSCRAPERMACMANUFACTURER::~DIOWEBSCRAPERMACMANUFACTURER()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERMACMANUFACTURER::ChangeURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      22/04/2013 18:56:55
//
//  @return       bool :
//  @param        maskurl :
//  @param        url :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERMACMANUFACTURER::ChangeURL(XCHAR* maskurl, DIOURL& url)
{
  XSTRING MACmanufactured;

  MACmanufactured.Format(__L("%02X%02X%02X"), MAC.Get()[0], MAC.Get()[1], MAC.Get()[2]);

  url.Format(maskurl, MACmanufactured.Get());

  return true;
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPERMACMANUFACTURER::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/05/2014 16:44:04
//
//  @return       bool :
//
//  @param        MAC :
//  @param        manufactured :
//  @param        timeoutforurl :
//  @param        localIP :
//  @param        usecache :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERMACMANUFACTURER::Get(DIOMAC& MAC, XSTRING& manufactured, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  XSTRING IPstring;
  bool    status = false;

  if(xmutexdo) xmutexdo->Lock();

  this->MAC.GetXString(IPstring);

  this->MAC.Set(MAC.Get());

  DIOMACMANUFACTUREDRESULT* macmanufactured;

  if(usecache)
    {
      macmanufactured = (DIOMACMANUFACTUREDRESULT*)cache->Get(IPstring);
      if(macmanufactured)
        {
          manufactured = (*macmanufactured->Get());

          if(xmutexdo) xmutexdo->UnLock();

          return true;
        }
    }

  if(Load(DIOWEBSCRAPERMACMANUFACTURER_NAMEFILE))
    {
      if(Do(DIOWEBSCRAPERMACMANUFACTURER_NAMESERVICE, timeoutforurl, localIP))
        {
          manufactured = GetValue(__L("MANUFACTURED"));

          if(!manufactured.IsEmpty())
            {
              if(usecache)
                {
                  macmanufactured = new DIOMACMANUFACTUREDRESULT();
                  if(macmanufactured)
                    {
                      macmanufactured->Get()->Set(manufactured);
                      cache->Add(IPstring, macmanufactured);
                    }
                }

              status = true;
            }

        }
    }

  if(xmutexdo) xmutexdo->UnLock();

  return status;
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERMACMANUFACTURER::Clean
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
void DIOWEBSCRAPERMACMANUFACTURER::Clean()
{

}

