//------------------------------------------------------------------------------------------
//  COMPRESSMANAGER.H
//
/**
// \class
//
//  Compress Manager
//
//  @author  Abraham J. Velez
//  @version 03/05/2004 14:54:16
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _COMPRESSMANAGER_H_
#define _COMPRESSMANAGER_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "CompressBase.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

//---- CLASS -------------------------------------------------------------------------------

class COMPRESSMANAGER
{
  public:
                        COMPRESSMANAGER         ();
    virtual            ~COMPRESSMANAGER         ();

    COMPRESSBASE*       Create                  (COMPRESSBASE_TYPE type);


    static COMPRESSMANAGER* Get()
    {
      if (!Instance)
        Instance=new COMPRESSMANAGER();

      return Instance;
    }

    static void Destroy()
    {
      if (Instance)
          delete  Instance;
    }

  protected:

    static COMPRESSMANAGER* Instance;

  private:

    void                Clean                   ();

    COMPRESSBASE_TYPE   lastcompresstype;
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------


#endif

