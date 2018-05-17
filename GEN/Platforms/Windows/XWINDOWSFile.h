//------------------------------------------------------------------------------------------
//  XWINDOWSFILE.H
//
/**
// \class
//
//  WINDOWS file class
//
//  @author  Abraham J. Velez
//  @version 13/03/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XWINDOWSFILE_H_
#define _XWINDOWSFILE_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include <stdio.h>

#include "XBase.h"
#include "XFile.h"
#include "XPath.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

enum XWINDOWSFILEMODE
{
  XWINDOWSFILEMODE_READONLY       = 0 ,
  XWINDOWSFILEMODE_READWRITE          ,
  XWINDOWSFILEMODE_CREATE             ,
};



//---- CLASS -------------------------------------------------------------------------------

class CIPHER;

class XWINDOWSFILE : public XFILE
{
  public:
                      XWINDOWSFILE          ();
    virtual          ~XWINDOWSFILE          ();

    bool              Exist                 (XCHAR* xpath);

    bool              Open                  (XCHAR* xpath, bool readonly = true);
    bool              Create                (XCHAR* xpath);

    bool              SetSize               (XDWORD size);

    bool              GetPosition           (XDWORD& position);
    bool              SetPosition           (XDWORD position);

    bool              Read                  (XBYTE* buffer, XDWORD size , CIPHER* cipher = NULL);
    bool              Read                  (XBYTE* buffer, XDWORD* size, CIPHER* cipher = NULL);

    bool              Write                 (XBYTE* buffer, XDWORD size , CIPHER* cipher = NULL);

    bool              Flush                 ();

    bool              Close                 ();

    bool              Erase                 (XCHAR* xpath, bool overwrite = false);
    bool              Rename                (XCHAR* xpathold, XCHAR* xpathnew);

    FILE*             GetFileStructHandle   ();

  protected:

    bool              ActualizeSize         ();

  private:

    void              Clean                 ()
                      {
                        filehandle  = NULL;
                        mode        = XWINDOWSFILEMODE_READONLY;
                        isdup       = false;
                        fp          = NULL;
                        fd          = -1;
                      }

    bool              ExtendedOpen        (XCHAR* xpath, XWINDOWSFILEMODE mode);

    HANDLE            filehandle;
    XWINDOWSFILEMODE  mode;

    bool              isdup;
    int               fd;
    FILE*             fp;
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

