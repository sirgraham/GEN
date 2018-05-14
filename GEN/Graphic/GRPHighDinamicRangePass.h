/*------------------------------------------------------------------------------------------
// GRPHighDynamicrangePass.HPP
*/
/**
// \class
//
// High dinamic range
//
//  @author: Diego Martinez Ruiz de Gaona
//
//  Date of Creation : 28/11/2017 12:55:38
//  Last Modification :
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/

#ifndef _GRPHIGHDYNAMICRANGEPASS_HPP
#define _GRPHIGHDYNAMICRANGEPASS_HPP

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPPass.h"
#include "GRPRenderTargetPass.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPHIGHDINAMICRANGEPASS : public GRPRENDERTARGETPASS
{
public:

                            GRPHIGHDINAMICRANGEPASS ();
    virtual                 ~GRPHIGHDINAMICRANGEPASS();
    virtual void            Begin                   ();
    virtual void            End                     ();
    virtual void            SetViewport             (GRPVIEWPORT* viewport);
    virtual void            CreateRenderTarget      ();

protected:

    unsigned int hdrFBO;
    unsigned int colorBuffer;
    unsigned int rboDepth;



private:

    void Clean()
    {

    }

};
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif