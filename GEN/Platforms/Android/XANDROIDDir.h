//------------------------------------------------------------------------------------------
//  XANDROIDDIR.H
//
/**
// \class
//
//  File Directory Class
//
//  @author  Abraham J. Velez
//  @version 29/10/2004 10:14:00
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XANDROIDDIR_H_
#define _XANDROIDDIR_H_

//---- INCLUDES ----------------------------------------------------------------------------

#include "XBase.h"
#include "XDir.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------

class XANDROIDFACTORY;

class XANDROIDDIR : public XDIR
{
  public:
                        XANDROIDDIR         ();
    virtual            ~XANDROIDDIR         ();

    bool                Exist               (XCHAR* path);
    bool                Make                (XCHAR* path);
    bool                ChangeTo            (XCHAR* path);
    bool                Delete              (XCHAR* path,bool all);

    bool                FirstSearch         (XCHAR* path,XCHAR* patternsearch,XDIRELEMENT* searchelement);
    bool                NextSearch          (XDIRELEMENT* searchelement);

  private:

    void                Clean               ();
    XDIRELEMENTTYPE     TypeOfEntry         (XCHAR* path);
};


//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

