/*------------------------------------------------------------------------------------------
//  GRPFONTMANAGER.H
*/
/**
// \class
//
//  Manages TTF font 3d vbos
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 28/07/2014 13:18:46
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPFONTMANAGER_H_
#define _GRPFONTMANAGER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPOpenGL.h"
#include "GRPFont.h"

#include "XMemory.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define GRPFONTMANAGER_DEFAULTFONT __L("arial.ttf")

/*---- CLASS -----------------------------------------------------------------------------*/

#define GRPFONTFAMILY XMAP<XDWORD, GRPFONT*>

class GRPFONTMANAGER
{
  public:
                                    GRPFONTMANAGER          ();
    virtual                         ~GRPFONTMANAGER         ();

    bool                            Init                    ();

    bool                            Load                    (XCHAR* fname,XCHAR* name, GRPFONTATTRIBUTES* attributes=NULL);
    GRPFONT*                        GetFont                 (const XCHAR* name);
    GRPFONT*                        GetFontByFamily         (XCHAR* familyname, GRPFONT_STYLE style);

    int                             GetLastError            ()                        { return lasterror;                   }
    void                            SetLastError            (XDWORD error)            { this->lasterror = error;            }

    XMAP<XSTRING*,GRPFONT*>*        GetFonts                ()                        { return &fonts;}
    XSTRING*                        GetDefaultFont          ()                        { return &defaultfont; }

    bool                            IndexFont               (XCHAR* filename);

  protected:

    XSTRING                         defaultfont;
    XDWORD                          lasterror;
    XMAP<XSTRING*, GRPFONT*>        fonts;
    XMAP<XSTRING*, GRPFONTFAMILY*>  families;

  private:

    void                            Clean                   ()
                                    {
                                        lasterror   = GRPFONTSTATUS_OK;
                                        defaultfont = GRPFONTMANAGER_DEFAULTFONT;
                                      }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

