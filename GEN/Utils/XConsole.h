//------------------------------------------------------------------------------------------
//  XCONSOLE.H
//
/**
// \class
//
// xconsole Class
//
//  @author  Abraham J. Velez
//  @version 03/03/2004 12:15:55
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XCONSOLE_H_
#define _XCONSOLE_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XString.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

#define XCONSOLE_MAXSIZEDATABLOCK             10240

//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;

class XCONSOLE
{
  public:

                                XCONSOLE                      ();
    virtual                    ~XCONSOLE                      ();

    virtual bool                GetSize                       (int& width, int& height)
                                {
                                  width = 0;
                                  height = 0;
                                  return false;
                                }

    virtual bool                SetSize                       (int width, int height)         { return false;         }

    virtual bool                Maximize                      ()                              { return false;         }
    virtual bool                Minimize                      ()                              { return false;         }

    virtual bool                Hide                          ()                              { return false;         }
    virtual bool                IsHide                        ()                              { return false;         }
    virtual bool                UnHide                        ()                              { return false;         }


    virtual bool                Print                         (XCHAR* string);
    bool                        Printf                        (XCHAR* mask,...);

    bool                        PrintDataBlock                (XBYTE* data, XDWORD _size, XDWORD marginsize = 1, XDWORD sizeline = 16, bool showoffset = true, bool showtext = true);
    bool                        PrintDataBlock                (XBUFFER& data, XDWORD marginsize = 1, XDWORD sizeline = 16, bool showoffset = true, bool showtext = true);

    virtual bool                Clear                         ()                              { return false;         }

    virtual bool                KBHit                         ()                              { return false;         }
    virtual int                 GetChar                       ()                              { return 0;             }

    bool                        FormatMessage                 (XCHAR* message, XDWORD margin, bool prelude, bool returnline, XSTRING& string);
    bool                        PrintMessage                  (XCHAR* message, XDWORD margin, bool prelude, bool returnline);
    bool                        TipicalHeader_Create          (int yearorigin, XCHAR* nameapp, int version, int subversion, int subversionerr, XCHAR* enterprise, XSTRING& header);
    bool                        TipicalHeader_Show            (int yearorigin, XCHAR* nameapp, int version, int subversion, int subversionerr, XCHAR* enterprise);
    bool                        WaitKey                       (XCHAR* text, XDWORD margin, bool prelude, XDWORD timeout);

  protected:

    void                        Clean                         ()
                                {

                                }

};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

