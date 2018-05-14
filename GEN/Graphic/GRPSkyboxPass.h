/*------------------------------------------------------------------------------------------
//  GRPSKYBOXPASS.H
*/
/**
// \class
//
//  Manages skybox pass
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 20/06/2017 10:56:42
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPSKYBOXPASS_H_
#define _GRPSKYBOXPASS_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPbackgroundPass.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPMATERIAL;

/*---- CLASS -----------------------------------------------------------------------------*/

class GRPSKYBOXPASS : public GRPBACKGROUNDPASS
{
public:

                GRPSKYBOXPASS() : GRPBACKGROUNDPASS(__L("skybox")){}
  virtual       ~GRPSKYBOXPASS() {}

  virtual bool   OnAdded();
  virtual void  CreateCamera    (GRPCAMERA_TYPE type) { };

protected:

private:

  void Clean()
  {

  }

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

