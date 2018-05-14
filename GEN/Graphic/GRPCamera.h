/*------------------------------------------------------------------------------------------
//  GRPCAMERA.H
*/
/**
// \class
//
//  Graphic Generic Camera class
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 13/05/2014 12:23:57
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPCAMERA_H_
#define _GRPCAMERA_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include <math.h>

#include "GRPNode.h"
#include "GRPFrustum.h"
#include "GRPMatrix.h"
#include "GRPViewport.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum GRPCAMERA_TYPE
{
  GRPCAMERA_TYPE_UNKNOWN=-1,
  GRPCAMERA_TYPE_FREE,
  GRPCAMERA_TYPE_TARGET,
  GRPCAMERA_TYPE_AIM_UP
};

/*---- CLASS -----------------------------------------------------------------------------*/
class GRPVIEWPORT;
class GRPRENDERTARGET;
class GRPCAMERAFPS;


class GRPCAMERA : public GRPNODE
{
  public:

                          GRPCAMERA               ();
    virtual               ~GRPCAMERA              ();

    void                  BuildLookAtMatrix       ();

    virtual void          BuildLookAt             ();
    void                  BuildLookAtTargetMatrix ();
    void                  BuildLookAtFreeMatrix   ();

    void                  SetTarget               (GRPNODE*       nodetarget);
    void                  CopyFrom                (GRPCAMERA*     cam);
    void                  UpdateFov               ();

    virtual bool          SetViewport             (GRPVIEWPORT* viewport);
    GRPVIEWPORT*          GetViewport             ();

    operator              GRPMATRIX               ()                              { return ((*this).viewmatrix);                                                                }

    GRPNODE*              GetTarget               ()                              { return target;                                                                              }
    GRPNODE*              GetUpNode               ()                              { return upnode;                                                                              }
  //GRPSPRITE*            GetImagePlane           ()                              { return ImagePlane;                                                                        }
    void                  SetUpNode               (GRPNODE*       up)             { upnode=up;                                                                                  }
    void                  SetUpVector             (GRPVECTOR*     upv)            { this->UpVector=*upv;                                                                        }
  //void                  SetImagePlane           (GRPSPRITE*     texturePlane)   { this->SetMaterial(texturePlane->GetNode()->GetMaterial());  this->ImagePlane=texturePlane;}

    void                  SetType                 (GRPCAMERA_TYPE type)           { this->Type=type;                                                                            }
    GRPCAMERA_TYPE        GetCameraType           ()                              { return this->Type;                                                                          }
    XDWORD                GetSubjectType          ()                              { return GRPNAMED_TYPE_GRPCAMERA;                                                             }
    GRPVECTOR*            GetEyePosition          ()                              { return &EyePosition;                                                                        }
    void                  SetEyePosition          (GLFLOAT x,GLFLOAT y,GLFLOAT z) { this->EyePosition=GRPVECTOR(x,y,z);                                                       }
    GRPVECTOR*            GetUpVector             ()                              { return &UpVector;                                                                           }
    void                  SetUpVector             (GLFLOAT x,GLFLOAT y,GLFLOAT z) { this->UpVector=GRPVECTOR(x,y,z);                                                          }
    GRPVECTOR*            Getinterestposition     ()                              { return &interestposition;                                                                   }
    void                  Setinterestposition     (GLFLOAT x,GLFLOAT y,GLFLOAT z) { this->interestposition=GRPVECTOR(x,y,z);                                                  }
    void                  SetAspect               (GLFLOAT w,GLFLOAT h)           { this->AspectWidth=w; this->AspectWidth=h;                                                 }
    void                  SetFilm                 (GLFLOAT w,GLFLOAT h)           { this->FilmWidth=w; this->FilmHeight=h;                                                      }
    GRPFRUSTUM*           GetFrustum              ()                              { return &this->frustum;                                                                      }

    bool                  SetValue                (GRPANIMATIONCURVETYPE type,    GRPANIMATIONCURVEAXIS subtype, double value,  double base);
    bool                  CreateDepthBuffer       (XDWORD width,XDWORD height,    GRPPROPERTYMODE mode=GRPPROPERTYMODE_32_RGBA_8888);
    bool                  CreateRenderBuffer      (XDWORD width,XDWORD height,    GRPPROPERTYMODE mode=GRPPROPERTYMODE_32_RGBA_8888);



    GRPRAY                CreateRayFromScreenPoint(GLFLOAT x,GLFLOAT y);

    bool                  Orbit                   (float x, float y);


    GRPVECTOR             unProject               (int x, int y, float depth);


    GRPCAMERA_TYPE        Type;

    GRPMATRIX             viewmatrix;

    // these two are only copied form other camera, don't really seem useful
    GRPVECTOR             EyePosition;
    GRPVECTOR             UpVector;

    GRPVECTOR             interestposition;

    GLFLOAT               AspectWidth;
    GLFLOAT               AspectHeight;
    GLFLOAT               FilmWidth;
    GLFLOAT               FilmHeight;

    GLFLOAT               NearPlane;
    GLFLOAT               FarPlane;

    GLFLOAT               fov;
    GLFLOAT               lens;

    GLFLOAT               FocalLength;

    GLFLOAT               BackPlaneDistance;
    GLFLOAT               BackPlaneScaleX;
    GLFLOAT               BackPlaneScaleY;

    bool                  UseFocalLength;

  //GRPSPRITE*            ImagePlane;
    GRPNODE*              target;
    GRPNODE*              upnode;

    GRPVIEWPORT           viewport;

    GRPVECTOR             l;

    GRPVECTOR             z;
    GRPVECTOR             x;
    GRPVECTOR             y;

    GRPVECTOR             eye,obj,up;

    GRPRENDERTARGET*      rendertarget;
    GRPTEXTURE*           depthtexture;
    GRPTEXTURE*           rendertexture;
    GLUINT                texturehandle;

    GRPFRUSTUM            frustum;

  private:

    void            Clean()
                    {
                      target            = NULL;
                      upnode            = NULL;
                      depthtexture      = NULL;

                      AspectWidth       = 1080.0f;
                      AspectHeight      = 960.0f;

                      FilmWidth         = 1080.0f;
                      FilmHeight        = 960.0f;

                      fov               = 45.0f;
                      lens              = 43.456f;

                      //NearPlane       = 0.0001f;
                      NearPlane         = 1.0f;
                      FarPlane          = 10000000.0f;

                      FocalLength       = 35.0f;
                      //ImagePlane        = NULL;
                      UseFocalLength    = false;

                      Type              = GRPCAMERA_TYPE_FREE;

                      modified          = true;
                      scaleChanged      = true;
                      positionChanged   = true;
                      rotationChanged   = true;

                      UsesPostRotation  = true;
                      UsesPreRotation   = true;

                      depthtexture      = NULL;
                      rendertarget      = NULL;
                      rendertexture     = NULL;
                      texturehandle     =0;
                    }
};

class GRPCAMERAFPS : public GRPCAMERA
{
public:
    enum Camera_Movement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };


    // Default camera values
    float YAW;
    float PITCH;
    float SPEED;
    float SENSITIVTY;
    float ZOOM;

    // Camera Attributes
    //GRPVECTOR Position;
    GRPVECTOR Front;
    //GRPVECTOR Up;
    GRPVECTOR Right;
    GRPVECTOR WorldUp;

    // Eular Angles
    float Yaw;
    float Pitch;

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // Constructor with vectors
    GRPCAMERAFPS(GRPVECTOR position = GRPVECTOR(0.0f, 0.0f, 0.0f),
                 GRPVECTOR up = GRPVECTOR(0.0f, 1.0f, 0.0f),
                 float      yaw = -90.0f,
                 float      pitch = 0.0f);

    GRPCAMERAFPS(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    void ProcessMovement        (Camera_Movement direction, float deltaTime);
    void ProcessMouseMovement   (float xoffset, float yoffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll     (float yoffset);

private:

    double radians(double degrees)
    {
        return degrees * (0.01745329251994329576923690768489);
    }

    void BuildLookAt();

    void updateCameraVectors();

    void Clean()
    {
        // Default camera values
        YAW = -90.0f;
        PITCH = 0.0f;
        SPEED = 2.5f;
        SENSITIVTY = 0.1f;
        ZOOM = 45.0f;
    }

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif


