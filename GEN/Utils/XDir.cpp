//------------------------------------------------------------------------------------------
//  XDIR.CPP
//
//  File Dir class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 29/10/2004 10:16:15
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>

#include "XFactory.h"
#include "XFile.h"
#include "XDir.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------



//-------------------------------------------------------------------
//  XDIR::XDIR
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/10/2004 10:16:41
//
//  @return
//  */
//-------------------------------------------------------------------
XDIR::XDIR()
{
  Clean();
}


//-------------------------------------------------------------------
//  XDIR::~XDIR
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/10/2004 10:17:05
//
//  @return
//  */
//-------------------------------------------------------------------
XDIR::~XDIR()
{
  Clean();
}



/*-------------------------------------------------------------------
//  XDIR::Exist
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/04/2011 9:55:35
//
//  @return       bool :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool XDIR::Exist(XPATH& xpath)
{
  return Exist(xpath.Get());
}



/*-------------------------------------------------------------------
//  XDIR::Make
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/04/2011 9:56:13
//
//  @return       bool :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool XDIR::Make(XPATH& xpath)
{
  return Make(xpath.Get());
}



/*-------------------------------------------------------------------
//  XDIR::ChangeTo
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/04/2011 9:56:28
//
//  @return       bool :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool XDIR::ChangeTo(XPATH& xpath)
{
  return ChangeTo(xpath.Get());
}



/*-------------------------------------------------------------------
//  XDIR::Delete
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      13/04/2011 9:56:32
//
//  @return       bool :
//  @param        xpath :
*/
/*-----------------------------------------------------------------*/
bool XDIR::Delete(XPATH& xpath,bool all)
{
  return Delete(xpath.Get(),all);
}



/*-------------------------------------------------------------------
//  XDIR::FirstSearch
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/04/2011 11:37:22
//
//  @return       bool :
//  @param        xpath :
//  @param        patternsearch :
//  @param        searchelement :
*/
/*-----------------------------------------------------------------*/
bool XDIR::FirstSearch(XPATH& xpath,XSTRING& patternsearch,XDIRELEMENT* searchelement)
{
  return FirstSearch(xpath.Get(),patternsearch.Get(),searchelement);
}



/*-------------------------------------------------------------------
//  XDIR::FirstSearch
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/04/2011 11:38:05
//
//  @return       bool :
//  @param        xpath :
//  @param        patternsearch :
//  @param        searchelement :
*/
/*-----------------------------------------------------------------*/
bool XDIR::FirstSearch(XPATH& xpath,XCHAR* patternsearch,XDIRELEMENT* searchelement)
{
  return FirstSearch(xpath.Get(),patternsearch,searchelement);
}



/*-------------------------------------------------------------------
//  XDIR::FirstSearch
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/04/2011 11:38:52
//
//  @return       bool :
//  @param        xpath :
//  @param        patternsearch :
//  @param        searchelement :
*/
/*-----------------------------------------------------------------*/
bool XDIR::FirstSearch(XCHAR* xpath,XSTRING& patternsearch,XDIRELEMENT* searchelement)
{
  return FirstSearch(xpath,patternsearch.Get(),searchelement);
}



/*-------------------------------------------------------------------
//  XDIR::Copy
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      18/03/2008 13:05:46
//
//  @return       bool :

//  @param        xpathsource :
//  @param        xpathtarget :
*/
/*-----------------------------------------------------------------*/
bool XDIR::Copy(XPATH& xpathsource,XPATH& xpathtarget)
{
  #define COPYFILE_MAXBUFFER 2048

  bool status = true;

  XFILE* fsource = xfactory->Create_File();
  XFILE* ftarget = xfactory->Create_File();

  if(fsource && ftarget)
    {
      if(fsource->Open(xpathsource))
        {
          if(ftarget->Create(xpathtarget))
            {
              XBYTE* buffer = new XBYTE[COPYFILE_MAXBUFFER];
              if(buffer)
                {
                  XDWORD rsize  = 0;

                  do { rsize = COPYFILE_MAXBUFFER;

                       memset(buffer,0,rsize);

                       fsource->Read(buffer,&rsize);

                       if(rsize) ftarget->Write(buffer,rsize);

                     } while(rsize == COPYFILE_MAXBUFFER);

                  delete[] buffer;
                }

              ftarget->Close();

            } else status = false;

          fsource->Close();

        } else status = false;

    } else status = false;

  xfactory->Delete_File(fsource);
  xfactory->Delete_File(ftarget);

  return status;
}



/*-------------------------------------------------------------------
//  XDIR::Copy
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/04/2011 11:42:06
//
//  @return       bool :

//  @param        xpathsource :
//  @param        xpathtarget :
*/
/*-----------------------------------------------------------------*/
bool XDIR::Copy(XCHAR* xpathsource,XPATH& xpathtarget)
{
  XPATH _xpathsource;

  _xpathsource = xpathsource;

  return Copy(_xpathsource, xpathtarget);
}




/*-------------------------------------------------------------------
//  XDIR::Copy
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/04/2011 11:42:23
//
//  @return       bool :

//  @param        xpathsource :
//  @param        xpathtarget :
*/
/*-----------------------------------------------------------------*/
bool XDIR::Copy(XPATH& xpathsource,XCHAR* xpathtarget)
{
  XPATH _xpathtarget;

  _xpathtarget = xpathtarget;

  return Copy(xpathsource, _xpathtarget);
}




/*-------------------------------------------------------------------
//  XDIR::Copy
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      14/04/2011 11:42:41
//
//  @return       bool :

//  @param        xpathsource :
//  @param        xpathtarget :
*/
/*-----------------------------------------------------------------*/
bool XDIR::Copy(XCHAR* xpathsource,XCHAR* xpathtarget)
{
  XPATH _xpathsource;
  XPATH _xpathtarget;

  _xpathsource = xpathsource;
  _xpathtarget = xpathtarget;

  return Copy(_xpathsource, _xpathtarget);

}

