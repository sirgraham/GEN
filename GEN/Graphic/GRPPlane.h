/*------------------------------------------------------------------------------------------
// GRPPlane.HPP
*/
/**
// \class
//
// Manages plane class
//
//  @author: Diego Martinez Ruiz de Gaona
//
//  Date of Creation : 08/11/2017 16:44:34
//  Last Modification :
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


#ifndef _GRPPLANE_HPP
#define _GRPPLANE_HPP

/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>
#include "GRPVector.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPPLANE
{
public:

    double a, b, c, d;
    GRPVECTOR normal;


    void Set(GRPVECTOR v1, GRPVECTOR v2, GRPVECTOR v3);
    float distance(GRPVECTOR v);
    void Normalize();

    /*
    GRPVECTOR normal, point;

    GRPPLANE::GRPPLANE  (GRPVECTOR &v1, GRPVECTOR &v2, GRPVECTOR &v3) { Set(v1, v2, v3);    }
    GRPPLANE::GRPPLANE  ()                                            {}
    GRPPLANE::~GRPPLANE ()                                            {}

    void GRPPLANE::Set(GRPVECTOR &v1, GRPVECTOR &v2, GRPVECTOR &v3)
    {
        GRPVECTOR aux1, aux2;

        aux1 = v1 - v2;
        aux2 = v3 - v2;

        normal = aux2 * aux1;

        normal.Normalize();
        point=v2;

        d = -(normal.InnerProduct(normal,point));
    }

    void GRPPLANE::setNormalAndPoint(GRPVECTOR &normal, GRPVECTOR &point)
    {
        this->normal=normal;
        this->normal.Normalize();
        d = -(this->normal.InnerProduct(this->normal,point));
    }

    void GRPPLANE::setCoefficients(float a, float b, float c, float d)
    {
        // set the normal vector
        normal = GRPVECTOR(a, b, c);

        //compute the lenght of the vector
        float l = normal.GetLengthf();

        // normalize the vector
        normal=GRPVECTOR(a / l, b / l, c / l);

        // and divide d by th length as well
        this->d = d / l;
    }

    float GRPPLANE::distance(GRPVECTOR &p)
    {
        return (d + normal.InnerProduct(normal,p));
    }
    */
};














/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif