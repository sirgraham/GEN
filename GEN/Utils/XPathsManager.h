//-------------------------------------------------------------------------------------------
//  XPATHSMANAGER.H
//
/**
// \class
//
//  xPaths Manager Functions
//
//  @author  Abraham J. Velez
//  @version 09/08/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XPATHSMANAGER_H_
#define _XPATHSMANAGER_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XVector.h"
#include "XString.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum XPATHSMANAGERSECTIONTYPE
{
  XPATHSMANAGERSECTIONTYPE_UNKNOWN        = 0 ,
  XPATHSMANAGERSECTIONTYPE_ROOT               ,
  XPATHSMANAGERSECTIONTYPE_LOGS               ,
  XPATHSMANAGERSECTIONTYPE_DATABASES          ,
  XPATHSMANAGERSECTIONTYPE_WEB                ,
  XPATHSMANAGERSECTIONTYPE_WEBSCRAPER         ,
  XPATHSMANAGERSECTIONTYPE_FONTS              ,
  XPATHSMANAGERSECTIONTYPE_GRAPHICS           ,
  XPATHSMANAGERSECTIONTYPE_SOUNDS             ,
  XPATHSMANAGERSECTIONTYPE_SCRIPTS            ,
  XPATHSMANAGERSECTIONTYPE_GENERIC1           ,
  XPATHSMANAGERSECTIONTYPE_GENERIC2           ,
  XPATHSMANAGERSECTIONTYPE_GENERIC3           ,
};

//---- CLASS -------------------------------------------------------------------------------

class XPATH;

class XPATHSMANAGERSECTION
{
  public:

                                      XPATHSMANAGERSECTION          ();
    virtual                          ~XPATHSMANAGERSECTION          ();

    XPATHSMANAGERSECTIONTYPE          type;
    XPATH*                            xpath;

  private:

    void                              Clean                         ()
                                      {
                                        type = XPATHSMANAGERSECTIONTYPE_UNKNOWN;
                                        xpath = NULL;
                                      }
};


class XPATHSMANAGER
{
  public:

    static XPATHSMANAGER&             GetInstance()
                                      {
                                        if(!instance) instance = new XPATHSMANAGER();

                                        return (*instance);
                                      }

    static bool                       DelInstance()
                                      {
                                        if(instance)
                                          {
                                            delete instance;
                                            instance = NULL;

                                            return true;
                                          }

                                        return false;
                                      }


    bool                              GetPathOfSection              (XPATHSMANAGERSECTIONTYPE sectiontype, XPATH& xpath, bool addroot = true);

    XPATH*                            GetAppExecPath                ()
                                      {
                                        return appexecpath;
                                      }

    XPATHSMANAGERSECTION*             GetPathSection                (XPATHSMANAGERSECTIONTYPE sectiontype);

    bool                              AddPathSection                (XPATHSMANAGERSECTIONTYPE sectiontype, XSTRING& xpath);
    bool                              AddPathSection                (XPATHSMANAGERSECTIONTYPE sectiontype, const XCHAR* string);

    bool                              DeleteAllPathSections         ();

    bool                              CreateAllPathSectionOnDisk    ();

    bool                              AdjustRootPathDefault         (XCHAR* nameapp, XCHAR* recopilatedir);
    bool                              AdjustRootPathDefault         (XSTRING& nameapp, XCHAR* recopilatedir);

  private:
                                      XPATHSMANAGER                 ();
                                      XPATHSMANAGER                 (XPATHSMANAGER const&);       // Don't implement
    virtual                          ~XPATHSMANAGER                 ();

    void                              operator =                    (XPATHSMANAGER const&);       // Don't implement

    void                              Clean                         ()
                                      {
                                        appexecpath = NULL;
                                      }

    XPATH*                            appexecpath;
    XVECTOR<XPATHSMANAGERSECTION*>    pathsections;
    static XPATHSMANAGER*             instance;
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

