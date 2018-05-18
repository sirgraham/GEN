/*------------------------------------------------------------------------------------------
//  GRPFILE3DFBX.H
*/
/**
// \class
//
//  Loads FilmBox Mesh File
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 12/01/2015 16:44:44
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPFILE3DFBX_H_
#define _GRPFILE3DFBX_H_

/*
                                               +-> settings       +-> Faces
                                               |                  |
                                               +-> mesh  +--------v-> Vertex
Scene  +----> Settings                         |
       |                                       +-> deformers * +----> Bones
       |                                       |               |
       +----> Objects *   +------>  elements * +               +----> Skinning
       |                  |                                    |
       |                  +------>  locators *                 +----> Deformers
       |                  |                                    |
       |                  +------>  cameras * +---+            +----> Weights
       |                  |                       +--> target
       |                  +------>  lights *  +---+
       |                  |
       |                  +------>  cameras *
       |                  |
       |                  +------>  data
       |                  |
       |                  +------>  logic  +----->  States
       |                                   |
       |                                   +----->  Controllers
       |                                   |
       +----->  Materials                  +----->  Commands
       |                                   |
       |                                   +------> Transitions
       +----->  Shaders                    |
       |                                   +------> Blends
       |
       +----->  Physics
       |
       +----->  Sounds
       |
       +----->  Media
*/

/*---- INCLUDES --------------------------------------------------------------------------*/


#include "GRPAnimation.h"
#include "GRPOpenGL.h"
#include "GRPPoint.h"
#include "GRPVector.h"
#include "GRPMatrix.h"
#include "GRPFile3D.h"
#include "GRPColor.h"
#include "GRPMesh.h"

#include "XMap.h"
#include "XVector.h"
#include "XFileTXT.h"
#include "XDebug.h"

#include "XFString.h"

#define   FBX_STATUS  0
#define   FBX_PROCESS 1
#define   FBX_WARNING 2
#define   FBX_NOTIFY  3
#define   FBX_ERROR   4

#ifdef XDEBUG
#define   FBX_DEBUG XDEBUG_PRINTCOLOR
#else
#define   FBX_DEBUG {}
#endif

#define USE_XFSTRING

#define   FBXFILETXT  FFILETXT
#define   FBXMAP      XMAP
#ifdef USE_XFSTRING
#define   FBXSTRING   XFSTRING
#else
#define   FBXSTRING   XSTRING
#endif
#define   FBXVECTOR   XVECTOR

//#define   MAX_FBX_NODES 18000
//#define   MAX_FBX_DATAS 18000
#define   MAX_FBX_DATAS 12000
#define   MAX_FBX_NODES 12000
//#define   MAX_FBX_NODES 9000

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


class GRPNODE;
class GRPMATERIAL;
class GRPTEXTURE;
class GRPCAMERA;
class GRPLIGHT;
class GRPDEFORMER;
class GRPSUBDEFORMER;
class GRPSUBDEFORMERSHAPE;
class GRPANIMATIONSTACK;
class GRPANIMATIONLAYER;
class GRPANIMATIONCURVE;
class GRPANIMATIONNODE;
class GRPSKINCLUSTER;
class GRPSKINPOSE;
class GRPSKINLIMB;
class GRPSKIN;


enum GRPFILE3DFBX_STEP
{
  GRPFILE3DFBX_STEP_LOADINGFILE       =0,
  GRPFILE3DFBX_STEP_PARSING           ,
  GRPFILE3DFBX_STEP_LOADOBJECTS       ,
  GRPFILE3DFBX_STEP_LOADINGDICTIONARY
};

enum GRPFILE3DFBX_INHERIT_TYPE
{
  GRPFILE3DFBX_INHERIT_TYPE_RRSS = 0,
  GRPFILE3DFBX_INHERIT_TYPE_RSRS,
  GRPFILE3DFBX_INHERIT_TYPE_RRS
};

enum GRPFILE3DFBX_APPLICATION_TYPE
{
  GRPFILE3DFBX_APPLICATION_TYPE_UNKNOWN=0,
  GRPFILE3DFBX_APPLICATION_TYPE_MAYA,
  GRPFILE3DFBX_APPLICATION_TYPE_3DSMAX,
  GRPFILE3DFBX_APPLICATION_TYPE_MUDBOX
};

enum GRPFILE3DFBX_INDEXING_TYPE
{
  GRPFILE3DFBX_INDEXING_TYPE_UNKNOWN=0,
  GRPFILE3DFBX_INDEXING_TYPE_BYPOLYGON,
  GRPFILE3DFBX_INDEXING_TYPE_BYVERTEX,
  GRPFILE3DFBX_INDEXING_TYPE_ALLSAME
};

enum GRPFILE3DFBX_REFERENCE_TYPE
{
  GRPFILE3DFBX_REFERENCE_TYPE_UNKNOWN=0,
  GRPFILE3DFBX_REFERENCE_TYPE_DIRECT,
  GRPFILE3DFBX_REFERENCE_TYPE_INDEXTODIRECT
};

enum GRPFILE3DFBX_CONNECTION_TYPE
{
  GRPFILE3DFBX_CONNECTION_TYPE_UNKNOWN=0,
  GRPFILE3DFBX_CONNECTION_TYPE_MODEL,
  GRPFILE3DFBX_CONNECTION_TYPE_LOCATOR,
  GRPFILE3DFBX_CONNECTION_TYPE_MESH,
  GRPFILE3DFBX_CONNECTION_TYPE_CAMERA,
  GRPFILE3DFBX_CONNECTION_TYPE_LIGHT,
  GRPFILE3DFBX_CONNECTION_TYPE_NODEATTRIBUTE,
  GRPFILE3DFBX_CONNECTION_TYPE_NODEATTRIBUTE_LIGHT,
  GRPFILE3DFBX_CONNECTION_TYPE_NODEATTRIBUTE_CAMERA,
  GRPFILE3DFBX_CONNECTION_TYPE_NODEATTRIBUTE_LIMB,
  GRPFILE3DFBX_CONNECTION_TYPE_NODEATTRIBUTE_NULL,
  GRPFILE3DFBX_CONNECTION_TYPE_GEOMETRY,
  GRPFILE3DFBX_CONNECTION_TYPE_TEXTURE,
  GRPFILE3DFBX_CONNECTION_TYPE_MATERIAL,
  GRPFILE3DFBX_CONNECTION_TYPE_ANIMATION_LAYER,
  GRPFILE3DFBX_CONNECTION_TYPE_ANIMATION_STACK,
  GRPFILE3DFBX_CONNECTION_TYPE_ANIMATION_NODE,
  GRPFILE3DFBX_CONNECTION_TYPE_ANIMATION_CURVE,
  GRPFILE3DFBX_CONNECTION_TYPE_VIDEO,
  GRPFILE3DFBX_CONNECTION_TYPE_DEFORMER_MORPH,
  GRPFILE3DFBX_CONNECTION_TYPE_DEFORMER_SKIN,
  GRPFILE3DFBX_CONNECTION_TYPE_SUBDEFORMER_MORPH,
  GRPFILE3DFBX_CONNECTION_TYPE_SUBDEFORMER_CLUSTER,
  GRPFILE3DFBX_CONNECTION_TYPE_SUBDEFORMER_SHAPE,
  GRPFILE3DFBX_CONNECTION_TYPE_SKIN_LIMB,
  GRPFILE3DFBX_CONNECTION_TYPE_SKIN_POSE,
  GRPFILE3DFBX_CONNECTION_TYPE_SKIN_ROOT,
  GRPFILE3DFBX_CONNECTION_TYPE_CONSTRAINT
};


/*---- CLASS ---------------------------------------------------------------------------- */
class FFILETXT : public XFILETXT
{
public:

    bool                  ReadAllFile             ()
    {
      if(!file)           return false;
      if(!file->IsOpen()) return false;

      DeleteAllLines();

      XDWORD             sizeBOM              = 0;
      XFILETXTFORMATCHAR formatchar           = GetFormatCharFromFile(&sizeBOM);
      int                sizebytescharacter   = SizeOfCharacter(formatchar);

      if(this->formatchar==XFILETXTFORMATCHAR_UNKNOWN) this->formatchar = formatchar;

      file->SetPosition(sizeBOM);

      unsigned long totalsize=file->GetSize();

      bool    endfile;
      XDWORD  br;
      XBUFFER dataline(false);

      XBYTE*  readbuffer = new XBYTE[totalsize];
      if(!readbuffer) return false;

      memset(readbuffer, 0, totalsize);

          unsigned long  bufferpos = 0;

          br            = totalsize;
          endfile = !file->Read(readbuffer, &br);
          if(!br) return false;;

          XFILETXTTYPELF  _typeLF   = XFILETXTTYPELF_UNKNOWN;
          int             sizeLF    = 0;
          int             sizeline  = 0;

          lines.SetIsMulti(true);
          lines.SetAddInLimit(40000);

          XSTRING all;
          all.ConvertFromUTF8(readbuffer,totalsize);

          all.Explode(__C('\n'),&lines);

        delete [] readbuffer;

      return true;
    }

  bool GetSizeOfLine(XBYTE* buffer, int& size, int maxsize)
  {
    bool status = false;
    int  c      = 0;

    typeLF = XFILETXTTYPELF_UNKNOWN;

    XWORD* bufw = (XWORD*) buffer;
    XBYTE* bufb = (XBYTE*) buffer;

    for(c=0; c<maxsize; c++)
    {
      if(bufb[c]==0x0A)
        {
          status = true;
          typeLF = XFILETXTTYPELF_0A;
          break;
        }

    }

    if(status) size = c;

    return status;
  }

};
/*---- CLASS -----------------------------------------------------------------------------*/


//----------------------------------------------------- FBXData
/*
//  Holds data for a fbx block.
//
/--------------------------------------------------------------*/

class FBXData
{
public:
                      FBXData   ();
                      ~FBXData  ();

  FBXSTRING           name;
  FBXSTRING           type;
  FBXSTRING           subtype;
  FBXSTRING           label;
  FBXSTRING           value;

  XQWORD              tag;

  bool                property;

  //FBXVECTOR<double>*    GetArrayValues();
  //FBXVECTOR<long long>* GetArrayIndex();

  void                SetArrayValues(FBXVECTOR<double>*   v);
  void                SetArrayIndex (FBXVECTOR<long long>* v);

  FBXVECTOR<double>*    ArrayValues;
  FBXVECTOR<long long>* ArrayIndex;

  void                Delete();

};


//----------------------------------------------------- FBXNODE
/*
//  Holds hierarchy for a fbx block.
//
/--------------------------------------------------------------*/

class FBXNode
{
public:

          FBXNode   ();
          ~FBXNode  ();
  void    Show      (XDWORD level);

  operator XDWORD   ();
  operator GLFLOAT  ();
  operator GRPVECTOR();
  operator GRPPOINT ();
  operator GRPMATRIX();
  operator GRPCOLOR ();

  XDWORD   LastIndex;

  FBXData*          data;
  FBXVECTOR<FBXNode*>*Children;

  void Delete()
  {
  if (Children!=NULL)
    Children->DeleteAll();
  delete(Children);
  }

private:

};

//-------------------------------------------------- Comparators
class FBXNodeComparator
{
public:
  virtual int Compare(FBXNode*)=0;
};

class FBXNodeComparatorByName : public FBXNodeComparator
{
public:

  FBXSTRING name;

  virtual int Compare(FBXNode* a)
  {
    if (a->data)
      return a->data->name.Compare(name);
    return 1;
  }
};
class FBXNodeComparatorByNameAndValue : public FBXNodeComparator
{
public:

  FBXSTRING name;
  FBXSTRING value;

  virtual int Compare(FBXNode* a)
  {
    if (a->data)
        if (a->data->value.Compare(value)==0)
          if (a->data->name.Compare(name)==0)
            return 0;
    return 1;
  }
};


//----------------------------------------------------- GRPFILE3DFBX
/*
//  FBX loader class
//
/--------------------------------------------------------------*/
class GRPFILE3DFBX : public GRPFILE3D
{
public:
                                                              GRPFILE3DFBX                  () : GRPFILE3D()
                                                              {
                                                                SetupIgnoredNodes();
                                                                Clean();
                                                              };

        virtual                                               ~GRPFILE3DFBX                 ();

        virtual GRPOBJECT*                                    Load                          (XCHAR* file);
                void                                          ShowTree                      ()              { Root->Show(0);}

protected:
                void                                          SetupIgnoredNodes             ();
                bool                                          IsIgnoredNode                 (FBXSTRING& name);
                bool                                          LoadHeaderData                ();
                bool                                          LoadNameData                  ();
                bool                                          LoadConnectionsAndProperties  ();

                FBXVECTOR<FBXSTRING*>                         IgnoredNodes;

                XDWORD                                        UpAxis;
                int                                           UpAxisSign;
                XDWORD                                        FrontAxis;
                int                                           FrontAxisSign;
                XDWORD                                        RightAxis;
                int                                           RightAxisSign;
                XQWORD                                        TimeMode;
                XQWORD                                        TimeProtocol;
                int                                           CustomFrameRate;
                float                                         UnitScaleFactor;
                FBXSTRING                                     DefaultCamera;


                XPATH                                         FBXfile;

                FBXFILETXT*                                   xfiletxt;
                int                                           CurrentLine;
                int                                           nlines;

                FBXSTRING                                     line;
                FBXSTRING                                     name;
                FBXSTRING                                     value;
                FBXSTRING                                     connectionType;

                FBXNode*                                      Root;
                FBXNode*                                      Connections;

                int                                           level;

                FBXNode*                                      Objects;

                GRPFILE3DFBX_APPLICATION_TYPE                 Application;

                FBXNode                                       FBXNodes[MAX_FBX_NODES];
                XQWORD                                        FBXNodeIndex;

                XVECTOR<FBXNode*>                             ExcessNodes;

                FBXData                                       FBXDatas[MAX_FBX_NODES];
                XQWORD                                        FBXDataIndex;

                XVECTOR<FBXData*>                             ExcessDatas;

                FBXMAP<XQWORD,FBXSTRING*>                     TagMap;
                FBXMAP<XQWORD,FBXSTRING*>                     ObjectNameMap;

                FBXMAP<XQWORD,XQWORD>                         TypesMap;
                FBXMAP<XQWORD,XQWORD>                         ObjectConnectionsMap;
                FBXMAP<XQWORD,XQWORD>                         PropertyConnectionsMap;
                FBXMAP<XQWORD,FBXSTRING*>                     PropertyConnectionsTypeMap;

                //---------- Geometric
                FBXMAP<XQWORD,GRPELEMENT*>                    GeometricMap;           // element data
                FBXVECTOR<GRPMESH*>                           MeshMap;                // mesh data

                //---------- surfaces
                FBXMAP<XQWORD,GRPMATERIAL*>                   MaterialMap;            // material data
                FBXMAP<XQWORD,GRPTEXTURE*>                    TextureMap;             // texture data

                FBXMAP<XQWORD,XQWORD>                         NodeAttributeMap;       // nodeattribute data , camera, light, limb

                //--------- camera
                FBXMAP<XQWORD,GRPCAMERA*>                     CameraMap;              // camera data
                FBXMAP<XQWORD,GRPCAMERA*>                     CameraAttributeMap;     // camera attributes data

                //---------- lights
                FBXMAP<XQWORD,GRPLIGHT*>                      LightMap;               // light data
                FBXMAP<XQWORD,GRPLIGHT*>                      LightAttributeMap;      // light attributes data

                //----------- spatial
                FBXMAP<XQWORD,GRPNODE*>                       LocatorMap;             // null nodes
                FBXMAP<XQWORD,GRPNODE*>                       NodeMap;                // pure model nodes


                //------------- morph
                FBXMAP<XQWORD,GRPDEFORMER*>                   DeformerMap;            // deformers [blending, morpher]
                FBXMAP<XQWORD,GRPSUBDEFORMER*>                SubdeformerMap;         // deformer channel geometry
                FBXMAP<XQWORD,GRPSUBDEFORMERSHAPE*>           SubdeformerShapeMap;    // deformer shape geometry

                //------------- skin
                FBXMAP<XQWORD,GRPSKINLIMB*>                   SkinLimbMap;            // Skin limb nodes
                FBXMAP<XQWORD,GRPSKINCLUSTER*>                SkinClusterMap;         // vertex limb association
                FBXMAP<XQWORD,GRPSKINPOSE*>                   SkinPoseMap;            // Limb pose , optional, might not be on the fbx
                FBXMAP<XQWORD,GRPSKIN*>                       SkinMap;                // skin object
                FBXMAP<XQWORD,GRPSKINLIMB*>                   SkinLimbAttributeMap;   // limb attribute size
                FBXMAP<XQWORD,GRPNODE*>                       SkinRootMap;            // null nodes

                //------------ animation
                FBXMAP<XQWORD,GRPANIMATIONSTACK*>             AnimationStackMap;      // animation stack data
                FBXMAP<XQWORD,GRPANIMATIONLAYER*>             AnimationLayerMap;      // animation layer data
                FBXMAP<XQWORD,GRPANIMATIONCURVE*>             AnimationCurveMap;      // animation curve data
                FBXMAP<XQWORD,GRPANIMATIONNODE*>              AnimationNodeMap;       // animation node data


                GRPFILE3DFBX_CONNECTION_TYPE                  IsType                  (XQWORD id);

                bool                                          FindNode                (FBXNode* CurrentNode,FBXNodeComparator* Comparator,FBXNode** Result);
                bool                                          FindNode                (XCHAR* name,int size,FBXNode**,bool Case=false);
                bool                                          FindNode                (XCHAR* name,int size,FBXNode* CurrentNode,FBXNode**,bool Case=false);
                bool                                          FindChildNode           (XCHAR* name,int size,FBXNode* CurrentNode,FBXNode**,bool Case=false);

        virtual void                                          ConsumeBlock            ();
                bool                                          ReadNode                (FBXNode* node);
                void                                          SkipArray               ();
    FBXVECTOR<double>*                                        ReadArrayDouble         (XDWORD size);
    FBXVECTOR<long long>*                                     ReadArrayIndex          (XDWORD size);

                bool                                          LoadGlobalSettings      ();
                GRPOBJECT*                                    LoadDictionary          ();
                bool                                          LoadObjects             ();

                bool                                          LoadModelNode           (XQWORD,FBXNode*);
                bool                                          LoadModelNode           (XQWORD,FBXNode*,GRPNODE*);

                bool                                          LoadGeometryNode        (XQWORD,FBXNode*);
                bool                                          LoadMorpherShapeNode    (XQWORD,FBXNode*);

                bool                                          LoadMaterialNode        (XQWORD,FBXNode*);
                bool                                          LoadTextureNode         (XQWORD,FBXNode*);

                bool                                          LoadNodeAttribute       (XQWORD,FBXNode*);
                bool                                          LoadNodeAttributeCamera (XQWORD,FBXNode*);
                bool                                          LoadNodeAttributeLight  (XQWORD,FBXNode*);

                bool                                          LoadAnimationStackNode  (XQWORD,FBXNode*);
                bool                                          LoadAnimationLayerNode  (XQWORD,FBXNode*);
                bool                                          LoadAnimationNode       (XQWORD,FBXNode*);
                bool                                          LoadAnimationCurve      (XQWORD,FBXNode*);

                bool                                          LoadDeformer            (XQWORD,FBXNode*);
                bool                                          LoadMorphDeformer       (XQWORD,FBXNode*);
                bool                                          LoadSubdeformer         (XQWORD,FBXNode*);
                bool                                          LoadNodeAttributeLimb   (XQWORD,FBXNode*);
                bool                                          LoadLimbNode            (XQWORD,FBXNode*);
                bool                                          LoadSkinDeformer        (XQWORD,FBXNode*);
                bool                                          LoadSkinPose            (XQWORD,FBXNode*);
                bool                                          LoadSkinCluster         (XQWORD,FBXNode*);

                void                                          DeleteTree              (FBXNode*);

                bool                                          IsConnected             (XQWORD id);
                bool                                          Setup                   (GRPOBJECT* obj);
                void                                          Abort                   ();

                GRPANIMATIONCURVEAXIS                         GetPropertyTypeByName   (FBXSTRING* subtype);

                FBXNode*                                      CreateFBXNode           ();
                FBXData*                                      CreateFBXData           ();


                FBXSTRING                                     ConnectionString;
                FBXSTRING                                     ParameterString;

                GRPFILE3DFBX_STEP                             CurrentStep;

                void                                          SetObject               (GRPOBJECT* obj) { this->object=obj;}

                //-------------- task control

                void                                          UpdateTask();
                int                                           currentDictionary;
                int                                           currentObject;

private:


                FBXSTRING animationnodename;
                void                  Clean                   ()
                {
                  ConnectionString  =__L("C:");
                  ParameterString   =__L("p:");
                  CurrentLine       =0;
                  UpAxis            =1;
                  UpAxisSign        =1;
                  FrontAxis         =2;
                  FrontAxisSign     =1;
                  RightAxis         =0;
                  RightAxisSign     =1;
                  level             =0;
                  FBXNodeIndex      =0;
                  FBXDataIndex      =0;
                  xfiletxt          =NULL;
                  Root              =NULL;
                  Connections       =NULL;
                  Objects           =NULL;

                  ObjectConnectionsMap.SetIsMulti         (true);
                  PropertyConnectionsMap.SetIsMulti       (true);
                  PropertyConnectionsTypeMap.SetIsMulti   (true);
                  TypesMap.SetIsMulti                     (true);
                  TypesMap.DeleteAll                      ();
                  NodeMap.SetIsMulti                      (true);
                  GeometricMap.SetIsMulti                 (true);
                  MaterialMap.SetIsMulti                  (true);
                  TextureMap.SetIsMulti                   (true);
                  NodeAttributeMap.SetIsMulti             (true);
                  CameraMap.SetIsMulti                    (true);
                  CameraAttributeMap.SetIsMulti           (true);
                  LightMap.SetIsMulti                     (true);
                  LightAttributeMap.SetIsMulti            (true);
                  LocatorMap.SetIsMulti                   (true);
                  DeformerMap.SetIsMulti                  (true);
                  SubdeformerMap.SetIsMulti               (true);
                  SkinLimbMap.SetIsMulti                  (true);
                  SkinClusterMap.SetIsMulti               (true);
                  SkinPoseMap.SetIsMulti                  (true);
                  SkinMap.SetIsMulti                      (true);
                  SkinLimbAttributeMap.SetIsMulti         (true);
                  AnimationStackMap.SetIsMulti            (true);
                  AnimationLayerMap.SetIsMulti            (true);
                  AnimationCurveMap.SetIsMulti            (true);
                  AnimationNodeMap.SetIsMulti             (true);
                  TagMap.SetIsMulti                       (false);
                  ObjectNameMap.SetIsMulti                (false);
                  ExcessDatas.SetIsMulti                  (true);

                  nlines=0;
                  percent = 0;
                  currentask = 0;
                }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif





