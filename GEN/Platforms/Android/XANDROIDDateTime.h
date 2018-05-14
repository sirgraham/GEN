//------------------------------------------------------------------------------------------
//  XANDROIDDATETIME.H
//
/**
// \class
//
//  ANDROID time class
//
//  @author  Abraham J. Velez
//  @version 04/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XANDROIDDATETIME_H_
#define _XANDROIDDATETIME_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XDateTime.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------



//---- CLASS -------------------------------------------------------------------------------


class XANDROIDDATETIME : public XDATETIME
{
  public:
                        XANDROIDDATETIME                ();
    virtual            ~XANDROIDDATETIME                ();

    bool                Read                            ();
    bool                Write                           ();

    bool                FileTime                        (XPATH& xpath,void* tmzip, XDWORD* dt);
    bool                FileTime                        (XPATH& xpath);


    int                 GetDifferenceGMT                ();

    bool                IsDayLigthSavingTime            (int* bias = NULL);

  protected:

    void                GetActualDateTime               (XANDROIDDATETIME* timed);
    void                SetActualDateTime               (XANDROIDDATETIME* timed);
};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

