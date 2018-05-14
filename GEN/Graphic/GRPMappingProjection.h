/*------------------------------------------------------------------------------------------
//  GRPMAPPINGPROJECTION.H
*/
/**
// \class
//
//  Generates U,V,W mapping coordinates
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 30/06/2014 10:12:22
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPMAPPINGPROJECTION_H_
#define _GRPMAPPINGPROJECTION_H_

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPVector.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPOBJECT;
class GRPMATERIAL;
class GRPELEMENT;

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPMAPPINGPROJECTION
{
  public:
                            GRPMAPPINGPROJECTION            ();
                            GRPMAPPINGPROJECTION            (GRPELEMENT* object, GRPMATERIAL* material);
    virtual                ~GRPMAPPINGPROJECTION            ();

    virtual bool            ProjectMapping                  (GRPELEMENT* object)   { return false;                }
    virtual void            ReassignUVs                     (GRPELEMENT* object);

    GRPVECTOR               position;
    GRPVECTOR               direction;
    GRPVECTOR               scalefactor;
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif


