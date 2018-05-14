/*------------------------------------------------------------------------------------------
// GRPFrustum.HPP
*/
/**
// \class
//
// Frustum class
//
//  @author: Diego Martinez Ruiz de Gaona
//
//  Date of Creation : 08/11/2017 16:43:40
//  Last Modification :
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


#ifndef _GRPFRUSTUM_HPP
#define _GRPFRUSTUM_HPP
/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPPlane.h"
#include "GRPPoint.h"
#include "GRPMatrix.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPAABB;
class GRPCAMERA;
class GRPELEMENT;

/*---- CLASS -----------------------------------------------------------------------------*/
enum { GRPFRUSTUM_OUTSIDE, GRPFRUSTUM_INTERSECT, GRPFRUSTUM_INSIDE };

class GRPFRUSTUM
{
public:
    enum FRUSTRUM_PLANE
    {
        FRUSTRUM_PLANE_NEAR,
        FRUSTRUM_PLANE_FAR,
        FRUSTRUM_PLANE_TOP,
        FRUSTRUM_PLANE_LEFT,
        FRUSTRUM_PLANE_BOTTOM,
        FRUSTRUM_PLANE_RIGHT
    };

    enum FRUSTUM_POINT
    {
        NEAR_TOP_LEFT,
        NEAR_TOP_RIGHT,
        NEAR_BOT_LEFT,
        NEAR_BOT_RIGHT,

        FAR_TOP_LEFT,
        FAR_TOP_RIGHT,
        FAR_BOT_LEFT,
        FAR_BOT_RIGHT
    };




    GRPPLANE pl[6];


    GRPVECTOR ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
    float nearD, farD, ratio, angle, tang;
    float nw, nh, fw, fh;

    GRPFRUSTUM();
    ~GRPFRUSTUM();


    void Update         (GRPCAMERA* cam);
    void setCamInternals(float angle,   float ratio,    float nearD, float farD);
    void setCamDef      (GRPVECTOR p,   GRPVECTOR l,    GRPVECTOR u);
    int  pointInFrustum (GRPVECTOR p);
    int  sphereInFrustum(GRPVECTOR p, float raio);
    int  boxInFrustum   (GRPAABB b);
    int objectVisible (GRPMATRIX &mvp, GRPVECTOR* pos, const GLfloat &radius);
private:

    void Clean()
    {

    }
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif