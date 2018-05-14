/*------------------------------------------------------------------------------------------
//  GRPNODE.H
*/
/**
// \class
//
//  Node transformation class
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 12/06/2014 16:57:16
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPNODE_H_
#define _GRPNODE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPVector.h"
#include "GRPMatrix.h"
#include "XObserver.h"
#include "math.h"
#include "GRPUserDataContainer.h"
#include "GRPMaterial.h"
#include "GRPInterfaceAnimable.h"
#include "GRPNamed.h"
#include "GRPSubject.h"

//#include "GRPNodeManager.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum GRPNODE_MODE
{
  GRPNODE_MODE_OGL=0,
  GRPNODE_MODE_MAX,
  GRPNODE_MODE_MAYA
};

enum GRPNODE_INHERIT_TYPE
{
  GRPNODE_INHERIT_TYPE_RrSs=0,
  GRPNODE_INHERIT_TYPE_RSrs,
  GRPNODE_INHERIT_TYPE_Rrs
};

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPNODEMANAGER;
class GRPNODE;

class GRPATTACHEABLE : public GRPSUBJECT, public GRPOBSERVER
{
public:

                                    GRPATTACHEABLE            ()            { Clean();      }
    GRPNODE*                        GetParent                 ();
    void                            SetParent                 (GRPNODE* p);
    void                            Dettach                   ();

    GRPNODE*                        parent;
    bool                            IsOrphan;
    int                             Attachees;

    bool                            IsAttacheable;

private:

  void Clean()
  {
      Attachees     =0;
      IsOrphan      =true;
      parent        =NULL;
      IsAttacheable =true;
  }
};

class GRPNODE : public GRPINTERFACEANIMABLE, public GRPATTACHEABLE
{
  public:
    friend class GRPNODEMANAGER;

                                    GRPNODE                 ();
                                    ~GRPNODE                ();


    GRPMATRIX*                      GetModelMatrix          ();

    GRPMATRIX                       GetMatrix               ();
    GRPMATRIX                       GetLocalMatrix          ();
    GRPMATRIX                       GetCurrentMatrix        ();
    GRPMATRIX&                      GetWorldMatrix          ();
    GRPMATRIX*                      GetWorldMatrixPointer   ();

    void                            UpdateWorldMatrix       ();
    void                            UpdateWorldMatrix       (bool * mustUpdate); //recursive


    GRPVECTOR                       GetWorldPosition        ();
    GRPVECTOR                       GetWorldPosition        (const GRPVECTOR& v);
    GRPVECTOR                       GetWorldPosition        (GRPVECTOR* v);
    GRPMATRIX*                      GetGeometricMatrix      ();


    bool                            Push                    ();
    GRPMATRIX                       Pop                     ();
    GRPMATRIXSTACK*                 GetStack                ();

    bool                            LinkTo                  (GRPNODE* node);

    virtual bool                    CanBeLinkedTo           ();
    virtual bool                    OnLinkedTo              (GRPNODE* node);
    GRPUSERDATACONTAINER*           GetUserData             ();




    void                            Init                    ();
    void                            InitMaya                ();
    void                            InitMax                 ();

    void                            Update                  ();

    void                            GenUpdate               ();
    void                            MaxUpdate               ();
    void                            MayaUpdate              ();
    void                            PreUpdate               ();

 // ------------------------ vectors ----------------------

    inline const GRPVECTOR* GetPosition                     () { return &this->position;  }
    inline const GRPVECTOR* GetRotation                     () { return &this->rotation;  }
    inline const GRPVECTOR* GetScale                        () { return &this->scale;     }

    inline GRPVECTOR* GetPrerotation                        ()  { return &this->prerotation;}
    inline GRPVECTOR* GetPostrotation                       ()  { return &this->postrotation;}
    inline GRPVECTOR* GetGeometricPosition                  ()  { return &this->geometricposition;}
    inline GRPVECTOR* GetGeometricScale                     ()  { return &this->geometricscale;}
    inline GRPVECTOR* GetGeometricRotation                  ()  { return &this->geometricrotation;}

    inline GRPVECTOR*               GetRotationOffset       ()  { return &this->RotationOffset;   }

    virtual bool                    Notify                    (GRPMSGTYPE msg,GRPSUBJECT* notifier)
    {
      return true;
    }

    XDWORD                          GetSubjectType            ()  { return GRPNAMED_TYPE_GRPNODE;   }

    void                            SetPosition               (GLFLOAT x, GLFLOAT y, GLFLOAT z);
    void                            SetScale                  (GLFLOAT x);
    void                            SetScale                  (GLFLOAT x, GLFLOAT y, GLFLOAT z);
    void                            SetRotation               (GLFLOAT x, GLFLOAT y, GLFLOAT z);
    void                            SetRotationQ              (GLFLOAT x, GLFLOAT y, GLFLOAT z, GLFLOAT w);

    void                            SetTexturePosition        (GLFLOAT x, GLFLOAT y, GLFLOAT z);
    void                            SetTextureScale           (GLFLOAT x);
    void                            SetTextureScale           (GLFLOAT x, GLFLOAT y, GLFLOAT z);
    void                            SetTextureRotation        (GLFLOAT x, GLFLOAT y, GLFLOAT z);

    void                            SetGeometricPosition      (GLFLOAT x, GLFLOAT y, GLFLOAT z);
    void                            SetGeometricScale         (GLFLOAT x);
    void                            SetGeometricScale         (GLFLOAT x, GLFLOAT y, GLFLOAT z);
    void                            SetGeometricRotation      (GLFLOAT x, GLFLOAT y, GLFLOAT z);

    virtual bool                    SetValue                  (GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS subtype, double value,double base);

    bool                            GetIsVisible              ();
    void                            SetVisible                (bool f);


    GRPMATERIAL*                    GetMaterial               ();

    GRPMATERIALATTRIBUTES*          GetBaseMaterial           ();
    void                            SetBaseMaterial           (GRPMATERIALATTRIBUTES* newmaterial);

    void                            SetMaterial               (GRPMATERIAL* material);
    void                            Copy                      (GRPNODE*);

     // ------------------------ MEMBERS ----------------------

    GRPNODE_MODE                    NodeMode;

    double                          Value;
    bool                            managed;

    bool                            UsesGeometric;

    bool                            UsesGeometricTranslation;
    bool                            UsesGeometricScale;
    bool                            UsesGeometricRotation;

    bool                            UsesRotationOffset;
    bool                            UsesRotationPivot;
    bool                            UsesScalingOffset;
    bool                            UsesScalingPivot;
    bool                            UsesPreRotation;
    bool                            UsesPostRotation;

    bool                            UsesTextureMatrix;

    bool                            modified;
    bool                            worldmatrix_is_pending;

    double                          visibility;
    double                          animated_visibility;

    bool                            visibility_isdefined;
    double                          inherited_visibility;

    bool                            visibility_inheritance;
    bool                            rendereable;



    GRPMATERIAL*                    material;
    GRPMATERIALATTRIBUTES           BaseMaterial;

    GRPMATRIXSTACK                  stack;

    GLFLOAT                         degrees;


    GRPMATRIX                       translationmatrix;
    GRPMATRIX                       scalematrix;
    GRPMATRIX                       rotationmatrix;

    GRPMATRIX                       geometrictranslationmatrix;
    GRPMATRIX                       geometricscalematrix;
    GRPMATRIX                       geometricrotationmatrix;

    GRPMATRIX                       geometricmatrix;

    GRPMATRIX                       prerotationmatrix;
    GRPMATRIX                       postrotationmatrix;
    GRPMATRIX                       modelmatrix;              // Translation * Rotation * Scale

    GRPMATRIX                       worldmatrix;              // Translation * Rotation * Scale

    GRPVECTOR                       RotationOffset;         //  rotation pivot offset //maya
    GRPVECTOR                       RotationPivot;          //  rotation pivot position
    GRPVECTOR                       ScalingOffset;          //  Scaling offset
    GRPVECTOR                       ScalingPivot;           //  Scaling Pivot

    GRPVECTOR                       prerotation;            //  Rotation axis in pre
    GRPVECTOR                       postrotation;           //  Rotation axis in post

    int                             InheritType;

    //------------- maya matrix

    GRPMATRIX                     T;
    GRPMATRIX                     Roff;
    GRPMATRIX                     Rp;
    GRPMATRIX                     Rpre;

    GRPMATRIX                     RoffRpRpre;
    GRPMATRIX                     RoffRp;
    GRPMATRIX                     RpostRpiSoffSp;
    GRPMATRIX                     RpiSoffSp;

    GRPMATRIX                     R;
    GRPMATRIX                     Rx;
    GRPMATRIX                     Ry;
    GRPMATRIX                     Rz;
    GRPMATRIX                     Rt;

    GRPMATRIX                     Rpost;
    GRPMATRIX                     Rpi;
    GRPMATRIX                     Soff;
    GRPMATRIX                     Sp;
    GRPMATRIX                     S;
    GRPMATRIX                     Spi;



    bool                          positionChanged;
    bool                          rotationChanged;
    bool                          scaleChanged;

    XVECTOR<GRPINTERFACEANIMABLE*> animables;


    GRPVECTOR                     textureposition;        //  Absolute position
    GRPVECTOR                     texturerotation;        //  Rotation axis in degrees
    GRPVECTOR                     texturescale;           //  Scale factor per axis

protected:

    GRPUSERDATACONTAINER          data;



    GRPVECTOR                     position;               //  Absolute position
    GRPVECTOR                     rotation;               //  Rotation axis in degrees
    GRPVECTOR                     previousrotation;               //  Rotation axis in degrees
    GRPVECTOR                     scale;                  //  Scale factor per axis

    GRPVECTOR                     geometricposition;      //  geometric position
    GRPVECTOR                     geometricrotation;      //  geometric Rotation
    GRPVECTOR                     geometricscale;         //  geometric Scale factor per axis




  private:

    //temporal matrix for multiplications
    GRPMATRIX                     t;
    GRPMATRIX                     r;
    GRPMATRIX                     s;
    GRPMATRIX                     m;

    void                          Clean()
                                  {
                                    animated_visibility                     = 1.0f;
                                    visibility_isdefined                    = false;
                                    NodeMode                                = GRPNODE_MODE_OGL;
                                    parent                                  = NULL;
                                    tag                                     = 0;
                                    degrees                                 = 0.0f;
                                    visibility                              = 1.0f;

                                    scale.vector    [0]                     = scale.vector   [1]      = scale.vector    [2] = 1.0f;
                                    position.vector [0]                     = position.vector[1]      = position.vector [2] = 0.0f;
                                    rotation.vector [0]                     = rotation.vector[1]      = rotation.vector [2] = 0.0f;

                                    translationmatrix.LoadIdentity          ();
                                    modelmatrix.LoadIdentity                ();
                                    scalematrix.LoadIdentity                ();
                                    rotationmatrix.LoadIdentity             ();

                                    geometrictranslationmatrix.LoadIdentity ();
                                    geometricscalematrix.LoadIdentity       ();
                                    geometricrotationmatrix.LoadIdentity    ();

                                    prerotationmatrix.LoadIdentity          ();
                                    postrotationmatrix.LoadIdentity         ();

                                    UsesGeometric                           =false;
                                    UsesGeometricTranslation                =false;
                                    UsesGeometricScale                      =false;
                                    UsesGeometricRotation                   =false;

                                    UsesRotationOffset                      =false;
                                    UsesRotationPivot                       =false;
                                    UsesScalingOffset                       =false;
                                    UsesScalingPivot                        =false;
                                    UsesPreRotation                         =false;
                                    UsesPostRotation                        =false;

                                    UsesTextureMatrix                       =false;

                                    modified                                =true;
                                    worldmatrix_is_pending                  =true;

                                    rotationChanged                         =true;
                                    positionChanged                         =true;
                                    scaleChanged                            =true;

                                    material                                =NULL;

                                    T.LoadIdentity                          ();
                                    Roff.LoadIdentity                       ();
                                    Rp.LoadIdentity                         ();
                                    Rpre.LoadIdentity                       ();

                                    RoffRpRpre.LoadIdentity                 ();
                                    RpostRpiSoffSp.LoadIdentity             ();

                                    R.LoadIdentity                          ();
                                    Rpost.LoadIdentity                      ();
                                    Rpi.LoadIdentity                        ();
                                    Soff.LoadIdentity                       ();
                                    Sp.LoadIdentity                         ();
                                    S.LoadIdentity                          ();
                                    Spi.LoadIdentity                        ();

                                    Rx.LoadIdentity                         ();
                                    Ry.LoadIdentity                         ();
                                    Rz.LoadIdentity                         ();

                                    inherited_visibility                    =1.0f;
                                    rendereable                             =true;
                                    visibility_inheritance                  =true;
                                    InheritType                             =GRPNODE_INHERIT_TYPE_RSrs;
                                    IsOrphan                                =false;

                                    texturescale.vector[0]    = texturescale.vector[1]    = texturescale.vector[2] = 1.0f;
                                    textureposition.vector[0] = textureposition.vector[1] = textureposition.vector[2] = 0.0f;
                                    texturerotation.vector[0] = texturerotation.vector[1] = texturerotation.vector[2] = 0.0f;
                                  }


};



/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif


