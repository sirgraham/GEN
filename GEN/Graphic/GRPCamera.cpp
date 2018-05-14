
/*------------------------------------------------------------------------------------------
//  GRPCAMERA.CPP
//
//  Graphic Generic Camera class
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 13/05/2014 12:32:22
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPCamera.h"
#include "GRPFactory.h"
#include "GRPRenderTarget.h"
#include "GRPMaterialLibrary.h"
#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  GRPCAMERA::GRPCAMERA
*/
/**
//
//  Class Constructor GRPCAMERA
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/05/2014 13:06:07
//
*/
/*-----------------------------------------------------------------*/
GRPCAMERA::GRPCAMERA()
{
  Clean();
  this->SetName(__L("Default Camera Name"));
  this->viewmatrix.LoadIdentity();

  up                = GRPVECTOR( 0.0f, 1.0f, 0.0);
  SetUpVector                  ( 0.0f, 1.0f, 0.0);
  interestposition  = GRPVECTOR( 0.0f, 0.0f,-1.0);
  eye               = GRPVECTOR( 0.0f, 0.0f, 0.0);
  fov = 54.0f;

  GRPNAMED::classtype = GRPNAMED_TYPE_GRPCAMERA;
}


/*-------------------------------------------------------------------
//  GRPCAMERA::~GRPCAMERA
*/
/**
//
//  Class Destructor GRPCAMERA
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/05/2014 12:35:29
//
*/
/*-----------------------------------------------------------------*/
GRPCAMERA::~GRPCAMERA()
{
    Publish(GRPMSG_DESTROYED);

    if (rendertarget)
    {
        delete(rendertarget);
        rendertarget = NULL;
    }
}

/*-------------------------------------------------------------------
//  GRPCAMERA::BuildCameraLookAtmatrix
*/
/**
//
//  Builds Camera LookAt Frustum Proyection matrix for MVP multiply
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/05/2014 13:06:23
//
*/
/*-----------------------------------------------------------------*/
void GRPCAMERA::BuildLookAtMatrix()
{
  this->modified=true;

  this->UsesPostRotation=false;

  GRPNODE::Update();
  GRPNODE::UpdateWorldMatrix();

  BuildLookAt();

  //-----------------

  up.Normalize();

  z.vector[0] = eye.vector[0]-obj.vector[0];
  z.vector[1] = eye.vector[1]-obj.vector[1];
  z.vector[2] = eye.vector[2]-obj.vector[2];
  z.Normalize();

  x.CrossProduct(&up,&z);
  x.Normalize();

  y.CrossProduct(&z,&x);
  y.Normalize();

  this->viewmatrix.matrix[0][0] = x.vector[0];
  this->viewmatrix.matrix[0][1] = y.vector[0];
  this->viewmatrix.matrix[0][2] = z.vector[0];
  this->viewmatrix.matrix[0][3] = 0.0f;

  this->viewmatrix.matrix[1][0] = x.vector[1];
  this->viewmatrix.matrix[1][1] = y.vector[1];
  this->viewmatrix.matrix[1][2] = z.vector[1];
  this->viewmatrix.matrix[1][3] = 0.0f;

  this->viewmatrix.matrix[2][0] = x.vector[2];
  this->viewmatrix.matrix[2][1] = y.vector[2];
  this->viewmatrix.matrix[2][2] = z.vector[2];
  this->viewmatrix.matrix[2][3] = 0.0f;

  this->viewmatrix.matrix[3][0] = (-x.vector[0] * eye.vector[0] - x.vector[1] * eye.vector[1] - x.vector[2] * eye.vector[2]);
  this->viewmatrix.matrix[3][1] = (-y.vector[0] * eye.vector[0] - y.vector[1] * eye.vector[1] - y.vector[2] * eye.vector[2]);
  this->viewmatrix.matrix[3][2] = (-z.vector[0] * eye.vector[0] - z.vector[1] * eye.vector[1] - z.vector[2] * eye.vector[2]);

  this->modified=false;
  this->worldmatrix_is_pending=true;
}

/*-------------------------------------------------------------------
//  GRPCAMERA::BuildLookAt
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/06/2016 23:41:20
//
*/
/*-----------------------------------------------------------------*/
void GRPCAMERA::BuildLookAt()
{
    if (target == NULL)
        this->Type = GRPCAMERA_TYPE_FREE;

    switch (this->Type)
    {
        case GRPCAMERA_TYPE_FREE:   BuildLookAtFreeMatrix   (); break;
        case GRPCAMERA_TYPE_TARGET: BuildLookAtTargetMatrix (); break;
        default:                    return;                     break;
    }

    this->frustum.Update(this);
}

/*-------------------------------------------------------------------
//  GRPCAMERA::BuildLookAtTargetMatrix
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      02/06/2015 10:05:22
//
*/
/*-----------------------------------------------------------------*/
void GRPCAMERA::BuildLookAtFreeMatrix()
{

  eye =this->GetWorldMatrix()*GRPVECTOR(0.0,0.0,0.0);
  obj =this->GetWorldMatrix()*GRPVECTOR(0.0,0.0,-1.0);
  up  =this->rotationmatrix*UpVector;//GRPVECTOR(0.0,1.0,0.0);
}


/*-------------------------------------------------------------------
//  GRPCAMERA::BuildLookAtTargetMatrix
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      02/06/2016 12:56:20
//
*/
/*-----------------------------------------------------------------*/
void GRPCAMERA::BuildLookAtTargetMatrix()
{
  target->modified=true;

  if (upnode!=NULL)
    upnode->modified=true;

  target->Update();
  target->UpdateWorldMatrix();

  if (upnode!=NULL)
  {
    upnode->Update();
    upnode->UpdateWorldMatrix();
  }

  eye =this->GetWorldMatrix()*GRPVECTOR(0.0,0.0,0.0);
  obj =target->GetWorldMatrix()*GRPVECTOR(0.0,0.0,0.0);

  if (upnode!=NULL)
    up  =upnode->GetWorldMatrix()*GRPVECTOR(0.0,0.0,0.0);
  else
    up  =this->rotationmatrix*GRPVECTOR(0.0,1.0,0.0);
}

/*-------------------------------------------------------------------
//  GRPCAMERA::SetTarget
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      15/06/2015 11:27:57
//
//  @param        nodetarget :
*/
/*-----------------------------------------------------------------*/
void  GRPCAMERA::SetTarget(GRPNODE* nodetarget)
{
  if (nodetarget==NULL)
    this->Type=GRPCAMERA_TYPE_FREE;
  else
  {
    target=nodetarget;
    this->Type=GRPCAMERA_TYPE_TARGET;
  }
}


/*-------------------------------------------------------------------
//  GRPCAMERA::SetValue
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/07/2015 17:08:09
//
//  @return       bool :
//
//  @param        type :
//  @param        subtype :
//  @param        value :
//  @param        base :
*/
/*-----------------------------------------------------------------*/
bool GRPCAMERA::SetValue(GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS subtype, double value,double base)
{

  switch(type)
  {
  case GRPANIMATIONCURVETYPE_FOV          : this->fov=(GLFLOAT)value;   return true;  break;
  case GRPANIMATIONCURVETYPE_FIELDOFVIEWX:
  case GRPANIMATIONCURVETYPE_FIELDOFVIEWY:                              return false; break;
  default: break;

  }
  return GRPNODE::SetValue(type,subtype,value,base);
}

/*-------------------------------------------------------------------
//  GRPCAMERA::CopyFrom
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      02/06/2015 10:05:09
//
//  @param        cam :
*/
/*-----------------------------------------------------------------*/
void  GRPCAMERA::CopyFrom (GRPCAMERA* cam)
{
  this->AspectHeight      =cam->AspectHeight;
  this->AspectWidth       =cam->AspectWidth;
  this->BackPlaneDistance =cam->BackPlaneDistance;
  this->BackPlaneScaleX   =cam->BackPlaneScaleX;
  this->BackPlaneScaleY   =cam->BackPlaneScaleY;
  this->BaseMaterial      =cam->BaseMaterial;
  this->FarPlane          =cam->FarPlane;
  this->FilmHeight        =cam->FilmHeight;
  this->FilmWidth         =cam->FilmWidth;
  this->FocalLength       =cam->FocalLength;
  this->fov               =cam->fov;
  //this->ImagePlane        =cam->ImagePlane;
  this->interestposition  =cam->interestposition;

  this->lens              =cam->lens;
  this->NearPlane         =cam->NearPlane;
  this->Type              =cam->Type;

  this->EyePosition       =cam->EyePosition;
  this->UpVector          =cam->UpVector;

  this->UsesPreRotation   = cam->UsesPreRotation;
  this->UsesPostRotation  = cam->UsesPostRotation;

  modified                = true;
  scaleChanged            = true;
  positionChanged         = true;
  rotationChanged         = true;

}


/*-------------------------------------------------------------------
//  GRPCAMERA::UpdateFov
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/06/2016 0:32:33
//
*/
/*-----------------------------------------------------------------*/
void  GRPCAMERA::UpdateFov()
{
    //GLFLOAT FoV=GRPMATRIX_RAD_TO_DEG*2.0f*(float)atan((this->AspectWidth/2.0f)/this->FocalLength);

    GLFLOAT focalLength=this->FocalLength;

    /*
    if (focalLength>=200.0)
      FoV=10.286f;
    else
      if (focalLength>=100.0)
        FoV=15.189f;
      else
        if (focalLength>=85.0)
          FoV=23.913f;
        else
          if (focalLength>=50.0)
            FoV=39.598f;
          else
            if (focalLength>=35.0)
              FoV=54.432f;
            else
              if (focalLength>=28.0)
                FoV=65.47f;
              else
                if (focalLength>=24.0)
                  FoV=73.74f;
                else
                  if (focalLength>=20.0)
                    FoV=83.974f;
                  else
                      FoV=100.389f;
    */
    //processed function
    //FoV = 4.287285 + (136.25) / (1 + pow((focalLength / 24.75836), 1.545845));

    //FOV = 2.0f*atanf((pixelwidth / 2.0f) / focalLength);

    GLFLOAT FoV =float( 1.595374 + 204.838326/(1.0 + pow((double(focalLength)/ 14.30451), 1.180062)));

    this->fov=FoV;
    this->BuildLookAtMatrix();
  //  this->modified = true;


}

/*-------------------------------------------------------------------
//  GRPCAMERA::SetViewport
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/06/2016 0:42:37
//
//  @return       bool :
//
//  @param        viewport :
*/
/*-----------------------------------------------------------------*/
bool GRPCAMERA::SetViewport             (GRPVIEWPORT* viewport)
{
    this->viewport=*viewport;
    return true;
}

/*-------------------------------------------------------------------
//  GRPCAMERA::GetViewport
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/06/2016 1:29:23
//
//  @return       GRPVIEWPORT* :
//
*/
/*-----------------------------------------------------------------*/
GRPVIEWPORT* GRPCAMERA::GetViewport()
{
    return &this->viewport;
}

/*-------------------------------------------------------------------
//  GRPCAMERA::CreateRenderBuffer
*/
/**
//
//  Creates diffuse render buffer
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/07/2017 20:03:12
//
//  @return       bool :
//
//  @param        width :
//  @param        height :
//  @param        mode :
*/
/*-----------------------------------------------------------------*/
bool GRPCAMERA::CreateRenderBuffer(XDWORD width,XDWORD height, GRPPROPERTYMODE mode)
{
      if (width==0 || height==0) return false;

      GRPTEXTURE* texture = new GRPTEXTURE();
      if (!texture)
          return false;

      if (this->Type==GRPCAMERA_TYPE_TARGET)
        texture->Create(NULL,width*height*2,width,height,4,8);
      else
        texture->CreateCubeMap(width,height,4,8);


      texture->GetHandle(); //avoid lazy

      texture->GetName()->Format(__L("%s::DiffuseRenderTexture"),this->GetName()->Get());
      this->rendertexture=texture;

      if (!GRPMATERIALLIBRARY::Get()->Create(texture))
          return false;

      rendertarget=new GRPRENDERTARGET(width,height);
      rendertarget->InitFrameBuffers(texture);
      rendertarget->Deselect();

      texturehandle=texture->GetHandle();
      rendertexture=texture;
      return true;
}

/*-------------------------------------------------------------------
//  GRPCAMERA::CreateDepthBuffer
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/06/2016 0:05:28
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPCAMERA::CreateDepthBuffer(XDWORD width,XDWORD height, GRPPROPERTYMODE mode)
{
      OPENGLCHECKERROR(__L(" GRPCAMERA::CreateDepthBuffer"));

      if (width==0 || height==0) return false;

      GRPTEXTURE* texture = new GRPTEXTURE();
      if (!texture)
          return false;

      if (this->Type==GRPCAMERA_TYPE_TARGET)
        texture->Create(NULL,width*height*2,width,height,1,16,GRPTEXTURELAYERTYPE_DEPTH);
      else
        texture->CreateCubeMap(width,height,1,16,GRPTEXTURELAYERTYPE_DEPTH);


      texture->GetHandle(); //avoid lazy

      texture->GetName()->Format(__L("%s::DepthRenderTexture"),this->GetName()->Get());
      this->depthtexture=texture;

      if (!GRPMATERIALLIBRARY::Get()->Create(texture))
          return false;

      if (rendertarget)
          delete(rendertarget);

      rendertarget=new GRPRENDERTARGET(width,height);
      rendertarget->InitFrameBuffers(texture,GRPRENDERTARGETCOMPONENT_DEPTH);
      rendertarget->Deselect();

      texturehandle=texture->GetHandle();

      return true;
}

/*-------------------------------------------------------------------
//  GRPCAMERA::CreateRayFromScreenPoint
*/
/**
//
//  Create a ray from the frustum viewport point
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      03/05/2017 19:38:45
//
//  @return       const :
//
*/
/*-----------------------------------------------------------------*/
GRPRAY GRPCAMERA::CreateRayFromScreenPoint(GLFLOAT x,GLFLOAT y)
{

GRPRAY ray;
GLFLOAT screenWidth,screenHeight;
this->viewport.GetWindowSize(screenWidth,screenHeight);
/*

        // The ray Start and End positions, in Normalized Device Coordinates (Have you read Tutorial 4 ?)

  GLFLOAT screenx=((float)x/(float)screenWidth  - 0.5f) * 2.0f; // [0,1024] -> [-1,1]
  GLFLOAT screeny=(((float)y/(float)screenHeight - 0.5f) * 2.0f); // [0, 768] -> [-1,1]

screenx*=tan(fov/2.0f)/2.0*viewport.GetAspectRatio();
screeny*=tan(fov/2.0f)/2.0;

GRPMATR IX view=*this;
          view.Inverse();

GRPVECTOR p1(screenx*NearPlane,screeny*NearPlane,NearPlane);
GRPVECTOR p2(screenx*FarPlane,screeny*FarPlane,FarPlane);

p1=view*p1;
p2=view*p2;

ray.start       =GRPPOINT(this->GetPosition()->vector[0],this->GetPosition()->vector[1],this->GetPosition()->vector[2]);
//ray.start.w     =1.0f;
ray.end         =GRPPOINT(this->target->GetPosition()->vector[0]+(this->UpVector.vector[0]*screeny),this->target->GetPosition()->vector[1]+(this->UpVector.vector[1]*screeny),this->target->GetPosition()->vector[2]);
//ray.end.w       =1.0f;
ray.direction   =(ray.end-ray.start);
ray.length      =ray.direction.GetLengthf();
ray.direction.Normalize();
*/

double ndsx = (((float)x/(float)screenWidth)* 2.0f)-1.0f;
//ndsx/=this->viewport.GetAspectRatio();
double ndsy = (((float)y/(float)screenHeight)* 2.0f)-1.0f;;
//ndsy/=this->viewport.GetAspectRatio();
double ndsz = -this->NearPlane;//+1.0f;

ndsx*=(tan(((double(fov)/2.0)* PI) / 360.0))*this->viewport.GetAspectRatio();;
ndsy*=(tan(((double(fov)/2.0)* PI) / 360.0));

GRPVECTOR ray_nds = GRPVECTOR((float)ndsx, (float)ndsy, (float)ndsz);

GRPMATRIX view_matrix=*this;
          view_matrix.Inverse();

ray.start=view_matrix.TransformPoint(GRPPOINT(0.0f,0.0f,0.0f));
ray.start.w=0.0f;

GRPVECTOR ray_wor= view_matrix.TransformPoint(ray_nds);
          ray_wor=ray_wor-GRPVECTOR(ray.start);
          ray_wor.Normalize();



ray.direction=ray_wor;

GRPMATRIX mv=(this->modelmatrix*this->viewmatrix);

XDEBUG_PRINTCOLOR(0,__L("Screen pos: [%f,%f]"),x,y);
XDEBUG_PRINTCOLOR(0,__L("Camera pos: [%f,%f,%f]"),this->position.vector[0],this->position.vector[1],this->position.vector[2]);
XDEBUG_PRINTCOLOR(0,__L("Ray start: [%f,%f,%f]"),ray.start.x,ray.start.y,ray.start.z);
//XDEBUG_PRINTCOLOR(0,__L("Ray end: [%f,%f,%f]"),p2.vector[0],p2.vector[1],p2.vector[2]);
XDEBUG_PRINTCOLOR(0,__L("Ray direction: [%f,%f,%f]"),ray.direction.vector[0],ray.direction.vector[1],ray.direction.vector[2]);
XDEBUG_PRINTCOLOR(0,__L("Ray length %f"),ray.length);


return ray;
}

/*-------------------------------------------------------------------
//  GRPCAMERA::Orbit
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/09/2016 14:35:42
//
//  @return       bool :
//
//  @param        x :
//  @param        y :
*/
/*-----------------------------------------------------------------*/
bool GRPCAMERA::Orbit(float x, float y)
{
  double tx     =0.0, ty=0.0, tz=0.0;
  double radius =0.0;
  if (target)
  {
   tx     = this->target->GetPosition()->vector[0];
   ty     = this->target->GetPosition()->vector[1];
   tz     = this->target->GetPosition()->vector[2];
   radius= GRPVECTOR((GRPVECTOR)*GetPosition()-(GRPVECTOR)*target->GetPosition()).GetLengthf();
  }
  else
  {
    radius= GRPVECTOR((GRPVECTOR)*GetPosition()).GetLengthf();
  }

  double ax=x;
  double ay=y;

  double eyeX   = tx + radius*cos(ax)*sin(ay);
  double eyeY   = ty + radius*sin(ax)*sin(ay);
  double eyeZ   = tz + radius*cos(ay);

  this->SetPosition((float)eyeX,(float)eyeY,(float)eyeZ);
  return false;
}



//---- FPS CAMERA



    // Constructor with vectors
GRPCAMERAFPS::GRPCAMERAFPS(GRPVECTOR position, GRPVECTOR up,  float yaw, float pitch) : Front(GRPVECTOR(0.0f, 0.0f, -1.0f)),MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
        this->position = position;
        this->position.vector[3] = 0.0f;

        this->UpVector = up;

        WorldUp = up;
        WorldUp.vector[3] = 0.0f;

        Yaw = yaw;
        Pitch = pitch;

        updateCameraVectors();
}

GRPCAMERAFPS::GRPCAMERAFPS(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(GRPVECTOR(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
this->position = GRPVECTOR(posX, posY, posZ);
WorldUp = GRPVECTOR(upX, upY, upZ);
Yaw = yaw;
Pitch = pitch;
updateCameraVectors();
}

void GRPCAMERAFPS::ProcessMovement(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        position += Front * velocity;
    if (direction == BACKWARD)
        position -= Front * velocity;
    if (direction == LEFT)
        position -= Right * velocity;
    if (direction == RIGHT)
        position += Right * velocity;
}

void GRPCAMERAFPS::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void GRPCAMERAFPS::ProcessMouseScroll(float yoffset)
{
    if (Zoom >= 1.0f && Zoom <= 45.0f)
        Zoom -= yoffset;
    if (Zoom <= 1.0f)
        Zoom = 1.0f;
    if (Zoom >= 45.0f)
        Zoom = 45.0f;
}

void GRPCAMERAFPS::updateCameraVectors()
{
    // Calculate the new Front vector
    GRPVECTOR   front;
                front.vector[0] = float(cos(radians(Yaw)) * cos(radians(Pitch)));
                front.vector[1] = float(sin(radians(Pitch)));
                front.vector[2] = float(sin(radians(Yaw)) * cos(radians(Pitch)));
                front.Normalize();
    Front   =   front;

    // Also re-calculate the Right and Up vector
    GRPVECTOR   crossFrontUp;
                crossFrontUp.CrossProduct(&this->Front, &this->WorldUp);
                crossFrontUp.Normalize();
    Right   =   crossFrontUp;

    GRPVECTOR   crossRightFront;
                crossRightFront.CrossProduct(&this->Right, &this->Front);
    this->up =  crossRightFront;

    //Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    //Up = glm::normalize(glm::cross(Right, Front));
}


void GRPCAMERAFPS::BuildLookAt()
{
    GRPVECTOR f;
    f = (position + Front);
    f-= eye;
    f.Normalize();

    GRPVECTOR s;
    s.CrossProduct(&f,&up);
    s.Normalize();

    GRPVECTOR u;
    u.CrossProduct(&s, &f);
    u.Normalize();

    this->viewmatrix.matrix[0][0] = s.vector[0];
    this->viewmatrix.matrix[1][0] = s.vector[1];
    this->viewmatrix.matrix[2][0] = s.vector[2];
    this->viewmatrix.matrix[0][1] = u.vector[0];
    this->viewmatrix.matrix[1][1] = u.vector[1];
    this->viewmatrix.matrix[2][1] = u.vector[2];
    this->viewmatrix.matrix[0][2] = -f.vector[0];
    this->viewmatrix.matrix[1][2] = -f.vector[1];
    this->viewmatrix.matrix[2][2] = -f.vector[2];
    this->viewmatrix.matrix[3][0] = -GRPVECTOR::DotProduct(&s, &eye);
    this->viewmatrix.matrix[3][1] = -GRPVECTOR::DotProduct(&u, &eye);
    this->viewmatrix.matrix[3][2] =  GRPVECTOR::DotProduct(&f, &eye);

}