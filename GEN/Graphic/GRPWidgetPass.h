/*------------------------------------------------------------------------------------------
// GRPWidgetPass.HPP
*/
/**
// \class
//
// Manages widget pass
//
//  @author: Diego Martinez Ruiz de Gaona
//
//  Date of Creation : 12/12/2017 11:20:36
//  Last Modification :
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


#ifndef _GRPWIDGETPASS_HPP
#define _GRPWIDGETPASS_HPP

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPPass.h"
#include "GRPWidgetStyle.h"
#include "XDictionary.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/
class GRPWIDGETPAGE;
class GRPWIDGETPASS : public GRPPASS
{
public:
                            GRPWIDGETPASS   ();
                            ~GRPWIDGETPASS  ();
    virtual bool            OnAdded         ();

    virtual bool            Add             (INPDEVICE* device);
    GRPWIDGETPAGE*          AddPage         (XCHAR* toload);
    GRPWIDGETPAGE*          GetPage         ();

    void                    SetCurrentPage  (XDWORD page);

    bool                    DrawRenderOrder ();
    float                   GetScale        ();
    void                    SetScale        (float sc);
    GRPSCREEN*              GetScreen       ();

    XVECTOR<GRPWIDGETPAGE*>*GetPages        ();


protected:

    float                   width, height,scale;

    XVECTOR<GRPWIDGETPAGE*> pages;
    XDWORD                  active_page;

    GRPSCREEN*              screen;
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif
