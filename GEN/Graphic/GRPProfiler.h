/*------------------------------------------------------------------------------------------
//  GRPPROFILER.H
*/
/**
// \class
//
//  Profile the graphic application
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 14/10/2015 10:21:12
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPPROFILER_H_
#define _GRPPROFILER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XBase.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class XTIMER;

/*---- CLASS -----------------------------------------------------------------------------*/
class GRPPROFILER
{
public:

  static const XDWORD nmeasures=10;

  static XDWORD   showFrameEvery;
  static XDWORD   frames;
  static XDWORD   framesFromLastMeasure;
  static XDWORD   measures;

  static double   AddFrames;

  static XDWORD   fps;
  static double   fpsAvg;
  static double   ft;

  static XDWORD   Passes;
  static XDWORD   Scenes;


  static XDWORD   nShaderChanges;
  static XDWORD   nBufferChanges;
  static XDWORD   nTextureChanges;
  static XDWORD   nRegisterAccess;
  static XDWORD   nMatrixOperations;
  static XDWORD   nFacesRendered;
  static XDWORD   nDrawCalls;
  static XDWORD   nNodes;
  static XDWORD   nNodesUpdates;
  static XDWORD   nElements;
  static XDWORD   nCulledElements;

  static XDWORD   vertexmemory;
  static XDWORD   texturememory;

  static XDWORD   DomainChecks;

  static XTIMER*  xtimershowframerate;

  static bool     showstatistics;

  static XDWORD   last_measures[nmeasures];

  static void     SetShowStatistics   (bool showstatistics);
  static void     ShowStatistics      ();
  static void     Clear               ();
  static void     Delete              ();

};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

