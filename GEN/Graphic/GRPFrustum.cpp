/*------------------------------------------------------------------------------------------
// GRPFRUSTUM.CPP
*/
/**
// \class
//
// Manages the frustum
//
//  @author: Diego Martinez Ruiz de Gaona
//
//  Date of Creation : 08/11/2017 17:00:42
//  Last Modification :
*/
/* GEN  Copyright (C).  All right reserved.
/*------------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPFrustum.h"
#include "GRPCamera.h"
#include "GRPBoundingBox.h"
#include "GRPElement.h"
#include "GRPNode.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define ANG2RAD 3.14159265358979323846/180.0

/*---- CLASS -----------------------------------------------------------------------------*/

GRPFRUSTUM::GRPFRUSTUM()
{

}
GRPFRUSTUM::~GRPFRUSTUM()
{
}

void GRPFRUSTUM::Update(GRPCAMERA* cam)
{
    setCamInternals (cam->fov, cam->viewport.GetAspectRatio(), cam->NearPlane, cam->FarPlane);
    setCamDef       (cam->eye, cam->obj, cam->up);
}

void GRPFRUSTUM::setCamInternals(float angle, float ratio, float nearD, float farD)
{

    this->ratio = ratio;
    this->angle = angle;
    this->nearD = nearD;
    this->farD = farD;

    tang = (float)tan(angle* ANG2RAD * 0.5);
    nh = nearD * tang;
    nw = nh * ratio;
    fh = farD  * tang;
    fw = fh * ratio;

}

void GRPFRUSTUM::setCamDef(GRPVECTOR p, GRPVECTOR l, GRPVECTOR u)
{
    GRPVECTOR dir, nc, fc, X, Y, Z;

    Z = l-p;
    Z.Normalize();

  //  X = u * Z;
    X.CrossProduct(&u,&Z);
    X.Normalize();

    //Y = Z * X;
    Y.CrossProduct(&Z, &X); //up de la camara

    nc = p - Z * nearD; //vector de posicion a plano near
    fc = p - Z * farD; //vector de posicion a plano far

    ntl = nc + (Y * nh) - (X * nw);
    ntr = nc + (Y * nh) + (X * nw);
    nbl = nc - (Y * nh) - (X * nw);
    nbr = nc - (Y * nh) + (X * nw);

    ftl = fc + Y * fh - (X * fw);
    ftr = fc + Y * fh + (X * fw);
    fbl = fc - Y * fh - (X * fw);
    fbr = fc - Y * fh + (X * fw);

    pl[FRUSTRUM_PLANE_NEAR].Set(ntl, ntr, nbr);

    pl[FRUSTRUM_PLANE_TOP   ].Set(ntr, ntl, ftl);
    pl[FRUSTRUM_PLANE_BOTTOM].Set(nbl, nbr, fbr);

    pl[FRUSTRUM_PLANE_LEFT  ].Set(ntl, nbl, fbl);
    pl[FRUSTRUM_PLANE_RIGHT ].Set(nbr, ntr, fbr);


    pl[FRUSTRUM_PLANE_FAR   ].Set(ftr, ftl, fbl);

}

int GRPFRUSTUM::pointInFrustum(GRPVECTOR p)
{
    int result = GRPFRUSTUM_INSIDE;

    if(pl[FRUSTRUM_PLANE_NEAR].distance(p) < 0)
      return GRPFRUSTUM_OUTSIDE;
    if (pl[FRUSTRUM_PLANE_FAR].distance(p) < 0)
      return GRPFRUSTUM_OUTSIDE;
    if (pl[FRUSTRUM_PLANE_LEFT].distance(p) < 0)
      return GRPFRUSTUM_OUTSIDE;
    if (pl[FRUSTRUM_PLANE_RIGHT].distance(p) < 0)
      return GRPFRUSTUM_OUTSIDE;
    if (pl[FRUSTRUM_PLANE_TOP].distance(p) < 0)
      return GRPFRUSTUM_OUTSIDE;
    if (pl[FRUSTRUM_PLANE_BOTTOM].distance(p) < 0)
      return GRPFRUSTUM_OUTSIDE;
    /*
    for (int i = 0; i < 6; i++) {

      if (pl[i].distance(p) < 0)
        return OUTSIDE;
    }
    */

    return(result);
}

int GRPFRUSTUM::sphereInFrustum(GRPVECTOR p, float raio)
{
    return GRPFRUSTUM_INSIDE;
}

int GRPFRUSTUM::boxInFrustum(GRPAABB b)
{

    return GRPFRUSTUM_INSIDE;

}

int GRPFRUSTUM::objectVisible(GRPMATRIX &mvp, GRPVECTOR *pos, const GLfloat &radius)
{




  /*

    GLfloat MVPMatrix[16];
    const GLfloat xPos=pos->vector[0];
    const GLfloat yPos=pos->vector[1];
    const GLfloat zPos=pos->vector[2];

    mvp.CopyTo(MVPMatrix);

    // The equation for a plane is: Ax + By + Cz + D = 0,
    // where A, B and C define the plane's normal vector, D is the distance from the origin to the plane,

    // and x, y and z are any points on the plane..
    // You can plug any point into the equation and if the result is 0 then the point lies on the plane. If the
    // result is greater than 0 then the point is in front of the plane, and if it's negative the point is behind the plane.

    enum term { A = 0, B, C, D };

    GLfloat leftPlane[4];
    leftPlane[A] = MVPMatrix[3] + MVPMatrix[0];
    leftPlane[B] = MVPMatrix[7] + MVPMatrix[4];
    leftPlane[C] = MVPMatrix[11] + MVPMatrix[8];
    leftPlane[D] = MVPMatrix[15] + MVPMatrix[12];

    // Normalise the plane
    GLfloat length = sqrtf(leftPlane[A] * leftPlane[A] + leftPlane[B] * leftPlane[B] + leftPlane[C] * leftPlane[C]);
    leftPlane[A] /= length;
    leftPlane[B] /= length;
    leftPlane[C] /= length;
    leftPlane[D] /= length;

    // Check the point's location with respect to the left plane of our viewing frustrum
    GLfloat distance = leftPlane[A] * xPos + leftPlane[B] * yPos + leftPlane[C] * zPos + leftPlane[D];
    if (distance <= -radius)
    {
        return OUTSIDE; // Bounding sphere is completely outside the left plane
    }

    // Check the point's location with respect to the right plane of our viewing frustum
    GLfloat rightPlane[4];
    rightPlane[A] = MVPMatrix[3] - MVPMatrix[0];
    rightPlane[B] = MVPMatrix[7] - MVPMatrix[4];
    rightPlane[C] = MVPMatrix[11] - MVPMatrix[8];
    rightPlane[D] = MVPMatrix[15] - MVPMatrix[12];

    // Normalise the plane
    length = sqrtf(rightPlane[A] * rightPlane[A] + rightPlane[B] * rightPlane[B] + rightPlane[C] * rightPlane[C]);
    rightPlane[A] /= length;
    rightPlane[B] /= length;
    rightPlane[C] /= length;
    rightPlane[D] /= length;

    distance = rightPlane[A] * xPos + rightPlane[B] * yPos + rightPlane[C] * zPos + rightPlane[D];
    if (distance <= -radius)
    {
        return OUTSIDE; // Bounding sphere is completely outside the right plane
    }

    // Check the point's location with respect to the bottom plane of our viewing frustum
    GLfloat bottomPlane[4];
    bottomPlane[A] = MVPMatrix[3] + MVPMatrix[1];
    bottomPlane[B] = MVPMatrix[7] + MVPMatrix[5];
    bottomPlane[C] = MVPMatrix[11] + MVPMatrix[9];
    bottomPlane[D] = MVPMatrix[15] + MVPMatrix[13];

    // Normalise the plane
    length = sqrtf(bottomPlane[A] * bottomPlane[A] + bottomPlane[B] * bottomPlane[B] + bottomPlane[C] * bottomPlane[C]);
    bottomPlane[A] /= length;
    bottomPlane[B] /= length;
    bottomPlane[C] /= length;
    bottomPlane[D] /= length;

    distance = bottomPlane[A] * xPos + bottomPlane[B] * yPos + bottomPlane[C] * zPos + bottomPlane[D];
    if (distance <= -radius)
    {
        return OUTSIDE; // Bounding sphere is completely outside the bottom plane
    }

    // Check the point's location with respect to the top plane of our viewing frustrum
    GLfloat topPlane[4];
    topPlane[A] = MVPMatrix[3] - MVPMatrix[1];
    topPlane[B] = MVPMatrix[7] - MVPMatrix[5];
    topPlane[C] = MVPMatrix[11] - MVPMatrix[9];
    topPlane[D] = MVPMatrix[15] - MVPMatrix[13];

    // Normalise the plane
    length = sqrtf(topPlane[A] * topPlane[A] + topPlane[B] * topPlane[B] + topPlane[C] * topPlane[C]);
    topPlane[A] /= length;
    topPlane[B] /= length;
    topPlane[C] /= length;
    topPlane[D] /= length;

    distance = topPlane[A] * xPos + topPlane[B] * yPos + topPlane[C] * zPos + topPlane[D];
    if (distance <= -radius)
    {
        return OUTSIDE; // Bounding sphere is completely outside the top plane
    }

    // Check the point's location with respect to the near plane of our viewing frustum
    GLfloat nearPlane[4];
    nearPlane[A] = MVPMatrix[3] + MVPMatrix[2];
    nearPlane[B] = MVPMatrix[7] + MVPMatrix[6];
    nearPlane[C] = MVPMatrix[11] + MVPMatrix[10];
    nearPlane[D] = MVPMatrix[15] + MVPMatrix[14];

    // Normalise the plane
    length = sqrtf(nearPlane[A] * nearPlane[A] + nearPlane[B] * nearPlane[B] + nearPlane[C] * nearPlane[C]);
    nearPlane[A] /= length;
    nearPlane[B] /= length;
    nearPlane[C] /= length;
    nearPlane[D] /= length;

    distance = nearPlane[A] * xPos + nearPlane[B] * yPos + nearPlane[C] * zPos + nearPlane[D];
    if (distance <= -radius)
    {
        return OUTSIDE; // Bounding sphere is completely outside the near plane
    }

    // Check the point's location with respect to the far plane of our viewing frustum
    GLfloat farPlane[4];
    farPlane[A] = MVPMatrix[3] - MVPMatrix[2];
    farPlane[B] = MVPMatrix[7] - MVPMatrix[6];
    farPlane[C] = MVPMatrix[11] - MVPMatrix[10];
    farPlane[D] = MVPMatrix[15] - MVPMatrix[14];

    // Normalise the plane
    length = sqrtf(farPlane[A] * farPlane[A] + farPlane[B] * farPlane[B] + farPlane[C] * farPlane[C]);
    farPlane[A] /= length;
    farPlane[B] /= length;
    farPlane[C] /= length;
    farPlane[D] /= length;

    distance = farPlane[A] * xPos + farPlane[B] * yPos + farPlane[C] * zPos + farPlane[D];
    if (distance <= -radius)
    {
        return OUTSIDE; // Bounding sphere is completely outside the far plane
    }
    */
    // If we got here, then the bounding sphere is within at least all six sides of the view frustum, so it's visible and we should draw it!
    return GRPFRUSTUM_INSIDE;
}