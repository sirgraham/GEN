//------------------------------------------------------------------------------------------
//  GRPANDROIDFACTORY.H
//
/**
// \class
//
//  Android Graphics Factory
//
//  @author  Abraham J. Velez
//  @version 15/07/2002
*/
//  GEN  Copyright (C).  All right reserved.
//------------------------------------------------------------------------------------------

#ifndef _GRPANDROIDFACTORY_H_
#define _GRPANDROIDFACTORY_H_


//---- INCLUDES ----------------------------------------------------------------------------

#include "GRPProperties.h"
#include "GRPFactory.h"

//---- DEFINES & ENUMS  --------------------------------------------------------------------

//---- CLASS -------------------------------------------------------------------------------


class XFACTORY;
class XPUBLISHER;
class GRPCONTEXT;


class GRPANDROIDFACTORY : public GRPFACTORY
{
  public:

                            GRPANDROIDFACTORY       ()   {    }
    virtual                ~GRPANDROIDFACTORY       ()   {    }


    GRPSCREEN*              CreateScreen            ( );
    bool                    DeleteScreen            (GRPSCREEN* screen);

    GRPCONTEXT*             CreateContext           ( );
    bool                    DeleteContext           (GRPCONTEXT* context);

    virtual void            GenerateTextureMap      ();
};

//---- INLINE FUNCTIONS --------------------------------------------------------------------

#endif

