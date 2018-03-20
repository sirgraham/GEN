
/*------------------------------------------------------------------------------------------
//	AISTATE.CPP
//	
//	Manages AI states
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 02/12/2016 11:15:59
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "AIState.h"
#include "AIController.h"
#include "AIAnimation.h"

#include "XVariant.h"

#include "GRPAnimation.h"
#include "GRPObject.h"

	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/


/*-------------------------------------------------------------------
//	COMMAND::~COMMAND
*/	
/**	
//	
//	 Class Destructor COMMAND
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/03/2017 16:49:37
//	
*/
/*-----------------------------------------------------------------*/
AI::COMMAND::~COMMAND()
{
	properties.DeleteAll();		
	params.DeleteElementContents();
	params.DeleteAll();
}


/*-------------------------------------------------------------------
//	TRACK::TRACK
*/	
/**	
//	
//	Class Constructor TRACK
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/09/2017 13:05:19
//	
*/
/*-----------------------------------------------------------------*/
AI::TRACK::TRACK() : priority(0),weight(1.0f),speed(1.0),status	(0),currentAnimation(0),currentLoop(0)
{ 	

}


/*-------------------------------------------------------------------
//	TRACK::~TRACK
*/	
/**	
//	
//	 Class Destructor TRACK
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/09/2017 13:05:12
//	
*/
/*-----------------------------------------------------------------*/
AI::TRACK::~TRACK()
{ 
	animation.DeleteContents(); 
	animation.DeleteAll();							
}

/*-------------------------------------------------------------------
//	AI::TRACK::Add
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/09/2017 13:06:20
//	
//	@return 			bool : 
//
//  @param				ani : 
*/
/*-----------------------------------------------------------------*/
bool AI::TRACK::Add(AI::ANIMATION* ani)	
{	
		if (ani)
				ani->ownerstate = this->ownerstate;
	return	this->animation.Add(ani);		
}


/*-------------------------------------------------------------------
//	AI::STATE::STATE
*/	
/**	
//	
//	constructor
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:18:44
//	
//	@return 			AI::STATE::STATE(XCHAR* : 
//
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
AI::STATE::STATE(XDWORD id, XCHAR* name) 
{
	Clean();

	SetName(name);
	this->id=id;
}

/*-------------------------------------------------------------------
//	AI::STATE::~STATE
*/	
/**	
//	
//	destructor
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:18:52
//	
//	@return 			AI::STATE::~STATE() : 
//
*/
/*-----------------------------------------------------------------*/
AI::STATE::~STATE()						
{
	this->transitions.DeleteAll();
	this->tracks.DeleteContents();
	this->tracks.DeleteAll();
}			

/*-------------------------------------------------------------------
//	AI::STATE::Add
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:21:30
//	
//  @param				ani : 
*/
/*-----------------------------------------------------------------*/
void AI::STATE::Add(AI::ANIMATION* ani)
{
	if (this->tracks.GetSize()==0)	
		this->Add(new AI::TRACK());		
	
	this->Subscribe(ani);
	this->tracks.GetLast()->Add(ani);
}

/*-------------------------------------------------------------------
//	AI::STATE::Add
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/09/2017 12:33:54
//	
//  @param				track : 
*/
/*-----------------------------------------------------------------*/
void AI::STATE::Add(AI::TRACK*			track)
{
	track->ownerstate=this;
	this->tracks.Add(track);
}

/*-------------------------------------------------------------------
//	AI::STATE::Begin
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:21:35
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int AI::STATE::Begin()
{
	for (XDWORD t=0;t<tracks.GetSize();t++)
	{
		AI::TRACK* track=this->tracks.Get(t);	

		for (XDWORD e=0;e<track->animation.GetSize();e++)
		{
			AI::ANIMATION* animation=track->animation.Get(e);
			if (!animation) continue;

			GRPANIMATIONSTACK* stack=this->controller->GetOwner()->GetAnimationControl()->GetStack(animation->id);
			if (!stack) continue;
		
			stack->Rewind();

				for (XDWORD i=0;i<track->animation.Get(e)->Attachees.GetSize();i++)
				{
					AI::ANIMATION* ani=animation->Attachees.Get(i);				
					GRPANIMATIONSTACK*	stack=this->controller->GetOwner()->GetAnimationControl()->GetStack(ani->id);		
																if (stack)
																{
																stack->SetActive(true);
																stack->Rewind();
																stack->SetActive(false);
																}
				}		
		}
	}
	//this->Publish(GRPMSG_ENTER);
	return 0;
}

/*-------------------------------------------------------------------
//	AI::STATE::Play
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:21:42
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int AI::STATE::Play()
{
	for (XDWORD t=0;t<tracks.GetSize();t++)
	{
		AI::TRACK* track=this->tracks.Get(t);	
		if (track->animation.GetSize()==0)
			continue;

		GRPANIMATIONSTACK* stack=this->controller->GetOwner()->GetAnimationControl()->GetStack(track->animation.Get(track->currentAnimation)->id);
		if (stack)
		{
		stack->SetActive(true);
		stack->Play();

				for (XDWORD i=0;i<track->animation.Get(track->currentAnimation)->Attachees.GetSize();i++)
				{
					AI::ANIMATION* ani=track->animation.Get(track->currentAnimation)->Attachees.Get(i);
					if (ani)
					{
						AI::ANIMATION* ani=track->animation.Get(track->currentAnimation)->Attachees.Get(i);					
						GRPANIMATIONSTACK* attachedStack=this->controller->GetOwner()->GetAnimationControl()->GetStack(ani->id);															
						if (attachedStack) 
						{						
							attachedStack->SetActive(true);
							attachedStack->Play();
						}
					}
				}
		}
	}
	return 0;
}

/*-------------------------------------------------------------------
//	AI::STATE::Stop
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:21:49
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int AI::STATE::Stop()
{
	for (XDWORD t=0;t<tracks.GetSize();t++)
	{
	AI::TRACK* track=this->tracks.Get(t);	

	if (track->animation.GetSize() == 0) continue;

	XDWORD stack_id	= track->animation.Get(track->currentAnimation)->id;

	
	GRPANIMATIONSTACK*	stack=this->controller->GetOwner()->GetAnimationControl()->GetStack(stack_id);
	if (!stack)
		return 0;

											stack->Rewind();
											stack->Stop();

			for (XDWORD i=0;i<track->animation.Get(track->currentAnimation)->Attachees.GetSize();i++)
			{
				if (track->animation.Get(track->currentAnimation)->Attachees.Get(i))
				{
					AI::ANIMATION* ani=track->animation.Get(track->currentAnimation)->Attachees.Get(i);
					GRPANIMATIONSTACK*	stack=this->controller->GetOwner()->GetAnimationControl()->GetStack(ani->id);
					if (stack)
					{
															stack->DisableMilestones();
															stack->Rewind();
															stack->Stop();
					}
															
				}
			}
	}
	return 0;
}

/*-------------------------------------------------------------------
//	AI::STATE::Step
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:21:53
//	
//	@return 			int : 
//
*/
/*-----------------------------------------------------------------*/
int AI::STATE::Step()
{
	
	for(XDWORD t=0;t<tracks.GetSize();t++)
	{
			AI::TRACK* track=this->tracks.Get(t);	
			return track->Step();
	}
	return 1;
}


/*-------------------------------------------------------------------
//	 AI::TRACK::Step
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		int : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
int AI::TRACK::Step()
{
	XDWORD id=0;
			if (animation.Get(currentAnimation))
				id=animation.Get(currentAnimation)->id;
			else 
				return 0;

			GRPANIMATIONSTACK*	stack=ownerstate->GetController()->GetOwner()->GetAnimationControl()->GetStack(id);


			XDWORD ownerCurrentState=ownerstate->GetController()->GetCurrentState();

			if (stack)
			{
				
				//--------------------------------- check animation advance
				if (ownerCurrentState!=ownerstate->GetController()->GetCurrentState())	//STEP CHANGED THE STATE!
					return 1;

				if (this->animation.Get(this->currentAnimation)->id!=id)		//STEP CHANGED THE SEQUENCE!
					return 0;

				if (stack->IsFinished())
						return ManageFinish(stack);
				else
						this->ownerstate->Publish(GRPMSG_STEP);
			}
			else
				return 0;

	return 1;
}


/*-------------------------------------------------------------------
//	AI::STATE::ManageFinish
*/	
/**	
//	
//	Manage Animation Finish
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			25/01/2017 11:58:02
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool AI::STATE::ManageFinish(GRPANIMATIONSTACK*	stack)
{				
	return true;
}


/*-------------------------------------------------------------------
//	 AI::TRACK::ManageFinish 
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	17/10/2017 19:40:47
//	@return		bool : 
//
//	@param		GRPANIMATIONSTACK* : 
//
*//*-----------------------------------------------------------------*/
bool AI::TRACK::ManageFinish	(GRPANIMATIONSTACK*	stack)
{
			AI::ANIMATION* ani = this->animation.Get(currentAnimation);
			currentLoop++;
			

			if (this->ownerstate->GetController()->verbose)			
			{
					XDEBUG_PRINTCOLOR(0, __L("animation ended : %d %s  %s"), ani->id, ani->name.Get(), stack->GetName()->Get());
			}

			if (this->animation.Get(currentAnimation)->loops==0 || currentLoop < animation.Get(currentAnimation)->loops) // 0 is infinite
			{
				AI::ANIMATION* ani = this->animation.Get(currentAnimation);
				stack->Rewind();	
				for (XDWORD e=0;e<ani->Attachees.GetSize();e++)
							{
								XDWORD id= ani->Attachees.Get(e)->id;
								GRPANIMATIONSTACK*	stack=ownerstate->GetController()->GetOwner()->GetAnimationControl()->GetStack(id);
																		stack->SetActive	(true);					
																		stack->Rewind			();
																		stack->Play				();								
							}
				//ownerstate->GetController()->Publish(GRPMSG_RESTART);	
				if (this->ownerstate->GetController()->verbose) 
				{ XDEBUG_PRINTCOLOR(0, __L("animation restart : %d %s loop: %d/%d"), ani->id, ani->name.Get(), currentLoop, ani->loops); }

				ani->Publish(GRPMSG_RESTART);
				return 1;
			}						
			// animation ended
			ani->Publish(GRPMSG_EXIT);

				if (stack)
				{
				stack->DisableMilestones();
				stack->Stop							();
				stack->SetActive				(false);	

					for (XDWORD e=0;e<animation.Get(currentAnimation)->Attachees.GetSize();e++)
							{
								XDWORD id=animation.Get(currentAnimation)->Attachees.Get(e)->id;
								GRPANIMATIONSTACK*	stack=ownerstate->GetController()->GetOwner()->GetAnimationControl()->GetStack(id);
																		if (stack)
																		{
																		stack->DisableMilestones();
																		stack->Stop							();								
																		stack->SetActive				(false);					
																		}
							}

							// do end event -------------------

					if (animation.Get(currentAnimation)->onEndEvent & AI::TRANSITION::ENDEVENT_FORWARD)				
									stack->Forward();

							if (animation.Get(currentAnimation)->onEndEvent & AI::TRANSITION::ENDEVENT_PAUSE)				
							{
									animation.Get(currentAnimation)->finished=true;

									stack->SetActive(true);				
									stack->Forward();				
									
									if (stack->GetStatus() != GRPANIMATION_STATUS_PAUSED)
									{											
											ownerstate->Publish(GRPMSG_EXIT);
									}
									else
									{
											ownerstate->Publish(GRPMSG_PAUSE);
											ani->Publish(GRPMSG_PAUSE);
									}
									stack->Pause();
									return 1;
							}

							if (animation.Get(currentAnimation)->onEndEvent & AI::TRANSITION::ENDEVENT_REWIND)
							{
									stack->Rewind();
							}

							if (animation.Get(currentAnimation)->onEndEvent & AI::TRANSITION::ENDEVENT_DISABLE)				
									stack->SetActive(false);

							if (animation.Get(currentAnimation)->onEndEvent & AI::TRANSITION::ENDEVENT_FORWARD)				
									stack->Forward();

							if (animation.Get(currentAnimation)->onEndEvent & AI::TRANSITION::ENDEVENT_MSG)													
							{
									//	this->gui->NotifyServer(MULTIWILDPOTSTATUSVARTYPE_CURRENTSEQUENCESTATUS,MULTIWILDPOT_SEQUENCESTATUS_NOTIFY);
								ownerstate->GetController()->Publish(GRPMSG_CUSTOM);
							}

							//------------------
			
				

				currentLoop=0;
				if (currentAnimation+1>=animation.GetSize())
				{								
					animation.Get(currentAnimation)->finished=true;
					ownerstate->Publish(GRPMSG_EXIT);

					if (ownerstate->GetController()->verbose)
					{
					XDEBUG_PRINTCOLOR(0,__L("finished state : %d"),animation.Get(currentAnimation)->id);
					}
					currentAnimation=0;
					return 0;
				}

				currentAnimation++;

				SetMilestones();
				XDWORD id=animation.Get(currentAnimation)->id;
				stack=ownerstate->GetController()->GetOwner()->GetAnimationControl()->GetStack(id);

				ani = animation.Get(currentAnimation);
					if (stack)
					{
						if (animation.Get(currentAnimation)->Attachees.GetSize()==0)
							{
								if (ownerstate->GetController()->verbose)
								{
								XDEBUG_PRINTCOLOR(2,__L("\tSwitched stack to [%d] %s milestone: %d"), id, stack->GetName()->Get(), animation.Get(currentAnimation)->transition);
								}
							}
						else
							{
								if (ownerstate->GetController()->verbose)
								{
								XDEBUG_PRINTCOLOR(2,__L("\tSwitched stack to [%d] %s milestone: %d  (%d attached)"), id, stack->GetName()->Get(),animation.Get(currentAnimation)->transition, animation.Get(currentAnimation)->Attachees.GetSize());
								}
							}

							stack->SetActive	(true);					
							stack->Rewind			();
							stack->Play				();

							ani->Publish(GRPMSG_ENTER);
							//ownerstate->Publish(GRPMSG_ENTER);

							for (XDWORD e=0;e<animation.Get(currentAnimation)->Attachees.GetSize();e++)
							{
								XDWORD id=animation.Get(currentAnimation)->Attachees.Get(e)->id;
								stack=ownerstate->GetController()->GetOwner()->GetAnimationControl()->GetStack(id);
								if (stack)
								{
								stack->SetActive	(true);					
								stack->Rewind			();
								stack->Play				();								
								}
							}					
					return 1;
					}
				}
return true;
}

/*-------------------------------------------------------------------
//	AI::STATE::SetMilestones
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:21:58
//	
//  @param				current : 
*/
/*-----------------------------------------------------------------*/
void AI::TRACK::SetMilestones()
{
	if (this->animation.GetSize()==0) return;

		AI::ANIMATION*			ani					=this->animation.Get(this->currentAnimation);		
		AI::CONTROLLER*			controller	=this->ownerstate->GetController();		
		GRPANIMATIONSTACK*	stack				=controller->GetOwner()->GetAnimationControl()->GetStack((XDWORD)ani->id);

		if (stack!=NULL)
		{
				if (ani->transition!=-1)
				{
					if (stack->GetMilestone(ani->transition))
							stack->GetMilestone(ani->transition)->Enable(true);					
				}
		}

		for (XDWORD e=0;e<ani->Attachees.GetSize();e++)
		{
			int to=ani->Attachees.Get(e)->transition;
			GRPANIMATIONSTACK*			stack	=controller->GetOwner()->GetAnimationControl()->GetStack((XDWORD)ani->Attachees.Get(e)->id);
			
			if (stack)
			if (stack->GetMilestone(to))
					stack->GetMilestone(to)->Enable(true);		
		}
}


/*-------------------------------------------------------------------
//	AI::STATE::GetLastAnimation
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:22:03
//	
//	@return 			AI::ANIMATION* : 
//
*/
/*-----------------------------------------------------------------*/
AI::ANIMATION* AI::STATE::GetLastAnimation()
{
	return this->tracks.Get(0)->animation.Get(this->tracks.Get(0)->currentAnimation-1);
}

/*-------------------------------------------------------------------
//	AI::STATE::GetCurrentAnimation
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:22:21
//	
//	@return 			AI::ANIMATION* : 
//
*/
/*-----------------------------------------------------------------*/
AI::ANIMATION* AI::STATE::GetCurrentAnimation()
{
	return this->tracks.Get(0)->animation.Get(tracks.Get(0)->currentAnimation);
}

/*-------------------------------------------------------------------
//	AI::STATE::GetNextAnimation
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:22:29
//	
//	@return 			AI::ANIMATION* : 
//
*/
/*-----------------------------------------------------------------*/
AI::ANIMATION* AI::STATE::GetNextAnimation()
{
	return tracks.Get(0)->animation.Get(tracks.Get(0)->currentAnimation+1);
}



/*-------------------------------------------------------------------
//	AI::STATE::GetIndex
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:22:57
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD AI::STATE::GetIndex()
{
	return id;
}

/*-------------------------------------------------------------------
//	AI::STATE::StartAsCurrentState
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:23:21
//	
*/
/*-----------------------------------------------------------------*/
void AI::STATE::StartAsCurrentState()
{
	if (this->controller->verbose)
	{
		XDEBUG_PRINTCOLOR(0,__L("%s starting state: %s"),this->controller->GetName()->Get(),this->GetName()->Get());
	}

	for (XDWORD t=0;t<tracks.GetSize();t++)
	{
		AI::TRACK* track=this->tracks.Get(t);

			for (XDWORD e=0;e<track->animation.GetSize();e++)			
				track->animation.Get(e)->finished=false;
			

				track->currentAnimation=0;	
			if (track->animation.GetSize()==0)	continue;

			GRPANIMATIONSTACK* stack=this->controller->GetOwner()->GetAnimationControl()->GetStack(track->animation.Get(track->currentAnimation)->id);
			if (stack)
			{
				stack->SetActive(true);						
				stack->DisableMilestones();				
			}
	}

			this->Begin					();	
			this->Play					();	

	for (XDWORD t=0;t<tracks.GetSize();t++)
	{
		AI::TRACK* track=this->tracks.Get(t);

		track->SetMilestones	();
	}
	
	this->Publish				(GRPMSG_ENTER);
		
}

/*-------------------------------------------------------------------
//	AI::STATE::FinishCurrentState
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:23:15
//	
*/
/*-----------------------------------------------------------------*/
void AI::STATE::FinishCurrentState()
{	
	
	if (this->controller->verbose)
	{
		XDEBUG_PRINTCOLOR(0,__L("%s finishing state: %s"),this->controller->GetName()->Get(),this->GetName()->Get());
	}
	
		Publish(GRPMSG_EXIT);

		this->Stop();

	for (XDWORD t=0;t<tracks.GetSize();t++)
	{		
		AI::TRACK* track=tracks.Get(t);

		if (track->animation.GetSize() == 0) continue;

		XDWORD current	=track->animation.Get(track->currentAnimation)->id;

		if (track->animation.Get(track->currentAnimation)->onStartEvent != ENDEVENT_NONE)	
		{
			GRPANIMATIONSTACK* stack=this->controller->GetOwner()->GetAnimationControl()->GetStack(track->animation.Get(track->currentAnimation)->id);
			if (stack)
			{
			stack->Rewind();
			stack->SetActive(false);
			}							
		}
		track->currentAnimation=0;		
	}
		
}

/*-------------------------------------------------------------------
//	AI::STATE::Accept
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 11:23:35
//	
//	@return 			bool : 
//
//  @param				newstate : 
*/
/*-----------------------------------------------------------------*/
bool AI::STATE::Accept(AI::STATE* newstate)
{
	if (!newstate)
		return false;

	AI::TRACK* track=tracks.Get(0);

	AI::ANIMATION* animation=track->animation.GetLast();

	if (animation)
			switch (animation->acceptor)
			{
			case AI::TRANSITION::ACCEPT_NEVER:
					return false;

			case AI::TRANSITION::ACCEPT_ONLY_IF_FINISHED:
					if (animation->finished == true)
							return AI::TRANSITION::Accept(newstate->GetIndex());


					return false;

			case AI::TRANSITION::ACCEPT_ALWAYS:
					return AI::TRANSITION::Accept(newstate->GetIndex());
			}
	else
			return true;

	if (this->controller->verbose)
	{
		XDEBUG_PRINTCOLOR(4,__L("%s unnaceptable state: %s"),this->controller->GetName()->Get(),this->GetName()->Get());
	}
		return false;
}

/*-------------------------------------------------------------------
//	AI::STATE::Reject
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			01/06/2017 9:41:33
//	
//	@return 			bool : 
//
//  @param				newstate : 
*/
/*-----------------------------------------------------------------*/
bool AI::STATE::Reject(AI::STATE* newstate)
{
		if (!newstate)
		return false;

		return AI::TRANSITION::Reject(newstate->GetIndex());
}

/*-------------------------------------------------------------------
//	AI::STATE::GetStack
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/09/2017 13:08:27
//	
//	@return 			GRPANIMATIONSTACK* : 
//
*/
/*-----------------------------------------------------------------*/
GRPANIMATIONSTACK* AI::STATE::GetStack()
{
	AI::ANIMATION* ani= this->GetCurrentAnimation();
	return this->controller->GetOwner()->GetAnimationControl()->GetStack(ani->id);
}

/*-------------------------------------------------------------------
//	AI::STATE::GetSubjectType
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/09/2017 13:08:34
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD AI::STATE::GetSubjectType()
{		
	return AI_STATE;							
}

/*-------------------------------------------------------------------
//	AI::STATE::SetName
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/09/2017 13:08:40
//	
//  @param				name : 
*/
/*-----------------------------------------------------------------*/
void AI::STATE::SetName(XCHAR* name)									
{		
	this->name.Set(name);					
}

/*-------------------------------------------------------------------
//	AI::STATE::GetName
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/09/2017 13:08:46
//	
//	@return 			XSTRING* : 
//
*/
/*-----------------------------------------------------------------*/
XFSTRING* AI::STATE::GetName()
{		
	return &name;									
}

/*-------------------------------------------------------------------
//	AI::STATE::SetController
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/09/2017 13:08:52
//	
//  @param				controller : 
*/
/*-----------------------------------------------------------------*/
void AI::STATE::SetController(AI::CONTROLLER* controller)	
{		
	this->controller=controller;	
}

/*-------------------------------------------------------------------
//	AI::STATE::GetController
*/	
/**	
//	
//	
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			19/09/2017 13:08:58
//	
//	@return 			AI::CONTROLLER* : 
//
*/
/*-----------------------------------------------------------------*/
AI::CONTROLLER* AI::STATE::GetController()														
{		
	return this->controller;			
}


/*-------------------------------------------------------------------
//	 AI::STATE::Notify
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version	20/10/2017 14:24:37
//	@return		bool : 
//
//	@param		GRPMSGTYPE : 
//	@param		GRPSUBJECT* : 
//
*//*-----------------------------------------------------------------*/
bool AI::STATE::Notify(GRPMSGTYPE msg, GRPSUBJECT* subject)
{
		if (this->controller)
				return controller->Notify(msg, subject);
		return true;
}