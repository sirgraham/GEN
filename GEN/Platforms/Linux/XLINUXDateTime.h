//------------------------------------------------------------------------------------------
//  XLINUXDATETIME.H
//
/**
// \class
//
//  LINUX date time class
//
//  @author  Abraham J. Velez
//  @version 04/04/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _XLINUXDATETIME_H_
#define _XLINUXDATETIME_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "XDateTime.h"


//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------



class XLINUXDATETIME : public XDATETIME
{
  public:
                        XLINUXDATETIME                  ();
    virtual            ~XLINUXDATETIME                  ();

    bool                Read                            ();
    bool                Write                           ();

    bool                FileTime                        (XPATH& xpath,void* tmzip, XDWORD* dt);
    bool                FileTime                        (XPATH& xpath);

    int                 GetDifferenceGMT                ();

    bool                IsDayLigthSavingTime            (int* bias = NULL);

  protected:

    void                GetActualDateTime               (XLINUXDATETIME* timed);
    void                SetActualDateTime               (XLINUXDATETIME* timed);


};



//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

