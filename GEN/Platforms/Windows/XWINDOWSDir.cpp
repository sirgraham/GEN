//------------------------------------------------------------------------------------------
//  XWINDOWSDIR.CPP
//
//  WINDOWS Dir class
//
//  Author            : Abraham J. Velez
//  Date Of Creation  : 29/10/2004 10:16:15
//  Last Mofificacion :
//
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XString.h"
#include "XPath.h"
#include "XDebug.h"

#include "XWINDOWSDir.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


/*-------------------------------------------------------------------
//  XWINDOWSDIR::XWINDOWSDIR
*/
/**
//
//  Class Constructor XWINDOWSDIR
//
//  @author       Abraham J. Velez
//  @version      26/05/2014 11:01:50
//

*/
/*-----------------------------------------------------------------*/
XWINDOWSDIR::XWINDOWSDIR() : XDIR()
{
  Clean();
}


//-------------------------------------------------------------------
//  XWINDOWSDIR::~XWINDOWSDIR
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/10/2004 10:17:05
//
//  @return
//  */
//-------------------------------------------------------------------
XWINDOWSDIR::~XWINDOWSDIR()
{
  Clean();
}



/*-------------------------------------------------------------------
//  XWINDOWSDIR::Exist
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      19/04/2008 21:40:37
//
//  @return       bool :
//  @param        path :
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSDIR::Exist(XCHAR* path)
{
  XDIRELEMENT search;
  XPATH       xpath;
  XSTRING     patternsearch;
  bool        status = false;

  xpath = path;
  xpath.Slash_Delete();
  xpath.GetNamefileExt(patternsearch);
  xpath.SetOnlyDriveAndPath();

  if(xpath.IsEmpty())
    {
      XDWORD size = _MAXPATH;
      xpath.AdjustSize(size, false, __L(" "));

      GetCurrentDirectory(size,xpath.Get());
    }

  if(FirstSearch(xpath.Get(),patternsearch.Get(),&search))
    {
      /*
      if(search.GetType()==XDIRELEMENTTYPE_DIR)
        {
          NextSearch(&search);
          status = true;

        } else
      */

      status = true;

      while(NextSearch(&search))
        {

        }
    }

  return status;
}


//-------------------------------------------------------------------
//  XWINDOWSDIR::Make
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/10/2004 10:20:39
//
//  @return       bool :
//  @param        path :
*/
//-------------------------------------------------------------------
bool XWINDOWSDIR::Make(XCHAR* path)
{
  if(!path)   return false;
  if(!path[0]) return false;

  if(Exist(path)) return true;

  XPATH   xpath;
  XPATH   xpathsequence;
  XSTRING pathpart;
  int     index = 0;

  xpathsequence.Empty();

  xpath = path;

  xpath.Slash_Normalize(true);

  do{
      if(xpath.GetPathInSequence(index,pathpart))
        {
          xpathsequence += pathpart;
          xpathsequence += __L("\\");

          if(pathpart.Character_GetLast()!=__C(':'))
            {
              if(!Exist(xpathsequence.Get()))
                {
                  //XDEBUG_PRINTCOLOR(2, __L("---> Make: %s"), xpathsequence.Get());

                  int result = CreateDirectory(xpathsequence.Get(), NULL);
                  if(!result) return false;
                }
            }

        } else break;

      index++;

    } while(1);

  return true;
}



/*-------------------------------------------------------------------
//  XWINDOWSDIR::ChangeTo
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      28/03/2009 09:43:57 p.m.
//
//  @return       bool :
//  @param        path :
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSDIR::ChangeTo(XCHAR* path)
{
  if(!path)    return false;
  if(!path[0]) return false;

  int result = SetCurrentDirectory(path);

  return result?true:false;
}



/*-------------------------------------------------------------------
//  XWINDOWSDIR::Delete
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/02/2012 10:02:03
//
//  @return       bool :
//  @param        path :
//  @param        all :
*/
/*-----------------------------------------------------------------*/
bool XWINDOWSDIR::Delete(XCHAR* path,bool all)
{
  XDIRELEMENT search;
  XPATH       xpathname;
  XSTRING patternsearch;

  if(all)
    {
      xpathname     = path;
      patternsearch = __L("*");

      if(FirstSearch(xpathname.Get(),patternsearch.Get(),&search))
        {
          do { xpathname  = path;
               xpathname.Slash_Add();
               xpathname += search.GetNameFile()->Get();

               if(search.GetType() == XDIRELEMENTTYPE_DIR)
                     Delete(xpathname.Get(),all);
                else DeleteFile(xpathname.Get());

             } while(NextSearch(&search));
        }
    }

  BOOL result = RemoveDirectory(path);

  return result?true:false;
}




//-------------------------------------------------------------------
//  XWINDOWSDIR::FirstSearch
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/10/2004 13:05:07
//
//  @return       bool :
//  @param        xpath :
//  @param        patternsearch :
//  @param        searchelement :
*/
//-------------------------------------------------------------------
bool XWINDOWSDIR::FirstSearch(XCHAR* xpath,XCHAR* patternsearch,XDIRELEMENT* searchelement)
{
  if(!xpath)         return false;
  if(!patternsearch) return false;

  XBYTE* findfiledata = new XBYTE[sizeof(WIN32_FIND_DATA)];
  if(!findfiledata) return false;

  memset(findfiledata,0,sizeof(WIN32_FIND_DATA));

  searchelement->SetFindFileData(findfiledata);

  searchelement->GetPathSearch()->Set(xpath);

  XPATH pathsearch;

  pathsearch  = xpath;
  pathsearch.Slash_Add();
  pathsearch += patternsearch;

  WIN32_FIND_DATA* search = (WIN32_FIND_DATA*)searchelement->GetFindFileData();

  searchelement->SetHandle((void*)FindFirstFile(pathsearch.Get() , search));
  if((HANDLE)(searchelement->GetHandle()) == INVALID_HANDLE_VALUE)
    {
      searchelement->SetHandle((void*)-1);
      searchelement->DeleteFindFileData();

      return false;
    }

  XSTRING searchfilename(search->cFileName);

  if((!searchfilename.Compare(__L(".")))||
     (!searchfilename.Compare(__L(".."))))
    {
      return NextSearch(searchelement);
    }

  ReconvertSearchData(searchelement);

  return true;
}



//-------------------------------------------------------------------
//  XWINDOWSDIR::NextSearch
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/10/2004 13:06:10
//
//  @return       bool :
//  @param        searchelement :
*/
//-------------------------------------------------------------------
bool XWINDOWSDIR::NextSearch(XDIRELEMENT* searchelement)
{
  WIN32_FIND_DATA* search = (WIN32_FIND_DATA*)searchelement->GetFindFileData();
  HANDLE           handle = (HANDLE)searchelement->GetHandle();

  if(!FindNextFile(handle,search))
    {
      FindClose(handle);

      searchelement->SetHandle((void*)-1);
      searchelement->DeleteFindFileData();

      return false;
    }

   XSTRING searchfilename(search->cFileName);

   if((!searchfilename.Compare(__L(".")))||
      (!searchfilename.Compare(__L(".."))))
     {
       return NextSearch(searchelement);
     }

  ReconvertSearchData(searchelement);

  return true;
}



//-------------------------------------------------------------------
//  XWINDOWSDIR::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/10/2004 10:17:30
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void XWINDOWSDIR::Clean()
{

}


//-------------------------------------------------------------------
//  XWINDOWSDIR::ReconvertSearchData
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/10/2004 13:30:00
//
//  @return       bool :
//  @param        searchelement :
*/
//-------------------------------------------------------------------
bool XWINDOWSDIR::ReconvertSearchData(XDIRELEMENT* searchelement)
{
  if(!searchelement) return false;

  WIN32_FIND_DATA* search =(WIN32_FIND_DATA*)searchelement->GetFindFileData();

  searchelement->GetNameFile()->Set(search->cFileName);

  if(search->dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
          searchelement->SetType(XDIRELEMENTTYPE_DIR);
    else  searchelement->SetType(XDIRELEMENTTYPE_FILE);

  return true;
}
