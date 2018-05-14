/*------------------------------------------------------------------------------------------
//  DIOWEBSCRAPERSEXNAME.CPP
//
//  Sex Name (Web Service)
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 05/11/2007 16:26:37
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XThread.h"

#include "DIOWebScraperSexName.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  DIOSEXNAMERESULT::DIOSEXNAMERESULT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/06/2013 18:36:13
//
//  @return
//  */
/*-----------------------------------------------------------------*/
DIOSEXNAMERESULT::DIOSEXNAMERESULT()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOSEXNAMERESULT::~DIOSEXNAMERESULT
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/06/2013 18:35:49
//
//  @return       virtual :
//  */
/*-----------------------------------------------------------------*/
DIOSEXNAMERESULT::~DIOSEXNAMERESULT()
{
  Clean();
}



/*-------------------------------------------------------------------
//  DIOSEXNAMERESULT::Get
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/06/2013 18:35:33
//
//  @return       DIOWEBSCRAPERSEXNAMETYPE :
//  */
/*-----------------------------------------------------------------*/
DIOWEBSCRAPERSEXNAMETYPE DIOSEXNAMERESULT::Get()
{
  return sex;
}



/*-------------------------------------------------------------------
//  DIOSEXNAMERESULT::Set
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/06/2013 18:35:17
//
//  @return       bool :
//  @param        sex :
*/
/*-----------------------------------------------------------------*/
bool DIOSEXNAMERESULT::Set(DIOWEBSCRAPERSEXNAMETYPE sex)
{
  this->sex = sex;

  return true;
}




/*-------------------------------------------------------------------
//  DIOSEXNAMERESULT::GetNameClear
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/06/2013 18:54:26
//
//  @return       XSTRING* :
//  */
/*-----------------------------------------------------------------*/
XSTRING* DIOSEXNAMERESULT::GetNameClear()
{
  return &nameclear;
}




/*-------------------------------------------------------------------
//  DIOSEXNAMERESULT::Clean
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/06/2013 18:35:05
//
//  @return       void :
//  */
/*-----------------------------------------------------------------*/
void DIOSEXNAMERESULT::Clean()
{
  sex = DIOWEBSCRAPERSEXNAMETYPE_NOTOBTAINED;
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERSEXNAME::DIOWEBSCRAPERSEXNAME
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      08/02/2014 17:20:57
//
//  @return
*/
/*-----------------------------------------------------------------*/
DIOWEBSCRAPERSEXNAME::DIOWEBSCRAPERSEXNAME(  ) : DIOWEBSCRAPER()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERSEXNAME::~DIOWEBSCRAPERSEXNAME
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
DIOWEBSCRAPERSEXNAME::~DIOWEBSCRAPERSEXNAME()
{
  Clean();
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERSEXNAME::ChangeURL
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      02/04/2013 22:29:13
//
//  @return       bool :
//  @param        maskurl :
//  @param        url :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERSEXNAME::ChangeURL(XCHAR* maskurl, DIOURL& url)
{
  url.Format(maskurl, name.Get());

  return true;
}



/*-------------------------------------------------------------------
//  DIOWEBSCRAPERSEXNAME::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/05/2014 16:43:52
//
//  @return       bool :
//
//  @param        name :
//  @param        sexnametype :
//  @param        nameclear :
//  @param        timeoutforurl :
//  @param        localIP :
//  @param        usecache :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERSEXNAME::Get(XCHAR* name, DIOWEBSCRAPERSEXNAMETYPE& sexnametype, XSTRING* nameclear, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  bool status = false;

  if(xmutexdo) xmutexdo->Lock();

  this->name = name;

  DIOSEXNAMERESULT* sexnameresult;
  if(usecache)
    {
      sexnameresult = (DIOSEXNAMERESULT*)cache->Get(this->name);
      if(sexnameresult)
        {
          sexnametype = sexnameresult->Get();
          if(nameclear) nameclear->Set(sexnameresult->GetNameClear()->Get());

          if(xmutexdo) xmutexdo->UnLock();

          return true;
        }
    }

  sexnametype = DIOWEBSCRAPERSEXNAMETYPE_NOTOBTAINED;
  if(this->name.IsEmpty())
    {
      if(Load(DIOWEBSCRAPERSEXNAME_NAMEFILE))
        {
          if(Do(DIOWEBSCRAPERSEXNAME_NAMESERVICE, timeoutforurl, localIP))
            {
              sexnametype = DIOWEBSCRAPERSEXNAMETYPE_NONAME;

              XSTRING sexnamestr;

              sexnamestr = GetValue(__L("SEX"));
              if(!sexnamestr.Compare(__L("male")))    sexnametype = DIOWEBSCRAPERSEXNAMETYPE_MALE;
              if(!sexnamestr.Compare(__L("female")))  sexnametype = DIOWEBSCRAPERSEXNAMETYPE_FEMALE;

              if(nameclear)
                {
                  if((sexnametype == DIOWEBSCRAPERSEXNAMETYPE_MALE) || (sexnametype == DIOWEBSCRAPERSEXNAMETYPE_FEMALE))
                        (*nameclear) = GetValue(__L("NAME"));
                   else (*nameclear) = this->name.Get();
                }

              if(usecache)
                {
                  sexnameresult = new DIOSEXNAMERESULT();
                  if(sexnameresult)
                    {
                      if(nameclear) sexnameresult->GetNameClear()->Set(nameclear->Get());
                      sexnameresult->Set(sexnametype);

                      cache->Add(this->name, sexnameresult);
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
//  DIOWEBSCRAPERSEXNAME::Get
*/
/**
//
//
//
//  @author       Abraham J. Velez
//  @version      12/05/2014 17:00:59
//
//  @return       bool :
//
//  @param        name :
//  @param        sexnametype :
//  @param        nameclear :
//  @param        timeoutforurl :
//  @param        localIP :
//  @param        usecache :
*/
/*-----------------------------------------------------------------*/
bool DIOWEBSCRAPERSEXNAME::Get(XSTRING& name, DIOWEBSCRAPERSEXNAMETYPE& sexnametype, XSTRING* nameclear, int timeoutforurl, XSTRING* localIP, bool usecache)
{
  return Get(name.Get(), sexnametype, nameclear, timeoutforurl, localIP, usecache);
}




/*-------------------------------------------------------------------
//  DIOWEBSCRAPERSEXNAME::Clean
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
void DIOWEBSCRAPERSEXNAME::Clean()
{

}

