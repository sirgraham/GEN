/*------------------------------------------------------------------------------------------
//  GRPOBJECTBUILDERALGORITHM.H
*/
/**
// \class
//
//  Algorithms for 3d building class
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 12/06/2014 11:07:08
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


#ifndef _GRPOBJECTBUILDERALGORITHM_H_
#define _GRPOBJECTBUILDERALGORITHM_H_

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPOpenGL.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPOBJECTBUILDER;
class GRPSHAPE;
class GRPMATERIAL;
class GRPELEMENT;
class GRPSHADERPROGRAM;

class GRPOBJECTBUILDERALGORITHM
{
  public:
                              GRPOBJECTBUILDERALGORITHM           ()                    {  Clean();                       }

                              GRPOBJECTBUILDERALGORITHM           (GRPOBJECTBUILDER* builder)
                              {
                                Clean();
                                this->builder = builder;
                              }

    virtual                  ~GRPOBJECTBUILDERALGORITHM           ()                    {  Clean();                       }

    virtual GRPELEMENT*       Execute                             (GRPSHAPE* shape)   = 0;



    GRPSHADERPROGRAM*         programID;
    GRPMATERIAL*              material;
    GRPOBJECTBUILDER*         builder;

    int                       debugsteps;

  private:

    void                      Clean                 ()
                              {
                                programID     = 0;
                                material      = NULL;
                                builder       = NULL;
                                debugsteps    = 0;
                              }
};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

