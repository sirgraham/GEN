
/*------------------------------------------------------------------------------------------
//	AILOGIC.CPP
//	
//	Manages general logic
//   
//	Author						: Diego Martinez Ruiz de Gaona
//	Date Of Creation	: 02/12/2016 16:28:12
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "AILogic.h"
#include "INPDevice.h"
	
/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/

/*-------------------------------------------------------------------
//	LOGIC::LOGIC
*/	
/**	
//	
//	Class Constructor LOGIC
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 16:28:57
//	
*/
/*-----------------------------------------------------------------*/
AI::LOGIC::LOGIC() 
{

}

/*-------------------------------------------------------------------
//	LOGIC::LOGIC
*/	
/**	
//	
//	Class Destructor
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 16:28:57
//	
*/
/*-----------------------------------------------------------------*/
AI::LOGIC::~LOGIC() 
{
	controllers.DeleteContents();
	controllers.DeleteAll();
}

/*-------------------------------------------------------------------
//	AI::LOGIC::Add
*/	
/**	
//	
//	Add controller to controlller list
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 16:30:48
//	
//  @param				controller : 
*/
/*-----------------------------------------------------------------*/
void		AI::LOGIC::Add(AI::CONTROLLER* controller)
{	
	this->controllers.Add(controller);
}

/*-------------------------------------------------------------------
//	AI::LOGIC::Update
*/	
/**	
//	
//	Updates logic engine
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 16:30:59
//	
//	@return 			bool : 
//
*/
/*-----------------------------------------------------------------*/
bool		AI::LOGIC::Update(INPDEVICE* keyboard) 
{
	//--

	for (XDWORD e=0;e<controllers.GetSize();e++)		
		controllers.FastGet(e)->Update();
	
	//--

	if (keyboard)
	for (XDWORD c = 0; c < controllers.GetSize(); c++)
	{
			AI::CONTROLLER* controller = controllers.Get(c);
			for (XDWORD e = 0; e < keyboard->GetButtons()->GetSize(); e++)
			{
					INPBUTTON* button = keyboard->GetButtons()->Get(e);
					if (button->GetState()==INPBUTTON_STATE_RELEASED)					
							if (controller->Process(button->GetID()))
							{
									button->SetPressed(false);
									return true;
							}					
			}
	}
	return false;
}

/*-------------------------------------------------------------------
//	AI::LOGIC::GetSubjectType
*/	
/**	
//	
//	Subject virtual 
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 16:31:10
//	
//	@return 			XDWORD : 
//
*/
/*-----------------------------------------------------------------*/
XDWORD	AI::LOGIC::GetSubjectType() 
{
	return AI_LOGIC; 
}

/*-------------------------------------------------------------------
//	AI::LOGIC::Notify
*/	
/**	
//	
//	Observer virtual
//	
//	@author				Diego Martinez Ruiz de Gaona
//	@version			02/12/2016 16:31:24
//	
//	@return 			bool : 
//
//  @param				msg : 
//  @param				subject : 
*/
/*-----------------------------------------------------------------*/
bool		AI::LOGIC::Notify(GRPMSGTYPE msg, GRPSUBJECT* subject)
{
	return true;
}

/*-------------------------------------------------------------------
//	 AI::LOGIC::GetControllers
*/
/**
//
//	
//
//	@author		Diego Martinez Ruiz de Gaona 
//	@version		
//	@return		XVECTOR<AI::CONTROLLER*>* : 
//
//	@param		 : 
//
*//*-----------------------------------------------------------------*/
XVECTOR<AI::CONTROLLER*>* AI::LOGIC::GetControllers()
{ 
		return &controllers; 
}
