/*------------------------------------------------------------------------------------------
//  AIANIMATION.CPP
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

/*---- INCLUDES --------------------------------------------------------------------------*/

#include "AIAnimation.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


/*---- CLASS -----------------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  AI::ANIMATION::ANIMATION
*/
/**
//
//  constructor
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return    :
//
//  @param    XDWORD : identifier of the animation
//  @param     XDWORD nloops : number of loops to proceed, 0 means infinite
//  @param    int : transition_milestone : the milestone that must be activated when transitioning
//
*//*-----------------------------------------------------------------*/
AI::ANIMATION::ANIMATION(XDWORD id, XDWORD nloops,int transition_milestone):
id            (id),
loops         (nloops),
transition    (transition_milestone),
next_start    (0),
onStartEvent  (AI::TRANSITION::ENDEVENT_REWIND),
onEndEvent    (AI::TRANSITION::ENDEVENT_NONE),
finished      (false),
acceptor      (AI::TRANSITION::ACCEPT_NEVER),
weight        (1.0f)
{

};

/*-------------------------------------------------------------------
//  ANIMATION::~ANIMATION
*/
/**
//
//  destructor
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return    :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
AI::ANIMATION::~ANIMATION()
{
    Attachees.DeleteContents();
    Attachees.DeleteAll();
}

/*-------------------------------------------------------------------
//   AI::ANIMATION::GetSubjectType
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version  20/10/2017 13:41:30
//  @return   XDWORD :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
XDWORD AI::ANIMATION::GetSubjectType()
{
    return AI_ANIMATION;
}

/*-------------------------------------------------------------------
//   AI::ANIMATION::Attach
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return   bool :
//
//  @param    ANIMATION* :
//
*//*-----------------------------------------------------------------*/
bool AI::ANIMATION::Attach(ANIMATION* ani)
{
    return this->Attachees.Add(ani);
}


