//------------------------------------------------------------------------------------------
//  GRPLINUXFACTORY.H
//
/**
// \class
//
//  LINUX graphics platform factory class
//
//  @author  Abraham J. Velez
//  @version 15/07/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _GRPLINUXFACTORY_H_
#define _GRPLINUXFACTORY_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "GRPFactory.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------


//---- CLASS -------------------------------------------------------------------------------

class XFACTORY;
class XPUBLISHER;
class GRPCONTEXT;

class GRPLINUXFACTORY : public GRPFACTORY
{
  public:

                            GRPLINUXFACTORY       ()  {  }
    virtual                ~GRPLINUXFACTORY       ()  {  }

    GRPSCREEN*              CreateScreen          ();
    bool                    DeleteScreen          (GRPSCREEN* screen);

    GRPCONTEXT*             CreateContext         ();
    bool                    DeleteContext         (GRPCONTEXT* context);

    virtual void            GenerateTextureMap    ();

};

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

