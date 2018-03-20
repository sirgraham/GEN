//------------------------------------------------------------------------------------------
//	IFAMENU.CPP
//	
//	Interface Functions Add (Menu Class)
//   
//	Author						: Abraham J. Velez
//	Date Of Creation	: 12/03/2004 16:35:10
//	Last Mofificacion	:	
//	
//	GEN  Copyright (C).  All right reserved.			 
//------------------------------------------------------------------------------------------
	
	
//---- INCLUDES ----------------------------------------------------------------------------

#include <stdlib.h>

#include "XPublisher.h"

#include "GRPRect.h"
#include "GRPCanvas.h"
#include "GRPScene.h"
#include "GRPScreen.h"

#include "INPButton.h"
#include "INPManager.h"

#include "IFAXEvent.h"
#include "IFASkinManager.h"
#include "IFAScroll.h"

#include "IFAMenu.h"

#include "XMemory.h"

	
//---- GENERAL VARIABLE --------------------------------------------------------------------

	
//---- CLASS MEMBERS -----------------------------------------------------------------------


//-------------------------------------------------------------------
//  IFAMENU::IFAMENU
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/03/2004 16:35:54
//	
//	@return				
//	@param				canvas : 
//  @param				scene :
//  @param				screen : 
//  @param				inpmanager : 
 
//	@param        skinmanager :
//	@param				poller :
//  @param				type : 
*/
//-------------------------------------------------------------------
IFAMENU::IFAMENU(GRPCANVAS* canvas, GRPSCENE* scene, GRPSCREEN* screen, INPMANAGER* inpmanager, XFACTORY* xfactory, IFASKINMANAGER* skinmanager, IFAOBJTYPE type) : IFAFORM(canvas, screen, inpmanager, xfactory, skinmanager, type)
{
	Clean();

	overspace	= 8;	

	this->scene			= scene;
	this->publisher = publisher;

	SetSubjectPublisher(publisher);	

	RegisterEventForPolling(IFAXEVENTTYPE_SELECT);
}



//-------------------------------------------------------------------
//  IFAMENU::~IFAMENU
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/03/2004 16:36:30
//	
//	@return				
//	*/
//-------------------------------------------------------------------
IFAMENU::~IFAMENU()
{
	DeRegisterEventForPolling(IFAXEVENTTYPE_SELECT);

	DeleteAllObj();

	Clean();
}


//-------------------------------------------------------------------
//  IFAMENU::Ini
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/04/2004 17:23:40
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAMENU::Ini()
{
	if(inpmanager) SetInputControl();

	return true;
}


//-------------------------------------------------------------------
//  IFAMENU::SetInputControl
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/04/2004 17:22:17
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAMENU::SetInputControl()
{
	INPDEVICE* inpdevice = inpmanager->GetDevice(INPDEVICE_TYPE_KEYBOARD);
	if(!inpdevice) return false;
	
	button[IFAMENU_BUTTON_UP]		 = inpdevice->GetButton(INPBUTTON_ID_UP);
	button[IFAMENU_BUTTON_DOWN]  = inpdevice->GetButton(INPBUTTON_ID_DOWN);
	button[IFAMENU_BUTTON_LEFT]  = inpdevice->GetButton(INPBUTTON_ID_LEFT);
	button[IFAMENU_BUTTON_RIGHT] = inpdevice->GetButton(INPBUTTON_ID_RIGHT);
	button[IFAMENU_BUTTON_B]		 = inpdevice->GetButton(INPBUTTON_ID_B);												
	button[IFAMENU_BUTTON_C]		 = inpdevice->GetButton(INPBUTTON_ID_C);
	
	return true;
}



//-------------------------------------------------------------------
//  IFAMENU::UpdateInputState
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			14/01/2004 15:44:11
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAMENU::UpdateInputState()
{	
	if(GetScroll()->IsMakeStep()) return true;

	for(int c=0;c<IFAMENU_MAXKEYS;c++)
		{	
			if(button[c])
				{
					switch(c)
						{
							case IFAMENU_BUTTON_UP    :
							case IFAMENU_BUTTON_DOWN  :
							case IFAMENU_BUTTON_LEFT  :
							case IFAMENU_BUTTON_RIGHT : if(button[c]->IsPressed())
																						{	
																							IFAOBJ* nowopt = GetOption(GetSelectOption());
																							IFAOBJ* newopt;

																							if(!nowopt) break;
																																														
																							if(parentpulldownmenu)
																								{																									
																									if(parentpulldownmenu->distributeoptions!=distributeoptions)	
																										{
																											bool move = false;

																											switch(parentpulldownmenu->distributeoptions)
																												{																													
																													case IFAMENU_DISTRIBUTEOPTIONS_NONE				: break;

																													case IFAMENU_DISTRIBUTEOPTIONS_VERTICAL		: switch(c)
																																																				{
																																																					case IFAMENU_BUTTON_UP    : 
  																																																				case IFAMENU_BUTTON_DOWN  : move = true;  break;
																																																				}
																																																			break;

																													case IFAMENU_DISTRIBUTEOPTIONS_HORIZONTAL	: switch(c)
																																																				{
																																																					case IFAMENU_BUTTON_LEFT  : 
																																																					case IFAMENU_BUTTON_RIGHT :	move = true;	break;
																																																				}
  																																																		break;
																						  					}			

																											if(move)	
																												{									
																													SetActive(false);
																													SetVisible(false);
																													parentpulldownmenu->SetActive(true);
																													
																													parentpulldownmenu->UpdateInputState();
																													
																													parentpulldownmenu->SetActive(false);																													

																													if(inpmanager) inpmanager->GetDevice(INPDEVICE_TYPE_KEYBOARD)->Release();																													

																													IFAOBJ* sobj = parentpulldownmenu->GetOption(parentpulldownmenu->GetSelectOption());
																													if(sobj)
																														{
																															IFAMENU* pulldownmenu = parentpulldownmenu->GetOptionPullDownMenu(sobj);
																															if(pulldownmenu) ShowPullDownMenu(sobj,pulldownmenu);																																																																		
																														}
																													
																													return true;

																												} 
																										}
																								}
																								
																							switch(distributeoptions)
																								{
																									case IFAMENU_DISTRIBUTEOPTIONS_NONE				: break;

																									case IFAMENU_DISTRIBUTEOPTIONS_VERTICAL		: switch(c)
																																																{
																																																	case IFAMENU_BUTTON_UP    : SelectOption(GetSelectOption()-1,false);																																																																
																																																															newopt = GetOption(GetSelectOption());
																																																															if(!newopt) break;
																																																															if(features&IFAFEATURE_SELECTBAR)
																																																																{
																																																																	if(rectdirect.IsInto(newopt->GetRectDirect()))  break;																																																																
																																																																}
																																																															GetScroll()->SetStep(0,abs(newopt->GetRect()->GetY()-nowopt->GetRect()->GetY()),true,0,-8);																																																															
																																																															break;

  																																																case IFAMENU_BUTTON_DOWN  : SelectOption(GetSelectOption()+1,true);
																																																															
																																																															newopt = GetOption(GetSelectOption());
																																																															if(!newopt) break;
																																																															if(features&IFAFEATURE_SELECTBAR)
																																																																{																																																																
																																																																	if(rectdirect.IsInto(newopt->GetRectDirect())) break;																																																																
																																																																}
																																																															GetScroll()->SetStep(0,abs(nowopt->GetRect()->GetY()-newopt->GetRect()->GetY()),true,0, 8);
																																																															
																																																															break;
																																																}
																																															break;

																									case IFAMENU_DISTRIBUTEOPTIONS_HORIZONTAL	: switch(c)
																																																{
																																																	case IFAMENU_BUTTON_LEFT  : SelectOption(GetSelectOption()-1,false);
																																																															
																																																															newopt = GetOption(GetSelectOption());
																																																															if(!newopt) break;
																																																															if(features&IFAFEATURE_SELECTBAR)
																																																																{																																																																
																																																																	if(rectdirect.IsInto(newopt->GetRectDirect()))  break;
																																																																}
																																																															GetScroll()->SetStep((newopt->GetRectFull()->GetX()-nowopt->GetRectFull()->GetX()),0,true,-8,0);
																																																															
																																																															break;

																																																	case IFAMENU_BUTTON_RIGHT :	SelectOption(GetSelectOption()+1,true);
																																																															
																																																															newopt = GetOption(GetSelectOption());
																																																															if(!newopt) break;
																																																															if(features&IFAFEATURE_SELECTBAR)
																																																																{																																																																
																																																																	if(rectdirect.IsInto(newopt->GetRectDirect()))  break;
																																																																}
																																																															GetScroll()->SetStep((nowopt->GetRectFull()->GetX()-newopt->GetRectFull()->GetX()),0,true, 8,0);
																																																															
																																																															break;
																																																}

  																																														break;
																						  	 }

																							button[c]->SetPressed(false);

																							return true;

																						 }
                                                
  																					break;

								case IFAMENU_BUTTON_B    : 	if(button[c]->IsPressedWithRelease())
																							{
																								if(features&IFAFEATURE_CANCELACTIVE)
																									{	
																										inpmanager->GetDevice(INPDEVICE_TYPE_KEYBOARD)->Release();

																										IFAXEVENTSELECT xevent(IFAMENU_CANCEL,NULL,this,this);
																										PostEvent(&xevent, true);

																										if(parentpulldownmenu)
																											{
																												SetActive(false);
																												SetVisible(false);

																												parentpulldownmenu->SetActive(true);																														
																											}
																									}
																							}
																					  break;								

								case IFAMENU_BUTTON_C    : 	if(button[c]->IsPressedWithRelease())
																							{	
																								inpmanager->GetDevice(INPDEVICE_TYPE_KEYBOARD)->Release();


																								IFAOBJ* selectobj = GetOption(GetSelectOption());

																								if(selectobj)
																									{																										
																										if(selectobj->IsActive())
																											{
																												IFAMENU* pulldownmenu = GetOptionPullDownMenu(selectobj);

																												if(pulldownmenu)
																													{
																														SetActive(false);
																														ShowPullDownMenu(selectobj,pulldownmenu);																														
																													}
																												 else
																													{																															
																														IFAXEVENTSELECT xevent(selectoption,selectobj,this,this);
																														PostEvent(&xevent, true);

																														/*

																														if(parentpulldownmenu)
																															{
																																SetActive(false);
																																SetVisible(false);

																																parentpulldownmenu->SetActive(true);																														
																															}
																														 else 
																															{
																														*/
																																if((features&IFAFEATURE_CANCELTOSELECT)||(selectobj->GetFeatures()&IFAFEATURE_CANCELTOSELECT))
																																	{
																																		IFAXEVENTSELECT xevent(IFAMENU_CANCEL,NULL,this,this);
																																		PostEvent(&xevent, true);
																																	}																																	
																														//	}
																													}
																											}
																									}
																							}
																					  break;								
						}
				}
		}

	return true;
}


//-------------------------------------------------------------------
//  IFAMENU::UpdateObj
/**
//
//	
//	@author				Abraham J. Velez
//	@version			30/03/2004 16:28:43
//	
//	@return				bool : 
//	@param				nobj : 
//  @param				obj : 
*/
//-------------------------------------------------------------------
bool IFAMENU::UpdateObj(int nobj,IFAOBJ* obj)
{
	if(!obj) return false;

	if(selectoption==nobj)
		{		
			if(features&IFAFEATURE_SELECTBAR)
				{
					GRPCOLOR* fill = obj->GetColor(GRPCOLORTYPE_BACKGROUND);
					GRPCOLOR* edge = obj->GetColor(GRPCOLORTYPE_EDGE);
					
					GRPRECT trect;
					
					trect.Set(obj->GetRect());
										
					if(features&IFAFEATURE_SELECTBARSIZEOVER)
						{
							trect.SetOriginX(trect.GetX()-IFAMENU_OPTIONOVERSIZE);					
							trect.SetWidth(trect.GetWidth()+(IFAMENU_OPTIONOVERSIZE*2));					
						}

					if(features&IFAFEATURE_SELECTBARSIZEFULL)
						{
						  trect.Set(rectdirect.GetX(),obj->GetRectDirect()->GetY(),rectdirect.GetWidth(),obj->GetRectDirect()->GetHeight());
						}
										
					canvas->DrawBox(&trect,fill->Get(),fill->GetAlpha(),0,edge->Get(),edge->GetAlpha());
				}
		}

	obj->Update();

	return true;
}


//-------------------------------------------------------------------
//  IFAMENU::Update
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			30/12/2003 16:36:00
//	
//	@return				bool : 
//	@param				enviroment : 
*/
//-------------------------------------------------------------------
bool IFAMENU::Update()
{		
	#ifdef XDEBUG
	//XBYTE RGB[4]= { 0x00,0xFF,0x00 };
	//canvas->DrawRect(&rect,RGB);
	#endif

	if(active)  UpdateInputState();	
	if(visible)	IFAFORM::Update();
		
	return true;
}



//-------------------------------------------------------------------
//  IFAMENU::End
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			02/04/2004 17:26:03
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAMENU::End()
{
	return true;
}





//-------------------------------------------------------------------
//  IFAMENU::GetOption
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/03/2004 18:25:12
//	
//	@return				IFAOBJ* : 
//	@param				option : 
*/
//-------------------------------------------------------------------
IFAOBJ* IFAMENU::GetOption(int option)
{
	if(objs.IsEmpty()) return NULL;

	if(option >(int)(objs.GetSize()-1)) return NULL;

	IFAOBJ* obj = (IFAOBJ*)objs.Get(option);

	return obj;
}



//-------------------------------------------------------------------
//  IFAMENU::GetNumberOptions
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			05/04/2004 17:14:53
//	
//	@return				int : 
//	*/
//-------------------------------------------------------------------
int IFAMENU::GetNumberOptions()
{
	return objs.GetSize();
}


//-------------------------------------------------------------------
//  IFAMENU::GetSelectOption
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			17/03/2004 10:29:10
//	
//	@return				int : 
//	*/
//-------------------------------------------------------------------
int IFAMENU::GetSelectOption()
{
	return selectoption;
}


//-------------------------------------------------------------------
//  IFAMENU::SelectOption
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/08/2004 15:16:54
//	
//	@return				bool : 
//	@param				option : 
//  @param				goahead : 
*/
//-------------------------------------------------------------------
bool IFAMENU::SelectOption(int option,bool goahead)
{
	if(objs.IsEmpty()) return false;
  
	selectoption = option;	

	CheckLimitsOptions(option);

	IFAOBJ* obj = (IFAOBJ*)objs.Get(selectoption);

	do{	if(!obj->IsActive()) 
				{
					if(goahead) selectoption++; else selectoption--;
				}
		
			CheckLimitsOptions(option);
			
		} while(!obj->IsActive()); 

	return true;
}


//-------------------------------------------------------------------
//  IFAMENU::GetSelectObj
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			07/05/2004 10:37:37
//	
//	@return				IFAOBJ* : 
//	*/
//-------------------------------------------------------------------
IFAOBJ* IFAMENU::GetSelectObj()
{
	return GetOption(GetSelectOption());
}


//-------------------------------------------------------------------
//  IFAMENU::AjustScrollSelectOption
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			08/10/2004 12:31:38
//	
//	@return				bool : 
//	*/
//-------------------------------------------------------------------
bool IFAMENU::AjustScrollSelectOption()
{
	IFAOBJ* selectobj = GetOption(GetSelectOption());
	if(!selectobj) return false;

	if(!(features&IFAFEATURE_SELECTBAR)) return false;

	if((!rectdirect.GetWidth())||(!rectdirect.GetHeight())) return false;

	GetScroll()->SetOrigin();

	switch(distributeoptions)
		{
			case IFAMENU_DISTRIBUTEOPTIONS_NONE				: break;

			case IFAMENU_DISTRIBUTEOPTIONS_VERTICAL		: while(!rectdirect.IsInto(selectobj->GetRectDirect()))
																										{
																											GetScroll()->SetStep(0,abs(selectobj->GetRect()->GetY()-selectobj->GetRect()->GetY()),true,0,-8);		
																										}
																									break;

			case IFAMENU_DISTRIBUTEOPTIONS_HORIZONTAL	: while(!rectdirect.IsInto(selectobj->GetRectDirect()))
																										{																									
																											GetScroll()->SetStep((selectobj->GetRectFull()->GetX()-selectobj->GetRectFull()->GetX()),0,true,-8,0);
																										}
																									break;
		}

	return true;
}



//-------------------------------------------------------------------
//  IFAMENU::DistributeOptions
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			29/03/2004 11:46:23
//	
//	@return				bool : 
//	@param				mode : 
//  @param				overspace : 
*/
//-------------------------------------------------------------------
bool IFAMENU::DistributeOptions(IFAMENU_DISTRIBUTEOPTIONS mode,int overspace)
{
	if(objs.IsEmpty()) return false;

	distributeoptions = mode;

	int max	= (overspace/2);

	optionoverspace = overspace;

	switch(mode)
		{
			case IFAMENU_DISTRIBUTEOPTIONS_NONE				: break;

			case IFAMENU_DISTRIBUTEOPTIONS_HORIZONTAL	: {	for(XDWORD c=0;c<objs.GetSize();c++)
																											{
																												IFAOBJ* tobj = (IFAOBJ*)objs.Get(c);

																												if(tobj)
																													{
																														tobj->GetRect()->SetOriginX(max);
																														max   += (tobj->GetRect()->GetWidth() + overspace);					
																													}
																											}
																									}
																									break;

			case IFAMENU_DISTRIBUTEOPTIONS_VERTICAL		: {	int c=(int)(objs.GetSize()-1);
	
																										for(;c>=0;c--)
																											{
																												IFAOBJ* tobj = (IFAOBJ*)objs.Get(c);

																												if(tobj)
																													{
																														tobj->GetRect()->SetOriginY(max);					
																														max	+=  (tobj->GetRect()->GetHeight() + overspace);
																													}
																											}
																									}
																									break;
		}

	return true;
}



//-------------------------------------------------------------------
//  IFAMENU::GetParentPullDownMenu
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/09/2004 20:58:53
//	
//	@return				IFAMENU* : 
//	*/
//-------------------------------------------------------------------
IFAMENU* IFAMENU::GetParentPullDownMenu()
{
	return parentpulldownmenu;
}


//-------------------------------------------------------------------
//  IFAMENU::SetParentPullDownMenu
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			28/09/2004 21:21:31
//	
//	@return				bool : 
//	@param				menu : 
*/
//-------------------------------------------------------------------
bool IFAMENU::SetParentPullDownMenu(IFAMENU* menu)
{
	parentpulldownmenu = menu;

	return true;
}



//-------------------------------------------------------------------
//  IFAMENU::AddOptionPullDownMenu
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/09/2004 20:38:00
//	
//	@return				bool : 
//	@param				option : 
//  @param				menu : 
*/
//-------------------------------------------------------------------
bool IFAMENU::AddOptionPullDownMenu(IFAOBJ* option,IFAMENU* menu,IFAMENU* parent)
{
	if(!option) return false;
	if(!menu)		return false;

	menu->parentpulldownmenu = parent;

	optionpulldown.Add(option,menu);										

	return true;
}



//-------------------------------------------------------------------
//  IFAMENU::GetOptionPullDownMenu
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/09/2004 20:48:52
//	
//	@return				IFAMENU* : 
//	@param				option : 
*/
//-------------------------------------------------------------------
IFAMENU* IFAMENU::GetOptionPullDownMenu(IFAOBJ* option)
{
	if(!option) return NULL;

	int index = optionpulldown.Find(option);
	if(index == -1) return NULL;

	IFAMENU* menu = (IFAMENU*)optionpulldown.GetElement(index);

	return menu;
}



//-------------------------------------------------------------------
//  IFAMENU::DelOptionPullDownMenu
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			25/09/2004 20:38:08
//	
//	@return				bool : 
//	@param				option : 
*/
//-------------------------------------------------------------------
bool IFAMENU::DelOptionPullDownMenu(IFAOBJ* option)
{
	if(!option) return false;

	int index = optionpulldown.Find(option);
	if(index == -1) return NULL;

	IFAMENU* menu = optionpulldown.GetElement(index);
	
	if(menu) optionpulldown.Delete(menu);	
	
	return true;	
}




//-------------------------------------------------------------------
//  IFAMENU::CheckLimitsOptions
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			01/08/2004 15:15:20
//	
//	@return				bool : 
//	@param				option : 
*/
//-------------------------------------------------------------------
bool IFAMENU::CheckLimitsOptions(int option)
{
 	int nmaxoptions = (int)(objs.GetSize()-1);

  if(features&IFAFEATURE_CICLYC)
	  {
	  	 if(selectoption>nmaxoptions)  selectoption = 0;
		   if(selectoption<0)						 selectoption = nmaxoptions;
 	  }
	 else		
	  {
	     if(option>nmaxoptions)				 selectoption = nmaxoptions;
	     if(option<0)									 selectoption = 0;
	  }			

 return true;

}



//-------------------------------------------------------------------
//  IFAMENU::ShowPullDownMenu
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			16/12/2004 12:02:56
//	
//	@return				bool : 
//	@param				sobj : 
//  @param				pulldownmenu : 
*/
//-------------------------------------------------------------------
bool IFAMENU::ShowPullDownMenu(IFAOBJ* sobj,IFAMENU* pulldownmenu)
{
	if(!sobj)					return false;
	if(!pulldownmenu) return false;

	int xpos = (sobj->GetRectDirect()->GetX() - IFAMENU_OPTIONOVERSIZE);
	int ypos = (sobj->GetRectDirect()->GetY() - pulldownmenu->GetRect()->GetHeight() - 4);	

	if((xpos+pulldownmenu->GetRect()->GetWidth())>(canvas->GetScreenZone()->GetX() + screen->GetWidth())) xpos -= (pulldownmenu->GetRect()->GetWidth() - sobj->GetRect()->GetWidth() - (IFAMENU_OPTIONOVERSIZE*2));
		
	pulldownmenu->SetOrigin(xpos-canvas->GetScreenZone()->GetX() , ypos-canvas->GetScreenZone()->GetY());																														

	pulldownmenu->SetActive(true);
	pulldownmenu->SetVisible(true);

	if(pulldownmenu->scenehdl) scene->GotoToFinalQueue(pulldownmenu->scenehdl);	

	return true;
}



//-------------------------------------------------------------------
//  IFAMENU::Clean
/**
//	
//	
//	@author				Abraham J. Velez
//	@version			12/03/2004 16:38:45
//	
//	@return				void : 
//	*/
//-------------------------------------------------------------------
void IFAMENU::Clean()
{	
	int c;

	scene								= NULL;
	publisher						= NULL;
	
	for(c=0;c<IFAMENU_MAXKEYS;c++)
		{
			button[c] = NULL;
		}

	distributeoptions		= IFAMENU_DISTRIBUTEOPTIONS_NONE;
	optionoverspace			= 0;
	selectoption				= 0;

	optionpulldown.DeleteAll();

	parentpulldownmenu	= NULL;
}
