
/*------------------------------------------------------------------------------------------
//  GRPNODE.CPP
//
//  dsdsadsa
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 24/08/2014 11:27:22
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPNode.h"

#include "XMemory.h"
#include "GRPAnimation.h"
#include "GRPElement.h"
#include "GRPProfiler.h"

#include "XMemory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/


/*---- CLASS MEMBERS ---------------------------------------------------------------------*/



/*-------------------------------------------------------------------
//  GRPATTACHEABLE::GetParent
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      29/10/2015 10:56:57
//
//  @return       GRPNODE* :
//
*/
/*-----------------------------------------------------------------*/
GRPNODE*  GRPATTACHEABLE::GetParent()
{
  return parent;
}

/*-------------------------------------------------------------------
//  GRPATTACHEABLE::SetParent
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      29/10/2015 10:57:01
//
//  @param        p :
*/
/*-----------------------------------------------------------------*/
void  GRPATTACHEABLE::SetParent(GRPNODE* p)
{
  if (p==this) return;

  if (parent!=NULL)
    Dettach();

  if (p!=NULL)
  {
    this->IsOrphan=false;
    this->parent=p;
    this->parent->Attachees++;
    p->AddObserver(this);
  }


}

/*-------------------------------------------------------------------
//  GRPATTACHEABLE::Dettach
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      29/10/2015 10:57:11
//
*/
/*-----------------------------------------------------------------*/
void  GRPATTACHEABLE::Dettach ()
{
  if (this->parent!=NULL)
  {
  this->parent->Attachees--;
  this->IsOrphan=true;
  this->parent->Notify(GRPMSG_DETTACHED,this);
  this->parent=NULL;
  }
}


/*-------------------------------------------------------------------
//  GRPNODE::GRPNODE
*/
/**
//
//  Class Constructor GRPNODE
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/08/2014 11:30:40
//
*/
/*-----------------------------------------------------------------*/
GRPNODE::GRPNODE()
{
  GRPPROFILER::nNodes++;

  Clean();
  managed=false;
  this->name.Set(__L("unnassigned"));
  this->modelmatrix.LoadIdentity();
  this->worldmatrix.LoadIdentity();

  GRPNAMED::classtype = GRPNAMED_TYPE_GRPNODE;
}

/*-------------------------------------------------------------------
//  GRPNODE::~GRPNODE
*/
/**
//
//   Class Destructor GRPNODE
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 9:53:55
//
*/
/*-----------------------------------------------------------------*/
GRPNODE::~GRPNODE()
{
  GRPPROFILER::nNodes--;
  this->Publish(GRPMSG_DESTROYED);
  this->data.Clear();
  animables.DeleteAll();
}

/*-------------------------------------------------------------------
//  GRPNODE::GetModelMatrix
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 9:57:09
//
//  @return       GRPMATRIX* :
//
*/
/*-----------------------------------------------------------------*/
GRPMATRIX* GRPNODE::GetModelMatrix()
{
  return &modelmatrix;
}

/*-------------------------------------------------------------------
//  GRPNODE::GetMatrix
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 9:57:04
//
//  @return       GRPMATRIX :
//
*/
/*-----------------------------------------------------------------*/
GRPMATRIX GRPNODE::GetMatrix()
{
  return modelmatrix;
}

/*-------------------------------------------------------------------
//  GRPNODE::GetLocalMatrix
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 9:57:00
//
//  @return       GRPMATRIX :
//
*/
/*-----------------------------------------------------------------*/
GRPMATRIX GRPNODE::GetLocalMatrix()
{
  return (*stack.Get()) * modelmatrix;
}

/*-------------------------------------------------------------------
//  GRPNODE::GetCurrentMatrix
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 9:56:56
//
//  @return       GRPMATRIX :
//
*/
/*-----------------------------------------------------------------*/
GRPMATRIX GRPNODE::GetCurrentMatrix()
{
  return modelmatrix;
}


/*-------------------------------------------------------------------
//  GRPNODE::SetValue
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/02/2015 17:17:25
//
//  @param        type :
//  @param        subtype :
//  @param        value :
*/
/*-----------------------------------------------------------------*/
bool GRPNODE::SetValue(GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS subtype, double value,double base)
{

  switch (type)
  {
  case GRPANIMATIONCURVETYPE_VISIBILITY:
                                        modified=true;
                                        this->visibility=value;
                                        break;

  case GRPANIMATIONCURVETYPE_TRANSLATION:
                                        modified=true;
                                        positionChanged=true;
                                        switch(subtype)
                                        {
                                        case GRPANIMATIONCURVEAXIS_X: this->position.vector[0]=(float) value; break;
                                        case GRPANIMATIONCURVEAXIS_Y: this->position.vector[1]=(float) value; break;
                                        case GRPANIMATIONCURVEAXIS_Z: this->position.vector[2]=(float) value; break;
                                        default:
                                            return false;
                                        }
                                        break;
  case GRPANIMATIONCURVETYPE_ROTATION:
                                        modified=true;
                                        rotationChanged=true;
                                        switch(subtype)
                                        {
                                        case GRPANIMATIONCURVEAXIS_X: this->rotation.vector[0]=(float) value; break;
                                        case GRPANIMATIONCURVEAXIS_Y: this->rotation.vector[1]=(float) value; break;
                                        case GRPANIMATIONCURVEAXIS_Z: this->rotation.vector[2]=(float) value; break;
                                        default:
                                            return false;
                                        }
                                        break;

  case GRPANIMATIONCURVETYPE_SCALE:
                                        modified=true;
                                        scaleChanged=true;
                                        if (value>0.0001f)
                                        switch(subtype)
                                        {
                                        case GRPANIMATIONCURVEAXIS_X:   this->scale.vector[0]=(float) value;  break;
                                        case GRPANIMATIONCURVEAXIS_Y:   this->scale.vector[1]=(float) value;  break;
                                        case GRPANIMATIONCURVEAXIS_Z:   this->scale.vector[2]=(float) value;  break;
                                        default:
                                            return false;
                                        }
                                        break;
  case GRPANIMATIONCURVETYPE_USERVALUE:
                                        this->Value=value;
                                        for (XDWORD e=0;e<this->animables.GetSize();e++)
                                          animables.FastGet(e)->SetValue(type,subtype, value,base);
                                        return true;
                                        break;

  case GRPANIMATIONCURVETYPE_DIFFUSEFACTOR:
                                        this->BaseMaterial.SetIntensity((float)value);
                                        for (XDWORD e=0;e<this->animables.GetSize();e++)
                                          animables.FastGet(e)->SetValue(type,subtype, value,base);

                                        break;
  default:
                                        //no se reconoce la curva, la desactivamos
                                        return false;
                                        break;
  }

  return true;
}

/*-------------------------------------------------------------------
//  GRPNODE::GetIsVisible
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 9:50:31
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPNODE::GetIsVisible()
{
  if (visibility>0.5f && this->parent && visibility_inheritance)
  {
  return this->parent->GetIsVisible();
  }

  return visibility>0.5f;
}

/*-------------------------------------------------------------------
//  GRPNODE::SetVisible
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 9:50:27
//
//  @param        f :
*/
/*-----------------------------------------------------------------*/
void GRPNODE::SetVisible(bool f)
{
  visibility=(f?1.0f:0.0f);
}

/*-------------------------------------------------------------------
//  GRPNODE::GetMaterial
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 9:50:22
//
//  @return       GRPMATERIAL* :
//
*/
/*-----------------------------------------------------------------*/
GRPMATERIAL* GRPNODE::GetMaterial()
{
  return material;
}

/*-------------------------------------------------------------------
//  GRPNODE::GetBaseMaterial
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 9:50:17
//
//  @return       GRPMATERIALATTRIBUTES* :
//
*/
/*-----------------------------------------------------------------*/
GRPMATERIALATTRIBUTES* GRPNODE::GetBaseMaterial()
{
  return &this->BaseMaterial;
}

/*-------------------------------------------------------------------
//  GRPNODE::SetBaseMaterial
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/01/2016 11:53:28
//
//  @param        newmaterial :
*/
/*-----------------------------------------------------------------*/
void GRPNODE::SetBaseMaterial(GRPMATERIALATTRIBUTES* newmaterial)
{
  this->BaseMaterial=*newmaterial;
  this->BaseMaterial.isupdated=true;
}

/*-------------------------------------------------------------------
//  GRPNODE::SetMaterial
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 9:50:13
//
//  @param        material :
*/
/*-----------------------------------------------------------------*/
void GRPNODE::SetMaterial(GRPMATERIAL* material)
{
  this->material = material;
  if (material)
  {
  this->BaseMaterial = *material->GetAttributes();
  this->BaseMaterial.isupdated=true;
  }
}


/*-------------------------------------------------------------------
//  GRPNODE::Copy
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      25/04/2017 16:51:07
//
//  @param        node :
*/
/*-----------------------------------------------------------------*/
void GRPNODE::Copy(GRPNODE* node)
{
  this->SetPosition (node->GetPosition()->vector[0],node->GetPosition()->vector [1],node->GetPosition()->vector [2]);
  this->SetRotation (node->GetRotation()->vector[0],node->GetRotation()->vector [1],node->GetRotation()->vector [2]);
  this->SetScale    (node->GetScale()->vector   [0],node->GetScale()->vector    [1],node->GetScale()->vector    [2]);

  this->prerotation               =node->prerotation;
  this->postrotation              =node->postrotation;
  this->UsesPreRotation           =node->UsesPreRotation;
  this->UsesPostRotation          =node->UsesPostRotation;
  this->UsesGeometricTranslation  =node->UsesGeometricTranslation;
  this->UsesGeometricRotation     =node->UsesGeometricRotation;
  this->UsesGeometricScale        =node->UsesGeometricScale;

  this->visibility                =node->visibility;
  this->geometricmatrix           =node->geometricmatrix;
  this->geometricposition         =node->geometricposition;
  this->geometricrotation         =node->geometricrotation;
  this->geometricscale            =node->geometricscale;
  this->degrees                   =node->degrees;

  this->InheritType               =node->InheritType;
  this->rendereable               =node->rendereable;
  this->ScalingPivot              =node->UsesScalingPivot;
  this->ScalingOffset             =node->UsesScalingOffset;

  this->UsesRotationOffset        =node->UsesRotationOffset;
  this->UsesRotationPivot         =node->UsesRotationPivot;

  this->NodeMode                  =node->NodeMode;

  this->modified=true;
  this->Init();
}

/*-------------------------------------------------------------------
//  GRPNODE::GetWorldMatrix
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/08/2014 11:31:38
//
//  @return       GRPMATRIX :
//
*/
/*-----------------------------------------------------------------*/
void GRPNODE::Update()
{
  if (modified)
  {
  GRPPROFILER::nNodesUpdates++;
  switch(NodeMode)
  {
  case GRPNODE_MODE_OGL:        GenUpdate();      break;
  case GRPNODE_MODE_MAX:        MaxUpdate();      break;
  case GRPNODE_MODE_MAYA:       MayaUpdate();     break;
  }
  worldmatrix_is_pending=true;
  modified=false;
  }

  inherited_visibility=this->visibility;
}

/*-------------------------------------------------------------------
//  GRPNODE::UpdateWorldMatrix
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/07/2015 11:13:39
//
*/
/*-----------------------------------------------------------------*/
void GRPNODE::UpdateWorldMatrix()
{
  bool mustUpdate=false;
  UpdateWorldMatrix(&mustUpdate);
}

/*-------------------------------------------------------------------
//  GRPNODE::UpdateWorldMatrix
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/07/2015 11:13:47
//
//  @param        mustUpdate :
*/
/*-----------------------------------------------------------------*/
void GRPNODE::UpdateWorldMatrix(bool * mustUpdate)
{
  if (!parent)
    return;

      parent->UpdateWorldMatrix(mustUpdate);

  if (worldmatrix_is_pending)
    *mustUpdate=true;

  if (*mustUpdate)
  {
      //this->worldmatrix.GetMultiplicationMatrix(parent->GetWorldMatrixPointer(),&modelmatrix);
      this->worldmatrix.GetMultiplicationMatrix(parent->GetWorldMatrix(),modelmatrix);
  }
}


/*-------------------------------------------------------------------
//  GRPNODE::GetWorldPosition
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 9:58:45
//
//  @return       GRPVECTOR :
//
*/
/*-----------------------------------------------------------------*/
GRPVECTOR GRPNODE::GetWorldPosition()
{
  return GetWorldPosition(GRPMATRIX::Zero);
}


/*-------------------------------------------------------------------
//  GRPNODE::GetWorldMatrix
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/07/2015 11:13:55
//
//  @return       GRPMATRIX& :
//
*/
/*-----------------------------------------------------------------*/
GRPMATRIX& GRPNODE::GetWorldMatrix()
{
  if (!parent)
    return modelmatrix;
  return worldmatrix;
}

/*-------------------------------------------------------------------
//  GRPNODE::GetWorldMatrixPointer
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/07/2015 11:14:02
//
//  @return       GRPMATRIX* :
//
*/
/*-----------------------------------------------------------------*/
GRPMATRIX* GRPNODE::GetWorldMatrixPointer()
{
  if (!parent)
    return &modelmatrix;
  return &worldmatrix;
}

/*-------------------------------------------------------------------
//  GRPNODE::GetWorldPosition
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/07/2015 11:14:18
//
//  @return       GRPVECTOR :
//
//  @param        v :
*/
/*-----------------------------------------------------------------*/
GRPVECTOR GRPNODE::GetWorldPosition(const GRPVECTOR& v)
{
  if (!parent)
  {
    return modelmatrix*v;
  }
  else
  {
    return worldmatrix*v;
  }
}

/*-------------------------------------------------------------------
//  GRPNODE::GetWorldPosition
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/07/2015 11:14:28
//
//  @return       GRPVECTOR :
//
//  @param        v :
*/
/*-----------------------------------------------------------------*/
GRPVECTOR GRPNODE::GetWorldPosition(GRPVECTOR* v)
{
  if (!parent)
  {
    return modelmatrix.TransformPoint(*v);
  }
  else
  {
    return worldmatrix.TransformPoint(*v);
  }
}

GRPMATRIX* GRPNODE::GetGeometricMatrix      ()
{
  return &geometricmatrix;
}

/*-------------------------------------------------------------------
//  GRPNODE::Push
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/08/2014 11:32:32
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPNODE::Push()
{
  GRPMATRIX  matrix     = (*GetModelMatrix());

  GRPMATRIX* matrixcopy = new GRPMATRIX();
  if(!matrixcopy) return false;

  (*matrixcopy) = matrix;

  stack.Push(matrixcopy);

  position  = GRPVECTOR(0.0f,0.0f,0.0f);
  rotation  = GRPVECTOR(0.0f,0.0f,0.0f);
  scale     = GRPVECTOR(1.0f,1.0f,1.0f);

  modelmatrix.LoadIdentity();

  return true;
}


/*-------------------------------------------------------------------
//  GRPNODE::Pop
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/08/2014 11:33:28
//
//  @return       GRPMATRIX :
//
*/
/*-----------------------------------------------------------------*/
GRPMATRIX GRPNODE::Pop()
{
  GRPMATRIX* matrix = stack.Pop();
  GRPMATRIX  copy;

  copy = (*matrix);

  delete matrix;
  return copy;
}

/*-------------------------------------------------------------------
//  GRPNODE::GetStack
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 9:59:44
//
//  @return       GRPMATRIXSTACK* :
//
*/
/*-----------------------------------------------------------------*/
GRPMATRIXSTACK* GRPNODE::GetStack()
{
  return &stack;
}

/*-------------------------------------------------------------------
//  GRPNODE::LinkTo
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/08/2014 11:35:24
//
//  @return       bool :
//
//  @param        node :
*/
/*-----------------------------------------------------------------*/
bool GRPNODE::LinkTo(GRPNODE* node)
{
  if(node!=NULL)
    {
      if(node->CanBeLinkedTo())
        {
          //parent=node;
          this->SetParent(node);
          parent->modified=true;
          parent->Attachees++;
          node->modified=true;
          return node->OnLinkedTo(this);
        }
    }
  return false;
}

/*-------------------------------------------------------------------
//  GRPNODE::CanBeLinkedTo
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 10:00:52
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool GRPNODE::CanBeLinkedTo()
{
  return true;
}

/*-------------------------------------------------------------------
//  GRPNODE::OnLinkedTo
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 10:00:56
//
//  @return       bool :
//
//  @param        node :
*/
/*-----------------------------------------------------------------*/
bool GRPNODE::OnLinkedTo(GRPNODE* node)
{
  if (node==this)
  {
    XDEBUG_PRINTCOLOR(4,__L("Error: recursive linking detected on node: %x %s"),node,(node->GetName()!=NULL) ? node->GetName()->Get() : __L(" unknown "));
    return false;
  }
  return true;
}

/*-------------------------------------------------------------------
//  GRPNODE::GetUserData
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 10:01:03
//
//  @return       GRPUSERDATACONTAINER* :
//
*/
/*-----------------------------------------------------------------*/
GRPUSERDATACONTAINER*  GRPNODE::GetUserData ()
{
  return &this->data;
}


/*-------------------------------------------------------------------
//  GRPNODE::Update
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      24/08/2014 11:36:24
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
void GRPNODE::Init()
{

  geometricmatrix.LoadIdentity();

  switch(NodeMode)
  {
  case GRPNODE_MODE_OGL:                        break;
  case GRPNODE_MODE_MAX:        InitMax();      break;
  case GRPNODE_MODE_MAYA:       InitMaya();     break;
  }
  return;
}

/*-------------------------------------------------------------------
//  GRPNODE::InitMax
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/07/2015 11:14:48
//
*/
/*-----------------------------------------------------------------*/
void GRPNODE::InitMax()
{
  geometricmatrix.LoadIdentity();

  UsesGeometric=false;

  if (this->UsesGeometricTranslation)
  {
    UsesGeometric=true;
    //geometrictranslationmatrix.BuildTranslationMatrix(geometricposition);
    //geometricmatrix= geometricmatrix*geometrictranslationmatrix;
    geometricmatrix.BuildTranslationMatrix(geometricposition);
  }
  if (this->UsesGeometricRotation)
  {
    UsesGeometric=true;
    geometricrotationmatrix.BuildRotationMatrix(geometricrotation.vector,GRPMATRIX_ROTATION_ORDER_ZYX);
    geometricmatrix= geometricmatrix*geometricrotationmatrix;
  }
  if (this->UsesGeometricScale)
  {
    UsesGeometric=true;
    geometricscalematrix.BuildScaleMatrix(geometricscale.vector);
    geometricmatrix= geometricmatrix*geometricscalematrix;
  }
  if (this->UsesPreRotation)
  {
      prerotationmatrix.BuildRotationMatrix       (this->prerotation.vector,GRPMATRIX_ROTATION_ORDER_ZYX);
  }
  if (this->UsesPostRotation)
  {
      postrotationmatrix.BuildRotationMatrix      (this->postrotation.vector,GRPMATRIX_ROTATION_ORDER_ZYX);
  }
}

/*-------------------------------------------------------------------
//  GRPNODE::InitMaya
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/07/2015 11:14:55
//
*/
/*-----------------------------------------------------------------*/
void GRPNODE::InitMaya()
{

    Roff.BuildPivotMatrix     (this->RotationOffset.vector[0],  this->RotationOffset.vector [1],  this->RotationOffset.vector [2]);
    Rp.BuildPivotMatrix       (this->RotationPivot.vector [0],  this->RotationPivot.vector  [1],  this->RotationPivot.vector  [2]);

        Rpre.BuildRotationMatrix      (this->prerotation.vector,GRPMATRIX_ROTATION_ORDER_ZYX);
        Rpost.BuildRotationMatrix     (this->postrotation.vector,GRPMATRIX_ROTATION_ORDER_ZYX);

    Rpi=Rp;
    Rpi.Inverse();

    Soff.BuildPivotMatrix     (this->ScalingOffset.vector [0],  this->ScalingOffset.vector  [1],  this->ScalingOffset.vector  [2]);
    Sp.BuildPivotMatrix       (this->ScalingPivot.vector  [0],  this->ScalingPivot.vector   [1],  this->ScalingPivot.vector   [2]);

    Spi=Sp;
    Spi.Inverse();

      RoffRp=Roff*Rp;
      RoffRpRpre=RoffRp*Rpre;

      RpostRpiSoffSp=Rpost;
      RpostRpiSoffSp=RpostRpiSoffSp*Rpi;
      RpostRpiSoffSp=RpostRpiSoffSp*Soff;
      RpostRpiSoffSp=RpostRpiSoffSp*Sp;

      RpiSoffSp=Rpi;
      RpiSoffSp=RpiSoffSp*Soff;
      RpiSoffSp=RpiSoffSp*Sp;
}


/*-------------------------------------------------------------------
//  GRPNODE::GenUpdate
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/07/2015 11:15:01
//
*/
/*-----------------------------------------------------------------*/
void GRPNODE::GenUpdate()
{
  // 1.0 Create Scale matrix
  if (scaleChanged)
  {
  scalematrix.BuildScaleMatrix(scale.vector[0], scale.vector[1], scale.vector[2]);
  scaleChanged=false;
  }


  if (rotationChanged)
  {
  // 1.1 Create current Rotation Translation Matrix

    if (this->rotation.vector[0]!=this->previousrotation.vector[0])
    {
      Rx.BuildRotationMatrixX   (this->rotation.vector[0]);
      this->previousrotation.vector[0]=this->rotation.vector[0];
    }

    if (this->rotation.vector[1]!=this->previousrotation.vector[1])
    {
      Ry.BuildRotationMatrixY   (this->rotation.vector[1]);
      this->previousrotation.vector[1]=this->rotation.vector[1];
    }

    if (this->rotation.vector[2]!=this->previousrotation.vector[2])
    {
      Rz.BuildRotationMatrixZ   (this->rotation.vector[2]);
      this->previousrotation.vector[2]=this->rotation.vector[2];
    }

    if (UsesPreRotation)
    {
    Rpre.GetMultiplicationMatrix              (&prerotationmatrix,&Rz);
    Rt.GetMultiplicationMatrix                (&Rpre,&Ry);
    rotationmatrix.GetMultiplicationMatrix    (&Rt,&Rx);
    }
    else
    {
      Rt.GetMultiplicationMatrix                (&Rz,&Ry);
      rotationmatrix.GetMultiplicationMatrix    (&Rt,&Rx);
    }

        if (UsesPostRotation)
        {
        Rpost.GetMultiplicationMatrix(&rotationmatrix,&postrotationmatrix);
        rotationmatrix=Rpost;
        }

    rotationChanged=false;
  }

  if (positionChanged)
  {
  translationmatrix.BuildTranslationMatrix(position);
  positionChanged=false;
  }

  //1.2. Create current model matrix (from stored matrix with rotation/translation)
  m.GetMultiplicationMatrix           (translationmatrix, rotationmatrix);
  modelmatrix.GetMultiplicationMatrix (m,                 scalematrix);
}

/*-------------------------------------------------------------------
//  GRPNODE::MaxUpdate
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/07/2015 11:15:07
//
*/
/*-----------------------------------------------------------------*/
void GRPNODE::MaxUpdate()
{

//WorldTransform = ParentWorldTransform * T * R * S * OT * OR * OS
//
//Where this term:  Is a 4 x 4 matrix that contains:
//WorldTransform  Transformation matrix of the node
//ParentWorldTransform  Transformation matrix of the parent node
//T Translation
//R Rotation
//S Scaling
//OT  Geometric transform translation
//OR  Geometric transform rotation
//OS  Geometric transform translation

  // 1.0 Create Scale matrix
  if (scaleChanged)
  scalematrix.BuildScaleMatrix(scale.vector[0], scale.vector[1], scale.vector[2]);


  // 1.1 Create current Rotation Translation Matrix
  if (rotationChanged)
  {
    Rx.BuildRotationMatrixX   (this->rotation.vector[0]);
    Ry.BuildRotationMatrixY   (this->rotation.vector[1]);
    Rz.BuildRotationMatrixZ   (this->rotation.vector[2]);


    if (UsesPreRotation)
    {
    Rpre.GetMultiplicationMatrix(&prerotationmatrix,&Rz);
    Rt.GetMultiplicationMatrix  (&Rpre,&Ry);
    rotationmatrix.GetMultiplicationMatrix    (&Rt,&Rx);
    }
    else
    {
      Rt.GetMultiplicationMatrix  (&Rz,&Ry);
      rotationmatrix.GetMultiplicationMatrix    (&Rt,&Rx);
    }

    if (UsesPostRotation)
        {
        Rpost.GetMultiplicationMatrix(&rotationmatrix,&postrotationmatrix);
        rotationmatrix=Rpost;
        }
  }

  if (positionChanged)
  translationmatrix.BuildTranslationMatrix(position);


  //1.2. Create current model matrix (from stored matrix with rotation/translation)

  if (positionChanged || rotationChanged)
  m.GetMultiplicationMatrix(translationmatrix,rotationmatrix);

  modelmatrix.GetMultiplicationMatrix(m,scalematrix);

  scaleChanged    =false;
  positionChanged =false;
  rotationChanged =false;

}


/*-------------------------------------------------------------------
//  GRPNODE::MayaUpdate
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      14/07/2015 11:15:13
//
*/
/*-----------------------------------------------------------------*/
void GRPNODE::MayaUpdate()
{

//  A transformation matrix is composed of the following components:
//
//Scale pivot point point around which scales are performed [Sp]
//Scale scaling about x, y, z axes [S]
//Shear shearing in xy, xz, yx [Sh]
//Scale pivot translation translation introduced to preserve existing scale transformations when moving pivot. This is used to prevent the object from moving when the objects pivot point is not at the origin and a non-unit scale is applied to the object [St].
//Rotate pivot point point about which rotations are performed [Rp]
//Rotation orientation rotation to orient local rotation space [Ro]
//Rotation rotation [R]
//Rotate pivot translation translation introduced to preserve exisitng rotate transformations when moving pivot. This is used to prevent the object from moving when the objects pivot point is not at the origin and the pivot is moved. [Rt]
//Translate translation in x, y, z axes [T]
//Note that the default RotationOrder is kXYZ.
//The matrices are post-multiplied in Maya. For example, to transform a point P from object-space to world-space (P') you would need to post-multiply by the worldMatrix. (P' = P x WM)
//
//The transformation matrix is then constructed as follows:
//
//     -1                       -1
//  [Sp]x[S]x[Sh]x[Sp]x[St]x[Rp]x[Ro]x[R]x[Rp]x[Rt]x[T]
//where 'x' denotes matrix multiplication and '-1' denotes matrix inversion
   //  Sp = |  1    0    0    0 |     St = |  1    0    0    0 |
  //        |  0    1    0    0 |          |  0    1    0    0 |
  //        |  0    0    1    0 |          |  0    0    1    0 |
  //        | spx  spy  spz   1 |          | sptx spty sptz  1 |

  //   S  = |  sx   0    0    0 |     Sh = |  1    0    0    0 |
  //        |  0    sy   0    0 |          | shxy  1    0    0 |
  //        |  0    0    sz   0 |          | shxz shyz  1    0 |
  //        |  0    0    0    1 |          |  0    0    0    1 |

  //   Rp = |  1    0    0    0 |     Rt = |  1    0    0    0 |
  //        |  0    1    0    0 |          |  0    1    0    0 |
  //        |  0    0    1    0 |          |  0    0    1    0 |
  //        | rpx  rpy  rpz   1 |          | rptx rpty rptz  1 |

  //   Ro = AX * AY * AZ

  //   AX = |  1    0    0    0 |     AY = |  cy   0   -sy   0 |
  //        |  0    cx   sx   0 |          |  0    1    0    0 |
  //        |  0   -sx   cx   0 |          |  sy   0    cy   0 |
  //        |  0    0    0    1 |          |  0    0    0    1 |

  //   AZ = |  cz   sz   0    0 |     sx = sin(rax), cx = cos(rax)
  //        | -sz   cz   0    0 |     sy = sin(ray), cx = cos(ray)
  //        |  0    0    1    0 |     sz = sin(raz), cz = cos(raz)
  //        |  0    0    0    1 |

  //       R  = RX * RY * RZ  (Note: order is determined by rotateOrder)

  //   RX = |  1    0    0    0 |     RY = |  cy   0   -sy   0 |
  //        |  0    cx   sx   0 |          |  0    1    0    0 |
  //        |  0   -sx   cx   0 |          |  sy   0    cy   0 |
  //        |  0    0    0    1 |          |  0    0    0    1 |

  //   RZ = |  cz   sz   0    0 |     sx = sin(rx), cx = cos(rx)
  //        | -sz   cz   0    0 |     sy = sin(ry), cx = cos(ry)
  //        |  0    0    1    0 |     sz = sin(rz), cz = cos(rz)
  //        |  0    0    0    1 |

  //   T  = |  1    0    0    0 |
  //        |  0    1    0    0 |
  //        |  0    0    1    0 |
  //        |  tx   ty   tz   1 |

  //     -1                       -1
  //  [Sp]x[S]x[Sh]x[Sp]x[St]x[Rp]x[Ro]x[R]x[Rp]x[Rt]x[T]

  // 17 multiplicaciones de matrices y 2 inversiones! eso son
  // (16^16)*17 multiplicaciones + ((16^16)^3 + 16*3) sumas + 3 Divisiones

  //en Max son (16^16)*4 multiplicaciones, sin comentarios

/*
WorldTransform = ParentWorldTransform * T * Roff * Rp * Rpre * R * Rpost * Rp-1
                                     * Soff * Sp * S * Sp-1
Where this term:  Is a 4 x 4 matrix that contains:
WorldTransform  Transformation matrix of the node
ParentWorldTransform  Transformation matrix of the parent node
T Translation
Roff  Rotation offset
Rp  Rotation pivot
Rpre  Pre-rotation
R Rotation
Rpost Post-rotation
Rp-1
Inverse of the rotation pivot
Soff  Scaling offset
Sp  Scaling pivot
S Scaling
Sp-1
Inverse of the scaling pivot

*/

    T.BuildTranslationMatrix  (this->position.vector[0],  this->position.vector[1], this->position.vector[2]);

    //R.BuildRotationMatrix     (this->rotation.vector, GRPMATRIX_ROTATION_ORDER_XYZ);

    Rx.BuildRotationMatrixX   (this->rotation.vector[0]);
    Ry.BuildRotationMatrixY   (this->rotation.vector[1]);
    Rz.BuildRotationMatrixZ   (this->rotation.vector[2]);

    Rt.GetMultiplicationMatrix  (&Rz,&Ry);
    R.GetMultiplicationMatrix   (&Rt,&Rx);

    //R.BuildRotationMatrix     (this->rotation.vector, GRPMATRIX_ROTATION_ORDER_ZYX);
    //R.fastBuildRotationMatrix   (this->rotation.vector,GRPMATRIX_ROTATION_ORDER_XYZ);

    S.BuildScaleMatrix        (this->scale.vector);

/*
      modelmatrix=T*RoffRpRpre;
      modelmatrix=modelmatrix*R;
      modelmatrix=modelmatrix*RpostRpiSoffSp;
      modelmatrix=modelmatrix*S;
      modelmatrix=modelmatrix*Spi;
  */

    if (UsesPreRotation)
      t.GetMultiplicationMatrix(&T,&RoffRpRpre);
    else
      t.GetMultiplicationMatrix(&T,&RoffRp);

    if (UsesPostRotation)
      r.GetMultiplicationMatrix(&R,&RpostRpiSoffSp);
    else
      r.GetMultiplicationMatrix(&R,&RpiSoffSp);

    rotationmatrix=r;

    s.GetMultiplicationMatrix(&S,&Spi);

    m.GetMultiplicationMatrix(&t,&r);
    modelmatrix.GetMultiplicationMatrix(&m,&s);
    //*/
}

void  GRPNODE::PreUpdate()
{

}
/*-------------------------------------------------------------------
//  GRPNODE::SetPosition
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 10:03:00
//
//  @param        x :
//  @param        y :
//  @param        z :
*/
/*-----------------------------------------------------------------*/
void GRPNODE::SetPosition (GLFLOAT x, GLFLOAT y, GLFLOAT z)
{
  positionChanged=true;
  this->position.vector[0]=x;
  this->position.vector[1]=y;
  this->position.vector[2]=z;
  modified=true;
}

/*-------------------------------------------------------------------
//  GRPNODE::SetScale
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 10:03:05
//
//  @param        x :
*/
/*-----------------------------------------------------------------*/
void GRPNODE::SetScale  (GLFLOAT x)
{
  SetScale(x,x,x);
}

/*-------------------------------------------------------------------
//  GRPNODE::SetScale
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 10:03:10
//
//  @param        x :
//  @param        y :
//  @param        z :
*/
/*-----------------------------------------------------------------*/
void GRPNODE::SetScale  (GLFLOAT x, GLFLOAT y, GLFLOAT z)
{
  scaleChanged=true;
  this->scale.vector[0]=x;
  this->scale.vector[1]=y;
  this->scale.vector[2]=z;
  modified=true;
}

/*-------------------------------------------------------------------
//  GRPNODE::SetRotation
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      06/11/2015 10:03:15
//
//  @param        x :
//  @param        y :
//  @param        z :
*/
/*-----------------------------------------------------------------*/
void GRPNODE::SetRotation(GLFLOAT x, GLFLOAT y, GLFLOAT z)
{
  rotationChanged=true;
  this->rotation.vector[0]=x;
  this->rotation.vector[1]=y;
  this->rotation.vector[2]=z;
  modified=true;
}

void  GRPNODE::SetRotationQ(GLFLOAT x, GLFLOAT y, GLFLOAT z,GLFLOAT w)
{

}

void  GRPNODE::SetTexturePosition(GLFLOAT x, GLFLOAT y, GLFLOAT z)
{
    textureposition.vector[0] = x;
    textureposition.vector[1] = y;
    textureposition.vector[2] = z;
}

void  GRPNODE::SetTextureScale(GLFLOAT x)
{
    texturescale.vector[0] = x;
    texturescale.vector[1] = x;
    texturescale.vector[2] = x;
}

void  GRPNODE::SetTextureScale(GLFLOAT x, GLFLOAT y, GLFLOAT z)
{
    texturescale.vector[0] = x;
    texturescale.vector[1] = y;
    texturescale.vector[2] = z;
}

void  GRPNODE::SetTextureRotation(GLFLOAT x, GLFLOAT y, GLFLOAT z)
{
    texturerotation.vector[0] = x;
    texturerotation.vector[1] = y;
    texturerotation.vector[2] = z;
}


void GRPNODE::SetGeometricPosition(GLFLOAT x, GLFLOAT y, GLFLOAT z)
{
  //positionChanged=true;
  this->geometricposition.vector[0]=x;
  this->geometricposition.vector[1]=y;
  this->geometricposition.vector[2]=z;
  //modified=true;
}

void GRPNODE::SetGeometricScale(GLFLOAT x)
{
  SetGeometricScale(x, x, x);
}

void GRPNODE::SetGeometricScale(GLFLOAT x, GLFLOAT y, GLFLOAT z)
{
  //positionChanged=true;
  this->geometricscale.vector[0]=x;
  this->geometricscale.vector[1]=y;
  this->geometricscale.vector[2]=z;
  //modified=true;
}

void GRPNODE::SetGeometricRotation(GLFLOAT x, GLFLOAT y, GLFLOAT z)
{
  //positionChanged=true;
  this->geometricrotation.vector[0]=x;
  this->geometricrotation.vector[1]=y;
  this->geometricrotation.vector[2]=z;
  //modified=true;
}
