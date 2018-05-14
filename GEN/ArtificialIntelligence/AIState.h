/*------------------------------------------------------------------------------------------
//  AISTATE.H
*/
/**
// \class
//
//  Manages States
//
//  @author  Diego Martinez Ruiz de Gaona
//
//  Date Of Creation  : 01/12/2016 11:40:44
//  Last Modification :
*/
/*  GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/

#ifndef _AISTATE_H_
#define _AISTATE_H_


/*---- INCLUDES --------------------------------------------------------------------------*/

#include "GRPSubject.h"
#include "AIState.h"
#include "AITransition.h"
#include "XMap.h"

/*---- DEFINES & ENUMS  ------------------------------------------------------------------*/

class GRPANIMATIONSTACK;
class XVARIANT;

namespace AI
{
  class ANIMATION;
  class CONTROLLER;

//--------------------------------------------------------------------------------OVERLAY
class OVERLAY : public AI::TRANSITION
{
public:
                                OVERLAY                 ()                { Clean();                                          }
                                ~OVERLAY                ()                { Overlayables.DeleteAll();Attachees.DeleteAll();   }
    XVECTOR <XDWORD>*           GetOverlayables         ()                { return &this->Overlayables;                       }
    XMAP    <XDWORD,XDWORD>*    GetAttachees            ()                { return &this->Attachees;                          }
    XDWORD                      GetID                   ()                { return id;                                        }
    void                        SetID                   (XDWORD id)       { this->id=id;                                      }
    XDWORD                      GetStatus               ()                { return status;                                    }
    void                        SetStatus               (XDWORD status)   { this->status=status;                              }
    void                        SetInitialStatus        (XDWORD status)   { this->initialstatus=status;                       }
    XDWORD                      GetInitialStatus        ()                { return this->initialstatus;                       }
    XDWORD                      GetSubjectType          ()                { return AI_OVERLAY;                                }
    void                        SetController           (AI::CONTROLLER* c) { this->Owner=c;}


protected:

    XVECTOR<XDWORD>             Overlayables;
    XMAP<XDWORD,XDWORD>         Attachees;

    XDWORD                      id;
    XDWORD                      status;
    XDWORD                      initialstatus;

    AI::CONTROLLER*             Owner;

private:

    void Clean()
    {
      id    =0;
      status=0;
      Owner =NULL;
    }
};

//--------------------------------------------------------------------------------COMMAND
class COMMAND
{
public:
  enum
  {
    UNKNOWN=0,
    CHANGE_STATE,
    CHANGE_OVERLAY,
    ALTER_PROPERTY,
    ADD_PROPERTY,
    DEL_PROPERTY,
    PUSH_STATE
  };

  ~COMMAND();

  XDWORD type;
  XDWORD id;
  XDWORD value;
  XSTRING name;

  XMAP<XDWORD,XDWORD>     properties;
  XMAP<XDWORD,XVARIANT*>  params;
};

//--------------------------------------------------------------------------------TRACK

class TRACK
{
public:
                            TRACK         ();
                            ~TRACK        ();
    bool                    Add           (AI::ANIMATION* ani);
    int                     Step          ();
    bool                    ManageFinish  (GRPANIMATIONSTACK* stack);


    void                    SetMilestones ();

    XDWORD                  currentAnimation;
    XDWORD                  currentLoop;
    XDWORD                  status;
    XBYTE                   priority;
    float                   weight;
    float                   speed;
    XSTRING                 name;

    AI::STATE*              ownerstate;
    XVECTOR<AI::ANIMATION*> animation;
};

/*---- CLASS -----------------------------------------------------------------------------*/
class STATE : public AI::TRANSITION , public GRPOBSERVER
{
public:

                                          STATE                   (XDWORD id,XCHAR* name);
virtual                                   ~STATE                  ();
      void                                Add                     (AI::ANIMATION* ani);
      void                                Add                     (AI::TRACK*     track);
      int                                 Begin                   ();
      int                                 Play                    ();
      int                                 Stop                    ();
      int                                 Step                    ();

      //void                                SetMilestones           (int current=0);

      AI::ANIMATION*                      GetLastAnimation        ();
      AI::ANIMATION*                      GetCurrentAnimation     ();
      AI::ANIMATION*                      GetNextAnimation        ();

      XDWORD                              GetIndex                ();

      void                                StartAsCurrentState     ();
      void                                FinishCurrentState      ();
      bool                                Accept                  (STATE* newstate);
      bool                                Reject                  (STATE* newstate);

      GRPANIMATIONSTACK*                  GetStack();

      XDWORD                              GetSubjectType          ();
      void                                SetName                 (XCHAR* name);
      XFSTRING*                           GetName                 ();
      void                                SetController           (AI::CONTROLLER* controller);
      AI::CONTROLLER*                     GetController           ();

      XVECTOR<AI::TRACK*>*                GetTracks               ()  { return &tracks;}

      bool                                Notify                  (GRPMSGTYPE, GRPSUBJECT*);

  protected:

      bool                                ManageFinish            (GRPANIMATIONSTACK* stack);

      XDWORD                              id;
      XVECTOR<AI::TRACK*>                 tracks;
      CONTROLLER*                         controller;

protected:

private:

      void Clean()
      {
        id=0;
        //currentAnimation=0;
        //currentLoop=0;
        controller=NULL;
      }
};
};

/*---- INLINE FUNCTIONS ------------------------------------------------------------------*/

#endif

