/*------------------------------------------------------------------------------------------
//  FBX.H
*/
/**
// \class
//
//  reads fbx contents
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 17/02/2016 10:14:11
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _FBX_H_
#define _FBX_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"
#include "XString.h"
#include "XMap.h"
#include "XVector.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPSCENE;
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
class GRPOBJECT;

namespace FBX
{

enum INHERIT_TYPE
{
  INHERIT_TYPE_RRSS = 0,
  INHERIT_TYPE_RSRS,
  INHERIT_TYPE_RRS
};

enum APPLICATION_TYPE
{
  APPLICATION_TYPE_UNKNOWN=0,
  APPLICATION_TYPE_MAYA,
  APPLICATION_TYPE_3DSMAX,
  APPLICATION_TYPE_MUDBOX
};

enum INDEXING_TYPE
{
  INDEXING_TYPE_UNKNOWN=0,
  INDEXING_TYPE_BYPOLYGON,
  INDEXING_TYPE_BYVERTEX,
  INDEXING_TYPE_ALLSAME
};

enum REFERENCE_TYPE
{
  REFERENCE_TYPE_UNKNOWN=0,
  REFERENCE_TYPE_DIRECT,
  REFERENCE_TYPE_INDEXTODIRECT
};

enum CONNECTION_TYPE
{
  CONNECTION_TYPE_UNKNOWN=0,
  CONNECTION_TYPE_MODEL,
  CONNECTION_TYPE_LOCATOR,
  CONNECTION_TYPE_MESH,
  CONNECTION_TYPE_CAMERA,
  CONNECTION_TYPE_LIGHT,
  CONNECTION_TYPE_NODEATTRIBUTE,
  CONNECTION_TYPE_NODEATTRIBUTE_LIGHT,
  CONNECTION_TYPE_NODEATTRIBUTE_CAMERA,
  CONNECTION_TYPE_NODEATTRIBUTE_LIMB,
  CONNECTION_TYPE_NODEATTRIBUTE_NULL,
  CONNECTION_TYPE_GEOMETRY,
  CONNECTION_TYPE_TEXTURE,
  CONNECTION_TYPE_MATERIAL,
  CONNECTION_TYPE_ANIMATION_LAYER,
  CONNECTION_TYPE_ANIMATION_STACK,
  CONNECTION_TYPE_ANIMATION_NODE,
  CONNECTION_TYPE_ANIMATION_CURVE,
  CONNECTION_TYPE_VIDEO,
  CONNECTION_TYPE_DEFORMER_MORPH,
  CONNECTION_TYPE_DEFORMER_SKIN,
  CONNECTION_TYPE_SUBDEFORMER_MORPH,
  CONNECTION_TYPE_SUBDEFORMER_CLUSTER,
  CONNECTION_TYPE_SUBDEFORMER_SHAPE,
  CONNECTION_TYPE_SKIN_LIMB,
  CONNECTION_TYPE_SKIN_POSE,
  CONNECTION_TYPE_CONSTRAINT
};


#define FBXDEBUG  XDEBUG_PRINTCOLOR
#define FBXMAP    XMAP
#define FBXVECTOR XVECTOR

  /*---- CLASS -----------------------------------------------------------------------------*/
  class FILE
  {
  public:

                          FILE      ();
    virtual               ~FILE     ();

    virtual   GRPOBJECT*  Load      (XCHAR* file);

  protected:

    virtual   void        ParseDataBlocks ();
    virtual   bool        ReadLine        ();
    virtual   bool        ReadWord        (XSTRING&);
    virtual   bool        SkipBlock       ();
    virtual   XDWORD      SkipLine        ();

    XDWORD    cursor;
    XDWORD    filesize;

    XSTRING   line;

    XSTRING   Header;
    XSTRING   Definitions;
    XSTRING   Objects;
    XSTRING   Connections;
    XSTRING   Takes;

    XSTRING   data;

  private:

              void        Clean     ();
  };

}

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

