/*------------------------------------------------------------------------------------------
//  GRPOVERLAYPASS.H
*/
/**
// \class
//
//  Overlay stencil pass
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 13/ Octubre /2017
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPOVERLAYPASS_H_
#define _GRPOVERLAYPASS_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPBackgroundPass.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPMATERIAL;

/*---- CLASS -----------------------------------------------------------------------------*/

class  GRPOVERLAYPASS : public GRPBACKGROUNDPASS
{
public:
    GRPOVERLAYPASS(XCHAR* name = __L("overlay"));
    ~GRPOVERLAYPASS();

    void              Begin     ();
    void              End       ();
    bool              OnAdded   ();

protected:

private:

    void    Clean()
    {

    }



};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

