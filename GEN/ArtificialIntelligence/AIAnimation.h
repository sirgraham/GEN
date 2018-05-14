/*------------------------------------------------------------------------------------------
//  AIANIMATION.H
*/
/**
// \class
//
//  Manages AI animations
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 01/12/2016 13:27:14
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _AIANIMATION_H_
#define _AIANIMATION_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "AITransition.h"
#include "GRPSubject.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/

namespace AI
{
  class TRANSITION;

  class ANIMATION : public GRPSUBJECT
  {
  public:
                                ANIMATION       (XDWORD id, XDWORD nloops = 1, int transition_milestone = -1);
                                ~ANIMATION      ();
    XDWORD                      GetSubjectType  ();
    bool                        Attach          (ANIMATION* ani);

    float                       weight;

    XDWORD                      id;
    XDWORD                      loops;
    bool                        finished;

    XDWORD                      start_frame;
    XDWORD                      end_frame;

    int                         transition;
    int                         next_start;
    int                         onStartEvent;
    int                         onEndEvent;

    XVECTOR<ANIMATION*>         Attachees;
    AI::TRANSITION::ACCEPTTYPE  acceptor;
    XSTRING                     name;

    AI::STATE*                  ownerstate;
  };
};


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

