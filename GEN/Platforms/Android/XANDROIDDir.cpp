//------------------------------------------------------------------------------------------
//  XANDROIDDIR.CPP
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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fnmatch.h>
#include <dirent.h>
#include <ctype.h>

#include "XANDROIDFactory.h"
#include "XPath.h"
#include "XBuffer.h"
#include "XANDROIDDir.h"

#include "XMemory.h"

//---- GENERAL VARIABLE --------------------------------------------------------------------


//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  XANDROIDDIR::XANDROIDDIR
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/10/2004 10:16:41
//
//  @return
//  */
//-------------------------------------------------------------------
XANDROIDDIR::XANDROIDDIR() : XDIR()
{
  Clean();
}


//-------------------------------------------------------------------
//  XANDROIDDIR::~XANDROIDDIR
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/10/2004 10:17:05
//
//  @return
//  */
//-------------------------------------------------------------------
XANDROIDDIR::~XANDROIDDIR()
{
  Clean();
}


/*-------------------------------------------------------------------
//  XANDROIDDIR::Exist
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
bool XANDROIDDIR::Exist(XCHAR* path)
{
  if(!xfactory) return false;

  XPATH   xpath;
  XBUFFER actualpath((XDWORD)_MAXPATH);
  XBUFFER newpath;
  bool    status = false;

  getcwd((char*)actualpath.Get(),_MAXPATH);

  xpath = path;
  xpath.ConvertToUTF8(newpath);

  if(!chdir((char*)newpath.Get())) status = true;

  chdir((char*)actualpath.Get());

  return status;
}



//-------------------------------------------------------------------
//  XANDROIDDIR::Make
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
bool XANDROIDDIR::Make(XCHAR* path)
{
  if(!path)   return false;
  if(!path[0]) return false;

  if(Exist(path)) return true;

  XPATH      xpath;
  XPATH      xpathsequence;
  XSTRING pathpart;
  int        index = 0;

  xpathsequence.Empty();

  xpath = path;

  do{
      if(xpath.GetPathInSequence(index,pathpart))
        {
          xpathsequence += pathpart;
          xpathsequence += __L("/");

          if(pathpart.Character_GetLast()!=__C(':'))
            {
              if(!Exist(xpathsequence.Get()))
                {
                  XBUFFER newpath;
                  int     status;

                  xpathsequence.ConvertToUTF8(newpath);

                  status = mkdir((char*)newpath.Get(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
                  if(status) return false;
                }
            }

        } else break;

      index++;

    } while(1);

  return true;
}




/*-------------------------------------------------------------------
//  XANDROIDDIR::ChangeTo
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      31/03/2009 08:29:55 p.m.
//
//  @return       bool :
//  @param        path :
*/
/*-----------------------------------------------------------------*/
bool XANDROIDDIR::ChangeTo(XCHAR* path)
{
  if(!xfactory) return false;

  XPATH   xpath;
  XBUFFER newpath;
  int     status = 1;

  xpath = path;
  xpath.ConvertToUTF8(newpath);

  status = chdir((char*)newpath.Get());

  return (!status)?true:false;
}



/*-------------------------------------------------------------------
//  XANDROIDDIR::Delete
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      24/02/2012 10:08:06
//
//  @return       bool :
//  @param        path :
//  @param        all :
*/
/*-----------------------------------------------------------------*/
bool XANDROIDDIR::Delete(XCHAR* path,bool all)
{
  if(!xfactory) return false;

  XDIRELEMENT search;
  XPATH       pathfile;
  XPATH       xpath;
  XBUFFER     newpath;

  if(all)
    {
      if(FirstSearch(path,__L("*"),&search))
        {
          do { pathfile = path;
               pathfile.Slash_Add();
               pathfile += search.GetNameFile()->Get();

               if(search.GetType()==XDIRELEMENTTYPE_DIR)
                 {
                   Delete(pathfile.Get(),all);
                 }
                else
                 {
                   xpath = pathfile;
                   xpath.ConvertToUTF8(newpath);

                   unlink((char*)newpath.Get());
                 }

             } while(NextSearch(&search));
        }
    }

  xpath = path;
  xpath.ConvertToUTF8(newpath);

  int status = rmdir((char*)newpath.Get());

  return (!status)?true:false;
}



//-------------------------------------------------------------------
//  XANDROIDDIR::FirstSearch
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/10/2004 13:05:07
//
//  @return       bool :
//  @param        path :
//  @param        patternsearch :
//  @param        searchelement :
*/
//-------------------------------------------------------------------
bool XANDROIDDIR::FirstSearch(XCHAR* path,XCHAR* patternsearch,XDIRELEMENT* searchelement)
{
  if(!xfactory)      return false;
  if(!searchelement) return false;

  XPATH   xpath;
  XBUFFER newpath;
  bool    status = false;

  xpath = path;
  xpath.ConvertToUTF8(newpath);

  DIR* dir = opendir((char*)newpath.Get());
  if(dir)
    {
      searchelement->GetPathSearch()->Set(path);
      searchelement->GetPatternSearch()->Set(patternsearch);
      searchelement->SetHandle((void*)dir);

      struct dirent* entry = readdir(dir);
      if(!entry)
        {
          closedir(dir);
          status = false;
        }
       else
        {
          searchelement->SetFindFileData((void*)entry);
          status = NextSearch(searchelement);
        }

    } else status = false;

  return status;
}



//-------------------------------------------------------------------
//  XANDROIDDIR::NextSearch
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
bool XANDROIDDIR::NextSearch(XDIRELEMENT* searchelement)
{
  if(!xfactory)      return false;
  if(!searchelement) return false;

  DIR*           dir    = (DIR*)searchelement->GetHandle();
  struct dirent* entry  = (struct dirent*)searchelement->GetFindFileData();
  bool           status = false;
  XSTRING    name;
  XBUFFER        patternsearch;

  while(entry != NULL)
    {
      name.ConvertFromUTF8((XBYTE*)entry->d_name,_MAXPATH);

      if((name.Compare(__L("."))) && (name.Compare(__L(".."))))
        {
          searchelement->GetPatternSearch()->ConvertToUTF8(patternsearch);
          if(!fnmatch((char*)patternsearch.Get(), entry->d_name,0))
            {
              searchelement->SetType(XDIRELEMENTTYPE_NONE);
              searchelement->GetNameFile()->Set(name);

              XPATH xpath;

              xpath = searchelement->GetPathSearch()->Get();
              xpath.Slash_Add();
              xpath += searchelement->GetNameFile()->Get();

              searchelement->SetType(TypeOfEntry(xpath.Get()));

              status = true;
            }
        }

      entry = readdir(dir);
      searchelement->SetFindFileData((void*)entry);

      if(status) break;
    }

  if(!status) closedir (dir);

  return status;
}




//-------------------------------------------------------------------
//  XANDROIDDIR::Clean
/**
//
//
//  @author       Abraham J. Velez
//  @version      29/10/2004 10:17:30
//
//  @return       void :
//  */
//-------------------------------------------------------------------
void XANDROIDDIR::Clean()
{

}



/*-------------------------------------------------------------------
//  XANDROIDDIR::TypeOfEntry
*/
/**
//
//
//  @author       Abraham J. Velez
//  @version      06/03/2010 17:55:09
//
//  @return       XDIRELEMENTTYPE :
//  @param        path :
*/
/*-----------------------------------------------------------------*/
XDIRELEMENTTYPE XANDROIDDIR::TypeOfEntry(XCHAR* path)
{
  if(!xfactory) return XDIRELEMENTTYPE_NONE;
  if(!path)     return XDIRELEMENTTYPE_NONE;

  XPATH           xpath;
  XBUFFER         newpath;
  XDIRELEMENTTYPE type = XDIRELEMENTTYPE_NONE;

  xpath = path;
  xpath.ConvertToUTF8(newpath);

  struct stat datafile;

  if(lstat((char*)newpath.Get(), &datafile) != -1)
    {
      //Link
      if(S_ISLNK(datafile.st_mode))
        {

        }

      // File
      if(S_ISREG(datafile.st_mode))
        {
          type = XDIRELEMENTTYPE_FILE;
        }
       else
        {
          // Directory
          if(S_ISDIR(datafile.st_mode)) type = XDIRELEMENTTYPE_DIR;
        }
    }

  return type;
}

