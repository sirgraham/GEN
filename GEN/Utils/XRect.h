/*------------------------------------------------------------------------------------------
//  XRECT.H
*/
/**
// \class
//
//  Manages rect areas
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 09/08/2017 17:34:17
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _XRECT_H_
#define _XRECT_H_


/*---- INCLUDES --------------------------------------------------------------------------*/


/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/
class XRECT
{
public:

  XRECT() : originx(0.0f),originy(0.0f),endx(1.0f),endy(1.0f) {};
  virtual ~XRECT() {}

  float originx;
  float originy;
  float endx;
  float endy;

private:

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

