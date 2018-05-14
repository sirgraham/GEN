/*------------------------------------------------------------------------------------------
//  GRPSKINNING.H
*/
/**
// \class
//
//  Skinning control
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 19/05/2015 14:44:27
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPSKINNING_H_
#define _GRPSKINNING_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"
#include "GRPDeformer.h"
#include "GRPNode.h"


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

#define GRPSKINBONEMAXINFLUENCES 4

class GRPELEMENT;
class GRPSKINLIMB;
class GRPSKINCLUSTER;
class GRPSKINPOSE;
class GRPSKIN;
class GRPSHADER;


/*---- CLASS  ----------------------------------------------------------------------------*/

class GRPSKINLIMB : public GRPNODE
{
public:
                            GRPSKINLIMB ()              { SetSize(1.0f);                    }
  virtual GRPNAMED_TYPE     GetType     ()              { return GRPNAMED_TYPE_GRPSKINLIMB; }
                           ~GRPSKINLIMB ()              {                                   }
  void                      SetSize     (GLFLOAT size)  {         this->Size=size;          }
  GLFLOAT                   SetSize     ()              { return  this->Size;               }

  XDWORD                    id;
  GLFLOAT                   Size;
};

class GRPSKINCLUSTER : public virtual GRPNAMED
{
public:
                            GRPSKINCLUSTER  ()                  { GRPNAMED::classtype = GRPNAMED_TYPE_GRPSKINCLUSTER;   }
                           ~GRPSKINCLUSTER  ()
                            {
                              this->Limbs.DeleteAll();
                              Indexes.DeleteAll();
                              Weights.DeleteAll();
                            }
  virtual GRPNAMED_TYPE     GetType         ()                  { return GRPNAMED_TYPE_GRPSKINCLUSTER;                  }
  bool                      AddLimb         (GRPSKINLIMB* limb) { return Limbs.Add(limb);                               }
  XVECTOR<GRPSKINLIMB*>*    GetLimbs        ()                  { return &Limbs;                                        }

  XVECTOR<GRPSKINLIMB*>     Limbs;
  XVECTOR<XDWORD>           Indexes;
  XVECTOR<double>           Weights;

  GRPMATRIX                 Transform;
  GRPMATRIX                 TransformLink;

};


class GRPSKINPOSE : public virtual GRPNAMED
{
public:
                            GRPSKINPOSE     ()  {   GRPNAMED::classtype = GRPNAMED_TYPE_GRPSKINPOSE;  Node=NULL; }
  virtual GRPNAMED_TYPE     GetType         ()  {   return GRPNAMED_TYPE_GRPSKINPOSE;                 }

  XDWORD        NodeId;
  GRPMATRIX     Matrix;
  GRPNODE*      Node;
  XDWORD        skinlimbtag;
};


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPSKIN : public GRPDEFORMER
{
public:

                            GRPSKIN     ()                        {   GRPNAMED::classtype = GRPNAMED_TYPE_GRPSKIN;  }
  virtual                   ~GRPSKIN    ();
  virtual GRPNAMED_TYPE     GetType     ()                        {   return GRPNAMED_TYPE_GRPSKINPOSE;             }

  bool                      AddPose     (GRPSKINPOSE* pose)       { return PoseNodes.Add(pose);                     }
  bool                      AddCluster  (GRPSKINCLUSTER* cluster) { return Clusters.Add(cluster);                   }
  XVECTOR<GRPSKINCLUSTER*>* GetClusters ()                        { return &Clusters;}
  bool                      SetPose     (GRPSKINPOSE* pose)       { return this->PoseNodes.Add(pose);}
  XVECTOR<GRPSKINPOSE*>*    GetPoses    ()                        { return &this->PoseNodes;}

  bool                      Init        ();
  bool                      Update      ();

  GRPMATRIX Root;

  GRPELEMENT*               target;
  GLFLOAT *                 BoneMatrices;
  GLFLOAT *                 PoseMatrices;
  GRPSHADER*                shader;

  GLFLOAT*                  BoneWeightsArray;
  GLUINT*                   BoneIndexArray;

protected:

  XVECTOR<GRPSKINCLUSTER*>  Clusters;
  XVECTOR<GRPSKINPOSE*>     PoseNodes;

      virtual bool          SetValue    (GRPANIMATIONCURVETYPE type,GRPANIMATIONCURVEAXIS subtype, double value,double base,unsigned int offbound);

private:

    void                    Clean       ()
                            {
                              BoneMatrices      =NULL;
                              PoseMatrices      =NULL;
                              BoneWeightsArray  =NULL;
                              BoneIndexArray    =NULL;
                              target            =NULL;
                              shader            =NULL;
                            }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/



#endif

