/*------------------------------------------------------------------------------------------
//  AICONTROLLER.H
*/
/**
// \class
//
//  Controls State Machines for animations
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 01/12/2016 11:45:57
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _AICONTROLLER_H_
#define _AICONTROLLER_H_


/*---- INCLUDES --------------------------------------------------------------------------*/
#include "AIState.h"
#include "XThread.h"

#include "GRPSubject.h"
#include "GRPObserver.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/


class GRPOBJECT;
class XVARIANT;

namespace AI
{


/*---- CLASS -----------------------------------------------------------------------------*/
class CONTROLLER : public GRPSUBJECT, public GRPOBSERVER
{
public:
                            CONTROLLER            ();
  virtual                   ~CONTROLLER           ();

  bool                      Process               (AI::COMMAND* command);
  bool                      Process               (XDWORD command);

  bool                      Update                ();
  bool                      UpdateStates          ();
  bool                      UpdateOverlays        ();

  bool                      ChangeState           (XDWORD state);
  XDWORD                    GetCurrentState       ();
  bool                      SetState              (XDWORD state);
  bool                      SetState              (AI::STATE* state);

  XDWORD                    PopState              ();
  bool                      PushState             (XDWORD state);
  bool                      InsertState           (XDWORD state);
  XDWORD                    PeekStack             (XDWORD* state=NULL);

  bool                      StartOverlay          (AI::STATE* state,AI::OVERLAY* overlay);
  bool                      RestartOverlay        (AI::STATE* state,AI::OVERLAY* overlay);
  bool                      StopOverlay           (AI::STATE* state,AI::OVERLAY* overlay);
  bool                      PauseOverlay          (AI::STATE* state,AI::OVERLAY* overlay);
  bool                      DisableOverlay        (AI::STATE* state,AI::OVERLAY* overlay);
  bool                      FinishOverlay         (AI::STATE* state,AI::OVERLAY* overlay);
  bool                      ManageAttachees       (AI::STATE* state,AI::OVERLAY* overlay);
  void                      SubscribeToEvents     ();

  void                      SetOwner              (GRPOBJECT* obj);

  XDWORD                    GetSubjectType        ()                                    { return AI_CONTROLLER;                       }

  void                      SetDefaultState       (XDWORD state)                        { defaultState=state;                         }
  XDWORD                    GetDefaultState       ()                                    { return defaultState;                        }
  void                      ChangeToDefaultState  ()                                    { ChangeState(defaultState);                  }
  bool                      Add                   (XDWORD id, AI::STATE* state);
  bool                      Add                   (XDWORD id, AI::OVERLAY* state);

  XMAP<XDWORD,AI::STATE*>*  GetStates             ()                                    { return &states;                             }
  XMAP<XDWORD,AI::OVERLAY*>*GetOverlays           ()                                    { return &overlays;                           }
  XMAP<XDWORD,AI::COMMAND*>*GetCommands           ()                                    { return &commands;                           }
  XMAP<XDWORD,XVARIANT*>*   GetVariables          ()                                    { return &variables;                          }

  AI::STATE*                GetState              ()                                    { return states.Get(this->currentState);      }
  XVECTOR<XDWORD>*          GetStateStack         ()                                    { return &stateStack;                         }


  GRPOBJECT*                GetOwner              ()                                    { return owner;                               }
  virtual bool              Notify                (GRPMSGTYPE msg,GRPSUBJECT* notifier) { this->Publish(msg,notifier);   return false;}
  void                      SetVerbose            (bool b)                              { this->verbose=b;                            }

  bool                      verbose;


protected:

  XMAP<XDWORD,AI::COMMAND*> commands;
  XMAP<XDWORD,AI::STATE*>   states;
  XMAP<XDWORD,AI::OVERLAY*> overlays;

  XMAP<XDWORD,XVARIANT*>    variables;

  GRPOBJECT*                owner;
  XDWORD                    currentState;
  XDWORD                    defaultState;
  XDWORD                    lastState;

  XMUTEX*                   mutex;

  XVECTOR<XDWORD>           stateStack;

private:

  void Clean()
  {
    verbose       =false;
    currentState  =0;
    lastState     =0;
    defaultState  =0;
    owner         =NULL;
    mutex         =NULL;

    stateStack.SetIsMulti(true);
  }
};




};
/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

