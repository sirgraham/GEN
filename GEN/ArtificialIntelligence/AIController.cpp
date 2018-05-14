
/*------------------------------------------------------------------------------------------
//  AICONTROLLER.CPP
//
//  Manages ai state controller
//
//  Author            : Diego Martinez Ruiz de Gaona
//  Date Of Creation  : 01/12/2016 13:11:13
//  Last Modification :
//
//  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "AIController.h"
#include "AIState.h"
#include "AIAnimation.h"
#include "GRPObject.h"
#include "SNDAnimationListener.h"

#include "XVariant.h"
#include "XFactory.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/

/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//  AI::CONTROLLER::CONTROLLER
*/
/**
//  Constructor
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/12/2016 13:30:06
//
//  @return       AI::CONTROLLER::CONTROLLER() :
//
*/
/*-----------------------------------------------------------------*/
AI::CONTROLLER::CONTROLLER()
{
  Clean();
  this->mutex=xfactory->Create_Mutex();
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::~CONTROLLER
*/
/**
//  Destructor
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/12/2016 13:30:15
//
//  @return       AI::CONTROLLER::~CONTROLLER() :
//
*/
/*-----------------------------------------------------------------*/
AI::CONTROLLER::~CONTROLLER()
{
  this->states.DeleteElementContents();
  this->states.DeleteAll();

  this->commands.DeleteElementContents();
  this->commands.DeleteAll();

  this->overlays.DeleteElementContents();
  this->overlays.DeleteAll();

  this->variables.DeleteElementContents();
  this->variables.DeleteAll();

  if (mutex)
    delete mutex;
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::Process
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      09/12/2016 14:05:34
//
//  @return       bool :
//
//  @param        command :
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::Process(AI::COMMAND* command)
{
  if (!command)
    return false;

  switch(command->type)
  {
  case AI::COMMAND::PUSH_STATE:
    if (this->stateStack.GetSize()==0)
        return this->PushState(command->value);
    else
    if (this->stateStack.GetLast()!=command->value)
    {
        XSTRING name = __L("Invalid");
        if (this->states.Get(this->currentState))
            this->states.Get(this->currentState)->GetName()->Get();
        if (verbose)
        {
            XDEBUG_PRINTCOLOR(0, __L("Pushing state: %d over %d states [current:%s]"), command->value, this->stateStack.GetSize(), name.Get());
        }
      return this->PushState(command->value);
    }
    break;

  case AI::COMMAND::CHANGE_STATE:
    return this->ChangeState(command->value);
    break;


  case AI::COMMAND::CHANGE_OVERLAY:
    for (XDWORD e=0;e<command->properties.GetSize();e++)
    {
      XDWORD key=command->properties.GetKey(e);
      XDWORD value=command->properties.GetElement(e);
      if (this->overlays.Get(key))
        this->overlays.Get(key)->SetStatus(value);
    }
    break;

  case AI::COMMAND::ALTER_PROPERTY:

    for (XDWORD e=0;e<command->params.GetSize();e++)
    {
      XDWORD    key   =command->params.GetKey(e);
      XVARIANT* value =command->params.GetElement(e);

      if (this->variables.Find(key)!=NOTFOUND)
      {
        XVARIANT* variant=this->variables.Get(key);
                  *variant=*value;
        Publish(GRPMSG_PROPERTY_CHANGED);
      }
    }
    break;
  }



  return true;
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::Process
*/
/**
//
//  Processes commmand by id
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      12/12/2016 11:08:36
//
//  @return       bool :
//
//  @param        command :
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::Process(XDWORD commandid)
{
  AI::COMMAND* command=this->commands.Get(commandid);
  return this->Process(command);
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::Update
*/
/**
//  Updates the machine state, triggers events, manages ovelays
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/12/2016 13:32:00
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::Update()
{
  if (!this->owner)
    return false;

  if (states.GetSize())
    UpdateStates  ();

    UpdateOverlays();

  return true;
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::UpdateStates
*/
/**
//
//  Updates FSM
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/12/2016 13:31:41
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::UpdateStates()
{
    AI::STATE*  state   =this->GetStates()->Get(currentState);

    XDWORD pop=0;
    if (PeekStack(&pop)!=0)
    {
        AI::STATE* newstate=this->GetStates()->Get(pop);
        if (!newstate)
        {
            PopState();
            return false;
        }

        if (state != NULL)
        {
            if (state->Accept(newstate))
            {
                PopState();
                this->ChangeState(newstate->GetIndex());
                if (verbose){ XDEBUG_PRINTCOLOR(2, __L("---- POP STATE: %d %s"), newstate->GetIndex(), GetStates()->Get(newstate->GetIndex())->GetName()->Get()); }
            }
            else
                if (state->Reject(newstate))
                {
                    PopState();
                    if (verbose){ XDEBUG_PRINTCOLOR(3, __L("---- POP STATE: %d %s [REJECTED]"), newstate->GetIndex(), GetStates()->Get(newstate->GetIndex())->GetName()->Get());}
                }
        }
        else
            SetState(pop);
    }

    if (state==NULL || state->Step()==0)
    {
      if (state==NULL)
      {
        if (this->verbose){ XDEBUG_PRINTCOLOR(4,__L("AI::CONTROLLER::Update()  <%s> SEQUENCE ERROR : requested[%d] turned out not valid [maximun animation id<%d]"),this->owner->GetName()->Get(),currentState,this->owner->GetAnimationControl()->GetStacks()->GetSize()); }
        this->lastState     = 0;
        this->currentState  = 0;
      }
      else
      {
        int n=this->owner->GetAnimationControl()->GetStacks()->GetSize();

        for (int e=n-1;e>=0;e--)
          this->owner->GetAnimationControl()->GetStacks()->GetElement(e)->DisableMilestones();
      }

      //------------

        if (this->verbose)        { XDEBUG_PRINTCOLOR(4,__L("AI::CONTROLLER::Update()  going back to default: %s"),this->states.Get(defaultState)->GetName()->Get()); }
            SetState(defaultState);
    }

    return true;
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::UpdateOverlays
*/
/**
//
//  Updates overlays on state
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      07/12/2016 13:32:40
//
//  @return       bool :
//
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::UpdateOverlays()
{

    //-------------------------------------------------------------------------------------- OVERLAYS

  XDWORD currentStateID=this->currentState;
  AI::STATE* state=this->states.Get(currentStateID);

    for (XDWORD e=0;e<this->overlays.GetSize();e++)
    {
      AI::OVERLAY* overlay=this->overlays.GetElement(e);
      if (!overlay)
        continue;

      XDWORD ol_index=overlay->GetID();
      XDWORD ol_state=overlay->GetStatus();

      GRPANIMATIONSTACK* stack=this->owner->GetAnimationControl()->GetStack(ol_index);
      if (!stack)
        continue;

      switch(ol_state)
      {
      case GRPANIMATION_STATUS_WAITING:
                                        break;
      case GRPANIMATION_STATUS_STARTED:
                                          if (StartOverlay(state,overlay))
                                          {
                                          ManageAttachees(state,overlay);
                                          overlay->SetStatus(GRPANIMATION_STATUS_PLAYING);
                                          overlay->Publish(GRPMSG_ENTER);
                                          }
                                        break;

      case GRPANIMATION_STATUS_STOPPED  : StopOverlay   (state,overlay);  break;
      case GRPANIMATION_STATUS_PAUSED   : PauseOverlay  (state,overlay);  break;
      case GRPANIMATION_STATUS_INACTIVE : DisableOverlay(state,overlay);  break;
      case GRPANIMATION_STATUS_PLAYING  :
                                        if (stack->IsFinished())
                                        {
                                            if (stack->GetLoopMode()!=GRPANIMATIONLOOPMODE_REPEAT)
                                            {
                                              overlay->SetStatus(GRPANIMATION_STATUS_FINISHED);
                                              overlay->Publish  (GRPMSG_EXIT);
                                            }
                                            else
                                            if (RestartOverlay(state,overlay))
                                            {
                                            //ManageAttachees(state,overlay);
                                            overlay->SetStatus(GRPANIMATION_STATUS_PLAYING);
                                            }
                                        }
                                        break;

      case GRPANIMATION_STATUS_FINISHED:
                                        {
                                          if (DisableOverlay(state,overlay))
                                            ManageAttachees(state,overlay);
                                        }
                                        break;
      }
    }

    return true;
}


/*-------------------------------------------------------------------
//  AI::CONTROLLER::SetState
*/
/**
//  Set current estate overrinding the state machine and change events
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/12/2016 13:32:09
//
//  @return       bool :
//
//  @param        state :
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::SetState(XDWORD newstate)
{
  this->mutex->Lock();

    AI::STATE*  next  =this->GetStates()->Get((XDWORD)newstate);
    SetState(next);

  this->mutex->UnLock();
  return next!=NULL;
}

/*-------------------------------------------------------------------
//   AI::CONTROLLER::SetState
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return   bool :
//
//  @param    AI::STATE* :
//
*//*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::SetState(AI::STATE* next)
{
    this->mutex->Lock();
    if (next)
    {
        next->StartAsCurrentState();
        this->currentState = next->GetIndex();
    }

    this->mutex->UnLock();
    return next != NULL;
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::PopState
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/02/2017 16:16:24
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD AI::CONTROLLER::PopState()
{
  if (PeekStack()>0)
  {
    XDWORD state= stateStack.Get(0);
                  stateStack.DeleteIndex(0);
    return state;
  }
  return 0;
}
/*-------------------------------------------------------------------
//  AI::CONTROLLER::PushState
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/02/2017 12:09:59
//
//  @return       bool :
//
//  @param        state :
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::PushState(XDWORD state)
{
  return this->stateStack.Add(state);
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::InsertState
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/02/2017 16:16:36
//
//  @return       bool :
//
//  @param        state :
*/
/*-----------------------------------------------------------------*/
bool AI::CONTROLLER::InsertState(XDWORD state)
{
  if (this->GetStates()->Get(currentState))
    this->GetStates()->Get(currentState)->FinishCurrentState();
  return this->stateStack.Insert(0,state);
}


/*-------------------------------------------------------------------
//  AI::CONTROLLER::PeekStack
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      16/02/2017 16:16:41
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD AI::CONTROLLER::PeekStack(XDWORD* state)
{
  XDWORD n=this->stateStack.GetSize();
  if (state!=NULL)
    *state=this->stateStack.Get(0);
  return n;
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::StartOverlay
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/02/2017 12:10:07
//
//  @return       bool :
//
//  @param        state :
//  @param        overlay :
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::StartOverlay(AI::STATE* state,AI::OVERLAY* overlay)
{
  if (state)
  {
    GRPANIMATIONSTACK* stack=this->owner->GetAnimationControl()->GetStack(overlay->GetID());
    if (!stack) return false;

    if (overlay->GetOverlayables()->GetSize()==0 || overlay->GetOverlayables()->Find(state->GetCurrentAnimation()->id)!=NOTFOUND)
    {
        overlay->SetStatus(GRPANIMATION_STATUS_STARTED);
        overlay->Publish(GRPMSG_ENTER);
        stack->SetActive(true);
        stack->Rewind();
        stack->Play();
    }
  }

  return true;
}


/*-------------------------------------------------------------------
//  AI::CONTROLLER::RestartOverlay
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/02/2017 12:10:07
//
//  @return       bool :
//
//  @param        state :
//  @param        overlay :
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::RestartOverlay(AI::STATE* state, AI::OVERLAY* overlay)
{
    if (state)
    {
        GRPANIMATIONSTACK* stack = this->owner->GetAnimationControl()->GetStack(overlay->GetID());
        if (!stack) return false;

        overlay->Publish(GRPMSG_RESTART);

        if (overlay->GetOverlayables()->GetSize() == 0 || overlay->GetOverlayables()->Find(state->GetCurrentAnimation()->id) != NOTFOUND)
        {
            overlay->SetStatus(GRPANIMATION_STATUS_STARTED);
            stack->SetActive(true);
            stack->Rewind();
            stack->Play();
        }
    }

    return true;
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::StopOverlay
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/02/2017 12:10:15
//
//  @return       bool :
//
//  @param        state :
//  @param        overlay :
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::StopOverlay(AI::STATE* state,AI::OVERLAY* overlay)
{
  if (state)
  {
    GRPANIMATIONSTACK* stack=this->owner->GetAnimationControl()->GetStack(overlay->GetID());;
    if (!stack) return false;

    if (overlay->GetOverlayables()->GetSize()==0 || overlay->GetOverlayables()->Find(state->GetCurrentAnimation()->id)!=NOTFOUND)
    {
        overlay->SetStatus(GRPANIMATION_STATUS_STOPPED);
        stack->SetActive(true);
        stack->Stop();
    }

    ManageAttachees(state,overlay);
  }
  return true;
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::PauseOverlay
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/02/2017 12:10:22
//
//  @return       bool :
//
//  @param        state :
//  @param        overlay :
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::PauseOverlay(AI::STATE* state,AI::OVERLAY* overlay)
{
  if (state)
  {
    GRPANIMATIONSTACK* stack=this->owner->GetAnimationControl()->GetStack(overlay->GetID());;
    if (!stack) return false;

    if (overlay->GetOverlayables()->GetSize()==0 || overlay->GetOverlayables()->Find(state->GetCurrentAnimation()->id)!=NOTFOUND)
    {
        overlay->SetStatus(GRPANIMATION_STATUS_PAUSED);
        stack->SetActive(false);
        stack->Pause();
    }
  }
  return true;
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::DisableOverlay
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/02/2017 12:10:32
//
//  @return       bool :
//
//  @param        state :
//  @param        overlay :
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::DisableOverlay(AI::STATE* state,AI::OVERLAY* overlay)
{
  if (state)
  {
        overlay->SetStatus(GRPANIMATION_STATUS_INACTIVE);
        GRPANIMATIONSTACK* stack=this->owner->GetAnimationControl()->GetStack(overlay->GetID());
        if (!stack) return false;
        stack->SetActive(false);
        stack->Pause();
  }
  return true;
}


/*-------------------------------------------------------------------
//  AI::CONTROLLER::FinishOverlay
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/02/2017 12:10:37
//
//  @return       bool :
//
//  @param        state :
//  @param        overlay :
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::FinishOverlay(AI::STATE* state,AI::OVERLAY* overlay)
{
  if (state)
  {
    GRPANIMATIONSTACK* stack=this->owner->GetAnimationControl()->GetStack(overlay->GetID());;
    if (!stack) return false;

    if (overlay->GetOverlayables()->GetSize()==0 || overlay->GetOverlayables()->Find(state->GetCurrentAnimation()->id)!=NOTFOUND)
    {
      overlay->Publish(GRPMSG_EXIT);
      overlay->SetStatus(GRPANIMATION_STATUS_FINISHED);
      stack->Rewind   ();
      stack->SetActive(false);
      stack->SetFrame (0);
    }
  }
  return true;
}


/*-------------------------------------------------------------------
//  AI::CONTROLLER::ManageAttachees
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      10/02/2017 12:10:44
//
//  @return       bool :
//
//  @param        state :
//  @param        overlay :
*/
/*-------------------------------------------------------------*/
bool  AI::CONTROLLER::ManageAttachees(AI::STATE* state,AI::OVERLAY* overlay)
{
for (XDWORD e=0;e<overlay->GetAttachees()->GetSize();e++)
    {
      XDWORD attached_id=overlay->GetAttachees()->GetKey(e);
      XDWORD status_id=overlay->GetAttachees()->GetElement(e);
      GRPANIMATIONSTACK* attached=this->owner->GetAnimationControl()->GetStack(attached_id);
      if (attached)
      {
        AI::OVERLAY* attachee=this->overlays.Get(attached_id);
        if (attachee)
        switch(status_id)
        {
        case GRPANIMATION_STATUS_STARTED:  StartOverlay   (state,attachee); break;
        case GRPANIMATION_STATUS_STOPPED:  StopOverlay    (state,attachee); break;
        case GRPANIMATION_STATUS_INACTIVE: DisableOverlay (state,attachee); break;
        }
      }
    }
return true;
}


/*-------------------------------------------------------------------
//   AI::CONTROLLER::SubscribeToEvents
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return   void :
//
//  @param     :
//
*//*-----------------------------------------------------------------*/
void  AI::CONTROLLER::SubscribeToEvents()
{

    GRPOBJECT* obj = this->owner;
    for (XDWORD e = 0; e < obj->GetAnimationControl()->GetStacks()->GetSize(); e++)
    {
        GRPANIMATIONSTACK* stack = obj->GetAnimationControl()->GetStacks()->GetElement(e);
        for (XDWORD m = 0; m < stack->GetMilestones()->GetSize(); m++)
        {
            GRPANIMATIONSTACKMILESTONE* milestone = stack->GetMilestones()->Get(m);
            if (milestone->GetType() == GRPANIMATIONSTACKMILESTONE::NOTIFY)
            {
                this->Subscribe(milestone);
            }

            SNDANIMATIONLISTENER* snd = dynamic_cast<SNDANIMATIONLISTENER*>(stack->GetMilestones()->Get(m));
            if (snd)
                if (snd->GetAudioType() == SNDANIMATIONLISTENER::PLAYLIST)
                {
                    this->Subscribe(snd);
                }

        }
    }

}

/*-------------------------------------------------------------------
//   AI::CONTROLLER::SetOwner
*/
/**
//
//
//
//  @author   Diego Martinez Ruiz de Gaona
//  @version
//  @return   void :
//
//  @param    GRPOBJECT* :
//
*//*-----------------------------------------------------------------*/
void  AI::CONTROLLER::SetOwner(GRPOBJECT* obj)
{
    this->owner = obj;
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::ChangeState
*/
/**
//  Changes state and check sequence is valid, execute change events
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/12/2016 13:32:24
//
//  @return       bool :
//
//  @param        state :
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::ChangeState(XDWORD newstate)
{
  bool success=false;

  this->mutex->Lock();

  int index=this->GetStates()->Find(newstate);
  if (index==NOTFOUND)
    {
      XDEBUG_PRINTCOLOR(4,__L("Invalid state!"));
      this->mutex->UnLock();
      return false;
    }


  //-----------
  XDWORD last           =this->currentState;


  AI::STATE*  next  =this->GetStates()->Get((XDWORD)newstate);

    if (this->currentState==next->GetIndex())
    {
      this->mutex->UnLock();
      return true;
    }

  AI::STATE*  state =this->GetStates()->Get((XDWORD)currentState);

    if(state)
    {
      if (state->Accept(next))
      {
          if (verbose){ XDEBUG_PRINTCOLOR(2,__L("Accepted state! %s"),next->GetName()->Get()); }

          state->FinishCurrentState();
          state = next;
          if (state)
          {
              state->StartAsCurrentState();
              this->currentState    =newstate;
              success=true;
          }
      }
      else
      {
        this->currentState=last;

        if (verbose)        { XDEBUG_PRINTCOLOR(4,__L("Unnaceptable : State Requested: %d %s Current: %d %s"),next->GetIndex(),next->GetName()->Get(),state->GetIndex(),state->GetName()->Get()); }
        success=false;
      }
    }
    else
    {
        SetState(newstate);
        success=false;
    }

  this->mutex->UnLock();
  return success;
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::GetCurrentState
*/
/**
//
//  Get current state
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      01/12/2016 13:32:58
//
//  @return       XDWORD :
//
*/
/*-----------------------------------------------------------------*/
XDWORD  AI::CONTROLLER::GetCurrentState ()
{
  return this->currentState;
}


/*-------------------------------------------------------------------
//  AI::CONTROLLER::Add
*/
/**
//
//  adds controller to controller pool
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      02/12/2016 11:03:02
//
//  @return       bool :
//
//  @param        state :
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::Add(XDWORD id, AI::STATE* state)
{
  state->SetController(this);
  this->Subscribe(state);

  return this->states.Add(id, state);
}

/*-------------------------------------------------------------------
//  AI::CONTROLLER::Add
*/
/**
//
//
//
//  @author       Diego Martinez Ruiz de Gaona
//  @version      13/02/2017 15:52:27
//
//  @return       bool :
//
//  @param        id :
//  @param        overlay :
*/
/*-----------------------------------------------------------------*/
bool  AI::CONTROLLER::Add(XDWORD id, AI::OVERLAY* overlay)
{
  overlay->SetController(this);
  this->Subscribe(overlay);
  return this->overlays.Add(id, overlay);
}