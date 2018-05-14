/*------------------------------------------------------------------------------------------
//  GRPREFLECTIONPASS.H
*/
/**
// \class
//
//  Reflections
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 09/08/2016 11:16:35
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _GRPREFLECTIONPASS_H_
#define _GRPREFLECTIONPASS_H_


/*---- INCLUDES --------------------------------------------------------------------------*/


#include "GRPPass.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/
class GRPREFLECTIONPASS : public GRPPASS
{
public:

                    GRPREFLECTIONPASS   ();
  virtual           ~GRPREFLECTIONPASS  ();
  virtual   void    Start               ();
  virtual   void    Begin               ();
  virtual   bool    Draw                ();
  virtual   void    End                 ();

  virtual   void    GetReflectionCasters();

protected:

  GRPRENDERLIST Reflectors;
  GRPRENDERLIST Reflectables;

private:

  void Clean()
  {

  }

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

