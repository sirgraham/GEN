/*------------------------------------------------------------------------------------------
//  AITRANSITION.H
*/
/**
// \class
//
//  Manages transitions between animations
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 01/12/2016 13:22:46
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _AITRANSITION_H_
#define _AITRANSITION_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPSubject.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

/*---- CLASS -----------------------------------------------------------------------------*/
namespace AI
{
    class STATE;

    class TRANSITION : public GRPSUBJECT
    {
    public:
      enum ENDEVENT
      {
        ENDEVENT_NONE       =0,
        ENDEVENT_PAUSE      =1<<1,
        ENDEVENT_REWIND     =1<<2,
        ENDEVENT_FORWARD    =1<<3,
        ENDEVENT_DISABLE    =1<<4,
        ENDEVENT_HIDE       =1<<5,
        ENDEVENT_MSG        =1<<6,
        ENDEVENT_PUSHFRAME  =1<<7,
        ENDEVENT_POPFRAME   =1<<8
      };

        enum ACCEPTTYPE
        {
          ACCEPT_NEVER=0,
          ACCEPT_ALWAYS,
          ACCEPT_ONLY_IF_FINISHED
        };

        XVECTOR<XDWORD>*      GetTransitions();
        XVECTOR<XDWORD>*      GetRejections ();
        bool                  Accept        (XDWORD newanimation);
        bool                  Reject        (XDWORD newanimation);

    protected:

        STATE*                owner_state;

        XVECTOR<XDWORD>       transitions;
        XVECTOR<XDWORD>       rejections;
        XVECTOR<bool>         hasjump;
        XVECTOR<XQWORDSIG>    framejump;
    };
  };


/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

