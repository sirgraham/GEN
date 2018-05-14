/*------------------------------------------------------------------------------------------
//  GRPOBJECTBUILDERALGORITHMSTROKE.H
*/
/**
// \class
//
//  gives width to shape
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 25/11/2014 11:15:51
//  Last Modification :
//
//  http://www.codeproject.com/Articles/226569/Drawing-polylines-by-tessellation
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPOBJECTBUILDERALGORITHMTESELLATE_H_
#define _GRPOBJECTBUILDERALGORITHMTESELLATE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPElement.h"
#include "GRPObjectBuilder.h"
#include "GRPObjectBuilderAlgorithm.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

enum GRPOBJECTBUILDERALGORITHMSTROKEENDTYPE
{
        GRPOBJECTBUILDERALGORITHMSTROKEENDTYPE_SQUARE=0,
        GRPOBJECTBUILDERALGORITHMSTROKEENDTYPE_ROUND //uninmplemented
};

enum GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE
{
        GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE_AXED=0,
        GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE_OPEN,
        GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE_MITTER,
        GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE_BEVEL, //uninmplemented
        GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE_ROUND  //uninmplemented
};

enum GRPOBJECTBUILDERALGORITHMSTROKEPENTYPE
{
        GRPOBJECTBUILDERALGORITHMSTROKEPENTYPE_NORMAL=0,
        GRPOBJECTBUILDERALGORITHMSTROKEPENTYPE_DASHES,
        GRPOBJECTBUILDERALGORITHMSTROKEPENTYPE_DOTS
};


/*---- CLASS -----------------------------------------------------------------------------*/
class GRPOBJECTBUILDERALGORITHMSTROKE : public GRPOBJECTBUILDERALGORITHM
{


   // ------------------------ GETTER / SETTERS ----------------------
public:
  GLFLOAT                                  GetExtrude     ()                                           { return extrude;}
  void                                     SetExtrude     (GLFLOAT p)                                  { this->extrude=p; }
  float                                    GetInnerWidth  ()                                           { return innerwidth;}
  void                                     SetInnerWidth  (float p)                                    { this->innerwidth=p; }
  float                                    GetOuterWidth  ()                                           { return outerwidth;}
  void                                     SetOuterWidth  (float p)                                    { this->outerwidth=p; }
  float                                    GetDoubleSided ()                                           { return doublesided;}
  void                                     SetDoubleSided (float p)                                    { this->doublesided=p; }
  int                                      GetNSegments   ()                                           { return nsegments;}
  void                                     SetNSegments   (int p)                                      { this->nsegments=p; }
  int                                      GetCloseSpline ()                                           { return closeSpline;}
  void                                     SetCloseSpline (int p)                                      { this->closeSpline=p; }
  GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE GetJointType   ()                                           { return jointtype;}
  void                                     SetJointType   (GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE p) { this->jointtype=p; }
  GRPOBJECTBUILDERALGORITHMSTROKEENDTYPE   GetEndType     ()                                           { return endtype;}
  void                                     SetEndType     (GRPOBJECTBUILDERALGORITHMSTROKEENDTYPE p)   { this->endtype=p; }
  GRPOBJECTBUILDERALGORITHMSTROKEPENTYPE   GetPenType     ()                                           { return pentype;}
  void                                     SetPenType     (GRPOBJECTBUILDERALGORITHMSTROKEPENTYPE p)   { this->pentype=p; }
  void                                     SetBevel       (float p)                                    { this->bevel=p; }
  void                                     SetMitterWidth (float p)                                    { this->mitterTreshold=p;}

                        GRPOBJECTBUILDERALGORITHMSTROKE   (GRPOBJECTBUILDER* builder) ;
  GRPELEMENT*                             Execute         (GRPSHAPE* shape);

  //----------- calculations
  void                                    Bevel           (XVECTOR<GRPPOINT*>* Stack, int Current, GRPPOINT**  v, GRPSHAPE* shape, int* index);

  //----------- surface
  void                                    Triangulate     (XVECTOR<GRPPOINT*>* Stack,XVECTOR<GRPFACE*>*  faces);
  void                                    Points          (XVECTOR<GRPPOINT*>* Stack,XVECTOR<GRPFACE*>*  faces);

protected:
 // ------------------------ MEMBERS ----------------------

  GLFLOAT                                  extrude;
  GLFLOAT                                  bevel;
  float                                    innerwidth;
  float                                    outerwidth;
  float                                    doublesided;
  int                                      nsegments;
  int                                      closeSpline;
  GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE jointtype;
  GRPOBJECTBUILDERALGORITHMSTROKEENDTYPE   endtype;
  GRPOBJECTBUILDERALGORITHMSTROKEPENTYPE   pentype;

  float                                    mitterTreshold;

private:

      void Clean()
      {
        pentype         = GRPOBJECTBUILDERALGORITHMSTROKEPENTYPE_NORMAL;
        jointtype       = GRPOBJECTBUILDERALGORITHMSTROKEJOINTTYPE_AXED;
        endtype         = GRPOBJECTBUILDERALGORITHMSTROKEENDTYPE_SQUARE;
        doublesided     = false;
        extrude         = 0.0;
        nsegments       = 1;
        closeSpline     = 0;
        innerwidth      = 0.0f;
        outerwidth      = 0.02f;
        mitterTreshold  = 0.0f;
        bevel=0.0;
      }
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

