//------------------------------------------------------------------------------------------
//  XFILECONFIG.H
//
/**
// \class
//
//  Application CFG class
//
//  @author  Abraham J. Velez
//  @version 05/05/2004 20:51:37
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XFILECONFIG_H_
#define _XFILECONFIG_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XString.h"
#include "XPath.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define XFILECONFIG_EXTENSIONFILE  __L(".ini")

enum XFILECONFIGVALUETYPE
{
  XFILECONFIGVALUETYPE_UNKNOWN          = 0 ,
  XFILECONFIGVALUETYPE_INT                ,
  XFILECONFIGVALUETYPE_MASK               ,
  XFILECONFIGVALUETYPE_FLOAT              ,
  XFILECONFIGVALUETYPE_STRING             ,
  XFILECONFIGVALUETYPE_BOOLEAN
};


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPATHS;
class XFILEINI;


class XFILECONFIGVALUE
{
  public:
                                        XFILECONFIGVALUE          ();
    virtual                            ~XFILECONFIGVALUE          ();

    XFILECONFIGVALUETYPE                GetType                   ();
    bool                                SetType                   (XFILECONFIGVALUETYPE type);

    XSTRING*                            GetGroup                  ();
    XSTRING*                            GetID                     ();

    void*                               GetValue                  ();
    bool                                SetValue                  (void* value);

  private:

    void                                Clean                     ();

    XFILECONFIGVALUETYPE                type;
    XSTRING                             group;
    XSTRING                             ID;
    void*                               value;
};



class XFILECONFIG
{
  public:
                                        XFILECONFIG               (XCHAR* namefile = NULL);
    virtual                            ~XFILECONFIG               ();

    virtual bool                        Ini                       ();

    bool                                Load                      ()
                                        {
                                          if(xpathfile.IsEmpty()) return false;
                                          return Load(xpathfile);
                                        }
    virtual bool                        Load                      (XPATH& xpath);


    bool                                Save                      ()
                                        {
                                          if(xpathfile.IsEmpty()) return false;
                                          return Save(xpathfile);
                                        }
    virtual bool                        Save                      (XPATH& xpath);

    virtual bool                        Default                   ();

    virtual bool                        End                       ();

    bool                                AddValue                  (XFILECONFIGVALUETYPE type, XCHAR* group, XCHAR* ID, void* value);
    bool                                DeleteAllValues           ();

  protected:

    XPATH                               xpathfile;
    XSTRING                             namefile;
    XFILEINI*                           fileini;

  private:

    void                                Clean                     ()
                                        {
                                          fileini     = NULL;
                                          namefile.Empty();
                                        }

    bool                                IniFile                   (XPATH& xpath);
    bool                                EndFile                   ();

    XVECTOR<XFILECONFIGVALUE*>          values;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

