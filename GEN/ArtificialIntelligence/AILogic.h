/*------------------------------------------------------------------------------------------
//  AILOGIC.H
*/
/**
// \class
//
//  Manages engine logic
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 01/12/2016 13:15:08
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _AILOGIC_H_
#define _AILOGIC_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "XVector.h"
#include "GRPSubject.h"
#include "AIController.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class INPDEVICE;

/*---- CLASS -----------------------------------------------------------------------------*/

namespace AI
{

  class LOGIC : public GRPOBSERVER, public GRPSUBJECT
  {
  public:

                                        LOGIC           ();
    virtual                             ~LOGIC          ();

            void                        Add             (AI::CONTROLLER*    controller);
    virtual bool                        Update          (INPDEVICE*         device=NULL);

    virtual XDWORD                      GetSubjectType  ();
    virtual bool                        Notify          (GRPMSGTYPE         msg,        GRPSUBJECT* subject);

            XVECTOR<AI::CONTROLLER*>*   GetControllers();

  protected:

    XVECTOR<AI::CONTROLLER*> controllers;

  private:

    void Clean()
    {

    }

  };

};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

